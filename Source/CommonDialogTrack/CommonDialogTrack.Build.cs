// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CommonDialogTrack : ModuleRules
{
	public CommonDialogTrack(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"MovieSceneTracks",
				"MovieScene",
				"CommonDialog"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Slate",
				"SlateCore",
				"LevelSequence"
				// ... add private dependencies that you statically link with here ...	
			}
			);

	}
}
