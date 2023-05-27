#include <DinrusPro/DinrusPro.h>

namespace ДинрусРНЦП {

static кткст0 s_www_month[] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

Ткст WwwFormat(Время tm)
{
	static кткст0 dayofweek[] =
	{ "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
	return Ткст().кат()
		<< dayofweek[деньНедели(tm)] << ", "
		<< (цел)tm.day << ' ' << s_www_month[tm.month - 1]
		<< ' ' << (цел)tm.year
		<< ' ' << спринтф("%2d:%02d:%02d " + дайТкстЧПояса(), tm.hour, tm.minute, tm.second);
}

цел FindMonth(const Ткст& m)
{
	for(цел i = 0; i < 12; i++)
		if(s_www_month[i] == m)
			return i;
	return -1;
}

бул ScanWwwTime(кткст0 s, Время& tm)
{
	СиПарсер p(s);
	try {
		if(p.ид_ли()) { // пропусти day of week
			p.пропустиТерм();
			p.сим(',');
		}
		tm.day = p.читайЦел(1, 31);
		цел n = FindMonth(p.читайИд()) + 1;
		if(n < 1 || n > 12)
			return false;
		tm.month = n;
		tm.year = p.читайЦел(1, 4000);
		if(tm.year < 50)
			tm.year += 2000;
		else
		if(tm.year < 100)
			tm.year += 1900;
		tm.hour = p.читайЦел(0, 23);
		p.передайСим(':');
		tm.minute = p.читайЦел(0, 59);
		if(p.сим(':'))
		   tm.second = p.читайЦел(0, 59);
		tm += 60 * (дайЧПояс() - сканЧПояс(p.дайУк()));
	}
	catch(СиПарсер::Ошибка) {
		return false;
	}
	return tm.пригоден();
}

Время ScanWwwTime(кткст0 s)
{
	Время tm;
	if(ScanWwwTime(s, tm))
		return tm;
	return Null;
}

Ткст MIMECharsetName(ббайт charset)
{
	if(charset == CHARSET_DEFAULT)
		charset = дайДефНабСим();
	switch(charset) {
	case CHARSET_ISO8859_1:  return "ISO-8859-1";
	case CHARSET_ISO8859_2:  return "ISO-8859-2";
	case CHARSET_ISO8859_3:  return "ISO-8859-3";
	case CHARSET_ISO8859_4:  return "ISO-8859-4";
	case CHARSET_ISO8859_5:  return "ISO-8859-5";
	case CHARSET_ISO8859_6:  return "ISO-8859-6";
	case CHARSET_ISO8859_7:  return "ISO-8859-7";
	case CHARSET_ISO8859_8:  return "ISO-8859-8";
	case CHARSET_ISO8859_9:  return "ISO-8859-9";
	case CHARSET_ISO8859_10: return "ISO-8859-10";
	case CHARSET_ISO8859_13: return "ISO-8859-13";
	case CHARSET_ISO8859_14: return "ISO-8859-14";
	case CHARSET_ISO8859_15: return "ISO-8859-15";
	case CHARSET_ISO8859_16: return "ISO-8859-16";
	case CHARSET_WIN1250:    return "windows-1250";
	case CHARSET_WIN1251:    return "windows-1251";
	case CHARSET_WIN1252:    return "windows-1252";
	case CHARSET_WIN1253:    return "windows-1253";
	case CHARSET_WIN1254:    return "windows-1254";
	case CHARSET_WIN1255:    return "windows-1255";
	case CHARSET_WIN1256:    return "windows-1256";
	case CHARSET_WIN1257:    return "windows-1257";
	case CHARSET_WIN1258:    return "windows-1258";
//	case CHARSET_KOI8_R:
//	case CHARSET_CP852:
//	case CHARSET_MJK:
	case НАБСИМ_ВАСКИ:    return "us-ascii";
	case НАБСИМ_УТФ8:       return "UTF-8";
//	case НАБСИМ_ЮНИКОД:
	default:                 return Null;
	}
}

static const char hex_digits[] = "0123456789ABCDEF";

Ткст UrlEncode(кткст0 p, кткст0 e)
{
	ТкстБуф out;
	out.резервируй((цел)(e - p));
	for(; p < e; p++)
	{
		кткст0 b = p;
		while(p < e && (ббайт)*p > ' ' && (ббайт)*p < 127
		      && (алчис_ли(*p) || *p == '.' || *p == '-' || *p == '_'))
			p++;
		if(p > b)
			out.кат(b, цел(p - b));
		if(p >= e)
			break;
		if(*p == ' ')
			out << '+';
		else
			out << '%' << hex_digits[(*p >> 4) & 15] << hex_digits[*p & 15];
	}
	return Ткст(out);
}

Ткст UrlEncode(кткст0 s, цел len)
{
	return UrlEncode(s, s + len);
}

Ткст UrlEncode(const Ткст& s)
{
	return UrlEncode(~s, s.дайДлину());
}

Ткст UrlDecode(кткст0 b, кткст0 e)
{
	ТкстБуф out;
	ббайт d1, d2, d3, d4;
	for(кткст0 p = b; p < e; p++)
		if(*p == '+')
			out.кат(' ');
		else if(*p == '%' && (d1 = ctoi(p[1])) < 16 && (d2 = ctoi(p[2])) < 16) {
			out.кат(d1 * 16 + d2);
			p += 2;
		}
		else if(*p == '%' && (p[1] == 'u' || p[1] == 'U')
		&& (d1 = ctoi(p[2])) < 16 && (d2 = ctoi(p[3])) < 16
		&& (d3 = ctoi(p[4])) < 16 && (d4 = ctoi(p[5])) < 16) {
			out.кат(ШТкст((d1 << 12) | (d2 << 8) | (d3 << 4) | d4, 1).вТкст());
			p += 5;
		}
		else
			out.кат(*p);
	return Ткст(out);
}

Ткст UrlDecode(кткст0 s, цел len)
{
	return UrlDecode(s, s + len);
}

Ткст UrlDecode(const Ткст& s)
{
	return UrlDecode(~s, s.дайДлину());
}
          
Ткст QPEncode(const char* s)
{
	ТкстБуф r;
	цел len = 0;
	const цел  limit  = 70;
	while(*s) {
		if(s[0] >= 33 && s[0] <= 126 && s[0] != '=' && s[0] != '_') {
			r.кат(s[0]);
			len++;
		}
		else
		if(s[0] == '\r')
			;
		else
		if(s[0] == '\n') {
			r.кат("\r\n");
			len = 0;
		}
		else // Encode HT or SP only if they are at the end of line (before CRLF or EOF)
		if((s[0] == ' ' || s[0] == '\t') &&
		   (s[1] && s[1] != '\n') &&
		   (s[1] != '\r' || (s[1] == '\r' && s[2] && s[2] != '\n'))) {
			r.кат(s[0]);
			len++;
		}
		else {
			static const char hex[] = "0123456789ABCDEF";
			r.кат('=');
			r.кат(hex[(s[0] >> 4) & 15]);
			r.кат(hex[s[0] & 15]);
			len += 3;
		}
		if(len > limit) {
			// Soft-break.
			r.кат('=');
			r.кат("\r\n");
			len = 0;
		}
		s++;
	}
	return Ткст(r);
}
    
Ткст QPDecode(кткст0 s, бул underscore_to_space)
{
	ТкстБуф r;
	while(*s) {
		цел c = *s++;
		if(c == '=') {
			цел c1 = '0', c2 = '0';
			if(*s)
				c1 = *s++;
			if(*s)
				c2 = *s++;
			if(цифраикс_ли(c1) && цифраикс_ли(c2))
				r.кат((ctoi(c1) << 4) | ctoi(c2));
		}
		else
		if(underscore_to_space && c == '_')
			r.кат(' ');
		else
			r.кат(c);
	}
	return Ткст(r);
}

Ткст Base64Encode(кткст0 _b, кткст0 _e)
{
	static const char encoder[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789+/";
	const ббайт *b = (ббайт *)_b; // avoid left shift of negative значение sanitizer issue
	const ббайт *e = (ббайт *)_e;
	if(b == e)
		return Null;
	цел out = (цел(e - b) + 2) / 3 * 4;
	цел rem = цел(e - b) % 3;
	e -= rem;
	ТкстБуф s(out);
	char *p = s;
	while(b < e)
	{
		p[0] = encoder[(b[0] >> 2) & 0x3F];
		p[1] = encoder[((b[0] << 4) & 0x30) | ((b[1] >> 4) & 0x0F)];
		p[2] = encoder[((b[1] << 2) & 0x3C) | ((b[2] >> 6) & 0x03)];
		p[3] = encoder[b[2] & 0x3F];
		b += 3;
		p += 4;
	}
	if(rem == 1)
	{
		p[0] = encoder[(b[0] >> 2) & 0x3F];
		p[1] = encoder[(b[0] << 4) & 0x30];
		p[2] = p[3] = '=';
	}
	else if(rem == 2)
	{
		p[0] = encoder[(b[0] >> 2) & 0x3F];
		p[1] = encoder[((b[0] << 4) & 0x30) | ((b[1] >> 4) & 0x0F)];
		p[2] = encoder[(b[1] << 2) & 0x3C];
		p[3] = '=';
	}
	return Ткст(s);
}

Ткст Base64Encode(кткст0 b, цел len)
{
	return Base64Encode(b, b + len);
}

Ткст Base64Encode(const Ткст& данные)
{
	return Base64Encode(~данные, данные.дайСчёт());
}

Ткст Base64Decode(кткст0 b, кткст0 e)
{
	static ббайт dec64[] =
	{
/* 0x */0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
/* 1x */0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
/* 2x */0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E, 0xFF, 0xFF, 0xFF, 0x3F,
/* 3x */0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
/* 4x */0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
/* 5x */0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
/* 6x */0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
/* 7x */0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
/* 8x */0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
/* 9x */0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
/* Ax */0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
/* Bx */0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
/* Cx */0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
/* Dx */0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
/* Ex */0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
/* Fx */0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	};
	ТкстБуф out;
	ббайт c[4];
	цел pos = 0;
	for(; b < e; b++)
		if((ббайт)*b > ' ') {
			ббайт ch = dec64[(ббайт)*b];
			if(ch & 0xC0)
				break;
			c[pos++] = ch;
			if(pos == 4) {
				out.кат((c[0] << 2) | (c[1] >> 4));
				out.кат((c[1] << 4) | (c[2] >> 2));
				out.кат((c[2] << 6) | (c[3] >> 0));
				pos = 0;
			}
		}
	if(pos >= 2) {
		out.кат((c[0] << 2) | (c[1] >> 4));
		if(pos >= 3) {
			out.кат((c[1] << 4) | (c[2] >> 2));
			if(pos >= 4)
				out.кат((c[2] << 6) | (c[3] >> 0));
		}
	}
	return Ткст(out);
}

Ткст Base64Decode(кткст0 s, цел len)
{
	return Base64Decode(s, s + len);
}

Ткст Base64Decode(const Ткст& данные)
{
	return Base64Decode(~данные, данные.дайДлину());
}

Ткст DeHtml(кткст0 s)
{
	Ткст result;
	while(*s) {
		if(*s == 31)
			result.кат("&nbsp;");
		else
		if(*s == '<')
			result.кат("&lt;");
		else
		if(*s == '>')
			result.кат("&gt;");
		else
		if(*s == '&')
			result.кат("&amp;");
		else
		if(*s == '\"')
			result.кат("&quot;");
		else
		if((ббайт)*s < ' ')
			result.кат(фмт("&#%d;", (ббайт)*s));
		else
			result.кат(*s);
		s++;
	}
	return result;
}

проц HMAC_SHA1(const ббайт *text, цел text_len, const ббайт *ключ, цел key_len, ббайт *digest)
{
	unsigned char k_ipad[65];
	unsigned char k_opad[65];
	unsigned char tk[20];
	цел i;
	
	if(key_len > 64) {
		SHA1(tk, ключ, key_len);
		ключ = tk;
		key_len = 20;
	}

	memset( k_ipad, 0, sizeof(k_ipad));
	memset( k_opad, 0, sizeof(k_opad));
	memcpy( k_ipad, ключ, key_len);
	memcpy( k_opad, ключ, key_len);
	
	for(i = 0; i < 64; i++) {
		k_ipad[i] ^= 0x36;
		k_opad[i] ^= 0x5c;
	}
	
	Sha1Stream sha1;
	sha1.помести(k_ipad, 64);
	sha1.помести(text, text_len);
	sha1.финиш(digest);
	
	sha1.нов();
	sha1.помести(k_opad, 64);
	sha1.помести(digest, 20);
	sha1.финиш(digest);
}

Ткст HMAC_SHA1(const Ткст& text, const Ткст& ключ)
{
	ббайт digest[20];
	HMAC_SHA1(text, text.дайСчёт(), ключ, ключ.дайСчёт(), digest);
	return Ткст(digest, 20);
}

Ткст HMAC_SHA1_Hex(const Ткст& text, const Ткст& ключ)
{
	return гексТкст(HMAC_SHA1(text, ключ));
}

проц HttpCookie::очисть()
{
	ид.очисть();
	значение.очисть();
	domain.очисть();
	path.очисть();
	raw.очисть();
}

бул HttpCookie::Parse(const Ткст& cookie)
{
	очисть();
	кткст0 s = cookie;
	raw = cookie;
	бул first = true;
	while(s && *s) {
		while(*s == ' ')
			s++;
		кткст0 e = strchr(s, ';');
		if(!e)
			e = s + strlen(s);
		кткст0 eq = strchr(s, '=');
		if(eq && eq < e) {
			Ткст h = Ткст(s, eq);
			if(first) {
				ид = h;
				значение = Ткст(eq + 1, e);
				first = false;
			}
			else {
				h = впроп(h);
				if(h == "domain")
					domain = Ткст(eq + 1, e);
				if(h == "path")
					path = Ткст(eq + 1, e);
			}
		}
		if(*e == 0)
			break;
		s = e + 1;
	}
	return !first;
}

проц HttpHeader::очисть()
{
	first_line.очисть();
	fields.очисть();
	cookies.очисть();
	f1 = f2 = f3 = Null;
	scgi = false;
}

бул  HttpHeader::HasContentLength() const
{
	return !пусто_ли((*this)["content-length"]);
}

дол HttpHeader::GetContentLength() const
{
	return Nvl(сканЦел64((*this)["content-length"]), (дол)0);
}

проц HttpHeader::добавь(const Ткст& id_, const Ткст& значение)
{
	Ткст ид = впроп(id_);
	fields.добавь(ид, значение);
	if(ид == "set-cookie") {
		HttpCookie c;
		if(c.Parse(значение))
			cookies.добавь(c.ид, c);
	}
}

бул HttpHeader::ParseAdd(const Ткст& hdrs)
{
	ТкстПоток ss(hdrs);
	first_line = ss.дайСтроку();

	while(!ss.кф_ли()) {
		Ткст s = ss.дайСтроку();
		if(s.пустой()) break;
		цел q = s.найди(':');
		if(q >= 0)
			добавь(впроп(s.середина(0, q)), обрежьЛево(s.середина(q + 1)));
	}

	кткст0 s = first_line;
	if((ббайт)*s <= ' ')
		return false;
	while(*s != ' ' && *s)
		f1.кат(*s++);
	while(*s == ' ')
		s++;
	if(!*s)
		return false;
	while(*s != ' ' && *s)
		f2.кат(*s++);
	while(*s == ' ')
		s++;
	f3 = s;

	return true;
}

бул HttpHeader::Parse(const Ткст& hdrs)
{
	очисть();
	return ParseAdd(hdrs);
}

цел CharFilterScgiHttp(цел c)
{
	return c == '_' ? '-' : c;
}

бул HttpHeader::ParseSCGI(const Ткст& scgi_hdr)
{
	очисть();
	scgi = true;
	Ткст ключ, uri, qs;
	кткст0 b = scgi_hdr;
	кткст0 e = scgi_hdr.стоп();
	дол content_length = Null;
	for(кткст0 s = scgi_hdr; s < e; s++) {
		if(*s == '\0') {
			Ткст h(b, s);
			b = s + 1;
			if(ключ.дайСчёт()) {
				if(ключ.начинаетсяС("http_"))
					добавь(фильтруй(ключ.середина(5), CharFilterScgiHttp), h);
				if(ключ == "content_length")
					content_length = сканЦел64(h);
				if(ключ == "request_method")
					f1 = h;
				if(ключ == "request_uri")
					uri = h;
				if(ключ == "query_string")
					qs = h;
				if(ключ == "server_protocol")
					f3 = h;
				ключ.очисть();
			}
			else
				ключ = впроп(h);
		}
	}
	f2 = uri + qs;
	first_line = f1 + ' ' + f2 + ' ' + f3;
	if(!пусто_ли(content_length) && content_length && fields.найди("content-length") < 0)
		fields.добавь("content-length", какТкст(content_length));
	return true;
}

бул HttpHeader::читай(TcpSocket& socket)
{
	очисть();
	Ткст h;
	if(цифра_ли(socket.подбери())) {
		цел len = 0;
		while(цифра_ли(socket.подбери()))
			len = 10 * len + socket.дай() - '0';
		if(socket.дай() != ':' || len < 0 || len > 10000000)
			return false;
		h = socket.дайВсе(len);
		if(socket.дай() != ',')
			return false;
		return ParseSCGI(h);
	}
	h = socket.дайСтроку();
	if(h.проц_ли())
		return false;
	h << "\r\n";
	for(;;) {
		Ткст s = socket.дайСтроку();
		if(s.проц_ли())
			return false;
		if(s.пустой()) break;
		h << s << "\r\n";
	}
	return Parse(h);
}

цел HttpHeader::дайКод() const
{
	return сканЦел(f2);
}

бул HttpHeader::Request(Ткст& method, Ткст& uri, Ткст& версия) const
{
	method = GetMethod();
	uri = GetURI();
	версия = дайВерсию();
	return true;
}

Ткст HttpHeader::GetCookie(кткст0 ид) const
{
	цел q = cookies.найди(ид);
	return q < 0 ? Ткст() : cookies[q].значение;
}

бул HttpHeader::Response(Ткст& protocol, цел& код_, Ткст& reason) const
{
	protocol = GetProtocol();
	код_ = дайКод();
	reason = GetReason();
	return !пусто_ли(код_);
}

бул HttpResponse(TcpSocket& socket, бул scgi, цел код_, кткст0 phrase,
                  кткст0 content_type, const Ткст& данные, кткст0 server,
                  бул gzip)
{
	Ткст r;
	r << (scgi ? "Статус: " : "HTTP/1.1 ") << код_ << ' ' << phrase << "\r\n"
		"Дата: " <<  WwwFormat(GetUtcTime()) << "\r\n"
		"Server: " << (server ? server : "U++ based server") << "\r\n"
		"Connection: close\r\n";
	if(данные.дайСчёт())
		r << "Content-длина: " << данные.дайСчёт() << "\r\n";
	if(content_type)
		r << "Content-Type: " << content_type << "\r\n";
	if(gzip)
		r << "Content-Encoding: gzip\r\n";
	r << "\r\n";
	if(!socket.PutAll(r))
		return false;
	return данные.дайСчёт() == 0 || socket.PutAll(данные);
}

Ткст UrlInfo::operator[](кткст0 ид) const
{
	return parameters.дай(ид, Ткст());
}

const Вектор<Ткст>& UrlInfo::дайМассив(кткст0 ид) const
{
	static Вектор<Ткст> empty;
	return array_parameters.дай(ид, empty);
}

проц UrlInfo::Parse(const Ткст& url_)
{
	очисть();

	url = url_;
	Ткст h = url;
	цел q = h.найдирек('#');
	if(q >= 0) {
		fragment = UrlDecode(h.середина(q + 1));
		h.обрежь(q);
	}
	q = h.найдирек('?');
	if(q >= 0) {
		query = UrlDecode(h.середина(q + 1));
		h.обрежь(q);
	}
	q = h.найди("://");
	if(q >= 0) {
		scheme = UrlDecode(h.середина(0, q));
		h = h.середина(q + 3);
	}
	else {
		цел q = 0;
		while(h[q] == '/')
			q++;
		h.удали(0, q);
	}
		
	q = h.найди('@');
	if(q >= 0) {
		username = h.середина(0, q);
		h = h.середина(q + 1);
		q = username.найди(':');
		if(q >= 0) {
			password = username.середина(q + 1);
			username.обрежь(q);
		}
	}
	q = h.найди('/');
	if(q >= 0) {
		path = UrlDecode(h.середина(q));
		h.обрежь(q);
	}
	q = h.найди(':');
	if(q >= 0) {
		port = UrlDecode(h.середина(q + 1));
		h.обрежь(q);
	}
	host = h;
	
	кткст0 p = query;
	while(*p) {
		кткст0 last = p;
		while(*p && *p != '=' && *p != '&')
			p++;
		Ткст ключ = UrlDecode(last, p);
		if(*p == '=')
			p++;
		last = p;
		while(*p && *p != '&')
			p++;
		if(*ключ != '.' && *ключ != '@') {
			Ткст знач = UrlDecode(last, p);
			if(ключ.заканчиваетсяНа("[]"))
				array_parameters.дайДобавь(ключ.середина(0, ключ.дайСчёт() - 2)) << знач;
			else
				parameters.дайДобавь(ключ) = UrlDecode(last, p);
		}
		if(*p)
			p++;
	}
}

}
