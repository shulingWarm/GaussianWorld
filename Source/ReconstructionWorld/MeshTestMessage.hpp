#pragma once
#include"AbstractMessage.hpp"

class MeshTestMessage : public AbstractMessage {
public:
	MeshTestMessage() : AbstractMessage("MeshTestMessage") {

	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) override {
		// ��ֻ�Ƿ���һ�����󣬲���Ҫ�����κ���Ϣ
	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {

	}
};