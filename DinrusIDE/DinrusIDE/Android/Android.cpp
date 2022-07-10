#include "Android.h"

namespace РНЦП {

Ткст Android::GetScriptExt()
{
#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64)
	return ".bat";
#else
	return "";
#endif
}

Ткст Android::GetCmdExt()
{
#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64)
	return ".cmd";
#else
	return "";
#endif
}

void Android::NormalizeVersions(Вектор<Ткст>& versions)
{
	Ткст prefix = FindVersionsPrefix(versions);
	Ткст longestVersion = FindLongestVersion(versions);
	
	int longestVersionCount = longestVersion.дайСчёт();
	for(int i = 0; i < versions.дайСчёт(); i++) {
		int diff = longestVersionCount - versions[i].дайСчёт();
		if(diff <= 0)
			continue;
			
		Ткст version = versions[i];
		if(!prefix.пустой())
			version.замени(prefix, "");
		for(int j = 0; j < diff; j++)
			version = "0" + version;
		versions[i] = prefix + version;
	}
}

void Android::RemoveVersionsNormalization(Вектор<Ткст>& versions)
{
	Ткст prefix = FindVersionsPrefix(versions);
	for(int i = 0; i < versions.дайСчёт(); i++) {
		Ткст version = versions[i];
		if(!prefix.пустой())
			version.замени(prefix, "");
		for(;;) {
			if(!version.начинаетсяС("0"))
				break;
			version.удали(0);
		}
		versions[i] = prefix + version;
	}
}

Ткст Android::FindVersionsPrefix(const Вектор<Ткст>& versions)
{
	Ткст prefix;
	for(int i = 0; i < versions.дайСчёт(); i++) {
		Ткст currentPrefix;
		Ткст version = versions[i];
		for(int j = 0; j < 10; j++) {
			int idx = version.найди(целТкт(j));
			if(idx < 0)
				continue;
			Ткст left = version.лево(idx);
			if(currentPrefix.пустой() || left.дайСчёт() < currentPrefix.дайСчёт())
				currentPrefix = left;
		}
		if(i == 0)
			prefix = currentPrefix;
		else
		if(i > 0 && currentPrefix != prefix)
			return "";
	}
	
	return prefix;
}

Ткст Android::FindLongestVersion(const Вектор<Ткст>& versions)
{
	Ткст longest;
	for(int i = 0; i < versions.дайСчёт(); i++) {
		Ткст current = versions[i];
		if(current.дайСчёт() > longest.дайСчёт())
			longest = current;
	}
	return longest;
}

}
