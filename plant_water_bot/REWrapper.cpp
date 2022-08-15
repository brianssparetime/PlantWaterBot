#pragma once
#include "REWrapper.h"
#include "Encoder.h"
#include "Arduino.h"
#include "Machine.h"


REWrapper::REWrapper(Encoder* enc) {
    _last_action = 0UL;
    _long_press = false;
    _long_press_sent = false;
    _encoder = enc;
}


void REWrapper::update() {


    // do this even if we ignore to prevent buildup
    // read the debounced value of the encoder button
    bool pb = _encoder->button();
    // get the encoder variation since our last check, it can be positive or negative, or zero if the encoder didn't move
    // only call this once per loop cicle, or at any time you want to know any incremental change
    int delta = _encoder->delta();

    // button
    if(_long_press) {
        if (!_long_press_sent && (millis() - _last_action > _long_press_delay)) {
            if(pb) {
                #ifdef DEBUG
                    Serial.println("button long press");
                #endif
                button_long_press();
                _long_press_sent = true;
                return;
            } 
        }
        if(! pb ) {
            if(_long_press_sent) {
                #ifdef DEBUG
                    Serial.println("button long release");
                #endif
                button_long_release();
            } else { // long enough to activate, but not long enough for long
                #ifdef DEBUG
                    Serial.println("button press");
                #endif
                button_press();
            }
            _long_press_sent = false;
            _long_press = false;
            _last_action = millis(); 
            return;
        }
    } else {
        if (pb && (millis() - _last_action > push_cooldown)) {
            _last_action = millis(); 
            _long_press = true;
            return;
        }
    }


    // if its been more than rot_delay since last rotary action
    if (millis() > _last_action + rot_delay) {
        // some time has passed since the rotary did anything
        delta = - delta;
        if (delta > 0) {
            #ifdef DEBUG
                Serial.println("Right :  rb = "+String(delta) );
            #endif
            rotation(delta);
        } else if (delta < 0) {
            #ifdef DEBUG
                Serial.println("Left :  rb = "+String(delta) );
            #endif
            rotation(delta);
        } else {
            return;
        }
        _last_action = millis();
    } 

}

void REWrapper::button_press() {
    Machine::handle_button_press();
}

void REWrapper::button_long_press() {
    Machine::handle_button_long_press();
}

void REWrapper::button_long_release() {
    Machine::handle_button_long_release();
}

void REWrapper::rotation(int delta) {
    LCD_Wrapper::backlight();
    Machine::handle_rotation(delta);
}

