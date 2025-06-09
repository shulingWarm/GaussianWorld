#pragma once
#include"AbstractMessage.hpp"
#include"ImagePackageInfo.hpp"
#include"ImageRowDataMessage.hpp"
#include"ImageEndMessage.hpp"

// 这是对方已经接收到图片消息时的回传
// 这里面会告知下一个
class ImageReceiveMessage : public AbstractMessage{
public:
	ImageReceiveMessage() : AbstractMessage("ImageReceiveMessage") {

	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) override {
		// C++这边暂时不实现发送的逻辑
	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {
		// 接收图片消息包的id
		uint32_t packageId = stream->readData<uint32_t>();
		// 获取暂存的图片数据
		auto imgPackage = (ImagePackageInfo*)(stream->getPackageManager()->getLocalInfo(packageId));
		// 请求获取的行id
		uint32_t idRow = stream->readData<uint32_t>();
		ImageSolver* image = imgPackage->image;
		// 判断请求行是否已经到头了
		if (idRow == image->getHeight()) {
			// 发送图片发送完成的信息
			ImageEndMessage endMessage(packageId);
			messageManager->sendMessage(endMessage);
			// 删除消息
			stream->getPackageManager()->deletePackagInfo(packageId);
		}
		// 获取需要的下一行
		auto rowData = image->getRowData(idRow);
		// 图片行数据
		ImageRowData rowDataMessage(rowData, imgPackage->image->getWidth(), idRow, packageId);
		// 把图片行数据发送回去
		messageManager->sendMessage(rowDataMessage);
	}
};