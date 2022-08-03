#pragma once
#include "PinsGlobals.h"

// purely static

class Relay {
    public:
        static void init();
        static void activate();
        static void update();
        static void set_amount(uint8_t amount, uint8_t relay);
        static uint8_t get_amount(uint8_t relay);
        static unsigned long get_duration(uint8_t relay);
        static void testing(bool t);
        static void turn_on();
        static void turn_off(uint8_t relay);
        static void turn_off();

    private:
        static unsigned long amount_to_duration(uint8_t amount);
        static unsigned long get_max_duration();

        static uint8_t _amount[NUM_PUMPS];
        static unsigned long _start_time;
        static uint8_t _active;
        static bool _testing;

};