#include "AndroidBuilder.h"

namespace Upp {

AndroidBuilderCommands::AndroidBuilderCommands(
	AndroidProject* projectPtr, AndroidSDK* sdkPtr, Jdk* jdkPtr)
	: projectPtr(projectPtr)
	, sdkPtr(sdkPtr)
	, jdkPtr(jdkPtr)
{
	ASSERT_(projectPtr, "Проект не инициирован.");
	ASSERT_(sdkPtr, "Android sdk не инициирован.");
	ASSERT_(jdkPtr, "JDK не инициирован.");
}

String AndroidBuilderCommands::PreperCompileJavaSourcesCommand(
		const Vector<String>& sources)
{
	String cmd;

	cmd << NormalizeExePath(jdkPtr->GetJavacPath());

	if (jdkPtr->GetVersion().IsGreaterOrEqual(9)) {
		cmd << " -source 1.8 -target 1.8";
	}

	cmd << (projectPtr->IsDebug() ? " -g" : " -g:none");
	cmd << " -d "<< projectPtr->GetClassesDir();
	cmd << " -classpath ";
	cmd << NormalizeExePath(sdkPtr->AndroidJarPath()) << Java::GetDelimiter();
	cmd << projectPtr->GetBuildDir();
	cmd << " -sourcepath ";
	cmd << projectPtr->GetJavaDir() << " ";
	for(int i = 0; i < sources.GetCount(); ++i) {
		cmd << sources[i];
		if(i < sources.GetCount() - 1)
			cmd << " ";
	}

	return cmd;
}

}