#include "DinrusIDE.h"

#define LLOG(x) // DLOG(x)

void Иср::GotoPos(Ткст path, int line)
{
	LLOG("GotoPos " << path << ':' << line);
	if(path.дайСчёт()) {
		AddHistory();
		if(IsDesignerFile(path))
			DoEditAsText(path);
		EditFile(path);
	}
	editor.устКурсор(editor.дайПоз64(line - 1));
	editor.TopCursor(4);
	editor.устФокус();
	AddHistory();
}

Ткст PosFn(const Ткст& pkg, const Ткст& n)
{
	return Ткст() << фильтруй(pkg, [](int c) { return c == '\\' ? '/' : c; }) << '/' << n;
}

void Иср::CopyPosition()
{
	const РОбласть& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.дайСчёт(); i++) {
		const Пакет& pk = wspc.дайПакет(i);
		Ткст pkg = wspc[i];
		for(Ткст n : pk.file)
			if(путиРавны(SourcePath(pkg, n), editfile)) {
				WriteClipboardText(PosFn(pkg, n) << ":" << editor.GetCurrentLine());
				return;
			}
	}
	
	WriteClipboardText(дайИмяф(editfile) << ":" << editor.GetCurrentLine());
}

void Иср::GotoPosition()
{
	Ткст cs = ReadClipboardText();
	Ткст f;
	int line = 0;
	for(char c : ":( \t") {
		Ткст l;
		if(SplitTo(cs, c, f, l)) {
			f = обрежьОба(f);
			line = atoi(l);
			if(line)
				break;
		}
	}
	if(!line)
		return;
	f.замени("\\", "/");
	const РОбласть& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.дайСчёт(); i++) {
		const Пакет& pk = wspc.дайПакет(i);
		Ткст pkg = wspc[i];
		for(Ткст n : pk.file) {
			Ткст pf = PosFn(pkg, n);
			int q = f.дайСчёт() - pf.дайСчёт() - 1;
			if(pf == f || q >= 0 && f.заканчиваетсяНа(PosFn(pkg, n)) && f[q] == '/') {
				GotoPos(SourcePath(pkg, n), line);
				return;
			}
		}
	}
}

void Иср::GotoCpp(const CppItem& pos)
{
	GotoPos(GetSourceFilePath(pos.file), pos.line);
}

void Иср::CheckCodeBase()
{
	InvalidateFileTimeCache();
	InvalidateIncludes();
	CodeBaseSync();
}

void Иср::RescanCode()
{
/*
	ТаймСтоп tm;
	for(int i = 0; i < 10; i++)
		ReQualifyCodeBase();
	LOG(tm);
	вКонсоль(какТкст(tm));
//*/
//*
	InvalidateIncludes();
	сохраниФайл();
	ТаймСтоп t;
	console.очисть();
	RescanCodeBase();
	SyncRefsShowProgress = true;
	SyncRefs();
	editor.SyncNavigator();
//*/
}

void Иср::OpenTopic(const Ткст& topic, const Ткст& createafter, bool before)
{
	TopicLink tl = ParseTopicLink(topic);
	if(tl) {
		EditFile(приставьИмяф(PackageDirectory(tl.package), tl.группа + ".tpp"));
		if(designer) {
			TopicEditor *te = dynamic_cast<TopicEditor *>(&designer->DesignerCtrl());
			if(te)
				te->GoTo(tl.topic, tl.label, createafter, before);
		}
	}
}

void Иср::OpenTopic(const Ткст& topic)
{
	OpenTopic(topic, Ткст(), false);
}

void Иср::OpenATopic()
{
	Ткст t = doc.GetCurrent();
	if(!t.начинаетсяС("topic:"))
		return;
	OpenTopic(t);
}

void Иср::исрСлейФайл()
{
	FlushFile();
}

void Иср::IdeOpenTopicFile(const Ткст& file)
{
	EditFile(дайПапкуФайла(file));
	if(designer) {
		TopicEditor *te = dynamic_cast<TopicEditor *>(&designer->DesignerCtrl());
		if(te)
			te->GoTo(дайТитулф(file), "", "", false);
	}
}

struct FileStat {
	int  count;
	int  len;
	int  lines;
	int  oldest;
	int  newest;
	int  days;

	void добавь(const FileStat& a) {
		count += a.count;
		len += a.len;
		lines += a.lines;
		oldest = max(a.oldest, oldest);
		newest = min(a.newest, newest);
		days += a.days;
	}

	FileStat() { count = 0; len = lines = 0; oldest = 0; newest = INT_MAX; days = 0; }
};

Ткст StatLen(int len)
{
	return фмт("%d.%d KB", len >> 10, (len & 1023) / 103);
}

Ткст StatDate(int d)
{
	return Ткст().конкат() << d << " дней";
}

void sPut(const Ткст& имя, Ткст& qtf, const FileStat& fs)
{
	qtf << "::@W " << DeQtf(Nvl(имя, ".<none>"))
	    << ":: [> " << fs.count
	    << ":: " << fs.lines
	    << ":: " << (fs.count ? fs.lines / fs.count : 0)
	    << ":: " << StatLen(fs.len)
	    << ":: " << StatLen(fs.len ? fs.len / fs.count : 0)
	    << ":: " << StatDate(fs.oldest)
	    << ":: " << StatDate(fs.newest)
	    << ":: " << (fs.count ? fs.days / fs.count : 0) << " дней]";
}

void sPut(Ткст& qtf, МассивМап<Ткст, FileStat>& pfs, МассивМап<Ткст, FileStat>& all) {
	FileStat pall;
	for(int i = 0; i < pfs.дайСчёт(); i++) {
		FileStat& fs = pfs[i];
		sPut(pfs.дайКлюч(i), qtf, fs);
		pall.добавь(fs);
		all.дайДобавь(pfs.дайКлюч(i)).добавь(fs);
	}
	sPut("Все файлы", qtf, pall);
	qtf << "}}&&";
}


void ShowQTF(const Ткст& qtf, const char *title)
{
	RichText txt = ParseQTF(qtf);
	ClearClipboard();
	AppendClipboard(ParseQTF(qtf));

	WithStatLayout<ТопОкно> dlg;
	CtrlLayoutOK(dlg, title);
	dlg.stat = qtf;
	dlg.Sizeable().Zoomable();
	dlg.пуск();
}

void Иср::Licenses()
{
	Progress pi;
	const РОбласть& wspc = роблИср();
	pi.устВсего(wspc.дайСчёт());
	ВекторМап<Ткст, Ткст> license_package;
	for(int i = 0; i < wspc.дайСчёт(); i++) {
		Ткст n = wspc[i];
		pi.устТекст(n);
		if(pi.StepCanceled()) return;
		Ткст l = загрузиФайл(SourcePath(n, "Copying"));
		if(l.дайСчёт())
			MergeWith(license_package.дайДобавь(l), ", ", n);
	}
	if(license_package.дайСчёт() == 0) {
		Exclamation("Файлов лицензий ('Copying') не найдено.");
		return;
	}
	Ткст qtf;
	for(int i = 0; i < license_package.дайСчёт(); i++) {
		bool m = license_package[i].найди(',') >= 0;
		qtf << (m ? "Пакеты [* \1" : "Пакет [* \1")
		    << license_package[i]
		    << (m ? "\1] имеют" : "\1] имеет")
		    << " следующее лицензионное уведомление:&"
		    << "{{@Y [C1 " << DeQtf(license_package.дайКлюч(i)) << "]}}&&";
	}
	
	ShowQTF(qtf, "Лицензии");
}

void Иср::Statistics()
{
	Вектор< МассивМап<Ткст, FileStat> > stat;
	Progress pi;
	const РОбласть& wspc = роблИср();
	pi.устВсего(wspc.дайСчёт());
	Дата now = дайСисДату();
	for(int i = 0; i < wspc.дайСчёт(); i++) {
		const Пакет& pk = wspc.дайПакет(i);
		Ткст n = wspc[i];
		pi.устТекст(n);
		if(pi.StepCanceled()) return;
		МассивМап<Ткст, FileStat>& pfs = stat.добавь();
		for(int i = 0; i < pk.дайСчёт(); i++)
			if(!pk[i].separator) {
				Ткст file = SourcePath(n, pk[i]);
				if(файлЕсть(file)) {
					FileStat& fs = pfs.дайДобавь(дайРасшф(file));
					int d = minmax(now - дайВремяф(file), 0, 9999);
					fs.oldest = max(d, fs.oldest);
					fs.newest = min(d, fs.newest);
					Ткст data = загрузиФайл(file);
					for(const char *s = data; *s; s++)
						if(*s == '\n')
							fs.lines++;
					fs.len += data.дайСчёт();
					fs.days += d;
					fs.count++;
				}
			}
	}
	Ткст qtf = "[1 ";
	МассивМап<Ткст, FileStat> all;
	Ткст tab = "{{45:20:25:20:35:30:30:30:30@L [* ";
	Ткст hdr = "]:: [= Files:: Lines:: - avg.:: длина:: - avg.:: Oldest:: Newest:: Avg. age]";
	for(int i = 0; i < wspc.дайСчёт(); i++) {
		qtf << tab << DeQtf(wspc[i]) << hdr;
		sPut(qtf, stat[i], all);
	}

	qtf << tab << "Все пакеты" << hdr;
	sPut(qtf, all, all);

	ShowQTF(qtf, "Статистика");
}

Ткст FormatElapsedTime(double run)
{
	Ткст rtime;
	double hrs = floor(run / 3600);
	if(hrs > 0)
		rtime << фмт("%0n часов, ", hrs);
	int minsec = fround(run - 3600 * hrs);
	int min = minsec / 60, sec = minsec % 60;
	if(min || hrs)
		rtime << фмт("%d мин, ", min);
	rtime << фмт("%d сек", sec);
	return rtime;
}

void Иср::AlterText(ШТкст (*op)(const ШТкст& in))
{
	if(designer || !editor.выделение_ли() || editor.толькочтен_ли())
		return;
	editor.NextUndo();
	ШТкст w = editor.GetSelectionW();
	editor.удалиВыделение();
	int l = editor.дайКурсор();
	editor.Paste((*op)(w));
	editor.устВыделение(l, editor.дайКурсор64());
}

void Иср::TextToUpper()
{
	AlterText(РНЦП::взаг);
}

void Иср::TextToLower()
{
	AlterText(РНЦП::впроп);
}

void Иср::TextToAscii()
{
	AlterText(РНЦП::вАски);
}

void Иср::TextInitCaps()
{
	AlterText(РНЦП::иницШапки);
}

static ШТкст sSwapCase(const ШТкст& s)
{
	ШТкстБуф r;
	r.устСчёт(s.дайСчёт());
	for(int i = 0; i < s.дайСчёт(); i++)
		r[i] = проп_ли(s[i]) ? впроп(s[i]) : взаг(s[i]);
	return ШТкст(r);
}

void Иср::SwapCase()
{
	AlterText(sSwapCase);
}

static ШТкст sCString(const ШТкст& s)
{
	return какТкстСи(s.вТкст()).вШТкст();
}

void Иср::ToCString()
{
	AlterText(sCString);
}

static ШТкст sComment(const ШТкст& s) 
{
	return "/*" + s + "*/";
}

void Иср::ToComment()
{
	AlterText(sComment);
}

static ШТкст sCommentLines(const ШТкст& s)
{
	Ткст r;
	ТкстПоток ss(s.вТкст());
	for(;;) {
		Ткст line = ss.дайСтроку();
		if(ss.ошибка_ли())
			return s;
		else
		if(!line.проц_ли())
			r << "//" << line << "\n";
		if(ss.кф_ли())
			break;
	}
	return r.вШТкст();
}

void Иср::CommentLines()
{
	AlterText(sCommentLines);
}

static ШТкст sUncomment(const ШТкст& s) 
{
	ШТкст h = s;
	h.замени("/*", "");
	h.замени("//", "");
	h.замени("*/", "");
	return h;
}

void Иср::UnComment()
{
	AlterText(sUncomment);
}

void Иср::перефмтКоммент()
{
	editor.перефмтКоммент();
}

void Иср::Times()
{
	WithStatisticsLayout<ТопОкно> statdlg;
	CtrlLayout(statdlg, "Прошло времени");
	statdlg.ok.Ok();
	statdlg.ok << [&] { statdlg.Break(); };
	statdlg.устОбрвызВремени(-1000, statdlg.Breaker(IDRETRY), 50);
	do
	{
		int session_time = int(дайСисВремя() - start_time);
		int idle_time = int(session_time - editor.GetStatEditTime() - stat_build_time);
		statdlg.session_time <<= FormatElapsedTime(session_time);
		statdlg.edit_time    <<= FormatElapsedTime(editor.GetStatEditTime());
		statdlg.build_time   <<= FormatElapsedTime(stat_build_time);
		statdlg.idle_time    <<= FormatElapsedTime(idle_time);
	}
	while(statdlg.пуск() == IDRETRY);
}

ИНИЦБЛОК {
	региструйГлобКонфиг("svn-msgs");
}

void RepoSyncDirs(const Вектор<Ткст>& working)
{
	Ук<Ктрл> f = Ктрл::GetFocusCtrl();
	RepoSync repo;
	Ткст repocfg = конфигФайл("repo.cfg");
	repo.SetMsgs(загрузиФайл(repocfg));
	for(Ткст d : working)
		repo.Dir(d);
	repo.DoSync();
	сохраниФайл(repocfg, repo.GetMsgs());
	if(f)
		f->устФокус();
}

void Иср::SyncRepoDirs(const Вектор<Ткст>& working)
{
	сохраниФайл();
	RepoSyncDirs(working);
	ScanWorkspace();
	SyncWorkspace();
}

void Иср::SyncRepo(){
	Вектор<Ткст> d = RepoDirs();
	if(d.дайСчёт())
		SyncRepoDirs(d);
	else
		SyncRepoDirs(RepoDirs(true));
}

void Иср::SyncRepoDir(const Ткст& working)
{
	SyncRepoDirs(Вектор<Ткст>() << working);
}

void Иср::GotoDirDiffLeft(int line, DirDiffDlg *df)
{
	EditFile(df->GetLeftFile());
	editor.устКурсор(editor.дайПоз64(line));
	editor.устФокус();
}

void Иср::GotoDirDiffRight(int line, DirDiffDlg *df)
{
	EditFile(df->GetRightFile());
	editor.устКурсор(editor.дайПоз64(line));
	editor.устФокус();
}

void Иср::DoDirDiff()
{
	Индекс<Ткст> dir;
	Вектор<Ткст> d = GetUppDirs();
	for(int i = 0; i < d.дайСчёт(); i++)
		dir.найдиДобавь(d[i]);
	ФайлПоиск ff(конфигФайл("*.bm"));
	while(ff) {
		ВекторМап<Ткст, Ткст> var;
		LoadVarFile(ff.дайПуть(), var);
		Вектор<Ткст> p = разбей(var.дай("РНЦП", Ткст()), ';');
		for(int i = 0; i < p.дайСчёт(); i++)
			dir.найдиДобавь(p[i]);
		ff.следщ();
	}
	Ткст n = дайПапкуФайла(editfile);
	if(n.дайСчёт())
		dir.найдиДобавь(n);
	SortIndex(dir);
	
	static DirDiffDlg dlg;
	dlg.diff.WhenLeftLine = THISBACK1(GotoDirDiffLeft, &dlg);
	dlg.diff.WhenRightLine = THISBACK1(GotoDirDiffRight, &dlg);
	for(int i = 0; i < dir.дайСчёт(); i++) {
		dlg.Dir1AddList(dir[i]);
		dlg.Dir2AddList(dir[i]);
	}
	if(d.дайСчёт())
		dlg.Dir1(d[0]);
	if(!dlg.открыт()) {
		dlg.устШрифт(veditorfont);
		dlg.разверни();
		dlg.Титул("Сравнить папки");
		dlg.откройГлавн();
	}
	else
		dlg.устФокус();
}

void Иср::DoPatchDiff()
{
	Ткст patch = SelectFileOpen("Пропатчить файлы (*.diff *.patch)\t*.diff *.patch\nВсе файлы\t*.*");
	if(пусто_ли(patch))
		return;
	Индекс<Ткст> dir;
	if(editfile.дайСчёт())
		dir.добавь(дайПапкуФайла(editfile));
	Вектор<Ткст> d = GetUppDirs();
	for(int i = 0; i < d.дайСчёт(); i++)
		dir.найдиДобавь(d[i]);
	const РОбласть& wspc = роблИср();
	for(int i = 0; i < wspc.дайСчёт(); i++)
		dir.найдиДобавь(дайПапкуФайла(PackagePath(wspc[i])));
	static PatchDiff dlg;
	dlg.diff.WhenLeftLine = THISBACK1(GotoDirDiffLeft, &dlg);
	if(!dlg.открыт()) {
		dlg.устШрифт(veditorfont);
		dlg.разверни();
		if(dlg.открой(patch, dir.дайКлючи()))
			dlg.откройГлавн();
	}
	else
		dlg.устФокус();
}

void Иср::AsErrors()
{
	ClearErrorsPane();
	устНиз(BERRORS);
	Ткст s = editor.выделение_ли() ? editor.дайВыделение() : editor.дай();
	ТкстПоток ss(s);
	while(!ss.кф_ли())
		ConsoleLine(ss.дайСтроку(), true);
	SetErrorEditor();
}

void Иср::RemoveDs()
{
	if(designer || editor.толькочтен_ли())
		return;
	static Индекс<Ткст> ds = { "DLOG", "DDUMP", "DDUMPC", "DDUMPM", "DTIMING",
	                            "DLOGHEX", "DDUMPHEX", "DTIMESTOP", "DHITCOUNT" };
	editor.NextUndo();
	int l = 0;
	int h = editor.дайСчётСтрок() - 1;
	int ll, hh;
	if(editor.дайВыделение(ll, hh)) {
		l = editor.дайСтроку(ll);
		h = editor.дайСтроку(hh);
	}
	for(int i = h; i >= l; i--) {
		Ткст ln = editor.дайУтф8Строку(i);
		try {
			СиПарсер p(ln);
			if(p.ид_ли()) {
				Ткст id = p.читайИд();
				if(ds.найди(id) >= 0 && p.сим('(')) {
					int pos = editor.дайПоз(i);
					int end = min(editor.дайДлину(), editor.дайПоз(i) + editor.дайДлинуСтроки(i) + 1);
					editor.удали(editor.дайПоз(i), end - pos);
				}
			}
		}
		catch(СиПарсер::Ошибка) {}
	}
	editor.GotoLine(l);
}

void Иср::LaunchAndroidSDKManager(const AndroidSDK& androidSDK)
{
	Хост host;
	CreateHost(host, darkmode, disable_uhd);
	IGNORE_RESULT(host.выполни(androidSDK.GetLauchSDKManagerCmd()));
}

void Иср::LaunchAndroidAVDManager(const AndroidSDK& androidSDK)
{
	Хост host;
	CreateHost(host, darkmode, disable_uhd);
	IGNORE_RESULT(host.выполни(androidSDK.GetLauchAVDManagerCmd()));
}

void Иср::LauchAndroidDeviceMonitor(const AndroidSDK& androidSDK)
{
	Хост host;
	CreateHost(host, darkmode, disable_uhd);
	IGNORE_RESULT(host.выполни(androidSDK.MonitorPath()));
}
