// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class iB_TechDemonstrator : ModuleRules
{
	public iB_TechDemonstrator(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
			{ 
				"Core", 
				"CoreUObject", 
				"Engine", 
				"InputCore", 
				"EnhancedInput", 
				"NetCore",
				"AIModule"
			});

		PrivateDependencyModuleNames.AddRange(new string[] 
			{
				"OnlineSubsystem",
				"OnlineSubsystemNull",
				"Networking"
			});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
