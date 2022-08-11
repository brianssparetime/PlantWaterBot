#pragma once
#include <LiquidCrystal_I2C.h>
#include "LCD_Wrapper.h"
#include "Arduino.h"
#include <assert.h>

// long unsigned LCD_Wrapper::_last_action = 0;
// long unsigned LCD_Wrapper::_specified_ontime = 0;
// bool LCD_Wrapper::_quiet = false;
LiquidCrystal_I2C lcd_actual(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C* LCD_Wrapper::_lcd = &lcd_actual;


void LCD_Wrapper::init() {
    _lcd->init();
}

void LCD_Wrapper::backlight() {
    _lcd->backlight();
}

void LCD_Wrapper::backlightOff() {
    _lcd->noBacklight();
}

void LCD_Wrapper::display(char* line1, char* line2) {
    _lcd->clear();
    #ifdef DEBUG 
      if(line1.length() >= 16) {
        Serial.println("display line too long:  " + line1);
      }
      if(line2.length() >= 16) {
        Serial.println("display line too long:  " + line2);
      }
    #endif
    _lcd->setCursor(0,0); // col, row)
    _lcd->print(line1);
    _lcd->setCursor(0,1); // col, row)
    _lcd->print(line2);
}
void LCD_Wrapper::display(const __FlashStringHelper* line1, const __FlashStringHelper* line2) {
  char sba[17];
  char sbb[17];
  strcpy_P(sba, (const char*) line1);
  strcpy_P(sbb, (const char*) line2);
  display((char *) sba, (char *) sbb);
}
void LCD_Wrapper::display(const __FlashStringHelper* line1, char* line2) {
  char sb[17];
  strcpy_P(sb, (const char*) line1);
  display(line1, sb);
}


void LCD_Wrapper::update() {
}
