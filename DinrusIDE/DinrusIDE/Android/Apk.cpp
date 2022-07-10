#include "Android.h"

namespace РНЦП {

Apk::Apk(const Ткст& path, const AndroidSDK& androidSDK)
{
	this->path = path;
	this->sdk = androidSDK;
}

Apk::~Apk()
{
	
}

Ткст Apk::FindPackageName() const
{
	return FindValueInAndroidManifest("package", "имя");
}

Ткст Apk::FindLaunchableActivity() const
{
	return FindValueInAndroidManifest("launchable-activity", "имя");
}

Ткст Apk::FindValueInAndroidManifest(const Ткст& badge, const Ткст& tag) const
{
	Ткст out;
	if(Sys(sdk.AaptPath() + " dump badging " + path + " AndroidManifest.xml", out) == 0) {
		Вектор<Ткст> lines = разбей(out, '\n');
		for(int i = 0; i < lines.дайСчёт(); i++) {
			if(lines[i].найди(badge) >= 0)
				return FindBadgeTagValue(lines[i], tag);
		}
	}
	
	return Ткст();
}

Ткст Apk::FindBadgeTagValue(const Ткст& badge, const Ткст& tag) const
{
	Ткст tagValue;
	Ткст tagBeginning = tag + "='";
	int pos = badge.найди(tagBeginning);
	if(pos >= 0) {
		for(int i = pos + tagBeginning.дайСчёт(); i < badge.дайСчёт(); i++) {
			if(badge[i] == '\'')
				break;
			tagValue += badge[i];
		}
	}
	
	return tagValue;
}

}
