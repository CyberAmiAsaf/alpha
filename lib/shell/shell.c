#include "../shell.h"

bool unlocked = false;

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

  if (!inode_exists(path)) {
    touch(path);
    printf("file \"%s\" has been created succesfully\n", path);
  } else {
    printf("path \"%s\" already exists :(\n", path);
  }
}

void handle_rm(int argc, char argv[32][64]) {
  char *path = argv[1];
  struct INODE *i;

  if (argc != 2 || strlen(path) >= MAX_NAME_LEN || strncmp(path, "--help", 6) == 0) {
    printf("usage:\n\trm <file_name>\n\t\tlen(file_name) < %d\n", MAX_NAME_LEN);
    return;
  }

  i = find_inode_by_name(path);
  printf("num => %d\n", i->num);
  if (i->num == -1) {
    printf("path \"%s\" doesn't exist :(\n", path);
  } else {
    free_inode(i->num);
    printf("file \"%s\" has been removed\n", path);
  }
}

void handle_ls(int argc, char argv[32][64]) {
  ls();
}

void handle_help(int argc, char argv[32][64]) {
  printf("help screen:\n\tcalc - calculates 2 operands\n\ttouch - create file\n\trm - remove file\n\tappend - append text to file\n\tcat - output file\n\tdate - print out time and date\n\techo - output to stdout text\n\treboot - reboot system\n\tls - display all files in dir\n\tclear - clear screen\n\tpassword - set new password\n\tunlock - unlock system with password\n");
}

void handle_calc(int argc, char argv[32][64]) {
  if (argc != 4) {
    printf("usage: calc <n1> <op> <n2>\n");
    return;
  }

  int a = atoi(argv[1]);
  int b = atoi(argv[3]);
  char op = *argv[2];

  switch (op) {
  case '*':
    printf("%d", a * b);
    break;

  case '+':
    printf("%d", a + b);
    break;

  case '-':
    printf("%d", a - b);
    break;

  case '/':
    if (b == 0) {
      printf("division by 0 is not allowed");
      break;
    }
    printf("%d", a / b);
    break;

  default:
    printf("unknown op");
  }

  printf("\n");
}

void cat(int argc, char argv[32][64]) {
  char *path = argv[1];
  struct INODE *i;

  if (argc != 2 || strlen(path) >= MAX_NAME_LEN || strncmp(path, "--help", 6) == 0) {
    printf("usage:\n\tcat <file_name>\n\t\tlen(file_name) < %d\n", MAX_NAME_LEN);
    return;
  }

  i = find_inode_by_name(path);
  // printf("num => %d\n", i->num);
  if (i->num == -1) {
    printf("file \"%s\" doesn't exist :(\n", path);
  } else {
    char buffer[512] = {0};

    file_read(i, buffer);

    printf("%s\n", buffer);
  }
}

void write(int argc, char argv[32][64]) {
  char *path = argv[1];
  struct INODE *i;

  if (argc < 3 || strlen(path) >= MAX_NAME_LEN || strncmp(path, "--help", 6) == 0) {
    printf("usage:\n\twrite <file_name> <content>\n\t\tlen(file_name) < %d\n", MAX_NAME_LEN);
    return;
  }

  i = find_inode_by_name(path);
  // printf("num => %d\n", i->num);
  if (i->num == -1) {
    printf("file \"%s\" doesn't exist :(\n", path);
  } else {
    int l = 0;
    char buffer[512] = {0};
    for (int x = 2; x < argc && strlen(argv[x]) > 0; x++) {
      strcpy(argv[x], &buffer[l]);
      l += strlen(argv[x]) + 1;
      buffer[l-1] = ' ';
    }

    file_write(i, buffer);
  }
}

void handle_unlock(int argc, char argv[32][64]) {
  if (argc != 2) {
    printf("usage:\n\tunlock <pass>\n");
    return;
  }

  char buffer[512] = {0};
  get_password(buffer);
  if (strcmp(buffer, argv[1]) == 0) {
    printf("unlocked!\n");
    unlocked = true;
  } else {
    printf("wrong password!\n");
  }
}

void handle_password(int argc, char argv[32][64]) {
  if (argc != 2) {
    printf("usage:\n\tpassword <pass>");
    return;
  }

  char buffer[512] = {0};
  strcpy(argv[1], buffer);
  save_password(buffer);
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
  } else if (strcmp(argv[0], "help") == 0) {
    handle_help(argc, argv);
  } else if (strcmp(argv[0], "unlock") == 0) {
    handle_unlock(argc, argv);
  } else if (!unlocked) {
    printf("Run unlock to unlock, help for more info.\n");
  } else if (strcmp(argv[0], "touch") == 0) {
    handle_touch(argc, argv);
  } else if (strcmp(argv[0], "reboot") == 0) {
    reboot();
  } else if (strcmp(argv[0], "echo") == 0) {
    handle_echo(argc, argv);
  } else if (strcmp(argv[0], "ls") == 0) {
    handle_ls(argc, argv);
  } else if (strcmp(argv[0], "rm") == 0) {
    handle_rm(argc, argv);
  } else if (strcmp(argv[0], "calc") == 0) {
    handle_calc(argc, argv);
  } else if (strcmp(argv[0], "clear") == 0) {
    clear_screen();
  } else if (strcmp(argv[0], "write") == 0) {
    write(argc, argv);
  } else if (strcmp(argv[0], "password") == 0) {
    handle_password(argc, argv);
  } else if (strcmp(argv[0], "cat") == 0) {
    cat(argc, argv);
  } else if (strcmp(argv[0], "date") == 0) {
    printf("%2d:%2d:%2d\n", hw_time.hour, hw_time.min, hw_time.sec);
  } else {
    printf("'%s' is not a recognized command (%d args inputted)\n", argv[0], argc);
  }

  printf(PS1);
  enable_cursor(0, 255);
  update_cursor(cursor_row, cursor_col);
  return 0;
}
