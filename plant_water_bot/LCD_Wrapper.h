#pragma once
#include "Arduino.h"
#include <LiquidCrystal_I2C.h>

// purely static

class LCD_Wrapper {
    public:
        static void init();
        static void backlight();
        static void backlight(long unsigned ms);
        static void backlightOff();
        static void update();
        static void display(String line1, String line2);

    private:
        static LiquidCrystal_I2C* _lcd;
};

