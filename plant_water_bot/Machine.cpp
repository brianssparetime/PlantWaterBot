#pragma once
#include "Machine.h"
#include "UI_States.h"
#include "LCD_Wrapper.h"

#define DEBUG

unsigned long Machine::_last_action = 0UL;

Machine::Machine(){
    _current_state = new UI_Welcome();
}

void Machine::changeState(UI_State* new_state) {
    #ifdef DEBUG
      Serial.println("changing states...");
    #endif
    // if (instance == NULL)
    //     instance = new Machine();
    UI_State *old_state = _instance->_current_state;
    _instance->_current_state = new_state;
    delete old_state; 
    LCD_Wrapper::backlight();
    _instance->_current_state->activate();
}

void Machine::init() {
    getInstance();
}

Machine* Machine::getInstance() {
    if (_instance == NULL)
        _instance = new Machine();
    return _instance;
}

void Machine::activate() { _instance->_current_state->activate();}
void Machine::update() { _instance->_current_state->update(); }
void Machine::handle_button_press() { 
    _last_action = millis();
    _instance->_current_state->handle_button_press();
}
void Machine::handle_button_long_press() { 
    _last_action = millis();
    _instance->_current_state->handle_button_long_press();
}
void Machine::handle_button_long_release() { 
    _last_action = millis();
    _instance->_current_state->handle_button_long_release();
}
void Machine::handle_rotation(int delta) { 
    _last_action = millis();
    _instance->_current_state->handle_rotation(delta);
}


unsigned long Machine::get_last_action() {
    return _last_action;
}

Machine* Machine::_instance = NULL;

