#include "DinrusIDE.h"

const char tempaux[] = "<temp-aux>";
const char prjaux[] = "<prj-aux>";
const char ideaux[] = "<ide-aux>";

Рисунок OverLtRed(const Рисунок& m)
{
	Рисунок red = CreateImage(m.дайРазм(), смешай(светлоКрасный, SColorPaper));
	Over(red, Точка(0, 0), m, m.дайРазм());
	return red;
}

Рисунок ImageOverRed(const Рисунок& m)
{
	return MakeImage(m, OverLtRed);
}

Шрифт WorkspaceWork::ListFont()
{
	return StdFont();
};

void WorkspaceWork::SetErrorFiles(const Вектор<Ткст>& files)
{
	errorfiles <<= Индекс<Ткст>(files, 1);
	int i = filelist.дайКурсор();
	int s = filelist.GetSbPos();
	SaveLoadPackage(false);
	filelist.SetSbPos(s);
	filelist.устКурсор(i);
	SyncErrorPackages();
}

Ткст WorkspaceWork::PackagePathA(const Ткст& pn) {
	if(pn == prjaux) {
		Ткст nm;
		Ткст cfg = конфигФайл("cfg");
		for(const char *s = main; *s; s++)
			nm.конкат(*s == '\\' || *s == '/' ? '$' : *s);
		реализуйДир(cfg);
		return приставьИмяф(cfg, форсируйРасш(nm + '@' + GetVarsName(), ".aux"));
	}
	if(pn == ideaux)
		return конфигФайл("ide.aux");
	if(pn == tempaux)
		return конфигФайл(спринтф("aux%x.tmp",
#ifdef PLATFORM_WIN32
		          GetCurrentProcessId()
#endif
#ifdef PLATFORM_POSIX
		          getpid()
#endif
		       ));
	if(pn == METAPACKAGE)
		return Null;
	return PackagePath(pn);
}


void WorkspaceWork::SyncErrorPackages()
{
	for(int i = 0; i < package.дайСчёт(); i++) {
		СписокФайлов::Файл f = package.дай(i);
		if(!IsAux(f.имя)) {
			СписокФайлов::Файл ff = f;
			Ткст path = дайПапкуФайла(PackagePath(f.имя));
		#ifdef PLATFORM_WIN32
			path = впроп(path);
		#endif
			ff.icon = i ? IdeImg::Package() : IdeImg::MainPackage();
			ff.underline = Null;
			for(int q = 0; q < errorfiles.дайСчёт(); q++) {
				if(errorfiles[q].начинаетсяС(path)) {
					ff.icon = ImageOverRed(ff.icon);
					ff.underline = светлоКрасный;
					break;
				}
			}
			ff.icon = DPI(ff.icon);
			package.уст(i, ff);
		}
	}
}

struct PackageOrder {
	Ткст mainpath;
	
	int GetMatchLen(const Ткст& x) const {
		if(*x == '<')
			return 0;
		Ткст h = PackagePath(x);
		for(int i = 0; i < mainpath.дайСчёт(); i++)
			if(mainpath[i] != h[i])
				return i;
		return mainpath.дайСчёт();
	}
	
	bool operator()(const Ткст& p1, const Ткст& p2) const {
		int l1 = GetMatchLen(p1);
		int l2 = GetMatchLen(p2);
		return l1 != l2 ? l1 > l2 : p1 < p2;
	}
};

void WorkspaceWork::ScanWorkspace() {
	РОбласть wspc;
	if(main.дайСчёт())
		wspc.скан(main);
	actualpackage.очисть();
	actualfileindex = -1;
	filelist.очисть();
	package.очисть();
	Вектор<Ткст> pks;
	for(int i = 0; i < wspc.package.дайСчёт(); i++)
		pks.добавь(wspc.package.дайКлюч(i));
	if(sort && wspc.дайСчёт()) {
		PackageOrder po;
		po.mainpath = PackagePath(pks[0]);
		сортируй(СубДиапазон(pks, 1, pks.дайСчёт() - 1), po);
	}
	for(int i = 0; i < wspc.package.дайСчёт(); i++) {
		Ткст pk = pks[i];
		Шрифт fnt = ListFont();
		if(i == 0)
			fnt.Bold();
		PackageInfo pi = GetPackageInfo(pk);
		if(pi.bold)
			fnt.Bold();
		if(pi.italic)
			fnt.Italic();
		package.добавь(pk, Null, fnt, Nvl(AdjustIfDark(pi.ink), SColorText()), false, 0, Null, SColorMark);
	}
	if(!organizer) {
		if(main.дайСчёт())
			package.добавь(prjaux, IdeImg::PrjAux(), ListFont(), AdjustIfDark(магента));
		package.добавь(ideaux, IdeImg::IdeAux(), ListFont(), AdjustIfDark(магента));
		package.добавь(tempaux, IdeImg::TempAux(), ListFont(), AdjustIfDark(магента));
		if(main.дайСчёт())
			package.добавь(METAPACKAGE, IdeImg::Meta(), ListFont(), AdjustIfDark(красный));
	}
	package.устКурсор(0);
	
	SyncErrorPackages();
}

void WorkspaceWork::SavePackage()
{
	if(пусто_ли(actualpackage) || actualpackage == METAPACKAGE)
		return;
	InvalidatePackageInfo(actualpackage);
	Ткст pp = PackagePathA(actualpackage);
	if(organizer && backup.найди(pp) < 0) {
		Backup& b = backup.добавь(pp);
		ФайлПоиск ff(pp);
		if(ff) {
			b.time = ff.дайВремяПоследнЗаписи();
			b.data = загрузиФайл(pp);
		}
		else
			b.data = Ткст::дайПроц();
	}
	if(файлЕсть(pp) || actual.дайСчёт())
		actual.сохрани(pp);
}

void WorkspaceWork::RestoreBackup()
{
	for(int i = 0; i < backup.дайСчёт(); i++) {
		Backup& b = backup[i];
		Ткст фн = backup.дайКлюч(i);
		if(b.data.проц_ли())
			DeleteFile(фн);
		else {
			сохраниФайл(фн, b.data);
			устФВремя(фн, b.time);
		}
	}
}

void WorkspaceWork::SaveLoadPackageNS(bool sel)
{
	SavePackage();
	Ткст p = actualpackage;
	Ткст f;
	if(IsActiveFile())
		f = ActiveFile();
	int psc = package.GetSbPos();
	int fsc = filelist.GetSbPos();
	ScanWorkspace();
	package.SetSbPos(psc);
	package.FindSetCursor(p);
	if (sel) {
		filelist.SetSbPos(fsc);
		filelist.FindSetCursor(f);
	}
}

void WorkspaceWork::SaveLoadPackage(bool sel)
{
	SaveLoadPackageNS(sel);
	SyncWorkspace();
}

bool PathIsLocal(const Ткст& path)
{
#ifdef PLATFORM_WIN32
	char drive[4] = "?:\\";
	*drive = *path;
	return GetDriveType(drive) == DRIVE_FIXED;
#else
	return false;
#endif
}

void WorkspaceWork::LoadActualPackage()
{
#ifdef PLATFORM_WIN32
	static BOOL (WINAPI *PathIsNetworkPathA)(LPCSTR pszPath);
	ONCELOCK {
		фнДлл(PathIsNetworkPathA, "Shlwapi.dll", "PathIsNetworkPathA");
	}
#endif

	Время utime = дайВремяф(конфигФайл("version"));
	filelist.очисть();
	fileindex.очисть();
	bool open = true;
	Время tm = дайСисВремя();
	for(int i = 0; i < actual.file.дайСчёт(); i++) {
		Пакет::Файл& f = actual.file[i];
		if(f.separator) {
			open = closed.найди(Sepfo(actualpackage, f)) < 0;
			filelist.добавь(f, open ? IdeImg::SeparatorClose() : IdeImg::SeparatorOpen(),
			             ListFont().Bold(), open ? SColorMark : SColorText, true, 0, Null);
			fileindex.добавь(i);
		}
		else
		if(open) {
			Цвет uln = Null;
			Ткст p = SourcePath(GetActivePackage(), f);
			if(showtime && (findarg(actualpackage, "<ide-aux>", "<prj-aux>", "<temp-aux>") < 0 || PathIsLocal(p))
	#ifdef PLATFORM_WIN32
			   && !(PathIsNetworkPathA && PathIsNetworkPathA(p))
	#endif
			) {
				ФайлПоиск ff(p);
				if(ff) {
					Время ftm = Время(ff.дайВремяПоследнЗаписи());
					if(ftm > utime) {
						int64 t = tm - ftm;
						if(t < 24 * 3600)
							uln = SColorMark;
						else
						if(t < 32 * 24 * 3600)
							uln = SColorDisabled;
					}
				}
			}
			Рисунок m = IdeFileImage(f, false, f.pch);
			if(дайРасшф(p) == ".tpp" && папка_ли(p)) {
				if(файлЕсть(приставьИмяф(p, "all.i")))
					m = TopicImg::IGroup();
				else
					m = TopicImg::Group();
			}
		#ifdef PLATFORM_WIN32
			p = впроп(p);
		#endif
			if(errorfiles.найди(p) >= 0) {
				m = ImageOverRed(m);
				uln = светлоКрасный;
			}
			filelist.добавь(f, m, ListFont(), SColorText, false, 0,
			             Null, SColorMark, Null, Null, Null, uln);
			fileindex.добавь(i);
		}
	}
}

void WorkspaceWork::TouchFile(const Ткст& path)
{
	if(!showtime)
		return;
	Ткст n = дайИмяф(path);
	for(int i = 0; i < filelist.дайСчёт(); i++) {
		СписокФайлов::Файл f = filelist[i];
		if(f.имя == n && path == SourcePath(GetActivePackage(), f.имя))
			filelist.уст(i, f.имя, f.icon, f.font, f.ink, false, 0,
			             Null, SColorMark, Null, Null, Null, SColorMark);
	}
}

void WorkspaceWork::Fetch(Пакет& p, const Ткст& pkg)
{
	if(pkg.пустой()) return;
	if(pkg == METAPACKAGE) {
		p.file.очисть();
		p.file.добавьПодбор(Пакет::Файл(Ткст(HELPNAME)));
		p.file.добавьПодбор(Пакет::Файл(конфигФайл("global.defs")));
		for(Ткст d : GetUppDirs()) {
			Пакет::Файл sep(дайИмяф(d));
			sep.separator = true;
			p.file.добавьПодбор(pick(sep));
			p.file.добавьПодбор(Пакет::Файл(приставьИмяф(d, "_.tpp")));
			for(Ткст f : { "readme", "license", "copying" }) {
				for(int u = 0; u < 4; u++) {
					ФайлПоиск ff(приставьИмяф(d, (u & 1 ? взаг(f) : f) + (u & 2 ? ".md" : "")));
					if(ff) {
						p.file.добавьПодбор(Пакет::Файл(ff.дайПуть()));
						break;
					}
				}
			}
		}
	}
	else {
		Ткст pp = PackagePathA(pkg);
		p.грузи(pp);
	}
}

void WorkspaceWork::PackageCursor()
{
	InvalidatePackageCache();
	filelist.WhenBar.очисть();
	actualpackage = GetActivePackage();
	repo_dirs = false;
	Fetch(actual, actualpackage);
	LoadActualPackage();
	filelist.вкл();
	if(actualpackage != METAPACKAGE)
		filelist.WhenBar = THISBACK(FileMenu);
	repo_dirs = RepoDirs(true).дайСчёт();
}

Вектор<Ткст> WorkspaceWork::RepoDirs(bool actual)
{
	Вектор<Ткст> u = GetUppDirs();
	for(Ткст& s : u)
		s = нормализуйПуть(s);
	Индекс<Ткст> id;
	const РОбласть& w = GetIdeWorkspace();
	for(int i = 0; i < w.дайСчёт(); i++) {
		Ткст pp = PackagePath(w[i]);
		for(Ткст s : u)
			if(pp.начинаетсяС(s))
				id.найдиДобавь(s);
	}
	
	Вектор<Ткст> d = id.подбериКлючи();
	
	if (actual && !IsAux())
		d.вставь(0, дайПапкуФайла(PackagePath(actualpackage)));
	Вектор<Ткст> r;
	for(int i = 0; i < d.дайСчёт(); i++)
		if(GetRepoKind(d[i]))
			r.добавь(d[i]);
	return r;
}

void WorkspaceWork::FileCursor()
{
	int i = filelist.дайКурсор();
	actualfileindex = -1;
	if(i >= 0 && i < fileindex.дайСчёт())
		actualfileindex = fileindex[i];
}

Ткст WorkspaceWork::FileName(int i) const
{
	return i >= 0 && i < fileindex.дайСчёт() ? (Ткст)actual.file[fileindex[i]] : Null;
}

bool WorkspaceWork::IsSeparator(int i) const
{
	return i >= 0 && i < fileindex.дайСчёт() ? actual.file[fileindex[i]].separator : true;
}

Ткст WorkspaceWork::GetActiveFileName() const
{
	return FileName(filelist.дайКурсор());
}

Ткст WorkspaceWork::GetActiveFilePath() const
{
	return SourcePath(GetActivePackage(), GetActiveFileName());
}

bool   WorkspaceWork::IsActiveFile() const
{
	int i = filelist.дайКурсор();
	return i >= 0 && i < fileindex.дайСчёт() && fileindex[i] < actual.file.дайСчёт();
}

Пакет::Файл& WorkspaceWork::ActiveFile()
{
	return actual.file[fileindex[filelist.дайКурсор()]];
}

void WorkspaceWork::AddFile(ADDFILE af)
{
	Ткст active = GetActivePackage();
	if(active.пустой()) return;
	FileSel *fs = &OutputFs();
	реализуйДир(GetLocalDir());
	switch(af)
	{
	case PACKAGE_FILE: fs = &BasedSourceFs(); fs->BaseDir(дайПапкуФайла(PackagePathA(active))); break;
	case ANY_FILE:     fs = &AnySourceFs(); break;
	case OUTPUT_FILE:  fs->ActiveDir(GetOutputDir()); break;
	case CONFIG_FILE:  fs->ActiveDir(GetConfigDir()); break;
	case HOME_FILE:    fs->ActiveDir(дайДомПапку()); break;
	case LOCAL_FILE:   fs->ActiveDir(GetLocalDir()); break;
	default: ; // GCC warns otherwise
	}
	if(!fs->ExecuteOpen("Добавить файлы в пакет..")) return;
	int fci = filelist.дайКурсор();
	int cs = filelist.GetSbPos();
	int ci = fci >= 0 && fci < fileindex.дайСчёт() ? fileindex[fci] : -1;
	for(int i = 0; i < fs->дайСчёт(); i++) {
		Пакет::Файл& f = ci >= 0 ? actual.file.вставь(ci++) : actual.file.добавь();
		f = (*fs)[i];
		f.readonly = fs->GetReadOnly();
	}
	SaveLoadPackage(false);
	filelist.SetSbPos(cs);
	filelist.устКурсор(fci >= 0 ? fci : filelist.дайСчёт() - 1);
	FileSelected();
}

void WorkspaceWork::добавьЭлт(const Ткст& имя, bool separator, bool readonly)
{
	int fci = filelist.дайКурсор();
	int cs = filelist.GetSbPos();
	int ci = fci >= 0 && fci < fileindex.дайСчёт() ? fileindex[fci] : -1;
	Пакет::Файл& f = ci >= 0 ? actual.file.вставь(ci) : actual.file.добавь();
	f = имя;
	f.separator = separator;
	f.readonly = readonly;
	if(separator)
		SaveLoadPackageNS(false);
	else
		SaveLoadPackage(false);
	filelist.SetSbPos(cs);
	filelist.устКурсор(fci >= 0 ? fci : filelist.дайСчёт() - 1);
	FileSelected();
}

void WorkspaceWork::добавьСепаратор()
{
	Ткст active = GetActivePackage();
	if(active.пустой()) return;
	Ткст имя;
	if(!редактируйТекст(имя, "Добавить сепаратор", "Имя"))
		return;
	добавьЭлт(~имя, true, true);
}

class ImportDlg : public WithImportLayout<ТопОкно> {
	typedef ImportDlg ИМЯ_КЛАССА;

	ФреймПраво<Кнопка> dir;
	
	void SetFolder();

public:
	ImportDlg();
};

void ImportDlg::SetFolder()
{
	if(!AnySourceFs().ExecuteSelectDir()) return;
	folder <<= ~AnySourceFs();
}

ImportDlg::ImportDlg()
{
	CtrlLayoutOKCancel(*this, "Импортировать дерево папки в пакет");
	folder.добавьФрейм(dir);
	dir <<= THISBACK(SetFolder);
	dir.устРисунок(CtrlImg::smallright()).NoWantFocus();
	files <<= "*.cpp *.h *.hpp *.c *.C *.cxx *.cc";
}

bool FileOrder_(const Ткст& a, const Ткст& b)
{
	return stricmp(a, b) < 0;
}

void WorkspaceWork::DoImportTree(const Ткст& dir, const Ткст& mask, bool sep, Progress& pi, int from)
{
	Ткст active = GetActivePackage();
	if(active.пустой()) return;
	ФайлПоиск ff(приставьИмяф(dir, "*.*"));
	Вектор<Ткст> dirs, files;
	while(ff) {
		Ткст p = приставьИмяф(dir, ff.дайИмя());
		if(ff.файл_ли() && PatternMatchMulti(mask, ff.дайИмя()))
			files.добавь(p);
		if(ff.папка_ли())
			dirs.добавь(p);
		ff.следщ();
	}
	Ткст relPath(dir.середина(from)),
		absPath = SourcePath(active, relPath);
	if(sep && files.дайСчёт()) {
		if(!дирЕсть(absPath))
			if(!реализуйДир(absPath))
				throw фмт("Ошибка при создании папки:&\1%s", absPath);
		Пакет::Файл& f = actual.file.добавь();
		f = relPath;
		f.separator = f.readonly = true;
	}
	сортируй(files, &FileOrder_);
	сортируй(dirs, &FileOrder_);
	for(int i = 0; i < files.дайСчёт(); i++) {
		if(pi.StepCanceled())
			throw Ткст();
		Ткст имя = дайИмяф(files[i]);
		if(копируйфл(files[i], приставьИмяф(absPath, имя))) {
			Пакет::Файл& f = actual.file.добавь();
			f = приставьИмяф(relPath, имя);
			f.separator = f.readonly = false;
		}
		else
			throw фмт("Ошибка при копировании файла:&\1%s", files[i]);
	}
	for(int i = 0; i < dirs.дайСчёт(); i++)
		DoImportTree(dirs[i], mask, true, pi, from);
}

void WorkspaceWork::DoImportTree(const Ткст& dir, const Ткст& mask, bool sep, Progress& pi)
{
	int from = dir.заканчиваетсяНа(какТкст(DIR_SEP)) ? dir.дайСчёт() : dir.дайСчёт() + 1;
	DoImportTree(dir, mask, sep, pi, from);
}

void WorkspaceWork::DoImport(const Ткст& dir, const Ткст& mask, bool sep, Progress& pi)
{
	Ткст active = GetActivePackage();
	if(active.пустой()) return;
	ФайлПоиск ff(приставьИмяф(dir, "*.*"));
	Вектор<Ткст> dirs, files;
	while(ff) {
		Ткст p = приставьИмяф(dir, ff.дайИмя());
		if(ff.файл_ли() && PatternMatchMulti(mask, ff.дайИмя()))
			files.добавь(p);
		if(ff.папка_ли())
			dirs.добавь(p);
		ff.следщ();
	}
	if(sep && files.дайСчёт()) {
		Пакет::Файл& f = actual.file.добавь();
		f = дайТитулф(dir);
		f.separator = f.readonly = true;
	}
	сортируй(files, &FileOrder_);
	сортируй(dirs, &FileOrder_);
	for(int i = 0; i < files.дайСчёт(); i++) {
		if(pi.StepCanceled())
			throw Ткст();
		Ткст имя = дайИмяф(files[i]);
		if(копируйфл(files[i], SourcePath(active, имя))) {
			Пакет::Файл& f = actual.file.добавь();
			f = имя;
			f.separator = f.readonly = false;
		}
		else
			throw фмт("Ошибка при копировании файла:&\1%s", files[i]);
	}
	for(int i = 0; i < dirs.дайСчёт(); i++)
		DoImport(dirs[i], mask, true, pi);
}

void WorkspaceWork::DoImport(const Ткст& dir, const Ткст& mask, bool sep, Progress& pi, bool tree)
{
	if(tree)
		DoImportTree(dir, mask, sep, pi);
	else
		DoImport(dir, mask, sep, pi);
}

void WorkspaceWork::Import()
{
	Ткст active = GetActivePackage();
	if(active.пустой()) return;
	ImportDlg dlg;
	if(dlg.выполни() != IDOK)
		return;
	Progress pi("Иммпортируется файл %d");
	int fci = filelist.дайКурсор();
	int cs = filelist.GetSbPos();
	try {
		DoImport(~dlg.folder, ~dlg.files, false, pi, ~dlg.tree);
	}
	catch(Ткст msg) {
		if(!msg.пустой())
			Exclamation(msg);
	}
	SaveLoadPackage(false);
	filelist.SetSbPos(cs);
	filelist.устКурсор(fci >= 0 ? fci : filelist.дайСчёт() - 1);
	FileSelected();
}

Ткст TppName(const Ткст& s)
{
	if(s == "src")
		return "Справка - src";
	if(s == "srcdoc")
		return "Документы - srcdoc";
	if(s == "srcimp")
		return "Реализация - srcimp";
	return s;
}

class Tpp : public WithTppLayout<ТопОкно> {
public:
	void синх() {
		bool en = группа.курсор_ли() && пусто_ли(группа.дайКлюч());
		name_lbl.вкл(en);
		имя.вкл(en);
		name_tpp.вкл(en);
	}

	void грузи(const char *dir)
	{
		Индекс<Ткст> exist;
		ФайлПоиск ff(приставьИмяф(dir, "*.tpp"));
		while(ff) {
			if(ff.папка_ли()) {
				Ткст s = дайТитулф(ff.дайИмя());
				группа.добавь(s, AttrText(TppName(s)).устШрифт(StdFont().Bold()));
				exist.добавь(s);
			}
			ff.следщ();
		}
		static const char *h[4] = { "src.tpp", "srcdoc.tpp", "srcimp.tpp", "app.tpp" };
		for(int i = 0; i < __countof(h); i++) {
			Ткст s = дайТитулф(h[i]);
			if(exist.найди(s) < 0)
				группа.добавь(s, TppName(s) + " (new)");
		}
		группа.добавь(Null, "<other new>");
		группа.идиВНач();
	}

	Ткст дайИмя()
	{
		Ткст s;
		if(группа.курсор_ли()) {
			s = группа.дайКлюч();
			if(пусто_ли(s))
				s << ~имя;
			s << ".tpp";
		}
		return s;
	}

	typedef Tpp ИМЯ_КЛАССА;

	Tpp() {
		CtrlLayoutOKCancel(*this, "Вставить группу тематик");
		группа.добавьКлюч();
		группа.добавьКолонку("Группа");
		группа.WhenSel = THISBACK(синх);
		имя.устФильтр(CharFilterAlpha);
	}
};

void WorkspaceWork::AddTopicGroup()
{
	Ткст package = GetActivePackage();
	if(пусто_ли(package)) return;
	Tpp dlg;
	dlg.грузи(PackageDirectory(package));
	if(dlg.пуск() != IDOK) return;
	Ткст g = dlg.дайИмя();
	if(g == "app.tpp") {
		Ткст h = SourcePath(package, g);
		реализуйДир(h);
		сохраниФайл(приставьИмяф(h, "all.i"), "");
	}
	if(g.дайСчёт())
		добавьЭлт(g, false, false);
}

void WorkspaceWork::RemoveFile()
{
	int i = filelist.дайКурсор();
	int s = filelist.GetSbPos();
	bool separator = false;
	if(i >= 0 && i < fileindex.дайСчёт()) {
		int fx = fileindex[i];
		separator = actual.file[fx].separator;
		if(separator && closed.найди(GetActiveSepfo()) >= 0) {
			int px = fx;
			while(--px >= 0 && !actual.file[fx].separator)
				;
			if(px >= 0) {
				int c = closed.найди(Sepfo(GetActivePackage(), actual.file[px]));
				if(c >= 0)
					closed.отлинкуй(c);
			}
		}
		actual.file.удали(fx);
	}
	if(separator || IsAux())
		SaveLoadPackageNS(false);
	else
		SaveLoadPackage(false);

	if (separator || удалиФайл()) {
		filelist.SetSbPos(s);
		filelist.устКурсор(i);
	}
}

void WorkspaceWork::DelFile()
{
	if(!filelist.курсор_ли() || filelist[filelist.дайКурсор()].isdir) return;
	Ткст file = GetActiveFilePath();
	if(папка_ли(file)) {
		if(!PromptYesNo("Удалить группу тематик и скинуть ВСЕ тематики?")) return;
		RemoveFile();
		DeleteFolderDeep(file);
	}
	else {
		if(!PromptYesNo("Удалить файл из пакета и скинуть его?")) return;
		RemoveFile();
		::DeleteFile(file);
	}
}

void WorkspaceWork::RenameFile()
{
	if(!filelist.курсор_ли()) return;
	Ткст n = GetActiveFileName();
	int i = filelist.дайКурсор();
	if(i < 0 || i >= fileindex.дайСчёт())
		return;
	int ii = fileindex[i];
	WithEditStringLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, "Переименовать файл");
	dlg.lbl = "Новое имя";
	dlg.text <<= n;
	dlg.открой();
	dlg.text.устФокус();
	int l = int(дайПозИмяф(n) - ~n);
	int h = int(дайПозРасшф(n) - ~n);
	if(l >= h)
		l = 0;
	dlg.text.устВыделение(l, h);
	if(dlg.пуск() != IDOK)
		return;
	n = ~dlg.text;
	Ткст spath = GetActiveFilePath();
	Ткст dpath = SourcePath(GetActivePackage(), n);
	if(!filelist[i].isdir && дайДлинуф(spath) >= 0) {
		if(!::MoveFile(spath, dpath)) {
			Exclamation("Не удалось переименовать файл.&&" + дайОшСооб(GetLastError()));
			return;
		}
	}
	переименуйФайл(dpath);
	int s = filelist.GetSbPos();
	(Ткст &)actual.file[ii] = n;
	SaveLoadPackage(false);
	filelist.SetSbPos(s);
	filelist.устКурсор(i);
	if(дайРасшф(spath) == ".iml" || дайРасшф(dpath) == ".iml")
		SyncWorkspace();
}

WorkspaceWork::Sepfo WorkspaceWork::GetActiveSepfo()
{
	return Sepfo(GetActivePackage(), GetActiveFileName());
}

void WorkspaceWork::GroupOrFile(Точка pos)
{
	if(pos.x < filelist.GetIconWidth())
		Группа();
	if(filelist.курсор_ли() && !filelist[filelist.дайКурсор()].isdir)
		FileSelected();
}

void   WorkspaceWork::Группа()
{
	if(!filelist.курсор_ли() || !filelist[filelist.дайКурсор()].isdir)
		return;
	int i = filelist.дайКурсор();
	int s = filelist.GetSbPos();
	Sepfo as = GetActiveSepfo();
	if(closed.найди(as) >= 0)
		closed.отлинкуйКлюч(as);
	else
		closed.помести(as);
	SaveLoadPackage(false);
	filelist.SetSbPos(s);
	filelist.устКурсор(i);
}

void WorkspaceWork::OpenAllGroups()
{
	for(int i = 0; i < actual.file.дайСчёт(); i++)
		if(actual.file[i].separator)
			closed.отлинкуйКлюч(Sepfo(GetActivePackage(), actual.file[i]));
	SaveLoadPackage();
}

void WorkspaceWork::CloseAllGroups()
{
	for(int i = 0; i < actual.file.дайСчёт(); i++)
		if(actual.file[i].separator)
			closed.найдиПомести(Sepfo(GetActivePackage(), actual.file[i]));
	SaveLoadPackage();
}

void  WorkspaceWork::ShowFile(int pi)
{
	bool open = true;
	Sepfo sf;
	for(int i = 0; i < actual.file.дайСчёт(); i++) {
		if(actual.file[i].separator) {
			sf = Sepfo(GetActivePackage(), actual.file[i]);
			open = closed.найди(sf) < 0;
		}
		else
		if(i == pi) {
			if(!open) {
				int i = filelist.дайКурсор();
				int s = filelist.GetSbPos();
				closed.отлинкуйКлюч(sf);
				SaveLoadPackage(false);
				filelist.SetSbPos(s);
				filelist.устКурсор(i);
			}
			return;
		}
	}
}

bool WorkspaceWork::IsAux(const Ткст& p)
{
	return p == tempaux || p == prjaux || p == ideaux || p == METAPACKAGE;
}

bool WorkspaceWork::IsAux()
{
	return IsAux(actualpackage);
}

bool WorkspaceWork::IsMeta()
{
	return actualpackage == METAPACKAGE;
}

void WorkspaceWork::InsertSpecialMenu(Бар& menu)
{
	bool isaux = IsAux();
	menu.добавь("Вставить файл(ы)..", THISBACK1(AddFile, ANY_FILE))
		.Ключ(K_SHIFT|K_CTRL_I)
		.Help("Вставить файлы с диска (вредно для портабельных пакетов)");
	menu.добавь(isaux && GetOutputDir().дайСчёт(), "Вставить файл(ы) из папки вывода..", THISBACK1(AddFile, OUTPUT_FILE))
		.Help("Открыть файлвыборку в папке вывода / промежуточной папке для текущего пакета");
#ifdef PLATFORM_POSIX
	menu.добавь(isaux && GetConfigDir().дайСчёт(), "Вставить файл(ы) конфигурации папки..", THISBACK1(AddFile, CONFIG_FILE))
		.Help("Открыть файлвыборку в папке вывода / промежуточной папке для текущего пакета");
#endif
	menu.добавь(isaux, "Вставить файл(ы) из локальной папки..", THISBACK1(AddFile, LOCAL_FILE))
		.Help("Открыть файлвыборку в локальной папке для текущего пакета");
	menu.добавь("Вставить файл(ы) из домашней папки..", THISBACK1(AddFile, HOME_FILE))
		.Help("Открыть файлвыборку в папке HOME текущего пользователя");
}

void WorkspaceWork::SpecialFileMenu(Бар& menu)
{
	InsertSpecialMenu(menu);
	menu.добавь("Импортировать исходники из дерева папки..", THISBACK(Import));
}

void WorkspaceWork::OpenFileFolder()
{
	ShellOpenFolder(дайДиректориюФайла(GetActiveFilePath()));
}

void WorkspaceWork::OpenPackageFolder()
{
	ShellOpenFolder(дайДиректориюФайла(GetActivePackagePath()));
}

void WorkspaceWork::FileMenu(Бар& menu)
{
	bool sel = filelist.курсор_ли() && filelist[filelist.дайКурсор()].isdir;
	
	bool isaux = IsAux();
	if(isaux)
		InsertSpecialMenu(menu);
	else {
		menu.добавь("Новый файл в пакете..", IdeCommonImg::PageAdd(), [=] { NewPackageFile(); });
		menu.добавь(!isaux, "Вставить файл(Ы) из папки пакета..", THISBACK1(AddFile, PACKAGE_FILE))
			.Help("Вставить файл относительно текущего пакета");
		menu.добавь(!isaux, "Вставить группу topic++..", TopicImg::IGroup(), THISBACK(AddTopicGroup));
	}
	menu.добавь("Вставить сепаратор..", IdeImg::Separator(), [=] { добавьСепаратор(); })
		.Help("Добавить строку текста сепаратора");
	if(!isaux) {
		menu.добавь("Особая вставка", THISBACK(SpecialFileMenu))
		    .Help("Менее часто используемые методы добавления файлов в пакет");
	}
	menu.Separator();
	if(!organizer) {
		if(sel)
			menu.добавь(closed.найди(GetActiveSepfo()) < 0 ? "Закрыть группу\t[double-click]"
			                                           : "Открыть группу\t[double-click]", THISBACK(Группа));
		menu.добавь("Открыть все группы", THISBACK(OpenAllGroups));
		menu.добавь("Закрыть все группы", THISBACK(CloseAllGroups));
		menu.Separator();
		BuildFileMenu(menu);
		menu.Separator();
	}
	menu.добавь("Переименовать...", THISBACK(RenameFile))
	    .Help("Переименовать файл / сепаратор / группу тематик");
	menu.добавь("Удалить", THISBACK(RemoveFile))
		.Ключ(organizer ? K_DELETE : K_ALT_DELETE)
		.Help("Удалить файл / сепаратор / группу тематик из пакета");
	menu.добавь(filelist.курсор_ли(), "Стереть", sel ? THISBACK(RemoveFile) : THISBACK(DelFile))
		.Help("Удалить файл / справку по группе тематик из пакета & удалить файл / папку на диске");
	menu.Separator();
	menu.добавь("Открыть папку файла",THISBACK(OpenFileFolder));
	menu.добавь("Копировать путь файла", callback1(WriteClipboardText, GetActiveFilePath()));
	menu.добавь("Открыть терминал в папке файла", [=] { LaunchTerminal(дайДиректориюФайла(GetActiveFilePath())); });
	menu.Separator();
	menu.добавь(filelist.дайКурсор() > 0, "Поднять", THISBACK1(MoveFile, -1))
		.Ключ(organizer ? K_CTRL_UP : K_ALT|K_CTRL_UP)
		.Help("Перместить текущий файл к началу пакета на позицию");
	menu.добавь(filelist.курсор_ли() && filelist.дайКурсор() < filelist.дайСчёт() - 1, "Опустить",
	         THISBACK1(MoveFile, 1))
		.Ключ(organizer ? K_CTRL_DOWN : K_ALT|K_CTRL_DOWN)
		.Help("Перместить текущий файл к концу пакета на позицию");
	if(IsActiveFile()) {
		menu.Separator();
		Ткст p = GetActiveFilePath();
		Ткст ext = дайРасшф(p);
		if(ext == ".tpp" && папка_ли(p)) {
			menu.добавь("Включаемая группа тематик", THISBACK(ToggleIncludeable))
			    .Check(файлЕсть(приставьИмяф(p, "all.i")));
			if(GetRepoKind(p))
				menu.добавь("Синхронизовать Репозиторий " + p, THISBACK1(SyncSvnDir, p));
		}
		else {
			if(IsHeaderExt(ext))
				menu.добавь("Прекомпилировать заголовок", THISBACK(TogglePCH))
				    .Check(ActiveFile().pch);
		}
	}
	FilePropertiesMenu(menu);
}

void WorkspaceWork::TogglePCH()
{
	if(IsActiveFile()) {
		ActiveFile().pch = !ActiveFile().pch;
		SaveLoadPackageNS();
	}
}

void WorkspaceWork::ToggleIncludeable()
{
	if(IsActiveFile()) {
		Ткст p = GetActiveFilePath();
		SetTopicGroupIncludeable(p, !файлЕсть(приставьИмяф(p, "all.i")));
		SaveLoadPackageNS();
	}
}

void WorkspaceWork::AddNormalUses()
{
	Ткст p = SelectPackage("Выбрать пакет");
	if(p.пустой()) return;
	OptItem& m = actual.uses.добавь();
	m.text = p;
	SaveLoadPackage();
	InvalidateIncludes();
}

void WorkspaceWork::RemovePackageMenu(Бар& bar)
{
	if(bar.IsScanKeys() || bar.IsScanHelp() || !bar.барМеню_ли())
		return;
	InvalidateIncludes();
	Ткст active = UnixPath(GetActivePackage());
	int usecnt = 0;
	for(int i = 0; i < package.дайСчёт(); i++) {
		Ткст pn = UnixPath(package[i].имя);
		Пакет prj;
		Ткст pp = PackagePath(pn);
		prj.грузи(pp);
		for(int i = 0; i < prj.uses.дайСчёт(); i++)
			if(UnixPath(prj.uses[i].text) == active) {
				usecnt++;
				bar.добавь("Удалить из '" + pn + '\'', THISBACK1(RemovePackage, pn))
					.Help(фмт("Удалить пакет '%s' из раздела использований в '%s'", active, pp));
			}
	}
	if(usecnt > 1) {
		bar.MenuSeparator();
		bar.добавь("Удалить все использования", THISBACK1(RemovePackage, Ткст(Null)))
			.Help(фмт("Удалить пакет '%s' из всех использований в активном проекте и его подмодулях", active));
	}
}

void WorkspaceWork::PackageOp(Ткст active, Ткст from_package, Ткст rename)
{
	active = UnixPath(active);
	from_package = UnixPath(from_package);
	rename = UnixPath(rename);
	for(int i = 0; i < package.дайСчёт(); i++)
		if(*package[i].имя != '<' &&
		   (пусто_ли(from_package) || UnixPath(package[i].имя) == from_package)) {
			Ткст pp = PackagePath(package[i].имя);
			Пакет prj;
			if(prj.грузи(pp)) {
				for(int i = prj.uses.дайСчёт(); --i >= 0;)
					if(UnixPath(prj.uses[i].text) == active) {
						if(rename.дайСчёт())
							prj.uses[i].text = rename;
						else
							prj.uses.удали(i);
					}
				prj.сохрани(pp);
			}
		}
	ScanWorkspace();
	SyncWorkspace();
	InvalidateIncludes();
}

void WorkspaceWork::RemovePackage(Ткст from_package)
{
	Ткст active = UnixPath(GetActivePackage());
	if(пусто_ли(from_package) && !PromptYesNo(фмт(
		"Удалить пакет [* \1%s\1] из разделов использования всех текущих пакетов ?", active)))
		return;
	PackageOp(GetActivePackage(), from_package, Null);
	InvalidateIncludes();
}

void WorkspaceWork::TogglePackageSpeed()
{
	if(!package.курсор_ли())
		return;
	SaveLoadPackageNS();
}

void WorkspaceWork::RenamePackage()
{
	if(IsAux() || !package.курсор_ли())
		return;
	WithRenamePackageLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, "Переименовать пакет");
	dlg.имя.устФильтр(FilterPackageName);
	dlg.имя <<= package.дай(package.дайКурсор()).имя;
	dlg.имя.выбериВсе();
again:
	if(dlg.выполни() != IDOK)
		return;
	Ткст pn = ~dlg.имя;
	Ткст ap = GetActivePackage();
	if(!RenamePackageFs(PackagePath(ap), pn))
		goto again;
	PackageOp(ap, Null, pn);
}

void WorkspaceWork::DeletePackage()
{
	Ткст active = GetActivePackage();
	if(package.дайКурсор() == 0) {
		Exclamation("Нельзя удалить главный пакет!");
		return;
	}
	if(IsAux() || !package.курсор_ли() ||
	   !PromptYesNo("Вы действительно хотите удалить пакет [* \1" + active + "\1]?&&"
	                "[/ Предупреждение:] [* Пакет будет удалён только&"
	                "для пакетов текущего рабочего пространства!]"))
		return;
	if(!PromptYesNo("Эта операция необратима.&Действительно продолжить?"))
		return;
	if(!DeleteFolderDeep(дайПапкуФайла(GetActivePackagePath()))) {
		Exclamation("Неудачное удаление папки.");
		return;
	}
	PackageOp(active, Null, Null);
}

void WorkspaceWork::PackageMenu(Бар& menu)
{
	if(!menu.IsScanKeys()) {
		bool cando = !IsAux() && package.курсор_ли();
		Ткст act = UnixPath(GetActivePackage());
		menu.добавь(cando, ~фмт("Добавить пакет в '%s'", act), IdeImg::package_add(), THISBACK(AddNormalUses));
		RemovePackageMenu(menu);
		if(menu.барМеню_ли()) {
			bool main = package.дайКурсор() == 0;
			
			menu.добавь(cando, "Переименовать пакет..", THISBACK(RenamePackage));
			menu.добавь(cando && !main, "Удалить пакет", THISBACK(DeletePackage));
			menu.Separator();
			BuildPackageMenu(menu);
			menu.добавь("Открыть папку пакета",THISBACK(OpenPackageFolder));
			menu.добавь("Открыть терминал в папке пакета", [=] { LaunchTerminal(дайДиректориюФайла(GetActivePackagePath())); });
		}
	}
}

void WorkspaceWork::DoMove(int b, bool drag)
{
	int fi = filelist.дайКурсор();
	if(fi < 0 || fi >= fileindex.дайСчёт())
		return;
	int a = fileindex[fi];
	if(a < 0 || b < 0 || a >= actual.file.дайСчёт() ||
	   (drag ? b > actual.file.дайСчёт() : b >= actual.file.дайСчёт()))
		return;
	int s = filelist.GetSbPos();
	ShowFile(a);
	ShowFile(b);
	if(drag) {
		actual.file.перемести(a, b);
		if(b >= a)
			b--;
	}
	else
		разверни(actual.file[a], actual.file[b]);
	ShowFile(a);
	ShowFile(b);
	SavePackage();
	LoadActualPackage();
	filelist.SetSbPos(s);
	for(int i = 0; i < fileindex.дайСчёт(); i++)
		if(fileindex[i] == b) {
			filelist.устКурсор(i);
			break;
		}
	filelist.синх();
}

void WorkspaceWork::MoveFile(int d)
{
	int bi = filelist.дайКурсор() + d;
	if(bi < 0 || bi >= fileindex.дайСчёт())
		return;
	DoMove(fileindex[bi], false);
}

void WorkspaceWork::вставьТиБ(int line, PasteClip& d)
{
	if(GetActivePackage() == METAPACKAGE)
		return;
	if(GetInternalPtr<UppList>(d, "package-file") == &filelist && d.прими())
		DoMove(line < fileindex.дайСчёт() ? fileindex[line] : actual.file.дайСчёт(), true);
	InvalidateIncludes();
}

void WorkspaceWork::тяни()
{
	filelist.DoDragAndDrop(InternalClip(filelist, "package-file"),
	                       filelist.дайСэиплТяга(), DND_MOVE);
}

WorkspaceWork::WorkspaceWork()
{
	package <<= THISBACK(PackageCursor);
	package.WhenBar = THISBACK(PackageMenu);
	package.NoRoundSize();
	package.Columns(2);
	filelist <<= THISBACK(FileCursor);
	filelist.WhenLeftClickPos = THISBACK(GroupOrFile);
	filelist.WhenLeftDouble = THISBACK(Группа);
	filelist.Columns(2);
	filelist.NoRoundSize();
	actualfileindex = -1;
	organizer = false;
	package.BackPaintHint();
	filelist.BackPaintHint();
	filelist.WhenDrag = THISBACK(тяни);
	filelist.WhenDropInsert = THISBACK(вставьТиБ);
	showtime = false;
	sort = true;
	repo_dirs = false;
}

void WorkspaceWork::SerializeClosed(Поток& s)
{
	РОбласть wspc;
	wspc.скан(main);
	Вектор<Sepfo> list;
	for(int i = 0; i < wspc.дайСчёт(); i++) {
		Ткст pk = wspc[i];
		const Пакет& p = wspc.дайПакет(i);
		for(int i = 0; i < p.дайСчёт(); i++)
			if(p[i].separator) {
				Sepfo sf(pk, p[i]);
				if(closed.найди(sf) >= 0)
					list.добавь(sf);
			}
	}
	s % list;
	closed = pick(list);
}

void UppList::рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const
{
	СписокФайлов::Файл file = ValueTo<СписокФайлов::Файл>(q);
	if(дайИмяф(file.имя) == "$.tpp" && папка_ли(file.имя))
		file.имя = дайИмяф(дайПапкуФайла(file.имя)) + " templates";
	if(file.имя == конфигФайл("global.defs"))
		file.имя = "фиксирован macros";
	СписокФайлов::рисуй(w, r, RawToValue(file), ink, paper, style);
}
