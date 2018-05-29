#include "../fs.h"

struct INODE iget(unsigned int n) {
  struct INODE inodes[MAX_INODES] = {0};

  hd_read(1, INODE_SECTORS, inodes);

  struct INODE i = inodes[n];
  struct INODE dummy = {0};
  dummy.num = -1;

  return i.num == 0 && n != 0 ? dummy : i;
}

struct INODE iput(struct INODE inode) {
  struct INODE inodes[MAX_INODES] = {0};
  hd_read(1, INODE_SECTORS, inodes);

  memcpy(&inodes[inode.num], &inode, sizeof(struct INODE));

  hd_write(1, INODE_SECTORS, inodes);

  return inodes[inode.num];
}

struct INODE alloc_inode(struct INODE new) {
  struct INODE inodes[MAX_INODES] = {0};
  hd_read(1, INODE_SECTORS, inodes);

  for (int i = 1; i < MAX_INODES; i++) {
    struct INODE t = inodes[i];
    if (t.num != i || t.name[0] == '\0') {
      printf("found! %d", i);
      new.num = i;
      memcpy(&inodes[i], &new, sizeof(struct INODE));
      hd_write(1, INODE_SECTORS, inodes);
      break;
    }
  }

  return new;
}

struct INODE free_inode(unsigned int n) {
  struct INODE inodes[MAX_INODES] = {0};
  hd_read(1, INODE_SECTORS, inodes);

  memset(&inodes[n], 0, sizeof(struct INODE));

  hd_write(1, INODE_SECTORS, inodes);

  return inodes[n];
}

bool verify_alpha_fs() {
  struct INODE root = iget(0);
  return root.num == 0 && root.type == FS_DIR && root.father_num == -1 && strcmp(root.name, "/") == 0;
}

void init_alpha_fs() {
  struct INODE root = {.type = FS_DIR, .size = 0, .num = 0, .father_num = -1, .name = "/"};
  iput(root);
}

void touch(char *filename) {
  struct INODE i = {0};
  strcpy(filename, i.name);
  alloc_inode(i);
}

void ls() {
  struct INODE inodes[MAX_INODES] = {0};
  hd_read(1, INODE_SECTORS, inodes);

  for (int i = 1; i < MAX_INODES; i++) {
    struct INODE t = inodes[i];
    if (t.name[0] != '\0') {
      printf("%s\n", t.name);
    }
  }
}

void mkdir(char *foldername) {
  // char *simplified_path = simplify(foldername);
	// struct FOLDER f = getNameOfContainingFolder(simplified_path);

	// makeFolder(f.dirName, f.pathToDir);

	// free(simplified_path);
	// free(f.pathToDir);
  // free(f.dirName);
}
