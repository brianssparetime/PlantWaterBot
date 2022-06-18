#pragma once
#include "RHTimer.h"
#include "Arduino.h"
#include "Relay.h"


int RHTimer::minutes_elapsed = 0;
int RHTimer::hours_elapsed = 0;
long unsigned RHTimer::next_min_millis = 0;
int RHTimer::cur_interval = 6;

void RHTimer::start() {
    unsigned long now = millis();
    RHTimer::next_min_millis = now + 60 * 1000; // milliseconds to min 
}

void RHTimer::start(int interval) {
    RHTimer::cur_interval = interval;
    start();
}

int RHTimer::get_h_remaining() {
    return RHTimer::cur_interval - RHTimer::hours_elapsed - 1;
}

int RHTimer::get_m_remaining() {
    return 60 - RHTimer::minutes_elapsed;
}

void RHTimer::update() {
    unsigned long now = millis();
    // if next_min_millis is in the past...
    if(now > RHTimer::next_min_millis) {
        RHTimer::minutes_elapsed++;
        RHTimer::next_min_millis = now + 60 * 1000; // milliseconds to min
    }
    if(RHTimer::minutes_elapsed == 60) {
        RHTimer::hours_elapsed ++;
        RHTimer::minutes_elapsed = 0;
    }
    if(RHTimer::hours_elapsed == RHTimer::cur_interval) {
        alarm();
        start(RHTimer::cur_interval);
    }
}

int RHTimer::get_current_interval() {
    return RHTimer::cur_interval;
}


void RHTimer::alarm() {
    // TODO:  activate relay, or call relay object?
    #ifdef DEBUG
        Serial.println("==========ALARM=================="); 
    #endif DEBUG
    Relay::activate();

}
