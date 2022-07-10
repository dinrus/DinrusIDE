#include "Draw.h"

namespace РНЦП {

const byte *UnpackRLE(КЗСА *t, const byte *s, int len)
{
	КЗСА *e = t + len;
	while(t < e)
		if(*s & 0x80) {
			if(*s == 0x80)
				break;
			int count = min<int>((int)(*s & 0x3F), (int)(e - t));
			КЗСА h;
			if(*s++ & 0x40)
				обнули(h);
			else {
				h.b = s[0];
				h.g = s[1];
				h.r = s[2];
				h.a = 255;
				s += 3;
			}
			count = min<int>(count, (int)(e - t));
			Fill(t, h, count);
			t += count;
		}
		else {
			if(*s == 0)
				break;
			int count = *s++;
			while(count-- && t < e) {
				КЗСА h;
				h.b = s[0];
				h.g = s[1];
				h.r = s[2];
				h.a = 255;
				*t++ = h;
				s += 3;
			}
		}
	while(t < e)
		обнули(*t++);
	return s;
}

Ткст PackRLE(const КЗСА *s, int len)
{
	ТкстБуф r;
	const КЗСА *e = s + len;
	while(s < e) {
		const КЗСА *q = s;
		if(s->a == 0) {
			s++;
			while(s < e && s->a == 0 && s - q < 0x3f)
				s++;
			r.конкат((0x80|0x40) + (int)(s - q));
		}
		else
		if(s + 1 < e && s[0] == s[1]) {
			s++;
			while(s + 1 < e && s[0] == s[1] && s - q < 0x3e)
				s++;
			s++;
			r.конкат(0x80 + (int)(s - q));
			r.конкат(q->b);
			r.конкат(q->g);
			r.конкат(q->r);
		}
		else {
			s++;
			while(s + 1 < e && s->a && s[0] != s[1] && s - q < 0x3f)
				s++;
			r.конкат((int)(s - q));
			while(q < s) {
				r.конкат(q->b);
				r.конкат(q->g);
				r.конкат(q->r);
				q++;
			}
		}
	}
	return Ткст(r);
}

int Premultiply(КЗСА *t, const КЗСА *s, int len)
{
	const КЗСА *e = s + len;
	while(s < e) {
		if(s->a != 255) {
			while(s < e) {
				byte a = s->a;
				if(s->a != 0 && s->a != 255) {
					while(s < e) {
						int alpha = s->a + (s->a >> 7);
						t->r = alpha * (s->r) >> 8;
						t->g = alpha * (s->g) >> 8;
						t->b = alpha * (s->b) >> 8;
						t->a = s->a;
						s++;
						t++;
					}
					return IMAGE_ALPHA;
				}
				t->r = a & s->r;
				t->g = a & s->g;
				t->b = a & s->b;
				t->a = s->a;
				s++;
				t++;
			}
			return IMAGE_MASK;
		}
		*t++ = *s++;
	}
	return IMAGE_OPAQUE;
}

int um_table__[256];

void sInitUmTable__()
{
	ONCELOCK {
		for(int i = 1; i < 256; i++)
			um_table__[i] = 65536 / i;
	}
}

int Unmultiply(КЗСА *t, const КЗСА *s, int len)
{
	sInitUmTable__();
	const КЗСА *e = s + len;
	while(s < e) {
		if(s->a != 255) {
			while(s < e) {
				byte a = s->a;
				if(s->a != 0 && s->a != 255) {
					while(s < e) {
						int alpha = um_table__[s->a];
						t->r = (alpha * s->r) >> 8;
						t->g = (alpha * s->g) >> 8;
						t->b = (alpha * s->b) >> 8;
						t->a = s->a;
						s++;
						t++;
					}
					return IMAGE_ALPHA;
				}
				t->r = a & s->r;
				t->g = a & s->g;
				t->b = a & s->b;
				t->a = s->a;
				s++;
				t++;
			}
			return IMAGE_MASK;
		}
		*t++ = *s++;
	}
	return IMAGE_OPAQUE;
}

void AlphaBlend(КЗСА *t, const КЗСА *s, int len)
{
	const КЗСА *e = s + len;
	while(s < e) {
		int alpha = 256 - (s->a + (s->a >> 7));
		t->r = s->r + (alpha * t->r >> 8);
		t->g = s->g + (alpha * t->g >> 8);
		t->b = s->b + (alpha * t->b >> 8);
		t->a = s->a + (alpha * t->a >> 8);
		s++;
		t++;
	}
}

void AlphaBlendOpaque(КЗСА *t, const КЗСА *s, int len)
{
	const КЗСА *e = s + len;
	while(s < e) {
		int alpha = 256 - (s->a + (s->a >> 7));
		t->r = s->r + (alpha * t->r >> 8);
		t->g = s->g + (alpha * t->g >> 8);
		t->b = s->b + (alpha * t->b >> 8);
		t->a = 255;
		s++;
		t++;
	}
}

void AlphaBlend(КЗСА *t, const КЗСА *s, int len, Цвет color)
{
	const КЗСА *e = s + len;
	int r = color.дайК();
	int g = color.дайЗ();
	int b = color.дайС();
	while(s < e) {
		int alpha = s->a + (s->a >> 7);
		t->r += alpha * (r - t->r) >> 8;
		t->g += alpha * (g - t->g) >> 8;
		t->b += alpha * (b - t->b) >> 8;
		t->a = s->a + ((256 - alpha) * t->a >> 8);
		s++;
		t++;
	}
}

void AlphaBlendOpaque(КЗСА *t, const КЗСА *s, int len, Цвет color)
{
	const КЗСА *e = s + len;
	int r = color.дайК();
	int g = color.дайЗ();
	int b = color.дайС();
	while(s < e) {
		int alpha = s->a + (s->a >> 7);
		t->r += alpha * (r - t->r) >> 8;
		t->g += alpha * (g - t->g) >> 8;
		t->b += alpha * (b - t->b) >> 8;
		t->a = 255;
		s++;
		t++;
	}
}

void AlphaBlendStraightOpaque(КЗСА *t, const КЗСА *s, int len)
{
	const КЗСА *e = s + len;
	while(s < e) {
		int alpha = s->a + (s->a >> 7);
		t->r += alpha * (s->r - t->r) >> 8;
		t->g += alpha * (s->g - t->g) >> 8;
		t->b += alpha * (s->b - t->b) >> 8;
		t->a = 255;
		s++;
		t++;
	}
}

void AlphaBlendStraightOpaque(КЗСА *t, const КЗСА *s, int len, int alpha)
{
	if(alpha >= 256) {
		AlphaBlendStraightOpaque(t, s, len);
		return;
	}
	const КЗСА *e = s + len;
	alpha *= 0x102;
	while(s < e) {
		int a = (s->a * alpha) >> 16;
		t->r += a * (s->r - t->r) >> 8;
		t->g += a * (s->g - t->g) >> 8;
		t->b += a * (s->b - t->b) >> 8;
		t->a = 255;
		s++;
		t++;
	}
}

struct sBlends {
	int16 m;
	byte  a;
};

sBlends *sblends;

void sOnceInitBlends()
{
	ONCELOCK {
		sblends = (sBlends *)MemoryAllocPermanent(256 * 256 * sizeof(sBlends));
		for(int Fa = 0; Fa <= 255; Fa++)
			for(int Ba = 0; Ba <= 255; Ba++) {
				double A = (Fa / 255.0 + Ba / 255.0 - Fa / 255.0 * Ba / 255.0);
				sblends[(Ba << 8) + Fa].a = minmax((int)(255 * A + 0.5), 0, 255);
				sblends[(Ba << 8) + Fa].m = A > 0.001 ? int(256 * (Fa / 255.0) / A + 0.5) : 0;
			}
	}
}

inline void sInitBlends()
{
	if(!sblends)
		sOnceInitBlends();
}

void AlphaBlendStraight(КЗСА *b, const КЗСА *f, int len)
{
	sInitBlends();
	const КЗСА *e = f + len;
	while(f < e) {
		sBlends& x = sblends[(b->a << 8) + f->a];
		int m = x.m;
		b->a = x.a;
		b->r += m * (f->r - b->r) >> 8;
		b->g += m * (f->g - b->g) >> 8;
		b->b += m * (f->b - b->b) >> 8;
		b++;
		f++;
	}
}

void AlphaBlendOverBgStraight(КЗСА *b, КЗСА bg, int len)
{
	sInitBlends();
	const КЗСА *e = b + len;
	while(b < e) {
		sBlends& x = sblends[(bg.a << 8) + b->a];
		int m = x.m;
		b->a = x.a;
		b->r = bg.r + (m * (b->r - bg.r) >> 8);
		b->g = bg.g + (m * (b->g - bg.g) >> 8);
		b->b = bg.b + (m * (b->b - bg.b) >> 8);
		b++;
	}
}

void AlphaBlendStraight(КЗСА *b, const КЗСА *f, int len, Цвет color)
{
	sInitBlends();
	const КЗСА *e = f + len;
	int cr = color.дайК();
	int cg = color.дайЗ();
	int cb = color.дайС();
	while(f < e) {
		sBlends& x = sblends[(b->a << 8) + f->a];
		int m = x.m;
		b->a = x.a;
		b->r += m * (cr - b->r) >> 8;
		b->g += m * (cg - b->g) >> 8;
		b->b += m * (cb - b->b) >> 8;
		b++;
		f++;
	}
}

int GetChMaskPos32(dword mask)
{
	if(mask == 0xff000000)
		return 3;
	if(mask == 0xff0000)
		return 2;
	if(mask == 0xff00)
		return 1;
	return 0;
}

void TransformComponents(КЗСА *t, const КЗСА *s, int len,
	const byte r[], const byte g[], const byte b[], const byte a[])
{
	while(--len >= 0) {
		t->r = r[s->r];
		t->g = g[s->g];
		t->b = b[s->b];
		t->a = a[s->a];
		s++;
		t++;
	}
}

void MultiplyComponents(КЗСА *t, const КЗСА *s, int len, int num, int den)
{
	byte trans[256];
	for(int i = 0; i < 256; i++)
		trans[i] = (byte)minmax((2 * i + 1) * num / (2 * den), 0, 255);
	TransformComponents(t, s, len, trans, trans, trans, trans);
}

}
