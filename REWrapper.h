#pragma once
#include "Encoder.h"
#include "Machine.h"

// TODO:  I should either make this a singleton like Machine, or make everything static like LCD_Wrapper/RHTimer...

class REWrapper {
    public:
        REWrapper(Encoder enc);
        void update();

    private:

        void button_action();
        void rotate_action(int delta);
        
        long unsigned first_push;
        const int push_cooldown = 300; //ms
        bool prev_RE_button;
        int rot_buffer;
        long unsigned last_rot;
        const int rot_delay = 100; // ms
        Encoder* encoder;

};
