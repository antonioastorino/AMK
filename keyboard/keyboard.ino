#include <Keyboard.h>
#define LED 17
#define CLK 21
#define S_DATA 5
#define COLS 16
#define ROWS 4
#define SLEEP_MS 1
#define BUTTON 65
uint8_t keys[ROWS][COLS] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 'u', 'i', 'o', 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 'j', 'k', 'l', 0, 0, KEY_RETURN, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 'm', ',', '.', 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, KEY_F13, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};
uint8_t num_keys[ROWS][COLS] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '=', 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, '4', '5', '6', '+', 0, KEY_RETURN, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, '1', '2', '3', '-', 0, 0, 0},
    {0, 0, 0, 0, 0, 0, KEY_F13, 0, 0, '0', 0, '.', 0, 0, 0, 0},
};
uint8_t row_to_pin[ROWS] = {8, 9, 16, 10};
char monitor_str[64] = {0};

void setup() {
  Serial.begin(115200);
  keys[0][0] = KEY_CAPS_LOCK;
  for (int i = 0; i < ROWS; i++) {
    pinMode(row_to_pin[i], INPUT);
    digitalWrite(row_to_pin[i],
                 LOW); // should disable internal pull-up resistors
  }
  pinMode(LED, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(S_DATA, OUTPUT);
  Keyboard.begin();
}

void loop() {
  uint8_t ledStatus = 0;
  uint8_t ledStatus1 = 0;
  uint8_t(*current_layout)[ROWS][COLS] = &keys;
  uint8_t button;
  delay(SLEEP_MS);
  for (int col = 0; col < COLS; col++) {
    if (col == COLS - 1) {
      digitalWrite(S_DATA, HIGH);
    }
    digitalWrite(CLK, HIGH);
    delay(SLEEP_MS);
    digitalWrite(CLK, LOW);
    delay(SLEEP_MS);
    digitalWrite(S_DATA, LOW);
    for (int row = 0; row < ROWS; row++) {
      button = (*current_layout)[row][col];
      if (digitalRead(row_to_pin[row])) {
        if (button == KEY_F13) {
          // Change layout and do not send this button
          current_layout = &num_keys;
          continue;
        } else {
          current_layout = &keys;
        }
        Keyboard.press(button);
        sprintf(monitor_str, "Row: %d, Col: %d, S%d-%d\n", row, col, ledStatus,
                ledStatus1);
        Serial.write(monitor_str);

        if (ledStatus) {
          digitalWrite(LED, HIGH);
        } else {
          digitalWrite(LED, LOW);
        }
      } else {
        Keyboard.release(button);
      }
    }
  }
}
