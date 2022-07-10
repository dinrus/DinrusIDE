#include "Painter.h"

namespace РНЦП {

void БуфРисовало::BeginMaskOp()
{
	attr.mask = true;
	Размер sz = ip->дайРазм();
	mask.добавь() = *ip;
	ip->создай(sz);
	очисть(обнулиКЗСА());
	старт();
}

static inline byte *sSpan(byte *t, int c, int& len)
{
	while(len > 128) {
		int n = min(len, 128);
		*t++ = 0;
		*t++ = c + n - 1;
		len -= n;
	}
	if(len) {
		*t++ = 0;
		*t++ = c + len - 1;
		len = 0;
	}
	return t;
}

void БуфРисовало::FinishMask()
{
	Буфер<byte> wb(mode == MODE_SUBPIXEL ? 6 * ip->дайШирину() : 2 * ip->дайШирину());
	bool creating = false;
	if(!attr.hasclip) {
		clip.добавь().размести(ip->дайВысоту());
		attr.hasclip = true;
		attr.cliplevel = clip.дайСчёт();
		creating = true;
	}
	Буфер<ClippingLine>& cl = clip.верх();
	for(int y = 0; y < ip->дайВысоту(); y++)
		if(creating || !cl[y].пустой()) {
			int  c0 = 0;
			int  c256 = 0;
			const КЗСА *s = (*ip)[y];
			const КЗСА *e = (*ip)[y] + ip->дайШирину();
			byte  *t = wb;
			while(s < e) {
				int val = s->a * (56 * s->r + 183 * s->g + 20 * s->b) >> 16;
				if(val == 0) {
					if(c256) t = sSpan(t, 128, c256);
					c0++;
					if(mode == MODE_SUBPIXEL)
						c0 += 2;
				}
				else
				if(val == 256) {
					if(c0) t = sSpan(t, 0, c0);
					c256++;
					if(mode == MODE_SUBPIXEL)
						c256 += 2;
				}
				else {
					if(c256) t = sSpan(t, 128, c256);
					if(c0) t = sSpan(t, 0, c0);
					*t++ = val;
					if(mode == MODE_SUBPIXEL) {
						*t++ = val;
						*t++ = val;
					}
				}
				s++;
			}
			if(c256) t = sSpan(t, 128, c256);
			if(c0) t = sSpan(t, 0, c0);
			cl[y].очисть();
			cl[y].уст(~wb, int(t - ~wb));
		}
	*ip = mask.верх();
	mask.сбрось();
	attr.mask = false;
}

}
