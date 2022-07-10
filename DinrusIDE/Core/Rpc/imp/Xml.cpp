#include <Core/Core.h>
#include <Core/Rpc/Rpc.h>

namespace РНЦПДинрус {

Значение ParseXmlRpcValue(ПарсерРяр& p)
{
	Значение r;
	p.передайТэг("значение");
	if(p.Тэг("int") || p.Тэг("i4")) {
		Ткст s = p.читайТекст();
		СиПарсер p(s);
		if(!p.цел_ли())
			throw ОшибкаРяр("integer expected");
		r = p.читайЦел();
	}
	else
	if(p.Тэг("i8")) {
		Ткст s = p.читайТекст();
		СиПарсер p(s);
		if(!p.цел_ли())
			throw ОшибкаРяр("integer expected");
		r = p.читайЦел64();
	}
	else
	if(p.Тэг("boolean")) {
		int n = тктЦел(p.читайТекст());
		if(n != 0 && n != 1)
			throw ОшибкаРяр("boolean expected");
		r = (bool)n;
	}
	else
	if(p.Тэг("double")) {
		Ткст s = p.читайТекст();
		СиПарсер p(s);
		if(!p.дво_ли())
			throw ОшибкаРяр("double expected");
		r = p.читайДво();
	}
	else
	if(p.Тэг("string") || p.Тэг("base64"))
		r = p.читайТекст();
	else
	if(p.TagE("nil")) {
		p.передайКонец();
		return r;
	}
	else
	if(p.Тэг("dateTime.iso8601")) {
		Ткст s = обрежьОба(p.читайТекст());
		// 19980717T14:08:55
		// 01234567890123456
		if(s.дайСчёт() < 17 || s[8] != 'T' || s[11] != ':' || s[14] != ':') // <17 to allow zone ignored zone инфо
			throw ОшибкаРяр("invalid dateTime формат");
		Время tm;
		tm.year = atoi(s.середина(0, 4));
		tm.month = atoi(s.середина(4, 2));
		tm.day = atoi(s.середина(6, 2));
		tm.hour = atoi(s.середина(9, 2));
		tm.minute = atoi(s.середина(12, 2));
		tm.second = atoi(s.середина(15, 2));
		r = tm;
	}
	else
	if(p.Тэг("array")) {
		МассивЗнач va;
		p.передайТэг("данные");
		while(!p.стоп())
			va.добавь(ParseXmlRpcValue(p));
		r = va;
	}
	else
	if(p.Тэг("struct")) {
		МапЗнач vm;
		while(p.Тэг("member")) {
			p.передайТэг("имя");
			Ткст имя = p.читайТекст();
			p.передайКонец(); // имя
			vm.добавь((Значение)имя, ParseXmlRpcValue(p));
			p.передайКонец(); // member
		}
		r = vm;
	}
	else
		throw ОшибкаРяр("unknown значение тип");
	p.передайКонец();
	p.передайКонец();
	return r;
}

Значение ParseXmlRpcParam(ПарсерРяр& p)
{
	p.передайТэг("param");
	Значение v = ParseXmlRpcValue(p);
	p.передайКонец();
	return v;
	}

МассивЗнач ParseXmlRpcParams(ПарсерРяр& p)
{
	МассивЗнач va;
	if(p.Тэг("params"))
		while(!p.стоп())
			va.добавь(ParseXmlRpcParam(p));
	return va;
}

Ткст FormatXmlRpcValue(const Значение& _v, bool supports_i8)
{
	Ткст r;
	Значение v = _v;
	if(v.дайТип() == INT64_V && !supports_i8) {
		int64 x = v;
		if((int)x == x)
			v = (int)x;
	}
	if(пусто_ли(v) && !ткст_ли(v) && !массивЗнач_ли(v))
		r = ТэгРяр("nil")();
	else
	if(v.дайТип() == INT64_V && supports_i8)
		r = ТэгРяр("i8")(какТкст((int64)v));
	else
	if(v.дайТип() == INT_V)
		r = ТэгРяр("int")(фмт("%d", (int)v));
	else
	if(v.дайТип() == BOOL_V)
		r = ТэгРяр("boolean")(какТкст((int)(bool)v));
	else
	if(число_ли(v))
		r = ТэгРяр("double")(фмт("%.16g", (double)v));
	else
	if(датаВремя_ли(v)) {
		Время t = v;
		r = ТэгРяр("dateTime.iso8601")
					(фмт("%04.4d%02.2d%02.2d`T%02.2d`:%02.2d`:%02.2d",
					        t.year, t.month, t.day, t.hour, t.minute, t.second));
	}
	else
	if(v.дайТип() == VALUEMAP_V) {
		r = "<struct>";
		МапЗнач vm = v;
		const Индекс<Значение>& k = vm.дайКлючи();
		МассивЗнач va = vm.дайЗначения();
		for(int i = 0; i < k.дайСчёт(); i++)
			r << ТэгРяр("member")(ТэгРяр("имя")(k[i]) + FormatXmlRpcValue(va[i], supports_i8));
		r << "</struct>";
	}
	else
	if(v.дайТип() == VALUEARRAY_V) {
		r = "<array><данные>";
		МассивЗнач va = v;
		for(int i = 0; i < va.дайСчёт(); i++)
			r << FormatXmlRpcValue(va[i], supports_i8);
		r << "</данные></array>";
	}
	else
	if(v.является<RawJsonText>())
		r = ТэгРяр("string").устТекст(v.To<RawJsonText>().json);
	else
		r = ТэгРяр("string").устТекст(v);
	return ТэгРяр("значение")(r);
}

Ткст FormatXmlRpcParam(const Значение& param, bool supports_i8)
{
	return ТэгРяр("param")(FormatXmlRpcValue(param, supports_i8));
}

Ткст FormatXmlRpcParams(const МассивЗнач& params, bool supports_i8)
{
	Ткст r;
	r = "<params>";
	for(int i = 0; i < params.дайСчёт(); i++)
		r << FormatXmlRpcParam(params[i], supports_i8);
	r << "</params>";
	return r;
}

Ткст FormatXmlRpcError(int code, const char *text)
{
	Ткст r;
	r << XmlHeader() <<
		"<methodResponse>"
		  "<fault>"
		    "<значение>"
		      "<struct>"
		        "<member>"
		          "<имя>faultCode</имя>"
		          "<значение><int>" << code << "</int></значение>"
		        "</member>"
		        "<member>"
		          "<имя>faultString</имя>"
		          "<значение><string>" << DeXml(text) << "</string></значение>"
		        "</member>"
		      "</struct>"
		    "</значение>"
		  "</fault>"
		"</methodResponse>"
	;
	return r;
}

}
