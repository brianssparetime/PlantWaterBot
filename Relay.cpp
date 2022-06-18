#pragma once
#include "Relay.h"
#include "Arduino.h"
#include "LCD_Wrapper.h"

void Relay::turn_on() {
    digitalWrite(Relay::pin, LOW);
}

void Relay::turn_off() {
    digitalWrite(Relay::pin, HIGH);
}

long unsigned Relay::amount_to_duration(int amount) {
    const int flowrate = 3; // ml per sec
    // (ml / (ml/s) ) * ms/s
    return (amount / flowrate) * 1000;
}


int Relay::amount = 100; // TODO:  make sure this gets set elsewhere after testing
void Relay::set_amount(int amount) {
    Relay::amount = amount;
}
unsigned long Relay::get_duration() {
    return amount_to_duration(Relay::amount);

}


void Relay::activate() {
    turn_on();
    Relay::start_time = millis();
    // TODO: activate a UI state transition ??
    long unsigned ms = amount_to_duration(Relay::amount);
    LCD_Wrapper::backlight(ms);
    // Note:  Do this after state transition to avoid the backlight call in changeState overriding this...
}

void Relay::update() {
    long unsigned now = millis();
    if(now - Relay::start_time > amount_to_duration(Relay::amount)) {
        turn_off();
    }
    // TODO: activate a UI state transition ??

}

long unsigned Relay::start_time = 0; 
