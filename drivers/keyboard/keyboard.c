#include "../keyboard.h"

#define SC_MAX 57

enum KEYCODE {
  BACKSPACE_PRESSED = 0x0e,
  ENTER_PRESSED = 0x1c,

  LSHIFT_PRESSED = 0x2a,
  LSHIFT_RELEASED = 0xaa,

  RSHIFT_PRESSED = 0x36,
  RSHIFT_RELEASED = 0xb6,

  CAPSLOCK_PRESSED = 0x3a,
  CAPSLOCK_RELEASED = 0xba,

  SPACEBAR_PRESSED = 0x39,
  SPACEBAR_RELEASED = 0xb9,

  ALT_PRESSED = 0x38,
  ALT_RELEASED = 0xb8,

  CTRL_PRESSED = 0x1d,
  CTRL_RELEASED = 0x9d
};

static char keycache[256];

struct {
  bool lshift;
  bool rshift;
  bool capslock;
  bool spacebar;
  bool ctrl;
  bool alt;
} kbd_pressed;

const char *sc_name[] = {
  "ERROR", "Esc", "1", "2", "3", "4", "5", "6",
  "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E",
  "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl",
  "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`",
  "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".",
  "/", "RShift", "Keypad *", "LAlt", "Spacebar"
};

const char sc_uppercase[] = {
  '?', '?', '!', '@', '#', '$', '%', '^',
  '&', '*', '(', ')', '_', '+', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
  'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G',
  'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V',
  'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '
};

const char sc_lowercase[] = {
  '?', '?', '1', '2', '3', '4', '5', '6',
  '7', '8', '9', '0', '-', '=', '?', '?', 'q', 'w', 'e', 'r', 't', 'y',
  'u', 'i', 'o', 'p', '[', ']', '?', '?', 'a', 's', 'd', 'f', 'g',
  'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v',
  'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' '
};

bool should_use_uppercase() {
  return (kbd_pressed.lshift | kbd_pressed.rshift) ^ kbd_pressed.capslock;
}

static void clear_keycache() {
  for (int i = 0; i < 256; i++) {
    keycache[i] = 0x00;
  }
}

static void append_key_to_cache(char letter) {
  char *key = keycache;
  while (*key) {
    key++;
  }

  *key = letter;
}

void backspace() {
  char *key = keycache;
  while (*key) {
    key++;
  }

  if (key != keycache) {
    *(--key) = 0x00;
    backwards_cursor();
    update_cursor(cursor_row, cursor_col);
  }
}

static void keyboard_callback(registers_t regs) {
  // PIC leaves scancode in port 0x60
  u8 scancode = port_byte_in(0x60);
  char letter;

  switch (scancode) {
    // shift
    case LSHIFT_PRESSED:
      kbd_pressed.lshift = true;
      break;

    case LSHIFT_RELEASED:
      kbd_pressed.lshift = false;
      break;

    case RSHIFT_PRESSED:
      kbd_pressed.rshift = true;
      break;

    case RSHIFT_RELEASED:
      kbd_pressed.rshift = false;
      break;
    
    // space bar
    case SPACEBAR_PRESSED:
      print_char(' ');
      update_cursor(cursor_row, cursor_col);
      append_key_to_cache(' ');
      kbd_pressed.spacebar = true;
      break;

    case SPACEBAR_RELEASED:
      kbd_pressed.spacebar = false;
      break;
    
    // caps lock
    case CAPSLOCK_PRESSED:
      kbd_pressed.capslock = !kbd_pressed.capslock;
      break;
    
    // ctrl
    case CTRL_PRESSED:
      kbd_pressed.ctrl = true;
      break;

    case CTRL_RELEASED:
      kbd_pressed.ctrl = false;
      break;

    // alt
    case ALT_PRESSED:
      kbd_pressed.alt = true;
      break;

    case ALT_RELEASED:
      kbd_pressed.alt = false;
      break;

    case BACKSPACE_PRESSED:
      backspace();
      break;

    // enter
    case ENTER_PRESSED:
      printf("\n");
      int result = execute_command(keycache);
      clear_keycache();
      break;

    default:
      if (scancode < SC_MAX) {
        letter = should_use_uppercase() ? sc_uppercase[(int)scancode] : sc_lowercase[(int)scancode];
        append_key_to_cache(letter);
        print_char(letter);
        update_cursor(cursor_row, cursor_col);
      }

      break;
  }
}

void init_keyboard() {
  memset(&kbd_pressed, false, sizeof(kbd_pressed));
  clear_keycache();
  register_interrupt_handler(IRQ1, (isr_t)keyboard_callback);
}
