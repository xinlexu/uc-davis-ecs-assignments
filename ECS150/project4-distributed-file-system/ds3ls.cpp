#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
#include <map>

#include "LocalFileSystem.h"
#include "Disk.h"
#include "ufs.h"

using namespace std;

void dfs_ls(LocalFileSystem *lfs, const inode_t *inodes, const size_t parent_id, const string parent_prefix);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout << argv[0] << ": diskImageFile" << endl;
    return 1;
  }

  Disk *disk = new Disk(argv[1], UFS_BLOCK_SIZE);
  LocalFileSystem *lfs = new LocalFileSystem(disk);

  super_t super;
  lfs->readSuperBlock(&super);

  // 读取 inode_region
  inode_t *inodes = new inode_t [super.num_inodes]();
  lfs->readInodeRegion(&super, inodes);

  dfs_ls(lfs, inodes, 0, "/");

  delete [] inodes;
  delete lfs;
  delete disk;
  return 0;
}

/**
 * ds3ls command with deep first search style
*/
void dfs_ls(LocalFileSystem *lfs, const inode_t *inodes, const size_t parent_id, const string parent_prefix)
{
  cout << "Directory " << parent_prefix << endl;

  map<string, int> dir_entries_map;

  // 计算目录中的entry数目
  int num_of_entries = inodes[parent_id].size / sizeof(dir_ent_t);

  // 读取entries
  unsigned char *dir_entries = new unsigned char [inodes[parent_id].size]();
  lfs->read(parent_id, (void*)dir_entries, inodes[parent_id].size);
  for (int j = 0; j < num_of_entries; j++)
  {
    dir_entries_map.insert(pair<string, int>(((dir_ent_t*)dir_entries)[j].name, ((dir_ent_t*)dir_entries)[j].inum));
  }
  delete [] dir_entries;

  // 输出排序后的结果
  for (map<string, int>::iterator it = dir_entries_map.begin(); it != dir_entries_map.end(); it++)
  {
    cout << it->second << "\t" << it->first << endl;
  }
  cout << endl;

  // 删掉`.`和`..`两个entry
  dir_entries_map.erase(".");
  dir_entries_map.erase("..");

  // 递归
  for (map<string, int>::iterator it = dir_entries_map.begin(); it != dir_entries_map.end(); it++)
  {
    inode_t temp_inode;
    lfs->stat(it->second, &temp_inode);
    if (temp_inode.type == UFS_DIRECTORY)
    {
      dfs_ls(lfs, inodes, it->second, parent_prefix + string(it->first) + "/");
    }
  }
}
