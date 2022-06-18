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


class UI_Welcome : public UI_State {
    public:
        UI_Welcome();
        virtual void activate();
        virtual void update();
        virtual void handle_button_press();
        virtual void handle_rotation(int delta);
        virtual ~UI_Welcome();
    private:
        const int duration = 3; // seconds
        unsigned long start;
        

};

class UI_Interval : public UI_State {
    public:

        UI_Interval();
        virtual void activate();
        virtual void update();
        virtual void handle_button_press();
        virtual void handle_rotation(int delta);
        virtual ~UI_Interval();

    private:

        void adjust_lcd_state();
};


class UI_Interval_Set : public UI_State {
    public:

        UI_Interval_Set();
        virtual void activate();
        virtual void update();
        virtual void handle_button_press();
        virtual void handle_rotation(int delta);
        virtual ~UI_Interval_Set();

    private:

        int new_interval_selected;
        const int intervals[5];
        const int intervals_size = 5;
        int next_interval_left();
        int next_interval_right();
        void adjust_lcd_state(int intv);
    
};


// TODO:  define and implement amount states