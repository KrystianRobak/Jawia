using UnrealBuildTool;

public class ReGraphDialogueEditor : ModuleRules
{
	public ReGraphDialogueEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"ReGraphBaseRuntime",
				"ReGraphBaseEditor",
				"ReGraphDialogueRuntime"
			});

		PrivateDependencyModuleNames.AddRange(
			new[]
			{
				"AssetTools",
				"EditorFramework",
				"GraphEditor",
				"PropertyEditor",
				"Slate",
				"SlateCore",
				"ToolMenus",
				"UnrealEd"
			});
	}
}
