#pragma once
#include "Machine.h"
#include "UI_States.h"



void Machine::changeState(UI_State* new_state) {
    UI_State *old_state = instance->current_state;
    instance->current_state = new_state;
    delete old_state;
    instance->current_state->activate();
}

Machine* Machine::getInstance() {
  static Machine* instance;
  if (instance == 0)
      instance = new Machine();
  return instance;
}

void Machine::activate() { instance->current_state->activate();}
void Machine::update() { instance->current_state->update();}
void Machine::handle_button_press() { instance->current_state->handle_button_press();}
void Machine::handle_rotation(int delta) { instance->current_state->handle_rotation(delta);}

Machine* Machine::instance = 0;

