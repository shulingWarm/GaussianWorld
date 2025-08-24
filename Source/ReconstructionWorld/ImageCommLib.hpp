#pragma once
#include"ImageEndMessage.hpp"
#include"LongArrayMessage.hpp"
#include"ImageSolver.hpp"
#include"ImageEndOperation.hpp"

// ����ͼƬʱ��LongArray��ɵĻص�
class LongArrayEndForImage : public LongArrayFinishFunctor {
public:
	ImageSolver* image;
	MessageManager* messageManager;
	ImageEndOperation* imgEndOperation;

	LongArrayEndForImage(ImageSolver* image,
		MessageManager* messageManager,
		ImageEndOperation* imgEndOperation
	) {
		this->image = image;
		this->messageManager = messageManager;
		this->imgEndOperation = imgEndOperation;
	}

	virtual void arrayFinishProcess(PackageMsgManager* packageManager,
		uint32_t idPackage) override {
		// ��ȡͼƬ�Ŀ��
		uint32_t imgWidth = image->getWidth();
		uint32_t imgHeight = image->getHeight();
		// ע��ͼƬ����Ϣ
		// ���������ע��һ���յ�package�Ϳ�����
		uint32_t imagePackageId = packageManager->registerPackageTask(
			new IdOnlyImagePackage()
		);
		// ͼƬ��ɵ���Ϣ
		ImageEndMessage imgEndMessage(idPackage, imgWidth, imgHeight, imagePackageId);
		messageManager->sendMessage(imgEndMessage);
		// ִ��ͼƬ������ɵ���Ϣ
		imgEndOperation->imageEndOperation(image, imagePackageId);
	}
};

// ��ͼƬ���͵�Զ��
void sendImage(ImageSolver* image, StreamInterface* stream, 
	MessageManager* messageManager, ImageEndOperation* imgEndOperation) {

	// LongArray�������ʱ�Ļص�
	auto arrayFinishFunctor = new LongArrayEndForImage(image,
		messageManager, imgEndOperation);

	// ��ͼƬ����ȡ��ָ��
	uint8_t* imgData = image->getRowData(0);
	// ��ȡ�������ֽ���
	uint32_t byteNum = image->getWidth() * image->getHeight() * 4;
	// ����LongArray message
	LongArrayMessage arrayMessage((char*)imgData, byteNum, arrayFinishFunctor);
	messageManager->sendMessage(arrayMessage);
}