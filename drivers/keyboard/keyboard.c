#include "keyboard.h"

#define BACKSPACE 0x0E
#define ENTER 0x1C

static char key_buffer[256];

#define SC_MAX 57

const char *sc_name[] = {
  "ERROR", "Esc", "1", "2", "3", "4", "5", "6",
  "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E",
  "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl",
  "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`",
  "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".",
  "/", "RShift", "Keypad *", "LAlt", "Spacebar"
};

const char sc_ascii[] = {
  '?', '?', '1', '2', '3', '4', '5', '6',
  '7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
  'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G',
  'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V',
  'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '
};

static void clear_key_buffer() {
  for (int i = 0; i < 256; i++) {
    key_buffer[i] = '\0';
  }
}

static void append_key_to_buffer(char letter) {
  char *key = key_buffer;
  while (*key) {
    key++;
  }

  *key = letter;
}

static void keyboard_callback(registers_t regs) {
  // PIC leaves scancode in port 0x60
  u8 scancode = port_byte_in(0x60);

  if (scancode > SC_MAX)
    return;

  if (scancode == BACKSPACE) {
    // backspace(key_buffer);
    // kprint_backspace();
  } else if (scancode == ENTER) {
    printf("\n");

    int result = execute_command(key_buffer);

    for (int i = 0; i < 256; i++) {
      key_buffer[i] =  '\0';
    }
  } else {
    char letter = sc_ascii[(int)scancode];
    append_key_to_buffer(letter);
    print_char(letter);
  }

//  UNUSED(regs);
}

void init_keyboard() {
  clear_key_buffer();
  register_interrupt_handler(IRQ1, keyboard_callback);
}
