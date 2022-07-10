#ifndef IDEEDITOR_H
#define IDEEDITOR_H

//#include <DinrusIDE/Common/Common.h>
#include <CtrlLib/CtrlLib.h>
#include <plugin/pcre/Pcre.h>

namespace РНЦП {

#define  LAYOUTFILE <CodeEditor/CodeEditor.lay>
#include <CtrlCore/lay.h>

#define IMAGEVECTOR Вектор
#define IMAGECLASS  CodeEditorImg
#define IMAGEFILE   <CodeEditor/CodeEditor.iml>
#include <Draw/iml_header.h>


void FindWildcardMenu(Обрвыз1<const char *> cb, Точка p, bool tablf, Ктрл *owner, bool regexp);

struct LineInfoRecord {
	int    lineno;
	Ткст breakpoint;
	int    count;
	int    Ошибка;
	int    firstedited;
	int    edited;

	LineInfoRecord() { Ошибка = 0; edited = 0; }
};

typedef Массив<LineInfoRecord> LineInfo;

void очистьОшибки(LineInfo& li);

struct LineInfoRemRecord : Движимое<LineInfoRemRecord> {
	int    firstedited;
	int    edited;
};

typedef Вектор<LineInfoRemRecord> LineInfoRem;

void Renumber(LineInfo& lf);
void ClearBreakpoints(LineInfo& lf);
void ValidateBreakpoints(LineInfo& lf);

class РедакторКода;

class EditorBar : public ФреймЛево<Ктрл> {
public:
	virtual void рисуй(Draw& w);
	virtual void двигМыши(Точка p, dword flags);
	virtual void выходМыши();
	virtual void леваяВнизу(Точка p, dword flags);
	virtual void леваяДКлик(Точка p, dword flags);
	virtual void праваяВнизу(Точка p, dword flags);
	virtual void колесоМыши(Точка p, int zdelta, dword keyflags);

private:
	struct LnInfo : Движимое<LnInfo> {
		int    lineno;
		Ткст breakpoint;
		int    Ошибка;
		int    firstedited;
		int    edited;
		Рисунок  icon;
		Ткст annotation;

		LnInfo() { lineno = -1; Ошибка = 0; firstedited = 0; edited = 0; }
	};
	
	Вектор<LnInfo>   li;
	LineInfoRem      li_removed;

	РедакторКода       *editor;
	int              ptrline[2];
	Рисунок            ptrimg[2];
	bool             bingenabled;
	bool             hilite_if_endif;
	bool             line_numbers;
	int              annotations;
	bool             ignored_next_edit;
	int              next_age;
	int              active_annotation;

	Ткст& PointBreak(int& y);
	void    sPaintImage(Draw& w, int y, int fy, const Рисунок& img);

public:
	Событие<int> WhenBreakpoint;
	Событие<>    WhenAnnotationMove;
	Событие<>    WhenAnnotationClick;
	Событие<>    WhenAnnotationRightClick;

	void вставьСтроки(int i, int count);
	void удалиСтроки(int i, int count);
	void очистьСтроки();

	void промотай()                          { освежи(); }

	void SyncSize();

	void Renumber(int linecount);
	void ClearBreakpoints();
	void ValidateBreakpoints();

	Ткст  GetBreakpoint(int ln);
	void    SetBreakpoint(int ln, const Ткст& s);
	void    SetEdited(int ln, int count = 1);
	void    ClearEdited();
	void    устОш(int ln, int err);
	void    очистьОшибки(int ln);

	void SetEditor(РедакторКода *e)           { editor = e; }

	LineInfo GetLineInfo() const;
	void     SetLineInfo(const LineInfo& li, int total);
	LineInfoRem & GetLineInfoRem()                   { return li_removed; }
	void     SetLineInfoRem(LineInfoRem pick_ li)    { li_removed = pick(li); }
	
	void     ClearAnnotations();
	void     SetAnnotation(int line, const Рисунок& img, const Ткст& ann);
	Ткст   GetAnnotation(int line) const;

	int      дайНомСтр(int lineno) const;
	int      GetNoLine(int line) const;

	void     устУк(int line, const Рисунок& img, int i);
	void     HidePtr();

	void     EnableBreakpointing(bool b)     { bingenabled = b; }
	void     HiliteIfEndif(bool b)           { hilite_if_endif = b; освежи(); }
	void     LineNumbers(bool b);
	void     Annotations(int width);
	
	bool     IsHiliteIfEndif() const         { return hilite_if_endif; }
	
	int      GetActiveAnnotationLine() const { return active_annotation; }

	EditorBar();
	virtual ~EditorBar();
};

struct IdentPos {
	int    begin;
	int    end;
	Ткст ident;
};

Массив<IdentPos> GetLineIdent(const char *line);
Вектор<Точка>   GetLineString(const wchar *wline, bool& is_begin, bool& is_end);

inline int  CharFilterCIdent(int i)  { return IsAlNum(i) || i == '_' ? i : 0; }
inline bool iscidl(int c)            { return iscid(c) || буква_ли(c); }
inline bool islbrkt(int c)           { return c == '{' || c == '[' || c == '('; }
inline bool isrbrkt(int c)           { return c == '}' || c == ']' || c == ')'; }
inline bool isbrkt(int c)            { return islbrkt(c) || isrbrkt(c); }

struct ДиалогНайтиЗаменить : ФреймНиз< WithIDEFindReplaceLayout<ТопОкно> > {
	ШТкст itext;
	bool    replacing;

	virtual bool Ключ(dword ключ, int count);
	void настрой(bool doreplace);
	void синх();
	bool инкрементален_ли() const              { return incremental.включен_ли() && incremental; }
	
	typedef ДиалогНайтиЗаменить ИМЯ_КЛАССА;

	ДиалогНайтиЗаменить();
};

#include "Syntax.h"
#include "CSyntax.h"
#include "DiffSyntax.h"
#include "TagSyntax.h"
#include "LogSyntax.h"
#include "PythonSyntax.h"

class РедакторКода : public СтрокРедакт, public HighlightSetup
{
	friend class EditorBar;

public:
	virtual bool  Ключ(dword code, int count);
	virtual void  леваяВнизу(Точка p, dword keyflags);
	virtual void  леваяДКлик(Точка p, dword keyflags);
	virtual void  LeftTriple(Точка p, dword keyflags);
	virtual void  леваяПовтори(Точка p, dword keyflags);
	virtual void  двигМыши(Точка p, dword keyflags);
	virtual Рисунок рисКурсора(Точка p, dword keyflags);
	virtual void  сериализуй(Поток& s);
	virtual void  выходМыши();
	virtual void  колесоМыши(Точка p, int zdelta, dword keyFlags);

protected:
	virtual void HighlightLine(int line, Вектор<СтрокРедакт::Highlight>& h, int64 pos);
	virtual void вставьПеред(int pos, const ШТкст& s);
	virtual void вставьПосле(int pos, const ШТкст& s);
	virtual void удалиПеред(int pos, int size);
	virtual void удалиПосле(int pos, int size);
	virtual void DirtyFrom(int line);
	virtual void SelectionChanged();

	virtual void очистьСтроки();
	virtual void вставьСтроки(int line, int count);
	virtual void удалиСтроки(int line, int count);

	virtual void NewScrollPos();

	virtual Ткст  дайВставьТекст();

	EditorBar   bar;
	Вектор<int> line2;

	struct SyntaxPos {
		int    line;
		Ткст data;
		
		void очисть() { line = 0; data.очисть(); }
	};
	
	SyntaxPos   syntax_cache[6];

//	EditorSyntax rm_ins;

	char    rmb;
	int64   highlight_bracket_pos0;
	int64   highlight_bracket_pos;
	bool    bracket_flash;
	int64   bracket_start;

	bool    barline;
	double  stat_edit_time;
	Время    last_key_time;

	bool    auto_enclose;
	bool    mark_lines;
	bool    check_edited;
	bool    persistent_find_replace;
	bool    do_ff_restore_pos;
	bool    withfindreplace;
	bool    wordwrap;

	int     ff_start_pos;

	ДиалогНайтиЗаменить findreplace;
	
	enum {
		WILDANY = 16,
		WILDONE,
		WILDSPACE,
		WILDNUMBER,
		WILDID,
	};

	struct Found {
		int     тип;
		ШТкст text;
	};

	Массив<Found> foundwild;
	ШТкст      foundtext;
	bool   foundsel;
	bool   found, notfoundfw, notfoundbk;
	int64  foundpos;
	int    foundsize;
	
	enum { SEL_CHARS, SEL_WORDS, SEL_LINES };
	int    selkind;

	ШТкст illuminated;

	Ткст  iwc;

	Ткст highlight;
	
	int    spellcheck_comments = 0;
	bool   wordwrap_comments = true;
	
	struct Подсказка : Ктрл {
		Значение v;
		const Дисплей *d;
		
		virtual void рисуй(Draw& w);
		
		Подсказка();
	};
	
	Подсказка   tip;
	int   tippos;
	
	int   replacei;
	
	bool          search_canceled;
	int           search_time0;
	Один<Progress> search_progress;
	
	Ткст        refresh_info; // serialized next line syntax context to detect the need of full освежи

	struct HlSt;
	
	bool   MouseSelSpecial(Точка p, dword flags);
	void   иницНайтиЗаменить();
	void   CancelBracketHighlight(int64& pos);
	void   FindPrevNext(bool prev);
	void   проверьФигСкобы();
	void   OpenNormalFindReplace0(bool replace);
	void   OpenNormalFindReplace(bool replace);
	void   FindReplaceAddHistory();
	void   FindWildcard();
	void   ReplaceWildcard();
	void   InsertWildcard(const char *s);
	void   IncrementalFind();
	void   NotFound();
	void   NoFindError();
	void   проверьОсвежиСинтакс(int64 pos, const ШТкст& text);

	void   устНайден(int fi, int тип, const ШТкст& text);

	int     сверь(const wchar *f, const wchar *s, int line, bool we, bool icase, int fi = 0);
	ШТкст GetWild(int тип, int& i);
	ШТкст GetReplaceText();

	bool   InsertRS(int chr, int count = 1);

	void   IndentInsert(int chr, int count);

	void   ForwardWhenBreakpoint(int i);

	bool   ToggleSimpleComment(int &start_line, int &end_line, bool usestars = true);
	void   ToggleLineComments(bool usestars = false);
	void   ToggleStarComments();
	void   Enclose(const char *c1, const char *c2, int l = -1, int h = -1);
	void   сделай(Событие<Ткст&> op);
	void   TabsOrSpaces(Ткст& out, bool maketabs);
	void   LineEnds(Ткст& out);

	enum {
		TIMEID_PERIODIC = Ктрл::TIMEID_COUNT,
		TIMEID_COUNT,
	};

	void   Periodic();

	void   StartSearchProgress(int64 l, int64 h);
	bool   SearchProgress(int line);
	bool   SearchCanceled();
	void   EndSearchProgress();

	Ткст дайОсвежиИнфо(int pos);

public:
	struct MouseTip {
		int            pos;
		Значение          значение;
		const Дисплей *дисплей;
		Размер           sz;
	};

	Событие<>            WhenSelection;
	Врата<MouseTip&>    WhenTip;
	Событие<>            WhenLeftDown;
	Событие<int64>       WhenCtrlClick;
	Событие<>            WhenAnnotationMove;
	Событие<>            WhenAnnotationClick;
	Событие<>            WhenAnnotationRightClick;
	Событие<>            WhenOpenFindReplace;
	Событие<Ткст&>     WhenPaste;
	Событие<>            WhenUpdate;
	Событие<int>         WhenBreakpoint;

	Событие<const Вектор<Кортеж<int64, int>>&> WhenFindAll;

	ФреймВерх<Кнопка>    topsbbutton;
	ФреймВерх<Кнопка>    topsbbutton1;

	static dword find_next_key;
	static dword find_prev_key;
	static dword replace_key;

	void   очисть();

	void   Highlight(const Ткст& h);
	Ткст дайПодсвет() const       { return highlight; }

	void   EscapeFindReplace();
	void   закройНайдиЗам();
	void   FindReplace(bool pick_selection, bool pick_text, bool replace);
	void   найдиВсе();
	bool   найдиОт(int64 pos, bool back, bool block);
	bool   найдиРегВыр(int64 pos, bool block);
	bool   найди(bool back, bool block);
	bool   найди(bool back, bool blockreplace, bool replace);
	void   найдиСледщ();
	void   найдиПредш();
	bool   GetStringRange(int64 cursor, int64& b, int64& e) const;
	bool   GetStringRange(int64& b, int64& e) const { return GetStringRange(дайКурсор64(), b, e); }
	bool   найдиТкст(bool back);
	bool   FindLangString(bool back);
	void   замени();
	void   ReplaceAll(bool rest);
	int    BlockReplace();

	void   MakeTabsOrSpaces(bool tabs);
	void   MakeLineEnds();

	void   копируйСлово();
	void   поменяйСимы();
	void   дублируйСтроку();
	void   помести(int chr);
	void   FinishPut();

	void   SerializeFind(Поток& s);
	bool   IsFindOpen() const                       { return findreplace.открыт(); }
	void   FindClose()                              { закройНайдиЗам(); }

	void   идиК();

	void   DoFind();
	void   DoFindBack();

//	void    FindWord(bool back);
	ШТкст GetI();
	void    SetI(Ктрл *edit);
	void    PutI(WithDropChoice<EditString>& edit);

	void   MoveNextWord(bool sel);
	void   MovePrevWord(bool sel);
	void   MoveNextBrk(bool sel);
	void   MovePrevBrk(bool sel);

	Ткст GetWord(int64 pos);
	Ткст GetWord();

	bool   GetWordPos(int64 pos, int64& l, int64& h);

	void   DeleteWord();
	void   DeleteWordBack();
	void   SetLineSelection(int l, int h);
	bool   GetLineSelection(int& l, int& h);
	void   TabRight();
	void   TabLeft();
	void   SwapUpDown(bool up);

	void    CheckEdited(bool e = true)                { check_edited = e; }
	bool    GetCheckEdited()                          { return check_edited; }

	void    EditorBarLayout()                         { bar.SyncSize(); }

	LineInfo GetLineInfo() const                      { return bar.GetLineInfo(); }
	void     SetLineInfo(const LineInfo& lf);
	LineInfoRem GetLineInfoRem()                      { return LineInfoRem(bar.GetLineInfoRem(), 0); }
	void     SetLineInfoRem(LineInfoRem pick_  lf)    { bar.SetLineInfoRem(LineInfoRem(lf, 0)); }
	double   GetStatEditTime() const                  { return stat_edit_time; }
	void     Renumber()                               { bar.Renumber(дайСчётСтрок()); }
	void     ClearBreakpoints()                       { bar.ClearBreakpoints(); }
	void     ValidateBreakpoints()                    { bar.ValidateBreakpoints(); }
	int      дайНомСтр(int line) const                { return bar.дайНомСтр(line); }
	int      GetNoLine(int line) const                { return bar.GetNoLine(line); }
	void     устУк(int line, const Рисунок& img, int i){ bar.устУк(line, img, i); }
	void     HidePtr()                                { bar.HidePtr(); }
	Ткст   GetBreakpoint(int line)                  { return bar.GetBreakpoint(line); }
	void     SetBreakpoint(int line, const Ткст& b) { bar.SetBreakpoint(line, b); }
	void     устОш(int line, int err)              { bar.устОш(line, err); }
	void     очистьОшибки(int line = -1)               { bar.очистьОшибки(line); }
	void     ClearEdited()                            { bar.ClearEdited(); }
	int		 GetUndoCount()                           { return undo.дайСчёт(); }
	void     GotoLine(int line);
	void     EnableBreakpointing()                    { bar.EnableBreakpointing(true); }
	void     DisableBreakpointing()                   { bar.EnableBreakpointing(false); }
	void     Renumber2();
	int      дайСтроку2(int i) const;
	void     перефмтКоммент();

// TODO: Syntax: удали
	void     HiliteScope(byte b)                      { EditorSyntax::hilite_scope = b; освежи(); }
	void     HiliteBracket(byte b)                    { EditorSyntax::hilite_bracket = b; освежи(); }
	void     HiliteIfDef(byte b)                      { EditorSyntax::hilite_ifdef = b; освежи(); }
	void     HiliteIfEndif(bool b)                    { bar.HiliteIfEndif(b); }

	void     ThousandsSeparator(bool b)               { thousands_separator = b; освежи(); }
	void     IndentSpaces(bool is)                    { indent_spaces = is; }
	void     IndentAmount(int ia)                     { indent_amount = ia; }
	void     NoParenthesisIndent(bool b)              { no_parenthesis_indent = b; }
	
	void     SpellcheckComments(int lang)             { spellcheck_comments = lang; освежи(); }
	int      GetSpellcheckComments() const            { return spellcheck_comments; }
	void     WordwrapComments(bool b)                 { wordwrap_comments = b; }
	bool     IsWordwrapComments() const               { return wordwrap_comments; }

	void     NoFindReplace()                          { withfindreplace = false; }

	void     LineNumbers(bool b)                      { bar.LineNumbers(b); }
	void     MarkLines(bool b)                        { mark_lines = b; }
	bool     GetMarkLines()                           { return mark_lines; }
	void     AutoEnclose(bool b)                      { auto_enclose = b; }
	void     BarLine(bool b)                          { barline = b; }
	void     WordWrap(bool b)                         { wordwrap = b; }
	
	void     PersistentFindReplace(bool b = true)     { persistent_find_replace = b; }
	bool     IsPersistentFindReplace() const          { return persistent_find_replace; }

	void     FindReplaceRestorePos(bool b = true)     { do_ff_restore_pos = b; }
	bool     IsFindReplaceRestorePos() const          { return do_ff_restore_pos; }
	
	void     Annotations(int width)                   { bar.Annotations(width); }
	void     ClearAnnotations()                       { bar.ClearAnnotations(); }
	void     SetAnnotation(int i, const Рисунок& icon, const Ткст& a) { bar.SetAnnotation(i, icon, a); }
	Ткст   GetAnnotation(int i) const               { return bar.GetAnnotation(i); }
	int      GetActiveAnnotationLine() const          { return bar.GetActiveAnnotationLine(); }

	void     HideBar()                                { bar.скрой(); }

	void     SyncTip();
	void     CloseTip()                               { if(tip.открыт()) tip.закрой(); tip.d = NULL;  }
	
	void     Illuminate(const ШТкст& text);
	ШТкст  GetIlluminated() const                   { return illuminated; }

	void     Zoom(int d);

	Один<EditorSyntax> дайСинтакс(int line);
	bool IsCursorBracket(int64 pos) const;
	bool IsMatchingBracket(int64 pos) const;

// TODO: Do we really need this ?
	Вектор<IfState> GetIfStack(int line)              { return дайСинтакс(line)->PickIfStack(); }

	struct FindReplaceData {
		Ткст find, replace;
		Ткст find_list, replace_list;
		bool   wholeword, wildcards, ignorecase, samecase, regexp;
	};
	
	FindReplaceData GetFindReplaceData();
	void            SetFindReplaceData(const FindReplaceData& d);

	typedef РедакторКода ИМЯ_КЛАССА;

	РедакторКода();
	virtual ~РедакторКода();

	static void InitKeywords();
};

Ткст ReadList(WithDropChoice<EditString>& e);
void   WriteList(WithDropChoice<EditString>& e, const Ткст& data);

}

#endif
