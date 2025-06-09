#pragma once
#include"AbstractMessage.hpp"

// 图片的行数据信息
class ImageRowData : public AbstractMessage {
public:
	// 用于发送的图片数据
	uint8_t* imgData;
	uint32_t imgWidth;
	//本次发送的行id
	uint32_t idRow;
	// 消息所属的包id
	uint32_t idPackage;

	ImageRowData(uint8_t* imgData,
		uint32_t imgWidth,
		uint32_t idRow,
		uint32_t idPackage //消息所属的包id
	) : AbstractMessage("ImageRowData") {
		this->imgData = imgData;
		this->imgWidth = imgWidth;
		this->idRow = idRow;
		this->idPackage = idPackage;
	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) override {
		// 发送消息包
		stream->writeData<uint32_t>(idPackage);
		// 写入数据行
		stream->writeData<uint32_t>(idRow);
		// 发送图片数据
		stream->send((const char*)this->imgData, sizeof(uint8_t) * imgWidth * 4);
	}

	//接收消息的逻辑
	//C++这边暂时不实现数据行接收的情况
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {

	}
};