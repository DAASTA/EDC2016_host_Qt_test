// Created by wxk14, 2016.8
//  为了解决在字符串中间传输0x00的问题，将string包装成了MyString。此类只用于通讯时的封装。
//
//  - MyString(string)
//  - MyString(char*, length): 从char*开始截断length个
//      - c_str(): 返回传统C风格的字符串(没有处理0x00)
//      - hex_str(): 返回十六进制输出的字符串(仅用于输出调试) //TODO

#pragma once

#include<string>
#include<cctype>

class MyString {
public:
    MyString(std::string s)
        : length_(s.length())
        , str_(s)
        , hex_str_(NULL)
    {
    }

    MyString(const char* line, int length)
        : length_(length)
        , hex_str_(NULL)
    {
        // copy
        str_ = std::string(length_, '\0');
        for (int i = 0; i < length_; ++i) str_[i] = line[i];
    }

    MyString(const MyString& ms) 
        : hex_str_(NULL)            // attention
        , length_(ms.length())
        , str_(ms.str_)
    {
    }

    ~MyString() {
        if (hex_str_ != NULL) delete[] hex_str_;
        hex_str_ = NULL;
    }

    const char* c_str() const {
        return str_.c_str();
    }
    const char* hex_str() {
        if (hex_str_ == NULL) {
            hex_str_ = new char[length_ * 3 + 1];
            if (length_ == 0) hex_str_[0] = '\0'; 
            for (int i = 0; i < length_; ++i)
                sprintf(hex_str_ + i * 3, "%02X ", (unsigned char)str_[i]);
        }
        return hex_str_;
    }

    const std::string str() const { return str_; }

    inline int length() const { return length_; }
    inline bool empty() const { return length_ == 0; }

    unsigned char operator[] (int i) const {
        if (i >= 0 && i < length_)
            return str_[i];
        else return '\0';
    }

    MyString operator+ (const MyString& s2) const {
        std::string s = str_ + s2.str_;
        return MyString(s.c_str(), length_ + s2.length_);
    }

private:
    int length_;
    std::string str_;
    char* hex_str_;
};
