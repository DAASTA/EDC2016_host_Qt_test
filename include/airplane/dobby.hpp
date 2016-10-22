#pragma once

#include"logic/Common.h"
#include"logic/Random.hpp"

class Dobby {

public:        

    Dobby()
        : pos_(128, 128)
        , target_(128, 128)
    { }

    inline void SetTarget(Point p) { target_ = p; }
    
    inline Point GetPos() {
        int dx = random.Rand(3);
        int dy = random.Rand(3);

        if (pos_.x > target_.x) dx = -dx;
        if (pos_.y > target_.y) dy = -dy;

        int x = dx + pos_.x;
        int y = dy + pos_.y;

        if (x < 0) x = 0; 
        if (x > 255) x = 255;
        if (y < 0) y = 0;
        if (y > 255) y = 255;        

        pos_.x = x;
        pos_.y = y;
        
        return pos_;
    }

private:

    Point pos_;
    Point target_;

    Random random;

};