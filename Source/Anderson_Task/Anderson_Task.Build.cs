using UnrealBuildTool;

public class Anderson_Task : ModuleRules
{
	public Anderson_Task(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine",
			"InputCore", 
			"EnhancedInput",
			"Niagara",
			"UMG",         
			"Slate",
			"SlateCore"
		});
	}
}
