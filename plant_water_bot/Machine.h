#pragma once
#include "UI_States.h"

// singleton

class UI_State;

class Machine {

    // singleton

    friend class UI_State;

    public:
        static Machine* getInstance();
        static void changeState(UI_State* new_state);
        static void init();
        static void activate();
        static void update();
        static void handle_button_press();
        static void handle_button_long_press();
        static void handle_button_long_release();
        static void handle_rotation(int delta);
        static unsigned long get_last_action();


    private:
        Machine(); //= default;
        UI_State* _current_state;
        static Machine* _instance;
        static unsigned long _last_action;

};
