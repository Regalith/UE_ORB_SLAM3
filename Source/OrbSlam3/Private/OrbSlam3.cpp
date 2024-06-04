// Copyright Epic Games, Inc. All Rights Reserved.

#include "OrbSlam3.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "OrbSlam3Library/ExampleLibrary.h"




#define LOCTEXT_NAMESPACE "FOrbSlam3Module"

void FOrbSlam3Module::StartupModule()
{
	//UE_LOG(LogTemp, Warning, TEXT("ZOOP"));
	
}

void FOrbSlam3Module::ShutdownModule()
{
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FOrbSlam3Module, OrbSlam3)
