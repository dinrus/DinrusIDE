#include "Debuggers.h"

void DbgDisas::колесоМыши(Точка, int zdelta, dword)
{
	sb.Wheel(zdelta);
}

Размер DbgDisas::дайБокс() const
{
	Размер sz = дайРазмТекста("12345678", CourierZ(12));
	if(mode64)
		sz.cx *= 2;
	sz.cy = max(sz.cy, opfont.GetCy());
	return sz;
}

void DbgDisas::Выкладка()
{
	sb.устСтраницу(дайРазм().cy / дайБокс().cy);
}

void DbgDisas::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	Размер box = дайБокс();
	int i = sb;
	int y = 0;
	Шрифт hexfont = CourierZ(12);
	int maxb = 0;
	for(int i = 0; i < inst.дайСчёт(); i++)
		maxb = max(inst[i].bytes.дайСчёт(), maxb);
	int bcx = дайРазмТекста("00", hexfont).cx + HorzLayoutZoom(2);
	Цвет lblink = HighlightSetup::GetHlStyle(HighlightSetup::INK_KEYWORD).color;
	Цвет lblpaper = HighlightSetup::GetHlStyle(HighlightSetup::PAPER_BRACKET).color;
	while(i < inst.дайСчёт() && y < sz.cy) {
		Inst& n = inst[i];
		Цвет ink = естьФокус() && i == cursor ? SColorPaper : SColorText;
		int x = 0;
		w.DrawRect(0, y, sz.cx, box.cy, i == cursor ? естьФокус() ? SColorHighlight : SColorFace
		                                            : i & 1 ? смешай(SColorMark, SColorPaper, 220) : SColorPaper());
		int hexy = y + (box.cy - hexfont.GetCy()) / 2;
		if(sz.cx > bcx * 15) {
			bool lbl = taddr.найди(addr[i]) >= 0;
			if(lbl && i != cursor)
				w.DrawRect(0, y, box.cx, box.cy, lblpaper);
			w.DrawText(0, hexy, mode64 ? спринтф("%16llX", (uint64)addr[i]) : спринтф("%08X", (uint32)addr[i]),
			           hexfont,
			           естьФокус() && i == cursor ? SColorPaper : lbl ? lblink : SColorText);
			x += box.cx;
		}
		if(i == ip)
			DrawHighlightImage(w, x + 2, y + (box.cy - HorzLayoutZoom(12)) / 2, ipimg);
		x += HorzLayoutZoom(18);
		if(sz.cx - x - (maxb + 1) * bcx > bcx * 12) {
			for(int i = 0; i < min(n.bytes.дайСчёт(), maxb); i++)
				w.DrawText(x + i * bcx, hexy, спринтф("%02X", (byte)n.bytes[i]), hexfont, ink);
			x += (maxb + 1) * bcx;
		}
		int opy = y + (box.cy - opfont.GetCy()) / 2;
		w.DrawText(x, opy, n.code, opfont, ink);
		w.DrawText(x + codecx, opy, n.args, opfont, ink);
		y += box.cy;
		i++;
	}
	w.DrawRect(0, y, sz.cx, sz.cy, SColorPaper);
}

void DbgDisas::леваяВнизу(Точка p, dword)
{
	устФокус();
	cursor = minmax(p.y / дайБокс().cy + sb, 0, addr.дайСчёт() - 1);
	WhenCursor();
	освежи();
}

bool DbgDisas::ключ(dword ключ, int)
{
	int p = дайРазм().cy / дайБокс().cy;
	switch(ключ) {
	case K_UP:
		cursor--;
		break;
	case K_DOWN:
		cursor++;
		break;
	case K_HOME:
		cursor = 0;
		break;
	case K_END:
		cursor = INT_MAX;
		break;
	case K_PAGEUP:
		cursor -= p;
		break;
	case K_PAGEDOWN:
		cursor += p;
		break;
	default:
		return false;
	}
	cursor = minmax(cursor, 0, addr.дайСчёт() - 1);
	sb.промотайДо(cursor);
	освежи();
	WhenCursor();
	return true;
}

void DbgDisas::очисть()
{
	low = 0xffffffff;
	high = 0;
	cursor = -1;
	inst.очисть();
	taddr.очисть();
	addr.очисть();
	opfont = StdFontZ(12);
	codecx = дайРазмТекста("movlmo", opfont).cx;
	
	освежи();
}

void  DbgDisas::добавь(adr_t adr, const Ткст& code, const Ткст& args, const Ткст& bytes)
{
	if(adr < low)
		low = adr;
	if(adr > high)
		high = adr;
	addr.добавь(adr);
	Inst& n = inst.добавь();
	n.code = code;
	n.args = args;
	n.bytes = bytes;
	sb.устВсего(inst.дайСчёт());
	освежи();
	int cx = дайРазмТекста(n.code, opfont).cx;
	if(cx > codecx)
		codecx = cx;
}

void DbgDisas::промотай()
{
	освежи();
}

void DbgDisas::устКурсор(adr_t adr)
{
	cursor = addr.найди(adr);
	if(cursor >= 0)
		sb.промотайДо(cursor);
	освежи();
}

void DbgDisas::SetIp(adr_t adr, const Рисунок& img)
{
	ip = addr.найди(adr);
	ipimg = img;
	освежи();
}

void DbgDisas::сфокусирован()
{
	WhenFocus();
	освежи();
}

void DbgDisas::расфокусирован()
{
	WhenFocus();
	освежи();
}

void DbgDisas::WriteClipboard()
{
	Ткст s;
	for(int i = 0; i < addr.дайСчёт(); i++)
		s << фмт("%08X  ", (int)addr[i]) << inst[i].code << " " << inst[i].args << "\n";
	WriteClipboardText(s);
}

DbgDisas::DbgDisas()
{
	устФрейм(фреймИнсет());
	добавьФрейм(sb);
	sb.ПриПромоте = THISBACK(промотай);
	low = high = 0;
	mode64 = false;
	очисть();
}
