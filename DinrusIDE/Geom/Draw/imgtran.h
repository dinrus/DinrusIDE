#ifndef _Geom_Draw_imgtran_h_
#define _Geom_Draw_imgtran_h_

namespace РНЦП {

void LinearSet(ImageBuffer& dest, Точка d0, Точка d1, Точка d2, КЗСА color, const Прям *opt_clip = NULL);
void LinearCopy(ImageBuffer& dest, Точка d0, Точка d1, Точка d2,
	const Рисунок& src, Точка s0, Точка s1, Точка s2, const Прям *opt_clip = NULL, bool interpolate = true);

void BilinearSet(ImageBuffer& dest, Точка d1, Точка d2, Точка d3, Точка d4, КЗСА color, const Прям *clip = NULL);
void BilinearSet(ImageBuffer& dest, Точка destpos[4], КЗСА color, const Прям *clip = NULL);

void BilinearCopy(ImageBuffer& dest, Точка d1, Точка d2, Точка d3, Точка d4,
	const Рисунок& src, Точка s1, Точка s2, Точка s3, Точка s4, const Прям *clip = NULL, bool interpolate = true);
void BilinearCopy(ImageBuffer& dest, Точка destpos[4],
	const Рисунок& src, Точка srcpos[4], const Прям *opt_clip = NULL, bool interpolate = true);
void BilinearCopy(ImageBuffer& dest, Точка d1, Точка d2, Точка d3, Точка d4,
	const Рисунок& src, Прям sr, const Прям *opt_clip = NULL, bool interpolate = true);
void BilinearCopy(ImageBuffer& dest, Точка destpos[4],
	const Рисунок& src, Прям sr, const Прям *opt_clip = NULL, bool interpolate = true);

Рисунок BilinearRotate(const Рисунок& m, int angle);

}

#endif
