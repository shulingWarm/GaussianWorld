#pragma once

#include "CoreMinimal.h"
#include "GaussianStruct.generated.h"

/* 为了让你的结构体可以被蓝图访问 */
USTRUCT(BlueprintType)
struct FGaussianStruct  // 必须以F开头
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)    //将字段公开给蓝图访问
	int32 test;
};