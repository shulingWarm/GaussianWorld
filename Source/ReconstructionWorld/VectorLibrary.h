// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class RECONSTRUCTIONWORLD_API VectorLibrary
{
public:
	VectorLibrary();
	~VectorLibrary();

	//����������õ�������ķ�����
	template<class RetType>
	static RetType getPointNormal(
		FVector point1,
		FVector point2,
		FVector point3
	)
	{
		//12������ķ�����
		FVector vec12 = point2 - point1;
		//��2��3������
		FVector vec23 = point3 - point2;
		//�������������Ĳ��
		FVector ret = FVector::CrossProduct(vec12, vec23);
		return RetType(ret.X, ret.Y, ret.Z);
	}

	//��������������֮���ת��
	template<class SrcType, class DstType>
	static DstType convertVector(const SrcType& srcVector) {
		return { (DstType::FReal)srcVector.X, 
			(DstType::FReal)srcVector.Y, 
			(DstType::FReal)srcVector.Z };
	}
};
