#pragma once
#include "UI_States.h"
#include "Machine.h"
#include "Arduino.h"
#include "LCD_Wrapper.h"
#include "RHTimer.h"
#include "Relay.h"

#define DEBUG

/*********** UI_State *************/

// methods implemented here can be omitted from classes inheriting UI_State below
// (as can constructors and destructors), as long as the implementions here match the
// .h header file


void UI_State::update() {}

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

UI_State::~UI_State() {}

/*********** UI_Welcome *************/


void UI_Welcome::activate() {
    #ifdef DEBUG
      Serial.println("UI Welcome activated");
    #endif
    start = millis();
    LCD_Wrapper::display("   PlantBot     ", "    active      ");
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


void UI_Interval::activate() {
    #ifdef DEBUG
      Serial.println("UI Interval activated");
    #endif
    int ci = RHTimer::get_current_interval();
    int hours_left = RHTimer::get_h_remaining();
    char sb[50];
    sprintf(sb, "%02d H (%02d H left)", ci, hours_left);
    LCD_Wrapper::display("< INTERVAL >", sb);
}
void UI_Interval::handle_button_press() {
    Machine::changeState(static_cast<UI_State *>(new UI_Interval_Set()));
}

void UI_Interval::handle_rotation(int delta) {
        Machine::changeState(static_cast<UI_State *>(new UI_Amount(0)));
}


/*********** UI_Interval_Set *************/


void UI_Interval_Set::adjust_lcd_state(int intv) {
    char sb[50];
    sprintf(sb, "< %02dH >", intv);
    LCD_Wrapper::display("  INTERVAL Set:", sb);
}

void UI_Interval_Set::activate() {
    #ifdef DEBUG
      Serial.println("UI Interval Set activated");
    #endif
    int ci = RHTimer::get_current_interval();
    adjust_lcd_state(ci);
    new_interval_selected = ci;
}

void UI_Interval_Set::handle_button_press() {
    #ifdef DEBUG
      Serial.println("UI Interval Set:  new interval set to " + String(new_interval_selected));
    #endif
    RHTimer::start(new_interval_selected);
    Machine::changeState(static_cast<UI_State *>(new UI_Interval()));
}

void UI_Interval_Set::handle_rotation(int delta) {
    if(delta > 0) {
        new_interval_selected = UI_State_Setter::next_right( \
            new_interval_selected, Globals::intervals, Globals::intervals_size);
        adjust_lcd_state(new_interval_selected);
    } else if (delta < 0) {
        new_interval_selected = UI_State_Setter::next_left( \
            new_interval_selected, Globals::intervals, Globals::intervals_size);
        adjust_lcd_state(new_interval_selected);
    }
}


/*********** UI_Watering *************/


void UI_Watering::activate() {
    #ifdef DEBUG
      Serial.println("UI Watering activated");
    #endif
    this->start = millis();
    LCD_Wrapper::display("    Slaking     ", "     Thirst!    ");

}
// TODO: consider adding a cancel state
// TODO:  consider adding a test/manual activation state
void UI_Watering::handle_button_press() { 
        Machine::changeState(static_cast<UI_State *>(new UI_Interval_Set()));
}
void UI_Watering::handle_rotation(int delta) { 
        Machine::changeState(static_cast<UI_State *>(new UI_Interval()));
}

// state change after timer ends is now handled within Relay::update()

/*********** UI_Amount *************/


UI_Amount::UI_Amount(int relay) { 
    _relay = relay;
}

void UI_Amount::activate() {
    #ifdef DEBUG
      Serial.println("UI Amount" + String(_relay) + " activated");
    #endif
    char sba[20];
    sprintf(sba, "< AMOUNT %d > ", _relay);
    char sbb[20];
    sprintf(sbb, "  %d ml  ", Relay::get_amount(_relay));
    LCD_Wrapper::display(sba, sbb);

}
void UI_Amount::handle_button_press() { 
    Machine::changeState(static_cast<UI_State *>(new UI_Amount_Set(_relay)));
}
void UI_Amount::handle_rotation(int delta) {
    if(_relay < NUM_PUMPS) {
        Machine::changeState(static_cast<UI_State *>(new UI_Amount(_relay+1)));
    } else {
        Machine::changeState(static_cast<UI_State *>(new UI_Interval()));
    }
}



/*********** UI_Amount_Set *************/


UI_Amount_Set::UI_Amount_Set(int relay) {
    new_amount_selected = Relay::get_amount(_relay);
    _relay = relay;
}

void UI_Amount_Set::activate() {
    #ifdef DEBUG
      Serial.println("UI Amount " + String(_relay) + " Set activated");
    #endif
    adjust_lcd_state(new_amount_selected);
}
void UI_Amount_Set::handle_button_press() { 
    #ifdef DEBUG
      Serial.println("UI Amount " + String(_relay) + " Set:  setting new amount to "+ String(new_amount_selected));
    #endif
    Relay::set_amount(new_amount_selected, _relay);
    Machine::changeState(static_cast<UI_State *>(new UI_Amount(_relay)));
}
void UI_Amount_Set::handle_rotation(int delta) { 
    if(delta > 0) {
        new_amount_selected = UI_State_Setter::next_right(new_amount_selected, Globals::amounts, Globals::amounts_size);
        adjust_lcd_state(new_amount_selected);
    } else if (delta < 0) {
        new_amount_selected = UI_State_Setter::next_left(new_amount_selected, Globals::amounts, Globals::amounts_size);
        adjust_lcd_state(new_amount_selected);
    }
}

// TODO:  perhaps implement this stuff in another inherited class
void UI_Amount_Set::adjust_lcd_state(int intv) {
    char sba[20];
    sprintf(sba, "  AMOUNT %d Set:", _relay);
    char sbb[20];
    sprintf(sbb, "< %d ml >", intv);
    LCD_Wrapper::display(sba, sbb);
}




/*********** UI_Empty *************/

// TODO:  consider not activating the relay if we sense empty/low.