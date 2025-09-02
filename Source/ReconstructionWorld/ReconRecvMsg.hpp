#pragma once
#include"AbstractMessage.hpp"
#include"ReconstructionPackage.hpp"
#include"ReconSingleImgMsg.hpp"
#include"ImageListPkg.hpp"

class ReconRecvMsg : public AbstractMessage{
public:
	ReconRecvMsg() : AbstractMessage("ReconRecvMsg") {

	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) override {

	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {
		// ������Ϣ��
		uint32_t idPackage = stream->readData<uint32_t>();
		// ��ȡ��Ӧ����Ϣ
		auto pkgManager = stream->getPackageManager();
		auto tempPackage = (ReconstructionPackage*)pkgManager->getLocalInfo(idPackage);
		// ���췢����ÿ��ͼƬʱ�Ļص�
		auto sendPerImageCallback = [stream, messageManager, tempPackage](uint32_t idImage, 
			uint32_t imagePackage, uint32_t parentImgListPackageId) {
			// ���͵���ͼƬ������ɵ���Ϣ
			ReconSingleImgMsg singleImgMsg(idImage, imagePackage);
			messageManager->sendMessage(singleImgMsg);
		};
		// ���췢��ͼƬ���е���Ϣ��
		auto imgListPkg = new ImageListPkg(tempPackage->imageList, 
			new SentPerImgLambda(sendPerImageCallback), false
		);
		// ע�ᷢ��ͼƬ���е���Ϣ��
		uint32_t imgListPkgId = pkgManager->registerPackageTask(imgListPkg);
		// ���췢��ͼƬʱ����Ϣ

	}
};