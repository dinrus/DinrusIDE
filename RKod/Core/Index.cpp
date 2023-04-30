#include <RKod/Core.h>

namespace РНЦПДинрус {

int ИндексОбщее::empty[1] = { -1 };

ИндексОбщее::ИндексОбщее()
{
	hash = NULL;
	map = empty;
	mask = 0;
	unlinked = -1;
}

void ИндексОбщее::подбери(ИндексОбщее& b)
{
	освободи();

	hash = b.hash;
	map = b.map;
	mask = b.mask;
	unlinked = b.unlinked;
	
	b.hash = NULL;
	b.map = empty;
	b.mask = 0;
	b.unlinked = -1;
}

void ИндексОбщее::копируй(const ИндексОбщее& b, int count)
{
	memcpy_t(hash, b.hash, count);
	mask = b.mask;
	unlinked = b.unlinked;

	освободиМап();
	map = (int *)MemoryAlloc((mask + 1) * sizeof(int));
	memcpy_t(map, b.map, mask + 1);
}

void ИндексОбщее::разверни(ИндексОбщее& b)
{
	РНЦП::разверни(hash, b.hash);
	РНЦП::разверни(map, b.map);
	РНЦП::разверни(mask, b.mask);
	РНЦП::разверни(unlinked, b.unlinked);
}

ИндексОбщее::~ИндексОбщее()
{
	освободи();
}

void ИндексОбщее::освободиМап()
{
	if(map != empty)
		MemoryFree(map);
}

void ИндексОбщее::освободи()
{
	if(hash)
		MemoryFree(hash);
	освободиМап();
}

void ИндексОбщее::ремапируй(int count)
{
	Fill(map, map + mask + 1, -1);
	for(int i = 0; i < count; i++) // todo: unlinked
		if(hash[i].hash)
			линкуй(i, hash[i].hash);
}

void ИндексОбщее::реиндексируй(int count)
{
	освободиМап();
	map = (int *)MemoryAlloc((mask + 1) * sizeof(int));
	ремапируй(count);
}

void ИндексОбщее::очисть()
{
	освободи();
	hash = NULL;
	map = empty;
	mask = 0;
	unlinked = -1;
}

void ИндексОбщее::нарастиМап(int count)
{
	mask = (mask << 1) | 3;
	реиндексируй(count);
}

Вектор<int> ИндексОбщее::дайОтлинкованно() const
{
	Вектор<int> r;
	int i = unlinked;
	if(i >= 0) {
		do {
			i = hash[i].prev;
			r.добавь(i);
		}
		while(i != unlinked);
	}
	return r;
}

void ИндексОбщее::AdjustMap(int count, int alloc)
{
	if(alloc == 0) {
		освободиМап();
		map = empty;
		mask = 0;
		return;
	}
	dword msk = 0;
	while(msk < (dword)alloc)
		msk = (msk << 1) | 3;
	if(msk != mask) {
		mask = msk;
		реиндексируй(count);
	}
}

void ИндексОбщее::сделайМап(int count)
{
	mask = 0;
	AdjustMap(count, count);
}

void ИндексОбщее::обрежь(int n, int count)
{
	if(n == 0) {
		int n = (int)(mask + 1);
		for(int i = 0; i < n; i++)
			map[i] = -1;
		unlinked = -1;
		return;
	}
	
	for(int i = n; i < count; i++) { // remove items in trimmed area from buckets / unlinked
		Хэш& hh = hash[i];
		if(hh.hash)
			уд(map[hh.hash & mask], hh, i);
		else
			уд(unlinked, hh, i);
	}
}

void ИндексОбщее::смети(int n)
{
	int ti = 0;
	for(int i = 0; i < n; i++)
		if(hash[i].hash)
			hash[ti++].hash = hash[i].hash;
	ремапируй(ti);
	unlinked = -1;
}

}