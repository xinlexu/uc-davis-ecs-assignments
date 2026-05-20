#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <string.h> // memcpy() and strncmp() function
#include <map>
#include "LocalFileSystem.h"
#include "ufs.h"

using namespace std;


LocalFileSystem::LocalFileSystem(Disk *disk) {
  this->disk = disk;
}

void LocalFileSystem::readSuperBlock(super_t *super) {
  assert(super != nullptr);
  unsigned char *temp_block = new unsigned char [UFS_BLOCK_SIZE]();
  disk->readBlock(0, temp_block);
  memcpy(super, temp_block, sizeof(super_t));
  delete [] temp_block;
}

void LocalFileSystem::readInodeBitmap(super_t *super, unsigned char *inodeBitmap)
{
  assert(super != nullptr && inodeBitmap != nullptr);

  int read_bytes = 0;
  int remain_bytes = super->num_inodes / 8; // 整数个bytes数
  int remain_bits = super->num_inodes - (remain_bytes * 8); // 不足一个byte的bits数

  assert(remain_bits >= 0 && remain_bits < 8);

  // 读取整块部分
  while (remain_bytes >= UFS_BLOCK_SIZE)
  {
    disk->readBlock(super->inode_bitmap_addr + read_bytes / UFS_BLOCK_SIZE, inodeBitmap + read_bytes);
    read_bytes += UFS_BLOCK_SIZE;
    remain_bytes -= UFS_BLOCK_SIZE;
  }

  assert(read_bytes % UFS_BLOCK_SIZE == 0);
  assert(remain_bytes >= 0 && remain_bytes < UFS_BLOCK_SIZE);

  // 读取剩余的不足一块的部分（如有）
  if (remain_bytes > 0)
  {
    unsigned char *temp_block = new unsigned char [UFS_BLOCK_SIZE]();
    disk->readBlock(super->inode_bitmap_addr + read_bytes / UFS_BLOCK_SIZE, temp_block);
    memcpy(inodeBitmap + read_bytes, temp_block, remain_bytes);
    read_bytes += remain_bytes;

    // 剩余不足1字节部分
    if (remain_bits > 0)
    {
      unsigned char last_byte = temp_block[remain_bytes];
      unsigned char mask = 0xff;
      last_byte &= mask >> (8 - remain_bits);
      inodeBitmap[remain_bytes] = last_byte;
    }

    delete [] temp_block;
  }
}

void LocalFileSystem::readDataBitmap(super_t *super, unsigned char *dataBitmap)
{
  assert(super != nullptr && dataBitmap != nullptr);

  int read_bytes = 0;
  int remain_bytes = super->num_data / 8; // 整数个bytes数
  int remain_bits = super->num_data - (remain_bytes * 8); // 不足一个byte的bits数

  assert(remain_bits >= 0 && remain_bits < 8);

  // 读取整块部分
  while (remain_bytes >= UFS_BLOCK_SIZE)
  {
    disk->readBlock(super->data_bitmap_addr + read_bytes / UFS_BLOCK_SIZE, dataBitmap + read_bytes);
    read_bytes += UFS_BLOCK_SIZE;
    remain_bytes -= UFS_BLOCK_SIZE;
  }

  assert(read_bytes % UFS_BLOCK_SIZE == 0);
  assert(remain_bytes >= 0 && remain_bytes < UFS_BLOCK_SIZE);

  // 读取剩余的不足一块的部分（如有）
  if (remain_bytes > 0)
  {
    unsigned char *temp_block = new unsigned char [UFS_BLOCK_SIZE]();
    disk->readBlock(super->data_bitmap_addr + read_bytes / UFS_BLOCK_SIZE, temp_block);
    memcpy(dataBitmap + read_bytes, temp_block, remain_bytes);
    read_bytes += remain_bytes;

    // 剩余不足1字节部分
    if (remain_bits > 0)
    {
      unsigned char last_byte = temp_block[remain_bytes];
      unsigned char mask = 0xff;
      last_byte &= mask >> (8 - remain_bits);
      dataBitmap[remain_bytes] = last_byte;
    }

    delete [] temp_block;
  }
}

void LocalFileSystem::readInodeRegion(super_t *super, inode_t *inodes)
{
  assert(super != nullptr && inodes != nullptr);

  int read_inodes = 0;
  int remain_inodes = super->num_inodes;

  // 读取整块部分
  while (remain_inodes >= (int)(UFS_BLOCK_SIZE / sizeof(inode_t)))
  {
    disk->readBlock(super->inode_region_addr + read_inodes * sizeof(inode_t) / UFS_BLOCK_SIZE, inodes + read_inodes);
    read_inodes += UFS_BLOCK_SIZE / sizeof(inode_t);
    remain_inodes -= UFS_BLOCK_SIZE / sizeof(inode_t);
  }

  assert(read_inodes % (UFS_BLOCK_SIZE / sizeof(inode_t)) == 0);
  assert(remain_inodes >= 0 && remain_inodes < (int)(UFS_BLOCK_SIZE / sizeof(inode_t)));

  // 读取剩余的不足一块的部分
  if (remain_inodes > 0)
  {
    unsigned char *temp_block = new unsigned char [UFS_BLOCK_SIZE]();
    disk->readBlock(super->inode_region_addr + read_inodes * sizeof(inode_t) / UFS_BLOCK_SIZE, temp_block);
    memcpy(inodes + read_inodes, temp_block, remain_inodes * sizeof(inode_t));
    read_inodes += remain_inodes;
    delete [] temp_block;
  }
}

// 依赖readSuperBlock(), readInodeBitmap()
int LocalFileSystem::stat(int inodeNumber, inode_t *inode) {

  assert(inode != nullptr);

  // 读取超级块
  super_t super;
  readSuperBlock(&super);

  // 检查inode索引的数组边界
  if (inodeNumber < 0 || inodeNumber >= super.num_inodes)
  {
    return -EINVALIDINODE;
  }

  // （可选）判断inode是否未分配，若未分配则返回-EINVALIDINODE
  unsigned char *inodeBitmap = new unsigned char [super.num_inodes / 8 + 1]();
  readInodeBitmap(&super, inodeBitmap);
  if (((inodeBitmap[inodeNumber / 8] >> (inodeNumber % 8)) & 0x01) == 0)
  {
    return -EINVALIDINODE;
  }
  delete [] inodeBitmap;

  // 计算inode所在的块地址
  int inode_in_block_addr = super.inode_region_addr + (inodeNumber / (UFS_BLOCK_SIZE / sizeof(inode_t)));
  assert(inode_in_block_addr >= super.inode_region_addr);
  assert(inode_in_block_addr < super.inode_region_addr + super.inode_region_len);

  // 计算inode在块中的偏移量
  int inode_in_block_offset_bytes = (inodeNumber % (UFS_BLOCK_SIZE / sizeof(inode_t))) * sizeof(inode_t);
  assert((inode_in_block_offset_bytes >= 0) && (inode_in_block_addr < UFS_BLOCK_SIZE));

  // 读取inode
  unsigned char *temp_block = new unsigned char [UFS_BLOCK_SIZE]();
  disk->readBlock(inode_in_block_addr, temp_block);
  memcpy(inode, temp_block + inode_in_block_offset_bytes, sizeof(inode_t));
  delete [] temp_block;

  return 0;
}

// 依赖readSuperBlock(), readInodeBitmap(), stat()
int LocalFileSystem::read(int inodeNumber, void *buffer, int size) {

  assert(buffer != nullptr);

  // 检查size是否合法
  if (size < 0)
  {
    return -EINVALIDSIZE;
  }
  // 若文件长度为0则直接返回，不必执行后边的操作
  else if (size == 0)
  {
    return 0;
  }

  // 读取超级块
  super_t super;
  readSuperBlock(&super);

  // 检查inodeNumber是否合法
  if (inodeNumber < 0 || inodeNumber >= super.num_inodes)
  {
    return -EINVALIDINODE;
  }

  // 获取inode bitmap，确认inode处于已分配的状态，若未分配则返回-EINVALIDINODE;
  unsigned char *inodeBitmap = new unsigned char [super.num_inodes / 8 + 1]();
  readInodeBitmap(&super, inodeBitmap);
  if (((inodeBitmap[inodeNumber / 8] >> (inodeNumber % 8)) & 0x01) == 0)
  {
    return -EINVALIDINODE;
  }
  delete [] inodeBitmap;

  // 获取inode
  inode_t inode;
  if (stat(inodeNumber, &inode) != 0)
  {
    return -EINVALIDINODE;
  }

  // 判断需要输出的数据长度，如果buffer足够容纳所有数据，则输出所有数据
  int remain_bytes;
  if (size >= inode.size)
  {
    remain_bytes = inode.size;
  }
  // 否则，根据inode类型判断
  else
  {
    // 情况1：inode是目录，则按照容纳最多个dir_ent_t的长度输出
    if (inode.type == UFS_DIRECTORY && size < inode.size)
    {
      assert(inode.size % sizeof(dir_ent_t) == 0);
      remain_bytes = size / sizeof(dir_ent_t) * sizeof(dir_ent_t);
    }
    // 情况2：inode是文件，则输出填满buffer的数据
    else if (inode.type == UFS_REGULAR_FILE && size < inode.size)
    {
      remain_bytes = size;
    }
    // 情况3：inode类型未知，视为文件处理
    else
    {
      remain_bytes = size;
    }
  }

  // 读取整块部分
  int read_bytes = 0;
  while (remain_bytes >= UFS_BLOCK_SIZE)
  {
    disk->readBlock(inode.direct[read_bytes / UFS_BLOCK_SIZE], ((unsigned char*)buffer) + read_bytes);
    read_bytes += UFS_BLOCK_SIZE;
    remain_bytes -= UFS_BLOCK_SIZE;
  }

  assert(read_bytes % UFS_BLOCK_SIZE == 0);
  assert(remain_bytes >= 0 && remain_bytes < UFS_BLOCK_SIZE);

  // 读取剩余的不足一块的部分（如有）
  if (remain_bytes > 0)
  {
    unsigned char *temp_block = new unsigned char [UFS_BLOCK_SIZE]();
    disk->readBlock(inode.direct[read_bytes / UFS_BLOCK_SIZE], temp_block);
    memcpy(((unsigned char*)buffer) + read_bytes, temp_block, remain_bytes);
    read_bytes += remain_bytes;
    remain_bytes = 0;
    delete [] temp_block;
  }

  return read_bytes;
}

// 依赖readSuperBlock(), stat(), read()
int LocalFileSystem::lookup(int parentInodeNumber, string name) {

  assert(name != "");

  // 检查parentInodeNumber是否合法
  super_t super;
  readSuperBlock(&super);
  if (parentInodeNumber < 0 || parentInodeNumber >= super.num_inodes)
  {
    return -EINVALIDINODE;
  }

  // 读取给定的inode
  inode_t inode;
  if (stat(parentInodeNumber, &inode) != 0)
  {
    return -EINVALIDINODE;
  }

  // 确认该inode类型为目录
  if (inode.type != UFS_DIRECTORY)
  {
    return -EINVALIDINODE;
  }

  map<string, int> dir_entries_map;

  // 计算目录中的entry数目
  int num_of_entries = inode.size / sizeof(dir_ent_t);

  // 读取entries
  unsigned char *dir_entries = new unsigned char [inode.size]();
  read(parentInodeNumber, dir_entries, inode.size);
  for (int i = 0; i < num_of_entries; i++)
  {
    dir_entries_map.insert(pair<string, int>(((dir_ent_t*)dir_entries + i)->name, ((dir_ent_t*)dir_entries + i)->inum));
  }
  delete [] dir_entries;

  // 在map中查询文件名，返回inode id
  if (dir_entries_map.find(name) != dir_entries_map.end())
  {
    return dir_entries_map.at(name);
  }
  else
  {
    return -ENOTFOUND;
  }
}

// 依赖readInodeBitmap(), readDataBitmap()
bool LocalFileSystem::diskHasSpace(super_t *super, int numInodesNeeded, int numDataBytesNeeded, int numDataBlocksNeeded)
{
  assert(super != nullptr);

  // inode_bitmap末尾不足一字节的bits数
  int inode_bitmap_last_bits_count = super->num_inodes % 8;
  // 同上
  int data_bitmap_last_bits_count = super->num_data % 8;

  // 读取inode_bitmap并计数
  unsigned char *inode_bitmap = new unsigned char [super->num_inodes / 8 + (inode_bitmap_last_bits_count > 0 ? 1 : 0)]();
  readInodeBitmap(super, inode_bitmap);
  int inode_remain = 0;
  for (int i = 0; i < super->num_inodes / 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      inode_remain += (inode_bitmap[i] >> j) & 0x01;
    }
  }
  for (int i = 0; i < inode_bitmap_last_bits_count; i++)
  {
    inode_remain += (inode_bitmap[super->num_inodes / 8] >> i) & 0x01;
  }
  delete [] inode_bitmap;

  // 读取data_bitmap并计数
  unsigned char *data_bitmap = new unsigned char [super->num_data / 8 + (data_bitmap_last_bits_count > 0 ? 1 : 0)]();
  readDataBitmap(super, data_bitmap);
  int data_block_remain = 0;
  for (int i = 0; i < super->num_data / 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      data_block_remain += (data_bitmap[i] >> j) & 0x01;
    }
  }
  for (int i = 0; i < data_bitmap_last_bits_count; i++)
  {
    data_block_remain += (data_bitmap[super->num_data / 8] >> i) & 0x01;
  }
  delete [] data_bitmap;

  // 计算所需的data block数
  numDataBlocksNeeded += numDataBytesNeeded / UFS_BLOCK_SIZE;
  if (numDataBytesNeeded % UFS_BLOCK_SIZE > 0)
  {
    numDataBlocksNeeded++;
  }

  // 约束1：直接请求的inodes需要满足
  if (numInodesNeeded > inode_remain)
  {
    return false;
  }
  // 约束2：直接请求的数据块需要满足
  else if (numDataBlocksNeeded > data_block_remain)
  {
    return false;
  }
  // 约束3：剩余的inode足够索引请求的数据块
  else if (numDataBlocksNeeded / DIRECT_PTRS > inode_remain)
  {
    return false;
  }

  return true;
}

//-------------------- 分割线 --------------------

void LocalFileSystem::writeInodeBitmap(super_t *super, unsigned char *inodeBitmap)
{
  assert(super != nullptr && inodeBitmap != nullptr);

  // 计算inodeBitmap有效长度（向上取整到字节）
  int inode_bitmap_len_bytes = super->num_inodes / 8;
  if (super->num_inodes % 8 > 0)
  {
    inode_bitmap_len_bytes++;
  }

  unsigned char *temp_blocks = new unsigned char [super->inode_bitmap_len * UFS_BLOCK_SIZE]();
  memcpy(temp_blocks, inodeBitmap, inode_bitmap_len_bytes);

  // 确保不多复制bits，保证写入的inodeBitmap合法
  unsigned char mask = 0xff;
  temp_blocks[inode_bitmap_len_bytes - 1] &= mask >> (8 - (super->num_inodes % 8));

  // 写入磁盘块
  disk->beginTransaction();
  for (int i = 0; i < super->inode_bitmap_len; i++)
  {
    disk->writeBlock(super->inode_bitmap_addr + i, temp_blocks + i * UFS_BLOCK_SIZE);
  }
  delete [] temp_blocks;
  disk->commit();
}

void LocalFileSystem::writeDataBitmap(super_t *super, unsigned char *dataBitmap)
{
  assert(super != nullptr && dataBitmap != nullptr);

  // 计算dataBitmap有效长度（向上取整到字节）
  int data_bitmap_len_bytes = super->num_data / 8;
  if (super->num_data % 8 > 0)
  {
    data_bitmap_len_bytes++;
  }

  // 加载data_bitmap
  unsigned char *temp_blocks = new unsigned char [super->data_bitmap_len * UFS_BLOCK_SIZE]();
  memcpy(temp_blocks, dataBitmap, data_bitmap_len_bytes);

  // 确保不多复制bits，保证写入的dataBitmap合法
  unsigned char mask = 0xff;
  temp_blocks[data_bitmap_len_bytes - 1] &= mask >> (8 - (super->num_data % 8));

  // 写入磁盘块
  disk->beginTransaction();
  for (int i = 0; i < super->data_bitmap_len; i++)
  {
    disk->writeBlock(super->data_bitmap_addr + i, temp_blocks + i * UFS_BLOCK_SIZE);
  }
  delete [] temp_blocks;
  disk->commit();
}

void LocalFileSystem::writeInodeRegion(super_t *super, inode_t *inodes)
{
  assert(super != nullptr && inodes != nullptr);

  // 计算inodes有效长度（对齐到sizeof(inode_t)的倍数）
  int inode_region_len_bytes = super->num_inodes * sizeof(inode_t);

  unsigned char *temp_blocks = new unsigned char [super->inode_region_len * UFS_BLOCK_SIZE]();
  memcpy(temp_blocks, inodes, inode_region_len_bytes);

  // 写入磁盘块
  disk->beginTransaction();
  for (int i = 0; i < super->inode_region_len; i++)
  {
    disk->writeBlock(super->inode_region_addr + i, temp_blocks + i * UFS_BLOCK_SIZE);
  }
  delete [] temp_blocks;
  disk->commit();
}

// 依赖readSuperBlock(), stat(), readDataBitmap(), writeDataBitmap(), readInodeRegion(), writeInodeRegion()
int LocalFileSystem::write(int inodeNumber, const void *buffer, int size) {

  assert(buffer != nullptr);

  // 检查inodeNumber是否数组越界
  super_t super;
  readSuperBlock(&super);
  if (inodeNumber < 0 || inodeNumber >= super.num_inodes)
  {
    return -EINVALIDINODE;
  }

  // 读取给定的inode
  inode_t inode;
  if (stat(inodeNumber, &inode) != 0)
  {
    return -EINVALIDINODE;
  }

  // 确认inode类型为文件
  if (inode.type != UFS_REGULAR_FILE)
  {
    return -EINVALIDTYPE;
  }

  // 确认size合法
  if (size < 0 || size > MAX_FILE_SIZE)
  {
    return -EINVALIDSIZE;
  }

  // 检查剩余空间是否够用
  if (!diskHasSpace(&super, 0, size, 0))
  {
    return -ENOTENOUGHSPACE;
  }

  // 计算所需block数
  unsigned int block_num_needed = size / UFS_BLOCK_SIZE;
  if (size % UFS_BLOCK_SIZE > 0)
  {
    block_num_needed++;
  }
  assert(block_num_needed <= DIRECT_PTRS);

  // 计算data_bitmap的有效长度（向上取整到字节）
  int data_bitmap_len_bytes = super.num_data / 8;
  if (super.num_data % 8 > 0)
  {
    data_bitmap_len_bytes++;
  }

  // 加载data_bitmap，寻找足够多的空块
  unsigned char *data_bitmap = new unsigned char [data_bitmap_len_bytes]();
  readDataBitmap(&super, data_bitmap);

  vector<int> allocated_data_blocks;
  int bytes_index, bits_index;
  for (bytes_index = 0; bytes_index < data_bitmap_len_bytes; bytes_index++)
  {
    if (data_bitmap[bytes_index] == 0xff)
    {
      continue;
    }

    for (bits_index = 0; bits_index < 8; bits_index++)
    {
      if (((data_bitmap[bytes_index] >> bits_index) & 0x01) == 0)
      {
        if (bytes_index * 8 + bits_index < super.num_data)
        {
          allocated_data_blocks.push_back(super.data_region_addr + bytes_index * 8 + bits_index);
        }
        else
        {
          break;
        }

        if (allocated_data_blocks.size() >= block_num_needed)
        {
          break;
        }
      }
    }

    if (bytes_index * 8 + bits_index >= super.num_data)
    {
      break;
    }

    if (allocated_data_blocks.size() >= block_num_needed)
    {
      break;
    }
  }
  assert(allocated_data_blocks.size() >= block_num_needed);

  while (allocated_data_blocks.size() > block_num_needed)
  {
    allocated_data_blocks.pop_back();
  }

  // 向数据块中写入数据
  int write_bytes = 0;
  int remain_bytes = size;
  unsigned char *temp_block = new unsigned char [UFS_BLOCK_SIZE]();
  disk->beginTransaction();
  for (vector<int>::iterator it = allocated_data_blocks.begin(); it != allocated_data_blocks.end(); it++)
  {
    int copy_num_of_bytes = remain_bytes > UFS_BLOCK_SIZE ? UFS_BLOCK_SIZE : remain_bytes;
    memcpy(temp_block, (void*)((unsigned char*)buffer + write_bytes), copy_num_of_bytes);
    disk->writeBlock(*it, temp_block);
    write_bytes += copy_num_of_bytes;
    remain_bytes -= copy_num_of_bytes;
  }
  disk->commit();
  delete [] temp_block;

  // 释放inode中原有的块
  int block_num_existing = inode.size / UFS_BLOCK_SIZE;
  if (inode.size % UFS_BLOCK_SIZE > 0)
  {
    block_num_existing++;
  }
  for (int i = 0; i < block_num_existing; i++)
  {
    int bytes_index = (inode.direct[i] - super.data_region_addr) / 8;
    int bit_index = (inode.direct[i] - super.data_region_addr) % 8;
    unsigned char mask = 0x01 << bit_index;
    data_bitmap[bytes_index] &= (~mask);
  }

  // 在data_bitmap中标记新块并写回
  for (vector<int>::iterator it = allocated_data_blocks.begin(); it != allocated_data_blocks.end(); it++)
  {
    int bytes_index = (*it - super.data_region_addr) / 8;
    int bits_index = (*it - super.data_region_addr) % 8;
    assert(((data_bitmap[bytes_index] >> bits_index) & 0x01) == 0);
    unsigned char mask = 0x01;
    data_bitmap[bytes_index] |= mask << bits_index;
  }
  writeDataBitmap(&super, data_bitmap);

  // 加载inode_region，修改并写回
  inode_t *inodes = (inode_t*)new unsigned char [super.num_inodes * sizeof(inode_t)]();
  readInodeRegion(&super, (inode_t*)inodes);
  for (unsigned i = 0; i < block_num_needed; i++)
  {
    inodes[inodeNumber].direct[i] = allocated_data_blocks.at(i);
  }
  inodes[inodeNumber].size = write_bytes;
  writeInodeRegion(&super, inodes);
  delete [] inodes;

  delete [] data_bitmap;
  return write_bytes;
}

// 依赖readSuperBlock(), stat(), lookup(), diskHasSpace(), readInodeBitmap(), writeInodeBitmap(), readInodeRegion(), writeInodeRegion(), read(), write()
int LocalFileSystem::create(int parentInodeNumber, int type, string name) {

  // 检查parentInodeNumber是否数组越界
  super_t super;
  readSuperBlock(&super);
  if (parentInodeNumber < 0 || parentInodeNumber >= super.num_inodes)
  {
    return -EINVALIDINODE;
  }

  // 加载partne_inode
  inode_t parent_inode;
  if (stat(parentInodeNumber, &parent_inode) != 0)
  {
    return -EINVALIDINODE;
  }

  // 确认parent_inode类型为目录
  if (parent_inode.type != UFS_DIRECTORY)
  {
    return -EINVALIDINODE;
  }

  // 检查type是否合法
  if (type != UFS_DIRECTORY && type != UFS_REGULAR_FILE)
  {
    return -EINVALIDTYPE;
  }

  // 检查文件名长度是否合法
  if (name.length() <= 0 || name.length() > DIR_ENT_NAME_SIZE - 1)
  {
    return -EINVALIDNAME;
  }
  // 禁止创建`.`和`..`两个文件名
  else if (name == "." || name == "..")
  {
    return -EINVALIDNAME;
  }

  // 检查parent目录下是否存在同名文件
  int lookup_result = lookup(parentInodeNumber, name);
  assert(lookup_result != -EINVALIDINODE);
  if (lookup_result >= 0) // 若存在同名文件，判断类型是否相同
  {
    // 获取同名文件inode
    inode_t lookup_inode;
    stat(lookup_result, &lookup_inode);

    // 同名文件的类型也相同，则视为create成功，否则不允许使用该文件名create
    if (lookup_inode.type == type)
    {
      return 0;
    }
    else
    {
      return -EINVALIDNAME;
    }
  }

  // 以下为创建新文件/目录的操作
  assert(lookup_result == -ENOTFOUND);

  // 检查inode是否够用
  if (!diskHasSpace(&super, 1, 0, 0))
  {
    return -ENOTENOUGHSPACE;
  }
  // 如果创建新目录，还要再分配一个block用于存储默认的`.`和`..`两个entries
  if (type == UFS_DIRECTORY && (!diskHasSpace(&super, 0, 0, 1)))
  {
    return -ENOTENOUGHSPACE;
  }

  // 计算inodeBitmap有效长度（向上取整到字节）
  int inode_bitmap_len_bytes = super.num_inodes / 8;
  if (super.num_inodes % 8 > 0)
  {
    inode_bitmap_len_bytes++;
  }

  // 读取inode_bitmap
  unsigned char *inode_bitmap = new unsigned char [inode_bitmap_len_bytes]();
  readInodeBitmap(&super, inode_bitmap);

  // 遍历inode_bitmap，找到一个未使用的inode
  int bytes_index = 0;
  for (; bytes_index < inode_bitmap_len_bytes; bytes_index++)
  {
    if (inode_bitmap[bytes_index] < 0xff)
    {
      break;
    }
  }
  assert(bytes_index != inode_bitmap_len_bytes);

  int bits_index = 0;
  for (; bits_index < 8; bits_index++)
  {
    if (((inode_bitmap[bytes_index] >> bits_index) & 0x01) == 0)
    {
      break;
    }
  }
  assert(bits_index != 8);

  // 分配一个inode号
  int allocated_inode_id = bytes_index * 8 + bits_index;
  assert(allocated_inode_id < super.num_inodes);

  // 如果创建的是目录，还需要分配一个新的block用于存储默认的`.`和`..`两个entries
  int one_more_block = -1;
  if (type == UFS_DIRECTORY)
  {
    // 计算data_bitmap的有效长度（向上取整到字节）
    int data_bitmap_len_bytes = super.num_data / 8;
    if (super.num_data % 8 > 0)
    {
      data_bitmap_len_bytes++;
    }

    // 加载data_bitmap，寻找一个空块
    unsigned char *data_bitmap = new unsigned char [data_bitmap_len_bytes]();
    readDataBitmap(&super, data_bitmap);

    int bytes_index, bits_index;
    for (bytes_index = 0; bytes_index < data_bitmap_len_bytes; bytes_index++)
    {
      if (data_bitmap[bytes_index] == 0xff)
      {
        continue;
      }

      for (bits_index = 0; bits_index < 8; bits_index++)
      {
        if (((data_bitmap[bytes_index] >> bits_index) & 0x01) == 0)
        {
          if (bytes_index * 8 + bits_index < super.num_data)
          {
            one_more_block = super.data_region_addr + bytes_index * 8 + bits_index;
            // 顺便将该块标记为已使用
            unsigned char mask = 0x01 << bits_index;
            data_bitmap[bytes_index] |= mask;
            break;
          }
        }
      }

      if (one_more_block >= 0)
      {
        break;
      }
    }

    assert(one_more_block >= super.data_region_addr);
    assert(one_more_block < super.data_region_addr + super.data_region_len);

    // 构造初始目录
    dir_ent_t temp_dir[2];
    memcpy(temp_dir[0].name, ".", 2);
    temp_dir[0].inum = allocated_inode_id;
    memcpy(temp_dir[1].name, "..", 3);
    temp_dir[1].inum = parentInodeNumber;

    // 将新增的entries写入该块
    unsigned char *temp_block = new unsigned char [UFS_BLOCK_SIZE]();
    memcpy(temp_block, &temp_dir, sizeof(dir_ent_t) * 2);
    disk->beginTransaction();
    disk->writeBlock(one_more_block, temp_block);
    disk->commit();
    delete [] temp_block;

    // 写回data_bitmap
    writeDataBitmap(&super, data_bitmap);
    delete [] data_bitmap;
  }

  // 修改父目录的inode_region并写回
  inode_t *inodes = (inode_t*)new unsigned char [super.num_inodes * sizeof(inode_t)]();
  readInodeRegion(&super, (inode_t*)inodes);
  inodes[allocated_inode_id].type = type;
  if (type == UFS_DIRECTORY)
  {
    inodes[allocated_inode_id].size = sizeof(dir_ent_t) * 2;
  }
  else
  {
    inodes[allocated_inode_id].size = 0;
  }
  inodes[allocated_inode_id].direct[0] = one_more_block;
  // 从1开始是因为0留给了one_more_block
  for (int i = 1; i < DIRECT_PTRS; i++)
  {
    inodes[allocated_inode_id].direct[i] = INT32_MAX;
  }
  writeInodeRegion(&super, inodes);

  // 修改inode_bitmap并写回
  inode_bitmap[bytes_index] |= 0x01 << bits_index;
  writeInodeBitmap(&super, inode_bitmap);
  delete [] inode_bitmap;

  // 判断是否需要为父目录分配新块
  int block_num_existing = parent_inode.size / UFS_BLOCK_SIZE;
  if (parent_inode.size % UFS_BLOCK_SIZE > 0)
  {
    block_num_existing++;
  }
  int block_num_needed = (parent_inode.size + sizeof(dir_ent_t)) / UFS_BLOCK_SIZE;
  if ((parent_inode.size + sizeof(dir_ent_t)) % UFS_BLOCK_SIZE > 0)
  {
    block_num_needed++;
  }

  // 如果目录项超出(MAX_FILE_SIZE / sizeof(dir_ent_t))个，则无法再创建新的子文件了
  if (block_num_needed > DIRECT_PTRS)
  {
    return -ENOTENOUGHSPACE;
  }

  // 构造要写入的目录项
  dir_ent_t temp_dir;
  strncpy(temp_dir.name, name.data(), DIR_ENT_NAME_SIZE - 1);
  temp_dir.inum = allocated_inode_id;

  // 如果需要额外再分配一个块
  if (block_num_needed > block_num_existing)
  {
    // 但无法分配一个块，则返回
    if (!diskHasSpace(&super, 0, sizeof(dir_ent_t), 0))
    {
      return -ENOTENOUGHSPACE;
    }

    // 计算data_bitmap的有效长度（向上取整到字节）
    int data_bitmap_len_bytes = super.num_data / 8;
    if (super.num_data % 8 > 0)
    {
      data_bitmap_len_bytes++;
    }

    int one_more_block = -1;

    // 加载data_bitmap，寻找足够多的空块
    unsigned char *data_bitmap = new unsigned char [data_bitmap_len_bytes]();
    readDataBitmap(&super, data_bitmap);

    int bytes_index, bits_index;
    for (bytes_index = 0; bytes_index < data_bitmap_len_bytes; bytes_index++)
    {
      if (data_bitmap[bytes_index] == 0xff)
      {
        continue;
      }

      for (bits_index = 0; bits_index < 8; bits_index++)
      {
        if (((data_bitmap[bytes_index] >> bits_index) & 0x01) == 0)
        {
          if (bytes_index * 8 + bits_index < super.num_data)
          {
            one_more_block = super.data_region_addr + bytes_index * 8 + bits_index;
            // 顺便将该块标记为已使用
            unsigned char mask = 0x01 << bits_index;
            data_bitmap[bytes_index] |= mask;
            break;
          }
        }
      }

      if (one_more_block >= 0)
      {
        break;
      }
    }

    // 写回data_bitmap
    writeDataBitmap(&super, data_bitmap);
    delete [] data_bitmap;

    assert(one_more_block >= super.data_region_addr);
    assert(one_more_block < super.data_region_addr + super.data_region_len);

    // 将新增的entry写入该块
    unsigned char *temp_block = new unsigned char [UFS_BLOCK_SIZE]();
    memcpy(temp_block, &temp_dir, sizeof(dir_ent_t));
    disk->beginTransaction();
    disk->writeBlock(one_more_block, temp_block);
    disk->commit();
    delete [] temp_block;

    // 更新inode_region
    inodes[parentInodeNumber].direct[block_num_needed - 1] = one_more_block;
  }
  // 在原有的块末尾新增entry
  else
  {
    int write_block_id = parent_inode.direct[block_num_existing - 1];
    int write_block_offset = parent_inode.size - (block_num_existing - 1) * UFS_BLOCK_SIZE;
    assert(write_block_offset >= 0 && write_block_offset < UFS_BLOCK_SIZE);

    unsigned char *temp_block = new unsigned char [UFS_BLOCK_SIZE]();
    disk->readBlock(write_block_id, temp_block);
    memcpy(temp_block + write_block_offset, &temp_dir, sizeof(dir_ent_t));
    disk->beginTransaction();
    disk->writeBlock(write_block_id, temp_block);
    disk->commit();
    delete [] temp_block;
  }

  // 修改inode_region并写回
  inodes[parentInodeNumber].size = parent_inode.size + sizeof(dir_ent_t);
  writeInodeRegion(&super, inodes);
  delete [] inodes;

  return 0;
}

int LocalFileSystem::unlink(int parentInodeNumber, string name) {

  // 检查parentInodeNumber是否数组越界
  super_t super;
  readSuperBlock(&super);
  if (parentInodeNumber < 0 || parentInodeNumber >= super.num_inodes)
  {
    return -EINVALIDINODE;
  }

  // 读取给定的inode
  inode_t parent_inode;
  if (stat(parentInodeNumber, &parent_inode) != 0)
  {
    return -EINVALIDINODE;
  }

  // 确认parent_inode类型为目录
  if (parent_inode.type != UFS_DIRECTORY)
  {
    return -EINVALIDINODE;
  }

  // 检查文件名长度是否合法
  if (name.length() <= 0 || name.length() > DIR_ENT_NAME_SIZE - 1)
  {
    return -EINVALIDNAME;
  }
  else if (name == "." || name == "..")
  {
    return -EINVALIDNAME;
  }

  // 检查当前目录下是否存在该文件
  // 注意：文件不存在并不被视为错误，直接返回即可，无需继续执行
  int lookup_result = lookup(parentInodeNumber, name);
  assert(lookup_result != -EINVALIDINODE);
  if (lookup_result == -ENOTFOUND)
  {
    return 0;
  }

  // 获取被删除的inode
  inode_t unlink_inode;
  if (stat(lookup_result, &unlink_inode))
  {
    return -EINVALIDINODE;
  }

  // 不允许unlink未知类型的inode
  if (unlink_inode.type != UFS_REGULAR_FILE && unlink_inode.type != UFS_DIRECTORY)
  {
    return -EUNLINKNOTALLOWED;
  }

  // 若被删除的是目录
  if (unlink_inode.type == UFS_DIRECTORY)
  {
    // 加载目录内容
    dir_ent_t *dir_entries = (dir_ent_t*)new unsigned char [unlink_inode.size];
    assert(unlink_inode.size % sizeof(dir_ent_t) == 0);
    read(lookup_result, dir_entries, unlink_inode.size);
    map<string, int> dir_entries_map;
    for (unsigned i = 0; i < unlink_inode.size / sizeof(dir_ent_t); i++)
    {
      dir_entries_map.insert(pair<string, int>(dir_entries[i].name, dir_entries[i].inum));
    }
    delete [] dir_entries;
    assert(dir_entries_map.size() == unlink_inode.size / sizeof(dir_ent_t));

    // 检查该目录是否为空（即有且仅有`.`和`..`两个entries），若为空则将其视同文件进行删除，否则返回-EDIRNOTEMPTY
    if (dir_entries_map.size() == 2 && dir_entries_map.find(".") != dir_entries_map.end() && dir_entries_map.find("..") != dir_entries_map.end())
    {
      unlink_inode.type = UFS_REGULAR_FILE;
    }
    else
    {
      return -EDIRNOTEMPTY;
    }
  }

  // 若被删除的是文件（以及被当作文件的空目录）
  if (unlink_inode.type == UFS_REGULAR_FILE)
  {
    // 计算inode索引了多少个块
    int blocks_in_use = unlink_inode.size / UFS_BLOCK_SIZE;
    if (unlink_inode.size % UFS_BLOCK_SIZE)
    {
      blocks_in_use++;
    }

    // 计算data_bitmap的有效长度（向上取整到字节）
    int data_bitmap_len_bytes = super.num_data / 8;
    if (super.num_data % 8 > 0)
    {
      data_bitmap_len_bytes++;
    }

    // 释放block：修改data_bitmap并写回
    unsigned char *data_bitmap = new unsigned char [data_bitmap_len_bytes]();
    readDataBitmap(&super, data_bitmap);
    for (int i = 0; i < blocks_in_use; i++)
    {
      int byte_index = (unlink_inode.direct[i] - super.data_region_addr) / 8;
      int bit_index = (unlink_inode.direct[i] - super.data_region_addr) % 8;
      unsigned char mask = 0x01 << bit_index;
      data_bitmap[byte_index] &= (~mask);
    }
    writeDataBitmap(&super, data_bitmap);
    delete [] data_bitmap;


    // 计算inodeBitmap有效长度（向上取整到字节）
    int inode_bitmap_len_bytes = super.num_inodes / 8;
    if (super.num_inodes % 8 > 0)
    {
      inode_bitmap_len_bytes++;
    }

    // 释放inode：修改inode_bitmap并写回
    unsigned char *inode_bitmap = new unsigned char [inode_bitmap_len_bytes]();
    readInodeBitmap(&super, inode_bitmap);
    unsigned mask = 0x01 << (lookup_result % 8);
    inode_bitmap[lookup_result / 8] &= (~mask);
    writeInodeBitmap(&super, inode_bitmap);
    delete [] inode_bitmap;

    // 加载父目录内容
    dir_ent_t *dir_entries = (dir_ent_t*)new unsigned char [parent_inode.size];
    assert(parent_inode.size % sizeof(dir_ent_t) == 0);
    read(parentInodeNumber, dir_entries, parent_inode.size);

    // 用目录的最后一条覆盖
    unsigned int unlink_entry_index = 0;
    for (; unlink_entry_index < parent_inode.size / sizeof(dir_ent_t); unlink_entry_index++)
    {
      if (dir_entries->inum == lookup_result)
      {
        break;
      }
    }
    memcpy((void*)(dir_entries + unlink_entry_index), (void*)(dir_entries + (parent_inode.size / sizeof(dir_ent_t)) - 1), sizeof(dir_ent_t));
    write(parentInodeNumber, dir_entries, parent_inode.size - sizeof(dir_ent_t));
    delete [] dir_entries;

    // 修改inode_region并写回
    inode_t *inodes = (inode_t*)new unsigned char [super.num_inodes * sizeof(inode_t)]();
    readInodeRegion(&super, (inode_t*)inodes);
    inodes[parentInodeNumber].size = parent_inode.size - sizeof(dir_ent_t);
    writeInodeRegion(&super, inodes);
    delete [] inodes;
  }

  return 0;
}
