#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <stdexcept>
#include<iostream>

#pragma comment(lib, "ws2_32.lib")

class WinSocketSolver {
public:
    using SocketType = SOCKET;  // Windowsʹ��SOCKET����

    // ��ʼ��Winsock�⣨Ӧ�ڳ���ʼʱ����һ�Σ�
    static void initialize() {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            throw std::runtime_error("WSAStartup failed");
        }
    }

    // ����Winsock�⣨Ӧ�ڳ������ʱ����һ�Σ�
    static void cleanup() {
        WSACleanup();
    }

    // ���ӷ�����
    static bool connect(const std::string& ipString,
        uint32_t port,
        SocketType& socket)
    {
        // �����׽���
        socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (socket == INVALID_SOCKET) {
            return false;
        }

        // ���÷�������ַ
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(static_cast<u_short>(port));

        // ת��IP��ַ
        if (inet_pton(AF_INET, ipString.c_str(), &serverAddr.sin_addr) <= 0) {
            closesocket(socket);
            socket = INVALID_SOCKET;
            return false;
        }

        // ���ӵ�������
        if (::connect(socket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
            closesocket(socket);
            socket = INVALID_SOCKET;
            return false;
        }

        return true;
    }

    // ��������
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

    // ��������
    static uint32_t receive(SocketType socket,
        char* data,
        uint32_t maxDataSize)
    {
        int bytesReceived = ::recv(socket, data, static_cast<int>(maxDataSize), 0);
        if (bytesReceived <= 0) {  // 0��ʾ���ӹرգ�������ʾ����
            return 0;
        }
        return static_cast<uint32_t>(bytesReceived);
    }

    // �ر��׽���
    static void close(SocketType& socket) {
        if (socket != INVALID_SOCKET) {
            closesocket(socket);
            socket = INVALID_SOCKET;
        }
    }
};