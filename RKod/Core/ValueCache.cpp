#include "Core.h"

#define LLOG(x) // LOG(x)

namespace РНЦПДинрус {

СтатическийСтопор ValueCacheMutex;

std::atomic<bool> sValueCacheFinished;

struct ValueMakeCacheClass : LRUCache<Значение> {
	~ValueMakeCacheClass() { sValueCacheFinished = true; }
};

LRUCache<Значение>& TheValueCache()
{
	static ValueMakeCacheClass m;
	return m;
}

bool IsValueCacheActive()
{
	return !sValueCacheFinished;
}

int ValueCacheMaxSize = 4000000;

int ValueCacheMaxSizeLimitLow = 1024*1024;
int ValueCacheMaxSizeLimitHigh = 0;
double ValueCacheRatio = 0.125;

void AdjustValueCache()
{
	Стопор::Замок __(ValueCacheMutex);
	uint64 total, available;
	дайСтатусСисПамяти(total, available);
	if(ValueCacheMaxSizeLimitHigh == 0)
		ValueCacheMaxSizeLimitHigh = INT_MAX;
	ValueCacheMaxSize = clamp((int)min((int64)(ValueCacheRatio * available), (int64)2000*1024*1024),
	                          ValueCacheMaxSizeLimitLow, ValueCacheMaxSizeLimitHigh);
	LLOG("нов MakeValue max size " << ValueCacheMaxSize << " high limit " << ValueCacheMaxSizeLimitHigh);
	ShrinkValueCache();
}

void ShrinkValueCache()
{
	Стопор::Замок __(ValueCacheMutex);
	if(!ValueCacheMaxSizeLimitHigh)
		AdjustValueCache();
	TheValueCache().сожми(ValueCacheMaxSize, 20000);
	LLOG("MakeValue cache size after shrink: " << TheValueCache().дайРазм());
}

void SetupValueCache(int limit_low, int limit_high, double ratio)
{
	Стопор::Замок __(ValueCacheMutex);

	ValueCacheMaxSizeLimitLow = 1000000;
	ValueCacheMaxSizeLimitHigh = 256000000;
	ValueCacheRatio = 0.125;
}

Значение MakeValue(ValueMaker& m)
{
	Стопор::Замок __(ValueCacheMutex);
	LLOG("MakeValue cache size before make: " << TheValueCache().дайРазм());
	Значение v = TheValueCache().дай(m);
	LLOG("MakeValue cache size after make: " << TheValueCache().дайРазм());
	ShrinkValueCache();
	LLOG("-------------");
	return v;
}

};