#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sstream>
#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include <filesystem> // 使用filesystem::path库处理路径字符串
#include <cassert>

#include "DistributedFileSystemService.h"
#include "ClientError.h"
#include "ufs.h"
#include "WwwFormEncodedDict.h"

using namespace std;

DistributedFileSystemService::DistributedFileSystemService(string diskFile) : HttpService("/ds3/") {
  this->fileSystem = new LocalFileSystem(new Disk(diskFile, UFS_BLOCK_SIZE));
}

// 自定义辅助函数，将字符串文件目录转换为链表
deque<pair<int, string>> get_rlookup_linked_list(LocalFileSystem *lfs, string path)
{
  filesystem::path p = path;
  deque<pair<int, string>> rlookup;
  int inode_number = 0;
  rlookup.push_back(pair<int, string>(0, "/"));
  // 注意：rlookup的第0个元素为`/`，第1个元素为`ds3`，这里从第2个开始处理
  for (filesystem::path::iterator it = ++(++p.begin()); it != p.end(); it++)
  {
    // 以`/`结尾的路径也属于UFS_DIRECTORY
    if (*it == "")
    {
      break;
    }

    inode_number = lfs->lookup(inode_number, *it);
    if (inode_number == -ENOTFOUND)
    {
      rlookup.push_back(pair<int, string>(-ENOTFOUND, *it));
    }
    else if (inode_number == -EINVALIDINODE)
    {
      rlookup.push_back(pair<int, string>(-EINVALIDINODE, *it));
    }
    else
    {
      rlookup.push_back(pair<int, string>(inode_number, *it));
    }
  }

  return rlookup;
}


void DistributedFileSystemService::get(HTTPRequest *request, HTTPResponse *response) {

  // 将字符串文件目录转换为链表
  deque<pair<int, string>> rlookup = get_rlookup_linked_list(fileSystem, request->getPath());

  // 确认文件（及其递归父目录）存在
  for (deque<pair<int, string>>::iterator it = rlookup.begin(); it != rlookup.end(); it++)
  {
    if (it->first == -ENOTFOUND)
    {
      throw ClientError::notFound();
      return;
    }
    else if (it->first == -EINVALIDINODE)
    {
      throw ClientError::badRequest();
      return;
    }
  }

  // 获取inode
  int inode_number = rlookup[rlookup.size() - 1].first;
  inode_t inode;
  assert(fileSystem->stat(inode_number, &inode) >= 0);

  // 如果inode类型为文件，则输出文件内容
  if (inode.type == UFS_REGULAR_FILE)
  {
    string result = "";
    unsigned char *buffer = new unsigned char [inode.size]();
    assert(fileSystem->read(inode_number, buffer, inode.size) == inode.size);
    result.append((char*)buffer);
    delete [] buffer;

    response->setStatus(200);
    response->setBody(result);
    return;
  }
  // 如果inode类型为目录
  else if (inode.type == UFS_DIRECTORY)
  {
    map<string, int> dir_entries_map;

    // 计算目录中的entry数目
    int num_of_entries = inode.size / sizeof(dir_ent_t);

    // 读取entries
    unsigned char *dir_entries = new unsigned char [inode.size]();
    fileSystem->read(inode_number, (void*)dir_entries, inode.size);
    for (int j = 0; j < num_of_entries; j++)
    {
      dir_entries_map.insert(pair<string, int>(((dir_ent_t*)dir_entries)[j].name, ((dir_ent_t*)dir_entries)[j].inum));
    }
    delete [] dir_entries;

    // 删掉`.`和`..`两个entry
    dir_entries_map.erase(".");
    dir_entries_map.erase("..");

    // 输出排序后的结果（map是默认有序的）
    stringstream result;
    for (map<string, int>::iterator it = dir_entries_map.begin(); it != dir_entries_map.end(); it++)
    {
      result << it->first;

      // 如果是文件夹，则添加一个`/`后缀
      inode_t temp_inode;
      fileSystem->stat(it->second, &temp_inode);
      if (temp_inode.type == UFS_DIRECTORY)
      {
        result << "/";
      }

      result << endl;
    }

    response->setStatus(200);
    response->setBody(result.str());
    return;
  }
}

void DistributedFileSystemService::put(HTTPRequest *request, HTTPResponse *response) {

  // 将字符串文件目录转换为链表
  deque<pair<int, string>> rlookup = get_rlookup_linked_list(fileSystem, request->getPath());

  // 不允许把根目录作为文件写入
  if (rlookup.size() == 1)
  {
    throw ClientError::badRequest();
    return;
  }

  // 将文件的递归父目录的inode补全
  bool incomplete_path = false;
  do
  {
    incomplete_path = false;
    rlookup = get_rlookup_linked_list(fileSystem, request->getPath());
    for (unsigned int i = 1; i < rlookup.size() - 1; i++)
    {
      if (rlookup[i].first == -ENOTFOUND)
      {
        incomplete_path = true;
        string create_dirname = rlookup[i].second;
        int parent_inode_number = rlookup[i-1].first;
        int create_result = fileSystem->create(parent_inode_number, UFS_DIRECTORY, create_dirname);
        if (create_result == -ENOTENOUGHSPACE)
        {
          throw ClientError::insufficientStorage();
          return;
        }
        else if (create_result < 0)
        {
          throw ClientError::badRequest();
          return;
        }
        break;
      }
      else if (rlookup[i].first < 0)
      {
        throw ClientError::badRequest();
        return;
      }

      // 需要确认entry的类型是目录而不是文件，否则抛出ClientError::conflict()
      inode_t temp_inode;
      fileSystem->stat(rlookup[i].first, &temp_inode);
      if (temp_inode.type != UFS_DIRECTORY)
      {
        throw ClientError::conflict();
        return;
      }
    }
  }while (incomplete_path);

  // 如果文件不存在则创建
  if (rlookup[rlookup.size() - 1].first == -ENOTFOUND)
  {
    int create_result = fileSystem->create(rlookup[rlookup.size() - 2].first, UFS_REGULAR_FILE, rlookup[rlookup.size() - 1].second);
    if (create_result == -ENOTENOUGHSPACE)
    {
      throw ClientError::insufficientStorage();
      return;
    }
    else if (create_result < 0)
    {
      throw ClientError::badRequest();
      return;
    }

    rlookup = get_rlookup_linked_list(fileSystem, request->getPath());
  }

  // 获取inode
  int inode_number = rlookup[rlookup.size() - 1].first;
  inode_t inode;
  assert(fileSystem->stat(inode_number, &inode) >= 0);

  // 如果inode类型为文件，则覆盖写入
  if (inode.type == UFS_REGULAR_FILE)
  {
    int write_result = fileSystem->write(inode_number, request->getBody().c_str(), request->getBody().length());
    if (write_result == -ENOTENOUGHSPACE)
    {
      throw ClientError::insufficientStorage();
      return;
    }
    else if (write_result < 0)
    {
      throw ClientError::badRequest();
      return;
    }

    response->setStatus(200);
    response->setBody("");
    return;
  }
  // 如果inode类型为目录，不允许写入
  else if (inode.type == UFS_DIRECTORY)
  {
    throw ClientError::badRequest();
    return;
  }
}

void DistributedFileSystemService::del(HTTPRequest *request, HTTPResponse *response) {

  // 将字符串文件目录转换为链表
  deque<pair<int, string>> rlookup = get_rlookup_linked_list(fileSystem, request->getPath());

  // 确认文件（及其递归父目录）存在
  for (deque<pair<int, string>>::iterator it = rlookup.begin(); it != rlookup.end(); it++)
  {
    if (it->first == -ENOTFOUND)
    {
      throw ClientError::notFound();
      return;
    }
    else if (it->first == -EINVALIDINODE)
    {
      throw ClientError::badRequest();
      return;
    }
    else if (it->first < 0)
    {
      throw ClientError::badRequest();
      return;
    }
  }

  // 不允许删除根目录
  if (rlookup.size() <= 1)
  {
    throw ClientError::badRequest();
    return;
  }

  // 直接做删除操作，并返回结果
  string unlink_filename = rlookup[rlookup.size() - 1].second;
  int unlink_parent_inode_number = rlookup[rlookup.size() - 2].first;

  int unlink_result = fileSystem->unlink(unlink_parent_inode_number, unlink_filename);

  if (unlink_result == -ENOTENOUGHSPACE)
  {
    throw ClientError::insufficientStorage();
    return;
  }
  else if (unlink_result < 0)
  {
    throw ClientError::badRequest();
    return;
  }
  else if (unlink_result == 0)
  {
    response->setStatus(200);
    response->setBody("");
    return;
  }
}
