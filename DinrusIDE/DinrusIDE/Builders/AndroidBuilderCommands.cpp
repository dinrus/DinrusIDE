#include "AndroidBuilder.h"

namespace РНЦП {

AndroidBuilderCommands::AndroidBuilderCommands(
	AndroidProject* projectPtr, AndroidSDK* sdkPtr, Jdk* jdkPtr)
	: projectPtr(projectPtr)
	, sdkPtr(sdkPtr)
	, jdkPtr(jdkPtr)
{
	ПРОВЕРЬ_(projectPtr, "Project is not initialized.");
	ПРОВЕРЬ_(sdkPtr, "Android sdk is not initialized.");
	ПРОВЕРЬ_(jdkPtr, "JDK is not initialized.");
}

Ткст AndroidBuilderCommands::PreperCompileJavaSourcesCommand(
		const Вектор<Ткст>& sources)
{
	Ткст cmd;
	
	cmd << NormalizeExePath(jdkPtr->GetJavacPath());
	
	if (jdkPtr->дайВерсию().IsGreaterOrEqual(9)) {
		cmd << " -source 1.8 -target 1.8";
	}
	
	cmd << (projectPtr->IsDebug() ? " -g" : " -g:none");
	cmd << " -d "<< projectPtr->GetClassesDir();
	cmd << " -classpath ";
	cmd << NormalizeExePath(sdkPtr->AndroidJarPath()) << Java::GetDelimiter();
	cmd << projectPtr->GetBuildDir();
	cmd << " -sourcepath ";
	cmd << projectPtr->GetJavaDir() << " ";
	for(int i = 0; i < sources.дайСчёт(); ++i) {
		cmd << sources[i];
		if(i < sources.дайСчёт() - 1)
			cmd << " ";
	}
	
	return cmd;
}

}
