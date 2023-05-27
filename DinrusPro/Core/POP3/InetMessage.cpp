#include "POP3.h"

#define LLOG(x)  // DLOG(x)

namespace ДинрусРНЦП {

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
	цел charset = набсимПоИмени(chs);
	return charset >= 0 ? вНабсим(CHARSET_DEFAULT, r, charset, '?') : r;
}

Ткст DecodeHeaderValue(const Ткст& s)
{
	Ткст r, p, q;
	цел pos = 0, part = 0, length = s.дайДлину();
	бул isq = false;
	while(pos < length) {
		if(s[pos] == '=' && s[pos + 1] == '?') {
			// Process q-encoded text.
			q.кат(s.середина(pos, 2));
			pos += 2;	
			while(pos < length) {
				if(s[pos] == '?' && s[pos + 1] == '=' && part == 2) {
					q.кат(s.середина(pos, 2));
					r.кат(QDecode(q));
					q.очисть();
					pos++;
					isq = true;
					part = 0;
					break;
				}
				else
				if(s[pos] == '?' && part < 2)
					part++;
				q.кат(s[pos]);
				pos++;
				if(pos == length && !q.пустой())
					r.кат(q);
			}
		}
		else {
			// Process plain text
			while(pos < length) {
				if(s[pos] == '=' && s[pos + 1] == '?') {
					pos--;
					break;
				}
				p.кат(s[pos], 1);
				pos++;				
				if(pos == length) 
					isq = false;
			}
			бул haschar = false;
			for(цел i = 0; i < p.дайДлину(); i++) 
				if(!пробел_ли(p[i])) {
				   haschar = true;
				   break;
				}
			if((isq && haschar) || !isq)
				r.кат(p);
			p.очисть();
		}
		pos++;
	}
	return r;	
}

Ткст sEncode(const Ткст& text)
{
	for(кткст0 q = text; *q; q++)
		if((ббайт)*q < ' ' || (ббайт)*q > 127) {
			Ткст txt = вНабсим(НАБСИМ_УТФ8, text);
			Ткст r = "=?UTF-8?Q?";
			for(кткст0 s = txt; *s; s++) {
				if((ббайт)*s < ' ' || (ббайт)*s > 127 || найдиарг(*s, '=', '?', ' ', ',', '_') >= 0)
					r << '=' << фмтЦелГексВерхРег((ббайт)*s, 2);
				else
					r.кат(*s);
			}
			r << "?=";
			return r;
		}
	return text;
}

проц FormatMessageHeader(Ткст& r, const ВекторМап<Ткст, Ткст>& hdr)
{
	for(цел i = 0; i < hdr.дайСчёт(); i++) {
		Ткст line;
		line << иницШапки(hdr.дайКлюч(i)) << ": ";
		Ткст h = sEncode(hdr[i]);
		for(кткст0 s = h; *s; s++) {
			if(line.дайСчёт() >= 80) {
				r << line << "\r\n";
				line = "\t";
			}
			line.кат(*s);
		}
		r << line << "\r\n";
	}
}

static проц sLn(Ткст& r)
{
	if(r.дайСчёт() == 0)
		return;
	if(r.дайСчёт() > 1) {
		кткст0 s = r.последний();
		if(s[-1] == '\r' && s[0] == '\n')
			return;
	}
	r.кат("\r\n");
}

проц InetMessage::PutBody(цел pi, Ткст& r, цел level) const
{
	if(level > 20) return; // Cycle protection
	FormatMessageHeader(r, part[pi].header);
	Ткст boundary;
	Ткст end_boundary;
	r << "\r\n";
	if(part[pi].IsMultipart(boundary, end_boundary)) {
		for(цел i = 0; i < part.дайСчёт(); i++)
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

бул InetMessage::ReadHeader(ВекторМап<Ткст, Ткст>& hdr, Поток& ss)
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
				hdr.верх().кат(s.середина(1));
		}
		else {
			цел q = s.найди(':');
			if(q >= 0)
				hdr.добавь(впроп(s.середина(0, q)), обрежьЛево(s.середина(q + 1)));
		}
	}
	for(цел i = 0; i < hdr.дайСчёт(); i++)
		hdr[i] = DecodeHeaderValue(hdr[i]);
	return true;
}

бул InetMessage::ReadHeader(const Ткст& s)
{
	LLOG("ReadHeader " << s.дайСчёт());
	part.очисть();
	ТкстПоток ss(s);
	return ReadHeader(part.добавь().header, ss);
}

бул InetMessage::Part::IsMultipart(Ткст& boundary, Ткст& end_boundary) const
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

бул InetMessage::ReadPart(Поток& ss, цел parent, цел level)
{
	LLOG("ReadPart parent: " << parent << ", level: " << level);

	if(level > 5 || part.дайСчёт() > 200) // Sanity limit
		return false;

	цел newparent = part.дайСчёт();

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
	бул end = false;
	while(!end) {
		Ткст body;
		бул next = false;
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

бул InetMessage::читай(const Ткст& s)
{
	part.очисть();
	ТкстПоток ss(s);
	return ReadPart(ss, Null, 0);
}

Ткст InetMessage::Part::Decode() const
{
	Ткст r = раскодируй(впроп(header.дай("content-transfer-encoding", "")),
	                  "quoted-printable", QPDecode(body),
	                  "base64", Base64Decode(body),
	                  body);
	цел cs = набсимПоИмени(MIMEHeader(впроп(header.дай("content-тип", Null)))["charset"]);
	if(cs >= 0)
		r = вНабсим(CHARSET_DEFAULT, r, cs, '?');
	return r;
}

бул MIMEHeader::Parse(кткст0 s)
{
	значение.очисть();
	param.очисть();
	СиПарсер p(s);
	try {
		СиПарсер p(s);
		кткст0 b = p.дайУк();
		while(!p.кф_ли() && !p.сим_ли(';'))
			p.пропустиТерм();
		значение = обрежьОба(Ткст(b, p.дайУк()));
		if(*значение == '\'' && *значение.последний() == '\'' || // mime тип can be quoted...
		   *значение == '\"' && *значение.последний() == '\"')
			значение = обрежьОба(значение.середина(1, значение.дайСчёт() - 2));
		while(!p.кф_ли()) {
			if(p.сим(';') && p.ид_ли()) {
				кткст0 b = p.дайУк();
				while(!p.кф_ли() && !p.сим_ли(';') && !p.сим_ли('='))
					p.пропустиТерм();
				Ткст ид = впроп(обрежьОба(Ткст(b, p.дайУк())));
				Ткст знач;
				if(p.сим('=')) {
					if(p.сим_ли('\''))
						знач = p.читайТкст('\'');
					else
					if(p.ткст_ли())
						знач = p.читайТкст();
					else {
						кткст0 b = p.дайУк();
						while(!p.кф_ли() && !p.сим_ли(';'))
							p.пропустиТерм();
						знач = обрежьОба(Ткст(b, p.дайУк()));
					}
				}
				param.добавь(ид, знач);
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
	for(цел i = 0; i < param.дайСчёт(); i++)
		r << "; " << param.дайКлюч(i) << '=' << какТкстСи(param[i]);
	return r;
}

Индекс<Ткст> ParseMessageIDs(const Ткст& s)
{
	Индекс<Ткст> ref;
	цел q = 0;
	for(;;) {
		q = s.найди('<', q);
		if(q < 0)
			break;
		цел w = s.найди('>', q);
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
	for(цел i = 0; i < ид.дайСчёт(); i++)
		if(ид[i].дайСчёт()) {
			if(r.дайСчёт())
				r << ' ';
			r << '<' << ид[i] << '>';
		}
	return r;
}

}
