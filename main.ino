#include "Keyboard.h"

// PIN numbers (CONSTANTS)
const int TLPIN = 12; // top row: left, center right
const int TCPIN = 11;
const int TRPIN = 10;
const int MLPIN = 9; // middle row: left, center, right
const int MCPIN = 8;
const int MRPIN = 7;
const int BLPIN = 6; // bottom row: left, right
const int BRPIN = 5;
const int BNPIN = 4;// rotary encoder: button
const int RPIN = 2; // rotary encoder: right
const int LPIN = 3; // rotary encoder: left
const int FPIN = A0; // foot switch pin (digital)
const int APIN = A1; // potentiometer pin (analog)
const int JPIN = A3; // jumper pin (deactivates all functions if pulled down)

// pins to monitor
const int MPINS[] = {TLPIN, TCPIN, TRPIN, MLPIN, MCPIN, MRPIN, BLPIN, BRPIN, BNPIN, RPIN, FPIN};
int last_state[11]; // last pin states
int decay[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // current decay times per pin

// other digital pins
const int OPINS[] = {LPIN, JPIN};

// other variables and constants
const int DLY = 0; // extra delay per run, in ms (refresh rate)
const int DCY = 5; // decay per key, in runs (min. DCY*DLY milliseconds)
int analog_val = 0;
double analog_ratio = 0;


// *********************************
// MAIN FUNCTION, WITH CONFIGURATION
// *********************************

void keyEvent(int n, bool pressed) {
  // handles key event at n-th monitored pin
  // 'pressed' == false if released

  // *** TOP LEFT KEY ***
  if (n == 0) {
    if (pressed) {
      Keyboard.press('q');
    }
    else {
      Keyboard.release('q');
    }
  }

  // *** TOP CENTER KEY ***
  if (n == 1) {
    if (pressed) {
      Keyboard.press('w');
    }
    else {
      Keyboard.release('w');
    }
  }

  // *** TOP RIGHT KEY ***
  if (n == 2) {
    if (pressed) {
      Keyboard.press('e');
    }
    else {
      Keyboard.release('e');
    }
  }

  // *** MIDDLE LEFT KEY ***
  if (n == 3) {
    if (pressed) {
      Keyboard.press('a');
    }
    else {
      Keyboard.release('a');
    }
  }

  // *** MIDDLE CENTER KEY ***
  if (n == 4) {
    if (pressed) {
      Keyboard.press('s');
    }
    else {
      Keyboard.release('s');
    }
  }

  // *** MIDDLE RIGHT KEY ***
  if (n == 5) {
    if (pressed) {
      Keyboard.press('d');
    }
    else {
      Keyboard.release('d');
    }
  }
  
  // *** BOTTOM LEFT KEY ***
  if (n == 6) {
    if (pressed) {
      Keyboard.press(KEY_LEFT_ALT);
    }
    else {
      Keyboard.release(KEY_LEFT_ALT);
    }
  }

  // *** BOTTOM RIGHT KEY ***
  if (n == 7) {
    if (pressed) {
      Keyboard.press(KEY_LEFT_CTRL);
    }
    else {
      Keyboard.release(KEY_LEFT_CTRL);
    }
  }

  // *** ROTARY ENCODER BUTTON ***
  if (n == 8) {
    if (pressed) {
      Keyboard.press(KEY_RETURN);
    }
    else {
      Keyboard.release(KEY_RETURN);
    }
  }

  // *** MONITOR ROTARY ENCODER ***
  if (n == 9) {
    if (!pressed) { // right pin is high...
      if (digitalRead(LPIN)) { // ... and left one not yet: clockwise
        Keyboard.write(']'); // plus on German keyboard
      }
      else { // ... and left one already high: counter-clockwise
        Keyboard.write('/'); // minus on German keyboard
      }
    }
  }

  // *** FOOT SWITCH ***
  if (n == 10) {
    if (!pressed) { // foot switch is inverted!
      Keyboard.press((char) 0x20); // space key
    }
    else {
      Keyboard.release((char) 0x20);
    }
  }

  
  // important: save state
  last_state[n] = !pressed; // pressed==false is HIGH
  
}

// ********************
// END OF MAIN FUNCTION




void setup() {
  // initialize monitored pins
  for (int i = 0; i < 11; i++) {
    pinMode(MPINS[i], INPUT_PULLUP);
    last_state[i] = digitalRead(MPINS[i]);
  }

  // initialize other pins and analog in
  for (int i = 0; i < 2; i++) {
    pinMode(OPINS[i], INPUT_PULLUP);
  }
  analog_val = analogRead(APIN);
  analog_ratio = analog_val / 1024;
  
  Serial.begin(9600); // for debugging only
  Keyboard.begin();
}

void loop() {
  // monitor pins
  for (int i = 0; i < 11; i++) {
    if (decay[i] > 0) {
      decay[i] -= 1;
    }
    else if (digitalRead(MPINS[i]) != last_state[i]) {
      keyEvent(i, last_state[i]); // second argument: pressed (true) or released?
      decay[i] = DCY;
    }
  }

  // read analog value
  analog_val = analogRead(APIN);
  analog_ratio = analog_val / 1024;
  //Serial.println(analog_ratio);

  // extra delay per run
  delay(DLY);
}
