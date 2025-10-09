// Fill out your copyright notice in the Description page of Project Settings.


#include "TransformLibrary.h"

FTransform UTransformLibrary::getCameraTransform(FVector location, 
	float pitch, float planeAngle, float distance)
{
    // ��鲢ǯ�� pitch ������ [0, 90] ��Χ
    pitch = FMath::Clamp(pitch, 0.0f, 90.0f);

    // ȷ�����������
    if (distance <= 0.0f)
    {
        distance = 1.0f; // ����Ϊ��С��ȫֵ
    }

    // ���Ƕ�ת��Ϊ����
    float pitchRad = FMath::DegreesToRadians(pitch);
    float planeAngleRad = FMath::DegreesToRadians(planeAngle);

    // ������������Ŀ���λ��ƫ�ƣ�������ת�ѿ������꣩
    // UE ����ϵ��X-ǰ��Y-����Z-����
    float sinPitch = FMath::Sin(pitchRad);
    float cosPitch = FMath::Cos(pitchRad);
    float sinPlane = FMath::Sin(planeAngleRad);
    float cosPlane = FMath::Cos(planeAngleRad);

    // ����ƫ������������ (distance, pitch, planeAngle)
    FVector offset;
    offset.X = distance * sinPitch * cosPlane; // ˮƽ�� X ����
    offset.Y = distance * sinPitch * sinPlane; // ˮƽ�� Y ����
    offset.Z = distance * cosPitch;            // ��ֱ������Z �ᣩ

    // ���λ�� = Ŀ��λ�� + ƫ����
    FVector cameraLocation = location + offset;

    // 3. ��ȡ��ת��
    FRotator cameraRotation(0,-pitch,0);

    // ��������任������Ĭ��Ϊ 1��
    return FTransform(cameraRotation, cameraLocation);
}

FVector UTransformLibrary::getCameraLocation(FVector location, float pitch, float planeAngle, float distance)
{
    // ��鲢ǯ�� pitch ������ [0, 90] ��Χ
    // pitch = FMath::Clamp(pitch, 0.0f, 90.0f);

    // ȷ�����������
    if (distance <= 0.0f)
    {
        distance = 1.0f; // ����Ϊ��С��ȫֵ
    }

    // ���Ƕ�ת��Ϊ����
    float pitchRad = FMath::DegreesToRadians(pitch);
    float planeAngleRad = FMath::DegreesToRadians(planeAngle);

    // ������������Ŀ���λ��ƫ�ƣ�������ת�ѿ������꣩
    // UE ����ϵ��X-ǰ��Y-����Z-����
    float sinPitch = FMath::Sin(pitchRad);
    float cosPitch = FMath::Cos(pitchRad);
    float sinPlane = FMath::Sin(planeAngleRad);
    float cosPlane = FMath::Cos(planeAngleRad);

    // ����ƫ������������ (distance, pitch, planeAngle)
    FVector offset;
    offset.X = distance * sinPitch * cosPlane; // ˮƽ�� X ����
    offset.Y = distance * sinPitch * sinPlane; // ˮƽ�� Y ����
    offset.Z = distance * cosPitch;            // ��ֱ������Z �ᣩ

    // ���λ�� = Ŀ��λ�� + ƫ����
    FVector cameraLocation = location + offset;

    return cameraLocation;
}
