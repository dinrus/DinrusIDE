#include "Android.h"

#include <plugin/pcre/Pcre.h>

namespace РНЦП {

Ткст AndroidSDK::GetDownloadUrl()
{
	return "https://developer.android.com/studio/Индекс.html";
}

AndroidSDK::AndroidSDK()
{
}

AndroidSDK::AndroidSDK(const Ткст& path, bool deduceValues)
{
	this->path = path;
	if(deduceValues)
		DeducePathReleatedValues();
}

AndroidSDK::~AndroidSDK()
{
}

void AndroidSDK::DeducePathReleatedValues()
{
	DeducePlatform();
	DeduceBuildToolsRelease();
}

void AndroidSDK::DeducePlatform()
{
	platform = FindDefaultPlatform();
}

void AndroidSDK::DeduceBuildToolsRelease()
{
	buildToolsRelease = FindDefaultBuildToolsRelease();
}

bool AndroidSDK::Validate() const
{
	if(!дирЕсть(path) || !файлЕсть(AndroidPath())) return false;
	
	return true;
}

bool AndroidSDK::ValidatePlatform() const
{
	return дирЕсть(ConcretePlatformDir());
}

bool AndroidSDK::ValidateBuildTools() const
{
	return дирЕсть(ConcreteBuildToolsDir());
}

Вектор<Ткст> AndroidSDK::FindPlatforms() const
{
	Вектор<Ткст> platforms;
	
	for(ФайлПоиск ff(приставьИмяф(PlatformsDir(), "*")); ff; ff.следщ()) {
		if(!ff.скрыт_ли() && ff.папка_ли())
			platforms.добавь(ff.дайИмя());
	}
	
	return platforms;
}

Вектор<Ткст> AndroidSDK::FindBuildToolsReleases() const
{
	Вектор<Ткст> buildTools;
	
	for(ФайлПоиск ff(приставьИмяф(BuildToolsDir(), "*")); ff; ff.следщ()) {
		if(!ff.скрыт_ли() && ff.папка_ли())
			buildTools.добавь(ff.дайИмя());
	}
	
	return buildTools;
}

Вектор<AndroidDevice> AndroidSDK::FindDevices() const
{
	Вектор<AndroidDevice> devices;
	
	Ткст out;
	if(Sys(NormalizeExePath(AdbPath()) + " devices -l", out) == 0) {
		Вектор<Ткст> lines;
		Вектор<Ткст> outputLines = разбей(out, "\n");
		for(int i = 0; i < outputLines.дайСчёт(); i++) {
			if(!outputLines[i].начинаетсяС("*") && !outputLines[i].начинаетсяС(" "))
				lines.добавь(outputLines[i]);
		}
		
		for(int i = 1; i < lines.дайСчёт(); i++) {
			AndroidDevice device;
			
			Вектор<Ткст> elements = разбей(lines[i], " ");
			for(int j = 0; j < elements.дайСчёт(); j++) {
				if(j == 0)
					device.SetSerial(elements[j]);
				else if(j == 1)
					device.SetState(elements[j]);
				else {
					Вектор<Ткст> element = разбей(elements[j], ":");
					if(element.дайСчёт() == 2) {
						Ткст tag  = element[0];
						Ткст data = element[1];
						if(tag == "usb")
							device.SetUsb(data);
						else
						if(tag == "model")
							device.SetModel(data);
					}
				}
			}
			devices.добавь(device);
		}
	}
	
	return devices;
}

Вектор<AndroidVirtualDevice> AndroidSDK::FindVirtualDevices() const
{
	Вектор<AndroidVirtualDevice> avdes;
	
	Ткст out;
	if(Sys(NormalizeExePath(AndroidPath()) + " list avd", out) == 0) {
		Вектор<Ткст> lines = разбей(out, "\n");
		
		AndroidVirtualDevice avd;
		for(int i = 0; i < lines.дайСчёт(); i++) {
			Вектор<Ткст> line = разбей(lines[i], ":");
			if(line.дайСчёт() == 2) {
				Ткст tag  = line[0];
				Ткст data = line[1];
				if(data.начинаетсяС(" "))
					data.удали(0);
				if(tag.найди("Имя") > -1) {
					if(!avd.дайИмя().пустой() && avd.дайИмя() != data)
						avdes.добавь(avd);
					avd.SetName(data);
				}
				else
				if(tag.найди("Device") > -1)
					avd.SetDeviceType(data);
				else
				if(tag.найди("Path") > -1)
					avd.SetPath(data);
				else
				if(tag.найди("Target") > -1)
					avd.SetTarget(data);
				else
				if(tag.найди("Тэг/ABI") > -1)
					avd.SetAbi(data);
				
				// TODO: implement all possible tags
			}
		}
		
		if(!avd.дайИмя().пустой())
			avdes.добавь(avd);
	}
	
	return avdes;
}

Ткст AndroidSDK::FindDefaultPlatform() const
{
	Вектор<Ткст> platforms = FindPlatforms();
	if(platforms.дайСчёт()) {
		Android::NormalizeVersions(platforms);
		сортируй(platforms, StdGreater<Ткст>());
		Android::RemoveVersionsNormalization(platforms);
		
		int idx = 0;
		for(int i = 0; i < platforms.дайСчёт(); i++) {
			if(RegExp("^android-[0-9]*$").сверь(platforms[i])) {
				idx = i;
				break;
			}
		}
		return platforms[idx];
	}
	return "";
}

Ткст AndroidSDK::FindDefaultBuildToolsRelease() const
{
	Вектор<Ткст> releases = FindBuildToolsReleases();
	if(releases.дайСчёт()) {
		сортируй(releases, StdGreater<Ткст>());
		int idx = 0;
		for(int i = 0; i < releases.дайСчёт(); i++) {
			if(RegExp("^[1-9][0-9.]*$").сверь(releases[i])) {
				idx = i;
				break;
			}
		}
		return releases[idx];
	}
	return "";
}

// ---------------------------------------------------------------

Ткст AndroidSDK::BinDir() const
{
	return path + DIR_SEPS + "bin";
}

Ткст AndroidSDK::BuildToolsDir() const
{
	return path + DIR_SEPS + "build-tools";
}

Ткст AndroidSDK::PlatformsDir() const
{
	return path + DIR_SEPS + "platforms";
}

Ткст AndroidSDK::ConcreteBuildToolsDir() const
{
	return BuildToolsDir() + DIR_SEPS + buildToolsRelease;
}

Ткст AndroidSDK::ConcretePlatformDir() const
{
	return PlatformsDir() + DIR_SEPS + platform;
}

Ткст AndroidSDK::PlatformToolsDir() const
{
	return path + DIR_SEPS + "platform-tools";
}

Ткст AndroidSDK::ToolsDir() const
{
	return path + DIR_SEPS + "tools";
}

// ---------------------------------------------------------------

}
