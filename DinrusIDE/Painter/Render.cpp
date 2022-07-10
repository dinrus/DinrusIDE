#include "Painter.h"
#include "Fillers.h"

namespace РНЦП {

#define LLOG(x)   // DLOG(x)
#define LDUMP(x)  // DDUMP(x)

void PainterTarget::Fill(double width, SpanSource *ss, const КЗСА& color) {}

void БуфРисовало::ClearOp(const КЗСА& color)
{
	РНЦП::Fill(~*ip, color, ip->дайДлину());
	ip->SetKind(color.a == 255 ? IMAGE_OPAQUE : IMAGE_ALPHA);
}

БуфРисовало::PathJob::PathJob(Rasterizer& rasterizer, double width, const ИнфОПути *path_info,
                                const SimpleAttr& attr, const ПрямПЗ& preclip, bool isregular)
:	trans(attr.mtx)
{
	evenodd = attr.evenodd;
	regular = isregular && width < 0 && !path_info->сим_ли;

	g = &rasterizer;

	if(!пусто_ли(preclip.left) && !path_info->сим_ли) {
		double ex = max(width, 0.0) * (1 + attr.miter_limit);
		if(path_info->path_max.y + ex < preclip.top || path_info->path_min.y - ex > preclip.bottom ||
		   path_info->path_max.x + ex < preclip.left || path_info->path_min.x - ex > preclip.right) {
			preclipped = true;
			return;
		}
	}

	preclipped = false;

	if(regular)
		tolerance = 0.3;
	else {
		trans.target = g;
		g = &trans;
		tolerance = 0.3 / attr.mtx.GetScale();
	}

	if(width == ONPATH) {
		g = &onpathtarget;
		regular = false;
	}
	else
	if(width > 0) {
		stroker.иниц(width, attr.miter_limit, tolerance, attr.cap, attr.join, preclip);
		stroker.target = g;
		if(attr.dash) {
			dasher.иниц(attr.dash->dash, attr.dash->start);
			dasher.target = &stroker;
			g = &dasher;
		}
		else
			g = &stroker;
		evenodd = false;
	}
}

void БуфРисовало::RenderPathSegments(LinearPathConsumer *g, const Вектор<byte>& path,
                                       const SimpleAttr *attr, double tolerance)
{
	ТочкаПЗ pos = ТочкаПЗ(0, 0);
	const byte *data = path.begin();
	const byte *end = path.end();
	while(data < end) {
		const ЛинейнДанные *d = (ЛинейнДанные *)data;
		switch(d->тип) {
		case MOVE: {
			g->Move(pos = attr ? attr->mtx.Transform(d->p) : d->p);
			data += sizeof(ЛинейнДанные);
			break;
		}
		case LINE: {
			PAINTER_TIMING("LINE");
			g->Строка(pos = attr ? attr->mtx.Transform(d->p) : d->p);
			data += sizeof(ЛинейнДанные);
			break;
		}
		case QUADRATIC: {
			PAINTER_TIMING("QUADRATIC");
			const КвадратнДанные *d = (КвадратнДанные *)data;
			if(attr) {
				ТочкаПЗ p = attr->mtx.Transform(d->p);
				ApproximateQuadratic(*g, pos, attr->mtx.Transform(d->p1), p, tolerance);
				pos = p;
			}
			else {
				ApproximateQuadratic(*g, pos, d->p1, d->p, tolerance);
				pos = d->p;
			}
			data += sizeof(КвадратнДанные);
			break;
		}
		case CUBIC: {
			PAINTER_TIMING("CUBIC");
			const КубичДанные *d = (КубичДанные *)data;
			if(attr) {
				ТочкаПЗ p = attr->mtx.Transform(d->p);
				ApproximateCubic(*g, pos, attr->mtx.Transform(d->p1),
				                 attr->mtx.Transform(d->p2), p, tolerance);
				pos = p;
			}
			else {
				ApproximateCubic(*g, pos, d->p1, d->p2, d->p, tolerance);
				pos = d->p;
			}
			data += sizeof(КубичДанные);
			break;
		}
		case CHAR: {
			const СимДанные *ch = (СимДанные *)data;
			ApproximateChar(*g, ch->p, ch->ch, ch->fnt, tolerance);
			data += sizeof(СимДанные);
			break;
		}
		default:
			NEVER();
			g->стоп();
			return;
		}
	}
	g->стоп();
}

void БуфРисовало::SyncCo()
{
	if(co && subpixel && !co_subpixel) {
		int n = СоРабота::дайРазмерПула();
		co_subpixel.размести(n);
		for(int i = 0; i < n; i++)
			co_subpixel[i].размести(render_cx + 30);
	}
}

Буфер<ClippingLine> БуфРисовало::RenderPath(double width, Событие<Один<SpanSource>&> ss, const КЗСА& color)
{
	PAINTER_TIMING("RenderPath");
	Буфер<ClippingLine> newclip;

	if(width == FILL)
		закрой();

	current = Null;

	if(width == 0 || !ss && color.a == 0 && width >= FILL) {
		if(co && ++emptycount + jobcount >= BATCH_SIZE) { // not to exhaust 'paths'
			FinishPathJob();
			FinishFillJob();
		}
		return newclip;
	}

	if(pathattr.mtx_serial != preclip_mtx_serial) {
		if(dopreclip) {
			ТочкаПЗ tl, br, a;
			Xform2D imx = Inverse(pathattr.mtx);
			tl = br = imx.Transform(0, 0);
			a = imx.Transform(size.cx, 0);
			tl = min(a, tl);
			br = max(a, br);
			a = imx.Transform(0, size.cy);
			tl = min(a, tl);
			br = max(a, br);
			a = imx.Transform(size.cx, size.cy);
			tl = min(a, tl);
			br = max(a, br);
			preclip = ПрямПЗ(tl, br);
		}
		else
			preclip = Null;
		regular = pathattr.mtx.IsRegular();
		preclip_mtx_serial = pathattr.mtx_serial;
	}

	if(co) {
		if(width >= FILL && !ss && !alt && findarg(mode, MODE_ANTIALIASED, MODE_SUBPIXEL) >= 0) {
			for(int i = 0; i < path_info->path.дайСчёт(); i++) {
				while(jobcount >= cojob.дайСчёт())
					cojob.добавь().rasterizer.создай(ip->дайШирину(), ip->дайВысоту(), mode == MODE_SUBPIXEL);
				CoJob& job = cojob[jobcount++];
				job.path_info = path_info;
				job.subpath = i;
				job.attr = pathattr;
				job.width = width;
				job.color = color;
				job.preclip = preclip;
				job.regular = regular;
			}
			if(jobcount + emptycount >= BATCH_SIZE)
				FinishPathJob();
			return newclip;
		}
	
		FinishPathJob();
		FinishFillJob();
	}
	
	rasterizer.переустанов();

	PathJob j(rasterizer, width, path_info, pathattr, preclip, regular);
	if(j.preclipped)
		return newclip;

	if(co && ss && !co_span) {
		int n = СоРабота::дайРазмерПула();
		co_span.размести(n);
		for(int i = 0; i < n; i++)
			co_span[i].размести((subpixel ? 3 : 1) * ip->дайШирину() + 3);
	}
	
	bool doclip = width == CLIP;
	auto fill = [&](СоРабота *co) {
		int opacity = int(256 * pathattr.opacity);
		if(!opacity)
			return;
		Rasterizer::Filler *rg;
		SpanFiller          span_filler;
		SolidFiller         solid_filler;
		SubpixelFiller      subpixel_filler;
		ClipFiller          clip_filler;
		NoAAFillerFilter    noaa_filler;
		MaskFillerFilter    mf;
		Один<SpanSource>     rss;

		if(subpixel) {
			int ci = СоРабота::дайИндексТрудяги();
			subpixel_filler.sbuffer = co && ci >= 0 ? co_subpixel[ci] : subpixel;
		}
		
		if(doclip) {
			clip_filler.иниц(render_cx);
			rg = &clip_filler;
			newclip.размести(ip->дайВысоту());
		}
		else
		if(ss) {
			ss(rss);
			КЗСА           *lspan;
			int ci = СоРабота::дайИндексТрудяги();
			if(co && ci >= 0)
				lspan = co_span[ci];
			else {
				if(!span)
					span.размести((subpixel ? 3 : 1) * ip->дайШирину() + 3);
				lspan = span;
			}
			if(subpixel) {
				subpixel_filler.ss = ~rss;
				subpixel_filler.буфер = lspan;
				subpixel_filler.alpha = opacity;
				rg = &subpixel_filler;
			}
			else {
				span_filler.ss = ~rss;
				span_filler.буфер = lspan;
				span_filler.alpha = opacity;
				rg = &span_filler;
			}
		}
		else {
			if(subpixel) {
				subpixel_filler.ss = NULL;
				subpixel_filler.color = Mul8(color, opacity);
				subpixel_filler.invert = pathattr.invert;
				rg = &subpixel_filler;
			}
			else {
				solid_filler.c = Mul8(color, opacity);
				solid_filler.invert = pathattr.invert;
				rg = &solid_filler;
			}
		}
		if(mode == MODE_NOAA) {
			noaa_filler.уст(rg);
			rg = &noaa_filler;
		}
		if(width != ONPATH) {
			if(alt)
				alt->Fill(width, ~rss, color);
			else {
				PAINTER_TIMING("Fill");
				int ii = 0;
				for(;;) {
					int y = (co ? co->следщ() : ii++) + rasterizer.MinY();
					if(y > rasterizer.MaxY())
						break;
					solid_filler.t = subpixel_filler.t = span_filler.t = (*ip)[y];
					subpixel_filler.end = subpixel_filler.t + ip->дайШирину();
					span_filler.y = subpixel_filler.y = y;
					Rasterizer::Filler *rf = rg;
					if(clip.дайСчёт()) {
						const ClippingLine& s = clip.верх()[y];
						if(s.пустой()) goto empty;
						if(!s.IsFull()) {
							mf.уст(rg, s);
							rf = &mf;
						}
					}
					if(doclip)
						clip_filler.очисть();
					rasterizer.Render(y, *rf, j.evenodd);
					if(doclip)
						clip_filler.финиш(newclip[y]);
				empty:;
				}
			}
		}
	};
	PAINTER_TIMING("RenderPath2");
	bool doco = co && !doclip && !alt && !(subpixel && clip.дайСчёт());
	for(const auto& p : path_info->path) {
		RenderPathSegments(j.g, p, j.regular ? &pathattr : NULL, j.tolerance);
		if(width != ONPATH) {
			int n = rasterizer.MaxY() - rasterizer.MinY();
			if(n >= 0) {
				PAINTER_TIMING("RenderPath2 Fill");
				if(doco && n > 6) {
					СоРабота co;
					co * [&] { fill(&co); };
				}
				else
					fill(NULL);
				rasterizer.переустанов();
			}
		}
	}
	if(width == ONPATH) {
		onpath = pick(j.onpathtarget.path);
		pathlen = j.onpathtarget.len;
	}
	return newclip;
}

void БуфРисовало::FinishPathJob()
{
	if(jobcount == 0)
		return;

	СоРабота co;
	co * [&] {
		for(;;) {
			int i = co.следщ();
			if(i >= jobcount)
				break;
			CoJob& b = cojob[i];
			b.rasterizer.переустанов();
			PathJob j(b.rasterizer, b.width, b.path_info, b.attr, b.preclip, b.regular);
			if(!j.preclipped) {
				b.evenodd = j.evenodd;
				БуфРисовало::RenderPathSegments(j.g, b.path_info->path[b.subpath], j.regular ? &b.attr : NULL, j.tolerance);
			}
		}
	};

	FinishFillJob();
	
	fillcount = jobcount;
	разверни(cofill, cojob); // разверни to keep allocated rasters (instead of pick)

	fill_job & [=] {
		int miny = ip->дайВысоту() - 1;
		int maxy = 0;

		for(int i = 0; i < fillcount; i++) {
			CoJob& j = cofill[i];
			miny = min(miny, j.rasterizer.MinY());
			maxy = max(maxy, j.rasterizer.MaxY());
			j.c = Mul8(j.color, int(256 * j.attr.opacity));
		}
		auto fill = [&](int ymin, int ymax) {
			if(subpixel) {
				SubpixelFiller subpixel_filler;
				subpixel_filler.ss = NULL;
				int ci = СоРабота::дайИндексТрудяги();
				subpixel_filler.sbuffer = ci >= 0 ? co_subpixel[ci] : subpixel;
				for(int i = 0; i < fillcount; i++) {
					CoJob& j = cofill[i];
					int jymin = max(j.rasterizer.MinY(), ymin);
					int jymax = min(j.rasterizer.MaxY(), ymax);
					for(int y = jymin; y <= jymax; y++)
						if(j.rasterizer.NotEmpty(y)) {
							subpixel_filler.color = j.c;
							subpixel_filler.invert = j.attr.invert;
							subpixel_filler.t = (*ip)[y];
							subpixel_filler.end = subpixel_filler.t + ip->дайШирину();
							if(clip.дайСчёт()) {
								MaskFillerFilter mf;
								const ClippingLine& s = clip.верх()[y];
								if(!s.пустой() && !s.IsFull()) {
									mf.уст(&subpixel_filler, s);
									j.rasterizer.Render(y, mf, j.evenodd);
								}
							}
							else
								j.rasterizer.Render(y, subpixel_filler, j.evenodd);
						}
				}
			}
			else {
				SolidFiller solid_filler;
				for(int i = 0; i < fillcount; i++) {
					CoJob& j = cofill[i];
					int jymin = max(j.rasterizer.MinY(), ymin);
					int jymax = min(j.rasterizer.MaxY(), ymax);
					for(int y = jymin; y <= jymax; y++)
						if(j.rasterizer.NotEmpty(y)) {
							solid_filler.c = j.c;
							solid_filler.invert = j.attr.invert;
							solid_filler.t = (*ip)[y];
							if(clip.дайСчёт()) {
								MaskFillerFilter mf;
								const ClippingLine& s = clip.верх()[y];
								if(!s.пустой() && !s.IsFull()) {
									mf.уст(&solid_filler, s);
									j.rasterizer.Render(y, mf, j.evenodd);
								}
							}
							else
								j.rasterizer.Render(y, solid_filler, j.evenodd);
						}
				}
			}
		};
	
		int n = maxy - miny;
		if(n >= 0) {
			if(n > 6) {
				СоРабота co;
				co * [&] {
					for(;;) {
						const int N = 4;
						int y = N * co.следщ() + miny;
						if(y > maxy)
							break;
						fill(y, min(y + N - 1, maxy));
					}
				};
			}
			else
				fill(miny, maxy);
		}
	};

	jobcount = emptycount = 0;
}

void БуфРисовало::финиш()
{
	FinishPathJob();
	FinishFillJob();
}

void БуфРисовало::FillOp(const КЗСА& color)
{
	RenderPath(FILL, Null, color);
}

void БуфРисовало::StrokeOp(double width, const КЗСА& color)
{
	RenderPath(width, Null, color);
}

void БуфРисовало::ClipOp()
{
	FinishPathJob();
	Буфер<ClippingLine> newclip = RenderPath(CLIP, Null, обнулиКЗСА());
	if(attr.hasclip)
		clip.верх() = pick(newclip);
	else {
		clip.добавь() = pick(newclip);
		attr.hasclip = true;
		attr.cliplevel = clip.дайСчёт();
	}
}

}
