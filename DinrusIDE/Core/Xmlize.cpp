#include "Core.h"

namespace РНЦПДинрус {

template<>
void вРяр(РярВВ& xml, Ткст& var)
{
	if(xml.грузится()) {
		for(int i = 0; i < xml->дайСчёт(); i++)
			if(xml->Узел(i).текст_ли()) {
				var = xml->Узел(i).дайТекст();
				return;
			}
		var.очисть();
	}
	else {
		for(int i = 0; i < xml->дайСчёт(); i++)
			if(xml->Узел(i).текст_ли()) {
				xml->по(i).создайТекст(var);
				return;
			}
		xml->добавь().создайТекст(var);
	}
}

template<>
void вРяр(РярВВ& xml, ШТкст& var)
{
	Ткст h;
	if(xml.сохраняется())
		h = вУтф8(var);
	вРяр(xml, h);
	if(xml.грузится())
		var = вУтф32(h);
}

template<> void загрузиАтрРяр(ШТкст& var, const Ткст& text)
{
	var = вУтф32(text);
}

template<> Ткст сохраниАтрРяр(const ШТкст& var)
{
	return вУтф8(var);
}

template<> void загрузиАтрРяр(int& var, const Ткст& text)
{
	var = сканЦел(text);
}

template<> Ткст сохраниАтрРяр(const int& var)
{
	return какТкст(var);
}

template<> void   загрузиАтрРяр(dword& var, const Ткст& text)
{
	СиПарсер p(text);
	var = p.число_ли() ? p.читайЧисло() : 0;
}

template<> Ткст сохраниАтрРяр(const double& var)
{
	return фмтГ(var, 17);
}

template<> void загрузиАтрРяр(double& var, const Ткст& text)
{
	var = сканДво(text);
}

template<> Ткст сохраниАтрРяр(const dword& var)
{
	return какТкст(var);
}

template<> void загрузиАтрРяр(bool& var, const Ткст& text)
{
	var = text == "1";
}

template<> Ткст сохраниАтрРяр(const bool& var)
{
	return var ? "1" : "0";
}

template<> void загрузиАтрРяр(int16& var, const Ткст& text) {
	var = сканЦел(text);
}

template<> Ткст сохраниАтрРяр(const int16& var) {
	return какТкст(var);
}

template<> void загрузиАтрРяр(int64& var, const Ткст& text) {
	var = сканЦел64(text);
}

template<> Ткст сохраниАтрРяр(const int64& var) {
	return какТкст(var);
}

template<> void загрузиАтрРяр(byte& var, const Ткст& text) {
	var = сканЦел(text);
}

template<> Ткст сохраниАтрРяр(const byte& var) {
	return какТкст((int)var);
}

template<> void загрузиАтрРяр(Дата& var, const Ткст& text) {
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

template<> void загрузиАтрРяр(Время& var, const Ткст& text) {
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

#define VALUE_XMLIZE(тип) template <> void вРяр(РярВВ& xml, тип& var) { xml.Атр("значение", var); }

VALUE_XMLIZE(int);
VALUE_XMLIZE(dword);
VALUE_XMLIZE(double);
VALUE_XMLIZE(bool);
VALUE_XMLIZE(int16);
VALUE_XMLIZE(int64);
VALUE_XMLIZE(byte);
VALUE_XMLIZE(Время);
VALUE_XMLIZE(Дата);

void XmlizeLangAttr(РярВВ& xml, int& lang, const char *ид)
{
	Ткст l;
	if(xml.сохраняется())
		l = LNGAsText(lang);
	xml.Атр(ид, l);
	if(xml.грузится() && !пусто_ли(l))
		lang = LNGFromText(l);
}

void XmlizeLang(РярВВ& xml, const char *tag, int& lang, const char *ид)
{
	РярВВ n(xml, tag);
	XmlizeLangAttr(n, lang, ид);
}

Ткст DoStoreAsXML(Событие<РярВВ> xmlize, const char *имя)
{
	Ткст n = фильтруй(имя ? имя : "app", CharFilterAlpha);
	УзелРяр node;
	xmlize(РярВВ(node(n), false, Значение()));
	return какРЯР(node);
}

bool LoadFromXML0(Событие<РярВВ> xmlize, const Ткст& xml)
{
	УзелРяр node = разбериРЯР(xml);
	if(node.дайСчёт() == 0)
		return false;
	for(int i = 0; i < node.дайСчёт(); i++)
		if(node.Узел(i).тэг_ли()) {
			Значение dummy;
			xmlize(РярВВ(node.по(i), true, dummy));
			break;
		}
	return true;
}

bool DoLoadFromXML(Событие<РярВВ> xmlize, const Ткст& xml)
{
	try {
		return LoadFromXML0(xmlize, xml);
	}
	catch(ОшибкаТипаЗначения) {}
	catch(ОшибкаРяр) {}
	return false;
}

bool DoTryLoadFromXML(Событие<РярВВ> xmlize, const Ткст& xml)
{
	return LoadFromXML0(xmlize, xml);
}

static Ткст sXMLFile(const char *file)
{
	return file ? Ткст(file) : конфигФайл(дайТитулИсп() + ".xml");
}

bool StoreAsXMLFile(Событие<РярВВ> xmlize, const char *имя, const char *file)
{
	return сохраниФайл(sXMLFile(file), DoStoreAsXML(xmlize, имя ? (Ткст)имя : дайТитулИсп()));
}

bool LoadFromXMLFile(Событие<РярВВ> xmlize, const char *file)
{
	return DoLoadFromXML(xmlize, загрузиФайл(sXMLFile(file)));
}

bool TryLoadFromXMLFile(Событие<РярВВ> xmlize, const char *file)
{
	return DoTryLoadFromXML(xmlize, загрузиФайл(sXMLFile(file)));
}

void StoreJsonValue(РярВВ& xio, const Значение& v)
{
	if(v.дайТип() == VALUEMAP_V) {
		МапЗнач m = v;
		МассивЗнач va = m.дайЗначения();
		for(int i = 0; i < m.дайСчёт(); i++) {
			Значение h = m.дайЗначение(i);
			РярВВ io = xio.добавь((Ткст)m.дайКлюч(i));
			StoreJsonValue(io, h);
		}
		return;
	}
	else
	if(v.дайТип() == VALUEARRAY_V) {
		МассивЗнач va = v;
		for(int i = 0; i < va.дайСчёт(); i++) {
			РярВВ io = xio.добавь("элт");
			Значение h = va[i];
			StoreJsonValue(io, h);
		}
	}
	else
	if(v.дайТип() == BOOL_V) {
		bool b = v;
		вРяр(xio, b);
	}
	else
	if(число_ли(v)) {
		double h = v;
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
		NEVER();
}

Значение LoadJsonValue(const УзелРяр& n)
{
	Ткст h = n.Атр("значение");
	if(h.дайСчёт())
		return сканДво(h);
	МапЗнач m;
	Ткст text;
	for(int i = 0; i < n.дайСчёт(); i++)
		if(n[i].тэг_ли())
			m.добавь(n[i].дайТэг(), LoadJsonValue(n[i]));
		else
			return n[i].дайТекст();
	return m;
}

}
