// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CommonDialog : ModuleRules
{
	public CommonDialog(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"GameplayTags",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Slate",
				"SlateCore",
				"CommonGame",
				"CommonUI"
				// ... add private dependencies that you statically link with here ...	
			}
			);

	}
}
