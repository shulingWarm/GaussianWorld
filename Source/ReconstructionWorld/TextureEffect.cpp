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
    const int octaves = 6;          // 增加细节层次
    const float persistence = 0.5f; // 每层衰减系数
    const float scale = 150.0f;     // 控制云彩密度
    unsigned int seed = static_cast<unsigned int>(time(nullptr));

    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            // 生成噪声坐标（添加随机偏移实现不同效果）
            float fx = (x + seed % 1000) / scale;
            float fy = (y + seed % 1000) / scale;

            // 计算分形噪声
            float t = fractalNoise(fx, fy, octaves, persistence, seed);

            // 颜色混合
            uint8_t r = static_cast<uint8_t>(color1.r * (1 - t) + color2.r * t);
            uint8_t g = static_cast<uint8_t>(color1.g * (1 - t) + color2.g * t);
            uint8_t b = static_cast<uint8_t>(color1.b * (1 - t) + color2.b * t);

            // 写入像素数据
            uint32_t index = (y * width + x) * 4;
            imageData[index] = b;
            imageData[index + 1] = g;
            imageData[index + 2] = r;
        }
    }
}

//本地的一个简易的随机数生成器
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
//静态的随机数生成器
static SimpleRandomGenerator randGenerator;

void TextureEffect::addNoise(uint8_t* imageData, uint32_t width, uint32_t height, Vec3 color, float rate)
{
    //计算需要添加的像素数
    uint32_t pixelNum = rate * width * height;
    uint32 randPixel[2];
    //杂色的比例
    float colorScale;
    //遍历每个需要被处理的像素
    for (uint32 idPixel = 0; idPixel < pixelNum; ++idPixel)
    {
        //计算一个随机的坐标
        randPixel[0] = randGenerator.getRandom() % height;
        randPixel[1] = randGenerator.getRandom() % width;
        //计算杂色的比例
        colorScale = (float)(randGenerator.getRandom() % 10000) / 10000;
        //指定位置的像素
        uint8* targetPixel = imageData + (randPixel[0] * width + randPixel[1]) * 4;
        //给指定的位置添加杂色
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

//对单独一个数据的采样
void samplePixelForBlur(uint8* imageData, uint32 width, uint32 height,
    uint32 idRow, uint32 idCol, uint8* targetColor,
    uint32 sampleStepNum,
    float sampleDis
) {
    //初始化平均颜色
    float avgColor[3] = { 0,0,0 };
    //遍历每一步的采样位置
    for (uint32 idStep = 1; idStep <= sampleStepNum; ++idStep) {
        //当前位置的向上采样点
        int upRow = idRow - idStep * sampleDis;
        if (upRow < 0)
            upRow = 0;
        int downRow = idRow + idStep * sampleDis;
        if (downRow >= (int)height)
            downRow = height - 1;
        //上方位置的颜色
        uint8* upColor = imageData + (idCol + upRow * width) * 4;
        uint8* downColor = imageData + (idCol + downRow * width) * 4;
        //把颜色累加到全局颜色里面
        for (uint32 i = 0; i < 3; ++i)
        {
            avgColor[i] += upColor[i];
            avgColor[i] += downColor[i];
        }
    }
    //对得到的颜色数值取平均
    for (uint32 i = 0; i < 3; ++i) {
        avgColor[i] /= (sampleStepNum * 2);
        //记录最终的颜色
        if (avgColor[i] > 255)
            targetColor[i] = 255;
        else
            targetColor[i] = avgColor[i];
    }
}

void TextureEffect::motionBlur(uint8_t* imageData, uint32_t width, uint32_t height)
{
    //采样的步数
    uint32 sampleStepNum = 40;
    //每一步的采样距离
    float sampleDis = height * 0.8 / sampleStepNum;
    //先复制一份原始数据
    uint8_t* copyData = new uint8[width * height * 4];
    memcpy(copyData, imageData, sizeof(uint8) * width * height * 4);
    //遍历图片的每一列
    for (uint32 idWidth = 0; idWidth < width; ++idWidth) {
        //对于当前列，遍历每一行
        for (uint32 idRow = 0; idRow < height; ++idRow) {
            //要处理的目标像素
            uint8* targetColor = imageData + (idRow * width + idWidth) * 4;
            //对单个颜色的采样
            samplePixelForBlur(copyData, width, height, idRow, idWidth, targetColor,
                sampleStepNum, sampleDis);
        }
    }
}


