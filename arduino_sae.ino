/*
 * The simple activity emulator for Digispark usb =)
 * In loop changes chrome and phpstorm windows (and tabs in it) and preses 'ALT' key
 * just for fun...
 * 
 * for now it works only on linux
 */

#include "DigiKeyboard.h"

const int LED_PIN =  1;

const int OPEN_CHROME = 0;
const int OPEN_PHPSTORM = 1;
const int OPEN_TERMINAL = 2;
const int OPEN_DESKTOP = 3;


#define MAC_OS 0
#define WIN_OS 1
#define LINUX_OS 2

#define KEY_DELETE 42
#define KEY_TAB 43
#define KEY_PAGE_UP 0x4B
#define KEY_PAGE_DOWN 0x4E


/*
 * existing key codes: https://github.com/digistump/DigisparkArduinoIntegration/blob/master/libraries/DigisparkKeyboard/DigiKeyboard.h
 * additional key codes: https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf
 */

void setup() {
  // LED on.
  pinMode(LED_PIN, OUTPUT);
  randomSeed(analogRead(0));
}

int detectPlatform() {
  /*
   * Here we should read pin jumper to detect
   * to what os it was switched
   */
//  return LINUX_OS;
  return MAC_OS;
}

void openSearch() {
  switch(detectPlatform()) {
    case MAC_OS:
      DigiKeyboard.sendKeyStroke(KEY_SPACE, MOD_GUI_LEFT); // Super key, open 'search'
      break;
    case LINUX_OS:
    case WIN_OS:
      DigiKeyboard.sendKeyStroke(0, MOD_GUI_LEFT); // Super key, open 'search'
      break;
  }
  
  delay(300);
  DigiKeyboard.sendKeyStroke(KEY_A, MOD_CONTROL_LEFT);
  DigiKeyboard.sendKeyStroke(KEY_DELETE); //clear
  delay(300);
}

void openByCommand(char* cmd) {
  openSearch();
  DigiKeyboard.print(cmd);
  delay(200);
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
}

void changeTab() {
  DigiKeyboard.sendKeyStroke(KEY_TAB, MOD_CONTROL_LEFT);
}

void scroll() {
  if (random(0,1)) {
    DigiKeyboard.sendKeyStroke(KEY_PAGE_DOWN);
  } else {
    DigiKeyboard.sendKeyStroke(KEY_PAGE_UP);
  }
  delay(random(1000, 5000));
}

void openChrome() {
  openByCommand("chrome");
  alts();
  delay(random(500, 3000));
  changeTab();
  scroll();
}

void openPhpStorm() {
  openByCommand("phpstorm");
  alts();
  delay(random(500, 3000));
  changeTab();
  scroll();
}

void openTerminal() {
//  openByCommand("terminal");
  alts();
  delay(random(500, 3000));
}

void openDesktop() {
  alts();
  DigiKeyboard.sendKeyStroke(KEY_D, MOD_GUI_LEFT);
}

void blinkRandomly() {
  digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(random(10, 500));
  digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
  delay(random(10, 1000));
}

void alts() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(LED_PIN, HIGH);
    DigiKeyboard.sendKeyStroke(0, MOD_ALT_LEFT);
    delay(50);
    digitalWrite(LED_PIN, LOW);
    delay(random(50, 100));
  }
}

void loop() {
  // this is generally not necessary but with some older systems it seems to
  // prevent missing the first character after a delay:
  DigiKeyboard.sendKeyStroke(0);
  
  alts();
  
  switch (random(0, 4)) {
    case OPEN_CHROME:
      openChrome();
      break;
    case OPEN_PHPSTORM:
      openPhpStorm();
      break;
    case OPEN_TERMINAL:
      openTerminal();
      break;
    case OPEN_DESKTOP:
      openDesktop();
      break;
    default:
      delay(random(200, 5000));
      break;
  }

  delay(random(2000, 5000));
}
