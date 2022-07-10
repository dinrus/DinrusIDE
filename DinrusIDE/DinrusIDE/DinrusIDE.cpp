#include "DinrusIDE.h"

void Иср::MakeTitle()
{
	Ткст title;
	if(!main.пустой())
		title << main;
	if(!mainconfigname.пустой() &&  mainconfigname == mainconfigparam)
		title << " - " << mainconfigname;
	else
	if(!mainconfigname.пустой()) {
		title << " - " << mainconfigname;
		title << " ( " << mainconfigparam << " )";
	}
	if(!title.пустой())
		title << " - ";
	title << "ИСР РНЦП Динрус";
	if(designer) {
		title << " - " << designer->дайИмяф();
		int cs = designer->дайНабсим();
		if(cs >= 0)
			title << " " << имяНабСим(cs);
	}
	else
	if(!editfile.пустой()) {
		title << " - " << editfile;
		int chrset = editor.дайНабсим();
		title << " " << исрИмяНабСима(chrset)
		      << " " << (findarg(Nvl(editfile_line_endings, line_endings), LF, DETECT_LF) >= 0 ? "LF" : "CRLF");
		if(editor.IsTruncated())
			title << " [Обрезан]";
		if(editor.IsView())
			title << " [Обзор]";
		else
		if(editor.толькочтен_ли())
			title << " [Только Чтение]";
		if(editor.IsDirty())
			title << " *";
	}
	if(!пусто_ли(editfile))
		for(int i = 0; i < 10; i++)
			if(нормализуйПуть(editfile) == нормализуйПуть(bookmark[i].file))
				title << фмт(" <%d>", i);
	title << " { " << GetAssemblyId() << " }";
	if(isscanning)
		title << " (сканирование файлов)";
	Титул(title.вШТкст());
}

bool Иср::CanToggleReadOnly()
{
	return нормализуйПуть(GetActiveFilePath()) == нормализуйПуть(editfile) && !editor.IsView();
}

void Иср::ToggleReadOnly()
{
	if(CanToggleReadOnly() && IsActiveFile()) {
#ifdef PLATFORM_WIN32
		ФайлПоиск ff(editfile);
		if(ff && ff.толькочтен_ли()) {
			dword attrib = GetFileAttributes(editfile);
			attrib &= ~FILE_ATTRIBUTE_READONLY;
			SetFileAttributes(editfile, attrib);
		}
#endif
		editor.устРедактируем(editor.толькочтен_ли());
		ActiveFile().readonly = editor.толькочтен_ли();
		SavePackage();
		MakeTitle();
		SetBar();
	}
}

void Иср::AdjustMainConfig()
{
	const РОбласть& wspc = роблИср();
	if(!wspc.дайСчёт())
		return;
	const Массив<Пакет::Конфиг>& f = wspc.дайПакет(0).config;
	for(int i = 0; i < f.дайСчёт(); i++)
		if(f[i].param == mainconfigparam)
			return;
	if(f.дайСчёт()) {
		mainconfigparam = f[0].param;
		mainconfigname = f[0].имя;
		SetMainConfigList();
	}
}

Ткст Иср::GetFirstFile()
{
	const РОбласть& wspc = роблИср();
	for(int i = 0; i < wspc.дайСчёт(); i++) {
		const Пакет& p = wspc.дайПакет(i);
		for(int j = 0; j < p.дайСчёт(); j++)
			if(!p[j].separator)
				return SourcePath(wspc[i], p[j]);
	}
	return Null;
}

void Иср::SetMain(const Ткст& package)
{
	FlushFile();
	SaveWorkspace();
	transferfilecache.очисть();
	main = package;
	export_dir = дайФайлИзДомПапки(main);
	history.очисть();
	mainconfigname.очисть();
	mainconfigparam.очисть();

	UppHubAuto(main);

	ScanWorkspace();
	editfile.очисть();
	грузиИзФайла(THISBACK(SerializeWorkspace), WorkspaceFile());
	tabs.FixIcons();
	editorsplit.Zoom(0);
	Ткст e = editfile;
	UpdateFormat();
	editfile.очисть();
	MakeTitle();
	MakeIcon();
	SyncMainConfigList();
	AdjustMainConfig();
	SyncBuildMode();
	SetHdependDirs();
	SetBar();
	HideBottom();
	SyncUsc();
	InvalidateIncludes();
	if(auto_check)
		NewCodeBase();
	if(пусто_ли(e))
		e = GetFirstFile();
	EditFile(e);
}

void Иср::выход()
{
	if(debugger)
		debugger->стоп();
	сохраниФайл();
	SaveWorkspace();
	FlushFile();
	console.глуши();
	Break(IDOK);
	IdeExit = true;
}

bool Иср::OpenMainPackage()
{
	Ткст version = SplashCtrl::GenerateVersionNumber();
	Ткст tt = "Выберите главный пакет";
#ifdef bmYEAR
	tt << " (ИСР РНЦП Динрус " << version
	   << фмт(" %d-%02d-%02d %d:%02d)", bmYEAR , бмМЕСЯЦ, бмДЕНЬ, бмЧАС, бмМИНУТА);
#else
	tt << " (ИСР РНЦП Динрус " << version << ')';
#endif
	Ткст nest;
	Ткст p = SelectPackage(nest, tt, main, true, true);
	if(p.пустой()) return false;
	InvalidatePackageCache();
	SetMainNest(nest);
	main.очисть();
	if(!открыт())
		открой();
	SetMain(p);
	return true;
}

void Иср::NewMainPackage()
{
	if(setmain_newide) {
		Хост h;
		CreateHost(h, false, false);
		h.Launch(дайФПутьИсп() + " --nosplash");
	}
	else {
		SaveCodeBase();
		OpenMainPackage();
	}
}

void Иср::PackageCursor()
{
	WorkspaceWork::PackageCursor();
	Ткст p = GetActivePackage();
	if(p.пустой()) return;
	Ткст ef = впроп(нормализуйПуть(editfile));
	for(int i = 0; i < filelist.дайСчёт(); i++)
		if(впроп(нормализуйПуть(SourcePath(p, filelist[i]))) == ef) {
			filelist.устКурсор(i);
			break;
		}
	SetBar();
}

void Иср::EditWorkspace()
{
	EditPackages(main, GetActivePackage(), pocfg);
	ScanWorkspace();
	SyncWorkspace();
	InvalidateIncludes();
}

Ткст Иср::WorkspaceFile()
{
	Ткст nm;
	for(const char *s = main; *s; s++)
		nm.конкат(*s == '\\' || *s == '/' ? '$' : *s);
	Ткст cfg = конфигФайл("cfg");
	реализуйДир(cfg);
	return приставьИмяф(cfg, форсируйРасш(nm + '@' + GetVarsName(), ".cfg"));
}

void Иср::SaveWorkspace()
{
	if(console.console) return;
	if(main.пустой()) return;
	сохраниВФайл(THISBACK(SerializeWorkspace), WorkspaceFile());
}

void Иср::SyncMainConfigList()
{
	mainconfiglist.очисть();
	const РОбласть& wspc = роблИср();
	if(wspc.дайСчёт() <= 0) return;
	const Массив<Пакет::Конфиг>& f = wspc.дайПакет(0).config;
	for(int i = 0; i < f.дайСчёт(); i++)
		mainconfiglist.добавь(f[i].param, Nvl(f[i].имя, f[i].param));
	SetMainConfigList();
}

void Иср::SetMainConfigList()
{
	mainconfiglist <<= mainconfigparam;
	mainconfigname = mainconfiglist.дайЗначение();
	mainconfiglist.Подсказка("Главная конфигурация: " + mainconfigparam);
}

void Иср::OnMainConfigList()
{
	mainconfigparam = ~mainconfiglist;
	SetMainConfigList();
	MakeTitle();
}

void Иср::UscFile(const Ткст& file)
{
	try {
		ParseUscFile(file);
	}
	catch(СиПарсер::Ошибка& e) {
		ShowConsole();
		console << e << "\n";
	}
}

void Иср::UscProcessDir(const Ткст& dir)
{
	for(ФайлПоиск ff(приставьИмяф(dir, "*.usc")); ff; ff.следщ())
		UscFile(приставьИмяф(dir, ff.дайИмя()));
}

void Иср::UscProcessDirDeep(const Ткст& dir)
{
	UscProcessDir(dir);
	for(ФайлПоиск ff(приставьИмяф(dir, "*")); ff; ff.следщ())
		if(ff.папка_ли())
			UscProcessDirDeep(ff.дайПуть());
}

void Иср::SyncUsc()
{
	CleanUsc();
	UscProcessDir(GetLocalDir());
	UscProcessDir(дайПапкуФайла(конфигФайл("x")));

	if(пусто_ли(main))
		return;
	::РОбласть wspc;
	wspc.скан(main);
	int i;
	for(i = 0; i < wspc.дайСчёт(); i++) {
		const Пакет& p = wspc.дайПакет(i);
		for(int j = 0; j < p.file.дайСчёт(); j++) {
			Ткст file = SourcePath(wspc[i], p.file[j]);
			if(впроп(дайРасшф(file)) == ".usc")
				UscFile(file);
		}
	}
}

void Иср::CodeBaseSync()
{
	if(auto_check)
		SyncCodeBase();
}

void Иср::SyncWorkspace()
{
	SyncUsc();
	CodeBaseSync();
}

bool текстфл_ли(const Ткст& file, int maxline) {
	byte буфер[16384];
	ФайлВвод fi(file);
	if(!fi.открыт())
		return false;
	int count = fi.дай(буфер, sizeof(буфер) - 1);
	буфер[count] = 0;
	const byte *end = буфер + count;
	const byte *q = буфер;
	const byte *x = q;
	while(q < end) {
		if(*q < 32) {
			int c = *q;
			if(c == '\n') {
				if(q - x > maxline) return false;
				x = q;
			}
			else
			if(c != '\r' && c != '\t' && c != '\v' && c != '\f' && c != 0x1a)
				return false;
		}
		q++;
	}
	return true;
}

/*
Консоль& Иср::GetConsole()
{
	int q = btabs.дайКурсор();
	return q == BFINDINFILES ? console2 : console;
}
*/
void Иср::Renumber() {
	for(int i = 0; i < filedata.дайСчёт(); i++)
		::Renumber(filedata[i].lineinfo);
	editor.Renumber();
}

void Иср::CycleFiles()
{
	if(++tabi >= tablru.дайСчёт())
		tabi = 0;
	if(tabi < tablru.дайСчёт()) {
		blocktabs = true;
		EditFile(tablru[tabi]);
	}
}

void Иср::дезактивируйПо(Ктрл *new_focus)
{
	if(deactivate_save && issaving == 0 && !new_focus && editor.дайДлину64() < 1000000) {
		DeactivationSave(true);
		сохраниФайл();
		DeactivationSave(false);
	}
	ТопОкно::дезактивируйПо(new_focus);
}

void Иср::активируй()
{
	InvalidateFileTimeCache();
	ТопОкно::активируй();
}

bool Иср::Ключ(dword ключ, int count)
{
	dword *k = IdeKeys::AK_DELLINE().ключ;
	if(ключ == k[0] || ключ == k[1]) {
		editor.DeleteLine();
		return true;
	}
	k = IdeKeys::AK_CUTLINE().ключ;
	if(ключ == k[0] || ключ == k[1]) {
		editor.CutLine();
		return true;
	}
	switch(ключ) {
	case K_ALT|K_CTRL_UP:
	case K_ALT|K_CTRL_DOWN:
	case K_ALT_DELETE:
		return filelist.Ключ(ключ, count);
	case K_ALT_UP:
		return filelist.Ключ(K_UP, 0);
	case K_ALT_DOWN:
		return filelist.Ключ(K_DOWN, 0);
	case K_ALT_PAGEUP:
		return package.Ключ(K_UP, 0);
	case K_ALT_PAGEDOWN:
		return package.Ключ(K_DOWN, 0);
	case K_CTRL|K_ALT_LEFT:
		TabsLR( БарТаб::JumpDirLeft );
		return true;
	case K_CTRL|K_ALT_RIGHT:
		TabsLR( БарТаб::JumpDirRight );
		return true;
	case K_CTRL|K_ALT_B:
		TabsStackLR( БарТаб::JumpDirLeft );
		return true;
	case K_CTRL|K_ALT_N:
		TabsStackLR( БарТаб::JumpDirRight );
		return true;
	case K_SHIFT|K_CTRL_O:
		AddFile(WorkspaceWork::ANY_FILE);
		return true;
#ifdef PLATFORM_COCOA
	case K_ALT_KEY|K_KEYUP:
	case K_OPTION_KEY|K_KEYUP:
#endif
	case K_CTRL_KEY|K_KEYUP:
		if(tabi) {
			tabi = 0;
			AddLru();
		}
		return true;
	case K_CTRL_TAB:
#ifdef PLATFORM_COCOA
	case K_ALT|K_TAB:
	case K_OPTION|K_TAB:
#endif
		CycleFiles();
		return true;
	case K_ALT_C|K_SHIFT:
		CodeBrowser();
		return true;
	case K_MOUSE_BACKWARD:
		History(-1);
		return true;
	case K_MOUSE_FORWARD:
		History(1);
		return true;
	default:
		if(ключ >= K_SHIFT_CTRL_0 && ключ <= K_SHIFT_CTRL_9) {
			Bookmark& b = bookmark[ключ - K_SHIFT_CTRL_0];
			b.file = editfile;
			b.pos = editor.GetEditPos();
			MakeTitle();
			return true;
		}
		if(ключ >= K_CTRL_0 && ключ <= K_CTRL_9) {
			GotoBookmark(bookmark[ключ - K_CTRL_0]);
			return true;
		}
	}
	return false;
}

void Иср::GotoBookmark(const Bookmark& b)
{
	if(b.file.пустой()) return;
	EditFile(b.file);
	if(bookmark_pos)
		editor.SetEditPos(b.pos);
}

bool Иср::IsHistDiff(int i)
{
	if(i < 0 || i >= history.дайСчёт())
		return false;
	Bookmark& b = history[i];
	return b.file != editfile || абс(editor.дайКурсор64() - b.pos.cursor) > 20;
}

void Иср::IdePaste(Ткст& data)
{
	data.очисть();
	if(AcceptFiles(Clipboard())) {
		Вектор<Ткст> s = GetFiles(Clipboard());
		for(int i = 0; i < s.дайСчёт(); i++)
			if(файлЕсть(s[i]) && текстфл_ли(s[i], 10000)) {
				int64 len = дайДлинуф(s[i]);
				if(data.дайДлину() + len > 104857600) {
					Exclamation("Размер вставки превышает лимит 100MB.");
					return;
				}
				data.конкат(загрузиФайл(s[i]));
			}
	}
}

void Иср::AddHistory()
{
	if(history.дайСчёт()) {
		if(IsHistDiff(histi))
			++histi;
	}
	else
		histi = 0;
	history.по(histi);
	Bookmark& b = history.верх();
	b.file = editfile;
	b.pos = editor.GetEditPos();
	SetBar();
}

void Иср::EditorEdit()
{
	AddHistory();
	TouchFile(editfile);
}

int  Иср::GetHistory(int d)
{
	if(history.дайСчёт())
		for(int i = histi + (d > 0); i >= 0 && i < history.дайСчёт(); i += d)
			if(IsHistDiff(i))
				return i;
	return -1;
}

void Иср::History(int d)
{
	int i = GetHistory(d);
	if(i >= 0) {
		histi = i;
		GotoBookmark(history[histi]);
		SetBar();
	}
}

void Иср::BookKey(int ключ)
{
	Ключ(ключ, 1);
}

void Иср::DoDisplay()
{
	Точка p = editor.GetColumnLine(editor.дайКурсор64());
	Ткст s;
	s << "Ln " << p.y + 1 << ", Col " << p.x + 1;
	int64 l, h;
	editor.дайВыделение(l, h);
	if(h > l)
		s << ", Sel " << h - l;
	дисплей.устНадпись(s);
	
	ManageDisplayVisibility();
}

void Иср::ManageDisplayVisibility()
{
	дисплей.покажи(!designer);
}

void Иср::SetIdeState(int newstate)
{
	if(newstate != idestate)
	{
		if(newstate == BUILDING)
			build_start_time = дайСисВремя();
		else
		{
			if(idestate == BUILDING && !пусто_ли(build_start_time))
				stat_build_time += int(дайСисВремя() - build_start_time);
			build_start_time = Null;
		}
	}
	idestate = newstate;
	MakeTitle();
	SetBar();
}

void Иср::MakeIcon() {
	Рисунок li = IdeImg::PackageLarge2();
	ШТкст mp = main.вШТкст();
	if(!пусто_ли(mp))
	{
		Размер isz = li.дайРазм();
		ImageDraw idraw(isz);
		Draw& mdraw = idraw.Alpha();
		idraw.DrawImage(0, 0, li);
		mdraw.DrawImage(0, 0, li, белый);
		int fh = DPI(14);
		Размер sz(0, 0);
		Шрифт font;
		while(fh > DPI(8)) {
			font = StdFont(fh);
			sz = дайРазмТекста(mp, font) + Размер(4, 2);
			if(sz.cx <= isz.cx)
				break;
			fh--;
		}
		int x = max((isz.cx - sz.cx) / 2, 0);
		int y = isz.cy - sz.cy;
		idraw.DrawRect(x, y, sz.cx, sz.cy, белый);
		mdraw.DrawRect(x, y, sz.cx, sz.cy, белый);
		idraw.DrawText(x + 2, y + 1, mp, font, чёрный);
		DrawFrame(idraw, x, y, sz.cx, sz.cy, светлоСиний);
		if(state_icon)
			idraw.DrawImage(0, 0, decode(state_icon, 1, IdeImg::IconDebuggingLarge2(),
			                                         2, IdeImg::IconRunningLarge2(),
			                                         IdeImg::IconBuildingLarge2()));
		li = idraw;
	}
	LargeIcon(li);
}

void Иср::устИконку()
{
	int new_state_icon = 0;
	if((bool)debugger && !исрОтладБлокировка_ли()) {
		new_state_icon = 1;
		return;
	}
	else
	if((GetTimeClick() / 800) & 1) {
		if(debugger)
			new_state_icon = 2;
		else
		if(idestate == BUILDING)
			new_state_icon = 3;
	}
	if(state_icon == new_state_icon)
		return;
	state_icon = new_state_icon;
	MakeIcon();
#ifdef PLATFORM_WIN32
	switch(state_icon) {
	case 1:  Иконка(DPI(IdeImg::IconDebugging(), IdeImg::IconDebuggingLarge())); break;
	case 2:  Иконка(DPI(IdeImg::IconRunning(), IdeImg::IconRunningLarge())); break;
	case 3:  Иконка(DPI(IdeImg::IconBuilding(), IdeImg::IconBuildingLarge())); break;
	default: Иконка(DPI(IdeImg::Icon(), IdeImg::PackageLarge()));
	}
#else
	switch(state_icon) {
	case 1:  Иконка(IdeImg::IconDebugging()); break;
	case 2:  Иконка(IdeImg::IconRunning()); break;
	case 3:  Иконка(IdeImg::IconBuilding()); break;
	default: Иконка(IdeImg::Icon());
	}
#endif
}

void Иср::Periodic()
{
	CheckFileUpdate();
	устИконку();
	if(debugger && debugger->окончен() && !исрОтладБлокировка_ли())
		исрЗавершиОтладку();
	if(file_scanned && Ктрл::GetСобытиеLevel() == 0 && EditFileAssistSync2())
		file_scanned = false;
}

const РОбласть& Иср::роблИср() const
{
	static РОбласть wspc;
	static Ткст _main;
	if(main != _main || wspc.дайСчёт() == 0) {
		wspc.скан(main);
		_main = main;
	}
	else {
		for(int i = 0; i < wspc.дайСчёт(); i++)
			if(wspc.дайПакет(i).time != дайВремяф(PackagePath(wspc[i]))) {
				wspc.скан(main);
				break;
			}
	}
	return wspc;
}

void Иср::AddPackage(const Ткст& p)
{
	const РОбласть& wspc = роблИср();
	for(int i = 0; i < wspc.дайСчёт(); i++){
		if(wspc[i] == p)
			return;
	}
	if(!PromptOKCancel("Пакета [* " + p + "] ещё нет в рабочем пространстве.&Добавить его?"))
		return;
	OptItem& m = actual.uses.добавь();
	m.text = p;
	SaveLoadPackage();
}

int Иср::GetPackageIndex()
{
	const РОбласть& wspc = роблИср();
	for(int i = 0; i < wspc.дайСчёт(); i++)
		if(wspc[i] == package.GetCurrentName())
			return i;
	return -1;
}

void Иср::GotoDiffLeft(int line, ДиффДлг *df)
{
	EditFile(df->editfile);
	editor.устКурсор(editor.дайПоз64(line));
	editor.устФокус();
}

void Иср::GotoDiffRight(int line, FileDiff *df)
{
	EditFile(df->GetExtPath());
	editor.устКурсор(editor.дайПоз64(line));
	editor.устФокус();
}

void Иср::Diff()
{
	if(пусто_ли(editfile))
		return;
	FileDiff diffdlg(AnySourceFs());
	diffdlg.diff.WhenLeftLine = THISBACK1(GotoDiffLeft, &diffdlg);
	diffdlg.diff.WhenRightLine = THISBACK1(GotoDiffRight, &diffdlg);
	diffdlg.выполни(editfile);
	Ткст s = diffdlg.GetExtPath();
	if(файлЕсть(s))
		LruAdd(difflru, s);
}

void Иср::DiffWith(const Ткст& path)
{
	if(пусто_ли(editfile) || пусто_ли(path) || max(дайДлинуф(editfile), дайДлинуф(path)) > 100*1024*1024)
		return;
	FileDiff diffdlg(AnySourceFs());
	diffdlg.diff.WhenLeftLine = THISBACK1(GotoDiffLeft, &diffdlg);
	diffdlg.diff.WhenRightLine = THISBACK1(GotoDiffRight, &diffdlg);
	diffdlg.выполни(editfile, path);
}

struct ConflictDiff : ТопОкно {
	Надпись        left, right;
	КтрлДиффТекст diff;
	
	virtual void Выкладка()
	{
		Размер sz = дайРазм();
		int  fy = GetStdFont().GetCy() + DPI(5);
		left.LeftPos(0, sz.cx / 2).TopPos(0, fy);
		right.RightPos(0, sz.cx / 2).TopPos(0, fy);
		diff.HSizePos().VSizePos(fy, 0);
	}

	void уст(const char *lname, const Ткст& l, const char *rname, const Ткст& r)
	{
		left = "\1[=* \1" + Ткст(lname);
		right = "\1[=* \1" + Ткст(rname);
		diff.уст(l, r);
	}
	
	ConflictDiff() {
		устПрям(GetWorkArea().дефлят(DPI(32)));
		Sizeable().Zoomable();
		добавь(left);
		добавь(right);
		добавь(diff);
	}
};

Ткст Иср::LoadConflictFile(const Ткст& n)
{
	return n.дайСчёт() == 1 ? GitCmd(дайПапкуФайла(editfile), "show :" + n + ":./" + дайИмяф(editfile))
	                         : загрузиФайл(n);
}

void Иср::DiffFiles(const char *lname, const Ткст& l, const char *rname, const Ткст& r)
{
	ConflictDiff diff;
	diff.уст(lname, LoadConflictFile(l), rname, LoadConflictFile(r));
	diff.выполни();
}
