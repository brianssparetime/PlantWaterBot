#pragma once
#include "Arduino.h"

// purely static

class RHTimer {
    public:
        static void start();
        static void start(uint8_t interval);
        static uint8_t get_d_remaining();
        static uint8_t get_h_remaining();
        static uint8_t get_m_remaining();
        static uint8_t get_s_remaining();
        static void update();
        static uint8_t get_current_interval();

    private:

        static void alarm();

        static unsigned long _next_min_millis;
        static uint8_t _minutes_elapsed;
        static uint8_t _hours_elapsed;
        static uint8_t _cur_interval;
};