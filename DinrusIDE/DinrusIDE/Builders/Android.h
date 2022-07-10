#ifndef _ide_Builders_Android_h_
#define _ide_Builders_Android_h_

#include <Core/Core.h>

namespace РНЦП {

class AndroidProject {
public:
	AndroidProject();
	AndroidProject(const Ткст& dir, bool debug);
	virtual ~AndroidProject() {}
	
	Ткст GetDir() const;
	Ткст GetJavaDir() const;
	Ткст GetJavaDir(const Ткст& package) const;
	Ткст GetJniDir() const;
	Ткст GetJniDir(const Ткст& package) const;
	Ткст GetLibsDir() const;
	Ткст GetResDir() const;
	Ткст GetBuildDir() const;
	Ткст GetClassesDir() const;
	Ткст GetBinDir() const;
	Ткст GetObjDir() const;
	Ткст GetObjLocalDir() const;
	
	Ткст GetManifestPath() const;
	Ткст GetJniMakeFilePath() const;
	Ткст GetJniApplicationMakeFilePath() const;
	
	bool IsDebug() const;
	bool IsRelease() const;
	
private:
	Ткст dir;
	bool   debug;
};

class AndroidApplicationMakeFile {
public:
	AndroidApplicationMakeFile();
	virtual ~AndroidApplicationMakeFile();
	
	Ткст вТкст() const;
	
public:
	void SetPlatform(const Ткст& platform);
	void SetArchitectures(const Вектор<Ткст>& architectures);
	void AddArchitecture(const Ткст& architecture);
	void SetCppRuntime(const Ткст& cppRuntime);
	void SetCppFlags(const Ткст& cppFlags);
	void SetCFlags(const Ткст& cFlags);
	void SetOptim(const Ткст& optim);
	void SetToolchain(const Ткст& toolchain);
	
protected:
	void AppendPlatform(Ткст& makeFile) const;
	void AppendArchitectures(Ткст& makeFile) const;
	void AppendCppRuntime(Ткст& makeFile) const;
	void AppendCppFlags(Ткст& makeFile) const;
	void AppendCFlags(Ткст& makeFile) const;
	void AppendOptim(Ткст& makeFile) const;
	void AppendToolchain(Ткст& makeFile) const;
	
private:
	Ткст         platform;
	Вектор<Ткст> architectures;
	Ткст         cppRuntime;
	Ткст         cppFlags;
	Ткст         cFlags;
	Ткст         toolchain;
	Ткст         optim;
};

class AndroidModuleMakeFile : public Движимое<AndroidModuleMakeFile> {
public:
	AndroidModuleMakeFile();
	AndroidModuleMakeFile(const Ткст& имя);
	virtual ~AndroidModuleMakeFile();

	void очисть();
	Ткст вТкст() const;

public:
	void AddSourceFile(const Ткст& path);
	void AddInclude(const Ткст& includePath);
	void AddCppFlag(const Ткст& имя, const Ткст& значение = "");
	void AddLdLibrary(const Ткст& ldLibrary);
	void AddStaticLibrary(const Ткст& staticLibrary);
	void AddStaticModuleLibrary(const Ткст& staticModuleLibrary);
	void AddSharedLibrary(const Ткст& sharedLibrary);
	
	Ткст дайИмя() const             { return this->имя; }
	void   SetName(const Ткст& имя) { this->имя = имя; }
	
protected:
	void AppendName(Ткст& makeFile) const;
	void AppendSourceFiles(Ткст& makeFile) const;
	void AppendIncludes(Ткст& makeFile) const;
	void AppendCppFlags(Ткст& makeFile) const;
	void AppendLdLibraries(Ткст& makeFile) const;
	void AppendStaticLibraries(Ткст& makeFile) const;
	void AppendSharedLibraries(Ткст& makeFile) const;
	void AppendModules(Ткст& makeFile) const;
	
private:
	Ткст                    имя;
	Вектор<Ткст>            sourceFiles;
	Индекс<Ткст>             includes;
	ВекторМап<Ткст, Ткст> cppFlags;
	Вектор<Ткст>            ldLibraries;
	Вектор<Ткст>            staticLibraries;
	Вектор<Ткст>            staticModuleLibraries;
	Вектор<Ткст>            sharedLibraries;
};

class AndroidMakeFile {
public:
	static void AppendString(
		Ткст& makeFile,
	    const Ткст& variable,
	    const Ткст& variableName);
	static void AppendStringVector(
		Ткст& makeFile,
	    const Вектор<Ткст>& vec,
	    const Ткст& variableName,
	    const Ткст& variablePrefix = "",
	    const Ткст& variableSuffix = "");
	
public:
	AndroidMakeFile();
	virtual ~AndroidMakeFile();
	
public:
	bool пустой() const;
	void очисть();
	
	bool HasFooter();
	
	void AddHeader();
	void AddInclusion(const Ткст& inclusion);
	void AddModuleImport(const Ткст& moduleName);
	
	Ткст вТкст() const;
	
protected:
	void AppendHeader(Ткст& makeFile) const;
	void AppendInclusions(Ткст& makeFile) const;
	void AppendImportedModules(Ткст& makeFile) const;
	
private:
	bool hasHeader;
	Вектор<Ткст> inclusions;
	Вектор<Ткст> importedModules;
};

}

#endif
