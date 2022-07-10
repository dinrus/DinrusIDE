#include "Draw.h"

namespace РНЦП {

void DrawTextEllipsis(Draw& w, int x, int y, int cx, const wchar *text, const char *ellipsis,
				      Шрифт font, Цвет ink, int n)
{
	if(n < 0) n = strlen__(text);
	FontInfo f = font.Info();
	const char *s;
	int dtl = 0;
	int el = 0;
	for(s = ellipsis; *s; s++) {
		dtl += f[*s];
		el++;
	}
	int l = 0;
	int i;
	for(i = 0; i < n; i++) {
		l += f[(byte) text[i]];
		if(l > cx) {
			while(l + dtl > cx && i > 0) {
				l -= f[text[i]];
				i--;
			}
			i++;
			break;
		}
	}
	w.DrawText(x, y, text, font, ink, i);
	if(i < n)
		w.DrawText(x + l, y, ellipsis, font, ink, el);
}

void DrawTextEllipsis(Draw& w, int x, int y, int cx, const char *text, const char *ellipsis,
				      Шрифт font, Цвет ink, int n)
{
	return DrawTextEllipsis(w, x, y, cx, ШТкст(text), ellipsis, font, ink, n);
}

Размер GetTLTextSize(const wchar *text, Шрифт font)
{
	Размер sz(0, 0);
	int cy = font.GetCy();
	const wchar *s = text;
	const wchar *t = s;
	for(;;) {
		if(*s == '\n' || *s == '\0') {
			int a = 0;
			const wchar *q = t;
			while(q < s) {
				while(q < s && *q < ' ') {
					if(*q == '\t')
						a = (a + 2 * cy) / (2 * cy) * (2 * cy);
					q++;
				}
				t = q;
				while(q < s && *q >= ' ')
					q++;
				a += дайРазмТекста(t, font, (int) (q - t)).cx;
			}
			t = s + 1;
			sz.cy += cy;
			sz.cx = max(sz.cx, a);
		}
		if(*s++ == '\0') break;
	}
	return sz;
}

int GetTLTextHeight(const wchar *s, Шрифт font)
{
	return GetTLTextSize(s, font).cy;
}

void DrawTLText(Draw& draw, int x, int y, int cx, const wchar *text,
                Шрифт font, Цвет ink, int accesskey) {
	int cy = font.GetCy();
	const wchar *s = text;
	const wchar *t = s;
	int apos = HIWORD(accesskey);
	int akey = LOWORD(accesskey);
	for(;;) {
		if(*s == '\n' || *s == '\0') {
			int a = x;
			const wchar *q = t;
			const wchar *start = NULL;
			while(q < s) {
				while(q < s && *q < ' ') {
					if(*q == '\t')
						a = (a - x + 2 * cy) / (2 * cy) * (2 * cy) + x;
					q++;
				}
				t = q;
				bool ak = false;
				start = q;
				while(q < s && *q >= ' ') {
					if(akey && (int)взаг(вАски(*q)) == akey && (apos == 0 || q - start + 1 == apos)) {
						ak = true;
						akey = 0;
						break;
					}
					q++;
				}
				start = NULL;
				draw.DrawText(a, y, t, font, ink, (int)(q - t));
				a += дайРазмТекста(t, font, (int)(q - t)).cx;
				if(ak) {
					draw.DrawText(a, y, q, font().Underline(), ink, 1);
					a += дайРазмТекста(q, font().Underline(), 1).cx;
					q++;
				}
			}
			t = s + 1;
			y += cy;
		}
		if(*s++ == '\0') break;
	}
}

}
