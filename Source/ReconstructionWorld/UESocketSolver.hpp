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

    // 清理Winsock库（应在程序结束时调用一次）
    static void cleanup() {
    }

    // 连接服务器
    static bool connect(const std::string& ipString,
        uint32_t port,
        SocketType& socket)
    {
        //获得内部的socket指针
        TSharedPtr<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
        // 把字符串转换成FString
        FString fStringIp(ipString.c_str());
        // 给地址设置ip
        bool ipValid;
        Addr->SetIp(*fStringIp, ipValid);
        // 如果设置失败了，报个错误信息
        if (ipValid) {
            UE_LOG(LogTemp, Warning, TEXT("ip set ok!"));
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("ip set failed!"));
            return false;
        }
        // 给地址设置端口号
        Addr->SetPort(23456);
        // 把接收消息的形式设置成阻塞状态
        socket = FTcpSocketBuilder(TEXT("TcpClient")).AsBlocking().WithReceiveBufferSize(2 * 1024 * 1024);
        // 通知socket进行地址连接
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

    // 发送数据
    static uint32_t send(SocketType socket,
        const char* data,
        uint32_t dataSize)
    {
        // 被实际发送的比特数
        int32_t sentByte;
        bool success = socket->Send((uint8*)data, dataSize, sentByte);
        if (!success) {
            UE_LOG(LogTemp, Warning, TEXT("Send failed"));
        }
        return sentByte;
    }

    // 接收数据
    static uint32_t receive(SocketType socket,
        char* data,
        uint32_t maxDataSize)
    {
        uint32 totalRead = 0;
        while (totalRead < maxDataSize) {
            // 读取数据
            int32 readBytes;
            socket->Recv((uint8*)(data + totalRead), maxDataSize - totalRead, readBytes);
            totalRead += readBytes;
        }
        return totalRead;
    }

    // 关闭套接字
    static void close(SocketType& socket) {
        socket->Close();
    }
};