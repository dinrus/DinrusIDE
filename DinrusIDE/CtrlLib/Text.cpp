#include <CtrlLib/CtrlLib.h>

#define LTIMING(x)  // RTIMING(x)

namespace РНЦП {

ТекстКтрл::ТекстКтрл()
{
	Unicode();
	undosteps = 1000;
	очисть();
	undoserial = 0;
	incundoserial = false;
	undo_op = false;
	WhenBar = THISBACK(StdBar);
	charset = CHARSET_UTF8;
	color[INK_NORMAL] = SColorText;
	color[INK_DISABLED] = SColorDisabled;
	color[INK_SELECTED] = SColorHighlightText;
	color[PAPER_NORMAL] = SColorPaper;
	color[PAPER_READONLY] = SColorFace;
	color[PAPER_SELECTED] = SColorHighlight;
	color[WHITESPACE] = смешай(SColorLight, SColorHighlight);
	color[WARN_WHITESPACE] = смешай(SColorLight, SRed);
	processtab = true;
	processenter = true;
	nobg = false;
	rectsel = false;
#ifdef CPU_64
	max_total = 2047 * 1024 * 1024;
#else
#ifdef _ОТЛАДКА
	max_total = 100 * 1024 * 1024;
#else
	max_total = 200 * 1024 * 1024;
#endif
#endif
	max_line_len = 100000;
	truncated = false;
}

ТекстКтрл::~ТекстКтрл() {}

void ТекстКтрл::MiddleDown(Точка p, dword flags)
{
	if(толькочтен_ли())
		return;
	if(AcceptText(Selection())) {
		ШТкст w = дайШТкст(Selection());
		selclick = false;
		леваяВнизу(p, flags);
		Paste(w);
		Action();
	}
}

void ТекстКтрл::режимОтмены()
{
	selclick = false;
	dropcaret = Null;
	isdrag = false;
}

void ТекстКтрл::очисть()
{
	ЗамкниГип __;
	view = NULL;
	viewlines = 0;
	cline = 0;
	cpos = 0;
	total = 0;
	truncated = false;
	lin.очисть();
	очистьСтроки();
	lin.добавь();
	вставьСтроки(0, 1);
	DirtyFrom(0);
	undo.очисть();
	redo.очисть();
	ClearDirty();
	anchor = -1;
	cursor = 0;
	SetSb();
	поместиКаретку(0);
	SelectionChanged();
	освежи();
}

void ТекстКтрл::DirtyFrom(int line) {}
void ТекстКтрл::SelectionChanged() {}
void ТекстКтрл::очистьСтроки() {}
void ТекстКтрл::вставьСтроки(int line, int count) {}
void ТекстКтрл::удалиСтроки(int line, int count) {}
void ТекстКтрл::вставьПеред(int pos, const ШТкст& text) {}
void ТекстКтрл::вставьПосле(int pos, const ШТкст& text) {}
void ТекстКтрл::удалиПеред(int pos, int size) {}
void ТекстКтрл::удалиПосле(int pos, int size) {}
void ТекстКтрл::освежиСтроку(int i) {}
void ТекстКтрл::инвалидируйСтроку(int i) {}
void ТекстКтрл::SetSb() {}
void ТекстКтрл::поместиКаретку(int64 newcursor, bool sel) {}

int ТекстКтрл::RemoveRectSelection() { return 0; }
ШТкст ТекстКтрл::CopyRectSelection() { return Null; }
int ТекстКтрл::PasteRectSelection(const ШТкст& s) { return 0; }

void   ТекстКтрл::CachePos(int64 pos)
{
	ЗамкниГип __;
	int64 p = pos;
	cline = дайПозСтроки64(p);
	cpos = pos - p;
}

void   ТекстКтрл::CacheLinePos(int linei)
{
	ЗамкниГип __;
	if(linei >= 0 && linei < дайСчётСтрок()) {
		cpos = дайПоз64(linei);
		cline = linei;
	}
}

bool   ТекстКтрл::IsUnicodeCharset(byte charset)
{
	return findarg(charset, CHARSET_UTF8, CHARSET_UTF8_BOM, CHARSET_UTF16_LE, CHARSET_UTF16_BE,
	                        CHARSET_UTF16_LE_BOM, CHARSET_UTF16_BE_BOM) >= 0;
}

int   ТекстКтрл::загрузи0(Поток& in, byte charset_, bool view) {
	ЗамкниГип __;
	очисть();
	lin.очисть();
	очистьСтроки();
	total = 0;
	устНабсим(charset_);
	truncated = false;
	viewlines = 0;
	this->view = NULL;
	view_all = false;
	offset256.очисть();
	total256.очисть();
	view_cache[0].blk = view_cache[1].blk = -1;
	if(view) {
		this->view = &in;
		устТолькоЧтен();
	}
	if(charset == CHARSET_UTF8_BOM && in.GetLeft() >= 3) {
		int64 pos = in.дайПоз();
		byte h[3];
		if(!(in.дай(h, 3) == 3 && h[0] == 0xEF && h[1] == 0xBB && h[2] == 0xBF))
			in.перейди(pos);
		charset = CHARSET_UTF8;
	}
	int be16 = findarg(charset, CHARSET_UTF16_LE_BOM, CHARSET_UTF16_BE_BOM);
	if(be16 >= 0 && in.GetLeft() >= 2) {
		int64 pos = in.дайПоз();
		dword h = in.Get16le();
		if(h != (be16 ? 0xfffe : 0xfeff))
			in.перейди(pos);
		charset = be16 ? CHARSET_UTF16_BE : CHARSET_UTF16_LE;
	}
	
	if(view) {
		view_loading_pos = in.дайПоз();
		view_loading_lock = 0;
		ViewLoading();
		поместиКаретку(0);
		return 0;
	}

	int m = загрузиСтроки(lin, INT_MAX, total, in, charset, max_line_len, max_total, truncated);

	вставьСтроки(0, lin.дайСчёт());
	Update();
	SetSb();
	поместиКаретку(0);
	return m;
}

int ТекстКтрл::загрузиСтроки(Вектор<Ln>& ls, int n, int64& total, Поток& in, byte charset,
                        int max_line_len, int max_total, bool& truncated,
                        int *view_line_count) const
{
	ТкстБуф ln;
	bool cr = false;
	byte b8 = 0;
	auto line_count = [&] { return view_line_count ? *view_line_count : ls.дайСчёт(); };
	if(charset == CHARSET_UTF16_LE || charset == CHARSET_UTF16_BE) {
		ШТкстБуф wln;
		auto put_wln = [&]() {
			if(view_line_count)
				(*view_line_count)++;
			else {
				Ln& ln = ls.добавь();
				ln.len = wln.дайСчёт();
				ln.text = вУтф8(~wln, ln.len);
			}
		};
		for(;;) {
			int c = charset == CHARSET_UTF16_LE ? in.Get16le() : in.Get16be();
			if(c < 0) {
				total += wln.дайСчёт();
				put_wln();
				goto finish;
			}
			if(c == '\r')
				cr = true;
			else
			if(c == '\n') {
			truncate_line:
				total += wln.дайСчёт() + 1;
				put_wln();
				if(line_count() >= n)
					goto finish;
				wln.очисть();
			}
			else {
				wln.конкат(c);
				if(wln.дайСчёт() >= max_line_len)
					goto truncate_line;
			}
		}
	}
	else {
		for(;;) {
			byte h[200];
			int size;
			int64 pos = in.дайПоз();
			const byte *s = in.GetSzPtr(size);
			if(size == 0)  {
				size = in.дай(h, 200);
				s = h;
				if(size == 0)
					break;
			}
			const byte *posptr = s;
			const byte *e = s + size;
			while(s < e) {
				const byte *b = s;
				const byte *ee = s + min(size_t(e - s), size_t(max_line_len - ln.дайСчёт()));
				{
					while(s < ee && *s != '\r' && *s != '\n') {
						b8 |= *s++;
						while(s < ee && *s >= ' ' && *s < 128) // Interestingly, this speeds things up
							s++;
						while(s < ee && *s >= ' ')
							b8 |= *s++;
					}
				}
				if(b < s) {
					if(s - b + ln.дайСчёт() > max_total)
						ln.конкат((const char *)b, max_total - ln.дайСчёт());
					else
						ln.конкат((const char *)b, (const char *)s);
				}
				auto put_ln = [&]() -> bool {
					if(view_line_count) {
						(*view_line_count)++;
						total += charset == CHARSET_UTF8 && (b8 & 0x80) ? длинаУтф32(~ln, ln.дайСчёт())
						                                                : ln.дайСчёт();
					}
					else {
						Ln& l = ls.добавь();
						if(charset == CHARSET_UTF8) {
							l.len = (b8 & 0x80) ? длинаУтф32(~ln, ln.дайСчёт()) : ln.дайСчёт();
							l.text = ln;
						}
						else {
							l.len = ln.дайСчёт();
							l.text = вНабсим(CHARSET_UTF8, ln, charset);
						}
						if(total + l.len + 1 > max_total) {
							ls.сбрось();
							truncated = true;
							return false;
						}
						total += l.len + 1;
					}
					return true;
				};
				while(ln.дайСчёт() >= max_line_len) {
					int ei = max_line_len;
					if(charset == CHARSET_UTF8)
						while(ei > 0 && ei > max_line_len - 6 && !((byte)ln[ei] < 128 || ведущийУтф8_ли((byte)ln[ei]))) // break lse at whole utf8 codepoint if possible
							ei--;
					Ткст nln(~ln + ei, ln.дайСчёт() - ei);
					ln.устСчёт(ei);
					truncated = true;
					if(!put_ln())
						goto out_of_limit;
					if(line_count() >= n) {
						in.перейди(s - posptr + pos);
						goto finish;
					}
					ln = nln;
				}
				if(s < e && *s == '\r') {
					s++;
					cr = true;
				}
				if(s < e && *s == '\n') {
					if(!put_ln())
						goto out_of_limit;
					s++;
					if(line_count() >= n) {
						in.перейди(s - posptr + pos);
						goto finish;
					}
					ln.очисть();
					b8 = 0;
				}
			}
		}
	}

out_of_limit:
	{
		ШТкст w = вЮникод(~ln, ln.дайСчёт(), charset);
		if(total + w.дайДлину() <= max_total) {
			if(view_line_count) {
				(*view_line_count)++;
				total += w.дайСчёт();
			}
			else {
				Ln& ln = ls.добавь();
				ln.len = w.дайСчёт();
				ln.text = вУтф8(~w, ln.len);
				total += ln.len;
			}
		}
	}
finish:
	return ls.дайСчёт() > 1 ? cr ? LE_CRLF : LE_LF : LE_DEFAULT;
}

void ТекстКтрл::ViewLoading()
{
	ЗамкниГип __;
	if(view_all || !view)
		return;
	int start = msecs();
	view->перейди(view_loading_pos);
	int lines0 = viewlines;
	for(;;) {
		offset256.добавь(view->дайПоз());
		Вектор<Ln> l;
		bool b;
		int64 t = 0;

		int line_count = 0;
		загрузиСтроки(l, 256, t, *view, charset, 10000, INT_MAX, b, &line_count);
		viewlines += line_count;
		total += t;
		total256.добавь((int)t);
		
	#ifdef CPU_32
		enum { MAX_LINES = 128000000 };
	#else
		enum { MAX_LINES = INT_MAX - 512 };
	#endif

		if(view->кф_ли() || viewlines > INT_MAX - 512) {
			WhenViewMapping(view->дайПоз());
			view_all = true;
			break;
		}
		
		if(view_loading_lock) {
			view_loading_pos = view->дайПоз();
			WhenViewMapping(view_loading_pos);
			break;
		}
		
		if(msecs(start) > 20) {
			view_loading_pos = view->дайПоз();
			постОбрвыз([=] { ViewLoading(); });
			WhenViewMapping(view_loading_pos);
			break;
		}
	}
	вставьСтроки(lines0, viewlines - lines0);
	SetSb();
	Update();
}

void ТекстКтрл::UnlockViewMapping()
{
	view_loading_lock--;
	ViewLoading();
}

void ТекстКтрл::ждиВид(int line, bool progress)
{
	if(view) {
		if(progress) {
			LockViewMapping();
			Progress pi("Сканируется файл");
			pi.Delay(1000);
			while(view && !view_all && viewlines < line) {
				if(pi.SetCanceled(int(view_loading_pos >> 10), int(view->дайРазм()) >> 10))
					break;
				ViewLoading();
			}
			UnlockViewMapping();
		}
		else
			while(view && !view_all && viewlines <= line)
				ViewLoading();
	}
}

void ТекстКтрл::SerializeViewMap(Поток& s)
{
	ЗамкниГип __;
	int version = 0;
	s / version;
	s.Magic(327845692);
	s % view_loading_pos
	  % total
	  % viewlines
	  % view_all
	  % total256
	  % offset256
	;
	if(s.грузится()) {
		SetSb();
		Update();
		освежи();
	}
}

const ТекстКтрл::Ln& ТекстКтрл::GetLn(int i) const
{
	if(view) {
		ЗамкниГип __;
		int blk = i >> 8;
		if(view_cache[0].blk != blk)
			разверни(view_cache[0], view_cache[1]); // trivial LRU
		if(view_cache[0].blk != blk) {
			разверни(view_cache[0], view_cache[1]); // trivial LRU
			view->перейди(offset256[blk]);
			int64 t = 0;
			bool b;
			view_cache[0].line.очисть();
			view_cache[0].blk = blk;
			загрузиСтроки(view_cache[0].line, 256, t, *view, charset, 5000, INT_MAX, b);
		}
		return view_cache[0].line[i & 255];
	}
	else
		return lin[i];
}

const Ткст& ТекстКтрл::дайУтф8Строку(int i) const
{
	return GetLn(i).text;
}

int ТекстКтрл::дайДлинуСтроки(int i) const
{
	return GetLn(i).len;
}

void   ТекстКтрл::сохрани(Поток& s, byte charset, int line_endings) const {
	if(charset == CHARSET_UTF8_BOM) {
		static byte bom[] = { 0xEF, 0xBB, 0xBF };
		s.помести(bom, 3);
		charset = CHARSET_UTF8;
	}
	if(charset == CHARSET_UTF16_LE_BOM) {
		s.Put16le(0xfeff);
		charset = CHARSET_UTF16_LE;
	}
	if(charset == CHARSET_UTF16_BE_BOM) {
		s.Put16be(0xfeff);
		charset = CHARSET_UTF16_BE;
	}
	charset = разрешиНабСим(charset);
	Ткст le = "\n";
#ifdef PLATFORM_WIN32
	if(line_endings == LE_DEFAULT)
		le = "\r\n";
#endif
	if(line_endings == LE_CRLF)
		le = "\r\n";
	int be16 = findarg(charset, CHARSET_UTF16_LE, CHARSET_UTF16_BE);
	if(be16 >= 0) {
		Ткст wle;
		for(int i = 0; i < le.дайСчёт(); i++) {
			if(be16)
				wle.конкат(0);
			wle.конкат(le[i]);
			if(!be16)
				wle.конкат(0);
		}
		for(int i = 0; i < дайСчётСтрок(); i++) {
			if(i)
				s.помести(wle);
			ШТкст txt = дайШСтроку(i);
			const wchar *e = txt.стоп();
			if(be16)
				for(const wchar *w = txt; w != e; w++)
					if(*w < 0x10000)
						s.Put16be((word)*w);
					else {
						char16 h[2];
						вУтф16(h, w, 1);
						s.Put16be(h[0]);
						s.Put16be(h[1]);
					}
			else
				for(const wchar *w = txt; w != e; w++)
					if(*w < 0x10000)
						s.Put16le((word)*w);
					else {
						char16 h[2];
						вУтф16(h, w, 1);
						s.Put16le(h[0]);
						s.Put16le(h[1]);
					}
		}
		return;
	}
	for(int i = 0; i < дайСчётСтрок(); i++) {
		if(i)
			s.помести(le);
		if(charset == CHARSET_UTF8)
			s.помести(дайУтф8Строку(i));
		else {
			Ткст txt = изЮникода(дайШСтроку(i), charset);
			const char *e = txt.стоп();
			for(const char *w = txt; w != e; w++)
				s.помести(*w == DEFAULTCHAR ? '?' : *w);
		}
	}
}

void   ТекстКтрл::уст(const Ткст& s, byte charset) {
	ТкстПоток ss(s);
	грузи(ss, charset);
}

Ткст ТекстКтрл::дай(byte charset) const
{
	ТкстПоток ss;
	сохрани(ss, charset);
	return ss;
}

int ТекстКтрл::GetInvalidCharPos(byte charset) const
{
	int q = 0;
	if(!IsUnicodeCharset(charset))
		for(int i = 0; i < дайСчётСтрок(); i++) {
			ШТкст txt = дайШСтроку(i);
			ШТкст ctxt = вЮникод(изЮникода(txt, charset), charset);
			for(int w = 0; w < txt.дайДлину(); w++)
				if(txt[w] != ctxt[w])
					return q + w;
			q += txt.дайДлину() + 1;
		}
	return -1;
}

void   ТекстКтрл::ClearDirty()
{
	dirty = 0;
	ClearModify();
	WhenState();
}

ТекстКтрл::ОтмениДанные ТекстКтрл::PickUndoData()
{
	ОтмениДанные data;
	data.undo = pick(undo);
	data.redo = pick(redo);
	data.undoserial = undoserial;
	return data;
}

void ТекстКтрл::SetPickUndoData(ТекстКтрл::ОтмениДанные&& data)
{
	undo = pick(data.undo);
	redo = pick(data.redo);
	undoserial = data.undoserial;
	incundoserial = true;
}

void ТекстКтрл::уст(const ШТкст& s)
{
	очисть();
	вставь0(0, s);
}

void  ТекстКтрл::устДанные(const Значение& v)
{
	уст((ШТкст)v);
}

Значение ТекстКтрл::дайДанные() const
{
	return дайШ();
}

Ткст ТекстКтрл::GetEncodedLine(int i, byte charset) const
{
	charset = разрешиНабСим(charset);
	Ткст h = дайУтф8Строку(i);
	return charset == CHARSET_UTF8 ? h : изЮникода(вУтф32(h), charset);
}

int   ТекстКтрл::дайПозСтроки64(int64& pos) const {
	ЗамкниГип __;
	if(pos < cpos && cpos - pos < pos && !view) {
		int i = cline;
		int64 ps = cpos;
		for(;;) {
			ps -= дайДлинуСтроки(--i) + 1;
			if(ps <= pos) {
				pos = pos - ps;
				return i;
			}
		}
	}
	else {
		int i = 0;
		if(view) {
			ЗамкниГип __;
			int blk = 0;
			for(;;) {
				int n = total256[blk];
				if(pos < n)
					break;
				pos -= n;
				blk++;
				if(blk >= total256.дайСчёт()) {
					pos = дайДлинуСтроки(дайСчётСтрок() - 1);
					return дайСчётСтрок() - 1;
				}
			}
			i = blk << 8;
		}
		else
		if(pos >= cpos) {
			pos -= cpos;
			i = cline;
		}
		for(;;) {
			int n = дайДлинуСтроки(i) + 1;
			if(pos < n) return i;
			pos -= n;
			i++;
			if(i >= дайСчётСтрок()) {
				pos = дайДлинуСтроки(дайСчётСтрок() - 1);
				return дайСчётСтрок() - 1;
			}
		}
	}
	return 0; // just silencing GCC warning, cannot get here
}

int64  ТекстКтрл::дайПоз64(int ln, int lpos) const {
	ЗамкниГип __;
	ln = minmax(ln, 0, дайСчётСтрок() - 1);
	int i;
	int64 pos;
	if(ln < cline && cline - ln < ln && !view) {
		pos = cpos;
		i = cline;
		while(i > ln)
			pos -= дайДлинуСтроки(--i) + 1;
	}
	else {
		pos = 0;
		i = 0;
		if(view) {
			for(int j = 0; j < ln >> 8; j++) {
				pos += total256[j];
				i += 256;
			}
		}
		else
		if(ln >= cline) {
			pos = cpos;
			i = cline;
		}
		while(i < ln)
			pos += дайДлинуСтроки(i++) + 1;
	}
	return pos + min(дайДлинуСтроки(ln), lpos);
}

ШТкст ТекстКтрл::дайШ(int64 pos, int size) const
{
	int i = дайПозСтроки64(pos);
	ШТкстБуф r;
	for(;;) {
		if(i >= дайСчётСтрок()) break;
		ШТкст ln = дайШСтроку(i++);
		int sz = min(ограничьРазм(ln.дайДлину() - pos), size);
		if(pos == 0 && sz == ln.дайДлину())
			r.конкат(ln);
		else
			r.конкат(ln.середина((int)pos, sz));
		size -= sz;
		if(size == 0) break;
#ifdef PLATFORM_WIN32
		r.конкат('\r');
#endif
		r.конкат('\n');
		size--;
		if(size == 0) break;
		pos = 0;
	}
	return ШТкст(r);
}

Ткст ТекстКтрл::дай(int64 pos, int size, byte charset) const
{
	if(charset == CHARSET_UTF8) {
		int i = дайПозСтроки64(pos);
		ТкстБуф r;
		for(;;) {
			if(i >= дайСчётСтрок()) break;
			int sz = min(ограничьРазм(дайДлинуСтроки(i) - pos), size);
			const Ткст& h = дайУтф8Строку(i);
			const char *s = h;
			int n = h.дайСчёт();
			i++;
			if(pos == 0 && sz == n)
				r.конкат(s, n);
			else
				r.конкат(вУтф32(s, n).середина((int)pos, sz).вТкст());
			size -= sz;
			if(size == 0) break;
	#ifdef PLATFORM_WIN32
			r.конкат('\r');
	#endif
			r.конкат('\n');
			size--;
			if(size == 0) break;
			pos = 0;
		}
		return Ткст(r);
	}
	return изЮникода(дайШ(pos, size), charset);
}

int  ТекстКтрл::дайСим(int64 pos) const {
	if(pos < 0 || pos >= дайДлину64())
		return 0;
	int i = дайПозСтроки64(pos);
	ШТкст ln = дайШСтроку(i);
	int c = ln.дайДлину() == pos ? '\n' : ln[(int)pos];
	return c;
}

int ТекстКтрл::дайПозСтроки32(int& pos) const
{
	int64 p = pos;
	int l = дайПозСтроки64(p);
	pos = (int)p;
	return l;
}

bool ТекстКтрл::дайВыделение32(int& l, int& h) const
{
	int64 ll, hh;
	bool b = дайВыделение(ll, hh);
	if(hh >= INT_MAX) {
		l = h = (int)cursor;
		return false;
	}
	l = (int)ll;
	h = (int)hh;
	return b;
}

int ТекстКтрл::дайКурсор32() const
{
	int64 h = дайКурсор64();
	return h < INT_MAX ? (int)h : 0;
}

int ТекстКтрл::дайДлину32() const
{
	int64 h = дайДлину64();
	return h < INT_MAX ? (int)h : 0;
}

int ТекстКтрл::вставь0(int pos, const ШТкст& txt) { // TODO: Do this with utf8
	ЗамкниГип __;
	int inspos = pos;
	вставьПеред(inspos, txt);
	if(pos < cpos)
		cpos = cline = 0;
	int i = дайПозСтроки32(pos);
	DirtyFrom(i);
	int size = 0;

	ШТкстБуф lnb;
	Вектор<ШТкст> iln;
	const wchar *s = txt;
	while(s < txt.стоп())
		if(*s >= ' ') {
			const wchar *b = s;
			while(*s >= ' ') // txt is zero teminated...
				s++;
			int sz = int(s - b);
			lnb.конкат(b, sz);
			size += sz;
		}
		else
		if(*s == '\t') {
			lnb.конкат(*s);
			size++;
			s++;
		}
		else
		if(*s == '\n') {
			iln.добавь(lnb);
			size++;
			lnb.очисть();
			s++;
		}
		else
			s++;
	ШТкст ln = lnb;
	ШТкст l = дайШСтроку(i);
	if(iln.дайСчёт()) {
		iln[0] = l.середина(0, pos) + iln[0];
		ln.конкат(l.середина(pos));
		устСтроку(i, ln);
		инвалидируйСтроку(i);
		вставьСтроку(i, iln.дайСчёт());
		for(int j = 0; j < iln.дайСчёт(); j++)
			устСтроку(i + j, iln[j]);
		вставьСтроки(i, iln.дайСчёт());
		освежи();
	}
	else {
		устСтроку(i, l.середина(0, pos) + ln + l.середина(pos));
		инвалидируйСтроку(i);
		освежиСтроку(i);
	}
	total += size;
	SetSb();
	Update();
	очистьВыделение();
	вставьПосле(inspos, txt);
	return size;
}

void ТекстКтрл::удали0(int pos, int size) {
	ЗамкниГип __;
	int rmpos = pos, rmsize = size;
	удалиПеред(rmpos, rmsize);
	total -= size;
	if(pos < cpos)
		cpos = cline = 0;
	int i = дайПозСтроки32(pos);
	DirtyFrom(i);
	ШТкст ln = дайШСтроку(i);
	int sz = min(ограничьРазм(ln.дайДлину() - pos), size);
	ln.удали(pos, sz);
	size -= sz;
	устСтроку(i, ln);
	if(size == 0) {
		инвалидируйСтроку(i);
		освежиСтроку(i);
	}
	else {
		size--;
		int j = i + 1;
		for(;;) {
			int sz = дайДлинуСтроки(j) + 1;
			if(sz > size) break;
			j++;
			size -= sz;
		}
		ШТкст p1 = дайШСтроку(i);
		ШТкст p2 = дайШСтроку(j);
		p1.вставь(p1.дайДлину(), p2.середина(size, p2.дайДлину() - size));
		устСтроку(i, p1);
		удалиСтроку(i + 1, j - i);
		удалиСтроки(i + 1, j - i);
		инвалидируйСтроку(i);
		освежи();
	}
	Update();
	очистьВыделение();
	удалиПосле(rmpos, rmsize);
	SetSb();
}

void ТекстКтрл::Undodo()
{
	while(undo.дайСчёт() > undosteps)
		undo.сбросьГолову();
	redo.очисть();
}

void ТекстКтрл::NextUndo()
{
	undoserial += incundoserial;
	incundoserial = false;
}

void ТекстКтрл::IncDirty() {
	dirty++;
	if(dirty == 0 || dirty == 1)
	{
		if(dirty)
			SetModify();
		else
			ClearModify();
		WhenState();
	}
}

void ТекстКтрл::DecDirty() {
	dirty--;
	if(dirty == 0 || dirty == -1)
	{
		if(dirty)
			SetModify();
		else
			ClearModify();
		WhenState();
	}
}

int ТекстКтрл::InsertU(int pos, const ШТкст& txt, bool typing) {
	int sz = вставь0(pos, txt);
	if(undosteps) {
		if(undo.дайСчёт() > 1 && typing && *txt != '\n' && IsDirty()) {
			UndoRec& u = undo.дайХвост();
			if(u.typing && u.pos + u.size == pos) {
				u.size += txt.дайДлину();
				return sz;
			}
		}
		UndoRec& u = undo.добавьХвост();
		incundoserial = true;
		IncDirty();
		u.serial = undoserial;
		u.pos = pos;
		u.size = sz;
		u.typing = typing;
	}
	return sz;
}

void ТекстКтрл::RemoveU(int pos, int size) {
	if(size + pos > total)
		size = int(total - pos);
	if(size <= 0) return;
	if(undosteps) {
		UndoRec& u = undo.добавьХвост();
		incundoserial = true;
		IncDirty();
		u.serial = undoserial;
		u.pos = pos;
		u.size = 0;
		u.устТекст(дай(pos, size, CHARSET_UTF8));
		u.typing = false;
	}
	удали0(pos, size);
}

int ТекстКтрл::вставь(int pos, const ШТкст& _txt, bool typing) {
	if(pos + _txt.дайСчёт() > max_total)
		return 0;
	ШТкст txt = _txt;
	if(!IsUnicodeCharset(charset))
		for(int i = 0; i < txt.дайСчёт(); i++)
			if(изЮникода(txt[i], charset) == DEFAULTCHAR)
				txt.уст(i, '?');
	int sz = InsertU(pos, txt, typing);
	Undodo();
	return sz;
}

int ТекстКтрл::вставь(int pos, const Ткст& txt, byte charset)
{
	return вставь(pos, вЮникод(txt, charset), false);
}

void ТекстКтрл::удали(int pos, int size) {
	RemoveU(pos, size);
	Undodo();
}

void ТекстКтрл::Undo() {
	if(undo.пустой()) return;
	undo_op = true;
	int nc = 0;
	int s = undo.дайХвост().serial;
	while(undo.дайСчёт()) {
		const UndoRec& u = undo.дайХвост();
		if(u.serial != s)
			break;
		UndoRec& r = redo.добавьХвост();
		r.serial = s;
		r.typing = false;
		nc = r.pos = u.pos;
		CachePos(r.pos);
		if(u.size) {
			r.size = 0;
			r.устТекст(дай(u.pos, u.size, CHARSET_UTF8));
			удали0(u.pos, u.size);
		}
		else {
			ШТкст text = вУтф32(u.дайТекст());
			r.size = вставь0(u.pos, text);
			nc += r.size;
		}
		undo.сбросьХвост();
		DecDirty();
	}
	очистьВыделение();
	поместиКаретку(nc, false);
	Action();
	undo_op = false;
}

void ТекстКтрл::Redo() {
	if(!redo.дайСчёт()) return;
	NextUndo();
	int s = redo.дайХвост().serial;
	int nc = 0;
	while(redo.дайСчёт()) {
		const UndoRec& r = redo.дайХвост();
		if(r.serial != s)
			break;
		nc = r.pos + r.size;
		CachePos(r.pos);
		if(r.size)
			RemoveU(r.pos, r.size);
		else
			nc += InsertU(r.pos, вУтф32(r.дайТекст()));
		redo.сбросьХвост();
		IncDirty();
	}
	очистьВыделение();
	поместиКаретку(nc, false);
	Action();
}

void  ТекстКтрл::очистьВыделение() {
	if(anchor >= 0) {
		anchor = -1;
		освежи();
		SelectionChanged();
		WhenSel();
	}
}

void   ТекстКтрл::устВыделение(int64 l, int64 h) {
	if(l != h) {
		поместиКаретку(minmax(l, (int64)0, total), false);
		поместиКаретку(minmax(h, (int64)0, total), true);
	}
	else
		устКурсор(l);
}

bool   ТекстКтрл::дайВыделение(int64& l, int64& h) const {
	if(anchor < 0 || anchor == cursor) {
		l = h = cursor;
		return false;
	}
	else {
		l = min(anchor, cursor);
		h = max(anchor, cursor);
		return !rectsel;
	}
}

Ткст ТекстКтрл::дайВыделение(byte charset) const {
	int64 l, h;
	if(дайВыделение(l, h))
		return дай(l, ограничьРазм(h - l), charset);
	return Ткст();
}

ШТкст ТекстКтрл::GetSelectionW() const {
	int64 l, h;
	if(дайВыделение(l, h))
		return дайШ(l, ограничьРазм(h - l));
	return ШТкст();
}

bool   ТекстКтрл::удалиВыделение() {
	int64 l, h;
	if(anchor < 0) return false;
	if(IsRectSelection())
		l = RemoveRectSelection();
	else {
		if(!дайВыделение(l, h))
			return false;
		удали((int)l, int(h - l));
	}
	anchor = -1;
	освежи();
	поместиКаретку(l);
	Action();
	return true;
}

void ТекстКтрл::освежиСтроки(int l1, int l2) {
	int h = max(l1, l2);
	for(int i = min(l1, l2); i <= h; i++)
		освежиСтроку(i);
}

void ТекстКтрл::вырежь() {
	if(!толькочтен_ли() && IsAnySelection()) {
		копируй();
		удалиВыделение();
	}
}

void ТекстКтрл::копируй() {
	int64 l, h;
	if(!дайВыделение(l, h) && !IsAnySelection()) {
		int i = дайСтроку(cursor);
		l = дайПоз64(i);
		h = l + дайДлинуСтроки(i) + 1;
	}
	ШТкст txt;
	if(IsRectSelection())
		txt = CopyRectSelection();
	else
		txt = дайШ(l, ограничьРазм(h - l));
	ClearClipboard();
	AppendClipboardUnicodeText(txt);
	AppendClipboardText(txt.вТкст());
}

void ТекстКтрл::выбериВсе() {
	устВыделение();
}

int  ТекстКтрл::Paste(const ШТкст& text) {
	if(толькочтен_ли()) return 0;
	int n;
	if(IsRectSelection())
		n = PasteRectSelection(text);
	else {
		удалиВыделение();
		n = вставь((int)cursor, text);
		поместиКаретку(cursor + n);
	}
	освежи();
	return n;
}

Ткст ТекстКтрл::дайВставьТекст()
{
	return Null;
}

void ТекстКтрл::Paste() {
	ШТкст w = ReadClipboardUnicodeText();
	if(w.пустой())
		w = ReadClipboardText().вШТкст();
	if(w.пустой())
		w = дайВставьТекст().вШТкст();
	Paste(w);
	Action();
}

void ТекстКтрл::StdBar(Бар& menu) {
	NextUndo();
	if(undosteps) {
		menu.добавь(undo.дайСчёт() && редактируем_ли(), t_("Undo"), CtrlImg::undo(), THISBACK(Undo))
			.Ключ(K_ALT_BACKSPACE)
			.Ключ(K_CTRL_Z);
		menu.добавь(redo.дайСчёт() && редактируем_ли(), t_("Redo"), CtrlImg::redo(), THISBACK(Redo))
			.Ключ(K_SHIFT|K_ALT_BACKSPACE)
			.Ключ(K_SHIFT_CTRL_Z);
		menu.Separator();
	}
	menu.добавь(редактируем_ли() && IsAnySelection(),
			t_("вырежь"), CtrlImg::cut(), THISBACK(вырежь))
		.Ключ(K_SHIFT_DELETE)
		.Ключ(K_CTRL_X);
	menu.добавь(IsAnySelection(),
			t_("копируй"), CtrlImg::copy(), THISBACK(копируй))
		.Ключ(K_CTRL_INSERT)
		.Ключ(K_CTRL_C);
	bool canpaste = редактируем_ли() && IsClipboardAvailableText();
	menu.добавь(canpaste,
			t_("Paste"), CtrlImg::paste(), THISBACK(DoPaste))
		.Ключ(K_SHIFT_INSERT)
		.Ключ(K_CTRL_V);
	СтрокРедакт *e = dynamic_cast<СтрокРедакт *>(this);
	if(e) {
		menu.добавь(canpaste,
				 t_("Paste in column"), CtrlImg::paste_vert(), callback(e, &СтрокРедакт::DoPasteColumn))
			.Ключ(K_ALT_V|K_SHIFT);
		menu.добавь(e->IsRectSelection(),
				 t_("сортируй"), CtrlImg::sort(), callback(e, &СтрокРедакт::сортируй));
	}
	menu.добавь(редактируем_ли() && IsAnySelection(),
			t_("Erase"), CtrlImg::remove(), THISBACK(DoRemoveSelection))
		.Ключ(K_DELETE);
	menu.Separator();
	menu.добавь(дайДлину64(),
			t_("выдели all"), CtrlImg::select_all(), THISBACK(выбериВсе))
		.Ключ(K_CTRL_A);
}

Ткст ТекстКтрл::GetSelectionData(const Ткст& fmt) const
{
	return GetTextClip(GetSelectionW(), fmt);
}

void ТекстКтрл::ПозРедакт::сериализуй(Поток& s) {
	int version = 1;
	s / version;
	if(version >= 1)
		s % sby % cursor;
	else {
		int c = (int)cursor;
		s % sby % c;
		cursor = c;
	}
}

}
