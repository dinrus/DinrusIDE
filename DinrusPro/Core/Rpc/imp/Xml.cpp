#include <DinrusPro/DinrusCore.h>
#include <RKod/Rpc/Rpc.h>

Значение ParseXmlRpcValue(ПарсерРяр& p)
{
	Значение r;
	p.передайТэг("значение");
	if(p.Тэг("цел") || p.Тэг("i4")) {
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
		цел n = тктЦел(p.читайТекст());
		if(n != 0 && n != 1)
			throw ОшибкаРяр("boolean expected");
		r = (бул)n;
	}
	else
	if(p.Тэг("дво")) {
		Ткст s = p.читайТекст();
		СиПарсер p(s);
		if(!p.дво_ли())
			throw ОшибкаРяр("дво expected");
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

Ткст FormatXmlRpcValue(const Значение& _v, бул supports_i8)
{
	Ткст r;
	Значение v = _v;
	if(v.дайТип() == INT64_V && !supports_i8) {
		дол x = v;
		if((цел)x == x)
			v = (цел)x;
	}
	if(пусто_ли(v) && !ткст_ли(v) && !массивЗнач_ли(v))
		r = ТэгРяр("nil")();
	else
	if(v.дайТип() == INT64_V && supports_i8)
		r = ТэгРяр("i8")(какТкст((дол)v));
	else
	if(v.дайТип() == INT_V)
		r = ТэгРяр("цел")(фмт("%d", (цел)v));
	else
	if(v.дайТип() == BOOL_V)
		r = ТэгРяр("boolean")(какТкст((цел)(бул)v));
	else
	if(число_ли(v))
		r = ТэгРяр("дво")(фмт("%.16g", (дво)v));
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
		for(цел i = 0; i < k.дайСчёт(); i++)
			r << ТэгРяр("member")(ТэгРяр("имя")(k[i]) + FormatXmlRpcValue(va[i], supports_i8));
		r << "</struct>";
	}
	else
	if(v.дайТип() == VALUEARRAY_V) {
		r = "<array><данные>";
		МассивЗнач va = v;
		for(цел i = 0; i < va.дайСчёт(); i++)
			r << FormatXmlRpcValue(va[i], supports_i8);
		r << "</данные></array>";
	}
	else
	if(v.является<RawДжейсонText>())
		r = ТэгРяр("string").устТекст(v.To<RawДжейсонText>().json);
	else
		r = ТэгРяр("string").устТекст(v);
	return ТэгРяр("значение")(r);
}

Ткст FormatXmlRpcParam(const Значение& param, бул supports_i8)
{
	return ТэгРяр("param")(FormatXmlRpcValue(param, supports_i8));
}

Ткст FormatXmlRpcParams(const МассивЗнач& params, бул supports_i8)
{
	Ткст r;
	r = "<params>";
	for(цел i = 0; i < params.дайСчёт(); i++)
		r << FormatXmlRpcParam(params[i], supports_i8);
	r << "</params>";
	return r;
}

Ткст FormatXmlRpcError(цел код_, кткст0 text)
{
	Ткст r;
	r << XmlHeader() <<
		"<methodResponse>"
		  "<fault>"
		    "<значение>"
		      "<struct>"
		        "<member>"
		          "<имя>faultCode</имя>"
		          "<значение><цел>" << код_ << "</цел></значение>"
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
