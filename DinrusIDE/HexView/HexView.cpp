#include "HexView.h"

namespace РНЦП {

inline int фмтГексЦифра(int c) {
	return c < 10 ? c + '0' : c - 10 + 'a';
}

void фмтГекс(char *буфер, int64 number, int n) {
	буфер[n] = '\0';
	while(n) {
		буфер[--n] = фмтГексЦифра((byte)number & 0x0f);
		number >>= 4;
	}
}

void ИнфОГексОбзоре::выведиЗнач(Draw& w, int x, int y, int bytes, bool be)
{
	dword d = 0;
	Размер fsz = дайРазмТекста("X", font);
	for(int i = 0; i < bytes; i++) {
		int b = data[be ? i : bytes - i - 1];
		if(b < 0) {
			w.DrawText(x, y, Ткст('?', 2 * bytes), font, SColorHighlight);
			x += 2 * bytes * fsz.cx;
			w.DrawText(x, y, "=", font);
			x += fsz.cx;
			w.DrawText(x, y, "?", font, красный);
			return;
		}
		d = (d << 8) | (byte) b;
	}
	w.DrawText(x, y, фмтЦелГекс(d, 2 * bytes), font, SColorHighlight);
	x += 2 * bytes * fsz.cx;
	w.DrawText(x, y, "=", font);
	x += fsz.cx;
	Ткст txt = фмтБцел(d);
	w.DrawText(x, y, txt, font, красный);
	x += дайРазмТекста(txt, font).cx;
	w.DrawText(x, y, "=", font);
	x += fsz.cx;
	int q = d;
	if(bytes == 1)
		q = (int8) d;
	else
	if(bytes == 2)
		q = (int16) d;
	w.DrawText(x, y, фмтЦел(q), font, магента);
}

void ИнфОГексОбзоре::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	w.DrawRect(sz, SColorLtFace);
	if(mode < 1 || empty)
		return;
	Размер fsz = дайРазмТекста("X", font);
	char h[17];
	фмтГекс(h, pos, longmode ? 16 : 8);
	int xx = 0;
	w.DrawText(xx, 0, h, font, SColorHighlight);
	xx += (longmode ? 16 : 8) * fsz.cx;
	w.DrawText(xx, 0, "=", font);
	xx += fsz.cx;
	w.DrawText(xx, 0, фмт64(pos), font, красный);
	xx += (longmode ? 22 : 12) * fsz.cx;
	int y = 0;
	int x = 0;
	for(int q = 0; q < mode; q++) {
		x = xx;
		if(q < 1)
			выведиЗнач(w, x, y, 1, q);
		x += 12 * fsz.cx;
		выведиЗнач(w, x, y, 2, q);
		x += 18 * fsz.cx;
		выведиЗнач(w, x, y, 4, q);
		x += 32 * fsz.cx;
		y += fsz.cy;
	}
	char sh[80];
	memset(sh, 0, sizeof(sh));
	int i;
	for(i = 0; i < 80; i++) {
		if(data[i] < 0)
			break;
		sh[i] = data[i];
	}
	ШТкст ws = вУтф32(sh, i);
	w.DrawText(x, 0, ws, font, цыан, i);
	if(mode < 2)
		return;
	wchar wh[40];
	memset(wh, 0, sizeof(wh));
	for(i = 0; i < 40; i++) {
		if(data[2 * i] < 0 || data[2 * i + 1] < 0)
			break;
		wh[i] = MAKEWORD(data[2 * i], data[2 * i + 1]);
	}
	w.DrawText(x, fsz.cy, wh, font, цыан, i);
	Ткст txt;
	Ткст ftxt;
	i = 0;
	for(;;) {
		if(data[i] < 0) {
			if((unsigned)i < sizeof(float))
				ftxt = "?";
			txt = "?";
			break;
		}
		if((unsigned)i >= sizeof(double)) {
			double h;
			memcpy(&h, sh, sizeof(double));
			txt = спринтф("%.8g", h);
			break;
		}
		if(i == sizeof(float)) {
			float h;
			memcpy(&h, sh, sizeof(float));
			ftxt = спринтф("%.6g", h);
		}
		sh[i] = data[i];
		i++;
	}
	w.DrawText(0, fsz.cy, txt, font, красный);
	w.DrawText(18 * fsz.cx, fsz.cy, ftxt, font, красный);
}

void ИнфОГексОбзоре::устРежим(int _mode)
{
	mode = _mode;
	устВысоту(mode * дайРазмТекста("X", CourierZ(12)).cy + 3);
	покажи(mode);
}

ИнфОГексОбзоре::ИнфОГексОбзоре()
{
	устРежим(0);
	добавьФрейм(фреймВерхнСепаратора());
	добавьФрейм(фреймПравСепаратора());
	font = CourierZ(12);
}

int ГексОбзор::Байт(int64 adr)
{
	return 0;
}

void ГексОбзор::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	w.DrawRect(sz, SColorPaper);
	if(!total) {
		w.DrawText(Zx(10), Zx(10), "Нет данных", ArialZ(20).Italic(), SRed());
		return;
	}
	int y = 0;
	uint64 adr = sc;
	while(y < sz.cy) {
		char h[17];
		фмтГекс(h, adr + start, IsLongMode() ? 16 : 8);
		w.DrawText(0, y, h, font);
		int x = (IsLongMode() ? 17 : 9) * fsz.cx;
		int tx = x + columns * fcx3;
		for(int q = columns; q--;) {
			if(adr >= total)
				return;
			if(adr == cursor) {
				w.DrawRect(x, y, fsz.cx * 2, fsz.cy, светлоЦыан);
				w.DrawRect(tx, y, fsz.cx, fsz.cy, светлоЦыан);
			}
			int b = Байт(adr++);
			if(b < 0) {
				w.DrawText(x, y, "??", font, коричневый);
				w.DrawText(tx, y, "?", font, коричневый);
			}
			else {
				h[0] = фмтГексЦифра((b & 0xf0) >> 4);
				h[1] = фмтГексЦифра(b & 0x0f);
				h[2] = '\0';
				w.DrawText(x, y, h, font, SColorText);
				Цвет color = SColorMark;
				switch(b) {
				case '\a': *h = 'a'; break;
				case '\b': *h = 'b'; break;
				case '\t': *h = 't'; break;
				case '\f': *h = 'f'; break;
				case '\r': *h = 'r'; break;
				case '\n': *h = 'n'; break;
				case '\v': *h = 'v'; break;
				case '\0': *h = '0'; break;
				default:
					if(b >= 32) {
						*h = b;
						color = SColorText;
					}
					else {
						*h = '.';
						color = SColorDisabled;
					}
				}
				h[1] = '\0';
				w.DrawText(tx, y, h, charset, font, color);
			}
			tx += fsz.cx;
			x += fcx3;
		}
		y += fsz.cy;
	}
}

void ГексОбзор::колесоМыши(Точка, int zdelta, dword)
{
	sb.Wheel(zdelta);
}

void ГексОбзор::SetSb()
{
	sbm = 0;
	uint64 sz = total;
	while((sz >> sbm) > (1 << 30))
		sbm++;
	sb.устВсего(int(sz >> sbm) / columns + 2);
	sb.устСтраницу(int(rows >> sbm));
	sb.уст(int(sc >> sbm) / columns + 1);
}

void ГексОбзор::Выкладка()
{
	Размер sz = дайРазм();
	columns = fixed ? fixed : max(4, (sz.cx - (IsLongMode() ? 18 : 10) * fsz.cx) / (4 * fsz.cx));
	rows = max(1, sz.cy / fsz.cy);
	bytes = columns * rows;
	SetSb();
}

void ГексОбзор::устСтарт(uint64 start_)
{
	start = start_;
	Выкладка();
	SetSb();
	освежи();
	освежиИнфо();
}

void ГексОбзор::устВсего(uint64 _total)
{
	total = _total;
	Выкладка();
	SetSb();
	освежи();
	освежиИнфо();
}

void ГексОбзор::SetSc(uint64 address)
{
	sc = minmax(address, start, total);
	SetSb();
	освежи();
}

void ГексОбзор::промотай()
{
	int64 q = (int64)(int)sb << sbm;
	if(q == 0)
		sc = 0;
	else
		sc = (q - 1) * columns + sc % columns;
	освежи();
}

void ГексОбзор::освежиИнфо()
{
	if(total) {
		info.устПоз(cursor + start, IsLongMode());
		for(int i = 0; i < 80; i++)
			info.уст(i, Байт(cursor + i));
	}
	else
		info.SetEmpty();
}

void ГексОбзор::устКурсор(uint64 _cursor)
{
	cursor = _cursor;
	
	if(cursor > total)
		cursor = total - 1;
	int q = int(sc % columns);
	if(cursor >= sc + bytes)
		sc = cursor - bytes + columns;
	if(cursor < sc) {
		sc = cursor;
	}
	if(sc > (uint64)q)
		sc = (sc - q) / columns * columns + q;
	if(sc >= total)
		sc = total - 1;
	SetSb();
	освежи();
	освежиИнфо();
}

void ГексОбзор::леваяВнизу(Точка p, dword)
{
	int rowi = p.y / fsz.cy;
	int x = (IsLongMode() ? 17 : 9) * fsz.cx;
	int tx = x + columns * fcx3;
	if(p.x >= x && p.x < tx) {
		x = p.x - x;
		int q = x / fcx3;
		if(x - q * fcx3 < 2 * fsz.cx && q < columns) {
			uint64 c = sc + rowi * columns + q;
			if(c < total)
				устКурсор(c);
		}
	}
	else
	if(p.x >= tx) {
		int q = (p.x - tx) / fsz.cx;
		if(q >= 0 && q < columns) {
			uint64 c = sc + rowi * columns + q;
			if(c < total)
				устКурсор(c);
		}
	}
	устФокус();
}

bool ГексОбзор::Ключ(dword ключ, int)
{
	int pg = max(columns, bytes - columns);
	int q = int(sc % columns);
	switch(ключ) {
	case K_LEFT:
		устКурсор(cursor - 1);
		return true;
	case K_RIGHT:
		устКурсор(cursor + 1);
		return true;
	case K_UP:
		устКурсор(cursor - columns);
		return true;
	case K_DOWN:
		устКурсор(cursor + columns);
		return true;
	case K_PAGEUP:
		SetSc(sc - pg);
		устКурсор(cursor - pg);
		return true;
	case K_PAGEDOWN:
		SetSc(sc + pg);
		устКурсор(cursor + pg);
		return true;
	case K_CTRL_LEFT:
		SetSc(sc - 1);
		break;
	case K_CTRL_RIGHT:
		SetSc(sc + 1);
		break;
	case K_CTRL_UP:
		SetSc(sc - columns);
		break;
	case K_CTRL_DOWN:
		SetSc(sc + columns);
		break;
	case K_HOME:
		устКурсор((cursor - q) / columns * columns + q);
		break;
	case K_END:
		устКурсор((cursor - q) / columns * columns + q + columns - 1);
		break;
	case K_CTRL_HOME:
	case K_CTRL_PAGEUP:
		устКурсор(0);
		break;
	case K_CTRL_END:
	case K_CTRL_PAGEDOWN:
		устКурсор(total - 1);
		break;
	}
	return БарМеню::скан(WhenBar, ключ);
}

void ГексОбзор::устКолонки(int x)
{
	FixedColumns(x);
}

void ГексОбзор::устНабсим(int chr)
{
	Charset(chr);
}

void ГексОбзор::стдИдиК(const Ткст& s)
{
	СиПарсер p(s);
	int n = 10;
	if(p.сим2('0', 'x') || p.сим('$') || p.сим('#'))
		n = 16;
	if(p.число_ли(n)) {
		uint64 a = p.читайЧисло(n);
		if(a < total) {
			устКурсор(a);
			SetSc(a);
			return;
		}
	}
	Exclamation("Неверная позиция!");
}

void ГексОбзор::идиК()
{
	if(go.выполни() == IDOK)
		WhenGoto((Ткст)~go.text);
}

void ГексОбзор::менюКолонок(Бар& bar)
{
	bar.добавь("Авто", THISBACK1(устКолонки, 0))
	   .Radio(fixed == 0);
	bar.добавь("8", THISBACK1(устКолонки, 8))
	   .Radio(fixed == 8);
	bar.добавь("16", THISBACK1(устКолонки, 16))
	   .Radio(fixed == 16);
	bar.добавь("32", THISBACK1(устКолонки, 32))
	   .Radio(fixed == 32);
}

void ГексОбзор::устИнфо(int m)
{
	info.устРежим(m);
}

void ГексОбзор::менюИнфо(Бар& bar)
{
	bar.добавь("Отсутствует", THISBACK1(устИнфо, 0))
	   .Check(info.дайРежим() == 0);
	bar.добавь("Стандартное", THISBACK1(устИнфо, 1))
	   .Check(info.дайРежим() == 1);
	bar.добавь("Расширенное", THISBACK1(устИнфо, 2))
	   .Check(info.дайРежим() == 2);
}

void ГексОбзор::менюНабсим(Бар& bar)
{
	for(int i = 1; i < счётНабСим(); i++)
		bar.добавь(имяНабСим(i), THISBACK1(устНабсим, i))
		   .Radio(charset == i);
}

void ГексОбзор::стдМеню(Бар& bar)
{
	bar.добавь("Перейти к..", [=] { WhenGotoDlg(); })
	   .Ключ(K_CTRL_G);
	bar.добавь("Колонки", THISBACK(менюКолонок));
	bar.добавь("Символ", THISBACK(менюНабсим));
	bar.добавь("Инфо о позиции", THISBACK(менюИнфо));
}

void ГексОбзор::праваяВнизу(Точка p, dword w)
{
	леваяВнизу(p, w);
	БарМеню::выполни(WhenBar);
}

ГексОбзор& ГексОбзор::устШрифт(Шрифт fnt)
{
	font = fnt;
	fsz = дайРазмТекста("X", font);
	fcx3 = 3 * fsz.cx;
	Выкладка();
	освежи();
	SetSb();
	return *this;
}

void  ГексОбзор::сериализуйНастройки(Поток& s)
{
	int version = 0;
	s / version;
	s / fixed;
	s % charset;
	int mode = info.дайРежим();
	s / mode;
	info.устРежим(mode);
	go.text.SerializeList(s);
}

ГексОбзор::ГексОбзор()
{
	устШрифт(CourierZ(12));
	BackPaint();
	charset = CHARSET_WIN1252;
	sb <<= THISBACK(промотай);
	устФрейм(фреймИнсет());
	добавьФрейм(sb);
	cursor = sc = 0;
	total = 0;
	fixed = 0;
	SetSc(0);
	устКурсор(0);
	добавьФрейм(info);
	info.устРежим(1);
	WhenBar = THISBACK(стдМеню);
	CtrlLayoutOKCancel(go, "Перейти к");
	WhenGoto = THISBACK(стдИдиК);
	WhenGotoDlg = THISBACK(идиК);
}

}
