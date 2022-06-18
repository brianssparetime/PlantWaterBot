#pragma once
#include "REWrapper.h"
#include "Encoder.h"
#include "Arduino.h"
#include "Machine.h"

REWrapper::REWrapper(Encoder enc) {
    first_push = 0;
    prev_RE_button = false;
    rot_buffer = 0;
    last_rot = 0;
    encoder = &enc;
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
    if (pb && (first_push + push_cooldown < now)) {
        first_push = now; 
        button_action();
    }

    // rotate
    if(delta != 0) {
        rot_buffer -= delta;
        last_rot = now;
        #ifdef DEBUG
            Serial.println("rb = "+String(rot_buffer) +"   delta = "+String(delta));
        #endif
    } 
    long unsigned lr = last_rot;

    
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
            rotate_action(rb);
        } else if (rb < 0) {
            #ifdef DEBUG
                dir = "Left   ";
                Serial.println(dir + ":  rb = "+String(rb) );
            #endif
            rotate_action(rb);
        }
    } else if (now > lr + rot_delay) {
    rot_buffer = 0;
    }

}

void REWrapper::button_action() {
    Machine::handle_button_press();
}

void REWrapper::rotate_action(int delta) {
    Machine::handle_rotation(delta);
}

