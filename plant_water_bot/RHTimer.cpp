#pragma once
#include "RHTimer.h"
#include "Arduino.h"
#include "Relay.h"
#include "PinsGlobals.h"
#include "Machine.h"
#include "UI_States.h"


uint8_t RHTimer::_minutes_elapsed = 0;
uint8_t RHTimer::_hours_elapsed = 0;
unsigned long RHTimer::_next_min_millis = 0;

uint8_t RHTimer::_cur_interval = Globals::intervals[0];

void RHTimer::start() {
    unsigned long now = millis();
    _next_min_millis = now + 60UL * 1000UL; // milliseconds to min 
    _hours_elapsed = 0;
    _minutes_elapsed = 0;
    #ifdef DEBUG
      Serial.println("timer start:  next_min_millis = "+ String(_next_min_millis) + " and now = "+String(now));
    #endif DEBUG
}

void RHTimer::start(uint8_t interval) {
    _cur_interval = interval; // hours
    start();
}

uint8_t RHTimer::get_d_remaining() {
    if(_cur_interval > 0) {
        return (_cur_interval - _hours_elapsed - 1) / 24;
    } else {
        return 0;
    }
}

uint8_t RHTimer::get_h_remaining() {
    if(_cur_interval > 0) {
        return (_cur_interval - _hours_elapsed - 1) % 24;
    } else {
        return 0;
    }
}

uint8_t RHTimer::get_m_remaining() {
    if(_cur_interval > 0) { 
        return 60 - _minutes_elapsed - 1;
    } else {
        return 0;
    }
}

uint8_t RHTimer::get_s_remaining() {
    return (_next_min_millis - millis()) / 1000UL;
}


void RHTimer::update() {
    // if next_min_millis is in the past...
    if(millis() > _next_min_millis) {
        // NOTE:  when millis() overflows and loops back to zero, we'll just wind up
        // adding an extra minute because this will fire immediately.

        if(_cur_interval == 0) { // testing mode
            alarm();
            start(Globals::intervals[0]); // TODO:  testing mode effectivly erases your set interval
            return;
        }

        // this is for testing so you don't have to wait to see whether watering works
        #ifdef WARP_SPEED
            alarm();
            start(_cur_interval);
            return;
        #endif

        _minutes_elapsed++;
        _next_min_millis = millis() + 60UL * 1000UL; // milliseconds to min
        #ifdef DEBUG
            Serial.println("minute tick");
        #endif DEBUG

        if(_minutes_elapsed >= 60) {
            _hours_elapsed ++;
            _minutes_elapsed = 0;
            #ifdef DEBUG
                Serial.println("hour tick");
            #endif DEBUG

            if(_hours_elapsed >= _cur_interval) {
                _minutes_elapsed = 0;
                _hours_elapsed = 0;
                _next_min_millis = 0;
                #ifdef DEBUG
                    Serial.println("alarm tick");
                #endif DEBUG
                alarm();
                start(_cur_interval);
            }
        }
    }
}

uint8_t RHTimer::get_current_interval() {
    return _cur_interval;
}


void RHTimer::alarm() {
    #ifdef DEBUG
        Serial.println("==========ALARM=================="); 
    #endif DEBUG
    Machine::changeState(static_cast<UI_State *>(new UI_Watering()));
}
