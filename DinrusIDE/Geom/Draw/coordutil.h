#ifndef _Geom_Draw_coordutil_h_
#define _Geom_Draw_coordutil_h_

namespace РНЦП {

void AlphaTransformPaint(ImageBuffer& out, const Рисунок& in,
	const PlanarSegmentTree& planar,
	const LinearSegmentTree& left, const LinearSegmentTree& top,
	const LinearSegmentTree& right, const LinearSegmentTree& bottom,
	const SegmentTreeInfo& info, Цвет background);

void PaintPlanarTree(Draw& draw, const PlanarSegmentTree& planar,
	const LinearSegmentTree& left, const LinearSegmentTree& top,
	const LinearSegmentTree& right, const LinearSegmentTree& bottom);

void   PlotLine(PlotterTool& tool, GisTransform transform, ТочкаПЗ source1, ТочкаПЗ source2);
void   PlotRect(PlotterTool& tool, GisTransform transform, const ПрямПЗ& src);

}

#endif
