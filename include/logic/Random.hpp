/*
 * 随机数生成
 */

#pragma once

#include<cstdlib>
#include<ctime>

static bool Random_initialized = false;

class Random
{
public:
    Random() {
        if (!Random_initialized) {
            srand((unsigned int)time(NULL));
            Random_initialized = true;
        }
    }
    int Rand() { return rand(); }
};

