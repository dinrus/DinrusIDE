#ifndef _Android_Android_h_
#define _Android_Android_h_

#include <Core/Core.h>
#include <DinrusIDE/Core/Core.h>

#include "Executables.h"
#include "Devices.h"

namespace РНЦП {

class Android {
public:
	static Ткст GetScriptExt();
	static Ткст GetCmdExt();
	
	static void NormalizeVersions(Вектор<Ткст>& versions);
	static void RemoveVersionsNormalization(Вектор<Ткст>& versions);
	
private:
	static Ткст FindVersionsPrefix(const Вектор<Ткст>& versions);
	static Ткст FindLongestVersion(const Вектор<Ткст>& versions);
	
private:
	Android();
	Android(const Android&);
	Android& operator=(const Android&);
	virtual ~Android();
};

class AndroidSDK {
public:
	static Ткст GetDownloadUrl();
	
public:
	AndroidSDK();
	AndroidSDK(const Ткст& path, bool deduceValues = false);
	virtual ~AndroidSDK();
	
	void DeducePathReleatedValues();
	void DeducePlatform();
	void DeduceBuildToolsRelease();
	
	bool Validate() const;
	bool ValidatePlatform() const;
	bool ValidateBuildTools() const;
	
	Вектор<Ткст> FindPlatforms() const;
	Вектор<Ткст> FindBuildToolsReleases() const;
	Ткст         FindDefaultPlatform() const;
	Ткст         FindDefaultBuildToolsRelease() const;
	
	Вектор<AndroidDevice>        FindDevices() const;
	Вектор<AndroidVirtualDevice> FindVirtualDevices() const;
	
public:
	Adb MakeAdb() const { return Adb(AdbPath()); }
	
public:
	Ткст BinDir() const;
	Ткст BuildToolsDir() const;
	Ткст PlatformsDir() const;
	Ткст ConcreteBuildToolsDir() const;
	Ткст ConcretePlatformDir() const;
	Ткст PlatformToolsDir() const;
	Ткст ToolsDir() const;
	
	Ткст AaptPath() const       { return ConcreteBuildToolsDir() + DIR_SEPS + "aapt" + GetExeExt(); }
	Ткст DxPath() const         { return ConcreteBuildToolsDir() + DIR_SEPS + "dx" + Android::GetScriptExt(); }
	Ткст ZipalignPath() const   { return ConcreteBuildToolsDir() + DIR_SEPS + "zipalign" + GetExeExt(); }
	Ткст AndroidJarPath() const { return ConcretePlatformDir() + DIR_SEPS + "android.jar"; }
	Ткст AdbPath() const        { return PlatformToolsDir() + DIR_SEPS + "adb" + GetExeExt(); }
	Ткст AndroidPath() const    { return ToolsDir() + DIR_SEPS + "android" + Android::GetScriptExt(); }
	Ткст EmulatorPath() const   { return ToolsDir() + DIR_SEPS + "emulator" + GetExeExt(); }
	Ткст MonitorPath() const    { return ToolsDir() + DIR_SEPS + "monitor" + Android::GetScriptExt(); }
	
public:
	Ткст GetLauchSDKManagerCmd() const { return NormalizeExePath(AndroidPath()) + " sdk"; }
	Ткст GetLauchAVDManagerCmd() const { return NormalizeExePath(AndroidPath()) + " avd"; }
	
public:
	Ткст дайПуть() const              { return this->path; }
	Ткст GetPlatform() const          { return this->platform; }
	Ткст GetBuildToolsRelease() const { return this->buildToolsRelease; }
	
	void SetPath(const Ткст& path)                           { this->path = path; }
	void SetPlatform(const Ткст& platform)                   { this->platform = platform; }
	void SetBuildToolsRelease(const Ткст& buildToolsRelease) { this->buildToolsRelease = buildToolsRelease; }
	
private:
	Ткст path;
	Ткст platform;
	Ткст buildToolsRelease;
};

class AndroidNDK {
public:
	static Ткст GetDownloadUrl();
	
public:
	AndroidNDK();
	AndroidNDK(const Ткст& path);
	virtual ~AndroidNDK();
	
	bool Validate() const;
	
	Ткст FindDefaultPlatform() const;
	Ткст FindDefaultToolchain() const;
	Ткст FindDefaultCppRuntime() const;
	
	Вектор<Ткст> FindPlatforms() const;
	Вектор<Ткст> FindToolchains() const;
	Вектор<Ткст> FindCppRuntimes() const;
	
public:
	Ткст GetIncludeDir() const;
	Ткст GetCppIncludeDir(const Ткст& cppRuntime) const;

	Ткст GetPlatformsDir() const  { return path + DIR_SEPS + "platforms"; }
	Ткст GetToolchainsDir() const { return path + DIR_SEPS + "toolchains"; }
	Ткст GetSourcesDir() const    { return path + DIR_SEPS + "sources"; }
	
	Ткст GetNdkBuildPath() const  { return path + DIR_SEPS + "ndk-build" + Android::GetCmdExt(); }
	Ткст GetGdbPath() const       { return path + DIR_SEPS + "ndk-gdb"; }
	
public:
	Ткст дайПуть() const { return this->path; }
	
	void SetPath(const Ткст& path) { this->path = path; }
		
private:
	Ткст path;
};

class Apk {
public:
	Apk(const Ткст& path, const AndroidSDK& androidSDK);
	virtual ~Apk();
	
	Ткст FindPackageName() const;
	Ткст FindLaunchableActivity() const;
	
private:
	Ткст FindValueInAndroidManifest(const Ткст& badge, const Ткст& tag) const;
	Ткст FindBadgeTagValue(const Ткст& badge, const Ткст& tag) const;
	
private:
	Ткст path;
	AndroidSDK sdk;
};

class AndroidManifest {
public:
	AndroidManifest();
	virtual ~AndroidManifest();
	
private:
	
};

}

#endif
