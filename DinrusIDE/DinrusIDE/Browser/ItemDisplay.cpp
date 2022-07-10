#include "Browser.h"

void PaintText(Draw& w, int& x, int y, const char *text, const Вектор<ItemTextPart>& n,
           int starti, int count, bool focuscursor, Цвет _ink, bool italic)
{
	static int maxascent = MaxAscent(BrowserFont());
	for(int i = starti; i < count; i++) {
		const ItemTextPart& p = n[i];
		Шрифт f = BrowserFont();
		Цвет ink = SColorText;
		switch(p.тип) {
		case ITEM_PNAME:
			f.Bold();
		case ITEM_NUMBER:
			ink = SRed();
			break;
		case ITEM_TNAME:
			ink = SGreen();
		case ITEM_NAME:
			f.Bold();
			break;
		case ITEM_UPP:
			ink = SCyan();
			break;
		case ITEM_CPP_TYPE:
		case ITEM_CPP:
		case ITEM_SIGN:
			ink = SLtBlue();
			break;
		}
		if(italic)
			f.Italic();
		Размер fsz = дайРазмТекста(text + p.pos, f, p.len);
		w.DrawText(x, y + maxascent - f.GetAscent(), text + p.pos,
		           f, focuscursor ? _ink : ink, p.len);
		x += fsz.cx;
	}
}

void PaintText(Draw& w, int& x, int y, const CppItemInfo& m, const Вектор<ItemTextPart>& n,
           int starti, int count, bool focuscursor, Цвет _ink)
{
	PaintText(w, x, y, ~m.natural, n, starti, count, focuscursor, _ink, m.overed);
}

void PaintCppItemImage(Draw& w, int& x, int ry, int access, int kind, bool focuscursor)
{
	Рисунок img = decode(access, PROTECTED, BrowserImg::mprotected(),
	                           PRIVATE, BrowserImg::mprivate(),
	                           WITHBODY, BrowserImg::impl(),
	                           Рисунок());
	if(!пусто_ли(img))
		w.DrawImage(x, ry - img.дайВысоту() / 2, DPI(img));
	x += Zx(3);
	img = BrowserImg::unknown();
	Рисунок bk;
	switch(kind) {
	case FUNCTIONTEMPLATE:
		bk = BrowserImg::template_function();
	case FUNCTION:
		img = BrowserImg::function();
		break;
	case INSTANCEFUNCTIONTEMPLATE:
		bk = BrowserImg::template_function();
	case INSTANCEFUNCTION:
		img = BrowserImg::instance_function();
		break;
	case CLASSFUNCTIONTEMPLATE:
		bk = BrowserImg::template_function();
	case CLASSFUNCTION:
		img = BrowserImg::class_function();
		break;
	case STRUCTTEMPLATE:
		bk = BrowserImg::template_struct();
	case STRUCT:
		img = BrowserImg::type_struct();
		break;
	case INSTANCEVARIABLE:
		img = BrowserImg::instance_data();
		break;
	case CLASSVARIABLE:
		img = BrowserImg::class_data();
		break;
	case VARIABLE:
		img = BrowserImg::data();
		break;
	case ENUM:
		img = BrowserImg::type_enum();
		break;
	case INLINEFRIEND:
		img = BrowserImg::inline_friend();
		break;
	case TYPEDEF:
		img = BrowserImg::type_def();
		break;
	case CONSTRUCTOR:
		img = BrowserImg::constructor();
		break;
	case DESTRUCTOR:
		img = BrowserImg::destructor();
		break;
	case MACRO:
		img = BrowserImg::macro();
		break;
	case FLAGTEST:
		img = BrowserImg::flagtest();
		break;
	case FRIENDCLASS:
		img = BrowserImg::friend_class();
		break;
	case KIND_INCLUDEFILE:
		img = IdeCommonImg::Header();
		break;
	case KIND_INCLUDEFILE_ANY:
		img = CtrlImg::File();
		break;
	case KIND_INCLUDEFOLDER:
		img = CtrlImg::Dir();
		break;
	}
	
	img = DPI(img);
	bk = DPI(bk);

	int by = ry - bk.дайРазм().cy / 2;
	int iy = ry - img.дайРазм().cy / 2;

	if(focuscursor) {
		DrawHighlightImage(w, x, by, bk);
		w.DrawImage(x, iy, img);
	}
	else {
		w.DrawImage(x, by, bk);
		w.DrawImage(x, iy, img);
	}
}

int CppItemInfoDisplay::DoPaint(Draw& w, const Прям& r, const Значение& q,
	                            Цвет _ink, Цвет paper, dword style) const
{
	const CppItemInfo& m = ValueTo<CppItemInfo>(q);
	w.DrawRect(r, paper);
	bool focuscursor = (style & (FOCUS|CURSOR)) == (FOCUS|CURSOR) || (style & SELECT);
	if(пусто_ли(q)) return 0;
	int x = r.left;
	int ry = r.top + r.дайВысоту() / 2;
	PaintCppItemImage(w, x, ry, m.access, m.kind, focuscursor);

	if(m.inherited) {
		w.DrawImage(x + Zx(10), r.top, BrowserImg::inherited());
		for(int i = 1; i < min(m.inherited, 5); i++)
			w.DrawRect(x + Zx(10), r.top + Zy(7) + 2 * i, Zx(7), Zy(1), SColorText);
	}
	x += Zx(16);
	int y = ry - Draw::GetStdFontCy() / 2;
	int x0 = x;
	Вектор<ItemTextPart> n = ParseItemNatural(m);
	int starti = 0;
	if(namestart)
		for(int i = 0; i < n.дайСчёт(); i++)
			if(n[i].тип == ITEM_NAME) {
				starti = i;
				break;
			}
	PaintText(w, x, y, m, n, starti, n.дайСчёт(), focuscursor, _ink);
	if(starti) {
		const char *h = " : ";
		w.DrawText(x, y, h, BrowserFont(), SColorText);
		x += дайРазмТекста(h, BrowserFont()).cx;
	}
	PaintText(w, x, y, m, n, 0, starti, focuscursor, _ink);
	if(m.virt || m.over)
		w.DrawRect(x0, r.bottom - 2, x - x0, 1, m.over ? m.virt ? SLtRed() : SLtBlue() : SColorText());
	if(m.inherited && m.IsType())
		w.DrawRect(r.left, r.top, r.устШирину(), 1, SColorDisabled);

	if(showtopic) {
		Ткст k = MakeCodeRef(m.scope, m.qitem);
		int cnt = GetRefLinks(k).дайСчёт();
		if(cnt) {
			Размер sz = BrowserImg::Ref().дайРазм();
			int xx = r.right - sz.cx - 1;
			int yy = r.top + (r.устВысоту() - sz.cy) / 2;
			DrawHighlightImage(w, xx, yy, BrowserImg::Ref());
			if(cnt > 1) {
				Ткст txt = какТкст(cnt);
				Шрифт fnt = Arial(Ктрл::VertLayoutZoom(10)).Bold();
				Размер tsz = дайРазмТекста(txt, fnt);
				Точка p(xx + (sz.cx - tsz.cx) / 2, yy + (sz.cy - tsz.cy) / 2);
				for(int ax = -1; ax <= 1; ax++)
					for(int ay = -1; ay <= 1; ay++)
						w.DrawText(p.x + ax, p.y + ay, txt, fnt, SWhite());
				w.DrawText(p.x, p.y, txt, fnt, SBlue());
			}
			x += sz.cx + Zx(3);
		}
	}

	return x;
}

void CppItemInfoDisplay::рисуй(Draw& w, const Прям& r, const Значение& q,
                                  Цвет _ink, Цвет paper, dword style) const {
	DoPaint(w, r, q, _ink, paper, style);
}

Размер CppItemInfoDisplay::дайСтдРазм(const Значение& q) const
{
	NilDraw w;
	return Размер(DoPaint(w, Прям(0, 0, INT_MAX, INT_MAX), q, Null, Null, 0),
	            max(Zy(16), BrowserFont().Info().дайВысоту()));
}
