#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <stdexcept>
#include<iostream>

#pragma comment(lib, "ws2_32.lib")

class WinSocketSolver {
public:
    using SocketType = SOCKET;  // Windows使用SOCKET类型

    // 初始化Winsock库（应在程序开始时调用一次）
    static void initialize() {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            throw std::runtime_error("WSAStartup failed");
        }
    }

    // 清理Winsock库（应在程序结束时调用一次）
    static void cleanup() {
        WSACleanup();
    }

    // 连接服务器
    static bool connect(const std::string& ipString,
        uint32_t port,
        SocketType& socket)
    {
        // 创建套接字
        socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (socket == INVALID_SOCKET) {
            return false;
        }

        // 设置服务器地址
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(static_cast<u_short>(port));

        // 转换IP地址
        if (inet_pton(AF_INET, ipString.c_str(), &serverAddr.sin_addr) <= 0) {
            closesocket(socket);
            socket = INVALID_SOCKET;
            return false;
        }

        // 连接到服务器
        if (::connect(socket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
            closesocket(socket);
            socket = INVALID_SOCKET;
            return false;
        }

        return true;
    }

    // 发送数据
    static uint32_t send(SocketType socket,
        const char* data,
        uint32_t dataSize)
    {
        int bytesSent = ::send(socket, data, static_cast<int>(dataSize), 0);
        if (bytesSent == SOCKET_ERROR) {
            return 0;
        }
        return static_cast<uint32_t>(bytesSent);
    }

    // 接收数据
    static uint32_t receive(SocketType socket,
        char* data,
        uint32_t maxDataSize)
    {
        int bytesReceived = ::recv(socket, data, static_cast<int>(maxDataSize), 0);
        if (bytesReceived <= 0) {  // 0表示连接关闭，负数表示错误
            return 0;
        }
        return static_cast<uint32_t>(bytesReceived);
    }

    // 关闭套接字
    static void close(SocketType& socket) {
        if (socket != INVALID_SOCKET) {
            closesocket(socket);
            socket = INVALID_SOCKET;
        }
    }
};