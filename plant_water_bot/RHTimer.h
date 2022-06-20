#pragma once


// purely static

class RHTimer {
    public:
        static void start();
        static void start(int interval);
        static int get_h_remaining();
        static int get_m_remaining();
        static void update();
        static int get_current_interval();

        static const int intervals[5];
        static const int intervals_size = 5;

    private:

        static void alarm();

        static unsigned long next_min_millis;
        static int minutes_elapsed;
        static int hours_elapsed;
        static int cur_interval;
};