// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OrbSlam3 : ModuleRules
{
	public OrbSlam3(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		bUseRTTI = true;
		bEnableExceptions = true;
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
				"OpenCV"
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				
				"OrbSlam3Library",
				"Projects",
				"Networking",
				"Sockets",
				"OpenCV4",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
