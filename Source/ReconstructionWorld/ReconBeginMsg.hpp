#pragma once
#include"AbstractMessage.hpp"

class ReconBeginMsg : public AbstractMessage {
public:
	uint32_t reconPkgId = 0;

	ReconBeginMsg(uint32_t reconPkgId) : AbstractMessage("ReconBeginMsg") {
		this->reconPkgId = reconPkgId;
	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) override {
		// �����ؽ���package
		stream->writeData<uint32_t>(reconPkgId);
	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {

	}


};