#pragma once
#include "Relay.h"
#include "Arduino.h"
#include "LCD_Wrapper.h"
#include "RHTimer.h"
#include "Machine.h"
#include "PinsGlobals.h"



uint8_t Relay::_active = 0;
bool Relay::_testing = false;
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
    for(uint8_t i = 0; i < NUM_PUMPS; i++) {
        digitalWrite(Globals::RELAY_PINS[i], LOW);
        _active++;
    }
}

void Relay::turn_off() {
    for(uint8_t i = 0; i < NUM_PUMPS; i++) {
        turn_off(i);
    }
}

void Relay::turn_off(uint8_t relay) {
    #ifdef DEBUG
      Serial.println("relay " + String(relay + 1) + " off");
    #endif
    digitalWrite(Globals::RELAY_PINS[relay], HIGH);
    _active--;
}

unsigned long Relay::amount_to_duration(uint8_t amount) {
    // (ml / (ml/s) ) * ms/s
    return (unsigned long) ((unsigned long) amount * 1000UL / Globals::flowrate);
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
    bool finished = false;
    for(uint8_t i=0; i < NUM_PUMPS; i++) {
        if(_active && (now > _start_time + amount_to_duration(_amount[i]))) {
            #ifdef DEBUG
                Serial.println("relay " + String(i+1) + " update - deactivating...");
            #endif
            turn_off(i);
            if(_active == 0) {
                RHTimer::start();
                #ifdef DEBUG
                    Serial.println("restarting timer...");
                #endif
                Machine::changeState(static_cast<UI_State *>(new UI_Interval()));
            }
        }
    }
}

