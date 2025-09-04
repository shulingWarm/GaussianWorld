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
	MessageManagerInterface* messageManager;
	Ptr<ImageEndOperation> imgEndOperation;

	LongArrayEndForImage(Ptr<ImageSolver> image,
		MessageManagerInterface* messageManager,
		Ptr<ImageEndOperation> imgEndOperation
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
void sendImage(Ptr<ImageSolver> image,
	MessageManagerInterface* messageManager, Ptr<ImageEndOperation> imgEndOperation) {

	// LongArray�������ʱ�Ļص�
	auto arrayFinishFunctor = makePtr<LongArrayEndForImage>(image,
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
class ImageListPerEndCallback : public ImageEndOperation, 
	public std::enable_shared_from_this<ImageListPerEndCallback> {
public:
	Ptr<ImageList> imgList;
	Ptr<SentPerImgFuncotr> perImgFunctor;
	// ����ͼƬ�������ʱ�Ļص�����
	Ptr<ImgListEndFunctor> imgListEndOp;
	MessageManagerInterface* msgManager;
	PackageMsgManager* pkgManager;
	// Ŀǰ�Ѿ�������ɵ�ͼƬ��
	uint32_t sentImgNum = 0;

	ImageListPerEndCallback(Ptr<ImageList> imgList, 
		Ptr<SentPerImgFuncotr> perImgFunctor,
		Ptr<ImgListEndFunctor> imgListEndOp,
		MessageManagerInterface* msgManager,
		PackageMsgManager* pkgManager
	) {
		this->imgList = imgList;
		this->perImgFunctor = perImgFunctor;
		this->imgListEndOp = imgListEndOp;
		this->msgManager = msgManager;
		this->pkgManager = pkgManager;
		sentImgNum = 0;
	}

	// ������һ��ͼƬ
	void sendNextImg() {
		// �ж��Ƿ��Ѿ����������е�ͼƬ
		if (this->sentImgNum == imgList->getImageNum()) {
			++this->sentImgNum;
			// ����img list���ͽ���ʱ�Ĳ���
			this->imgListEndOp->imgEndOp(this->imgList);
			return;
		}
		// ��һ��ͼƬ��id
		uint32_t nextId = this->sentImgNum;
		// ��ȡ��һ��image
		auto imgSolver = makePtr<ArrayImage>(FormatLibrary::convertToFString(
			this->imgList->imagePathList[nextId]
		));
		// ����ͼƬ
		sendImage(imgSolver, msgManager, shared_from_this());
	}

	virtual void imageEndOperation(Ptr<ImageSolver> image, uint32_t idPackage) override {
		// ����ÿ��ͼƬ������ʱ�ĺ���
		perImgFunctor->onPerImgSent(sentImgNum, idPackage);
		// �ͷ�package
		this->pkgManager->deletePackagInfo(idPackage);
		++sentImgNum;
		// ������һ��ͼƬ
		sendNextImg();
	}
};

// ����ͼƬ���б�
void sendImgList(Ptr<ImageList> imgList, 
	Ptr<SentPerImgFuncotr> perImgFunctor,
	MessageManagerInterface* msgManager,
	PackageMsgManager* pkgManager,
	Ptr<ImgListEndFunctor> imgListEndOp // ͼƬ���ͽ���ʱ��op
) {
	// ��ʼ��ÿ��ͼƬ�������ʱ�Ļص�
	auto imgListPerEnd = makePtr<ImageListPerEndCallback>(imgList, 
		perImgFunctor, imgListEndOp, msgManager, pkgManager);
	// ����img list������һ��ͼƬ
	imgListPerEnd->sendNextImg();
}