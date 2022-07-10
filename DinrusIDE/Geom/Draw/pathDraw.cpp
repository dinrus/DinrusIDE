#include "GeomDraw.h"

namespace –Ќ÷ѕ {

#define LLOG(x) // LOG(x)

enum { THICK = 5, THICKDOT = 20 };
extern void (*PathDraw_Output_Thick[THICK][THICK])(PathDraw::Output& out, Точка a, Точка b);

static const char *ParseCString(const char *p, “кст& out)
{
	try
	{
		СиПарсер parser(p);
		out = parser.читай1Ткст();
		return parser.дайУк();
	}
	catch(СиПарсер::Ошибка e)
	{
		throw Искл(e);
	}
}

static “кст FormatPrec(double d)
{
	return фмт("%+.*g", PathStyle::PREC, d);
}

//////////////////////////////////////////////////////////////////////
// PathStyle::Trace::

“кст PathStyle::Trace::Encode() const
{
	“кст out;
	out << FormatPrec(left) << FormatPrec(left_top) << FormatPrec(left_bottom)
		<< FormatPrec(right) << FormatPrec(right_top) << FormatPrec(right_bottom);
	if(!пусто_ли(color))
		out << фмт("*%02x%02x%02x", color.дайК(), color.дайЗ(), color.дайС());
	return out;
}

const char *PathStyle::Trace::Decode(const char *s)
{
	if(пусто_ли(left  = сканДво(s, &s)) || пусто_ли(left_top  = сканДво(s, &s)) || пусто_ли(left_bottom  = сканДво(s, &s))
	|| пусто_ли(right = сканДво(s, &s)) || пусто_ли(right_top = сканДво(s, &s)) || пусто_ли(right_bottom = сканДво(s, &s)))
		return 0;
	if(*s == '*')
	{ // color
		for(int i = 1; i <= 6; i++)
			if(!IsXDigit(s[i]))
				return 0; // Ошибка
		color = Цвет(16 * ctoi(s[1]) + ctoi(s[2]), 16 * ctoi(s[3]) + ctoi(s[4]), 16 * ctoi(s[5]) + ctoi(s[6]));
		s += 7;
	}
	return s;
}

void PathStyle::Trace::сериализуй(Поток& stream)
{
	“кст s;
	if(stream.сохраняется())
		s = Encode();
	stream % s;
	if(stream.грузится())
	{
		const char *p = Decode(s);
		if(!p || *p)
			stream.устОш();
	}
}

bool PathStyle::Trace::равен(const Trace& t) const
{
	return left == t.left && left_top == t.left_top && left_bottom == t.left_bottom
		&& right == t.right && right_top == t.right_top && right_bottom == t.right_bottom
		&& color == t.color;
}

PathStyle::Trace& PathStyle::Trace::Crop(double l, double r)
{
	if(l <= left && r >= right)
		return *this;

	if(r <= l || right <= l)
	{
		left = right = l;
		left_top = right_top;
		left_bottom = right_bottom;
	}
	else if(left >= r)
	{
		right = left = r;
		right_top = left_top;
		right_bottom = left_bottom;
	}
	else
	{
		if(l < left)  l = left;
		if(r > right) r = right;
		double rl = (l - left) / устШирину(), rr = (right - r) / устШирину();
		double th = TopHeight(), bh = BottomHeight();
		left = l;
		right = r;
		left_top += rl * th;
		right_top -= rr * th;
		left_bottom += rl * bh;
		right_bottom -= rr * bh;
	}
	return *this;
}

void PathStyle::Trace::инфлируй(ТочкаПЗ amount)
{
	left -= amount.x;
	right += amount.x;
	left_top -= amount.y;
	right_top -= amount.y;
	left_bottom += amount.y;
	right_bottom += amount.y;
}

bool PathStyle::Trace::содержит(ТочкаПЗ pt) const
{
	if(pt.x < left || pt.x > right
		|| pt.y < min(left_top, right_top) || pt.y > max(left_bottom, right_bottom))
		return false;
	double wd = right - left;
	if(left_top != right_top && wd * (pt.y - left_top) < (pt.x - left) * (right_top - left_top))
		return false;
	if(left_bottom != right_bottom && wd * (pt.y - left_bottom) > (pt.x - left) * (right_bottom - left_bottom))
		return false;
	return true;
}

double PathStyle::Trace::GetDistance(ТочкаПЗ pt) const
{
	double dl = (pt.y < left_top  ? pt | LeftTop()  : pt.y > left_bottom  ? pt | LeftBottom()  : tabs(pt.x - left));
	double dr = (pt.y < right_top ? pt | RightTop() : pt.y > right_bottom ? pt | RightBottom() : tabs(pt.x - right));
	double dt = расстояние(pt, LeftTop(), RightTop());
	double db = расстояние(pt, LeftBottom(), RightBottom());
	return min(min(dl, dr), min(dt, db));
}

static bool CheckTrack(ТочкаПЗ A, ТочкаПЗ B, ТочкаПЗ tolerance)
{
	return tabs(A.x - B.x) <= tolerance.x && tabs(A.y - B.y) <= tolerance.y;
}

int PathStyle::Trace::GetTrackStyle(ТочкаПЗ pt, ТочкаПЗ tolerance, ТочкаПЗ& start) const
{
	return CheckTrack(pt, start = LeftTop(),      tolerance) ? LEFT | LEFT_TOP
		:  CheckTrack(pt, start = CenterTop(),    tolerance) ? LEFT_TOP | RIGHT_TOP
		:  CheckTrack(pt, start = RightTop(),     tolerance) ? RIGHT | RIGHT_TOP
		:  CheckTrack(pt, start = LeftCenter(),   tolerance) ? LEFT
		:  CheckTrack(pt, start = RightCenter(),  tolerance) ? RIGHT
		:  CheckTrack(pt, start = LeftBottom(),   tolerance) ? LEFT | LEFT_BOTTOM
		:  CheckTrack(pt, start = CenterBottom(), tolerance) ? LEFT_BOTTOM | RIGHT_BOTTOM
		:  CheckTrack(pt, start = RightBottom(),  tolerance) ? RIGHT | RIGHT_BOTTOM
		:  0;
}

Рисунок PathStyle::Trace::GetTrackCursor(int style)
{
	switch(style)
	{
	case LEFT | LEFT_TOP:      return Рисунок::SizeTopLeft();
	case RIGHT | RIGHT_BOTTOM: return Рисунок::SizeBottomRight();
	case RIGHT | RIGHT_TOP:    return Рисунок::SizeTopRight();
	case LEFT | LEFT_BOTTOM:   return Рисунок::SizeBottomLeft();
	case LEFT_TOP | RIGHT_TOP:
	case LEFT_BOTTOM | RIGHT_BOTTOM:
		return Рисунок::SizeVert();

	case LEFT:
	case RIGHT:
		return Рисунок::SizeHorz();
	}
	return Null;
}

void PathStyle::Trace::рисуй(PlotterTool& tool, bool reduce, Цвет outline) const
{
	bool he = IsHorzEmpty();
	bool ve = IsVertEmpty();

	if(he && ve)
	{ // point
		Plotter& plot = tool.GetPlotter();
		if(plot.InLClip(LeftTop()))
		{
			Точка pt = plot.LtoPoint(LeftTop());
			plot.draw->DrawRect(pt.x - 4, pt.y - 4, 9, 9, outline);
		}
	}
	else if(reduce && (he || ve))
	{
		tool.MoveTo(left, верх());
		tool.LineTo(right, низ());
		tool.рисуй();
	}
	else
	{
		tool.MoveTo(left,  left_top);
		tool.LineTo(right, right_top);
		tool.LineTo(right, right_bottom);
		tool.LineTo(left,  left_bottom);
		tool.LineTo(left,  left_top);
		tool.рисуй();
	}
}

PathStyle::Trace& PathStyle::Trace::Track(ТочкаПЗ delta, int style)
{
	if(style & LEFT)         left         += delta.x;
	if(style & LEFT_TOP)     left_top     += delta.y;
	if(style & RIGHT_TOP)    right_top    += delta.y;

	if(style & RIGHT)        right        += delta.x;
	if(style & LEFT_BOTTOM)  left_bottom  += delta.y;
	if(style & RIGHT_BOTTOM) right_bottom += delta.y;

	if(left > right)
	{
		разверни(left, right);
		разверни(left_top, right_top);
		разверни(left_bottom, right_bottom);
	}
	if(left_top > left_bottom)
		разверни(left_top, left_bottom);
	if(right_top > right_bottom)
		разверни(right_top, right_bottom);
	return *this;
}

PathStyle::Trace& PathStyle::Trace::свяжи(const ПрямПЗ& rc)
{
	if(rc.left   > left)         left         = rc.left;
	if(rc.right  < right)        right        = rc.right;
	if(rc.top    > left_top)     left_top     = rc.top;
	if(rc.top    > right_top)    right_top    = rc.top;
	if(rc.bottom < left_bottom)  left_bottom  = rc.bottom;
	if(rc.bottom < right_bottom) right_bottom = rc.bottom;
	return *this;
}

//////////////////////////////////////////////////////////////////////
// PathStyle::Clip::

void PathStyle::Clip::сериализуй(Поток& stream)
{
	int version = StreamHeading(stream, 1, 1, 1, "PathStyle::Clip");
	if(version >= 1)
		stream % traces;
}

ПрямПЗ PathStyle::Clip::GetExtent() const
{
	ПрямПЗ rc = Null;
	for(int i = 0; i < traces.дай—чЄт(); i++)
		rc |= traces[i].GetExtent();
	return rc;
}

//////////////////////////////////////////////////////////////////////
// PathStyle::

PathStyle::PathStyle(pick_ PathStyle& style)
: PathStyleMisc(style), traces(style.traces)
{
	ПРОВЕРЬ(&style != &PathStyle::solid());
}

PathStyle& PathStyle::operator = (pick_ PathStyle& style)
{
	ПРОВЕРЬ(&style != &PathStyle::solid());
	PathStyleMisc::operator = (style);
	traces = style.traces;
	return *this;
}

bool PathStyle::IsSolid(double& wd, Цвет& co) const
{
	double tol = 1e-6;
	if(traces.дай—чЄт() != 1 || tabs(begin) > tol || tabs(end) > tol)
		return false;
	const Trace& tr = traces[0];
	if(tabs(tr.left) > tol || tabs(tr.right - segment) > tol
	|| tabs(tr.left_top - tr.right_top) > tol || tabs(tr.left_bottom - tr.right_bottom) > tol
	|| tabs(tr.left_top + tr.left_bottom) > tol)
		return false;
	wd = 2 * tabs(tr.left_top);
	co = tr.color;
	return true;
}

“кст PathStyle::Encode() const
{
	“кст out;
	out << 'p' << FormatPrec(width)
		<< FormatPrec(begin) << FormatPrec(segment) << FormatPrec(end)
		<< '<';
	for(int i = 0; i < traces.дай—чЄт(); i++)
		out.конкат(traces[i].Encode());
	out << '>' << (miter == MITER_ROUND ? 'r' : miter == MITER_SHARP ? 's' : 'f');
	if(miter == MITER_SHARP)
		out << FormatPrec(chamfer);
	return out;
}

const char *PathStyle::Decode(const char *s)
{
	if(*s++ == 'p'
		&& !пусто_ли(width   = сканДво(s, &s))
		&& !пусто_ли(begin   = сканДво(s, &s))
		&& !пусто_ли(segment = сканДво(s, &s))
		&& !пусто_ли(end     = сканДво(s, &s))
		&& *s++ == '<')
	{
		while(*s == '+' || *s == '-')
		{
			Trace t;
			if((s = t.Decode(s)) == 0)
				return 0;
			traces.добавь(t);
		}
		if(*s++ == '>' && (*s == 'r' || *s == 's' || *s == 'f'))
		{
			chamfer = STD_CHAMFER;
			miter = (*s == 'r' ? MITER_ROUND : *s == 's' ? MITER_SHARP : MITER_FLAT);
			s++;
			if(miter != MITER_SHARP || !пусто_ли(chamfer = сканДво(s, &s)))
				return s;
		}
	}
	return 0;
}

void PathStyle::сериализуй(Поток& stream)
{
	“кст s;
	if(stream.сохраняется())
		s = Encode();
	stream % s;
	if(stream.грузится() && !Decode(s))
		stream.устОш();
}

const PathStyle& PathStyle::empty()
{
	static PathStyle p;
	return p;
}

const PathStyle& PathStyle::solid()
{
	static PathStyle p;
	if(p.пустой())
		p.traces << PathStyle::Trace(0, -0.5, +0.5, 1, -0.5, +0.5);
	return p;
}

const PathStyle& PathStyle::dash()
{
	static PathStyle p;
	if(p.пустой())
	{
		p.segment = 4;
		p.traces
			<< PathStyle::Trace(0, -0.5, +0.5, 1, -0.5, +0.5)
			<< PathStyle::Trace(3, -0.5, +0.5, 4, -0.5, +0.5);
	}
	return p;
}

const PathStyle& PathStyle::dot()
{
	static PathStyle p;
	if(p.пустой())
	{
		p.segment = 2;
		p.traces << PathStyle::Trace(0.5, -0.5, +0.5, 1.5, -0.5, +0.5);
	}
	return p;
}

const PathStyle& PathStyle::dash_dot()
{
	static PathStyle p;
	if(p.пустой())
	{
		p.segment = 7;
		p.traces
			<< PathStyle::Trace(0, -0.5, +0.5, 1, -0.5, +0.5)
			<< PathStyle::Trace(3, -0.5, +0.5, 4, -0.5, +0.5)
			<< PathStyle::Trace(6, -0.5, +0.5, 7, -0.5, +0.5);
	}
	return p;
}

const PathStyle& PathStyle::dash_dot_dot()
{
	static PathStyle p;
	if(p.traces.пустой())
	{
		p.segment = 10;
		p.traces
			<< PathStyle::Trace(0, -0.5, +0.5, 1, -0.5, +0.5)
			<< PathStyle::Trace(3, -0.5, +0.5, 4, -0.5, +0.5)
			<< PathStyle::Trace(6, -0.5, +0.5, 7, -0.5, +0.5)
			<< PathStyle::Trace(9, -0.5, +0.5, 10, -0.5, +0.5);
	}
	return p;
}

void PathStyleMap::сериализуй(Поток& stream)
{
	int version = 1;
	stream / version % map;
	if(stream.грузится())
		sort.очисть();
}

“кст PathStyleMap::Export() const
{
	“кст out;
	out << "# PathStyleMap version 1\n"
		"$" << какТкстСи(имя) << "\n";
	for(int i = 0; i < map.дай—чЄт(); i++)
		out << какТкстСи(map.дайКлюч(i)) << '=' << map[i].Encode() << '\n';
	return out;
}

void PathStyleMap::Import(“кст s, bool update_existing)
{
	int line = 1;
	try
	{
		for(const char *p = s; *p;)
			if((byte)*p <= ' ')
			{
				if(*p++ == '\n')
					line++;
			}
			else if(*p == '#')
			{ // comment - skip until end of line
				while(*p && *p++ != '\n')
					;
				line++;
			}
			else if(*p == '$')
			{ // style имя
				if(*++p != '\"')
					throw Искл(“кст().конкат() << "oиekбvбn znak \", nalezeno: " << StringSample(p, 10));
				p = ParseCString(p + 1, имя);
			}
			else if(*p == '\"')
			{
				“кст sn;
				p = ParseCString(p + 1, sn);
				if(*p != '=')
					throw Искл(“кст().конкат() << "oиekбvбno '=', nalezeno: " << StringSample(p, 10));
				if(пусто_ли(sn))
					throw Искл(“кст().конкат() << "nбzev stylu je prбzdnэ");
				for(p++; *p && *p != '\n' && (byte)*p <= ' '; p++)
					;
				PathStyle ps;
				p = ps.Decode(p);
				if(!p)
					throw Искл(“кст().конкат() << "neplatnб definice stylu '" << имя << "'");
				if(map.найди(sn) < 0 || update_existing)
					уст(sn, ps);
			}
			else
				throw Искл(“кст().конкат() << "neplatnэ zaибtek шбdku: " << StringSample(p, 10));
	}
	catch(Искл e)
	{
		throw Искл(“кст().конкат() << "шбdek " << line << ": " << e);
	}
}

const ¬ектор<int>& PathStyleMap::GetSort() const
{
	if(sort.пустой() && !map.пустой())
		sort = GetSortOrder(map.дайКлючи(), GetLanguageInfo());
	return sort;
}

PathStyle PathStyleMap::скан(“кст имя, const PathStyle& dflt) const
{
	if(пусто_ли(имя))
		return PathStyle(dflt, 0);
	if(this)
	{
		int i = map.найди(имя);
		if(i >= 0)
			return PathStyle(map[i], 0);
	}
	PathStyle ps;
	const char *end = ps.Decode(имя);
	if(!end || *end)
		return PathStyle(dflt, 0);
	return ps;
}

int PathStyleMap::FindSortName(“кст имя, int exclude) const
{
	for(int i = 0; i < дай—чЄт(); i++)
		if(i != exclude && GetSortName(i) == имя)
			return i;
	return -1;
}

“кст PathStyleMap::GetUniqueName(“кст prefix) const
{
	int max_index = -1;
	int pl = prefix.дайДлину();
	for(int i = 0; i < дай—чЄт(); i++)
	{
		“кст s = map.дайКлюч(i);
		if(s.дайДлину() >= pl + 2 && !memcmp(s, prefix, pl))
		{
			const char *p = s.старт() + pl;
			if(*p == ' ' && цифра_ли(*++p))
			{
				int x = stou(p, &p);
				if(*p == 0)
					max_index = max(max_index, x);
			}
		}
		else if(s == prefix)
			max_index = 0;
	}
	if(max_index < 0)
		return prefix;
	return prefix + ' ' + целТкт(max_index + 1);
}

PathStyleMap& PathStyleMap::App()
{
	static PathStyleMap psm;
	if(psm.пустой())
	{
		psm.уст(".empty",        PathStyle::empty());
		psm.уст(".solid",        PathStyle::solid());
		psm.уст(".dash",         PathStyle::dash());
		psm.уст(".dot",          PathStyle::dot());
		psm.уст(".dash_dot",     PathStyle::dash_dot());
		psm.уст(".dash_dot_dot", PathStyle::dash_dot_dot());
	}
	return psm;
}

static void RecurseArc(¬ектор<Точка>& out, ТочкаПЗ begin, ТочкаПЗ end, double l, double h, int maxvert)
{
	if(maxvert <= 1 || tabs(h) <= 1 || l <= 1)
	{
		out.добавь(PointfToPoint(end));
		return;
	}
	ТочкаПЗ normal = лево(end - begin) * (h / l);
	ТочкаПЗ centre = середина(begin, end) + normal;
	double ll = centre | end;
	double hh = h * ll / (2 * ll + l);
	maxvert >>= 1;
	RecurseArc(out, begin,  centre, ll, hh, maxvert);
	RecurseArc(out, centre, end,    ll, hh, maxvert);
}

static ¬ектор<Точка>& GetArcPath(¬ектор<Точка>& out, Точка begin, Точка end, double height, int maxvert = 256)
{
	RecurseArc(out, begin, end, hypot(begin.x - end.x, begin.y - end.y), height, maxvert);
	return out;
}

static ¬ектор<Точка> GetArcPath(Точка begin, Точка end, double height, int maxvert = 256)
{
	¬ектор<Точка> out;
	return GetArcPath(out, begin, end, height, maxvert);
}

void PathDraw::Output::AddNext(const ¬ектор<Точка>& list)
{
	Точка old = vertices.верх();
	int c = vertices.дай—чЄт();
	for(const Точка *p = list.старт(), *e = list.стоп(); p != e; ++p)
		if(*p != old)
			vertices.добавь(old = *p);
	counts.верх() += vertices.дай—чЄт() - c;
}

void PathDraw::Output::AddFirst(Точка pt)
{
	if(!counts.пустой() && counts.верх() == 1)
		vertices.верх() = pt;
	else if(vertices.пустой() || vertices.верх() != pt)
	{
		vertices.добавь(pt);
		counts.добавь(1);
	}
}

void PathDraw::Output::AddThick(void (*ln)(Output& out, Точка a, Точка b), Точка start, const ¬ектор<Точка>& list)
{
	if(!list.пустой())
	{
		ln(*this, start, list[0]);
		for(const Точка *p = list.старт() + 1, *e = list.стоп(); p < e; p++)
			ln(*this, p[-1], p[0]);
	}
}

Точка *PathDraw::Output::AddSeg(int segments, int seg_count)
{
	int c = vertices.дай—чЄт();
	int n = counts.дай—чЄт();
	if(c > 0 && counts[n - 1] == 1)
		c--, n--;
	vertices.устСчётР(c + segments * seg_count);
	counts.устСчётР(n + segments);
	Fill(counts.старт() + n, counts.стоп(), seg_count);
	return vertices.старт() + c;
}

Точка *PathDraw::Output::AddSeg(int count)
{
	int c = vertices.дай—чЄт();
	int n = counts.дай—чЄт();
	if(n > 0 && counts[n - 1] == 1)
	{
		c--;
		counts[n - 1] = count;
	}
	else
		counts.добавь(count);
	vertices.устСчётР(c + count);
	return &vertices[c];
}

void PathDraw::Output::слей()
{
	int cc = counts.дай—чЄт();
	if(cc == 0)
		return;
	int vc = vertices.дай—чЄт();
	if(counts[cc - 1] == 1)
	{
		if(--cc == 0)
			return;
		vc--;
	}
	draw.DrawPolyPolyline(vertices.старт(), vc, counts.старт(), cc, width, color);
	Точка end = vertices.верх();
	vertices.устСчётР(1);
	vertices[0] = end;
	counts.устСчётР(1);
	counts[0] = 1;
}

PathDraw::PathDraw()
{
	moveto = 0;
	lineto = 0;
}

PathDraw::PathDraw(Draw& _draw, const PathStyle& _style, Цвет _color, double _width, double _dash, bool _closed)
{
	уст(_draw, _style, _color, _width, _dash, _closed);
}

void PathDraw::очисть()
{
	draw = 0;
	style = 0;
	split_buffer.очисть();
	outputs.очисть();
	segment_lines.очисть();
	open_traces.очисть();
	traces.очисть();
}

void PathDraw::AddSegment(const PathStyle::Trace& trace, bool infinite)
{
	int ltop, lbot;
	ltop = fround(width * trace.верх());
	lbot = fround(width * trace.низ());
	if(width * trace.LeftHeight() > 1)
	{
		ltop = fround(width * trace.left_top);
		lbot = ltop + fround(width * trace.LeftHeight());
	}
	else
		ltop = lbot = fround(width * (trace.left_top + trace.left_bottom) / 2);
	Цвет c = Nvl(trace.color, std_color);
/*
	int   wd =
	int x = outputs.найди(c);
	if(x < 0 || infinite)
	{
		x = outputs.дай—чЄт();
		outputs.добавь(c, new Output(c, *draw));
	}
*/
	if(infinite)
	{
		Linear& ln = segment_lines.добавь();
		ln = Linear(ltop, lbot, c, -1);
		int w = ln.bottom - ln.top;
		ln.ground = false;
		ln.color = c;
		if(w <= (draw->Pixels() ? THICK : THICKDOT))
		{
			w = max<int>(w - 1, 0);
			ln.ground = (tabs(ln.top + ln.bottom) <= 1);
			ln.top = ln.bottom = (ln.top + ln.bottom) >> 1;
			if(draw->Pixels())
			{
				ln.hline = PathDraw_Output_Thick[w][w];
				w = 0;
			}
			else
				ln.hline = PathDraw_Output_Thick[0][0];
		}
		if(!ln.ground && (ltop || lbot))
			calc_miter = true;
		int x;
		for(x = outputs.найди(c); x >= 0 && outputs[x].width != w; x = outputs.найдиСледщ(c))
			;
		if(x < 0)
		{
			x = outputs.дай—чЄт();
			outputs.добавь(c, new Output(c, w, *draw));
		}
		ln.Индекс = x;
	}
	else
	{
		if(traces.дай—чЄт() == seg.start)
		{
			seg.top = ltop;
			seg.bottom = lbot;
		}
		else
		{
			seg.top = min(seg.top, ltop);
			seg.bottom = max(seg.bottom, lbot);
		}

		Trace& t = traces.добавь();
		t.left_top = ltop;
		t.left_bottom = lbot;
		t.right_top = fround(width * trace.right_top);
		t.right_bottom = t.right_top + fround(width * trace.RightHeight());
		double wd = dash * trace.устШирину();
		t.top_step = (t.right_top - t.left_top) / wd;
		t.bottom_step = (t.right_bottom - t.left_bottom) / wd;
		t.start = fround(dash * trace.left);
		t.width = fround(dash * trace.right) - t.start + 1;
		t.vline = (t.width <= 1);
		t.hline = 0;
		int lw = t.left_bottom - t.left_top;
		int rw = t.right_bottom - t.right_top;
		int sw = 0;
		if(draw->Dots() && lw <= THICKDOT && tabs(rw - lw) <= 5)
		{
			sw = (rw + lw) >> 1;
			t.hline = PathDraw_Output_Thick[0][0];
/*			if(t.width > 0)
			{
				t.start++;
				t.width = max<int>(t.width - 2, 1);
			}
*/		}
		else if(draw->Pixels() && lw <= THICK && rw <= THICK)
		{
			sw = 0;
			t.hline = PathDraw_Output_Thick[max<int>(0, lw - 1)]
				[max<int>(0, rw - 1)];
			t.left_top  = t.left_bottom  = (t.left_top  + t.left_bottom)  >> 1;
			t.right_top = t.right_bottom = (t.right_top + t.right_bottom) >> 1;
/*			if(t.width > 0 && lw > 1)
			{
				t.start++;
				t.width = max<int>(t.width - 2, 1);
			}
*/		}
		t.left_ground = (t.left_top ^ t.left_bottom) < 0;
		t.right_ground = (t.right_top ^ t.right_bottom) < 0;
		t.color = c;
		int x;
		for(x = outputs.найди(c); x >= 0 && outputs[x].width != sw; x = outputs.найдиСледщ(x))
			;
		if(x < 0)
		{
			x = outputs.дай—чЄт();
			outputs.добавь(c, new Output(c, sw, *draw));
		}
		t.Индекс = x;
		calc_miter = true;
	}
}

void PathDraw::уст(Draw& _draw, const PathStyle& _style, Цвет _color, double _width, double _dash, bool _closed)
{
	//RTIMING("PathDraw::уст");

	ПРОВЕРЬ(_dash != 0);

	очисть();

	draw = &_draw;

	style = &_style;
	std_color = _color;
	width = _width;
	double avg = GetAvgPixelsPerDot(*draw);
	if(width < 0)
		width *= -avg;
	if(width < 1)
		width = 1;
	LLOG("PathDraw::уст: pixels = " << draw->Pixels() << ", avg pixels per dot = " << avg << ", px width = " << width);
	dash = Nvl(_dash, width);
	if(dash < 0)
		dash *= -avg;

	miter = style->miter;
	clip_rect = draw->GetPageSize(); //draw->GetClip();
	double cr = style->width;
	if(style->miter == style->MITER_SHARP)
	{
		double ca = 2 * atan2(1, style->chamfer);
		chamfer_ratio = -cos(ca);
		chamfer_dist = style->chamfer;
		cr = hypot(cr, style->chamfer);
	}
	clip_radius = fceil(width * cr);
	clip_rect.инфлируй(clip_radius);
	closed = _closed;
	calc_miter = false;

	Массив<PathStyle::Trace> bseg, sseg, eseg;
	Индекс<int> overlap_begin, overlap_end;
	double prec = 1.0 / width;
	double endpos = style->begin + style->segment;
	double bsub = style->begin - prec, badd = style->begin + prec;
	double ssub = endpos - prec, sadd = endpos + prec;

	{ // split style traces
		double prec2 = sqr(prec);
		double total = endpos + style->end;
		int i;
		for(i = 0; i < style->traces.дай—чЄт(); i++)
		{
			const PathStyle::Trace& t = style->traces[i];
			if(t.left <= bsub)
				bseg.добавь(t.GetCropRel(0, style->begin));
			if(t.left <= ssub && t.right >= badd)
				sseg.добавь(t.GetCropRel(style->begin, endpos));
			if(t.right >= sadd)
				eseg.добавь(t.GetCropRel(endpos, total));
		}

		// generate overlaps
/*
		for(i = 0; i < sseg.дай—чЄт() && sseg[i].left <= prec; i++)
		{
			const PathStyle::Trace& beg = sseg[i];
			for(int j = i; ++j < sseg.дай—чЄт();)
			{
				const PathStyle::Trace& end = sseg[j];
				if(end.color == beg.color
				&& end.left > beg.right && end.right >= ssub
				&& tabs(beg.left_top - end.right_top) <= prec
				&& tabs(beg.left_bottom - end.right_bottom) <= prec
				&& tabs(end.устШирину() * beg.TopHeight() - beg.устШирину() * end.TopHeight()) <= prec2
				&& tabs(end.устШирину() * beg.BottomHeight() -beg.устШирину() * end.BottomHeight()) <= prec2)
				{ // join segments
					overlap_begin.добавь(i);
					overlap_end.добавь(j);
					break;
				}
			}
		}
//*/
	}

	seg = Segment(0, 0, 0, 0, 0);
	if(!closed)
	{ // process begin segment
		seg.length = style->begin * dash;
		int i;
		for(i = 0; i < bseg.дай—чЄт(); i++)
			AddSegment(bseg[i], false);
		// add left overlap segments to begin segment
		for(i = 0; i < overlap_begin.дай—чЄт(); i++)
			AddSegment(sseg[overlap_begin[i]], false);
	}
	seg.end = traces.дай—чЄт();
	seg_begin = seg;

	// process main segment
	seg = Segment(0, 0, seg.end, seg.end, dash * style->segment);
	for(int i = 0; i < sseg.дай—чЄт(); i++)
		if(overlap_begin.найди(i) < 0)
		{
			const PathStyle::Trace& ti = sseg[i];
			int o = overlap_end.найди(i);
			if(o >= 0)
			{ // join with begin segment
				const PathStyle::Trace& ji = sseg[overlap_begin[o]];
				PathStyle::Trace new_trace;
				new_trace.left = ti.left;
				new_trace.left_top = ti.left_top;
				new_trace.left_bottom = ti.left_bottom;
				new_trace.right = ji.right + style->segment;
				new_trace.right_top = ji.right_top;
				new_trace.right_bottom = ji.right_bottom;
				AddSegment(new_trace, false);
			}
			else
				AddSegment(sseg[i], ti.left <= 0 && ti.right >= style->segment
					&& tabs(ti.TopHeight()) <= prec && tabs(ti.BottomHeight()) <= prec);
		}

	seg.end = traces.дай—чЄт();
	seg_segment = seg;

	seg = Segment(0, 0, seg.end, seg.end, 0);
	if(!closed)
	{
		seg.length = dash * style->end;
		for(int i = 0; i < eseg.дай—чЄт(); i++)
			AddSegment(eseg[i], false);
	}
	seg.end = traces.дай—чЄт();
	seg_end = seg;

	moveto = &PathDraw::MoveToFull;
	lineto = &PathDraw::LineToFull;
	if(seg_begin.пустой() && seg_end.пустой()
		&& traces.пустой() && segment_lines.дай—чЄт() == 1)
	{
		const Linear& l = segment_lines[0];
		one_output = &outputs[0];
		if(l.hline == PathDraw_Output_Thick[0][0])
		{
			moveto = &PathDraw::MoveToSimple;
			lineto = &PathDraw::LineToSimple;
		}
		else if(l.hline && l.ground && draw->Pixels() && tabs(l.top + l.bottom) <= 1)
		{
			moveto = &PathDraw::MoveToThick;
			lineto = &PathDraw::LineToThick;
			one_thick = l.hline;
		}
	}

	init_empty = false;
	init_part = (closed ? PART_OPEN : seg_begin.пустой() ? PART_SEGMENT : PART_BEGIN);
	init_seg = (init_part == PART_BEGIN ? seg_begin : seg_segment);
	if(init_seg.пустой() && seg_segment.пустой() && segment_lines.пустой())
		if(closed)
		{
			empty = init_empty = true;
			return;
		}
		else
			init_part = PART_SKIPEND;
	if(init_part == PART_SKIPEND && seg_end.пустой())
		init_empty = true;

	Restart();
}

void PathDraw::SetSegment(const Segment& s, PART p)
{
	seg = s;
	part = p;
	next_index = seg.start;
	skip_size = (seg.пустой() ? 1e100 : traces[next_index].start);
}

void PathDraw::Restart()
{
	if(empty = init_empty)
		return;
	open_traces.очисть();
	Fill<Точка>(raw_pos, raw_pos + __countof(raw_pos), Null);
	Fill<Точка>(split_pos, split_pos + __countof(split_pos), Null);
	pos = raw_pos;
	part_total = 0;
	ПРОВЕРЬ(init_seg.end <= traces.дай—чЄт());
	SetSegment(init_seg, init_part);
}

bool PathDraw::SetExtent(const Прям& rc)
{
	if(!пусто_ли(rc) && !rc.пересекается(clip_rect))
		return false;
	// todo: clip optimization
	return true;
}

void PathDraw::MoveToSimple(Точка pt)
{
	//RTIMING("PathDraw::MoveToSimple");
	if(closed && !пусто_ли(close_line[0]) && !пусто_ли(pos[0]))
		one_output->AddNext(close_line[0]);
	pos[0] = close_line[0] = pt;
	one_output->AddFirst(pt);
}

void PathDraw::LineToSimple(Точка pt)
{
	//RTIMING("PathDraw::LineToSimple");
	one_output->CheckFlush();
	one_output->AddNext(pos[0] = pt);
}

void PathDraw::MoveToThick(Точка pt)
{
	//RTIMING("PathDraw::MoveToThick");
	if(closed && !пусто_ли(close_line[0]) && !пусто_ли(pos[0]))
		one_thick(*one_output, pos[0], close_line[0]);
	one_output->CheckFlush();
	pos[0] = close_line[0] = pt;
}

void PathDraw::LineToThick(Точка pt)
{
	//RTIMING("PathDraw::LineToThick");
	one_output->CheckFlush();
	one_thick(*one_output, pos[0], pt);
	pos[0] = pt;
}

void PathDraw::MoveToFull(Точка p)
{
	//RTIMING("PathDraw::MoveToFull");
	if(empty)
		return;

	if(!closed)
		добавьПоз(Null);
	bool split_this = true;
	if(pos[2] != Null)
		switch(part)
		{
		case PART_OPEN:
			break;

		case PART_BEGIN:
			if(!DoBegin())
				break;
			split_this = false;

		case PART_SKIPEND:
		case PART_SEGMENT:
			if(closed)
			{
				for(int i = 0; i < __countof(close_line); i++)
					if(close_line[i] != raw_pos[0])
					{
						добавьПоз(close_line[i]);
						LineToRaw();
					}
			}
			else if(seg_end.length > 0)
			{ // process split буфер
				if(split_this)
					AddSplit();
				FlushSplit(seg_end.length);
				if(split_buffer.пустой())
					break;
				pos = split_pos;
				разбей sp = split_buffer.дайГолову();
				split_buffer.сбросьГолову();
				добавьПоз(sp.pos);
				part_total -= seg_end.length;
				double t = sp.GetEndPos(sp.total - part_total);
				pos[0].x = pos[1].x + fround((pos[0].x - pos[1].x) * t);
				pos[0].y = pos[1].y + fround((pos[0].y - pos[1].y) * t);
				Точка bk = pos[0];
				if(pos[0] != pos[1])
				{
					if(part != PART_SKIPEND)
						LineToRaw();
					добавьПоз();
				}
				pos[0] = Null;
				if(part != PART_SKIPEND)
					LineToRaw();
				Restart();
				pos[1] = bk;
				pos[2] = pos[3] = Null;
				pos[0] = sp.pos;
				if(pos[0] == pos[1])
					pos[1] = Null;
				SetSegment(seg_end, PART_END);
				while(!split_buffer.пустой())
				{
					добавьПоз(split_buffer.дайГолову().pos);
					split_buffer.сбросьГолову();
					LineToRaw();
				}
				добавьПоз(Null);
				LineToRaw();
				pos = raw_pos;
			}
			else
				LineToRaw();
			break;
		}
	Restart();
	split_pos[0] = raw_pos[0] = p;
}

void PathDraw::CheckSplit()
{
	double tl = 0;
	for(int i = 0; i < split_buffer.дай—чЄт(); i++)
		tl += split_buffer[i].total;
	if(tabs(tl - part_total) >= 0.1)
	{
		NEVER();
	}
}

void PathDraw::FlushSplit(double keep_length)
{
//	CheckSplit();
	pos = split_pos;
	if(part == PART_SKIPEND)
	{
		while(!split_buffer.пустой())
		{ // flush part of буфер
			const разбей& sp = split_buffer.дайГолову();
			if(part_total - sp.total < seg_end.length)
				break;
			добавьПоз(sp.pos);
			part_total -= sp.total;
			split_buffer.сбросьГолову();
		}
	}
	else
	{
		while(!split_buffer.пустой())
		{ // flush part of буфер
			const разбей& sp = split_buffer.дайГолову();
			if(part_total - sp.total < seg_end.length)
				break;
			добавьПоз(sp.pos);
			LineToRaw();
			part_total -= sp.total;
			split_buffer.сбросьГолову();
		}
	}
	pos = raw_pos;
//	CheckSplit();
}

#define CALC_MITER() \
	bool start = пусто_ли(pos[3]); \
	bool end = пусто_ли(pos[0]); \
	ТочкаПЗ a, b, a0, b0, at, bt, ar, up; \
	double al, bl; \
	double a_b, axb, /* rseg, */ ulen; \
	bool too_sharp; \
	if(calc_miter) \
	{ \
		a = РазмерПЗ(pos[1] - pos[2]); \
		al = длина(a); \
		a0 = a / al; \
		ar = лево(a0); \
		too_sharp = false; \
		if(!end) \
		{ \
			b = РазмерПЗ(pos[0] - pos[1]); \
			bl = длина(b); \
			b0 = b / bl; \
			a_b = a0 ^ b0; \
			axb = a0 % b0; \
			/* rseg = (axb + 1) / 2; */ \
			if(a_b < 0) /* sharp angle */ \
				up = a0 - b0; \
			else \
			{ \
				up.x = b0.y + a0.y; \
				up.y = -b0.x - a0.x; \
			} \
			double ua = a0 % up; \
			up /= (ua <= 1 && ua >= -1 ? (ua >= 0 ? 1 : -1) : ua); \
			ulen = -(up ^ a0); \
			if(miter == PathStyle::MITER_SHARP) \
			{ \
				ПРОВЕРЬ(a0 % up >= 0); \
				too_sharp = (a_b <= chamfer_ratio); \
				if(too_sharp) \
				{ \
					double cd = (axb >= 0 ? -chamfer_dist : chamfer_dist); \
					at = лево(a0) + a0 * cd; \
					bt = лево(b0) - b0 * cd; \
				} \
			} \
			else if(miter == PathStyle::MITER_FLAT) \
			{ /* calc flat miter */ \
				ТочкаПЗ mr = Unit(up); \
				if(tabs(mr % ar) <= 1e-5) \
					at = bt = mr; \
				else \
				{ \
					double k = (tabs(ar.x + mr.x) >= tabs(ar.y + mr.y) \
						? (ar.y - mr.y) / (ar.x + mr.x) : (mr.x - ar.x) / (ar.y + mr.y)); \
					at = mr + k * лево(mr); \
					bt = mr + k * право(mr); \
				} \
			} \
		} \
		else \
		{ \
			up.x = -a0.y; \
			up.y = +a0.x; \
			/* rseg = 0; */ \
		} \
		if(start) \
		{ \
			old_ulen = 0; \
			old_up.x = -a0.y; \
			old_up.y = +a0.x; \
		} \
	}

#define CALC_TOP_SHARP(t) \
	if(top_sharp) \
		new_top = next_top = pos[1] + PointfToSize(up * double(t)); \
	else \
	{ \
		new_top = pos[1] + PointfToSize(at * double(t)); \
		next_top = pos[1] + PointfToSize(bt * double(t)); \
	}

#define CALC_TOP_ROUND(t) \
	if(!top_sharp) \
		next_top = new_top = pos[1] + PointfToSize(up * double(t)); \
	else \
	{ \
		new_top.x = pos[1].x - fround((t) * a0.y); \
		new_top.y = pos[1].y + fround((t) * a0.x); \
		next_top.x = pos[1].x - fround((t) * b0.y); \
		next_top.y = pos[1].y + fround((t) * b0.x); \
		if(miter == PathStyle::MITER_ROUND) \
		{ \
			double ht = tabs((t)) - sqrt(max(0.0, (t) * (t) - вКвадрате(next_top - new_top) * 0.25)); \
			GetArcPath(arc_path, new_top, next_top, (t) >= 0 ? ht : -ht); \
		} \
	}

#define DRAW_TOP_SHARP(ln, ot) \
	ln(out, ot, new_top); \
	ln(out, new_top, next_top);

#define DRAW_TOP_ROUND(ln, ot) \
	ln(out, ot, new_top); \
	if(!arc_path.пустой()) \
		out.AddThick(ln, new_top, arc_path); \
	else \
		ln(out, new_top, next_top);

#define DRAW_BOTTOM_SHARP(b, col) \
	bool bot_sharp = (!too_sharp || axb > 0 || (b) < 0); \
	if(bot_sharp) \
		new_bot = next_bot = pos[1] + PointfToSize(up * double(b)); \
	else \
	{ \
		new_bot  = pos[1] + PointfToSize(at * double(b)); \
		next_bot = pos[1] + PointfToSize(bt * double(b)); \
	} \
	poly[px++] = new_top; \
	if(too_sharp) \
	{ \
		poly[px++] = next_top; \
		poly[px++] = next_bot; \
	} \
	poly[px++] = new_bot; \
	DrawPolygon(*draw, poly, px, col, 0, Null, 0);

#define DRAW_BOTTOM_ROUND(b, col) \
	if(top_sharp && arc_path.пустой()) \
		arc_path.добавь(next_top); \
	new_bot.x = pos[1].x - fround(b * a0.y); \
	new_bot.y = pos[1].y + fround(b * a0.x); \
	if(end || too_sharp) \
		arc_path.добавь(new_bot); \
	else if((b < 0) == (axb < 0)) \
		arc_path.добавь(next_bot = pos[1] + PointfToSize(up * double(b))); \
	else \
	{ \
		next_bot.x = pos[1].x - fround(b * b0.y); \
		next_bot.y = pos[1].y + fround(b * b0.x); \
		arc_path.добавь(next_bot); \
		double ht = tabs(b) - sqrt(max(0.0, b * b - вКвадрате(next_bot - new_bot) * 0.25)); \
		GetArcPath(arc_path, next_bot, new_bot, b <= 0 ? ht : -ht); \
	} \
	if(px) \
	{ \
		int pos = arc_path.дай—чЄт(); \
		arc_path.вставьН(pos, px); \
		копируй(&arc_path[pos], poly, poly + px); \
	} \
	arc_path.добавь(new_top); \
	DrawPolygon(*draw, arc_path, col, 0, Null, 0);

enum { LEADER_DELTA = 0 };

#define CALC_SHARP_LEADER(dest, y, ix) \
	{ \
		bool ll = (!start && (ix) <= (y) * old_ulen - LEADER_DELTA); \
		bool rl = (!end   && atotal - (ix) <= (y) * ulen - LEADER_DELTA); \
		if(ll && rl) \
			dest = pos[2] + PointfToSize(old_up * double(atotal / (old_ulen + ulen))); \
		else if(ll) \
			dest = pos[2] + PointfToSize(old_up * double(y)); \
		else if(rl) \
			dest = pos[1] + PointfToSize(up * double(y)); \
		else \
			dest = pos[2] + PointfToSize(a0 * double(ix) + ar * double(y)); \
	}

#define CALC_START_TOP_LEADER() \
	Точка old_top; \
	old_top = pos[1] - (apart + ar * t.left_top).AsSize();
/*
	bool start_top_leader = (!start && old_axb_left == (t.left_top > 0) && atotal - al <= t.left_top * old_ulen - LEADER_DELTA); \
	if(start_top_leader) \
		old_top = pos[1] - (apart + old_up * t.left_top).AsSize(); \
	else \
		old_top = pos[1] - (apart + ar * t.left_top).AsSize(); \
*/
#define CALC_START_BOTTOM_LEADER() \
	poly[px++] = pos[1] - (apart + ar * t.left_bottom).AsSize(); \
	poly[px++] = old_top;
/*
	bool start_bot_leader = (!start && old_axb_left == (t.left_bottom > 0) && atotal - al <= t.left_bottom * old_ulen - LEADER_DELTA); \
	if(start_bot_leader) \
		poly[px++] = pos[1] - (apart + old_up * t.left_bottom).AsSize(); \
	else \
		poly[px++] = pos[1] - (apart + ar * t.left_bottom).AsSize(); \
	if(t.left_ground && start_bot_leader != start_top_leader) \
		poly[px++] = pos[1] - apart.AsSize(); \
	poly[px++] = old_top;
*/

#define CALC_END_TOP_LEADER(rem) \
	Точка new_top; \
	new_top = pos[1] - (bpart + ar * t.right_top).AsSize();

/*
	bool end_top_leader = (!end && (axb < 0) == (t.right_top > 0) && al - (rem) <= t.right_top * ulen - LEADER_DELTA); \
	if(end_top_leader) \
		new_top = pos[1] - (bpart + up * t.right_top).AsSize(); \
	else \
		new_top = pos[1] - (bpart + ar * t.right_top).AsSize();
*/

#define CALC_END_BOTTOM_LEADER(rem) \
	poly[px++] = new_top; \
	poly[px++] = pos[1] - (bpart + ar * t.right_bottom).AsSize();
/*
	bool end_bot_leader = (!end && (axb < 0) == (t.right_bottom > 0) && al - (rem) <= t.right_bottom * ulen - LEADER_DELTA); \
	if(t.right_ground && end_top_leader != end_bot_leader) \
		poly[px++] = pos[1] - bpart.AsSize(); \
	if(end_bot_leader) \
		poly[px++] = pos[1] - (bpart + up * t.right_bottom).AsSize(); \
	else \
		poly[px++] = pos[1] - (bpart + ar * t.right_bottom).AsSize();
*/

void PathDraw::CalcLineBegin()
{
	CALC_MITER();
	for(int i = 0; i < segment_lines.дай—чЄт(); i++)
	{
		Linear& sl = segment_lines[i];
		bool top_sharp = (!too_sharp || axb < 0 || sl.top > 0);
		sl.last_top = pos[1] + PointfToSize((top_sharp ? up : bt) * double(sl.top));
		bool bot_sharp = (!too_sharp || axb > 0 || sl.bottom < 0);
		sl.last_bottom = pos[1] + PointfToSize((bot_sharp ? up : bt) * double(sl.bottom));
	}
	old_up = up;
	old_ulen = ulen;
	old_axb_left = (axb < 0);
}

bool PathDraw::DoBegin()
{
	if(пусто_ли(pos[2]))
		return false;

	разбей split = SplitLine();
	if((part_total += split.total) >= seg.length)
	{ // break segment
		Точка p0 = pos[0], p1 = pos[1], p2 = pos[2], p3 = pos[3], px;
		double t = minmax<double>(split.GetEndPos(part_total - seg.length), 0, 1);
		pos[1].x = pos[2].x + fround((pos[1].x - pos[2].x) * t);
		pos[1].y = pos[2].y + fround((pos[1].y - pos[2].y) * t);
		px = pos[1];
		if(pos[1] != pos[2])
		{
			pos[0] = (px == p1 ? p0 : p1);
			LineToRaw();
		}
		Restart();
		pos[3] = Null;
		pos[0] = p1;
		bool end_seg = (px != p1);
		if(end_seg)
		{
			pos[1] = px;
			pos[2] = (px == p2 ? Null : p2);
		}
		else
		{
			pos[1] = p2;
			pos[2] = p3;
		}
		if(!seg_segment.пустой() || !segment_lines.пустой())
		{
			SetSegment(seg_segment, PART_SEGMENT);
			CalcLineBegin();
		}
		else if(!seg_end.пустой())
			part = PART_SKIPEND;
		else
		{
			empty = true;
			return false;
		}
		добавьПоз(p0);
		if(seg_end.length > 0)
		{
			if(!end_seg)
				split_pos[0] = pos[1];
			else
			{
				split_pos[0] = pos[2];
				AddSplit();
			}
		}
		if(end_seg && part == PART_SEGMENT && seg_end.length <= 0)
			LineToRaw();
	}
	else
		LineToRaw();
	return true;
}

void PathDraw::LineToFull(Точка p)
{
	//RTIMING("PathDraw::LineToFull");
	if(p == raw_pos[0] || empty)
		return;
	добавьПоз(p);
	if(part == PART_SEGMENT)
	{
		if(seg_end.length <= 0)
		{
			LineToRaw();
			return;
		}
		else if(!пусто_ли(pos[2]))
		{
			AddSplit();
			FlushSplit(seg_end.length);
		}
		return;
	}

	switch(part)
	{
	case PART_OPEN:
		if(!пусто_ли(pos[2]))
		{
			close_line[0] = pos[2];
			close_line[1] = pos[1];
			close_line[2] = pos[0];
			part = PART_SEGMENT;
			CalcLineBegin();
		}
		return;

	case PART_BEGIN:
		if(!DoBegin())
			return;
		if(part != PART_SKIPEND)
			break;

	case PART_SKIPEND:
		{
			AddSplit();
			FlushSplit(seg_end.length);
		}
		break;
	}
}

PathDraw::разбей PathDraw::SplitLine()
{
	ПРОВЕРЬ(pos[0] != pos[1] && pos[1] != pos[2] && pos[2] != pos[3]);
	if(пусто_ли(pos[2]) || пусто_ли(pos[1]))
		return разбей(0, 0, 0, Null);
	CALC_MITER();
	double pos_start = (start ? 0 : max<double>(-(old_up ^ a0) * (old_axb_left ? seg.bottom : seg.top), 0));
	double pos_end   = (end ? 0 : max<double>((up ^ a0) * (axb < 0 ? seg.bottom : seg.top), 0));
	return разбей(pos_start, pos_end, pos_end + pos_start + al, pos[1]);
}

void PathDraw::LineToRaw()
{
	if(пусто_ли(pos[1]) || пусто_ли(pos[2]))
		return;

	ПРОВЕРЬ(part != PART_SKIPEND);
	ПРОВЕРЬ(pos[0] != pos[1] && pos[1] != pos[2] && pos[2] != pos[3]);
	CALC_MITER();

	if(!traces.пустой())
	{
		double pos_start = (start ? 0 : (old_up ^ a0) * (old_axb_left ? seg.bottom : seg.top));
		double pos_end   = al + (end ? 0 : (up ^ a0) * (axb < 0 ? seg.bottom : seg.top));
		double atotal = al, adelta = pos_end - pos_start, aindex = pos_start;

		for(int o = open_traces.дай—чЄт(); --o >= 0;)
		{
			Trace& t = traces[open_traces[o]];
			Output& out = outputs[t.Индекс];
			out.CheckFlush();
			if(adelta <= t.width - t.pos)
			{ // segment spans the whole line - draw joint
				double top = t.left_top + t.top_step * (t.pos += adelta);
				Точка new_top, next_top;
				bool top_sharp = (!too_sharp || axb < 0 || t.left_top > 0);
				CALC_TOP_SHARP(top);
				if(t.hline)
				{ // single line
					DRAW_TOP_SHARP(t.hline, t.last_top);
				}
				else
				{
					Точка poly[6];
					Точка next_bot, new_bot;
					double bottom = t.left_bottom + t.bottom_step * t.pos;
					poly[0] = t.last_bottom;
					poly[1] = t.last_top;
					int px = 2;
					DRAW_BOTTOM_SHARP(bottom, t.color);
					t.last_bottom = next_bot;
				}
				t.last_top = next_top;
/*
				double top = t.left_top + t.top_step * (t.pos += adelta);
				Точка new_top = pos[1] + (top * up).AsSize();
				if(t.hline)
				{
					out.AddFirst(t.last_top);
					out.AddNext(new_top);
				}
				else
				{
					double bottom = t.left_bottom + t.bottom_step * t.pos;
					Точка poly[6];
					poly[0] = t.last_bottom;
					poly[1] = t.last_top;
					Точка new_bot = pos[1] + (up * bottom).AsSize();
					poly[2] = new_top;
					poly[3] = new_bot;
					DrawPolygon(*draw, poly, 4, t.color, 0, Null, 0);
					t.last_bottom = new_bot;
				}
				t.last_top = new_top;
*/
			}
			else
			{ // finish segment
				double end_pos = pos_start + t.width - t.pos;
				Точка new_top;
				CALC_SHARP_LEADER(new_top, t.right_top, end_pos);
				if(t.hline)
					t.hline(out, t.last_top, new_top);
				else
				{
					Точка poly[6];
					poly[0] = t.last_bottom;
					poly[1] = t.last_top;
					Точка new_bot;
					CALC_SHARP_LEADER(new_bot, t.right_bottom, end_pos);
					poly[2] = new_top;
					poly[3] = new_bot;
					DrawPolygon(*draw, poly, 4, t.color, 0, Null, 0);
				}
				open_traces.удали(o);
			}
		}

		while((aindex += skip_size) < pos_end)
		{
			double left = pos_end - aindex;
			int tix = next_index++;
//			bool skip = (done.найди(next_index) >= 0);
			Trace& t = traces[tix];
			Output& out = outputs[t.Индекс];
			skip_size = (next_index == seg.end
				? (part == PART_SEGMENT
					? seg.length + traces[next_index = seg.start].start - t.start : 1e100)
				: traces[next_index].start - t.start);
//			if(skip)
//				continue;
			Точка old_top;
			CALC_SHARP_LEADER(old_top, t.left_top, aindex);
			Точка new_top;
			if(left >= t.width)
			{ // full trace fits here
				CALC_SHARP_LEADER(new_top, t.right_top, aindex + t.width);
				if(t.hline)
					t.hline(out, old_top, new_top);
				else
				{
					Точка poly[4];
					CALC_SHARP_LEADER(poly[0], t.right_bottom, aindex + t.width);
					CALC_SHARP_LEADER(poly[1], t.left_bottom, aindex);
					poly[2] = old_top;
					poly[3] = new_top;
					DrawPolygon(*draw, poly, 4, t.color, 0, Null, 0);
				}
			}
			else
			{ // split trace & add to open list
				double top = t.left_top + t.top_step * (t.pos = left);
				Точка new_top, next_top;
				bool top_sharp = (!too_sharp || axb < 0 || t.left_top > 0);
				CALC_TOP_SHARP(top);
				if(t.hline)
				{ // single line
					DRAW_TOP_SHARP(t.hline, old_top);
				}
				else
				{
					Точка poly[6];
					Точка next_bot, new_bot;
					double bottom = t.left_bottom + t.bottom_step * t.pos;
					CALC_SHARP_LEADER(poly[0], t.left_bottom, aindex);
					poly[1] = old_top;
					int px = 2;
					DRAW_BOTTOM_SHARP(bottom, t.color);
					t.last_bottom = next_bot;
				}
				t.last_top = next_top;
/*
				Точка new_top = pos[1] + (up * top).AsSize();
				if(t.hline)
				{
					out.AddFirst(old_top);
					out.AddNext(new_top);
				}
				else
				{
					Точка poly[4];
					double bottom = t.left_bottom + t.bottom_step * t.pos;
					poly[0] = pos[1] + (up * bottom).AsSize();
					CALC_SHARP_LEADER(poly[1], t.left_bottom, aindex);
					poly[2] = old_top;
					poly[3] = new_top;
					DrawPolygon(*draw, poly, 4, t.color, 0, Null, 0);
					t.last_bottom = poly[0];
				}
				t.last_top = new_top;
*/				open_traces.добавь(tix);
			}
#ifdef DEBUG_DRAW
			FlushLines();
			GdiFlush();
#endif
		}
		skip_size = aindex - pos_end;
	}


/*
		bool try_rep = (al >= 2 * segment);
		for(int o = open_traces.дай—чЄт(); --o >= 0;)
		{
			int x = open_traces[o];
//			done.добавь(x);
			Trace& t = traces[x];
			Output& out = outputs[t.Индекс];
			out.CheckFlush();
			if(al <= t.width - t.pos)
			{ // segment spans the whole line - draw joint
				double top = t.left_top + t.top_step * (t.pos += al);
				Точка new_top, next_top;
				if(miter == PathStyle::MITER_SHARP)
				{
					bool top_sharp = (!too_sharp || (axb < 0) == (top >= 0));
					CALC_TOP_SHARP(top)
					if(t.hline)
					{
						DRAW_TOP_SHARP(t.hline, t.last_top);
					}
					else
					{
						Точка new_bot, next_bot;
						double bottom = t.left_bottom + t.bottom_step * t.pos;
						Точка poly[6];
						poly[0] = t.last_bottom;
						poly[1] = t.last_top;
						int px = 2;
						DRAW_BOTTOM_SHARP(bottom, t.color);
						t.last_bottom = next_bot;
					}
				}
				else
				{
					ПРОВЕРЬ(miter == PathStyle::MITER_ROUND || miter == PathStyle::MITER_FLAT);
					¬ектор<Точка> arc_path;
					bool top_sharp = (!end && (axb < 0) != (top >= 0));
					CALC_TOP_ROUND(top)
					if(t.hline)
					{
						DRAW_TOP_ROUND(t.last_top);
					}
					else
					{
						Точка poly[2];
						poly[0] = t.last_bottom;
						poly[1] = t.last_top;
						enum { px = 2 };
						Точка new_bot, next_bot;
						double bottom = t.left_bottom + t.bottom_step * t.pos;
						DRAW_BOTTOM_ROUND(bottom, t.color);
						t.last_bottom = next_bot;
					}
				}
				t.last_top = next_top;
			}
			else
			{ // segment is shorter, just calculate end & join
				double rem = t.width - t.pos;
				ТочкаПЗ bpart = a0 * (al - rem);
				CALC_END_TOP_LEADER(rem);
				if(t.hline)
				{
					out.AddFirst(t.last_top);
					out.AddNext(new_top);
				}
				else
				{
					Точка poly[8];
					poly[0] = t.last_bottom;
					poly[1] = t.last_top;
					int px = 2;
					CALC_END_BOTTOM_LEADER(rem);
					DrawPolygon(*draw, poly, px, t.color, 0, Null, 0);
				}
//				if(al > segment - t.pos + 1)
//				{ // repeat segment
//
//				}

				open_traces.удали(o);
			}
#ifdef DEBUG_DRAW
			FlushLines();
			GdiFlush();
#endif
		}

		double atotal = al;
		while(skip_size <= al)
		{
			al -= skip_size;
			int tix = next_index++;
//			bool skip = (done.найди(next_index) >= 0);
			Trace& t = traces[tix];
			Output& out = outputs[t.Индекс];
			if(next_index == end_index)
				skip_size = (repeat ? segment + traces[next_index = seg_segment.start].start - t.start : 1e100);
			else
				skip_size = traces[next_index].start - t.start;
//			if(skip)
//				continue;
			ТочкаПЗ apart = a0 * al;
			CALC_START_TOP_LEADER();
			if(al >= t.width)
			{ // full trace fits here
				ТочкаПЗ bpart = a0 * (al - t.width);
				CALC_END_TOP_LEADER(t.width);
				if(t.hline)
				{
					out.AddFirst(old_top);
					out.AddNext(new_top);
				}
				else
				{
					Точка poly[6];
					int px = 0;
					CALC_START_BOTTOM_LEADER();
					CALC_END_BOTTOM_LEADER(t.width);
					DrawPolygon(*draw, poly, px, t.color, 0, Null, 0);
				}
			}
			else
			{ // split trace & add to open list
				double top = t.left_top + t.top_step * (t.pos = al);
				Точка new_top, next_top;
				if(miter == PathStyle::MITER_SHARP)
				{
					bool top_sharp = (!too_sharp || (axb < 0) == (top >= 0));
					CALC_TOP_SHARP(top);
					if(t.hline)
					{
						DRAW_TOP_SHARP(t.hline, old_top);
					}
					else
					{
						Точка poly[8];
						int px = 0;
						CALC_START_BOTTOM_LEADER();
						double bottom = t.left_bottom + t.bottom_step * t.pos;
						Точка new_bot, next_bot;
						DRAW_BOTTOM_SHARP(bottom, t.color);
						t.last_bottom = next_bot;
					}
				}
				else
				{
					ПРОВЕРЬ(miter == PathStyle::MITER_ROUND || miter == PathStyle::MITER_FLAT);
					¬ектор<Точка> arc_path;
					bool top_sharp = (!end && (axb < 0) != (top >= 0));
					CALC_TOP_ROUND(top);
					if(t.hline)
					{
						DRAW_TOP_ROUND(old_top);
					}
					else
					{
						Точка poly[4];
						int px = 0;
						CALC_START_BOTTOM_LEADER();
						double bottom = t.left_bottom + t.bottom_step * t.pos;
						Точка new_bot, next_bot;
						DRAW_BOTTOM_ROUND(bottom, t.color);
						t.last_bottom = next_bot;
					}
				}
				t.last_top = next_top;
				open_traces.добавь(tix);
			}
#ifdef DEBUG_DRAW
			FlushLines();
			GdiFlush();
#endif
		}
		skip_size -= al;
*/

	if(part == PART_SEGMENT)
	{
		for(int i = 0; i < segment_lines.дай—чЄт(); i++)
		{
			Linear& sl = segment_lines[i];
			Output& out = outputs[sl.Индекс];
			out.CheckFlush();
			if(sl.ground)
			{ // simple case
				sl.hline(out, pos[2], pos[1]);
			}
			else if(miter == PathStyle::MITER_SHARP)
			{ // complex case
				Точка old_top = start ? pos[2] + PointfToSize(ar * double(sl.top)) : sl.last_top;
				Точка new_top, next_top;
				bool top_sharp = (!too_sharp || axb < 0 || sl.top > 0);
				CALC_TOP_SHARP(sl.top);
				if(sl.hline)
				{ // single line
					DRAW_TOP_SHARP(sl.hline, old_top);
				}
				else
				{
					Точка poly[6];
					Точка next_bot, new_bot;
					poly[0] = start ? pos[2] + PointfToSize(ar * double(sl.bottom)) : sl.last_bottom;
					poly[1] = old_top;
					int px = 2;
					DRAW_BOTTOM_SHARP(sl.bottom, sl.color);
					sl.last_bottom = next_bot;
				}
				sl.last_top = next_top;
			}
			else if(miter == PathStyle::MITER_ROUND)
			{
				Точка new_top, next_top;
				Точка old_top = start ? pos[2] + PointfToSize(ar * double(sl.top)) : sl.last_top;
				¬ектор<Точка> arc_path;
				bool top_sharp = (!end && (axb < 0) == (sl.top >= 0));
				CALC_TOP_ROUND(sl.top);
				if(sl.hline)
				{ // single line
					DRAW_TOP_ROUND(sl.hline, old_top);
				}
				else
				{
					Точка poly[2];
					poly[0] = (start ? pos[2] + PointfToSize(ar * double(sl.bottom)) : sl.last_bottom);
					poly[1] = old_top;
					enum { px = 2 };
					Точка new_bot, next_bot;
					DRAW_BOTTOM_ROUND(sl.bottom, sl.color);
					sl.last_bottom = next_bot;
				}
				sl.last_top = next_top;
			}
			else if(miter == PathStyle::MITER_FLAT)
			{
				Точка old_top = start ? pos[2] + PointfToSize(ar * double(sl.top)) : sl.last_top;
				Точка new_top, next_top;
				bool top_sharp = (!end && (axb < 0) == (sl.top >= 0));
				if(top_sharp)
				{
					new_top = pos[1] + PointfToSize(at * double(sl.top));
					next_top = pos[1] + PointfToSize(bt * double(sl.top));
				}
				else
					new_top = next_top = pos[1] + PointfToSize(up * double(sl.top));
				if(sl.hline)
				{
					sl.hline(out, old_top, new_top);
					sl.hline(out, new_top, next_top);
				}
				else
				{
					Точка poly[6];
					poly[0] = (start ? pos[2] + PointfToSize(ar * double(sl.bottom)) : sl.last_bottom);
					poly[1] = old_top;
					poly[2] = new_top;
					int px = 3;
					if(top_sharp)
						poly[px++] = next_top;
					bool bot_sharp = (!too_sharp || axb > 0 || sl.bottom < 0);
					if(bot_sharp)
					{
						poly[px++] = sl.last_bottom = pos[1] + PointfToSize(bt * double(sl.bottom));
						poly[px++] = pos[1] + PointfToSize(at * double(sl.bottom));
					}
					else
						poly[px++] = sl.last_bottom = pos[1] + PointfToSize(up * double(sl.bottom));
					DrawPolygon(*draw, poly, px, sl.color, 0, Null, 0);
				}
				sl.last_top = next_top;
			}
			else
				NEVER();
#ifdef DEBUG_DRAW
			FlushLines();
			GdiFlush();
#endif
		}
	}

	if(calc_miter)
	{
		old_up = up;
		old_ulen = ulen;
		old_axb_left = (axb < 0);
	}
}

void PathDraw::рисуй()
{
	MoveTo(Null);
	FlushLines();
	Restart();
}

void PathDraw::FlushLines()
{
	for(int i = 0; i < outputs.дай—чЄт(); i++)
		outputs[i].слей();
}

void PathDraw::RecurseArc(Точка next, int length, int bulge, int levels)
{
	if(--levels < 0 || tabs(bulge) <= 1 || length <= 2)
		LineTo(next); // degenerate next line
	else
	{ // bisect arc
		Размер normal = iscale(Размер(next.y - pos[0].y, pos[0].x - next.x), bulge, length);
		Точка centre = ((pos[0] + next) >> 1) + normal;
		int ll = (int)длина(centre - next);
		int hh = iscale(bulge, ll, 2 * ll + length);
		RecurseArc(centre, ll, hh, levels);
		RecurseArc(next, ll, hh, levels);
	}
}

void PathDraw::ArcTo(Точка pt, int bulge)
{
	ПРОВЕРЬ(!пусто_ли(raw_pos[0]));
	RecurseArc(pt, (int)длина(pt - pos[0]), bulge, 10);
}

void PathDraw::Circle(Точка pt, int radius)
{
	MoveTo(pt.x - radius, pt.y);
	ArcTo(pt.x + radius, pt.y, radius);
	ArcTo(pt.x - radius, pt.y, radius);
}

PathStyleDisplay::PathStyleDisplay(const PathStyleMap *path_map) : path_map(path_map) {}
PathStyleDisplay::~PathStyleDisplay() {}

void PathStyleDisplay::рисуй(Draw& draw, const Прям& rc, const Значение& v, Цвет i, Цвет p, dword s) const
{
	draw.DrawRect(rc, p);
	if(пусто_ли(v) || ткст_ли(v))
	{
		“кст s = v;
		PathStyle style = path_map->скан(s);
		PathDraw pd;
		pd.уст(draw, style, i, rc.устВысоту() >> 2, rc.устВысоту() >> 2, false);
		int mid = (rc.top + rc.bottom) >> 1;
		pd.MoveTo(rc.left, mid);
		pd.LineTo(rc.right, mid);
		pd.рисуй();
	}
}

}
