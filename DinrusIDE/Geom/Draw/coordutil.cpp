#include "GeomDraw.h"

namespace РНЦП {

static void ExpandSegmentUp(Вектор<Точка>& src, Вектор<Точка>& dest, const LinearSegmentTree::Узел *node, Точка smin, Точка dmin)
{
	if(node) {
		ExpandSegmentUp(src, dest, ~node->below, smin, dmin);
		ExpandSegmentUp(src, dest, ~node->above, node->source, node->target);
	}
	else {
		src.добавь(smin);
		dest.добавь(dmin);
	}
}

static void ExpandSegmentDown(Вектор<Точка>& src, Вектор<Точка>& dest, const LinearSegmentTree::Узел *node, Точка smax, Точка dmax)
{
	if(node) {
		ExpandSegmentDown(src, dest, ~node->above, smax, dmax);
		ExpandSegmentDown(src, dest, ~node->below, node->source, node->target);
	}
	else {
		src.добавь(smax);
		dest.добавь(dmax);
	}
}

static void PaintSplit(ImageBuffer& out, const Рисунок& in,
	const PlanarSegmentTree::Узел& planar,
	const LinearSegmentTree::Узел *left, const LinearSegmentTree::Узел *top,
	const LinearSegmentTree::Узел *right, const LinearSegmentTree::Узел *bottom,
	const SegmentTreeInfo& info, Цвет background, int Индекс)
{
	if(planar.split) {
		Индекс <<= 2;
		PaintSplit(out, in, planar.split->topleft,
			left ? ~left->below : NULL, top ? ~top->below : NULL, NULL, NULL,
			info, background, Индекс + 0);
		PaintSplit(out, in, planar.split->topright,
			NULL, top ? ~top->above : NULL, right ? ~right->below : NULL, NULL,
			info, background, Индекс + 1);
		PaintSplit(out, in, planar.split->bottomleft,
			left ? ~left->above : NULL, NULL, NULL, bottom ? ~bottom->below : NULL,
			info, background, Индекс + 2);
		PaintSplit(out, in, planar.split->bottomright,
			NULL, NULL, right ? ~right->above : NULL, bottom ? ~bottom->above : NULL,
			info, background, Индекс + 3);
	}
	else if(!left && !top && !right && !bottom) {
		РазмерПЗ d01 = длина(planar.trg_topright - planar.trg_topleft, 1);
		РазмерПЗ d12 = длина(planar.trg_bottomright - planar.trg_topright, 1);
		РазмерПЗ d23 = длина(planar.trg_bottomleft - planar.trg_bottomright, 1);
		РазмерПЗ d30 = длина(planar.trg_topleft - planar.trg_bottomleft, 1);
		Размер d0 = длина(d30 - d01, 2), d1 = длина(d01 - d12, 2), d2 = длина(d12 - d23, 2), d3 = длина(d23 - d30, 2);
		Точка destpos[4];
		destpos[0] = planar.trg_topleft + d0;
		destpos[1] = planar.trg_topright + d1;
		destpos[2] = planar.trg_bottomright + d2;
		destpos[3] = planar.trg_bottomleft + d3;
		if(!in)
			BilinearSet(out, destpos, background);
		else
			BilinearCopy(out, destpos, in, planar.source, NULL, info.antialias);
	}
	else {
		bool empty = !in;
		Точка msrc = planar.source.центрТочка();
		Точка mdest = msrc * info;
		Вектор<Точка> bsrc;
		Вектор<Точка> bdest;
		ExpandSegmentUp  (bsrc, bdest, top,    planar.source.верхЛево(),     planar.trg_topleft);
		ExpandSegmentUp  (bsrc, bdest, right,  planar.source.верхПраво(),    planar.trg_topright);
		ExpandSegmentDown(bsrc, bdest, bottom, planar.source.низПраво(), planar.trg_bottomright);
		ExpandSegmentDown(bsrc, bdest, left,   planar.source.низЛево(),  planar.trg_bottomleft);
		Точка psrc = bsrc.верх();
		Точка pdest = bdest.верх();
		for(int i = 0; i < bsrc.дайСчёт(); i++) {
			Точка nsrc = bsrc[i];
			Точка ndest = bdest[i];
			РазмерПЗ pn = длина(nsrc - psrc, 1);
			РазмерПЗ nm = длина(msrc - nsrc, 1);
			РазмерПЗ mp = длина(psrc - msrc, 1);
			Размер dp = длина(mp - pn, 2), dn = длина(pn - nm, 2), dm = длина(nm - mp, 2);
			if(empty)
				LinearSet(out, mdest + dm, pdest + dp, ndest + dn, background);
			else
				LinearCopy(out, mdest + dm, pdest + dp, ndest + dn, in, msrc + dm, psrc + dp, nsrc + dn, NULL, info.antialias);
			psrc = nsrc;
			pdest = ndest;
		}
	}
}

void AlphaTransformPaint(ImageBuffer& out, const Рисунок& in,
	const PlanarSegmentTree& planar,
	const LinearSegmentTree& left, const LinearSegmentTree& top,
	const LinearSegmentTree& right, const LinearSegmentTree& bottom,
	const SegmentTreeInfo& info, Цвет background)
{
	PaintSplit(out, in, planar.root, ~left.split, ~top.split, ~right.split, ~bottom.split, info, background, 1);
}

static void PaintTreeSplit(Draw& draw, const PlanarSegmentTree::Узел& planar,
	const LinearSegmentTree::Узел *left, const LinearSegmentTree::Узел *top,
	const LinearSegmentTree::Узел *right, const LinearSegmentTree::Узел *bottom, int Индекс)
{
	if(planar.split) {
		Индекс <<= 2;
		PaintTreeSplit(draw, planar.split->topleft,     left ? ~left->below : NULL, top ? ~top->below : NULL, NULL, NULL, Индекс + 0);
		PaintTreeSplit(draw, planar.split->topright,    NULL, top ? ~top->above : NULL, right ? ~right->below : NULL, NULL, Индекс + 1);
		PaintTreeSplit(draw, planar.split->bottomleft,  left ? ~left->above : NULL, NULL, NULL, bottom ? ~bottom->below : NULL, Индекс + 2);
		PaintTreeSplit(draw, planar.split->bottomright, NULL, NULL, right ? ~right->above : NULL, bottom ? ~bottom->above : NULL, Индекс + 3);
	}
	else {
		Вектор<Точка> bsrc;
		Вектор<Точка> bdest;
		ExpandSegmentUp  (bsrc, bdest, top,    planar.source.верхЛево(),     planar.trg_topleft);
		ExpandSegmentUp  (bsrc, bdest, right,  planar.source.верхПраво(),    planar.trg_topright);
		ExpandSegmentDown(bsrc, bdest, bottom, planar.source.низПраво(), planar.trg_bottomright);
		ExpandSegmentDown(bsrc, bdest, left,   planar.source.низЛево(),  planar.trg_bottomleft);
		Точка center = (planar.trg_topleft + planar.trg_topright + planar.trg_bottomleft + planar.trg_bottomright) >> 2;
		Ткст fmt = фмтЦелГекс(Индекс, 4);
		Шрифт font = StdFont();
		Размер fmtsize = дайРазмТекста(fmt, font);
		draw.DrawText(center.x - (fmtsize.cx >> 1), center.y - (fmtsize.cy >> 1), fmt, font, светлоЗелёный());
		Точка pdest = bdest[0];
		bdest.добавь(pdest);
		draw.DrawPolyline(bdest, 0, светлоСиний());
		for(int i = 1; i < bdest.дайСчёт(); i++)
			draw.DrawRect(bdest[i].x - 1, bdest[i].y - 1, 3, 3, светлоЗелёный());
	}
}

void PaintPlanarTree(Draw& draw, const PlanarSegmentTree& planar,
	const LinearSegmentTree& left, const LinearSegmentTree& top,
	const LinearSegmentTree& right, const LinearSegmentTree& bottom)
{
	PaintTreeSplit(draw, planar.root, ~left.split, ~top.split, ~right.split, ~bottom.split, 1);
}

static void PlotLine(PlotterTool& tool, GisTransform transform, ТочкаПЗ src1, ТочкаПЗ src2, int level, double maxdev)
{
	double dev;
	if(--level <= 0 || (dev = transform.SourceDeviation(src1, src2)) <= maxdev) {
		tool.LineTo(transform.Target(src2));
		return;
	}
	ТочкаПЗ mid = (src1 + src2) / 2.0;
	PlotLine(tool, transform, src1, mid, level, maxdev);
	PlotLine(tool, transform, mid, src2, level, maxdev);
}

/*
void PlotLine(PlotterTool& tool, GisCoords proj, ТочкаПЗ lonlat1, ТочкаПЗ lonlat2, int branch)
{
	tool.MoveTo(proj.Project(lonlat1, branch));
	PlotLine(tool, proj, lonlat1, lonlat2, branch, 8,
		(tool.GetPlotter().GetDraw().Dots() ? 5 : 2) / tool.GetPlotter().measure);
}

void PlotLineSafe(PlotterTool& tool, GisCoords proj, ТочкаПЗ lonlat1, ТочкаПЗ lonlat2, int branch)
{
	int nseg = minmax<int>((int)fpabsmax(lonlat2 - lonlat1), 1, 360);
	tool.MoveTo(proj.Project(lonlat1, branch));
	for(int i = 1; i <= nseg; i++)
		tool.LineTo(proj.Project(lonlat1 + (lonlat2 - lonlat1) * (i / double(nseg)), branch));
}

void PlotRectSafe(PlotterTool& tool, GisCoords proj, const ПрямПЗ& lonlat, int branch)
{
	PlotLineSafe(tool, proj, lonlat.верхЛево(),     lonlat.верхПраво(),    branch);
	PlotLineSafe(tool, proj, lonlat.верхПраво(),    lonlat.низПраво(), branch);
	PlotLineSafe(tool, proj, lonlat.низПраво(), lonlat.низЛево(),  branch);
	PlotLineSafe(tool, proj, lonlat.низЛево(),  lonlat.верхЛево(),     branch);
}
*/

void PlotLine(PlotterTool& tool, GisTransform transform, ТочкаПЗ source1, ТочкаПЗ source2)
{
	if(transform.IsIdentity())
		tool.Строка(source1, source2);
	else {
		ТочкаПЗ target1 = transform.Target(source1), target2 = transform.Target(source2);
		if(!tool.GetPlotter().InLClip(target1) || !tool.GetPlotter().InLClip(target2)) {
			if(!ClipLine(source1, source2, transform.SourceExtent(tool.GetPlotter().logclip)))
				return;
			target1 = transform.Target(source1);
		}
		tool.MoveTo(target1);
		PlotLine(tool, transform, source1, source2, 8,
			(tool.GetPlotter().GetDraw().Dots() ? 5 : 2) / tool.GetPlotter().measure);
	}
}

void PlotRect(PlotterTool& tool, GisTransform transform, const ПрямПЗ& src)
{
	PlotLine(tool, transform, src.верхЛево(),     src.верхПраво());
	PlotLine(tool, transform, src.верхПраво(),    src.низПраво());
	PlotLine(tool, transform, src.низПраво(), src.низЛево());
	PlotLine(tool, transform, src.низЛево(),  src.верхЛево());
}

}
