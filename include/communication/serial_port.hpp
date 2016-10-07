// Created by wxk14, 2016.8
//  serial port at Windows
//
//  - SerialPortProtol(header_list, length): 简单的串口协议。header_list是可以接受的字符串头；length是协议规定的字符串长度。
//  - SerialPort(com, baud_rate, protol): 开启串口。给出串口号，波特率，串口协议。
//      - send(MyString): 发送字符串。
//      - receive(): 接受一次信息，并返回符合串口协议的所有字符串。

#pragma once

#include"communication/com.hpp"
#include"communication/my_string.hpp"

#include<vector>
#include<string>

class SerialPortProtol {

public:
    SerialPortProtol() 
        : no_protol(true)
    {
    }
    SerialPortProtol(std::vector<std::string> header_list, int length) 
        : header_list_(header_list)
        , length_(length)
        , no_protol(false)
    {
        if (length_ <= 0) no_protol = true;
    }

    ~SerialPortProtol() {
    }

    std::vector<MyString> decodeLines(char* line, int& len)
    {
        std::vector<MyString> list;
        if (len <= 0) return list;
        if (no_protol) {
            list.push_back(MyString(line, len));
            len = 0;
            return list;
        }

        int offset = 0;
        while (len - offset > length_) {
            if (checkHeader(line + offset)) {
                list.push_back(MyString(line + offset, length_));
                offset += length_;
            }
            else {
                offset += 1;
            }
        }
        for (int i = offset; i < len; ++i) line[i - offset] = line[i];
        len -= offset;

        return list;
    }

    inline int getLength() { return length_; }

private:
    bool no_protol;
    std::vector<std::string> header_list_;
    int length_;

    bool checkHeader(const char* line) {
        if (header_list_.size() == 0) return true;
        for (int i = 0; i < header_list_.size(); ++i)
            if (header_list_[i].compare(0, std::string::npos, line, header_list_[i].length()) == 0)
                return true;
        return false;
    }
};

class SerialPort {
public:
    SerialPort(int com, int baud_rate, SerialPortProtol protol = SerialPortProtol())
        : com_(com)
        , baud_rate_(baud_rate)
        , valid_(false)
        , protol_(protol)
        , total_length(0)
    {
        int result;
        
        // open com device
        result = OpenCOMDevice(com_, baud_rate_);
        if (result != 0) {
            printf("[Error] Failed to open COM%d\n", com_);
            return;
        }

        printf("[Info] COM%d launched.\n", com_);
        valid_ = true;
    }

    ~SerialPort() {
    }

    void send(const char* msg, int length) {
        SendUARTMessageLength(com_, msg, length);
    }
    void send(MyString& s) {
        SendUARTMessageLength(com_, s.c_str(), s.length());
    }

    std::vector<MyString> receive() {
        if (!valid_) {
            std::vector<MyString> list;
            printf("[Error] This serial port (com%d) is invalid.\n", com_);
            return list;
        }
        total_length += CollectUARTData(com_, buffer);
        return protol_.decodeLines(buffer, total_length);
    }

    inline bool isValid() { return valid_; }
    
private: 

    SerialPortProtol protol_;

    bool valid_;
    int com_;
    int baud_rate_;
    
    char buffer[32768];
    int total_length;

};
