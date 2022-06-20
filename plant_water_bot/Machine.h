#pragma once
#include "UI_States.h"

// singleton

class UI_State;

class Machine {

    // singleton

    friend class UI_State;

    public:
        static Machine *getInstance();
        static void changeState(UI_State* new_state);
        static void activate();
        static void update();
        static void handle_button_press();
        static void handle_rotation(int delta);


    private:
        Machine() = default;
        UI_State* current_state;
        static Machine* instance;

};
