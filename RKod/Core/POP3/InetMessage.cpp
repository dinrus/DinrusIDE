#include "POP3.h"

#define LLOG(x)  // DLOG(x)

namespace РНЦПДинрус {

Ткст QDecode(const Ткст& s) 
{
	Ткст r, begin, end, chs, enc, txt;
	// q-encoded text формат (RFC 2047):
	// =?(charset)?(encoding)?(encoded text)?=
	if(!SplitTo(s, '?', begin, chs, enc, txt, end))
		return s;
	if(begin != "=" || end != "=")
		return s;
	enc = взаг(enc);
	if(enc == "B")
		r = Base64Decode(txt);	
	else
	if(enc == "Q")
		r = QPDecode(txt, true);
	else
		return s;
	int charset = набсимПоИмени(chs);
	return charset >= 0 ? вНабсим(CHARSET_DEFAULT, r, charset, '?') : r;
}

Ткст DecodeHeaderValue(const Ткст& s)
{
	Ткст r, p, q;
	int pos = 0, part = 0, length = s.дайДлину();
	bool isq = false;
	while(pos < length) {
		if(s[pos] == '=' && s[pos + 1] == '?') {
			// Process q-encoded text.
			q.конкат(s.середина(pos, 2));
			pos += 2;	
			while(pos < length) {
				if(s[pos] == '?' && s[pos + 1] == '=' && part == 2) {
					q.конкат(s.середина(pos, 2));
					r.конкат(QDecode(q));
					q.очисть();
					pos++;
					isq = true;
					part = 0;
					break;
				}
				else
				if(s[pos] == '?' && part < 2)
					part++;
				q.конкат(s[pos]);
				pos++;
				if(pos == length && !q.пустой())
					r.конкат(q);
			}
		}
		else {
			// Process plain text
			while(pos < length) {
				if(s[pos] == '=' && s[pos + 1] == '?') {
					pos--;
					break;
				}
				p.конкат(s[pos], 1);
				pos++;				
				if(pos == length) 
					isq = false;
			}
			bool haschar = false;
			for(int i = 0; i < p.дайДлину(); i++) 
				if(!пробел_ли(p[i])) {
				   haschar = true;
				   break;
				}
			if((isq && haschar) || !isq)
				r.конкат(p);
			p.очисть();
		}
		pos++;
	}
	return r;	
}

Ткст sEncode(const Ткст& text)
{
	for(const char *q = text; *q; q++)
		if((byte)*q < ' ' || (byte)*q > 127) {
			Ткст txt = вНабсим(НАБСИМ_УТФ8, text);
			Ткст r = "=?UTF-8?Q?";
			for(const char *s = txt; *s; s++) {
				if((byte)*s < ' ' || (byte)*s > 127 || findarg(*s, '=', '?', ' ', ',', '_') >= 0)
					r << '=' << фмтЦелГексВерхРег((byte)*s, 2);
				else
					r.конкат(*s);
			}
			r << "?=";
			return r;
		}
	return text;
}

void FormatMessageHeader(Ткст& r, const ВекторМап<Ткст, Ткст>& hdr)
{
	for(int i = 0; i < hdr.дайСчёт(); i++) {
		Ткст line;
		line << иницШапки(hdr.дайКлюч(i)) << ": ";
		Ткст h = sEncode(hdr[i]);
		for(const char *s = h; *s; s++) {
			if(line.дайСчёт() >= 80) {
				r << line << "\r\n";
				line = "\t";
			}
			line.конкат(*s);
		}
		r << line << "\r\n";
	}
}

static void sLn(Ткст& r)
{
	if(r.дайСчёт() == 0)
		return;
	if(r.дайСчёт() > 1) {
		const char *s = r.последний();
		if(s[-1] == '\r' && s[0] == '\n')
			return;
	}
	r.конкат("\r\n");
}

void InetMessage::PutBody(int pi, Ткст& r, int level) const
{
	if(level > 20) return; // Cycle protection
	FormatMessageHeader(r, part[pi].header);
	Ткст boundary;
	Ткст end_boundary;
	r << "\r\n";
	if(part[pi].IsMultipart(boundary, end_boundary)) {
		for(int i = 0; i < part.дайСчёт(); i++)
			if(part[i].parent == pi) {
				sLn(r);
				r << boundary << "\r\n";
				PutBody(i, r, level + 1);
			}
		sLn(r);
		r << end_boundary << "\r\n" ;
	}
	else
		r << part[pi].body << "\r\n";
}

Ткст InetMessage::GetMessage() const
{
	Ткст r;
	if(part.дайСчёт())
		PutBody(0, r, 0);
	return r;
}

bool InetMessage::ReadHeader(ВекторМап<Ткст, Ткст>& hdr, Поток& ss)
{
	hdr.очисть();
	for(;;) {
		if(ss.кф_ли())
			return false;
		Ткст s = ss.дайСтроку();
		if(s.пустой())
			break;
		if(s[0] == ' ' || s[0] == '\t') {
			if(hdr.дайСчёт())
				hdr.верх().конкат(s.середина(1));
		}
		else {
			int q = s.найди(':');
			if(q >= 0)
				hdr.добавь(впроп(s.середина(0, q)), обрежьЛево(s.середина(q + 1)));
		}
	}
	for(int i = 0; i < hdr.дайСчёт(); i++)
		hdr[i] = DecodeHeaderValue(hdr[i]);
	return true;
}

bool InetMessage::ReadHeader(const Ткст& s)
{
	LLOG("ReadHeader " << s.дайСчёт());
	part.очисть();
	ТкстПоток ss(s);
	return ReadHeader(part.добавь().header, ss);
}

bool InetMessage::Part::IsMultipart(Ткст& boundary, Ткст& end_boundary) const
{
	MIMEHeader h(header.дай("content-тип", Ткст()));
	LLOG("content-тип: " << h);
	if(впроп(~h).начинаетсяС("multipart")) {
		boundary = "--" + h["boundary"];
		end_boundary = boundary + "--";
		return true;
	}
	return false;
}

bool InetMessage::ReadPart(Поток& ss, int parent, int level)
{
	LLOG("ReadPart parent: " << parent << ", level: " << level);

	if(level > 5 || part.дайСчёт() > 200) // Sanity limit
		return false;

	int newparent = part.дайСчёт();

	Part& p = part.добавь();
	p.parent = parent;

	if(!ReadHeader(p.header, ss))
		return false;

	Ткст boundary, end_boundary;
	if(!p.IsMultipart(boundary, end_boundary)) {
		p.body = загрузиПоток(ss);
		return true;
	}

	for(;;) {
		Ткст ln = ss.дайСтроку();
		if(ln == boundary)
			break;
		if(ss.кф_ли())
			return false;
	}
	bool end = false;
	while(!end) {
		Ткст body;
		bool next = false;
		for(;;) {
			if(ss.кф_ли())
				return false;
			Ткст ln = ss.дайСтроку();
			if(ln == boundary)
				break;
			if(ln == end_boundary) {
				end = true;
				break;
			}
			if(next)
				body << "\r\n";
			body << ln;
			next = true;
		}
		ТкстПоток nss(body);
		ReadPart(nss, newparent, level + 1);
	}
	return true;
}

bool InetMessage::читай(const Ткст& s)
{
	part.очисть();
	ТкстПоток ss(s);
	return ReadPart(ss, Null, 0);
}

Ткст InetMessage::Part::Decode() const
{
	Ткст r = decode(впроп(header.дай("content-transfer-encoding", "")),
	                  "quoted-printable", QPDecode(body),
	                  "base64", Base64Decode(body),
	                  body);
	int cs = набсимПоИмени(MIMEHeader(впроп(header.дай("content-тип", Null)))["charset"]);
	if(cs >= 0)
		r = вНабсим(CHARSET_DEFAULT, r, cs, '?');
	return r;
}

bool MIMEHeader::Parse(const char *s)
{
	значение.очисть();
	param.очисть();
	СиПарсер p(s);
	try {
		СиПарсер p(s);
		const char *b = p.дайУк();
		while(!p.кф_ли() && !p.сим_ли(';'))
			p.пропустиТерм();
		значение = обрежьОба(Ткст(b, p.дайУк()));
		if(*значение == '\'' && *значение.последний() == '\'' || // mime тип can be quoted...
		   *значение == '\"' && *значение.последний() == '\"')
			значение = обрежьОба(значение.середина(1, значение.дайСчёт() - 2));
		while(!p.кф_ли()) {
			if(p.сим(';') && p.ид_ли()) {
				const char *b = p.дайУк();
				while(!p.кф_ли() && !p.сим_ли(';') && !p.сим_ли('='))
					p.пропустиТерм();
				Ткст ид = впроп(обрежьОба(Ткст(b, p.дайУк())));
				Ткст val;
				if(p.сим('=')) {
					if(p.сим_ли('\''))
						val = p.читайТкст('\'');
					else
					if(p.ткст_ли())
						val = p.читайТкст();
					else {
						const char *b = p.дайУк();
						while(!p.кф_ли() && !p.сим_ли(';'))
							p.пропустиТерм();
						val = обрежьОба(Ткст(b, p.дайУк()));
					}
				}
				param.добавь(ид, val);
			}
			else
				p.пропустиТерм();
		}
	}
	catch(СиПарсер::Ошибка) {}
	return значение.дайСчёт();
}

Ткст MIMEHeader::вТкст() const
{
	Ткст r = значение;
	for(int i = 0; i < param.дайСчёт(); i++)
		r << "; " << param.дайКлюч(i) << '=' << какТкстСи(param[i]);
	return r;
}

Индекс<Ткст> ParseMessageIDs(const Ткст& s)
{
	Индекс<Ткст> ref;
	int q = 0;
	for(;;) {
		q = s.найди('<', q);
		if(q < 0)
			break;
		int w = s.найди('>', q);
		if(w < 0)
			break;
		ref.найдиДобавь(s.середина(q + 1, w - q - 1));
		q = w;
	}
	return ref;
}

Ткст FormatMessageIDs(const Индекс<Ткст>& ид)
{
	Ткст r;
	for(int i = 0; i < ид.дайСчёт(); i++)
		if(ид[i].дайСчёт()) {
			if(r.дайСчёт())
				r << ' ';
			r << '<' << ид[i] << '>';
		}
	return r;
}

}
