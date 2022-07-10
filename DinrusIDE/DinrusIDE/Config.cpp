#include "DinrusIDE.h"

#define METHOD_NAME "Иср::" << UPP_FUNCTION_NAME << "(): "

void Иср::SerializeWorkspace(Поток& s) {
	int i;
	int version = 21;
	s / version;
	s.Magic(0x12354);
	if(s.сохраняется()) {
		for(i = 0; i < filedata.дайСчёт(); i++) {
			Ткст фн = filedata.дайКлюч(i);
			if(!фн.пустой() && файлЕсть(фн)) {
				s % фн;
				s % filedata[i].editpos;
				if(version >= 1)
					s % filedata[i].columnline;
			}
		}
		Ткст h;
		s % h;
	}
	else {
		Ткст фн;
		filedata.очисть();
		for(;;) {
			s % фн;
			if(фн.пустой()) break;
			FileData& fd = filedata.дайДобавь(фн);
			fd.очисть();
			s % fd.editpos;
			if(version >= 1)
				s % fd.columnline;
		}
	}
	Ткст pk = GetActivePackage();
	s % pk;
	package.FindSetCursor(pk);
	s % tablru;
	s % mainconfigname;
	s % mainconfigparam;
	s % console.verbosebuild;
	s % stoponerrors;
	byte dummy = 0;
	s % dummy;
	s % runarg;
	s % recent_runarg;
	s % rundir;
	s % stdout_file % recent_stdout_file / runmode;
	if(version >= 1)
		s % runexternal;
	if(version >= 11) {
		bool dummy = false;
		s % dummy;
	}
	if(version >= 13)
		s % consolemode;
	if(version >= 14)
		s % console_utf8;
	if(version >= 17)
		s % disable_uhd;
	if(version >= 18)
		s % darkmode;
	if(version >= 21)
		s % minimize;
	s % editfile;
	for(i = 0; i < 10; i++) {
		s % bookmark[i];
	}
	editor.сериализуй(s);
	if(version >= 5)
		s % editorsplit;
	if(version == 6) {
		Ткст n;
		int v;
		s / v;
		for(int i = 0; i < 10; i++) {
			s % n;
			s / v;
		}
	}
	if(version >= 8) {
		bool dummyb = false;
		Ткст dummy;
		s % dummyb;
		s % dummy;
	}
	SerializeFindInFiles(s);
	Ткст om;
	s % om;
	s % recentoutput;
	s % recentflags;
	s / editortabsize / indent_amount % indent_spaces;
	if(version < 15)
		for(int j = 0; j < дайСчётМодульИСР(); j++)
			дайМодульИСР(j).сериализуй(s);
	else {
		ВекторМап<Ткст, Ткст> module_cfg;
		for(int j = 0; j < дайСчётМодульИСР(); j++) {
			МодульИСР& m = дайМодульИСР(j);
			module_cfg.добавь(m.GetID(), сохраниКакТкст(m));
		}
		s % module_cfg;
		for(int j = 0; j < дайСчётМодульИСР(); j++) {
			МодульИСР& m = дайМодульИСР(j);
			грузиИзТкст(m, module_cfg.дай(m.GetID(), Ткст()));
		}
	}
	SerializeWorkspaceConfigs(s);
	SerializeOutputMode(s);
	SerializeClosed(s);
	if(version >= 10) {
		bool dotabs = tabs_serialize;
		if(version >= 19)
			s % dotabs;
		if(dotabs)
			s % tabs;
	}
	if(version >= 11) {
		s % find_file_search_string;
	}
	if(version >= 16) {
		s % find_file_search_in_current_package;
	}
	if(version >= 12)
		SerializePlacement(s);
	if(version >= 20)
		s % difflru;
}

void Иср::SerializeLastMain(Поток& s)
{
	s % main;
	Ткст varsname = GetVarsName();
	s % varsname;
	s % pocfg;
	if(s.грузится())
		LoadVars(varsname);
}

void Иср::EditorMode()
{
	Logd() << METHOD_NAME;
	
	editormode = true;
	main = Null;
	pocfg = Null;
	
	// TODO: This is probably bug with CtrlLib tool bar. If
	// we did not call clear here - we will have blocked two icons
	// in editor mode :(. This should be investigated with
	// appropriate test case.
	toolbar.очисть();
	SetBar();
	
	editor.SyncNavigator();
}

bool Иср::IsEditorMode() const
{
	return editormode;
}

void Иср::SaveLastMain()
{
	сохраниВФайл(THISBACK(SerializeLastMain), конфигФайл("lastmain.cfg"));
}

void Иср::LoadLastMain()
{
	bool editor = IsEditorMode();
	грузиИзФайла(THISBACK(SerializeLastMain), конфигФайл("lastmain.cfg"));
	if(editor)
		main = Null;
}

void Sentinel(Поток& s, const char *txt)
{
	Ткст h;
	h << "<123456789:" << txt << ":123456789>";
	Буфер<char> hh(h.дайДлину());
	memcpy(hh, h, h.дайДлину());
	s.SerializeRaw((byte *)~hh, h.дайДлину());
}

void Иср::сериализуй(Поток& s)
{
	int version = 20;
	Sentinel(s, "before 12341234");
	s.Magic(0x12341234);
	Sentinel(s, "after magic");
	s / version;
	Sentinel(s, "before first FileSel");
	s % AnySourceFs();
	Sentinel(s, "after AnySourceFs");
	s % BasedSourceFs();
	Sentinel(s, "after BasedSourceFs");
	s % AnyPackageFs();
	Sentinel(s, "after AnyPackageFs");
	s % pfsplit;
	if (version >= 12)
		s % weframe;
	else {
		Сплиттер dummy;
		s % dummy;
	}
	package.сериализуйНастройки(s);
	filelist.сериализуйНастройки(s);
	s % editorfont;
	s % tfont;
	s % veditorfont;
	s % consolefont;
	s % font1;
	s % font2;
	s % show_status_bar;
	s % toolbar_in_row;
	s % filetabs;
	s % auto_enclose;
	s % persistent_find_replace;
	if(version >= 4) {
		s % find_replace_restore_pos;
	}
	s % show_tabs;
	if(version >= 18)
		s % show_spaces;
	if(version >= 7)
		s % warnwhitespace;
	s % tabs_icons;
	s % tabs_crosses;
	s % tabs_grouping;
	s % tabs_serialize;
	s % tabs_stacking;
	if(version >= 11)
		s % spellcheck_comments;
	if(version >= 13) {
		s % wordwrap_comments;
		s % wordwrap;
	}
	if(version >= 16) {
		s % setmain_newide;
	}
	bool dummy_force_crlf = false;
	s % dummy_force_crlf;
	if(version >= 1)
		s % line_endings;
	s % no_parenthesis_indent;
	s % hilite_scope;
	s % hilite_if_endif;
	s % hilite_bracket;
	s % hilite_ifdef;
	if(version >= 3)
		s % thousands_separator;
	if(version >= 5)
		s % hline;
	if(version >= 15)
		s % vline;
	s % barline;
	s % qtfsel;
	s % wrap_console_text;
	s % mute_sounds;
	s % line_numbers;
	s % bookmark_pos;
	s % use_target;
	s % default_charset;
	s % header_guards;
	s % insert_include;
	сериализуйГлобКонфиги(s);
	doc.сериализуй(s);
	s % right_split;
	s % splash_screen;
	s % editor.auto_assist;
	if(version >= 9)
		s % auto_rescan;
	if(version >= 10)
		s % auto_check;
	s % editor.commentdp;
	s % bordercolumn;
	s % bordercolor;
	if(version >= 20)
		s % find_pick_sel % find_pick_text % deactivate_save;
	s % hydra1_threads;
	if(s.грузится())
		console.устСлоты(hydra1_threads);
	if(version < 14) {
		int dummy_gdb_selector = 0;
		s % dummy_gdb_selector;
	}
	s % doc;
	s % chstyle;
	s % astyle_BracketIndent;
	s % astyle_NamespaceIndent;
	s % astyle_BlockIndent;
	s % astyle_CaseIndent;
	s % astyle_ClassIndent;
	s % astyle_LabelIndent;
	s % astyle_SwitchIndent;
	s % astyle_PreprocessorIndent;
	s % astyle_MinInStatementIndentLength;
	s % astyle_MaxInStatementIndentLength;
	s % astyle_BreakClosingHeaderBracketsMode;
	s % astyle_BreakElseIfsMode;
	s % astyle_BreakOneLineBlocksMode;
	s % astyle_SingleStatementsMode;
	s % astyle_BreakBlocksMode;
	s % astyle_BreakClosingHeaderBlocksMode;
	s % astyle_BracketFormatMode;
	s % astyle_ParensPaddingMode;
	s % astyle_ParensUnPaddingMode;
	s % astyle_OperatorPaddingMode;
	s % astyle_EmptyLineFill;
	s % astyle_TabSpaceConversionMode;
	s % astyle_TestBox;
	if(version >= 6 && version < 8) {
		Ткст dummy;
		s % dummy;
	}
	s % HostConsole;
	editor.SerializeNavigator(s);
	s % showtime;
	s % DiffFs();
	s % sort;
	s % output_per_assembly;
	s.Magic();
	if(version >= 17)
		s % hlstyle_is_default;
	if(version >= 19) {
		s % gui_font % gui_font_override;
	}

#ifdef PLATFORM_WIN32
	if(s.грузится() && HostConsole == "/usr/bin/xterm -e")
		HostConsole = "powershell.exe";
#endif
}

Время Иср::ConfigTime()
{
	return дайВремяф(конфигФайл());
}

void Иср::SaveConfig()
{
	сохраниИзменёнФайл(конфигФайл(), сохраниКакТкст(*this));
	if(дайКлючИни("DebugClipboard") == "1") {
		сохраниИзменёнФайл(конфигФайл() + ".bak", сохраниКакТкст(*this));
		сохраниВФайл(*this, конфигФайл() + ".bak1");
	}
	сохраниИзменёнФайл(конфигФайл("ide.ключ"), сохраниКлючи());
	сохраниИзменёнФайл(конфигФайл("ide.colors"), editor.StoreHlStyles());
	config_time = ConfigTime();
}

void Иср::SaveConfigOnTime()
{
	if(ConfigTime() == config_time)
		SaveConfig();
}

void Иср::LoadConfig()
{
	if(!грузиИзФайла(*this) && дайКлючИни("DebugClipboard") == "1") {
		Exclamation("Неудачная загрузка конфигурации!");
		if(!грузиИзФайла(*this, конфигФайл() + ".bak")) {
			Exclamation("Неудачный бэкап конфигурации .bak!");
			if(!грузиИзФайла(*this, конфигФайл() + ".bak1"))
				Exclamation("Неудачный бэкап конфигурации .bak1!");
		}
	}
	восстановиКлючи(загрузиФайл(конфигФайл("ide.ключ")));
	if(hlstyle_is_default)
		editor.DefaultHlStyles();
	else
		editor.LoadHlStyles(загрузиФайл(конфигФайл("ide.colors")));
	config_time = дайВремяф(конфигФайл());
	UpdateFormat();
	if(filelist.курсор_ли()) {
		FlushFile();
		FileCursor();
	}
	SaveLoadPackage();
}
