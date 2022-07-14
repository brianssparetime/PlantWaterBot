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
        
        unsigned long first_push;
        const unsigned long push_cooldown = 200UL; //ms
        bool prev_RE_button;
        int rot_buffer;
        unsigned long last_rot;
        const unsigned long rot_delay = 200UL; // ms
        const unsigned long _long_press_delay = 500UL; //mss
        bool _long_press;
        Encoder* encoder;

};
