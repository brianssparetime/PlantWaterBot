#pragma once
#include "Arduino.h"

// to match PCB from darkroom timer

// LCD uses I2C SDA / SCL on A4 and A5

// rotary encoder
#define RE_BUT_PIN 6
#define RE_A_PIN 7
#define RE_B_PIN 8

//empty sensor, indicator
//#define LOW_SENSOR_PIN 5
//pinmode(LOW_SENSOR_PIN, INPUT);

//buzzer
#define BUZZER_PIN 9



//relay
#define NUM_PUMPS 2
// #define RELAY_PIN_A A0  // relay 1
// #define RELAY_PIN_B A1  // relay 2


namespace Globals {
    static const uint8_t amounts_size = 5;
    static const uint8_t amounts[amounts_size] = {20, 50, 100, 200, 0}; // ml
    //uint8_t maxes at 255

    // static const uint8_t intervals_size = 4;
    // static const uint8_t intervals[intervals_size] = {24, 48, 72, 168}; // hours
    static const uint8_t intervals_size = 5;
    static const uint8_t intervals[intervals_size] = {24, 48, 72, 168, 1}; // hours

    static const uint8_t RELAY_PINS[NUM_PUMPS] = {A0, A1};
    static const float flowrate = 3.6; // ml per sec

}
