#include "CtrlCore.h"

#ifdef GUI_WIN

namespace РНЦП {

#define LLOG(x)

HFONT  GetWin32Font(Шрифт fnt, int angle);
extern СтатическийСтопор sFontLock;

void SystemDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Шрифт font, Цвет ink,
                      int n, const int *dx) {
	Std(font);
	ЗамкниГип __;
	COLORREF cr = дайЦвет(ink);
	if(cr != lastTextColor) {
		LLOG("Setting text color: " << ink);
		::SetTextColor(handle, lastTextColor = cr);
	}
	Стопор::Замок ___(sFontLock); // need this because of GetWin32Font
	HGDIOBJ orgfont = ::SelectObject(handle, GetWin32Font(font, angle));
	int ascent = font.Info().GetAscent();
	Вектор<char16> text16 = вУтф16(text, n);
	if(angle) {
		double sina, cosa;
		Draw::SinCos(angle, sina, cosa);
		Размер offset;
		::ExtTextOutW(handle, x + fround(ascent * sina), y + fround(ascent * cosa), 0, NULL, text16.begin(), text16.дайСчёт(), dx);
	}
	else
		::ExtTextOutW(handle, x, y + ascent, 0, NULL, text16.begin(), text16.дайСчёт(), dx);
	::SelectObject(handle, orgfont);
}

}

#endif
