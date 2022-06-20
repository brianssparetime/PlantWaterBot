#pragma once
#include "UI_States.h"
#include "Machine.h"
#include "Arduino.h"
#include "LCD_Wrapper.h"
#include "RHTimer.h"
#include "Relay.h"

#define DEBUG

// Note:  cannot remove empty methods here... bummer.


/*********** UI_State_Setter *************/


static int UI_State_Setter::next_left(int current, int arr[], int arr_size) {
    
    // on match, return holder, which is either previous 
    // or if i=0, then return holder (which is already set to the last)
    int p = arr[arr_size - 1];
    for (int i = 0; i < arr_size; i++) {
        int x = arr[i];
        if(x == current) {
            return p;
        }
        p = x;
    }
}

static int UI_State_Setter::next_right(int current, int arr[], int arr_size) {
    // if we're currently at the last position, return first one
    if (arr[arr_size - 1] == current) {
        return arr[0];
    }
    // otherwise, start from begining, go until second to last, and return next after match
    for (int i = 0; i < arr_size - 1; i++) {
        int x = arr[i];
        if(x == current) {
            return arr[i+1];
        }
    }
}



/*********** UI_Welcome *************/

UI_Welcome::UI_Welcome() { 
    start = 0;
}

void UI_Welcome::activate() {
    start = millis();
    LCD_Wrapper::display("   PlantBot     ", "    active      ");
    #ifdef DEBUG
      Serial.println("UI Welcome activated");
    #endif
}

void UI_Welcome::handle_button_press() {
    Machine::changeState(static_cast<UI_State *>(new UI_Interval()));
 }
void UI_Welcome::handle_rotation(int delta) { 
    Machine::changeState(static_cast<UI_State *>(new UI_Interval()));
}

void UI_Welcome::update() { 
    unsigned long now = millis();
    if(now - start > 1000 * duration) {
        #ifdef DEBUG 
            Serial.println("about to change to interval");
        #endif 
        LCD_Wrapper::backlight();
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
    sprintf(sb, "%02d H (%02d H left)", ci, hours_left);
    LCD_Wrapper::display("< INTERVAL >", sb);
    #ifdef DEBUG
      Serial.println("UI Interval activated");
    #endif
}
void UI_Interval::handle_button_press() {
    Machine::changeState(static_cast<UI_State *>(new UI_Interval_Set()));
}

void UI_Interval::handle_rotation(int delta) {
        Machine::changeState(static_cast<UI_State *>(new UI_Amount()));
}

void UI_Interval::update() { }

/*********** UI_Interval_Set *************/

UI_Interval_Set::UI_Interval_Set() {
    new_interval_selected = RHTimer::get_current_interval();
}


void UI_Interval_Set::adjust_lcd_state(int intv) {
    char sb[50];
    sprintf(sb, "< %02dH >", intv);
    LCD_Wrapper::display("  INTERVAL Set:", sb);
}

void UI_Interval_Set::activate() {
    int ci = RHTimer::get_current_interval();
    adjust_lcd_state(ci);
    new_interval_selected = ci;
    #ifdef DEBUG
      Serial.println("UI Interval Set activated");
    #endif
}

void UI_Interval_Set::handle_button_press() {
    RHTimer::start(new_interval_selected);
    Machine::changeState(static_cast<UI_State *>(new UI_Interval()));
}

void UI_Interval_Set::handle_rotation(int delta) {
    if(delta > 0) {
        new_interval_selected = UI_State_Setter::next_right( \
            new_interval_selected, RHTimer::intervals, RHTimer::intervals_size);
        adjust_lcd_state(new_interval_selected);
    } else if (delta < 0) {
        new_interval_selected = UI_State_Setter::next_left( \
            new_interval_selected, RHTimer::intervals, RHTimer::intervals_size);
        adjust_lcd_state(new_interval_selected);
    }
}

void UI_Interval_Set::update() { }


/*********** UI_Watering *************/


UI_Watering::UI_Watering() { }

void UI_Watering::activate() {
    this->start = millis();
    LCD_Wrapper::display("    Slaking     ", "     Thirst!    ");

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


/*********** UI_Amount *************/


UI_Amount::UI_Amount() { }

void UI_Amount::activate() {
    char sb[50];
    sprintf(sb, "  %d ml  ", Relay::get_amount());
    LCD_Wrapper::display("< AMOUNT >", sb);

}
void UI_Amount::handle_button_press() { 
    Machine::changeState(static_cast<UI_State *>(new UI_Amount_Set()));
}
void UI_Amount::handle_rotation(int delta) {
    Machine::changeState(static_cast<UI_State *>(new UI_Interval()));
}

void UI_Amount::update() { 
}






/*********** UI_Amount_Set *************/

static int new_amount_selected;

UI_Amount_Set::UI_Amount_Set() {
    new_amount_selected = Relay::get_amount();
}

void UI_Amount_Set::activate() {
    adjust_lcd_state(new_amount_selected);
}
void UI_Amount_Set::handle_button_press() { 
    Relay::set_amount(new_amount_selected);
    Machine::changeState(static_cast<UI_State *>(new UI_Amount()));
}
void UI_Amount_Set::handle_rotation(int delta) { 
    if(delta > 0) {
        new_amount_selected = UI_State_Setter::next_right(new_amount_selected, Relay::amounts, Relay::amounts_size);
        adjust_lcd_state(new_amount_selected);
    } else if (delta < 0) {
        new_amount_selected = UI_State_Setter::next_left(new_amount_selected, Relay::amounts, Relay::amounts_size);
        adjust_lcd_state(new_amount_selected);
    }
}
void UI_Amount_Set::update() { 
}

// TODO:  perhaps implement this stuff in another inherited class
void UI_Amount_Set::adjust_lcd_state(int intv) {
    char sb[50];
    sprintf(sb, "< %d ml >", intv);
    LCD_Wrapper::display("  AMOUNT Set:", sb);
}



/*********** destructors *************/


UI_State::~UI_State() {}
UI_Welcome::~UI_Welcome() {}
UI_Interval::~UI_Interval() {}
UI_Interval_Set::~UI_Interval_Set() {}
UI_Watering::~UI_Watering() {}
UI_Amount::~UI_Amount() {}
UI_Amount_Set::~UI_Amount_Set() {}