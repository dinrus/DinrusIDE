#include "TextDiffCtrl.h"

namespace РНЦП {

inline Цвет HistoryBg() { return Цвет(255, 255, 0); }

КтрлСравниТекст::КтрлСравниТекст()
{
	letter = Размер(1, 1);
	number_width = 0;
	number_yshift = 0;
	number_bg = белоСерый();
	устФрейм(фреймПоле());
	добавьФрейм(scroll);
	устШрифт(CourierZ(10), CourierZ(10));
	scroll.безАвтоСкрой();
	scroll.ПриПромоте = THISBACK(SelfScroll);
	maxwidth = 0;
	tabsize = 4;
	gutter_width = 0;
	gutter_bg = AdjustIfDark(Цвет(151, 190, 239));
	gutter_fg = SGreen;
	cursor = anchor = Null;
	gutter_capture = false;
	show_line_number = true;
	show_white_space = false;
	show_diff_highlight = true;
	change_paper_color = true;
}

int КтрлСравниТекст::дайНомСтр(int y, int& ii)
{
	ii = scroll.дай().y + y / letter.cy;
	return ii >= 0 && ii < lines.дайСчёт() ? lines[ii].number : Null;
}

void КтрлСравниТекст::DoSelection(int y, bool shift)
{
	int ii;
	int i = дайНомСтр(y, ii);
	if(!пусто_ли(i)) {
		if(shift)
			cursor = i;
		else
			cursor = anchor = i;
		освежи();
		scroll.промотайДоУ(ii);
		WhenSel();
	}
}

void КтрлСравниТекст::леваяВнизу(Точка pt, dword keyflags)
{
	Размер sz = дайРазм();
	if(pt.x > sz.cx - gutter_width || HasCapture() && gutter_capture) {
		if(!HasCapture())
			SetCapture();
		int line = (pt.y * lines.дайСчёт()) / sz.cy;
		int page_lines = sz.cy / letter.cy;
		scroll.устУ(line - page_lines / 2);
		gutter_capture = true;
	}
	else {
		DoSelection(pt.y, keyflags & K_SHIFT);
		SetCapture();
		gutter_capture = false;
	}
	SetWantFocus();
}

void КтрлСравниТекст::леваяДКлик(Точка pt, dword keyflags)
{
	int ii;
	int i = дайНомСтр(pt.y, ii);
	if(!пусто_ли(i))
		WhenLeftDouble(i - 1, ii);
}

void КтрлСравниТекст::двигМыши(Точка pt, dword flags)
{
	if(HasCapture()) {
		if(gutter_capture)
			леваяВнизу(pt, flags);
		else
			DoSelection(pt.y, true);
	}
}

void КтрлСравниТекст::леваяПовтори(Точка pt, dword keyflags)
{
	if(HasCapture() && !gutter_capture)
		DoSelection(pt.y, true);
}

void КтрлСравниТекст::леваяВверху(Точка pt, dword keyflags)
{
	ReleaseCapture();
}

void РНЦП::КтрлСравниТекст::расфокусирован()
{
	освежи();
}

void КтрлСравниТекст::копируй()
{
	int sell, selh;
	if(дайВыделение(sell, selh)) {
		Ткст clip;
		for(int i = 0; i < lines.дайСчёт(); i++) {
			const Строка& l = lines[i];
			if(l.number >= sell && l.number <= selh) {
				clip << l.text;
			#ifdef PLATFORM_WIN32
				clip << "\r\n";
			#else
				clip << "\n";
			#endif
			}
		}
		ClearClipboard();
		AppendClipboardText(clip);
	}
}

void КтрлСравниТекст::праваяВнизу(Точка p, dword keyflags)
{
	БарМеню b;
	b.добавь(!пусто_ли(cursor), t_("копируй"), CtrlImg::copy(), THISBACK(копируй)).Ключ(K_CTRL_C);
	b.выполни();
}

bool КтрлСравниТекст::Ключ(dword ключ, int repcnt)
{
	Точка pos = scroll, newpos = pos, page = scroll.дайСтраницу();
	switch(ключ) {
		case K_CTRL_A:
			if(lines.дайСчёт() > 1) {
				cursor = lines[0].number;
				anchor = lines.верх().number;
				освежи();
			}
			break;
		case K_LEFT:       newpos.x--; break;
		case K_RIGHT:      newpos.x++; break;
		case K_CTRL_LEFT:  newpos.x -= page.x >> 1; break;
		case K_CTRL_RIGHT: newpos.x += page.x >> 1; break;
		case K_UP:         newpos.y--; break;
		case K_DOWN:       newpos.y++; break;
		case K_PAGEUP:     newpos.y -= page.y; break;
		case K_PAGEDOWN:   newpos.y += page.y; break;
		case K_HOME:       newpos.x = 0; break;
		case K_END:        newpos.x = maxwidth - page.x; break;
		case K_CTRL_HOME:  newpos.y = 0; break;
		case K_CTRL_END:   newpos.y = lines.дайСчёт() - page.y; break;
		case K_CTRL_C:
			копируй();
			break;
		case K_F3: {
			bool found = false;
			int i = max(pos.y + 2, 0);
			while(i < lines.дайСчёт() && lines[i].level)
				i++;
			while(i < lines.дайСчёт())
				if(lines[i++].level) {
					newpos.y = i - 2;
					found = true;
					break;
				}
			if(!found) {
				бипВосклицание();
				return true;
			}
			break;
		}
		case K_SHIFT_F3: {
			bool found = false;
			int i = min(pos.y, lines.дайСчёт() - 1);
			while(i > 0 && lines[i].level)
				i--;
			while(i >= 0)
				if(lines[i--].level) {
					newpos.y = i;
					found = true;
					break;
				}
			if(!found) {
				бипВосклицание();
				return true;
			}
			break;
		}
		default: return false;
	}
	if(newpos != pos)
		scroll.уст(newpos);
	return true;
}

int КтрлСравниТекст::GetMatchLen(const wchar *s1, const wchar *s2, int len)
{
	for(int q = 0; q < len; q++)
		if(s1[q] != s2[q])
			return q;
	return len;
}

bool КтрлСравниТекст::LineDiff(bool left, Вектор<СтрокРедакт::Highlight>& hln, Цвет eq_color,
                               const wchar *s1, int l1, int h1,
                               const wchar *s2, int l2, int h2, int depth)
{
	int p1 = 0;
	int p2 = 0;
	int matchlen = 0;
	int subscore = 0; // prefer matches that are not in words
	
	for(int pos1 = l1; pos1 < h1; pos1++)
		for(int pos2 = l2; pos2 < h2; pos2++) {
			int ml = GetMatchLen(s1 + pos1, s2 + pos2, min(h1 - pos1, h2 - pos2));
			if(ml && ml >= matchlen) {
				int l = pos1;
				int h = pos1 + matchlen;
				int sc = (l == 0 || IsAlNum(s1[l - 1]) != IsAlNum(s1[l])) +
				         (h == 0 || h >= l1 || IsAlNum(s1[h - 1]) != IsAlNum(s1[h]));
				if(ml > matchlen || sc > subscore) {
					p1 = pos1;
					p2 = pos2;
					matchlen = ml;
					subscore = sc;
				}
			}
		}
	
	if(matchlen > 1 || matchlen && !IsAlNum(s1[p1])) {
		for(int i = 0; i < matchlen; i++)
			hln[(left ? p1 : p2) + i].paper = eq_color;

		if(++depth < 20) {
			LineDiff(left, hln, eq_color, s1, l1, p1, s2, l2, p2, depth);
			LineDiff(left, hln, eq_color, s1, p1 + matchlen, h1, s2, p2 + matchlen, h2, depth);
		}
		return true;
	}
	return false;
}

void КтрлСравниТекст::рисуй(Draw& draw)
{
	Точка sc = scroll.дай();
	Размер offset = (Размер)sc * letter;
	Размер sz = дайРазм();

	int lcnt = lines.дайСчёт();
	int first_line = offset.cy / letter.cy;
	int last_line = min(idivceil(sz.cy + offset.cy, letter.cy), lines.дайСчёт() - 1);

	if(gutter_width > 0)
	{
		int t = 0, b = 0;
		int gx = sz.cx - gutter_width;
		for(int i = 0; i < lcnt; i++)
			if(lines[i].level > 1) {
				b = idivceil(sz.cy * i, lcnt);
				if(b >= t) {
					draw.DrawRect(gx, t, gutter_width, b - t, gutter_bg);
					draw.DrawRect(gx, b, gutter_width, 1, gutter_fg);
					t = b + 1;
				}
			}

		draw.DrawRect(gx, t, gutter_width, sz.cy - t, gutter_bg);

		int total = letter.cy * lcnt;
		if(total <= 0)
			total = 1;
		int page_height = (sz.cy * sz.cy) / total;
		int ty = max(0, (sz.cy * offset.cy) / total);
		int by = min(sz.cy, ty + page_height);
		draw.DrawRect(gx, ty, gutter_width, 2, чёрный);
		draw.DrawRect(gx, by - 2, gutter_width, 2, чёрный);
		draw.DrawRect(gx, ty, 2, by - ty, чёрный);
		draw.DrawRect(gx + gutter_width - 2, ty, 2, by - ty, чёрный);
	}
	
	ШТкст test = "č"; // read text/paper colors from highlighting scheme using likely non-highlighted text
	Вектор<СтрокРедакт::Highlight> th;
	th.устСчёт(2);
	th[0].ink = SColorText();
	th[0].paper = SColorPaper();
	WhenHighlight(th, test);
	Цвет text_color = th[0].ink;
	Цвет paper_color = th[0].paper;

	Цвет diffpaper = тёмен(paper_color) ? магента() : жёлтый();
	Цвет missingpaper = тёмен(paper_color) ? серый() : светлоСерый();

	int n_width = show_line_number ? number_width : 0;
	if(show_line_number) {
		for(int i = first_line; i <= last_line; i++) {
			const Строка& l = lines[i];
			int y = i * letter.cy - offset.cy;
			Цвет paper = пусто_ли(l.number) ? missingpaper : l.diff ? diffpaper : SColorPaper();
			Цвет ink = l.diff ? SRed(): SGray();
			draw.DrawRect(0, y, n_width, letter.cy, paper);
			draw.DrawRect(n_width - 1, y, 1, letter.cy, серый());
			if(!пусто_ли(l.number))
				draw.DrawText(0, y + number_yshift, фмтЦел(l.number_diff), number_font, ink);
		}
	}
	draw.Clip(n_width, 0, sz.cx - gutter_width - n_width, sz.cy);

	for(int i = first_line; i <= last_line; i++) {
		const Строка& l = lines[i];
		int y = i * letter.cy - offset.cy;
		Цвет ink = text_color;
		Цвет paper = пусто_ли(l.number) ? missingpaper : l.diff ? diffpaper : paper_color;
		bool sel = выделен(i);
		if(sel) {
			ink = SColorHighlightText();
			paper = естьФокус() ? SColorHighlight() : смешай(SColorHighlight(), SColorDisabled());
		}

		ШТкст ln = l.text.вШТкст();
		if(ln.дайСчёт() > 20000)
			ln.обрежь(20000);
		ln = разверниТабы(ln);
		Вектор<СтрокРедакт::Highlight> hln;
		hln.устСчёт(ln.дайСчёт() + 1);
		for(int i = 0; i < hln.дайСчёт(); i++) {
			СтрокРедакт::Highlight& h = hln[i];
			h.paper = sel ? paper : diffpaper;
			h.ink = ink;
			h.chr = ln[i];
			h.font = StdFont();
		}
		
		bool ldiff = false;

		if(!sel) {
			WhenHighlight(hln, ln);

			for(int i = 0; i < hln.дайСчёт(); ++i) {
				СтрокРедакт::Highlight& h = hln[i];
				if (change_paper_color)
					h.paper = paper;
			}
			if(show_diff_highlight) {
				ШТкст ln_diff = l.text_diff.вШТкст();
				ln_diff = разверниТабы(ln_diff);
				if((int64)ln_diff.дайСчёт() * ln.дайСчёт() < 50000) {
					if(left)
						ldiff = LineDiff(true, hln, paper_color,
						                 ~ln, 0, ln.дайСчёт(), ~ln_diff, 0, ln_diff.дайСчёт(), 0);
					else
						ldiff = LineDiff(false, hln, paper_color,
						                 ~ln_diff, 0, ln_diff.дайСчёт(), ~ln, 0, ln.дайСчёт(), 0);
				}
			}
			if(show_white_space) {
				for(int i = ln.дайСчёт(); i >= 0; --i) {
					СтрокРедакт::Highlight& h = hln[i];
					if (ln[i] == 0)
						continue;
					if(ln[i] == 32)
						h.paper = тёмен(paper_color) ? светлоКрасный() : красный();
					else
						break;
				}
			}
			
			if(ldiff)
				paper = paper_color;
		}

		draw.DrawRect(0, y, sz.cx, letter.cy, paper); // paint the end of line
		int x = 0;
		int ii = 0;
		while(ii < hln.дайСчёт() - 1) {
			СтрокРедакт::Highlight& h = hln[ii];
			int l = 1;
			while(ii + l < hln.дайСчёт() - 1 && hln[ii + l].paper == h.paper)
				l++;
			draw.DrawRect(n_width - offset.cx + x, y, l * letter.cx, letter.cy, h.paper);
			x += l * letter.cx;
			ii += l;
		}
		x = 0;
		ii = 0;
		Вектор<int> dx;
		while(ii < hln.дайСчёт() - 1) {
			Шрифт fnt = font;
			СтрокРедакт::Highlight& h = hln[ii];
			ШТкст text;
			text.конкат(h.chr);
			for(;;) {
				int i = ii + text.дайСчёт();
				if(i < hln.дайСчёт() - 1 && hln[i].ink == h.ink && hln[i].font == h.font)
					text.конкат(hln[i].chr);
				else
					break;
			}
			fnt.Bold(h.font.IsBold());
			fnt.Italic(h.font.IsItalic());
			fnt.Underline(h.font.IsUnderline());
			dx.устСчёт(text.дайСчёт());
			dx.уст(0, letter.cx, text.дайСчёт());
			
			draw.DrawText(n_width - offset.cx + x, y, text, fnt, h.ink, dx);

			x += text.дайСчёт() * letter.cx;
			ii += text.дайСчёт();
		}
	}
	int lcy = lcnt * letter.cy - offset.cy;
	draw.DrawRect(0, lcy, sz.cx, sz.cy - lcy, SGray());
	draw.стоп();
	draw.DrawRect(0, lcy, n_width, sz.cy - lcy, number_bg);
}

void КтрлСравниТекст::TabSize(int t)
{
	tabsize = t;
	обновиШирину();
	Выкладка();
}

void КтрлСравниТекст::устШрифт(Шрифт f, Шрифт nf)
{
	font = f;
	number_font = nf;
	letter.cy = f.GetCy();
	letter.cx = f.GetMonoWidth();
	number_width = 5 * nf.GetMonoWidth();
	number_yshift = (f.GetCy() - nf.GetCy() + 2) >> 1;
	Выкладка();
}

void КтрлСравниТекст::устШрифт(Шрифт f)
{
	устШрифт(f, GetNumberFont());
}

void КтрлСравниТекст::Выкладка()
{
	int n_width = show_line_number ? number_width : 0;

	scroll.уст(scroll, (scroll.дайРедуцРазмОбзора() - Размер(n_width + gutter_width, 0)) / letter, Размер(maxwidth, lines.дайСчёт()));
	освежи();
}

void КтрлСравниТекст::колесоМыши(Точка pt, int zdelta, dword keyflags)
{
	scroll.колесоУ(zdelta);
}

void КтрлСравниТекст::устСчёт(int c)
{
	bool rl = (c < lines.дайСчёт());
	lines.устСчёт(c);
	if(rl)
		обновиШирину();
	Выкладка();
}

void КтрлСравниТекст::AddCount(int c)
{
	lines.вставьН(lines.дайСчёт(), c);
	Выкладка();
}

void КтрлСравниТекст::уст(int line, Ткст text, bool diff, int number, int level, Ткст text_diff, int number_diff)
{
	Строка& l = lines.по(line);
	int tl = меряйДлину(text.вШТкст());
	int lt = меряйДлину(l.text.вШТкст());
	bool rl = (tl < lt && lt == maxwidth);
	l.number = number;
	l.text = text;
	l.diff = diff;
	l.level = level;
	l.text_diff = text_diff;
	l.number_diff = number_diff;
	if(rl)
		обновиШирину();
	else if(tl > maxwidth) {
		maxwidth = tl;
		Выкладка();
	}
}

void КтрлСравниТекст::SelfScroll()
{
	освежи();
	ПриПромоте();
}

void КтрлСравниТекст::PairScroll(КтрлСравниТекст *ctrl)
{
	scroll.уст(ctrl->scroll.дай());
}

void КтрлСравниТекст::обновиШирину()
{
	maxwidth = 0;
	for(int i = 0; i < lines.дайСчёт(); i++)
		maxwidth = max(maxwidth, меряйДлину(lines[i].text.вШТкст()));
}

int КтрлСравниТекст::меряйДлину(const wchar *text) const
{
	int pos = 0;
	while(*text)
		if(*text++ == '\t')
			pos += tabsize - pos % tabsize;
		else
			pos++;
	return pos;
}

bool КтрлСравниТекст::выделен(int i) const
{
	int sell, selh;
	if(HasLine(i) && дайВыделение(sell, selh)) {
		i = GetNumber(i);
		return i >= sell && i <= selh;
	}
	return false;
}

bool КтрлСравниТекст::дайВыделение(int& l, int& h) const
{
	if(пусто_ли(cursor)) {
		l = h = -1;
		return false;
	}
	l = min(cursor, anchor);
	h = max(cursor, anchor);
	return true;
}

ШТкст КтрлСравниТекст::разверниТабы(const wchar *text) const
{
	ШТкст out;
	for(wchar c; (c = *text++);)
		if(c == '\t')
			out.конкат(' ', tabsize - out.дайДлину() % tabsize);
		else
			out.конкат(c);
	return out;
}

Точка КтрлСравниТекст::дайПоз() const
{
	Точка pos(0, 0);
	int ltop = minmax(scroll.дай().y, 0, lines.дайСчёт() - 1);
	while(ltop >= 0 && пусто_ли(lines[ltop].number)) {
		ltop--;
		pos.y++;
	}
	if(ltop >= 0)
		pos.x = lines[ltop].number;
	return pos;
}

void КтрлСравниТекст::устПоз(Точка pos)
{
	int l = найдиИндексПоля(lines, &Строка::number,pos.x);
	if(l < 0)
		l = 0;
	SetSb(l + pos.y);
}

};
