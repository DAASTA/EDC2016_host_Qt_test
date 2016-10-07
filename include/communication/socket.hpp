// Created by wxk14, 2016.8
//  C++ socket at Windows
//
//  - SocketBase: 加载和释放WinSock2需要的资源
//  - SocketServer(port, function): 建立UDP服务器，监听本地port端口，并设置字符串处理函数function自动处理收到的字符串并返回回复的字符串。
//      - run(): 启动服务器，新建线程进行循环监听。注意这里可能出现的线程安全问题。
//      - stop(): 终止服务器。
//  - SocketClient(port, ip="127.0.0.1"): 建立UDP客户端
//      - send(MyString): 发送信息到指定端口。
//      - MyString sendAndRecv(MyString): 发送信息并立即接受返回信息到echo。此函数是堵塞的，默认等待时间为20ms。

#pragma once

#include<cstdio>
#include<exception>

#include<WinSock2.h>
#include<windows.h>

#include"communication/my_string.hpp"

#pragma comment(lib, "ws2_32.lib")

class SocketBase {
public:
    SocketBase() {
        if (count == 0) StartUp();
        ++count;
    }
    ~SocketBase() {
        --count;
        if (count == 0) Finish();
    }

    static int getSocketCount() { return count; }

private:
    static void StartUp() {
        // start up WSA
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            printf("[Error] Failed to start up WSA.\n");
            return;
        }
        printf("[Info] WSA launched.\n");
    }
    static void Finish() {
        WSACleanup();
        printf("[Info] WSA Clean up.\n");
    }
    static int count;
};
int SocketBase::count = 0;

class SocketServer : SocketBase {
public:
    // 定义在收到字符串时进行处理的函数 SlotFunction
    typedef MyString (*SlotFunction)(MyString);

    SocketServer(int PORT, SlotFunction function)
        : valid_(false)
        , running_(false)
        , function_(function)
        , socket(INVALID_SOCKET)
        , port_(PORT)
    {
        try {
            int result;

            // init socket
            socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
            sockaddr_in addr;
            addr.sin_addr.S_un.S_addr = INADDR_ANY;
            addr.sin_family = AF_INET;
            addr.sin_port = ntohs(PORT);
            if (socket == INVALID_SOCKET) {
                printf("[Error] Failed to init socket.\n");
                return;
            }

            // bind socket
            result = bind(socket, (sockaddr*)&addr, sizeof(addr));
            if (result != 0) {
                printf("[Error] Failed to bind socket at port %d.\n", PORT);
                return;
            }

        }
        catch (const std::exception &e) {
            printf("[Error] Unknown exception when init SocketServer: %s\n ", e.what());
            valid_ = false;
            return;
        }

        printf("[Info] Server at %d init finished.\n", port_);
        valid_ = true;
    }

    ~SocketServer() {
        if (running_) stop();
        if (socket != INVALID_SOCKET) closesocket(socket);
    }

    void run() { 
        if (running_) {
            printf("[Warning] The server has been running.\n");
            return;
        }
        running_ = true;
        
        HANDLE hThread = CreateThread(NULL, 0, MainLoop, (LPVOID)this, 0, NULL);
        if (hThread == NULL) {
            printf("[Warning] Failed to create thred for MainLoop.\n");
            return;
        }
        // close handle
        CloseHandle(hThread);
    }
    void stop() { 
        running_ = false; 
        printf("[Info] The server has been paused.\n");
    }

    bool setSlotFunction(SlotFunction function) { function_ = function; }    
    inline int getPort() { return port_; }
    inline bool isValid() { return valid_; }
    inline bool isRunning() { return running_; }
    inline SlotFunction getFunction() { return function_; }

private:
    static DWORD WINAPI MainLoop(LPVOID lpParameter) {
        SocketServer *socket_server = (SocketServer*)lpParameter;
        return socket_server->MainLoopKernel();
    }
    DWORD MainLoopKernel() {
        if (!valid_ || !running_ || socket == INVALID_SOCKET) return -1;

        char msg[8192] = "";
        int len;

        sockaddr_in addrcl;
        int fromlen = sizeof(sockaddr);

        printf("[Info] SocketServer kernel launched.\n");

        while (valid_ && running_) {
            try {
                len = recvfrom(socket, msg, sizeof(msg), 0, (sockaddr*)&addrcl, &fromlen);
                MyString m_msg(msg, len);
                printf("[Debug] receive from %s:%d : %s\n", inet_ntoa(addrcl.sin_addr), addrcl.sin_port, m_msg.c_str());
                MyString m_echo = function_(m_msg);
                if (!m_echo.empty()) {
                    sendto(socket, m_echo.c_str(), m_echo.length(), 0, (sockaddr*)&addrcl, sizeof(sockaddr_in));
                }
            }
            catch (const std::exception e) {
                printf("[Warning] Exception catched at MainLoop: %s\n", e.what());
                continue;
            }
        }

        return 0;
    }

    SOCKET socket;
    int port_;
    bool running_;
    bool valid_;
    SlotFunction function_;
};

class SocketClient : SocketBase {
public:
    SocketClient(int PORT, const char* IP_ADDRESS = "127.0.0.1")
        : valid_(false)
    {
        try {
            int result;

            // init socket
            socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
            if (socket == INVALID_SOCKET) {
                printf("[Error] (SocketClient) Failed to init socket.\n");
                return;
            }

            // set up addr
            strcpy(ip_address_, IP_ADDRESS);
            addr.sin_addr.S_un.S_addr = inet_addr(ip_address_);
            addr.sin_family = AF_INET;
            addr.sin_port = ntohs(PORT);

            // time
            wait_time_ = 20;
            result = setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&wait_time_), sizeof(int));
            if (result != 0) {
                printf("[Error] (SocketClient) Cannot set recv time limit.\n");
                return;
            }
        }
        catch (const std::exception &e) {
            printf("[Error] Unknown exception when init SocketServer: %s\n ", e.what());
            valid_ = false;
            return;
        }

        valid_ = true;
        
    }
    
    ~SocketClient() {
        if (socket != INVALID_SOCKET) closesocket(socket);
    }

    void send(MyString s) {
        if (!valid_) {
            printf("[Warning] This SocketClient is not valid!\n");
            return;
        }

        try {
            sendto(socket, s.c_str(), s.length(), 0, (SOCKADDR*)&addr, sizeof(sockaddr));
        }
        catch (std::exception e) {
            printf("[Error] Failed to send the message.\n");
            valid_ = false;
            return;
        }
    }
    MyString sendAndRecv(MyString msg) {
        MyString null_s("");
        
        if (valid_) send(msg);
        if (!valid_) {
            printf("[Warning] This SocketClient is not valid!\n");
            return null_s;
        }

        int len;
        int fromlen = sizeof(sockaddr);
        char echo[8192];

        try {
            len = recvfrom(socket, echo, 8192, 0, (sockaddr*)&addr, &fromlen);
        }
        catch (const std::exception& e) {
            printf("[Error] (SocketClient) Exception catched when recv : %s\n",e.what());
            valid_ = false;
            return null_s;
        }

        if (len <= 0) {
            printf("[Warning] (SocketClient) No info received. \n");
            return null_s;
        }

        return MyString(echo, len);
    }

    inline bool isValid() { return valid_; }
    inline int getPort() { return port_; }
    inline char* getIpAddress() { return ip_address_; }

    bool setWaitTime(int tv) {
        wait_time_ = tv;
        try {
            int result = setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&wait_time_), sizeof(wait_time_));
            if (result != 0) {
                printf("[Error] (SocketClient) Cannot set recv time limit.");
                valid_ = false;
                return false;
            }
        }
        catch(const std::exception& e) {
            printf("[Error] (SocketClient) Exception catched when setWaitTime : %s\n", e.what());
            valid_ = false;
            return false;
        }
        return true;
    }

private:
    SOCKET socket;
    sockaddr_in addr;
    
    int wait_time_;
    bool valid_;
    int port_;
    char ip_address_[128];
    
};