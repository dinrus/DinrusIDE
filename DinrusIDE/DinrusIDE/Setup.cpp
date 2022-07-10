#include "DinrusIDE.h"

#define LLOG(x) // LOG(x)

int CharFilterVar(int c)
{
	return IsAlNum(c) || c == '_' ? c : 0;
}

void DlCharset(СписокБроса& d)
{
	d.добавь(CHARSET_UTF8, "UTF8");
	d.добавь(ТекстКтрл::CHARSET_UTF8_BOM, "UTF8 BOM");
	d.добавь(ТекстКтрл::CHARSET_UTF16_LE, "UTF16 LE");
	d.добавь(ТекстКтрл::CHARSET_UTF16_BE, "UTF16 BE");
	d.добавь(ТекстКтрл::CHARSET_UTF16_LE_BOM, "UTF16 LE BOM");
	d.добавь(ТекстКтрл::CHARSET_UTF16_BE_BOM, "UTF16 BE BOM");
	for(int i = 1; i < счётНабСим(); i++)
		d.добавь(i, имяНабСим(i));
}

class FontSelectManager {
	СписокБроса *face;
	СписокБроса *height;
	Опция   *bold;
	Опция   *italic;

	void FaceSelect();
	void выдели();

public:
	Событие<>   WhenAction;

	typedef FontSelectManager ИМЯ_КЛАССА;

	void уст(СписокБроса& _face, СписокБроса& _height,
	         Опция& _bold, Опция& _italic, bool gui = false);
	void уст(Шрифт f);
	Шрифт дай();
};

void FontSelectManager::FaceSelect() {
	выдели();
}

void FontSelectManager::выдели() {
	WhenAction();
}

void LoadFonts(СписокБроса *face, Индекс<Ткст>& fni, bool fixed)
{
	for(int i = 0; i < Шрифт::GetFaceCount(); i++)
		if(!!(Шрифт::GetFaceInfo(i) & Шрифт::FIXEDPITCH) == fixed) {
			Ткст n = Шрифт::GetFaceName(i);
			if(fni.найди(n)
			 < 0) {
				fni.добавь(n);
				face->добавь(i, n);
			}
		}
}

void FontSelectManager::уст(СписокБроса& _face, СписокБроса& _height,
                            Опция& _bold, Опция& _italic, bool gui) {
	face = &_face;
	face->WhenAction = THISBACK(FaceSelect);
	height = &_height;
	height->WhenAction = THISBACK(выдели);
	bold = &_bold;
	bold->WhenAction = THISBACK(выдели);
	italic = &_italic;
	italic->WhenAction = THISBACK(выдели);
	face->очисть();
	if(gui) {
		face->добавь(Шрифт::ARIAL);
		face->добавь(Шрифт::ROMAN);
		face->добавь(Шрифт::COURIER);
		for(int i = Шрифт::COURIER + 1; i < Шрифт::GetFaceCount(); i++)
			if(Шрифт::GetFaceInfo(i) & Шрифт::SCALEABLE)
				face->добавь(i);

		struct DisplayFace : public Дисплей {
			void рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const {
				int ii = q;
				Шрифт fnt = StdFont();
				if(!(Шрифт::GetFaceInfo(ii) & Шрифт::SPECIAL))
					fnt.Face(ii);
				w.DrawRect(r, paper);
				w.DrawText(r.left, r.top + (r.устВысоту() - fnt.Info().дайВысоту()) / 2,
				           Шрифт::GetFaceName((int)q), fnt, ink);
			}
		};
		face->устДисплей(Single<DisplayFace>());
	}
	else {
		Индекс<Ткст> fni;
		LoadFonts(face, fni, true);
		face->добавьСепаратор();
		LoadFonts(face, fni, false);
		face->SetIndex(0);
		height->очистьСписок();
	}
	for(int i = 6; i < 64; i++)
		height->добавь(i);
	FaceSelect();
}

void FontSelectManager::уст(Шрифт f) {
	int fi = f.GetFace();
	if(!face->HasKey(fi)) {
		fi = face->найдиЗначение(f.GetFaceName());
		if(fi < 0)
			fi = Шрифт::COURIER;
		else
			fi = face->дайКлюч(fi);
	}
	face->устДанные(fi);
	FaceSelect();
	height->устДанные(f.дайВысоту());
	for(int i = 0; i < height->дайСчёт(); i++) {
		int q = height->дайКлюч(i);
		if(f.дайВысоту() <= q) {
			height->устДанные(q);
			break;
		}
	}
	*bold = f.IsBold();
	*italic = f.IsItalic();
}

Шрифт FontSelectManager::дай() {
	Шрифт f(face->дайДанные(), height->дайДанные());
	if(*bold) f.Bold();
	if(*italic) f.Italic();
	return f;
}

void Иср::UpdateFormat(РедакторКода& editor)
{
	if(!IsActiveFile() || ActiveFile().tabsize <= 0)
		editor.TabSize(editortabsize);
	editor.IndentSpaces(indent_spaces);
	editor.IndentAmount(indent_amount);
	editor.ShowTabs(show_tabs);
	editor.ShowSpaces(show_spaces);
	editor.ShowLineEndings(show_tabs);
	editor.WarnWhiteSpace(warnwhitespace);
	editor.NoParenthesisIndent(no_parenthesis_indent);
	editor.HiliteScope(hilite_scope);
	editor.HiliteBracket(hilite_bracket);
	editor.HiliteIfDef(hilite_ifdef);
	editor.BarLine(barline);
	editor.HiliteIfEndif(hilite_if_endif);
	editor.ThousandsSeparator(thousands_separator);
	editor.ShowCurrentLine(hline ? HighlightSetup::GetHlStyle(HighlightSetup::SHOW_LINE).color : (Цвет)Null);
	editor.ShowCurrentColumn(vline ? HighlightSetup::GetHlStyle(HighlightSetup::SHOW_COLUMN).color : (Цвет)Null);
	editor.LineNumbers(line_numbers);
	editor.AutoEnclose(auto_enclose);
	editor.MarkLines(mark_lines);
	editor.BorderColumn(bordercolumn, bordercolor);
	editor.PersistentFindReplace(persistent_find_replace);
	editor.FindReplaceRestorePos(find_replace_restore_pos);
	editor.освежи();
}

void Иср::UpdateFormat() {
	SetupEditor();
	UpdateFormat(editor);
	UpdateFormat(editor2);
	console.устШрифт(consolefont);
	console.WrapText(wrap_console_text);
	statusbar.покажи(show_status_bar);
	SetupBars();
	
	if(!designer) {
		if(filetabs >=0) {
			tabs.устЛин(filetabs);
			editpane.устФрейм(tabs);
		}
		else
			editpane.устФрейм(фреймПусто());
	}

	tabs.сГруппингом(tabs_grouping);
	tabs.сСтэкингом(tabs_stacking);
	tabs.FileIcons(tabs_icons, false);
	tabs.сКроссами(tabs_crosses >= 0, tabs_crosses);
}

void Иср::EditorFontScroll(int d)
{
	if(!IsActiveFile())
		return;
	Пакет::Файл& f = ActiveFile();
	if(f.font != 0 || editorsplit.GetZoom() < 0)
		return;
	int h = editorfont.GetCy();
	int q = editorfont.дайВысоту();
	while(editorfont.GetCy() == h && (d < 0 ? editorfont.GetCy() > 5 : editorfont.GetCy() < 40))
		editorfont.устВысоту(q += d);
	editor.устШрифт(editorfont);
	editor.EditorBarLayout();
}

struct FormatDlg : ВкладкаДлг {
	ColorPusher hl_color[РедакторКода::HL_COUNT];
};

void HlPusherFactory(Один<Ктрл>& ctrl)
{
	ctrl.создай<ColorPusher>().неПусто().Track();
}

void Иср::ReadHlStyles(КтрлМассив& hlstyle)
{
	hlstyle.очисть();
	for(int i = 0; i < РедакторКода::HL_COUNT; i++) {
		const HlStyle& s = editor.GetHlStyle(i);
		hlstyle.добавь(editor.GetHlName(i), s.color, s.bold, s.italic, s.underline);
	}
}

class AStyleSetupDialog : public WithSetupAstyleLayout<КтрлРодитель> {
	Иср *ide;

	typedef AStyleSetupDialog ИМЯ_КЛАССА;

public:
	AStyleSetupDialog(Иср *_ide);
	void AstyleTest();
	void UppDefaults();
};

AStyleSetupDialog::AStyleSetupDialog(Иср *_ide)
{
	ide = _ide;

	BracketFormatMode.добавь(astyle::NONE_MODE, "none");
	BracketFormatMode.добавь(astyle::ATTACH_MODE, "attach");
	BracketFormatMode.добавь(astyle::BREAK_MODE, "break");
	ParensPaddingMode.добавь(astyle::PAD_NONE, "no space pad around parenthesis");
	ParensPaddingMode.добавь(astyle::PAD_INSIDE, "pad parenthesis inside with space");
	ParensPaddingMode.добавь(astyle::PAD_OUTSIDE, "pad parenthesis outside with space");
	ParensPaddingMode.добавь(astyle::PAD_BOTH, "pad both parenthesis sides with пробелы");

	Test <<= THISBACK(AstyleTest);
	Defaults << THISBACK(UppDefaults);

}

void AStyleSetupDialog::AstyleTest()
{
	astyle::ASFormatter Formatter;

	// sets up parameters from astyle dialog
	Formatter.setBracketIndent(BracketIndent);
	Formatter.setNamespaceIndent(NamespaceIndent);
	Formatter.setBlockIndent(BlockIndent);
	Formatter.setCaseIndent(CaseIndent);
	Formatter.setClassIndent(ClassIndent);
	Formatter.setLabelIndent(LabelIndent);
	Formatter.setSwitchIndent(SwitchIndent);
	Formatter.setPreprocessorIndent(PreprocessorIndent);
	Formatter.setMaxInStatementIndentLength(MaxInStatementIndentLength);
	Formatter.setMinConditionalIndentLength(MinInStatementIndentLength);
	Formatter.setBreakClosingHeaderBracketsMode(BreakClosingHeaderBracketsMode);
	Formatter.setBreakElseIfsMode(BreakElseIfsMode);
	Formatter.setBreakOneLineBlocksMode(BreakOneLineBlocksMode);
	Formatter.setSingleStatementsMode(SingleStatementsMode);
	Formatter.setBreakBlocksMode(BreakBlocksMode);
	Formatter.setBreakClosingHeaderBlocksMode(BreakClosingHeaderBlocksMode);
	Formatter.setBracketFormatMode((astyle::BracketMode)BracketFormatMode.дайИндекс());
	switch(ParensPaddingMode.дайИндекс()) {
	case astyle::PAD_INSIDE :
		Formatter.setParensInsidePaddingMode(true);
		Formatter.setParensOutsidePaddingMode(false);
		break;
	case astyle::PAD_OUTSIDE :
		Formatter.setParensInsidePaddingMode(false);
		Formatter.setParensOutsidePaddingMode(true);
		break;
	case astyle::PAD_BOTH :
		Formatter.setParensInsidePaddingMode(true);
		Formatter.setParensOutsidePaddingMode(true);
		break;
	default :
		Formatter.setParensOutsidePaddingMode(false);
		Formatter.setParensInsidePaddingMode(false);
		break;
	}
	Formatter.setParensUnPaddingMode(ParensUnPaddingMode);
	Formatter.setOperatorPaddingMode(OperatorPaddingMode);
	Formatter.setEmptyLineFill(EmptyLineFill);
	Formatter.setTabSpaceConversionMode(TabSpaceConversionMode);
	Formatter.setTabIndentation(ide->editortabsize, ide->indent_spaces ? false : true);
	Formatter.setSpaceIndentation(ide->indent_spaces ? ide->indent_amount : ide->editortabsize);

	// formats text in test box
	TestBox.уст(ide->FormatCodeString(TestBox.дайШ(), Formatter));
}

void AStyleSetupDialog::UppDefaults()
{
	BracketIndent = false;
	NamespaceIndent = true;
	BlockIndent = false;
	CaseIndent = true;
	ClassIndent = true;
	LabelIndent = true;
	SwitchIndent = true;
	PreprocessorIndent = false;
	MaxInStatementIndentLength = 20;
	MinInStatementIndentLength = 2;
	BreakClosingHeaderBracketsMode = 0;
	BreakElseIfsMode = true;
	BreakOneLineBlocksMode = true;
	SingleStatementsMode = true;
	BreakBlocksMode = false;
	BreakClosingHeaderBlocksMode = false;
	BracketFormatMode.SetIndex(1);
	ParensPaddingMode.SetIndex(0);
	ParensUnPaddingMode = true;
	OperatorPaddingMode = false;
	EmptyLineFill = false;
	TabSpaceConversionMode = false;
}

void SetConsole(EditString *e, const char *text)
{
	*e <<= text;
}

void AddPath(EditString *es)
{
	Ткст s = SelectDirectory();
	if(пусто_ли(s))
		return;
	Ткст h = ~*es;
	if(h.дайСчёт() && *h.последний() != ';')
		h << ';';
	h << s;
	*es <<= h;
	es->SetWantFocus();
}

void InsertPath(EditString *es)
{
	Ткст s = SelectDirectory();
	if(пусто_ли(s))
		return;
	es->очисть();
	*es <<= s;
	es->SetWantFocus();
}

void DlSpellerLangs(СписокБроса& dl)
{
	static Вектор<int> lng;
	ONCELOCK {
		ВекторМап<int, Ткст> lngs;
		Ткст path = дайФайлИзПапкиИсп("speller") + ';' + конфигФайл("speller") + ';' +
		              дайПапкуИсп() + ';' + дайКонфигПапку() + ';' +
		              getenv("LIB") + ';' + getenv("PATH");
#ifdef PLATFORM_POSIX
		path << "/usr/local/share/upp/speller;/usr/local/share/upp;/usr/share/upp/speller;/usr/share/upp";
#endif
		Вектор<Ткст> p = разбей(path, ';');
		for(auto dir : p) {
			ФайлПоиск ff(приставьИмяф(dir, "*.udc"));
			while(ff) {
				int lang = LNGFromText(ff.дайИмя());
				if(lang)
					lngs.дайДобавь(lang) = LNGAsText(lang);
				ff.следщ();
			}
		}
		SortByValue(lngs);
		lng = lngs.подбериКлючи();
	}
	dl.добавь(0, "Off");
	for(auto l : lng)
		dl.добавь(l, LNGAsText(l));
}

void Иср::SetupFormat() {
	FormatDlg dlg;
	dlg.Титул("Настройки");
	WithSetupFontLayout<КтрлРодитель> fnt;
	WithSetupHlLayout<КтрлРодитель> hlt;
	WithSetupEditorLayout<КтрлРодитель> edt;
	WithSetupIdeLayout<КтрлРодитель> ide;
	WithSetupAssistLayout<КтрлРодитель> assist;
	WebSearchTab web_search;
	AStyleSetupDialog ast(this);
	edt.lineends
		.добавь(LF, "LF")
		.добавь(CRLF, "CRLF")
		.добавь(DETECT_LF, "Detect with default LF")
		.добавь(DETECT_CRLF, "Detect with default CRLF");
	
	edt.filetabs
		.добавь(ЛинФрейм::LEFT, "лево")
		.добавь(ЛинФрейм::TOP, "верх")
		.добавь(ЛинФрейм::RIGHT, "право")
		.добавь(ЛинФрейм::BOTTOM, "низ")
		.добавь(-1, "Off");
		
	edt.tabs_crosses
		.добавь(ЛинФрейм::LEFT, "лево")
		.добавь(ЛинФрейм::RIGHT, "право")
		.добавь(-1, "Off");
	
	dlg.добавь(fnt, "Шрифты");
	dlg.добавь(hlt, "Подсветка синтаксиса");
	dlg.добавь(edt, "Редактор");
	dlg.добавь(assist, "Ассист");
	dlg.добавь(ide, "ИСР");
	dlg.добавь(ast, "Форматирование кода");
	dlg.добавь(web_search, "Веб-поиск");
	dlg.WhenClose = dlg.Acceptor(IDEXIT);

#ifdef PLATFORM_WIN32
	ide.xterm.устНадпись("powershell");
	ide.xterm << [&] { ide.console <<= "powershell.exe"; };
	ide.gnome.устНадпись("cmd");
	ide.gnome << [&] { ide.console <<= "cmd.exe"; };
	ide.kde.скрой();
	ide.mate.скрой();
	ide.lxde.скрой();
#else
	ide.kde <<= callback2(SetConsole, &ide.console, "/usr/bin/konsole -e");
	ide.gnome <<= callback2(SetConsole, &ide.console, "/usr/bin/gnome-terminal -x");
	ide.mate <<= callback2(SetConsole, &ide.console, "/usr/bin/mate-terminal -x");
	ide.lxde <<= callback2(SetConsole, &ide.console, "/usr/bin/lxterminal -e");
	ide.xterm <<= callback2(SetConsole, &ide.console, "/usr/bin/xterm -e");
#endif

	FontSelectManager ed, vf, con, f1, f2, tf, gui;
	ed.уст(fnt.face, fnt.height, fnt.bold, fnt.italic);
	vf.уст(fnt.vface, fnt.vheight, fnt.vbold, fnt.vitalic);
	con.уст(fnt.cface, fnt.cheight, fnt.cbold, fnt.citalic);
	tf.уст(fnt.tface, fnt.theight, fnt.tbold, fnt.titalic);
	f1.уст(fnt.face1, fnt.height1, fnt.bold1, fnt.italic1);
	f2.уст(fnt.face2, fnt.height2, fnt.bold2, fnt.italic2);
	gui.уст(ide.face, ide.height, ide.bold, ide.italic, true);

	ed.уст(editorfont);
	vf.уст(veditorfont);
	con.уст(consolefont);
	tf.уст(tfont);
	f1.уст(font1);
	f2.уст(font2);
	gui.уст(gui_font);

	DlCharset(edt.charset);
	edt.tabsize.минмакс(1, 100).неПусто();
	edt.tabsize <<= editortabsize;
	edt.indent_amount.минмакс(1, 100).неПусто();
	edt.indent_amount <<= indent_spaces ? indent_amount : editortabsize;
	edt.indent_amount.вкл(indent_spaces);
	CtrlRetriever rtvr;
	int hs = hilite_scope;

	DlSpellerLangs(edt.spellcheck_comments);
	
	web_search.грузи();

	rtvr
		(hlt.hilite_scope, hs)
		(hlt.hilite_bracket, hilite_bracket)
		(hlt.hilite_ifdef, hilite_ifdef)
		(hlt.hilite_if_endif, hilite_if_endif)
		(hlt.thousands_separator, thousands_separator)
		(hlt.hline, hline)
		(hlt.vline, vline)

		(edt.indent_spaces, indent_spaces)
		(edt.no_parenthesis_indent, no_parenthesis_indent)
		(edt.showtabs, show_tabs)
		(edt.showspaces, show_spaces)
		(edt.warnwhitespace, warnwhitespace)
		(edt.lineends, line_endings)
		(edt.numbers, line_numbers)
		(edt.bookmark_pos, bookmark_pos)
		(edt.bordercolumn, bordercolumn)
		(edt.bordercolor, bordercolor)
		(edt.findpicksel, find_pick_sel)
		(edt.findpicktext, find_pick_text)
		(edt.deactivate_save, deactivate_save)
		(edt.filetabs, filetabs)
		(edt.tabs_icons, tabs_icons)
		(edt.tabs_crosses, tabs_crosses)
		(edt.tabs_grouping, tabs_grouping)
		(edt.tabs_stacking, tabs_stacking)
		(edt.tabs_serialize, tabs_serialize)
		(edt.spellcheck_comments, spellcheck_comments)
		(edt.wordwrap_comments, wordwrap_comments)
		(edt.persistent_find_replace, persistent_find_replace)
		(edt.find_replace_restore_pos, find_replace_restore_pos)

		(assist.barline, barline)
		(assist.auto_enclose, auto_enclose)
		(assist.commentdp, editor.commentdp)
		(assist.header_guards, header_guards)
		(assist.insert_include, insert_include)
		(assist.mark_lines, mark_lines)
		(assist.qtfsel, qtfsel)
		(assist.assist, editor.auto_assist)
		(assist.rescan, auto_rescan)
		(assist.check, auto_check)
		(assist.navigator_right, editor.navigator_right)

		(ide.showtime, showtime)
		(ide.show_status_bar, show_status_bar)
		(ide.toolbar_in_row, toolbar_in_row)
		(ide.splash_screen, splash_screen)
		(ide.sort, sort)
		(ide.mute_sounds, mute_sounds)
		(ide.wrap_console_text, wrap_console_text)
		(ide.hydra1_threads, hydra1_threads)
		(ide.chstyle, chstyle)
		(ide.console, HostConsole)
		(ide.output_per_assembly, output_per_assembly)
		(ide.setmain_newide, setmain_newide)
		(ide.gui_font, gui_font_override)

		(ast.BracketIndent,					astyle_BracketIndent)
		(ast.NamespaceIndent,               astyle_NamespaceIndent)
		(ast.BlockIndent,                   astyle_BlockIndent)
		(ast.CaseIndent,                    astyle_CaseIndent)
		(ast.ClassIndent,                   astyle_ClassIndent)
		(ast.LabelIndent,                   astyle_LabelIndent)
		(ast.SwitchIndent,                  astyle_SwitchIndent)
		(ast.PreprocessorIndent,            astyle_PreprocessorIndent)
		(ast.MinInStatementIndentLength,    astyle_MinInStatementIndentLength)
		(ast.MaxInStatementIndentLength,    astyle_MaxInStatementIndentLength)
		(ast.BreakClosingHeaderBracketsMode,astyle_BreakClosingHeaderBracketsMode)
		(ast.BreakElseIfsMode,              astyle_BreakElseIfsMode)
		(ast.BreakOneLineBlocksMode,        astyle_BreakOneLineBlocksMode)
		(ast.SingleStatementsMode,          astyle_SingleStatementsMode)
		(ast.BreakBlocksMode,               astyle_BreakBlocksMode)
		(ast.BreakClosingHeaderBlocksMode,  astyle_BreakClosingHeaderBlocksMode)
		(ast.BracketFormatMode,             astyle_BracketFormatMode)
		(ast.ParensPaddingMode,             astyle_ParensPaddingMode)
		(ast.ParensUnPaddingMode,           astyle_ParensUnPaddingMode)
		(ast.OperatorPaddingMode,           astyle_OperatorPaddingMode)
		(ast.EmptyLineFill,                 astyle_EmptyLineFill)
		(ast.TabSpaceConversionMode,        astyle_TabSpaceConversionMode)
		(ast.TestBox,						astyle_TestBox)
	;
	hlt.hlstyle.добавьКолонку("Стиль");
	hlt.hlstyle.добавьКолонку("Цвет").Ctrls(HlPusherFactory);
	hlt.hlstyle.добавьКолонку("Жирный").Ctrls<Опция>();
	hlt.hlstyle.добавьКолонку("Курсив").Ctrls<Опция>();
	hlt.hlstyle.добавьКолонку("Подчёркнутый").Ctrls<Опция>();
	hlt.hlstyle.ColumnWidths("211 80 45 45 80");
	hlt.hlstyle.EvenRowColor().NoHorzGrid().SetLineCy(EditField::GetStdHeight() + 2);
	ReadHlStyles(hlt.hlstyle);
	edt.charset <<= (int)default_charset;
	edt.tabsize.WhenAction = rtvr <<=
		hlt.hlstyle.WhenCtrlsAction = ed.WhenAction = tf.WhenAction =
		con.WhenAction = f1.WhenAction = f2.WhenAction = dlg.Breaker(222);
	ide.showtimeafter <<= Nvl((Дата)дайВремяф(конфигФайл("version")), дайСисДату() - 1);
	hlt.hl_restore <<= dlg.Breaker(333);
	hlt.hl_restore_white <<= dlg.Breaker(334);
	hlt.hl_restore_dark <<= dlg.Breaker(335);
	
	for(auto sk : GetAllChSkins())
		ide.chstyle.добавь(ide.chstyle.дайСчёт(), sk.b);

	Ткст usc_path = дайФайлИзДомПапки("usc.path");
	ide.uscpath <<= загрузиФайл(usc_path);
	выборДир(ide.uscpath, ide.uscpath_sel);

	ide.upphub <<= загрузиФайл(DefaultHubFilePath());
	ide.upphub.NullText(конфигФайл("UppHub"));
	выборДир(ide.upphub, ide.upphub_sel);
	
	fnt.defaults << [&] {
		Иср def;

		ed.уст(def.editorfont);
		vf.уст(def.veditorfont);
		con.уст(def.consolefont);
		tf.уст(def.tfont);
		f1.уст(def.font1);
		f2.уст(def.font2);
	};

	for(;;) {
		int c = dlg.пуск();

		SetHubDir(~ide.upphub);

		if(пусто_ли(ide.uscpath))
			удалифл(usc_path);
		else
			РНЦП::сохраниФайл(usc_path, ~ide.uscpath);

		editorfont = ed.дай();
		tfont = tf.дай();
		veditorfont = vf.дай();
		consolefont = con.дай();
		font1 = f1.дай();
		font2 = f2.дай();
		gui_font = gui.дай();
		
		editortabsize = Nvl((int)~edt.tabsize, 4);
		rtvr.Retrieve();
		console.устСлоты(minmax(hydra1_threads, 1, 256));
		hilite_scope = hs;
		if(indent_spaces)
			indent_amount = ~edt.indent_amount;
		else {
			indent_amount = editortabsize;
			edt.indent_amount <<= editortabsize;
		}
		edt.indent_amount.вкл(indent_spaces);
		default_charset = (byte)(int)~edt.charset;
		for(int i = 0; i < РедакторКода::HL_COUNT; i++)
			editor.SetHlStyle(i, hlt.hlstyle.дай(i, 1), hlt.hlstyle.дай(i, 2),
			                     hlt.hlstyle.дай(i, 3), hlt.hlstyle.дай(i, 4));
		UpdateFormat();
		
		web_search.сохрани();
		
		if(c == IDEXIT)
			break;
		if(c == 222)
			hlstyle_is_default = false;
		if(c == 333 && PromptYesNo("Восстановить цвета подсветки по умолчанию?")) {
			editor.DefaultHlStyles();
			SetupEditor();
			ReadHlStyles(hlt.hlstyle);
			hlstyle_is_default = true;
		}
		if(c == 334 && PromptYesNo("Установить белую тему?")) {
			editor.WhiteTheme();
			SetupEditor();
			ReadHlStyles(hlt.hlstyle);
			hlstyle_is_default = false;
		}
		if(c == 335 && PromptYesNo("Установить тёмную тему?")) {
			editor.тёмнаяТема();
			SetupEditor();
			ReadHlStyles(hlt.hlstyle);
			hlstyle_is_default = false;
		}
	}
	устВремяф(конфигФайл("version"), воВремя(~ide.showtimeafter));
	FinishConfig();
	SaveConfig();
}

void Иср::FinishConfig()
{
	if(filelist.курсор_ли()) {
		FlushFile();
		FileCursor();
	}
	SaveLoadPackage();
}

bool Иср::IsPersistentFindReplace()
{
	return persistent_find_replace;
}
