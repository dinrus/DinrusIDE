Значение  ParseJSON(СиПарсер& p);
Значение  ParseJSON(const char *s);

inline Ткст AsJSON(int i)             { return пусто_ли(i) ? "null" : какТкст(i); }
inline Ткст AsJSON(double n)          { return пусто_ли(n) ? "null" : какТкст(n); }
inline Ткст AsJSON(bool b)            { return b ? "true" : "false"; }
inline Ткст AsJSON(const Ткст& s)   { return какТкстСи(s, INT_MAX, NULL, ASCSTRING_JSON); }
inline Ткст AsJSON(const ШТкст& s)  { return какТкстСи(s.вТкст(), INT_MAX, NULL, ASCSTRING_JSON); }
inline Ткст AsJSON(const char *s)     { return какТкстСи(s, INT_MAX, NULL, ASCSTRING_JSON); }
Ткст AsJSON(Время tm);
Ткст AsJSON(Дата dt);

Ткст AsJSON(const Значение& v, const Ткст& indent, bool pretty);
Ткст AsJSON(const Значение& v, bool pretty = false);

class JsonArray;

class Json {
	Ткст text;

public:
	Json& CatRaw(const char *ключ, const Ткст& val);

	Ткст вТкст() const                                     { return "{" + text + "}"; }
	Ткст operator~() const                                    { return вТкст(); }
	operator Ткст() const                                     { return вТкст(); }

	operator bool() const                                       { return text.дайСчёт(); }
	
	Json& operator()(const char *ключ, const Значение& значение)       { return CatRaw(ключ, AsJSON(значение)); }
	Json& operator()(const char *ключ, int i)                    { return CatRaw(ключ, AsJSON(i)); }
	Json& operator()(const char *ключ, double n)                 { return CatRaw(ключ, AsJSON(n)); }
	Json& operator()(const char *ключ, bool b)                   { return CatRaw(ключ, AsJSON(b)); }
	Json& operator()(const char *ключ, Дата d)                   { return CatRaw(ключ, AsJSON(d)); }
	Json& operator()(const char *ключ, Время t)                   { return CatRaw(ключ, AsJSON(t)); }
	Json& operator()(const char *ключ, const Ткст& s)          { return CatRaw(ключ, AsJSON(s)); }
	Json& operator()(const char *ключ, const ШТкст& s)         { return CatRaw(ключ, AsJSON(s)); }
	Json& operator()(const char *ключ, const char *s)            { return CatRaw(ключ, AsJSON(s)); }
	Json& operator()(const char *ключ, const Json& object)       { return CatRaw(ключ, ~object); }
	Json& operator()(const char *ключ, const JsonArray& array);
		
	Json() {}
	Json(const char *ключ, const Значение& значение)                   { CatRaw(ключ, AsJSON(значение)); }
	Json(const char *ключ, int i)                                { CatRaw(ключ, AsJSON(i)); }
	Json(const char *ключ, double n)                             { CatRaw(ключ, AsJSON(n)); }
	Json(const char *ключ, bool b)                               { CatRaw(ключ, AsJSON(b)); }
	Json(const char *ключ, Дата d)                               { CatRaw(ключ, AsJSON(d)); }
	Json(const char *ключ, Время t)                               { CatRaw(ключ, AsJSON(t)); }
	Json(const char *ключ, const Ткст& s)                      { CatRaw(ключ, AsJSON(s)); }
	Json(const char *ключ, const ШТкст& s)                     { CatRaw(ключ, AsJSON(s)); }
	Json(const char *ключ, const char *s)                        { CatRaw(ключ, AsJSON(s)); }
	Json(const char *ключ, const Json& object)                   { CatRaw(ключ, ~object); }
	Json(const char *ключ, const JsonArray& array)               { operator()(ключ, array); }
};

class JsonArray {
	Ткст text;

public:
	JsonArray& CatRaw(const Ткст& val);

	Ткст вТкст() const                                     { return "[" + text + "]"; }
	Ткст operator~() const                                    { return вТкст(); }
	operator Ткст() const                                     { return вТкст(); }
	
	operator bool() const                                       { return text.дайСчёт(); }
	
	JsonArray& operator<<(const Значение& значение)                   { return CatRaw(AsJSON(значение)); }
	JsonArray& operator<<(int i)                                { return CatRaw(AsJSON(i)); }
	JsonArray& operator<<(double n)                             { return CatRaw(AsJSON(n)); }
	JsonArray& operator<<(bool b)                               { return CatRaw(AsJSON(b)); }
	JsonArray& operator<<(Дата d)                               { return CatRaw(AsJSON(d)); }
	JsonArray& operator<<(Время t)                               { return CatRaw(AsJSON(t)); }
	JsonArray& operator<<(const Ткст& s)                      { return CatRaw(AsJSON(s)); }
	JsonArray& operator<<(const ШТкст& s)                     { return CatRaw(AsJSON(s)); }
	JsonArray& operator<<(const char *s)                        { return CatRaw(AsJSON(s)); }
	JsonArray& operator<<(const Json& object)                   { return CatRaw(~object); }
	JsonArray& operator<<(const JsonArray& array)               { return CatRaw(~array); }
		
	JsonArray() {}
};

inline Json& Json::operator()(const char *ключ, const JsonArray& array)
{
	return CatRaw(ключ, array);
}

class ДжейсонВВ {
	const Значение   *ист;
	Один<МапЗнач>  map;
	Значение          tgt;

public:
	bool грузится() const                       { return ист; }
	bool сохраняется() const                       { return !ист; }
	
	const Значение& дай() const                     { ПРОВЕРЬ(грузится()); return *ист; }
	void         уст(const Значение& v)             { ПРОВЕРЬ(сохраняется() && !map); tgt = v; }
	
	Значение        дай(const char *ключ)            { ПРОВЕРЬ(грузится()); return (*ист)[ключ]; }
	void         уст(const char *ключ, const Значение& v);

	void         помести(Значение& v)                   { ПРОВЕРЬ(сохраняется()); if(map) v = *map; else v = tgt; }
	Значение        дайРез() const               { ПРОВЕРЬ(сохраняется()); return map ? Значение(*map) : tgt; }

	template <class T>
	ДжейсонВВ& operator()(const char *ключ, T& значение);

	template <class T>
	ДжейсонВВ& operator()(const char *ключ, T& значение, const T& defvalue);

	template <class T>
	ДжейсонВВ& Список(const char *ключ, const char *, T& var) { return operator()(ключ, var); }

	template <class T, class X>
	ДжейсонВВ& Var(const char *ключ, T& значение, X item_jsonize);

	template <class T, class X>
	ДжейсонВВ& Массив(const char *ключ, T& значение, X item_jsonize, const char * = NULL);
	
	ДжейсонВВ(const Значение& ист) : ист(&ист)         {}
	ДжейсонВВ()                                     { ист = NULL; }
};

struct JsonizeError : Искл {
	JsonizeError(const Ткст& s) : Искл(s) {}
};

template <class T>
void вДжейсон(ДжейсонВВ& io, T& var)
{
	var.вДжейсон(io);
}

template <class T>
ДжейсонВВ& ДжейсонВВ::operator()(const char *ключ, T& значение)
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
ДжейсонВВ& ДжейсонВВ::Var(const char *ключ, T& значение, X jsonize)
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
void JsonizeArray(ДжейсонВВ& io, T& array, X item_jsonize)
{
	if(io.грузится()) {
		const Значение& va = io.дай();
		array.устСчёт(va.дайСчёт());
		for(int i = 0; i < va.дайСчёт(); i++) {
			ДжейсонВВ jio(va[i]);
			item_jsonize(jio, array[i]);
		}
	}
	else {
		Вектор<Значение> va;
		va.устСчёт(array.дайСчёт());
		for(int i = 0; i < array.дайСчёт(); i++) {
			ДжейсонВВ jio;
			item_jsonize(jio, array[i]);
			jio.помести(va[i]);
		}
		io.уст(МассивЗнач(pick(va)));
	}
}

template <class T, class X> ДжейсонВВ& ДжейсонВВ::Массив(const char *ключ, T& значение, X item_jsonize, const char *)
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
ДжейсонВВ& ДжейсонВВ::operator()(const char *ключ, T& значение, const T& defvalue)
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
void LoadFromJsonValue(T& var, const Значение& x)
{
	ДжейсонВВ io(x);
	вДжейсон(io, var);
}

template <class T>
Ткст StoreAsJson(const T& var, bool pretty = false)
{
	return AsJSON(StoreAsJsonValue(var), pretty);
}

template <class T>
bool LoadFromJson(T& var, const char *json)
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

Ткст sJsonFile(const char *file);

template <class T>
bool StoreAsJsonFile(const T& var, const char *file = NULL, bool pretty = false)
{
	return сохраниФайл(sJsonFile(file), StoreAsJson(var, pretty));;
}

template <class T>
bool LoadFromJsonFile(T& var, const char *file = NULL)
{
	return LoadFromJson(var, загрузиФайл(sJsonFile(file)));
}

template<> void вДжейсон(ДжейсонВВ& io, int& var);
template<> void вДжейсон(ДжейсонВВ& io, byte& var);
template<> void вДжейсон(ДжейсонВВ& io, int16& var);
template<> void вДжейсон(ДжейсонВВ& io, int64& var);
template<> void вДжейсон(ДжейсонВВ& io, double& var);
template<> void вДжейсон(ДжейсонВВ& io, bool& var);
template<> void вДжейсон(ДжейсонВВ& io, Ткст& var);
template<> void вДжейсон(ДжейсонВВ& io, ШТкст& var);
template<> void вДжейсон(ДжейсонВВ& io, Дата& var);
template<> void вДжейсон(ДжейсонВВ& io, Время& var);

template <class T>
void JsonizeArray(ДжейсонВВ& io, T& array)
{
	JsonizeArray(io, array, [](ДжейсонВВ& io, типЗначУ<T>& элт) { вДжейсон(io, элт); });
}

template <class T, class K, class V>
void JsonizeMap(ДжейсонВВ& io, T& map, const char *keyid, const char *valueid)
{
	if(io.грузится()) {
		map.очисть();
		const Значение& va = io.дай();
		map.резервируй(va.дайСчёт());
		for(int i = 0; i < va.дайСчёт(); i++) {
			K ключ;
			V значение;
			LoadFromJsonValue(ключ, va[i][keyid]);
			LoadFromJsonValue(значение, va[i][valueid]);
			map.добавь(ключ, pick(значение));
		}
	}
	else {
		Вектор<Значение> va;
		va.устСчёт(map.дайСчёт());
		for(int i = 0; i < map.дайСчёт(); i++)
			if(!map.отлинкован(i)) {
				МапЗнач элт;
				элт.добавь(keyid, StoreAsJsonValue(map.дайКлюч(i)));
				элт.добавь(valueid, StoreAsJsonValue(map[i]));
				va[i] = элт;
			}
		io.уст(МассивЗнач(pick(va)));
	}
}

template <class T, class K, class V>
void JsonizeSortedMap(ДжейсонВВ& io, T& map, const char *keyid, const char *valueid)
{
	if(io.грузится()) {
		map.очисть();
		const Значение& va = io.дай();
		for(int i = 0; i < va.дайСчёт(); i++) {
			K ключ;
			V значение;
			LoadFromJsonValue(ключ, va[i][keyid]);
			LoadFromJsonValue(значение, va[i][valueid]);
			map.добавь(ключ, pick(значение));
		}
	}
	else {
		Вектор<Значение> va;
		va.устСчёт(map.дайСчёт());
		for(int i = 0; i < map.дайСчёт(); i++) {
			МапЗнач элт;
			элт.добавь(keyid, StoreAsJsonValue(map.дайКлюч(i)));
			элт.добавь(valueid, StoreAsJsonValue(map[i]));
			va[i] = элт;
		}
		io.уст(МассивЗнач(pick(va)));
	}
}

template <class T, class K, class V>
void JsonizeStringMap(ДжейсонВВ& io, T& map)
{
	if(io.грузится()) {
		map.очисть();
		const МапЗнач& va = io.дай();
		map.резервируй(va.дайСчёт());
		for(int i = 0; i < va.дайСчёт(); i++) {
			V значение;
			Ткст ключ = va.дайКлюч(i);
			LoadFromJsonValue(ключ, va.дайКлюч(i));
			LoadFromJsonValue(значение, va.дайЗначение(i));
			map.добавь(ключ, pick(значение));
		}
	}
	else {
		Индекс<Значение>  индекс;
		Вектор<Значение> values;
		индекс.резервируй(map.дайСчёт());
		values.резервируй(map.дайСчёт());
		for (int i=0; i<map.дайСчёт(); ++i)
		{
			индекс.добавь(StoreAsJsonValue(map.дайКлюч(i)));
			values.добавь(StoreAsJsonValue(map[i]));
		}
		МапЗнач vm(pick(индекс), pick(values));
		io.уст(vm);
	}
}

template <class K, class V>
void StringMap(ДжейсонВВ& io, ВекторМап<K, V>& map)
{
	JsonizeStringMap<ВекторМап<K, V>, K, V>(io, map);
}

template <class K, class V>
void StringMap(ДжейсонВВ& io, МассивМап<K, V>& map)
{
	JsonizeStringMap<МассивМап<K, V>, K, V>(io, map);
}

template <class T, class V>
void JsonizeIndex(ДжейсонВВ& io, T& индекс)
{
	if(io.грузится()) {
		const Значение& va = io.дай();
		индекс.резервируй(va.дайСчёт());
		for(int i = 0; i < va.дайСчёт(); i++) {
			V v;
			LoadFromJsonValue(v, va[i]);
			индекс.добавь(pick(v));
		}
	}
	else {
		Вектор<Значение> va;
		for(int i = 0; i < индекс.дайСчёт(); i++)
			if(!индекс.отлинкован(i))
				va.добавь(StoreAsJsonValue(индекс[i]));
		io.уст(МассивЗнач(pick(va)));
	}
}

template <class T>
void JsonizeBySerialize(ДжейсонВВ& jio, T& x)
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

	void вДжейсон(ДжейсонВВ& io) { ize(io); }
	void вРяр(РярВВ& io) { ize(io); }
	
	LambdaIzeVar(IZE& ize) : ize(ize) {}
};

template <class IO, class IZE>
void LambdaIze(IO& io, const char *ид, IZE ize)
{
	LambdaIzeVar<IZE> var(ize);
	io(ид, var);
}
