#pragma once
#include"AbstractMessage.hpp"

// ���ڷ���ͼƬ���е���Ϣ
// ��������ǿ��������˵�
class ImgListMsg : public AbstractMessage {
public:
	uint32_t imgListPkgId;

	ImgListMsg(uint32_t imgListPkgId) : AbstractMessage("ImgListMsg") {
		this->imgListPkgId = imgListPkgId;
	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) {
		// ����ImgList��
	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) = 0;
};