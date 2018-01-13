#include "logger.h"

int log(char *type, char *str) {
  to_uppercase(type);

  printf("[%s] %s\r\n", type, str);
}
