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

	//ʵ��PS������Ʋ�Ч��
	//Ĭ����ɫ��ʽ��BGRA�ģ��Ժ��������Ҫ����չ���
	static void PSCloudEffect(uint8_t* imageData, uint32_t width, uint32_t height, Vec3 color1, Vec3 color2);

	//��ͼƬ�����ɫ
	//�������rate��ʾ���ǰٷ�֮���ٵ�������Ҫ�������ɫ
	static void addNoise(uint8_t* imageData, uint32_t width, uint32_t height, Vec3 color, float rate);

	//����ģ����Ч��
	//Ŀǰֻ֧�������ģ��Ч��
	static void motionBlur(uint8_t* imageData, uint32_t width, uint32_t height);
};
