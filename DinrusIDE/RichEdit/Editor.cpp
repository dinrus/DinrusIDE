#include "RichEdit.h"

namespace РНЦП {

#define TFILE <RichEdit/RichEdit.t>
#include <Core/t.h>

bool FontHeight::Ключ(dword ключ, int count)
{
	if(ключ == K_ENTER) {
		if(!ошибка_ли(дайДанные()))
			WhenSelect();
		return true;
	}
	return WithDropChoice<EditDouble>::Ключ(ключ, count);
}

double RichEdit::DotToPt(int dt)
{
	return 7200 * minmax(dt, 8, 8000) / 600 / 10 / 10.0;
}

int RichEdit::PtToDot(double pt)
{
	return int((600 * pt + 71) / 72);
}

struct EditPageDraw : public PageDraw {
	virtual Draw& Page(int _page);

	Draw&              w;
	int                page;
	int                x, y;
	Размер               size;

	EditPageDraw(Draw& w) : w(w) { w.старт(); w.старт(); page = -1; }
	~EditPageDraw() { w.стоп(); w.стоп(); }
};

Draw& EditPageDraw::Page(int _page)
{
	if(page == _page) return w;
	page = _page;
	w.стоп();
	w.стоп();
	if(size.cy < INT_MAX)
		w.Clipoff(0, y + (size.cy + 3) * page + 2, 9999, size.cy);
	else
		w.смещение(0, y + 2);
	w.смещение(x, 0);
	return w;
}

Прям RichEdit::GetTextRect() const
{
	Размер sz = дайРазм();
	if(sz.cy < Zx(16))
		sz.cy = Zx(16);
	if(sz.cx < Zx(80))
		return RectC(0, 0, Zx(48), max(sz.cy, Zy(16)));
	int cx = zoom * (sz.cx - 2 * viewborder) / 100;
	return RectC((sz.cx - cx) / 2, 0, cx, sz.cy);
}

Zoom RichEdit::GetZoom() const
{
	return Zoom(GetTextRect().устШирину(), pagesz.cx);
}

Размер RichEdit::GetZoomedPage() const
{
	return Размер(GetTextRect().устШирину(), pagesz.cy == INT_MAX ? INT_MAX / 2 : GetZoom() * pagesz.cy);
}

int  RichEdit::GetPosY(PageY py) const
{
	return py.page * (GetZoomedPage().cy + 3) + py.y * GetZoom() + 2;
}

PageY RichEdit::GetPageY(int y) const
{
	PageY py;
	int q = GetZoomedPage().cy + 3;
	py.page = y / q;
	py.y = (y % q - 2) / GetZoom();
	return py;
}

static void sPaintHotSpot(Draw& w, int x, int y)
{
	w.DrawRect(x, y, DPI(8), DPI(8), светлоКрасный);
	DrawFrame(w, x, y, DPI(8), DPI(8), SColorText);
}

void RichEdit::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	p_size = sz;
	Прям tr = GetTextRect();
	Zoom zoom = GetZoom();
	w.DrawRect(sz, белый);
	PageY py = text.дайВысоту(pagesz);
	{
		EditPageDraw pw(w);
		pw.x = tr.left;
		pw.y = -sb;
		pw.size = GetZoomedPage();
		if(pagesz.cy == INT_MAX) {
			pw.size.cy = INT_MAX;
			if(viewborder)
				DrawFrame(w, tr.left - 1, (int)sb ? -1 : 0, pw.size.cx + 4, 9999, SColorShadow);
		}
		else
		if(viewborder)
			for(int i = 0; i <= py.page; i++)
				DrawFrame(w, tr.left - 1, i * (pw.size.cy + 3) + 1 - sb,
				          pw.size.cx + 4, pw.size.cy + 2, SColorShadow);
		PaintInfo pi = paint_info;
		pi.context = context;
		pi.zoom = zoom;
		pi.top = GetPageY(sb);
		pi.bottom = GetPageY(sb + sz.cy);
		pi.usecache = true;
		pi.sizetracking = sizetracking;
		pi.showcodes = showcodes;
		pi.showlabels = !пусто_ли(showcodes) && viewborder >= 16;
		pi.hyperlink = светлоСиний; // because we have white paper even in dark mode
		
		if(spellcheck)
			pi.spellingchecker = SpellParagraph;
		if(выделение_ли()) {
			if(tablesel) {
				pi.tablesel = tablesel;
				pi.cells = cells;
			}
			else {
				pi.sell = begtabsel ? -1 : min(cursor, anchor);
				pi.selh = max(cursor, anchor);
			}
		}
		text.рисуй(pw, pagesz, pi);
	}
	w.DrawRect(tr.left, GetPosY(py) - sb, 20, 3, showcodes);
	if(objectpos >= 0) {
		Прям r = objectrect;
		r.смещение(tr.left, -sb);
		DrawFrame(w, r, чёрный());
		r.дефлируй(DPI(1));
		DrawFatFrame(w, r, белый(), DPI(2));
		r.дефлируй(DPI(2));
		DrawFrame(w, r, чёрный());
		r.дефлируй(DPI(1));
		sPaintHotSpot(w, r.left + r.устШирину() / 2 - DPI(3), r.bottom - DPI(7));
		sPaintHotSpot(w, r.right - DPI(7), r.bottom - DPI(7));
		sPaintHotSpot(w, r.right - DPI(7), r.top + r.устВысоту() / 2 - DPI(3));
		w.Clip(r);
		w.стоп();
	}
	else
	if(paintcarect)
		w.DrawRect(GetCaretRect(), InvertColor);
	if(!пусто_ли(dropcaret))
		DrawTiles(w, dropcaret.вертСмещенец(-sb), CtrlImg::checkers());
	scroller.уст(sb);
}

int RichEdit::GetHotSpot(Точка p) const
{
	if(objectpos < 0) return -1;
	Прям r = objectrect;
	r.смещение(GetTextRect().left, -sb);
	r.дефлируй(DPI(4), DPI(4));
	if(RectC(r.right - DPI(7), r.bottom - DPI(7), DPI(8), DPI(12)).содержит(p))
		return 0;
	if(RectC(r.left + r.устШирину() / 2 - DPI(3), r.bottom - DPI(7), DPI(12), DPI(12)).содержит(p))
		return 1;
	if(RectC(r.right - DPI(7), r.top + r.устВысоту() / 2 - DPI(3), DPI(12), DPI(8)).содержит(p))
		return 2;
	return -1;
}

void RichEdit::SetZsc()
{
	zsc = (int)sb / GetZoom();
}

void RichEdit::SetSb()
{
	sb.устВсего(GetPosY(text.дайВысоту(pagesz)) + 10);
}

void RichEdit::промотай()
{
	scroller.промотай(*this, дайРазм(), sb);
	поместиКаретку();
}

void RichEdit::EndSizeTracking()
{
	if(sizetracking) {
		sizetracking = false;
		освежи();
	}
}

void RichEdit::FixObjectRect()
{
	if(objectpos >= 0) {
		Прям r = GetObjectRect(objectpos);
		if(r != objectrect) {
			objectrect = r;
			освежи(objectrect);
		}
	}
}

RichEdit& RichEdit::Floating(double zoomlevel_)
{
	floating_zoom = zoomlevel_;
	RefreshLayoutDeep();
	return *this;
}

void RichEdit::Выкладка()
{
	Размер sz = GetTextRect().дайРазм();
	if(!пусто_ли(floating_zoom)) {
		Zoom m = GetRichTextStdScreenZoom();
		устСтраницу(Размер(int(1 / floating_zoom * m.d / m.m * sz.cx), INT_MAX));
	}
	sb.устСтраницу(sz.cy > 10 ? sz.cy - 4 : sz.cy);
	SetupRuler();
	SetSb();
	sb = zsc * GetZoom();
	поместиКаретку();
	if(дайРазм() != p_size) {
		sizetracking = true;
		KillSetTimeCallback(250, THISBACK(EndSizeTracking), TIMEID_ENDSIZETRACKING);
	}
	FixObjectRect();
}

Прям RichEdit::GetCaretRect(const RichCaret& pr) const
{
	Zoom zoom = GetZoom();
	Прям tr = GetTextRect();
	Прям r = RectC(pr.left * zoom + tr.left, GetPosY(pr) + (pr.lineascent - pr.caretascent) * zoom - sb,
	               overwrite && дайСим(cursor) != '\n' ? pr.устШирину() * zoom
	                         : (pr.caretascent + pr.caretdescent) * zoom > 20 ? 2 : 1,
	               (pr.caretascent + pr.caretdescent) * zoom);
	if(r.right > tr.right)
		return Прям(tr.right - r.устШирину(), r.top, tr.right, r.bottom);
	return r;
}

Прям RichEdit::GetCaretRect() const
{
	return GetCaretRect(text.дайКаретку(cursor, pagesz));
}

Прям RichEdit::поместиКаретку()
{
	Zoom zoom = GetZoom();
	Прям rr = Прям(zoom * cursorc.left, GetPosY(cursorc), zoom * cursorc.right,
	               GetPosY(PageY(cursorc.page, cursorc.bottom)));
	if(objectpos >= 0) {
		анулируйКаретку();
		return rr;
	}
	if(!пусто_ли(objectrect)) {
		objectrect = Null;
		освежи();
	}
	if(выделение_ли())
		анулируйКаретку();
	else
		устКаретку(GetCaretRect(cursorc));
	return rr;
}

void RichEdit::SetupRuler()
{
	Zoom zoom = GetZoom();
	static struct Tl {
		double grid;
		int    numbers;
		double numbermul;
		int    marks;
	}
	tl[] = {
		{ 25, 20, 25, 4 },
		{ 600 / 72 * 4, 9, 4, 1000 },
		{ 600 / 10, 10, 1 / 10.0, 5 },
		{ 600 / 25.4, 10, 1, 5 },
		{ 600 / 25.4, 10, 1 / 10.0, 5 },
	};
	const Tl& q = tl[unit];
	ruler.SetLayout(GetTextRect().left + zoom * cursorc.textpage.left, cursorc.textpage.устШирину(),
	                zoom, q.grid, q.numbers, q.numbermul, q.marks);
}

void RichEdit::SetupUnits()
{
	WithUnitLayout<ТопОкно> d;
	CtrlLayoutOKCancel(d, t_("Units"));
	d.accels <<= THISBACK(StyleKeys);
	for(int i = 1; i <= 10; i++)
		d.zoom.добавь(10 * i, фмт(t_("%d%% of width"), 10 * i));
	CtrlRetriever r;
	r(d.unit, unit)(d.showcodes, showcodes)(d.zoom, zoom);
	if(d.выполни() == IDOK) {
		r.Retrieve();
		освежи();
		FinishNF();
	}
}

void RichEdit::ZoomView(int d)
{
	zoom = clamp(zoom + d * 10, 10, 100);
	освежи();
	FinishNF();
}

int  RichEdit::GetNearestPos(int x, PageY py)
{
	int c = text.дайПоз(x, py, pagesz);
	Ткст dummy;
	RichPos p = text.GetRichPos(c);
	if(c >= text.дайДлину() - 1 || c < 0 || p.object || p.field
	   || p.table && (p.posincell == 0 || p.posincell == p.celllen))
		return c;
	Прям r1 = text.дайКаретку(c, pagesz);
	Прям r2 = text.дайКаретку(c + 1, pagesz);
	return r1.top == r2.top && x - r1.left > r2.left - x ? c + 1 : c;
}

int RichEdit::дайХ(int x)
{
	return (x - GetTextRect().left) / GetZoom();
}

int RichEdit::GetSnapX(int x)
{
	return дайХ(x) / 32 * 32;
}

void RichEdit::GetPagePoint(Точка p, PageY& py, int& x)
{
	py = GetPageY(p.y + sb);
	x = дайХ(p.x);
}

int  RichEdit::дайПозМыши(Точка p) {
	PageY py;
	int    x;
	GetPagePoint(p, py, x);
	return GetNearestPos(x, py);
}

Прям RichEdit::GetObjectRect(int pos) const {
	Zoom zoom = GetZoom();
	RichCaret pr = text.дайКаретку(pos, pagesz);
	Прям r = Прям(zoom * pr.left,
	              GetPosY(PageY(pr.page, pr.top + pr.lineascent - pr.objectcy + pr.objectyd)),
	              zoom * pr.right,
	              GetPosY(PageY(pr.page, pr.top + pr.lineascent + pr.objectyd)));
	return r;
}

bool RichEdit::Print()
{
	text.SetFooter(footer);
	text.PrintNoLinks(nolinks);
	return РНЦП::Print(text, pagesz, cursorc.page);
}

struct DisplayFont : public Дисплей {
	void рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const
	{
		Шрифт fnt;
		fnt.Face((int)q);
		fnt.устВысоту(r.устВысоту() - Zy(4));
		w.DrawRect(r, paper);
		w.DrawText(r.left, r.top + (r.устВысоту() - fnt.Info().дайВысоту()) / 2,
		           Шрифт::GetFaceName((int)q), fnt, ink);
	}
};

struct ValueDisplayFont : public Дисплей {
	void рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const
	{
		w.DrawRect(r, paper);
		w.DrawText(r.left, r.top + (r.устВысоту() - StdFont().Info().дайВысоту()) / 2,
		           Шрифт::GetFaceName((int)q), StdFont(), ink);
	}
};

void RichEdit::очисть()
{
	undo.очисть();
	redo.очисть();
	text.очисть();
	переустанов();
	RichPara h;
	h.формат.language = GetCurrentLanguage();
	text.конкат(h);
	освежи();
	финиш();
	ReadStyles();
	SetModify();
	modified = true;
	zsc = 0;
}

void RichEdit::SetupLanguage(Вектор<int>&& _lng)
{
	Вектор<int>& lng = const_cast<Вектор<int>&>(_lng);
	сортируй(lng);
	language.очистьСписок();
	for(int i = 0; i < lng.дайСчёт(); i++)
		language.добавь(lng[i], lng[i] ? LNGAsText(lng[i]) : Ткст(t_("None")));
}

void RichEdit::подбери(RichText pick_ t)
{
	очисть();
	text = pick(t);
	if(text.GetPartCount() == 0)
		text.конкат(RichPara());
	ReadStyles();
	EndSizeTracking();
	SetupLanguage(text.GetAllLanguages());
	Move(0);
	Update();
}

Значение RichEdit::дайДанные() const
{
	return AsQTF(text);
}

void  RichEdit::устДанные(const Значение& v)
{
	подбери(ParseQTF((Ткст)v, 0, context));
}

void  RichEdit::сериализуй(Поток& s)
{
	int version = 0;
	s / version;
	Ткст h;
	if(s.сохраняется())
		h = AsQTF(text);
	s % h;
	if(s.грузится())
		подбери(ParseQTF(h, 0, context));
}

int RichEdit::fh[] = {
	6, 7, 8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28, 36, 42, 48, 60, 72, 0
};

RichEdit& RichEdit::FontFaces(const Вектор<int>& ff)
{
	ffs <<= ff;
	face.очистьСписок();
	for(int i = 0; i < ff.дайСчёт(); i++)
		face.добавь(ff[i]);
	return *this;
}

void SetupFaceList(СписокБроса& face)
{
	face.ValueDisplay(Single<ValueDisplayFont>());
	face.устДисплей(Single<DisplayFont>());
}

void RichEdit::SpellCheck()
{
	spellcheck = !spellcheck;
	освежи();
	RefreshBar();
}

void RichEdit::сериализуйНастройки(Поток& s)
{
	int version = 3;
	s / version;
	s % unit;
	s % showcodes;
	if(version >= 1)
		s % zoom;
	s % spellcheck;
	s % findreplace.find;
	findreplace.find.SerializeList(s);
	s % findreplace.replace;
	findreplace.replace.SerializeList(s);
	s % findreplace.wholeword;
	s % findreplace.ignorecase;
	RefreshBar();
	imagefs.сериализуй(s);
	if(version >= 3)
		for(int i = 0; i < 20; i++) {
			StyleKey& k = stylekey[i];
			s % k.styleid % k.stylename % k.face % k.height % k.ink % k.paper;
		}
}

void RichEdit::переустанов()
{
	undoserial = 0;
	incundoserial = false;

	objectpos = -1;
	objectrect = Null;
	sizetracking = true;

	anchor = cursor = 0;
	gx = 0;
	oselh = osell = 0;

	RichPara::фмт pmt;
	formatinfo.уст(pmt);

	tabmove.table = 0;
}

RichEdit::UndoInfo RichEdit::PickUndoInfo()
{
	UndoInfo f;
	f.undoserial = undoserial;
	f.undo = pick(undo);
	f.redo = pick(redo);
	очисть();
	return f;
}

void RichEdit::SetPickUndoInfo(UndoInfo pick_ f)
{
	undoserial = f.undoserial;
	incundoserial = true;
	undo = pick(f.undo);
	redo = pick(f.redo);
	финиш();
}

void RichEdit::PosInfo::сериализуй(Поток& s)
{
	int version = 2;
	s / version;
	s % cursor % anchor % zsc % begtabsel;
	if(version == 0)
		zsc = 0;
}

RichEdit::PosInfo RichEdit::GetPosInfo() const
{
	PosInfo f;
	f.cursor = cursor;
	f.anchor = anchor;
	f.begtabsel = begtabsel;
	f.zsc = zsc;
	return f;
}

void RichEdit::SetPosInfo(const PosInfo& f)
{
	int l = text.дайДлину();
	cursor = min(l, f.cursor);
	anchor = min(l, f.anchor);
	begtabsel = f.begtabsel;
	if(begtabsel)
		anchor = 0;
	финиш();
	zsc = f.zsc;
	Выкладка();
}

void RichEdit::DoRefreshBar()
{
	WhenRefreshBar();
}

void RichEdit::RefreshBar()
{
	глушиОбрвызВремени(TIMEID_REFRESHBAR);
	устОбрвызВремени(0, THISBACK(DoRefreshBar), TIMEID_REFRESHBAR);
}

void StdLinkDlg(Ткст& s, ШТкст&)
{
	редактируйТекст(s, t_("Hyperlink"), t_("Hyperlink"), CharFilterAscii128, 1000);
}

void StdLabelDlg(Ткст& s)
{
	редактируйТекст(s, t_("Paragraph label"), t_("Надпись"), CharFilterAscii128, 1000);
}

void StdIndexEntryDlg(Ткст& s)
{
	редактируйТекст(s, t_("Индекс Entry"), t_("Индекс entry"), CharFilterAscii128, 1000);
}

RichEdit::RichEdit()
{
	floating_zoom = Null;

	Unicode();
	BackPaint();

	viewborder = Zx(16);

	face.NoWantFocus();
	height.NoWantFocus();
	style.NoWantFocus();
	language.NoWantFocus();

	setstyle = &style.InsertButton(0).SetMonoImage(CtrlImg::smallleft()).Подсказка(t_("сохрани as style"));
	setstyle->WhenClick = THISBACK(устСтиль);
	style.InsertButton(0).SetMonoImage(RichEditImg::ManageStyles()).Подсказка(t_("Стиль manager"))
	     .WhenClick = THISBACK(Styles);
	style.Подсказка(t_("Стиль"));

	style <<= THISBACK(Стиль);
	
	WhenBar = THISBACK(StdBar);

	pagesz = Размер(3968, 6074);
	unit = UNIT_POINT;
	zoom = 100;
	очисть();

	context = NULL;
	
	nolinks = false;

	showcodes = светлоСиний;
	spellcheck = true;

	overwrite = false;

	sb.ПриПромоте = THISBACK(промотай);
	sb.устСтроку(16);
	Выкладка();
	SetSb();

	adjustunits.Рисунок(RichEditImg::AdjustUnits());
	adjustunits <<= THISBACK(SetupUnits);
	ruler.добавь(adjustunits.RightPosZ(4, 16).VSizePosZ(2, 2));

	undosteps = 500;

	добавьФрейм(ViewFrame());
	добавьФрейм(ruler);
	добавьФрейм(sb);
	RefreshBar();

	ruler.WhenBeginTrack = THISBACK(BeginRulerTrack);
	ruler.WhenTrack = THISBACK(RulerTrack);
	ruler.WhenEndTrack = THISBACK(ReadFormat);
	ruler.WhenLeftDown = THISBACK(AddTab);
	ruler.WhenRightDown = THISBACK(TabMenu);

	SetupFaceList(face);
	face <<= THISBACK(SetFace);
	face.Подсказка(t_("Шрифт face"));
	Вектор<int> ff;
	ff.добавь(Шрифт::ARIAL);
	ff.добавь(Шрифт::ROMAN);
	ff.добавь(Шрифт::COURIER);
	FontFaces(ff);

	language <<= THISBACK(SetLanguage);
	language.Подсказка(t_("Language"));
	language.WhenClick = THISBACK(Language);
	language.добавь(0, t_("None"));

	for(int i = 0; fh[i]; i++)
		height.добавьСписок(fh[i]);
	height.WhenSelect = THISBACK(SetHeight);
	height.Подсказка(t_("Шрифт height"));

	hyperlink <<= THISBACK(Hyperlink);
	hyperlink.NoWantFocus();
	label <<= THISBACK(Надпись);
	indexentry << THISBACK(IndexEntry);
	indexentry.NoWantFocus();

	gotolabel.SetMonoImage(RichEditImg::GoTo());
	label.добавьФрейм(gotolabel);
	gotolabel.Подсказка(t_("иди to label"));
	gotolabel <<= THISBACK(GotoLbl);
	gotolabel.NoWantFocus();

	gotoentry.SetMonoImage(RichEditImg::GoTo());
	indexentry.добавьФрейм(gotoentry);
	gotoentry.Подсказка(t_("иди to Индекс entry"));
	gotoentry <<= THISBACK(GotoEntry);

	gototable.Normal();
	gototable.добавьИндекс();
	gototable.добавьИндекс();

	gototable.WhenSelect = THISBACK(идиК);

	ink.ColorImage(RichEditImg::InkColor())
	   .NullImage(RichEditImg::NullInkColor())
	   .StaticImage(RichEditImg::ColorA());
	ink.неПусто();
	paper.ColorImage(RichEditImg::PaperColor())
	     .NullImage(RichEditImg::NullPaperColor())
	     .StaticImage(RichEditImg::ColorA());
	ink <<= THISBACK(устЧернила);
	ink.Подсказка(t_("устТекст color"));
	paper <<= THISBACK(SetPaper);
	paper.Подсказка(t_("фон color"));

	ReadStyles();

	paintcarect = false;

	CtrlLayoutOKCancel(findreplace, t_("найди / замени"));
	findreplace.cancel <<= callback(&findreplace, &ТопОкно::закрой);
	findreplace.ok <<= THISBACK(найди);
	findreplace.amend <<= THISBACK(замени);
	notfoundfw = found = false;
	findreplace.NoCenter();

	WhenHyperlink = callback(StdLinkDlg);
	WhenLabel = callback(StdLabelDlg);
	WhenIndexEntry = callback(StdIndexEntryDlg);

	p_size = Размер(-1, -1);

	useraction = modified = false;
	ClearModify();
	финиш();
	
	imagefs.Type("Images (*.png *.gif *.jpg *.bmp *.svg)", "*.png *.gif *.jpg *.bmp *.svg");
	
	singleline = false;
	
	clipzoom = Zoom(1, 1);
	
	bullet_indent = 150;
	
	persistent_findreplace = true;
	
	ignore_physical_size = false;
}

RichEdit::~RichEdit() {}

void RichEditWithToolBar::TheBar(Бар& bar)
{
	DefaultBar(bar, extended);
}

void RichEditWithToolBar::RefreshBar()
{
	toolbar.уст(THISBACK(TheBar));
}

void RichEdit::EvaluateFields()
{
	WhenStartEvaluating();
	text.EvaluateFields(vars);
	финиш();
}

RichEditWithToolBar::RichEditWithToolBar()
{
	вставьФрейм(0, toolbar);
	WhenRefreshBar = callback(this, &RichEditWithToolBar::RefreshBar);
	extended = true;
}

}
