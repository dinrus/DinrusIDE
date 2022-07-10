#include "GeomDraw.h"

namespace ���� {

#define LLOG(x) // LOG(x)

enum { THICK = 5, THICKDOT = 20 };
extern void (*PathDraw_Output_Thick[THICK][THICK])(PathDraw::Output& out, ����� a, ����� b);

static const char *ParseCString(const char *p, ����& out)
{
	try
	{
		������� parser(p);
		out = parser.�����1����();
		return parser.����();
	}
	catch(�������::������ e)
	{
		throw ����(e);
	}
}

static ���� FormatPrec(double d)
{
	return ���("%+.*g", PathStyle::PREC, d);
}

//////////////////////////////////////////////////////////////////////
// PathStyle::Trace::

���� PathStyle::Trace::Encode() const
{
	���� out;
	out << FormatPrec(left) << FormatPrec(left_top) << FormatPrec(left_bottom)
		<< FormatPrec(right) << FormatPrec(right_top) << FormatPrec(right_bottom);
	if(!�����_��(color))
		out << ���("*%02x%02x%02x", color.���(), color.���(), color.���());
	return out;
}

const char *PathStyle::Trace::Decode(const char *s)
{
	if(�����_��(left  = ������(s, &s)) || �����_��(left_top  = ������(s, &s)) || �����_��(left_bottom  = ������(s, &s))
	|| �����_��(right = ������(s, &s)) || �����_��(right_top = ������(s, &s)) || �����_��(right_bottom = ������(s, &s)))
		return 0;
	if(*s == '*')
	{ // color
		for(int i = 1; i <= 6; i++)
			if(!IsXDigit(s[i]))
				return 0; // ������
		color = ����(16 * ctoi(s[1]) + ctoi(s[2]), 16 * ctoi(s[3]) + ctoi(s[4]), 16 * ctoi(s[5]) + ctoi(s[6]));
		s += 7;
	}
	return s;
}

void PathStyle::Trace::����������(�����& stream)
{
	���� s;
	if(stream.�����������())
		s = Encode();
	stream % s;
	if(stream.��������())
	{
		const char *p = Decode(s);
		if(!p || *p)
			stream.����();
	}
}

bool PathStyle::Trace::�����(const Trace& t) const
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
		double rl = (l - left) / ��������(), rr = (right - r) / ��������();
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

void PathStyle::Trace::��������(������� amount)
{
	left -= amount.x;
	right += amount.x;
	left_top -= amount.y;
	right_top -= amount.y;
	left_bottom += amount.y;
	right_bottom += amount.y;
}

bool PathStyle::Trace::��������(������� pt) const
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

double PathStyle::Trace::GetDistance(������� pt) const
{
	double dl = (pt.y < left_top  ? pt | LeftTop()  : pt.y > left_bottom  ? pt | LeftBottom()  : tabs(pt.x - left));
	double dr = (pt.y < right_top ? pt | RightTop() : pt.y > right_bottom ? pt | RightBottom() : tabs(pt.x - right));
	double dt = ����������(pt, LeftTop(), RightTop());
	double db = ����������(pt, LeftBottom(), RightBottom());
	return min(min(dl, dr), min(dt, db));
}

static bool CheckTrack(������� A, ������� B, ������� tolerance)
{
	return tabs(A.x - B.x) <= tolerance.x && tabs(A.y - B.y) <= tolerance.y;
}

int PathStyle::Trace::GetTrackStyle(������� pt, ������� tolerance, �������& start) const
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

������� PathStyle::Trace::GetTrackCursor(int style)
{
	switch(style)
	{
	case LEFT | LEFT_TOP:      return �������::SizeTopLeft();
	case RIGHT | RIGHT_BOTTOM: return �������::SizeBottomRight();
	case RIGHT | RIGHT_TOP:    return �������::SizeTopRight();
	case LEFT | LEFT_BOTTOM:   return �������::SizeBottomLeft();
	case LEFT_TOP | RIGHT_TOP:
	case LEFT_BOTTOM | RIGHT_BOTTOM:
		return �������::SizeVert();

	case LEFT:
	case RIGHT:
		return �������::SizeHorz();
	}
	return Null;
}

void PathStyle::Trace::�����(PlotterTool& tool, bool reduce, ���� outline) const
{
	bool he = IsHorzEmpty();
	bool ve = IsVertEmpty();

	if(he && ve)
	{ // point
		Plotter& plot = tool.GetPlotter();
		if(plot.InLClip(LeftTop()))
		{
			����� pt = plot.LtoPoint(LeftTop());
			plot.draw->DrawRect(pt.x - 4, pt.y - 4, 9, 9, outline);
		}
	}
	else if(reduce && (he || ve))
	{
		tool.MoveTo(left, ����());
		tool.LineTo(right, ���());
		tool.�����();
	}
	else
	{
		tool.MoveTo(left,  left_top);
		tool.LineTo(right, right_top);
		tool.LineTo(right, right_bottom);
		tool.LineTo(left,  left_bottom);
		tool.LineTo(left,  left_top);
		tool.�����();
	}
}

PathStyle::Trace& PathStyle::Trace::Track(������� delta, int style)
{
	if(style & LEFT)         left         += delta.x;
	if(style & LEFT_TOP)     left_top     += delta.y;
	if(style & RIGHT_TOP)    right_top    += delta.y;

	if(style & RIGHT)        right        += delta.x;
	if(style & LEFT_BOTTOM)  left_bottom  += delta.y;
	if(style & RIGHT_BOTTOM) right_bottom += delta.y;

	if(left > right)
	{
		��������(left, right);
		��������(left_top, right_top);
		��������(left_bottom, right_bottom);
	}
	if(left_top > left_bottom)
		��������(left_top, left_bottom);
	if(right_top > right_bottom)
		��������(right_top, right_bottom);
	return *this;
}

PathStyle::Trace& PathStyle::Trace::�����(const ���쏇& rc)
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

void PathStyle::Clip::����������(�����& stream)
{
	int version = StreamHeading(stream, 1, 1, 1, "PathStyle::Clip");
	if(version >= 1)
		stream % traces;
}

���쏇 PathStyle::Clip::GetExtent() const
{
	���쏇 rc = Null;
	for(int i = 0; i < traces.�������(); i++)
		rc |= traces[i].GetExtent();
	return rc;
}

//////////////////////////////////////////////////////////////////////
// PathStyle::

PathStyle::PathStyle(pick_ PathStyle& style)
: PathStyleMisc(style), traces(style.traces)
{
	�������(&style != &PathStyle::solid());
}

PathStyle& PathStyle::operator = (pick_ PathStyle& style)
{
	�������(&style != &PathStyle::solid());
	PathStyleMisc::operator = (style);
	traces = style.traces;
	return *this;
}

bool PathStyle::IsSolid(double& wd, ����& co) const
{
	double tol = 1e-6;
	if(traces.�������() != 1 || tabs(begin) > tol || tabs(end) > tol)
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

���� PathStyle::Encode() const
{
	���� out;
	out << 'p' << FormatPrec(width)
		<< FormatPrec(begin) << FormatPrec(segment) << FormatPrec(end)
		<< '<';
	for(int i = 0; i < traces.�������(); i++)
		out.������(traces[i].Encode());
	out << '>' << (miter == MITER_ROUND ? 'r' : miter == MITER_SHARP ? 's' : 'f');
	if(miter == MITER_SHARP)
		out << FormatPrec(chamfer);
	return out;
}

const char *PathStyle::Decode(const char *s)
{
	if(*s++ == 'p'
		&& !�����_��(width   = ������(s, &s))
		&& !�����_��(begin   = ������(s, &s))
		&& !�����_��(segment = ������(s, &s))
		&& !�����_��(end     = ������(s, &s))
		&& *s++ == '<')
	{
		while(*s == '+' || *s == '-')
		{
			Trace t;
			if((s = t.Decode(s)) == 0)
				return 0;
			traces.������(t);
		}
		if(*s++ == '>' && (*s == 'r' || *s == 's' || *s == 'f'))
		{
			chamfer = STD_CHAMFER;
			miter = (*s == 'r' ? MITER_ROUND : *s == 's' ? MITER_SHARP : MITER_FLAT);
			s++;
			if(miter != MITER_SHARP || !�����_��(chamfer = ������(s, &s)))
				return s;
		}
	}
	return 0;
}

void PathStyle::����������(�����& stream)
{
	���� s;
	if(stream.�����������())
		s = Encode();
	stream % s;
	if(stream.��������() && !Decode(s))
		stream.����();
}

const PathStyle& PathStyle::empty()
{
	static PathStyle p;
	return p;
}

const PathStyle& PathStyle::solid()
{
	static PathStyle p;
	if(p.������())
		p.traces << PathStyle::Trace(0, -0.5, +0.5, 1, -0.5, +0.5);
	return p;
}

const PathStyle& PathStyle::dash()
{
	static PathStyle p;
	if(p.������())
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
	if(p.������())
	{
		p.segment = 2;
		p.traces << PathStyle::Trace(0.5, -0.5, +0.5, 1.5, -0.5, +0.5);
	}
	return p;
}

const PathStyle& PathStyle::dash_dot()
{
	static PathStyle p;
	if(p.������())
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
	if(p.traces.������())
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

void PathStyleMap::����������(�����& stream)
{
	int version = 1;
	stream / version % map;
	if(stream.��������())
		sort.������();
}

���� PathStyleMap::Export() const
{
	���� out;
	out << "# PathStyleMap version 1\n"
		"$" << �������(���) << "\n";
	for(int i = 0; i < map.�������(); i++)
		out << �������(map.������(i)) << '=' << map[i].Encode() << '\n';
	return out;
}

void PathStyleMap::Import(���� s, bool update_existing)
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
			{ // style ���
				if(*++p != '\"')
					throw ����(����().������() << "o�ek�v�n znak \", nalezeno: " << StringSample(p, 10));
				p = ParseCString(p + 1, ���);
			}
			else if(*p == '\"')
			{
				���� sn;
				p = ParseCString(p + 1, sn);
				if(*p != '=')
					throw ����(����().������() << "o�ek�v�no '=', nalezeno: " << StringSample(p, 10));
				if(�����_��(sn))
					throw ����(����().������() << "n�zev stylu je pr�zdn�");
				for(p++; *p && *p != '\n' && (byte)*p <= ' '; p++)
					;
				PathStyle ps;
				p = ps.Decode(p);
				if(!p)
					throw ����(����().������() << "neplatn� definice stylu '" << ��� << "'");
				if(map.�����(sn) < 0 || update_existing)
					���(sn, ps);
			}
			else
				throw ����(����().������() << "neplatn� za��tek ��dku: " << StringSample(p, 10));
	}
	catch(���� e)
	{
		throw ����(����().������() << "��dek " << line << ": " << e);
	}
}

const ������<int>& PathStyleMap::GetSort() const
{
	if(sort.������() && !map.������())
		sort = GetSortOrder(map.�������(), GetLanguageInfo());
	return sort;
}

PathStyle PathStyleMap::����(���� ���, const PathStyle& dflt) const
{
	if(�����_��(���))
		return PathStyle(dflt, 0);
	if(this)
	{
		int i = map.�����(���);
		if(i >= 0)
			return PathStyle(map[i], 0);
	}
	PathStyle ps;
	const char *end = ps.Decode(���);
	if(!end || *end)
		return PathStyle(dflt, 0);
	return ps;
}

int PathStyleMap::FindSortName(���� ���, int exclude) const
{
	for(int i = 0; i < �������(); i++)
		if(i != exclude && GetSortName(i) == ���)
			return i;
	return -1;
}

���� PathStyleMap::GetUniqueName(���� prefix) const
{
	int max_index = -1;
	int pl = prefix.�������();
	for(int i = 0; i < �������(); i++)
	{
		���� s = map.������(i);
		if(s.�������() >= pl + 2 && !memcmp(s, prefix, pl))
		{
			const char *p = s.�����() + pl;
			if(*p == ' ' && �����_��(*++p))
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
	return prefix + ' ' + �����(max_index + 1);
}

PathStyleMap& PathStyleMap::App()
{
	static PathStyleMap psm;
	if(psm.������())
	{
		psm.���(".empty",        PathStyle::empty());
		psm.���(".solid",        PathStyle::solid());
		psm.���(".dash",         PathStyle::dash());
		psm.���(".dot",          PathStyle::dot());
		psm.���(".dash_dot",     PathStyle::dash_dot());
		psm.���(".dash_dot_dot", PathStyle::dash_dot_dot());
	}
	return psm;
}

static void RecurseArc(������<�����>& out, ������� begin, ������� end, double l, double h, int maxvert)
{
	if(maxvert <= 1 || tabs(h) <= 1 || l <= 1)
	{
		out.������(PointfToPoint(end));
		return;
	}
	������� normal = ����(end - begin) * (h / l);
	������� centre = ��������(begin, end) + normal;
	double ll = centre | end;
	double hh = h * ll / (2 * ll + l);
	maxvert >>= 1;
	RecurseArc(out, begin,  centre, ll, hh, maxvert);
	RecurseArc(out, centre, end,    ll, hh, maxvert);
}

static ������<�����>& GetArcPath(������<�����>& out, ����� begin, ����� end, double height, int maxvert = 256)
{
	RecurseArc(out, begin, end, hypot(begin.x - end.x, begin.y - end.y), height, maxvert);
	return out;
}

static ������<�����> GetArcPath(����� begin, ����� end, double height, int maxvert = 256)
{
	������<�����> out;
	return GetArcPath(out, begin, end, height, maxvert);
}

void PathDraw::Output::AddNext(const ������<�����>& list)
{
	����� old = vertices.����();
	int c = vertices.�������();
	for(const ����� *p = list.�����(), *e = list.����(); p != e; ++p)
		if(*p != old)
			vertices.������(old = *p);
	counts.����() += vertices.�������() - c;
}

void PathDraw::Output::AddFirst(����� pt)
{
	if(!counts.������() && counts.����() == 1)
		vertices.����() = pt;
	else if(vertices.������() || vertices.����() != pt)
	{
		vertices.������(pt);
		counts.������(1);
	}
}

void PathDraw::Output::AddThick(void (*ln)(Output& out, ����� a, ����� b), ����� start, const ������<�����>& list)
{
	if(!list.������())
	{
		ln(*this, start, list[0]);
		for(const ����� *p = list.�����() + 1, *e = list.����(); p < e; p++)
			ln(*this, p[-1], p[0]);
	}
}

����� *PathDraw::Output::AddSeg(int segments, int seg_count)
{
	int c = vertices.�������();
	int n = counts.�������();
	if(c > 0 && counts[n - 1] == 1)
		c--, n--;
	vertices.������(c + segments * seg_count);
	counts.������(n + segments);
	Fill(counts.�����() + n, counts.����(), seg_count);
	return vertices.�����() + c;
}

����� *PathDraw::Output::AddSeg(int count)
{
	int c = vertices.�������();
	int n = counts.�������();
	if(n > 0 && counts[n - 1] == 1)
	{
		c--;
		counts[n - 1] = count;
	}
	else
		counts.������(count);
	vertices.������(c + count);
	return &vertices[c];
}

void PathDraw::Output::����()
{
	int cc = counts.�������();
	if(cc == 0)
		return;
	int vc = vertices.�������();
	if(counts[cc - 1] == 1)
	{
		if(--cc == 0)
			return;
		vc--;
	}
	draw.DrawPolyPolyline(vertices.�����(), vc, counts.�����(), cc, width, color);
	����� end = vertices.����();
	vertices.������(1);
	vertices[0] = end;
	counts.������(1);
	counts[0] = 1;
}

PathDraw::PathDraw()
{
	moveto = 0;
	lineto = 0;
}

PathDraw::PathDraw(Draw& _draw, const PathStyle& _style, ���� _color, double _width, double _dash, bool _closed)
{
	���(_draw, _style, _color, _width, _dash, _closed);
}

void PathDraw::������()
{
	draw = 0;
	style = 0;
	split_buffer.������();
	outputs.������();
	segment_lines.������();
	open_traces.������();
	traces.������();
}

void PathDraw::AddSegment(const PathStyle::Trace& trace, bool infinite)
{
	int ltop, lbot;
	ltop = fround(width * trace.����());
	lbot = fround(width * trace.���());
	if(width * trace.LeftHeight() > 1)
	{
		ltop = fround(width * trace.left_top);
		lbot = ltop + fround(width * trace.LeftHeight());
	}
	else
		ltop = lbot = fround(width * (trace.left_top + trace.left_bottom) / 2);
	���� c = Nvl(trace.color, std_color);
/*
	int   wd =
	int x = outputs.�����(c);
	if(x < 0 || infinite)
	{
		x = outputs.�������();
		outputs.������(c, new Output(c, *draw));
	}
*/
	if(infinite)
	{
		Linear& ln = segment_lines.������();
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
		for(x = outputs.�����(c); x >= 0 && outputs[x].width != w; x = outputs.���������(c))
			;
		if(x < 0)
		{
			x = outputs.�������();
			outputs.������(c, new Output(c, w, *draw));
		}
		ln.������ = x;
	}
	else
	{
		if(traces.�������() == seg.start)
		{
			seg.top = ltop;
			seg.bottom = lbot;
		}
		else
		{
			seg.top = min(seg.top, ltop);
			seg.bottom = max(seg.bottom, lbot);
		}

		Trace& t = traces.������();
		t.left_top = ltop;
		t.left_bottom = lbot;
		t.right_top = fround(width * trace.right_top);
		t.right_bottom = t.right_top + fround(width * trace.RightHeight());
		double wd = dash * trace.��������();
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
		for(x = outputs.�����(c); x >= 0 && outputs[x].width != sw; x = outputs.���������(x))
			;
		if(x < 0)
		{
			x = outputs.�������();
			outputs.������(c, new Output(c, sw, *draw));
		}
		t.������ = x;
		calc_miter = true;
	}
}

void PathDraw::���(Draw& _draw, const PathStyle& _style, ���� _color, double _width, double _dash, bool _closed)
{
	//RTIMING("PathDraw::���");

	�������(_dash != 0);

	������();

	draw = &_draw;

	style = &_style;
	std_color = _color;
	width = _width;
	double avg = GetAvgPixelsPerDot(*draw);
	if(width < 0)
		width *= -avg;
	if(width < 1)
		width = 1;
	LLOG("PathDraw::���: pixels = " << draw->Pixels() << ", avg pixels per dot = " << avg << ", px width = " << width);
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
	clip_rect.��������(clip_radius);
	closed = _closed;
	calc_miter = false;

	������<PathStyle::Trace> bseg, sseg, eseg;
	������<int> overlap_begin, overlap_end;
	double prec = 1.0 / width;
	double endpos = style->begin + style->segment;
	double bsub = style->begin - prec, badd = style->begin + prec;
	double ssub = endpos - prec, sadd = endpos + prec;

	{ // split style traces
		double prec2 = sqr(prec);
		double total = endpos + style->end;
		int i;
		for(i = 0; i < style->traces.�������(); i++)
		{
			const PathStyle::Trace& t = style->traces[i];
			if(t.left <= bsub)
				bseg.������(t.GetCropRel(0, style->begin));
			if(t.left <= ssub && t.right >= badd)
				sseg.������(t.GetCropRel(style->begin, endpos));
			if(t.right >= sadd)
				eseg.������(t.GetCropRel(endpos, total));
		}

		// generate overlaps
/*
		for(i = 0; i < sseg.�������() && sseg[i].left <= prec; i++)
		{
			const PathStyle::Trace& beg = sseg[i];
			for(int j = i; ++j < sseg.�������();)
			{
				const PathStyle::Trace& end = sseg[j];
				if(end.color == beg.color
				&& end.left > beg.right && end.right >= ssub
				&& tabs(beg.left_top - end.right_top) <= prec
				&& tabs(beg.left_bottom - end.right_bottom) <= prec
				&& tabs(end.��������() * beg.TopHeight() - beg.��������() * end.TopHeight()) <= prec2
				&& tabs(end.��������() * beg.BottomHeight() -beg.��������() * end.BottomHeight()) <= prec2)
				{ // join segments
					overlap_begin.������(i);
					overlap_end.������(j);
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
		for(i = 0; i < bseg.�������(); i++)
			AddSegment(bseg[i], false);
		// add left overlap segments to begin segment
		for(i = 0; i < overlap_begin.�������(); i++)
			AddSegment(sseg[overlap_begin[i]], false);
	}
	seg.end = traces.�������();
	seg_begin = seg;

	// process main segment
	seg = Segment(0, 0, seg.end, seg.end, dash * style->segment);
	for(int i = 0; i < sseg.�������(); i++)
		if(overlap_begin.�����(i) < 0)
		{
			const PathStyle::Trace& ti = sseg[i];
			int o = overlap_end.�����(i);
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

	seg.end = traces.�������();
	seg_segment = seg;

	seg = Segment(0, 0, seg.end, seg.end, 0);
	if(!closed)
	{
		seg.length = dash * style->end;
		for(int i = 0; i < eseg.�������(); i++)
			AddSegment(eseg[i], false);
	}
	seg.end = traces.�������();
	seg_end = seg;

	moveto = &PathDraw::MoveToFull;
	lineto = &PathDraw::LineToFull;
	if(seg_begin.������() && seg_end.������()
		&& traces.������() && segment_lines.�������() == 1)
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
	init_part = (closed ? PART_OPEN : seg_begin.������() ? PART_SEGMENT : PART_BEGIN);
	init_seg = (init_part == PART_BEGIN ? seg_begin : seg_segment);
	if(init_seg.������() && seg_segment.������() && segment_lines.������())
		if(closed)
		{
			empty = init_empty = true;
			return;
		}
		else
			init_part = PART_SKIPEND;
	if(init_part == PART_SKIPEND && seg_end.������())
		init_empty = true;

	Restart();
}

void PathDraw::SetSegment(const Segment& s, PART p)
{
	seg = s;
	part = p;
	next_index = seg.start;
	skip_size = (seg.������() ? 1e100 : traces[next_index].start);
}

void PathDraw::Restart()
{
	if(empty = init_empty)
		return;
	open_traces.������();
	Fill<�����>(raw_pos, raw_pos + __countof(raw_pos), Null);
	Fill<�����>(split_pos, split_pos + __countof(split_pos), Null);
	pos = raw_pos;
	part_total = 0;
	�������(init_seg.end <= traces.�������());
	SetSegment(init_seg, init_part);
}

bool PathDraw::SetExtent(const ����& rc)
{
	if(!�����_��(rc) && !rc.������������(clip_rect))
		return false;
	// todo: clip optimization
	return true;
}

void PathDraw::MoveToSimple(����� pt)
{
	//RTIMING("PathDraw::MoveToSimple");
	if(closed && !�����_��(close_line[0]) && !�����_��(pos[0]))
		one_output->AddNext(close_line[0]);
	pos[0] = close_line[0] = pt;
	one_output->AddFirst(pt);
}

void PathDraw::LineToSimple(����� pt)
{
	//RTIMING("PathDraw::LineToSimple");
	one_output->CheckFlush();
	one_output->AddNext(pos[0] = pt);
}

void PathDraw::MoveToThick(����� pt)
{
	//RTIMING("PathDraw::MoveToThick");
	if(closed && !�����_��(close_line[0]) && !�����_��(pos[0]))
		one_thick(*one_output, pos[0], close_line[0]);
	one_output->CheckFlush();
	pos[0] = close_line[0] = pt;
}

void PathDraw::LineToThick(����� pt)
{
	//RTIMING("PathDraw::LineToThick");
	one_output->CheckFlush();
	one_thick(*one_output, pos[0], pt);
	pos[0] = pt;
}

void PathDraw::MoveToFull(����� p)
{
	//RTIMING("PathDraw::MoveToFull");
	if(empty)
		return;

	if(!closed)
		���������(Null);
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
						���������(close_line[i]);
						LineToRaw();
					}
			}
			else if(seg_end.length > 0)
			{ // process split �����
				if(split_this)
					AddSplit();
				FlushSplit(seg_end.length);
				if(split_buffer.������())
					break;
				pos = split_pos;
				������ sp = split_buffer.��������();
				split_buffer.������������();
				���������(sp.pos);
				part_total -= seg_end.length;
				double t = sp.GetEndPos(sp.total - part_total);
				pos[0].x = pos[1].x + fround((pos[0].x - pos[1].x) * t);
				pos[0].y = pos[1].y + fround((pos[0].y - pos[1].y) * t);
				����� bk = pos[0];
				if(pos[0] != pos[1])
				{
					if(part != PART_SKIPEND)
						LineToRaw();
					���������();
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
				while(!split_buffer.������())
				{
					���������(split_buffer.��������().pos);
					split_buffer.������������();
					LineToRaw();
				}
				���������(Null);
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
	for(int i = 0; i < split_buffer.�������(); i++)
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
		while(!split_buffer.������())
		{ // flush part of �����
			const ������& sp = split_buffer.��������();
			if(part_total - sp.total < seg_end.length)
				break;
			���������(sp.pos);
			part_total -= sp.total;
			split_buffer.������������();
		}
	}
	else
	{
		while(!split_buffer.������())
		{ // flush part of �����
			const ������& sp = split_buffer.��������();
			if(part_total - sp.total < seg_end.length)
				break;
			���������(sp.pos);
			LineToRaw();
			part_total -= sp.total;
			split_buffer.������������();
		}
	}
	pos = raw_pos;
//	CheckSplit();
}

#define CALC_MITER() \
	bool start = �����_��(pos[3]); \
	bool end = �����_��(pos[0]); \
	������� a, b, a0, b0, at, bt, ar, up; \
	double al, bl; \
	double a_b, axb, /* rseg, */ ulen; \
	bool too_sharp; \
	if(calc_miter) \
	{ \
		a = ��������(pos[1] - pos[2]); \
		al = �����(a); \
		a0 = a / al; \
		ar = ����(a0); \
		too_sharp = false; \
		if(!end) \
		{ \
			b = ��������(pos[0] - pos[1]); \
			bl = �����(b); \
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
				�������(a0 % up >= 0); \
				too_sharp = (a_b <= chamfer_ratio); \
				if(too_sharp) \
				{ \
					double cd = (axb >= 0 ? -chamfer_dist : chamfer_dist); \
					at = ����(a0) + a0 * cd; \
					bt = ����(b0) - b0 * cd; \
				} \
			} \
			else if(miter == PathStyle::MITER_FLAT) \
			{ /* calc flat miter */ \
				������� mr = Unit(up); \
				if(tabs(mr % ar) <= 1e-5) \
					at = bt = mr; \
				else \
				{ \
					double k = (tabs(ar.x + mr.x) >= tabs(ar.y + mr.y) \
						? (ar.y - mr.y) / (ar.x + mr.x) : (mr.x - ar.x) / (ar.y + mr.y)); \
					at = mr + k * ����(mr); \
					bt = mr + k * �����(mr); \
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
			double ht = tabs((t)) - sqrt(max(0.0, (t) * (t) - ��������(next_top - new_top) * 0.25)); \
			GetArcPath(arc_path, new_top, next_top, (t) >= 0 ? ht : -ht); \
		} \
	}

#define DRAW_TOP_SHARP(ln, ot) \
	ln(out, ot, new_top); \
	ln(out, new_top, next_top);

#define DRAW_TOP_ROUND(ln, ot) \
	ln(out, ot, new_top); \
	if(!arc_path.������()) \
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
	if(top_sharp && arc_path.������()) \
		arc_path.������(next_top); \
	new_bot.x = pos[1].x - fround(b * a0.y); \
	new_bot.y = pos[1].y + fround(b * a0.x); \
	if(end || too_sharp) \
		arc_path.������(new_bot); \
	else if((b < 0) == (axb < 0)) \
		arc_path.������(next_bot = pos[1] + PointfToSize(up * double(b))); \
	else \
	{ \
		next_bot.x = pos[1].x - fround(b * b0.y); \
		next_bot.y = pos[1].y + fround(b * b0.x); \
		arc_path.������(next_bot); \
		double ht = tabs(b) - sqrt(max(0.0, b * b - ��������(next_bot - new_bot) * 0.25)); \
		GetArcPath(arc_path, next_bot, new_bot, b <= 0 ? ht : -ht); \
	} \
	if(px) \
	{ \
		int pos = arc_path.�������(); \
		arc_path.�������(pos, px); \
		�������(&arc_path[pos], poly, poly + px); \
	} \
	arc_path.������(new_top); \
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
	����� old_top; \
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
	����� new_top; \
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
	for(int i = 0; i < segment_lines.�������(); i++)
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
	if(�����_��(pos[2]))
		return false;

	������ split = SplitLine();
	if((part_total += split.total) >= seg.length)
	{ // break segment
		����� p0 = pos[0], p1 = pos[1], p2 = pos[2], p3 = pos[3], px;
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
		if(!seg_segment.������() || !segment_lines.������())
		{
			SetSegment(seg_segment, PART_SEGMENT);
			CalcLineBegin();
		}
		else if(!seg_end.������())
			part = PART_SKIPEND;
		else
		{
			empty = true;
			return false;
		}
		���������(p0);
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

void PathDraw::LineToFull(����� p)
{
	//RTIMING("PathDraw::LineToFull");
	if(p == raw_pos[0] || empty)
		return;
	���������(p);
	if(part == PART_SEGMENT)
	{
		if(seg_end.length <= 0)
		{
			LineToRaw();
			return;
		}
		else if(!�����_��(pos[2]))
		{
			AddSplit();
			FlushSplit(seg_end.length);
		}
		return;
	}

	switch(part)
	{
	case PART_OPEN:
		if(!�����_��(pos[2]))
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

PathDraw::������ PathDraw::SplitLine()
{
	�������(pos[0] != pos[1] && pos[1] != pos[2] && pos[2] != pos[3]);
	if(�����_��(pos[2]) || �����_��(pos[1]))
		return ������(0, 0, 0, Null);
	CALC_MITER();
	double pos_start = (start ? 0 : max<double>(-(old_up ^ a0) * (old_axb_left ? seg.bottom : seg.top), 0));
	double pos_end   = (end ? 0 : max<double>((up ^ a0) * (axb < 0 ? seg.bottom : seg.top), 0));
	return ������(pos_start, pos_end, pos_end + pos_start + al, pos[1]);
}

void PathDraw::LineToRaw()
{
	if(�����_��(pos[1]) || �����_��(pos[2]))
		return;

	�������(part != PART_SKIPEND);
	�������(pos[0] != pos[1] && pos[1] != pos[2] && pos[2] != pos[3]);
	CALC_MITER();

	if(!traces.������())
	{
		double pos_start = (start ? 0 : (old_up ^ a0) * (old_axb_left ? seg.bottom : seg.top));
		double pos_end   = al + (end ? 0 : (up ^ a0) * (axb < 0 ? seg.bottom : seg.top));
		double atotal = al, adelta = pos_end - pos_start, aindex = pos_start;

		for(int o = open_traces.�������(); --o >= 0;)
		{
			Trace& t = traces[open_traces[o]];
			Output& out = outputs[t.������];
			out.CheckFlush();
			if(adelta <= t.width - t.pos)
			{ // segment spans the whole line - draw joint
				double top = t.left_top + t.top_step * (t.pos += adelta);
				����� new_top, next_top;
				bool top_sharp = (!too_sharp || axb < 0 || t.left_top > 0);
				CALC_TOP_SHARP(top);
				if(t.hline)
				{ // single line
					DRAW_TOP_SHARP(t.hline, t.last_top);
				}
				else
				{
					����� poly[6];
					����� next_bot, new_bot;
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
				����� new_top = pos[1] + (top * up).AsSize();
				if(t.hline)
				{
					out.AddFirst(t.last_top);
					out.AddNext(new_top);
				}
				else
				{
					double bottom = t.left_bottom + t.bottom_step * t.pos;
					����� poly[6];
					poly[0] = t.last_bottom;
					poly[1] = t.last_top;
					����� new_bot = pos[1] + (up * bottom).AsSize();
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
				����� new_top;
				CALC_SHARP_LEADER(new_top, t.right_top, end_pos);
				if(t.hline)
					t.hline(out, t.last_top, new_top);
				else
				{
					����� poly[6];
					poly[0] = t.last_bottom;
					poly[1] = t.last_top;
					����� new_bot;
					CALC_SHARP_LEADER(new_bot, t.right_bottom, end_pos);
					poly[2] = new_top;
					poly[3] = new_bot;
					DrawPolygon(*draw, poly, 4, t.color, 0, Null, 0);
				}
				open_traces.�����(o);
			}
		}

		while((aindex += skip_size) < pos_end)
		{
			double left = pos_end - aindex;
			int tix = next_index++;
//			bool skip = (done.�����(next_index) >= 0);
			Trace& t = traces[tix];
			Output& out = outputs[t.������];
			skip_size = (next_index == seg.end
				? (part == PART_SEGMENT
					? seg.length + traces[next_index = seg.start].start - t.start : 1e100)
				: traces[next_index].start - t.start);
//			if(skip)
//				continue;
			����� old_top;
			CALC_SHARP_LEADER(old_top, t.left_top, aindex);
			����� new_top;
			if(left >= t.width)
			{ // full trace fits here
				CALC_SHARP_LEADER(new_top, t.right_top, aindex + t.width);
				if(t.hline)
					t.hline(out, old_top, new_top);
				else
				{
					����� poly[4];
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
				����� new_top, next_top;
				bool top_sharp = (!too_sharp || axb < 0 || t.left_top > 0);
				CALC_TOP_SHARP(top);
				if(t.hline)
				{ // single line
					DRAW_TOP_SHARP(t.hline, old_top);
				}
				else
				{
					����� poly[6];
					����� next_bot, new_bot;
					double bottom = t.left_bottom + t.bottom_step * t.pos;
					CALC_SHARP_LEADER(poly[0], t.left_bottom, aindex);
					poly[1] = old_top;
					int px = 2;
					DRAW_BOTTOM_SHARP(bottom, t.color);
					t.last_bottom = next_bot;
				}
				t.last_top = next_top;
/*
				����� new_top = pos[1] + (up * top).AsSize();
				if(t.hline)
				{
					out.AddFirst(old_top);
					out.AddNext(new_top);
				}
				else
				{
					����� poly[4];
					double bottom = t.left_bottom + t.bottom_step * t.pos;
					poly[0] = pos[1] + (up * bottom).AsSize();
					CALC_SHARP_LEADER(poly[1], t.left_bottom, aindex);
					poly[2] = old_top;
					poly[3] = new_top;
					DrawPolygon(*draw, poly, 4, t.color, 0, Null, 0);
					t.last_bottom = poly[0];
				}
				t.last_top = new_top;
*/				open_traces.������(tix);
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
		for(int o = open_traces.�������(); --o >= 0;)
		{
			int x = open_traces[o];
//			done.������(x);
			Trace& t = traces[x];
			Output& out = outputs[t.������];
			out.CheckFlush();
			if(al <= t.width - t.pos)
			{ // segment spans the whole line - draw joint
				double top = t.left_top + t.top_step * (t.pos += al);
				����� new_top, next_top;
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
						����� new_bot, next_bot;
						double bottom = t.left_bottom + t.bottom_step * t.pos;
						����� poly[6];
						poly[0] = t.last_bottom;
						poly[1] = t.last_top;
						int px = 2;
						DRAW_BOTTOM_SHARP(bottom, t.color);
						t.last_bottom = next_bot;
					}
				}
				else
				{
					�������(miter == PathStyle::MITER_ROUND || miter == PathStyle::MITER_FLAT);
					������<�����> arc_path;
					bool top_sharp = (!end && (axb < 0) != (top >= 0));
					CALC_TOP_ROUND(top)
					if(t.hline)
					{
						DRAW_TOP_ROUND(t.last_top);
					}
					else
					{
						����� poly[2];
						poly[0] = t.last_bottom;
						poly[1] = t.last_top;
						enum { px = 2 };
						����� new_bot, next_bot;
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
				������� bpart = a0 * (al - rem);
				CALC_END_TOP_LEADER(rem);
				if(t.hline)
				{
					out.AddFirst(t.last_top);
					out.AddNext(new_top);
				}
				else
				{
					����� poly[8];
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

				open_traces.�����(o);
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
//			bool skip = (done.�����(next_index) >= 0);
			Trace& t = traces[tix];
			Output& out = outputs[t.������];
			if(next_index == end_index)
				skip_size = (repeat ? segment + traces[next_index = seg_segment.start].start - t.start : 1e100);
			else
				skip_size = traces[next_index].start - t.start;
//			if(skip)
//				continue;
			������� apart = a0 * al;
			CALC_START_TOP_LEADER();
			if(al >= t.width)
			{ // full trace fits here
				������� bpart = a0 * (al - t.width);
				CALC_END_TOP_LEADER(t.width);
				if(t.hline)
				{
					out.AddFirst(old_top);
					out.AddNext(new_top);
				}
				else
				{
					����� poly[6];
					int px = 0;
					CALC_START_BOTTOM_LEADER();
					CALC_END_BOTTOM_LEADER(t.width);
					DrawPolygon(*draw, poly, px, t.color, 0, Null, 0);
				}
			}
			else
			{ // split trace & add to open list
				double top = t.left_top + t.top_step * (t.pos = al);
				����� new_top, next_top;
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
						����� poly[8];
						int px = 0;
						CALC_START_BOTTOM_LEADER();
						double bottom = t.left_bottom + t.bottom_step * t.pos;
						����� new_bot, next_bot;
						DRAW_BOTTOM_SHARP(bottom, t.color);
						t.last_bottom = next_bot;
					}
				}
				else
				{
					�������(miter == PathStyle::MITER_ROUND || miter == PathStyle::MITER_FLAT);
					������<�����> arc_path;
					bool top_sharp = (!end && (axb < 0) != (top >= 0));
					CALC_TOP_ROUND(top);
					if(t.hline)
					{
						DRAW_TOP_ROUND(old_top);
					}
					else
					{
						����� poly[4];
						int px = 0;
						CALC_START_BOTTOM_LEADER();
						double bottom = t.left_bottom + t.bottom_step * t.pos;
						����� new_bot, next_bot;
						DRAW_BOTTOM_ROUND(bottom, t.color);
						t.last_bottom = next_bot;
					}
				}
				t.last_top = next_top;
				open_traces.������(tix);
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
		for(int i = 0; i < segment_lines.�������(); i++)
		{
			Linear& sl = segment_lines[i];
			Output& out = outputs[sl.������];
			out.CheckFlush();
			if(sl.ground)
			{ // simple case
				sl.hline(out, pos[2], pos[1]);
			}
			else if(miter == PathStyle::MITER_SHARP)
			{ // complex case
				����� old_top = start ? pos[2] + PointfToSize(ar * double(sl.top)) : sl.last_top;
				����� new_top, next_top;
				bool top_sharp = (!too_sharp || axb < 0 || sl.top > 0);
				CALC_TOP_SHARP(sl.top);
				if(sl.hline)
				{ // single line
					DRAW_TOP_SHARP(sl.hline, old_top);
				}
				else
				{
					����� poly[6];
					����� next_bot, new_bot;
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
				����� new_top, next_top;
				����� old_top = start ? pos[2] + PointfToSize(ar * double(sl.top)) : sl.last_top;
				������<�����> arc_path;
				bool top_sharp = (!end && (axb < 0) == (sl.top >= 0));
				CALC_TOP_ROUND(sl.top);
				if(sl.hline)
				{ // single line
					DRAW_TOP_ROUND(sl.hline, old_top);
				}
				else
				{
					����� poly[2];
					poly[0] = (start ? pos[2] + PointfToSize(ar * double(sl.bottom)) : sl.last_bottom);
					poly[1] = old_top;
					enum { px = 2 };
					����� new_bot, next_bot;
					DRAW_BOTTOM_ROUND(sl.bottom, sl.color);
					sl.last_bottom = next_bot;
				}
				sl.last_top = next_top;
			}
			else if(miter == PathStyle::MITER_FLAT)
			{
				����� old_top = start ? pos[2] + PointfToSize(ar * double(sl.top)) : sl.last_top;
				����� new_top, next_top;
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
					����� poly[6];
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

void PathDraw::�����()
{
	MoveTo(Null);
	FlushLines();
	Restart();
}

void PathDraw::FlushLines()
{
	for(int i = 0; i < outputs.�������(); i++)
		outputs[i].����();
}

void PathDraw::RecurseArc(����� next, int length, int bulge, int levels)
{
	if(--levels < 0 || tabs(bulge) <= 1 || length <= 2)
		LineTo(next); // degenerate next line
	else
	{ // bisect arc
		������ normal = iscale(������(next.y - pos[0].y, pos[0].x - next.x), bulge, length);
		����� centre = ((pos[0] + next) >> 1) + normal;
		int ll = (int)�����(centre - next);
		int hh = iscale(bulge, ll, 2 * ll + length);
		RecurseArc(centre, ll, hh, levels);
		RecurseArc(next, ll, hh, levels);
	}
}

void PathDraw::ArcTo(����� pt, int bulge)
{
	�������(!�����_��(raw_pos[0]));
	RecurseArc(pt, (int)�����(pt - pos[0]), bulge, 10);
}

void PathDraw::Circle(����� pt, int radius)
{
	MoveTo(pt.x - radius, pt.y);
	ArcTo(pt.x + radius, pt.y, radius);
	ArcTo(pt.x - radius, pt.y, radius);
}

PathStyleDisplay::PathStyleDisplay(const PathStyleMap *path_map) : path_map(path_map) {}
PathStyleDisplay::~PathStyleDisplay() {}

void PathStyleDisplay::�����(Draw& draw, const ����& rc, const ��������& v, ���� i, ���� p, dword s) const
{
	draw.DrawRect(rc, p);
	if(�����_��(v) || ����_��(v))
	{
		���� s = v;
		PathStyle style = path_map->����(s);
		PathDraw pd;
		pd.���(draw, style, i, rc.��������() >> 2, rc.��������() >> 2, false);
		int mid = (rc.top + rc.bottom) >> 1;
		pd.MoveTo(rc.left, mid);
		pd.LineTo(rc.right, mid);
		pd.�����();
	}
}

}
