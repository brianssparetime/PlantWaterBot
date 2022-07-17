#pragma once
#include "Arduino.h"

// to match PCB from darkroom timer

// LCD uses I2C SDA / SCL on A4 and A5

// rotary encoder
#define RE_BUT_PIN 6
#define RE_A_PIN 7
#define RE_B_PIN 8

//empty sensor, indicator
#define LOW_SENSOR 5
#define LOW_INDICATOR A1 // relay 2 on the pcb


//relay
#define NUM_PUMPS 2
// #define RELAY_PIN_A A0  // relay 1
// #define RELAY_PIN_B A1  // relay 2


namespace Globals {
    // TODO: consider moving to progmem, rewriting next_left next_right to work off that
    static const int amounts[5] = {10, 25, 50, 100, 200}; // hours
    static const int8_t amounts_size = 5;

    static const int intervals[5] = {8, 12, 24, 48, 72}; // hours
    static const int8_t intervals_size = 5;

    static const uint8_t RELAY_PINS[NUM_PUMPS] = {A0, A1};
}
