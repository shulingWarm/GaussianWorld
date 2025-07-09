// Fill out your copyright notice in the Description page of Project Settings.


#include "SocketTest.h"
#include "Sockets.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "MessageManager.hpp"
#include"AdvCommunicate.hpp"
#include "HelloMessage.hpp"
#include "MessageRunner.hpp"
#include"ImageMessage.hpp"
#include"ImageReceiveMessage.hpp"
#include"ImageEndMessage.hpp"
#include"ImageRowDataMessage.hpp"
#include"HunyuanMeshGenMessage.hpp"
#include"MeshMessage.hpp"
#include"VertexArrayBack.hpp"
#include"RequestMeshVertices.hpp"
#include"VertexFinishMessage.hpp"
#include"MeshTestMessage.hpp"
#include"RequestFaceMessage.hpp"
#include"FaceArrayBack.hpp"
#include"FaceFinishMessage.hpp"
#include"LongArrayMessage.hpp"
#include"LongArrayBackMessage.hpp"
#include"RequestLongArrayMessage.hpp"
#include"UvVertexFinishMessage.hpp"
#include"RequestTextureMessage.hpp"
#include"TextureFinishMessage.hpp"
#include"RequestFaceUvMessage.hpp"
#include"FaceUvFinishMessage.hpp"

#include"ArrayImage.hpp"

#include "Internationalization/Text.h"
#include "Misc/OutputDevice.h"
#include "Internationalization/Internationalization.h"
#include "Common/TcpSocketBuilder.h"

// Sets default values
ASocketTest::ASocketTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASocketTest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASocketTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASocketTest::testSocket()
{
	//最后得到的socket指针
	TSharedPtr<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	//IP地址
	FString ipString = "192.168.2.156";
	//端口号
	int port = 54321;
	//指定ip地址
	bool ipValid;
	Addr->SetIp(*ipString, ipValid);
	//判断是否设置成功
	if (ipValid) {
		UE_LOG(LogTemp, Warning, TEXT("ip set ok!"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ip set failed!"));
	}
	//设置端口号
	Addr->SetPort(port);
	//把接收消息设置成阻塞状态
	this->socket = FTcpSocketBuilder(TEXT("TcpClient")).AsBlocking().WithReceiveBufferSize(2 * 1024 * 1024);
	//用socket连接指定地址
	bool connected = this->socket->Connect(*Addr);
	if (connected) {
		UE_LOG(LogTemp, Warning, TEXT("Connect ok!"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Connect failed!"));
	}
}

void ASocketTest::sendData()
{
	if (!this->socket)
	{
		UE_LOG(LogTemp, Log, TEXT("Socket is not connected!"));
		return;
	}

	// 发送数据部分
	int32 SentBytes = 0;
	char message[] = "Hello from UE";
	uint32 dataSize = strlen(message);
 	bool bSuccess = this->socket->Send((uint8*)message, dataSize, SentBytes);

	if (!bSuccess || SentBytes != dataSize)
	{
		UE_LOG(LogTemp, Log, TEXT("Failed to send data!"));
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, TEXT("Success to send data!"));
	UE_LOG(LogTemp, Log, TEXT("Success to send data!"));
}

void ASocketTest::receiveData()
{

	if (this->socket)
	{
		uint32 Size;
		while (socket->HasPendingData(Size))
		{
			//待接收的数据
			char recData[1024];

			int32 ReadBytes = 0;
			// 读取数据到字节流中
			socket->Recv((uint8*)recData, Size, ReadBytes);
			UE_LOG(LogTemp, Warning, TEXT("read byte: %d\n"), ReadBytes);
			UE_LOG(LogTemp, Warning, TEXT("message: %s\n"), *FString(recData));
		}
	}
}

void ASocketTest::loadImageTest()
{
	//原始文件的路径
	FString filePath = "E:/temp/test2.jpeg";
	//用于存储读取数据的数组
	TArray<uint8> RawFileData;
	//读取原始图片文件里面的数据
	if (!FFileHelper::LoadFileToArray(RawFileData, *filePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Load image file failed!"));
		return;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Load image ok!"));
	}
	//准备image warper里面的module
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	//生成获取图片的warper
	auto imageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
	//把图片数据注册进image wrapper
	imageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num());
	//从imageWrapper里面转换成BGRA数据
	//这是原始的bgra数据
	TArray<uint8> uncompressImageData;
	imageWrapper->GetRaw(ERGBFormat::RGBA, 8, uncompressImageData);
	//向Linux端发送图片
	if (!socket) {
		UE_LOG(LogTemp, Warning, TEXT("Invlaid socket"));
		return;
	}
	//正式发送数据
	int32 sentByte = 0;
	socket->Send((uint8*)uncompressImageData.GetData(), uncompressImageData.Num(), sentByte);
	//打印字节数和实际发送出去的字节数
	UE_LOG(LogTemp, Warning, TEXT("Actual: %d, Sent: %d\n"), uncompressImageData.Num(), sentByte);
	
}

void ASocketTest::sendHello()
{
	//通过已经启动的manager发送一个信息
	if (launchedManager != nullptr) {
		// 新建图片
		ArrayImage* image = new ArrayImage("E:/temp/car.jpeg");
		// 新建传输图片的消息
		ImageMessage imageMessage(image, new ImageFuncEndOperation(
			[this](ImageSolver* image, uint32_t idPackage) {
				// 请求目标根据图片生成mesh
				HunyuanMeshGenMessage meshGenMessage(idPackage);
				this->launchedManager->sendMessage(meshGenMessage);
		}));
		// 发送图片消息
		launchedManager->sendMessage(imageMessage);

		// 发送mesh测试的消息
		/*MeshTestMessage testMessage;
		launchedManager->sendMessage(testMessage);*/
	}
}

void ASocketTest::launchMessageManager()
{
	//新建一个通信接口
	auto commPtr = new AdvCommunication();
	//与服务端建立连接
	commPtr->connect("192.168.2.156", 23456);
	//新建一个manager
	auto manager = new MessageManager(commPtr);
	// 给manager里面注册信息
	manager->registerMessage(new HelloMessage());
	manager->registerMessage(new ImageMessage(nullptr, nullptr));
	manager->registerMessage(new ImageReceiveMessage());
	manager->registerMessage(new ImageEndMessage(0));
	manager->registerMessage(new ImageRowData(nullptr, 0, 0, 0));
	manager->registerMessage(new HunyuanMeshGenMessage(0));
	manager->registerMessage(new MeshMessage());
	manager->registerMessage(new VertexArrayBack());
	manager->registerMessage(new VertexFinishMessage());
	manager->registerMessage(new RequestMeshVertices(0,0));
	manager->registerMessage(new MeshTestMessage());
	manager->registerMessage(new RequestFaceMessage(0,0));
	manager->registerMessage(new FaceArrayBack());
	manager->registerMessage(new FaceFinishMessage());
	manager->registerMessage(new LongArrayMessage());
	manager->registerMessage(new RequestLongArrayMessage(0,0));
	manager->registerMessage(new LongArrayBackMessage());
	manager->registerMessage(new UvVertexFinishMessage());
	manager->registerMessage(new RequestTextureMessage(0));
	manager->registerMessage(new TextureFinishMessage());
	manager->registerMessage(new RequestFaceUvMessage(0));
	manager->registerMessage(new FaceUvFinishMessage());
	
	
	// 新建一个主循环的执行器
	MessageRunner* runner = new MessageRunner(manager);
	// 开始执行线程
	FRunnableThread* tempThread = FRunnableThread::Create(runner, TEXT("Message thread"));
	this->launchedManager = manager;
}

