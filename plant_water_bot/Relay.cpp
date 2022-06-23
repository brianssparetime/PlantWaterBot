#pragma once
#include "Relay.h"
#include "Arduino.h"
#include "LCD_Wrapper.h"
#include "RHTimer.h"
#include "Machine.h"
#include "PinsGlobals.h"

#define DEBUG

bool Relay::active = false;
int Relay::amount = Globals::amounts[0];

void Relay::turn_on() {
    #ifdef DEBUG
      Serial.println("relay on");
    #endif
    digitalWrite(pin, LOW);
}

void Relay::turn_off() {
    #ifdef DEBUG
      Serial.println("relay off");
    #endif
    digitalWrite(pin, HIGH);
    RHTimer::start();
    Serial.println("restarting timer...");
}

long unsigned Relay::amount_to_duration(int amount) {
    const int flowrate = 3; // ml per sec
    // (ml / (ml/s) ) * ms/s
    return ((unsigned long) amount * 1000UL / (unsigned long) flowrate);
}

void Relay::set_amount(int amt) {
    amount = amt;
}

int Relay::get_amount() {
    return amount;
}

unsigned long Relay::get_duration() {
    return amount_to_duration(amount);

}


void Relay::activate() {
    #ifdef DEBUG
      Serial.println("relay activated");
    #endif
    turn_on();
    start_time = millis();
    active = true;
    Machine::changeState(static_cast<UI_State *>(new UI_Watering()));
    // Note:  Do this after state transition to avoid the backlight call in changeState overriding this...
    long unsigned ms = amount_to_duration(amount);
    LCD_Wrapper::backlight(ms);
}

void Relay::update() {
    long unsigned now = millis();
    if(active && (now > start_time + amount_to_duration(amount))) {
        #ifdef DEBUG
          Serial.println("relay update - deactivating...");
        #endif
        turn_off();
        active = false;
        Machine::changeState(static_cast<UI_State *>(new UI_Interval()));
    }
}

long unsigned Relay::start_time = 0UL; 
