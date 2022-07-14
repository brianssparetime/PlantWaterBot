#pragma once
#include "REWrapper.h"
#include "Encoder.h"
#include "Arduino.h"
#include "Machine.h"

#define DEBUG

REWrapper::REWrapper(Encoder* enc) {
    first_push = 0UL;
    prev_RE_button = false;
    rot_buffer = 0;
    last_rot = 0UL;
    encoder = enc;
}


void REWrapper::update() {


    // do this even if we ignore to prevent buildup
    // read the debounced value of the encoder button
    bool pb = encoder->button();
    // get the encoder variation since our last check, it can be positive or negative, or zero if the encoder didn't move
    // only call this once per loop cicle, or at any time you want to know any incremental change
    int delta = encoder->delta();

    long unsigned now = millis();


    // button
    if (pb && (now - first_push > push_cooldown)) {
        first_push = now; 
        #ifdef DEBUG
            Serial.println("button press");
        #endif
        button_press();
    }
    // TODO:  maybe don't issue both down and long both?  but that induces lag
    if (pb && (now - first_push > _long_press_delay)) {
        first_push = now; 
        _long_press = true;
        #ifdef DEBUG
            Serial.println("button long press");
        #endif
        button_long_press();
    }
    if(_long_press && (! pb) ) {
        #ifdef DEBUG
            Serial.println("button long release");
        #endif
        button_long_release();
    }


    // rotate
    if(delta != 0) {
        rot_buffer -= delta;
        last_rot = now;
        #ifdef DEBUG
            Serial.println("rb = "+String(rot_buffer) +"   delta = "+String(delta));
        #endif
    } 
    unsigned long lr = last_rot;

    
    // if its been more than rot_delay since last rotary action
    if (now > lr + rot_delay) {
        int rb = rot_buffer;
        rot_buffer = 0;
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

