// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include"Types.hpp"
#include "GeometryActors/GeneratedDynamicMeshActor.h"
#include "MessageManager.hpp"
#include"GaussianSplatSolver.hpp"
#include"Containers/CircularQueue.h"
#include"GaussianDescriptor.h"
#include"MeshSolver.hpp"
#include "SocketTest.generated.h"

UCLASS()
class RECONSTRUCTIONWORLD_API ASocketTest : public AActor
{
	GENERATED_BODY()
	
public:	
	FSocket* socket;
	//目前已经注册过的message manager
	MessageManager* launchedManager = nullptr;
	// 需要被添加的mesh solver
	// 图生3D处理完成的mesh会被放进这里面
	TQueue<Ptr<MeshSolver>> meshTaskQueue;
	// 待添加到场景中的gaussian splatting
	TQueue<Ptr<GaussianSplatSolver>> splatTaskQueue;

	// Sets default values for this actor's properties
	ASocketTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	UTexture2D* buildFromMeshQueue(UDynamicMesh* mesh);

	// 判断是否有待生成的队列
	UFUNCTION(BlueprintCallable)
	bool judgeHaveMeshToLoad();

	// 判断是否有3DGS需要加载
	UFUNCTION(BlueprintCallable)
	bool judgeHaveSplatLoad();

	UFUNCTION(BlueprintCallable)
	void testSocket();

	//发送数据的测试接口
	UFUNCTION(BlueprintCallable)
	void sendData();

	//测试接收数据
	//现在可以暂时使用按键来控制
	UFUNCTION(BlueprintCallable)
	void receiveData();

	//加载图片的测试
	UFUNCTION(BlueprintCallable)
	void loadImageTest();

	// 发送Hello信息
	UFUNCTION(BlueprintCallable)
	void sendHello();

	UFUNCTION(BlueprintCallable)
	void GenerateMeshFromImage(FString imgPath);

	// 发送重建消息的逻辑
	UFUNCTION(BlueprintCallable)
	void sendReconRequest(FString imgFolder);

	UFUNCTION(BlueprintCallable)
	void launchMessageManager();

	// 用于测试从文件中读取mesh的接口
	UFUNCTION(BlueprintCallable)
	UTexture2D* loadMeshFromFile(UDynamicMesh* mesh);

	// 将gaussian splat加载到场景中
	UFUNCTION(BlueprintCallable)	
	void loadGsSplatToScene(UGaussianDescriptor* descriptor);
};
