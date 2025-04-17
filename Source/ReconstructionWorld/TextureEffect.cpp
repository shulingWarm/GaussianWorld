// Fill out your copyright notice in the Description page of Project Settings.


#include "TextureEffect.h"
#include <cstdint>
#include <vector>
#include <algorithm>
#include <array>
#include<random>
#include <cstdint>
#include <cmath>
#include <ctime>

TextureEffect::TextureEffect()
{
}

TextureEffect::~TextureEffect()
{
}

inline float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

inline float hash(int x, int y, unsigned int seed) {
    unsigned int h = x * 374761393 + y * 668265263 + seed;
    h = (h ^ (h >> 15)) * 3266489917;
    h ^= h >> 15;
    h *= 668265263;
    h ^= h >> 15;
    return static_cast<float>(h & 0x7FFFFFFF) / 0x7FFFFFFF * 2.0f - 1.0f;
}

inline float valueNoise(float x, float y, unsigned int seed) {
    int x0 = static_cast<int>(std::floor(x));
    int y0 = static_cast<int>(std::floor(y));
    float sx = x - x0;
    float sy = y - y0;

    float n00 = hash(x0, y0, seed);
    float n10 = hash(x0 + 1, y0, seed);
    float n01 = hash(x0, y0 + 1, seed);
    float n11 = hash(x0 + 1, y0 + 1, seed);

    float ix0 = lerp(n00, n10, sx);
    float ix1 = lerp(n01, n11, sx);
    return lerp(ix0, ix1, sy);
}

float fractalNoise(float x, float y, int octaves, float persistence, unsigned int seed) {
    float total = 0.0f;
    float amplitude = 1.0f;
    float frequency = 1.0f;
    float maxValue = 0.0f;

    for (int i = 0; i < octaves; ++i) {
        total += valueNoise(x * frequency, y * frequency, seed) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= 2.0f;
    }

    return std::max(0.0f, std::min(1.0f, (total / maxValue + 1.0f) / 2.0f));
}

void TextureEffect::PSCloudEffect(uint8_t* imageData, uint32_t width, uint32_t height, Vec3 color1, Vec3 color2)
{
    const int octaves = 6;          // ����ϸ�ڲ��
    const float persistence = 0.5f; // ÿ��˥��ϵ��
    const float scale = 150.0f;     // �����Ʋ��ܶ�
    unsigned int seed = static_cast<unsigned int>(time(nullptr));

    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            // �����������꣨������ƫ��ʵ�ֲ�ͬЧ����
            float fx = (x + seed % 1000) / scale;
            float fy = (y + seed % 1000) / scale;

            // �����������
            float t = fractalNoise(fx, fy, octaves, persistence, seed);

            // ��ɫ���
            uint8_t r = static_cast<uint8_t>(color1.r * (1 - t) + color2.r * t);
            uint8_t g = static_cast<uint8_t>(color1.g * (1 - t) + color2.g * t);
            uint8_t b = static_cast<uint8_t>(color1.b * (1 - t) + color2.b * t);

            // д����������
            uint32_t index = (y * width + x) * 4;
            imageData[index] = b;
            imageData[index + 1] = g;
            imageData[index + 2] = r;
        }
    }
}

//���ص�һ�����׵������������
class SimpleRandomGenerator {
public:
    uint64_t seed = 5;

    uint32 getRandom()
    {
        seed = (seed * 123432493243842ULL) + 11ULL;
        seed &= 0x3ffffffffffffff;
        return seed;
    }
};
//��̬�������������
static SimpleRandomGenerator randGenerator;

void TextureEffect::addNoise(uint8_t* imageData, uint32_t width, uint32_t height, Vec3 color, float rate)
{
    //������Ҫ��ӵ�������
    uint32_t pixelNum = rate * width * height;
    uint32 randPixel[2];
    //��ɫ�ı���
    float colorScale;
    //����ÿ����Ҫ�����������
    for (uint32 idPixel = 0; idPixel < pixelNum; ++idPixel)
    {
        //����һ�����������
        randPixel[0] = randGenerator.getRandom() % height;
        randPixel[1] = randGenerator.getRandom() % width;
        //������ɫ�ı���
        colorScale = (float)(randGenerator.getRandom() % 10000) / 10000;
        //ָ��λ�õ�����
        uint8* targetPixel = imageData + (randPixel[0] * width + randPixel[1]) * 4;
        //��ָ����λ�������ɫ
        for (uint32 i = 0; i < 3; ++i)
        {
            uint32 result = (uint32)targetPixel[i] + color[i];
            if (result > 255)
                targetPixel[i] = 255;
            else
                targetPixel[i] = result;
        }
    }
}

//�Ե���һ�����ݵĲ���
void samplePixelForBlur(uint8* imageData, uint32 width, uint32 height,
    uint32 idRow, uint32 idCol, uint8* targetColor,
    uint32 sampleStepNum,
    float sampleDis
) {
    //��ʼ��ƽ����ɫ
    float avgColor[3] = { 0,0,0 };
    //����ÿһ���Ĳ���λ��
    for (uint32 idStep = 1; idStep <= sampleStepNum; ++idStep) {
        //��ǰλ�õ����ϲ�����
        int upRow = idRow - idStep * sampleDis;
        if (upRow < 0)
            upRow = 0;
        int downRow = idRow + idStep * sampleDis;
        if (downRow >= (int)height)
            downRow = height - 1;
        //�Ϸ�λ�õ���ɫ
        uint8* upColor = imageData + (idCol + upRow * width) * 4;
        uint8* downColor = imageData + (idCol + downRow * width) * 4;
        //����ɫ�ۼӵ�ȫ����ɫ����
        for (uint32 i = 0; i < 3; ++i)
        {
            avgColor[i] += upColor[i];
            avgColor[i] += downColor[i];
        }
    }
    //�Եõ�����ɫ��ֵȡƽ��
    for (uint32 i = 0; i < 3; ++i) {
        avgColor[i] /= (sampleStepNum * 2);
        //��¼���յ���ɫ
        if (avgColor[i] > 255)
            targetColor[i] = 255;
        else
            targetColor[i] = avgColor[i];
    }
}

void TextureEffect::motionBlur(uint8_t* imageData, uint32_t width, uint32_t height)
{
    //�����Ĳ���
    uint32 sampleStepNum = 40;
    //ÿһ���Ĳ�������
    float sampleDis = height * 0.8 / sampleStepNum;
    //�ȸ���һ��ԭʼ����
    uint8_t* copyData = new uint8[width * height * 4];
    memcpy(copyData, imageData, sizeof(uint8) * width * height * 4);
    //����ͼƬ��ÿһ��
    for (uint32 idWidth = 0; idWidth < width; ++idWidth) {
        //���ڵ�ǰ�У�����ÿһ��
        for (uint32 idRow = 0; idRow < height; ++idRow) {
            //Ҫ�����Ŀ������
            uint8* targetColor = imageData + (idRow * width + idWidth) * 4;
            //�Ե�����ɫ�Ĳ���
            samplePixelForBlur(copyData, width, height, idRow, idWidth, targetColor,
                sampleStepNum, sampleDis);
        }
    }
}


