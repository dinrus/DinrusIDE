#ifndef _TextDiffCtrl_TextDiffCtrl_h
#define _TextDiffCtrl_TextDiffCtrl_h

#include <CtrlLib/CtrlLib.h>

namespace РНЦП {

#define IMAGECLASS DiffImg
#define IMAGEFILE <TextDiffCtrl/Diff.iml>
#include <Draw/iml_header.h>

class ТекстСекция
{
public:
	ТекстСекция(int start1, int count1, int start2, int count2, bool same)
		: start1(start1), count1(count1), start2(start2), count2(count2), same(same) {}

public:
	int      start1;
	int      count1;
	int      start2;
	int      count2 : 31;
	unsigned same   : 1;
};

Массив<ТекстСекция> сравниМапыСтрок(const Вектор<Ткст>& l1, const Вектор<Ткст>& l2);
Вектор<Ткст>     дайМапСтрок(Поток& stream);
Вектор<Ткст>     дайМапСтрокФайла(const Ткст& path);
Вектор<Ткст>     дайМапСтрокТкст(const Ткст &s);

class КтрлСравниТекст : public Ктрл {
public:
	virtual void   рисуй(Draw& draw);
	virtual void   Выкладка();
	virtual void   колесоМыши(Точка pt, int zdelta, dword keyflags);
	virtual void   двигМыши(Точка pt, dword keyflags);
	virtual void   леваяВнизу(Точка pt, dword keyflags);
	virtual void   леваяДКлик(Точка pt, dword keyflags);
	virtual void   леваяВверху(Точка pt, dword keyflags);
	virtual void   леваяПовтори(Точка pt, dword keyflags);
	virtual void   праваяВнизу(Точка p, dword keyflags);
	virtual bool   Ключ(dword ключ, int repcnt);
	virtual void   расфокусирован();

private:
	void           SelfScroll();
	void           PairScroll(КтрлСравниТекст *ctrl);
	void           обновиШирину();
	ШТкст          разверниТабы(const wchar *line) const;
	int            меряйДлину(const wchar *line) const;
	bool           дайВыделение(int& l, int& h) const;
	void           DoSelection(int y, bool shift);
	void           копируй();
	int            дайНомСтр(int y, int& yy);
	int            GetMatchLen(const wchar *s1, const wchar *s2, int len);
	bool           LineDiff(bool left, Вектор<СтрокРедакт::Highlight>& hln, Цвет eq_color,
	                        const wchar *s1, int l1, int h1,
	                        const wchar *s2, int l2, int h2, int depth);

private:
	struct Строка {
		Строка() : number(Null), level(0) {}
		int    number;
		bool   diff;
		Ткст text;
		int    level;
		Ткст text_diff;
		int    number_diff;
	};
	Массив<Строка>    lines;
	int            maxwidth;
	ПрокрутБары     scroll;
	Шрифт           font;
	Шрифт           number_font;
	Цвет          number_bg;
	Цвет          gutter_fg;
	Цвет          gutter_bg;
	Размер           letter;
	int            tabsize;
	int            number_width;
	int            number_yshift;
	int            gutter_width;
	int            cursor;
	int            anchor;
	bool           gutter_capture;
	bool           show_line_number;
	bool           show_white_space;
	bool           show_diff_highlight;
	bool           change_paper_color;
	bool           left = false;

	typedef КтрлСравниТекст ИМЯ_КЛАССА;
	
	friend struct КтрлДиффТекст;

public:
	Событие<>        WhenSel;
	Событие<>        ПриПромоте;
	Обрвыз2<int, int> WhenLeftDouble;
	Событие<Вектор<СтрокРедакт::Highlight>&, const ШТкст&> WhenHighlight;

	void           устСчёт(int c);
	void           AddCount(int c);
	int            дайСчёт() const { return lines.дайСчёт(); }

	void           устШрифт(Шрифт f, Шрифт nf);
	void           устШрифт(Шрифт f);
	Шрифт           дайШрифт() const { return font; }
	Шрифт           GetNumberFont() const { return number_font; }

	void           NumberBgColor(Цвет bg)  { number_bg = bg; освежи(); }
	Цвет          GetNumberBgColor() const { return number_bg; }

	void           AutoHideSb(bool ssb=true){ scroll.автоСкрой(ssb); }
	void           ShowSb(bool ssb)         { scroll.покажиУ(ssb); }
	void           HideSb()                 { ShowSb(false); }

	void           Gutter(int size)         { gutter_width = size; освежи(); }
	void           NoGutter()               { gutter_width = 0; освежи(); }

	void           TabSize(int t);
	int            GetTabSize() const { return tabsize; }

	void           уст(int line, Ткст text, bool diff, int number, int level, Ткст text_diff, int number_diff);
	Ткст         дайТекст(int line) const { return lines[line].text; }
	bool           GetDiff(int line) const { return lines[line].diff; }
	int            GetNumber(int line) const { return lines[line].number; }
	int            GetNumberDiff(int line) const { return lines[line].number_diff; }
	bool           HasLine(int line) const { return !пусто_ли(lines[line].number); }

	Точка          дайПоз() const;
	void           устПоз(Точка pos);

	int            GetSb() const { return scroll.дай().y; }
	void           SetSb(int y)  { scroll.уст(0, y); }

	void           очистьВыделение()           { cursor = Null; освежи(); WhenSel(); }
	void           устВыделение(int l, int h) { cursor = l; anchor = h; освежи(); WhenSel(); }
	bool           выделение_ли() const        { return cursor >= 0; }
	bool           выделен(int i) const;

	void           ShowLineNumber(bool sln)   { show_line_number = sln; освежи(); }
	void           HideLineNumber()           { ShowLineNumber(false); }

	void           ShowWhiteSpace(bool sws)   { show_white_space = sws; освежи(); }
	void           HideWhiteSpace()           { ShowWhiteSpace(false); }

	void           DiffHighlight(bool dh)     { show_diff_highlight = dh; освежи(); }
	void           NoDiffHighlight()          { DiffHighlight(false); }

	void           ChangePaperColor(bool cpc) { change_paper_color = cpc; освежи(); }
	void           NoChangePaperColor()       { ChangePaperColor(false); }
	
	void           устЛев()                  { left = true; }
	
	Ткст         RemoveSelected(bool cr);

	Событие<>        ScrollWhen(КтрлСравниТекст& pair) { return THISBACK1(PairScroll, &pair); }

	КтрлСравниТекст();
};

struct КтрлДиффТекст : public Сплиттер {
	КтрлСравниТекст left;
	КтрлСравниТекст right;
	ФреймВерх<Кнопка> next, prev;

	typedef КтрлДиффТекст ИМЯ_КЛАССА;
	
	virtual bool Ключ(dword ключ, int count);

	void уст(Поток& l, Поток& r);
	void уст(const Ткст& l, const Ткст& r);
	void вставьФреймСлева(КтрлФрейм& f)                     { left.вставьФрейм(0, f); }
	void вставьФреймСправа(КтрлФрейм& f)                    { right.вставьФрейм(0, f); }
	void добавьФреймСлева(КтрлФрейм& f)                        { left.добавьФрейм(f); }
	void добавьФреймСправа(КтрлФрейм& f)                       { right.добавьФрейм(f); }
	void устШрифт(Шрифт f, Шрифт nf)                          { left.устШрифт(f, nf); right.устШрифт(f, nf); }
	void устШрифт(Шрифт f)                                   { left.устШрифт(f); right.устШрифт(f); }

	void GetLeftLine(int number, int line);
	void GetRightLine(int number, int line);
	
	void найдиДифф(bool fw);

	Ткст Merge(bool l, bool cr);
	
	int  GetSc() const                                     { return left.GetSb(); }
	void Sc(int sc)                                        { left.SetSb(sc); }

	Обрвыз1<int> WhenLeftLine;
	Обрвыз1<int> WhenRightLine;

	КтрлДиффТекст();
};

struct ДиффДлг : public ТопОкно {
	bool Ключ(dword ключ, int count) override;

	КтрлДиффТекст         diff;
	ФреймВерх<СтатичПрям> p;
	DataPusher           l;
	Кнопка               write;
	Кнопка               revert;
	Кнопка               remove;
	Ткст               editfile;
	Ткст               backup;
	Ткст               extfile;

	typedef ДиффДлг ИМЯ_КЛАССА;

	void освежи();
	void пиши();
	void выполни(const Ткст& f);

	static Событие<const Ткст&, Вектор<СтрокРедакт::Highlight>&, const ШТкст&> WhenHighlight;

	ДиффДлг();
};

bool HasCrs(const Ткст& path);

FileSel& DiffFs();

struct FileDiff : ДиффДлг {
	ФреймВерх<DataPusher> r;

	virtual void открой();
	void выполни(const Ткст& f);

	typedef FileDiff ИМЯ_КЛАССА;

	Ткст GetExtPath() const { return ~r; }

	FileDiff(FileSel& fs);

	FileSel& fs;

	void выполни(const Ткст& lpath, const Ткст& rpath);
};

class DirDiffDlg : public ТопОкно {
public:
	virtual bool горячаяКлав(dword ключ);
	virtual bool Ключ(dword ключ, int count);

protected:
	Сплиттер                   files_diff;
	КтрлРодитель                 files_pane;
	СписокФайлов                   files;
	СписокБроса                   recent;

	SelectDirButton            seldir1;
	WithDropChoice<EditString> dir1;
	SelectDirButton            seldir2;
	WithDropChoice<EditString> dir2;
	Опция                     hidden;
	Опция                     split_lines;
	Кнопка                     compare;
	
	Опция                     removed, added, modified;
	EditString				   find;
	Кнопка					   clearFind;

	ФреймВерх<КтрлРодитель>       left, right;
	EditString                 lfile, rfile;
	Кнопка                     copyleft, copyright;
	Кнопка                     revertleft, revertright;
	Кнопка                     removeleft, removeright;
	
	ВекторМап<Ткст, Ткст>  backup;
	
	enum { NORMAL_FILE, DELETED_FILE, NEW_FILE, FAILED_FILE, PATCHED_FILE };
	
	Массив<Кортеж<Ткст, Ткст, Ткст, int>> list;

	static bool FileEqual(const Ткст& f1, const Ткст& f2, int& n);

	void GatherFilesDeep(Индекс<Ткст>& files, const Ткст& base, const Ткст& path);
	void сравни();
	void ShowResult();
	void ClearFiles();
	virtual void Файл();
	void освежи();
	void Backup(const Ткст& path);
	void копируй(bool left);
	СписокФайлов::Файл MakeFile(int i);
	
	friend class PatchDiff;

public:
	Функция<Рисунок(const char *path)> WhenIcon;

	КтрлДиффТекст               diff;

	typedef DirDiffDlg ИМЯ_КЛАССА;

	void устШрифт(Шрифт fnt)                      { diff.устШрифт(fnt); }
	void Dir1(const Ткст& dir)                { dir1 <<= dir; }
	void Dir2(const Ткст& dir)                { dir2 <<= dir; }
	void Dir1AddList(const Ткст& dir)         { dir1.добавьСписок(dir); }
	void Dir2AddList(const Ткст& dir)         { dir2.добавьСписок(dir); }

	Ткст GetLeftFile() const                  { return ~lfile; }
	Ткст GetRightFile() const                 { return ~rfile; }

	DirDiffDlg();
};

struct Patch {
	struct Chunk {
		int            line;
		Вектор<Ткст> orig;
		Вектор<Ткст> patch;
		Ткст         src;
	};
	
	ВекторМап<Ткст, Массив<Chunk>> file;
	Ткст  common_path;
	Ткст  target_dir;

	static int MatchLen(const Ткст& a, const Ткст& b);
	int        MatchCount(const char *dir);
	bool       загрузи0(Поток& in, Progress& pi);

public:
	bool грузи(Поток& in, Progress& pi);
	bool грузи(const char *фн, Progress& pi);
	bool MatchFiles(const Вектор<Ткст>& dir, Progress& pi);
	void SetTargetDir(const char *dir) { target_dir = приставьИмяф(dir, common_path); }
	
	int    дайСчёт() const            { return file.дайСчёт(); }
	Ткст GetTargetDir() const        { return target_dir; }
	Ткст GetFile(int i) const        { return file.дайКлюч(i); }
	Ткст дайПуть(int i) const        { return приставьИмяф(target_dir, file.дайКлюч(i)); }
	Ткст GetPatch(int i) const;
	Ткст GetPatchedFile(int i, const Ткст& file) const;
};

class PatchDiff : public DirDiffDlg {
	Patch  patch;
	Ткст file_path;
	Ткст patched_file;
	
	EditString patch_file;
	EditString target_dir;
	Надпись      failed;

	OpenFileButton  selfile;
	SelectDirButton seldir;
	
	int             failed_count = 0;
	
	void Файл();
	Ткст GetBackup(const Ткст& path);
	int  GetFileIndex() const;

public:
	bool открой(const char *patch_path, const Вектор<Ткст>& target_dirs);
	
	PatchDiff();
};

};

#endif
