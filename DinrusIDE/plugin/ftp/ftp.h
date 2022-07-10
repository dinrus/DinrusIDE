#ifndef _plugin_ftp_ftp_h_
#define _plugin_ftp_ftp_h_

#include <Core/Core.h>

typedef struct NetBuf netbuf;

namespace РНЦП {

class FtpClient {
public:
	FtpClient();
	~FtpClient();

	bool    Connect(const char *host, const char *user = NULL, const char *password = NULL,
		bool pasv = true, int idletimeout_secs = 30);
	void    закрой();
	bool    открыт() const;

	bool    сохрани(const char *path, Ткст data);
	int     SaveCount(const char *path, Ткст data);
	Ткст  грузи(const char *path);
	Ткст  Список(const char *path);

	Ткст  GetLoadedPart() const { return load_data; }
	int     GetSavePos() const    { return save_pos; }
	int     GetSaveTotal() const  { return save_total; }

	bool    Rename(const char *oldpath, const char *newpath);
	bool    Cd(const char *path);
	bool    MkDir(const char *path);
	bool    RmDir(const char *path);
	bool    Delete(const char *path);
	bool    Exists(const char *path);
	void    RealizePath(const char *path);

	void    устОш(Ткст s) { Ошибка = s; }
	Ткст  дайОш() const   { return Ошибка; }
	void    сотриОш()       { Ошибка.очисть(); }

	bool    CheckOpen();

public:
	Gate2<int, int>    WhenProgress;

private:
	static int Callback(netbuf *nControl, int xfered, void *arg);

private:
	netbuf *ftpconn;
	Ткст  Ошибка;
	Ткст  load_data;
	int     save_pos;
	int     save_total;
};

Ткст FtpClientGet(Ткст url, Ткст *Ошибка = NULL);

}

#endif
