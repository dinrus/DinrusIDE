#include "Debuggers.h"

#ifdef PLATFORM_WIN32

#define LLOG(x)  // DLOG(x)

int    Pdb::Байт(adr_t addr)
{
	if(!win64)
		addr &= 0xffffffff;
	int page = (int) (addr >> 10);
	if(invalidpage.найди(page) >= 0)
		return -1;
	int pos = (int) (addr & 1023);
	int q = mempage.найди(page);
	if(q >= 0)
		return (byte)mempage[q].data[pos];
	if(mempage.дайСчёт() > 1024)
		mempage.очисть();
	byte data[1024];
	if(ReadProcessMemory(hProcess, (LPCVOID) (addr & ~1023), data, 1024, NULL)) {
		LLOG("ReadProcessMemory " << Гекс(addr) << " OK");
		memcpy(mempage.добавь(page).data, data, 1024);
		return (byte)data[pos];
	}
	LLOG("ReadProcessMemory " << Гекс(addr) << ": " << дайПоследнОшСооб());
	invalidpage.добавь(page);
	return -1;
}

bool    Pdb::копируй(adr_t addr, void *ptr, int count)
{
	byte *s = (byte *)ptr;
	while(count--) {
		int q = Байт(addr++);
		if(q < 0)
			return false;
		*s++ = (byte)q;
	}
	return true;
}

Ткст Pdb::читайТкст(adr_t addr, int maxlen, bool allowzero)
{
	Ткст r;
	while(r.дайДлину() < maxlen) {
		int q = Байт(addr++);
		if(q < 0)
			break;
		if(!q && !allowzero)
			break;
		r.конкат(q);
	}
	return r;
}

ШТкст Pdb::читайШТкст(adr_t addr, int maxlen, bool allowzero)
{
	ШТкст r;
	while(r.дайДлину() < maxlen) {
		int q = Байт(addr++);
		if(q < 0)
			break;
		int w = Байт(addr++);
		if(w < 0)
			break;
		w = MAKEWORD(q, w);
		if(w == 0 && !allowzero)
			break;
		r.конкат(w);
	}
	return r;
}

#endif
