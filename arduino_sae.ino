/**
 * @author Alex Nevpryaga
 * @source https://github.com/santaklouse/arduino_sae
 * 
 * The simple activity emulator for Digispark usb =)
 * In loop changes chrome and phpstorm windows (and tabs in it) and preses 'CTRL' key
 * just for fun...
 * 

  You can also change vendor of device 
  path ~/Library/Arduino15/packages/digistump/hardware/avr/1.6.7/libraries/DigisparkKeyboard/usbconfig.h

  Below there are values for Apple Keyboard
  #define USB_CFG_VENDOR_ID 0xac, 0x05
  #define USB_CFG_DEVICE_ID 0x02, 0x02
  #define USB_CFG_VENDOR_NAME     'A','p','p','l','e',' ','I','n','c','.'
  #define USB_CFG_VENDOR_NAME_LEN 10
  #define USB_CFG_DEVICE_NAME     'K','e','y','b','o','a','r','d'
  #define USB_CFG_DEVICE_NAME_LEN 8

 * 
 * for now it works only on mac and linux
 */

#include "DigiKeyboard.h"

const int LED_PIN =  1;

#define LIGHT_ON(x) digitalWrite(x, HIGH)
#define LIGHT_OFF(x) digitalWrite(x, LOW )
#define TOGGLE(x) digitalWrite(x, digitalRead(x) ? LOW : HIGH) //toggle the LED on x pin (HIGH and LOW is the voltage level)

#define TOGGLE_LED() TOOGLE(LED_PIN) 
#define LED_LIGHT_ON() LIGHT_ON(LED_PIN)
#define LED_LIGHT_OFF() LIGHT_OFF(LED_PIN)

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
#define KEY_LEFT_CTRL 0xe0


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
//  return WIN_OS;
  return MAC_OS;
}

void openSearch() {
  delay(random(100, 200));
  switch(detectPlatform()) {
    case MAC_OS:
      DigiKeyboard.sendKeyStroke(KEY_SPACE, MOD_GUI_LEFT); // Super key, open 'search'
      break;
    case LINUX_OS:
    case WIN_OS:
      DigiKeyboard.sendKeyStroke(0, MOD_GUI_LEFT); // Super key, open 'search'
      break;
  }
  
  delay(random(300, 400));
  DigiKeyboard.sendKeyStroke(KEY_A, MOD_CONTROL_LEFT);
  DigiKeyboard.sendKeyStroke(KEY_DELETE); //clear
  delay(random(300, 400));
}

void openByCommand(char* cmd) {
  openSearch();
  delay(random(100, 300));
  DigiKeyboard.print(cmd);
  delay(random(200, 400));
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
  ctrls();
  delay(random(500, 3000));
  changeTab();
  scroll();
}

void openPhpStorm() {
  openByCommand("phpstorm");
  ctrls();
  delay(random(500, 3000));
  changeTab();
  scroll();
}

void openTerminal() {
  //TODO
  openByCommand("iterm");
  blinkRandomly();
  ctrls();
  delay(random(500, 3000));
}

void openDesktop() {
  //TODO
  blinkRandomly();
  ctrls();
//  DigiKeyboard.sendKeyStroke(KEY_D, MOD_GUI_LEFT);
}

void blinkRandomly() {
  LED_LIGHT_ON();
  delay(random(10, 500));
  LED_LIGHT_OFF();
  delay(random(10, 1000));
}

void alts() {
  for (int i = 0; i < 10; i++) {
    LED_LIGHT_ON();
    DigiKeyboard.sendKeyStroke(0, MOD_ALT_LEFT);
    delay(50);
    LED_LIGHT_OFF();
    delay(random(50, 100));
  }
}

void ctrls() {
  for (int i = 0; i < 10; i++) {
    LED_LIGHT_ON();
    //use magic Ctrl's combination in order to avoid side-effects
    DigiKeyboard.sendKeyStroke(KEY_LEFT_CTRL, MOD_CONTROL_LEFT);
    DigiKeyboard.delay(100);
    LED_LIGHT_OFF();
    //random delay between 1s and 10s
    DigiKeyboard.delay(random(1110, 10201));
  }
}

void loop() {
  // this is generally not necessary but with some older systems it seems to
  // prevent missing the first character after a delay:
  DigiKeyboard.sendKeyStroke(0);
  
  ctrls();
  
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
      blinkRandomly();
      delay(random(200, 5000));
      break;
  }

  delay(random(2000, 5000));
}
