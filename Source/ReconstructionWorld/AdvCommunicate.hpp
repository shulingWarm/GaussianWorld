#pragma once
#include<string>
#include<iostream>
#include"UESocketSolver.hpp"
#include"StreamInterface.hpp"

//Socket��ʵ�ַ�������ΪUE�İ汾��C++�Ļ����汾
using SocketSolver = UESocketSolver;

class AdvCommunication : public StreamInterface{
public:
	//�����ʱ��û�в���

	//�ж��Ƿ��Ѿ�����
	bool connectFlag = false;

	//�Ѿ������õ�socket����
	SocketSolver::SocketType socket;

	//��ʼ��ϵͳsocket����
	static void initSystemSocket() {
		SocketSolver::initialize();
	}

	//��Ŀ���������������
	bool connect(std::string ipString, uint32_t port) {
		//����������socket����
		connectFlag = SocketSolver::connect(ipString, port, socket);
		return connectFlag;
	}

	//д������
	//�����Ҫ�Ż����Զ��ְ�����ķ�ʽ�����ھ�ֱ�Ӵ���Ϳ�����
	void send(const char* data, uint32_t dataSize) {
		//���Ŀǰû�����ӣ�����Ҫ����
		if (!connectFlag) {
			throw std::runtime_error("��δ���ӣ����ܵ���send");
		}
		uint32_t sentSize = SocketSolver::send(socket, data, dataSize);
		if (sentSize < dataSize) {
			throw std::runtime_error("����û����������");
		}
	}

	//��������
	void receive(char* data, uint32_t dataSize) {
		SocketSolver::receive(socket, data, dataSize);
	}
};