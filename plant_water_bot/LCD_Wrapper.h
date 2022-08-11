#pragma once
#include "Arduino.h"
#include <LiquidCrystal_I2C.h>

// purely static

class LCD_Wrapper {
    public:
        static void init();
        static void backlight();
        static void backlightOff();
        static void update();
        static void display(char* line1, char* line2);
        static void display(const __FlashStringHelper* line1, const __FlashStringHelper* line2);
        static void display(const __FlashStringHelper* line1, char* line2);

    private:
        static LiquidCrystal_I2C* _lcd;
};

