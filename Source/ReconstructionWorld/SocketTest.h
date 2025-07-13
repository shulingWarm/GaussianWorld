// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeometryActors/GeneratedDynamicMeshActor.h"
#include "MessageManager.hpp"
#include "SocketTest.generated.h"

UCLASS()
class RECONSTRUCTIONWORLD_API ASocketTest : public AActor
{
	GENERATED_BODY()
	
public:	
	FSocket* socket;
	//目前已经注册过的message manager
	MessageManager* launchedManager = nullptr;

	// Sets default values for this actor's properties
	ASocketTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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
	void launchMessageManager();

	// 用于测试从文件中读取mesh的接口
	UFUNCTION(BlueprintCallable)
	void loadMeshFromFile(UDynamicMesh* mesh);
};
