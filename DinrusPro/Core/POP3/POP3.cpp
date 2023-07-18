#include "POP3.h"

static бул sPop3Trace;
#define LLOG(x) do { if(sPop3Trace) RLOG(сожмиЛог(Ткст().кат() << x)); } while(0)

проц Pop3::Trace(бул b)
{
	sPop3Trace = b;
}

Pop3& Pop3::прокси(кткст0 p)
{
	proxy_port = 8080;
	ParseProxyUrl(p, proxy_host, proxy_port);
	return *this;
}

Ткст Pop3::GetTimeStamp()
{
	цел begin = данные.найди('<');
	if(begin >= 0) {
		цел end = данные.найди('>', begin);
		if(end > begin) {
			end++;
			return данные.середина(begin, end - begin);
		}
	}
	return Null;
}

бул Pop3::GetListItems(МапЗнач& list, бцел type1, бцел type2)
{
	ТкстПоток s(данные);
	while(!s.кф_ли()) {
		Ткст line = s.дайСтроку();
		Вектор<Ткст> s = разбей(line, ' ');
		if(s.дайСчёт() < 2)
			return false;
		list.добавь(скан(type1, s[0]), скан(type2, s[1]));
	}
	return true;	
}

цел Pop3::GetMessageCount()
{
	if(!PutGet("STAT\r\n"))
		return Null;
	Ткст ok, cnt, tsz;
	if(!SplitTo(данные, ' ', ok, cnt, tsz))
		return Null;
	return тктЦел(cnt);
}

Ткст Pop3::GetMessage(цел индекс)
{
	if(!PutGet(фмт("RETR %d\r\n", индекс), true))
		return Null;
	return данные;
}

Ткст Pop3::GetMessageHeader(цел индекс)
{
	if(!PutGet(фмт("TOP %d %d\r\n", индекс, 0), true))
		return Null;
	return данные;
}

бул Pop3::GetMessageList(МапЗнач& list)
{
	if(!PutGet("LIST\r\n", true))
		return false;
	return GetListItems(list, INT_V, INT_V);
}


Ткст Pop3::GetMessageUniqueId(цел индекс)
{
	if(!PutGet(фмт("UIDL %d\r\n", индекс)))
		return Null;
	Ткст ok, ид, tsz;
	if(!SplitTo(данные, ' ', ok, ид, tsz))
		return Null;
	return ид;
}

бул Pop3::GetMessageUniqueIds(МапЗнач& uids)
{
	if(!PutGet("UIDL\r\n", true))
		return false;
	return GetListItems(uids, INT_V, STRING_V);
}

бул Pop3::RemoveMessage(цел индекс)
{
	return PutGet(фмт("DELE %d\r\n", индекс));
}

бул Pop3::Undo()
{
	return PutGet("RSET\r\n");
}

бул Pop3::Noop()
{
	return PutGet("NOOP\r\n");
}

бул Pop3::PutGet(const Ткст& s, бул multiline, бул nolog)
{
	// помести() request.
	if(!s.пустой()) {
		if(!nolog)
			LLOG(">> " << обрежьПраво(s));
		if(!PutAll(s)) {
			LLOG("-- " << GetLastError());
			return false;
		}		
	}
	// дай() respone.
	данные.очисть();
	const цел MAXLINE = 20000000;
	Ткст line = дайСтроку(MAXLINE);
	if(!line.проц_ли()) {
		LLOG("<< " << обрежьПраво(line));
		if(line.начинаетсяС("+OK")) {
			if(!multiline) {
				данные.кат(line);
				данные.кат("\r\n");
				return true;
			}
			else 
				for(;;) {
					line = дайСтроку(MAXLINE);
					if(line.проц_ли())
						break;
					if(line == ".") {
						LLOG("<< ...");
						return true;
					}
					данные.кат(*line == '.' ? line.середина(1) : line);
					данные.кат("\r\n");
				}
		}
		else
		if(line.начинаетсяС("-ERR"))
			Ошибка = line;
	}
	LLOG("-- " << GetLastError());
	return false;
}

бул Pop3::Authenticate()
{
	// Try using APOP authentication.
	Ткст timestamp = GetTimeStamp();
	if(!timestamp.пустой()) {
		if(PutGet("APOP " + user + " " + мд5Ткст(timestamp << pass) + "\r\n"))
			return true;
	}
	else
	if(PutGet("USER " + user + "\r\n")) {
		LLOG(">>PASS ******");
		if(PutGet("PASS " + pass + "\r\n", false, true))
			return true;
	}
	
	return false;
}

бул Pop3::Login()
{
	try {
		if(host.пустой())
			throw Искл(t_("Hostname is not specified."));
		if(user.пустой())
			throw Искл(t_("Username is not specified."));
		if(pass.пустой())
			throw Искл(t_("Password is nor specified."));
		if(proxy_host.дайСчёт()) {
			Ткст host_port = host;
			host_port << ':' << Nvl(port, ssl ? 995 : 110);
			Ткст данные;
			данные << "CONNECT " << host_port << " HTTP/1.1\r\n"
			     << "Хост: " << host_port << "\r\n";
			if(!пусто_ли(proxy_username))
				данные << "прокси-Authorization: Basic "
				     << Base64Encode(proxy_username + ':' + proxy_password) << "\r\n";
			данные << "\r\n";
			LLOG("Trying to connect proxy " << proxy_host << ":" << proxy_port);
			if(!Connect(proxy_host, proxy_port))
				throw Искл("Unable to connect the proxy");
			LLOG("About to send proxy request:\n" << данные);
			if(!PutAll(данные))
				throw Искл("Unable to send request to the proxy");
			Ткст response;
			for(;;) {
				Ткст l = дайСтроку();
				if(l.дайСчёт() == 0)
					break;
				LLOG("< " << l);
				if(response.дайСчёт() == 0)
					response = l;
			}
			LLOG("прокси response: " << response);
			if(!response.начинаетсяС("HTTP") || response.найди(" 2") < 0)
				throw Искл("Invalid proxy reply: " + response);
			LLOG("Connected via proxy");
		}
		else
		if(!Connect(host, Nvl(port, ssl ? 995 : 110)))
			throw Искл(GetErrorDesc());
		LLOG(фмт(t_("Opening connection to %s:%d."), host, port));
		if(ssl) {
			if(!StartSSL())
				throw Искл(t_("Couldn't start SSL session."));
			LLOG(t_("SSL session successfully started."));
		}
		// Receive server greetings.
		if(!PutGet(Null))
			throw Искл(GetLastError());
		if(!Authenticate())
			throw Искл(GetLastError());
	}
	catch (Искл e) {
		Ошибка = e;
		LLOG("-- " + e);
		Logout();
		return false;
	}
	return online = true;
}

бул Pop3::Logout()
{
	if(IsOnline()) 
		PutGet("QUIT\r\n");
	LLOG(фмт(t_("Closing connection to %s:%d."), host, port));
	if(открыт())
		открой();
	online = false;
	return true;
}

Pop3::Pop3()
{
	ssl         = false;
	online		= false;
	Timeout(60000);
}

Pop3::~Pop3()
{
	Logout();
}
