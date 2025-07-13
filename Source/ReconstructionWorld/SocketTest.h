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
	//Ŀǰ�Ѿ�ע�����message manager
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

	//�������ݵĲ��Խӿ�
	UFUNCTION(BlueprintCallable)
	void sendData();

	//���Խ�������
	//���ڿ�����ʱʹ�ð���������
	UFUNCTION(BlueprintCallable)
	void receiveData();

	//����ͼƬ�Ĳ���
	UFUNCTION(BlueprintCallable)
	void loadImageTest();

	// ����Hello��Ϣ
	UFUNCTION(BlueprintCallable)
	void sendHello();

	UFUNCTION(BlueprintCallable)
	void launchMessageManager();

	// ���ڲ��Դ��ļ��ж�ȡmesh�Ľӿ�
	UFUNCTION(BlueprintCallable)
	void loadMeshFromFile(UDynamicMesh* mesh);
};
