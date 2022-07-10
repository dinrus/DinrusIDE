#include "CtrlLib.h"

namespace РНЦП {

#define LLOG(x)  // RLOG(x)

#define FIXED_COLORS
#define IMAGECLASS CtrlsImg // contains some default definitions
#define IMAGEFILE <CtrlLib/Ctrls.iml>
#include <Draw/iml_source.h>

Цвет AdjustColor(Цвет c, int adj)
{
	return Цвет(clamp(c.дайК() + adj, 0, 255),
	             clamp(c.дайЗ() + adj, 0, 255),
	             clamp(c.дайС() + adj, 0, 255));
};

Цвет FaceColor(int adj)
{
	return AdjustColor(SColorFace(), adj);
};

void SyntheticTab(int i, int roundness, Цвет ink)
{
	КтрлВкладка::Стиль& s = КтрлВкладка::дефСтиль().пиши();
	s.body = сделайКнопку(0, FaceColor(8), DPI(1), ink);
	Рисунок t = сделайКнопку(roundness, FaceColor(decode(i, CTRL_NORMAL, -20,
	                                                    CTRL_HOT, 2,
	                                                    CTRL_PRESSED, 8,
	                                                    -8)), DPI(1), ink,
	                                                    CORNER_TOP_LEFT|CORNER_TOP_RIGHT);
	s.first[i] = s.last[i] = s.both[i] = s.normal[i] = ChHot(Crop(t, 0, 0, t.дайШирину(), t.дайВысоту() - DPI(3)), DPI(3));
	s.margin = 0;
	s.sel = Прям(0, DPI(1), 0, DPI(1));
	s.extendleft = DPI(2);
	s.text_color[i] = SColorText();
}

void MakeDialogIcons()
{
	auto MakeCircularIcon = [](Цвет c, const byte *signc, int csz, double ssz = 0.6, Цвет sc = белый()) -> Рисунок
	{
		Ткст sign = ZDecompress(signc, csz);
		int sz = DPI(32);
		ImagePainter w(sz, sz);
		double r = 0.5 * sz;
		w.очисть(обнулиКЗСА());
		w.Circle(r, r, r).Fill(серый());
		w.Circle(r, r, r - DPI(1)).Fill(белый());
		w.Circle(r, r, r - DPI(2)).Fill(ТочкаПЗ(0.5 * r, 0.6 * r), смешай(белый(), c), r, c);
		
		ssz *= sz;
		ПрямПЗ sr = RectfC((sz - ssz) / 2, (sz - ssz) / 2, ssz, ssz);
		ПрямПЗ br = GetSVGPathBoundingBox(sign);
		double scale = sr.дайВысоту() / br.дайВысоту();
		w.Translate(sr.left + (sr.устШирину() - br.устШирину() * scale) / 2, sr.top);
		w.Scale(scale);
		w.Translate(-br.верхЛево());
		w.Path(sign).Stroke(2, чёрный()).Fill(sc);
		
		return w;
	};

	static const byte s_information[] = { 120,156,85,81,193,117,67,49,8,91,197,19,240,12,216,216,222,225,231,224,107,246,31,164,8,72,211,94,18,244,65,72,200,47,217,180,102,243,95,229,118,85,200,180,128,78,146,221,148,169,119,111,44,58,163,169,146,157,86,245,162,249,219,24,252,97,204,54,70,50,132,166,181,97,96,164,70,214,87,38,250,213,16,240,138,193,7,226,185,42,235,210,40,160,197,248,186,146,1,241,180,251,247,142,247,43,15,153,139,186,181,199,249,219,87,118,206,109,49,200,125,134,78,1,251,215,90,196,7,22,48,224,96,237,240,176,183,35,223,191,155,116,140,23,184,194,184,244,211,10,97,176,198,136,91,253,15,90,236,177,30,12,156,19,158,148,115,69,143,220,157,229,31,153,45,244,57,72,9,238,2,169,128,159,187,188,30,52,150,115,66,149,5,94,229,227,32,2,78,96,32,106,186,17,4,240,112,164,189,4,161,93,86,140,217,142,129,172,45,60,23,152,225,72,176,196,234,125,24,82,22,244,131,131,178,188,251,251,217,16,146,41,198,159,5,51,22,73,61,254,44,222,200,231,120,255,0,15,160,120,181 };
	CtrlImg::уст(CtrlImg::I_information, MakeCircularIcon(Цвет(85, 127, 200), s_information, sizeof(s_information), 0.6, Цвет(243, 255, 211)));
	static const byte s_question[] = { 120,156,77,84,201,145,32,33,12,75,165,35,160,240,193,21,196,60,248,110,254,129,172,36,96,119,62,93,184,13,178,44,11,126,172,149,105,223,88,197,226,219,54,74,235,223,180,82,235,103,179,244,248,102,20,235,72,172,210,215,55,91,9,251,238,26,91,235,191,196,210,9,129,172,90,124,34,49,9,181,140,95,194,174,111,249,57,209,137,187,162,100,126,38,64,172,215,64,34,9,178,146,187,204,153,62,235,109,246,63,161,114,88,86,176,157,37,38,15,163,194,16,142,19,121,247,226,141,133,177,177,169,84,101,114,55,130,204,89,86,126,103,57,74,237,239,119,242,76,39,75,54,191,136,109,235,27,131,187,81,30,13,13,81,37,73,20,29,78,116,83,215,163,114,211,246,96,173,190,120,206,215,255,245,142,70,130,39,200,74,53,239,137,204,50,218,133,74,84,50,214,0,131,221,140,204,198,100,191,77,233,233,228,183,91,146,195,236,44,123,214,232,77,77,156,96,50,0,212,156,159,213,166,76,54,158,180,170,78,210,78,63,149,140,118,104,192,102,65,102,96,57,28,65,215,169,112,154,194,76,106,69,45,19,167,220,75,176,207,41,120,111,36,127,3,236,160,123,94,164,113,33,192,80,13,72,244,15,20,1,174,69,144,39,2,114,138,20,13,108,188,17,106,249,160,250,6,54,28,2,138,144,111,168,7,207,131,174,128,114,223,90,4,60,129,143,55,9,69,73,229,206,151,228,217,23,0,105,46,12,14,238,242,94,6,246,193,156,244,171,75,60,131,35,209,10,7,216,37,13,83,40,66,134,240,1,78,81,26,202,187,216,255,6,107,10,5,121,33,101,116,161,87,185,142,242,66,87,171,242,69,160,34,247,137,238,134,13,232,75,217,7,51,129,172,11,166,117,36,68,243,220,141,179,198,205,154,246,18,83,86,74,217,3,150,160,87,82,6,24,50,92,72,73,152,86,204,58,65,224,40,176,133,236,199,93,188,53,148,12,14,78,25,245,156,208,122,187,46,216,77,152,252,223,184,149,175,2,64,70,215,85,254,245,92,252,249,129,226,236,177,155,74,186,182,223,136,83,72,5,178,134,235,229,232,149,37,92,255,172,173,131,190,52,228,118,108,176,168,184,181,206,169,237,243,4,89,211,76,94,16,154,255,139,212,12,78,17,61,23,7,186,189,234,49,202,249,26,1,235,60,151,253,210,72,249,234,250,132,55,100,138,60,31,36,92,74,224,162,47,146,57,129,204,91,95,20,186,60,118,238,21,47,10,169,229,147,89,232,131,219,105,141,118,105,4,175,151,104,232,249,226,147,64,215,52,249,234,5,33,161,94,164,103,233,158,106,122,152,30,96,83,95,183,22,53,204,127,52,142,188,151,225,83,254,144,191,67,249,61,175,63,127,1,105,41,38,184 };
	CtrlImg::уст(CtrlImg::I_question, MakeCircularIcon(Цвет(85, 127, 200), s_question, sizeof(s_question)));
	static const byte s_exclamation[] = { 120,156,77,80,203,17,68,33,8,107,133,10,24,249,41,246,240,246,224,117,251,47,100,1,121,51,123,81,34,36,36,126,152,113,25,248,70,85,120,120,224,24,13,14,109,180,9,206,40,4,180,144,4,22,35,109,120,72,208,29,204,179,125,104,224,86,208,89,83,183,166,20,233,134,204,164,36,131,64,4,217,162,49,113,110,144,145,12,166,60,111,125,216,106,234,54,28,151,23,195,225,8,165,199,144,10,250,173,117,228,104,55,212,210,73,48,166,180,171,39,164,66,228,250,61,44,249,182,42,213,127,220,239,39,214,199,189,29,197,107,125,8,92,16,98,22,182,71,216,211,90,19,2,52,180,204,54,184,241,27,81,201,54,139,12,23,191,122,68,158,113,239,166,6,39,190,51,162,52,34,205,144,47,139,234,71,90,176,65,239,122,145,166,195,100,217,117,232,37,24,73,219,251,95,168,239,15,254,54,92,127 };
	CtrlImg::уст(CtrlImg::I_exclamation, MakeCircularIcon(красный(), s_exclamation, sizeof(s_exclamation)));
	static const byte s_error[] = { 120,156,101,142,209,13,128,48,8,68,87,233,4,132,22,68,187,3,46,224,254,131,152,122,71,162,241,135,203,3,238,224,220,186,132,181,112,81,109,57,198,18,115,177,163,0,53,209,35,96,27,206,156,254,90,235,26,127,98,222,30,75,120,137,163,105,79,6,137,194,200,34,56,224,230,31,53,194,157,47,193,124,221,193,112,41,174 };
	CtrlImg::уст(CtrlImg::I_error, MakeCircularIcon(красный(), s_error, sizeof(s_error), 0.5, жёлтый()));
}

void  DrawClassicButton(Draw& w, const Прям& r_, Цвет tl1, Цвет br1, Цвет tl2, Цвет br2, Цвет face)
{
	Прям r = r_;
	DrawFrame(w, r, tl1, br1);
	r.дефлируй(1);
	DrawFrame(w, r, tl2, br2);
	r.дефлируй(1);
	w.DrawRect(r, face);
}

Рисунок MakeClassicButton(Цвет tl1, Цвет br1, Цвет tl2, Цвет br2, Цвет face)
{
	Прям r = Размер(8, 8);
	ImageDraw iw(r.дайРазм());
	DrawClassicButton(iw, r, tl1, br1, tl2, br2, face);
	return WithHotSpot(iw, 2, 2);
}

void ChClassicSkin()
{
	LLOG("ChInitWinClassic");

	ChBaseSkin();

	GUI_GlobalStyle_Write(GUISTYLE_CLASSIC);
	GUI_PopUpEffect_Write(Ктрл::IsCompositedGui() ? GUIEFFECT_NONE : GUIEFFECT_SLIDE);
	
	Цвет grayface = смешай(SColorFace(), серый());

	Рисунок edge = IsDarkTheme() ? MakeClassicButton(grayface, белый(), серый(), светлоСерый(), grayface)
	                           : MakeClassicButton(серый(), белый(), чёрный(), светлоСерый(), SColorFace());
	CtrlsImg::уст(CtrlsImg::I_EFE, edge);
	CtrlsImg::уст(CtrlsImg::I_VE, edge);

	for(int i = 0; i < 6; i++)
		CtrlsImg::уст(CtrlsImg::I_DA + i, CtrlsImg::дай(CtrlsImg::I_kDA + i));

	Цвет wg = смешай(SColorFace(), белоСерый());
	{
		Кнопка::Стиль& s = Кнопка::StyleNormal().пиши();
		Рисунок edge = MakeClassicButton(wg, IsDarkTheme() ? смешай(серый(), чёрный()) : чёрный(), белый(), серый(), SColorFace());
		s.look[CTRL_HOT] = s.look[CTRL_NORMAL] = IsDarkTheme() ? edge : MakeClassicButton(белый(), чёрный(), wg, серый(), SColorFace());
		s.look[CTRL_PRESSED] = MakeClassicButton(серый(), серый(), серый(), серый(), grayface);
		s.look[CTRL_DISABLED] = MakeClassicButton(SWhite(), чёрный(), wg, серый(), SColorFace());
		s.monocolor[0] = s.monocolor[1] = s.monocolor[2] = s.monocolor[3] = SColorText();
		s.pressoffset.x = s.pressoffset.y = 1;
		s.transparent = false;

		Кнопка::Стиль& es = Кнопка::StyleEdge().пиши();
		es.look[CTRL_HOT] = es.look[CTRL_NORMAL] = edge;
		es.look[CTRL_PRESSED] = s.look[CTRL_PRESSED];
		es.look[CTRL_DISABLED] = s.look[CTRL_DISABLED];
		
		for(int i = 0; i < 4; i++)
			Кнопка::StyleOk().пиши().look[i] = Кнопка::StyleLeftEdge().пиши().look[i] = Кнопка::StyleScroll().пиши().look[i] = es.look[i];
	}

	{
		ПромотБар::Стиль& s = ПромотБар::дефСтиль().пиши();
		auto SbWc = [&](Значение *look) {
			Цвет wc = смешай(SColorFace(), серый());
			look[CTRL_NORMAL] = wc;
			look[CTRL_HOT] = wc;
			look[CTRL_PRESSED] = SColorText();
			look[CTRL_DISABLED] = wc;
		};
		
		SbWc(s.hupper);
		SbWc(s.hlower);
		SbWc(s.vupper);
		SbWc(s.vlower);
		for(int i = 0; i < 4; i++)
			s.vthumb[i] = s.hthumb[i] = Кнопка::StyleNormal().look[i];
	}

	{
		MultiButton::Стиль& s = MultiButton::дефСтиль().пиши();
		s.border = s.trivialborder = 2;
	}

	{
		КтрлСепаратор::Стиль& s = КтрлСепаратор::дефСтиль().пиши();
		s.l1 = SColorShadow();
		s.l2 = SColorLight();
	}

	{
		БарМеню::Стиль& s = БарМеню::дефСтиль().пиши();
		s.popupbody = SColorFace();
	}
	
	int c = DPI(14);

	for(int i = 0; i < 4; i++) {
		{
			КтрлВкладка::Стиль& s = КтрлВкладка::дефСтиль().пиши();
			s.body = MakeClassicButton(белый(), чёрный(), wg, серый(), SColorFace());
			Цвет f = i == CTRL_PRESSED ? SColorFace : grayface;
			Рисунок t = MakeClassicButton(белый(), чёрный(), wg, серый(), f);
			Размер isz = t.дайРазм();
			isz.cy -= 2;
			ImageDraw iw(isz);
			iw.DrawImage(0, 0, t);
			if(i == CTRL_PRESSED) { // the active tab
				iw.DrawRect(isz.cx - 1, isz.cy - 2, 2, 1, белый());
				iw.DrawRect(isz.cx - 2, isz.cy - 1, 2, 1, wg);
			}
			t = iw;
			SetHotSpots(t, Точка(2, 2));
			s.first[i] = s.both[i] = t;
			ImageDraw iw2(isz);
			iw2.DrawImage(0, 0, t);
			if(i == CTRL_PRESSED) // the active tab
				iw2.DrawRect(0, isz.cy - 1, 1, 1, wg);
			t = iw2;
			SetHotSpots(t, Точка(2, 2));
			s.last[i] = s.normal[i] = t;
			s.margin = 0;
			s.sel = Прям(0, 2, 0, 2);
			s.extendleft = 2;
			s.text_color[i] = SColorText();
		}
//		static int adj[] = { 10, 80, -5, -10 };
//		Цвет f = FaceColor(adj[i]);
		Цвет f = decode(i, 0, SColorPaper(), 1, смешай(SColorPaper(), SColorFace()), 2, SColorFace(), SColorFace());
		{
			for(int opt = 0; opt < 2; opt++) {
				ImagePainter p(c, c);
				p.очисть(обнулиКЗСА());
				p.Circle(DPI(7), DPI(7), DPI(6)).Fill(f).Stroke(2, ТочкаПЗ(DPI(2), DPI(2)), SGray(), ТочкаПЗ(DPI(14), DPI(14)), SWhite());
				p.Circle(DPI(7), DPI(7), DPI(6) - 1).Fill(f).Stroke(1, ТочкаПЗ(DPI(2) + 1, DPI(2) + 1), SBlack(), ТочкаПЗ(DPI(14) - 2, DPI(14) - 2), SColorFace());
				if(opt)
					p.Circle(DPI(7), DPI(7), DPI(4)).Fill(SColorText());
				CtrlsImg::уст((opt ? CtrlsImg::I_S1 : CtrlsImg::I_S0) + i, p);
			}
		}
		{
			for(int chk = 0; chk < 3; chk++) {
				ImagePainter p(c, c);
				p.очисть(обнулиКЗСА());
				DrawClassicButton(p, Размер(c, c), SGray(), SWhite(), SBlack(), SLtGray(), f);
				p.Scale(DPI(1));
				if(chk == 1)
					p.Move(3, 7).Строка(7, 10).Строка(11, 4).Stroke(2, SColorText());
				if(chk == 2)
					p.Rectangle(3, 6, 8, 2).Fill(SColorText());
				CtrlsImg::уст(decode(chk, 0, CtrlsImg::I_O0, 1, CtrlsImg::I_O1, CtrlsImg::I_O2) + i, p);
			}
		}
	}
	
	MakeDialogIcons();
}

void FillImage(Рисовало& p, const ПрямПЗ& r, const Рисунок& m)
{
	Xform2D xform = Xform2D::Translation(r.left, r.top);
	РазмерПЗ isz = m.дайРазм();
	xform = Xform2D::Scale(r.дайШирину() / isz.cx, r.дайВысоту() / isz.cy) * xform;
	p.Fill(m, xform);
}

void RoundedRect(Рисовало& w, double x, double y, double cx, double cy, double rx, double ry, dword corners)
{
	if(corners & CORNER_TOP_LEFT)
		w.Move(x + rx, y).Arc(x + rx, y + ry, rx, ry, -M_PI / 2, -M_PI / 2);
	else
		w.Move(x, y);
	
	if(corners & CORNER_BOTTOM_LEFT)
		w.Строка(x, y + cy - ry).Arc(x + rx, y + cy - ry, rx, ry, M_PI, -M_PI / 2);
	else
		w.Строка(x, y + cy);

	if(corners & CORNER_BOTTOM_RIGHT)
		w.Строка(x + cx - rx, y + cy).Arc(x + cx - rx, y + cy - ry, rx, ry, M_PI / 2, -M_PI / 2);
	else
		w.Строка(x + cx, y + cy);
	
	if(corners & CORNER_TOP_RIGHT)
		w.Строка(x + cx, y + ry).Arc(x + cx - rx, y + ry, rx, ry, 0, -M_PI / 2);
	else
		w.Строка(x + cx, y);

	w.закрой();
}

void RoundedRect(Рисовало& w, ПрямПЗ r, double rx, double ry, dword corner)
{
	RoundedRect(w, r.left, r.top, r.дайШирину(), r.дайВысоту(), rx, ry, corner);
}

Рисунок MakeElement(Размер sz, double radius, const Рисунок& face, double border_width, Цвет border_color, Событие<Рисовало&, const ПрямПЗ&> shape)
{
	ПрямПЗ r(0, 0, sz.cx, sz.cy);
	ImagePainter w(r.дайРазм());
	w.очисть(обнулиКЗСА());
	ПрямПЗ dr = r.дефлят(border_width / 2.0);
	if(!пусто_ли(face)) {
		shape(w, dr);
		w.Fill(SColorFace());
		FillImage(w, r.дефлят(border_width / 2.0 - 1), face);
	}
	shape(w, dr);
	if(!пусто_ли(border_color))
		w.Stroke(border_width, border_color);
	Рисунок m = w;
	Точка p1(int(radius + border_width), int(radius + border_width));
	SetHotSpots(m, p1, (Точка)r.низПраво() - p1 - Точка(1, 1));
	return m;
}

Рисунок сделайКнопку(int radius, const Рисунок& face, double border_width, Цвет border_color, dword corner)
{
	double q = radius + border_width + DPI(16);
	return MakeElement(Размер((int)q, (int)q), radius, face, border_width, border_color, [&](Рисовало& w, const ПрямПЗ& r) {
		RoundedRect(w, r, radius, radius, corner);
	});
}

Рисунок сделайКнопку(int radius, Цвет face, double border_width, Цвет border_color, dword corner)
{
	return сделайКнопку(radius, CreateImage(Размер(DPI(10), DPI(5)), face), border_width, border_color, corner);
}

Рисунок Hot3(const Рисунок& m)
{
	Размер sz = m.дайРазм();
	return WithHotSpots(m, sz.cx / 3, sz.cy / 3, sz.cx - sz.cx / 3, sz.cy - sz.cy / 3);
}

Рисунок ChHot(const Рисунок& m, int n)
{
	return WithHotSpots(m, DPI(n), DPI(n), 0, 0);
}

Цвет AvgColor(const Рисунок& m, const Прям& rr)
{
	int r = 0;
	int g = 0;
	int b = 0;
	int n = 0;
	for(int y = rr.top; y < rr.bottom; y++)
		for(int x = rr.left; x < rr.right; x++) {
			КЗСА c = m[y][x];
			Unmultiply(&c, &c, 1);
			if(c.a > 20) {
				r += c.r;
				g += c.g;
				b += c.b;
				n++;
			}
		}
	return n ? Цвет(r / n, g / n, b / n) : SWhite();
}

Цвет AvgColor(const Рисунок& m, int margin)
{
	return AvgColor(m, Прям(m.дайРазм()).дефлят(margin));
}

Цвет GetInk(const Рисунок& m)
{
	КЗСА avg = AvgColor(m);
	Цвет ink = SBlack();
	int   best = 0;
	for(КЗСА s : m) {
		КЗСА c = SColorFace();
		AlphaBlend(&c, &s, 1);
		if(c.a > 100) {
			c.a = 255;
			int q = Grayscale(абс(c.r - avg.r), абс(c.g - avg.g), абс(c.b - avg.b));
			if(q > best) {
				best = q;
				ink = c;
			}
		}
	}
	return ink;
}

int GetRoundness(const Рисунок& m)
{
	Размер isz = m.дайРазм();
	int bestd = 0, besth = 0;
	int di = 0, hi = 0;
	int hy = isz.cy / 2;
	КЗСА avg = AvgColor(m);
	auto Chk = [&](int x, int y, int& best, int& besti) {
		if(x < isz.cx && y < isz.cy) {
			КЗСА c = m[y][x];
			Unmultiply(&c, &c, 1);
			if(c.a > 100) {
				int q = Grayscale(абс(c.r - avg.r), абс(c.g - avg.g), абс(c.b - avg.b));
				if(q > best) {
					best = q;
					besti = x;
				}
			}
		}
	};
	for(int i = 0; i < 8; i++) {
		Chk(i, hy, besth, hi);
		Chk(i, i, bestd, di);
	}
	return max(di - hi, 0);
}

static Значение sSample;

void SetChameleonSample(const Значение& m, bool once)
{
	if(!once || пусто_ли(sSample))
		sSample = m;
}

Значение GetChameleonSample()
{
	return sSample;
}

Рисунок WithRect(Рисунок m, int x, int y, int cx, int cy, Цвет c)
{
	ImageBuffer ib(m);
	for(int i = 0; i < cx; i++)
		for(int j = 0; j < cy; j++)
			ib[y + j][x + i] = c;
	return ib;
}

Рисунок WithLeftLine(const Рисунок& m, Цвет c, int w)
{
	return WithRect(m, 0, 0, w, m.дайВысоту(), c);
}

Рисунок WithRightLine(const Рисунок& m, Цвет c, int w)
{
	return WithRect(m, m.дайШирину() - w, 0, w, m.дайВысоту(), c);
}

Рисунок WithTopLine(const Рисунок& m, Цвет c, int w)
{
	return WithRect(m, 0, 0, m.дайШирину(), w, c);
}

Рисунок WithBottomLine(const Рисунок& m, Цвет c, int w)
{
	return WithRect(m, 0, m.дайВысоту() - w, m.дайШирину(), w, c);
}

void ChSynthetic(Рисунок *button100x100, Цвет *text, bool macos)
{
	int roundness = DPI(3);
	int roundness2 = roundness;
	Цвет ink = SColorText();
	int lw = macos ? 1 : DPI(1);
	for(int i = 0; i < 4; i++) {
		Рисунок m = button100x100[i];
		Рисунок m2 = macos ? button100x100[i + 4] : m;
		auto Espots = [=](const Рисунок& m) { return WithHotSpots(m, DPI(3), DPI(1), CH_EDITFIELD_IMAGE, DPI(3)); };
		if(i == 0) {
			ink = GetInk(m);
			if(macos && IsDarkTheme())
				ink = серый();
			roundness = macos ? DPI(3) : GetRoundness(m) ? DPI(3) : 0;
			roundness2 = macos ? 0 : roundness;
			CtrlsImg::уст(CtrlsImg::I_EFE, Espots(сделайКнопку(roundness2, SColorPaper(), lw, ink)));
			CtrlsImg::уст(CtrlsImg::I_VE, WithHotSpots(сделайКнопку(DPI(0), SColorPaper(), lw, ink), DPI(2), DPI(2), 0, 0));
			БоксНадпись::SetLook(WithHotSpots(сделайКнопку(2 * roundness / 3, Рисунок(), lw, ink), DPI(3), DPI(3), 0, 0));
		}
		Размер sz = m.дайРазм();
		m = Crop(m, sz.cx / 8, sz.cy / 8, 6 * sz.cx / 8, 6 * sz.cy / 8);
		m2 = Crop(m2, sz.cx / 8, sz.cy / 8, 6 * sz.cx / 8, 6 * sz.cy / 8);
		{
			EditField::Стиль& s = EditField::дефСтиль().пиши();
			s.activeedge = true;
			s.edge[i] = Espots(сделайКнопку(roundness2,
			                              i == CTRL_DISABLED ? SColorFace() : SColorPaper(),
			                              macos && i == CTRL_PRESSED ? DPI(2) : lw,
			                              i == CTRL_PRESSED ? SColorHighlight() : ink));
			if(i == 0)
				s.coloredge = Espots(сделайКнопку(roundness2, чёрный(), DPI(2), Null));
		}
		{
			auto уст = [&](Кнопка::Стиль& s, const Рисунок& arrow = Null, Цвет ink2 = Null, Цвет border = Null) {
				Значение l = сделайКнопку(0, m, DPI(1), Nvl(border, Nvl(ink2, ink)), 0);
				s.look[i] = пусто_ли(arrow) ? l : ChLookWith(l, arrow, ink2);
			};
			Цвет c = смешай(SColorFace(), ink);
			Цвет k = text[i];

			уст(Кнопка::StyleScroll().пиши(), Null, k, c);
			уст(Кнопка::StyleEdge().пиши());
			уст(Кнопка::StyleLeftEdge().пиши());
			ПромотБар::Стиль& s = ПромотБар::дефСтиль().пиши();
			
			уст(s.up, CtrlsImg::UA(), k, c);
			уст(s.down, CtrlsImg::DA(), k, c);
			уст(s.left, CtrlsImg::LA(), k, c);
			уст(s.right, CtrlsImg::RA(), k, c);
		}
		{
			MultiButton::Стиль& s = MultiButton::дефСтиль().пиши();
			s.clipedge = true;
			s.border = s.trivialborder = 0;

			s.left[i] = сделайКнопку(roundness, m2, lw, ink, CORNER_TOP_LEFT|CORNER_BOTTOM_LEFT);
			s.trivial[i] = s.look[i] = s.right[i] = сделайКнопку(roundness, m2, lw, ink, CORNER_TOP_RIGHT|CORNER_BOTTOM_RIGHT);
			if(i == 0)
				s.coloredge = WithHotSpots(сделайКнопку(roundness, чёрный(), DPI(2), Null), DPI(3), lw, 0, 0);
			auto Middle = [&](Рисунок m) {
				ImageBuffer ib(m);
				for(int y = 0; y < lw; y++)
					for(int x = 0; x < ib.дайШирину(); x++) {
						ib[y][x] = ink;
						ib[ib.дайВысоту() - y - 1][x] = ink;
					}
				return WithHotSpot(ib, DPI(1), DPI(1));
			};
			s.lmiddle[i] = Middle(WithRightLine(m2, ink, lw));
			s.rmiddle[i] = Middle(WithLeftLine(m2, ink, lw));
			s.monocolor[i] = s.fmonocolor[i] = text[macos ? i + 4 : i];
			for(int i = 0; i < 4; i++)
				s.edge[i] = Espots(сделайКнопку(roundness, i == CTRL_DISABLED ? SColorFace() : SColorPaper(), lw, ink));
			s.margin = Прям(DPI(3), 2, lw, 2);
			s.activeedge = true;
			s.stdwidth = DPI(17);
		}
		{
			СпинКнопки::Стиль& sp = СпинКнопки::дефСтиль().пиши();
			if(i == 0)
				sp.dec = sp.inc = Кнопка::StyleNormal();
			auto Spin = [&](dword corners, const Рисунок& sm) {
				return ChLookWith(WithLeftLine(сделайКнопку(roundness2, m, 0, чёрный(), corners), ink, lw), sm, text[i]);
			};
			sp.inc.look[i] = Spin(CORNER_TOP_RIGHT, CtrlImg::spinup());
			sp.dec.look[i] = Spin(CORNER_BOTTOM_RIGHT, CtrlImg::spindown());
			sp.width = DPI(16);
			sp.over = DPI(2);
		}
		{
			СпинКнопки::Стиль& sp = СпинКнопки::StyleOnSides().пиши();
			if(i == 0)
				sp.dec = sp.inc = Кнопка::StyleNormal();
			auto Spin = [&](dword corners, const Рисунок& sm, bool left) {
				Рисунок mm = сделайКнопку(roundness2, m, 0, чёрный(), corners);
				mm = left ? WithLeftLine(mm, ink, lw) : WithRightLine(mm, ink, lw);
				return ChLookWith(mm, sm, text[i]);
			};
			sp.inc.look[i] = Spin(CORNER_TOP_RIGHT|CORNER_BOTTOM_RIGHT, CtrlImg::plus(), true);
			sp.dec.look[i] = Spin(CORNER_TOP_LEFT|CORNER_BOTTOM_LEFT, CtrlImg::minus(), false);
			sp.width = DPI(16);
			sp.over = DPI(2);
		}
		{
			HeaderCtrl::Стиль& hs = HeaderCtrl::дефСтиль().пиши();
			Рисунок h = m;
			if(macos)
				h = CreateImage(Размер(10, 10), FaceColor(decode(i, CTRL_NORMAL, 10,
			                                                      CTRL_HOT, IsDarkTheme() ? 15 : 0,
			                                                      CTRL_PRESSED, -5,
			                                                      -8)));
			hs.look[i] = ChHot(WithBottomLine(WithRightLine(h, ink, 1), ink));
		}
		if(i == CTRL_DISABLED) {
			ProgressIndicator::Стиль& s = ProgressIndicator::дефСтиль().пиши();
			ImageBuffer ib(1, 8);
			ImageBuffer ibb(1, 8);
			Цвет c = macos ? AvgColor(button100x100[4]) : SColorHighlight();
			for(int i = 0; i < 8; i++) {
				int a[] = { 20, 40, 10, 0, -10, -20, -30, -40 };
				ib[i][0] = AdjustColor(c, a[i]);
				ibb[i][0] = смешай(SColorFace(), SColorPaper(), i * 255 / 7);
			}
			Рисунок m = сделайКнопку(roundness, Magnify(ib, 10, 1), DPI(1), ink);
			s.hchunk = m;
			s.vchunk = RotateAntiClockwise(m);
			m = сделайКнопку(roundness, Magnify(ibb, 10, 1), DPI(1), ink);
			s.hlook = m;
			s.vlook = RotateAntiClockwise(m);
			s.bound = true;
			s.nomargins = true;
		}
		if(i == CTRL_NORMAL || i == CTRL_PRESSED) {
			Рисунок sm = MakeElement(Размер(DPI(10), DPI(20)), roundness,
			                       CreateImage(Размер(10, 10), серыйЦвет(224 - 20 * i)),
			                       lw, ink, [&](Рисовало& w, const ПрямПЗ& r) {
				double cx = r.дайШирину();
				double cy = r.дайВысоту();
				double uy = 0.4 * cy;
				double by = 0.85 * cy;
				double uq = 0.5 * uy;
				w.Move(r.left, r.top + by)
				 .Строка(r.left, r.top + uy)
				 .Quadratic(r.left, r.top + uq, r.left + cx / 2, r.top)
				 .Quadratic(r.left + cx, r.top + uq, r.left + cx, r.top + uy)
				 .Строка(r.left + cx, r.top + by)
				 .закрой();
			});
			CtrlImg::уст(i == CTRL_PRESSED ? CtrlImg::I_hthumb1 : CtrlImg::I_hthumb, sm);
			CtrlImg::уст(i == CTRL_PRESSED ? CtrlImg::I_vthumb1 : CtrlImg::I_vthumb, RotateClockwise(sm));
		}
		{
			SyntheticTab(i, roundness, ink);
		}
	}
}

void ChBaseSkin()
{
	GUI_GlobalStyle_Write(GUISTYLE_XP);
	GUI_PopUpEffect_Write(Ктрл::IsCompositedGui() ? GUIEFFECT_NONE : GUIEFFECT_SLIDE);
	ColoredOverride(CtrlsImg::Iml(), CtrlsImg::Iml());
}

void ChMakeSkin(int roundness, Цвет button_face, Цвет thumb, int *adj)
{
	GUI_GlobalStyle_Write(GUISTYLE_XP);

	ColoredOverride(CtrlsImg::Iml(), CtrlsImg::Iml());

	for(int i = 0; i < 6; i++)
		CtrlsImg::уст(CtrlsImg::I_DA + i, CtrlsImg::дай(CtrlsImg::I_kDA + i));
		
	int c = DPI(14);

	Цвет text[4];
	Рисунок button[4], sbutton[4];

	Цвет border = серый();
	
	roundness = DPI(roundness);
		
	{
		for(int pass = 0; pass < 2; pass++) {
			Кнопка::Стиль& s = pass ? Кнопка::StyleOk().пиши() : Кнопка::StyleNormal().пиши();
			s.focusmargin = DPI(4);
			for(int i = 0; i < 4; i++) {
				Цвет f = AdjustColor(button_face, adj[i]);
				Цвет ink = i == CTRL_DISABLED ? SColorDisabled() : SColorText();
				s.look[i] = сделайКнопку(roundness, f, DPI(1 + pass), border);
				text[i] = s.monocolor[i] = s.textcolor[i] = ink;
				if(pass == 0) {
					sbutton[i] = сделайКнопку(roundness, f, DPI(1), border);
					button[i] = сделайКнопку(roundness ? DPI(1) : 0, f, DPI(1), border);
					Цвет f = decode(i, 0, SColorPaper(), 1, смешай(SColorPaper(), SColorFace()), 2, SColorFace(), SColorFace());
					{
						for(int opt = 0; opt < 2; opt++) {
							ImagePainter p(c, c);
							p.Scale(DPI(1));
							p.очисть(обнулиКЗСА());
							p.Circle(7, 7, 6).Fill(f).Stroke(1, border);
							if(opt)
								p.Circle(7, 7, 4).Fill(ink);
							CtrlsImg::уст((opt ? CtrlsImg::I_S1 : CtrlsImg::I_S0) + i, p);
						}
					}
					{
						for(int chk = 0; chk < 3; chk++) {
							ImagePainter p(c, c);
							p.Scale(DPI(1));
							p.очисть(обнулиКЗСА());
							p.Rectangle(1, 1, 12, 12).Fill(f).Stroke(1, border);
							if(chk == 1)
								p.Move(3, 7).Строка(7, 10).Строка(11, 4).Stroke(2, ink);
							if(chk == 2)
								p.Rectangle(3, 6, 8, 2).Fill(ink);
							CtrlsImg::уст(decode(chk, 0, CtrlsImg::I_O0, 1, CtrlsImg::I_O1, CtrlsImg::I_O2) + i, p);
						}
					}
				}
			}
		}

		ChSynthetic(sbutton, text);

		{
			auto& s = ToolButton::дефСтиль().пиши();
			s.look[CTRL_NORMAL] = Рисунок();
			s.look[CTRL_HOT] = button[CTRL_HOT];
			s.look[CTRL_PRESSED] = button[CTRL_PRESSED];
			s.look[CTRL_DISABLED] = Рисунок();
			s.look[CTRL_CHECKED] = button[CTRL_PRESSED];
			s.look[CTRL_HOTCHECKED] = button[CTRL_HOT];
		}

		CtrlImg::уст(CtrlImg::I_MenuCheck0, CtrlsImg::O0());
		CtrlImg::уст(CtrlImg::I_MenuCheck1, CtrlsImg::O1());
		CtrlImg::уст(CtrlImg::I_MenuRadio0, CtrlsImg::S0());
		CtrlImg::уст(CtrlImg::I_MenuRadio1, CtrlsImg::S1());
	}

	{
		ПромотБар::Стиль& s = ПромотБар::дефСтиль().пиши();
		ImagePainter p(c, c);
		p.Rectangle(0, 0, c, c).Fill(0, 0, IsDarkTheme() ? SColorFace() : AdjustColor(thumb, 40), c, 0, IsDarkTheme() ? светлоСерый() : SColorPaper());
		Рисунок vtrough = p;

		for(int status = CTRL_NORMAL; status <= CTRL_DISABLED; status++) {
			s.hupper[status] = s.hlower[status] = ChHot(RotateClockwise(vtrough));
			s.vupper[status] = s.vlower[status] = ChHot(vtrough);
			static int adj[] = { 0, 10, -10, -20 };
			s.hthumb[status] = s.vthumb[status] = AdjustColor(thumb, adj[status]);
		}
	}
	
	{
		БарМеню::Стиль& s = БарМеню::дефСтиль().пиши();
		s.topitem[1] = смешай(SColorHighlight(), SColorPaper());
		s.icheck = button[CTRL_PRESSED];
	}
	GUI_PopUpEffect_Write(Ктрл::IsCompositedGui() ? GUIEFFECT_NONE : GUIEFFECT_SLIDE);
	
	MakeDialogIcons();
}

void ChStdSkin()
{
	ChReset();
	static int adj[] = { 10, 80, -5, -10 };
	SColorFace_Write(Цвет(240, 240, 240));
	SColorMenu_Write(Цвет(240, 240, 240));
	SColorHighlight_Write(Цвет(50, 50, 250));
	ChMakeSkin(3, SColorFace(), SLtGray(), adj);
}

void ChGraySkin()
{
	ChReset();
	static int adj[] = { 0, 70, -15, -20 };
	SColorHighlight_Write(серый());
	ChMakeSkin(3, SWhiteGray(), SLtGray(), adj);
}

void ChDarkSkin()
{
	ChReset();
	static int adj[] = { 10, 80, -5, -10 };
	SColorPaper_Write(чёрный());
	SColorHighlight_Write(серый());
	SColorHighlightText_Write(белый());
	ChMakeSkin(3, SWhiteGray(), SWhiteGray(), adj);
}

void ChFlatSkin()
{
	ChReset();
	static int adj[] = { 10, 80, -5, -10 };
	SColorFace_Write(Цвет(240, 240, 240));
	SColorMenu_Write(Цвет(240, 240, 240));
	SColorHighlight_Write(Цвет(50, 50, 250));
	ChMakeSkin(0, SColorFace(), SLtGray(), adj);
}

void ChFlatGraySkin()
{
	ChReset();
	static int adj[] = { 0, 70, -15, -20 };
	SColorHighlight_Write(серый());
	ChMakeSkin(0, SWhiteGray(), SLtGray(), adj);
}

void ChFlatDarkSkin()
{
	ChReset();
	static int adj[] = { 10, 80, -5, -10 };
	SColorPaper_Write(чёрный());
	SColorHighlight_Write(серый());
	SColorHighlightText_Write(белый());
	ChMakeSkin(0, SWhiteGray(), SWhiteGray(), adj);
}

#ifdef GUI_X11

void ChHostSkin()
{
	int h = Ктрл::GetPrimaryScreenArea().устВысоту();
	Шрифт::SetDefaultFont(Arial(h > 1300 ? 26 : h > 800 ? 14 : 12));
	SColorFace_Write(Цвет(242, 241, 240));
	SColorMenu_Write(Цвет(242, 241, 240));
	SColorHighlight_Write(Цвет(50, 50, 250));

	ChStdSkin();
}

#endif

Вектор<Кортеж<void (*)(), Ткст>> GetAllChSkins()
{
	return Вектор<Кортеж<void (*)(), Ткст>> {
		{ ChHostSkin, "Хост platform" },
	    { ChClassicSkin, "Classic" },
		{ ChStdSkin, "Standard" },
		{ ChGraySkin, "серый" },
		{ ChDarkSkin, "Dark" },
		{ ChFlatSkin, "Flat" },
		{ ChFlatGraySkin, "Flat серый" },
		{ ChFlatDarkSkin, "Flat Dark" }
	};
}

}
