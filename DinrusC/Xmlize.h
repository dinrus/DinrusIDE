class РярВВ;

template <class T>
void загрузиАтрРяр(T& var, const Ткст& text)
{
	var.загрузиАтрРяр(text);
}

template <class T>
Ткст сохраниАтрРяр(const T& var)
{
	return var.сохраниАтрРяр();
}

class РярВВ {
	УзелРяр& node;
	bool     loading;
	Значение    userdata;

public:
	bool грузится() const            { return loading; }
	bool сохраняется() const            { return !loading; }

	УзелРяр& Узел()                   { return node; }
	const УзелРяр& Узел() const       { return node; }

	УзелРяр *operator->()             { return &node; }

	Ткст дайАтр(const char *ид)                    { return node.Атр(ид); }
	void   устАтр(const char *ид, const Ткст& val) { node.устАтр(ид, val); }

	template <class T> РярВВ operator()(const char *tag, T& var);
	template <class T> РярВВ Список(const char *tag, const char *itemtag, T& var);
	template <class T, class X> РярВВ Var(const char *tag, T& var, X var_xmlize);
	template <class T, class X> РярВВ Массив(const char *tag, T& var, X item_xmlize, const char *itemtag = "элт");

	template <class T, class D> РярВВ operator()(const char *tag, T& var, const D& опр);
	template <class T, class D> РярВВ Список(const char *tag, const char *itemtag, T& var, const D& опр);

	template <class T> РярВВ Атр(const char *ид, T& var) {
		if(грузится())
			загрузиАтрРяр(var, node.Атр(ид));
		else
			node.устАтр(ид, сохраниАтрРяр(var));
		return *this;
	}

	template <class T, class D> РярВВ Атр(const char *ид, T& var, const D& опр) {
		if(грузится())
		    if(пусто_ли(node.Атр(ид)))
				var = опр;
		    else
				загрузиАтрРяр(var, node.Атр(ид));
		else
		if(var != опр)
			node.устАтр(ид, сохраниАтрРяр(var));
		return *this;
	}

	РярВВ по(int i)                                    { РярВВ m(node.по(i), грузится(), userdata); return m; }
	РярВВ добавь()                                        { РярВВ m(node.добавь(), грузится(), userdata); return m; }
	РярВВ добавь(const char *ид)                          { РярВВ m(node.добавь(ид), грузится(), userdata); return m; }
	РярВВ дайДобавь(const char *ид)                       { РярВВ m(node.дайДобавь(ид), грузится(), userdata); return m; }
	
	void  SetUserData(const Значение& v)                  { userdata = v; }
	Значение GetUserData() const                          { return userdata; }

	РярВВ(УзелРяр& xml, bool loading, const Значение& userdata) : node(xml), loading(loading), userdata(userdata) {}
	РярВВ(УзелРяр& xml, bool loading) : node(xml), loading(loading) {}
	РярВВ(РярВВ xml, const char *tag) : node(xml.node.дайДобавь(tag)), loading(xml.loading), userdata(xml.userdata) {}
};

template<> inline void загрузиАтрРяр(Ткст& var, const Ткст& text) { var = text; }
template<> inline Ткст сохраниАтрРяр(const Ткст& var)            { return var; }

template<> void загрузиАтрРяр(ШТкст& var, const Ткст& text);
template<> Ткст сохраниАтрРяр(const ШТкст& var);
template<> void загрузиАтрРяр(int& var, const Ткст& text);
template<> Ткст сохраниАтрРяр(const int& var);
template<> void загрузиАтрРяр(dword& var, const Ткст& text);
template<> Ткст сохраниАтрРяр(const dword& var);
template<> void загрузиАтрРяр(double& var, const Ткст& text);
template<> Ткст сохраниАтрРяр(const double& var);
template<> void загрузиАтрРяр(bool& var, const Ткст& text);
template<> Ткст сохраниАтрРяр(const bool& var);
template <> void загрузиАтрРяр(int16& var, const Ткст& text);
template <> Ткст сохраниАтрРяр(const int16& var);
template <> void загрузиАтрРяр(int64& var, const Ткст& text);
template <> Ткст сохраниАтрРяр(const int64& var);
template <> void загрузиАтрРяр(byte& var, const Ткст& text);
template <> Ткст сохраниАтрРяр(const byte& var);
template <> void загрузиАтрРяр(Дата& var, const Ткст& text);
template <> Ткст сохраниАтрРяр(const Дата& var);
template <> void загрузиАтрРяр(Время& var, const Ткст& text);
template <> Ткст сохраниАтрРяр(const Время& var);

template<> void вРяр(РярВВ& xml, Ткст& var);
template<> void вРяр(РярВВ& xml, ШТкст& var);
template<> void вРяр(РярВВ& xml, int& var);
template<> void вРяр(РярВВ& xml, dword& var);
template<> void вРяр(РярВВ& xml, double& var);
template<> void вРяр(РярВВ& xml, bool& var);
template<> void вРяр(РярВВ& xml, Дата& var);
template<> void вРяр(РярВВ& xml, Время& var);
template<> void вРяр(РярВВ& xml, int16& var);
template<> void вРяр(РярВВ& xml, int64& var);
template<> void вРяр(РярВВ& xml, byte& var);

void XmlizeLangAttr(РярВВ& xml, int& lang, const char *ид = "lang");
void XmlizeLang(РярВВ& xml, const char *tag, int& lang, const char *ид = "ид");

template <class T>
void вРяр(РярВВ& xml, T& var)
{
	var.вРяр(xml);
}

template <class T>
void вРяр(РярВВ& xml, const char* itemtag, T& var)
{
	var.вРяр(xml, itemtag);
}

template <class T, class X>
void контейнерВРяр(РярВВ& xml, const char *tag, T& данные, X item_xmlize);

template<class T>
void контейнерВРяр(РярВВ& xml, const char *tag, T& данные);

template<class K, class V, class T>
void мапВРяр(РярВВ& xml, const char *keytag, const char *valuetag, T& данные);

template<class K, class V, class T>
void XmlizeSortedMap(РярВВ& xml, const char *keytag, const char *valuetag, T& данные);

template<class K, class T>
void индексВРяр(РярВВ& xml, const char *keytag, T& данные);

template<class T>
void XmlizeStore(РярВВ& xml, const T& данные)
{
	ПРОВЕРЬ(xml.сохраняется());
	вРяр(xml, const_cast<T&>(данные));
}

template <class T>
struct ParamHelper__ {
	T&   данные;
	void Invoke(РярВВ xml) {
		вРяр(xml, данные);
	}

	ParamHelper__(T& данные) : данные(данные) {}
};

Ткст DoStoreAsXML(Событие<РярВВ> xmlize, const char *имя);
bool   DoLoadFromXML(Событие<РярВВ> xmlize, const Ткст& xml);
bool   DoTryLoadFromXML(Событие<РярВВ> xmlize, const Ткст& xml);

template <class T>
Ткст StoreAsXML(const T& данные, const char *имя = NULL)
{
	ParamHelper__<T> p(const_cast<T &>(данные));
	return DoStoreAsXML([&](РярВВ io) { вРяр(io, const_cast<T &>(данные)); }, имя);
}

template <class T>
bool LoadFromXML(T& данные, const Ткст& xml)
{
	ParamHelper__<T> p(данные);
	return DoLoadFromXML(callback(&p, &ParamHelper__<T>::Invoke), xml);
}

template <class T>
bool TryLoadFromXML(T& данные, const Ткст& xml)
{
	ParamHelper__<T> p(данные);
	return DoTryLoadFromXML(callback(&p, &ParamHelper__<T>::Invoke), xml);
}

bool StoreAsXMLFile(Событие<РярВВ> xmlize, const char *имя = NULL, const char *file = NULL);
bool LoadFromXMLFile(Событие<РярВВ> xmlize, const char *file = NULL);
bool TryLoadFromXMLFile(Событие<РярВВ> xmlize, const char *file = NULL);

template <class T>
bool StoreAsXMLFile(T& данные, const char *имя = NULL, const char *file = NULL)
{
	ParamHelper__<T> p(данные);
	return StoreAsXMLFile(callback(&p, &ParamHelper__<T>::Invoke), имя, file);
}

template <class T>
bool LoadFromXMLFile(T& данные, const char *file = NULL)
{
	ParamHelper__<T> p(данные);
	return LoadFromXMLFile(callback(&p, &ParamHelper__<T>::Invoke), file);
}

template <class T>
bool TryLoadFromXMLFile(T& данные, const char *file = NULL)
{
	ParamHelper__<T> p(данные);
	return TryLoadFromXMLFile(callback(&p, &ParamHelper__<T>::Invoke), file);
}

template <class T>
void XmlizeBySerialize(РярВВ& xio, T& x)
{
	Ткст h;
	if(xio.сохраняется())
		h = гексТкст(сохраниКакТкст(x));
	xio.Атр("данные", h);
	if(xio.грузится())
		try {
			грузиИзТкст(x, сканГексТкст(h));
		}
		catch(LoadingError) {
			throw ОшибкаРяр("xmlize by serialize Ошибка");
		}
}

void  StoreJsonValue(РярВВ& xio, const Значение& v);
Значение LoadJsonValue(const УзелРяр& n);

template <class T>
void XmlizeByJsonize(РярВВ& xio, T& x)
{
	if(xio.сохраняется())
		StoreJsonValue(xio, StoreAsJsonValue(x));
	else {
		try {
			LoadFromJsonValue(x, LoadJsonValue(xio.Узел()));
		}
		catch(JsonizeError e) {
			throw ОшибкаРяр("xmlize by jsonize Ошибка: " + e);
		}
	}
}

#include "Xmlize.hpp"