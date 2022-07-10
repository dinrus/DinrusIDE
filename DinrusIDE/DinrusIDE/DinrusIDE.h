#ifndef IDE_H
#define IDE_H

#include <DinrusIDE/Common/Common.h>

#include <RichEdit/RichEdit.h>

#include <Report/Report.h>

#include <DinrusIDE/Browser/Browser.h>
#include <TabBar/TabBar.h>
#include <CodeEditor/CodeEditor.h>
#include <DinrusIDE/IconDes/IconDes.h>
#include <DinrusIDE/Java/Java.h>
#include <DinrusIDE/LayDes/LayDes.h>
#include <DinrusIDE/Debuggers/Debuggers.h>
#include <TextDiffCtrl/TextDiffCtrl.h>
#include <DinrusIDE/Designers/Designers.h>
#include <DinrusIDE/Android/Android.h>
#include <plugin/md/Markdown.h>

#include "About.h"
#include "MethodsCtrls.h"

#define LAYOUTFILE <DinrusIDE/DinrusIDE.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS IdeImg
#define IMAGEFILE  <DinrusIDE/DinrusIDE.iml>
#include <Draw/iml_header.h>

#define KEYGROUPNAME "Иср"
#define KEYNAMESPACE IdeKeys
#define KEYFILE      <DinrusIDE/DinrusIDE.key>
#include             <CtrlLib/key_header.h>

#include "version.h"

#include <plugin/astyle/astyle.h>

#include <DinrusIDE/Builders/Builders.h>

const char *найдиТэг(const char *txt, const char *tag);
const char *найдиПосле(const char *txt, const char *tag);
int         исрЛоцируйСтроку(Ткст old_file, int old_line, Ткст new_file);

#include "UppDlg.h"

bool текстфл_ли(const Ткст& file, int maxline = INT_MAX);

void поместиТ(const char *s);

Дисплей& BoldDisplay();

class Консоль : public РедакторКода {
public:
	virtual bool Ключ(dword ключ, int count);
	virtual void леваяДКлик(Точка p, dword);
	virtual void праваяВнизу(Точка p, dword);
	virtual void приставь(const Ткст& s);

protected:
	struct Слот {
		Слот() : outfile(NULL), quiet(true), exitcode(Null) { serial = INT_MAX; }

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
	МассивМап<Ткст, Группа> groups;
	Массив<Финишер> finisher;
	Вектор<Ткст> error_keys;
	Ткст current_group;
	Ткст spooled_output;
	int console_lock;
	bool wrap_text;
	ФреймНиз<EditString> input;
	Ткст line;
	int    serial;

	void CheckEndGroup();
	void слейКонсоль();

public:
	Событие<>  WhenSelect;
	Событие<Бар&> WhenBar;
	Событие<const Ткст&> WhenLine;
	Событие<>         WhenRunEnd;
	bool console;
	bool verbosebuild;

	int  выполни(const char *cmdline, Поток *out = NULL, const char *envptr = NULL, bool quiet = false, bool noconvert = false);
	int  выполни(Один<ПроцессИнк> pick_ process, const char *cmdline, Поток *out = NULL, bool quiet = false);
	int  дайСчётСлотов() const { return processes.дайСчёт(); }
	int  разместиСлот();
	bool пуск(const char *cmdline, Поток *out = NULL, const char *endptr = NULL, bool quiet = false, int slot = 0, Ткст ключ = Null, int blitz_count = 1);
	bool пуск(Один<ПроцессИнк> pick_ process, const char *cmdline, Поток *out = NULL, bool quiet = false, int slot = 0, Ткст ключ = Null, int blitz_count = 1);
	void BeginGroup(Ткст группа);
	void EndGroup();

	Консоль& operator<<(const Ткст& s)      { приставь(s); return *this; }

	void вОшибки(const Ткст& s);
	void AppendOutput(const Ткст& s);
	bool пущен();
	bool пущен(int slot);
	int  слей();
	void глуши(int slot);
	void глуши();
	void сотриОш()                         { error_keys.очисть(); line.очисть(); WhenRunEnd(); }
	Вектор<Ткст> PickErrors()               { Вектор<Ткст> e = pick(error_keys); error_keys.очисть(); return pick(e); }
	void жди(int slot);
	bool жди();
	
	void OnFinish(Событие<>  cb);

	void WrapText(bool w)                     { wrap_text = w; }

	void устСлоты(int s);

	void Input(bool b);

	Консоль();
};

ВекторМап<Ткст, Построитель *(*)()>& BuilderMap();

Ткст SelectAnyPackage();

Ткст FindInDirs(const Вектор<Ткст>& dir, const Ткст& file);
Ткст FindCommand(const Вектор<Ткст>& exedir, const Ткст& cmdline);

void DlCharset(СписокБроса& d);
void DlCharsetD(СписокБроса& d);

void                      SetupBuildMethods();
ВекторМап<Ткст, Ткст> GetMethodVars(const Ткст& method);
Ткст                    GetDefaultMethod();

bool CheckLicense();
bool Install(bool& hasvars);
void AutoInstantSetup();

#define HELPNAME    "Тематики Справки"
#define METAPACKAGE "<meta>"

bool IsHelpName(const char *path);

class TopicCtrl : public HelpWindow {
public:
	virtual Topic AcquireTopic(const Ткст& topic);
	virtual void  BarEx(Бар& bar);
	virtual bool  Ключ(dword ключ, int count);
	virtual void  FinishText(RichText& text);

private:
	EditString search;
	СписокБроса   lang;
	bool       internal;
	bool       showwords, all;
	
	ВекторМап<Ткст, ВекторМап<Ткст, Индекс<Ткст> > > map;
	Индекс<Ткст> lang_list;
	Вектор<int>   spos;
	
	static  Индекс<Ткст> idelink;

	void OpenTopic();
	void ищи();
	void ShowWords();
	void All();
	void Lang();
	void SShow();
	void ScanDirForTpp(const char *dir, const Ткст& rel, Индекс<Ткст>& donepackage,
	                   Индекс<Ткст>& lang_list);
	void LoadMap();
	void FocusSearch();
	void Prev();
	void следщ();

public:
	Событие<>  WhenTopic;

	void SyncDocTree();
	void SearchWord(const Ткст& s);
	void сериализуй(Поток& s);

	typedef TopicCtrl ИМЯ_КЛАССА;

	TopicCtrl();
};

struct IdeCalc : РедакторКода {
	virtual bool Ключ(dword ключ, int count);
	virtual void леваяДКлик(Точка p, dword flags);

	МассивМап<Ткст, EscValue> vars;

	void    выполни();

	IdeCalc();
};

extern bool splash_screen;
void HideSplash();
void ShowSplash();
bool IsSplashOpen();

class RightTabs : public ФреймКтрл<Ктрл> {
public:
	virtual void рисуй(Draw& w);
	virtual void леваяВнизу(Точка p, dword);
	virtual void двигМыши(Точка p, dword);
	virtual void выходМыши();
	virtual void режимОтмены();
	virtual void рисуйФрейм(Draw& w, const Прям& r);
	virtual void выложиФрейм(Прям& r);
	virtual void добавьРазмФрейма(Размер& sz);

private:
	enum       { SPACE = 7 };

	struct Вкладка {
		int    y;
		Рисунок  img;
		Ткст tip;

		int    дайВысоту() const { return img.дайРазм().cy + SPACE; }
		int    GetRight() const  { return y + дайВысоту(); }
	};

	Массив<Вкладка> tab;
	int        cx;
	int        hl;
	int        cursor;

	void Repos();
	void рисуйТаб(Draw& w, int x, int y, int cx, int cy, Цвет paper, const Рисунок& img, Цвет hl);
	int  дайПоз(Точка p);

public:
	void очисть();
	void добавь(const Рисунок& img, const Ткст& tip);
	void устКурсор(int i);
	int  дайКурсор() const                                       { return cursor; }

	RightTabs();
};

struct EditorTabBar : public FileTabs {
	EditorTabBar();

	Ткст GetFile(int n) const;
	bool   FindSetFile(const Ткст& фн);
	void   SetAddFile(const Ткст& фн);
	void   RenameFile(const Ткст& фн, const Ткст& nn);
	void   SetSplitColor(const Ткст& фн, const Цвет& c);
	void   ClearSplitColor();
	void   FixIcons();

	typedef EditorTabBar ИМЯ_КЛАССА;
};

int    memcmp_i(const char *s, const char *t, int n);

Ткст ResolveTParam(const Ткст& тип, const Вектор<Ткст>& tparam);
void   ResolveTParam(Вектор<Ткст>& тип, const Вектор<Ткст>& tparam);
Ткст Qualify(const Ткст& scope, const Ткст& тип, const Ткст& usings);

int    CharFilterMacro(int c);
int    CharFilterFileName(int c);

bool   isincludefnchar(int c);

Размер   GetDrawFileNameSize(const Ткст& h);
void   DrawFileName(Draw& w, const Прям& r, const Ткст& h, Цвет ink);

void   AddPath(EditString *es);
void   InsertPath(EditString *es);

void   DlSpellerLangs(СписокБроса& dl);

#include "Assist.h"

void выборФайла(Ктрл& t, Кнопка& b, Событие<> ev, const char *types, bool saveas);
void выборФайлаСохраниКак(Ктрл& t, Кнопка& b, Событие<> ev, const char *types = NULL);
void выборФайлаОткрой(Ктрл& t, Кнопка& b, Событие<> ev, const char *types = NULL);
void выборФайлаСохраниКак(Ктрл& t, Кнопка& b, const char *types = NULL);
void выборФайлаОткрой(Ктрл& t, Кнопка& b, const char *types = NULL);
void выборДир(Ктрл& t, Кнопка& b);

void выбДир(EditField& f, ФреймПраво<Кнопка>& b);
bool копируйПапку(const char *dst, const char *src, Progress *pi);
void RepoSyncDirs(const Вектор<Ткст>& working);

struct FindInFilesDlg : WithFindInFilesLayout<ТопОкно> {
	ШТкст itext;

	virtual bool Ключ(dword ключ, int count);
	
	void настрой(bool replace);
	void синх();

	typedef FindInFilesDlg ИМЯ_КЛАССА;
	
	FindInFilesDlg();
};

struct WebSearchTab : WithSetupWebSearchTabLayout<КтрлРодитель> {
	void грузи();
	void сохрани();
	bool EditDlg(Ткст& имя, Ткст& uri, Ткст& zico);
	void добавь();
	void синх();
	void Edit();
	void дефолт();
	
	WebSearchTab();
};

void SearchEnginesDefaultSetup();
Ткст SearchEnginesFile();

struct Иср : public ТопОкно, public WorkspaceWork, public КонтекстИср, public MakeBuild {
public:
	virtual   void   рисуй(Draw& w);
	virtual   bool   Ключ(dword ключ, int count);
	virtual   bool   горячаяКлав(dword ключ);
	virtual   void   FileCursor();
	virtual   void   PackageCursor();
	virtual   void   SyncWorkspace();
	virtual   void   BuildFileMenu(Бар& menu);
	virtual   void   ProjectRepo(Бар& bar);
	virtual   void   FilePropertiesMenu0(Бар& menu);
	virtual   void   FilePropertiesMenu(Бар& menu);
	virtual   Ткст GetOutputDir();
	virtual   Ткст GetConfigDir();
	virtual   void   FileSelected();
	virtual   void   сериализуй(Поток& s);
	virtual   void   переименуйФайл(const Ткст& nm);
	virtual   bool   удалиФайл();
	virtual   void   тягИБрос(Точка p, PasteClip& d);
	virtual   void   дезактивируйПо(Ктрл *new_focus);
	virtual   void   активируй();
	virtual   void   Выкладка();

	virtual   bool   вербозно_ли() const;
	virtual   void   вКонсоль(const char *s);
	virtual   void   PutVerbose(const char *s);
	virtual   void   PutLinking();
	virtual   void   PutLinkingEnd(bool ok);

	virtual   const РОбласть& роблИср() const;
	virtual   bool             строитсяИср() const;
	virtual   Ткст           исрДайОдинФайл() const;
	virtual   int              выполниВКонсИср(const char *cmdline, Поток *out = NULL, const char *envptr = NULL, bool quiet = false, bool noconvert = false);
	virtual   int              выполниВКонсИср(Один<ПроцессИнк> pick_ process, const char *cmdline, Поток *out = NULL, bool quiet = false);
	virtual   int              выполниВКонсИсрСВводом(const char *cmdline, Поток *out, const char *envptr, bool quiet, bool noconvert);
	virtual   int              разместиСлотКонсИср();
	virtual   bool             пускКонсИср(const char *cmdline, Поток *out = NULL, const char *envptr = NULL, bool quiet = false, int slot = 0, Ткст ключ = Null, int blitz_count = 1);
	virtual   bool             пускКонсИср(Один<ПроцессИнк> pick_ process, const char *cmdline, Поток *out = NULL, bool quiet = false, int slot = 0, Ткст ключ = Null, int blitz_count = 1);
	virtual   void             слейКонсИср();
	virtual   void             начниГруппуКонсИср(Ткст группа);
	virtual   void             завершиГруппуКонсИср();
	virtual   bool             ждиКонсИср();
	virtual   bool             ждиКонсИср(int slot);
	virtual   void             приФинишеКонсИср(Событие<>  cb);

	virtual   bool      исрОтладка_ли() const;
	virtual   void      исрЗавершиОтладку();
	virtual   void      исрУстНиз(Ктрл& ctrl);
	virtual   void      исрАктивируйНиз();
	virtual   void      исрУдалиНиз(Ктрл& ctrl);
	virtual   void      исрУстПраво(Ктрл& ctrl);
	virtual   void      исрУдалиПраво(Ктрл& ctrl);

	virtual   Ткст    исрДайИмяф() const;
	virtual   int       исрДайСтрокуф();
	virtual   Ткст    исрДайСтроку(int i) const;

	virtual   void      исрУстПозОтладки(const Ткст& фн, int line, const Рисунок& img, int i);
	virtual   void      исрСкройУк();
	virtual   bool      исрОтладБлокируй();
	virtual   bool      исрОтладРазблокируй();
	virtual   bool      исрОтладБлокировка_ли() const;

	virtual   void      исрУстБар();
	virtual   void      IdeGotoCodeRef(Ткст coderef);
	virtual   void      IdeOpenTopicFile(const Ткст& file);
	virtual   void      исрСлейФайл();

	virtual   Ткст    исрДайИмяф();
	virtual   Ткст    IdeGetNestFolder();

	virtual   Ткст    IdeGetIncludePath();

	virtual   bool      IsPersistentFindReplace();

	virtual   int       IdeGetHydraThreads();
	virtual   Ткст    IdeGetCurrentBuildMethod();
	virtual   Ткст    IdeGetCurrentMainPackage();
	virtual   void      IdePutErrorLine(const Ткст& e);
	virtual   void      IdeGotoFileAndId(const Ткст& path, const Ткст& id);

	virtual void   ConsoleShow();
	virtual void   ConsoleSync();
	virtual void   ConsoleClear();
	virtual void   SetupDefaultMethod();
	virtual Вектор<Ткст> PickErrors();
	virtual void   BeginBuilding(bool clear_console);
	virtual void   EndBuilding(bool ok);
	virtual void   ClearErrorEditor();
	virtual void   DoProcessСобытиеs();
	virtual void   ReQualifyCodeBase();
	virtual void   SetErrorEditor();
	virtual Ткст GetMain();

	enum {
		EDITING, BUILDING, RUNNING, DEBUGGING,
	};

	Ткст     pocfg;

	БарМеню    menubar;
	ToolBar    toolbar;
	СтатусБар  statusbar;
	СтатичПрям editpane;

	int        idestate;
	int        debuglock;
	int        hydra1_threads;
	
	int        chstyle;

	Один<IdeDesigner> designer;
	AssistEditor     editor;
	ФайлВвод           view_file;
	AssistEditor     editor2; // no edits happen in editor2, just view
	ФайлВвод           view_file2;
	EditorTabBar     tabs;
	EscValue         macro_api;

	RightTabs   btabs;
	СтатичПрям  bottom;
	Сплиттер    editor_bottom;
	Консоль     console;
	
	КтрлМассив   ffound[3];
	int         ffoundi_next = 0;

	КтрлМассив   Ошибка;
	int         error_count;
	int         warning_count;
	bool        addnotes;
	МассивЗнач  prenotes;
	bool        linking;
	Вектор<Ткст> linking_line;
	bool        removing_notes;

	IdeCalc     calc;
	Ук<Ктрл>   bottomctrl;

	enum Bottoms { BCLOSE, BCONSOLE, BERRORS, BFINDINFILES1, BFINDINFILES2, BFINDINFILES3, BCALC, BDEBUG };

	ФайлВывод    stdout_fout;

	Сплиттер      editorsplit;
	Сплиттер      pfsplit;
	СтатичПрям    wepane;
	SplitterFrame weframe;

	Сплиттер   right_split;
	СтатичПрям right;

	Ткст    editfile;
	ФВремя  edittime;
	int       editfile_line_endings;
	int       editfile_repo;
	bool      editfile_isfolder;
	Ткст    editfile_includes;

	Ткст    editfile2;

	Вектор<Ткст> tablru;
	int            tabi;
	bool           blocktabs;

	struct Bookmark : Движимое<Bookmark> {
		Ткст            file;
		СтрокРедакт::ПозРедакт pos;

		void сериализуй(Поток& s)        { s % file; s % pos; }
	}
	bookmark[10];

	struct FileData {
		Время               filetime;
		СтрокРедакт::ПозРедакт  editpos;
		Точка              columnline;
		СтрокРедакт::ОтмениДанные undodata;
		int64              filehash; // make sure undodata work
		LineInfo           lineinfo;
		LineInfoRem        lineinforem;

		void очисть()  {
			filetime = Null; editpos.очисть(); undodata.очисть();
			columnline = Null; lineinfo.очисть(); lineinforem.очисть();
		}
		void ClearP() {
			lineinforem.очисть();
			undodata.очисть();
		}
	};

	Ткст posfile[2];
	int    posline[2];
	Рисунок  posimg[2];

	Вектор<Bookmark> history;
	int              histi;

	МассивМап<Ткст, FileData> filedata;
	Индекс<Ткст> editastext;
	Индекс<Ткст> editashex;
	
	Вектор<Ткст> difflru;

	СписокБроса   mainconfiglist;
	Ткст     mainconfigname;

	int          build_time;

	MultiButton               buildmode;
	PopUpTable                methodlist;
	PopUpTable                modelist;
	ВекторМап<Ткст, Ткст> recent_buildmode;

	Вектор<Ткст> recentoutput;
	Вектор<Ткст> recentflags;

	Ткст    runarg;
	Ткст    recent_runarg;
	Ткст    rundir;
	enum      { RUN_WINDOW, RUN_CONSOLE, RUN_FILE, RUN_FILE_CONSOLE };
	int       runmode;
	bool      runexternal;
	bool      console_utf8;
	int       consolemode;
	bool      disable_uhd = false;
	bool      darkmode = false;
	bool      minimize = false;
	Ткст    stdout_file;
	Ткст    recent_stdout_file;

// ------------------------------------
	enum LineEndingEnum { LF, CRLF, DETECT_LF, DETECT_CRLF };

	Один<Отладчик> debugger;

	Время      start_time;
	Время      build_start_time;
	int       stat_build_time;

	Шрифт      editorfont;
	Шрифт      veditorfont;
	Шрифт      consolefont;
	Шрифт      font1;
	Шрифт      font2;
	Шрифт      tfont;
	int       editortabsize;
	int       indent_amount;
	byte      default_charset;
	bool      indent_spaces;
	bool      show_status_bar;
	bool      toolbar_in_row;
	bool      show_tabs;
	bool      show_spaces;
	bool      warnwhitespace;
	int       line_endings;
	bool      tabs_icons;
	int       tabs_crosses;
	bool      tabs_grouping;
	bool      tabs_stacking;
	bool      tabs_serialize;
	bool      no_parenthesis_indent;
	bool      hilite_if_endif;
	bool      thousands_separator;
	bool      hline, vline;
	bool      wrap_console_text;
	bool      mute_sounds;
	bool      line_numbers;
	bool      header_guards;
	int       filetabs;
	bool      auto_enclose;
	bool      mark_lines;
	bool      find_pick_sel;
	bool      find_pick_text;
	bool      deactivate_save;
	int       insert_include;
	int       bordercolumn;
	Цвет     bordercolor;
	bool      persistent_find_replace;
	bool      find_replace_restore_pos;
	bool      auto_rescan;
	bool      auto_check;
	int       spellcheck_comments;
	bool      wordwrap_comments = true;
	bool      wordwrap = false;
	bool      setmain_newide;
	bool      gui_font_override = false;
	Шрифт      gui_font = StdFont();
	/*
		astyle code formatter control vars
		added 2008.01.27 by Massimo Del Fedele
	*/
	bool	astyle_BracketIndent;
	bool	astyle_NamespaceIndent;
	bool	astyle_BlockIndent;
	bool	astyle_CaseIndent;
	bool	astyle_ClassIndent;
	bool	astyle_LabelIndent;
	bool	astyle_SwitchIndent;
	bool	astyle_PreprocessorIndent;
	int		astyle_MinInStatementIndentLength;
	int		astyle_MaxInStatementIndentLength;
	bool	astyle_BreakClosingHeaderBracketsMode;
	bool	astyle_BreakElseIfsMode;
	bool	astyle_BreakOneLineBlocksMode;
	bool	astyle_SingleStatementsMode;
	bool	astyle_BreakBlocksMode;
	bool	astyle_BreakClosingHeaderBlocksMode;
	int		astyle_BracketFormatMode;
	int		astyle_ParensPaddingMode;
	bool	astyle_ParensUnPaddingMode;
	bool	astyle_OperatorPaddingMode;
	bool	astyle_EmptyLineFill;
	bool	astyle_TabSpaceConversionMode;
	ШТкст	astyle_TestBox;
	
	// Formats a string of code with a given formatter
	ШТкст FormatCodeString(ШТкст const &Src, astyle::ASFormatter &Formatter);

	// Formats editor's code with Иср формат parameters
	void FormatCode();
	void FormatJSON_XML(bool xml);
	void FormatJSON();
	void FormatXML();
	void FormatJSON_XML_File(bool xml);

	bool      browser_closeesc;
	bool      bookmark_pos;

	ФреймВерх<StaticBarArea> bararea;
	Надпись                   дисплей;


	byte      hilite_scope;
	int       hilite_bracket;
	int       hilite_ifdef;
	bool      barline;
	bool      qtfsel;

	enum { STYLE_NO_REPLACE, STYLE_CONFIRM_REPLACE, STYLE_AUTO_REPLACE };

	Ткст find_file_search_string;
	bool   find_file_search_in_current_package;

	FindInFilesDlg ff;
	Ткст         iwc;

	int           doc_serial;
	TopicCtrl     doc;
	TopicCtrl     windoc;

	int           state_icon;
	
	Ткст        export_dir;
	ВекторМап<Ткст, Ткст> abbr;
	
	int           issaving;
	int           isscanning;
	
	Ткст        current_builder;
	
	bool          hlstyle_is_default = true; // default style reacts to dark / light theme настройки
	
// ------------------------------------

	Время      config_time;
	Время      ConfigTime();
	void      LoadConfig();
	void      SaveConfig();
	void      FinishConfig();
	void      SaveConfigOnTime();

	int       GetPackageIndex();
	void      AddPackage(const Ткст& p);

	void      BeepMuteExclamation() { if(!mute_sounds) бипВосклицание(); }
	void      BeepMuteInformation() { if(!mute_sounds) бипИнформация(); }

	void      BookKey(int ключ);
	void      AddHistory();
	int       GetHistory(int d);
	void      History(int dir);
	void      EditorEdit();
	void      GotoBookmark(const Bookmark& b);
	bool      IsHistDiff(int i);
	
	void      IdePaste(Ткст& s);

	bool      HasFileData(const Ткст& file);
	FileData& Filedata(const Ткст& file);

	void      BTabs();
	void      SyncBottom();
	void      HideBottom();
	void      устНиз(int i);
	void      ToggleBottom(int i);
	void      ShowBottom(int i);
	void      ShowConsole()                      { ShowBottom(1); }
	void      ToggleConsole()                    { ToggleBottom(1); }
	void      SwapBottom();
	bool      IsBottomShown() const;
	bool      BottomIsFindInFiles() const        { return findarg(btabs.дайКурсор(), BFINDINFILES1, BFINDINFILES2, BFINDINFILES3) >= 0; }

	void      EditorFontScroll(int d);

	void      MakeTitle();
	void      MakeIcon();
	void      AdjustMainConfig();
	Ткст    GetFirstFile();
	void      SetMain(const Ткст& package);
	void      ChangeFileCharset(const Ткст& имя, Пакет& p, byte charset);
	void      ChangeCharset();
	void      FlushFile();
	void      EditFile0(const Ткст& path, byte charset, int spellcheck_comments,
	                    const Ткст& headername = Null);
	void      EditFile(const Ткст& path);
	void      AddEditFile(const Ткст& path);
	void      ReloadFile();
	void      AddLru();
	bool      CanToggleReadOnly();
	void      ToggleReadOnly();
	void      PosSync();
	Ткст    IncludesMD5();

	bool      EditFileAssistSync2();
	void      EditFileAssistSync();
	
	ОбрвызВремени     text_updated, trigger_assist;
	std::atomic<int> file_scan;
	bool             file_scanned = false;

	void      TriggerAssistSync();

	void      AKEditor();
	
	void      PackageMenu(Бар& menu);

	void      UscFile(const Ткст& file);
	void      UscProcessDir(const Ткст& dir);
	void      UscProcessDirDeep(const Ткст& dir);
	void      SyncUsc();
	void      CodeBaseSync();

	void      RefreshBrowser();

	void      SetupOutputMode();
	void      SyncBuildMode();
	void      SetupBuildMethods();
	void      DropMethodList();
	void      SetMethod(const Ткст& m);
	void      SelectMethod();
	void      DropModeList();
	void      SelectMode();
	void      SerializeOutputMode(Поток& s);

	void      GotoPos(Ткст path, int line);
	void      GotoCpp(const CppItem& pos);
	
	void      LoadAbbr();
	void      SaveAbbr();

	void      Файл(Бар& menu);
		void   EditWorkspace();
		void   EditAnyFile();
		bool   IsProjectFile(const Ткст& f) const;
		void   SaveEditorFile(Поток& out);
		int64  EditorHash();
		void   SaveFile0(bool always);
		void   сохраниФайл(bool always = false);
		void   DoSaveFile()    { сохраниФайл(); }
		void   FileBookmark	(Бар& menu);
		void   SwapPackagesFiles();
		void   Times();
		void   Statistics();
		void   Licenses();
		void   выход();
		Ткст GetOpposite();
		void   GoOpposite();
		void   Print();
		void   Diff();
		void   DiffWith(const Ткст& path);
		void   DiffFiles(const char *lname, const Ткст& l, const char *rname, const Ткст& r);
		Ткст LoadConflictFile(const Ткст& n);
		void   GotoDiffLeft(int line, ДиффДлг *df);
		void   GotoDiffRight(int line, FileDiff *df);

	void      Edit(Бар& menu);
		bool  IsDesignerFile(const Ткст& path);
		void  DoEditAsText(const Ткст& path);
		void  EditAsText();
		void  EditAsHex();
		void  EditUsingDesigner();
		void  EditMakeTabs()     { editor.MakeTabsOrSpaces(true); }
		void  EditMakeSpaces()   { editor.MakeTabsOrSpaces(false); }
		void  EditMakeLineEnds() { editor.MakeLineEnds(); }

		void  FindInFiles(bool replace);
		void  FindFileName();
		void  найдиТкст(bool back);
		void  ClearEditedFile();
		void  ClearEditedAll();
		void  FindFileAll(const Вектор<Кортеж<int64, int>>& f);
		void  InsertColor();
		void  InsertLay(const Ткст& фн);
		void  InsertIml(const Пакет& pkg, const Ткст& фн, Ткст classname);
		void  InsertText(const Ткст& text);
		void  InsertAs(const Ткст& data);
		void  InsertAs();
		void  InsertFilePath(bool c);
		void  InsertFileBase64();
		void  InsertMenu(Бар& bar);
		void  InsertInclude(Бар& bar);
		void  InsertAdvanced(Бар& bar);
		void  AssistEdit(Бар& menu);
		void  EditorMenu(Бар& bar);
		void  ToggleWordwrap();

        void  CopyPosition();
        void  GotoPosition();

	void OnlineSearchMenu(Бар& menu);

	void SearchMenu(Бар& bar);
		void  EditFind()                { editor.FindReplace(find_pick_sel, find_pick_text, false); }
		void  EditReplace()             { editor.FindReplace(find_pick_sel, find_pick_text, true); }
		void  EditFindReplacePickText() { editor.FindReplace(true, true, false); }
		void  EditFindNext()            { editor.найдиСледщ(); }
		void  EditFindPrevious()        { editor.найдиПредш(); }
		void  EditPaste()               { editor.Paste(); }
		bool  следщ(КтрлМассив& ctrl, int d);
		void  FindNextError();
		void  FindPrevError();
	
	void      EditSpecial(Бар& menu);
		void  TranslateString();
		void  поменяйСимы()               { editor.поменяйСимы(); }
		void  копируйСлово()                { editor.копируйСлово(); }
		void  Duplicate();
		void  AlterText(ШТкст (*op)(const ШТкст& in));
		void  TextToUpper();
		void  TextToLower();
		void  TextToAscii();
		void  TextInitCaps();
		void  SwapCase();
		void  ToCString();
		void  ToComment();
		void  CommentLines();
		void  UnComment();
		void  перефмтКоммент();

	void      MacroMenu(Бар& menu);
		bool  HasMacros();
		void  EditMacroMenu(Бар& menu, МассивЗнач items);
		void  EditMacro(int i);

	void      Project(Бар& menu);
		void  SyncT(int kind);
		void  ConvertST();
		void  ExportMakefile(const Ткст& ep);
		void  ExportProject(const Ткст& ep, bool all, bool gui, bool deletedir = true);
		void  SyncRepoDirs(const Вектор<Ткст>& working);
		void  SyncRepoDir(const Ткст& working);
		void  SyncRepo();

	void      BuildMenu(Бар& menu);
		void BuildPackageMenu(Бар& menu);

		void  DoBuild();
		void  PackageBuild();
		void  StopBuild();
		void  PackageClean();
		void  CreateMakefile();
		void  CleanUppOut();
		void  SwitchHeader();
		void  FileCompile();
		void  Preprocess(bool asmout);
		void  ToggleStopOnErrors();
		void  CreateHostRunDir(Хост& h);
		void  PreprocessInternal();

	void      DebugMenu(Бар& menu);
		void  RunArgs();
		bool  ShouldHaveConsole();
		void  BuildAndExecute();
		void  ExecuteBinary();
		void  ExecuteApk();
		void  BuildAndDebug0(const Ткст& srcfile);
		void  BuildAndDebug(bool runto);
		void  BuildAndExtDebug();
		void  BuildAndExtDebugFile();
		bool  IsValgrind();
		void  Valgrind();

		void  StartDebug();
		void  StopDebug();
		void  DebugToggleBreak();
		void  ConditionalBreak();
		void  DebugClearBreakpoints();
		void  OnBreakpoint(int i);

		bool  EditorTip(РедакторКода::MouseTip& mt);

	void      настрой(Бар& menu);
		void  SetupFormat();
		void  DoEditKeys();
		void  ToggleVerboseBuild();
		void  AutoSetup();
		void  CheckUpdates(bool verbose);
		void  CheckUpdatesManual();
		void  SetUpdateTimer(int period);
		void  Abbreviations();
		void  DoMacroManager();
		void  UpgradeTheIDE();
		void  InstallDesktop();
	
	void      SetupMobilePlatforms(Бар& bar);
		void  SetupAndroidMobilePlatform(Бар& bar, const AndroidSDK& androidSDK);
		void  LaunchAndroidSDKManager(const AndroidSDK& androidSDK);
		void  LaunchAndroidAVDManager(const AndroidSDK& androidSDK);
		void  LauchAndroidDeviceMonitor(const AndroidSDK& androidSDK);
	
	void      BrowseMenu(Бар& menu);
		void  CheckCodeBase();
		void  RescanCode();
		void  QueryId();
		void  OpenTopic(const Ткст& topic, const Ткст& createafter, bool before);
		void  OpenTopic(const Ткст& topic);
		void  OpenATopic();
		void  ToggleNavigator();
		void  SearchCode();
		void  идиК();
		void  NavigatorDlg();
		void  ScanFile(bool check_includes);
		bool  SwapSIf(const char *cref);
		void  SwapS();
		void  найдиИд(const Ткст& id);
		bool  OpenLink(const Ткст& s, int pos);
		void  ContextGoto0(int pos);
		void  ContextGoto();
		void  GoToLine();
		void  CtrlClick(int64 pos);
		void  Qtf();
		void  Xml();
		void  Json();
		void  GotoDirDiffLeft(int line, DirDiffDlg *df);
		void  GotoDirDiffRight(int line, DirDiffDlg *df);
		void  DoDirDiff();
		void  DoPatchDiff();
		void  AsErrors();
		void  RemoveDs();

	void      HelpMenu(Бар& menu);
	    void  ViewIdeLogFile();
		void  About();

	void      ConsoleMenu(Бар& menu);
		void  ConsoleCopy();
		void  ConsolePaste();

	void      MainMenu(Бар& menu);
	void      MainTool(Бар& bar);

	void      SaveWorkspace();
	Ткст    WorkspaceFile();
	void      SerializeWorkspace(Поток& s);

	void      SerializeLastMain(Поток& s);
	void      SaveLastMain();
	void      LoadLastMain();
	void      EditorMode();
	bool      IsEditorMode() const;

//	void      PrintTime(dword time);
	void      SetIdeState(int newstate);

	void      MainConfig();
	void      SyncMainConfigList();
	void      OnMainConfigList();
	void      SetMainConfigList();

	void      FileProperties();

	void      CustomSteps();

	void      CycleFiles();

	void      Renumber();
	
	Ткст    GetTargetLogPath();
	Ткст    GetIdeLogPath();
	void      OpenLog(const Ткст& logFilePath);
	
	Ткст    include_path; // cached значение of include path, GetIncludePath
	
	virtual void      InvalidateIncludes();

	virtual void      LaunchTerminal(const char *dir);
	
//	Консоль&  GetConsole();

	struct FindLineErrorCache {
		ВекторМап<Ткст, Ткст> file;
		Вектор<Ткст>            wspc_paths;
		Ткст                    upp;
		bool                      is_java;
		bool                      init;
		
		void очисть()            { init = false; file.очисть(); wspc_paths.очисть(); }
	};

	struct ErrorInfo {
		Ткст file;
		int    lineno;
		int    linepos;
		int    len;
		int    kind;
		Ткст message;
		Ткст error_pos;
		
		ErrorInfo() { lineno = linepos = kind = len = 0; }
	};
	
	FindLineErrorCache error_cache;
	void      ConsoleLine(const Ткст& line, bool assist = false);
	void      ConsoleRunEnd();
	void      SyncErrorsMessage();
	Ткст    GetErrorsText(bool all, bool src);
	void      CopyError(bool all);
	void      ErrorMenu(Бар& bar);
	void      ShowError();
	void      SetFFound(int ii);
	КтрлМассив& FFound();
	void      ShowFound();
	void      CopyFound(bool all);
	void      FFoundMenu(Бар& bar);
	void      SelError();
	void      ClearErrorsPane();
	ШТкст   FormatErrorLine(const Ткст& text, int& linecy);
	ШТкст   FormatErrorLineEP(const Ткст& text, const char *ep, int& linecy);
	
	struct FoundDisplay : Дисплей {
		virtual void рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const;
	};

	struct TopAlignedDisplay : Дисплей {
		virtual void рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const;
	};

	struct FoundFileDisplay : Дисплей {
		virtual void рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const;
		virtual Размер дайСтдРазм(const Значение& q) const;
	};
	
	bool      FindLineError(int l);
	void      GoToError(const ErrorInfo& f);
	void      GoToError(КтрлМассив& a);
	
	bool      FindLineError(const Ткст& ln, FindLineErrorCache& cache, ErrorInfo& f);
	void      FindError();
	void	  ClearErrorEditor(Ткст file);

	void      FindWildcard();
	void      найдиПапку();
	void      найдиУстСтдПап(Ткст n);
	void      найдиСтдПап();
	void      InsertWildcard(const char *s);
	void      AddFoundFile(const Ткст& фн, int ln, const Ткст& line, int pos, int count);
	bool      SearchInFile(const Ткст& фн, const Ткст& pattern,
		                   bool wholeword, bool ignorecase, int& n, RegExp *regexp);
	void      SyncFindInFiles();
	void      ConstructFindInFiles();
	void      SerializeFindInFiles(Поток& s);

	void      SetupEditor(int font, Ткст highlight, Ткст фн);
	void      SetupEditor();

	void      DoDisplay();
	void      ManageDisplayVisibility();

	void      устИконку();
	bool      IsCppBaseFile();
	void      CheckFileUpdate();
	void      Periodic();

	void      PassEditor();
	void      SyncEditorSplit();
	void      SplitEditor(bool horz);
	void      CloseSplit();
	void      KeySplit(bool horz);
	void      SwapEditors();
	void      TabFile();
	void      ClearTab();
	void      ClearTabs();
	void      CloseRest(EditorTabBar *tabs);
	void      TabsLR( int jd );
	void      TabsStackLR( int jd );

	void      освежиФрейм(bool auto_disasm);
	void      освежиСтроку(int frame, bool auto_disasm);

	void      SetBar();
	void      SetMenuBar();
	void      SetToolBar();
	ОбрвызВремени delayed_toolbar;
	
	
	void      UpdateFormat(РедакторКода& editor);
	void      UpdateFormat();
	void      ReadHlStyles(КтрлМассив& hlstyle);

	bool      OpenMainPackage();
	void      NewMainPackage();

	bool      GotoDesignerFile(const Ткст& path, const Ткст& scope, const Ткст& имя, int line);
	void      JumpToDefinition(const Массив<CppItem>& n, int q, const Ткст& scope);
	void      GotoFileAndId(const Ткст& path, const Ткст& id);
	void      SearchTopics();
	void      ShowTopics();
	void      ShowTopicsWin();

	Ткст    GetIncludePath();

	void      TopicBack();

	void      SetupBars();

	EscValue  MacroEditor();
		void  MacroGetLength(EscEscape& e);
		void  MacroGetLineCount(EscEscape& e);
		void  MacroGetLinePos(EscEscape& e);
		void  MacroGetLineLength(EscEscape& e);
		void  MacroGetCursor(EscEscape& e);
		void  MacroGetLine(EscEscape& e);
		void  MacroGetColumn(EscEscape& e);
		void  MacroGetSelBegin(EscEscape& e);
		void  MacroGetSelEnd(EscEscape& e);
		void  MacroGetSelCount(EscEscape& e);
		void  MacroSetCursor(EscEscape& e);
		void  MacroSetSelection(EscEscape& e);
		void  MacroClearSelection(EscEscape& e);
		void  MacroGet(EscEscape& e);
		void  MacroRemove(EscEscape& e);
		void  MacroInsert(EscEscape& e);
		void  MacroFind(EscEscape& e);
		void  MacroReplace(EscEscape& e);
		void  MacroнайдиСовпадениеingBrace(EscEscape& e);
		void  MacroFindClosingBrace(EscEscape& e);
		void  MacroFindOpeningBrace(EscEscape& e);
		void  MacroMoveLeft(EscEscape& e);
		void  MacroMoveRight(EscEscape& e);
		void  MacroMoveWordLeft(EscEscape& e);
		void  MacroMoveWordRight(EscEscape& e);
		void  MacroMoveUp(EscEscape& e);
		void  MacroMoveDown(EscEscape& e);
		void  MacroMoveHome(EscEscape& e);
		void  MacroMoveEnd(EscEscape& e);
		void  MacroMovePageUp(EscEscape& e);
		void  MacroMovePageDown(EscEscape& e);
		void  MacroMoveTextBegin(EscEscape& e);
		void  MacroMoveTextEnd(EscEscape& e);
		void  MacroInput(EscEscape& e);
		void  MacroBuild(EscEscape& e);
		void  MacroBuildProject(EscEscape& e);
		void  MacroExecute(EscEscape& e);
		void  MacroLaunch(EscEscape& e);
		void  MacroClearConsole(EscEscape& e);
		void  MacroEditFile(EscEscape& e);
		void  MacroSaveCurrentFile(EscEscape& e);
		void  MacroFileName(EscEscape& e);
		void  MacroMainPackage(EscEscape& e);
		void  MacroActivePackage(EscEscape& e);
		void  MacroPackageDir(EscEscape& e);
		void  MacroAssembly(EscEscape& e);
		void  MacroBuildMethod(EscEscape& e);
		void  MacroBuildMode(EscEscape& e);
		void  MacroFlags(EscEscape& e);
		void  MacroEcho(EscEscape& e);
		void  MacroCloseFile(EscEscape& e);
		void  MacroPackageFiles(EscEscape& e);
		void  MacroAllPackages(EscEscape& e);
		void  MacroTarget(EscEscape& e);
	
	Ткст GetAndroidSdkPath();
	
	typedef   Иср ИМЯ_КЛАССА;

	enum {
		TIMEID_PERIODIC = ТопОкно::TIMEID_COUNT,
		TIMEID_COUNT,
	};

	Иср();
	~Иср();
};

inline void ShowConsole() { if(TheIde()) ((Иср *)TheIde())->ShowConsole(); }

void InstantSetup();

bool SetupGITMaster();

Ткст UppHub();
void   UppHubAuto(const Ткст& s);

#include "urepo.h"

#endif
