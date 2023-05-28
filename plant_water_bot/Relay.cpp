#pragma once
#include "Relay.h"
#include "Arduino.h"
#include "LCD_Wrapper.h"
#include "RHTimer.h"
#include "Machine.h"
#include "PinsGlobals.h"



uint8_t Relay::_active = 0;
bool Relay::_testing = false; // used by UI_States.cpp to prevent update from turning off...
uint8_t Relay::_amount[NUM_PUMPS] = {10,10};
unsigned long Relay::_start_time = 0UL; 

void Relay::init() {
    for(uint8_t i=0; i < NUM_PUMPS; i++) {
        //_amount[i] = Globals::amounts[0];
        _amount[i] = Globals::amounts[0];
        pinMode(Globals::RELAY_PINS[i], OUTPUT); // low active
        digitalWrite(Globals::RELAY_PINS[i], HIGH);
    }
}

void Relay::testing(bool t) {
    _testing = t;
}

void Relay::turn_on() {
    #ifdef DEBUG
      Serial.println("relays on");
    #endif
    _active = 0;
    for(uint8_t i = 0; i < NUM_PUMPS; i++) {
        if (_amount[i] >= 0) {
            digitalWrite(Globals::RELAY_PINS[i], LOW);
            _active++;
        }
    }
}

void Relay::turn_off_all() {
    for(uint8_t i = 0; i < NUM_PUMPS; i++) {
        turn_off(i);
    }
    _active = 0;
    //_testing = false; -- do we need this?
}

void Relay::turn_off(uint8_t relay) {
    #ifdef DEBUG
      Serial.println("relay " + String(relay + 1) + " off");
    #endif
    digitalWrite(Globals::RELAY_PINS[relay], HIGH);
    if (_amount[relay] != 0) {
        _active > 0 ? _active-- : 0;
    }
}

unsigned long Relay::amount_to_duration(uint8_t amount) {
    // (ml / (ml/s) ) * ms/s
    return (unsigned long) (((unsigned long) amount * 1000UL) / Globals::flowrate);
}

void Relay::set_amount(uint8_t amt, uint8_t relay) {
    _amount[relay] = amt;
}

uint8_t Relay::get_amount(uint8_t relay) {
    return _amount[relay];
}

unsigned long Relay::get_duration(uint8_t relay) {
    return amount_to_duration(_amount[relay]);

}

void Relay::activate() {
    #ifdef DEBUG
      Serial.println("relay activated");
    #endif
    turn_on();
    _start_time = millis();
    // Note:  Do this after state transition to avoid the backlight call in changeState overriding this...
    LCD_Wrapper::backlight();
}

void Relay::update() {
    if(_testing) {
        return;
    }
    unsigned long now = millis();
    for(uint8_t i=0; i < NUM_PUMPS; i++) {
        if (_amount[i] == 0) {
            continue;
        }
        if((_active > 0) && (now > _start_time + amount_to_duration(_amount[i]))) {
            #ifdef DEBUG
                Serial.println("relay " + String(i+1) + " update - deactivating...");
            #endif
            turn_off(i);
            if(_active == 0) {
                turn_off_all();
                RHTimer::start();
                #ifdef DEBUG
                    Serial.println("restarting timer...");
                #endif
                Machine::changeState(static_cast<UI_State *>(new UI_Interval()));
            }
        }
    }
}

