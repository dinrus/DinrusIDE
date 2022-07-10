#include "CtrlLib.h"

namespace РНЦП {

int дайРазмТекста(const FontInfo& fi, const wchar *text, const wchar *end) {
	int w = 0;
	while(text < end)
		w += fi[*text++];
	return w;
}

int дайРазмТекста(const FontInfo& fi, const ШТкст& text) {
	return дайРазмТекста(fi, text, text.стоп());
}

const wchar *GetTextFitLim(const FontInfo& fi, const wchar *txt, const wchar *end, int& cx) {
	for(;;) {
		if(fi[*txt] > cx || txt >= end)
			return txt;
		cx -= fi[*txt++];
	}
}

int GetTextFitCount(const FontInfo& fi, const ШТкст& s, int& cx) {
	return (int)(GetTextFitLim(fi, s, s.стоп(), cx) - s.старт());
}

const wchar *strdirsep(const wchar *s) {
	while(*s) {
		if(*s == '\\' || *s == '/') return s;
		s++;
	}
	return NULL;
}

void DrawFileName(Draw& ww, int x0, int y, int wcx0, int cy, const ШТкст& mname, bool isdir, Шрифт font,
                  Цвет ink, Цвет extink, const ШТкст& desc, Шрифт descfont, bool justname, Цвет uln)
{
	for(int pass = пусто_ли(uln); pass < 2; pass++) {
		NilDraw nd;
		Draw *w = pass ? &ww : &nd;
		FontInfo fi = font.Info();
		int extpos = (isdir ? -1 : mname.найдирек('.'));
		int slash = isdir ? -1 : max(mname.найдирек('\\'), mname.найдирек('/'));
		if(extpos < slash)
			extpos = -1;
		const wchar *ext = extpos >= slash && extpos >= 0 ? mname.старт() + extpos + 1
		                                                  : mname.стоп();
		const wchar *имя = mname;
		if(justname && slash >= 0)
			имя += slash + 1;
		int txtcx = дайРазмТекста(fi, имя);
		int x = x0;
		int wcx = wcx0;
		if(txtcx <= wcx) {
			if(pass == 0)
				ww.DrawRect(x0, y + fi.GetAscent() + 1, txtcx, 1, uln);
			ww.DrawText(x, y, имя, font, ink, (int)(ext - имя));
			ww.DrawText(x + дайРазмТекста(fi, имя, ext), y, ext, font, extink, (int)(mname.стоп() - ext));
			if(!пустой(desc) && pass)
				DrawTextEllipsis(ww, x + дайРазмТекста(fi, имя), y, wcx - txtcx,
				                 desc, "...", descfont, extink);
			x += txtcx;
			return;
		}
		else {
			int dot3 = 3 * fi['.'];
			if(2 * dot3 > wcx) {
				int n = GetTextFitCount(fi, имя, wcx);
				w->DrawText(x, y, имя, font, ink, n);
				x += дайРазмТекста(fi, имя, имя + n);
			}
			else {
				const wchar *end = mname.стоп();
				int dircx = 2 * fi['.'] + fi[DIR_SEP];
				const wchar *bk = strdirsep(имя);
				if(bk) {
					wcx -= dircx;
					w->DrawText(x, y, ".." DIR_SEPS, font, SColorDisabled, 3);
					x += dircx;
					do {
						txtcx -= дайРазмТекста(fi, имя, bk + 1);
						имя = bk + 1;
						if(txtcx < wcx) {
							w->DrawText(x, y, имя, font, ink, (int)(ext - имя));
							x += дайРазмТекста(fi, имя, ext);
							w->DrawText(x, y, ext, font, extink, (int)(end - ext));
							x += дайРазмТекста(fi, ext, end);
							goto end;
						}
						bk = strdirsep(имя);
					}
					while(bk);
				}
				wcx -= dot3;
				int extcx = дайРазмТекста(fi, ext, end);
				if(2 * extcx > wcx || ext == end) {
					int n = GetTextFitCount(fi, имя, wcx);
					w->DrawText(x, y, имя, font, ink, n);
					x += дайРазмТекста(fi, имя, имя + n);
					w->DrawText(x, y, "...", font, SColorDisabled, 3);
					x += dot3;
				}
				else {
					wcx -= extcx;
					int n = (int)(GetTextFitLim(fi, имя, end, wcx) - имя);
					w->DrawText(x, y, имя, font, ink, n);
					x += дайРазмТекста(fi, имя, имя + n);
					w->DrawText(x, y, "...", font, SColorDisabled, 3);
					w->DrawText(x + dot3, y, ext, font, extink, (int)(end - ext));
					x += dot3 + extcx;
				}
			}
		}
	end:
		if(pass == 0)
			ww.DrawRect(x0, y + fi.GetAscent() + 1, x - x0, 1, uln);
	}
}

void СписокФайлов::рисуй(Draw& w, const Прям& r, const Значение& q,
		             Цвет ink, Цвет paper, dword style) const
{
	const Файл& m = ValueTo<Файл>(q);
	bool inv = ((style & Дисплей::SELECT) || (style & Дисплей::CURSOR)) && (style & Дисплей::FOCUS);
	w.DrawRect(r, inv ? SColorHighlight() : paper);
	int x = r.left + 2;
	w.DrawImage(x, r.top + (r.устВысоту() - m.icon.дайРазм().cy) / 2, m.icon);
	x += iconwidth;
	x += 2;
	FontInfo fi = m.font.Info();
	DrawFileName(w, x, r.top + (r.устВысоту() - fi.дайВысоту()) / 2,
	             r.right - x - 2, r.устВысоту(), ШТкст(m.имя), m.isdir, m.font,
	             inv ? SColorHighlightText : m.ink,
	             inv ? SColorHighlightText : m.extink,
	             ШТкст(m.desc), m.descfont, justname, m.underline);
}

Размер СписокФайлов::дайСтдРазм(const Значение& q) const
{
	const Файл& m = ValueTo<Файл>(q);
	FontInfo fi = m.font.Info();
	int cx = дайРазмТекста(fi, ШТкст(m.имя)) + 2 + iconwidth + 2 + 3;
	if(!пусто_ли(m.desc))
		cx += дайРазмТекста(m.descfont.Info(), ШТкст(m.desc)) + fi.дайВысоту();
	return Размер(cx, дайВысотуЭлта());
}

void СписокФайлов::StartEdit() {
	if(дайКурсор() < 0) return;
	Прям r = дайПрямЭлта(дайКурсор());
	const Файл& cf = дай(дайКурсор());
	Шрифт f = cf.font;
	int fcy = f.Info().дайВысоту();
	r.left += iconwidth + 2;
	r.top += (r.устВысоту() - fcy - 4) / 2;
	r.bottom = r.top + fcy + 2;
	edit.устПрям(r);
	edit.устШрифт(cf.font);
	edit <<= cf.имя.вШТкст();
	edit.покажи();
	edit.устФокус();
	int pos = int(дайПозРасшф(cf.имя) - ~cf.имя);
	edit.устВыделение(0, pos);
	sb.откл();
}

void СписокФайлов::EndEdit() {
	KillEdit();
	int b = edit.естьФокус();
	edit.скрой();
	if(b) устФокус();
}

void СписокФайлов::OkEdit() {
	EndEdit();
	int c = дайКурсор();
	if(c >= 0 && c < дайСчёт())
		WhenRename(дай(c).имя, ~edit);
}

void СписокФайлов::KillEdit()
{
	sb.вкл();
	глушиОбрвызВремени(TIMEID_STARTEDIT);
}

void СписокФайлов::леваяВнизу(Точка p, dword flags) {
	int c = дайКурсор();
	if(IsEdit()) {
		OkEdit();
		c = -1;
	}
	СписокКолонок::леваяВнизу(p, flags);
	KillEdit();
	if(c == дайКурсор() && c >= 0 && !HasCapture() && renaming && WhenRename && !(flags & (K_SHIFT|K_CTRL)))
		устОбрвызВремени(750, THISBACK(StartEdit), TIMEID_STARTEDIT);
}

bool СписокФайлов::FindChar(int from, int chr) {
	for(int i = max(0, from); i < дайСчёт(); i++) {
		ШТкст x = дай(i).имя.вШТкст();
		if((int)взаг(вАски(x[0])) == chr) {
			очистьВыделение();
			устКурсор(i);
			return true;
		}
	}
	return false;
}

bool СписокФайлов::Ключ(dword ключ, int count) {
	if(ключ == K_ESCAPE && IsEdit()) {
		EndEdit();
		return true;
	}
	if(ключ == K_ENTER && IsEdit()) {
		OkEdit();
		return true;
	}
	if(accelkey) {
		int c = взаг((int)ключ);
		if(ключ < 256 && IsAlNum(c)) {
			if(!FindChar(дайКурсор() + 1, c))
				FindChar(0, c);
			return true;
		}
	}
	return СписокКолонок::Ключ(ключ, count);
}

void СписокФайлов::вставь(int ii,
                      const Ткст& имя, const Рисунок& icon, Шрифт font, Цвет ink,
				      bool isdir, int64 length, Время time, Цвет extink,
				      const Ткст& desc, Шрифт descfont, Значение data, Цвет uln,
				      bool unixexe, bool hidden)
{
	Значение v;
	Файл& m = CreateRawValue<Файл>(v);
	m.isdir = isdir;
	m.unixexe = unixexe;
	m.hidden = hidden;
	m.icon = icon;
	m.имя = имя;
	m.font = font;
	m.ink = ink;
	m.length = length;
	m.time = time;
	m.extink = пусто_ли(extink) ? ink : extink;
	m.desc = desc;
	m.descfont = descfont;
	m.data = data;
	m.underline = uln;
	СписокКолонок::вставь(ii, v, !m.isdir || selectdir);
	KillEdit();
}

void СписокФайлов::уст(int ii,
                   const Ткст& имя, const Рисунок& icon, Шрифт font, Цвет ink,
				   bool isdir, int64 length, Время time, Цвет extink,
				   const Ткст& desc, Шрифт descfont, Значение data, Цвет uln,
				   bool unixexe, bool hidden)
{
	Значение v;
	Файл& m = CreateRawValue<Файл>(v);
	m.isdir = isdir;
	m.unixexe = unixexe;
	m.hidden = hidden;
	m.icon = icon;
	m.имя = имя;
	m.font = font;
	m.ink = ink;
	m.length = length;
	m.time = time;
	m.extink = пусто_ли(extink) ? ink : extink;
	m.desc = desc;
	m.descfont = descfont;
	m.data = data;
	m.underline = uln;
	СписокКолонок::уст(ii, v, !m.isdir || selectdir);
	KillEdit();
}

void СписокФайлов::уст(int ii, const Файл& f)
{
	Значение v;
	Файл& m = CreateRawValue<Файл>(v);
	m = f;
	СписокКолонок::уст(ii, v, !m.isdir);
	KillEdit();
}

void СписокФайлов::устИконку(int ii, const Рисунок& icon)
{
	Значение v;
	Файл& m = CreateRawValue<Файл>(v);
	m = дай(ii);
	m.icon = icon;
	СписокКолонок::уст(ii, v, !m.isdir);
	KillEdit();
}

void СписокФайлов::добавь(const Файл& f)
{
	Значение v;
	Файл& m = CreateRawValue<Файл>(v);
	m = f;
	СписокКолонок::добавь(v, !m.isdir);
}

void СписокФайлов::добавь(const Ткст& имя, const Рисунок& icon, Шрифт font, Цвет ink,
				   bool isdir, int64 length, Время time, Цвет extink,
				   const Ткст& desc, Шрифт descfont, Значение data, Цвет uln,
				   bool unixexe, bool hidden)
{
	Значение v;
	Файл& m = CreateRawValue<Файл>(v);
	m.isdir = isdir;
	m.unixexe = unixexe;
	m.hidden = hidden;
	m.icon = icon;
	m.имя = имя;
	m.font = font;
	m.ink = ink;
	m.length = length;
	m.time = time;
	m.extink = пусто_ли(extink) ? ink : extink;
	m.desc = desc;
	m.descfont = descfont;
	m.data = data;
	m.underline = uln;
	СписокКолонок::добавь(v, !m.isdir || selectdir);
}

const СписокФайлов::Файл& СписокФайлов::дай(int i) const
{
	return ValueTo<Файл>(СписокКолонок::дай(i));
}

Ткст СписокФайлов::GetCurrentName() const
{
	return дайСчёт() && дайКурсор() >= 0 && дайКурсор() < дайСчёт() ? дай(дайКурсор()).имя
	       : Null;
}

int СписокФайлов::найди(const char *s) {
	for(int i = 0; i < дайСчёт(); i++)
		if(дай(i).имя == s) return i;
	return -1;
}

bool СписокФайлов::FindSetCursor(const char *имя) {
	int i = найди(имя);
	if(i < 0) return false;
	устКурсор(i);
	return true;
}

struct СписокФайлов::FileOrder : public ПорядокЗнач {
	const СписокФайлов::Порядок *order;

	virtual bool operator()(const Значение& a, const Значение& b) const {
		return (*order)(ValueTo<Файл>(a), ValueTo<Файл>(b));
	}
};

void СписокФайлов::сортируй(const Порядок& order)
{
	KillEdit();
	FileOrder fo;
	fo.order = &order;
	int i = дайКурсор();
	Ткст фн;
	if(i >= 0)
		фн = дай(i).имя;
	СписокКолонок::сортируй(fo);
	if(i >= 0)
		FindSetCursor(фн);
}

СписокФайлов::СписокФайлов() {
	iconwidth = DPI(16);
	ItemHeight(max(Draw::GetStdFontCy(), DPI(17)));
	Ктрл::добавь(edit);
	edit.скрой();
	edit.устФрейм(фреймЧёрный());
	renaming = false;
	justname = false;
	accelkey = false;
	selectdir = false;
	устДисплей(*this);
}

СписокФайлов::~СписокФайлов() {}

struct FileListSort : public СписокФайлов::Порядок {
	int  kind;

	virtual bool operator()(const СписокФайлов::Файл& a, const СписокФайлов::Файл& b) const {
		if(a.isdir != b.isdir)
			return a.isdir;
		if(a.имя == "..")
			return b.имя != "..";
		if(b.имя == "..")
			return false;
		int q = 0;
		int k = kind & ~FILELISTSORT_DESCENDING;
		if(k == FILELISTSORT_TIME)
			q = сравниЗнак(a.time, b.time);
		else
		if(k == FILELISTSORT_SIZE)
			q = сравниЗнак(a.length, b.length);
		else
		if(k == FILELISTSORT_EXT) {
			const char *ae = strrchr(a.имя, '.');
			const char *be = strrchr(b.имя, '.');
			if(ae == NULL || be == NULL)
				q = ae ? -1 : be ? 1 : 0;
			else
				q = stricmp(ae, be);
		}
		if(!q)
			q = stricmp(a.имя, b.имя);
		return kind & FILELISTSORT_DESCENDING ? q > 0 : q < 0;
	}
};

void SortBy(СписокФайлов& list, int kind)
{
	FileListSort fs;
	fs.kind = kind;
	list.сортируй(fs);
}

}
