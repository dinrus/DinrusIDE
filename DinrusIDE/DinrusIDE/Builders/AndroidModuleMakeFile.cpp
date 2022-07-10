#include "Android.h"

namespace РНЦП {

AndroidModuleMakeFile::AndroidModuleMakeFile()
{
	
}

AndroidModuleMakeFile::AndroidModuleMakeFile(const Ткст& имя)
{
	this->имя = имя;
}

AndroidModuleMakeFile::~AndroidModuleMakeFile()
{
	
}

void AndroidModuleMakeFile::очисть()
{
	имя.очисть();
	sourceFiles.очисть();
	includes.очисть();
	cppFlags.очисть();
	ldLibraries.очисть();
	staticLibraries.очисть();
	sharedLibraries.очисть();
}

Ткст AndroidModuleMakeFile::вТкст() const
{
	Ткст makeFile;
	
	makeFile << "include $(CLEAR_VARS)\n";
	AppendName(makeFile);
	AppendSourceFiles(makeFile);
	AppendIncludes(makeFile);
	AppendCppFlags(makeFile);
	AppendLdLibraries(makeFile);
	AppendStaticLibraries(makeFile);
	AppendSharedLibraries(makeFile);
	makeFile << "include $(BUILD_SHARED_LIBRARY)\n";
	AppendModules(makeFile);
	
	return makeFile;
}

void AndroidModuleMakeFile::AddSourceFile(const Ткст& path)
{
	sourceFiles.добавь(path);
}

void AndroidModuleMakeFile::AddInclude(const Ткст& includePath)
{
	includes.найдиДобавь(includePath);
}

void AndroidModuleMakeFile::AddCppFlag(const Ткст& имя, const Ткст& значение)
{
	cppFlags.добавь(имя, значение);
}

void AndroidModuleMakeFile::AddLdLibrary(const Ткст& ldLibrary)
{
	ldLibraries.добавь(ldLibrary);
}

void AndroidModuleMakeFile::AddStaticLibrary(const Ткст& staticLibrary)
{
	staticLibraries.добавь(staticLibrary);
}

void AndroidModuleMakeFile::AddStaticModuleLibrary(const Ткст& staticModuleLibrary)
{
	staticModuleLibraries.добавь(staticModuleLibrary);
}

void AndroidModuleMakeFile::AddSharedLibrary(const Ткст& sharedLibrary)
{
	sharedLibraries.добавь(sharedLibrary);
}

void AndroidModuleMakeFile::AppendName(Ткст& makeFile) const
{
	makeFile << "LOCAL_MODULE := " << имя << "\n";
}

void AndroidModuleMakeFile::AppendSourceFiles(Ткст& makeFile) const
{
	AndroidMakeFile::AppendStringVector(makeFile, sourceFiles, "LOCAL_SRC_FILES");
}

void AndroidModuleMakeFile::AppendIncludes(Ткст& makeFile) const
{
	AndroidMakeFile::AppendStringVector(makeFile, includes.дайКлючи(), "LOCAL_C_INCLUDES");
}

void AndroidModuleMakeFile::AppendCppFlags(Ткст& makeFile) const
{
	if(cppFlags.пустой()) {
		return;
	}
	
	makeFile << "LOCAL_CPPFLAGS := ";
	for(int i = 0; i < cppFlags.дайСчёт(); i++) {
		Ткст значение = cppFlags[i];
		makeFile << "-D" << cppFlags.дайКлюч(i);
		if(!значение.пустой())
			makeFile << "=" << значение;
		if(i + 1 < sourceFiles.дайСчёт())
			makeFile << " ";
	}
	makeFile << "\n";
}

void AndroidModuleMakeFile::AppendLdLibraries(Ткст& makeFile) const
{
	AndroidMakeFile::AppendStringVector(makeFile, ldLibraries, "LOCAL_LDLIBS", "-l");
}

void AndroidModuleMakeFile::AppendStaticLibraries(Ткст& makeFile) const
{
	Вектор<Ткст> allLibs;
	allLibs.приставь(staticLibraries);
	allLibs.приставь(staticModuleLibraries);
	
	AndroidMakeFile::AppendStringVector(makeFile, allLibs, "LOCAL_STATIC_LIBRARIES");
}

void AndroidModuleMakeFile::AppendSharedLibraries(Ткст& makeFile) const
{
	AndroidMakeFile::AppendStringVector(makeFile, sharedLibraries, "LOCAL_SHARED_LIBRARIES");
}

void AndroidModuleMakeFile::AppendModules(Ткст& makeFile) const
{
	if(staticModuleLibraries.дайСчёт()) {
		for(int i = 0; i < staticModuleLibraries.дайСчёт(); i++) {
			if(i == 0)
				makeFile << "\n";
			
			const Ткст androidPrefix = "android_";
			
			Ткст module = staticModuleLibraries[i];
			if(module.начинаетсяС(androidPrefix))
				module.удали(0, androidPrefix.дайСчёт());
			module = "android/" + module;
			
			makeFile << "$(call import-module, " << module << ")\n";
		}
	}
}

}
