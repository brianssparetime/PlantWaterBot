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

        static const int _keep_on_time = 5 * 1000; // ms
        static unsigned long _last_action;
        static unsigned long _specified_ontime;
        static bool _quiet;
        static LiquidCrystal_I2C* _lcd;
};

