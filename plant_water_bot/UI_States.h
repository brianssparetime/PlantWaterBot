#pragma once
#include "Machine.h"
#include "LCD_Wrapper.h"


class UI_State {
    public:
        virtual void activate();
        virtual void update();
        virtual void handle_button_press();
        virtual void handle_rotation(int delta);
        virtual ~UI_State();
};



class UI_State_Setter {
    public:
        static int next_left(int current, int arr[], int arr_size);
        static int next_right(int current, int arr[], int arr_size);
};

class UI_Welcome : public UI_State {
    public:
        virtual void activate();
        virtual void update();
        virtual void handle_button_press();
        virtual void handle_rotation(int delta);
    private:
        const int duration = 3; // seconds
        unsigned long start;
        

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
        virtual void handle_rotation(int delta);
    private:
        int new_interval_selected;
        void adjust_lcd_state(int intv);
    
};

class UI_Watering : public UI_State {
    public:
        virtual void activate();
        virtual void handle_button_press();
        virtual void handle_rotation(int delta);
    private:
        unsigned long start;
        

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
        int new_amount_selected;
        void adjust_lcd_state(int intv);
        int _relay;
    
};

// TODO:  define and implement low/empty sensor states
// TODO:  consider abstractifying the idea of a timer state, and a set/get state
// TODO:  consider doing state transitions through some kind of look up table that would 
//          replace the handle_ virtual functions with optional hooks?

