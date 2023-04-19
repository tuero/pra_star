// File: timer.h
// Description: Measures user space time and signals a timeout

#ifndef PRA_UTIL_TIMER_H
#define PRA_UTIL_TIMER_H

#include <ctime>

namespace tpl_search {

class Timer {
    // Ensure clock_t is a 64 bit value
    // A 32 bit clock_t width will overflow after ~72 minutes which is longer than the expected runtime.
    // A 64 bit clock_t width will overflow after ~ 300,00 years
    static_assert(sizeof(std::clock_t) == 8);

public:
    Timer() = default;
    Timer(double seconds_limit) : seconds_limit(seconds_limit) {}

    void start() {
        cpu_start_time = std::clock();
    }

    bool is_timeout() {
        std::clock_t cpu_current_time = std::clock();
        double current_duration = static_cast<double>(cpu_current_time - cpu_start_time) / CLOCKS_PER_SEC;
        return seconds_limit > 0 && current_duration >= seconds_limit;
    }

    double get_duration() {
        std::clock_t cpu_current_time = std::clock();
        double current_duration = static_cast<double>(cpu_current_time - cpu_start_time) / CLOCKS_PER_SEC;
        return current_duration;
    }

    double get_time_remaining() {
        std::clock_t cpu_current_time = std::clock();
        double current_duration = static_cast<double>(cpu_current_time - cpu_start_time) / CLOCKS_PER_SEC;
        return seconds_limit - current_duration;
    }

private:
    double seconds_limit = -1;
    std::clock_t cpu_start_time = 0;
};

class ThreadTimer {
public:
    ThreadTimer() = default;
    ThreadTimer(double seconds_limit) : seconds_limit(seconds_limit) {}

    void start() {
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start_time);
    }

    bool is_timeout() {
        return seconds_limit > 0 && get_duration() >= seconds_limit;
    }

    bool is_timeout(double &current_duration) {
        current_duration = get_duration();
        return seconds_limit > 0 && current_duration >= seconds_limit;
    }

    double get_duration() {
        struct timespec current_time;
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &current_time);
        return static_cast<double>(current_time.tv_sec - start_time.tv_sec) +
               static_cast<double>(current_time.tv_nsec - start_time.tv_nsec) / 1e9;
    }

    double get_time_remaining() {
        struct timespec current_time;
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &current_time);
        double current_duration = static_cast<double>(current_time.tv_sec - start_time.tv_sec) +
                                  static_cast<double>(current_time.tv_nsec - start_time.tv_nsec) / 1e9;
        return seconds_limit - current_duration;
    }

private:
    double seconds_limit = -1;
    struct timespec start_time;
};

}    // namespace tpl_search

#endif    // PRA_UTIL_TIMER_H
