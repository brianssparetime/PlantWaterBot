#pragma once
#include "REWrapper.h"
#include "Encoder.h"
#include "Arduino.h"
#include "Machine.h"

#define DEBUG

REWrapper::REWrapper(Encoder* enc) {
    _first_push = 0UL;
    _long_press = false;
    _long_press_sent = false;
    _rot_buffer = 0;
    _last_rot = 0UL;
    _encoder = enc;
}


void REWrapper::update() {


    // do this even if we ignore to prevent buildup
    // read the debounced value of the encoder button
    bool pb = _encoder->button();
    // get the encoder variation since our last check, it can be positive or negative, or zero if the encoder didn't move
    // only call this once per loop cicle, or at any time you want to know any incremental change
    int delta = _encoder->delta();

    long unsigned now = millis();


    // button
    if(_long_press) {
        if (!_long_press_sent && (now - _first_push > _long_press_delay)) {
            if(pb) {
                #ifdef DEBUG
                    Serial.println("button long press");
                #endif
                // TODO:  maybe don't issue both down and long?  but that induces lag
                button_long_press();
                _long_press_sent = true;
            } else {
                _long_press = false;

            }
        }
        if(! pb ) {
            if(_long_press_sent) {
                #ifdef DEBUG
                    Serial.println("button long release");
                #endif
                button_long_release();
            _long_press_sent = false;
            _long_press = false;
            }
        }
    } else {
        if (pb && (now - _first_push > push_cooldown)) {
            _first_push = now; 
            #ifdef DEBUG
                Serial.println("button press");
            #endif
            button_press();
            _long_press = true;
        }
    }

    // deaden rotation around button pressing:
    if(pb) {
        _last_rot = now;
        _rot_buffer = 0;

    }

    // rotate
    if(delta != 0) {
        _rot_buffer -= delta;
        _last_rot = now;
        #ifdef DEBUG
            Serial.println("rb = "+String(_rot_buffer) +"   delta = "+String(delta));
        #endif
    } 

    
    // if its been more than rot_delay since last rotary action
    if (now > _last_rot + rot_delay) {
        int rb = _rot_buffer;
        _rot_buffer = 0;
        // some time has passed since the rotary did anything
        String dir = "Neutral";
        if (rb > 0) {
            #ifdef DEBUG
                dir = "Right  ";
                Serial.println(dir + ":  rb = "+String(rb) );
            #endif
            rotation(rb);
        } else if (rb < 0) {
            #ifdef DEBUG
                dir = "Left   ";
                Serial.println(dir + ":  rb = "+String(rb) );
            #endif
            rotation(rb);
        }
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

