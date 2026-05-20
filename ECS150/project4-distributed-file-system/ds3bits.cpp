#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>

#include "LocalFileSystem.h"
#include "Disk.h"
#include "ufs.h"

using namespace std;


int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout << argv[0] << ": diskImageFile" << endl;
    return 1;
  }

  Disk *disk = new Disk(argv[1], UFS_BLOCK_SIZE);
  LocalFileSystem *lfs = new LocalFileSystem(disk);

  super_t super;
  lfs->readSuperBlock(&super);

  cout << "Super" << endl;
  cout << "inode_region_addr " << super.inode_region_addr << endl;
  cout << "data_region_addr " << super.data_region_addr << endl << endl;

  unsigned char *inode_bitmap = new unsigned char [super.data_bitmap_len * UFS_BLOCK_SIZE];
  lfs->readInodeBitmap(&super, inode_bitmap);
  cout << "Inode bitmap" << endl;
  for (int i = 0; i < super.data_bitmap_len * UFS_BLOCK_SIZE; i++)
  {
    cout << (unsigned int) inode_bitmap[i] << " ";
  }
  cout << endl << endl;

  unsigned char *data_bitmap = new unsigned char [super.data_bitmap_len * UFS_BLOCK_SIZE];
  lfs->readDataBitmap(&super, data_bitmap);
  cout << "Data bitmap" << endl;
  for (int i = 0; i < super.data_bitmap_len * UFS_BLOCK_SIZE; i++)
  {
    cout << (unsigned int) data_bitmap[i] << " ";
  }
  cout << endl;

  delete [] data_bitmap;
  delete [] inode_bitmap;
  delete lfs;
  delete disk;
  return 0;
}
