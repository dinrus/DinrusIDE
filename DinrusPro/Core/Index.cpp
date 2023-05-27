#include <DinrusPro/DinrusPro.h>

namespace ДинрусРНЦП {

цел ИндексОбщее::empty[1] = { -1 };

ИндексОбщее::ИндексОбщее()
{
	hash = NULL;
	map = empty;
	mask = 0;
	unlinked = -1;
}

проц ИндексОбщее::подбери(ИндексОбщее& b)
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

проц ИндексОбщее::копируй(const ИндексОбщее& b, цел count)
{
	т_копирпам(hash, b.hash, count);
	mask = b.mask;
	unlinked = b.unlinked;

	освободиМап();
	map = (цел *)разместиПам((mask + 1) * sizeof(цел));
	т_копирпам(map, b.map, mask + 1);
}

проц ИндексОбщее::разверни(ИндексОбщее& b)
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

проц ИндексОбщее::освободиМап()
{
	if(map != empty)
		освободиПам(map);
}

проц ИндексОбщее::освободи()
{
	if(hash)
		освободиПам(hash);
	освободиМап();
}

проц ИндексОбщее::ремапируй(цел count)
{
	заполни(map, map + mask + 1, -1);
	for(цел i = 0; i < count; i++) // todo: unlinked
		if(hash[i].hash)
			линкуй(i, hash[i].hash);
}

проц ИндексОбщее::реиндексируй(цел count)
{
	освободиМап();
	map = (цел *)разместиПам((mask + 1) * sizeof(цел));
	ремапируй(count);
}

проц ИндексОбщее::очисть()
{
	освободи();
	hash = NULL;
	map = empty;
	mask = 0;
	unlinked = -1;
}

проц ИндексОбщее::нарастиМап(цел count)
{
	mask = (mask << 1) | 3;
	реиндексируй(count);
}

Вектор<цел> ИндексОбщее::дайОтлинкованно() const
{
	Вектор<цел> r;
	цел i = unlinked;
	if(i >= 0) {
		do {
			i = hash[i].prev;
			r.добавь(i);
		}
		while(i != unlinked);
	}
	return r;
}

проц ИндексОбщее::AdjustMap(цел count, цел alloc)
{
	if(alloc == 0) {
		освободиМап();
		map = empty;
		mask = 0;
		return;
	}
	бцел msk = 0;
	while(msk < (бцел)alloc)
		msk = (msk << 1) | 3;
	if(msk != mask) {
		mask = msk;
		реиндексируй(count);
	}
}

проц ИндексОбщее::сделайМап(цел count)
{
	mask = 0;
	AdjustMap(count, count);
}

проц ИндексОбщее::обрежь(цел n, цел count)
{
	if(n == 0) {
		цел n = (цел)(mask + 1);
		for(цел i = 0; i < n; i++)
			map[i] = -1;
		unlinked = -1;
		return;
	}
	
	for(цел i = n; i < count; i++) { // remove items in trimmed area from buckets / unlinked
		Хэш& hh = hash[i];
		if(hh.hash)
			уд(map[hh.hash & mask], hh, i);
		else
			уд(unlinked, hh, i);
	}
}

проц ИндексОбщее::смети(цел n)
{
	цел ti = 0;
	for(цел i = 0; i < n; i++)
		if(hash[i].hash)
			hash[ti++].hash = hash[i].hash;
	ремапируй(ti);
	unlinked = -1;
}

}