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

	//根据三个点得到三个点的法向量
	template<class RetType>
	static RetType getPointNormal(
		FVector point1,
		FVector point2,
		FVector point3
	)
	{
		//12两个点的法向量
		FVector vec12 = point2 - point1;
		//从2到3的向量
		FVector vec23 = point3 - point2;
		//计算两个向量的叉乘
		FVector ret = FVector::CrossProduct(vec12, vec23);
		return RetType(ret.X, ret.Y, ret.Z);
	}

	//向量的数据类型之间的转换
	template<class SrcType, class DstType>
	static DstType convertVector(const SrcType& srcVector) {
		return { (DstType::FReal)srcVector.X, 
			(DstType::FReal)srcVector.Y, 
			(DstType::FReal)srcVector.Z };
	}
};
