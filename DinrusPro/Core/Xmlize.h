class РярВВ;

template <class T>
проц загрузиАтрРяр(T& var, const Ткст& text)
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
	бул     loading;
	Значение    userdata;

public:
	бул грузится() const            { return loading; }
	бул сохраняется() const            { return !loading; }

	УзелРяр& Узел()                   { return node; }
	const УзелРяр& Узел() const       { return node; }

	УзелРяр *operator->()             { return &node; }

	Ткст дайАтр(кткст0 ид)                    { return node.Атр(ид); }
	проц   устАтр(кткст0 ид, const Ткст& знач) { node.устАтр(ид, знач); }

	template <class T> РярВВ operator()(кткст0 tag, T& var);
	template <class T> РярВВ Список(кткст0 tag, кткст0 itemtag, T& var);
	template <class T, class X> РярВВ Var(кткст0 tag, T& var, X var_xmlize);
	template <class T, class X> РярВВ Массив(кткст0 tag, T& var, X item_xmlize, кткст0 itemtag = "элт");

	template <class T, class D> РярВВ operator()(кткст0 tag, T& var, const D& опр);
	template <class T, class D> РярВВ Список(кткст0 tag, кткст0 itemtag, T& var, const D& опр);

	template <class T> РярВВ Атр(кткст0 ид, T& var) {
		if(грузится())
			загрузиАтрРяр(var, node.Атр(ид));
		else
			node.устАтр(ид, сохраниАтрРяр(var));
		return *this;
	}

	template <class T, class D> РярВВ Атр(кткст0 ид, T& var, const D& опр) {
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

	РярВВ по(цел i)                                    { РярВВ m(node.по(i), грузится(), userdata); return m; }
	РярВВ добавь()                                        { РярВВ m(node.добавь(), грузится(), userdata); return m; }
	РярВВ добавь(кткст0 ид)                          { РярВВ m(node.добавь(ид), грузится(), userdata); return m; }
	РярВВ дайДобавь(кткст0 ид)                       { РярВВ m(node.дайДобавь(ид), грузится(), userdata); return m; }
	
	проц  SetUserData(const Значение& v)                  { userdata = v; }
	Значение GetUserData() const                          { return userdata; }

	РярВВ(УзелРяр& xml, бул loading, const Значение& userdata) : node(xml), loading(loading), userdata(userdata) {}
	РярВВ(УзелРяр& xml, бул loading) : node(xml), loading(loading) {}
	РярВВ(РярВВ xml, кткст0 tag) : node(xml.node.дайДобавь(tag)), loading(xml.loading), userdata(xml.userdata) {}
};

template<> inline проц загрузиАтрРяр(Ткст& var, const Ткст& text) { var = text; }
template<> inline Ткст сохраниАтрРяр(const Ткст& var)            { return var; }

template<> проц загрузиАтрРяр(ШТкст& var, const Ткст& text);
template<> Ткст сохраниАтрРяр(const ШТкст& var);
template<> проц загрузиАтрРяр(цел& var, const Ткст& text);
template<> Ткст сохраниАтрРяр(const цел& var);
template<> проц загрузиАтрРяр(бцел& var, const Ткст& text);
template<> Ткст сохраниАтрРяр(const бцел& var);
template<> проц загрузиАтрРяр(дво& var, const Ткст& text);
template<> Ткст сохраниАтрРяр(const дво& var);
template<> проц загрузиАтрРяр(бул& var, const Ткст& text);
template<> Ткст сохраниАтрРяр(const бул& var);
template <> проц загрузиАтрРяр(крат& var, const Ткст& text);
template <> Ткст сохраниАтрРяр(const крат& var);
template <> проц загрузиАтрРяр(дол& var, const Ткст& text);
template <> Ткст сохраниАтрРяр(const дол& var);
template <> проц загрузиАтрРяр(ббайт& var, const Ткст& text);
template <> Ткст сохраниАтрРяр(const ббайт& var);
template <> проц загрузиАтрРяр(Дата& var, const Ткст& text);
template <> Ткст сохраниАтрРяр(const Дата& var);
template <> проц загрузиАтрРяр(Время& var, const Ткст& text);
template <> Ткст сохраниАтрРяр(const Время& var);

template<> проц вРяр(РярВВ& xml, Ткст& var);
template<> проц вРяр(РярВВ& xml, ШТкст& var);
template<> проц вРяр(РярВВ& xml, цел& var);
template<> проц вРяр(РярВВ& xml, бцел& var);
template<> проц вРяр(РярВВ& xml, дво& var);
template<> проц вРяр(РярВВ& xml, бул& var);
template<> проц вРяр(РярВВ& xml, Дата& var);
template<> проц вРяр(РярВВ& xml, Время& var);
template<> проц вРяр(РярВВ& xml, крат& var);
template<> проц вРяр(РярВВ& xml, дол& var);
template<> проц вРяр(РярВВ& xml, ббайт& var);

проц XmlizeLangAttr(РярВВ& xml, цел& lang, кткст0 ид = "lang");
проц XmlizeLang(РярВВ& xml, кткст0 tag, цел& lang, кткст0 ид = "ид");

template <class T>
проц вРяр(РярВВ& xml, T& var)
{
	var.вРяр(xml);
}

template <class T>
проц вРяр(РярВВ& xml, const char* itemtag, T& var)
{
	var.вРяр(xml, itemtag);
}

template <class T, class X>
проц контейнерВРяр(РярВВ& xml, кткст0 tag, T& данные, X item_xmlize);

template<class T>
проц контейнерВРяр(РярВВ& xml, кткст0 tag, T& данные);

template<class K, class V, class T>
проц мапВРяр(РярВВ& xml, кткст0 keytag, кткст0 valuetag, T& данные);

template<class K, class V, class T>
проц XmlizeSortedMap(РярВВ& xml, кткст0 keytag, кткст0 valuetag, T& данные);

template<class K, class T>
проц индексВРяр(РярВВ& xml, кткст0 keytag, T& данные);

template<class T>
проц XmlizeStore(РярВВ& xml, const T& данные)
{
	ПРОВЕРЬ(xml.сохраняется());
	вРяр(xml, const_cast<T&>(данные));
}

template <class T>
struct ParamHelper__ {
	T&   данные;
	проц Invoke(РярВВ xml) {
		вРяр(xml, данные);
	}

	ParamHelper__(T& данные) : данные(данные) {}
};

Ткст DoStoreAsXML(Событие<РярВВ> xmlize, кткст0 имя);
бул   DoLoadFromXML(Событие<РярВВ> xmlize, const Ткст& xml);
бул   DoTryLoadFromXML(Событие<РярВВ> xmlize, const Ткст& xml);

template <class T>
Ткст StoreAsXML(const T& данные, кткст0 имя = NULL)
{
	ParamHelper__<T> p(const_cast<T &>(данные));
	return DoStoreAsXML([&](РярВВ io) { вРяр(io, const_cast<T &>(данные)); }, имя);
}

template <class T>
бул LoadFromXML(T& данные, const Ткст& xml)
{
	ParamHelper__<T> p(данные);
	return DoLoadFromXML(callback(&p, &ParamHelper__<T>::Invoke), xml);
}

template <class T>
бул TryLoadFromXML(T& данные, const Ткст& xml)
{
	ParamHelper__<T> p(данные);
	return DoTryLoadFromXML(callback(&p, &ParamHelper__<T>::Invoke), xml);
}

бул StoreAsXMLFile(Событие<РярВВ> xmlize, кткст0 имя = NULL, кткст0 file = NULL);
бул LoadFromXMLFile(Событие<РярВВ> xmlize, кткст0 file = NULL);
бул TryLoadFromXMLFile(Событие<РярВВ> xmlize, кткст0 file = NULL);

template <class T>
бул StoreAsXMLFile(T& данные, кткст0 имя = NULL, кткст0 file = NULL)
{
	ParamHelper__<T> p(данные);
	return StoreAsXMLFile(callback(&p, &ParamHelper__<T>::Invoke), имя, file);
}

template <class T>
бул LoadFromXMLFile(T& данные, кткст0 file = NULL)
{
	ParamHelper__<T> p(данные);
	return LoadFromXMLFile(callback(&p, &ParamHelper__<T>::Invoke), file);
}

template <class T>
бул TryLoadFromXMLFile(T& данные, кткст0 file = NULL)
{
	ParamHelper__<T> p(данные);
	return TryLoadFromXMLFile(callback(&p, &ParamHelper__<T>::Invoke), file);
}

template <class T>
проц XmlizeBySerialize(РярВВ& xio, T& x)
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

проц  StoreJsonValue(РярВВ& xio, const Значение& v);
Значение LoadJsonValue(const УзелРяр& n);

template <class T>
проц XmlizeByJsonize(РярВВ& xio, T& x)
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