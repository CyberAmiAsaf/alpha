#ifndef ALPHA_FS_H
#define ALPHA_FS_H

#include <hd.h>
#include <util.h>

#define FS_FILE  1
#define FS_DIR   2

#define MAX_NAME_LEN 8
#define BLOCK_LEN 2

struct INODE {
  unsigned int type;            /* file mode */
  unsigned int size;            /* size in bytes */
  unsigned int block[BLOCK_LEN];
  unsigned int num;
  unsigned int father_num;
  char name[MAX_NAME_LEN];
};

#define INODE_BIT_BLKS          1
#define INODE_SECTORS           1
#define INODES_PER_BLK          (512/sizeof(struct INODE))
#define INODE_FIRST_SECTOR      1
#define MAX_INODES              (INODE_SECTORS * 512 / sizeof(struct INODE))

char* simplify_path(char* path);

bool verify_alpha_fs();
void init_alpha_fs();

// high-levle funcs
void touch(char *filename);
void mkdir(char *foldername);

void ls();
struct INODE find_inode_by_name(char *name);
bool inode_exists(char *name);


#endif
