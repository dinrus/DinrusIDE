#include "DinrusIDE.h"

#ifdef _ОТЛАДКА
#define LSLOW()    // спи(20) // Simulate HD seeks to test package cache
#else
#define LSLOW()
#endif

void SelectPackageDlg::PackageMenu(Бар& menu)
{
	bool b = GetCurrentName().дайСчёт();
	menu.добавь("Новый пакет..", [=] { OnNew(); });
	menu.Separator();
	menu.добавь(b, "Дублировать пакет..", [=] { RenamePackage(true); });
	menu.добавь(b, "Переименовать пакет..", [=] { RenamePackage(false); });
	menu.добавь(b, "Копировать пакет в..", [=] { MovePackage(true); });
	menu.добавь(b, "Переместить пакет в..", [=] { MovePackage(false); });
	menu.добавь(b, "Удалить пакет..", [=] { DeletePackage(); });
	menu.добавь(b, "Изменить описание..", [=] { ChangeDescription(); });
}

bool RenamePackageFs(const Ткст& upp, const Ткст& npf, const Ткст& nupp, bool copy)
{
	Ткст pf = дайПапкуФайла(upp);
	Ткст temp_pf = приставьИмяф(дайПапкуФайла(pf), какТкст(случ()) + какТкст(случ()));
	if(!переместифл(pf, temp_pf)) {
		Exclamation("Неудачная операция.");
		return false;
	}
	RealizePath(дайПапкуФайла(npf));
	if(copy) {
		bool b = копируйПапку(npf, temp_pf);
		переместифл(temp_pf, pf);
		if(!b) {
			переместифл(temp_pf, pf);
			DeleteFolderDeep(npf);
			Exclamation("Неудачное дублирование папки пакета.");
			return false;
		}
	}
	else
	if(!переместифл(temp_pf, npf)) {
		переместифл(temp_pf, pf);
		Exclamation("Неудачное переименование папки пакета.");
		return false;
	}
	if(!переместифл(npf + "/" + дайИмяф(upp), nupp)) {
		переместифл(npf, pf);
		Exclamation("Неудачное переименование файла .upp.");
		return false;
	}
	return true;
}

bool RenamePackageFs(const Ткст& upp, const Ткст& newname, bool duplicate)
{
	if(пусто_ли(newname)) {
		Exclamation("Неправильное имя.");
		return false;
	}
	Ткст npf = приставьИмяф(GetPackagePathNest(дайПапкуФайла(upp)), newname);
	Ткст nupp = npf + "/" + дайИмяф(newname) + ".upp";

	if(файлЕсть(nupp)) {
		Exclamation("Пакет [* \1" + newname + "\1] уже существует!");
		return false;
	}

	return RenamePackageFs(upp, npf, nupp, duplicate);
}

void SelectPackageDlg::RenamePackage(bool duplicate)
{
	Ткст n = GetCurrentName();
	if(пусто_ли(n))
		return;
again:
	if(!редактируйТекст(n, duplicate ? "Дублировать пакет:" : "Периментовать пакет", "Имя", FilterPackageName))
		return;
	if(!RenamePackageFs(PackagePath(GetCurrentName()), n, duplicate))
		goto again;
	search <<= Null;
	грузи(n);
	alist.ScrollIntoCursor();
}

void SelectPackageDlg::MovePackage(bool copy)
{
	WithMoveCopyPackageLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, copy ? "Копировать пакет в" : "Перместить пакет в");

	Ткст d0;
	for(int pass = 0; pass < 2; pass++) {
		Индекс<Ткст> udir;
		ФайлПоиск ff(конфигФайл("*.var"));
		while(ff) {
			if(int(дайТитулф(ff.дайИмя()) != base.дайКлюч()) == pass) {
				ВекторМап<Ткст, Ткст> var;
				LoadVarFile(ff.дайПуть(), var);
				for(Ткст d : разбей(var.дай("РНЦП", ""), ';'))
					if(дирЕсть(d)) {
						udir.найдиДобавь(d);
						d0 = Nvl(d0, d);
					}
			}
			ff.следщ();
		}
		
		Вектор<Ткст> sd = pick(udir.подбериКлючи());
		сортируй(sd, [](const Ткст& a, const Ткст& b) { return взаг(a) < взаг(b); });
		for(Ткст d : sd)
			dlg.dir.добавьСписок(d);
	}

	dlg.dir <<= d0;
	выборДир(dlg.dir, dlg.select);

	dlg.имя <<= GetCurrentName();

again:
	if(dlg.пуск() != IDOK)
		return;
	
	Ткст dir = ~dlg.dir;
	if(!дирЕсть(dir)) {
		Exclamation("Неверная целевая папка!");
		goto again;
	}
	Ткст pkg = приставьИмяф(dir, ~~dlg.имя);
	if(дирЕсть(pkg)) {
		Exclamation("Целавая папка пакета уже существует!");
		goto again;
	}
	if(файлЕсть(pkg)) {
		Exclamation("Неверная целевая папка пакета - это файл!");
		goto again;
	}

	if(!RenamePackageFs(PackagePath(GetCurrentName()), pkg, pkg + "/" + дайИмяф(~~dlg.имя) + ".upp", copy))
		goto again;

	грузи(~~dlg.имя);
}

void SelectPackageDlg::DeletePackage()
{
	Ткст n = GetCurrentName();
	if(пусто_ли(n))
		return;
	Ткст pp = дайПапкуФайла(PackagePath(GetCurrentName()));
	if(!дирЕсть(pp)) {
		Exclamation("Папки не существует!");
		return;
	}
	if(!PromptYesNo("Действительно удалить пакет [* \1" + GetCurrentName() + "\1]?&&"
	                "[/ Предупреждение:] [* Пакет не будет удалён "
	                "из использования любых жругих пакетов!]"))
		return;
	if(!PromptYesNo("Эта операция необратима.&На самом деле продолжить?"))
		return;
	DeleteFolderDeep(pp);
	грузи();
}

SelectPackageDlg::SelectPackageDlg(const char *title, bool selectvars_, bool main)
: selectvars(selectvars_)
{
	CtrlLayoutOKCancel(*this, title);
	Sizeable().Zoomable();
	Иконка(IdeImg::MainPackage(), IdeImg::PackageLarge());
	base.AutoHideSb();
	base.NoGrid();
	base.добавьКолонку("Сборка");
	base.WhenCursor = THISBACK(OnBase);
	base.WhenBar = THISBACK(ToolBase);
	base.WhenLeftDouble = THISBACK(OnBaseEdit);
	ok.WhenAction = clist.WhenLeftDouble = alist.WhenLeftDouble = THISBACK(OnOK);
	cancel.WhenAction = WhenClose = THISBACK(OnCancel);
	clist.Columns(4);
	clist.WhenEnterItem = clist.WhenKillCursor = THISBACK(ListCursor);
	alist.добавьКолонку("Пакет").добавь(3);
	alist.добавьКолонку("Гнездо");
	alist.добавьКолонку("Описание");
	alist.добавьИндекс();
	alist.ColumnWidths("108 79 317");
	alist.WhenCursor = THISBACK(ListCursor);
	alist.EvenRowColor();
	alist.SetLineCy(max(Zy(16), Draw::GetStdFontCy()));
	list.добавь(clist.SizePos());
	list.добавь(alist.SizePos());
	
	lists_status.устДефолт("");
	lists_status.NoSizeGrip();
	lists_status.устВысоту(Zy(10));
	lists_status.добавь(progress.SizePos());
	
	parent.добавь(list.SizePos());
	parent.добавьФрейм(splitter.лево(base, Zx(170)));

	if (!selectvars)
		splitter.скрой();
	
	newu << [=] { OnNew(); };
	kind.добавь(MAIN, "Главные пакеты");
	kind.добавь(NONMAIN, "Второстепенные пакеты");
	kind.добавь(ALL, "Все пакеты");
	kind << [=] { OnFilter(); };
	kind <<= main ? MAIN : NONMAIN;
	nest << [=] { OnFilter(); };
	OnFilter();
	nest <<= main ? 0 : ALL;
	brief <<= THISBACK(SyncBrief);
	search.NullText("Поиск (Ктрл+K)", StdFont().Italic(), SColorDisabled());
	search << [=] { SyncList(Null); };
	search.устФильтр(CharFilterDefaultToUpperAscii);
	SyncBrief();
	ActiveFocus(brief ? (Ктрл&)clist : (Ктрл&)alist);
	clist.BackPaintHint();
	alist.BackPaintHint();
	base.BackPaintHint();
	loadi = 0;
	loading = false;
	clist.WhenBar = alist.WhenBar = THISBACK(PackageMenu);
	
	upphub.устРисунок(IdeImg::UppHub());
	upphub << [=] {
		Ткст p = UppHub();
		OnBase();
		if(p.дайСчёт()) {
			alist.FindSetCursor(p);
			clist.FindSetCursor(p);
		}
	};
	
	help << [&] { запустиВебБраузер("https://www.ultimatepp.org/app$ide$PackagesAssembliesAndNests$en-us.html"); };
}

void SelectPackageDlg::SyncFilter()
{
	Значение n = ~nest;
	nest.очистьСписок();
	Вектор<Ткст> upp = GetUppDirsRaw();
	nest.добавь(ALL, AttrText("Все").Italic().Bold().NormalInk(SBlue()));
	bool hub = false;
	for(int i = 0; i < upp.дайСчёт(); i++) {
		if(upp[i].начинаетсяС(GetHubDir()) && !hub) {
			nest.добавь(UPPHUB|i, AttrText("UppHub").Italic().NormalInk(SBlue()));
			hub = true;
		}
		Ткст фн = дайИмяф(upp[i]);
		if(hub)
			nest.добавь(i, AttrText(фн).NormalInk(SCyan()));
		else
			nest.добавь(i, фн);
	}
	if(nest.HasKey(n))
		nest <<= n;
	else
		nest.идиВНач();
}

bool SelectPackageDlg::Ключ(dword ключ, int count)
{
	if(ключ == K_ALT_ENTER) {
		ChangeDescription();
		return true;
	}
	else if(ключ == K_CTRL_K) {
		search.устФокус();
		return true;
	}
	if((clist.естьФокус() || alist.естьФокус()) && search.Ключ(ключ, count))
		return true;
	return ТопОкно::Ключ(ключ, count);
}

void SelectPackageDlg::сериализуй(Поток& s)
{
	SerializePlacement(s);
	s % brief;
}

Ткст SelectPackageDlg::GetCurrentName()
{
	if(clist.показан_ли())
		return clist.GetCurrentName();
	else
	if(alist.курсор_ли())
		return alist.дай(0);
	return Null;
}

Ткст SelectPackageDlg::GetCurrentNest()
{
	int i = clist.показан_ли() ? clist.дайКурсор() : alist.дайКурсор();
	Ткст f = GetCurrentName();
	return i >= 0 && i < nest_list.дайСчёт() ? nest_list[i] : Ткст();
}

int   SelectPackageDlg::GetCurrentIndex()
{
	Ткст s = GetCurrentName();
	for(int i = 0; i < packages.дайСчёт(); i++)
		if(packages[i].package == s)
			return i;
	return -1;
}

void SelectPackageDlg::ChangeDescription()
{
	int ii = GetCurrentIndex();
	if(ii >= 0 && ii < packages.дайСчёт()) {
		PkInfo& p = packages[ii];
		WithDescriptionLayout<ТопОкно> dlg;
		CtrlLayoutOKCancel(dlg, "Описание пакета");
		Ткст pp = PackagePath(p.package);
		Пакет pkg;
		if(!pkg.грузи(pp)) {
			Exclamation("Пакета не существует.");
			return;
		}
		dlg.text <<= pkg.description;
		if(dlg.пуск() != IDOK)
			return;
		pkg.description = ~dlg.text;
		pkg.сохрани(pp);
		p.description = ~dlg.text;
		if(alist.курсор_ли())
			alist.уст(2, ~dlg.text);
	}
}

void SelectPackageDlg::ListCursor()
{
	int c = GetCurrentIndex();
	if(c >= 0 && c < packages.дайСчёт()) {
		Ткст pp = PackagePath(GetCurrentName());
		Пакет pkg;
		pkg.грузи(pp);
	}
}

void SelectPackageDlg::SyncBrief()
{
	bool b = brief;
	alist.покажи(!b);
	clist.покажи(b);
}

Ткст SelectPackageDlg::пуск(Ткст& nest, Ткст startwith)
{
	finished = canceled = false;
	if(!IsSplashOpen())
		открой();
	if(selectvars)
		SyncBase(GetVarsName());
	else
		OnBase();
	Ткст bkvar = GetVarsName();
	if(finished)
		return GetCurrentName();
	if(canceled)
		return Null;
	alist.FindSetCursor(startwith);
	clist.FindSetCursor(startwith);
	ActiveFocus(alist.показан_ли() ? (Ктрл&)alist : (Ктрл&)clist);
	switch(ТопОкно::пуск()) {
	case IDOK:
		nest = GetCurrentNest();
		return GetCurrentName();
	case IDYES:
		nest = selected_nest;
		return selected;
	default:
		LoadVars(bkvar);
		SyncFilter();
		SyncBase(GetVarsName());
		return Null;
	}
}

void SelectPackageDlg::OnOK()
{
	Пакет pkg;
	int fk = ~kind;
	Ткст n = GetCurrentName();
	if(n.дайСчёт() && pkg.грузи(PackagePath(n)) &&
	   (!(fk == MAIN) || pkg.config.дайСчёт()) &&
	   (!(fk == NONMAIN) || !pkg.config.дайСчёт())) {
		loading = false;
		finished = true;
		AcceptBreak(IDOK);
	}
}

void SelectPackageDlg::OnCancel()
{
	loading = false;
	canceled = true;
	RejectBreak(IDCANCEL);
}

void SelectPackageDlg::OnFilter()
{
	SyncList(Null);
}

void SelectPackageDlg::OnBase()
{
	if(!finished && !canceled) {
		SyncFilter();
		if(splitter.показан_ли())
			nest <<= nest.дайСчёт() ? 0 : ALL;
		грузи();
	}
}

void SelectPackageDlg::OnNew() {
	TemplateDlg dlg;
	грузиИзГлоба(dlg, "NewPackage");
	dlg.грузи(GetUppDirsRaw(), ~kind == MAIN);
	while(dlg.пуск() == IDOK) {
		Ткст nest = ~dlg.nest;
		Ткст имя = NativePath(Ткст(~dlg.package));
		Ткст path = приставьИмяф(приставьИмяф(nest, имя), дайИмяф(имя) + ".upp");
		if(файлЕсть(path) && !PromptYesNo("Пакет [* \1" + path + "\1] уже существует.&"
		                                    "Восстановить файлы?"))
			continue;
		RealizePath(path);
		if(!сохраниФайл(path, Null)) {
			Exclamation("Ошибка при записи в файл [* \1" + path + "\1].");
			continue;
		}
		dlg.создай();
		selected_nest = nest;
		selected = имя;
		Break(IDYES);
		break;
	}
	сохраниВГлоб(dlg, "NewPackage");
}

Вектор<Ткст> SelectPackageDlg::GetSvnDirs()
{
	Вектор<Ткст> r;
	Вектор<Ткст> dirs = SplitDirs(GetVar("РНЦП"));
	for(int i = 0; i < dirs.дайСчёт(); i++) {
		Ткст d = нормализуйПуть(dirs[i]);
		if(GetRepoKind(d))
			r.добавь(d);
	}
	return r;
}

void SelectPackageDlg::SyncSvnDir(const Ткст& dir)
{
	RepoSyncDirs(Вектор<Ткст>() << dir);
	грузи();
}

void SelectPackageDlg::SyncSvnDirs()
{
	RepoSyncDirs(GetSvnDirs());
	грузи();
}

void SelectPackageDlg::ToolBase(Бар& bar)
{
	bar.добавь("Новая сборка..", THISBACK(OnBaseAdd))
#ifdef PLATFORM_COCOA
		.Ключ(K_CTRL_N)
#else
		.Ключ(K_INSERT)
#endif
	;
	bar.добавь(base.курсор_ли(), "Редактировать сботку..", THISBACK(OnBaseEdit))
		.Ключ(K_CTRL_ENTER);
	bar.добавь(base.курсор_ли(), "Удалить сборку..", THISBACK(OnBaseRemove))
		.Ключ(K_CTRL_DELETE);
	Вектор<Ткст> d = GetSvnDirs();
	if(HasGit()) {
		bar.Separator();
		bar.добавь("Клонировать исходники U++ с GitHub..", [=] {
			Ткст vars = base.дай(0);
			SetupGITMaster();
			SyncBase(vars);
		});
	}
	if(d.дайСчёт()) {
		bar.Separator();
		for(int i = 0; i < d.дайСчёт(); i++)
			bar.добавь("Синхронизовать " + d[i], IdeImg::svn_dir(), THISBACK1(SyncSvnDir, d[i]));
		bar.добавь("Синхронизовать все..", IdeImg::svn(), THISBACK(SyncSvnDirs));
	}
}

void SelectPackageDlg::OnBaseAdd()
{
	Ткст vars;
	if(BaseSetup(vars))
		SyncBase(vars);
}

void SelectPackageDlg::OnBaseEdit()
{
	if(!base.курсор_ли())
		return;
	Ткст vars = base.дай(0), oldvars = vars;
	if(BaseSetup(vars)) {
		if(vars != oldvars)
			DeleteFile(VarFilePath(oldvars));
		DeleteFile(CachePath(vars));
		SyncBase(vars);
	}
}

void SelectPackageDlg::OnBaseRemove()
{
	int c = base.дайКурсор();
	if(c < 0)
		return;
	Ткст next;
	if(c + 1 < base.дайСчёт())
		next = base.дай(c + 1);
	else if(c > 0)
		next = base.дай(c - 1);
	Ткст vars = base.дай(0);
	Ткст varpath = VarFilePath(vars);
	if(PromptOKCancel(фмт("Удалить базовый файл [* \1%s\1]?", varpath))) {
		if(!удалифл(varpath))
			Exclamation(фмт("Ошибка при удалении файла [* \1%s\1].", varpath));
		else
			SyncBase(next);
	}
}

int DirSep(int c)
{
	return c == '\\' || c == '/' ? c : 0;
}

struct PackageDisplay : Дисплей {
	Шрифт fnt;

	virtual Размер дайСтдРазм(const Значение& q) const {
		МассивЗнач va = q;
		Размер sz = дайРазмТекста(Ткст(va[0]), fnt);
		sz.cx += Zx(20);
		sz.cy = max(sz.cy, Zy(16));
		return sz;
	}

	virtual void рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const {
		МассивЗнач va = q;
		Ткст txt = va[0];
		Рисунок icon = va[1];
		if(пусто_ли(icon))
			icon = IdeImg::Package();
		else
			icon = DPI(icon, 16);
		w.DrawRect(r, paper);
		w.DrawImage(r.left, r.top + (r.устВысоту() - icon.дайВысоту()) / 2, icon);
		w.DrawText(r.left + DPI(20), r.top + (r.устВысоту() - Draw::GetStdFontCy()) / 2, txt, fnt, ink);
	}

	PackageDisplay() { fnt = StdFont(); }
};

void SelectPackageDlg::SyncList(const Ткст& find)
{
	Ткст n = Nvl(find, GetCurrentName());
	int asc = alist.дайПромотку();
	int csc = clist.GetSbPos();

	packages.очисть();
	Ткст s = ~search;
	int фн = ~nest;
	int fk = ~kind;
	Индекс<Ткст> added;
	int from = 0;
	int to = data.дайСчёт() - 1;
	if(фн & UPPHUB)
		from = фн & NEST_MASK;
	else
	if(фн != ALL)
		from = to = фн & NEST_MASK;
	if(to < data.дайСчёт())
		for(int i = from; i <= to; i++) {
			const МассивМап<Ткст, PkData>& nest = data[i];
			for(int i = 0; i < nest.дайСчёт(); i++) {
				const PkData& d = nest[i];
				if(!nest.отлинкован(i) &&
				   d.ispackage &&
				   (!(fk == MAIN) || d.main) &&
				   (!(fk == NONMAIN) || !d.main) &&
				   взаг(d.package + d.description + d.nest).найди(s) >= 0 &&
				   added.найди(d.package) < 0) {
					packages.добавь() = d;
					if(!d.main)
						added.добавь(d.package);
				}
			}
		}
	сортируй(packages);
	alist.очисть();
	clist.очисть();
	nest_list.очисть();
	ListCursor();
	static PackageDisplay pd, bpd;
	bpd.fnt.Bold();
	for(int i = 0; i < packages.дайСчёт(); i++) {
		const PkInfo& pkg = packages[i];
		Рисунок icon = pkg.icon;
		if(пусто_ли(icon)) {
			if(pkg.main)
				icon = pkg.upphub ? IdeImg::HubMainPackage() : IdeImg::MainPackage();
			else
				icon = pkg.upphub ? IdeImg::HubPackage() : IdeImg::Package();
		}
		nest_list.добавь(pkg.nest);
		clist.добавь(pkg.package, DPI(icon, 16));
		alist.добавь(pkg.package, дайИмяф(pkg.nest), pkg.description, icon);
		alist.устДисплей(alist.дайСчёт() - 1, 0, pkg.main ? bpd : pd);
	}
	if(!alist.FindSetCursor(n))
		alist.идиВНач();
	if(!clist.FindSetCursor(n) && clist.дайСчёт())
		clist.устКурсор(0);
	alist.ScrollTo(asc);
	clist.SetSbPos(csc);
	alist.HeaderTab(0).устТекст("Пакет (" + какТкст(alist.дайСчёт()) + ")");
}

void SelectPackageDlg::ScanFolder(const Ткст& path, МассивМап<Ткст, PkData>& nd,
                                  const Ткст& nest, Индекс<Ткст>& dir_exists,
                                  const Ткст& prefix)
{
	for(ФайлПоиск ff(приставьИмяф(path, "*.*")); ff; ff.следщ())
		if(ff.папка_ли() && !ff.скрыт_ли()) {
			Ткст p = ff.дайПуть();
			dir_exists.добавь(p);
			bool nw = nd.найди(p) < 0; // Do we have any info loaded about this package?
			PkData& d = nd.дайДобавь(p);
			d.package = prefix + ff.дайИмя();
			d.nest = nest;
			d.upphub = InUppHub(p);
			if(nw) { // No cached info available about the folder
				d.ispackage = буква_ли(*d.package) && d.package.найди('.') < 0; // First heuristic guess
				d.main = d.ispackage && prefix.дайСчёт() == 0; // Expect it is main
			}
		}
}

Ткст SelectPackageDlg::CachePath(const char *vn) const
{
	return приставьИмяф(конфигФайл("cfg"), Ткст(vn) + ".pkg_cache");
}

void SelectPackageDlg::грузи(const Ткст& find)
{
	SyncFilter();
	if(selectvars && !base.курсор_ли())
		return;
	if(loading) { // If we are called recursively from обработайСобытия, stop current loading and change loadi
		loadi++;
		loading = false;
		return;
	}
	int current_loadi = -1;
	while(current_loadi != loadi) {
		current_loadi = loadi;
		if(selectvars) {
			Ткст assembly = (Ткст)base.дай(0);
			list.вкл(base.курсор_ли());
			if(!base.курсор_ли())
				return;
			LoadVars(assembly);
			SyncFilter();
		}
		Вектор<Ткст> upp = GetUppDirsRaw();
		packages.очисть();
		list.добавьФрейм(lists_status);
		loading = true;
		data.очисть();
		Индекс<Ткст> dir_exists;
		Ткст cache_path = CachePath(GetVarsName());
		грузиИзФайла(data, cache_path);
		data.устСчёт(upp.дайСчёт());
		for(int i = 0; i < upp.дайСчёт(); i++) // скан nest folders for subfolders (additional package candidates)
			ScanFolder(upp[i], data[i], upp[i], dir_exists, Null);
		int update = msecs();
		for(int i = 0; i < data.дайСчёт() && loading; i++) { // Now investigate individual sub folders
			МассивМап<Ткст, PkData>& nest = data[i];
			Ткст nest_dir = нормализуйПуть(upp[i]);
			for(int i = 0; i < nest.дайСчёт() && loading; i++) {
				if(msecs(update) >= 100) { // each 100 ms update the list (and open select dialog after splash screen is closed)
					if(!IsSplashOpen() && !открыт())
						открой();
					progress++;
					SyncList(find);
					update = msecs();
				}
				обработайСобытия(); // keep GUI running

				PkData& d = nest[i];
				Ткст path = nest.дайКлюч(i);
				if(нормализуйПуть(path).начинаетсяС(nest_dir) && дирЕсть(path)) {
					Ткст upp_path = приставьИмяф(path, дайИмяф(d.package) + ".upp");
					LSLOW(); // this is used for testing only, normally it is NOP
					Время tm = дайВремяф(upp_path);
					if(пусто_ли(tm)) // .upp file does not exist - not a package
						d.ispackage = false;
					else
					if(tm != d.tm) { // cached info is outdated
						Пакет p;
						if(p.грузи(upp_path)) {
							d.description = p.description;
							d.main = p.config.дайСчёт();
							d.tm = tm;
							d.ispackage = true;
						}
						else
							d.ispackage = false;
					}
					else
						d.ispackage = true;
					if(d.ispackage) {
						Ткст icon_path;
						if(IsUHDMode())
							icon_path = приставьИмяф(path, "icon32x32.png");
						if(пусто_ли(icon_path) || !файлЕсть(icon_path))
							icon_path = приставьИмяф(path, "icon16x16.png");
						tm = дайВремяф(icon_path);
						if(пусто_ли(tm)) // package icon does not exist
							d.icon = Null;
						else
						if(tm != d.itm) { // chached package icon outdated
							d.icon = StreamRaster::LoadFileAny(icon_path);
							d.itm = tm;
						}
					}
					ScanFolder(path, nest, d.nest, dir_exists, d.package + '/');
				}
				else
					nest.отлинкуй(i); // cached folder was deleted or is not in nest dir
			}
			nest.смети();
		}
	
		сохраниВФайл(data, cache_path);
		list.удалиФрейм(lists_status);
		while(IsSplashOpen())
			обработайСобытия();
		if(!открыт())
			открой();
		if(loading) {
			loading = false;
			SyncList(find);
		}
	}
}

void SelectPackageDlg::SyncBase(Ткст initvars)
{
	Вектор<Ткст> varlist;
	for(ФайлПоиск ff(конфигФайл("*.var")); ff; ff.следщ())
		if(ff.файл_ли())
			varlist.добавь(дайТитулф(ff.дайИмя()));
	сортируй(varlist, &PackageLess);
	base.очисть();
	приставь(base, varlist);
	if(!base.FindSetCursor(initvars)) {
		if(base.дайСчёт() > 0)
			base.устКурсор(0);
		else
			OnBase();
	}
}

bool SelectPackageDlg::Pless(const SelectPackageDlg::PkInfo& a, const SelectPackageDlg::PkInfo& b)
{
	return PackageLess(a.package, b.package);
}

ИНИЦБЛОК
{
	региструйГлобКонфиг("SelectPkgMain");
	региструйГлобКонфиг("SelectPkg");
}

Ткст SelectPackage(Ткст& nest, const char *title, const char *startwith, bool selectvars, bool main)
{
	SelectPackageDlg dlg(title, selectvars, main);
	const char *c = main ? "SelectPkgMain" : "SelectPkg";
	грузиИзГлоба(dlg, c);
	dlg.SyncBrief();
	dlg.SyncFilter();
	Ткст b = dlg.пуск(nest, startwith);
	сохраниВГлоб(dlg, c);
	return b;
}

Ткст SelectPackage(const char *title, const char *startwith, bool selectvars, bool main)
{
	Ткст dummy;
	return SelectPackage(dummy, title, startwith, selectvars, main);
}
