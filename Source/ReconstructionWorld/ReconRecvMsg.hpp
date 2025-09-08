#pragma once
#include"AbstractMessage.hpp"
#include"ReconstructionPackage.hpp"
#include"ReconSingleImgMsg.hpp"
#include"ImageListPkg.hpp"
#include"ImageCommLib.hpp"
#include"ReconBeginMsg.hpp"
#include"ReconSingleFileImg.hpp"

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
		auto sendPerImageCallback = [stream, messageManager, idPackage](uint32_t idImage, 
			uint32_t imageArrayPackage, uint32_t imgFmt) {
			// �ļ���ʽ�ĵ���ͼƬ������ɵ���Ϣ
			ReconSingleFileImg fileImgMsg(imageArrayPackage, imgFmt, idPackage);
			messageManager->sendMessage(fileImgMsg);
		};
		// ���е�image�������ʱ�Ļص�
		auto imgListEndFunctor = makePtr<ImgListEndLambda>([idPackage, messageManager] (Ptr<ImageList> imgList) {
			// ����������Ϣ�Ѿ���ɵ�֪ͨ
			ReconBeginMsg reconBeginMsg(idPackage);
			messageManager->sendMessage(reconBeginMsg);
		});
		// ����ÿ��ͼƬ֮��Ļص�
		auto perImgFunctor = makePtr<SentPerImgLambda>(sendPerImageCallback);
		// ���췢��ͼƬʱ����Ϣ
		sendImgListByFile(tempPackage->imageList, perImgFunctor, messageManager,
			pkgManager, imgListEndFunctor);
	}
};