#ifndef _ide_Builders_AndroidBuilder_h_
#define _ide_Builders_AndroidBuilder_h_

#include "Android.h"

#include <DinrusIDE/Core/Core.h>
#include <DinrusIDE/Android/Android.h>
#include <DinrusIDE/Java/Java.h>

namespace РНЦП {

class AndroidBuilderCommands;

class AndroidBuilder : public Построитель {
public:
	AndroidSDK     sdk;
	AndroidNDK     ndk;
	
	bool           ndk_blitz;
	Вектор<Ткст> ndkArchitectures;
	Ткст         ndkToolchain;
	Ткст         ndkCppRuntime;
	Ткст         ndkCppFlags;
	Ткст         ndkCFlags;
	
public:
	static Индекс<Ткст> GetBuildersNames();
		
public:
	AndroidBuilder();
	
	void SetJdk(Один<Jdk> jdk);
	
	Ткст GetTargetExt() const override;
	bool постройПакет(
		const Ткст& packageName,
		Вектор<Ткст>& linkfile,
		Вектор<Ткст>& immfile,
		Ткст& linkoptions,
		const Вектор<Ткст>& all_uses,
		const Вектор<Ткст>& all_libraries,
		int optimize) override;
	bool Link(const Вектор<Ткст>& linkfile, const Ткст& linkoptions, bool createmap) override;
	bool Preprocess(
		const Ткст& package,
		const Ткст& file,
		const Ткст& target,
		bool asmout) override;
	void добавьФлаги(Индекс<Ткст>& cfg) override;
	void очистьПакет(const Ткст& package, const Ткст& outdir) override;
	void AfterClean() override;
	
protected:
	void ManageProjectCohesion();
	void DetectAndManageUnusedPackages(const Ткст& nest, const Индекс<Ткст>& packages);
	void DeleteUnusedSourceFiles(
		const Ткст& nest,
		const Вектор<Ткст>& files,
		Ткст exts,
		Ткст excludedFiles = "");
	bool MovePackageFileToAndroidProject(const Ткст& src, const Ткст& dest);
	bool RealizePackageJavaSourcesDirectory(const Ткст& packageName);
	bool RealizeLinkDirectories() const;
	
protected:
	bool FileNeedsUpdate(const Ткст& path, const Ткст& data);
	void обновиФайл(const Ткст& path, const Ткст& data);
	void GenerateApplicationMakeFile();
	void GenerateMakeFile();
	bool GenerateRFile();
	
protected:
	bool ValidateBuilderEnviorement();
	void PutErrorOnConsole(const Ткст& msg);
	
	bool SignApk(const Ткст& target, const Ткст& unsignedApkPath);
	bool GenerateDebugKey(const Ткст& keystorePath);
	
	bool AddSharedLibsToApk(const Ткст& apkPath);
	bool PreprocessJava(const Ткст& package, const Ткст& file, const Ткст& target);
	
protected:
	Ткст GetFilePathInAndroidProject(
		const Ткст& nestDir,
		const Ткст& packageName,
		const Ткст& fileName) const;
	
	Ткст RemoveDirNameFromFileName(Ткст fileName) const;
	Ткст NormalizeModuleName(Ткст moduleName) const;
	
	Ткст GetModuleMakeFilePath(const Ткст& package) const;
		
private:
	void   InitProject();
	Ткст GetSandboxDir() const;
	Ткст GetAndroidProjectDir() const;
	
private:
	Один<Jdk>                    jdk;
	Один<AndroidProject>         project;
	Один<AndroidBuilderCommands> commands;
	
	const РОбласть&            wspc;

private:
	static const Ткст RES_PKG_FLAG;
};

class AndroidBuilderCommands {
public:
	AndroidBuilderCommands(
		AndroidProject* projectPtr, AndroidSDK* sdkPtr, Jdk* jdkPtr);
	
	Ткст PreperCompileJavaSourcesCommand(
		const Вектор<Ткст>& sources);
	
private:
	const AndroidProject* projectPtr;
	const AndroidSDK*     sdkPtr;
	const Jdk*            jdkPtr;
};

class AndroidBuilderUtils final {
public:
	AndroidBuilderUtils() = delete;
	AndroidBuilderUtils(AndroidBuilderUtils&) = delete;
	AndroidBuilderUtils& operator=(AndroidBuilderUtils&) = delete;
	~AndroidBuilderUtils() = delete;
	
	static Ткст GetAssemblyDir(const Ткст& package);
	static Ткст GetAssemblyDir(const Ткст& packageDir, const Ткст& package);
};

class AndroidModuleMakeFileCreator {
public:
	AndroidModuleMakeFileCreator(const Индекс<Ткст>& builderConfig);
	virtual ~AndroidModuleMakeFileCreator() {}
	
	void SetModuleName(const Ткст& имя) { makeFile.SetName(имя); }

	void AddSources(Вектор<Ткст>& sources);
	void AddSources(Индекс<Ткст>& sources);
	void AddInclude(const Ткст& path);
	
	void AddIncludes(const Массив<OptItem>& uses);
	void добавьФлаги(const Массив<OptItem>& flags);
	void AddLdLibraries(const Массив<OptItem>& libraries);
	void AddStaticModuleLibrary(Массив<OptItem>& staticLibraries);
	void AddSharedLibraries(const Массив<OptItem>& uses);
	
	bool   сохрани(const Ткст& path);
	Ткст создай() { return makeFile.вТкст(); }
	
private:
	AndroidModuleMakeFile makeFile;
	const Индекс<Ткст>&  config;
};
	
}

#endif
