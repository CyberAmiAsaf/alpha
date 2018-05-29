#ifndef ALPHA_FS_H
#define ALPHA_FS_H

#include <hd.h>
#include <util.h>

#define FT_NML  1
#define FT_DIR  2

#define BLOCK_LEN 8

struct INODE {
        unsigned int i_mode;            /* file mode */
        unsigned int i_size;            /* size in bytes */
        unsigned int i_block[BLOCK_LEN];
};

#define MAX_NAME_LEN 11

struct DIR_ENTRY {
        char de_name[MAX_NAME_LEN]; // <?>
        int  de_inode;
};

struct INODE_NUM {
	struct INODE inode;
	unsigned int inode_num;
};

struct SUPER_BLOCK {
		/* Type of Partition 0x08 */
        unsigned char sb_magic;
        /* Partition Sector Number 0x0c */
        unsigned int sb_start;
        /* Partition Capacity */
        unsigned int sb_blocks;
        /* How Many Blocks to Store the Bitmap of Blocks */
        unsigned int sb_dmap_blks;
        /* How Many Blocks to Store the Bitmap of INodes */
        unsigned int sb_imap_blks;
        /* How Many Blocks to Store the Actual INode Array */
        unsigned int sb_inode_blks;
};


// Macros for Convenience

#define ABS_BOOT_BLK(sb)        ((sb).sb_start)
#define ABS_SUPER_BLK(sb)       ((ABS_BOOT_BLK(sb))+1)
#define ABS_DMAP_BLK(sb)        ((ABS_SUPER_BLK(sb))+1)
#define ABS_IMAP_BLK(sb)        ((ABS_DMAP_BLK(sb))+(sb).sb_dmap_blks)
#define ABS_INODE_BLK(sb)       ((ABS_IMAP_BLK(sb))+(sb).sb_imap_blks)
#define ABS_DATA_BLK(sb)        ((ABS_INODE_BLK(sb))+INODE_BLKS)

/* 512*8 = 4096 inodes */
#define INODE_BIT_BLKS          1
#define INODES_PER_BLK          (512/sizeof(struct INODE))
#define INODE_BLKS              ((INODE_BIT_BLKS*512*8+INODES_PER_BLK-1)/(INODES_PER_BLK))


void setb(void *s, unsigned int i);
void clrb(void *s, unsigned int i);
int testb(void *s, unsigned int i);

void printHD0(unsigned int* h);

void verify_fs(void);


//extern struct INODE iroot;

unsigned int alloc_blk(struct SUPER_BLOCK *sb);
void free_blk(struct SUPER_BLOCK *sb, unsigned int n);

int alloc_inode(struct SUPER_BLOCK *sb);
void free_inode(struct SUPER_BLOCK *sb, int n);


struct INODE * iget(struct SUPER_BLOCK *sb, struct INODE *inode, unsigned int n);
void iput(struct SUPER_BLOCK *sb, struct INODE *inode, unsigned int n);

void check_root(void);

void stat(struct INODE *inode);

void verify_dir(void);

void loadSB(struct SUPER_BLOCK* sb, unsigned int sb_start);
struct DIR_ENTRY* loadDE(struct INODE* inode, char* sectBuffer);

struct INODE_NUM findFile(char* path);
struct INODE_NUM findFileEx(struct SUPER_BLOCK* sb, char* path, struct DIR_ENTRY* currentDE, 
	struct INODE currentDirINODE);
struct INODE_NUM makeFile(char* fileName, char* pathToDir);
struct INODE_NUM makeFolder(char* folderName, char* pathToDir);



void removeDirEntry(struct DIR_ENTRY* de, unsigned int remove, unsigned int len);
void removeFile(struct SUPER_BLOCK* sb, struct DIR_ENTRY* de, unsigned int i,
				struct INODE fileToRemove, unsigned int len);
void removeFolder(struct SUPER_BLOCK* sb, struct DIR_ENTRY* de, unsigned int i, 
				struct INODE folderToRemove, unsigned int len);
void deleteFile(char* file);


void writeToFile(char* fileName, char* data, unsigned int len);
char* readFromFile(char* fileName);


unsigned int getFileType(unsigned int inode_num);

char* simplify(char* path);

// high-levle funcs
void touch(char *filename);
void mkdir(char *foldername);

struct FOLDER {
        char* pathToDir;
        char* dirName;
};
struct FOLDER getNameOfContainingFolder(char* path);

#endif
