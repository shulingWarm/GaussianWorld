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
	// 加载的3DGS文件临时保存的位置
	static constexpr const char* GS_SAVE = "E:/temp/splat.ply";

	ReconResultMsg() : AbstractMessage("ReconResultMsg") {

	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) override {

	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {
		// 接收 long array的package
		uint32_t idPackage = stream->readData<uint32_t>();
		// 接收原始的请求包id
		uint32_t requestIdPackage = stream->readData<uint32_t>();
		// 获取long array的package
		auto arrayPackage = (LongArrayPackage*)stream->getPackageManager()->getRemotePackage(idPackage);
		// 将package里面的数据直接存储成文件
		ULoadFileLibrary::saveByteArrayAsFile(arrayPackage->dataArray,
			arrayPackage->byteNum, GS_SAVE);
		// request时的package
		auto requestPackage = (ReconstructionPackage*)stream->getPackageManager()->getLocalInfo(requestIdPackage);
		// 调用从文件中读取3DGS的命令
		auto gsSolver = makePtr<FileGsSplat>(std::string(GS_SAVE));
		// 调用重建完成时的操作
		requestPackage->finishFunctor->onReconstructionFinish(gsSolver);
		// 删除package
		stream->getPackageManager()->deleteRemotePackage(idPackage);
		// 删除之前的请求package
		stream->getPackageManager()->deletePackagInfo(requestIdPackage);
	}
};