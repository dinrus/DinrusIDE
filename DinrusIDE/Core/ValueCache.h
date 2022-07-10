extern СтатическийСтопор ValueCacheMutex;

LRUCache<Значение>& TheValueCache();

typedef LRUCache<Значение>::Делец ValueMaker;

Значение MakeValue(ValueMaker& m);

bool IsValueCacheActive();

void AdjustValueCache();
void ShrinkValueCache();

void SetupValueCache(int limit_low, int limit_high, double ratio);

template <class P>
int ValueCacheRemove(P what)
{
	Стопор::Замок __(ValueCacheMutex);
	return TheValueCache().удали(what);
}

template <class P>
int ValueCacheRemoveOne(P what)
{
	Стопор::Замок __(ValueCacheMutex);
	return TheValueCache().удали(what);
}

template <class P>
void ValueCacheAdjustSize(P getsize)
{
	Стопор::Замок __(ValueCacheMutex);
	TheValueCache().настройРазм(getsize);
}
