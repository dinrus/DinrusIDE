#include "Android.h"

namespace РНЦП {

void AndroidMakeFile::AppendString(
	Ткст& makeFile,
    const Ткст& variable,
    const Ткст& variableName)
{
	if(!variable.пустой())
		makeFile << variableName << " := " << variable << "\n";
}

void AndroidMakeFile::AppendStringVector(
	Ткст& makeFile,
    const Вектор<Ткст>& vec,
    const Ткст& variableName,
    const Ткст& variablePrefix,
    const Ткст& variableSuffix)
{
	if(vec.пустой()) {
		return;
	}
	
	makeFile << variableName << " := ";
	for(int i = 0; i < vec.дайСчёт(); ++i) {
		makeFile << variablePrefix << vec[i] << variableSuffix;
		if(i + 1 < vec.дайСчёт())
			makeFile << " ";
	}
	makeFile << "\n";
}

AndroidMakeFile::AndroidMakeFile()
{
	hasHeader = false;
}

AndroidMakeFile::~AndroidMakeFile()
{
	
}

bool AndroidMakeFile::пустой() const
{
	return вТкст().пустой();
}

void AndroidMakeFile::очисть()
{
	this->hasHeader = false;
}

void AndroidMakeFile::AddHeader()
{
	this->hasHeader = true;
}

void AndroidMakeFile::AddInclusion(const Ткст& inclusion)
{
	inclusions.добавь(inclusion);
}

void AndroidMakeFile::AddModuleImport(const Ткст& moduleName)
{
	importedModules.добавь(moduleName);
}

Ткст AndroidMakeFile::вТкст() const
{
	Ткст makeFile;
	
	if(hasHeader)
		AppendHeader(makeFile);
	AppendInclusions(makeFile);
	AppendImportedModules(makeFile);
	
	return makeFile;
}

void AndroidMakeFile::AppendHeader(Ткст& makeFile) const
{
	makeFile << "LOCAL_PATH := $(call my-dir)\n";
}

void AndroidMakeFile::AppendInclusions(Ткст& makeFile) const
{
	if(!inclusions.пустой())
		makeFile << "\n";
	for(int i = 0; i < inclusions.дайСчёт(); i++)
		makeFile << "include $(LOCAL_PATH)" << DIR_SEPS << inclusions[i] << "\n";
}

void AndroidMakeFile::AppendImportedModules(Ткст& makeFile) const
{
	if(!importedModules.пустой())
		makeFile << "\n";
	for(int i = 0; i < importedModules.дайСчёт(); i++)
		makeFile << "$(call import-module, " << importedModules[i] << ")\n";
}

}
