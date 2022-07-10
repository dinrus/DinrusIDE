#ifndef _Browser_Browser_h
#define _Browser_Browser_h

#include <CtrlLib/CtrlLib.h>
#include <CppBase/CppBase.h>
#include <DinrusIDE/Common/Common.h>
#include <RichEdit/RichEdit.h>
#include <PdfDraw/PdfDraw.h>

#define LAYOUTFILE <DinrusIDE/Browser/Browser.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS BrowserImg
#define IMAGEFILE <DinrusIDE/Browser/Browser.iml>
#include <Draw/iml_header.h>

ИНИЦИАЛИЗУЙ(CodeBase)

class Browser;

void ReduceCacheFolder(const char *path, int max_total);

void LockCodeBase();
bool TryLockCodeBase();
void UnlockCodeBase();
void UnlockCodeBaseAll();

struct CodeBaseLock { // Use when accessing CodeBase
	CodeBaseLock()       { LockCodeBase(); }
	~CodeBaseLock()      { UnlockCodeBase(); }
};

CppBase&       CodeBase();

struct SourceFileInfo {
	Время                      time;
	Ткст                    dependencies_md5sum; // dependencies from other files - usings, initial namespace, macros
	Ткст                    md5sum; // preprocessing 'fingerprint' to detect changes
	Вектор<int>               depends; // indicies of file this files depends on, for time-check
	Время                      depends_time;
	
	void сериализуй(Поток& s);

	SourceFileInfo() { time = Null; depends_time = Null; }
};

void           NewCodeBase();
void           ParseSrc(Поток& in, int file, Событие<int, const Ткст&> Ошибка);
void           CodeBaseScanFile(Поток& in, const Ткст& фн);
void           CodeBaseScanFile(const Ткст& фн, bool auto_check);
bool           TryCodeBaseScanFile(Поток& in, const Ткст& фн);
void           ClearCodeBase();
// void           CheckCodeBase();
void           RescanCodeBase();
void           SyncCodeBase();
void           SaveCodeBase();
bool           ExistsBrowserItem(const Ткст& элт);
void           FinishCodeBase();

Ткст         PreprocessLayFile(const char *фн);
Вектор<Ткст> PreprocessSchFile(const char *фн);
Ткст         PreprocessImlFile(const char *фн);

int            GetSourceFileIndex(const Ткст& path);
Ткст         GetSourceFilePath(int file);

Ткст         MakeCodeRef(const Ткст& scope, const Ткст& элт);
void           SplitCodeRef(const Ткст& ref, Ткст& scope, Ткст& элт);

const CppItem *GetCodeRefItem(const Ткст& ref, const Ткст& rfile);
const CppItem *GetCodeRefItem(const Ткст& ref);

int            GetMatchLen(const char *s, const char *t);


enum WithBodyEnum { WITHBODY = 33 };

inline Шрифт BrowserFont() { return StdFont(); }

struct CppItemInfo : CppItem {
	Ткст scope;
	bool   over;
	bool   overed;
	int    inherited;
	int    typei;
	
	CppItemInfo() { over = overed = virt = false; inherited = line = 0; }
};

enum {
	ITEM_TEXT,
	ITEM_NAME,
	ITEM_CPP_TYPE,
	ITEM_CPP,
	ITEM_PNAME,
	ITEM_TNAME,
	ITEM_NUMBER,
	ITEM_SIGN,
	ITEM_UPP,
	ITEM_TYPE,
	
	ITEM_PTYPE = ITEM_TYPE + 10000,
};

struct ItemTextPart : Движимое<ItemTextPart> {
	int pos;
	int len;
	int тип;
	int ii;
	int pari;
};

Вектор<ItemTextPart> ParseItemNatural(const Ткст& имя, const Ткст& natural, const Ткст& ptype,
                                      const Ткст& pname, const Ткст& тип, const Ткст& tname,
                                      const Ткст& ctname, const char *s);
Вектор<ItemTextPart> ParseItemNatural(const Ткст& имя, const CppItem& m, const char *natural);
Вектор<ItemTextPart> ParseItemNatural(const CppItemInfo& m);
Вектор<ItemTextPart> ParseItemNatural(const CppItemInfo& m);

int дайВысотуЭлта(const CppItem& m, int cx);

enum AdditionalKinds {
	KIND_INCLUDEFILE = 100,
	KIND_INCLUDEFILE_ANY,
	KIND_INCLUDEFOLDER,
};

void PaintText(Draw& w, int& x, int y, const char *text,
               const Вектор<ItemTextPart>& n, int starti, int count, bool focuscursor,
               Цвет _ink, bool italic);
void PaintCppItemImage(Draw& w, int& x, int ry, int access, int kind, bool focuscursor);

struct CppItemInfoDisplay : public Дисплей
{
	bool   namestart;
	bool   showtopic;

	int DoPaint(Draw& w, const Прям& r, const Значение& q,
		        Цвет _ink, Цвет paper, dword style) const;
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q,
		               Цвет _ink, Цвет paper, dword style) const;
	virtual Размер дайСтдРазм(const Значение& q) const;
	
	CppItemInfoDisplay() { namestart = false; showtopic = false; }
};

int SearchItemFilter(int c);

struct CodeBrowser {
	typedef CodeBrowser ИМЯ_КЛАССА;

	CppItemInfoDisplay     дисплей;
	КтрлМассив              scope;
	КтрлМассив              элт;
	EditString             search_scope;
	EditString             search;
//	ФреймПраво<Кнопка>     clear;
	int                    range;
	ButtonOption           rangebutton[3];
	ButtonOption           sort;
	Событие<>                WhenKeyItem;
	Событие<>                WhenClear;
	
	Ткст             GetPm();
	void               грузи();
	void               LoadScope();
	void               идиК(const Ткст& coderef, const Ткст& rfile);
	void               ищи();
	void               NameStart()               { дисплей.namestart = true; }
	Ткст             GetCodeRef(int i) const;
	Ткст             GetCodeRef() const;
	CppItemInfo        GetItemInfo(int i) const;
	CppItemInfo        GetItemInfo() const;
	bool               Ключ(dword ключ, int count);
	bool               IsSearch() const;
	void               ClearSearch();
	void               устДиапазон(int r);
	void               сортируй();
	
	CodeBrowser();
};

struct TopicInfo : Движимое<TopicInfo> {
	Время           time;
	Ткст         path;
	Ткст         title;
	Вектор<int>    words;

	TopicInfo() {}

	rval_default(TopicInfo);
};

Ткст          GetTopicPath(const TopicLink& link);
Ткст          GetTopicPath(const Ткст& link);

extern bool SyncRefsFinished;
extern bool SyncRefsShowProgress;

void            SyncRefs();
void            SyncTopicFile(const RichText& text, const Ткст& link, const Ткст& path,
                              const Ткст& title);
void            SyncTopicFile(const Ткст& link);
Ткст          GetTopicTitle(const Ткст& link);
void            InvalidateTopicInfoPath(const Ткст& path);

bool LegacyRef(Ткст& ref);

Вектор<Ткст>  GetRefLinks(const Ткст& ref);

int             TopicWordIndex(const Ткст& w);
int             MatchWord(const Вектор<int>& w, const Ткст& pattern);
bool            MatchTopicLink(const Ткст& link, const Вектор<Ткст>& query);

#define LAYOUTFILE <DinrusIDE/Browser/Topic.lay>
#include <CtrlCore/lay.h>

struct ReferenceDlg : WithReferenceDlgLayout<ТопОкно>, CodeBrowser {
	void   EnterItem();
	void   EnterItemOk();
	void   уст(const Ткст& s);
	Ткст дай() const            { return ~reference; }

	void   сериализуй(Поток& s)   { SerializePlacement(s); }

	typedef ReferenceDlg ИМЯ_КЛАССА;

	ReferenceDlg();
};

#define IMAGEFILE <DinrusIDE/Browser/Topic.iml>
#define IMAGECLASS TopicImg
#include <Draw/iml_header.h>

Ткст DecoratedItem(const Ткст& имя, const CppItem& m, const char *natural, int pari = INT_MIN);

int  CharFilterID(int c);

bool ParseTopicFileName(const Ткст& фн, Ткст& topic, int& lang);

Topic  ReadTopic(const char *text);
Вектор<Ткст> GatherLabels(const RichText& text);
Ткст WriteTopic(const char *title, const RichText& text);
Ткст WriteTopicI(const char *title, const RichText& text);

bool LoadTopics(Вектор<Ткст>& topics, const Ткст& dir);
void FillTopicsList(СписокФайлов& list, const Вектор<Ткст>& topics);

TopicLink ParseTopicFilePath(const Ткст& path);
Ткст    TopicFilePath(const TopicLink& tl);

void SaveGroupInc(const Ткст& grouppath);

void SetTopicGroupIncludeable(const char *path, bool set);

Uuid CodeItemUuid();
Uuid StructItemUuid();
Uuid BeginUuid();
Uuid EndUuid();

struct StyleDlg;

template <class T>
struct TopicDlg : T {
	Ткст дайИмя() const {
		return (Ткст)~T::topic + "_" + впроп(LNGAsText(~T::lang)) + ".tpp";
	}

	TopicDlg(const char *title) {
		CtrlLayoutOKCancel(*this, title);
		T::topic.неПусто();
		T::topic.максдлин(30);
		T::topic.устФильтр(CharFilterID);
	}
};

class TopicEditor : public IdeDesigner, public Ктрл {
public:
	virtual Ткст дайИмяф() const;
	virtual void   сохрани();
	virtual void   EditMenu(Бар& menu);
	virtual Ктрл&  DesignerCtrl()                                   { return *this; }
	virtual void   устФокус();
	virtual void   RestoreEditPos()                                 { editor.устФокус(); }


	virtual bool Ключ(dword ключ, int);

protected:
	ToolBar           tool;
	
	Вектор<Ткст>    topics;
	КтрлРодитель        topics_parent;
	СписокФайлов          topics_list;
	EditString        topics_search;

	EditString        title;
	RichEdit          editor;

	СтатичПрям        right;
	Сплиттер          left_right;
	
	Ткст            grouppath;
	Ткст            topicpath;
	Ткст            singlefilepath;

	static Ткст     lasttemplate;
	static int        lastlang;
	static bool       allfonts;
	static int        serial;

	struct ИнфОФайле {
		Время               time;
		RichEdit::UndoInfo undo;
		RichEdit::PosInfo  pos;

		ИнфОФайле() { time = Время(1, 1, 1); }
	};

	static ВекторМап<Ткст, Ткст>  grouptopic;
	static МассивМап<Ткст, ИнфОФайле> editstate;

protected:
	void   FormatMenu(Бар& bar);
	void   TableMenu(Бар& bar);
	void   TopicMenu(Бар& bar);
	void   MainTool(Бар& bar);
	void   FileBar(Бар& bar);
	void   SetBar();

	Ткст GetCurrentTopicPath();

	void   InsertNew(const Ткст& coderef);
	void   NewTopic();
	void   MoveTopic();
	void   RemoveTopic();

	bool         autosave;
	ReferenceDlg ref;

	void   ShowTopic(bool b = true);
	void   HideTopic()                   { ShowTopic(false); }

	void   TopicCursor();

	void   ListTemplates(Вектор<Ткст>& path, Вектор<Ткст>& имя);
	Ткст ChooseTemplate(const char *title);

	void   SaveAsTemplate();
	void   ApplyStylesheet();
	void   ApplyStylesheetGroup();

	void   Hyperlink(Ткст&, ШТкст&);

	void   грузи(const Ткст& path);
	void   SaveTopic();
	void   SaveInc();
	void   слей();

	void   SyncFonts();
	void   AllFonts();

	void   Tools(Бар& bar);
	void   Надпись(Ткст&);
	void   InsertItem();

	void   FindBrokenRef();
	void   JumpToDefinition();
	Ткст GetLang() const;

	void   FixTopic();
	
	void   OnSearch();
	void   DoSearch();
	
public:
	Событие<Бар&> WhenTemplatesMenu;

	enum {
		TIMEID_AUTOSAVE = Ктрл::TIMEID_COUNT,
	    TIMEID_COUNT
	};

	static Размер TopicPage()                          { return Размер(3968, INT_MAX); }

	void   ExportPdf();
	void   ExportGroupPdf();
	void   ExportHTML();
	void   ExportGroupHTML();
	void   Print();

	typedef TopicEditor ИМЯ_КЛАССА;

	void ShowEditor(bool b)                          { editor.покажи(b); }
	bool NewTopicEx(const Ткст& имя, const Ткст& create);
	void открой(const Ткст& grouppath);
	void OpenFile(const Ткст& path);
	void GoTo(const Ткст& topic, const Ткст& link, const Ткст& create, bool before);
	void PersistentFindReplace(bool b)               { editor.PersistentFindReplace(b); }
	
	static int  GetSerial();

	static void SerializeEditPos(Поток& s);
	
	void сериализуй(Поток& s);

	TopicEditor();
	virtual ~TopicEditor();
};

#endif
