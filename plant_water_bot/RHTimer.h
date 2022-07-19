#pragma once


// purely static

class RHTimer {
    public:
        static void start();
        static void start(int interval);
        static int get_h_remaining();
        static int get_m_remaining();
        static int get_s_remaining();
        static void update();
        static int get_current_interval();

    private:

        static void alarm();

        static unsigned long _next_min_millis;
        static int _minutes_elapsed;
        static int _hours_elapsed;
        static int _cur_interval;
};