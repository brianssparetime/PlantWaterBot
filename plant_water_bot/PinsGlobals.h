#pragma once
#include "Arduino.h"
// to match PCB from darkroom timer
#define RELAY_PIN A0 

#define RE_BUT_PIN 6
#define RE_A_PIN 7
#define RE_B_PIN 8

// LCD uses I2C SDA / SCL

namespace Globals {
    const int amounts[5] = {10, 25, 50, 100, 200}; // hours
    static const int amounts_size = 5;

    const int intervals[5] = {8, 12, 24, 48, 72}; // hours
    static const int intervals_size = 5;
}
