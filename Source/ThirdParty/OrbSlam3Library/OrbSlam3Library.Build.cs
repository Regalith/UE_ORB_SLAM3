// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class OrbSlam3Library : ModuleRules
{
	public OrbSlam3Library(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		bUseRTTI = true;
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Includes"));
		
        if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			string libname = "libORB_SLAM3.so";
			string path = Path.Combine(ModuleDirectory, "Libraries", libname);
			PublicAdditionalLibraries.Add(path);
			RuntimeDependencies.Add(path);
			//string staticLibraryPath = Path.Combine("$(PluginDir)", "Binaries", "ThirdParty", "OrbSlam3Library", "Linux", "x86_64-unknown-linux-gnu", "libExampleLibrary.so");
			/*string ExampleSoPath = Path.Combine("$(PluginDir)", "Binaries", "ThirdParty", "OrbSlam3Library", "Linux", "x86_64-unknown-linux-gnu", "libExampleLibrary.so");
			PublicAdditionalLibraries.Add(ExampleSoPath);
			PublicDelayLoadDLLs.Add(ExampleSoPath);
			RuntimeDependencies.Add(ExampleSoPath);*/
		}
	}
}
