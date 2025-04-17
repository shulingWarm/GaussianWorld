// Fill out your copyright notice in the Description page of Project Settings.


#include "CppRotationAxis.h"

// Sets default values
ACppRotationAxis::ACppRotationAxis()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

static void projectPointByMatrix(const FVector& WorldPosition, const FIntRect& ViewRect, const FMatrix& ViewProjectionMatrix, FVector2D& out_ScreenPos)
{
	FPlane Result = ViewProjectionMatrix.TransformFVector4(FVector4(WorldPosition, 1.f));
	// the result of this will be x and y coords in -1..1 projection space
	const float RHW = 1.0f / Result.W;
	FPlane PosInScreenSpace = FPlane(Result.X * RHW, Result.Y * RHW, Result.Z * RHW, Result.W);

	// Move from projection space to normalized 0..1 UI space
	const float NormalizedX = ( PosInScreenSpace.X / 2.f ) + 0.5f;
	const float NormalizedY = 1.f - ( PosInScreenSpace.Y / 2.f ) - 0.5f;

	FVector2D RayStartViewRectSpace(
		( NormalizedX * (float)ViewRect.Width() ),
		( NormalizedY * (float)ViewRect.Height() )
	);

	out_ScreenPos = RayStartViewRectSpace + FVector2D(static_cast<float>(ViewRect.Min.X), static_cast<float>(ViewRect.Min.Y));

}

//�����Ƿ�ͶӰ����Ļ�ڶ����ض�άͶӰ����
static FVector2D projectWorldPointToScreen(APlayerController* playerController,
	FVector worldPoint)
{
	//���local player
	auto localPlayer = playerController->GetLocalPlayer();
	//���ͶӰ�������������
	FSceneViewProjectionData projectionData;
	//���շ��ص�ͶӰ��
	FVector2D pointInScreen;
	if (localPlayer->GetProjectionData(localPlayer->ViewportClient->Viewport, projectionData))
	{
		FMatrix const projectMatrix = projectionData.ComputeViewProjectionMatrix();
		projectPointByMatrix(worldPoint, projectionData.GetConstrainedViewRect(),
			projectMatrix, pointInScreen);
		//���ͶӰλ�õĺ���
		playerController->PostProcessWorldToScreen(worldPoint, pointInScreen,false);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("GetProjectionData Failed"));
	}
	return pointInScreen;
}

FVector2D ACppRotationAxis::GetTangentVectorInScreen(APlayerController* playerController, 
	FVector worldDirection, FVector axisCenter, FVector planeNorm, FVector cameraCenter, FRotator cameraRotation)
{
	//�����ƽ�淽�������D
	float planeD = - (planeNorm[0]*axisCenter[0] + planeNorm[1]*axisCenter[1] +
		planeNorm[2]*axisCenter[2]);
	//����ӹ��ĵ�����������ƫ����
	float centerOffset = (-planeD - planeNorm[0]*cameraCenter[0] - 
		planeNorm[1]*cameraCenter[1] -
		planeNorm[2]*cameraCenter[2])/
		(planeNorm[0]*worldDirection[0] +
			planeNorm[1]*worldDirection[1] +
			planeNorm[2]*worldDirection[2]);
	//�������ߺ�Բƽ��Ľ���
	FVector3d intersection = cameraCenter + centerOffset*worldDirection;
	//�����Բ�ĵ����������
	FVector3d centerDirection = intersection - axisCenter;
	//������������
	FVector tangentVector = FVector3d::CrossProduct(planeNorm,centerDirection).GetSafeNormal();
	//��ӹ���һ�������ϵ�λ��
	FVector tangentPoint = intersection + tangentVector;
	return projectWorldPointToScreen(playerController,tangentPoint);
	//����ת���ɷ��������
	//return FVector2D(camDir3D[0]/camDir3D[2], camDir3D[1]/camDir3D[2]);
}

TArray<FVector> ACppRotationAxis::TangentDebug(FVector worldDirection, FVector axisCenter, FVector planeNorm, FVector cameraCenter, FRotator cameraRotation)
{
	//�����ƽ�淽�������D
	float planeD = - (planeNorm[0]*axisCenter[0] + planeNorm[1]*axisCenter[1] +
		planeNorm[2]*axisCenter[2]);
	//����ӹ��ĵ�����������ƫ����
	float centerOffset = (-planeD - planeNorm[0]*cameraCenter[0] - 
		planeNorm[1]*cameraCenter[1] -
		planeNorm[2]*cameraCenter[2])/
		(planeNorm[0]*worldDirection[0] +
			planeNorm[1]*worldDirection[1] +
			planeNorm[2]*worldDirection[2]);
	//�������ߺ�Բƽ��Ľ���
	FVector3d intersection = cameraCenter + centerOffset*worldDirection;
	//�����Բ�ĵ����������
	FVector3d centerDirection = intersection - axisCenter;
	//������������
	FVector3d tangentVector = FVector3d::CrossProduct(planeNorm,centerDirection);
	TArray<FVector> retVectors;
	retVectors.Add(intersection);
	retVectors.Add(intersection + tangentVector.GetSafeNormal()*100);
	return retVectors;
}

FVector ACppRotationAxis::printTangentLine(FVector worldDirection, FVector axisCenter, FVector planeNorm,
	FVector cameraCenter)
{
	//�����ƽ�淽�������D
	float planeD = - (planeNorm[0]*axisCenter[0] + planeNorm[1]*axisCenter[1] +
		planeNorm[2]*axisCenter[2]);
	//��ӡ�������ƽ�淽��
	UE_LOG(LogTemp,Warning,TEXT("Plane param %f %f %f %f\n"),planeNorm[0],planeNorm[1],planeNorm[2],planeD);
	//����ӹ��ĵ�����������ƫ����
	float centerOffset = (-planeD - planeNorm[0]*cameraCenter[0] - 
		planeNorm[1]*cameraCenter[1] -
		planeNorm[2]*cameraCenter[2])/
		(planeNorm[0]*worldDirection[0] +
		planeNorm[1]*worldDirection[1] +
		planeNorm[2]*worldDirection[2]);
	//�������ߺ�Բƽ��Ľ���
	FVector3d intersection = cameraCenter + centerOffset*worldDirection;
	//�����Բ�ĵ����������
	FVector3d centerDirection = intersection - cameraCenter;
	//������������
	FVector3d tangentVector = FVector3d::CrossProduct(planeNorm,centerDirection);
	return intersection;
}

// Called when the game starts or when spawned
void ACppRotationAxis::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACppRotationAxis::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

