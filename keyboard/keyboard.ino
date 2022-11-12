#include "oled.h"
#include <Keyboard.h>
#define LED 17
#define LOCK_1 7
#define LOCK_2 6
#define CLK 21
#define S_DATA 5
#define COLS 16
#define ROWS 4
#define BUTTON 65
#define MAX_SIMULTANELUS_KEYS (16)
// Numeric keypad
#define KEY_NUM_LOCK 0xDB
#define KEY_KP_SLASH 0xDC
#define KEY_KP_ASTERISK 0xDD
#define KEY_KP_MINUS 0xDE
#define KEY_KP_PLUS 0xDF
#define KEY_KP_EQUAL 0x67
#define KEY_KP_ENTER 0xE0
#define KEY_KP_1 0xE1
#define KEY_KP_2 0xE2
#define KEY_KP_3 0xE3
#define KEY_KP_4 0xE4
#define KEY_KP_5 0xE5
#define KEY_KP_6 0xE6
#define KEY_KP_7 0xE7
#define KEY_KP_8 0xE8
#define KEY_KP_9 0xE9
#define KEY_KP_0 0xEA
#define KEY_KP_DOT 0xEB
#define KEY_NUM 255
#define KEY_FN_01 0
#define KEY_FN_02 0
#define KEY_FN_03 0
#define KEY_FN_04 0
#define KEY_FN_05 0
#define KEY_FN_06 0
#define KEY_FN_07 0
#define KEY_FN_08 0

uint8_t keys[ROWS][COLS] = {
    {KEY_FN_01, KEY_ESC, KEY_TAB, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o',
     'p', '[', ']', KEY_FN_05},
    {KEY_FN_02, KEY_CAPS_LOCK, '`', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
     0, 0, KEY_RETURN, KEY_FN_06},
    {KEY_FN_03, 0, KEY_LEFT_SHIFT, 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.',
     '/', KEY_RIGHT_SHIFT, '\\', KEY_FN_07},
    {KEY_FN_04, 0, 0, 0, 0, 0, KEY_NUM, KEY_BACKSPACE, ' ', 0, 0, 0, 0, 0, 0,
     KEY_FN_08},
};
uint8_t num_keys[ROWS][COLS] = {
    {'n', KEY_ESC, 0, 0, 0, 0, 0, 0, 0, KEY_KP_7, KEY_KP_8, KEY_KP_9,
     KEY_KP_EQUAL, '(', ')', 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, KEY_KP_4, KEY_KP_5, KEY_KP_6, KEY_KP_PLUS,
     KEY_KP_SLASH, KEY_KP_ENTER, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, KEY_KP_1, KEY_KP_2, KEY_KP_3, KEY_KP_MINUS, 0,
     0, 0},
    {0, 0, 0, 0, 0, 0, KEY_NUM, KEY_BACKSPACE, ' ', KEY_KP_0, 0, KEY_KP_DOT,
     KEY_KP_ASTERISK, 0, 0},
};
uint8_t row_to_pin[ROWS] = {8, 9, 16, 10};
char monitor_str[64] = {0};
uint8_t (*current_layout)[ROWS][COLS];

typedef struct _keyPressed {
  uint8_t row;
  uint8_t col;
} keyPressed;

bool key_num_pressed = false;

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
  current_layout = &keys;
  oled_init(S_DATA, CLK);
  Serial.begin(115200);
  delay(2000);
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
  uint8_t button;
  keyPressed key_pressed[MAX_SIMULTANELUS_KEYS];
  uint8_t key_pressed_counter = 0;
  bool new_key_num_pressed = false;

  digitalWrite(S_DATA, LOW);
  for (int col = 0; col < COLS; col++) {
    if (col == COLS - 1) {
      digitalWrite(S_DATA, HIGH);
    }
    digitalWrite(CLK, HIGH);
    digitalWrite(CLK, LOW);
    for (int row = 0; row < ROWS && key_pressed_counter < MAX_SIMULTANELUS_KEYS;
         row++) {
      button = (*current_layout)[row][col];
      if (digitalRead(row_to_pin[row])) {
        if (button == KEY_NUM) {
          new_key_num_pressed = true;
          continue;
        }
        key_pressed[key_pressed_counter].col = col;
        key_pressed[key_pressed_counter].row = row;
        key_pressed_counter++;
      } else {
        Keyboard.release(button);
      }
    }
  }
  // Change layout and do not send this button
  if (new_key_num_pressed) {
    current_layout = &num_keys;
    digitalWrite(LED, LOW);
    digitalWrite(LOCK_1, HIGH);
    digitalWrite(LOCK_2, HIGH);
  } else {
    digitalWrite(LED, HIGH);
    digitalWrite(LOCK_1, LOW);
    digitalWrite(LOCK_2, LOW);
    current_layout = &keys;
  }
  if (key_num_pressed != new_key_num_pressed) {
    oled_displayBitmap();
    oled_disable();
    flush_registers();
    Keyboard.releaseAll();
  }
  for (uint8_t p = 0; p < key_pressed_counter; p++) {
    uint8_t row = key_pressed[p].row;
    uint8_t col = key_pressed[p].col;
    uint8_t button = (*current_layout)[row][col];
    //    sprintf(monitor_str, "Row: %d, Col: %d, B%d\n", row, col, button);
    //    Serial.write(monitor_str);
    oled_addPixel(8 * col, 4 * row);
    Keyboard.press(button);
  }
  key_num_pressed = new_key_num_pressed;
}
