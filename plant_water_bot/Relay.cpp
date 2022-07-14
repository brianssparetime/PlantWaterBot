#pragma once
#include "Relay.h"
#include "Arduino.h"
#include "LCD_Wrapper.h"
#include "RHTimer.h"
#include "Machine.h"
#include "PinsGlobals.h"

#define DEBUG

int Relay::_active = 0;
int Relay::_amount[NUM_PUMPS] = {10,10};
unsigned long Relay::_start_time = 0UL; 
// for(int i=0; i < NUM_PUMPS; i++) {
//     int Relay::amount[i] = Globals::amounts[0];
// }
void Relay::init() {
    for(int i=0; i < NUM_PUMPS; i++) {
        _amount[i] = Globals::amounts[0];
        pinMode(Globals::RELAY_PINS[i], OUTPUT); // low active
    }
}

void Relay::turn_on() {
    #ifdef DEBUG
      Serial.println("relays on");
    #endif
    for(int i = 0; i < NUM_PUMPS; i++) {
        digitalWrite(Globals::RELAY_PINS[i], LOW);
        _active++;
    }
}

// TODO:  fix so that only last relay restarts timer
void Relay::turn_off(int relay) {
    #ifdef DEBUG
      Serial.println("relay " + String(relay + 1) + " off");
    #endif
    digitalWrite(Globals::RELAY_PINS[relay], HIGH);
    _active--;
}

unsigned long Relay::amount_to_duration(int amount) {
    const int flowrate = 3; // ml per sec
    // (ml / (ml/s) ) * ms/s
    return ((unsigned long) amount * 1000UL / (unsigned long) flowrate);
}

void Relay::set_amount(int amt, int relay) {
    _amount[relay] = amt;
}

int Relay::get_amount(int relay) {
    return _amount[relay];
}

unsigned long Relay::get_duration(int relay) {
    return amount_to_duration(_amount[relay]);

}

void Relay::activate() {
    #ifdef DEBUG
      Serial.println("relay activated");
    #endif
    turn_on();
    _start_time = millis();
    Machine::changeState(static_cast<UI_State *>(new UI_Watering()));
    // Note:  Do this after state transition to avoid the backlight call in changeState overriding this...
    LCD_Wrapper::backlight();
}

void Relay::update() {
    unsigned long now = millis();
    bool finished = false;
    for(int i=0; i < NUM_PUMPS; i++) {
        if(_active && (now > _start_time + amount_to_duration(_amount[i]))) {
            #ifdef DEBUG
                Serial.println("relay " + String(i+1) + " update - deactivating...");
            #endif
            turn_off(i);
        }
        
    }
    if(_active == 0) {
        RHTimer::start();
        Serial.println("restarting timer...");
        Machine::changeState(static_cast<UI_State *>(new UI_Interval()));
    }
}

