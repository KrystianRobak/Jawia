using UnrealBuildTool;

public class ReGraphBaseEditor : ModuleRules
{
	public ReGraphBaseEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"ReGraphBaseRuntime"
			});

		PrivateDependencyModuleNames.AddRange(
			new[]
			{
				"ApplicationCore",
				"AssetTools",
				"EditorFramework",
				"GraphEditor",
				"InputCore",
				"PropertyEditor",
				"Slate",
				"SlateCore",
				"ToolMenus",
				"UnrealEd"
			});
	}
}
