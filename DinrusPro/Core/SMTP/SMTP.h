#ifndef _smtp_smtp_h
#define _smtp_smtp_h

#include <DinrusPro/DinrusPro.h>

class Smtp : public TcpSocket {
    struct Attachment
    {
        Ткст имя; // mail имя
        Ткст file; // source path (dynamic attachments only)
        Ткст mime; // content тип (application/octet-stream by default)
        Ткст данные;
    };

	Ткст         host;
	цел            port; // default = 25
	бул           ssl;
	бул           starttls;
	Ткст         auth_user;
	Ткст         auth_pwd;
	Ткст         sender;
	Ткст         from;
	Ткст         from_name;
	Вектор<Ткст> to;
	Вектор<Ткст> to_name;
	Вектор<сим>   as;
	Вектор<Ткст> body;
	Вектор<Ткст> mime; // default: text/plain; charset=<default application charset>
	Массив<Attachment> attachments;
	цел            request_timeout;
	Ткст         add_header;

	бул           no_header; // default = false
	бул           no_header_sep; // default = false
	Время           time_sent;
	Ткст         reply_to;
	Ткст         reply_to_name;
	Ткст         subject;

	Ткст         smtp_msg;
	цел            smtp_code;
	Ткст         Ошибка;
	Ткст         message_id;

	Ткст      GetDomainName();
	МапЗнач    GetExtensions();
	цел         GetSmtpCode(const Ткст& s);
	проц        SetSender();
	проц        SetRecipients();
	проц        SendRecv(const Ткст& s);
	проц	    SendRecvOK(const Ткст& s);
	проц        SendMail(const Ткст& msg_);
	проц        SendData(const Ткст &s);
	бул	    SendHello();
	проц        StartTls();
	проц        Authenticate();
	проц        Quit();
 
	Ткст     GetMessage(бул chunks);
	

	бул       ReplyIsWait() const                                { return smtp_code >= 100 && smtp_code <= 199; }
	бул       ReplyIsSuccess() const                             { return smtp_code >= 200 && smtp_code <= 299; }
	бул       ReplyIsPending() const                             { return smtp_code >= 300 && smtp_code <= 399; }
	бул       ReplyIsFailure() const                             { return smtp_code >= 400 && smtp_code <= 499; }
	бул       ReplyIsError() const                               { return smtp_code >= 500 || smtp_code == -1;  }

public:
	enum AS { TO, CC, BCC };

	Smtp&      RequestTimeout(цел ms)                             { request_timeout = ms; return *this; }
	Smtp&      Хост(const Ткст& h)                              { host = h; return *this; }
	Smtp&      Port(цел p)                                        { port = p; return *this; }
	Smtp&      SSL(бул b = true)                                 { ssl = b; if(b) starttls = !b; return *this; }
	Smtp&      StartTLS(бул b = true)                            { starttls = b; if(b) ssl = !b; return *this; }
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
	Smtp&      AttachFile(кткст0 имяф, кткст0 mime = 0);
	Smtp&      прикрепи(кткст0 имя, const Ткст& данные, кткст0 mime = 0);
	Smtp&      AddHeader(const Ткст& text)                      { add_header << text << "\r\n"; return *this; }
	Smtp&      AddHeader(кткст0 ид, const Ткст& txt)       { add_header << ид << ": " << txt << "\r\n"; return *this; }

	Smtp&      нов();

    Ткст     GetMessage()                                       { return GetMessage(false); }
	Ткст     GetMessageID();
	бул       Send(const Ткст& message);

	бул       Send()                                             { return Send(Null); }

	Ткст     дайОш() const                                   { return Ошибка; }

	Smtp();

	static проц    Trace(бул b = true);
	static проц    TraceBody(бул b = true);

	static Ткст  Encode(const Ткст& text);
	static Ткст  FormatAddr(const Ткст& addr, const Ткст& имя);
};

#endif
