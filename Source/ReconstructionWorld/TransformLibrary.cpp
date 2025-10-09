// Fill out your copyright notice in the Description page of Project Settings.


#include "TransformLibrary.h"

FTransform UTransformLibrary::getCameraTransform(FVector location, 
	float pitch, float planeAngle, float distance)
{
    // 检查并钳制 pitch 参数到 [0, 90] 范围
    pitch = FMath::Clamp(pitch, 0.0f, 90.0f);

    // 确保距离大于零
    if (distance <= 0.0f)
    {
        distance = 1.0f; // 设置为最小安全值
    }

    // 将角度转换为弧度
    float pitchRad = FMath::DegreesToRadians(pitch);
    float planeAngleRad = FMath::DegreesToRadians(planeAngle);

    // 计算相机相对于目标的位置偏移（球坐标转笛卡尔坐标）
    // UE 坐标系：X-前向，Y-右向，Z-上向
    float sinPitch = FMath::Sin(pitchRad);
    float cosPitch = FMath::Cos(pitchRad);
    float sinPlane = FMath::Sin(planeAngleRad);
    float cosPlane = FMath::Cos(planeAngleRad);

    // 计算偏移量：球坐标 (distance, pitch, planeAngle)
    FVector offset;
    offset.X = distance * sinPitch * cosPlane; // 水平面 X 分量
    offset.Y = distance * sinPitch * sinPlane; // 水平面 Y 分量
    offset.Z = distance * cosPitch;            // 垂直分量（Z 轴）

    // 相机位置 = 目标位置 + 偏移量
    FVector cameraLocation = location + offset;

    // 3. 提取旋转器
    FRotator cameraRotation(0,-pitch,0);

    // 返回相机变换（缩放默认为 1）
    return FTransform(cameraRotation, cameraLocation);
}

FVector UTransformLibrary::getCameraLocation(FVector location, float pitch, float planeAngle, float distance)
{
    // 检查并钳制 pitch 参数到 [0, 90] 范围
    // pitch = FMath::Clamp(pitch, 0.0f, 90.0f);

    // 确保距离大于零
    if (distance <= 0.0f)
    {
        distance = 1.0f; // 设置为最小安全值
    }

    // 将角度转换为弧度
    float pitchRad = FMath::DegreesToRadians(pitch);
    float planeAngleRad = FMath::DegreesToRadians(planeAngle);

    // 计算相机相对于目标的位置偏移（球坐标转笛卡尔坐标）
    // UE 坐标系：X-前向，Y-右向，Z-上向
    float sinPitch = FMath::Sin(pitchRad);
    float cosPitch = FMath::Cos(pitchRad);
    float sinPlane = FMath::Sin(planeAngleRad);
    float cosPlane = FMath::Cos(planeAngleRad);

    // 计算偏移量：球坐标 (distance, pitch, planeAngle)
    FVector offset;
    offset.X = distance * sinPitch * cosPlane; // 水平面 X 分量
    offset.Y = distance * sinPitch * sinPlane; // 水平面 Y 分量
    offset.Z = distance * cosPitch;            // 垂直分量（Z 轴）

    // 相机位置 = 目标位置 + 偏移量
    FVector cameraLocation = location + offset;

    return cameraLocation;
}
