#include "TrinketHidCombo.h"

/**
 * @author Alex Nevpryaga
 * @source https://github.com/santaklouse/arduino_sae
 * 
 * The simple activity emulator for Digispark usb =)
 * In loop changes chrome and phpstorm windows (and tabs in it) and preses 'CTRL' key
 * just for fun...
 * 

  ...using TrinketHidCombo lib from https://github.com/adafruit/Adafruit-Trinket-USB

  You can change vendor of device here:
  <Arduino path>/libraries/TrinketHidCombo/usbconfig.h

  Below there are values for Apple Keyboard
  #define USB_CFG_VENDOR_ID 0xac, 0x05
  #define USB_CFG_DEVICE_ID 0x02, 0x02
  
  #define USB_CFG_VENDOR_NAME     'A','p','p','l','e',' ','I','n','c','.'
  #define USB_CFG_VENDOR_NAME_LEN 10
  #define USB_CFG_DEVICE_NAME     'A','p','p','l','e',' ','K','e','y','b','o','a','r','d'
  #define USB_CFG_DEVICE_NAME_LEN 14

 * 
 * for now it works only on mac and linux
 */


#define LED_PIN 1

#define LIGHT_ON(x) digitalWrite(x, HIGH)
#define LIGHT_OFF(x) digitalWrite(x, LOW )
#define TOGGLE(x) digitalWrite(x, digitalRead(x) ? LOW : HIGH) //toggle the LED on x pin (HIGH and LOW is the voltage level)

#define TOGGLE_LED() TOOGLE(LED_PIN) 
#define LED_LIGHT_ON() LIGHT_ON(LED_PIN)
#define LED_LIGHT_OFF() LIGHT_OFF(LED_PIN)

#define OPEN_CHROME 0
#define OPEN_PHPSTORM 1
#define OPEN_SLACK 2
#define OPEN_DESKTOP 3
#define OPEN_TERMINAL 4

#define MODE_LIGHT 0
#define MODE_MEDIUM 1
#define MODE_FULL 2

#define DEFAULT_MODE MODE_LIGHT

#define PIN_LIGHT_MODE_NUM 0
#define PIN_FULL_MODE_NUM 2

#define MAC_OS 0
#define WIN_OS 1
#define LINUX_OS 2

#define DEFAULT_PLATFORM MAC_OS
//#define DEFAULT_PLATFORM LINUX_OS

// "phpstorm" for example...
#define IDE_COMMAND "phpstorm"
/*
 * more codes can be found here: https://raw.githubusercontent.com/adafruit/Adafruit-Trinket-USB/master/TrinketHidCombo/TrinketHidCombo.h
 * additional key codes: https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf
 */

// keycodes
#define KEY_A 0x04
#define KEY_ENTER 0x28
#define KEY_DELETE 0x4C
#define KEY_SPACE 0x2C
#define KEY_TAB 0x2B
#define KEY_PAGE_UP 0x4B
#define KEY_PAGE_DOWN 0x4E
#define KEY_LEFT_CTRL 0xE0
#define KEY_LEFT_ALT 0xE2
#define KEY_LEFT_GUI 0xE3
#define KEY_RIGHT_CONTROL 0xE4

// modifier keys
#define MOD_GUI_LEFT 0x08
#define MOD_CONTROL_LEFT 0x01
#define MOD_ALT_LEFT 0x04

void blink(byte i = 1) {
  for (byte j = 0; j < i; j++) 
  {
    LED_LIGHT_ON();
    delay(300);
    LED_LIGHT_OFF();
    delay(100);
  }
}

void pressKey(uint8_t modifiers, uint8_t keycode1) {
  TrinketHidCombo.pressKey(modifiers, keycode1);
  TrinketHidCombo.pressKey(0, 0);
  blink();
}

void pressKey(uint8_t keycode1) {
  pressKey(0, keycode1);
}

void mouseMove(signed char x, signed char y, uint8_t buttonMask) {
  TrinketHidCombo.mouseMove(x, y, buttonMask);
  TrinketHidCombo.mouseMove(0, 0, 0);
}

void mouseMove(signed char x, signed char y) {
  mouseMove(x, y, 0);
}

void mouseMove(uint8_t buttonMask) {
  mouseMove(0, 0, buttonMask);
}

void mouseLeftClick() {
  mouseMove(MOUSEBTN_LEFT_MASK);
}

void mouseMiddleClick() {
  mouseMove(MOUSEBTN_MIDDLE_MASK);
}

void moveMouseToLeftBottom() {
  for (byte i = 0; i < 20; i++) {
    mouseMove(-127, 127);
  }
}

void mouseMoveEvent() {
  mouseMove(10, 10);
  mouseMove(-10, 10);
  mouseMove(10, -10);
  mouseMove(-10, -10);
}

void setPinToPullupMode(byte pinNumber) {
  pinMode ((int)pinNumber, INPUT_PULLUP);
  digitalWrite((int)pinNumber, HIGH); // enable pullup
}

bool isPinShorted(byte pin) {
  return digitalRead(pin) == LOW;
}

byte mode = -1;
byte detectMode() {
  LED_LIGHT_OFF();

  byte newMode;

  if (isPinShorted(PIN_LIGHT_MODE_NUM)) {
    newMode = MODE_MEDIUM;
  } else if (isPinShorted(PIN_FULL_MODE_NUM)) {
    newMode = MODE_FULL;
  } else if (mode != DEFAULT_MODE) {
    newMode = MODE_LIGHT;
  }

  LED_LIGHT_ON();

  bool changed = mode != newMode;
  if (changed) {
    mode = newMode;
  }

  return mode;
}

byte detectPlatform() {
  /*
   * Here we should read pin jumper to detect
   * to what os it was switched
   */
  return DEFAULT_PLATFORM;
//  return WIN_OS;
//  return MAC_OS;
}

void openSearch() {
  delay(random(100, 200));
  switch(detectPlatform()) {
    case MAC_OS:
      pressKey(MOD_GUI_LEFT, KEY_SPACE); // Super key, open 'search'
      break;
    case LINUX_OS:
    case WIN_OS:
      pressKey(KEY_LEFT_GUI); // Super key, open 'search'
      break;
  }
  
  delay(random(300, 400));
  pressKey(MOD_CONTROL_LEFT, KEY_A);
  pressKey(KEY_DELETE); //clear
  delay(random(300, 400));
}

void openByCommand(char* cmd) {
  openSearch();
  delay(random(400, 1000));
  TrinketHidCombo.print(cmd);
  delay(random(200, 400));
  pressKey(KEY_ENTER);
}

void changeTab() {
  pressKey(MOD_CONTROL_LEFT, KEY_TAB);
}

void scroll() {
  if (random(0,1)) {
    pressKey(KEY_PAGE_DOWN);
  } else {
    pressKey(KEY_PAGE_UP);
  }
  delay(random(1000, 5000));
}

void openChrome() {
  openByCommand("chrome");
  ctrls();
  delay(random(500, 3000));
  changeTab();
  moveMouseToLeftBottom();
  mouseLeftClick();
  scroll();
}

void openIDE() {
  if (sizeof(IDE_COMMAND) == 0) {
    return;
  }
  openByCommand(IDE_COMMAND);
  scroll();
  delay(random(500, 3000));
  changeTab();
  scroll();
}

void openSlack() {
  //TODO
  openByCommand("slack");
  pressKey(KEY_PAGE_UP);
  blinkRandomly();
  ctrls();
  delay(random(500, 3000));
}

void blinkRandomly() {
  LED_LIGHT_ON();
  delay(random(10, 500));
  LED_LIGHT_OFF();
  delay(random(10, 1000));
}

void ctrls() {
  int amount = random(1, 10);
  for (byte i = 0; i < amount; i++) {
    LED_LIGHT_ON();
    //use magic Ctrl's combination in order to avoid side-effects
    pressKey(MOD_CONTROL_LEFT, KEY_LEFT_CTRL);
    delay(100);
    mouseMiddleClick();
    LED_LIGHT_OFF();
    //random delay between 1s and 10s
    delay(random(1110, 10201));
  }
}

void runMediumCycle(bool delayed = false) {
  //turn light on
  LED_LIGHT_ON();

  //use magic Ctrl's combination (MOD_CTRL+CTRL) in order to avoid side-effects
  pressKey(MOD_CONTROL_LEFT, KEY_LEFT_CTRL);

  for (byte i =0; i < random(0, 4); i++) {
    mouseMoveEvent();
  }
  
  delay(150);

  //turn light off
  LED_LIGHT_OFF();

  int from = 7110;
  int to = 20201;
  if (delayed) {
    from *= 5;
    to *= 5;
  }

  //random delay between 7s and 20s
  delay(random(from, to));
}

void runFullCycle() {
  moveMouseToLeftBottom();
  mouseMiddleClick();
  
  // this is generally not necessary but with some older systems it seems to
  // prevent missing the first character after a delay:
  TrinketHidCombo.pressKey(0, 0);
  
  ctrls();

  switch (random(0, 5)) {
    case OPEN_CHROME:
      openChrome();
      break;
    case OPEN_PHPSTORM:
      openIDE();
      break;
    case OPEN_SLACK:
      openSlack();
      break;
    case OPEN_TERMINAL:
      // TODO
      //openTerminal();
    case OPEN_DESKTOP:
      // TODO
      //openDesktop();

      // due to above TODO just run light cycle
      runMediumCycle();
      
      break;
    default:
      blinkRandomly();
      delay(random(200, 5000));
      break;
  }
  delay(random(2000, 5000));
}

void runLightCycle() {
  runMediumCycle(true);
}

byte workMode = 0;
void setup() {
  setPinToPullupMode(PIN_LIGHT_MODE_NUM);
  pinMode(LED_PIN, OUTPUT);
  setPinToPullupMode(PIN_FULL_MODE_NUM);

  TrinketHidCombo.begin(); // start the USB device engine and enumerate

  randomSeed(analogRead(0));

  workMode = detectMode();
  if (workMode == MODE_FULL) {
    blink(3);
  } else if (workMode == MODE_MEDIUM) {
    blink(2);
  } else {
    blink(1);
  }
    
}

void runMode(byte mode) {
  switch(mode) {
    case MODE_LIGHT:
        /*
        runLightCycle();
        break;
        */
    case MODE_MEDIUM:
        runMediumCycle();
        break;
    case MODE_FULL:
        runFullCycle();
        break;
    default:
        runMediumCycle();
        break;
  }
}

void loop() {
  runMode(detectMode());
  TrinketHidCombo.poll(); // check if USB needs anything done
}
