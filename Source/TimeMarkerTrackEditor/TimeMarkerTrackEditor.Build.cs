// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TimeMarkerTrackEditor : ModuleRules
{
	public TimeMarkerTrackEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Engine",
				"MovieSceneTracks",
				"MovieScene",
				"Sequencer",
				"CoreUObject",
				"TimeMarkerTrack",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Slate",
				"SlateCore",
				"EditorStyle",
				"SequencerCore",
				"UnrealEd",
				// ... add private dependencies that you statically link with here ...	
			}
			);
	}
}
