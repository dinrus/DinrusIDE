#include "DinrusIDE.h"

EscValue Иср::MacroEditor()
{
	EscValue out;
	out.Escape("дайДлину()", THISBACK(MacroGetLength));
	out.Escape("дайСчётСтрок()", THISBACK(MacroGetLineCount));
	out.Escape("GetLinePos(line)", THISBACK(MacroGetLinePos));
	out.Escape("дайДлинуСтроки(line)", THISBACK(MacroGetLineLength));
	out.Escape("дайКурсор()", THISBACK(MacroGetCursor));
	out.Escape("дайСтроку(pos)", THISBACK(MacroGetLine));
	out.Escape("дайКолонку(pos)", THISBACK(MacroGetColumn));
	out.Escape("GetSelBegin()", THISBACK(MacroGetSelBegin));
	out.Escape("GetSelCount()", THISBACK(MacroGetSelCount));
	out.Escape("устКурсор(pos)", THISBACK(MacroSetCursor));
	out.Escape("устВыделение(begin, count)", THISBACK(MacroSetSelection));
	out.Escape("очистьВыделение()", THISBACK(MacroClearSelection));
	out.Escape("дай(...)", THISBACK(MacroGet));
	out.Escape("удали(...)", THISBACK(MacroRemove));
	out.Escape("вставь(...)", THISBACK(MacroInsert));
	out.Escape("найди(...)", THISBACK(MacroFind));
	out.Escape("найдиСовпадениеingBrace(pos)", THISBACK(MacroнайдиСовпадениеingBrace));
    out.Escape("FindClosingBrace(pos)", THISBACK(MacroFindClosingBrace));
    out.Escape("FindOpeningBrace(pos)", THISBACK(MacroFindOpeningBrace));
    out.Escape("замени(...)", THISBACK(MacroReplace));
	out.Escape("MoveLeft(...)", THISBACK(MacroMoveLeft));
	out.Escape("MoveRight(...)", THISBACK(MacroMoveRight));
	out.Escape("MoveWordLeft(...)", THISBACK(MacroMoveWordLeft));
	out.Escape("MoveWordRight(...)", THISBACK(MacroMoveWordRight));
	out.Escape("MoveUp(...)", THISBACK(MacroMoveUp));
	out.Escape("MoveDown(...)", THISBACK(MacroMoveDown));
	out.Escape("MoveHome(...)", THISBACK(MacroMoveHome));
	out.Escape("MoveEnd(...)", THISBACK(MacroMoveEnd));
	out.Escape("MovePageUp(...)", THISBACK(MacroMovePageUp));
	out.Escape("MovePageDown(...)", THISBACK(MacroMovePageDown));
	out.Escape("двигайВНачТекста(...)", THISBACK(MacroMoveTextBegin));
	out.Escape("двигайВКонТекста(...)", THISBACK(MacroMoveTextEnd));

	out.Escape("EditFile(...)", THISBACK(MacroEditFile));
	out.Escape("SaveCurrentFile()", THISBACK(MacroSaveCurrentFile));
	out.Escape("CloseFile()",THISBACK(MacroCloseFile));
	out.Escape("FileName()", THISBACK(MacroFileName));

	out.Escape("Input(...)", THISBACK(MacroInput));
	out.Escape("ClearConsole()", THISBACK(MacroClearConsole));
	out.Escape("Echo(...)", THISBACK(MacroEcho));

	out.Escape("Build(...)", THISBACK(MacroBuild));
	out.Escape("BuildProject(...)", THISBACK(MacroBuildProject));
	out.Escape("выполни(cmdline)", THISBACK(MacroExecute));
	out.Escape("Launch(cmdline)", THISBACK(MacroLaunch));

	out.Escape("MainPackage()",THISBACK(MacroMainPackage));
	out.Escape("ActivePackage()",THISBACK(MacroActivePackage));
	out.Escape("PackageDir(...)",THISBACK(MacroPackageDir));
	out.Escape("ProjectDir()", THISBACK(MacroPackageDir)); // BW compatibility
	out.Escape("Assembly()",THISBACK(MacroAssembly));
	out.Escape("BuildMethod()",THISBACK(MacroBuildMethod));
	out.Escape("BuildMode()",THISBACK(MacroBuildMode));
	out.Escape("Flags()",THISBACK(MacroFlags));
	out.Escape("PackageFiles(...)",THISBACK(MacroPackageFiles));
	out.Escape("AllPackages()",THISBACK(MacroAllPackages));
	out.Escape("Target()", THISBACK(MacroTarget));

	return out;
}

void Иср::MacroGetLength(EscEscape& e)
{
	e = editor.дайДлину();
}

void Иср::MacroGetLineCount(EscEscape& e)
{
	e = editor.дайСчётСтрок();
}

void Иср::MacroGetLinePos(EscEscape& e)
{
	e = editor.дайПоз(e.Цел(0));
}

void Иср::MacroGetLineLength(EscEscape& e)
{
	e = editor.дайДлинуСтроки(e.Цел(0));
}

void Иср::MacroGetCursor(EscEscape& e)
{
	e = editor.дайКурсор();
}

void Иср::MacroGetLine(EscEscape& e)
{
	e = editor.дайСтроку(e.Цел(0));
}

void Иср::MacroGetColumn(EscEscape& e)
{
	int pos = e.Цел(0);
	editor.GetLinePos(pos);
	e = pos;
}

void Иср::MacroGetSelBegin(EscEscape& e)
{
	int l, h;
	if(editor.дайВыделение(l, h))
		e = l;
	else
		e = editor.дайКурсор();
}

void Иср::MacroGetSelEnd(EscEscape& e)
{
	int l, h;
	if(editor.дайВыделение(l, h))
		e = h;
	else
		e = editor.дайКурсор();
}

void Иср::MacroGetSelCount(EscEscape& e)
{
	int l, h;
	if(editor.дайВыделение(l, h))
		e = h - l;
	else
		e = 0.0;
}

void Иср::MacroSetCursor(EscEscape& e)
{
	editor.устКурсор(e.Цел(0));
}

void Иср::MacroSetSelection(EscEscape& e)
{
	int b = e.Цел(0), c = e.Цел(1);
	if(b < 0 || b > editor.дайДлину() || c < 0 || c > editor.дайДлину() || b + c > editor.дайДлину())
		e.выведиОш(фмт("Неверное выделение: начало = %d, счёт = %d (длина текста = %d)",
			b, c, editor.дайДлину()));
	editor.устВыделение(b, b + c);
}

void Иср::MacroClearSelection(EscEscape& e)
{
	editor.очистьВыделение();
}

void Иср::MacroGet(EscEscape& e)
{
	if(e.дайСчёт() < 1 || e.дайСчёт() > 2)
		e.выведиОш("неверное число аргументов при вызове дай (1 или 2 ожидалось)");
	int pos = e.Цел(0);
	int count = e.дайСчёт() > 1 ? e.Цел(1) : 1;
	if(pos < 0 || pos > editor.дайДлину() || count <= 0 || pos + count > editor.дайДлину())
		e.выведиОш(фмт("ошибка в дай(%d, %d), длина текста = %d", pos, count, editor.дайДлину()));
	e = editor.дайШ(pos, count);
}

void Иср::MacroRemove(EscEscape& e)
{
	int c = e.дайСчёт();
	if(c > 2)
		e.выведиОш("wrong number of arguments in call to удали (0 to 2 expected)");
	int len = editor.дайДлину();
	int cur = editor.дайКурсор();
	if(c == 0) {
		int l, h;
		if(editor.дайВыделение(l, h))
			editor.удали(l, h - l);
		else if(cur < len)
			editor.удали(cur, 1);
	}
	else {
		int start = (c > 1 ? e.Цел(0) : cur);
		int count = e.Цел(c - 1);
		if(count < 0 || count > len || start < 0 || start + count > len)
			e.выведиОш(фмт("cannot remove %d character(s) at position %d, text length is only %d",
				count, start, len));
		editor.удали(start, count);
	}
}

void Иср::MacroInsert(EscEscape& e)
{
	int c = e.дайСчёт();
	if(c < 1 || c > 2)
		e.выведиОш("wrong number of arguments in call to вставь (1 or 2 expected)");
	ШТкст text = e[c - 1];
	editor.вставь(c > 1 ? e.Цел(0) : editor.дайКурсор(), text);
}

void Иср::MacroFind(EscEscape& e)
{
	int n = e.дайСчёт();
	if(n < 1 || n > 6)
		e.выведиОш("wrong number of arguments in call to найди (1 to 5 expected)");
	РедакторКода::FindReplaceData d = editor.GetFindReplaceData();
	bool down = (n <= 1 || e.Цел(1) > 0);
	d.wholeword = (n > 2 && e.Цел(2) > 0);
	d.ignorecase = (n > 3 && e.Цел(3) > 0);
	d.wildcards = (n > 4 && e.Цел(4) > 0);
	d.find = e[0];
	editor.SetFindReplaceData(d);
	e = editor.найди(!down, false);
}

void Иср::MacroReplace(EscEscape& e)
{
	int n = e.дайСчёт();
	if(n < 2 || n > 5)
		e.выведиОш("wrong number of arguments in call to найди (2 to 6 expected)");
	РедакторКода::FindReplaceData d = editor.GetFindReplaceData();
	d.find = e[0];
	d.replace = e[1];
	d.wholeword = (n > 2 && e.Цел(2) > 0);
	d.ignorecase = (n > 3 && e.Цел(3) > 0);
	d.wildcards = (n > 4 && e.Цел(4) > 0);
	d.samecase = (n > 5 && e.Цел(5) > 0);
	editor.SetFindReplaceData(d);
	e = editor.BlockReplace();
}

void Иср::MacroнайдиСовпадениеingBrace(EscEscape& e)
{
    int найдиСовпадениеingBrace(int pos);
}

void Иср::MacroFindClosingBrace(EscEscape& e)
{
    int FindClosingBrace(int pos);
}

void Иср::MacroFindOpeningBrace(EscEscape& e)
{
    int FindOpeningBrace(int pos);
}

void Иср::MacroMoveLeft(EscEscape& e)
{
	if(e.дайСчёт() > 1) e.выведиОш("MoveLeft(sel = false) takes at most 1 parameter");
	editor.MoveLeft(e.дайСчёт() > 0 && e.Цел(0) > 0);
}

void Иср::MacroMoveRight(EscEscape& e)
{
	if(e.дайСчёт() > 1) e.выведиОш("MoveRight(sel = false) takes at most 1 parameter");
	editor.MoveRight(e.дайСчёт() > 0 && e.Цел(0) > 0);
}

void Иср::MacroMoveUp(EscEscape& e)
{
	if(e.дайСчёт() > 1) e.выведиОш("MoveUp(sel = false) takes at most 1 parameter");
	editor.MoveUp(e.дайСчёт() > 0 && e.Цел(0) > 0);
}

void Иср::MacroMoveDown(EscEscape& e)
{
	if(e.дайСчёт() > 1) e.выведиОш("MoveDown(sel = false) takes at most 1 parameter");
	editor.MoveDown(e.дайСчёт() > 0 && e.Цел(0) > 0);
}

void Иср::MacroMoveHome(EscEscape& e)
{
	if(e.дайСчёт() > 1) e.выведиОш("MoveHome(sel = false) takes at most 1 parameter");
	editor.MoveHome(e.дайСчёт() > 0 && e.Цел(0) > 0);
}

void Иср::MacroMoveEnd(EscEscape& e)
{
	if(e.дайСчёт() > 1) e.выведиОш("MoveEnd(sel = false) takes at most 1 parameter");
	editor.MoveEnd(e.дайСчёт() > 0 && e.Цел(0) > 0);
}

void Иср::MacroMovePageUp(EscEscape& e)
{
	if(e.дайСчёт() > 1) e.выведиОш("MovePageUp(sel = false) takes at most 1 parameter");
	editor.MovePageUp(e.дайСчёт() > 0 && e.Цел(0) > 0);
}

void Иср::MacroMovePageDown(EscEscape& e)
{
	if(e.дайСчёт() > 1) e.выведиОш("MovePageDown(sel = false) takes at most 1 parameter");
	editor.MovePageDown(e.дайСчёт() > 0 && e.Цел(0) > 0);
}

void Иср::MacroMoveTextBegin(EscEscape& e)
{
	if(e.дайСчёт() > 1) e.выведиОш("двигайВНачТекста(sel = false) takes at most 1 parameter");
	editor.двигайВНачТекста(e.дайСчёт() > 0 && e.Цел(0) > 0);
}

void Иср::MacroMoveTextEnd(EscEscape& e)
{
	if(e.дайСчёт() > 1) e.выведиОш("двигайВКонТекста(sel = false) takes at most 1 parameter");
	editor.двигайВКонТекста(e.дайСчёт() > 0 && e.Цел(0) > 0);
}

void Иср::MacroMoveWordRight(EscEscape& e)
{
	if(e.дайСчёт() > 1) e.выведиОш("MoveWordRight(sel = false) takes at most 1 parameter");
	int p = editor.дайКурсор();
	int b = p;
	int l = editor.дайДлину();
	if(iscid(editor.дайСим(p)))
		while(p < l && iscid(editor.дайСим(p))) p++;
	else
		while(p < l && !iscid(editor.дайСим(p))) p++;
	if(e.дайСчёт() > 0 && e.Цел(0) > 0)
		editor.устВыделение(b, p);
	else
		editor.устКурсор(p);
}

void Иср::MacroMoveWordLeft(EscEscape& e)
{
	if(e.дайСчёт() > 1) e.выведиОш("MoveWordLeft(sel = false) takes at most 1 parameter");
	int p = editor.дайКурсор();
	if(p == 0) return;
	int b = p;
	if(iscid(editor.дайСим(p - 1)))
		while(p > 0 && iscid(editor.дайСим(p - 1))) p--;
	else
		while(p > 0 && !iscid(editor.дайСим(p - 1))) p--;
	if(e.дайСчёт() > 0 && e.Цел(0) > 0)
		editor.устВыделение(p, b);
	else
		editor.устКурсор(p);
}

void Иср::MacroInput(EscEscape& e)
{
	ТопОкно dialog;
	Вектор<Ткст> tags;
	tags.устСчёт(max(e.дайСчёт(), 1));
	int xdim = 0;
	enum {
		SIDE_GAP = 4,
		LINE_DIST = 22,
		LINE_HEIGHT = 19,
		BUTTON_HEIGHT = 22,
		BUTTON_WIDTH = 80,
	};
	for(int i = 0; i < e.дайСчёт(); i++)
		xdim = max(xdim, дайРазмТекста(tags[i] = e[i], StdFont()).cx + 10);
	dialog.HCenterPos(xdim + 200, 0).VCenterPos(LINE_DIST * tags.дайСчёт() + 2 * SIDE_GAP + BUTTON_HEIGHT, 0);
	Массив<Надпись> label;
	Массив<EditField> editors;
	for(int i = 0; i < tags.дайСчёт(); i++) {
		Надпись& lbl = label.добавь();
		lbl.устНадпись(tags[i]);
		lbl.LeftPos(SIDE_GAP, xdim).TopPos(SIDE_GAP + LINE_DIST * i, LINE_HEIGHT);
		dialog << lbl;
		EditField& fld = editors.добавь();
		fld.HSizePos(SIDE_GAP + xdim, SIDE_GAP).TopPos(SIDE_GAP + LINE_DIST * i, LINE_HEIGHT);
		dialog << fld;
	}
	Кнопка ok, cancel;
	ok.устНадпись("OK") <<= dialog.Acceptor(IDOK);
	cancel.устНадпись("Отмена") <<= dialog.Rejector(IDCANCEL);
	dialog << ok.Ok().BottomPos(SIDE_GAP, BUTTON_HEIGHT).RightPos(2 * SIDE_GAP + BUTTON_WIDTH, BUTTON_WIDTH)
		<< cancel.Cancel().BottomPos(SIDE_GAP, BUTTON_HEIGHT).RightPos(SIDE_GAP, BUTTON_WIDTH);
	dialog.Титул("Бокс ввода макроса");
	if(dialog.пуск() != IDOK) {
		e = EscValue();
		return;
	}
	if(tags.дайСчёт() == 1)
		e = (ШТкст)~editors[0];
	else {
		EscValue out;
		for(int i = 0; i < tags.дайСчёт(); i++)
			out.MapSet(i, (ШТкст)~editors[i]);
		e = out;
	}
}

void Иср::MacroBuild(EscEscape& e)
{
	Ткст outfile;
	Ткст maincfg = mainconfigparam;
	switch(e.дайСчёт()) {
		case 2: outfile = e[1]; break;
		case 1: maincfg = e[0]; break;
		case 0: break;
		default: e.выведиОш("Build: 0 to 2 arguments expected ([maincfg[, outfile]])");
	}
	e = Build(роблИср(), maincfg, outfile, false);
}

void Иср::MacroBuildProject(EscEscape& e)
{
	Ткст outfile;
	switch(e.дайСчёт()) {
		case 3: outfile = e[2];
		case 2: break;
		default:  e.выведиОш("BuildProject: 2 or 3 arguments expected (uppfile, maincfg[, outfile])");
	}
	Ткст uppfile = e[0];
	Ткст maincfg = e[1];
	РОбласть wspc;
	wspc.скан(uppfile);
	e = Build(wspc, maincfg, outfile, false);
}

void Иср::MacroExecute(EscEscape& e)
{
	int time = msecs();
	Ткст cmdline = e[0];
	Хост h;
	CreateHostRunDir(h);
	h.ChDir(Nvl(rundir, дайПапкуФайла(target)));
	ShowConsole();
	вКонсоль(Ткст().конкат() << "MacroExecute: " << cmdline);
	console.синх();
	e = h.выполни(cmdline);
	PutVerbose("Завершено за " + GetPrintTime(time));
}

void Иср::MacroLaunch(EscEscape& e)
{
	Ткст cmdline = e[0];
	Хост h;
	CreateHostRunDir(h);
	h.ChDir(Nvl(rundir, дайПапкуФайла(target)));
	h.Launch(cmdline);
}

void Иср::MacroClearConsole(EscEscape& e)
{
	console.очисть();
}

void Иср::MacroEditFile(EscEscape& e)
{
	Ткст filename;
	if(e.дайСчёт() == 1)
		filename = e[0];
	else if(e.дайСчёт() == 2)
		filename = SourcePath(e[0], e[1]);
	EditFile(filename);
}

void Иср::MacroSaveCurrentFile(EscEscape& e)
{
	сохраниФайл();
}

void Иср::MacroFileName(EscEscape& e)
{
	e = editfile;
}

void Иср::MacroMainPackage(EscEscape& e)
{
	e = GetMain();
}

void Иср::MacroActivePackage(EscEscape& e)
{
	e = GetActivePackage();
}

void Иср::MacroPackageDir(EscEscape& e)
{
	Ткст pkg;
	if(e.дайСчёт() == 0)
		pkg = GetActivePackage();
	else
		pkg = e[0];
	Ткст pp = PackagePathA(pkg);
	if(!файлЕсть(pp))
		e.выведиОш("PackageDir: Пакет не найден.");
	e = дайПапкуФайла(pp);
}

void Иср::MacroAssembly(EscEscape& e)
{
	e = GetVarsName();
}

void Иср::MacroBuildMethod(EscEscape& e)
{
	e = method;
}

void Иср::MacroBuildMode(EscEscape& e)
{
	e = double(targetmode);
}

void Иср::MacroFlags(EscEscape& e)
{
	Вектор<Ткст> v = разбей(mainconfigparam," ");
	EscValue ret;
	for(int i = 0; i < v.дайСчёт(); i++){
		ret.ArrayAdd(v[i]);
	}
	e = ret;
}

void Иср::MacroEcho(EscEscape& e)
{
	ShowConsole();
	for(int i = 0; i < e.дайСчёт(); i++){
		вКонсоль(Ткст(e[i]));
	}
}

void Иср::MacroCloseFile(EscEscape& e)
{
	int n = tabs.дайКурсор();
	if(n>=0)
		tabs.закрой(n,true);
}

void Иср::MacroPackageFiles(EscEscape& e)
{
	Ткст pp;
	Пакет pkg;
	if(e.дайСчёт()==0)
		pp = GetActivePackagePath();
	else
		pp = PackagePathA(Ткст(e[0]));
	if(!файлЕсть(pp))
		e.выведиОш("PackageFiles: Пакет не найден.");
	pkg.грузи(pp);
	EscValue ret;
	for(int i = 0; i < pkg.file.дайСчёт(); i++){
		ret.ArrayAdd(pkg.file[i]);
	}
	e = ret;
}

void Иср::MacroAllPackages(EscEscape& e)
{
	EscValue ret;
	for(int i = 0; i < package.дайСчёт(); i++) {
		Ткст p = package.дай(i).имя;
		if(!IsAux(p))
			ret.ArrayAdd(p);
	}
	e = ret;
}

void Иср::MacroTarget(EscEscape& e)
{
	e = target;
}
