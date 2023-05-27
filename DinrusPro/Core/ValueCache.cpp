#include <DinrusPro/DinrusPro.h>

#define LLOG(x) // LOG(x)

namespace ДинрусРНЦП {

СтатическийСтопор ValueCacheMutex;

std::atomic<бул> sValueCacheFinished;

struct ValueMakeCacheClass : LRUCache<Значение> {
	~ValueMakeCacheClass() { sValueCacheFinished = true; }
};

LRUCache<Значение>& TheValueCache()
{
	static ValueMakeCacheClass m;
	return m;
}

бул IsValueCacheActive()
{
	return !sValueCacheFinished;
}

цел ValueCacheMaxSize = 4000000;

цел ValueCacheMaxSizeLimitLow = 1024*1024;
цел ValueCacheMaxSizeLimitHigh = 0;
дво ValueCacheRatio = 0.125;

проц AdjustValueCache()
{
	Стопор::Замок __(ValueCacheMutex);
	бдол total, available;
	дайСтатусСисПамяти(total, available);
	if(ValueCacheMaxSizeLimitHigh == 0)
		ValueCacheMaxSizeLimitHigh = INT_MAX;
	ValueCacheMaxSize = clamp((цел)мин((дол)(ValueCacheRatio * available), (дол)2000*1024*1024),
	                          ValueCacheMaxSizeLimitLow, ValueCacheMaxSizeLimitHigh);
	LLOG("нов MakeValue макс size " << ValueCacheMaxSize << " high limit " << ValueCacheMaxSizeLimitHigh);
	ShrinkValueCache();
}

проц ShrinkValueCache()
{
	Стопор::Замок __(ValueCacheMutex);
	if(!ValueCacheMaxSizeLimitHigh)
		AdjustValueCache();
	TheValueCache().сожми(ValueCacheMaxSize, 20000);
	LLOG("MakeValue cache size after shrink: " << TheValueCache().дайРазм());
}

проц SetupValueCache(цел limit_low, цел limit_high, дво ratio)
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