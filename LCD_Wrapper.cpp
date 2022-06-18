#pragma once
#include <LiquidCrystal_I2C.h>
#include "LCD_Wrapper.h"
#include "Arduino.h"

long unsigned LCD_Wrapper::last_action = 0;
bool LCD_Wrapper::quiet = false;
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C* LCD_Wrapper::lcd = lcd;
//LiquidCrystal_I2C* LCD_Wrapper::lcd = LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
//LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display



void LCD_Wrapper::init() {
    lcd->init();
     // Print a message to the LCD.
    lcd->backlight();
    lcd->setCursor(0,0); // col, row)
    lcd->print("  PLANT BOT");
}

void LCD_Wrapper::action() {
    last_action = millis();
    lcd->backlight();
    quiet = false;
}

void LCD_Wrapper::display(String line1, String line2) {
    lcd->clear();
    lcd->setCursor(0,0); // col, row)
    lcd->print(line1);
    lcd->print(line2);
}


void LCD_Wrapper::update() {
    if (quiet) {
        return;
    }
    long unsigned now = millis();
    if (now > last_action + keep_on_time) {
        lcd->noBacklight();
        quiet = true;
    }
}
