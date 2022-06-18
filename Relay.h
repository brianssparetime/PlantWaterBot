#pragma once
#include "Pins.h"

class Relay {
    public:
        static void turn_on();
        static void turn_off();
    private:
        static const int pin = RELAY_PIN; 
};