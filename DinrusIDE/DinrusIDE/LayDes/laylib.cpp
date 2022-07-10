#include "LayDes.h"

#define LLOG(x) // LOG(x)

struct ImlImageMaker : ImageMaker {
	Ткст id;
	virtual Ткст Ключ() const {
		return id;
	}
	virtual Рисунок сделай() const {
		Вектор<Ткст> s = разбей(id, ':');
		if(s.дайСчёт() != 3)
			return LayImg::ImageError();
		Массив<ImlImage> iml;
		int f;
		LoadIml(загрузиФайл(SourcePath(s[0], s[1])), iml, f);
		bool recolor = false;
		if(*s[2].последний() == '*') {
			s[2].обрежь(s[2].дайДлину() - 1);
			recolor = true;
		}
		Рисунок m = LayImg::ImageError();
		for(int i = 0; i < iml.дайСчёт(); i++)
			if(iml[i].имя == s[2]) {
				m = iml[i].image;
				break;
			}
		return recolor ? AdjustColors(m) : m;
	}
};

Рисунок GetUscImage(const Ткст& id)
{
	Рисунок m = GetImlImage(id);
	if(!пусто_ли(m))
		return AdjustColors(m);
	ImlImageMaker q;
	q.id = id;
	return MakeImage(q);
}

EscValue EscColor(Цвет c)
{
	EscValue v;
	if(c.дайОсобый() >= 0)
		c = AdjustIfDark(RealizeColor(c));
	if(!пусто_ли(c)) {
		v.MapSet("r", c.дайК());
		v.MapSet("g", c.дайЗ());
		v.MapSet("b", c.дайС());
	}
	return v;
}

Цвет ColorEsc(EscValue v)
{
	return v.проц_ли() ? Цвет(Null) : Цвет(v.GetFieldInt("r"), v.GetFieldInt("g"), v.GetFieldInt("b"));
}

EscValue EscRect(const Прям& r)
{
	EscValue v;
	v.MapSet("left", r.left);
	v.MapSet("right", r.right);
	v.MapSet("top", r.top);
	v.MapSet("bottom", r.bottom);
	return v;
}

Прям RectEsc(EscValue v)
{
	return Прям(v.GetFieldInt("left"), v.GetFieldInt("top"),
	            v.GetFieldInt("right"), v.GetFieldInt("bottom"));
}

EscValue EscSize(Размер sz)
{
	EscValue v;
	v.MapSet("cx", sz.cx);
	v.MapSet("cy", sz.cy);
	return v;
}

Размер SizeEsc(EscValue v)
{
	return Размер(v.GetFieldInt("cx"), v.GetFieldInt("cy"));
}

EscValue EscPoint(Точка sz)
{
	EscValue v;
	v.MapSet("x", sz.x);
	v.MapSet("y", sz.y);
	return v;
}

Точка PointEsc(EscValue v)
{
	return Точка(v.GetFieldInt("x"), v.GetFieldInt("y"));
}

void SIC_Print(EscEscape& e)
{
	if(e[0].IsArray())
		VppLog() << (Ткст) e[0];
	else
	if(e[0].число_ли())
		VppLog() << e[0].GetNumber();
	else
	if(!e[0].проц_ли())
		e.выведиОш("invalid argument to 'print'");
}

bool GetOptFlag(EscEscape& e) {
	if(!e.дайСчёт())
		return true;
	return IsTrue(e[0]);
}

struct SIC_Font : public EscHandle {
	Шрифт font;

	void устВысоту(EscEscape& e) {
		e = EscFont(FontEsc(e.self).устВысоту(e.Цел(0)));
	}
	void Bold(EscEscape& e) {
		e = EscFont(FontEsc(e.self).Bold(GetOptFlag(e)));
	}
	void Italic(EscEscape& e) {
		e = EscFont(FontEsc(e.self).Italic(GetOptFlag(e)));
	}
	void Underline(EscEscape& e) {
		e = EscFont(FontEsc(e.self).Underline(GetOptFlag(e)));
	}

	typedef SIC_Font ИМЯ_КЛАССА;

	SIC_Font(EscValue& v) {
		v.Escape("устВысоту(h)", this, THISBACK(устВысоту));
		v.Escape("Bold(...)", this, THISBACK(Bold));
		v.Escape("Italic(...)", this, THISBACK(Italic));
		v.Escape("Underline(...)", this, THISBACK(Underline));
	}
};

EscValue EscFont(Шрифт f)
{
	EscValue v;
	(new SIC_Font(v))->font = f;
	return v;
}

Шрифт FontEsc(EscValue v)
{
	if(!v.IsMap())
		return Null;
	const ВекторМап<EscValue, EscValue>& m = v.дайМап();
	int q = m.найди("устВысоту");
	if(q < 0)
		return Null;
	const EscLambda& l = m[q].GetLambda();
	if(!dynamic_cast<SIC_Font *>(l.handle))
		return Null;
	Шрифт f = ((SIC_Font *)l.handle)->font;
	if(f.дайВысоту() == 0)
		f.устВысоту(StdFont().дайВысоту());
	return f;
}

void SIC_StdFont(EscEscape& e)
{
	if(e.дайСчёт() == 1)
		e = EscFont(StdFont()(Zy(e.Цел(0))));
	else
		e = EscFont(StdFont());
}

void SIC_Arial(EscEscape& e)
{
	e = EscFont(Arial(Zy(e.Цел(0))));
}

void SIC_Roman(EscEscape& e)
{
	e = EscFont(Roman(Zy(e.Цел(0))));
}

void SIC_Courier(EscEscape& e)
{
	e = EscFont(Courier(Zy(e.Цел(0))));
}

void SIC_GetImageSize(EscEscape& e)
{
	e.CheckArray(0);
	e = EscSize(GetUscImage((Ткст)e[0]).дайРазм());
}

void SIC_GetTextSize(EscEscape& e)
{
	if(e.дайСчёт() < 1 || e.дайСчёт() > 2)
		e.выведиОш("wrong number of arguments in call to 'дайРазмТекста'");
	e.CheckArray(0);
	ШТкст text = e[0];
	Шрифт font = StdFont();
	if(e.дайСчёт() > 1)
		font = FontEsc(e[1]);
	e = EscSize(дайРазмТекста(text, font));
}

void SIC_GetSmartTextSize(EscEscape& e)
{
	if(e.дайСчёт() < 1 || e.дайСчёт() > 2)
		e.выведиОш("wrong number of arguments in call to 'дайРазмТекста'");
	e.CheckArray(0);
	Ткст text = вУтф8((ШТкст)(e[0]));
	ExtractAccessKey(text, text);
	Шрифт font = StdFont();
	if(e.дайСчёт() > 1)
		font = FontEsc(e[1]);
	e = EscSize(GetSmartTextSize(text, font));
}

void SIC_GetQtfHeight(EscEscape& e)
{
	int zoom = e.Цел(0);
	e.CheckArray(0);
	Ткст text = e[1];
	int cx = e.Цел(2);
	RichText doc;
	doc = ParseQTF(text);//!!!!!
	e = doc.дайВысоту(Zoom(zoom, 1024), cx);
}

#ifdef _ОТЛАДКА
void SIC_DumpLocals(EscEscape& e)
{
	LOG("--- DUMP of SIC local variables -------------------------");
	for(int i = 0; i < e.esc.var.дайСчёт(); i++)
		LOG(e.esc.var.дайКлюч(i) << " = " << e.esc.var[i].вТкст());
	LOG("---------------------------------------------------------");
}
#endif

static const char laysrc[] = {
"Цвет(r, g, b) { c.r = r; c.g = g; c.b = b; return c; }\n"
"Точка(x, y) { p.x = x; p.y = y; return p; }\n"
"Размер(cx, cy) { sz.cx = cx; sz.cy = cy; return sz; }\n"
"Прям(l, t, r, b) { e.left = l; e.top = t; e.right = r; e.bottom = b; return e; }\n"
"RectC(x, y, cx, cy) { e.left = x; e.top = y; e.right = x + cx; e.bottom = y + cy; return e; }\n"
};

void LayLib()
{
	МассивМап<Ткст, EscValue>& global = UscGlobal();

	скан(global, laysrc, "laydes library");
	Escape(global, "StdFont(...)", SIC_StdFont);
	Escape(global, "Arial(h)", SIC_Arial);
	Escape(global, "Roman(h)", SIC_Roman);
	Escape(global, "Courier(h)", SIC_Courier);
	Escape(global, "GetImageSize(имя)", SIC_GetImageSize);
	Escape(global, "дайРазмТекста(...)", SIC_GetTextSize);
	Escape(global, "GetSmartTextSize(...)", SIC_GetSmartTextSize);
	Escape(global, "GetQtfHeight(zoom, text, cx)", SIC_GetQtfHeight);
#ifdef _ОТЛАДКА
	Escape(global, "dump_locals()", SIC_DumpLocals);
#endif

	Escape(global, "print(x)", SIC_Print);

	global.добавь("чёрный", EscColor(чёрный));
	global.добавь("серый", EscColor(серый));
	global.добавь("светлоСерый", EscColor(светлоСерый));
	global.добавь("белоСерый", EscColor(белоСерый));
	global.добавь("белый", EscColor(белый));
	global.добавь("красный", EscColor(красный));
	global.добавь("зелёный", EscColor(зелёный));
	global.добавь("коричневый", EscColor(коричневый));
	global.добавь("синий", EscColor(синий));
	global.добавь("магента", EscColor(магента));
	global.добавь("цыан", EscColor(цыан));
	global.добавь("жёлтый", EscColor(жёлтый));
	global.добавь("светлоКрасный", EscColor(светлоКрасный));
	global.добавь("светлоЗелёный", EscColor(светлоЗелёный));
	global.добавь("светлоЖёлтый", EscColor(светлоЖёлтый));
	global.добавь("светлоСиний", EscColor(светлоСиний));
	global.добавь("светлоМагента", EscColor(светлоМагента));
	global.добавь("светлоЦыан", EscColor(светлоЦыан));
	global.добавь("SBlack", EscColor(SBlack));
	global.добавь("SGray", EscColor(SGray));
	global.добавь("SLtGray", EscColor(SLtGray));
	global.добавь("SWhiteGray", EscColor(SWhiteGray));
	global.добавь("SWhite", EscColor(SWhite));
	global.добавь("SRed", EscColor(SRed));
	global.добавь("SGreen", EscColor(SGreen));
	global.добавь("SBrown", EscColor(SBrown));
	global.добавь("SBlue", EscColor(SBlue));
	global.добавь("SMagenta", EscColor(SMagenta));
	global.добавь("SCyan", EscColor(SCyan));
	global.добавь("SYellow", EscColor(SYellow));
	global.добавь("SLtRed", EscColor(SLtRed));
	global.добавь("SLtGreen", EscColor(SLtGreen));
	global.добавь("SLtYellow", EscColor(SLtYellow));
	global.добавь("SLtBlue", EscColor(SLtBlue));
	global.добавь("SLtMagenta", EscColor(SLtMagenta));
	global.добавь("SLtCyan", EscColor(SLtCyan));
	global.добавь("SColorFace", EscColor(SColorFace));
	global.добавь("SColorText", EscColor(SColorText));
	global.добавь("SColorPaper", EscColor(SColorPaper));
	global.добавь("SColorHighlight", EscColor(SColorHighlight));

	global.добавь("IntNull", (int)Null);
	global.добавь("DblNull", (double)Null);
	global.добавь("DblNullLim", -1e307);
}

void EscDraw::DrawRect(EscEscape& e)
{
	if(e.дайСчёт() == 2)
		w.DrawRect(RectEsc(e[0]), ColorEsc(e[1]));
	else
	if(e.дайСчёт() == 5)
		w.DrawRect(e.Цел(0), e.Цел(1), e.Цел(2), e.Цел(3), ColorEsc(e[4]));
	else
		e.выведиОш("wrong number of arguments in call to 'DrawRect'");
}

void EscDraw::DrawLine(EscEscape& e)
{
	if(e.дайСчёт() == 4)
		w.DrawLine(PointEsc(e[0]), PointEsc(e[1]), e.Цел(2), ColorEsc(e[3]));
	else
	if(e.дайСчёт() == 6)
		w.DrawLine(e.Цел(0), e.Цел(1), e.Цел(2), e.Цел(3), e.Цел(4), ColorEsc(e[5]));
	else
		e.выведиОш("wrong number of arguments in call to 'DrawLine'");
}

void EscDraw::DrawText(EscEscape& e)
{
	if(e.дайСчёт() < 3 || e.дайСчёт() > 6)
		e.выведиОш("wrong number of arguments in call to 'DrawText'");
	int x = e.Цел(0);
	int y = e.Цел(1);
	Шрифт font = StdFont();
	Цвет color = SColorText;
	if(e[2].цел_ли())
	{
		int z = e.Цел(2);
		e.CheckArray(3);
		ШТкст text = e[3];
		if(e.дайСчёт() > 4)
			font = FontEsc(e[4]);
		if(e.дайСчёт() > 5)
			color = ColorEsc(e[5]);
		w.DrawText(x, y, z, text, Nvl(font, StdFont()), color);
	}
	else
	{
		e.CheckArray(2);
		ШТкст text = e[2];
		if(e.дайСчёт() > 3)
			font = FontEsc(e[3]);
		if(e.дайСчёт() > 4)
			color = ColorEsc(e[4]);
		w.DrawText(x, y, text, Nvl(font, StdFont()), color);
	}
}

void EscDraw::DrawSmartText(EscEscape& e)
{
	if(e.дайСчёт() < 3 || e.дайСчёт() > 7)
		e.выведиОш("wrong number of arguments in call to 'DrawSmartText'");
	int x = e.Цел(0);
	int y = e.Цел(1);
	int ii = 2;
	Ткст text;
	if(ii < e.дайСчёт() && e[ii].IsArray())
		text = вУтф8((ШТкст)e[ii++]);
	int accesskey = ExtractAccessKey(text, text);
	Шрифт font = StdFont().устВысоту(11);
	if(ii < e.дайСчёт())
		font = FontEsc(e[ii++]);
	if(font.дайВысоту() == 0)
#ifdef GUI_X11
		font.устВысоту(12);
#else
		font.устВысоту(11);
#endif
	Цвет color = SColorText;
	if(ii < e.дайСчёт())
		color = ColorEsc(e[ii++]);
	int cx = INT_MAX;
	if(ii < e.дайСчёт())
		cx = e.Цел(ii++);
	::DrawSmartText(w, x, y, cx, text, font, color, accesskey);
}

void EscDraw::DrawQtf(EscEscape& e)
{
	if(e.дайСчёт() < 5 || e.дайСчёт() > 6)
		e.выведиОш("wrong number of arguments in call to 'DrawQtf'");
//	int zoom = e.Цел(0);
	int x = e.Цел(1);
	int y = e.Цел(2);
	e.CheckArray(3);
	ШТкст text = e[3];
	int cx = e.Цел(4);
	Ткст txt = '\1' + вУтф8(text);
	::DrawSmartText(w, x, y, cx, txt, StdFont(), SColorText, 0);
}

void EscDraw::дайРазмТекста(EscEscape& e)
{
	if(e.дайСчёт() < 1 || e.дайСчёт() > 2)
		e.выведиОш("wrong number of arguments in call to 'дайРазмТекста'");
	e.CheckArray(0);
	ШТкст text = e[0];
	Шрифт font = StdFont();
	if(font.дайВысоту() == 0)
#ifdef GUI_X11
		font.устВысоту(12);
#else
		font.устВысоту(11);
#endif
	if(e.дайСчёт() > 1)
		font = FontEsc(e[1]);
	e = EscSize(::дайРазмТекста(text, font));
}

void EscDraw::DrawImage(EscEscape& e)
{
	int cnt = e.дайСчёт();
	if(cnt == 2)
		w.DrawImage(RectEsc(e[0]), GetUscImage((Ткст)e[1]));
	else
	if(cnt == 3)
		w.DrawImage(e.Цел(0), e.Цел(1), GetUscImage((Ткст)e[2]));
	else
	if(cnt == 5)
		w.DrawImage(e.Цел(0), e.Цел(1), e.Цел(2), e.Цел(3), GetUscImage((Ткст)e[4]));
	else
		e.выведиОш("wrong number of arguments in call to 'DrawImage'");
}

void EscDraw::DrawImageColor(EscEscape& e)
{
	int cnt = e.дайСчёт();
	if(cnt != 4 && cnt != 5 && cnt != 7)
		e.выведиОш("wrong number of arguments in call to 'DrawImageColor'");

	Рисунок img = Colorize(GetUscImage((Ткст)e[cnt - 3]), ColorEsc(e[cnt - 2]), e.Цел(cnt - 1));

	if(cnt == 4)
		w.DrawImage(RectEsc(e[0]), img);
	else
	if(cnt == 5)
		w.DrawImage(e.Цел(0), e.Цел(1), img);
	else
	if(cnt == 7)
		w.DrawImage(e.Цел(0), e.Цел(1), e.Цел(2), e.Цел(3), img);
}

EscDraw::EscDraw(EscValue& v, Draw& w)
	: w(w)
{
	v.Escape("DrawRect(...)", this, THISBACK(DrawRect));
	v.Escape("DrawLine(...)", this, THISBACK(DrawLine));
	v.Escape("DrawText(...)", this, THISBACK(DrawText));
	v.Escape("DrawSmartText(...)", this, THISBACK(DrawSmartText));
	v.Escape("DrawQtf(...)", this, THISBACK(DrawQtf));
	v.Escape("дайРазмТекста(...)", this, THISBACK(дайРазмТекста));
	v.Escape("DrawImage(...)", this, THISBACK(DrawImage));
	v.Escape("DrawImageColor(...)", this, THISBACK(DrawImageColor));
}

МассивМап<Ткст, EscValue>& LayGlobal()
{
	static МассивМап<Ткст, EscValue> global;
	return global;
}
