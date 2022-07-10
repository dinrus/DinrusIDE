#include "DinrusIDE.h"


#define IMAGECLASS IdeImg
#define IMAGEFILE  <DinrusIDE/DinrusIDE.iml>
#include <Draw/iml_source.h>

void Иср::ToggleVerboseBuild() {
	console.verbosebuild = !console.verbosebuild;
	
	SetToolBar();
}

void Иср::ToggleStopOnErrors() {
	stoponerrors = !stoponerrors;
}

void Иср::SwapPackagesFiles()
{
	weframe.покажи(weframe.показан_ли() ? false : true);
}

void Иср::ConsoleClear()
{
	console <<= Null;
}

void Иср::ConsoleCopy()
{
	console.копируй();
}

void Иср::ConsolePaste()
{
	Ткст s = ReadClipboardText();
	if(!пусто_ли(s)) {
		s.вставь(0, '\n');
		int len = console.дайДлину();
		console.вставь(len, s.вШТкст());
		console.устКурсор(len + 1);
	}
}

bool Иср::вербозно_ли() const
{
	return console.verbosebuild;
}

void Иср::вКонсоль(const char *s)
{
	console << s << "\n";
}

void Иср::PutVerbose(const char *s)
{
	LOG("VERBOSE: " << s);
	if(console.verbosebuild) {
		вКонсоль(s);
		console.синх();
	}
}

bool Иср::строитсяИср() const
{
	return idestate == Иср::BUILDING;
}

Ткст Иср::исрДайОдинФайл() const
{
	return onefile;
}

int Иср::выполниВКонсИср(const char *cmdline, Поток *out, const char *envptr, bool quiet, bool noconvert)
{
	return console.выполни(cmdline, out, envptr, quiet, noconvert);
}

int Иср::выполниВКонсИсрСВводом(const char *cmdline, Поток *out, const char *envptr, bool quiet, bool noconvert)
{
	ShowConsole();
	console.Input(true);
	int r = console.выполни(cmdline, out, envptr, quiet, noconvert);
	console.Input(false);
	return r;
}

int Иср::выполниВКонсИср(Один<ПроцессИнк> pick_ process, const char *cmdline, Поток *out, bool quiet)
{
	return console.выполни(pick(process), cmdline, out, quiet);
}

int Иср::разместиСлотКонсИср()
{
	return console.разместиСлот();
}

bool Иср::пускКонсИср(const char *cmdline, Поток *out, const char *envptr, bool quiet, int slot, Ткст ключ, int blitz_count)
{
	return console.пуск(cmdline, out, envptr, quiet, slot, ключ, blitz_count);
}

bool Иср::пускКонсИср(Один<ПроцессИнк> pick_ process, const char *cmdline, Поток *out, bool quiet, int slot, Ткст ключ, int blitz_count)
{
	return console.пуск(pick(process), cmdline, out, quiet, slot, ключ, blitz_count);
}

void Иср::слейКонсИср()
{
	console.слей();
}

void Иср::начниГруппуКонсИср(Ткст группа)
{
	console.BeginGroup(группа);
}

void Иср::завершиГруппуКонсИср()
{
	console.EndGroup();
}

bool Иср::ждиКонсИср()
{
	return console.жди();
}

bool Иср::ждиКонсИср(int slot)
{
	console.жди(slot);
	return true;
}

void Иср::приФинишеКонсИср(Событие<>  cb)
{
	console.OnFinish(cb);
}

void Иср::исрУстПраво(Ктрл& ctrl)
{
	right.добавь(ctrl.SizePos());
	right_split.Zoom(-1);
}

void Иср::исрУдалиПраво(Ктрл& ctrl)
{
	ctrl.удали();
	if(!right.дайПервОтпрыск())
		right_split.Zoom(0);
}

bool Иср::исрОтладка_ли() const
{
	return debugger;
}

int Иср::IdeGetHydraThreads()
{
	return hydra1_threads;
}

Ткст Иср::IdeGetCurrentBuildMethod()
{
	return method;
}

Ткст Иср::IdeGetCurrentMainPackage()
{
	return main;
}

void Иср::IdePutErrorLine(const Ткст& line)
{
	if(console.verbosebuild) {
		ЗамкниГип __;
		устНиз(Иср::BERRORS);
		ConsoleRunEnd();
		ConsoleLine(line, true);
	}
}

void Иср::IdeGotoFileAndId(const Ткст& path, const Ткст& id)
{
	GotoFileAndId(path, id);
}

void Иср::исрЗавершиОтладку()
{
	debugger.очисть();
	debuglock = 0;
	MakeTitle();
	editor.EnableBreakpointing();
	SetBar();
	editor.HidePtr();
	for(int i = 0; i < 2; i++)
		posfile[i].очисть();
	PosSync();
	const РОбласть& wspc = роблИср();
	for(int i = 0; i < wspc.дайСчёт(); i++) {
		const Пакет& pk = wspc.дайПакет(i);
		Ткст n = wspc[i];
		for(int i = 0; i < pk.file.дайСчёт(); i++) {
			Ткст file = SourcePath(n, pk.file[i]);
			ValidateBreakpoints(Filedata(file).lineinfo);
		}
	}
	editor.ValidateBreakpoints();
}

void Иср::исрУстПозОтладки(const Ткст& file, int line, const Рисунок& img, int i)
{
	posfile[i] = file;
	posline[i] = line;
	posimg[i] = img;
	EditFile(file);
	editor.GotoLine(line);
	PosSync();
	Размер sz = editor.GetPageSize();
	Точка p = editor.дайПозПромота();
	int l = editor.GetCursorLine();
	if(sz.cy > 8 && l - p.y > sz.cy - 4)
		editor.SetScrollPos(Точка(p.x, max(0, l - 4)));
	устПП();
	editor.устФокус();
}

void Иср::исрСкройУк()
{
	editor.HidePtr();
}

bool Иср::исрОтладБлокируй()
{
	if(debuglock == 0) {
		debuglock = 1;
		MakeTitle();
		SetBar();
		синх();
		return true;
	}
	debuglock++;
	return false;
}

bool Иср::исрОтладРазблокируй()
{
	debuglock--;
	ПРОВЕРЬ(debuglock >= 0);
	if(debuglock == 0) {
		MakeTitle();
		SetBar();
		return true;
	}
	return false;
}

bool Иср::исрОтладБлокировка_ли() const
{
	return debuglock;
}

Ткст Иср::исрДайИмяф() const
{
	return !designer ? editfile : Null;
}

int Иср::исрДайСтрокуф()
{
	return !designer ? editor.GetNoLine(editor.GetCursorLine()) : 0;
}

Ткст Иср::исрДайСтроку(int i) const
{
	if(i >= 0 && i < editor.дайСчётСтрок())
		return editor.дайУтф8Строку(i);
	else
		return Null;
}

void Иср::исрУстБар()
{
	SetBar();
}

void Иср::SetupBars()
{
	очистьФреймы();
	
	int r = HorzLayoutZoom(170);
#ifdef PLATFORM_COCOA
	добавьФрейм(toolbar);
	дисплей.RightPos(4, r).VSizePos(2, 3);
#else
	int l = HorzLayoutZoom(350);
	
	menubar.Transparent();
	if(toolbar_in_row) {
		toolbar.устФрейм(фреймПусто());
		int tcy = max(mainconfiglist.дайСтдРазм().cy + DPI(2), toolbar.GetStdHeight());
		bararea.добавь(menubar.LeftPos(0, l).VCenterPos(menubar.GetStdHeight()));
		bararea.добавь(toolbar.HSizePos(l, r).VCenterPos(tcy));
		bararea.добавь(дисплей.RightPos(4, r).VSizePos(2, 3));
		bararea.устВысоту(max(menubar.GetStdHeight(), tcy));
		добавьФрейм(bararea);
		toolbar.Transparent();
	}
	else {
		bararea.добавь(menubar.LeftPos(0, l).VCenterPos(menubar.GetStdHeight()));
		bararea.добавь(дисплей.RightPos(4, r).VSizePos(2, 3));
		bararea.устВысоту(menubar.GetStdHeight());
		добавьФрейм(bararea);
		добавьФрейм(фреймВерхнСепаратора());
		добавьФрейм(toolbar);
		toolbar.NoTransparent();
	}
#endif
	добавьФрейм(statusbar);
	SetBar();
}

void SetupError(КтрлМассив& Ошибка, const char *s)
{
	Ошибка.добавьКолонку("Файл").устДисплей(Single<Иср::FoundFileDisplay>());
	Ошибка.добавьКолонку("Строка").устДисплей(Single<Иср::TopAlignedDisplay>());
	Ошибка.добавьКолонку(s);
	Ошибка.добавьИндекс("ИНФО");
	Ошибка.ColumnWidths("184 44 298");
	Ошибка.NoWantFocus();
}

void Иср::Выкладка()
{
	дисплей.покажи(!designer && (menubar.дайРазм().cx + дисплей.дайРазм().cx < дайРазм().cx));
}

static void sHighlightLine(const Ткст& path, Вектор<СтрокРедакт::Highlight>& hln, const ШТкст& ln)
{
	Один<EditorSyntax> es = EditorSyntax::создай(EditorSyntax::GetSyntaxForFilename(дайИмяф(path)));
	es->IgnoreErrors();
	HighlightOutput hl(hln);
	es->Highlight(ln.старт(), ln.стоп(), hl, NULL, 0, 0);
}

Иср::Иср()
{
	ДиффДлг::WhenHighlight = callback(sHighlightLine);

	editor.theide = this;
	editor.WhenSel << [=] {
		delayed_toolbar.глушиУст(150, [=] { SetToolBar(); });
	};
	
	editormode = false;
	
	start_time = дайСисВремя();
	stat_build_time = 0;
	build_start_time = Null;
	hydra1_threads = цпбЯдра();
	
	chstyle = 0;

	Sizeable().Zoomable();

	дисплей.устЛин(ALIGN_RIGHT);

	filelist.Columns(2);
	package.Columns(2);

	filetabs = ЛинФрейм::TOP;
	auto_enclose = false;
	mark_lines = true;
	
	persistent_find_replace = false;

	idestate = EDITING;
	debuglock = 0;

	menubar.WhenHelp = ~statusbar;
	menubar.AreaLook(1);
	toolbar.WhenHelp = ~statusbar;
	toolbar.AreaLook(1);
	toolbar_in_row = false;
	WhenClose = THISBACK(выход);

	editorsplit.верт(editor, editor2);
	editorsplit.Zoom(0);
	SyncEditorSplit();
	
	editpane.добавьФрейм(editor.navigatorframe);

	right_split.гориз(editpane, right);
	right_split.Zoom(0);
	
	SetupError(Ошибка, "Сообщение");
	Ошибка.добавьИндекс("ЗАМЕТКИ");
	Ошибка.ColumnWidths("207 41 834");
	Ошибка.WhenBar = THISBACK(ErrorMenu);

	for(int i = 0; i < 3; i++) {
		SetupError(ffound[i], "Источник");
		ffound[i].ColumnWidths("207 41 834");
		ffound[i].колонкаПо(0).устДисплей(Single<FoundFileDisplay>());
		ffound[i].колонкаПо(2).устДисплей(Single<FoundDisplay>());
		ffound[i].WhenBar = THISBACK(FFoundMenu);
		ffound[i].WhenSel = ffound[i].ПриЛевКлике = THISBACK(ShowFound);
	}

	Ошибка.WhenSel = THISBACK(SelError);
	Ошибка.ПриЛевКлике = THISBACK(ShowError);
	console.WhenLine = THISBACK1(ConsoleLine, false);
	console.WhenRunEnd = THISBACK(ConsoleRunEnd);
	
	addnotes = false;
	removing_notes = false;

	editor_bottom.верт(right_split, bottom);
	console.WhenBar = THISBACK(ConsoleMenu);
	editor_bottom.устПоз(8000);
	bottom.устФрейм(btabs);
	bottom.добавь(console.SizePos().устФрейм(фреймПусто()));
	bottom.добавь(Ошибка.SizePos().устФрейм(фреймПусто()));
	bottom.добавь(calc.SizePos().устФрейм(фреймПусто()));
	for(int i = 0; i < 3; i++)
		bottom.добавь(ffound[i].SizePos().устФрейм(фреймПусто()));
	btabs <<= THISBACK(SyncBottom);
	BTabs();
	
	editor.WhenSelectionChanged << [=] {
		editor2.Illuminate(editor.GetIlluminated());
	};

	pfsplit.устПоз(2000);
	pfsplit.верт(package, filelist);
	wepane.добавь(editor_bottom.SizePos());
	wepane.добавьФрейм(weframe.лево(pfsplit, HorzLayoutZoom(280)));
	добавь(wepane.SizePos());

	editor.topsbbutton.ScrollStyle().NoWantFocus().покажи();
	editor.topsbbutton1.ScrollStyle().NoWantFocus().покажи();
	tabs <<= THISBACK(TabFile);
//	tabs.WhenCloseRest = THISBACK1(CloseRest, &tabs);
//	editor2.устФрейм(фреймПусто());
	editor2.theide = this;
	editor2.topsbbutton.ScrollStyle().NoWantFocus().покажи();
	editor2.topsbbutton1.ScrollStyle().NoWantFocus().покажи();
	editor2.WhenLeftDown = THISBACK(SwapEditors);
	editor.WhenAction = THISBACK(EditorEdit);
	editor.WhenBar = THISBACK(EditorMenu);
	editor.WhenFontScroll = THISBACK(EditorFontScroll);
	editor.WhenOpenFindReplace = THISBACK(AddHistory);
	editor.WhenPaste = THISBACK(IdePaste);
	
	editor.WhenFindAll << THISFN(FindFileAll);

	macro_api = MacroEditor();

	mainconfiglist.WhenClick = THISBACK(MainConfig);
	mainconfiglist <<= THISBACK(OnMainConfigList);
	mainconfiglist.NoDropFocus();
	mainconfiglist.NoWantFocus();

	buildmode.WhenClick = THISBACK(SetupOutputMode);
	buildmode.NoWantFocus();
	buildmode.Подсказка("Режим вывода");
	buildmode.AddButton().Подсказка("Метод построения").лево() <<= THISBACK(DropMethodList);
	buildmode.AddButton().Подсказка("Режим построения") <<= THISBACK(DropModeList);
	methodlist.Normal();
	methodlist.WhenSelect = THISBACK(SelectMethod);
	modelist.Normal();
	modelist.WhenSelect = THISBACK(SelectMode);
	modelist.добавь("Отладка");
	modelist.добавь("Выпуск");

	tabi = 0;
	blocktabs = false;

	package.WhenBar = THISBACK(PackageMenu);
	editor.WhenState = THISBACK(MakeTitle);

	package.NoWantFocus();
	filelist.NoWantFocus();
	filelist.WhenLeftDouble = THISBACK(Группа);

	Иконка(IdeImg::Package(), IdeImg::Package());

	tfont = editorfont = font2 = veditorfont = CourierZ(12);
	consolefont = font1 = CourierZ(10);
	editortabsize = 4;
	indent_amount = 4;
	indent_spaces = false;
	show_status_bar = false;
	show_tabs = false;
	show_spaces = false;
	warnwhitespace = true;
	tabs_icons = false;
	tabs_crosses = ЛинФрейм::RIGHT;
	tabs_grouping = true;
	tabs_stacking = false;
	tabs_serialize = true;
	no_parenthesis_indent = false;
#ifdef PLATFORM_POSIX
	line_endings = DETECT_LF;
#else
	line_endings = DETECT_CRLF;
#endif
	spellcheck_comments = LNG_ENGLISH;
	wordwrap_comments = true;
#ifdef PLATFORM_COCOA
	setmain_newide = true;
#else
	setmain_newide = false;
#endif
	/*
		astyle code formatter control vars
		added 2008.01.27 by Massimo Del Fedele
	*/
	astyle_BracketIndent = false;
	astyle_NamespaceIndent = true;
	astyle_BlockIndent = false;
	astyle_CaseIndent = true;
	astyle_ClassIndent = true;
	astyle_LabelIndent = true;
	astyle_SwitchIndent = true;
	astyle_PreprocessorIndent = false;
	astyle_MinInStatementIndentLength = 2;
	astyle_MaxInStatementIndentLength = 20;
	astyle_BreakClosingHeaderBracketsMode = true;
	astyle_BreakElseIfsMode = true;
	astyle_BreakOneLineBlocksMode = true;
	astyle_SingleStatementsMode = true;
	astyle_BreakBlocksMode = true;
	astyle_BreakClosingHeaderBlocksMode = true;
	astyle_BracketFormatMode = astyle::BREAK_MODE;
	astyle_ParensPaddingMode = astyle::PAD_BOTH;
	astyle_ParensUnPaddingMode = true;
	astyle_OperatorPaddingMode = true;
	astyle_EmptyLineFill = false;
	astyle_TabSpaceConversionMode = false;
	astyle_TestBox = "#include <stdio.h>\n#ifndef __abcd_h\n#include <abcd.h>\n#endif\n\nvoid test(int a, int b)\n{\n  /* this is a switch */\n  switch(a)\n\n  {\n    case 1:\n      b = 2;\n      break;\n    case 2:\n      b = 4;\n      break;\n    default:\n    break;\n  }\n\n  /* this are more statements on one line */\n  a = 2*a;b=-5;a=2*(b+2)*(a+3)/4;\n\n  /* single line blocks */\n  {int z;z = 2*a+b;}\n\n  /* loop */\n  for(int i = 0;i< 10;i++) { a = b+2*i;}\n\n}\n";
	
	console.WhenSelect = THISBACK(FindError);
	console.устСлоты(hydra1_threads);

	editor.WhenSelection = THISBACK(DoDisplay);
	stoponerrors = true;
	hilite_scope = 1;
	hilite_bracket = 1;
	hilite_ifdef = 1;
	barline = true;
	qtfsel = true;
	hilite_if_endif = false;
	thousands_separator = true;
	hline = vline = true;
	wrap_console_text = true;
	mute_sounds = false;
	line_numbers = false;

	use_target = true;

	runmode = RUN_WINDOW;
	runexternal = false;
	consolemode = 0;
	console_utf8 = false;

	browser_closeesc = true;

	bookmark_pos = true;

	header_guards = true;
	insert_include = 1;

	устОбрвызВремени(-20, THISBACK(Periodic), TIMEID_PERIODIC);

	editor.WhenBreakpoint = THISBACK(OnBreakpoint);

	ConstructFindInFiles();

	ff.style <<= STYLE_NO_REPLACE;

	default_charset = CHARSET_UTF8;

	TheIde(this);

	targetmode = 0;

	doc.WhenTopic = THISBACK(OpenATopic);

	editor.NoCutLine();

	bordercolumn = 96;
	bordercolor = SColorFace();

	state_icon = -1;

	histi = 0;

	doc_serial = -1;

	showtime = true;
	
	editor.WhenTip = THISBACK(EditorTip);
	editor.WhenCtrlClick = THISBACK(CtrlClick);
	
	find_pick_sel = true;
	find_pick_text = false;
	
	deactivate_save = true;
	
	output_per_assembly = true;
	
	issaving = 0;
	isscanning = 0;
	
	linking = false;
	
	error_count = 0;
	warning_count = 0;
	
	editor.WhenUpdate = THISBACK(TriggerAssistSync);

	editfile_isfolder = false;
	editfile_repo = NOT_REPO_DIR;
	
	auto_rescan = auto_check = true;
	file_scan = 0;

	editfile_line_endings = Null;

	HideBottom();
	SetupBars();
	SetBar();

#ifdef PLATFORM_COCOA
	WhenDockMenu = [=](Бар& bar) {
		bar.добавь("Открыть главный пакет..", [=] {
			Хост h;
			CreateHost(h, false, false);
			h.Launch(дайФПутьИсп() + " --nosplash");
		});
	};
#endif
}

Иср::~Иср()
{
	TheIde(NULL);
}

void Иср::рисуй(Draw&) {}
