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
#include"ImagePackageInfo.hpp"
#include"RequestFaceUvMessage.hpp"
#include"FaceUvFinishMessage.hpp"
#include"MeshFinishFunctor.hpp"
#include"ImageCommLib.hpp"
#include"ReconstructionCommLib.hpp"
#include"ReconstructionMessage.hpp"
#include"ReconstructionPackage.hpp"
#include"ReconSingleFileImg.hpp"
#include"ReconSingleImgMsg.hpp"
#include"ReconRecvMsg.hpp"
#include"ReconBeginMsg.hpp"
#include"ReconResultMsg.hpp"
#include"FilePathGenSource.hpp"

#include"ArrayImage.hpp"
#include"ImageList.hpp"

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

UTexture2D* ASocketTest::buildFromMeshQueue(UDynamicMesh* mesh)
{
	//UE_LOG(LogTemp, Warning, TEXT("buildFromMeshQueue"));
	//Ptr<MeshSolver> ueMesh;
	//if (this->meshTaskQueue.Dequeue(ueMesh)) {
	//	// ����generator����mesh
	//	ueMesh->buildUeMesh(mesh);
	//	// ��mesh solver�����ȡue texture
	//	UTexture2D* texture = ueMesh->makeUeTexture();
	//	return texture;
	//}
	return nullptr;
}

UMeshDescriptor* ASocketTest::getMeshDescriptorFromQueue()
{
	Ptr<MeshSolver> ueMesh;
	if (this->meshTaskQueue.Dequeue(ueMesh)) {
		UMeshDescriptor* ret = NewObject<UMeshDescriptor>();
		ret->initMeshSolver(ueMesh);
		return ret;
	}
	return nullptr;
}

bool ASocketTest::judgeHaveMeshToLoad()
{
	return !this->meshTaskQueue.IsEmpty();
}

bool ASocketTest::judgeHaveSplatLoad()
{
	return !this->splatTaskQueue.IsEmpty(); 
}

void ASocketTest::testSocket()
{
	//���õ���socketָ��
	TSharedPtr<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	//IP��ַ
	FString ipString = "192.168.2.156";
	//�˿ں�
	int port = 54321;
	//ָ��ip��ַ
	bool ipValid;
	Addr->SetIp(*ipString, ipValid);
	//�ж��Ƿ����óɹ�
	if (ipValid) {
		UE_LOG(LogTemp, Warning, TEXT("ip set ok!"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ip set failed!"));
	}
	//���ö˿ں�
	Addr->SetPort(port);
	//�ѽ�����Ϣ���ó�����״̬
	this->socket = FTcpSocketBuilder(TEXT("TcpClient")).AsBlocking().WithReceiveBufferSize(2 * 1024 * 1024);
	//��socket����ָ����ַ
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

	// �������ݲ���
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
			//�����յ�����
			char recData[1024];

			int32 ReadBytes = 0;
			// ��ȡ���ݵ��ֽ�����
			socket->Recv((uint8*)recData, Size, ReadBytes);
			UE_LOG(LogTemp, Warning, TEXT("read byte: %d\n"), ReadBytes);
			UE_LOG(LogTemp, Warning, TEXT("message: %s\n"), *FString(recData));
		}
	}
}

void ASocketTest::loadImageTest()
{
	//ԭʼ�ļ���·��
	FString filePath = "E:/temp/test2.jpeg";
	//���ڴ洢��ȡ���ݵ�����
	TArray<uint8> RawFileData;
	//��ȡԭʼͼƬ�ļ����������
	if (!FFileHelper::LoadFileToArray(RawFileData, *filePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Load image file failed!"));
		return;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Load image ok!"));
	}
	//׼��image warper�����module
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	//���ɻ�ȡͼƬ��warper
	auto imageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
	//��ͼƬ����ע���image wrapper
	imageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num());
	//��imageWrapper����ת����BGRA����
	//����ԭʼ��bgra����
	TArray<uint8> uncompressImageData;
	imageWrapper->GetRaw(ERGBFormat::RGBA, 8, uncompressImageData);
	//��Linux�˷���ͼƬ
	if (!socket) {
		UE_LOG(LogTemp, Warning, TEXT("Invlaid socket"));
		return;
	}
	//��ʽ��������
	int32 sentByte = 0;
	socket->Send((uint8*)uncompressImageData.GetData(), uncompressImageData.Num(), sentByte);
	//��ӡ�ֽ�����ʵ�ʷ��ͳ�ȥ���ֽ���
	UE_LOG(LogTemp, Warning, TEXT("Actual: %d, Sent: %d\n"), uncompressImageData.Num(), sentByte);
	
}

void ASocketTest::sendHello()
{
	//ͨ���Ѿ�������manager����һ����Ϣ
	if (launchedManager != nullptr) {

		// ���mesh���ɺ󣬽�mesh��ӵ����ض�������
		auto meshFinishFunctor = [&](Ptr<MeshSolver> mesh, uint32_t idPackage) {
			// ��mesh��ӵ����ض�������
			meshTaskQueue.Enqueue(mesh);
		};

		// �½�ͼƬ
		auto image = makePtr<ArrayImage>("E:/temp/car.jpeg");
		// �½�����ͼƬ����Ϣ
		ImageMessage imageMessage(image, makePtr<ImageFuncEndOperation>(
			[this, meshFinishFunctor](Ptr<ImageSolver> image, uint32_t idPackage) {
				// ����Ŀ�����ͼƬ����mesh
				HunyuanMeshGenMessage meshGenMessage(idPackage,
					makePtr<MeshFinishLambdaFunctor>(meshFinishFunctor));
				this->launchedManager->sendMessage(meshGenMessage);
		}));
		// ����ͼƬ��Ϣ
		launchedManager->sendMessage(imageMessage);

		// ����mesh���Ե���Ϣ
		/*MeshTestMessage testMessage;
		launchedManager->sendMessage(testMessage);*/
	}
}

void ASocketTest::GenerateMeshFromImage(FString imgPath)
{
	// ���mesh���ɺ󣬽�mesh��ӵ����ض�������
	auto meshFinishFunctor = [this, imgPath](Ptr<MeshSolver> mesh, uint32_t idPackage) {
		UE_LOG(LogTemp, Warning, TEXT("Finish mesh functor"));
		// ��¼MeshGen����Դ
		mesh->meshGenSource = makePtr<FilePathGenSource>(
			FormatLibrary::convertToStdString(imgPath));
		// ��mesh��ӵ����ض�������
		this->meshTaskQueue.Enqueue(mesh);
	};

	// ͼƬ�������ʱ��ִ���߼�
	auto imgEndOperator = makePtr<ImageFuncEndOperation>(
		[this, meshFinishFunctor](Ptr<ImageSolver> image, uint32_t idPackage) {
			// ����ͼƬ����������mesh������
			HunyuanMeshGenMessage meshGenMessage(idPackage,
				makePtr<MeshFinishLambdaFunctor>(meshFinishFunctor));
			this->launchedManager->sendMessage(meshGenMessage);
	});

	//ͨ���Ѿ�������manager����һ����Ϣ
	if (launchedManager != nullptr) {
		// �½�ͼƬ
		auto image = makePtr<ArrayImage>(imgPath);
		// �½�����ͼƬ����Ϣ
		sendImage(image,
			launchedManager, imgEndOperator);

		// ����mesh���Ե���Ϣ
		/*MeshTestMessage testMessage;
		launchedManager->sendMessage(testMessage);*/
	}
}

void ASocketTest::EditMeshByImage(UMeshDescriptor* meshDesc, FString prompt)
{
	// ��mesh���������ȡ MeshSolver
	std::string stdImgPath = 
		meshDesc->meshSolver->meshGenSource->getImgPath();
	FString imgPath = FormatLibrary::convertToFString(stdImgPath);
	// ���mesh���ɺ󣬽�mesh��ӵ����ض�������
	auto meshFinishFunctor = [this, imgPath](Ptr<MeshSolver> mesh, uint32_t idPackage) {
		UE_LOG(LogTemp, Warning, TEXT("Finish mesh functor"));
		// ��¼MeshGen����Դ
		mesh->meshGenSource = makePtr<FilePathGenSource>(
			FormatLibrary::convertToStdString(imgPath));
		// ��mesh��ӵ����ض�������
		this->meshTaskQueue.Enqueue(mesh);
		};

	// ͼƬ�������ʱ��ִ���߼�
	auto imgEndOperator = makePtr<ImageFuncEndOperation>(
		[this, meshFinishFunctor](Ptr<ImageSolver> image, uint32_t idPackage) {
			// ����ͼƬ����������mesh������
			HunyuanMeshGenMessage meshGenMessage(idPackage,
			makePtr<MeshFinishLambdaFunctor>(meshFinishFunctor));
		this->launchedManager->sendMessage(meshGenMessage);
	});

	//ͨ���Ѿ�������manager����һ����Ϣ
	if (launchedManager != nullptr) {
		// �½�ͼƬ
		auto image = makePtr<ArrayImage>(imgPath);
		// �½�����ͼƬ����Ϣ
		sendImage(image,
			launchedManager, imgEndOperator);

		// ����mesh���Ե���Ϣ
		/*MeshTestMessage testMessage;
		launchedManager->sendMessage(testMessage);*/
	}
}

void ASocketTest::sendReconRequest(FString imgFolder)
{
	// �����ؽ���ɵĻص�
	// ����ľ������ݺ�����ʵ��
	auto reconFinishFunctor = makePtr<ReconstructionFinishLambda>(
		[this](Ptr<GaussianSplatSolver> gsSplat) {
			UE_LOG(LogTemp, Warning, TEXT("Saving Gaussian splat to queue"));
			// �ڱ��ض��������¼��Щ����
			this->splatTaskQueue.Enqueue(gsSplat);
	});
	// �����ؽ��������Ϣ
	requestReconstruction(FormatLibrary::convertToStdString(imgFolder),
		reconFinishFunctor, this->launchedManager, this->launchedManager->streamInterface);
}

void ASocketTest::launchMessageManager()
{
	// ���manager�Ѿ���������ֱ���˳�
	if (this->launchedManager != nullptr) {
		return;
	}
	//�½�һ��ͨ�Žӿ�
	auto commPtr = new AdvCommunication();
	//�����˽�������
	commPtr->connect("192.168.2.156", 23456);
	//�½�һ��manager
	auto manager = new MessageManager(commPtr);
	// ��manager����ע����Ϣ
	manager->registerMessage(new HelloMessage());
	manager->registerMessage(new ImageMessage(nullptr, nullptr));
	manager->registerMessage(new ImageReceiveMessage());
	manager->registerMessage(new ImageEndMessage(0,0,0,0));
	manager->registerMessage(new ImageRowData(nullptr, 0, 0, 0));
	manager->registerMessage(new HunyuanMeshGenMessage(0, nullptr));
	manager->registerMessage(new MeshMessage());
	manager->registerMessage(new VertexArrayBack());
	manager->registerMessage(new VertexFinishMessage());
	manager->registerMessage(new RequestMeshVertices(0,0));
	manager->registerMessage(new MeshTestMessage());
	manager->registerMessage(new RequestFaceMessage(0,0));
	manager->registerMessage(new FaceArrayBack());
	manager->registerMessage(new FaceFinishMessage());
	manager->registerMessage(new LongArrayMessage());
	manager->registerMessage(new RequestLongArrayMessage(
		LongArrayBackMessage::messageSend));
	manager->registerMessage(new LongArrayBackMessage(0,0));
	manager->registerMessage(new UvVertexFinishMessage());
	manager->registerMessage(new RequestTextureMessage(0));
	manager->registerMessage(new TextureFinishMessage());
	manager->registerMessage(new RequestFaceUvMessage(0));
	manager->registerMessage(new FaceUvFinishMessage());
	manager->registerMessage(new ReconstructionMessage(0));
	manager->registerMessage(new ReconRecvMsg());
	manager->registerMessage(new ReconBeginMsg(0));
	manager->registerMessage(new ReconResultMsg());
	manager->registerMessage(new ReconSingleFileImg(0,0,0));
	manager->registerMessage(new ReconSingleImgMsg(0, 0));
	
	
	// �½�һ����ѭ����ִ����
	MessageRunner* runner = new MessageRunner(manager);
	// ��ʼִ���߳�
	FRunnableThread* tempThread = FRunnableThread::Create(runner, TEXT("Message thread"));
	this->launchedManager = manager;
}

UTexture2D* ASocketTest::loadMeshFromFile(UDynamicMesh* mesh)
{
	// ���ص�mesh�ļ�
	std::string meshFile = "E:/temp/ue_mesh.bin";
	// ��ʼ��һ��mesh
	auto ueMesh = UEMeshSolver::create();
	ueMesh->loadFromFile(meshFile);
	// ����generator����mesh
	ueMesh->buildUeMesh(mesh);
	// ��mesh solver�����ȡue texture
	UTexture2D* texture = ueMesh->makeUeTexture();
	return texture;
}

void ASocketTest::loadGsSplatToScene(UGaussianDescriptor* descriptor)
{
	// ȡ����һ��gaussian splatting���ļ�
	Ptr<GaussianSplatSolver> gsSplat;
	if (splatTaskQueue.Dequeue(gsSplat)) {
		// ����solver��������3D Gaussian���ݵ��߼�
		gsSplat->fillGsScene(descriptor);
	}
	else {
		throw std::runtime_error("Read Gaussian Splat Queue failed");
	}
}

