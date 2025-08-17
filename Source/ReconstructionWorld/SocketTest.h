// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeometryActors/GeneratedDynamicMeshActor.h"
#include "MessageManager.hpp"
#include"Containers/CircularQueue.h"
#include"MeshSolver.hpp"
#include "SocketTest.generated.h"

UCLASS()
class RECONSTRUCTIONWORLD_API ASocketTest : public AActor
{
	GENERATED_BODY()
	
public:	
	FSocket* socket;
	//Ŀǰ�Ѿ�ע�����message manager
	MessageManager* launchedManager = nullptr;
	// ��Ҫ����ӵ�mesh solver
	// ͼ��3D������ɵ�mesh�ᱻ�Ž�������
	TQueue<MeshSolver*> meshTaskQueue;

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

	// �ж��Ƿ��д����ɵĶ���
	UFUNCTION(BlueprintCallable)
	bool judgeHaveMeshToLoad();

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
	void GenerateMeshFromImage(FString imgPath);

	UFUNCTION(BlueprintCallable)
	void launchMessageManager();

	// ���ڲ��Դ��ļ��ж�ȡmesh�Ľӿ�
	UFUNCTION(BlueprintCallable)
	UTexture2D* loadMeshFromFile(UDynamicMesh* mesh);
};
