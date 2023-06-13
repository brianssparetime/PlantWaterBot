#pragma once
#include "Relay.h"
#include "Arduino.h"
#include "LCD_Wrapper.h"
#include "RHTimer.h"
#include "Machine.h"
#include "PinsGlobals.h"
#include "buzzer.h"



uint8_t Relay::_active = 0;
bool Relay::_testing = false; // used by UI_States.cpp to prevent update from turning off...
uint8_t Relay::_amount[NUM_PUMPS] = {10,10};
unsigned long Relay::_start_time = 0UL; 

void Relay::init() {
    _active = 0;
    for(uint8_t i=0; i < NUM_PUMPS; i++) {
        //_amount[i] = Globals::amounts[0];
        _amount[i] = Globals::amounts[0];
        pinMode(Globals::RELAY_PINS[i], OUTPUT); // low active
        digitalWrite(Globals::RELAY_PINS[i], HIGH);
    }
    // TESTING TODO REMOVE
    _amount[1] = 100;
}

void Relay::testing(bool t_mode) {
    _testing = t_mode;
}

unsigned long Relay::amount_to_duration(uint8_t amount) {
    // (ml / (ml/s) ) * ms/s
    return (unsigned long) (((unsigned long) amount) / Globals::flowrate * 1000UL);
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

void Relay::turn_on() {
    #ifdef DEBUG
      Serial.println("relays on");
    #endif
    _active = 0;
    for(uint8_t i = 0; i < NUM_PUMPS; i++) {
        if (_amount[i] > 0) {
            char sb[30];
            sprintf(sb, "relay %d on for amt %d", i+1, _amount[i]);
            Serial.println(sb);
            digitalWrite(Globals::RELAY_PINS[i], LOW);
            _active++;
        }
    }
}

void Relay::turn_off_all() {
    Serial.println("turning off all relays");
    for(uint8_t i = 0; i < NUM_PUMPS; i++) {
        turn_off(i);
    }
    _active = 0;
    //_testing = false; -- do we need this?  I don' think so
}

void Relay::turn_off(uint8_t relay) {
    // #ifdef DEBUG
        char sb[20];
        sprintf(sb, "relay %d turning off", relay+1);
        Serial.println(sb);
    // #endif
    digitalWrite(Globals::RELAY_PINS[relay], HIGH);
    if (_amount[relay] > 0 && _active > 0) {
        _active--;  
    }
}

void Relay::update() {
    // don't try to turn off during testing
    if(_testing) {
        return;
    }

    // if not active, there's nothing to check
    if(_active == 0) {
        return;
    }

    // #ifdef DEBUG
    //     char sb[20];
    //     sprintf(sb, "_active = %d", _active);
    //     Serial.println(sb);
    // #endif 

    unsigned long now = millis();
    for(uint8_t i=0; i < NUM_PUMPS; i++) {

        // no need to turn off disabled pumps
        if (_amount[i] == 0) {
            continue;
        }

        // if the pump is active and we've exceeded the amount by duration...
        if(now > _start_time + amount_to_duration(_amount[i])) {
            // #ifdef DEBUG
            char sb[30];
            sprintf(sb, "relay %d exceeded time for amt %d", i+1, _amount[i]);
            Serial.println(sb);
            // #endif
            Buzzer::buzz(75);
            turn_off(i);


            // check only immediately after stopping an active pump....
            // if this was the last active pump, restart timer and state change
            if(_active == 0) {
                Buzzer::buzz(500);
                turn_off_all(); // just to be safe
                RHTimer::start();
                // #ifdef DEBUG
                Serial.println("restarting timer...");
                // #endif
                Machine::changeState(static_cast<UI_State *>(new UI_Inactive()));
            }
        }
    }
}

