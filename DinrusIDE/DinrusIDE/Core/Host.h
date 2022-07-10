enum { REMOTE_TIMEOUT = 2000 };

extern Ткст HostConsole;

struct Хост {
	struct ИнфОФайле : Время, Движимое<ИнфОФайле> {
		int length;
	};

	Вектор<Ткст> exedirs;
	Ткст         environment;

	Ткст        *cmdout;
	
	Ткст         onefile; // support for Ктрл+F7 - build single file
	
	bool           canlog = true; // it does PutVerbose for commands

	void    DoDir(const Ткст& s);

	Хост();
	
	void                  Log(const Ткст& s) { if(canlog) PutVerbose(s); }
	
	Ткст                GetEnvironment();
	void                  AddEnvironment(const char *id, const char *значение);
	Вектор<ИнфОФайле>      дайИнфОФайле(const Вектор<Ткст>& path);

	void                  ChDir(const Ткст& path);
	bool                  RealizeDir(const Ткст& path);

	int                   выполни(const char *cmdline);
	int                   ExecuteWithInput(const char *cmdline, bool noconvert);
	int                   выполни(const char *cmdline, Поток& out, bool noconvert = false);
	int                   разместиСлот();
	bool                  пуск(const char *cmdline, int slot, Ткст ключ, int blitz_count);
	bool                  пуск(const char *cmdline, Поток& out, int slot, Ткст ключ, int blitz_count);
	bool                  жди();
	bool                  жди(int slot);
	void                  OnFinish(Событие<> cb);
	bool                  StartProcess(ЛокальнПроцесс& p, const char *cmdline);
	void                  Launch(const char *cmdline, bool console = false);
	void                  добавьФлаги(Индекс<Ткст>& cfg);
	
	const Вектор<Ткст>& GetExecutablesDirs() const;
	
private:
	bool HasPlatformFlag(const Индекс<Ткст>& cfg);
};

#ifdef PLATFORM_POSIX
void RemoveConsoleScripts();
#endif