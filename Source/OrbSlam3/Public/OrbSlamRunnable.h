#pragma once

#pragma once
THIRD_PARTY_INCLUDES_START
#pragma push_macro("check")
#undef check
#include "System.h"

#pragma pop_macro("check")
THIRD_PARTY_INCLUDES_END

#include "HAL/Runnable.h"

class FOrbSlamRunnable : public FRunnable
{
public:
	// Custom constructor for setting up our thread with its target
	FOrbSlamRunnable();
	virtual ~FOrbSlamRunnable();

	// FRunnable functions
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;
	// FRunnable


protected:
	FRunnableThread* Thread = nullptr;

	bool bStopThread = false;

private:
    cv::VideoCapture cap;
	cv::Mat imCV;
	ORB_SLAM3::System* SLAM = nullptr;
};