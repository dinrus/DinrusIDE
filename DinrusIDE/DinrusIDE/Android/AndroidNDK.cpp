#include "Android.h"

namespace РНЦП {

static const Ткст TOOLCHAIN_CLANG = "clang";

Ткст AndroidNDK::GetDownloadUrl()
{
	return "https://developer.android.com/ndk/downloads/Индекс.html";
}

AndroidNDK::AndroidNDK()
{
}

AndroidNDK::AndroidNDK(const Ткст& path)
{
	this->path = path;
}

AndroidNDK::~AndroidNDK()
{
}

bool AndroidNDK::Validate() const
{
	if(!дирЕсть(path) || !файлЕсть(GetNdkBuildPath()))
		return false;
	
	return true;
}

Ткст AndroidNDK::FindDefaultPlatform() const
{
	Вектор<Ткст> platforms = FindPlatforms();
	
	Android::NormalizeVersions(platforms);
	сортируй(platforms, StdGreater<Ткст>());
	Android::RemoveVersionsNormalization(platforms);
	
	return !platforms.пустой() ? platforms[0] : "";
}

Ткст AndroidNDK::FindDefaultToolchain() const
{
	Индекс<Ткст> toolchains(FindToolchains());
	
	int clangIdx = toolchains.найди(TOOLCHAIN_CLANG);
	if (clangIdx >= 0) {
		return toolchains[clangIdx];
	}
	
	SortIndex(toolchains, StdGreater<Ткст>());
	return !toolchains.пустой() ? toolchains[toolchains.дайСчёт()] : "";
}

Ткст AndroidNDK::FindDefaultCppRuntime() const
{
	return "c++_shared";
}

Вектор<Ткст> AndroidNDK::FindPlatforms() const
{
	Вектор<Ткст> platforms;
	
	for(ФайлПоиск ff(приставьИмяф(GetPlatformsDir(), "*")); ff; ff.следщ()) {
		if(!ff.скрыт_ли() && ff.папка_ли())
			platforms.добавь(ff.дайИмя());
	}
	
	return platforms;
}

Вектор<Ткст> AndroidNDK::FindToolchains() const
{
	Вектор<Ткст> toolchains;
	
	for(ФайлПоиск ff(приставьИмяф(GetToolchainsDir(), "*")); ff; ff.следщ()) {
		if(!ff.скрыт_ли() && ff.папка_ли()) {
			Ткст имя = ff.дайИмя();
			if(имя.начинаетсяС("llvm")) {
				toolchains.добавь(TOOLCHAIN_CLANG);
				continue;
			}
			
			Вектор<Ткст> nameParts = разбей(имя, "-");
			if(nameParts.дайСчёт() > 1) {
				Ткст toolchain = nameParts[nameParts.дайСчёт() - 1];
				if(найдиИндекс(toolchains, toolchain) == -1)
					toolchains.добавь(toolchain);
			}
		}
	}

	return toolchains;
}

Вектор<Ткст> AndroidNDK::FindCppRuntimes() const
{
	Вектор<Ткст> runtimes;
	
	// Values from NDK documentation
	runtimes.добавь("c++_static");
	runtimes.добавь("c++_shared");
	runtimes.добавь("system");
	runtimes.добавь("none");
	
	return runtimes;
}

Ткст AndroidNDK::GetIncludeDir() const
{
	Ткст dir;
	dir << GetPlatformsDir() << DIR_SEPS << FindDefaultPlatform() << DIR_SEPS;
	// TODO: decide how to implement architecture selection.
	dir << "arch-arm" << DIR_SEPS;
	dir << "usr" << DIR_SEPS << "include";
	
	return дирЕсть(dir) ? dir : "";
}

Ткст AndroidNDK::GetCppIncludeDir(const Ткст& cppRuntime) const
{
	Ткст nest = GetSourcesDir() + DIR_SEPS + "cxx-stl" + DIR_SEPS;
	if(cppRuntime == "system") {
		return nest + "system" + DIR_SEPS + "include";
	}
	else
	if(cppRuntime.начинаетсяС("c++")) {
		return nest + "llvm-libc++" + DIR_SEPS + "include";
	}
	
	return "";
}

}
