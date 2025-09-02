#pragma once
#include"ImageEndMessage.hpp"
#include"LongArrayMessage.hpp"
#include"ImageSolver.hpp"
#include"ImageEndOperation.hpp"
#include"ImageList.hpp"
#include"ImageListPkg.hpp"
#include"FormatLibrary.h"
#include"Types.hpp"

// ����ͼƬʱ��LongArray��ɵĻص�
class LongArrayEndForImage : public LongArrayFinishFunctor {
public:
	Ptr<ImageSolver> image;
	MessageManager* messageManager;
	ImageEndOperation* imgEndOperation;

	LongArrayEndForImage(Ptr<ImageSolver> image,
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
void sendImage(Ptr<ImageSolver> image, StreamInterface* stream,
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

// ���image�ķ������ʱ�Ļص�
class ImageListPerEndCallback : public ImageEndOperation {
public:
	Ptr<ImageList> imgList;
	Ptr<SentPerImgFuncotr> perImgFunctor;
	MessageManager* msgManager;
	// Ŀǰ�Ѿ�������ɵ�ͼƬ��
	uint32_t sentImgNum = 0;

	ImageListPerEndCallback(Ptr<ImageList> imgList, 
		Ptr<SentPerImgFuncotr> perImgFunctor,
		MessageManager* msgManager
	) {
		this->imgList = imgList;
		this->perImgFunctor = perImgFunctor;
		this->msgManager = msgManager;
		sentImgNum = 0;
	}

	// ������һ��ͼƬ
	void sendNextImg() {
		// ��һ��ͼƬ��id
		uint32_t nextId = this->sentImgNum;
		// ��ȡ��һ��image
		auto imgSolver = makePtr<ArrayImage>(FormatLibrary::convertToFString(
			this->imgList->imagePathList[nextId]
		));
		// ����ͼƬ

	}

	virtual void imageEndOperation(Ptr<ImageSolver> image, uint32_t idPackage) override {

	}
};

// ����ͼƬ���б�
void sendImgList(Ptr<ImageList> imgList, 
	Ptr<SentPerImgFuncotr> perImgFunctor,
	MessageManager* msgManager
) {
	// ��ʼ��ÿ��ͼƬ�������ʱ�Ļص�
	auto imgListPerEnd = makePtr<ImageListPerEndCallback>(imgList, 
		perImgFunctor, msgManager);
	// ����img list������һ��ͼƬ

}