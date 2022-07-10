#ifndef _smtp_smtp_h
#define _smtp_smtp_h

#include <Core/Core.h>

namespace РНЦПДинрус {

class Smtp : public TcpSocket {
    struct Attachment
    {
        Ткст имя; // mail имя
        Ткст file; // source path (dynamic attachments only)
        Ткст mime; // content тип (application/octet-stream by default)
        Ткст данные;
    };

	Ткст         host;
	int            port; // default = 25
	bool           ssl;
	bool           starttls;
	Ткст         auth_user;
	Ткст         auth_pwd;
	Ткст         sender;
	Ткст         from;
	Ткст         from_name;
	Вектор<Ткст> to;
	Вектор<Ткст> to_name;
	Вектор<char>   as;
	Вектор<Ткст> body;
	Вектор<Ткст> mime; // default: text/plain; charset=<default application charset>
	Массив<Attachment> attachments;
	int            request_timeout;
	Ткст         add_header;

	bool           no_header; // default = false
	bool           no_header_sep; // default = false
	Время           time_sent;
	Ткст         reply_to;
	Ткст         reply_to_name;
	Ткст         subject;

	Ткст         smtp_msg;
	int            smtp_code;
	Ткст         Ошибка;
	Ткст         message_id;

	Ткст      GetDomainName();
	МапЗнач    GetExtensions();
	int         GetSmtpCode(const Ткст& s);
	void        SetSender();
	void        SetRecipients();
	void        SendRecv(const Ткст& s);
	void	    SendRecvOK(const Ткст& s);
	void        SendMail(const Ткст& msg_);
	void        SendData(const Ткст &s);
	bool	    SendHello();
	void        StartTls();
	void        Authenticate();
	void        Quit();
 
	Ткст     GetMessage(bool chunks);
	

	bool       ReplyIsWait() const                                { return smtp_code >= 100 && smtp_code <= 199; }
	bool       ReplyIsSuccess() const                             { return smtp_code >= 200 && smtp_code <= 299; }
	bool       ReplyIsPending() const                             { return smtp_code >= 300 && smtp_code <= 399; }
	bool       ReplyIsFailure() const                             { return smtp_code >= 400 && smtp_code <= 499; }
	bool       ReplyIsError() const                               { return smtp_code >= 500 || smtp_code == -1;  }

public:
	enum AS { TO, CC, BCC };

	Smtp&      RequestTimeout(int ms)                             { request_timeout = ms; return *this; }
	Smtp&      Хост(const Ткст& h)                              { host = h; return *this; }
	Smtp&      Port(int p)                                        { port = p; return *this; }
	Smtp&      SSL(bool b = true)                                 { ssl = b; if(b) starttls = !b; return *this; }
	Smtp&      StartTLS(bool b = true)                            { starttls = b; if(b) ssl = !b; return *this; }
	Smtp&      Auth(const Ткст& user, const Ткст& pwd)        { auth_user = user; auth_pwd = pwd; return *this; }
	Smtp&      From(const Ткст& email, const Ткст& имя = Null, const Ткст& sender = Null);
	Smtp&      To(const Ткст& email, const Ткст& имя, AS a = TO);
	Smtp&      To(const Ткст& email, AS a = TO)                     { return To(email, Null, a); }
	Smtp&      Cc(const Ткст& email, const Ткст& имя = Null)     { return To(email, имя, CC); }
	Smtp&      Bcc(const Ткст& email, const Ткст& имя = Null)    { return To(email, имя, BCC); }
	Smtp&      ReplyTo(const Ткст& email, const Ткст& имя = Null);
	Smtp&      TimeSent(Время t)                                   { time_sent = t; return *this; }
	Smtp&      Subject(const Ткст& s);
	Smtp&      Body(const Ткст& s, const Ткст& mime_ = Null)  { body.добавь(s); mime.добавь(mime_); return *this; }
	Smtp&      NoHeader()                                         { no_header = true; return *this; }
	Smtp&      NoHeaderSep()                                      { no_header_sep = true; return *this; }
	Smtp&      AttachFile(const char *filename, const char *mime = 0);
	Smtp&      прикрепи(const char *имя, const Ткст& данные, const char *mime = 0);
	Smtp&      AddHeader(const Ткст& text)                      { add_header << text << "\r\n"; return *this; }
	Smtp&      AddHeader(const char *ид, const Ткст& txt)       { add_header << ид << ": " << txt << "\r\n"; return *this; }

	Smtp&      нов();

    Ткст     GetMessage()                                       { return GetMessage(false); }
	Ткст     GetMessageID();
	bool       Send(const Ткст& message);

	bool       Send()                                             { return Send(Null); }

	Ткст     дайОш() const                                   { return Ошибка; }

	Smtp();

	static void    Trace(bool b = true);
	static void    TraceBody(bool b = true);

	static Ткст  Encode(const Ткст& text);
	static Ткст  FormatAddr(const Ткст& addr, const Ткст& имя);
};

}

#endif
