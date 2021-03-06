// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TerritoryFight : ModuleRules
{
	public TerritoryFight(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "OnlineSubsystem", "OnlineSubsystemSteam", "NavigationSystem", "AIModule" });
	}
}
