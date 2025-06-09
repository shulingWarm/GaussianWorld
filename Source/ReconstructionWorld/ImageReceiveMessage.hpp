#pragma once
#include"AbstractMessage.hpp"
#include"ImagePackageInfo.hpp"
#include"ImageRowDataMessage.hpp"
#include"ImageEndMessage.hpp"

// ���ǶԷ��Ѿ����յ�ͼƬ��Ϣʱ�Ļش�
// ��������֪��һ��
class ImageReceiveMessage : public AbstractMessage{
public:
	ImageReceiveMessage() : AbstractMessage("ImageReceiveMessage") {

	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) override {
		// C++�����ʱ��ʵ�ַ��͵��߼�
	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {
		// ����ͼƬ��Ϣ����id
		uint32_t packageId = stream->readData<uint32_t>();
		// ��ȡ�ݴ��ͼƬ����
		auto imgPackage = (ImagePackageInfo*)(stream->getPackageManager()->getLocalInfo(packageId));
		// �����ȡ����id
		uint32_t idRow = stream->readData<uint32_t>();
		ImageSolver* image = imgPackage->image;
		// �ж��������Ƿ��Ѿ���ͷ��
		if (idRow == image->getHeight()) {
			// ����ͼƬ������ɵ���Ϣ
			ImageEndMessage endMessage(packageId);
			messageManager->sendMessage(endMessage);
			// ɾ����Ϣ
			stream->getPackageManager()->deletePackagInfo(packageId);
		}
		// ��ȡ��Ҫ����һ��
		auto rowData = image->getRowData(idRow);
		// ͼƬ������
		ImageRowData rowDataMessage(rowData, imgPackage->image->getWidth(), idRow, packageId);
		// ��ͼƬ�����ݷ��ͻ�ȥ
		messageManager->sendMessage(rowDataMessage);
	}
};