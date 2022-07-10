#ifndef LAYDES_H
#define LAYDES_H

#include <DinrusIDE/Common/Common.h>
#include <RichEdit/RichEdit.h>
#include <CodeEditor/CodeEditor.h>
#include <IconDes/IconDes.h>
#include <DinrusIDE/Browser/Browser.h>

#define LAYOUTFILE <DinrusIDE/LayDes/LayDes.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS LayImg
#define IMAGEFILE  <DinrusIDE/LayDes/LayDes.iml>
#include <Draw/iml_header.h>

#define KEYNAMESPACE LayoutKeys
#define KEYGROUPNAME "Выкладка"
#define KEYFILE      <DinrusIDE/LayDes/laydes.key>
#include             <CtrlLib/key_header.h>

inline Шрифт LayFont() { return Arial(Zy(11)); }
inline Шрифт LayFont2() { return Arial(Zy(14)); }

class DiffPacker {
	Ткст master;
	int         mapn;
	Буфер<int> map;
	Буфер<int> prev;

public:
	void   SetMaster(const Ткст& master);
	Ткст Pack(const Ткст& data);
};

Ткст DiffPack(const Ткст& master, const Ткст& data);
Ткст DiffUnpack(const Ткст& master, const Ткст& pack);

struct TypeProperty : Движимое<TypeProperty> {
	int    level;
	Ткст тип;
	Ткст имя;
	Ткст defval;
	Ткст help;

	TypeProperty() { level = Null; }
};

enum { LAYOUT_CTRL, LAYOUT_SUBCTRL, LAYOUT_TEMPLATE };

struct LayoutType : Движимое<LayoutType> {
	int                        kind;
	Массив<TypeProperty>        property;
	МассивМап<Ткст, EscValue> methods;
	Ткст                     группа;
	Ткст                     name_space;
	Рисунок                      icon[2];
	Размер                       iconsize[2];

	LayoutType()               { iconsize[0] = iconsize[1] = Null; }
};

ВекторМап<Ткст, ВекторМап<Ткст, Ткст> >& LayoutEnums();
ВекторМап<Ткст, LayoutType>&                 LayoutTypes();

Точка ReadPoint(СиПарсер& p);

struct EscDraw : public EscHandle {
	Draw& w;

	void DrawRect(EscEscape& e);
	void DrawLine(EscEscape& e);
	void DrawText(EscEscape& e);
	void DrawSmartText(EscEscape& e);
	void DrawQtf(EscEscape& e);
	void дайРазмТекста(EscEscape& e);
	void DrawImage(EscEscape& e);
	void DrawImageColor(EscEscape& e);

	typedef EscDraw ИМЯ_КЛАССА;

	EscDraw(EscValue& v, Draw& w);
};

EscValue EscColor(Цвет c);
Цвет    ColorEsc(EscValue v);

EscValue EscSize(Размер sz);
Размер     SizeEsc(EscValue v);

EscValue EscPoint(Точка sz);
Точка    PointEsc(EscValue v);

EscValue EscRect(const Прям& r);
Прям     RectEsc(EscValue v);

EscValue EscFont(Шрифт f);
Шрифт     FontEsc(EscValue v);

void     EatUpp(СиПарсер& p);

struct ItemProperty : public Ктрл {
	int     level;
	Ткст  имя;
	Значение   defval;
	Ткст  help;

	virtual void     рисуй(Draw& w);

	virtual int      дайВысоту() const;

	virtual void     устНабсим(byte charset);
	virtual void     читай(СиПарсер& p);
	virtual Ткст   сохрани() const;
	virtual bool     PlaceFocus(dword k, int c);

	ItemProperty() { NoWantFocus(); level = 0; }
	virtual ~ItemProperty() {}

	static ВекторМап<Ткст, ItemProperty *(*)()>& Factory();
	static ItemProperty *создай(const Ткст& имя);
	static void          регистрируй(const char *имя, ItemProperty *(*creator)());
};

template <class Editor>
class EditorProperty : public ItemProperty {
public:
	virtual Значение    дайДанные() const            { return ~editor; }
	virtual bool     PlaceFocus(dword k, int c) { editor.устФокус(); return editor.Ключ(k, c); }

protected:
	void EditAction()                           { this->UpdateActionRefresh(); }

	Editor editor;

	EditorProperty() {
		editor.WhenAction = callback(this, &EditorProperty::EditAction);
	}
};

Ткст ReadPropertyParam(СиПарсер& p);

struct RawProperty : public EditorProperty<EditString>
{
	virtual Ткст   сохрани() const;
	virtual void     читай(СиПарсер& p);

	static ItemProperty *создай()            { return new RawProperty; }

	RawProperty() {
		добавь(editor.HSizePosZ(100, 2).TopPos(2));
	}
};

struct PropertyPane : СтатичПрям {
public:
	void Выкладка() override;
	void отпрыскФок() override;
	void колесоМыши(Точка, int zdelta, dword) override;

public:
	PropertyPane();
	
	int        y;
	СтатичПрям pane;
	ПромотБар  sb;

	void очисть();
	void добавь(ItemProperty& c);
	void SetSb();
	void промотай();
	void AfterCreate();
};

struct LayoutItem {
	Ткст              тип;
	Ткст              variable;
	Ктрл::ПозЛога        pos;
	Массив<ItemProperty> property;
	bool                hide;
	byte                charset;

private:
	Размер      csize;
	Чертёж   cache;

	void   CreateProperties(const Ткст& classname, int level);
	void   CreateMethods(EscValue& ctrl, const Ткст& тип, bool copy) const;
	void   UnknownPaint(Draw& w);

public:
	void     инивалидируй()               { csize.cx = -1; }
	EscValue CreateEsc() const;
	EscValue ExecuteMethod(const char *method, Вектор<EscValue>& arg) const;
	EscValue ExecuteMethod(const char *method) const;
	Размер     дайМинРазм();
	Размер     дайСтдРазм();
	void     рисуй(Draw& w, Размер sz, bool sample = false);
	void     создай(const Ткст& тип);
	int      FindProperty(const Ткст& s) const;
	void     устНабсим(byte charset);
	void     ReadProperties(СиПарсер& p, bool addunknow = true);
	Ткст   SaveProperties(int y = 0) const;
	Ткст   сохрани(int i, int y, const Ткст& eol) const;

	LayoutItem()                        { csize.cx = -1; hide = false; charset = CHARSET_UTF8; }
};

Рисунок GetTypeIcon(const Ткст& тип, int cx, int cy, int i, Цвет bg);

Массив<LayoutItem> ReadItems(СиПарсер& p, byte charset);

class LayoutUndo {
	Вектор<Ткст> stack;

public:
	void               очисть()                        { stack.очисть(); }
	int                дайСчёт() const               { return stack.дайСчёт(); }
	operator           bool() const                   { return stack.дайСчёт(); }
	void               сунь(const Ткст& state);
	const Ткст&      верх() const                    { return stack.верх(); }
	Ткст             вынь();
};

struct LayoutData {
	Ткст имя;
	Размер   size;
	Массив<LayoutItem> элт;
	LayoutUndo undo, redo;
	byte charset;

private:
	Ткст      MakeState();
	void        LoadState(const Ткст& s);
	void        Do(LayoutUndo& u1, LayoutUndo& u2);
	Ткст      GetTID(int i);

public:
	void        устНабсим(byte charset);
	void        читай(СиПарсер& p);
	Ткст      сохрани(int y, const Ткст& eol);
	Ткст      сохрани(const Вектор<int>& sel, int y, const Ткст& eol);
	void        SaveState();
	bool        IsUndo();
	void        Undo();
	bool        IsRedo();
	void        Redo();

	rval_default(LayoutData);

	LayoutData() { size = Размер(400, 200); charset = CHARSET_UTF8; }
};

class LayDesigner;

struct LayDesEditPos {
	Время              filetime;
	Массив<LayoutUndo> undo;
	Массив<LayoutUndo> redo;
	int               layouti;
	Вектор<int>       cursor;

	LayDesEditPos() { filetime = Null; }
};

class LayDes : public СтатичПрям {
public:
	virtual void   рисуй(Draw& w) override;
	virtual Рисунок  рисКурсора(Точка p, dword keyflags) override;
	virtual void   леваяВнизу(Точка p, dword keyflags) override;
	virtual void   леваяПовтори(Точка p, dword keyflags) override;
	virtual void   двигМыши(Точка p, dword keyflags) override;
	virtual void   леваяВверху(Точка p, dword keyflags) override;
	virtual void   праваяВнизу(Точка p, dword keyflags) override;
	virtual void   Выкладка() override;
	virtual bool   горячаяКлав(dword ключ) override;

private:
	bool   DoKey(dword ключ, int count);
	bool   DoHotKey(dword ключ);

	struct KeyMaster : public КтрлРодитель {
		LayDes *d;

		virtual bool горячаяКлав(dword ключ) override {
			return d->DoHotKey(ключ);
		}

		virtual bool Ключ(dword ключ, int count) override {
			return d->DoKey(ключ, count);
		}
	};

	Ткст   filename;
	byte     charset;
	Ткст   layfile;
	bool     newfile;
	Время     filetime;
	Ткст   fileerror;

	friend class LayDesigner;

	LayDesigner       *frame;
	ToolBar            toolbar;
	БарМеню            menubar;
	
	КтрлРодитель         layouts;
	КтрлМассив          list;
	EditString         search;

	КтрлМассив          элт;
	PropertyPane       property;

	WithDropChoice<EditString> тип;
	EditString                 variable;

	KeyMaster          km;
	Сплиттер           lsplit, isplit, rsplit;
	ФреймВерх<Сплиттер> twsplit;
	ПрокрутБары         sb;
	ФреймПраво<Надпись>  status;

	Массив<LayoutData> layout;
	int               currentlayout;

	Вектор<Точка>     handle;
	Вектор<int>       cursor;
	int               draghandle;
	Точка             dragbase;
	Размер              draglayoutsize;
	Прям              dragrect;
	Вектор<Прям>      itemrect;
	int               basesel;

	bool              usegrid;
	bool              ignoreminsize;
	bool              sizespring;

	WithMatrixLayout<ТопОкно>  matrix;
	WithSettingLayout<ТопОкно> setting;

	Ткст EOL = "\r\n";

	struct TempGroup {
		Ткст temp;
		Ткст группа;
		bool operator==(const TempGroup& b) const { return temp == b.temp && группа == b.группа; }
		friend unsigned дайХэшЗнач(const TempGroup& g) { return 0; }
		TempGroup(const Ткст& temp, const Ткст& группа) : temp(temp), группа(группа) {}
	};

	Прям   CtrlRect(Ктрл::ПозЛога pos, Размер sz);
	Прям   CtrlRectZ(Ктрл::ПозЛога pos, Размер sz);
	void   AddHandle(Draw& w, int x, int y);
	Точка  нормализуй(Точка p);
	Точка  ZPoint(Точка p);
	int    FindHandle(Точка p);
	int    FindItem(Точка p, bool cursor_first = false);
	void   GetSprings(Прям& l, Прям& t, Прям& r, Прям& b);
	void   DrawSpring(Draw& w, const Прям& r, bool horz, bool spring);
	void   выделиОдин(int ii, dword flags);
	void   StoreItemRects();
	void   LoadItems();
	void   LoadItem(int элт);
	void   ReloadItems();
	void   SyncItems();
	void   SyncItem(int элт, int style);
	void   SyncProperties(bool sync);
	void   SyncLayoutList();
	void   ищи();
	void   CreateCtrl(const Ткст& тип);
	void   Группа(Бар& bar, const Ткст& группа);
	void   Templates(Бар& bar);
	void   TemplateGroup(Бар& bar, TempGroup tg);
	void   Template(Бар& bar, const Ткст& temp);
	int    ParseLayoutRef(Ткст cls, Ткст& base) const;
	void   PaintLayoutItems(Draw& w, int layout, Размер size, Индекс<int>& passed, const Вектор<bool>& cursor);

	void   SaveState();
	void   SetSb();

	void   FrameFocus();
	void   PropertyChanged();

	void   SetBar();
	void   промотай();

	void   SetStatus(bool down = false);

	void        слей();
	LayoutData& CurrentLayout();
	void        LayoutCursor();

	LayoutItem& CurrentItem();
	void        ItemClick();

	Ткст      SaveSelection(bool scrolled = false);
	LayoutData  LoadLayoutData(const Ткст& s);

	void        Undo();
	void        Redo();
	void        вырежь();
	void        копируй();
	void        VisGen();
	void        Paste();
	void        Delete();
	void        Duplicate();
	void        MoveUp();
	void        MoveDown();
	void        вставьТиБ(int line, PasteClip& d);
	void        тяни();
	void        SortItems();
	void        выбериВсе();

	void        Matrix();

	void        TypeEdit();
	void        VariableEdit();

	void        ToggleGrid();
	void        ToggleMinSize();
	void        ToggleSizeSpring();
	void        Settings();

	enum Code {
		A_LEFT, A_HCENTER, A_RIGHT, A_TOP, A_VCENTER, A_BOTTOM, A_LABEL,
		A_SAMEWIDTH, A_SAMEHEIGHT, A_SAMESIZE, A_VERTCENTER, A_HORZCENTER,
		A_MINWIDTH, A_MINHEIGHT, AUTOSPRING
	};

	void        Align(int тип);
	void        SetSprings(dword s);
	void        ShowSelection(bool s);

	void        GoTo(int ключ);
	void        AddLayout(bool insert);
	void        DuplicateLayout();
	void        RenameLayout();
	void        RemoveLayout();
	void        PrevLayout();
	void        NextLayout();
	void        MoveLayoutUp();
	void        MoveLayoutDown();
	void        DnDInsertLayout(int line, PasteClip& d);
	void        DragLayout();
	void        LayoutMenu(Бар& bar);

	void        EditBar(Бар& bar);
	void        AlignBar(Бар& bar);
	void        SizeBar(Бар& bar);
	void        MoveBar(Бар& bar);
	void        SpringBar(Бар& bar);
	void        OptionBar(Бар& bar);
	void        HelpBar(Бар& bar);
	void        MainToolBar(Бар& bar);

	void        LayoutBar(Бар& bar);
	void        ItemBar(Бар& bar);
	void        MainMenuBar(Бар& bar);

	void        EditMenu(Бар& bar);
	
	void        GotoUsing();

	void        SyncUsc();
	void        сохрани();
	void        CloseDesigner();

	void        SaveEditPos();
	void        RestoreEditPos();
	
	void        FindLayout(const Ткст& имя, const Ткст& item_name);
	Ткст      GetLayoutName() const;

	bool        грузи(const char *filename, byte charset);

	typedef LayDes ИМЯ_КЛАССА;

	LayDes();

public:
	Ктрл&          DesignerCtrl()             { return km; }
	void           сериализуй(Поток& s) override;
};

class LayDesigner : public IdeDesigner {
	LayDes         designer;
	КтрлРодитель     parent;

public:
	virtual Ткст дайИмяф() const override { return designer.filename; }
	virtual void   сохрани() override              { designer.сохрани(); }
	virtual void   SyncUsc() override           { designer.SyncUsc(); }
	virtual void   SaveEditPos() override       { designer.SaveEditPos(); }
	virtual void   EditMenu(Бар& menu) override { designer.EditMenu(menu); }
	virtual int    дайНабсим() const override  { return designer.charset; }
	virtual Ктрл&  DesignerCtrl() override      { return parent; }

	void сериализуй(Поток& s)                   { designer.сериализуй(s); }
	bool грузи(const char *filename, byte cs)    { return designer.грузи(filename, cs); }

	void FindLayout(const Ткст& имя, const Ткст& элт) { designer.FindLayout(имя, элт); }
	Ткст GetCurrentLayout() const             { return designer.GetLayoutName(); }

	LayDesigner()                               { parent.добавь(designer.DesignerCtrl().SizePos()); }
};

#endif
