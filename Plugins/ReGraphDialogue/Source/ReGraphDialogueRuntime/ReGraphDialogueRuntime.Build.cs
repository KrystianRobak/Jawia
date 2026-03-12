using UnrealBuildTool;

public class ReGraphDialogueRuntime : ModuleRules
{
	public ReGraphDialogueRuntime(ReadOnlyTargetRules Target) : base(Target)
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
	}
}
