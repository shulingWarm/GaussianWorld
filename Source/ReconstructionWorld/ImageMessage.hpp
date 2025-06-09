#pragma once
#include "AbstractMessage.hpp"
#include "ImageSolver.hpp"
#include "ImagePackageInfo.hpp"

// ����ͼƬ��Ϣ������ͷ
// �������沢��������Ч������������Ϣ
class ImageMessage : public AbstractMessage{
public:
	//��Ϣ�й������Ϣ
	ImageSolver* image;

	//������ʱ����Ҫ����һ��ͼƬ��Ϣ
	ImageMessage(ImageSolver* image) : AbstractMessage("ImageMessage") {
		this->image = image;
	}

	//������Ϣ���߼�
	void send(StreamInterface* stream) override {
		// ע��ְ�����
		uint32_t packageId = stream->getPackageManager()->registerPackageTask(
			new ImagePackageInfo(this->image)
		);
		//����ͼƬ�İ�id
		stream->writeData<uint32_t>(packageId);
		//����ͼƬ�����Ŀ����Ϣ
		stream->writeData<uint32_t>(image->getWidth());
		stream->writeData<uint32_t>(image->getHeight());
	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {

	}
};