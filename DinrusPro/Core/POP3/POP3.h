#ifndef _POP3_POP3_h
#define _POP3_POP3_h

#include <DinrusPro/DinrusPro.h>

class Pop3 : public TcpSocket 
{
	Ткст       proxy_host;
	цел          proxy_port;
	Ткст       proxy_username;
	Ткст       proxy_password;

	Ткст       host;
	Ткст       user;
	Ткст       pass;
	Ткст       Ошибка;
	Ткст       данные;
	цел          port;
	бул         ssl;
	бул         online;

	бул         GetListItems(МапЗнач& list, бцел type1, бцел type2);
	Ткст       GetTimeStamp();
	бул         Authenticate();
	бул         PutGet(const Ткст& s, бул multiline = false, бул nolog = false);

public:
	Pop3&        Хост(const Ткст& h)                  { host = h; return *this; }
	Pop3&        Port(цел p)                            { port = p; return *this; }
	Pop3&        User(const Ткст& u, const Ткст& p) { user = u; pass = p; return *this; }
	Pop3&        SSL(бул b = true)                     { ssl  = b; return *this; }
	
	Pop3&        прокси(const Ткст& host, цел port)         { proxy_host = host; proxy_port = port; return *this; }
	Pop3&        прокси(кткст0 p);
	Pop3&        ProxyAuth(const Ткст& u, const Ткст& p) { proxy_username = u; proxy_password = p; return *this; }

	цел          GetMessageCount();
	Ткст       GetMessage(цел индекс);
	Ткст       GetMessageHeader(цел индекс);
	бул         RemoveMessage(цел индекс);

	бул         GetMessageList(МапЗнач& list);
	Ткст       GetMessageUniqueId(цел индекс);
	бул         GetMessageUniqueIds(МапЗнач& uids);

	бул         Undo();
	бул         Noop();
	
	бул         Login();
	бул         Logout();
	
	бул         IsOnline() const                         { return online; }

	Ткст       GetLastError()                           { return ошибка_ли() ? GetErrorDesc() : Ошибка; }
	static проц  Trace(бул b = true);

	Pop3();
	~Pop3();
};

struct InetMessage {
	struct Part : Движ<Part> {
		цел                       parent;
		ВекторМап<Ткст, Ткст> header;
		Ткст                    body;
		
		Ткст operator[](кткст0 ид) const          { return header.дай(ид, Null); }
		проц   уст(кткст0 ид, const Ткст& значение)  { header.дайДобавь(ид) = значение; }
		Ткст Decode() const;
		
		бул   IsMultipart(Ткст& boundary, Ткст& end_boundary) const;
	};

	Вектор<Part> part;

	бул   читай(const Ткст& msg);
	бул   ReadHeader(const Ткст& msg);
	
	проц   очисть()                                       { part.очисть(); }

	цел         дайСчёт() const                         { return part.дайСчёт(); }
	const Part& operator[](цел i) const                  { return part[i]; }
	Ткст      operator[](кткст0 ид) const         { return дайСчёт() ? part[0][ид] : Ткст(); }
	проц        уст(кткст0 ид, const Ткст& значение) { return part.по(0).уст(ид, значение); }
	
	Ткст GetMessage() const;

private:
	бул   ReadHeader(ВекторМап<Ткст, Ткст>& hdr, Поток& ss);
	бул   ReadPart(Поток& ss, цел parent, цел level);
	проц   PutBody(цел pi, Ткст& r, цел level) const;
};

struct MIMEHeader {
	Ткст значение;
	ВекторМап<Ткст, Ткст> param;
	
	Ткст operator[](кткст0 ид) const { return param.дай(ид, Null); }
	Ткст operator~() const                { return значение; }
	
	бул   Parse(кткст0 s);
	Ткст вТкст() const;
	
	MIMEHeader(кткст0 s)               { Parse(s); }
	MIMEHeader()                            {}
};

Индекс<Ткст> ParseMessageIDs(const Ткст& s);
Ткст        FormatMessageIDs(const Индекс<Ткст>& ид);


#endif
