#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>

#include "LocalFileSystem.h"
#include "Disk.h"
#include "ufs.h"

using namespace std;


int main(int argc, char *argv[]) {
  if (argc != 3) {
    cout << argv[0] << ": diskImageFile inodeNumber" << endl;
    return 1;
  }

  int i = atoi(argv[2]);

  Disk *disk = new Disk(argv[1], UFS_BLOCK_SIZE);
  LocalFileSystem *lfs = new LocalFileSystem(disk);

  super_t super;
  lfs->readSuperBlock(&super);

  inode_t *inode_region = (inode_t*)new unsigned char [super.inode_region_len * UFS_BLOCK_SIZE]();
  lfs->readInodeRegion(&super, inode_region);

  // 计算存储文件所需的块数
  int num_of_blocks = inode_region[i].size / UFS_BLOCK_SIZE;
  if (inode_region[i].size - num_of_blocks * UFS_BLOCK_SIZE > 0)
  {
    num_of_blocks++;
  }

  // 打印block id
  cout << "File blocks" << endl;
  for (int j = 0; j < num_of_blocks; j++)
  {
    cout << inode_region[i].direct[j] << endl;
  }
  cout << endl;

  // cat操作
  char *contents = new char [inode_region[i].size]();
  lfs->read(i, (void*)contents, inode_region[i].size);
  contents[inode_region[i].size] = '\0';
  cout << "File data" << endl << contents;

  delete [] contents;
  delete [] inode_region;
  delete lfs;
  delete disk;
}
