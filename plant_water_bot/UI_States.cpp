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

// these methods don't get invoked unless a) not defined for the inheriting class, or 
// b) explicitly called by inheriting class


void UI_State::update() {
    if( millis() - Machine::get_last_action() > UI_State::_inactive_timeout ) {
        #ifdef DEBUG
        Serial.println("going inactive!");
        #endif
        Machine::changeState(static_cast<UI_State *>(new UI_Inactive()));
    }
}

void UI_State::handle_button_press() { }
void UI_State::handle_button_long_press() { }
void UI_State::handle_button_long_release() { }
void UI_State::handle_rotation(int delta) { }

/*********** UI_State_Setter *************/


static int UI_State_Setter::next_left(int current, uint8_t arr[], uint8_t arr_size) {
    
    // on match, return holder, which is either previous 
    // or if i=0, then return holder (which is already set to the last)
    int p = arr[arr_size - 1];
    for (uint8_t i = 0; i < arr_size; i++) {
        int x = arr[i];
        if(x == current) {
            return p;
        }
        p = x;
    }
}

static int UI_State_Setter::next_right(int current, uint8_t arr[], uint8_t arr_size) {
    // if we're currently at the last position, return first one
    if (arr[arr_size - 1] == current) {
        return arr[0];
    }
    // otherwise, start from begining, go until second to last, and return next after match
    for (uint8_t i = 0; i < arr_size - 1; i++) {
        int x = arr[i];
        if(x == current) {
            return arr[i+1];
        }
    }
}


/*********** UI_Welcome *************/


void UI_Welcome::activate() {
    #ifdef DEBUG
      Serial.println("UI Welcome activated");
    #endif
    _start = millis();
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
    if(now - _start > 1000UL * _duration) {
        #ifdef DEBUG 
            Serial.println("about to change to interval");
        #endif 
        LCD_Wrapper::backlight();
        Machine::changeState(static_cast<UI_State *>(new UI_Interval()));
    }
}

/*********** UI_Interval *************/

void UI_Interval::adjust_lcd_state() {
    char sb[17];
    uint8_t lsd = UI_Inactive::get_time_left(sb);
    if(lsd != _lsd) {
        LCD_Wrapper::display("< INTERVAL >", sb);
        _lsd = lsd;
    }
}

void UI_Interval::activate() {
    #ifdef DEBUG
      Serial.println("UI Interval activated");
    #endif
    adjust_lcd_state();
}
void UI_Interval::handle_button_press() {
    Machine::changeState(static_cast<UI_State *>(new UI_Interval_Set()));
}

void UI_Interval::handle_rotation(int delta) {
    if(delta > 0) {
        Machine::changeState(static_cast<UI_State *>(new UI_Amount(0)));
    } else {
        Machine::changeState(static_cast<UI_State *>(new UI_Test()));
    }
}

void UI_Interval::update() {
    adjust_lcd_state();
    UI_State::update();
}


/*********** UI_Interval_Set *************/


void UI_Interval_Set::adjust_lcd_state(uint8_t intv) {
    char sb[17];
    if(intv == 24) {
        sprintf(sb, "< daily >", intv);
    } else if (intv > 24 && intv < 168) {
        sprintf(sb, "< every %d days >", intv / 24 );
    } else if (intv == 168) {
        sprintf(sb, "< weekly >", intv / 168);
    } else {
        sprintf(sb, "< %02dH >", intv);
    }
    LCD_Wrapper::display("  INTERVAL Set:", sb);
}

void UI_Interval_Set::activate() {
    #ifdef DEBUG
      Serial.println("UI Interval Set activated");
    #endif
    int ci = RHTimer::get_current_interval();
    adjust_lcd_state(ci);
    _new_interval_selected = ci;
}

void UI_Interval_Set::handle_button_press() {
    #ifdef DEBUG
      Serial.println("UI Interval Set:  new interval set to " + String(_new_interval_selected));
    #endif
    RHTimer::start(_new_interval_selected);
    Machine::changeState(static_cast<UI_State *>(new UI_Interval()));
}

void UI_Interval_Set::handle_button_long_press() {
    #ifdef DEBUG
      Serial.println("UI Interval Set long press:  new interval set to " + String(0));
    #endif
    RHTimer::start(0);
    Machine::changeState(static_cast<UI_State *>(new UI_Interval()));
}

void UI_Interval_Set::handle_rotation(int delta) {
    if(delta > 0) {
        _new_interval_selected = UI_State_Setter::next_right( \
            _new_interval_selected, Globals::intervals, Globals::intervals_size);
        adjust_lcd_state(_new_interval_selected);
    } else if (delta < 0) {
        _new_interval_selected = UI_State_Setter::next_left( \
            _new_interval_selected, Globals::intervals, Globals::intervals_size);
        adjust_lcd_state(_new_interval_selected);
    }
}


/*********** UI_Watering *************/


void UI_Watering::activate() {
    #ifdef DEBUG
      Serial.println("UI Watering activated");
    #endif
    this->_start = millis();
    LCD_Wrapper::display("    Slaking     ", "     Thirst!    ");
    Relay::activate();

}

void UI_Watering::handle_button_press() { 
        Machine::changeState(static_cast<UI_State *>(new UI_Interval()));
}
void UI_Watering::handle_rotation(int delta) { 
        Machine::changeState(static_cast<UI_State *>(new UI_Interval()));
}

// disable going inactive!
void UI_Watering::update() {}

// turn off before exiting state
UI_Watering::~UI_Watering() {
    Relay::turn_off();
}


// state change after timer ends is now handled within Relay::update()

/*********** UI_Amount *************/


UI_Amount::UI_Amount(uint8_t relay) { 
    _relay = relay;
}

void UI_Amount::activate() {
    #ifdef DEBUG
      Serial.println("UI Amount" + String(_relay + 1) + " activated");
    #endif
    char sba[17];
    sprintf(sba, "< AMOUNT %d > ", _relay + 1);
    char sbb[17];
    sprintf(sbb, "  %d ml  ", Relay::get_amount(_relay));
    LCD_Wrapper::display(sba, sbb);

}
void UI_Amount::handle_button_press() { 
    Machine::changeState(static_cast<UI_State *>(new UI_Amount_Set(_relay)));
}
void UI_Amount::handle_rotation(int delta) {
    // TODO  probably a better way to do this?
    if(delta > 0) {
        if(_relay < NUM_PUMPS - 1) {
            Machine::changeState(static_cast<UI_State *>(new UI_Amount(_relay+1)));
        } else {
            Machine::changeState(static_cast<UI_State *>(new UI_Test()));
        }
    } else {
        if(_relay > 0) {
            Machine::changeState(static_cast<UI_State *>(new UI_Amount(_relay-1)));
        } else {
            Machine::changeState(static_cast<UI_State *>(new UI_Interval()));
        }
    }
}



/*********** UI_Amount_Set *************/


UI_Amount_Set::UI_Amount_Set(uint8_t relay) {
    _new_amount_selected = Relay::get_amount(_relay);
    _relay = relay;
}

void UI_Amount_Set::activate() {
    #ifdef DEBUG
      Serial.println("UI Amount " + String(_relay + 1) + " Set activated");
    #endif
    _new_amount_selected = Relay::get_amount(_relay);
    adjust_lcd_state(_new_amount_selected);
}
void UI_Amount_Set::handle_button_press() { 
    #ifdef DEBUG
      Serial.println("UI Amount " + String(_relay + 1) + " Set:  setting new amount to "+ String(_new_amount_selected));
    #endif
    Relay::set_amount(_new_amount_selected, _relay);
    Machine::changeState(static_cast<UI_State *>(new UI_Amount(_relay)));
}
void UI_Amount_Set::handle_rotation(int delta) { 
    if(delta > 0) {
        _new_amount_selected = UI_State_Setter::next_right(_new_amount_selected, Globals::amounts, Globals::amounts_size);
        adjust_lcd_state(_new_amount_selected);
    } else if (delta < 0) {
        _new_amount_selected = UI_State_Setter::next_left(_new_amount_selected, Globals::amounts, Globals::amounts_size);
        adjust_lcd_state(_new_amount_selected);
    }
}

// TODO:  perhaps implement this stuff in another inherited class
void UI_Amount_Set::adjust_lcd_state(uint8_t intv) {
    char sba[17];
    sprintf(sba, "  AMOUNT %d Set:", _relay + 1);
    char sbb[17];
    sprintf(sbb, "< %d ml >", intv);
    LCD_Wrapper::display(sba, sbb);
}


/*********** UI_Inactive *************/

static uint8_t UI_Inactive::get_time_left(char* sb) {
    uint8_t d = RHTimer::get_d_remaining();
    uint8_t h = RHTimer::get_h_remaining();
    uint8_t m = RHTimer::get_m_remaining();
    uint8_t s = RHTimer::get_s_remaining();
    uint8_t v = RHTimer::get_current_interval();
    if(d > 0) {
        sprintf(sb, "%dd%dh / %02dh", d, s, v);
        return h;
    } else {
        sprintf(sb, "%dh%02dm%02ds / %02dh", h, m, s, v);
        return s;
    }
}

void UI_Inactive::adjust_lcd_state() {
    char sb[17];
    uint8_t lsd = UI_Inactive::get_time_left(sb);
    if(lsd != _lsd) {
        UI_Inactive::get_time_left(sb);
        LCD_Wrapper::display("   PlantBot     ", sb);
        _lsd = lsd;
    }
}

void UI_Inactive::activate() {
    #ifdef DEBUG
      Serial.println("UI Inactive activated");
    #endif
    _start = millis();
    adjust_lcd_state();
}

void UI_Inactive::handle_button_press() {
    Machine::changeState(static_cast<UI_State *>(new UI_Interval()));
}
void UI_Inactive::handle_rotation(int delta) { 
    Machine::changeState(static_cast<UI_State *>(new UI_Interval()));
}

void UI_Inactive::update() {
    // Note:  because this doesn't explicitly call UI_State::update(), that method is overridden
    // and not invoked from here, so we don't have to worry about constantly changing to Inactive
    unsigned long lcd_turnoff_delay = 5000UL;
    if( millis() - _start > lcd_turnoff_delay) {
        LCD_Wrapper::backlightOff();
    }
    adjust_lcd_state();
}

/*********** UI_Test *************/

void UI_Test::activate() {
    #ifdef DEBUG
      Serial.println("UI Test activated");
    #endif
    LCD_Wrapper::display("  < TEST >  ","press and hold");
}

void UI_Test::handle_button_long_press() {
    LCD_Wrapper::display("  < TEST >  ","Oh Yeah Baby!");
    Relay::testing(true);
    Relay::turn_on();
}
void UI_Test::handle_button_long_release() {
    Relay::testing(false);
    Relay::turn_off();
    Machine::changeState(static_cast<UI_State *>(new UI_Test()));
}

void UI_Test::handle_rotation(int delta) { 
    if(delta > 0) {
        Machine::changeState(static_cast<UI_State *>(new UI_Interval()));
    } else {
        Machine::changeState(static_cast<UI_State *>(new UI_Amount(NUM_PUMPS - 1)));
    }
}

//block inactive from being called by base class udpate
void UI_Test::update() {}

// turn off before exiting state
UI_Test::~UI_Test() {
    Relay::turn_off();
}






/*********** UI_Empty *************/

// TODO:  consider not activating the relay if we sense empty/low.