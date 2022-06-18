#pragma once
#include "RHTimer.h"
#include "Arduino.h"


int RoughHoursTimer::minutes_elapsed = 0;
int RoughHoursTimer::hours_elapsed = 0;
long unsigned RoughHoursTimer::next_min_millis = 0;
int RoughHoursTimer::cur_interval = 6;

void RoughHoursTimer::start() {
    unsigned long now = millis();
    RoughHoursTimer::next_min_millis = now + 60 * 1000; // milliseconds to min 
}

void RoughHoursTimer::start(int interval) {
    RoughHoursTimer::cur_interval = interval;
    start();
}

int RoughHoursTimer::get_h_remaining() {
    return RoughHoursTimer::cur_interval - RoughHoursTimer::hours_elapsed - 1;
}

int RoughHoursTimer::get_m_remaining() {
    return 60 - RoughHoursTimer::minutes_elapsed;
}

void RoughHoursTimer::update() {
    unsigned long now = millis();
    // if next_min_millis is in the past...
    if(now > RoughHoursTimer::next_min_millis) {
        RoughHoursTimer::minutes_elapsed++;
        RoughHoursTimer::next_min_millis = now + 60 * 1000; // milliseconds to min
    }
    if(RoughHoursTimer::minutes_elapsed == 60) {
        RoughHoursTimer::hours_elapsed ++;
        RoughHoursTimer::minutes_elapsed = 0;
    }
    if(RoughHoursTimer::hours_elapsed == RoughHoursTimer::cur_interval) {
        alarm();
        start(RoughHoursTimer::cur_interval);
    }
}

int RoughHoursTimer::get_current_interval() {
    return RoughHoursTimer::cur_interval;
}


void RoughHoursTimer::alarm() {
    // TODO:  activate relay, or call relay object?
    #ifdef DEBUG
        Serial.println("==========ALARM=================="); 
    #endif DEBUG
}
