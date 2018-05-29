#include "../fs.h"

void setb(void *s, unsigned int i) {
  unsigned char *v = s;
    v += i>>3; // adds the offset divided by 8 (offset in bytes instead of bits)

    // i%8 refers to which bit in the byte specified by v it refers to
    // 7 - (i&8) iis because it is a little endian architecture
    *v |= 1<<(7-(i%8)); // lights the specified bit up
}

void clrb(void *s, unsigned int i) {
    unsigned char *v = s;
    v += i>>3; // adds the offset divided by 8 (offset in bytes instead of bits)

    // i%8 refers to which bit in the byte specified by v it refers to
    // 7 - (i&8) iis because it is a little endian architecture
    *v &= ~(1<<(7-(i%8))); // turns the specified bit off
}

int testb(void *s, unsigned int i) {
    unsigned char *v = s;
    v += i>>3; // adds the offset divided by 8 (offset in bytes instead of bits)

    // i%8 refers to which bit in the byte specified by v it refers to
    // 7 - (i&8) iis because it is a little endian architecture
    return (*v&(1<<(7-(i%8)))) !=0; // returns if the specified bit is on or off
}

void printHD0(unsigned int* h) {
  char temp[16];

  printf("\nPARTITION 1:");

  printf("\n    START: ");
  itoa(h[0], temp);
  printf(temp);

  printf("\n    Capacity: ");
  itoa(h[1], temp);
  printf(temp);


  h += 2;

  printf("\nPARTITION 2:");

  printf("\n    START: ");
  itoa(h[0], temp);
  printf(temp);

  printf("\n    Capacity: ");
  itoa(h[1], temp);
  printf(temp);
}


void verify_fs(void) {
  extern unsigned int* hd0;

  unsigned int *q = hd0;

  // printHD0(q);

  unsigned char sect[512] = {0};
  struct SUPER_BLOCK sb;

  unsigned int i = 0, j = 0, m = 0, n = 0;

  /* get the fs sb */
  sb.sb_start = q[0];

  hd_rw(ABS_SUPER_BLK(sb), HD_READ, 1, &sb);

  /* the first partition must be FST_FS type */
    if (sb.sb_magic != FST_FS) {
      printf("Partition 1 does not have a valid file system\n");
      printf("Creating file system\n");
      sb.sb_magic = FST_FS;
      sb.sb_start = q[0];
    sb.sb_blocks = q[1];
    sb.sb_dmap_blks = (sb.sb_blocks+0xfff)>>12;
    sb.sb_imap_blks = INODE_BIT_BLKS;
    sb.sb_inode_blks = INODE_BLKS;
    hd_rw(ABS_SUPER_BLK(sb), HD_WRITE, 1, &sb);


    /* initial data bitmap */
    n = ABS_DMAP_BLK(sb);
    j = sb.sb_dmap_blks+sb.sb_imap_blks+sb.sb_inode_blks+2;
    memset(sect, 0xff, sizeof sect/sizeof sect[0]);
    for (i=j/(512*8); i>0; --i) {
      hd_rw(n++, HD_WRITE, 1, sect);
      m += 4096;
    }

    m += 4096;
    for (i=j%(512*8); i<512*8; ++i) {
      clrb(sect, i);
      --m;
    }
    hd_rw(n++, HD_WRITE, 1, sect);

    memset(sect, 0, sizeof sect/sizeof sect[0]);
    for (i=sb.sb_imap_blks-(n-ABS_DMAP_BLK(sb)); i>0; --i)
      hd_rw(n++, HD_WRITE, 1, sect);


    /* initail inode bitmap */
    for (i=sb.sb_imap_blks; i>0; --i)
      hd_rw(ABS_IMAP_BLK(sb)+i-1, HD_WRITE, 1, sect);

    /* initial inode blocks */
    for (i=sb.sb_inode_blks; i>0; --i)
      hd_rw(ABS_INODE_BLK(sb)+i-1, HD_WRITE, 1, sect);
    printf("DONE\n");

    }

    char tmp[200];

    printf("0: Type: FST_FS ");

    printf("start at: ");
    itoa(sb.sb_start, tmp);
    printf(tmp);
    printf(", ");

    printf("blocks: ");
    itoa(sb.sb_blocks, tmp);
    printf(tmp);
    printf(", ");

    printf("dmap: ");
    itoa(sb.sb_dmap_blks, tmp);
    printf(tmp);
    printf(", ");

    printf("imap: ");
    itoa(sb.sb_imap_blks, tmp);
    printf(tmp);
    printf(", ");

  printf("inodes: ");
    itoa(sb.sb_inode_blks, tmp);
  printf(tmp);


  printf("\n"); 
}


unsigned int alloc_blk(struct SUPER_BLOCK *sb) {
  unsigned int i = 0, j = 0, n = 0, m = 0;
  unsigned char sect[512] = {0};
  n = ABS_DMAP_BLK(*sb);

  for (; i < sb->sb_dmap_blks; ++i) {
    hd_rw(n, HD_READ, 1, sect);
        for (j=0; j<512*8; ++j) {
            if (testb(sect, j)) {
                ++m;
            } else {            /* gotcha */
                setb(sect, j);
                if (m >= sb->sb_blocks)
                    return 0;
                else {
                    hd_rw(n, HD_WRITE, 1, sect);
                    return ABS_BOOT_BLK(*sb) + m;
                }
            }

        }

        ++n;
  }

  return 0;
}

void free_blk(struct SUPER_BLOCK *sb, unsigned int n) {
  unsigned char sect[512] = {0};
    unsigned int t = (n-ABS_BOOT_BLK(*sb))/(512*8)+ABS_DMAP_BLK(*sb);
    hd_rw(t, HD_READ, 1, sect);
    clrb(sect, (n-ABS_BOOT_BLK(*sb))%(512*8));
    hd_rw(t, HD_WRITE, 1, sect);
}

int alloc_inode(struct SUPER_BLOCK *sb) {
  unsigned char sect[512] = {0};
  int i = 0;
  hd_rw(ABS_IMAP_BLK(*sb), HD_READ, 1, sect);
  for (; i<512; ++i) {
    if (!testb(sect, i)) {
      setb(sect, i);
      hd_rw(ABS_IMAP_BLK(*sb), HD_WRITE, 1, sect);
      break;
    }
  }

  return i == 512 ? -1 : i;
}

void free_inode(struct SUPER_BLOCK *sb, int n) {
  unsigned char sect[512] = {0};
  hd_rw(ABS_IMAP_BLK(*sb), HD_READ, 1, sect);
  clrb(sect, n);
  hd_rw(ABS_IMAP_BLK(*sb), HD_WRITE, 1, sect);
}


struct INODE* iget(struct SUPER_BLOCK *sb, struct INODE *inode, unsigned int n) {
    unsigned char sect[512] = {0};
    int i = INODES_PER_BLK;
    int j = INODES_PER_BLK;

    hd_rw(ABS_INODE_BLK(*sb)+i, HD_READ, 1, sect);
    memcpy(sect+j*sizeof(struct INODE), inode, sizeof(struct INODE));

    return inode;
}

void iput(struct SUPER_BLOCK *sb, struct INODE *inode, unsigned int n) {
    unsigned char sect[512] = {0};
    int i = n/INODES_PER_BLK;
    int j = n%INODES_PER_BLK;
    hd_rw(ABS_INODE_BLK(*sb)+i, HD_READ, 1, sect);
    memcpy(inode, sect+j*sizeof(struct INODE), sizeof(struct INODE));
    hd_rw(ABS_INODE_BLK(*sb)+i, HD_WRITE, 1, sect);
}


void check_root(void) {
  struct INODE iroot = {FT_DIR, 2*sizeof(struct DIR_ENTRY), {0,}};
  extern unsigned int* hd0;

  struct SUPER_BLOCK sb;
  unsigned char sect[512] = {0};
  struct DIR_ENTRY *de = 0;

  sb.sb_start = *(unsigned int *)(hd0);
  hd_rw(ABS_SUPER_BLK(sb), HD_READ, 1, sect);
  memcpy(sect, &sb, sizeof(struct SUPER_BLOCK));
  hd_rw(ABS_IMAP_BLK(sb), HD_READ, 1, sect);
  if (!testb(sect, 0)) {
    printf("/ has not been created, creating....");

    if (alloc_inode(&sb) != 0) {
        printf("\n/ must be inode 0!!!\n");
        for(;;); // halt();
    }

    iroot.i_block[0] = alloc_blk(&sb);
    iput(&sb, &iroot, 0);
    de = (struct DIR_ENTRY *)sect;
    strcpy(".", de->de_name);
    de->de_inode = 0;
    ++de;
    strcpy("..", de->de_name);
    de->de_inode = -1;
    hd_rw(iroot.i_block[0], HD_WRITE, 1, sect);

    printf("DONE\n");
  }

  iget(&sb, &iroot, 0);
  hd_rw(iroot.i_block[0], HD_READ, 1, sect);
  de = (struct DIR_ENTRY *)sect;

  if (strcmp(de[0].de_name, ".") || (de[0].de_inode) || strcmp(de[1].de_name, "..") || de[1].de_inode != -1) {
    printf("File system is corrupted!!!\n");
    for(;;); // halt();
  }
}

void stat(struct INODE *inode) {
  unsigned int i = 0;
  char sect[512] = {0};
  struct DIR_ENTRY *de;

  printf("======== stat / ========\n");

  switch (inode->i_mode) {
  case FT_NML:
    printf("File, ");
    break;
  case FT_DIR:
    printf("Dir,  ");
    break;
  default:
    printf("UNKNOWN FILE TYPE!!\n");
  }

  printf("Size: ");
  char size[20];
  itoa(inode->i_size, size);
  printf(size);
  printf(", ");

  printf("Blocks: ");
  for (; i<8; ++i) {
    // char block[get_num_digits(inode->i_block[i]) + 1];
    char block[20];
    itoa(inode->i_block[i], block);
    printf(block);
    printf(", ");
  }

  hd_rw(inode->i_block[0], HD_READ, 1, sect);
  switch (inode->i_mode) {
    case FT_DIR:
      printf("\nName    INode\n");
      de = (struct DIR_ENTRY *)sect;
      for (i = 0; i < inode->i_size/sizeof(struct DIR_ENTRY); ++i) {
        char inode_str[20];
        printf(de[i].de_name);
        printf("    ");
        itoa(de[i].de_inode, inode_str);
        printf(inode_str);
        printf("\n");
          }
          break;
      default:
        break;
  }
}


void verify_dir(void) {
  extern unsigned int* hd0;
  unsigned char sect[512] = {0};
  unsigned int *q = hd0;

  struct INODE inode;
  struct SUPER_BLOCK sb;

  sb.sb_start = q[0];

  hd_rw(ABS_SUPER_BLK(sb), HD_READ, 1, sect);
  check_root();

  memcpy(sect, &sb, sizeof(struct SUPER_BLOCK));

  // iget(&sb, &inode, 0);
  // stat(&inode);
}


void loadSB(struct SUPER_BLOCK* sb, unsigned int sb_start) {
  char sectBuffer[512]= {0};

  sb->sb_start = sb_start;

    hd_rw(ABS_SUPER_BLK(*sb), HD_READ, 1, sectBuffer);
    memcpy(sectBuffer, sb, sizeof(struct SUPER_BLOCK));
}


struct DIR_ENTRY* loadDE(struct INODE* inode, char* sectBuffer) {
  hd_rw(inode->i_block[0], HD_READ, 1, sectBuffer);
  return (struct DIR_ENTRY *) sectBuffer;
}


struct INODE_NUM findFile(char* path) {
  // Assumes path with syntax /XXX/YYY/ZZZ/ . . .
  extern unsigned int* hd0;

  unsigned int *q = hd0;

  struct INODE inode;
    struct SUPER_BLOCK sb;
    struct DIR_ENTRY* de;
    char sectBuffer[512];

    loadSB(&sb, q[0]);


    iget(&sb, &inode, 0);

    hd_rw(inode.i_block[0], HD_READ, 1, sectBuffer);
    de = (struct DIR_ENTRY *) sectBuffer;

    if(strcmp("/", path) == 0) {  // They are equal !
      struct INODE_NUM result = {inode, 0};
      return result;
    }


    char* newPath = path + 1;
    return findFileEx(&sb, newPath, de, inode);

}

struct INODE_NUM findFileEx(struct SUPER_BLOCK* sb, char* path, struct DIR_ENTRY* currentDE, 
  struct INODE currentDirINODE) {
  int isDir = 0;
  char fileName[MAX_NAME_LEN];
  char ch = path[0];
  int i = 0;
  while(ch != 0) {
    if (ch == '/') {
      isDir = 1;
      break;
    } else if(i < MAX_NAME_LEN) {
      fileName[i] = ch;
    } else {
      printf("\nFILE OR DIR NAME LONGER THAN MAX_NAME_LEN\n");
      printf("PART OF THE FILE NAME THAT FITS: ");
      printf(fileName);
      printf("\n");
      struct INODE_NUM dummy = {{0,0,{0,}}, -1};
      return dummy;
      for(;;); // halt();
    }

    i++;
    ch = path[i];
  }
  fileName[i] = 0;
  path += i + isDir; // + i to pass the first i characters of the 
                  // new dir name
                  // + isDir to pass the '/' if it is there
  for (int j = 0; j < currentDirINODE.i_size/sizeof(struct DIR_ENTRY); ++j) {
    if (strcmp(fileName, currentDE[j].de_name) == 0) {  // They are equal!

      struct INODE nextFile;
      iget(sb, &nextFile, currentDE[j].de_inode);

      if (path[0] != 0) {  // If it is a dir and there is request 
                      // to search in that dir, keep searching


        // Load new DE
        hd_rw(nextFile.i_block[0], HD_READ, 1, currentDE);
        return findFileEx(sb, path, currentDE, nextFile);
      } else {
        struct INODE_NUM res = {nextFile, currentDE[j].de_inode};
        return res;
      }

    }
  }

  struct INODE_NUM dummy = {{0,0,{0,}}, -1};
  return dummy;
}


struct INODE_NUM makeFile(char* fileName, char* pathToDir) {
  struct INODE_NUM dir = findFile(pathToDir);
  // stat(dir.inode);

  extern unsigned int* hd0;

  unsigned int *q = hd0;

  unsigned int sb_start = q[0];

  struct SUPER_BLOCK sb;

  loadSB(&sb, sb_start);

  char s[512];
  hd_rw(dir.inode.i_block[0], HD_READ, 1, s);
  // struct DIR_ENTRY* de = loadDE(dir, s);  // Check out why this is causing override of code segment;

  struct DIR_ENTRY* de = (struct DIR_ENTRY *) s;

  dir.inode.i_size += sizeof(struct DIR_ENTRY);
  unsigned int i = dir.inode.i_size/sizeof(struct DIR_ENTRY) - 1;
  strcpy(fileName, de[i].de_name);

  struct INODE file = {FT_NML, 0, {0,}};
  unsigned int inode_num = alloc_inode(&sb);
  file.i_block[0] = alloc_blk(&sb);
  iput(&sb, &file, inode_num);
  de[i].de_inode = inode_num;
  hd_rw(dir.inode.i_block[0], HD_WRITE, 1, de);
  iput(&sb, &dir.inode, dir.inode_num);
  struct INODE_NUM result = {file, inode_num};
  return result;
}



struct INODE_NUM makeFolder(char* folderName, char* pathToDir) {
  struct INODE_NUM dir = findFile(pathToDir);
  // stat(dir.inode);

  extern unsigned int* hd0;

  unsigned int *q = hd0;

  unsigned int sb_start = q[0];

  struct SUPER_BLOCK sb;

  loadSB(&sb, sb_start);

  char s[512];
  hd_rw(dir.inode.i_block[0], HD_READ, 1, s);
  // struct DIR_ENTRY* de = loadDE(dir, s);  // Check out why this is causing override of code segment;
  
  struct DIR_ENTRY* de = (struct DIR_ENTRY *) s;
  
  dir.inode.i_size += sizeof(struct DIR_ENTRY);
  unsigned int i = dir.inode.i_size/sizeof(struct DIR_ENTRY) - 1;
  strcpy(folderName, de[i].de_name);

  struct INODE folder = {FT_DIR, 2*sizeof(struct DIR_ENTRY), {0,}};
  unsigned int inode_num = alloc_inode(&sb);
  folder.i_block[0] = alloc_blk(&sb);
  iput(&sb, &folder, inode_num);
  de[i].de_inode = inode_num;
  hd_rw(dir.inode.i_block[0], HD_WRITE, 1, de);

  // Create 2 directory entries: "." & ".."
  memset(s, 0x00, 512);
  de = (struct DIR_ENTRY *) s;
  strcpy(".", de->de_name);
    de->de_inode = inode_num;
    ++de;
    strcpy("..", de->de_name);
    de->de_inode = dir.inode_num;
    hd_rw(folder.i_block[0], HD_WRITE, 1, s);

  iput(&sb, &dir.inode, dir.inode_num);
  struct INODE_NUM result = {folder, inode_num};
  return result;
}


void removeDirEntry(struct DIR_ENTRY* de, unsigned int remove, unsigned int len) {
  // - 1 because we removed the element in the index `remove`
  unsigned int number_of_bytes = (len - remove - 1)*sizeof(struct DIR_ENTRY);  
  

  struct DIR_ENTRY* source = de;
  source += remove + 1;

  struct DIR_ENTRY* destination = de;
  destination += remove;

  memcpy(source,
         destination, number_of_bytes);

  struct DIR_ENTRY* lastElement = de;
  lastElement += len - 1;  // - 1 because we removed one element
              // so the size of the array decreased by 1


  memset(lastElement, 0, sizeof(struct DIR_ENTRY));
}

void removeFile(struct SUPER_BLOCK* sb, struct DIR_ENTRY* de, unsigned int i, struct INODE fileToRemove, unsigned int len) {
  for(int j = 0; j < BLOCK_LEN; j++) {
    if(fileToRemove.i_block[j] != 0) {
      free_blk(sb, fileToRemove.i_block[j]);
      fileToRemove.i_block[j] = 0;
    }
  }
  
  free_inode(sb, de[i].de_inode);
}


void removeFolder(struct SUPER_BLOCK* sb, struct DIR_ENTRY* de, unsigned int i, struct INODE folderToRemove, unsigned int len) {
  char s[512];
  hd_rw(folderToRemove.i_block[0], HD_READ, 1, s);
  // struct DIR_ENTRY* de = loadDE(dir, s);  // Check out why this is causing override of code segment;
  
  struct DIR_ENTRY* inner_de = (struct DIR_ENTRY *) s;

  for (unsigned int j = 2; j < folderToRemove.i_size/sizeof(struct DIR_ENTRY); ++j) {  // 2 to skip . and ..
    struct INODE fileToRemove;
    iget(sb, &fileToRemove, inner_de[j].de_inode);
    if(fileToRemove.i_mode == FT_NML) {
      removeFile(sb, inner_de, j, fileToRemove,
            folderToRemove.i_size/sizeof(struct DIR_ENTRY));
    } else if(fileToRemove.i_mode == FT_DIR) {
      removeFolder(sb, inner_de, j, fileToRemove, 
            folderToRemove.i_size/sizeof(struct DIR_ENTRY));
    }

    // Remove the dir entry
    memset(inner_de + j*sizeof(struct DIR_ENTRY), 0, sizeof(struct DIR_ENTRY));
  }

  removeFile(sb, de, i, folderToRemove, len);
}


void deleteFile(char* file) {
  if(strcmp("/", file) == 0) {  // They are equal !
    printf("\n----------------CAN'T DELETE ROOT FOLDER!----------------\n");
    for(;;);  // halt();
  }
  
  int len = strlen(file);
  char pathToDir[len];
  char* fileName;
  strcpy(file, pathToDir);
  for(int i = len - 1; i >= 0; i--) {
    if(pathToDir[i] == '/') {
      pathToDir[i + 1] = 0;
      fileName = file + i + 1;
      break;
    }
  }

  struct INODE_NUM dir = findFile(pathToDir);
  // stat(dir.inode);

  extern unsigned int* hd0;

  unsigned int *q = hd0;

  unsigned int sb_start = q[0];

  struct SUPER_BLOCK sb;

  loadSB(&sb, sb_start);

  char s[512];
  hd_rw(dir.inode.i_block[0], HD_READ, 1, s);
  // struct DIR_ENTRY* de = loadDE(dir, s);  // Check out why this is causing override of code segment;
  
  struct DIR_ENTRY* de = (struct DIR_ENTRY *) s;
  

  for (unsigned int i = 0; i < dir.inode.i_size/sizeof(struct DIR_ENTRY); ++i) {
    if(strcmp(de[i].de_name, fileName) == 0) {  // They are equal !
      struct INODE fileToRemove;
      iget(&sb, &fileToRemove, de[i].de_inode);
      if(fileToRemove.i_mode == FT_NML) {
        removeFile(&sb, de, i, 
                fileToRemove, dir.inode.i_size/sizeof(struct DIR_ENTRY));
      } else if(fileToRemove.i_mode == FT_DIR) {
        removeFolder(&sb, de, i, fileToRemove, dir.inode.i_size/sizeof(struct DIR_ENTRY));
      }

      removeDirEntry(de, i, len);

      //Update the Dir Entries
      hd_rw(dir.inode.i_block[0], HD_WRITE, 1, de);


      //Update the directories size
      dir.inode.i_size -= sizeof(struct DIR_ENTRY);
      iput(&sb, &dir.inode, dir.inode_num);

      return;
    }
  }

  printf("\nDidn't find file or folder in the specified location\n");
  return;
}






void writeToFile(char* fileName, char* data, unsigned int len) {
  struct INODE_NUM file = findFile(fileName);
  if(file.inode_num == -1) {
    printf("Illegal file or file not found\n");
    printf("Creating the file automatically\n");
    touch(fileName);
    file = findFile(fileName);
  }
  unsigned int sects_num = len/512;

  unsigned int block = 0;
  for(; block < BLOCK_LEN && block < sects_num; block++) {
    hd_rw(file.inode.i_block[block], HD_WRITE, 1, data + block*512);
  }

  unsigned int remainder = len%512;
  if(remainder) {
    char sect[512];
    memcpy(data + sects_num*512, sect, remainder);
    hd_rw(file.inode.i_block[block], HD_WRITE, 1, sect);
  }

  file.inode.i_size = len;
  extern unsigned int* hd0;

  unsigned int *q = hd0;

    struct SUPER_BLOCK sb;

    loadSB(&sb, q[0]);

  iput(&sb, &file.inode, file.inode_num);


}

char* readFromFile(char* fileName) {
  struct INODE file = findFile(fileName).inode;
  if(file.i_mode == FT_DIR) {
    printf("Can't read contents of a directory\n");
    return 0;
  }
  char* dataBuffer = malloc(file.i_size);
  unsigned int sects_num = file.i_size/512;
  unsigned int remainder = file.i_size%512;

  unsigned int block = 0;
  for(; block < BLOCK_LEN && block < sects_num; block++) {
    hd_rw(file.i_block[block], HD_READ, 1, dataBuffer + block*512);
  }

  if(remainder) {
    char sect[512];
    hd_rw(file.i_block[block], HD_READ, 1, sect);
    memcpy(sect, dataBuffer + sects_num*512, remainder);
  }

  return dataBuffer;

}


unsigned int getFileType(unsigned int inode_num) {
  if(inode_num == -1) {  // Special case - the ".." dir from the "/" root dir
    return FT_DIR;
  }

  extern unsigned int* hd0;

  unsigned int *q = hd0;

  unsigned int sb_start = q[0];

  struct SUPER_BLOCK sb;

  loadSB(&sb, sb_start);

  struct INODE inode;
  iget(&sb, &inode, inode_num);
  return inode.i_mode;
}



char* simplify(char* path) {
  unsigned int len_path = strlen(path);
  unsigned int last_slash = 0;
  unsigned int pointer = 0;
  unsigned int removed = 0;
  while(pointer != len_path) {
    if(path[pointer] == '/') {

      if(path[pointer + 1] == '.' && path[pointer + 2] == '.') {
        if(pointer == 0) {
          pointer++;
          continue;
        }

        // Mark for removal
        for(int i = last_slash + 1; i <= pointer + 2; i++) {
          path[i] = 0;
          removed++;
        }

        if(path[pointer + 3] == '/') {
          path[pointer + 3] = 0;
          removed++;
        }

      } else if(path[pointer + 1] == '.') {
        // Mark for removal
        path[pointer + 1] = 0;
        removed++;
        if(path[pointer + 2] == '/') {
          path[pointer + 2] = 0;
          removed++;
        }
      }



      last_slash = pointer;
    }

    pointer++;
  }


  char* tmp = malloc(len_path - removed);
  unsigned int j = 0;
  for(int i = 0; i < len_path; i++) {
    if(path[i] != 0) {
      tmp[j++] = path[i];
    }
  }

  return tmp;
}


struct FOLDER getNameOfContainingFolder(char* path) {
  struct FOLDER f;

  int len = strlen(path);
  char* pathToDir = malloc(len);
  char* dirName = malloc(len);
  strcpy(path, pathToDir);
  for(int i = len - 1; i >= 0; i--) {
    if(pathToDir[i] == '/') {
      pathToDir[i + 1] = 0;
      strcpy(path + i + 1, dirName);
      break;
    }
  }

  f.pathToDir = pathToDir;
  f.dirName = dirName;
  return f;
}

void touch(char *filename) {
  printf("\nstarting touch!\n");
  char *simplified_path = simplify(filename);
  printf("%s\n", simplified_path);
	struct FOLDER f = getNameOfContainingFolder(simplified_path);

	makeFile(f.dirName, f.pathToDir);

	free(simplified_path);
	free(f.pathToDir);
  free(f.dirName);
}

void mkdir(char *foldername) {
  char *simplified_path = simplify(foldername);
	struct FOLDER f = getNameOfContainingFolder(simplified_path);

	makeFolder(f.dirName, f.pathToDir);

	free(simplified_path);
	free(f.pathToDir);
  free(f.dirName);
}
