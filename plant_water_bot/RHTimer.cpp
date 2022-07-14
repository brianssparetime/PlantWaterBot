#pragma once
#include "RHTimer.h"
#include "Arduino.h"
#include "Relay.h"
#include "PinsGlobals.h"

#define DEBUG

int RHTimer::_minutes_elapsed = 0;
int RHTimer::_hours_elapsed = 0;
unsigned long RHTimer::_next_min_millis = 0;

int RHTimer::_cur_interval = Globals::intervals[0];

void RHTimer::start() {
    unsigned long now = millis();
    _next_min_millis = now + 60UL * 1000UL; // milliseconds to min 
    _hours_elapsed = 0;
    _minutes_elapsed = 0;
    #ifdef DEBUG
      Serial.println("timer start:  next_min_millis = "+ String(_next_min_millis) + " and now = "+String(now));
    #endif DEBUG
}

void RHTimer::start(int interval) {
    _cur_interval = interval;
    start();
}

int RHTimer::get_h_remaining() {
    return _cur_interval - _hours_elapsed - 1;
}

int RHTimer::get_m_remaining() {
    return 60 - _minutes_elapsed - 1;
}




void RHTimer::update() {
    // if next_min_millis is in the past...
    if(millis() > _next_min_millis) {
        // NOTE:  when millis() overflows and loops back to zero, we'll just wind up
        // adding an extra minute because this will fire immediately.
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

int RHTimer::get_current_interval() {
    return _cur_interval;
}


void RHTimer::alarm() {
    #ifdef DEBUG
        Serial.println("==========ALARM=================="); 
    #endif DEBUG
    Relay::activate();

}
