#pragma once
#include "Arduino.h"
#include <LiquidCrystal_I2C.h>

// purely static

class LCD_Wrapper {
    public:
    
        static void init();
        static void backlight();
        static void backlight(long unsigned ms);
        static void update();
        static void display(String line1, String line2);

    private:

        static const int keep_on_time = 5 * 1000; // ms
        static unsigned long last_action;
        static unsigned long specified_ontime;
        static bool quiet;
        static LiquidCrystal_I2C* lcd;
};

