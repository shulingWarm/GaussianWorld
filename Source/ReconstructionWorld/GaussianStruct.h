#pragma once

#include "CoreMinimal.h"
#include "GaussianStruct.generated.h"

/* Ϊ������Ľṹ����Ա���ͼ���� */
USTRUCT(BlueprintType)
struct FGaussianStruct  // ������F��ͷ
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)    //���ֶι�������ͼ����
	int32 test;
};