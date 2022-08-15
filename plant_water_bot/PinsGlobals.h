#pragma once
#include "Arduino.h"

// to match PCB from darkroom timer

// LCD uses I2C SDA / SCL on A4 and A5

// rotary encoder
#define RE_BUT_PIN 6
#define RE_A_PIN 7
#define RE_B_PIN 8

//empty sensor, indicator
#define LOW_SENSOR_PIN 5
#define LOW_INDICATOR_PIN A1 // relay 2 on the pcb

//buzzer
#define BUZZER_FEATURE
#define BUZZER_PIN 9



//relay
#define NUM_PUMPS 2
// #define RELAY_PIN_A A0  // relay 1
// #define RELAY_PIN_B A1  // relay 2


namespace Globals {
    // TODO: consider moving to progmem, rewriting next_left next_right to work off that
    static const uint8_t amounts_size = 4;
    static const uint8_t amounts[amounts_size] = {20, 50, 100, 200}; // hours
    //uint8_t maxes at 255

    static const uint8_t intervals_size = 4;
    static const uint8_t intervals[intervals_size] = {24, 48, 72, 168}; // hours

    static const uint8_t RELAY_PINS[NUM_PUMPS] = {A0, A1};
    static const float flowrate = 3.6; // ml per sec

}
