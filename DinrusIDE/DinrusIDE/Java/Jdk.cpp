#include "Java.h"

namespace РНЦП {

#define METHOD_NAME "Jdk::" << UPP_FUNCTION_NAME << "(): "

Ткст Jdk::GetDownloadUrl()
{
	return "https://www.azul.com/downloads/zulu-community/?package=jdk";
}

Jdk::Jdk(const Ткст& path, Хост* host)
	: path(path)
{
	ПРОВЕРЬ_(host, "Хост is null.");
	
	FindVersion(host);
}

bool Jdk::Validate() const
{
	if(!файлЕсть(GetJavacPath())) return false;
	
	return true;
}

void Jdk::FindVersion(Хост* host)
{
	if (!Validate()) {
		Logw() << METHOD_NAME << "Path to JDK is wrong or files are corrupted.";
		return;
	}
	
	ТкстПоток ss;
	if (host->выполни(GetJavacPath() + " -version", ss) != 0) {
		Logw() << METHOD_NAME << "Cannot obtain version due to command execution failure.";
		return;
	}
	
	Ткст output = static_cast<Ткст>(ss);
	output.замени("\n", "");
	Вектор<Ткст> splitedOutput = разбей(output, " ");
	if (splitedOutput.дайСчёт() != 2) {
		Logw() << METHOD_NAME << "Splited output is too short (" + output + ").";
		return;
	}
	
	Вектор<Ткст> splitedVersion = разбей(splitedOutput[1], ".");
	if (splitedVersion.дайСчёт() != 3) {
		Logw() << METHOD_NAME << "Splited version is too short (" + output + ").";
		return;
	}
	
	int major = тктЦел(splitedVersion[0]);
	if (major == INT_NULL) {
		Logw() << METHOD_NAME << "Major version conversion to int failed (" + splitedVersion[0] + ").";
		return;
	}
	
	int minor = тктЦел(splitedVersion[1]);
	if (minor == INT_NULL) {
		Logw() << METHOD_NAME << "Minor version conversion to int failed (" + splitedVersion[1] + ").";
		return;
	}
	
	version = JavaVersion(major, minor);
}

}
