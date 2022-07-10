#include "LayDes.h"

#include <DinrusIDE/Common/Common.h>

#define LTIMING(x) //TIMING(x)

#define KEYNAMESPACE LayoutKeys
#define KEYGROUPNAME "Выкладка"
#define KEYFILE      <DinrusIDE/LayDes/laydes.key>
#include             <CtrlLib/key_source.h>

using namespace LayoutKeys;

void LayDes::EditBar(Бар& bar)
{
	bool islayout = !пусто_ли(currentlayout);
	bool iscursor = islayout && cursor.дайСчёт();
	bar.добавь(iscursor, "Вырезать", CtrlImg::cut(), THISBACK(вырежь))
	   .Ключ(K_SHIFT_DELETE)
	   .Ключ(K_CTRL_X);
	bar.добавь(iscursor, "Копировать", CtrlImg::copy(), THISBACK(копируй))
	   .Ключ(K_CTRL_INSERT)
	   .Ключ(K_CTRL_C);
	bar.добавь(islayout, "Вставить", CtrlImg::paste(), THISBACK(Paste))
	   .Ключ(K_SHIFT_INSERT)
	   .Ключ(K_CTRL_V);
	bar.добавь(iscursor, "Удалить", CtrlImg::remove(), THISBACK(Delete))
	   .Ключ(K_DELETE);
	bar.добавь(iscursor, AK_DUPLICATESEL, LayImg::Duplicate(), THISBACK(Duplicate));
	bar.добавь(iscursor, AK_MATRIXDUPLICATE, THISBACK(Matrix));
	bar.добавь(islayout, "Выбрать все", CtrlImg::select_all(), THISBACK(выбериВсе))
	   .Ключ(K_CTRL_A);
	bar.добавь(islayout, AK_VISGEN, LayImg::Members(), THISBACK(VisGen));
	bar.добавь(islayout, AK_FINDSOURCE, IdeCommonImg::Cpp(), THISBACK(GotoUsing));
	bar.Separator();
	bar.добавь(islayout && CurrentLayout().IsUndo(), "Undo", CtrlImg::undo(), THISBACK(Undo))
	   .Ключ(K_ALT_BACKSPACE)
	   .Ключ(K_CTRL_Z);
	bar.добавь(islayout && CurrentLayout().IsRedo(), "Redo", CtrlImg::redo(), THISBACK(Redo))
	   .Ключ(K_SHIFT|K_ALT_BACKSPACE)
	   .Ключ(K_SHIFT_CTRL_Z);
	bar.MenuSeparator();
	bar.добавь(iscursor, AK_HIDESELECTION, THISBACK1(ShowSelection, false))
		.Ключ(K_CTRL_SUBTRACT);
	bar.добавь(iscursor, AK_SHOWSELECTION, THISBACK1(ShowSelection, true))
		.Ключ(K_CTRL_ADD);
}

void LayDes::MoveBar(Бар& bar)
{
	bool iscursor = !пусто_ли(currentlayout) && cursor.дайСчёт();
	bar.добавь(iscursor, AK_MOVEUP, LayImg::MoveUp(), THISBACK(MoveUp));
	bar.добавь(iscursor, AK_MOVEDOWN, LayImg::MoveDown(), THISBACK(MoveDown));
	bar.добавь(cursor.дайСчёт() >= 2, AK_SORTITEMS, LayImg::SortItems(), THISBACK(SortItems));
}

void LayDes::AlignBar(Бар& bar)
{
	bool islayout = !пусто_ли(currentlayout);
	bool iscursor = islayout && cursor.дайСчёт();
	bool группа = islayout && cursor.дайСчёт() > 1;
	bar.добавь(iscursor, AK_HCENTERLAY, LayImg::HorzCenter(), THISBACK1(Align, A_HORZCENTER));
	bar.добавь(iscursor, AK_VCENTERLAY, LayImg::VertCenter(), THISBACK1(Align, A_VERTCENTER));
	bar.Separator();
	bar.добавь(группа, AK_LEFT, LayImg::AlignLeft(), THISBACK1(Align, A_LEFT));
	bar.добавь(группа, AK_HCENTER, LayImg::AlignHCenter(), THISBACK1(Align, A_HCENTER));
	bar.добавь(группа, AK_RIGHT, LayImg::AlignRight(), THISBACK1(Align, A_RIGHT));
	bar.Separator();
	bar.добавь(группа, AK_TOP, LayImg::AlignTop(), THISBACK1(Align, A_TOP));
	bar.добавь(группа, AK_VCENTER, LayImg::AlignVCenter(), THISBACK1(Align, A_VCENTER));
	bar.добавь(группа, AK_BOTTOM, LayImg::AlignBottom(), THISBACK1(Align, A_BOTTOM));
	bar.добавь(cursor.дайСчёт(), AK_LABELS, LayImg::AlignLbl(), THISBACK1(Align, A_LABEL));
}

void LayDes::SizeBar(Бар& bar)
{
	bool islayout = !пусто_ли(currentlayout);
	bool iscursor = islayout && cursor.дайСчёт();
	bool группа = islayout && cursor.дайСчёт() > 1;
	bar.добавь(группа, AK_SAMEWIDTH, LayImg::SameWidth(), THISBACK1(Align, A_SAMEWIDTH));
	bar.добавь(группа, AK_SAMEHEIGHT, LayImg::SameHeight(), THISBACK1(Align, A_SAMEHEIGHT));
	bar.добавь(группа, AK_SAMESIZE, LayImg::SameSize(), THISBACK1(Align, A_SAMESIZE));
	bar.Separator();
	bar.добавь(iscursor, AK_MINIMALWIDTH, LayImg::MinWidth(), THISBACK1(Align, A_MINWIDTH));
	bar.добавь(iscursor, AK_MINIMALHEIGHT, LayImg::MinHeight(), THISBACK1(Align, A_MINHEIGHT));
}

void LayDes::SpringBar(Бар& bar)
{
	bool islayout = !пусто_ли(currentlayout);
	bool iscursor = islayout && cursor.дайСчёт();
	int va = -1;
	int ha = -1;
	if(!пусто_ли(currentlayout) && cursor.дайСчёт()) {
		LayoutData& l = CurrentLayout();
		Ктрл::ПозЛога p = l.элт[cursor.верх()].pos;
		ha = p.x.дайЛин();
		va = p.y.дайЛин();
		for(int i = 0; i < cursor.дайСчёт() - 1; i++) {
			p = l.элт[cursor[i]].pos;
			if(p.x.дайЛин() != ha)
				ha = -1;
			if(p.y.дайЛин() != va)
				va = -1;
		}
	}
	bar.добавь(iscursor, AK_SPRINGLEFT, LayImg::SpringLeft(),
	       THISBACK1(SetSprings, MAKELONG(Ктрл::LEFT, -1)))
		.Check(ha == Ктрл::LEFT);
	bar.добавь(iscursor, AK_SPRINGRIGHT, LayImg::SpringRight(),
	       THISBACK1(SetSprings, MAKELONG(Ктрл::RIGHT, -1)))
		.Check(ha == Ктрл::RIGHT);
	bar.добавь(iscursor, AK_SPRINGHSIZE, LayImg::SpringHSize(),
	       THISBACK1(SetSprings, MAKELONG(Ктрл::SIZE, -1)))
		.Check(ha == Ктрл::SIZE);
	bar.добавь(iscursor, AK_SPRINGHCENTER, LayImg::SpringHCenter(),
	       THISBACK1(SetSprings, MAKELONG(Ктрл::CENTER, -1)))
		.Check(ha == Ктрл::CENTER);
	bar.Separator();
	bar.добавь(iscursor, AK_SPRINGTOP, LayImg::SpringTop(),
	       THISBACK1(SetSprings, MAKELONG(-1, Ктрл::TOP)))
		.Check(va == Ктрл::TOP);
	bar.добавь(iscursor, AK_SPRINGBOTTOM, LayImg::SpringBottom(),
	       THISBACK1(SetSprings, MAKELONG(-1, Ктрл::BOTTOM)))
		.Check(va == Ктрл::BOTTOM);
	bar.добавь(iscursor, AK_SPRINGVSIZE, LayImg::SpringVSize(),
	       THISBACK1(SetSprings, MAKELONG(-1, Ктрл::SIZE)))
		.Check(va == Ктрл::SIZE);
	bar.добавь(iscursor, AK_SPRINGVCENTER, LayImg::SpringVCenter(),
	       THISBACK1(SetSprings, MAKELONG(-1, Ктрл::CENTER)))
		.Check(va == Ктрл::CENTER);
	bar.Separator();
	bar.добавь(iscursor, AK_AUTOSPRINGS, LayImg::SpringAuto(),
	       THISBACK1(SetSprings, MAKELONG(AUTOSPRING, 0)));
	bar.добавь(iscursor, AK_TOPLEFTSPRINGS, LayImg::SpringTopLeft(),
	       THISBACK1(SetSprings, MAKELONG(Ктрл::LEFT, Ктрл::TOP)));
}

void LayDes::ToggleGrid()
{
	usegrid = !usegrid;
	SetBar();
}

void LayDes::ToggleMinSize()
{
	ignoreminsize = !ignoreminsize;
	SetBar();
}

void LayDes::ToggleSizeSpring()
{
	sizespring = !sizespring;
	SetBar();
}

void LayDes::Settings()
{
	setting.выполни();
	освежи();
	SyncItems();
}

void LayDes::GotoUsing()
{
	if(пусто_ли(currentlayout))
		return;
	
	Ткст lid = "With" + CurrentLayout().имя;
	const РОбласть& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.дайСчёт(); i++) { // find lowest file time
		const Пакет& pk = wspc.дайПакет(i);
		Ткст n = wspc[i];
		for(int i = 0; i < pk.дайСчёт(); i++) {
			Ткст path = SourcePath(n, pk.file[i]);
			if(IsCPPFile(path) || IsHFile(path)) {
				if(HasCPPFileKeyword(path, lid)) {
					IdeGotoFileAndId(path, lid);
					return;
				}
			}
		}
	}
	Exclamation("Не найдено кода, использующего эту выкладку.");
}

void LayDes::OptionBar(Бар& bar)
{
	bar.добавь("Использовать сетку", LayImg::Grid(), THISBACK(ToggleGrid))
	   .Check(usegrid);
	bar.добавь("Игнорировать мин.разер", LayImg::MinSize(), THISBACK(ToggleMinSize))
	   .Check(ignoreminsize);
	bar.добавь("Resize with springs", LayImg::SizeSpring(), THISBACK(ToggleSizeSpring))
	   .Check(sizespring);
	bar.добавь("Настройки..", THISBACK(Settings));
}

void LayDes::HelpBar(Бар& bar)
{
	bar.добавь("Узнать дополнительно о дизайнере выкладок..", IdeCommonImg::Help(),
		[=] { запустиВебБраузер("https://www.ultimatepp.org/app$ide$LayoutDes_en-us.html"); });
}

void LayDes::MainToolBar(Бар& bar)
{
	EditBar(bar);
	bar.Separator();
	MoveBar(bar);
	bar.Separator();
	OptionBar(bar);
	bar.Separator();
	bar.добавь(status, INT_MAX);
	bar.Break();
	AlignBar(bar);
	bar.Separator();
	SizeBar(bar);
	bar.Separator();
	SpringBar(bar);
	bar.GapRight();
	bar.Separator();
	HelpBar(bar);
}

void LayDes::ItemBar(Бар& bar)
{
	bar.добавь("Позиция", THISBACK(MoveBar));
	bar.добавь("Раскладка", THISBACK(AlignBar));
	bar.добавь("Размер", THISBACK(SizeBar));
	bar.добавь("Спринги", THISBACK(SpringBar));
}

void LayDes::MainMenuBar(Бар& bar)
{
	bar.добавь("Правка", THISBACK(EditBar));
	bar.добавь("Элемент", THISBACK(ItemBar));
	bar.добавь("Опции", THISBACK(OptionBar));
}

void LayDes::EditMenu(Бар& bar)
{
	EditBar(bar);
	bar.Separator();
	bar.добавь("Элемент", THISBACK(ItemBar));
	bar.Separator();
	OptionBar(bar);
}

void LayDes::SetBar()
{
	LTIMING("SetBar");
	toolbar.уст(THISBACK(MainToolBar));
}

class CVFrame : public КтрлФрейм {
	virtual void выложиФрейм(Прям& r) { r.bottom -= 4; r.left += 2; r.right -= 2; }
	virtual void рисуйФрейм(Draw& w, const Прям& r) { w.DrawRect(r, SColorFace); }
	virtual void добавьРазмФрейма(Размер& sz) { sz.cy += 4; sz.cx += 4; }
};

void LayDes::сериализуй(Поток& s)
{
	int version = 1;
	s / version;
	s % setting.gridx % setting.gridy;
	s % setting.paintgrid % setting.showicons;
	s % ignoreminsize % usegrid;
	if(version >= 1)
		s % sizespring;
	s % lsplit % isplit % rsplit;
	элт.сериализуйЗаг(s);
	SetBar();
}

int TypeFilter(int c)
{
	return iscid(c) || c == '<' || c == '>' || c == ':' || c == ',' ? c : 0;
}

int VariableFilter(int c)
{
	return iscid(c) || c == '.' || c == '[' || c == ']' ? c : 0;
}

bool LayDes::горячаяКлав(dword ключ)
{
	return БарМеню::скан(list.WhenBar, ключ) ||
	       БарМеню::скан(элт.WhenBar, ключ) ||
	       СтатичПрям::горячаяКлав(ключ);
}

LayDes::LayDes()
{
	charset = CHARSET_UTF8;

	currentlayout = Null;
	draghandle = -1;

	usegrid = true;
	ignoreminsize = false;
	sizespring = false;

	SetBar();

	status.устШирину(400);
	status.NoTransparent();

	km.d = this;
	km.добавь(lsplit.SizePos());
	lsplit.гориз(rsplit, *this).устПоз(2000);
	rsplit.верт(layouts, isplit);
	int cy = EditString::GetStdHeight();
	layouts.добавь(search.HSizePos().TopPos(0, cy));
	layouts.добавь(list.HSizePos().VSizePos(cy, 0));
	rsplit.устПоз(1000);
	isplit.верт(элт, property);
	twsplit.устВысоту(EditField::GetStdHeight() + 4);
	twsplit.гориз(тип, variable);
	twsplit.устФрейм(Single<CVFrame>());
	property.добавьФрейм(twsplit);
	property.добавьФрейм(фреймТонкийИнсет());
	property.AfterCreate();
	тип <<= THISBACK(TypeEdit);
	variable <<= THISBACK(VariableEdit);
	тип.откл();
	variable.откл();
	тип.устФильтр(TypeFilter);
	variable.устФильтр(VariableFilter);

	элт.добавьКолонку("Тип", 20).Margin(0);
	элт.добавьКолонку("Переменная / ярлык", 10).Margin(0);
	элт.ПриЛевКлике = THISBACK(ItemClick);
	элт.NoWantFocus();
	элт.WhenBar = THISBACK(MoveBar);

	элт.WhenDrag = THISBACK(тяни);
	элт.WhenDropInsert = THISBACK(вставьТиБ);

	устФрейм(toolbar);
	добавьФрейм(фреймИнсет());
	добавьФрейм(sb);

	sb.ПриПромоте = THISBACK(промотай);

	list.NoHeader().NoGrid();
	list.добавьКлюч();
	list.добавьКолонку();
	list.WhenCursor = THISBACK(LayoutCursor);
	list.WhenBar = THISBACK(LayoutMenu);
	list.WhenLeftDouble = THISBACK(RenameLayout);
	list.NoWantFocus();
	
	list.WhenDrag = THISBACK(DragLayout);
	list.WhenDropInsert = THISBACK(DnDInsertLayout);
	
	search.NullText("Поиск (Ктрл+F)");
	search <<= THISBACK(ищи);
	search.устФильтр(CharFilterToUpper);

	NoWantFocus();
	элт.NoWantFocus();

	BackPaint();

	SyncUsc();

	CtrlLayoutOKCancel(matrix, "Дубликат матрицы");
	matrix.nx.минмакс(1, 32);
	matrix.nx <<= 2;
	matrix.dx.минмакс(1, 32);
	matrix.dx <<= 0;
	matrix.ny.минмакс(1, 32);
	matrix.ny <<= 1;
	matrix.dy.минмакс(1, 32);
	matrix.dy <<= 0;

	CtrlLayoutOKCancel(setting, "Настройки");
	setting.gridx.минмакс(1, 32);
	setting.gridx <<= 4;
	setting.gridy.минмакс(1, 32);
	setting.gridy <<= 4;
}

LayDesigner *CreateLayDesigner(const char *filename, byte charset, const char *cfgname)
{
	LayDesigner *q = new LayDesigner();
	грузиИзГлоба(*q, "laydes-ctrl");
	if(q->грузи(filename, charset))
		return q;
	delete q;
	return NULL;
}

void LayUscClean();
bool LayUscParse(СиПарсер& p);
void SerializeLayEditPos(Поток& s);

bool IsLayFile(const char *path)
{
	return впроп(дайРасшф(path)) == ".lay";
}

struct LayDesModule : public МодульИСР {
	virtual Ткст       GetID() { return "LayDesModule"; }
	virtual void CleanUsc() {
		LayUscClean();
	}
	virtual bool ParseUsc(СиПарсер& p) {
		return LayUscParse(p);
	}
	virtual Рисунок FileIcon(const char *path) {
		return IsLayFile(path) ? LayImg::Layout() : Null;
	}
	virtual IdeDesigner *CreateDesigner(const char *path, byte cs) {
		if(IsLayFile(path)) {
			LayDesigner *d = CreateLayDesigner(path, cs, "laydes-ctrl");
			return d;
		}
		return NULL;
	}
	virtual void сериализуй(Поток& s) {
		int version = 0;
		s / version;
		SerializeLayEditPos(s);
	}

	virtual ~LayDesModule() {}
};

void RegisterLayDes()
{
	регМодульИСР(Single<LayDesModule>());
}

ИНИЦБЛОК {
	региструйГлобКонфиг("laydes-ctrl");
}
