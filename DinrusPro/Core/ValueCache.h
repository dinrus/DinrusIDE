extern СтатическийСтопор ValueCacheMutex;

LRUCache<Значение>& TheValueCache();

typedef LRUCache<Значение>::Делец ValueMaker;

Значение MakeValue(ValueMaker& m);

бул IsValueCacheActive();

проц AdjustValueCache();
проц ShrinkValueCache();

проц SetupValueCache(цел limit_low, цел limit_high, дво ratio);

template <class P>
цел ValueCacheRemove(P what)
{
	Стопор::Замок __(ValueCacheMutex);
	return TheValueCache().удали(what);
}

template <class P>
цел ValueCacheRemoveOne(P what)
{
	Стопор::Замок __(ValueCacheMutex);
	return TheValueCache().удали(what);
}

template <class P>
проц ValueCacheAdjustSize(P getsize)
{
	Стопор::Замок __(ValueCacheMutex);
	TheValueCache().настройРазм(getsize);
}
