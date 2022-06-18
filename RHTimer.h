#pragma once


// purely static

class RoughHoursTimer {
    public:
        static void start();
        static void start(int interval);
        static int get_h_remaining();
        static int get_m_remaining();
        static void update();
        static int get_current_interval();

    private:

        static void alarm();

        static unsigned long next_min_millis;
        static int minutes_elapsed;
        static int hours_elapsed;
        static int cur_interval;
};