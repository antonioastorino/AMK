#include "my-keyboard.h"
#include "oled.h"
#define LED 17
#define LOCK_1 7
#define LOCK_2 6
#define CLK 21
#define S_DATA 5
#define COLS 16
#define ROWS 4
#define BUTTON 65
#define MAX_SIMULTANELUS_KEYS (12)
// Numeric keypad
#define KEY_LAYER_NUM 255
#define KEY_LAYER_SYM 254
#define KEY_FN_01 0
#define KEY_FN_02 0
#define KEY_FN_03 0
#define KEY_FN_04 0
#define KEY_FN_05 KEY_VOLUMEDOWN
#define KEY_FN_06 KEY_VOLUMEUP
#define KEY_FN_07 0
#define KEY_FN_08 0
#define KEY_MOD_LCMD KEY_MOD_LMETA
#define KEY_MOD_RCMD KEY_MOD_RMETA
#define KEY_MOD_LOPTION KEY_MOD_LALT
#define KEY_MOD_ROPTION KEY_MOD_RALT

uint8_t layout_0[2][ROWS][COLS] = {
    {
        // Keys
        {KEY_FN_01, KEY_ESC, KEY_TAB, KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y,
         KEY_U, KEY_I, KEY_O, KEY_P, KEY_LEFTBRACE, KEY_RIGHTBRACE, KEY_FN_05},
        {KEY_FN_02, KEY_CAPSLOCK, KEY_GRAVE, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G,
         KEY_H, KEY_J, KEY_K, KEY_L, KEY_SEMICOLON, KEY_APOSTROPHE, KEY_ENTER,
         KEY_FN_06},
        {KEY_FN_03, 0, 0, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N, KEY_M,
         KEY_COMMA, KEY_DOT, KEY_SLASH, 0, KEY_BACKSLASH, KEY_FN_07},
        {KEY_FN_04, 0, 0, 0, 0, 0, 0, KEY_BACKSPACE, KEY_SPACE, 0, 0, KEY_LEFT,
         KEY_DOWN, KEY_UP, KEY_RIGHT, KEY_FN_08},
    },
    {
        // Modifiers
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, KEY_MOD_LSHIFT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, KEY_MOD_RSHIFT, 0,
         0},
        {0, 0, KEY_MOD_LCTRL, KEY_MOD_LOPTION, KEY_MOD_LCMD, KEY_LAYER_SYM,
         KEY_LAYER_NUM, 0, 0, KEY_MOD_RCMD, 0, 0, 0, 0, 0, 0},
    },
};
uint8_t layout_1[2][ROWS][COLS] = {
    {
        // Keys
        {0, KEY_ESC, 0, 0, 0, 0, 0, 0, 0, KEY_KP7, KEY_KP8, KEY_KP9,
         KEY_KPEQUAL, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, KEY_KP4, KEY_KP5, KEY_KP6, KEY_EQUAL,
         KEY_KPSLASH, KEY_KPENTER, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, KEY_KP1, KEY_KP2, KEY_KP3, KEY_KPMINUS, 0,
         0, 0},
        {0, 0, 0, 0, 0, 0, KEY_LAYER_NUM, KEY_BACKSPACE, KEY_SPACE, KEY_KP0, 0,
         KEY_KPDOT, KEY_KPASTERISK, 0, 0},
    },
    {
        // Modifiers
        {KEY_MOD_LSHIFT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, KEY_MOD_LSHIFT, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, KEY_LAYER_SYM, KEY_LAYER_NUM, 0, 0, 0, 0, 0, 0, 0, 0,
         0},
    },
};
uint8_t layout_2[2][ROWS][COLS] = {
    {
        // Keys
        {0, 0, 0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
         KEY_0, KEY_MINUS, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, KEY_LEFT, KEY_DOWN, KEY_UP, KEY_RIGHT, 0, 0, 0,
         0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, KEY_LAYER_SYM, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        // Modifiers
        {0, 0, 0, KEY_MOD_LSHIFT, KEY_MOD_LSHIFT, KEY_MOD_LSHIFT,
         KEY_MOD_LSHIFT, KEY_MOD_LSHIFT, KEY_MOD_LSHIFT, KEY_MOD_LSHIFT,
         KEY_MOD_LSHIFT, KEY_MOD_LSHIFT, KEY_MOD_LSHIFT, KEY_MOD_LSHIFT, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, KEY_LAYER_SYM, KEY_LAYER_NUM, 0, 0, 0, 0, 0, 0, 0, 0,
         0},
    },
};
const uint8_t row_to_pin[ROWS] = {8, 9, 16, 10};
char monitor_str[64] = {0};
uint8_t (*current_layout)[2][ROWS][COLS];

typedef struct _keyPressed {
  uint8_t row;
  uint8_t col;
} keyPressed;

bool key_num_pressed = false;
bool key_sym_pressed = false;

void flush_registers() {
  digitalWrite(S_DATA, LOW);
  for (uint8_t b = 0; b < 15; b++) {
    digitalWrite(CLK, HIGH);
    digitalWrite(CLK, LOW);
  }
  // Assert the last bit - this will be the first loaded in the shift register
  digitalWrite(S_DATA, HIGH);
  digitalWrite(CLK, HIGH);
  digitalWrite(CLK, LOW);
}

void setup() {
  current_layout = &layout_0;
  oled_init(S_DATA, CLK);
#ifdef DEBUG
  Serial.begin(115200);
  delay(2000);
#endif
  for (int i = 0; i < ROWS; i++) {
    pinMode(row_to_pin[i], INPUT);
    digitalWrite(row_to_pin[i],
                 LOW); // should disable internal pull-up resistors
  }
  pinMode(LED, OUTPUT);
  pinMode(LOCK_1, OUTPUT);
  pinMode(LOCK_2, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(S_DATA, OUTPUT);
  Keyboard.begin();
  oled_disable();
}

void loop() {
  uint8_t button = 0;
  uint8_t modifier = 0;
  keyPressed key_pressed[MAX_SIMULTANELUS_KEYS];
  uint8_t key_pressed_counter = 0;
  bool new_key_num_pressed = false;
  bool new_key_sym_pressed = false;

  Keyboard.releaseAll();
  digitalWrite(S_DATA, LOW);
  for (int col = 0; col < COLS; col++) {
    if (col == COLS - 1) {
      digitalWrite(S_DATA, HIGH);
    }
    digitalWrite(CLK, HIGH);
    digitalWrite(CLK, LOW);
    for (int row = 0; row < ROWS && key_pressed_counter < MAX_SIMULTANELUS_KEYS;
         row++) {
      button = (*current_layout)[0][row][col];
      modifier = (*current_layout)[1][row][col];
      if (digitalRead(row_to_pin[row])) {
        if (modifier == KEY_LAYER_NUM) {
          new_key_num_pressed = true;
          new_key_sym_pressed = false;
          continue;
        } else if (modifier == KEY_LAYER_SYM) {
          new_key_num_pressed = false;
          new_key_sym_pressed = true;
          continue;
        }
        key_pressed[key_pressed_counter].col = col;
        key_pressed[key_pressed_counter].row = row;
        key_pressed_counter++;
      }
    }
  }
  // Change layout and do not send this button
  if (new_key_num_pressed) {
    current_layout = &layout_1;
    digitalWrite(LED, LOW);
    digitalWrite(LOCK_1, HIGH);
    digitalWrite(LOCK_2, LOW);
  } else if (new_key_sym_pressed) {
    current_layout = &layout_2;
    digitalWrite(LED, LOW);
    digitalWrite(LOCK_1, LOW);
    digitalWrite(LOCK_2, HIGH);
  } else {
    digitalWrite(LED, HIGH);
    digitalWrite(LOCK_1, LOW);
    digitalWrite(LOCK_2, LOW);
    current_layout = &layout_0;
  }
  if (key_num_pressed != new_key_num_pressed) {
    oled_displayBitmap();
    oled_disable();
    flush_registers();
    Keyboard.releaseAll();
  }
  if (key_sym_pressed != new_key_sym_pressed) {
    Keyboard.releaseAll();
  }
  uint8_t button_pressed_count = 0;
  for (uint8_t p = 0; p < key_pressed_counter; p++) {

    uint8_t row = key_pressed[p].row;
    uint8_t col = key_pressed[p].col;
    uint8_t button = (*current_layout)[0][row][col];
    uint8_t modifier = (*current_layout)[1][row][col];
    if (button && button_pressed_count < 6) {
      Keyboard.pressRaw(button, button_pressed_count);
      button_pressed_count++;
    }
    if (modifier) {
      Keyboard.setModifiers(modifier);
    }
    oled_addInlineSymbol(col, row, 'a');

#ifdef DEBUG
    sprintf(monitor_str, "Row: %d, Col: %d, B%d\n", row, col, button);
    Serial.write(monitor_str);
#endif
  }

  Keyboard.sendReport();
  key_num_pressed = new_key_num_pressed;
  key_sym_pressed = new_key_sym_pressed;
}
