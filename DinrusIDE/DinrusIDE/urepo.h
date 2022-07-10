#define LAYOUTFILE <DinrusIDE/urepo.lay>
#include <CtrlCore/lay.h>

class UrepoConsole : public WithUrepoConsoleLayout<ТопОкно> {
	typedef UrepoConsole ИМЯ_КЛАССА;
	
	Шрифт font;
	void AddResult(const Ткст& out);
	bool canceled = false;
	int  hide_password_from = 0;
	int  hide_password_to = 0;

public:
	int  System(const char *s);
	int  CheckSystem(const char *s);
	int  Git(const char *dir, const char *command, bool pwd = false);
	void HidePassword(int from, int to)         { hide_password_from = from; hide_password_to = to; }
	void Log(const Значение& s, Цвет ink = SColorText());
	void Perform()	                            { exit.покажи(); выполни(); }
	void очисть()                                { list.очисть(); }

	UrepoConsole();
};

Ткст RepoSys(const char *cmd);

Ткст SvnCmd(const char *cmd);

Ткст GetSvnDir(const Ткст& p);

Ткст GitCmd(const char *dir, const char *command);

struct RepoSync : WithRepoSyncLayout<ТопОкно> {
	enum {
		MODIFY,
		CONFLICT,
		ADD,
		REMOVE,
		REPLACE,
		
		REPOSITORY,
		MESSAGE,
		
		SVN_IGNORE = -1,
		DELETEC = -2,
		SVN_ACTION = -3,
	};
	
	Массив<Switch>            revert;
	Массив<ДокРедакт>           message;
	Массив<Кнопка>            diff;
	МассивМап<Ткст, Ткст> msgmap;

	Индекс<Ткст>            svndir;
	
	struct SvnOptions : WithSvnOptionsLayout<КтрлРодитель> {
		SvnOptions() { CtrlLayout(*this); }
	};
	
	struct GitOptions : WithGitOptionsLayout<КтрлРодитель> {
		GitOptions() { CtrlLayout(*this); }
	};

	ВекторМап<Ткст, int> work;

	Ткст SvnCmd(UrepoConsole& sys, const char *svncmd, const Ткст& dir);
	bool ListGit(const Ткст& path);
	bool ListSvn(const Ткст& path);
	bool GitFile(UrepoConsole& sys, int action, const Ткст& path, bool revert);
	bool SvnFile(UrepoConsole& sys, Ткст& filelist, int action, const Ткст& path, bool revert);
	void SyncCommits();
	void SyncList();
	void DoDiff(int ii);
	void сериализуй(Поток& s);

	typedef RepoSync ИМЯ_КЛАССА;

public:
	void   SetMsgs(const Ткст& s);
	Ткст GetMsgs();

	void Dir(const char *dir, int kind);
	void Dir(const char *dir);
	void DoSync();
	
	RepoSync();
};


bool GetCredentials(const Ткст& url, const Ткст& dir, Ткст& username, Ткст& password);
void EditCredentials(const Вектор<Ткст>& url_hints);

void RunRepoDiff(const Ткст& filepath);
