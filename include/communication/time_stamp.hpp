#pragma once

#include<windows.h>
#include<string>

class TimeStamp {
public:

static char* getTimeStamp(char* buffer) {
    SYSTEMTIME sys;
    GetLocalTime(&sys);
    sprintf(buffer, "[%02d:%02d:%02d.%03d]", sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
    return buffer;
}

static std::string getTimeStamp() {
    char buffer[128];
    getTimeStamp(buffer);
    return std::string(buffer);
}

};