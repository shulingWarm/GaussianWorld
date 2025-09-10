#pragma once
#include"AbstractMessage.hpp"
#include"LoadFileLibrary.h"
#include"LongArrayPackage.hpp"
#include"FileGsSplat.hpp"
#include<string>
#include"Types.hpp"
#include"ReconstructionPackage.hpp"


class ReconResultMsg : public AbstractMessage {
public:
	// ���ص�3DGS�ļ���ʱ�����λ��
	static constexpr const char* GS_SAVE = "E:/temp/splat.ply";

	ReconResultMsg() : AbstractMessage("ReconResultMsg") {

	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) override {

	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {
		// ���� long array��package
		uint32_t idPackage = stream->readData<uint32_t>();
		// ����ԭʼ�������id
		uint32_t requestIdPackage = stream->readData<uint32_t>();
		// ��ȡlong array��package
		auto arrayPackage = (LongArrayPackage*)stream->getPackageManager()->getRemotePackage(idPackage);
		// ��package���������ֱ�Ӵ洢���ļ�
		ULoadFileLibrary::saveByteArrayAsFile(arrayPackage->dataArray,
			arrayPackage->byteNum, GS_SAVE);
		// requestʱ��package
		auto requestPackage = (ReconstructionPackage*)stream->getPackageManager()->getLocalInfo(requestIdPackage);
		// ���ô��ļ��ж�ȡ3DGS������
		auto gsSolver = makePtr<FileGsSplat>(std::string(GS_SAVE));
		// �����ؽ����ʱ�Ĳ���
		requestPackage->finishFunctor->onReconstructionFinish(gsSolver);
		// ɾ��package
		stream->getPackageManager()->deleteRemotePackage(idPackage);
		// ɾ��֮ǰ������package
		stream->getPackageManager()->deletePackagInfo(requestIdPackage);
	}
};