#include "ftp.h"
#include "lib/ftplib.h"

namespace РНЦП {

#define LLOGBLOCK(x)  // LOGBLOCK(x)
#define LLOG(x)       // DLOG(x)

static bool ftpinit = false;

FtpClient::FtpClient()
{
	ftpconn = NULL;
	if(!ftpinit) {
		INTERLOCKED {
			if(!ftpinit) {
				FtpInit();
				ftpinit = true;
			}
		}
	}
}

FtpClient::~FtpClient()
{
	закрой();
}

bool FtpClient::открыт() const
{
	return ftpconn;
}

bool FtpClient::Connect(const char *host, const char *user, const char *password, bool pasv, int idletimeout_secs)
{
	LOGBLOCK("FtpClient::Connect");
	закрой();
	char perror[512];
	LLOG("FtpConnect(" << host << ")");
	if(WhenProgress(0,0)) {
		Ошибка = "connect aborted";
		return false;
	}
	if(!FtpConnect(host, &ftpconn, perror, &FtpClient::Callback, this, 200, idletimeout_secs)) {
		Ошибка = perror;
		return false;
	}
	LLOG("FtpLogin(" << user << ", " << password << ")");
	if(!FtpLogin(user, password, ftpconn)) {
		Ошибка = FtpError(ftpconn);
		закрой();
		return false;
	}
	LLOG("FtpOptions(pasv = " << pasv << ")");
	if(!FtpOptions(FTPLIB_CONNMODE, pasv ? FTPLIB_PASSIVE : FTPLIB_PORT, ftpconn)) {
		Ошибка = FtpError(ftpconn);
		закрой();
		return false;
	}
	return true;
}

int FtpClient::Callback(netbuf *nControl, int xfered, void *arg)
{
	FtpClient *ftp = (FtpClient *)arg;
	return !ftp->WhenProgress(xfered, ftp->save_total);
}

void FtpClient::закрой()
{
	if(ftpconn) {
		FtpQuit(ftpconn);
		ftpconn = NULL;
	}
}

bool FtpClient::CheckOpen()
{
	if(!открыт()) {
		Ошибка = t_("not connected");
		return false;
	}
	return true;
}

Ткст FtpClient::грузи(const char *path)
{
	LLOGBLOCK("FtpClient::грузи");
	if(!CheckOpen())
		return Ткст::дайПроц();
	netbuf *ftpdata;
	LLOG("FtpAccess(" << path << ")");
	if(WhenProgress(0,0)) {
		Ошибка = t_("aborted");
		return Ткст::дайПроц();
	}
	if(!FtpAccess(path, FTPLIB_FILE_READ, FTPLIB_IMAGE, ftpconn, &ftpdata)) {
		Ошибка = FtpError(ftpconn);
		return Ткст::дайПроц();
	}
	load_data = Null;
	while(!WhenProgress(load_data.дайДлину(), 0)) {
		byte буфер[1024];
		int ndata = FtpRead(буфер, sizeof(буфер), ftpdata);
		LLOG("FtpRead -> " << ndata);
		if(ndata < 0) {
			Ошибка = FtpError(ftpdata);
			FtpClose(ftpdata);
			return Ткст::дайПроц();
		}
		if(ndata == 0) {
			load_data.сожми();
			Ошибка = FtpError(ftpdata);
			break;
		}
		load_data.конкат(буфер, ndata);
#ifdef SLOWTRANSFER
		int end = GetTickCount() + SLOWTRANSFER;
		for(int d; (d = end - GetTickCount()) > 0; спи(d))
			;
#endif
	}
	FtpClose(ftpdata);
	return load_data;
}

bool FtpClient::сохрани(const char *path, Ткст data)
{
	return SaveCount(path, data) == data.дайДлину();
}

int FtpClient::SaveCount(const char *path, Ткст data)
{
	LLOGBLOCK("FtpClient::сохрани");
	netbuf *ftpdata;
	save_pos = 0;
	save_total = data.дайДлину();
	if(!CheckOpen())
		return 0;
	LLOG("FtpAccess(" << path << ")");
	if(WhenProgress(0,data.дайДлину()))
		return 0;
	if(!FtpAccess(path, FTPLIB_FILE_WRITE, FTPLIB_IMAGE, ftpconn, &ftpdata)) {
		Ошибка = FtpError(ftpconn);
		return 0;
	}
	while(save_pos < data.дайДлину()) {
		if(WhenProgress(save_pos, data.дайДлину())) {
			Ошибка = фмтЧ(t_("write aborted after %d bytes(s)"), save_pos);
			FtpClose(ftpdata);
			return save_pos;
		}
		int chunk = min(data.дайДлину() - save_pos, 1024);
		int ndata = FtpWrite((void *)data.дайОбх(save_pos), chunk, ftpdata);
		LLOG("FtpWrite(" << chunk << ") -> " << ndata);
		if(ndata <= 0 || ndata > chunk) {
			Ошибка = FtpError(ftpdata);
			FtpClose(ftpdata);
			return save_pos;
		}
		save_pos += ndata;
#ifdef SLOWTRANSFER
		int end = GetTickCount() + SLOWTRANSFER;
		for(int d; (d = end - GetTickCount()) > 0; спи(d))
			;
#endif
	}
	WhenProgress(save_pos, data.дайДлину());
	LLOG("FtpClose");
	FtpClose(ftpdata);
	return save_pos;
}

bool  FtpClient::Exists(const char *path) {
	LLOGBLOCK("FtpClient::Exists");
	if(!CheckOpen())
		return false;
	netbuf *data;
	LLOG("FtpAccess(" << path << ")");
	if(!FtpAccess(path, FTPLIB_FILE_READ, FTPLIB_IMAGE, ftpconn, &data)) {
		Ошибка = FtpError(ftpconn);
		return false;
	}
	FtpClose(data);
	return true;
}

bool FtpClient::Rename(const char *oldpath, const char *newpath) {
	LLOGBLOCK("FtpClient::Rename");
	LLOG("FtpRename(oldname = " << oldpath << ", newname = " << newpath << ")");
	return CheckOpen() && !!FtpRename(oldpath, newpath, ftpconn);
}

bool FtpClient::Cd(const char *path) {
	LLOGBLOCK("FtpClient::Cd");
	LLOG("FtpChdir(" << path << ")");
	return CheckOpen() && !!FtpChdir(path, ftpconn);
}

bool FtpClient::MkDir(const char *path) {
	LLOGBLOCK("FtpClient::MkDir");
	LLOG("FtpMkdir(" << path << ")");
	return CheckOpen() && !!FtpMkdir(path, ftpconn);
}

bool FtpClient::RmDir(const char *path) {
	LLOGBLOCK("FtpClient::RmDir");
	LLOG("FtpRmdir(" << path << ")");
	return CheckOpen() && !!FtpRmdir(path, ftpconn);
}

bool FtpClient::Delete(const char *path) {
	LLOGBLOCK("FtpClient::Delete");
	LLOG("FtpDelete(" << path << ")");
	return CheckOpen() && !!FtpDelete(path, ftpconn);
}

Ткст FtpClient::Список(const char *path)
{
	LLOGBLOCK("FtpClient::Список");
	load_data = Null;
	if(!CheckOpen())
		return Ткст::дайПроц();
	netbuf *ftpdata;
	LLOG("FtpAccess(" << path << ")");
	if(WhenProgress(0,0)) {
		Ошибка = t_("aborted");
		return Ткст::дайПроц();
	}
	if(!FtpAccess(path, FTPLIB_DIR, FTPLIB_ASCII, ftpconn, &ftpdata)) {
		Ошибка = FtpError(ftpconn);
		return Ткст::дайПроц();
	}
	while(!WhenProgress(load_data.дайДлину(),0)) {
		byte буфер[1024];
		int ndata = FtpRead(буфер, sizeof(буфер), ftpdata);
		LLOG("FtpRead -> " << ndata);
		if(ndata < 0) {
			Ошибка = FtpError(ftpdata);
			FtpClose(ftpdata);
			return Ткст::дайПроц();
		}
		if(ndata == 0) {
			load_data.сожми();
			break;
		}
		load_data.конкат(буфер, ndata);
	#ifdef SLOWTRANSFER
		int end = GetTickCount() + SLOWTRANSFER;
		for(int d; (d = end - GetTickCount()) > 0; спи(d))
			;
	#endif
	}
	FtpClose(ftpdata);
	return load_data;
}

void FtpClient::RealizePath(const char *path)
{
	LLOGBLOCK("FtpClient::RealizePath");
	const char *s = path;
	if(*s == '\0') return;
	for(;;) {
		s = strchr(s + 1, '/');
		if(!s) break;
		MkDir(Ткст(path, s));
	}
}

Ткст FtpClientGet(Ткст url, Ткст *Ошибка)
{
	const char *u = url, *t = u;
	while(*t && *t != '?')
		if(*t++ == '/') {
			if(*t == '/')
				u = ++t;
			break;
		}
	t = u;
	while(*u && *u != '@' && *u != '/')
		u++;
	Ткст host = Ткст(t, u);
	Ткст user, pwd;
	if(*u == '@') {
		t = ++u;
		while(*u && *u != '/' && *u != ':')
			u++;
		user = Ткст(t, u);
		if(*u == ':') {
			t = ++u;
			while(*u && *u != '/')
				u++;
			pwd = Ткст(t, u);
		}
	}
	FtpClient ftp;
	if(!ftp.Connect(host, user, pwd)) {
		if(Ошибка)
			*Ошибка = ftp.дайОш();
		return Ткст::дайПроц();
	}
	Ткст data = ftp.грузи(u);
	if(data.проц_ли()) {
		if(Ошибка)
			*Ошибка = ftp.дайОш();
	}
	return data;
}

}
