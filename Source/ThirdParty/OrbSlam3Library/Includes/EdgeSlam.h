#ifndef EDGE_SLAM_H
#define EDGE_SLAM_H

#include <signal.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "System.h"
#include "Thirdparty/Sophus/sophus/se3.hpp"


typedef void(*UnrealCallbackPtr)(void*, Sophus::SE3f);


namespace ORB_SLAM3
{
	class EdgeSlam
	{

	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		//Initialize this janky wrapper 
		EdgeSlam(const std::string &strVocFile, const std::string &strSettingsFile);

		void InitializeUDPServer(const char* port);

		void SetCallback(UnrealCallbackPtr cb, void* p);
	private:
		ORB_SLAM3::System* SLAM;

		UnrealCallbackPtr unrealFunctionPointer = nullptr;
		void* unrealGameInstance = nullptr;
	};	
}

#endif // EDGE_SLAM_H