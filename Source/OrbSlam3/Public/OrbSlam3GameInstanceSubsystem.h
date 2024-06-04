// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



THIRD_PARTY_INCLUDES_START
#pragma push_macro("check")
#undef check
#include "System.h"

#pragma pop_macro("check")
THIRD_PARTY_INCLUDES_END

#include "CoreMinimal.h"
#include "Common/UdpSocketReceiver.h"
#include "Sockets.h"
#include <opencv2/core.hpp>
#include "Subsystems/GameInstanceSubsystem.h"
#include "OrbSlamRunnable.h"
#include "OrbSlam3GameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ORBSLAM3_API UOrbSlam3GameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OrbSlam3")
	FString RecieveSocketName = FString("OrbSlam");

	/** Port to listen for UDP messages */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OrbSlam3")
	int32 ReceivePort = 5005;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OrbSlam3")
	int32 BufferSize = 2 * 1024 * 1024;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OrbSlam3")
	int32 CaptureFPS = 30;

	UFUNCTION(BlueprintCallable, Category="OrbSlam3")
	void InitializeOrbSlam();

	UFUNCTION(BlueprintCallable, Category="OrbSlam3")
	void TerminateOrbSlam();

	UFUNCTION(BlueprintCallable, Category="OrbSlam3")
	void StartListenServer();

	void Receive(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& Endpt);

	void GrabNewImage();
private:

	TSharedPtr<FOrbSlamRunnable> OrbSlamThread; 

	FSocket* receiveSocket = nullptr;
	FUdpSocketReceiver* uDPReceiver = nullptr;

	cv::VideoCapture cap;
	cv::Mat imCV;
	ORB_SLAM3::System* SLAM = nullptr;
	char* buffer[100000];
	uint32 LastFrameNumberWeTicked = INDEX_NONE;

	float currentDuration = 0.0f;
};
