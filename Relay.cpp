#pragma once
#include "Relay.h"
#include "Arduino.h"
#include "LCD_Wrapper.h"
#include "RHTimer.h"
#include "Machine.h"

void Relay::turn_on() {
    digitalWrite(Relay::pin, LOW);
}

void Relay::turn_off() {
    digitalWrite(Relay::pin, HIGH);
    RHTimer::start();
}

long unsigned Relay::amount_to_duration(int amount) {
    const int flowrate = 3; // ml per sec
    // (ml / (ml/s) ) * ms/s
    return (amount / flowrate) * 1000;
}


int Relay::amount = 100; // ASSERT amount should be in array amounts
const int Relay::amounts[5] = {10, 25, 50, 100, 200}; // hours
// assert size of this list set in .h file

void Relay::set_amount(int amount) {
    Relay::amount = amount;
}

int Relay::get_amount() {
    return Relay::amount;
}

unsigned long Relay::get_duration() {
    return amount_to_duration(Relay::amount);

}


void Relay::activate() {
    turn_on();
    Relay::start_time = millis();
    Machine::changeState(static_cast<UI_State *>(new UI_Watering()));
    // Note:  Do this after state transition to avoid the backlight call in changeState overriding this...
    long unsigned ms = amount_to_duration(Relay::amount);
    LCD_Wrapper::backlight(ms);
}

void Relay::update() {
    long unsigned now = millis();
    if(now - Relay::start_time > amount_to_duration(Relay::amount)) {
        turn_off();
        Machine::changeState(static_cast<UI_State *>(new UI_Interval()));
    }
}

long unsigned Relay::start_time = 0; 
