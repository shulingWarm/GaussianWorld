#pragma once
#include<iostream>
#include "Internationalization/Text.h"
#include "Misc/OutputDevice.h"
#include "Internationalization/Internationalization.h"
#include "Common/TcpSocketBuilder.h"

class UESocketSolver {
public:
    using SocketType = FSocket*;
    static void initialize() {

    }

    // ����Winsock�⣨Ӧ�ڳ������ʱ����һ�Σ�
    static void cleanup() {
    }

    // ���ӷ�����
    static bool connect(const std::string& ipString,
        uint32_t port,
        SocketType& socket)
    {
        //����ڲ���socketָ��
        TSharedPtr<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
        // ���ַ���ת����FString
        FString fStringIp(ipString.c_str());
        // ����ַ����ip
        bool ipValid;
        Addr->SetIp(*fStringIp, ipValid);
        // �������ʧ���ˣ�����������Ϣ
        if (ipValid) {
            UE_LOG(LogTemp, Warning, TEXT("ip set ok!"));
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("ip set failed!"));
            return false;
        }
        // ����ַ���ö˿ں�
        Addr->SetPort(23456);
        // �ѽ�����Ϣ����ʽ���ó�����״̬
        socket = FTcpSocketBuilder(TEXT("TcpClient")).AsBlocking().WithReceiveBufferSize(2 * 1024 * 1024);
        // ֪ͨsocket���е�ַ����
        bool connected = socket->Connect(*Addr);
        if (connected) {
            UE_LOG(LogTemp, Warning, TEXT("Connect ok!"));
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("Connect failed!"));
            return false;
        }
        return true;
    }

    // ��������
    static uint32_t send(SocketType socket,
        const char* data,
        uint32_t dataSize)
    {
        // ��ʵ�ʷ��͵ı�����
        int32_t sentByte;
        bool success = socket->Send((uint8*)data, dataSize, sentByte);
        if (!success) {
            UE_LOG(LogTemp, Warning, TEXT("Send failed"));
        }
        return sentByte;
    }

    // ��������
    static uint32_t receive(SocketType socket,
        char* data,
        uint32_t maxDataSize)
    {
        uint32 totalRead = 0;
        while (totalRead < maxDataSize) {
            // ��ȡ����
            int32 readBytes;
            socket->Recv((uint8*)(data + totalRead), maxDataSize - totalRead, readBytes);
            totalRead += readBytes;
        }
        return totalRead;
    }

    // �ر��׽���
    static void close(SocketType& socket) {
        socket->Close();
    }
};