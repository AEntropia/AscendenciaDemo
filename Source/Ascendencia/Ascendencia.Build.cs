// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Ascendencia : ModuleRules
{
	public Ascendencia(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"NavigationSystem",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"Niagara",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Ascendencia",
			"Ascendencia/Variant_Strategy",
			"Ascendencia/Variant_Strategy/UI",
			"Ascendencia/Variant_TwinStick",
			"Ascendencia/Variant_TwinStick/AI",
			"Ascendencia/Variant_TwinStick/Gameplay",
			"Ascendencia/Variant_TwinStick/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
