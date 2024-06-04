#include "OrbSlamRunnable.h"

FOrbSlamRunnable::FOrbSlamRunnable()
{
	Thread = FRunnableThread::Create(this, TEXT("ORB SLAM3 Thread"));
}

FOrbSlamRunnable::~FOrbSlamRunnable()
{
    if(SLAM != nullptr)
    {
        delete SLAM;
        SLAM = nullptr;
    }
	if (Thread != nullptr)
	{
		Thread->Kill(true);
		delete Thread;
	}
}

uint32 FOrbSlamRunnable::Run()
{
    cap.open(0);
    if (!cap.isOpened()) {
        UE_LOG(LogTemp, Error, TEXT("Failed to open OpenCV camera 0"));
        return 1;
    }

    FString projectDir = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*FPaths::ProjectDir());
	FString slamDir = projectDir + "OrbSlam3/";
	FString vocabPath = slamDir + "ORBvoc.txt";
	FString settingsPath = slamDir + "unreal.yaml";
	SLAM = new ORB_SLAM3::System(TCHAR_TO_UTF8(*vocabPath), TCHAR_TO_UTF8(*settingsPath), ORB_SLAM3::System::MONOCULAR, true);


    UE_LOG(LogTemp, Warning, TEXT("ORB SLAM3 Initialized"));
    while(!bStopThread)
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

    SLAM->Shutdown();
    UE_LOG(LogTemp, Warning, TEXT("Shutting down ORB SLAM3"));

    while(!SLAM->isShutDown())
    {
        UE_LOG(LogTemp, Warning, TEXT("Zoop"));
    }

    return 0;
}

void FOrbSlamRunnable::Stop()
{
    bStopThread = true;
}

void FOrbSlamRunnable::Exit()
{

}