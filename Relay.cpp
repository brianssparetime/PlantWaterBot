#pragma once
#include "Relay.h"
#include "Arduino.h"

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



void Relay::activate() {
    turn_on();
    Relay::start_time = millis();
    // TODO: activate a UI state transition ??
}

void Relay::update() {
    long unsigned now = millis();
    if(now - Relay::start_time > amount_to_duration(Relay::amount)) {
        turn_off();
    }
    // TODO: activate a UI state transition ??

}

long unsigned Relay::start_time = 0; 
