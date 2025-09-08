#pragma once
#include"Types.hpp"
#include <fstream>
#include <string>
#include <cstdint> // 包含 uint32_t 的定义
#include"LongArrayMessage.hpp"
#include"MessageManager.hpp"

char* readDatasInFile(std::string filePath, uint32_t& size) {
    // 打开文件并定位到末尾以获取文件大小
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        size = 0;  // 文件打开失败，设置大小为0
        return nullptr;
    }

    // 获取文件大小
    size = static_cast<uint32_t>(file.tellg());
    file.seekg(0, std::ios::beg);  // 回到文件开头

    // 检查文件大小是否为0
    if (size == 0) {
        return nullptr;  // 空文件返回nullptr
    }

    // 分配内存缓冲区
    char* buffer = new (std::nothrow) char[size];
    if (!buffer) {
        size = 0;  // 内存分配失败，设置大小为0
        return nullptr;
    }

    // 一次性读取整个文件
    if (!file.read(buffer, size)) {
        delete[] buffer;  // 读取失败时释放内存
        size = 0;         // 设置大小为0
        return nullptr;
    }

    return buffer;
}

// 这里其实是把文件通过长数组发送出去
void sendFile(std::string filePath, MessageManagerInterface* messageManager,
    Ptr<LongArrayFinishFunctor> finishFunctor
) {
    // 一次性读取完整的内容
    uint32_t size;
    auto dataArray = readDatasInFile(filePath, size);
    if (size == 0) {
        throw std::runtime_error("Empty data for send");
    }
    // long array的消息
    LongArrayMessage longArrayMsg(dataArray, size, finishFunctor, true);
    // 调用发送long aray的逻辑
    messageManager->sendMessage(longArrayMsg);
}