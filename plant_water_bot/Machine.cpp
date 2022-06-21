#pragma once
#include "Machine.h"
#include "UI_States.h"
#include "LCD_Wrapper.h"

#define DEBUG

Machine::Machine(){
    current_state = new UI_Welcome();
}

void Machine::changeState(UI_State* new_state) {
    #ifdef DEBUG
      Serial.println("changing states...");
    #endif
    // if (instance == NULL)
    //     instance = new Machine();
    UI_State *old_state = instance->current_state;
    instance->current_state = new_state;
    delete old_state; 
    LCD_Wrapper::backlight();
    instance->current_state->activate();
}

void Machine::init() {
  getInstance();
}

Machine* Machine::getInstance() {
  if (instance == NULL)
      instance = new Machine();
  return instance;
}

void Machine::activate() { instance->current_state->activate();}
void Machine::update() { instance->current_state->update(); }
void Machine::handle_button_press() { instance->current_state->handle_button_press();}
void Machine::handle_rotation(int delta) { instance->current_state->handle_rotation(delta);}

Machine* Machine::instance = NULL;

