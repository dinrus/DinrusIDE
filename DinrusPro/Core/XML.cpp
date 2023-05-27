#include <DinrusPro/DinrusPro.h>

namespace ДинрусРНЦП {

#define LLOG(x)  // LOG(x)

static inline проц sDeXmlChar(ТкстБуф& result, char chr, ббайт charset, бул escapelf)
{
	/**/ if(chr == '<')  result.кат("&lt;");
	else if(chr == '>')  result.кат("&gt;");
	else if(chr == '&')  result.кат("&amp;");
	else if(chr == '\'') result.кат("&apos;");
	else if(chr == '\"') result.кат("&quot;");
	else if((ббайт)chr < ' ' && (escapelf || chr != '\n' && chr != '\t' && chr != '\r'))
		result.кат(фмт("&#x%02x;", (ббайт)chr));
	else if(!(chr & 0x80) || charset == CHARSET_UTF8) result.кат(chr);
	else result.кат(вУтф8(вЮникод(chr, charset)));
}

Ткст DeXml(кткст0 s, ббайт charset, бул escapelf)
{
	if(charset == CHARSET_DEFAULT)
		charset = дайДефНабСим();
	ТкстБуф result;
	for(; *s; s++)
		sDeXmlChar(result, *s, charset, escapelf);
	return Ткст(result);
}

Ткст DeXml(кткст0 s, кткст0 end, ббайт charset, бул escapelf)
{
	if(charset == CHARSET_DEFAULT)
		charset = дайДефНабСим();
	ТкстБуф result;
	for(; s < end; s++)
		sDeXmlChar(result, *s, charset, escapelf);
	return Ткст(result);
}

Ткст DeXml(const Ткст& s, ббайт charset, бул escapelf)
{
	return DeXml(~s, s.стоп(), charset, escapelf);
}

Ткст XmlHeader(кткст0 encoding, кткст0 версия, кткст0 standalone)
{
	ТкстБуф r;
	r << "<?xml версия=\"" << версия << "\"";
	if(encoding)
		r << " encoding=\"" << encoding << "\"";
	if(standalone)
		r << " standalone=\"" << standalone << "\"";
	r << " ?>\r\n";
	return Ткст(r);
}

Ткст XmlPI(кткст0 text)
{
	ТкстБуф r;
	r << "<?" << text << "?>\r\n";
	return Ткст(r);
}

Ткст XmlDecl(кткст0 text)
{
	ТкстБуф r;
	r << "<!" << text << ">\r\n";
	return Ткст(r);
}

Ткст XmlDocType(кткст0 text)
{
	return XmlDecl("DOCTYPE " + Ткст(text));
}

Ткст XmlDoc(кткст0 имя, кткст0 xmlbody)
{
	return XmlHeader() + XmlDocType(имя) + ТэгРяр(имя)(xmlbody);
}

Ткст XmlComment(кткст0 text)
{
	ТкстБуф out;
	out << "<!-- " << text << " -->\r\n";
	return Ткст(out);
}

Ткст  ТэгРяр::operator()()
{
	return tag + "/>";
}

ТэгРяр& ТэгРяр::Тэг(кткст0 s)
{
	tag.очисть();
	end.очисть();
	tag << '<' << s;
	end << "</" << s << '>';
	return *this;
}

Ткст  ТэгРяр::operator()(кткст0 text)
{
	ТкстБуф r;
	r << tag << ">";
	кткст0 s = text;
	бул wastag = true;
	бул wasslash = true;
	бул first = true;
	while(*s) {
		кткст0 b = s;
		while(*s == ' ' || *s == '\t')
			s++;
		if(s[0] == '<') {
			if(first)
				r << "\r\n";
			if(wastag && (wasslash || s[1] != '/'))
				r.кат('\t');
		}
		else
		if(first) {
			r << text << end;
			return Ткст(r);
		}
		first = false;
		wasslash = false;
		char last = 0;
		while(*s && *s != '\n' && *s != '\r') {
			if(*s == '<')
				wasslash = s[1] == '/';
			if(*s == '/' && s[1] == '>')
				wasslash = true;
			last = *s++;
		}
		wastag = last == '>';
		if(*s == '\r')
			s++;
		if(*s == '\n')
			s++;
		r.кат(b, s);
	}
	if(!first)
		r << "\r\n";
	r << end;
	return Ткст(r);
}

Ткст  ТэгРяр::устТекст(кткст0 text, ббайт charset)
{
	ТкстБуф r;
	return r << tag << '>' << DeXml(text, charset) << end;
}

Ткст ТэгРяр::PreservedText(кткст0 text, ббайт charset)
{
	ТкстБуф r;
	return r << tag << " xml:пробелы=\"preserve\">" << DeXml(text, charset) << end;
}

ТэгРяр& ТэгРяр::operator()(кткст0 attr, кткст0 знач)
{
	tag << ' ' << attr << "=\"" << DeXml(знач, CHARSET_DEFAULT, true) << "\"";
	return *this;
}

ТэгРяр& ТэгРяр::operator()(кткст0 attr, цел q)
{
	return operator()(attr, какТкст(q));
}

ТэгРяр& ТэгРяр::operator()(кткст0 attr, дво q)
{
	return operator()(attr, какТкст(q));
}

force_inline
Ткст ПарсерРяр::Преобр(ТкстБуф& b)
{
	if(acharset == scharset)
		return b;
	return вНабсим(acharset, b, scharset);
}

проц ПарсерРяр::Сущ(ТкстБуф& out)
{
	грузиЕщё();
	цел outconst = 0;
	кткст0 t = ++term;
	if(*t == '#') {
		if(*++t == 'X' || *t == 'x') {
			for(ббайт c; (c = ctoi(*++t)) < 16; outconst = 16 * outconst + c)
				;
		}
		else {
			while(цифра_ли(*t))
				outconst = 10 * outconst + *t++ - '0';
		}
		out.кат(вУтф8(outconst));
		if(*t == ';')
			t++;
		term = t;
		return;
	}
	кткст0 b = t;
	while(*t && *t != ';')
		t++;
	if(*t == ';') {
		цел q = entity.найди(Ткст(b, t));
		if(q >= 0) {
			out.кат(entity[q]);
			term = t + 1;
			return;
		}
	}
	if(!relaxed)
		throw ОшибкаРяр("Unknown entity");
	out.кат('&');
}

inline static бул IsXmlNameChar(цел c)
{
	return IsAlNum(c) || c == '.' || c == '-' || c == '_' || c == ':';
}

проц ПарсерРяр::грузиЕщё0()
{ // WARNING: Invalidates pointers to буфер
	if(in && !in->кф_ли()) {
		цел pos = цел(term - begin);
		if(len - pos < MCHARS) {
			LLOG("грузиЕщё0 " << pos << ", " << len);
			begincolumn = дайКолонку0();
			len -= pos;
			memmove(буфер, term, len);
			term = begin = буфер;
			len += in->дай(~буфер + len, CHUNK);
			буфер[len] = '\0';
		}
	}
}

бул ПарсерРяр::ещё()
{ // WARNING: Invalidates pointers to буфер
	begincolumn = дайКолонку();
	if(!in || in->кф_ли())
		return false;
	LLOG("ещё " << (цел)CHUNK);
	len = in->дай(буфер, CHUNK);
	буфер[len] = '\0';
	term = begin = буфер;
	return len;
}

проц ПарсерРяр::пропустиПробелы()
{
	while(естьЕщё() && (ббайт)*term <= ' ') {
		if(*term == '\n')
			line++;
		term++;
	}
	грузиЕщё();
}

проц ПарсерРяр::читайАтр(ТкстБуф& attrval, цел c)
{
	term++;
	while(естьЕщё() && *term != c)
		if(*term == '&')
			Сущ(attrval);
		else
			attrval.кат(*term++);
	if(*term == c)
		term++;
}

проц ПарсерРяр::следщ()
{
	nattr.очисть();
	nattr1 = nattrval1 = Null;
	if(empty_tag) {
		empty_tag = false;
		тип = XML_END;
		LLOG("XML_END (empty tag) " << tagtext);
		return;
	}

	тип = Null;
	ТкстБуф raw_text;
	for(;;) {
		if(!естьЕщё()) {
			тип = XML_EOF;
			LLOG("XML_EOF");
			return;
		}
		грузиЕщё();
		if(*term == '<') {
			if(term[1] == '!' && term[2] == '[' &&
			   term[3] == 'C' && term[4] == 'D' && term[5] == 'A' && term[6] == 'T' && term[7] == 'A' &&
			   term[8] == '[') { // ![CDATA[
				term += 9;
				LLOG("CDATA");
				for(;;) {
					if(!естьЕщё())
						throw ОшибкаРяр("Unterminated CDATA");
					грузиЕщё();
					if(term[0] == ']' && term[1] == ']' && term[2] == '>') { // ]]>
						term += 3;
						break;
					}
					if(*term == '\n')
						line++;
					raw_text.кат(*term++);
				}
				тип = XML_TEXT;
				continue;
			}
			else
				break;
		}
		if(*term == '\n')
			line++;
		if(*term == '&') {
			Сущ(raw_text);
			тип = XML_TEXT;
		}
		else {
			if((ббайт)*term > ' ')
				тип = XML_TEXT;
			raw_text.кат(*term++);
		}
	}
	cdata = Преобр(raw_text);

	if(cdata.дайСчёт() && (npreserve || preserveall))
		тип = XML_TEXT;
	
	if(тип == XML_TEXT)
		return;
	
	term++;
	грузиЕщё();
	if(*term == '!') {
		tagtext.очисть();
		тип = XML_DECL;
		term++;
		if(term[0] == '-' && term[1] == '-') {
			тип = XML_COMMENT;
			term += 2;
			for(;;) {
				грузиЕщё();
				if(term[0] == '-' && term[1] == '-' && term[2] == '>')
					break;
				if(!естьЕщё())
					throw ОшибкаРяр("Unterminated comment");
				if(*term == '\n')
					line++;
				tagtext.кат(*term++);
			}
			term += 3;
			return;
		}
		бул intdt = false;
		for(;;) {
			грузиЕщё();
			if (*term == '[')
				intdt = true;
			if(*term == '>' && intdt == false) {
				term++;
				break;
			}
			if(intdt == true && term[0] == ']' && term[1] == '>') {
				tagtext.кат(*term++);
				term++;
				break;
			}
			if(!естьЕщё())
				throw ОшибкаРяр("Unterminated declaration");
			if(*term == '\n')
				line++;
			tagtext.кат(*term++);
		}
		LLOG("XML_DECL " << tagtext);
	}
	else
	if(*term == '?') {
		tagtext.очисть();
		тип = XML_PI;
		term++;
		for(;;) {
			грузиЕщё();
			if(term[0] == '?' && term[1] == '>') {
				term += 2;
				LLOG("XML_PI " << tagtext);
				if(!tagtext.начинаетсяС("xml "))
					return;
				цел q = tagtext.найди("encoding");
				if(q < 0)
					return;
				q = tagtext.найди('\"', q);
				if(q < 0)
					return;
				q++;
				цел w = tagtext.найди('\"', q);
				if(w < 0)
					return;
				q = набсимПоИмени(tagtext.середина(q, w));
				if(q)
					scharset = q;
				return;
			}
			if(!естьЕщё())
				throw ОшибкаРяр("Unterminated processing инфо");
			if(*term == '\n')
				line++;
			tagtext.кат(*term++);
		}
	}
	else
	if(*term == '/') {
		тип = XML_END;
		term++;
		кткст0 t = term;
		while(IsXmlNameChar(*term))
			term++;
		tagtext = Ткст(t, term);
		LLOG("XML_END " << tagtext);
		if(*term != '>')
			throw ОшибкаРяр("Unterminated end-tag");
		term++;
	}
	else {
		тип = XML_TAG;
		кткст0 t = term;
		while(IsXmlNameChar(*term))
			term++;
		tagtext = Ткст(t, term);
		LLOG("XML_TAG " << tagtext);
		for(;;) {
			пропустиПробелы();
			if(*term == '>') {
				term++;
				break;
			}
			if(term[0] == '/' && term[1] == '>') {
				cdata.очисть();
				empty_tag = true;
				term += 2;
				break;
			}
			if(!естьЕщё())
				throw ОшибкаРяр("Unterminated tag");
			грузиЕщё();
			кткст0 t = term++;
			while((ббайт)*term > ' ' && *term != '=' && *term != '>')
				term++;
			Ткст attr(t, term);
			пропустиПробелы();
			if(*term == '=') {
				term++;
				пропустиПробелы();
				ТкстБуф attrval;
				if(*term == '\"')
					читайАтр(attrval, '\"');
				else
				if(*term == '\'')
					читайАтр(attrval, '\'');
				else
					while(естьЕщё() && (ббайт)*term > ' ' && *term != '>' && *term != '/')
						if(*term == '&')
							Сущ(attrval);
						else
							attrval.кат(*term++);
				if(attr == "xml:space" && attrval.дайДлину() == 8 && !memcmp(~attrval, "preserve", 8))
					npreserve = true;
				Ткст aval = Преобр(attrval);
				if(пусто_ли(nattr1)) {
					nattr1 = attr;
					nattrval1 = aval;
				}
				else
					nattr.добавь(attr, aval);
			}
		}
	}
}

проц ПарсерРяр::регСущность(const Ткст& ид, const Ткст& text)
{
	entity.добавь(ид, text);
}

бул   ПарсерРяр::кф_ли()
{
	return тип == XML_EOF;
}

бул   ПарсерРяр::тэг_ли()
{
	return тип == XML_TAG;
}

Ткст ПарсерРяр::читайТэг(бул next)
{
	if(тип != XML_TAG)
		throw ОшибкаРяр("Expected tag");
	LLOG("читайТэг " << tagtext);
	Ткст h = tagtext;
	if(next) {
		if(!raw)
			stack.добавь(Nesting(h, npreserve));
		attr = pick(nattr);
		attr1 = nattr1;
		attrval1 = nattrval1;
		следщ();
	}
	return h;
}

бул  ПарсерРяр::Тэг(кткст0 tag)
{
	if(тэг_ли() && tagtext == tag) {
		LLOG("Тэг " << tagtext);
		if(!raw)
			stack.добавь(Nesting(tagtext, npreserve));
		attr = pick(nattr);
		attr1 = nattr1;
		attrval1 = nattrval1;
		следщ();
		return true;
	}
	return false;
}

бул  ПарсерРяр::Тэг(const Ткст& tag)
{
	if(тэг_ли() && tagtext == tag) {
		LLOG("Тэг " << tagtext);
		if(!raw)
			stack.добавь(Nesting(tagtext, npreserve));
		attr = pick(nattr);
		attr1 = nattr1;
		attrval1 = nattrval1;
		следщ();
		return true;
	}
	return false;
}

проц  ПарсерРяр::передайТэг(кткст0 tag)
{
	if(!Тэг(tag))
		throw ОшибкаРяр(Ткст().кат() << '\'' << tag << "\' tag expected");
}

проц  ПарсерРяр::передайТэг(const Ткст& tag)
{
	if(!Тэг(tag))
		throw ОшибкаРяр(Ткст().кат() << '\'' << tag << "\' tag expected");
}

Ткст ПарсерРяр::читайКонец(бул next)
{
	if(тип != XML_END)
		throw ОшибкаРяр("Expected end-tag");
	Ткст x = tagtext;
	if(next)
		следщ();
	return x;
}

бул  ПарсерРяр::конец_ли()
{
	return тип == XML_END;
}

бул  ПарсерРяр::стоп()
{
	if(кф_ли())
		throw ОшибкаРяр("Unexpected end of file");
	if(конец_ли()) {
		LLOG("завершиТэг " << tagtext);
		if(!raw) {
			if(stack.пустой())
				throw ОшибкаРяр(фмт("Unexpected end-tag: </%s>", tagtext));
			if(stack.верх().tag != tagtext && !relaxed) {
				LLOG("Тэг/end-tag mismatch: <" << stack.верх().tag << "> </" << tagtext << ">");
				throw ОшибкаРяр(фмт("Тэг/end-tag mismatch: <%s> </%s>", stack.верх().tag, tagtext));
			}
			stack.сбрось();
		}
		npreserve = (!stack.пустой() && stack.верх().preserve_blanks);
		следщ();
		return true;
	}
	return false;
}

бул ПарсерРяр::стоп(кткст0 tag)
{
	if(конец_ли() && tagtext == tag) {
		следщ();
		return true;
	}
	return false;
}

бул ПарсерРяр::стоп(const Ткст& tag)
{
	if(конец_ли() && tagtext == tag) {
		следщ();
		return true;
	}
	return false;
}

проц  ПарсерРяр::передайКонец()
{
	if(!стоп())
		throw ОшибкаРяр(Ткст().кат() << "Expected \'" << (stack.дайСчёт() ? stack.верх().tag : Ткст()) << "\' end-tag");
}

проц ПарсерРяр::передайКонец(кткст0 tag)
{
	if(!стоп(tag))
		throw ОшибкаРяр(Ткст().кат() << "Expected \'" << tag << "\' end-tag");
}

бул  ПарсерРяр::TagE(кткст0 tag)
{
	if(Тэг(tag)) {
		пропустиКонец();
		return true;
	}
	return false;
}

проц  ПарсерРяр::PassTagE(кткст0 tag)
{
	передайТэг(tag);
	пропустиКонец();
}

бул ПарсерРяр::TagElseSkip(кткст0 tag)
{
	if(Тэг(tag))
		return true;
	пропусти();
	return false;
}

бул ПарсерРяр::LoopTag(кткст0 tag)
{
	while(!стоп()) {
		if(Тэг(tag))
			return true;
		пропусти();
	}
	return false;
}

ВекторМап<Ткст, Ткст> ПарсерРяр::PickAttrs()
{
	if(!пусто_ли(attr1))
		pick(const_cast<ВекторМап<Ткст, Ткст>&>(attr).вставь(0, attr1, attrval1));
	return pick(attr);
}

цел   ПарсерРяр::Цел(кткст0 ид, цел опр) const
{
	if(ид == attr1) return сканЦел(attrval1);
	цел q = attr.найди(ид);
	return q < 0 ? опр : сканЦел(attr[q]);
}

дво ПарсерРяр::Дво(кткст0 ид, дво опр) const
{
	if(ид == attr1) return сканДво(attrval1);
	цел q = attr.найди(ид);
	return q < 0 ? опр : сканДво(attr[q]);
}

бул  ПарсерРяр::текст_ли()
{
	if(npreserve || preserveall)
		return cdata.дайСчёт();
	кткст0 e = cdata.стоп();
	for(кткст0 s = cdata.старт(); s < e; s++)
		if((ббайт)*s > ' ')
			return true;
	return false;
}

Ткст ПарсерРяр::читайТекст()
{
	Ткст h = cdata;
	cdata.очисть();
	if(тип == XML_TEXT)
		следщ();
	return h;
}

Ткст ПарсерРяр::ReadTextE()
{
	ТкстБуф out;
	for(;;) {
		Ткст t = читайТекст();
		if(!пусто_ли(t))
			out.кат(t);
		else if(конец_ли()) {
			передайКонец();
			return Ткст(out);
		}
		else
			пропусти();
	}
}

бул   ПарсерРяр::декл_ли()
{
	return тип == XML_DECL;
}

Ткст ПарсерРяр::читайДекл(бул next)
{
	if(!декл_ли())
		throw ОшибкаРяр("Declaration expected");
	Ткст h = tagtext;
	if(next)
		следщ();
	return h;
}

бул   ПарсерРяр::IsPI()
{
	return тип == XML_PI;
}

Ткст ПарсерРяр::ReadPI(бул next)
{
	if(!IsPI())
		throw ОшибкаРяр("Processing инфо expected");
	Ткст h = tagtext;
	if(next)
		следщ();
	return h;
}

бул   ПарсерРяр::коммент_ли()
{
	return тип == XML_COMMENT;
}

Ткст ПарсерРяр::читайКоммент(бул next)
{
	if(!коммент_ли())
		throw ОшибкаРяр("Comment expected");
	Ткст h = tagtext;
	if(next)
		следщ();
	return h;
}

проц ПарсерРяр::пропусти()
{
	if(кф_ли())
		throw ОшибкаРяр("Unexpected end of file");
	if(cdata.дайСчёт() && тип != XML_TEXT)
		cdata.очисть();
	else
	if(тэг_ли() && !raw) {
		Ткст n = читайТэг();
		while(!стоп()) {
			if(кф_ли())
				throw ОшибкаРяр("Unexpected end of file expected when skipping tag \'" + n + "\'");
			пропусти();
		}
	}
	else
		следщ();
}

проц ПарсерРяр::пропустиКонец()
{
	while(!конец_ли()) пропусти();
	передайКонец();
}

цел ПарсерРяр::дайКолонку0() const
{
	кткст0 s = term;
	цел n = 0;
	while(s > begin && *(s - 1) != '\n') {
		s--;
		n++;
	}
	return n + begincolumn;
}

проц ПарсерРяр::иниц()
{
	регСущность("lt", "<");
	регСущность("gt", ">");
	регСущность("amp", "&");
	регСущность("apos", "\'");
	регСущность("quot", "\"");
	relaxed = false;
	empty_tag = false;
	npreserve = false;
	line = 1;
	preserveall = false;
	begincolumn = 0;
	in = NULL;
	len = 0;
	raw = false;
	acharset = дайДефНабСим();
	scharset = CHARSET_UTF8;
}

ПарсерРяр::ПарсерРяр(кткст0 s)
{
	иниц();
	begin = term = s;
	len = INT_MAX;
	try { // Need to catch first Ошибка as construction is usually outside try-catch client block
		следщ();
	}
	catch(ОшибкаРяр) {
		begin = term = s;
		len = INT_MAX;
	}
}

ПарсерРяр::ПарсерРяр(Поток& in_)
{
	иниц();
	буфер.размести(CHUNK + MCHARS + 1);
	begin = term = "";
	in = &in_;
	try {
		следщ();
	}
	catch(ОшибкаРяр) {
		begin = term = "";
	}
}

цел УзелРяр::найдиТэг(кткст0 _tag) const
{
	Ткст tag = _tag;
	for(цел i = 0; i < node.дайСчёт(); i++)
		if(node[i].тип == XML_TAG && node[i].text == tag)
			return i;
	return -1;
}

УзелРяр& УзелРяр::добавь(кткст0 tag)
{
	УзелРяр& m = node.добавь();
	m.создайТэг(tag);
	return m;
}

УзелРяр& УзелРяр::дайДобавь(кткст0 tag)
{
	цел q = найдиТэг(tag);
	return q >= 0 ? node[q] : добавь(tag);
}

const УзелРяр& УзелРяр::Проц()
{
	static УзелРяр h;
	return h;
}

const УзелРяр& УзелРяр::operator[](кткст0 tag) const
{
	цел q = найдиТэг(tag);
	return q < 0 ? Проц() : node[q];
}

проц УзелРяр::удали(цел i)
{
	node.удали(i);
}

проц УзелРяр::удали(кткст0 tag)
{
	цел q = найдиТэг(tag);
	if(q >= 0)
		node.удали(q);
}

Ткст УзелРяр::собериТекст() const
{
	Ткст r;
	for(цел i = 0; i < дайСчёт(); i++)
		if(node[i].текст_ли())
			r << node[i].дайТекст();
	return r;
}

бул УзелРяр::естьТэги() const
{
	for(цел i = 0; i < дайСчёт(); i++)
		if(node[i].тэг_ли())
			return true;
	return false;
}

цел  УзелРяр::целАтр(кткст0 ид, цел опр) const
{
	Ткст x = Атр(ид);
	СиПарсер p(x);
	return p.цел_ли() ? p.читайЦел() : опр;
}

УзелРяр& УзелРяр::устАтр(кткст0 ид, const Ткст& text)
{
	if(!attr)
		attr.создай();
	attr->дайДобавь(ид) = text;
	return *this;
}

проц УзелРяр::устАтры(ВекторМап<Ткст, Ткст>&& a)
{
	if(a.дайСчёт() == 0)
		attr.очисть();
	else {
		if(!attr)
			attr.создай();
		*attr = pick(a);
	}
}

УзелРяр& УзелРяр::устАтр(кткст0 ид, цел i)
{
	устАтр(ид, какТкст(i));
	return *this;
}

проц УзелРяр::сожми()
{
	if(attr) {
		if(attr->дайСчёт() == 0)
			attr.очисть();
		else
			attr->сожми();
	}
	node.сожми();
}

УзелРяр::УзелРяр(const УзелРяр& n, цел)
{
	тип = n.тип;
	text = n.text;
	node = клонируй(n.node);
	if(n.attr) {
		attr.создай();
		*attr = клонируй(*n.attr);
	}
}

бул Ignore(ПарсерРяр& p, бцел style)
{
	if((XML_IGNORE_DECLS & style) && p.декл_ли() ||
	   (XML_IGNORE_PIS & style) && p.IsPI() ||
	   (XML_IGNORE_COMMENTS & style) && p.коммент_ли()) {
		p.пропусти();
		return true;
	}
	return false;
}

static УзелРяр sReadXmlNode(ПарсерРяр& p, ФильтрРазбораРяр *filter, бцел style)
{
	УзелРяр m;
	if(p.тэг_ли()) {
		Ткст tag = p.читайТэг();
		if(!filter || filter->сделайТэг(tag)) {
			m.создайТэг(tag);
			m.устАтры(p.PickAttrs());
			while(!p.стоп())
				if(!Ignore(p, style)) {
					УзелРяр n = sReadXmlNode(p, filter, style);
					if(n.дайТип() != XML_DOC) // tag was ignored
						m.добавь() = pick(n);
				}
			if(filter)
				filter->завершиТэг();
		}
		else
			p.пропустиКонец();
		return m;
	}
	if(p.IsPI()) {
		m.CreatePI(p.ReadPI());
		return m;
	}
	if(p.декл_ли()) {
		m.создайДекл(p.читайДекл());
		return m;
	}
	if(p.коммент_ли()) {
		m.создайКоммент(p.читайКоммент());
		return m;
	}
	if(p.текст_ли()) {
		m.создайТекст(p.читайТекст());
		m.сожми();
		return m;
	}
	p.читайТекст(); // skip empty text
	return m;
}

проц ФильтрРазбораРяр::завершиТэг() {}

УзелРяр разбериРЯР(ПарсерРяр& p, бцел style, ФильтрРазбораРяр *filter)
{
	УзелРяр r;
	while(!p.кф_ли())
		if(!Ignore(p, style)) {
			УзелРяр n = sReadXmlNode(p, filter, style);
			if(n.дайТип() != XML_DOC) // tag was ignored
				r.добавь() = pick(n);
		}
	return r;
}

УзелРяр разбериРЯР(ПарсерРяр& p, бцел style)
{
	return разбериРЯР(p, style, NULL);
}

УзелРяр разбериРЯР(кткст0 s, бцел style)
{
	ПарсерРяр p(s);
	return разбериРЯР(p, style);
}

УзелРяр разбериРЯР(Поток& in, бцел style)
{
	ПарсерРяр p(in);
	return разбериРЯР(p, style);
}

УзелРяр разбериФайлРЯР(кткст0 path, бцел style)
{
	ФайлВвод in(path);
	if(!in)
		throw ОшибкаРяр("Unable to open intput file!");
	return разбериРЯР(in, style);
}

УзелРяр разбериРЯР(ПарсерРяр& p, ФильтрРазбораРяр& filter, бцел style)
{
	return разбериРЯР(p, style, &filter);
}

УзелРяр разбериРЯР(кткст0 s, ФильтрРазбораРяр& filter, бцел style)
{
	ПарсерРяр p(s);
	return разбериРЯР(p, filter, style);
}

УзелРяр разбериРЯР(Поток& in, ФильтрРазбораРяр& filter, бцел style)
{
	ПарсерРяр p(in);
	return разбериРЯР(p, filter, style);
}

УзелРяр разбериФайлРЯР(кткст0 path, ФильтрРазбораРяр& filter, бцел style)
{
	ФайлВвод in(path);
	if(!in)
		throw ОшибкаРяр("Unable to open intput file!");
	return разбериРЯР(in, filter, style);
}

бул ShouldPreserve(const Ткст& s)
{
	if(*s == ' ' || *s == '\t' || *s == '\n')
		return true;
	кткст0 l = s.последний();
	if(*l == ' ' || *l == '\t' || *l == '\n')
		return true;
	l = s.стоп();
	for(кткст0 x = s; x < l; x++)
		if(*x == '\t' || *x == '\n' || *x == ' ' && x[1] == ' ')
			return true;
	return false;
}

static проц sAsXML(Поток& out, const УзелРяр& node, бцел style, const Ткст& indent)
{
	if(style & XML_HEADER) {
		Ткст h = XmlHeader();
		if(!(style & XML_PRETTY))
			h.обрежь(h.дайСчёт() - 2);
		out << h;
	}
	if(style & XML_DOCTYPE)
		for(цел i = 0; i < node.дайСчёт(); i++) {
			const УзелРяр& m = node.Узел(i);
			if(m.дайТип() == XML_TAG) {
				Ткст h = XmlDocType(m.дайТекст());
				h.обрежь(h.дайСчёт() - 2);
				out << h;
				break;
			}
		}
	style &= ~(XML_HEADER|XML_DOCTYPE);
	switch(node.дайТип()) {
	case XML_PI:
		out << indent << "<?" << node.дайТекст() << "?>\r\n";
		break;
	case XML_DECL:
		out << indent << "<!" << node.дайТекст() << ">\r\n";
		break;
	case XML_COMMENT:
		out << indent << "<!--" << node.дайТекст() << "-->\r\n";
		break;
	case XML_DOC:
		for(цел i = 0; i < node.дайСчёт(); i++)
			sAsXML(out, node.Узел(i), style, indent);
		break;
	case XML_TEXT:
		out << DeXml(node.дайТекст());
		break;
	case XML_TAG:
		ТэгРяр tag(node.дайТекст());
		for(цел i = 0; i < node.дайСчётАтров(); i++)
			tag(node.идАтра(i), node.Атр(i));
		if(node.дайСчёт()) {
			out << indent << tag.дайНачало();
			Ткст indent2 = (style & XML_PRETTY) && node[0].тэг_ли() && node[node.дайСчёт() - 1].тэг_ли() ?
			                 indent + '\t' : Ткст();
			for(цел i = 0; i < node.дайСчёт(); i++)
				sAsXML(out, node.Узел(i), style, indent2);
			if(indent2.дайСчёт())
				out << indent;
			out << tag.дайКонец();
		}
		else
			out << indent << tag();
	}
}

проц какРЯР(Поток& out, const УзелРяр& n, бцел style)
{
	sAsXML(out, n, style, style & XML_PRETTY ? "\r\n" : "");
}

Ткст какРЯР(const УзелРяр& n, бцел style)
{
	ТкстПоток ss;
	какРЯР(ss, n, style);
	return ss.дайРез();
}

бул какФайлРЯР(кткст0 path, const УзелРяр& n, бцел style)
{
	ФайлВывод out(path);
	if(!out)
		return false;
	какРЯР(out, n, style);
	out.закрой();
	return !out.ошибка_ли();
}

бул IgnoreXmlPaths::сделайТэг(const Ткст& ид)
{
	Ткст new_path;
	if(path.дайСчёт())
		new_path = path.верх();
	new_path << '/' << ид;
	if(list.найди(new_path) >= 0)
		return false;
	path.добавь(new_path);
	return true;
}

проц IgnoreXmlPaths::завершиТэг()
{
	path.сбрось();
}

IgnoreXmlPaths::IgnoreXmlPaths(кткст0 s)
{
	list = разбей(s, ';');
}

}
