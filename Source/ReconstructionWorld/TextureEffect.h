// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class RECONSTRUCTIONWORLD_API TextureEffect
{
public:
	TextureEffect();
	~TextureEffect();

	struct Vec3 {
		uint8 r, g, b;

		Vec3(uint8 r_, uint8 g_, uint8 b_) {
			r = r_;
			g = g_;
			b = b_;
		}

		inline uint8 operator[](uint32 id)
		{
			if (id == 0)
				return b;
			else if (id == 1)
				return g;
			return r;
		}
	};

	//实现PS里面的云彩效果
	//默认颜色格式是BGRA的，以后如果有需要再扩展别的
	static void PSCloudEffect(uint8_t* imageData, uint32_t width, uint32_t height, Vec3 color1, Vec3 color2);

	//给图片添加杂色
	//这里面的rate表示的是百分之多少的像素需要被添加杂色
	static void addNoise(uint8_t* imageData, uint32_t width, uint32_t height, Vec3 color, float rate);

	//动感模糊的效果
	//目前只支持纵向的模糊效果
	static void motionBlur(uint8_t* imageData, uint32_t width, uint32_t height);
};
