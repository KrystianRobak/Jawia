using UnrealBuildTool;

public class ReGraphBaseRuntime : ModuleRules
{
	public ReGraphBaseRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core",
				"CoreUObject",
				"Engine"
			});
	}
}
