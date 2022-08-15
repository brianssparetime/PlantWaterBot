#pragma once
#include "Arduino.h"
#include "buzzer.h"


uint8_t Buzzer::_pin = 0;
uint16_t Buzzer::_stop_time = 0;

void Buzzer::init(int pin) {
    _pin = pin;
    _stop_time = 0;
}

void Buzzer::buzz() {
    buzz(0);
}
void Buzzer::buzz(uint16_t duration_ms) {
    if(duration_ms == 0) {
        duration_ms = _default_duration;
    }
    _stop_time = millis() + duration_ms;
    digitalWrite(_pin, HIGH);
}
void Buzzer::cancel() {
    _stop_time = 0;
    digitalWrite(_pin, LOW);
}
void Buzzer::update() {
    if(millis() > _stop_time) {
        cancel();
    }
}