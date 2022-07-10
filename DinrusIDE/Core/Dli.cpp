#include "Core.h"

#ifdef PLATFORM_WIN32
typedef HMODULE УКДЛЛ;
#else
typedef void   *УКДЛЛ;
#endif

#define LLOG(x) // RLOG(x)

#ifdef PLATFORM_WIN32

#include <winnt.h>

namespace РНЦПДинрус {

class ПеФайл
{
public:
	ПеФайл(const char *данные = 0) { открой(данные); }

	bool                        открой(const char *данные);

	int                         дайСчётЭкспортов() const;
	const char                 *дайЭкспорт(int индекс) const;
	const char                 *найдиРядЭкспорта(const char *имя, bool case_sensitive = true) const;

	int                         дайИндексСекции(const char *имя) const;

private:
	const char                 *данные;
	const IMAGE_NT_HEADERS     *заголовки;
	const IMAGE_SECTION_HEADER *секции;
	const IMAGE_EXPORT_DIRECTORY *экспорты;
};

bool ПеФайл::открой(const char *_data)
{
	данные = _data;
	if(!данные || данные[0] != 'M' || данные[1] != 'Z')
		return false;
	int pe = Peek32le(данные + 0x3C);
	if(IsBadReadPtr(данные + pe, sizeof(IMAGE_NT_HEADERS)))
		return false;
	if(memcmp(данные + pe, "PE\0\0", 4))
		return false;
	заголовки = (const IMAGE_NT_HEADERS *)(данные + pe);
	секции = (const IMAGE_SECTION_HEADER *)(заголовки + 1);
	экспорты = (const IMAGE_EXPORT_DIRECTORY *)(данные
		+ заголовки->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	return true;
}

int ПеФайл::дайИндексСекции(const char *имя) const
{
	for(int i = 0, n = заголовки->FileHeader.NumberOfSections; i < n; i++)
		if(!strcmp((const char *)секции[i].Name, имя))
			return i;
	return -1;
}

int ПеФайл::дайСчётЭкспортов() const
{
	return экспорты ? экспорты->NumberOfNames : 0;
}

const char *ПеФайл::дайЭкспорт(int индекс) const
{
	if(!экспорты || индекс < 0 || индекс >= (int) экспорты->NumberOfNames)
		return 0;
	return данные + ((const dword *)(данные + (dword)экспорты->AddressOfNames))[индекс];
}

static bool равнаяПам(const char *a, const char *b, int len, bool case_sensitive)
{
	if(case_sensitive)
		return !memcmp(a, b, len);
	else
		return !памсравнИ(a, b, len);
}

const char *ПеФайл::найдиРядЭкспорта(const char *имя, bool case_sensitive) const
{
	if(!экспорты || !имя || !*имя)
		return 0;
	int len = (int)strlen(имя);
	const dword *pnames = (const dword *)(данные + (dword)экспорты->AddressOfNames);
	for(int i = 0; i < (int) экспорты->NumberOfNames; i++) {
		const char *exp = данные + pnames[i];
		int elen = (int)strlen(exp);
		if(elen < len)
			continue;
		if(elen == len && равнаяПам(exp, имя, len, case_sensitive))
			return exp;
		if(равнаяПам(exp, имя, len, case_sensitive) && exp[len] == '@')
			return exp;
		if(exp[0] == '_' && равнаяПам(exp + 1, имя, len, case_sensitive)
			&& (exp[len + 1] == '@' || exp[len + 1] == 0))
			return exp;
	}
	return 0;
}

}

HMODULE проверьДлл__(const char *фн, const char *const *names, РНЦП::Вектор<void *>& plist)
{
	HMODULE hmod = LoadLibrary(фн);

	if(!hmod)
		return 0;

	РНЦП::ПеФайл pe;
	if(!pe.открой((const char *)hmod)) {
		FreeLibrary(hmod);
		return 0;
	}

	int missing = 0;
	for(const char *const *p = names; *p; p++) {
		const char *exp = *p;
		bool optional = (*exp == '?');
		if(optional) exp++;
		const char *имя = pe.найдиРядЭкспорта(exp);
		void *proc = 0;
		if(!имя || !(proc = (void *)GetProcAddress(hmod, имя)))
			if(!optional) {
				if(!missing) {
					LLOG(фн << " отсутстующие экспорты:");
				}
				LLOG(exp);
				missing++;
			}
		plist.добавь(proc);
	}
	if(missing) {
		LLOG(missing << " всего");
		FreeLibrary(hmod);
		return 0;
	}
	return hmod;
}

void освободиДлл__(HMODULE hmod)
{
	FreeLibrary(hmod);
}

#endif

#ifdef PLATFORM_POSIX

#include <dlfcn.h>

void *проверьДлл__(const char *фн, const char *const *names, РНЦП::Вектор<void *>& plist)
{
	void *hmod = dlopen(фн, RTLD_LAZY | RTLD_GLOBAL);
	if(!hmod) {
		RLOG("Ошибка при загрузки библиотеки " << фн << ": " << dlerror());
/*
		for(int i = 0; i < 100; i++) {
			hmod = dlopen(фн + ("." + РНЦП::какТкст(i)), RTLD_LAZY | RTLD_GLOBAL);
			if(hmod)
				break;
		}
		if(!hmod)
*/			return 0;
	}

	int missing = 0;
	for(const char *const *p = names; *p; p++) {
		const char *exp = *p;
		bool optional = (*exp == '?');
		if(optional) exp++;
		void *proc = dlsym(hmod, exp);
		if(!proc && !optional) {
			if(!missing) {
				RLOG(фн << " остутствующие экспорты:");
			}
			RLOG(exp);
		}
		plist.добавь(proc);
	}

	if(missing) {
		RLOG(missing << " всего отсутствующих символов");
		dlclose(hmod);
		return 0;
	}

	return hmod;
}

void освободиДлл__(void *hmod)
{
	if(hmod)
		dlclose(hmod);
}

#endif//PLATFORM_POSIX

УКДЛЛ грузиДлл__(РНЦП::Ткст& inoutfn, const char *const *names, void *const *procs)
{
	const char *фн = inoutfn;
	while(*фн) {
		const char *b = фн;
		while(*фн && *фн != ';'
#ifndef PLATFORM_WIN32
			&& *фн != ':'
#endif
		)
			фн++;
		РНЦП::Ткст libname(b, фн);
		if(*фн)
			фн++;
		РНЦП::Вектор<void *> plist;
		if(УКДЛЛ hmod = проверьДлл__(libname, names, plist)) {
			for(int i = 0; i < plist.дайСчёт(); i++)
				*(void **)*procs++ = plist[i];
			inoutfn = libname;
			return hmod;
		}
	}
	return 0;
}
