#pragma once
#include "Encoder.h"
#include "Machine.h"

// TODO:  I should either make this a singleton like Machine, or make everything static like LCD_Wrapper/RHTimer...

class REWrapper {
    public:
        REWrapper(Encoder* enc);
        void update();

    private:

        void button_press();
        void button_long_press();
        void button_long_release();
        void rotation(int delta);
        
        unsigned long _last_action;
        const uint8_t push_cooldown = 200; //ms
        bool _long_press;
        bool _long_press_sent;
        int _rot_buffer;
        const uint8_t rot_delay = 500; // ms
        const uint8_t _long_press_delay = 500; //ms
        Encoder* _encoder;

};
