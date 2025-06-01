// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundRectangleGenerator.h"
#include <vector>
#include <cmath>

RoundRectangleGenerator::RoundRectangleGenerator()
{
}

RoundRectangleGenerator::~RoundRectangleGenerator()
{
}

//计算圆半径
static float getRectRadius(float* rectRange, float circleRate) {
	//先计算矩形的最长边
	float range1 = rectRange[1] - rectRange[0];
	float range2 = rectRange[3] - rectRange[2];
	float minRange = range1;
	if (range2 < range1)
		minRange = range2;
	return (minRange / 2) * circleRate;
}

struct Vector2f {
    float x, y;
};

static void makeRoundRectangle(
    float* range,
    float roundRate,
    std::vector<Vector2f>& dstPointList)
{
    constexpr float M_PI = 3.1415926f;
    const float xMin = range[0];
    const float xMax = range[1];
    const float yMin = range[2];
    const float yMax = range[3];

    // 计算基本几何参数
    const float rectWidth = xMax - xMin;
    const float rectHeight = yMax - yMin;
    const float radius = std::min(rectWidth, rectHeight) * roundRate * 0.5f;
    const int numSegments = static_cast<int>(8 * roundRate) + 2; // 动态采样率

    // 圆角有效范围限制
    const float validWidth = rectWidth - 2 * radius;
    const float validHeight = rectHeight - 2 * radius;

    // 清除旧数据
    dstPointList.clear();

    // 生成四个圆角的路径（逆时针方向）
    auto generateCorner = [&](float cx, float cy, float startAngle, float endAngle) {
        for (int i = 0; i <= numSegments; ++i) {
            float t = static_cast<float>(i) / numSegments;
            float angle = startAngle + t * (endAngle - startAngle);
            dstPointList.push_back({
                cx + radius * cosf(angle),
                cy + radius * sinf(angle)
                });
        }
        };

    // 右下圆角 (0 ~ -π/2)
    generateCorner(xMax - radius, yMin + radius, -M_PI / 2, 0);

    // 右上圆角 (π/2 ~ 0)
    generateCorner(xMax - radius, yMax - radius, 0, M_PI / 2);

    // 左上圆角 (π ~ π/2)
    generateCorner(xMin + radius, yMax - radius, M_PI / 2, M_PI);

    // 左下圆角 (-π/2 ~ π)
    generateCorner(xMin + radius, yMin + radius, -M_PI ,-M_PI / 2);

    // 当曲率为0时添加标准矩形顶点
    if (radius < 1e-6f) {
        dstPointList = {
            {xMin, yMax}, {xMax, yMax},
            {xMax, yMin}, {xMin, yMin}
        };
    }

    //// 连接直线边（当存在有效区域时）
    //if (validWidth > 0) {
    //    // 顶部直线
    //    dstPointList.push_back({ xMin + radius, yMax });
    //    dstPointList.push_back({ xMax - radius, yMax });

    //    // 底部直线
    //    dstPointList.push_back({ xMax - radius, yMin });
    //    dstPointList.push_back({ xMin + radius, yMin });
    //}

    //if (validHeight > 0) {
    //    // 右侧直线
    //    dstPointList.push_back({ xMax, yMax - radius });
    //    dstPointList.push_back({ xMax, yMin + radius });

    //    // 左侧直线
    //    dstPointList.push_back({ xMin, yMin + radius });
    //    dstPointList.push_back({ xMin, yMax - radius });
    //}
}

//这里假设传进来的rectRange是 x_min x_max y_min y_max
void RoundRectangleGenerator::makeRoundRectangleSection(CrossSection& dstSection, 
    float* rectRange, float roundRate)
{
	//生成圆角矩形的采样点
    std::vector<Vector2f> vertexList;
    makeRoundRectangle(rectRange, roundRate, vertexList);
    //在此打印圆角矩形里面涉及到的点
    std::stringstream sstream;
    for (auto& eachVertex : vertexList) {
        sstream << eachVertex.x << " " << eachVertex.y << std::endl;
    }
    std::string printStr = sstream.str();
    UE_LOG(LogTemp, Warning, TEXT("%s\n"), *FString(printStr.c_str()));
    //把点列表复制进截面
    for (auto& eachPoint : vertexList) {
        //向目标截面里面添加点
        dstSection.addVertex(eachPoint.x, eachPoint.y);
    }
    //生成归一化的坐标
    dstSection.generateNormalCoord();
}


