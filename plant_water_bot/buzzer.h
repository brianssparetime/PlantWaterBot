#pragma once
#include "Arduino.h"



class Buzzer {
    public:
        static void init(int pin);
        static void buzz();
        static void buzz(uint16_t duration_ms);
        static void cancel();
        static void update();
    private:
        static uint8_t _pin;
        static uint16_t _stop_time;
        static const uint16_t _default_duration = 500;

};