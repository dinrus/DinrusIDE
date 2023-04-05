#include "SMTP.h"

namespace РНЦПДинрус {

namespace Ини {
	INI_BOOL(Smtp_Trace, false, "Activates HTTP requests tracing")
	INI_BOOL(Smtp_TraceBody, false, "Activates HTTP requests body tracing")
	INI_BOOL(Smtp_CompressLog, false, "Activates log compression (removes long hex/encode64-like данные)")
};

#define LLOG(x)      do { if(Ини::Smtp_Trace) { if(Ини::Smtp_CompressLog) RLOG(сожмиЛог(Ткст().конкат() << x)); else RLOG(x); } } while(0)
#define LLOGB(x)      do { if(Ини::Smtp_TraceBody) { if(Ини::Smtp_CompressLog) RLOG(сожмиЛог(Ткст().конкат() << x)); else RLOG(x); } } while(0)

void Smtp::Trace(bool b)
{
    Ини::Smtp_Trace = b;
}

void Smtp::TraceBody(bool b)
{
    Ини::Smtp_TraceBody = b;
}

static Ткст GetDelimiter(const char *b, const char *e, Ткст init)
{
	static const char delimiters[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdef"
		"ghijklmnopqrstuvxyz()+/:?0123456"
		"789abcdefghijklmnopqrstuvwxyzABC"
		"DEFGHIJKLMNOPQRSTUVWXYZ012345678";

	Ткст out = init;
	if(b == e)
		return out;
	if(пусто_ли(out))
		out.конкат(delimiters[*b++ & 0x7F]);
	int l = out.дайДлину();
	for(; b != e; b++)
	{
		b = (const char *)memchr(b, *out, e - b);
		if(!b || e - b < l)
			return out;
		if(!memcmp(b, out, l))
		{
			if(e - b == l)
				return out + '/';
			out.конкат(delimiters[b[l] & 0x7F]);
		}
	}
	return out;
}

static Ткст GetDelimiter(Ткст s, Ткст init)
{
	return GetDelimiter(s.старт(), s.стоп(), init);
}

Ткст Smtp::GetDomainName()
{
	Ткст org;
	auto pos = sender.найди('@');
	if(pos >= 0) {
		auto start = ++pos, len = sender.дайДлину();
		while(pos < len && sender[pos] != '>')
			pos++;
		org = sender.середина(start, pos - start);
	}
	else org << TcpSocket::GetHostName();
	return org;
}

МапЗнач Smtp::GetExtensions()
{
	МапЗнач features;
	if(!smtp_msg.пустой()) {
		ТкстПоток ss(smtp_msg);
		ss.дайСтроку();
		while(!ss.кф_ли()) {
			// дай smtp service extensions.
			auto e = нормализуйПробелы(обрежьОба(ss.дайСтроку().середина(4)));
			Вектор<Ткст> v = разбей(e, ' ');
			features.дайДобавь(впроп(v[0]));
			for(auto i = 1; i < v.дайСчёт(); i++)
				features(v[0]) << v[i];
		}
	}
	return pick(features);
}

int Smtp::GetSmtpCode(const Ткст& s)
{
	if(s.проц_ли() || s.дайДлину() < 3 || !цифра_ли(s[0]) || !цифра_ли(s[1]) || !цифра_ли(s[2]))
		return -1;
	return тктЦел(s.середина(0, 3));
}

void Smtp::SetSender()
{
	// Specify the e-mail address of the sender.
	SendRecvOK("MAIL FROM:<" + sender + ">");
}

void Smtp::SetRecipients()
{
	// A single e-mail can be sent to multiple recipients.
	for(auto& rcp : to)
		SendRecv("RCPT TO:<" + rcp + ">");
}

void Smtp::SendRecv(const Ткст& s)
{
	// We need to check the control connection.
	if(!открыт())
		throw Искл("Socket is not open.");

	// Send request.
	if(!s.пустой())
		SendData(s + "\r\n");

	// Receive response.
	// Response can be "multiline".
	smtp_msg = дайСтроку();
	smtp_code = GetSmtpCode(smtp_msg);
	if(smtp_code == -1)
		throw Искл("Recv failed. " << GetErrorDesc());
	LLOG("<< " << smtp_msg);
	smtp_msg.конкат('\n');
	if(smtp_msg[3] && smtp_msg[3] == '-') {
		for(;;) {
			auto line = дайСтроку();
			if(line.проц_ли()) {
				throw Искл("Recv failed: " << GetErrorDesc());
			}
			auto end_code = GetSmtpCode(line);
			LLOG("<< " << line);
			smtp_msg.конкат(line);
			smtp_msg.конкат('\n');
			if(smtp_code == end_code && line[3] && line[3] == ' ')
				break;
		}
	}
}

void Smtp::SendRecvOK(const Ткст& s)
{
	SendRecv(s);
	if(!ReplyIsSuccess())
		throw Искл(smtp_msg);
}

bool Smtp::SendHello()
{
	auto org = GetDomainName();
	// Try EHLO command first.
	// EHLO command may return available service extensions.
	SendRecv("EHLO " << org);
	if(!ReplyIsSuccess()) {
		// Fall back to original client greeting if EHLO fails.
		SendRecvOK("HELO " << org);
		return false;
	}
	return true;
}

void Smtp::StartTls()
{
	SendRecv("STARTTLS");
	if(!ReplyIsSuccess() || !StartSSL())
		throw Искл("Unable to init TLS session.");
	LLOG("++ STARTTLS successful.");
}

void Smtp::Authenticate()
{
	if(!пусто_ли(auth_user)) {
		SendRecv("AUTH LOGIN");
		while(ReplyIsPending()){
			auto param = Base64Decode(smtp_msg.дайОбх(4), smtp_msg.стоп());
			if(param == "Username:")
				SendRecv(Base64Encode(auth_user));
			else
			if(param == "Password:")
				SendRecv(Base64Encode(auth_pwd));
		}
		if(!ReplyIsSuccess())
			throw Искл(smtp_msg);
	}
}

void Smtp::Quit()
{
	SendRecv("QUIT");
}

void Smtp::SendMail(const Ткст& msg_)
{
	SetSender();
	SetRecipients();

	// Now send the actual e-mail.
	SendRecv("DATA");
	if(ReplyIsPending()) {
		Ткст msg = msg_;
		if(msg.пустой())
			msg = GetMessage(true);
		
		SendRecv(msg + ".");
		if(ReplyIsSuccess())
			return;
	}
	throw Искл(smtp_msg);
}

void Smtp::SendData(const Ткст &s)
{
	if(Ини::Smtp_TraceBody)
		LLOG(">> " << s);
	else
		LLOG(">> [Smtp send body: " << s.дайСчётСим() << " bytes]");
	if(!PutAll(s))
		throw Искл("Send failed. " << GetErrorDesc());
}


//////////////////////////////////////////////////////////////////////
// Smtp::

static const char default_mime[] = "application/octet-stream";

Ткст Smtp::Encode(const Ткст& text)
{
	Ткст txt = вНабсим(НАБСИМ_УТФ8, text);
	Ткст r = "=?UTF-8?Q?";
	for(const char *s = txt; *s; s++) {
		if((byte)*s < ' ' || (byte)*s > 127 || *s == '=' || *s == '?' || *s == ' ')
			r << '=' << фмтЦелГексВерхРег((byte)*s, 2);
		else
			r.конкат(*s);
	}
	r << "?=";
	return r;
}

Smtp& Smtp::To(const Ткст& t, const Ткст& имя, AS a)
{
	to.добавь(t);
	to_name.добавь(имя);
	as.добавь(a);
	return *this;
}

Smtp& Smtp::Subject(const Ткст& s)
{
	subject = s;
	return *this;
}

Smtp& Smtp::ReplyTo(const Ткст& r, const Ткст& имя)
{
	reply_to = r;
	reply_to_name = имя;
	return *this;
}

Smtp& Smtp::From(const Ткст& f, const Ткст& имя, const Ткст& s)
{
	from = f;
	from_name = имя;
	sender = Nvl(s, f);
	return *this;
}

Smtp& Smtp::AttachFile(const char *имяф, const char *mime)
{
	Attachment& attach = attachments.добавь();
	attach.имя = дайПозИмяф(имяф);
	attach.mime = (mime ? mime : default_mime);
	attach.file = имяф;
	return *this;
}

Smtp& Smtp::прикрепи(const char *имя, const Ткст& данные, const char *mime)
{
	Attachment& attach = attachments.добавь();
	attach.имя = имя;
	attach.mime = (mime ? mime : default_mime);
	attach.данные = данные;
	return *this;
}

Ткст Smtp::FormatAddr(const Ткст& addr, const Ткст& имя)
{
	Ткст r;
	if(имя.дайСчёт())
		r << "\"" << Encode(имя) << "\" ";
	r << '<' << addr << '>';
	return r;
}

Ткст Smtp::GetMessageID()
{
	int q = sender.найди('@');
	return message_id + (q >= 0 ? sender.середина(q) : "@unknown_host.org");
}

Ткст Smtp::GetMessage(bool chunks)
{
	Ткст delimiter = "?";
	for(int i = 0; i < body.дайСчёт(); i++)
		delimiter = GetDelimiter(body[i], delimiter);
	bool alter = body.дайСчёт() > 1;
	bool multi = !attachments.пустой();

	Ткст msg;
	if(!no_header) { // generate message header
		if (sender != from) msg << "Sender: " << sender << "\r\n";
		msg << "From: " << FormatAddr(from, from_name) << "\r\n";
		static const AS as_list[] = { TO, CC, BCC };
		static const char *as_name[] = { "To", "CC", "BCC" };
		for(int a = 0; a < __countof(as_list); a++)
		{
			int pos = 0;
			for(int i = 0; i < as.дайСчёт(); i++)
				if(as[i] == as_list[a])
				{
					if(pos && pos + to[i].дайДлину() >= 70)
					{
						msg << "\r\n     ";
						pos = 5;
					}
					else if(pos)
					{
						msg << ", ";
						pos += 2;
					}
					else
					{
						msg << as_name[a] << ": ";
						pos = (int)strlen(as_name[a]) + 2;
					}
					msg << FormatAddr(to[i], to_name[i]);
				}
			if(pos)
				msg << "\r\n";
		}
		if(!пусто_ли(subject))
			msg << "Subject: " << Encode(subject) << "\r\n";
		if(!пусто_ли(reply_to))
			msg << "Reply-To: " << FormatAddr(reply_to, reply_to_name) << "\r\n";
		msg << "Message-ИД: <" << GetMessageID() << ">\r\n";
		if(!пусто_ли(time_sent)) {
			static const char *dayofweek[] =
			{ "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
			static const char *month[] =
			{ "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
			msg << "Дата: "
				<< dayofweek[деньНедели(time_sent)] << ", "
				<< (int)time_sent.day << ' ' << month[time_sent.month - 1] << ' ' << (int)time_sent.year
				<< ' ' << спринтф("%2d:%02d:%02d " + дайТкстЧПояса(),
				                  time_sent.hour, time_sent.minute, time_sent.second)
				<< "\r\n";
		}
		if(multi || alter)
			msg << "Content-Type: multipart/" << (alter ? "alternative" : "mixed")
				<< "; boundary=\"" << delimiter << "\"\r\n"
				"\r\n";
		msg << add_header;
	}

	for(int i = 0; i < body.дайСчёт(); i++) {
		Ткст t = body[i], m = mime[i];
		if(!no_header) {
			if(multi || alter)
				msg << "--" << delimiter << "\r\n";
			if(пусто_ли(m))
				m << "text/plain; charset=\"" << MIMECharsetName(CHARSET_DEFAULT) << "\"";
			msg << "Content-Type: " << m << "\r\n"
			"Content-Transfer-Encoding: quoted-printable\r\n";
		}
		if(!no_header_sep)
			msg << "\r\n";
		bool begin = true;
		for(const char *p = t.старт(), *e = t.стоп(); p != e; p++)
			if(*p >= 33 && *p <= 126 && *p != '=' && (*p != '.' || !begin)) {
				msg.конкат(*p);
				begin = false;
			}
			else if(*p == '.' && begin) {
				msg.конкат("..");
				begin = false;
			}
			else if(*p == ' ' && p + 1 != e && p[1] != '\r' && p[1] != '\n') {
				msg.конкат(' ');
				begin = false;
			}
			else if(*p == '\r')
				;
			else if(*p == '\n') {
				msg.конкат("\r\n");
				begin = true;
			}
			else {
				static const char hex[] = "0123456789ABCDEF";
				msg.конкат('=');
				msg.конкат(hex[(*p >> 4) & 15]);
				msg.конкат(hex[*p & 15]);
				begin = false;
			}

		if(!begin)
			msg.конкат("\r\n");
	}
	for(int i = 0; i < attachments.дайСчёт(); i++) {
		const Attachment& a = attachments[i];
		Один<Поток> source;
		if(a.file.дайСчёт()) {
			ФайлВвод& in = source.создай<ФайлВвод>();
			if(!in.открой(a.file))
				throw Искл("Unable to open attachment file " + a.file);
		}
		else
			source.создай<ТкстПоток>().открой(a.данные);
		msg << "--" << delimiter << "\r\n"
			"Content-Type: " << a.mime << "; имя=\"" << a.имя << "\"\r\n"
			"Content-Transfer-Encoding: base64\r\n"
			"Content-Disposition: attachment; имяф=\"" << a.имя << "\"\r\n"
			"\r\n";

		char буфер[54];
		for(int c; (c = source -> дай(буфер, sizeof(буфер))) != 0;)
		{
			msg.конкат(Base64Encode(буфер, буфер + c));
			msg.конкат('\r');
			msg.конкат('\n');
			if(msg.дайДлину() >= 65536 && chunks) {
				SendData(msg);
				msg = Null;
			}
		}
	}
	if(multi || alter)
		msg << "--" << delimiter << "--\r\n";
	LLOGB("Msg:");
	LLOGB(msg);

	return msg;
}

bool Smtp::Send(const Ткст& msg_)
{
	smtp_code = 0;
	smtp_msg.очисть();

	try {
		if(пусто_ли(host))
			throw Искл(t_("Хост not set."));

		if(to.пустой())
			throw Искл(t_("Recipient not set."));

		if(!Connect(host, Nvl(port, starttls ? 587 : (ssl ? 465 : 25))))
			throw Искл(фмт("Cannot open socket %s:%d: %s", host, port, GetErrorDesc()));

		GlobalTimeout(request_timeout);

		if(ssl)
			if(!StartSSL())
				throw Искл("Unable to start SSL");

		// Receive initial message.
		SendRecv(Null);

		// Send HELO/EHLO command and query smtp service extensions.
		if(SendHello()) {
			auto ext = GetExtensions();
			if(!ext.пустой()) {
				// Check services.
				if(starttls) {
					if(ext.найди("starttls") < 0)
						throw Искл("STARTTLS is not supported by the server.");
					StartTls();
					SendHello();
				}
			}
		}

		// Everything is fine. Let us login now.
		Authenticate();

		// Send mail.
		SendMail(msg_);

		// открой connection.
		Quit();
		return true;
	}
	catch(Искл e) {
		Ошибка = e;
		LLOG("-- " << Ошибка);
		return false;
	}
}

Smtp& Smtp::нов() {
	to.очисть();
	to_name.очисть();
	as.очисть();
	body.очисть();
	mime.очисть();
	Ошибка.очисть();
	add_header.очисть();
	from.очисть();
	sender.очисть();
	message_id = какТкст(Uuid::создай());
	return *this;
}

Smtp::Smtp()
{
	port = Null;
	no_header = no_header_sep = false;
	time_sent = дайСисВремя();
	request_timeout = 120000;
	smtp_code = 0;
	ssl = false;
	starttls = false;
	нов();
}

}
