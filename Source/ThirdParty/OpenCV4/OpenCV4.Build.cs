// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class OpenCV4 : ModuleRules
{
	public OpenCV4(ReadOnlyTargetRules Target) : base(Target)
	{
		System.Console.WriteLine("ZOOOOP");
		Type = ModuleType.External;
		bUseRTTI = true;
		bEnableExceptions = true;
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Includes"));
		
        if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			//string[] test = new string[]
			string[] libs = new string[18] 
			{
				"libopencv_calib3d.so", 
				"libopencv_core.so",
				"libopencv_dnn.so",
				"libopencv_features2d.so",
				"libopencv_flann.so",
				"libopencv_gapi.so",
				"libopencv_highgui.so",
				"libopencv_imgcodecs.so",
				"libopencv_imgproc.so",
				"libopencv_ml.so",
				"libopencv_objdetect.so",
				"libopencv_photo.so",
				"libopencv_stitching.so",
				"libopencv_video.so",
				"libopencv_videoio.so",
				"libjpeg.so",
				"libopenjp2.so",
				"libopencv_world.so"
			};
			for(int i = 0; i < 18; i++)
			{
				//string libname = "libopencv_world.so";
				string libname = libs[i];
				string path = Path.Combine(ModuleDirectory, "Libraries", libname);
				System.Console.WriteLine(path);
				PublicAdditionalLibraries.Add(path);
				RuntimeDependencies.Add(path);
			}
			
			
		
			//string staticLibraryPath = Path.Combine("$(PluginDir)", "Binaries", "ThirdParty", "OrbSlam3Library", "Linux", "x86_64-unknown-linux-gnu", "libExampleLibrary.so");
			/*string ExampleSoPath = Path.Combine("$(PluginDir)", "Binaries", "ThirdParty", "OrbSlam3Library", "Linux", "x86_64-unknown-linux-gnu", "libExampleLibrary.so");
			PublicAdditionalLibraries.Add(ExampleSoPath);
			PublicDelayLoadDLLs.Add(ExampleSoPath);
			RuntimeDependencies.Add(ExampleSoPath);*/
		}
	}
}
