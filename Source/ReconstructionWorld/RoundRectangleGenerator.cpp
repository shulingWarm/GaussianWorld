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

//����Բ�뾶
static float getRectRadius(float* rectRange, float circleRate) {
	//�ȼ�����ε����
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

    // ����������β���
    const float rectWidth = xMax - xMin;
    const float rectHeight = yMax - yMin;
    const float radius = std::min(rectWidth, rectHeight) * roundRate * 0.5f;
    const int numSegments = static_cast<int>(8 * roundRate) + 2; // ��̬������

    // Բ����Ч��Χ����
    const float validWidth = rectWidth - 2 * radius;
    const float validHeight = rectHeight - 2 * radius;

    // ���������
    dstPointList.clear();

    // �����ĸ�Բ�ǵ�·������ʱ�뷽��
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

    // ����Բ�� (0 ~ -��/2)
    generateCorner(xMax - radius, yMin + radius, -M_PI / 2, 0);

    // ����Բ�� (��/2 ~ 0)
    generateCorner(xMax - radius, yMax - radius, 0, M_PI / 2);

    // ����Բ�� (�� ~ ��/2)
    generateCorner(xMin + radius, yMax - radius, M_PI / 2, M_PI);

    // ����Բ�� (-��/2 ~ ��)
    generateCorner(xMin + radius, yMin + radius, -M_PI ,-M_PI / 2);

    // ������Ϊ0ʱ��ӱ�׼���ζ���
    if (radius < 1e-6f) {
        dstPointList = {
            {xMin, yMax}, {xMax, yMax},
            {xMax, yMin}, {xMin, yMin}
        };
    }

    //// ����ֱ�߱ߣ���������Ч����ʱ��
    //if (validWidth > 0) {
    //    // ����ֱ��
    //    dstPointList.push_back({ xMin + radius, yMax });
    //    dstPointList.push_back({ xMax - radius, yMax });

    //    // �ײ�ֱ��
    //    dstPointList.push_back({ xMax - radius, yMin });
    //    dstPointList.push_back({ xMin + radius, yMin });
    //}

    //if (validHeight > 0) {
    //    // �Ҳ�ֱ��
    //    dstPointList.push_back({ xMax, yMax - radius });
    //    dstPointList.push_back({ xMax, yMin + radius });

    //    // ���ֱ��
    //    dstPointList.push_back({ xMin, yMin + radius });
    //    dstPointList.push_back({ xMin, yMax - radius });
    //}
}

//������贫������rectRange�� x_min x_max y_min y_max
void RoundRectangleGenerator::makeRoundRectangleSection(CrossSection& dstSection, 
    float* rectRange, float roundRate)
{
	//����Բ�Ǿ��εĲ�����
    std::vector<Vector2f> vertexList;
    makeRoundRectangle(rectRange, roundRate, vertexList);
    //�ڴ˴�ӡԲ�Ǿ��������漰���ĵ�
    std::stringstream sstream;
    for (auto& eachVertex : vertexList) {
        sstream << eachVertex.x << " " << eachVertex.y << std::endl;
    }
    std::string printStr = sstream.str();
    UE_LOG(LogTemp, Warning, TEXT("%s\n"), *FString(printStr.c_str()));
    //�ѵ��б��ƽ�����
    for (auto& eachPoint : vertexList) {
        //��Ŀ�����������ӵ�
        dstSection.addVertex(eachPoint.x, eachPoint.y);
    }
    //���ɹ�һ��������
    dstSection.generateNormalCoord();
}


