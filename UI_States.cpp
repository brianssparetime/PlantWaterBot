#pragma once
#include "UI_States.h"
#include "Machine.h"
#include "Arduino.h"
#include "LCD_Wrapper.h"
#include "RHTimer.h"
#include "Relay.h"


// Note:  cannot remove empty methods here... bummer.


/*********** UI_Welcome *************/

UI_Welcome::UI_Welcome() { }

void UI_Welcome::activate() {
    this->start = millis();
    LCD_Wrapper::display("   PlantBot     ", "     active     ");

}
void UI_Welcome::handle_button_press() { }
void UI_Welcome::handle_rotation(int delta) { }

void UI_Welcome::update() { 
    unsigned long now = millis();
    if(now - start > 1000 * this->duration) {
        Machine::changeState(static_cast<UI_State *>(new UI_Interval()));
    }
}

/*********** UI_Interval *************/
UI_Interval::UI_Interval() {
}

void UI_Interval::activate() {
    int ci = RHTimer::get_current_interval();
    int hours_left = RHTimer::get_h_remaining();
    char sb[50];
    sprintf(sb, "  %02dH (%02dH left)", ci, hours_left);
    LCD_Wrapper::display("< INTERVAL >", sb);
}
void UI_Interval::handle_button_press() {
    Machine::changeState(static_cast<UI_State *>(new UI_Interval_Set()));
}

void UI_Interval::handle_rotation(int delta) {
    // TODO: implement this after testing
}

void UI_Interval::update() { }

/*********** UI_Interval_Set *************/

UI_Interval_Set::UI_Interval_Set() {
}

static int new_interval_selected;
const int intervals[5] = {8, 12, 24, 48, 72}; // hours
const int intervals_size = 5; //DAsizeof(intervals) / sizeof(intervals[0]);


void UI_Interval_Set::adjust_lcd_state(int intv) {
    char sb[50];
    sprintf(sb, "< %02dH >", intv);
    LCD_Wrapper::display("  INTERVAL Set:", sb);
    // lcd.setCursor(0,0); // col, row)
    // lcd.print("  INTERVAL SET:");
    // char sb[50];
    // sprintf(sb, "< %02dH >", intv);
    // lcd.print(sb);
}

static int UI_Interval_Set::next_interval_left() {
    // set to last
    int p = intervals[intervals_size - 1];
    for (int i = 0; i < intervals_size; i++) {
        int x = intervals[i];
        if(x == new_interval_selected) {
            return p;
        }
        p = x;
    }
}

int UI_Interval_Set::next_interval_right() {
    if (intervals[intervals_size-1] == new_interval_selected) {
        return intervals[0];
    }
    for (int i = 0; i < intervals_size - 1; i++) {
        int x = intervals[i];
        if(x == new_interval_selected) {
            return intervals[i+1];
        }
    }
}

void UI_Interval_Set::activate() {
    int ci = RHTimer::get_current_interval();
    adjust_lcd_state(ci);
    new_interval_selected = ci;
}

void UI_Interval_Set::handle_button_press() {
    RHTimer::start(new_interval_selected);
    Machine::changeState(static_cast<UI_State *>(new UI_Interval()));
}

void UI_Interval_Set::handle_rotation(int delta) {
    if(delta > 0) {
        new_interval_selected = next_interval_right();
        adjust_lcd_state(new_interval_selected);
    } else if (delta < 0) {
        new_interval_selected = next_interval_left();
        adjust_lcd_state(new_interval_selected);
    }
}

void UI_Interval_Set::update() { }


/*********** UI_Watering *************/


UI_Watering::UI_Watering() { }

void UI_Watering::activate() {
    this->start = millis();
    LCD_Wrapper::display("    Slaking     ", "     thirst!    ");

}
void UI_Watering::handle_button_press() { 
        Machine::changeState(static_cast<UI_State *>(new UI_Interval_Set()));
}
void UI_Watering::handle_rotation(int delta) { 
        Machine::changeState(static_cast<UI_State *>(new UI_Interval()));
}

void UI_Watering::update() { 
    unsigned long now = millis();
    if(now - start > 1000 * Relay::get_duration()) {
        Machine::changeState(static_cast<UI_State *>(new UI_Interval()));
    }
}




/*********** destructors *************/


UI_State::~UI_State() {}
UI_Welcome::~UI_Welcome() {}
UI_Interval::~UI_Interval() {}
UI_Interval_Set::~UI_Interval_Set() {}
UI_Watering::~UI_Watering() {}