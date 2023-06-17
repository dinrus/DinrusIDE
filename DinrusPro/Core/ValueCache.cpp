#include <DinrusPro/DinrusCore.h>

#define LLOG(x) // LOG(x)

СтатическийСтопор СтопорКэшаЗначений;

std::atomic<бул> sValueCacheFinished;

struct ValueMakeCacheClass : КэшЛРУ<Значение> {
	~ValueMakeCacheClass() { sValueCacheFinished = true; }
};

КэшЛРУ<Значение>& дайКэшЗначений()
{
	static ValueMakeCacheClass m;
	return m;
}

бул активенКэшЗначений()
{
	return !sValueCacheFinished;
}

цел ValueCacheMaxSize = 4000000;

цел ValueCacheMaxSizeLimitLow = 1024*1024;
цел ValueCacheMaxSizeLimitHigh = 0;
дво ValueCacheRatio = 0.125;

проц регулируйКэшЗначений()
{
	Стопор::Замок __(СтопорКэшаЗначений);
	бдол total, available;
	дайСтатусСисПамяти(total, available);
	if(ValueCacheMaxSizeLimitHigh == 0)
		ValueCacheMaxSizeLimitHigh = INT_MAX;
	ValueCacheMaxSize = clamp((цел)мин((дол)(ValueCacheRatio * available), (дол)2000*1024*1024),
	                          ValueCacheMaxSizeLimitLow, ValueCacheMaxSizeLimitHigh);
	LLOG("нов сделайЗначение макс size " << ValueCacheMaxSize << " high limit " << ValueCacheMaxSizeLimitHigh);
	сожмиКэшЗначений();
}

проц сожмиКэшЗначений()
{
	Стопор::Замок __(СтопорКэшаЗначений);
	if(!ValueCacheMaxSizeLimitHigh)
		регулируйКэшЗначений();
	дайКэшЗначений().сожми(ValueCacheMaxSize, 20000);
	LLOG("сделайЗначение cache size after shrink: " << дайКэшЗначений().дайРазм());
}

проц настройКэшЗначений(цел limit_low, цел limit_high, дво ratio)
{
	Стопор::Замок __(СтопорКэшаЗначений);

	ValueCacheMaxSizeLimitLow = 1000000;
	ValueCacheMaxSizeLimitHigh = 256000000;
	ValueCacheRatio = 0.125;
}

Значение сделайЗначение(ДелецЗначения& m)
{
	Стопор::Замок __(СтопорКэшаЗначений);
	LLOG("сделайЗначение cache size before make: " << дайКэшЗначений().дайРазм());
	Значение v = дайКэшЗначений().дай(m);
	LLOG("сделайЗначение cache size after make: " << дайКэшЗначений().дайРазм());
	сожмиКэшЗначений();
	LLOG("-------------");
	return v;
}
