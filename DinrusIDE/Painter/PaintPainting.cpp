#include "Painter.h"

namespace РНЦП {

template <class T>
void sGet(T& r, ТкстПоток& ss)
{
	ss.дай(&r, sizeof(T));
}


template <class T>
T sGet(ТкстПоток& ss)
{
	T r;
	ss.дай(&r, sizeof(T));
	return r;
}

void Рисовало::рисуй(const Painting& pic)
{
	ТкстПоток ss(pic.cmd);
	ТочкаПЗ p, p1, p2;
	КЗСА  c, c1;
	Значение  v;
	int    f, ch, n, hasdx;
	Xform2D m;
	double r, w;
	Шрифт   fnt;
	int ii = 0;
	bool   large, sweep;
	for(;;) {
		int cmd = ss.дай();
		if(cmd < 0)
			return;
		bool rel = cmd & 1;
		switch(cmd) {
		case PAINTING_CLEAR:
			ClearOp(sGet<КЗСА>(ss));
			break;
		case PAINTING_MOVE:
		case PAINTING_MOVE_REL:
			sGet(p, ss);
			двигОп(p, rel);
			break;
		case PAINTING_LINE:
		case PAINTING_LINE_REL:
			sGet(p, ss);
			линияОп(p, rel);
			break;
		case PAINTING_QUADRATIC:
		case PAINTING_QUADRATIC_REL:
			sGet(p1, ss);
			sGet(p, ss);
			квадрОп(p1, p, rel);
			break;
		case PAINTING_QUADRATIC_S:
		case PAINTING_QUADRATIC_S_REL:
			sGet(p, ss);
			квадрОп(p, rel);
			break;
		case PAINTING_CUBIC:
		case PAINTING_CUBIC_REL:
			sGet(p1, ss);
			sGet(p2, ss);
			sGet(p, ss);
			кубОп(p1, p2, p, rel);
			break;
		case PAINTING_CUBIC_S:
		case PAINTING_CUBIC_S_REL:
			sGet(p2, ss);
			sGet(p, ss);
			кубОп(p2, p, rel);
			break;
		case PAINTING_ARC:
		case PAINTING_ARC_REL:
			sGet(p, ss);
			sGet(p1, ss);
			sGet(r, ss);
			sGet(w, ss);
			дугОп(p, p1, r, w, rel);
			break;
		case PAINTING_SVGARC:
		case PAINTING_SVGARC_REL:			
			sGet(p1, ss);
			sGet(r, ss);
			sGet(large, ss);
			sGet(sweep, ss);
			sGet(p, ss);
			SvgArcOp(p1, r, large, sweep, p, rel);
			break;
		case PAINTING_CLOSE:
			закройОп();
			break;
		case PAINTING_DIV:
			делиОп();
			break;
		case PAINTING_FILL_SOLID:
			FillOp(sGet<КЗСА>(ss));
			break;
		case PAINTING_FILL_IMAGE:
			sGet(m, ss);
			f = ss.дай();
			if(ii >= pic.data.дайСчёт())
				return;
			v = pic.data[ii++];
			if(!v.является<Рисунок>())
				return;
			FillOp((Рисунок)v, m, f);
			break;
		case PAINTING_FILL_GRADIENT:
			sGet(p, ss);
			sGet(c, ss);
			sGet(p1, ss);
			sGet(c1, ss);
			f = ss.дай();
			FillOp(p, c, p1, c1, f);
			break;
		case PAINTING_FILL_GRADIENT_X:
			sGet(c, ss);
			sGet(c1, ss);
			sGet(m, ss);
			f = ss.дай();
			FillOp(c, c1, m, f);
			break;
		case PAINTING_FILL_RADIAL_X:
			sGet(p1, ss);
			sGet(c, ss);
			sGet(c1, ss);
			sGet(m, ss);
			f = ss.дай();
			FillOp(p1, c, c1, m, f);
			break;
		case PAINTING_FILL_RADIAL:
			sGet(p, ss);
			sGet(c, ss);
			sGet(p1, ss);
			sGet(r, ss);
			sGet(c1, ss);
			f = ss.дай();
			FillOp(p, c, p1, r, c1, f);
			break;
		case PAINTING_STROKE_SOLID:
			sGet(w, ss);
			sGet(c, ss);
			StrokeOp(w, c);
			break;
		case PAINTING_STROKE_IMAGE:
			sGet(w, ss);
			sGet(m, ss);
			f = ss.дай();
			if(ii >= pic.data.дайСчёт())
				return;
			v = pic.data[ii++];
			if(!v.является<Рисунок>())
				return;
			StrokeOp(w, (Рисунок)v, m, f);
			break;
		case PAINTING_STROKE_GRADIENT:
			sGet(w, ss);
			sGet(p, ss);
			sGet(c, ss);
			sGet(p1, ss);
			sGet(c1, ss);
			f = ss.дай();
			StrokeOp(w, p, c, p1, c1, f);
			break;
		case PAINTING_STROKE_GRADIENT_X:
			sGet(w, ss);
			sGet(c, ss);
			sGet(c1, ss);
			sGet(m, ss);
			f = ss.дай();
			FillOp(c, c1, m, f);
			StrokeOp(w, c, c1, m, f);
			break;
		case PAINTING_STROKE_RADIAL_X:
			sGet(w, ss);
			sGet(p1, ss);
			sGet(c, ss);
			sGet(c1, ss);
			sGet(m, ss);
			f = ss.дай();
			FillOp(c, c1, m, f);
			StrokeOp(w, p1, c, c1, m, f);
			break;
		case PAINTING_STROKE_RADIAL:
			sGet(w, ss);
			sGet(p, ss);
			sGet(c, ss);
			sGet(p1, ss);
			sGet(r, ss);
			sGet(c1, ss);
			f = ss.дай();
			StrokeOp(w, p, c, p1, r, c1, f);
			break;
		case PAINTING_CLIP:
			ClipOp();
			break;
		case PAINTING_CHARACTER:
			sGet(p, ss);
			ch = ss.дай32();
			sGet(fnt, ss);
			симвОп(p, ch, fnt);
			break;
		case PAINTING_TEXT:
			{
				sGet(p, ss);
				n = ss.дай32();
				hasdx = ss.дай();
				sGet(fnt, ss);
				Буфер<wchar> txt(n);
				Буфер<double> dx(hasdx * n);
				for(int i = 0; i < n; i++) {
					txt[i] = ss.дай32();
					if(hasdx)
						sGet(dx[i], ss);
				}
				TextOp(p, txt, fnt, n, hasdx ? ~dx : NULL);
			}
			break;
		case PAINTING_COLORSTOP:
			sGet(r, ss);
			sGet(c, ss);
			ColorStopOp(r, c);
			break;
		case PAINTING_CLEARSTOPS:
			ClearStopsOp();
			break;
		case PAINTING_OPACITY:
			OpacityOp(sGet<double>(ss));
			break;
		case PAINTING_LINECAP:
			LineCapOp(ss.дай());
			break;
		case PAINTING_LINEJOIN:
			LineJoinOp(ss.дай());
			break;
		case PAINTING_MITERLIMIT:
			MiterLimitOp(ss.дай());
			break;
		case PAINTING_EVENODD:
			EvenOddOp(ss.дай());
			break;
		case PAINTING_INVERT:
			InvertOp(ss.дай());
			break;
		case PAINTING_DASH:
			{
				n = ss.дай32();
				Вектор<double> dash;
				for(int i = 0; i < n; i++)
					dash.добавь(sGet<double>(ss));
				r = sGet<double>(ss);
				DashOp(dash, r);
			}
			break;
		case PAINTING_TRANSFORM:
			sGet(m, ss);
			TransformOp(m);
			break;
		case PAINTING_BEGIN:
			BeginOp();
			break;
		case PAINTING_END:
			EndOp();
			break;
		case PAINTING_BEGINMASK:
			BeginMaskOp();
			break;
		case PAINTING_BEGINONPATH:
			sGet(r, ss);
			BeginOnPathOp(r, ss.дай());
			break;
		}
	}
}

void PaintImageBufferPaintingFn(ImageBuffer& ib, const Painting& p, Размер sz, Точка pos, int mode)
{
	БуфРисовало sw(ib, mode);
	РазмерПЗ psz = p.дайРазм();
	sw.Translate(-pos.x, -pos.y);
	sw.Scale(sz.cx / psz.cx, sz.cy / psz.cy);
	sw.рисуй(p);
}

void PaintImageBufferDrawingFn(ImageBuffer& ib, const Чертёж& iw, int mode)
{
	БуфРисовало sw(ib, mode);
	РазмерПЗ sz = ib.дайРазм();
	Размер isz = iw.дайРазм();
	sw.Scale(sz.cx / isz.cx, sz.cy / isz.cy);
	sw.DrawDrawing(0, 0, isz.cx, isz.cy, iw);
}

void RegisterPaintingFns__(void (*ig)(ImageBuffer& ib, const Painting& pw, Размер sz, Точка pos, int mode),
                           void (*iw)(ImageBuffer& ib, const Чертёж& p, int mode),
                           Рисунок (*rg)(Точка at, int angle, int chr, Шрифт fnt, Цвет color, Размер sz));

void ImageAnyDrawPainter(Draw *(*f)(Размер sz), Рисунок (*e)(Draw *w));

static Draw *sCP(Размер sz)
{
	return new ImagePainter(sz);
}

static Рисунок sEP(Draw *w)
{
	ImagePainter *ip = dynamic_cast<ImagePainter *>(w);
	return ip ? (Рисунок)(*ip) : Рисунок();
}

static Рисунок sRenderGlyph(Точка at, int angle, int chr, Шрифт fnt, Цвет color, Размер sz)
{
	ImageBuffer ib(sz);
	БуфРисовало sw(ib);
	sw.очисть(обнулиКЗСА());
	sw.DrawText(at.x, at.y, angle, ШТкст(chr, 1), fnt, color);
	Рисунок h = ib;
	return Premultiply(h);
}

ИНИЦИАЛИЗАТОР(PaintPainting)
{
	RegisterPaintingFns__(PaintImageBufferPaintingFn, PaintImageBufferDrawingFn, sRenderGlyph);
	ImageAnyDrawPainter(sCP, sEP);
}

}
