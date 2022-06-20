#pragma once
#include "Encoder.h"
#include "Machine.h"

// TODO:  I should either make this a singleton like Machine, or make everything static like LCD_Wrapper/RHTimer...

class REWrapper {
    public:
        REWrapper(Encoder* enc);
        void update();

    private:

        void button_action();
        void rotate_action(int delta);
        
        unsigned long first_push;
        const unsigned long push_cooldown = 300UL; //ms
        bool prev_RE_button;
        int rot_buffer;
        unsigned long last_rot;
        const unsigned long rot_delay = 300UL; // ms
        Encoder* encoder;

};
