#include "Builders.h"
#include "AndroidBuilder.h"
#include "BuilderUtils.h"

#define METHOD_NAME "AndroidBuilder::" << UPP_FUNCTION_NAME << "(): "

const Ткст AndroidBuilder::RES_PKG_FLAG = "ANDROID_RESOURCES_PACKAGE";

Индекс<Ткст> AndroidBuilder::GetBuildersNames()
{
	Индекс<Ткст> builders;
	
	builders.добавь("ANDROID");
	
	return builders;
}

AndroidBuilder::AndroidBuilder()
	: wspc(GetIdeWorkspace())
{
}

void AndroidBuilder::SetJdk(Один<Jdk> jdk)
{
	this->jdk = pick(jdk);
}

Ткст AndroidBuilder::GetTargetExt() const
{
	return ".apk";
}

bool AndroidBuilder::постройПакет(
	const Ткст& package, Вектор<Ткст>& linkfile,
	Вектор<Ткст>& immfile, Ткст& linkoptions,
	const Вектор<Ткст>& all_uses,
	const Вектор<Ткст>& all_libraries,
	int)
{
	Logd() << METHOD_NAME;
	
	InitProject();
	if(!ValidateBuilderEnviorement())
		return false;
	
	const bool isMainPackage = естьФлаг("MAIN");
	const bool isResourcesPackage = естьФлаг(RES_PKG_FLAG);
	const bool isBlitz = естьФлаг("BLITZ") || ndk_blitz;
	Ткст uppManifestPath = PackagePath(package);
	Ткст packageDir = дайПапкуФайла(uppManifestPath);
	Ткст assemblyDir = AndroidBuilderUtils::GetAssemblyDir(packageDir, package);
	
	ChDir(packageDir);
	PutVerbose("cd " + packageDir);
	
	Пакет pkg;
	pkg.грузи(uppManifestPath);
	
	Вектор<Ткст> javaFiles;
	Вектор<Ткст> nativeSources;
	Вектор<Ткст> nativeSourcesOptions;
	Вектор<Ткст> nativeObjects;
	
	Индекс<Ткст> noBlitzNativeSourceFiles;
	
	Ткст androidManifestPath;
	
	Ткст javaSourcesDir    = project->GetJavaDir();
	Ткст jniSourcesDir     = project->GetJniDir();
	Ткст pkgJavaSourcesDir = javaSourcesDir + DIR_SEPS + package;
	for(int i = 0; i < pkg.дайСчёт(); i++) {
		if(!строитсяИср())
			return false;
		if(pkg[i].separator)
			continue;
		
		Ткст globalOptions = Gather(pkg[i].option, config.дайКлючи());
		
		Ткст filePath       = SourcePath(package, pkg[i]);
		Ткст fileName       = NormalizePathSeparator(pkg[i]);
		Ткст packageFile    = приставьИмяф(package, fileName);
		Ткст packageFileDir = дайПапкуФайла(packageFile);
		if(isResourcesPackage) {
			if(packageFileDir.найди(package + DIR_SEPS) != -1)
				packageFileDir.удали(0, Ткст(package + DIR_SEPS).дайСчёт());
			Ткст filePathInAndroidProject
				= GetFilePathInAndroidProject(project->GetResDir(), packageFileDir, fileName);
			
			if(!MovePackageFileToAndroidProject(filePath, filePathInAndroidProject))
				return false;
		}
		else
		if(BuilderUtils::IsJavaFile(filePath)) {
			Ткст filePathInAndroidProject
				= GetFilePathInAndroidProject(javaSourcesDir, packageFileDir, fileName);
			
			if(!RealizePackageJavaSourcesDirectory(package))
				return false;
			if(!MovePackageFileToAndroidProject(filePath, filePathInAndroidProject))
				return false;
			
			javaFiles.добавь(filePathInAndroidProject);
		}
		else
		if(BuilderUtils::IsCppOrCFile(filePath)) {
			nativeSourcesOptions.добавь(globalOptions);
			if(pkg[i].noblitz) {
				if (isBlitz) {
					noBlitzNativeSourceFiles.добавь(filePath);
					continue;
				}
			}
			
			nativeSources.добавь(filePath);
		}
		else
		if(BuilderUtils::IsXmlFile(filePath)) {
			if(isMainPackage && fileName == "AndroidManifest.xml") {
				if(androidManifestPath.дайСчёт()) {
					вКонсоль("AndroidManifest.xml is duplicated.");
					return false;
				}
				
				if(!копируйфл(filePath, project->GetManifestPath()))
					return false;
					
				androidManifestPath = filePath;
			}
		}
		else
		if(BuilderUtils::IsObjectFile(filePath)) {
			Ткст filePathInAndroidProject = GetFilePathInAndroidProject(jniSourcesDir, packageFileDir, fileName);
			
			if(!MovePackageFileToAndroidProject(filePath, filePathInAndroidProject))
				return false;
			
			nativeObjects.добавь(filePathInAndroidProject);
		}
	}
	
	if(isMainPackage && androidManifestPath.пустой()) {
		вКонсоль("Failed to find Android manifest file.");
		return false;
	}
	
	DeleteUnusedSourceFiles(pkgJavaSourcesDir, javaFiles, ".java");
	if(!isResourcesPackage && !javaFiles.пустой()) {
		if(!реализуйДир(project->GetClassesDir()))
			return false;
				
		linkfile.добавь(commands->PreperCompileJavaSourcesCommand(javaFiles));
	}
	
	if(isResourcesPackage || nativeSources.пустой()) {
		Logd() << METHOD_NAME << "There are not native files in the following package " << package << ".";
		return true;
	}
	
	if(isBlitz) {
		Logd() << METHOD_NAME << "Creating blitz step for package " << package << ".";
		
		BlitzBuilderComponent bc(this);
		bc.SetWorkingDir(project->GetJniDir() + DIR_SEPS + package);
		bc.SetBlitzFileName("blitz");
		
		Blitz blitz = bc.MakeBlitzStep(
			nativeSources, nativeSourcesOptions,
		    nativeObjects, immfile, ".o",
		    noBlitzNativeSourceFiles);
		
		if(!файлЕсть(blitz.path)) {
			Loge() << METHOD_NAME << "Blitz was enable, but no blitz file generated.";
		}
		else {
			nativeSources.добавь(package + DIR_SEPS + дайИмяф(blitz.path));
		}
	}
	
	AndroidModuleMakeFileCreator creator(config);
	
	creator.SetModuleName(NormalizeModuleName(package));
	creator.AddSources(nativeSources);
	creator.AddSources(noBlitzNativeSourceFiles);
	creator.AddInclude(assemblyDir);
	creator.AddIncludes(pkg.uses);
	creator.добавьФлаги(pkg.flag);
	creator.AddLdLibraries(pkg.library);
	creator.AddStaticModuleLibrary(pkg.static_library);
	creator.AddSharedLibraries(pkg.uses);

	return creator.сохрани(GetModuleMakeFilePath(package));
}

bool AndroidBuilder::Link(
	const Вектор<Ткст>& linkfile, const Ткст& linkoptions,
	bool createmap)
{
	InitProject();
	if(!ValidateBuilderEnviorement())
		return false;
	
	ManageProjectCohesion();
	
	вКонсоль("Building Android Project");
	ТкстПоток ss;
	if(!GenerateRFile())
		return false;
	if(!RealizeLinkDirectories())
		return false;
	
	// We need to compile java packages in this place, because we need to generate "R.java" file before...
	// We don't know which packages contain resources.
	int time;
	if(linkfile.дайСчёт()) {
		вКонсоль("-----");
		вКонсоль("Compiling java sources...");
		time = msecs();
		for(int i = 0; i < linkfile.дайСчёт(); i++) {
			if(выполни(linkfile[i], ss) != 0) {
				вКонсоль(ss.дайРез());
				return false;
			}
		}
		вКонсоль("Java sources compiled in " + GetPrintTime(time) + ".");
	}
	
	// Now, we are going to start compiling c/c++ sources
	if(дирЕсть(project->GetJniDir())) {
		if(!ndk.Validate()) {
			PutErrorOnConsole("Android NDK was not detected");
			return false;
		}
		
		time = msecs();
		вКонсоль("-----");
		вКонсоль("Compiling native sources...");
		
		GenerateApplicationMakeFile();
		GenerateMakeFile();
		
		NDKBuild ndkBuild(ndk.GetNdkBuildPath());
		if(вербозно_ли()) {
			ndkBuild.EnableVerbose();
		}
		ndkBuild.SetWorkingDir(project->GetDir());
		ndkBuild.SetJobs(IdeGetHydraThreads());
		if(host->выполни(ndkBuild.MakeCmd()) != 0 ) {
			return false;
		}
		вКонсоль("Native sources compiled in " + GetPrintTime(time) + ".");
	}
	
	if(дирЕсть(project->GetClassesDir())) {
		вКонсоль("-----");
		вКонсоль("Creating dex file...");
		Ткст dxCmd;
		dxCmd << NormalizeExePath(sdk.DxPath());
		dxCmd << " --dex ";
		dxCmd << "--output=" << project->GetBinDir() << DIR_SEPS << "classes.dex ";
		dxCmd << project->GetClassesDir();
		if(выполни(dxCmd, ss) != 0) {
			вКонсоль(ss.дайРез());
			return false;
		}
	}
	
	вКонсоль("Creating apk file...");
	Ткст unsignedApkPath = GetSandboxDir() + DIR_SEPS + дайТитулф(target) + ".unsigned.apk";
	DeleteFile(unsignedApkPath);
	Ткст apkCmd;
	apkCmd << NormalizeExePath(sdk.AaptPath());
	apkCmd << " package -v -f";
	if(дирЕсть(project->GetResDir()))
		apkCmd << " -S " << project->GetResDir();
	apkCmd << " -M " << project->GetManifestPath();
	apkCmd << " -I " << NormalizeExePath(sdk.AndroidJarPath());
	apkCmd << " -F " << unsignedApkPath;
	apkCmd << " " << project->GetBinDir();
	// вКонсоль(apkCmd);
	if(выполни(apkCmd, ss) != 0) {
		вКонсоль(ss.дайРез());
		return false;
	}
	
	if(дирЕсть(project->GetLibsDir())) {
		вКонсоль("Adding native libraries to apk...");
		if(!AddSharedLibsToApk(unsignedApkPath))
			return false;
	}
	
	// In release mode we definitly shouldn't signing apk!!!
	if(!SignApk(target, unsignedApkPath))
		return false;
	
	return true;
}

bool AndroidBuilder::Preprocess(
	const Ткст& package,
    const Ткст& file,
    const Ткст& target,
    bool asmout)
{
	InitProject();
	
	Ткст ext = дайРасшф(file);
	if(ext == ".java")
		return PreprocessJava(package, file, target);
	return false;
}

void AndroidBuilder::добавьФлаги(Индекс<Ткст>& cfg)
{
	cfg.добавь("POSIX");
	cfg.добавь("LINUX");
	cfg.добавь("ANDROID");
}

void AndroidBuilder::очистьПакет(const Ткст& package, const Ткст& outdir)
{
	InitProject();
	
	Вектор<Ткст> pkgDirs;
	Вектор<Ткст> pkgFiles;
	if(естьФлаг(RES_PKG_FLAG))
		pkgDirs.добавь(project->GetResDir());
	else {
		// TODO: handle deletetion of (.class)es
		pkgDirs.добавь(project->GetJavaDir(package));
		вКонсоль(project->GetJavaDir(package));
		pkgDirs.добавь(project->GetJniDir(package));
		for(ФайлПоиск ff(приставьИмяф(project->GetObjLocalDir(), "*")); ff; ff.следщ()) {
			if(!ff.скрыт_ли() && !ff.симссылка_ли() && ff.папка_ли()) {
				pkgFiles.добавь(ff.дайПуть() + DIR_SEPS + "lib" + package + ".so");
				pkgDirs.добавь(ff.дайПуть() + DIR_SEPS + "objs" + DIR_SEPS + package);
			}
		}
		for(ФайлПоиск ff(приставьИмяф(project->GetLibsDir(), "*")); ff; ff.следщ())
			if(!ff.скрыт_ли() && !ff.симссылка_ли() && ff.папка_ли())
				pkgFiles.добавь(ff.дайПуть() + DIR_SEPS + "lib" + package + ".so");
	}
	
	for(int i = 0; i < pkgDirs.дайСчёт(); i++) {
		Ткст dir = pkgDirs[i];
		if(дирЕсть(dir))
			DeleteFolderDeep(dir);
	}
	for(int i = 0; i < pkgFiles.дайСчёт(); i++) {
		Ткст file = pkgFiles[i];
		if(файлЕсть(file))
			DeleteFile(file);
	}
}

void AndroidBuilder::AfterClean()
{
	InitProject();
	
	Ткст sandboxDir = GetSandboxDir();
	if(дирЕсть(sandboxDir))
		DeleteFolderDeep(sandboxDir);
}

void AndroidBuilder::ManageProjectCohesion()
{
	Индекс<Ткст> packages;
	for(int i = 0; i < wspc.дайСчёт(); i++)
		packages.добавь(wspc[i]);
	
	DetectAndManageUnusedPackages(project->GetJavaDir(), packages);
	DetectAndManageUnusedPackages(project->GetJniDir(), packages);
}

void AndroidBuilder::DetectAndManageUnusedPackages(
	const Ткст& nest,
    const Индекс<Ткст>& packages)
{
	for(ФайлПоиск ff(приставьИмяф(nest, "*")); ff; ff.следщ()) {
		if(!ff.скрыт_ли() && ff.директория_ли()) {
			Ткст имя = ff.дайИмя();
			if(packages.найди(имя) == -1)
				очистьПакет(имя, "");
		}
	}
}

void AndroidBuilder::DeleteUnusedSourceFiles(
	const Ткст& nest,
    const Вектор<Ткст>& files,
    Ткст exts,
    Ткст excludedFiles)
{
	exts.замени(" ", "");
	excludedFiles.замени(" ", "");
	Индекс<Ткст> extsIdx = Индекс<Ткст>(разбей(exts, ","));
	Индекс<Ткст> excludedFilesIdx = Индекс<Ткст>(разбей(excludedFiles, ","));
	
	Вектор<Ткст> dirs { nest };
	for(int i = 0; i < dirs.дайСчёт(); i++) {
		for(ФайлПоиск ff(приставьИмяф(dirs[i], "*")); ff; ff.следщ()) {
			if(ff.скрыт_ли()) {
				continue;
			}
			
			Ткст path = ff.дайПуть();
			Ткст имя = ff.дайИмя();
			if(ff.папка_ли() && ff.симссылка_ли())
				dirs.добавь(ff.дайПуть());
			else
			if(extsIdx.найди(дайРасшф(path)) == -1)
				continue;
			else
			if(excludedFilesIdx.найди(имя) > -1)
				continue;
			else {
				bool exists = false;
				for(int j = 0; j < files.дайСчёт(); j++) {
					if(files[j] == path) {
						exists = true;
						break;
					}
				}
				if(!exists)
					DeleteFile(path);
			}
		}
	}
}

bool AndroidBuilder::MovePackageFileToAndroidProject(const Ткст& src, const Ткст& dst)
{
	Ткст directory = дайДиректориюФайла(dst);
	if(!реализуйДир(directory)) {
		Loge() << METHOD_NAME << "Cannot relize following directory: \"" << directory << "\".";
		return false;
	}
	
	if(файлЕсть(dst)) {
		if(дайФВремя(dst) > дайФВремя(src))
			return true;
	}
	
	// TODO: Generic host should return bool flag.
	return ::сохраниФайл(dst, загрузиФайл(src));
}

bool AndroidBuilder::RealizePackageJavaSourcesDirectory(const Ткст& packageName)
{
	Ткст dir = project->GetJavaDir() + DIR_SEPS + packageName;
	
	return дирЕсть(dir) || реализуйДир(dir);
}

bool AndroidBuilder::RealizeLinkDirectories() const
{
	if(!реализуйДир(project->GetBinDir()))
		return false;
	
	return true;
}

bool AndroidBuilder::SignApk(const Ткст& target, const Ткст& unsignedApkPath)
{
	ТкстПоток ss;
	
	Ткст signedApkPath = GetSandboxDir() + DIR_SEPS + дайТитулф(target) + ".signed.apk";
	if(естьФлаг("DEBUG")) {
		Ткст keystorePath = GetSandboxDir() + DIR_SEPS + "debug.keystore";
		if(!GenerateDebugKey(keystorePath))
			return false;
	
		вКонсоль("Signing apk file...");
		DeleteFile(signedApkPath);
		Ткст jarsignerCmd;
		jarsignerCmd << NormalizeExePath(jdk->GetJarsignerPath());
		
		// Up to Java 6.0 below alogirms was by default
		// (In Java 7.0 and above we need to manually specific this algorithms)
		jarsignerCmd << " -sigalg SHA1withRSA";
		jarsignerCmd << " -digestalg SHA1";
		
		jarsignerCmd << " -keystore " + keystorePath;
		jarsignerCmd << " -storepass android";
		jarsignerCmd << " -keypass android";
		// TODO: not sure about below line. But I think for debug purpose we shouldn't use tsa.
		// http://en.wikipedia.org/wiki/Trusted_timestamping
		//jarsignerCmd << " -tsa https://timestamp.geotrust.com/tsa";
		jarsignerCmd << " -signedjar " << signedApkPath;
		jarsignerCmd << " " << unsignedApkPath;
		jarsignerCmd << " androiddebugkey";
		//вКонсоль(jarsignerCmd);
		if(выполни(jarsignerCmd, ss) != 0) {
			вКонсоль(ss.дайРез());
			return false;
		}
		
		вКонсоль("Aliging apk file...");
		DeleteFile(target);
		Ткст zipalignCmd;
		zipalignCmd << NormalizeExePath(sdk.ZipalignPath());
		zipalignCmd << " -f 4 ";
		zipalignCmd << (естьФлаг("DEBUG") ? signedApkPath : unsignedApkPath) << " ";
		zipalignCmd << target;
		//вКонсоль(zipalignCmd);
		if(выполни(zipalignCmd, ss) != 0) {
			вКонсоль(ss.дайРез());
			return false;
		}
	}
	
	return true;
}

bool AndroidBuilder::GenerateDebugKey(const Ткст& keystorePath)
{
	ТкстПоток ss;
	
	if(!файлЕсть(keystorePath)) {
		вКонсоль("Generating debug ключ...");
		
		Ткст keytoolCmd;
		keytoolCmd << NormalizeExePath(jdk->GetKeytoolPath());
		keytoolCmd << " -genkeypair -alias androiddebugkey -keypass android";
		keytoolCmd << " -keystore " << keystorePath;
		keytoolCmd << " -storepass android -dname \"CN=Android Debug,O=Android,C=US\"";
		keytoolCmd << " -keyalg RSA";
		keytoolCmd << " -validity 100000";
		//вКонсоль(keytoolCmd);
		if(выполни(keytoolCmd, ss) != 0) {
			вКонсоль(ss.дайРез());
			return false;
		}
	}
	return true;
}

bool AndroidBuilder::AddSharedLibsToApk(const Ткст& apkPath)
{
	// TODO: A little bit workearound (I know one thing that shared libs should be in "lib" directory not in "libs")
	// So, we need to create temporary lib directory with .so files :(
	const Ткст libDir = project->GetDir() + DIR_SEPS + "lib";
	
	Вектор<Ткст> sharedLibsToAdd;
	for(ФайлПоиск ff(приставьИмяф(project->GetLibsDir(), "*")); ff; ff.следщ()) {
		if (!ff.скрыт_ли () && !ff.симссылка_ли () && ff.директория_ли()) {
			for(ФайлПоиск ffa(приставьИмяф (ff.дайПуть(), "*")); ffa; ffa.следщ ()) {
				if(!ffa.скрыт_ли() && !ffa.симссылка_ли() && !ffa.директория_ли()) {
					// TODO: in libs directory we can find another java libs (.jar)
					Ткст fileExt = впроп(дайРасшф(ffa.дайПуть()));
					if(fileExt == ".so") {
						const Ткст libPath = Ткст("lib") + DIR_SEPS + ff.дайИмя() + DIR_SEPS + ffa.дайИмя();
						const Ткст destPath = project->GetDir() + DIR_SEPS + libPath;
						if(!RealizePath(destPath) || !копируйфл(ffa.дайПуть(), destPath))
							return false;
						sharedLibsToAdd.добавь(libPath);
					}
				}
			}
		}
	}
	
	ChDir(project->GetDir());
	Ткст aaptAddCmd;
	aaptAddCmd << NormalizeExePath(sdk.AaptPath());
	aaptAddCmd << " add " << apkPath;
	for(int i = 0; i < sharedLibsToAdd.дайСчёт(); i++)
		aaptAddCmd << " " << sharedLibsToAdd[i];
	// вКонсоль(aaptAddCmd);
	ТкстПоток ss;
	if(выполни(aaptAddCmd, ss) != 0) {
		вКонсоль(ss.дайРез());
		return false;
	}
	if(!DeleteFolderDeep(libDir))
		return false;
	
	return true;
}

bool AndroidBuilder::ValidateBuilderEnviorement()
{
	if(!sdk.Validate()) {
		PutErrorOnConsole("Android SDK was not detected");
		return false;
	}
	if(!sdk.ValidateBuildTools()) {
		PutErrorOnConsole("Android SDK build tools was not detected");
		return false;
	}
	if(!sdk.ValidatePlatform()) {
		PutErrorOnConsole("Android SDK platform was not detected");
		return false;
	}
	if(!jdk->Validate()) {
		PutErrorOnConsole("JDK was not detected");
		return false;
	}
	
	return true;
}

void AndroidBuilder::PutErrorOnConsole(const Ткст& msg)
{
	вКонсоль("Ошибка: " + msg + ".");
}

bool AndroidBuilder::FileNeedsUpdate(const Ткст& path, const Ткст& data)
{
	return !(файлЕсть(path) && загрузиФайл(path).сравни(data) == 0);
}

void AndroidBuilder::обновиФайл(const Ткст& path, const Ткст& data)
{
	if(!FileNeedsUpdate(path, data))
		return;
	
	сохраниФайл(path, data);
}

void AndroidBuilder::GenerateApplicationMakeFile()
{
	AndroidApplicationMakeFile makeFile;
	makeFile.SetPlatform(sdk.GetPlatform());
	makeFile.SetArchitectures(ndkArchitectures);
	makeFile.SetCppRuntime(ndkCppRuntime);
	makeFile.SetCppFlags(ndkCppFlags);
	makeFile.SetCFlags(ndkCFlags);
	makeFile.SetOptim(естьФлаг("DEBUG") ? "debug" : "release");
	makeFile.SetToolchain(ndkToolchain);
	
	PutVerbose("Architectures: " + какТкст(ndkArchitectures));
	PutVerbose("CppRuntime: " + ndkCppRuntime);
	PutVerbose("CppFlags: " + ndkCppFlags);
	PutVerbose("CFlags: " + ndkCFlags);
	PutVerbose("Toolchain: " + ndkToolchain);
	
	обновиФайл(project->GetJniApplicationMakeFilePath(), makeFile.вТкст());
}

void AndroidBuilder::GenerateMakeFile()
{
	const Ткст makeFileName = "Android.mk";
	const Ткст baseDir = project->GetJniDir();
	Вектор<Ткст> modules;
	
	БиВектор<Ткст> dirs;
	dirs.добавьГолову(baseDir);
	while(!dirs.пустой()) {
		Ткст currentDir = dirs.дайГолову();
		for(ФайлПоиск ff1(приставьИмяф(currentDir, "*")); ff1; ff1.следщ()) {
			if(!ff1.скрыт_ли() && !ff1.симссылка_ли() && ff1.папка_ли()) {
				bool isEmpty = true;
				for(ФайлПоиск ff2(приставьИмяф(ff1.дайПуть(), "*")); ff2; ff2.следщ()) {
					if(!ff2.скрыт_ли() && !ff2.симссылка_ли() && ff2.файл_ли()) {
						isEmpty = false;
						break;
					}
				}
				
				if(!isEmpty) {
					Ткст moduleName = ff1.дайПуть();
					moduleName.удали(moduleName.найди(baseDir), baseDir.дайСчёт() + 1);
					modules.добавь(moduleName);
				}
				else {
					dirs.добавьХвост(ff1.дайПуть());
				}
			}
		}
		dirs.сбросьГолову();
	}
	
	AndroidMakeFile makeFile;
	makeFile.AddHeader();
	for(const Ткст& module : modules)
		makeFile.AddInclusion(module + DIR_SEPS + makeFileName);
	
	обновиФайл(project->GetJniMakeFilePath(), makeFile.вТкст());
}

bool AndroidBuilder::GenerateRFile()
{
	// TODO: gen in gen folder
	if(дирЕсть(project->GetResDir())) {
		ТкстПоток ss;
		Ткст aaptCmd;
		aaptCmd << NormalizeExePath(sdk.AaptPath());
		aaptCmd << " package -v -f -m";
		aaptCmd << " -S " << project->GetResDir();
		aaptCmd << " -J " << project->GetJavaDir();
		aaptCmd << " -M " << project->GetManifestPath();
		aaptCmd << " -I " << NormalizeExePath(sdk.AndroidJarPath());
		
		if(выполни(aaptCmd, ss) != 0) {
			вКонсоль(ss.дайРез());
			return false;
		}
	}
	
	return true;
}

bool AndroidBuilder::PreprocessJava(const Ткст& package, const Ткст& file, const Ткст& target)
{
	ТкстПоток ss;
	Ткст ext = дайРасшф(file);
	if(ext != ".java")
		return false;
	Ткст fileName = дайИмяф(file);
	fileName.замени(ext, "");
	Ткст targetDir = дайДиректориюФайла(target);
	Ткст classesDir = targetDir + "classes";
	Ткст classFileName = fileName + ".class";
	
	if(дирЕсть(classesDir))
		DeleteFolderDeep(classesDir);
	реализуйДир(classesDir);
	
	Ткст compileCmd;
	compileCmd << NormalizeExePath(jdk->GetJavacPath());
	compileCmd << " -d "<< classesDir;
	compileCmd << " -classpath ";
	compileCmd << NormalizeExePath(sdk.AndroidJarPath()) << Java::GetDelimiter();
	compileCmd << project->GetBuildDir();
	compileCmd << " -sourcepath " << project->GetJavaDir();
	compileCmd << " " << file;
	if(выполни(compileCmd) != 0)
		return false;
	
	Ткст classFileDir;
	БиВектор<Ткст> paths;
	paths.добавьХвост(classesDir);
	while(!paths.пустой()) {
		for(ФайлПоиск ff(приставьИмяф(paths.дайГолову(), "*")); ff; ff.следщ()) {
			if(!ff.скрыт_ли()) {
				if(!ff.симссылка_ли() && ff.директория_ли())
					paths.добавьХвост(ff.дайПуть());
				else
				if(ff.дайИмя().сравни(classFileName) == 0) {
					classFileDir = дайДиректориюФайла(ff.дайПуть());
					paths.очисть();
					break;
				}
			}
		}
		if(!paths.пустой())
			paths.сбросьГолову();
	}
	
	if(classFileDir.пустой())
		return false;
	
	Ткст relativeClassFileDir = classFileDir;
	relativeClassFileDir.замени(classesDir + DIR_SEPS, "");
	Ткст className = relativeClassFileDir + fileName;
	className.замени(DIR_SEPS, ".");
	
	Ткст javahCmd;
	javahCmd << NormalizeExePath(jdk->GetJavahPath());
	javahCmd << " -classpath ";
	javahCmd << classesDir << Java::GetDelimiter();
	javahCmd << NormalizeExePath(sdk.AndroidJarPath()) << Java::GetDelimiter();
	javahCmd << project->GetBuildDir();
	javahCmd << " -o " << target;
	javahCmd << " " << className;
	// вКонсоль(javahCmd);
	if(выполни(javahCmd, ss) != 0) {
		вКонсоль(ss.дайРез());
		return false;
	}
	
	return true;
}

// -------------------------------------------------------------------

void AndroidBuilder::InitProject()
{
	if(!project) {
		project.создай<AndroidProject>(GetAndroidProjectDir(), естьФлаг("DEBUG"));
		commands.создай<AndroidBuilderCommands>(project.дай(), &sdk, jdk.дай());
	}
}

Ткст AndroidBuilder::GetSandboxDir() const
{
	if(target.пустой())
		return Ткст();
	
	int targetExtLen = GetTargetExt().дайСчёт();
	Ткст mainPackageName = дайИмяф(target);
	mainPackageName.удали(mainPackageName.дайСчёт() - targetExtLen, targetExtLen);
	return дайПапкуФайла(target) + DIR_SEPS + "Sandbox" + DIR_SEPS + mainPackageName;
}

Ткст AndroidBuilder::GetAndroidProjectDir() const
{
	return GetSandboxDir() + DIR_SEPS + "AndroidProject";
}

// -------------------------------------------------------------------

Ткст AndroidBuilder::GetFilePathInAndroidProject(
	const Ткст& nestDir,
	const Ткст& packageName,
	const Ткст& fileName) const
{
	return nestDir + DIR_SEPS + packageName + DIR_SEPS + RemoveDirNameFromFileName(fileName);
}

Ткст AndroidBuilder::RemoveDirNameFromFileName(Ткст fileName) const
{
	int idx = fileName.найдирек(DIR_SEPS);
	if(idx >= 0)
		fileName.удали(0, idx);
	return fileName;
}

Ткст AndroidBuilder::NormalizeModuleName(Ткст moduleName) const
{
	moduleName.замени(DIR_SEPS, "_");
	return moduleName;
}

Ткст AndroidBuilder::GetModuleMakeFilePath(const Ткст& package) const
{
	return project->GetJniDir() + DIR_SEPS + package + DIR_SEPS + "Android.mk";
}

// -------------------------------------------------------------------

Построитель *CreateAndroidBuilder()
{
	return new AndroidBuilder();
}

ИНИЦИАЛИЗАТОР(AndroidBuilder)
{
	Индекс<Ткст> builders = AndroidBuilder::GetBuildersNames();
	for(int i = 0; i < builders.дайСчёт(); i++)
		RegisterBuilder(builders[i], &CreateAndroidBuilder);
}
