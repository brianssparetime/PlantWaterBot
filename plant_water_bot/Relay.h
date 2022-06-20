#pragma once
#include "Pins.h"

// purely static

class Relay {
    public:
        static void activate();
        static void update();
        static void set_amount(int amount);
        static int get_amount();
        static unsigned long get_duration();

        static const int amounts_size = 5;
        static const int amounts[5];

    private:
        static void turn_on();
        static void turn_off();
        static long unsigned amount_to_duration(int amount);

        static const int pin = RELAY_PIN; 
        static int amount;
        static long unsigned start_time;

};