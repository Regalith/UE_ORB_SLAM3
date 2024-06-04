// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbSlam3GameInstanceSubsystem.h"
#include "Common/UdpSocketBuilder.h"
#include "SocketSubsystem.h"
#include "Misc/DateTime.h"

THIRD_PARTY_INCLUDES_START
#include "Thirdparty/Sophus/sophus/se3.hpp"
THIRD_PARTY_INCLUDES_END

#define BUFFSIZE 100000


void UOrbSlam3GameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    
    Super::Initialize(Collection);
    OrbSlamThread = MakeShared<FOrbSlamRunnable>();
}

void UOrbSlam3GameInstanceSubsystem::Deinitialize()
{
    OrbSlamThread->Stop();
    
    delete uDPReceiver;
    uDPReceiver = nullptr;

    if(receiveSocket)
    {
        receiveSocket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(receiveSocket);
    }

    Super::Deinitialize();
}

void UOrbSlam3GameInstanceSubsystem::InitializeOrbSlam()
{
    //UE_LOG(LogTemp, Warning, TEXT("ZOOP"));
    FString projectDir = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*FPaths::ProjectDir());
	FString slamDir = projectDir + "OrbSlam3/";
	FString vocabPath = slamDir + "ORBvoc.txt";
	FString settingsPath = slamDir + "webcam.yaml";
	SLAM = new ORB_SLAM3::System(TCHAR_TO_UTF8(*vocabPath), TCHAR_TO_UTF8(*settingsPath), ORB_SLAM3::System::MONOCULAR, true);
    
    cap.open(0);

	if(!cap.isOpened())
	{
		UE_LOG(LogTemp, Error, TEXT("UOrbSlam3GameInstanceSubsystem::BeginPlay: Failed to open webcam stream"));
	}

    //SLAM = new ORB_SLAM3::EdgeSlam(TCHAR_TO_UTF8(*vocabPath), TCHAR_TO_UTF8(*settingsPath));
    //SLAM->InitializeUDPServer("5005");
    //SLAM->SetCallback()
    //cap = new cv::VideoCapture(-1);

}

void UOrbSlam3GameInstanceSubsystem::TerminateOrbSlam()
{
    if(SLAM && !SLAM->isShutDown())
    {
        UE_LOG(LogTemp, Warning, TEXT("UOrbSlam3GameInstanceSubsystem::TerminateOrbSlam: Shutting down system"));
        SLAM->Shutdown();
    }
   
}

void UOrbSlam3GameInstanceSubsystem::StartListenServer()
{
    FIPv4Endpoint inPoint(FIPv4Address::Any, ReceivePort);
                            
    receiveSocket = FUdpSocketBuilder(RecieveSocketName)
        .AsNonBlocking()
        .AsReusable()
        .BoundToEndpoint(inPoint)
        .WithReceiveBufferSize(BufferSize);

    FTimespan threadWaitTime = FTimespan::FromMilliseconds(4);
    uDPReceiver = new FUdpSocketReceiver(receiveSocket, threadWaitTime, *RecieveSocketName);
    if(!uDPReceiver)
    {
        UE_LOG(LogTemp, Error, TEXT("OrbSlam3GameInstanceSubsystem::StartListenServer: UDP Receiver failed to start"));
        return;
    }

    uDPReceiver->OnDataReceived().BindUObject(this, &UOrbSlam3GameInstanceSubsystem::Receive);
    uDPReceiver->Start();

    UE_LOG(LogTemp, Warning, TEXT("OrbSlam3GameInstanceSubsystem::StartListenServer: UDP Receiver started at %d"), ReceivePort);

}

void UOrbSlam3GameInstanceSubsystem::Receive(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& Endpt)
{
    UE_LOG(LogTemp, Warning, TEXT("UOrbSlam3GameInstanceSubsystem::Receive: received %d"), ArrayReaderPtr->TotalSize());
    char* data[ArrayReaderPtr->TotalSize()];
    TArray<uint8> testData = TArray<uint8>();
    testData.AddUninitialized(ArrayReaderPtr->TotalSize());
    //Oh god
    std::string jank;

    

    //Skip if SLAM not running
    /*if(SLAM == nullptr)
    {
        return;
    }*/
    
    ArrayReaderPtr->Serialize(testData.GetData(), ArrayReaderPtr->TotalSize());
    std::vector<uchar> jankier;
    for(int i = 0; i < testData.Num(); i++)
    {
        jank += testData[i];
        jankier.push_back((uchar) testData[i]);
    }
    char* testBuff = new char[ArrayReaderPtr->TotalSize()];
    memcpy(testBuff, jank.c_str(), ArrayReaderPtr->TotalSize() * sizeof(char));
    

    //cv::Mat rawData(1, ArrayReaderPtr->TotalSize() + 1, CV_8UC1, testBuff);
    //cv::Mat resharedMat = rawData.reshape(0, 720);
    //cv::Mat rawData(1, ArrayReaderPtr->TotalSize(), CV_8UC1, (void*) jankier);
    cv::Mat convertedMat = cv::imdecode(jankier, cv::IMREAD_UNCHANGED);
    //cv::Mat convertedMat = cv::imdecode(jankier, 1);
    FDateTime currentTime = FDateTime::Now();
    float timestamp_ms = currentTime.ToUnixTimestamp() * 1000.0f;

    //UE_LOG(LogTemp, Warning, TEXT("Size %d"), ArrayReaderPtr->TotalSize());
    //UE_LOG(LogTemp, Warning, TEXT("jankier size %d"), jankier.size());

    //UE_LOG(LogTemp, Warning, TEXT("Raw = Rows: %d Cols: %d"), rawData.rows, rawData.cols);
    //UE_LOG(LogTemp, Warning, TEXT("Reshaped = Rows: %d Cols: %d"), resharedMat.rows, resharedMat.cols);
    
    //convertedMat = cv::imdecode(testData.GetData(), cv::IMREAD_UNCHANGED);
    //UE_LOG(LogTemp, Warning, TEXT("Converted = Rows: %d Cols: %d"), convertedMat.rows, convertedMat.cols);

    convertedMat = cv::imdecode(jankier, cv::IMREAD_UNCHANGED);
    UE_LOG(LogTemp, Warning, TEXT("Converted = Rows: %d Cols: %d"), convertedMat.rows, convertedMat.cols);

    //convertedMat = cv::imdecode(jank, cv::IMREAD_UNCHANGED);
    //UE_LOG(LogTemp, Warning, TEXT("Converted = Rows: %d Cols: %d"), convertedMat.rows, convertedMat.cols);

    convertedMat = cv::imdecode(cv::Mat(1, ArrayReaderPtr->TotalSize(), CV_8UC1, testBuff), cv::IMREAD_UNCHANGED);
    UE_LOG(LogTemp, Warning, TEXT("Converted = Rows: %d Cols: %d"), convertedMat.rows, convertedMat.cols);
    //free(testBuff);
    //Sophus::SE3 location = SLAM->TrackMonocular(convertedMat, timestamp_ms);
   
    //UE_LOG(LogTemp, Warning, TEXT("%f %f %f"),location.translation()[0], location.translation()[1], location.translation()[2]);
        
}

void UOrbSlam3GameInstanceSubsystem::GrabNewImage()
{
    if(cap.isOpened())
	{
        double timestamp_ms = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
        cap >> imCV;
        float imageScale = SLAM->GetImageScale();

        if (imageScale != 1.f)
        {
            int width = imCV.cols * imageScale;
            int height = imCV.rows * imageScale;
            cv::resize(imCV, imCV, cv::Size(width, height));
        }

        // Pass the image to the SLAM system
        SLAM->TrackMonocular(imCV, timestamp_ms);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("UOrbSlam3GameInstanceSubsystem::GrabNewImage: Camera stream isn't open"));
    }
}