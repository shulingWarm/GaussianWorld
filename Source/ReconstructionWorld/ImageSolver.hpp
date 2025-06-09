#pragma once
#include<iostream>

// �������Ƕ�ͼ���һ�ֳ����ʾ
// ��Ȼ�������ܲ��ü��ݶ���ͼƬ��ʾ
class ImageSolver {
public:
	//��ȡͼƬ�Ŀ��
	virtual uint32_t getWidth() = 0;
	virtual uint32_t getHeight() = 0;

	//��ȡĳһ�е�����ָ��
	virtual uint8_t* getRowData(uint32_t idRow) = 0;
};