#pragma once
#include"Types.hpp"
#include <fstream>
#include <string>
#include <cstdint> // ���� uint32_t �Ķ���
#include"LongArrayMessage.hpp"
#include"MessageManager.hpp"

char* readDatasInFile(std::string filePath, uint32_t& size) {
    // ���ļ�����λ��ĩβ�Ի�ȡ�ļ���С
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        size = 0;  // �ļ���ʧ�ܣ����ô�СΪ0
        return nullptr;
    }

    // ��ȡ�ļ���С
    size = static_cast<uint32_t>(file.tellg());
    file.seekg(0, std::ios::beg);  // �ص��ļ���ͷ

    // ����ļ���С�Ƿ�Ϊ0
    if (size == 0) {
        return nullptr;  // ���ļ�����nullptr
    }

    // �����ڴ滺����
    char* buffer = new (std::nothrow) char[size];
    if (!buffer) {
        size = 0;  // �ڴ����ʧ�ܣ����ô�СΪ0
        return nullptr;
    }

    // һ���Զ�ȡ�����ļ�
    if (!file.read(buffer, size)) {
        delete[] buffer;  // ��ȡʧ��ʱ�ͷ��ڴ�
        size = 0;         // ���ô�СΪ0
        return nullptr;
    }

    return buffer;
}

// ������ʵ�ǰ��ļ�ͨ�������鷢�ͳ�ȥ
void sendFile(std::string filePath, MessageManagerInterface* messageManager,
    Ptr<LongArrayFinishFunctor> finishFunctor
) {
    // һ���Զ�ȡ����������
    uint32_t size;
    auto dataArray = readDatasInFile(filePath, size);
    if (size == 0) {
        throw std::runtime_error("Empty data for send");
    }
    // long array����Ϣ
    LongArrayMessage longArrayMsg(dataArray, size, finishFunctor, true);
    // ���÷���long aray���߼�
    messageManager->sendMessage(longArrayMsg);
}