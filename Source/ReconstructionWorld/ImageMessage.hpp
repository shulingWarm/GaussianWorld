#pragma once
#include "AbstractMessage.hpp"
#include "ImageSolver.hpp"
#include "ImagePackageInfo.hpp"
#include"ImageEndOperation.hpp"

// ����ͼƬ��Ϣ������ͷ
// �������沢��������Ч������������Ϣ
class ImageMessage : public AbstractMessage{
public:
	//��Ϣ�й������Ϣ
	ImageSolver* image;
	//ͼƬ������ɺ����Ϣ
	ImageEndOperation* imageEndOperation;

	//������ʱ����Ҫ����һ��ͼƬ��Ϣ
	ImageMessage(ImageSolver* image,
		ImageEndOperation* imageEndOperation //ͼƬ������ɺ��operation
	) : AbstractMessage("ImageMessage") {
		this->image = image;
		// ��¼ͼƬ������ɺ����Ϣ
		this->imageEndOperation = imageEndOperation;
	}

	//������Ϣ���߼�
	void send(StreamInterface* stream) override {
		// ע��ְ�����
		uint32_t packageId = stream->getPackageManager()->registerPackageTask(
			new ImagePackageInfo(this->image, this->imageEndOperation)
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