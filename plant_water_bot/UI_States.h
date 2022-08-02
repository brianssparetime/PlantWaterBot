#pragma once
#include "Machine.h"
#include "LCD_Wrapper.h"


class UI_State {
    public:
        virtual void activate();
        virtual void update();
        virtual void handle_button_press();
        virtual void handle_button_long_press();
        virtual void handle_button_long_release();
        virtual void handle_rotation(int delta);
    private:
        static const unsigned long _inactive_timeout = 5UL * 1000UL; 

};


class UI_State_Setter {
    public:
        static int next_left(int current, uint8_t arr[], uint8_t arr_size);
        static int next_right(int current, uint8_t arr[], uint8_t arr_size);
};

class UI_Welcome : public UI_State {
    public:
        virtual void activate();
        virtual void update();
        virtual void handle_button_press();
        virtual void handle_rotation(int delta);
    private:
        const int _duration = 4; // seconds
        unsigned long _start;
        

};

class UI_Interval : public UI_State {
    public:
        virtual void activate();
        virtual void handle_button_press();
        virtual void handle_rotation(int delta);


};


class UI_Interval_Set : public UI_State, public UI_State_Setter {
    public:
        virtual void activate();
        virtual void handle_button_press();
        virtual void handle_button_long_press();
        virtual void handle_rotation(int delta);
    private:
        int _new_interval_selected;
        void adjust_lcd_state(uint8_t intv);
    
};

class UI_Watering : public UI_State {
    public:
        virtual void activate();
        virtual void handle_button_press();
        virtual void handle_rotation(int delta);
        virtual void update();
        virtual ~UI_Watering();
    private:
        unsigned long _start;
        

};


class UI_Amount : public UI_State {
    public:
        UI_Amount(int relay);
        virtual void activate();
        virtual void handle_button_press();
        virtual void handle_rotation(int delta);
    private:
        int _relay;


};


class UI_Amount_Set : public UI_State, public UI_State_Setter {
    public:
        UI_Amount_Set(int relay);
        virtual void activate();
        virtual void handle_button_press();
        virtual void handle_rotation(int delta);

    private:
        int _new_amount_selected;
        void adjust_lcd_state(uint8_t intv);
        int _relay;
    
};


class UI_Inactive : public UI_State {
    public:
        virtual void activate();
        virtual void handle_button_press();
        virtual void handle_rotation(int delta);
        static void get_time_left(char* sb);
        virtual void update();
    private:
        unsigned long _start;
};

class UI_Test : public UI_State {
    public:
        virtual void activate();
        virtual void handle_button_long_press();
        virtual void handle_button_long_release();
        virtual void handle_rotation(int delta);
        virtual void update();
        virtual ~UI_Test();
};

// TODO:  define and implement low/empty sensor states
// TODO:  consider abstractifying the idea of a timer state, and a set/get state
// TODO:  consider doing state transitions through some kind of look up table that would 
//          replace the handle_ virtual functions with optional hooks?

