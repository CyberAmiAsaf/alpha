#include "../shell.h"

void print_ps1() {
  printf(PS1);
  enable_cursor(0, 255);
  update_cursor(cursor_row, cursor_col);
}

/* handles */

void reboot() {
  u8 good = 0x02;
  while (good & 0x02) {
    good = port_byte_in(0x64);
  }

  port_byte_out(0x64, 0xfe);
  __asm__("hlt");
}

void handle_echo(int argc, char argv[32][64]) {
  for (int i = 1; i < argc && strlen(argv[i]) > 0; i++) {
    printf("%s ", argv[i]);
  }

  printf("\n");
}

void handle_touch(int argc, char argv[32][64]) {
  char *path;

  if (argc != 2 || strlen(argv[1]) >= MAX_NAME_LEN || strcmp(argv[1], "--help") == 0) {
    printf("usage:\n\ttouch <file_name>\n\t\tlen(file_name) < %d\n", MAX_NAME_LEN);
    return;
  }

  path = argv[1];

  if (true || !inode_exists(path)) {
    touch(path);
    printf("file \"%s\" has been created succesfully\n", path);
  } else {
    printf("path \"%s\" already exists :(\n", path);
  }
}

void handle_rm(int argc, char argv[32][64]) {
  char *path = argv[1];
  struct INODE i;

  if (argc != 2 || strlen(path) >= MAX_NAME_LEN || strncmp(path, "--help", 6) == 0) {
    printf("usage:\n\trm <file_name>\n\t\tlen(file_name) < %d\n", MAX_NAME_LEN);
    return;
  }

  if (false && !inode_exists(path)) {
    printf("path \"%s\" doesn't exist :(\n", path);
  } else {
     i = find_inode_by_name(path);
    free_inode(i.num);
    printf("file \"%s\" has been removed\n", path);
  }
}

void handle_ls(int argc, char argv[32][64]) {
  ls();
}

int execute_command(char *command_line) {
  char argv[32][64];
  int argc = 0;
  int arg_ch = 0;

  disable_cursor();

  // parse arguments
  char *str_ptr = command_line;

  // null args
  memset(argv, 0, 32 * 64);

  // parse loop
  while (*str_ptr) {
    while (*str_ptr && isspace(*str_ptr)) {
      ++str_ptr;
    }

    while (*str_ptr && !isspace(*str_ptr)) {
      argv[argc][arg_ch] = *str_ptr;

      ++str_ptr;
      ++arg_ch;
    }

    ++argc;
    arg_ch = 0;
  }

  if (strlen(argv[0]) == 0) {
  } else if (strcmp(argv[0], "reboot") == 0) {
    reboot();
  } else if (strcmp(argv[0], "touch") == 0) {
    handle_touch(argc, argv);
  } else if (strcmp(argv[0], "echo") == 0) {
    handle_echo(argc, argv);
  } else if (strcmp(argv[0], "ls") == 0) {
    handle_ls(argc, argv);
  } else if (strcmp(argv[0], "rm") == 0) {
    handle_rm(argc, argv);
  } else if (strcmp(argv[0], "clear") == 0) {
    clear_screen();
  } else {
    printf("'%s' is not a recognized command (%d args inputted)\n", argv[0], argc);
  }

  printf(PS1);
  enable_cursor(0, 255);
  update_cursor(cursor_row, cursor_col);
  return 0;
}
