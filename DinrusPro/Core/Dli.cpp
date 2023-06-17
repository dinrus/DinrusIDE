#include <DinrusPro/DinrusCore.h>

#ifdef PLATFORM_WIN32
typedef HMODULE УКДЛЛ;
#else
typedef проц   *УКДЛЛ;
#endif

#define LLOG(x) // RLOG(x)

#ifdef PLATFORM_WIN32

#include <winnt.h>

class ПеФайл
{
public:
	ПеФайл(кткст0 данные = 0) { открой(данные); }

	бул                        открой(кткст0 данные);

	цел                         дайСчётЭкспортов() const;
	const сим                 *дайЭкспорт(цел индекс) const;
	const сим                 *найдиРядЭкспорта(кткст0 имя, бул case_sensitive = true) const;

	цел                         дайИндексСекции(кткст0 имя) const;

private:
	const сим                 *данные;
	const IMAGE_NT_HEADERS     *заголовки;
	const IMAGE_SECTION_HEADER *секции;
	const IMAGE_EXPORT_DIRECTORY *экспорты;
};

бул ПеФайл::открой(кткст0 _data)
{
	данные = _data;
	if(!данные || данные[0] != 'M' || данные[1] != 'Z')
		return false;
	цел pe = подбери32лэ(данные + 0x3C);
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

цел ПеФайл::дайИндексСекции(кткст0 имя) const
{
	for(цел i = 0, n = заголовки->FileHeader.NumberOfSections; i < n; i++)
		if(!strcmp((кткст0 )секции[i].Name, имя))
			return i;
	return -1;
}

цел ПеФайл::дайСчётЭкспортов() const
{
	return экспорты ? экспорты->NumberOfNames : 0;
}

кткст0 ПеФайл::дайЭкспорт(цел индекс) const
{
	if(!экспорты || индекс < 0 || индекс >= (цел) экспорты->NumberOfNames)
		return 0;
	return данные + ((const бцел *)(данные + (бцел)экспорты->AddressOfNames))[индекс];
}

static бул равнаяПам(кткст0 a, кткст0 b, цел len, бул case_sensitive)
{
	if(case_sensitive)
		return !memcmp(a, b, len);
	else
		return !памсравнИ(a, b, len);
}

кткст0 ПеФайл::найдиРядЭкспорта(кткст0 имя, бул case_sensitive) const
{
	if(!экспорты || !имя || !*имя)
		return 0;
	цел len = (цел)strlen(имя);
	const бцел *pnames = (const бцел *)(данные + (бцел)экспорты->AddressOfNames);
	for(цел i = 0; i < (цел) экспорты->NumberOfNames; i++) {
		кткст0 exp = данные + pnames[i];
		цел elen = (цел)strlen(exp);
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

HMODULE проверьДлл__(кткст0 фн, кткст0 const *names, Вектор<проц *>& plist)
{
	HMODULE hmod = LoadLibrary(фн);

	if(!hmod)
		return 0;

	ПеФайл pe;
	if(!pe.открой((кткст0 )hmod)) {
		FreeLibrary(hmod);
		return 0;
	}

	цел missing = 0;
	for(кткст0 const *p = names; *p; p++) {
		кткст0 exp = *p;
		бул optional = (*exp == '?');
		if(optional) exp++;
		кткст0 имя = pe.найдиРядЭкспорта(exp);
		ук proc = 0;
		if(!имя || !(proc = (проц *)GetProcAddress(hmod, имя)))
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

проц освободиДлл__(HMODULE hmod)
{
	FreeLibrary(hmod);
}

#endif

#ifdef PLATFORM_POSIX

#include <dlfcn.h>

ук проверьДлл__(кткст0 фн, кткст0 const *names, Вектор<проц *>& plist)
{
	ук hmod = dlopen(фн, RTLD_LAZY | RTLD_GLOBAL);
	if(!hmod) {
		RLOG("Ошибка при загрузки библиотеки " << фн << ": " << dlerror());
/*
		for(цел i = 0; i < 100; i++) {
			hmod = dlopen(фн + ("." + какТкст(i)), RTLD_LAZY | RTLD_GLOBAL);
			if(hmod)
				break;
		}
		if(!hmod)
*/			return 0;
	}

	цел missing = 0;
	for(кткст0 const *p = names; *p; p++) {
		кткст0 exp = *p;
		бул optional = (*exp == '?');
		if(optional) exp++;
		ук proc = dlsym(hmod, exp);
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

проц освободиДлл__(ук hmod)
{
	if(hmod)
		dlclose(hmod);
}

#endif//PLATFORM_POSIX

УКДЛЛ грузиДлл__(Ткст& inoutfn, кткст0 const *names, ук const *procs)
{
	кткст0 фн = inoutfn;
	while(*фн) {
		кткст0 b = фн;
		while(*фн && *фн != ';'
#ifndef PLATFORM_WIN32
			&& *фн != ':'
#endif
		)
			фн++;
		Ткст libname(b, фн);
		if(*фн)
			фн++;
		Вектор<проц *> plist;
		if(УКДЛЛ hmod = проверьДлл__(libname, names, plist)) {
			for(цел i = 0; i < plist.дайСчёт(); i++)
				*(проц **)*procs++ = plist[i];
			inoutfn = libname;
			return hmod;
		}
	}
	return 0;
}
