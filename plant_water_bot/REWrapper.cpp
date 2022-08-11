#pragma once
#include "REWrapper.h"
#include "Encoder.h"
#include "Arduino.h"
#include "Machine.h"

#define DEBUG

REWrapper::REWrapper(Encoder* enc) {
    _last_action = 0UL;
    _long_press = false;
    _long_press_sent = false;
    _rot_buffer = 0;
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
                _rot_buffer = 0;
                return;
            } 
            _rot_buffer = 0;
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
            _rot_buffer = 0;
            return;
        }
    } else {
        if (pb && (millis() - _last_action > push_cooldown)) {
            _last_action = millis(); 
            _long_press = true;
            _rot_buffer = 0;
            return;
        }
    }


    // TODO:  consider changing this to avoid jumpiness in rotation
    // rotate
    if(delta != 0) {
        _rot_buffer -= delta;
        #ifdef DEBUG
            Serial.println("rb = "+String(_rot_buffer) +"   delta = "+String(delta));
        #endif
    } 

    
    // if its been more than rot_delay since last rotary action
    if (millis() > _last_action + rot_delay) {
        // some time has passed since the rotary did anything
        if (_rot_buffer > 0) {
            #ifdef DEBUG
                Serial.println("Right :  rb = "+String(_rot_buffer) );
            #endif
            rotation(_rot_buffer);
        } else if (_rot_buffer < 0) {
            #ifdef DEBUG
                Serial.println("Left :  rb = "+String(_rot_buffer) );
            #endif
            rotation(_rot_buffer);
        }
        _rot_buffer = 0;
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

