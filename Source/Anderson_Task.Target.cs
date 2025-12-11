using UnrealBuildTool;
using System.Collections.Generic;

public class Anderson_TaskTarget : TargetRules
{
	public Anderson_TaskTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
		ExtraModuleNames.Add("Anderson_Task");
	}
}
