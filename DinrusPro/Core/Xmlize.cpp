#include <DinrusPro/DinrusPro.h>

namespace ДинрусРНЦП {

template<>
проц вРяр(РярВВ& xml, Ткст& var)
{
	if(xml.грузится()) {
		for(цел i = 0; i < xml->дайСчёт(); i++)
			if(xml->Узел(i).текст_ли()) {
				var = xml->Узел(i).дайТекст();
				return;
			}
		var.очисть();
	}
	else {
		for(цел i = 0; i < xml->дайСчёт(); i++)
			if(xml->Узел(i).текст_ли()) {
				xml->по(i).создайТекст(var);
				return;
			}
		xml->добавь().создайТекст(var);
	}
}

template<>
проц вРяр(РярВВ& xml, ШТкст& var)
{
	Ткст h;
	if(xml.сохраняется())
		h = вУтф8(var);
	вРяр(xml, h);
	if(xml.грузится())
		var = вУтф32(h);
}

template<> проц загрузиАтрРяр(ШТкст& var, const Ткст& text)
{
	var = вУтф32(text);
}

template<> Ткст сохраниАтрРяр(const ШТкст& var)
{
	return вУтф8(var);
}

template<> проц загрузиАтрРяр(цел& var, const Ткст& text)
{
	var = сканЦел(text);
}

template<> Ткст сохраниАтрРяр(const цел& var)
{
	return какТкст(var);
}

template<> проц   загрузиАтрРяр(бцел& var, const Ткст& text)
{
	СиПарсер p(text);
	var = p.число_ли() ? p.читайЧисло() : 0;
}

template<> Ткст сохраниАтрРяр(const дво& var)
{
	return фмтГ(var, 17);
}

template<> проц загрузиАтрРяр(дво& var, const Ткст& text)
{
	var = сканДво(text);
}

template<> Ткст сохраниАтрРяр(const бцел& var)
{
	return какТкст(var);
}

template<> проц загрузиАтрРяр(бул& var, const Ткст& text)
{
	var = text == "1";
}

template<> Ткст сохраниАтрРяр(const бул& var)
{
	return var ? "1" : "0";
}

template<> проц загрузиАтрРяр(крат& var, const Ткст& text) {
	var = сканЦел(text);
}

template<> Ткст сохраниАтрРяр(const крат& var) {
	return какТкст(var);
}

template<> проц загрузиАтрРяр(дол& var, const Ткст& text) {
	var = сканЦел64(text);
}

template<> Ткст сохраниАтрРяр(const дол& var) {
	return какТкст(var);
}

template<> проц загрузиАтрРяр(ббайт& var, const Ткст& text) {
	var = сканЦел(text);
}

template<> Ткст сохраниАтрРяр(const ббайт& var) {
	return какТкст((цел)var);
}

template<> проц загрузиАтрРяр(Дата& var, const Ткст& text) {
	if(text.дайСчёт() > 6) {
		var.year = сканЦел(text.лево(4));
		var.month = сканЦел(text.середина(4, 2));
		var.day = сканЦел(text.середина(6));
		if(var.пригоден())
			return;
	}
	var = Null;
}

template<> Ткст сохраниАтрРяр(const Дата& var) {
	return фмт("%04d%02d%02d", var.year, var.month, var.day);
}

template<> проц загрузиАтрРяр(Время& var, const Ткст& text) {
	if(text.дайСчёт() > 15) {
		var.year = сканЦел(text.лево(4));
		var.month = сканЦел(text.середина(4, 2));
		var.day = сканЦел(text.середина(6, 2));
		var.hour = сканЦел(text.середина(9, 2));
		var.minute = сканЦел(text.середина(12, 2));
		var.second = сканЦел(text.середина(15));
		if(var.пригоден())
			return;
	}
	var = Null;
}

template<> Ткст сохраниАтрРяр(const Время& var) {
	return фмт("%04d%02d%02d`T%02d`:%02d`:%02d", var.year, var.month, var.day, var.hour, var.minute, var.second);
}

#define VALUE_XMLIZE(тип) template <> проц вРяр(РярВВ& xml, тип& var) { xml.Атр("значение", var); }

VALUE_XMLIZE(цел);
VALUE_XMLIZE(бцел);
VALUE_XMLIZE(дво);
VALUE_XMLIZE(бул);
VALUE_XMLIZE(крат);
VALUE_XMLIZE(дол);
VALUE_XMLIZE(ббайт);
VALUE_XMLIZE(Время);
VALUE_XMLIZE(Дата);

проц XmlizeLangAttr(РярВВ& xml, цел& lang, кткст0 ид)
{
	Ткст l;
	if(xml.сохраняется())
		l = LNGAsText(lang);
	xml.Атр(ид, l);
	if(xml.грузится() && !пусто_ли(l))
		lang = LNGFromText(l);
}

проц XmlizeLang(РярВВ& xml, кткст0 tag, цел& lang, кткст0 ид)
{
	РярВВ n(xml, tag);
	XmlizeLangAttr(n, lang, ид);
}

Ткст DoStoreAsXML(Событие<РярВВ> xmlize, кткст0 имя)
{
	Ткст n = фильтруй(имя ? имя : "app", CharFilterAlpha);
	УзелРяр node;
	xmlize(РярВВ(node(n), false, Значение()));
	return какРЯР(node);
}

бул LoadFromXML0(Событие<РярВВ> xmlize, const Ткст& xml)
{
	УзелРяр node = разбериРЯР(xml);
	if(node.дайСчёт() == 0)
		return false;
	for(цел i = 0; i < node.дайСчёт(); i++)
		if(node.Узел(i).тэг_ли()) {
			Значение dummy;
			xmlize(РярВВ(node.по(i), true, dummy));
			break;
		}
	return true;
}

бул DoLoadFromXML(Событие<РярВВ> xmlize, const Ткст& xml)
{
	try {
		return LoadFromXML0(xmlize, xml);
	}
	catch(ОшибкаТипаЗначения) {}
	catch(ОшибкаРяр) {}
	return false;
}

бул DoTryLoadFromXML(Событие<РярВВ> xmlize, const Ткст& xml)
{
	return LoadFromXML0(xmlize, xml);
}

static Ткст sXMLFile(кткст0 file)
{
	return file ? Ткст(file) : конфигФайл(дайТитулИсп() + ".xml");
}

бул StoreAsXMLFile(Событие<РярВВ> xmlize, кткст0 имя, кткст0 file)
{
	return сохраниФайл(sXMLFile(file), DoStoreAsXML(xmlize, имя ? (Ткст)имя : дайТитулИсп()));
}

бул LoadFromXMLFile(Событие<РярВВ> xmlize, кткст0 file)
{
	return DoLoadFromXML(xmlize, загрузиФайл(sXMLFile(file)));
}

бул TryLoadFromXMLFile(Событие<РярВВ> xmlize, кткст0 file)
{
	return DoTryLoadFromXML(xmlize, загрузиФайл(sXMLFile(file)));
}

проц StoreJsonValue(РярВВ& xio, const Значение& v)
{
	if(v.дайТип() == VALUEMAP_V) {
		МапЗнач m = v;
		МассивЗнач va = m.дайЗначения();
		for(цел i = 0; i < m.дайСчёт(); i++) {
			Значение h = m.дайЗначение(i);
			РярВВ io = xio.добавь((Ткст)m.дайКлюч(i));
			StoreJsonValue(io, h);
		}
		return;
	}
	else
	if(v.дайТип() == VALUEARRAY_V) {
		МассивЗнач va = v;
		for(цел i = 0; i < va.дайСчёт(); i++) {
			РярВВ io = xio.добавь("элт");
			Значение h = va[i];
			StoreJsonValue(io, h);
		}
	}
	else
	if(v.дайТип() == BOOL_V) {
		бул b = v;
		вРяр(xio, b);
	}
	else
	if(число_ли(v)) {
		дво h = v;
		вРяр(xio, h);
		return;
	}
	else
	if(ткст_ли(v)) {
		Ткст h = v;
		вРяр(xio, h);
	}
	else
	if(v.является<Дата>()) {
		Дата h = v;
		вРяр(xio, h);
	}
	else
	if(v.является<Время>()) {
		Время h = v;
		вРяр(xio, h);
	}
	else
		НИКОГДА();
}

Значение LoadJsonValue(const УзелРяр& n)
{
	Ткст h = n.Атр("значение");
	if(h.дайСчёт())
		return сканДво(h);
	МапЗнач m;
	Ткст text;
	for(цел i = 0; i < n.дайСчёт(); i++)
		if(n[i].тэг_ли())
			m.добавь(n[i].дайТэг(), LoadJsonValue(n[i]));
		else
			return n[i].дайТекст();
	return m;
}

}
