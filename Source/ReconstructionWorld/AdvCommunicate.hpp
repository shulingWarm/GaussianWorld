#pragma once
#include<string>
#include<iostream>
#include"UESocketSolver.hpp"
#include"StreamInterface.hpp"

//Socket的实现方案，分为UE的版本和C++的基础版本
using SocketSolver = UESocketSolver;

class AdvCommunication : public StreamInterface{
public:
	//构造的时候并没有参数

	//判断是否已经连接
	bool connectFlag = false;

	//已经建立好的socket连接
	SocketSolver::SocketType socket;

	//初始化系统socket环境
	static void initSystemSocket() {
		SocketSolver::initialize();
	}

	//与目标服务器建立连接
	bool connect(std::string ipString, uint32_t port) {
		//建立基本的socket连接
		connectFlag = SocketSolver::connect(ipString, port, socket);
		return connectFlag;
	}

	//写入数据
	//这后续要优化成自动分包传输的方式，现在就直接传输就可以了
	void send(const char* data, uint32_t dataSize) {
		//如果目前没有连接，就需要报错
		if (!connectFlag) {
			throw std::runtime_error("尚未连接，不能调用send");
		}
		uint32_t sentSize = SocketSolver::send(socket, data, dataSize);
		if (sentSize < dataSize) {
			throw std::runtime_error("数据没能完整发送");
		}
	}

	//接收数据
	void receive(char* data, uint32_t dataSize) {
		SocketSolver::receive(socket, data, dataSize);
	}
};