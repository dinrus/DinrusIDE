Значение  ParseJSON(СиПарсер& p);
Значение  ParseJSON(кткст0 s);

inline Ткст AsJSON(цел i)             { return пусто_ли(i) ? "null" : какТкст(i); }
inline Ткст AsJSON(дво n)          { return пусто_ли(n) ? "null" : какТкст(n); }
inline Ткст AsJSON(бул b)            { return b ? "true" : "false"; }
inline Ткст AsJSON(const Ткст& s)   { return какТкстСи(s, INT_MAX, NULL, ASCSTRING_JSON); }
inline Ткст AsJSON(const ШТкст& s)  { return какТкстСи(s.вТкст(), INT_MAX, NULL, ASCSTRING_JSON); }
inline Ткст AsJSON(кткст0 s)     { return какТкстСи(s, INT_MAX, NULL, ASCSTRING_JSON); }
Ткст AsJSON(Время tm);
Ткст AsJSON(Дата dt);

Ткст AsJSON(const Значение& v, const Ткст& indent, бул pretty);
Ткст AsJSON(const Значение& v, бул pretty = false);

class JsonArray;

class Json {
	Ткст text;

public:
	Json& CatRaw(кткст0 ключ, const Ткст& знач);

	Ткст вТкст() const                                     { return "{" + text + "}"; }
	Ткст operator~() const                                    { return вТкст(); }
	operator Ткст() const                                     { return вТкст(); }

	operator бул() const                                       { return text.дайСчёт(); }
	
	Json& operator()(кткст0 ключ, const Значение& значение)       { return CatRaw(ключ, AsJSON(значение)); }
	Json& operator()(кткст0 ключ, цел i)                    { return CatRaw(ключ, AsJSON(i)); }
	Json& operator()(кткст0 ключ, дво n)                 { return CatRaw(ключ, AsJSON(n)); }
	Json& operator()(кткст0 ключ, бул b)                   { return CatRaw(ключ, AsJSON(b)); }
	Json& operator()(кткст0 ключ, Дата d)                   { return CatRaw(ключ, AsJSON(d)); }
	Json& operator()(кткст0 ключ, Время t)                   { return CatRaw(ключ, AsJSON(t)); }
	Json& operator()(кткст0 ключ, const Ткст& s)          { return CatRaw(ключ, AsJSON(s)); }
	Json& operator()(кткст0 ключ, const ШТкст& s)         { return CatRaw(ключ, AsJSON(s)); }
	Json& operator()(кткст0 ключ, кткст0 s)            { return CatRaw(ключ, AsJSON(s)); }
	Json& operator()(кткст0 ключ, const Json& object)       { return CatRaw(ключ, ~object); }
	Json& operator()(кткст0 ключ, const JsonArray& array);
		
	Json() {}
	Json(кткст0 ключ, const Значение& значение)                   { CatRaw(ключ, AsJSON(значение)); }
	Json(кткст0 ключ, цел i)                                { CatRaw(ключ, AsJSON(i)); }
	Json(кткст0 ключ, дво n)                             { CatRaw(ключ, AsJSON(n)); }
	Json(кткст0 ключ, бул b)                               { CatRaw(ключ, AsJSON(b)); }
	Json(кткст0 ключ, Дата d)                               { CatRaw(ключ, AsJSON(d)); }
	Json(кткст0 ключ, Время t)                               { CatRaw(ключ, AsJSON(t)); }
	Json(кткст0 ключ, const Ткст& s)                      { CatRaw(ключ, AsJSON(s)); }
	Json(кткст0 ключ, const ШТкст& s)                     { CatRaw(ключ, AsJSON(s)); }
	Json(кткст0 ключ, кткст0 s)                        { CatRaw(ключ, AsJSON(s)); }
	Json(кткст0 ключ, const Json& object)                   { CatRaw(ключ, ~object); }
	Json(кткст0 ключ, const JsonArray& array)               { operator()(ключ, array); }
};

class JsonArray {
	Ткст text;

public:
	JsonArray& CatRaw(const Ткст& знач);

	Ткст вТкст() const                                     { return "[" + text + "]"; }
	Ткст operator~() const                                    { return вТкст(); }
	operator Ткст() const                                     { return вТкст(); }
	
	operator бул() const                                       { return text.дайСчёт(); }
	
	JsonArray& operator<<(const Значение& значение)                   { return CatRaw(AsJSON(значение)); }
	JsonArray& operator<<(цел i)                                { return CatRaw(AsJSON(i)); }
	JsonArray& operator<<(дво n)                             { return CatRaw(AsJSON(n)); }
	JsonArray& operator<<(бул b)                               { return CatRaw(AsJSON(b)); }
	JsonArray& operator<<(Дата d)                               { return CatRaw(AsJSON(d)); }
	JsonArray& operator<<(Время t)                               { return CatRaw(AsJSON(t)); }
	JsonArray& operator<<(const Ткст& s)                      { return CatRaw(AsJSON(s)); }
	JsonArray& operator<<(const ШТкст& s)                     { return CatRaw(AsJSON(s)); }
	JsonArray& operator<<(кткст0 s)                        { return CatRaw(AsJSON(s)); }
	JsonArray& operator<<(const Json& object)                   { return CatRaw(~object); }
	JsonArray& operator<<(const JsonArray& array)               { return CatRaw(~array); }
		
	JsonArray() {}
};

inline Json& Json::operator()(кткст0 ключ, const JsonArray& array)
{
	return CatRaw(ключ, array);
}

class ДжейсонВВ {
	const Значение   *ист;
	Один<МапЗнач>  map;
	Значение          tgt;

public:
	бул грузится() const                       { return ист; }
	бул сохраняется() const                       { return !ист; }
	
	const Значение& дай() const                     { ПРОВЕРЬ(грузится()); return *ист; }
	проц         уст(const Значение& v)             { ПРОВЕРЬ(сохраняется() && !map); tgt = v; }
	
	Значение        дай(кткст0 ключ)            { ПРОВЕРЬ(грузится()); return (*ист)[ключ]; }
	проц         уст(кткст0 ключ, const Значение& v);

	проц         помести(Значение& v)                   { ПРОВЕРЬ(сохраняется()); if(map) v = *map; else v = tgt; }
	Значение        дайРез() const               { ПРОВЕРЬ(сохраняется()); return map ? Значение(*map) : tgt; }

	template <class T>
	ДжейсонВВ& operator()(кткст0 ключ, T& значение);

	template <class T>
	ДжейсонВВ& operator()(кткст0 ключ, T& значение, const T& defvalue);

	template <class T>
	ДжейсонВВ& Список(кткст0 ключ, const char *, T& var) { return operator()(ключ, var); }

	template <class T, class X>
	ДжейсонВВ& Var(кткст0 ключ, T& значение, X item_jsonize);

	template <class T, class X>
	ДжейсонВВ& Массив(кткст0 ключ, T& значение, X item_jsonize, кткст0  = NULL);
	
	ДжейсонВВ(const Значение& ист) : ист(&ист)         {}
	ДжейсонВВ()                                     { ист = NULL; }
};

struct JsonizeError : Искл {
	JsonizeError(const Ткст& s) : Искл(s) {}
};

template <class T>
проц вДжейсон(ДжейсонВВ& io, T& var)
{
	var.вДжейсон(io);
}

template <class T>
ДжейсонВВ& ДжейсонВВ::operator()(кткст0 ключ, T& значение)
{
	if(грузится()) {
		const Значение& v = (*ист)[ключ];
		if(!v.проц_ли()) {
			ДжейсонВВ jio(v);
			вДжейсон(jio, значение);
		}
	}
	else {
		ПРОВЕРЬ(tgt.проц_ли());
		if(!map)
			map.создай();
		ДжейсонВВ jio;
		вДжейсон(jio, значение);
		if(jio.map)
			map->добавь(ключ, *jio.map);
		else
			map->добавь(ключ, jio.tgt);
	}
	return *this;
}

template <class T, class X>
ДжейсонВВ& ДжейсонВВ::Var(кткст0 ключ, T& значение, X jsonize)
{
	if(грузится()) {
		const Значение& v = (*ист)[ключ];
		if(!v.проц_ли()) {
			ДжейсонВВ jio(v);
			jsonize(jio, значение);
		}
	}
	else {
		ПРОВЕРЬ(tgt.проц_ли());
		if(!map)
			map.создай();
		ДжейсонВВ jio;
		jsonize(jio, значение);
		if(jio.map)
			map->добавь(ключ, *jio.map);
		else
			map->добавь(ключ, jio.tgt);
	}
	return *this;
}


template <class T, class X>
проц JsonizeArray(ДжейсонВВ& io, T& array, X item_jsonize)
{
	if(io.грузится()) {
		const Значение& va = io.дай();
		array.устСчёт(va.дайСчёт());
		for(цел i = 0; i < va.дайСчёт(); i++) {
			ДжейсонВВ jio(va[i]);
			item_jsonize(jio, array[i]);
		}
	}
	else {
		Вектор<Значение> va;
		va.устСчёт(array.дайСчёт());
		for(цел i = 0; i < array.дайСчёт(); i++) {
			ДжейсонВВ jio;
			item_jsonize(jio, array[i]);
			jio.помести(va[i]);
		}
		io.уст(МассивЗнач(пикуй(va)));
	}
}

template <class T, class X> ДжейсонВВ& ДжейсонВВ::Массив(кткст0 ключ, T& значение, X item_jsonize, const char *)
{
	if(грузится()) {
		const Значение& v = (*ист)[ключ];
		if(!v.проц_ли()) {
			ДжейсонВВ jio(v);
			JsonizeArray(jio, значение, item_jsonize);
		}
	}
	else {
		ПРОВЕРЬ(tgt.проц_ли());
		if(!map)
			map.создай();
		ДжейсонВВ jio;
		JsonizeArray(jio, значение, item_jsonize);
		if(jio.map)
			map->добавь(ключ, *jio.map);
		else
			map->добавь(ключ, jio.tgt);
	}
	return *this;
}

template <class T>
ДжейсонВВ& ДжейсонВВ::operator()(кткст0 ключ, T& значение, const T& defvalue)
{
	if(грузится()) {
		const Значение& v = (*ист)[ключ];
		if(v.проц_ли())
			значение = defvalue;
		else {
			ДжейсонВВ jio(v);
			вДжейсон(jio, значение);
		}
	}
	else {
		ПРОВЕРЬ(tgt.проц_ли());
		if(!map)
			map.создай();
		ДжейсонВВ jio;
		вДжейсон(jio, значение);
		if(jio.map)
			map->добавь(ключ, *jio.map);
		else
			map->добавь(ключ, jio.tgt);
	}
	return *this;
}

template <class T>
Значение StoreAsJsonValue(const T& var)
{
	ДжейсонВВ io;
	вДжейсон(io, const_cast<T&>(var));
	return io.дайРез();
}

template <class T>
проц LoadFromJsonValue(T& var, const Значение& x)
{
	ДжейсонВВ io(x);
	вДжейсон(io, var);
}

template <class T>
Ткст StoreAsJson(const T& var, бул pretty = false)
{
	return AsJSON(StoreAsJsonValue(var), pretty);
}

template <class T>
бул LoadFromJson(T& var, кткст0 json)
{
	try {
		Значение jv = ParseJSON(json);
		if(jv.ошибка_ли())
			return false;
		LoadFromJsonValue(var, jv);
	}
	catch(ОшибкаТипаЗначения) {
		return false;
	}
	catch(JsonizeError) {
		return false;
	}
	return true;
}

Ткст sJsonFile(кткст0 file);

template <class T>
бул StoreAsJsonFile(const T& var, кткст0 file = NULL, бул pretty = false)
{
	return сохраниФайл(sJsonFile(file), StoreAsJson(var, pretty));;
}

template <class T>
бул LoadFromJsonFile(T& var, кткст0 file = NULL)
{
	return LoadFromJson(var, загрузиФайл(sJsonFile(file)));
}

template<> проц вДжейсон(ДжейсонВВ& io, цел& var);
template<> проц вДжейсон(ДжейсонВВ& io, ббайт& var);
template<> проц вДжейсон(ДжейсонВВ& io, крат& var);
template<> проц вДжейсон(ДжейсонВВ& io, дол& var);
template<> проц вДжейсон(ДжейсонВВ& io, дво& var);
template<> проц вДжейсон(ДжейсонВВ& io, бул& var);
template<> проц вДжейсон(ДжейсонВВ& io, Ткст& var);
template<> проц вДжейсон(ДжейсонВВ& io, ШТкст& var);
template<> проц вДжейсон(ДжейсонВВ& io, Дата& var);
template<> проц вДжейсон(ДжейсонВВ& io, Время& var);

template <class T>
проц JsonizeArray(ДжейсонВВ& io, T& array)
{
	JsonizeArray(io, array, [](ДжейсонВВ& io, типЗначУ<T>& элт) { вДжейсон(io, элт); });
}

template <class T, class K, class V>
проц JsonizeMap(ДжейсонВВ& io, T& map, кткст0 keyid, кткст0 valueid)
{
	if(io.грузится()) {
		map.очисть();
		const Значение& va = io.дай();
		map.резервируй(va.дайСчёт());
		for(цел i = 0; i < va.дайСчёт(); i++) {
			K ключ;
			V значение;
			LoadFromJsonValue(ключ, va[i][keyid]);
			LoadFromJsonValue(значение, va[i][valueid]);
			map.добавь(ключ, пикуй(значение));
		}
	}
	else {
		Вектор<Значение> va;
		va.устСчёт(map.дайСчёт());
		for(цел i = 0; i < map.дайСчёт(); i++)
			if(!map.отлинкован(i)) {
				МапЗнач элт;
				элт.добавь(keyid, StoreAsJsonValue(map.дайКлюч(i)));
				элт.добавь(valueid, StoreAsJsonValue(map[i]));
				va[i] = элт;
			}
		io.уст(МассивЗнач(пикуй(va)));
	}
}

template <class T, class K, class V>
проц JsonizeSortedMap(ДжейсонВВ& io, T& map, кткст0 keyid, кткст0 valueid)
{
	if(io.грузится()) {
		map.очисть();
		const Значение& va = io.дай();
		for(цел i = 0; i < va.дайСчёт(); i++) {
			K ключ;
			V значение;
			LoadFromJsonValue(ключ, va[i][keyid]);
			LoadFromJsonValue(значение, va[i][valueid]);
			map.добавь(ключ, пикуй(значение));
		}
	}
	else {
		Вектор<Значение> va;
		va.устСчёт(map.дайСчёт());
		for(цел i = 0; i < map.дайСчёт(); i++) {
			МапЗнач элт;
			элт.добавь(keyid, StoreAsJsonValue(map.дайКлюч(i)));
			элт.добавь(valueid, StoreAsJsonValue(map[i]));
			va[i] = элт;
		}
		io.уст(МассивЗнач(пикуй(va)));
	}
}

template <class T, class K, class V>
проц JsonizeStringMap(ДжейсонВВ& io, T& map)
{
	if(io.грузится()) {
		map.очисть();
		const МапЗнач& va = io.дай();
		map.резервируй(va.дайСчёт());
		for(цел i = 0; i < va.дайСчёт(); i++) {
			V значение;
			Ткст ключ = va.дайКлюч(i);
			LoadFromJsonValue(ключ, va.дайКлюч(i));
			LoadFromJsonValue(значение, va.дайЗначение(i));
			map.добавь(ключ, пикуй(значение));
		}
	}
	else {
		Индекс<Значение>  индекс;
		Вектор<Значение> values;
		индекс.резервируй(map.дайСчёт());
		values.резервируй(map.дайСчёт());
		for (цел i=0; i<map.дайСчёт(); ++i)
		{
			индекс.добавь(StoreAsJsonValue(map.дайКлюч(i)));
			values.добавь(StoreAsJsonValue(map[i]));
		}
		МапЗнач vm(пикуй(индекс), пикуй(values));
		io.уст(vm);
	}
}

template <class K, class V>
проц StringMap(ДжейсонВВ& io, ВекторМап<K, V>& map)
{
	JsonizeStringMap<ВекторМап<K, V>, K, V>(io, map);
}

template <class K, class V>
проц StringMap(ДжейсонВВ& io, МассивМап<K, V>& map)
{
	JsonizeStringMap<МассивМап<K, V>, K, V>(io, map);
}

template <class T, class V>
проц JsonizeIndex(ДжейсонВВ& io, T& индекс)
{
	if(io.грузится()) {
		const Значение& va = io.дай();
		индекс.резервируй(va.дайСчёт());
		for(цел i = 0; i < va.дайСчёт(); i++) {
			V v;
			LoadFromJsonValue(v, va[i]);
			индекс.добавь(пикуй(v));
		}
	}
	else {
		Вектор<Значение> va;
		for(цел i = 0; i < индекс.дайСчёт(); i++)
			if(!индекс.отлинкован(i))
				va.добавь(StoreAsJsonValue(индекс[i]));
		io.уст(МассивЗнач(пикуй(va)));
	}
}

template <class T>
проц JsonizeBySerialize(ДжейсонВВ& jio, T& x)
{
	Ткст h;
	if(jio.сохраняется())
	   h = гексТкст(сохраниКакТкст(x));
	jio("данные", h);
	if(jio.грузится())
		try {
			грузиИзТкст(x, сканГексТкст(h));
		}
		catch(LoadingError) {
			throw JsonizeError("jsonize by serialize Ошибка");
		}
}

template <class IZE>
struct LambdaIzeVar {
	IZE& ize;

	проц вДжейсон(ДжейсонВВ& io) { ize(io); }
	проц вРяр(РярВВ& io) { ize(io); }
	
	LambdaIzeVar(IZE& ize) : ize(ize) {}
};

template <class IO, class IZE>
проц LambdaIze(IO& io, кткст0 ид, IZE ize)
{
	LambdaIzeVar<IZE> var(ize);
	io(ид, var);
}
