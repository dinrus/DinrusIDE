#ifndef _umake_umake_h_
#define _umake_umake_h_

#include <DinrusIDE/Builders/Builders.h>

extern bool SilentMode;

class Консоль {
public:
	virtual void приставь(const Ткст& s);

protected:
	struct Слот {
		Слот() : outfile(NULL), quiet(true), exitcode(Null) {}

		Один<ПроцессИнк>     process;
		Ткст            cmdline;
		Ткст            output;
		Ткст            ключ;
		Ткст            группа;
		Поток            *outfile;
		bool              quiet;
		int               exitcode;
		int               last_msecs;
		int               serial;
	};

	struct Группа {
		Группа() : count(0), start_time(::msecs()), finished(false), msecs(0), raw_msecs(0) {}

		int               count;
		int               start_time;
		bool              finished;
		int               msecs;
		int               raw_msecs;
	};

	struct Финишер {
		int               serial;
		Событие<>           cb;
	};

	Массив<Слот> processes;
	Массив<Финишер> finisher;
	МассивМап<Ткст, Группа> groups;
	Вектор<Ткст> error_keys;
	Ткст current_group;
	Ткст spooled_output;
	int console_lock;
	bool wrap_text;
	int  serial;

	void CheckEndGroup();
	void слейКонсоль();

public:
	bool console;
	bool verbosebuild;

	int  выполни(const char *cmdline, Поток *out = NULL, const char *envptr = NULL, bool quiet = false, bool noconvert = false);
	int  выполни(Один<ПроцессИнк> process, const char *cmdline, Поток *out = NULL, bool quiet = false);
	int  дайСчётСлотов() const { return processes.дайСчёт(); }
	int  разместиСлот();
	bool пуск(const char *cmdline, Поток *out = NULL, const char *endptr = NULL, bool quiet = false, int slot = 0, Ткст ключ = Null, int blitz_count = 1);
	bool пуск(Один<ПроцессИнк> process, const char *cmdline, Поток *out = NULL, bool quiet = false, int slot = 0, Ткст ключ = Null, int blitz_count = 1);
	void BeginGroup(Ткст группа);
	void EndGroup();

	Консоль& operator<<(const Ткст& s)      { приставь(s); return *this; }

	bool пущен();
	bool пущен(int slot);
	int  слей();
	void глуши(int slot);
	void глуши();
	void сотриОш()                         { error_keys.очисть(); }
	Вектор<Ткст> PickErrors()               { Вектор<Ткст> e = pick(error_keys); error_keys.очисть(); return e; }
	void жди(int slot);
	bool жди();

	void OnFinish(Событие<> cb);

	void WrapText(bool w)                     { wrap_text = w; }

	void устСлоты(int s);

	Консоль();
};

struct Иср : public КонтекстИср, public MakeBuild {
	РОбласть wspc;
	Консоль   console;
	Ткст    onefile;
	Ткст    mainconfigpatam;
	bool      verbose;
	Ткст    main;
	int       build_time;

	virtual bool             вербозно_ли() const;
	virtual void             вКонсоль(const char *s);
	virtual void             PutVerbose(const char *s);
	virtual void             PutLinking() {}
	virtual void             PutLinkingEnd(bool ok) {}

	virtual const РОбласть& роблИср() const ;
	virtual bool             строитсяИср() const ;
	virtual Ткст           исрДайОдинФайл() const ;
	virtual int              выполниВКонсИср(const char *cmdline, Поток *out = NULL, const char *envptr = NULL, bool quiet = false, bool = false);
	virtual int              выполниВКонсИсрСВводом(const char *cmdline, Поток *out, const char *envptr, bool quiet, bool = false);
	virtual int              выполниВКонсИср(Один<ПроцессИнк> process, const char *cmdline, Поток *out = NULL, bool quiet = false);
	virtual int              разместиСлотКонсИср();
	virtual bool             пускКонсИср(const char *cmdline, Поток *out = NULL, const char *envptr = NULL, bool quiet = false, int slot = 0, Ткст ключ = Null, int blitz_count = 1);
	virtual bool             пускКонсИср(Один<ПроцессИнк> process, const char *cmdline, Поток *out = NULL, bool quiet = false, int slot = 0, Ткст ключ = Null, int blitz_count = 1);
	virtual void             слейКонсИср();
	virtual void             начниГруппуКонсИср(Ткст группа);
	virtual void             завершиГруппуКонсИср();
	virtual bool             ждиКонсИср();
	virtual bool             ждиКонсИср(int slot);
	virtual void             приФинишеКонсИср(Событие<> cb);

	virtual bool      исрОтладка_ли() const ;
	virtual void      исрЗавершиОтладку();
	virtual void      исрУстНиз(Ктрл& ctrl);
	virtual void      исрАктивируйНиз();
	virtual void      исрУдалиНиз(Ктрл& ctrl);
	virtual void      исрУстПраво(Ктрл& ctrl);
	virtual void      исрУдалиПраво(Ктрл& ctrl);

	virtual Ткст    исрДайИмяф() const ;
	virtual int       исрДайСтрокуф();
	virtual Ткст    исрДайСтроку(int i) const ;

	virtual void      исрУстПозОтладки(const Ткст& фн, int line, const Рисунок& img, int i);
	virtual void      исрСкройУк();
	virtual bool      исрОтладБлокируй();
	virtual bool      исрОтладРазблокируй();
	virtual bool      исрОтладБлокировка_ли() const ;
	virtual void      исрУстБар();
	virtual void      IdeGotoCodeRef(Ткст link);
	virtual void      IdeOpenTopicFile(const Ткст& file);
	virtual void      исрСлейФайл();
	virtual Ткст    исрДайИмяф();
	virtual Ткст    IdeGetNestFolder();

	virtual void           ConsoleShow();
	virtual void           ConsoleSync();
	virtual void           ConsoleClear();
	virtual void           SetupDefaultMethod();
	virtual Вектор<Ткст> PickErrors();
	virtual void           BeginBuilding(bool clear_console);
	virtual void           EndBuilding(bool ok);
	virtual void           ClearErrorEditor();
	virtual void           DoProcessСобытиеs();
	virtual void           ReQualifyCodeBase();
	virtual void           SetErrorEditor();
	virtual Ткст         GetMain();

	virtual Ткст                    GetDefaultMethod();
	virtual ВекторМап<Ткст, Ткст> GetMethodVars(const Ткст& method);
	virtual Ткст                    GetMethodName(const Ткст& method);

	virtual bool IsPersistentFindReplace() { return false; }

	virtual Ткст    IdeGetIncludePath() { return Null; }

	virtual int       IdeGetHydraThreads() { return цпбЯдра(); }
	virtual Ткст    IdeGetCurrentBuildMethod() { return Null; }
	virtual Ткст    IdeGetCurrentMainPackage() { return Null; }
	virtual void      IdePutErrorLine(const Ткст&) {}
	virtual void      IdeGotoFileAndId(const Ткст& path, const Ткст& id) {}

	void ExportMakefile(const Ткст& ep);
	void ExportProject(const Ткст& ep, bool all, bool deletedir = true);

	Иср();
};

#endif
