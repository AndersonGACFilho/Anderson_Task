using UnrealBuildTool;
using System.Collections.Generic;

public class Anderson_TaskEditorTarget : TargetRules
{
	public Anderson_TaskEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
		ExtraModuleNames.Add("Anderson_Task");
	}
}
