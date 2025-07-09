#pragma once
#include<iostream>

// 这属于是对图像的一种抽象表示
// 不然后续可能不好兼容多种图片表示
class ImageSolver {
public:
	//获取图片的宽高
	virtual uint32_t getWidth() = 0;
	virtual uint32_t getHeight() = 0;

	//获取某一行的数据指针
	virtual uint8_t* getRowData(uint32_t idRow) = 0;

	// 设置图片的指定区域的像素
	virtual void setPixels(uint32_t beginId, uint32_t pixelNum, 
		uint32_t channelNum, uint8_t* pixelData) = 0;

	virtual ~ImageSolver() {

	}
};