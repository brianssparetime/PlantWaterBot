#pragma once
#include "RHTimer.h"
#include "Arduino.h"
#include "Relay.h"
#include "PinsGlobals.h"

#define DEBUG

int RHTimer::minutes_elapsed = 0;
int RHTimer::hours_elapsed = 0;
unsigned long RHTimer::next_min_millis = 0;

int RHTimer::cur_interval = Globals::intervals[0];

void RHTimer::start() {
    unsigned long now = millis();
    next_min_millis = now + 60UL * 1000UL; // milliseconds to min 
    hours_elapsed = 0;
    minutes_elapsed = 0;
    #ifdef DEBUG
      Serial.println("timer start:  next_min_millis = "+ String(next_min_millis) + " and now = "+String(now));
    #endif DEBUG
}

void RHTimer::start(int interval) {
    cur_interval = interval;
    start();
}

int RHTimer::get_h_remaining() {
    return cur_interval - hours_elapsed - 1;
}

int RHTimer::get_m_remaining() {
    return 60 - minutes_elapsed;
}

void RHTimer::update() {
    // if next_min_millis is in the past...
    if(millis() > next_min_millis) {
        minutes_elapsed++;
        next_min_millis = millis() + 60UL * 1000UL; // milliseconds to min
        #ifdef DEBUG
            Serial.println("minute tick");
        #endif DEBUG

        if(minutes_elapsed == 60) {
            hours_elapsed ++;
            minutes_elapsed = 0;
            #ifdef DEBUG
                Serial.println("hour tick");
            #endif DEBUG

            if(hours_elapsed == cur_interval) {
                minutes_elapsed = 0;
                hours_elapsed = 0;
                next_min_millis = 0;
                #ifdef DEBUG
                    Serial.println("alarm tick");
                #endif DEBUG
                alarm();
                start(cur_interval);
            }
        }
    }
}

int RHTimer::get_current_interval() {
    return cur_interval;
}


void RHTimer::alarm() {
    #ifdef DEBUG
        Serial.println("==========ALARM=================="); 
    #endif DEBUG
    Relay::activate();

}
