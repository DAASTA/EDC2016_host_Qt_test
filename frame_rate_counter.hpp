// Created by sugar10w, 2016.8
//    a basic Frame Rate Counter
//    call getFrameRate() each frame to update.

#pragma once

#include <ctime>

class FrameRateCounter {
public:
    FrameRateCounter() {
        last_time = clock();
    }
    
    int getFrameRate() {
        int FrameRate = 0;
        time_t curr_time = clock();
        if (last_time < curr_time) FrameRate = 1000 / (curr_time - last_time);
        last_time = curr_time;
        return FrameRate;
    }

private:
    time_t last_time;
};