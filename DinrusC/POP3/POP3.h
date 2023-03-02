#ifndef _POP3_POP3_h
#define _POP3_POP3_h

#include <Core/Core.h>

namespace РНЦПДинрус {

class Pop3 : public TcpSocket 
{
	Ткст       proxy_host;
	int          proxy_port;
	Ткст       proxy_username;
	Ткст       proxy_password;

	Ткст       host;
	Ткст       user;
	Ткст       pass;
	Ткст       Ошибка;
	Ткст       данные;
	int          port;
	bool         ssl;
	bool         online;

	bool         GetListItems(МапЗнач& list, dword type1, dword type2);
	Ткст       GetTimeStamp();
	bool         Authenticate();
	bool         PutGet(const Ткст& s, bool multiline = false, bool nolog = false);

public:
	Pop3&        Хост(const Ткст& h)                  { host = h; return *this; }
	Pop3&        Port(int p)                            { port = p; return *this; }
	Pop3&        User(const Ткст& u, const Ткст& p) { user = u; pass = p; return *this; }
	Pop3&        SSL(bool b = true)                     { ssl  = b; return *this; }
	
	Pop3&        прокси(const Ткст& host, int port)         { proxy_host = host; proxy_port = port; return *this; }
	Pop3&        прокси(const char *p);
	Pop3&        ProxyAuth(const Ткст& u, const Ткст& p) { proxy_username = u; proxy_password = p; return *this; }

	int          GetMessageCount();
	Ткст       GetMessage(int индекс);
	Ткст       GetMessageHeader(int индекс);
	bool         RemoveMessage(int индекс);

	bool         GetMessageList(МапЗнач& list);
	Ткст       GetMessageUniqueId(int индекс);
	bool         GetMessageUniqueIds(МапЗнач& uids);

	bool         Undo();
	bool         Noop();
	
	bool         Login();
	bool         Logout();
	
	bool         IsOnline() const                         { return online; }

	Ткст       GetLastError()                           { return ошибка_ли() ? GetErrorDesc() : Ошибка; }
	static void  Trace(bool b = true);

	Pop3();
	~Pop3();
};

struct InetMessage {
	struct Part : Движимое<Part> {
		int                       parent;
		ВекторМап<Ткст, Ткст> header;
		Ткст                    body;
		
		Ткст operator[](const char *ид) const          { return header.дай(ид, Null); }
		void   уст(const char *ид, const Ткст& значение)  { header.дайДобавь(ид) = значение; }
		Ткст Decode() const;
		
		bool   IsMultipart(Ткст& boundary, Ткст& end_boundary) const;
	};

	Вектор<Part> part;

	bool   читай(const Ткст& msg);
	bool   ReadHeader(const Ткст& msg);
	
	void   очисть()                                       { part.очисть(); }

	int         дайСчёт() const                         { return part.дайСчёт(); }
	const Part& operator[](int i) const                  { return part[i]; }
	Ткст      operator[](const char *ид) const         { return дайСчёт() ? part[0][ид] : Ткст(); }
	void        уст(const char *ид, const Ткст& значение) { return part.по(0).уст(ид, значение); }
	
	Ткст GetMessage() const;

private:
	bool   ReadHeader(ВекторМап<Ткст, Ткст>& hdr, Поток& ss);
	bool   ReadPart(Поток& ss, int parent, int level);
	void   PutBody(int pi, Ткст& r, int level) const;
};

struct MIMEHeader {
	Ткст значение;
	ВекторМап<Ткст, Ткст> param;
	
	Ткст operator[](const char *ид) const { return param.дай(ид, Null); }
	Ткст operator~() const                { return значение; }
	
	bool   Parse(const char *s);
	Ткст вТкст() const;
	
	MIMEHeader(const char *s)               { Parse(s); }
	MIMEHeader()                            {}
};

Индекс<Ткст> ParseMessageIDs(const Ткст& s);
Ткст        FormatMessageIDs(const Индекс<Ткст>& ид);

}

#endif
