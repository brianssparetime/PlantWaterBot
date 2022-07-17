#pragma once
#include "PinsGlobals.h"

// purely static

class Relay {
    public:
        static void init();
        static void activate();
        static void update();
        static void set_amount(int amount, int relay);
        static int get_amount(int relay);
        static unsigned long get_duration(int relay);
        static void testing(bool t);
        static void turn_on();
        static void turn_off(int relay);
        static void turn_off();

    private:
        static unsigned long amount_to_duration(int amount);
        static unsigned long get_max_duration();

        static int _amount[NUM_PUMPS];
        static unsigned long _start_time;
        static uint8_t _active;
        static bool _testing;

};