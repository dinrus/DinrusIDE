#include "Debuggers.h"

#ifdef PLATFORM_WIN32

void Pdb::Visual::конкат(const Ткст& text, Цвет ink)
{
	VisualPart& p = part.добавь();
	p.text = text;
	p.ink = ink;
	p.mark = false;
	length += text.дайДлину();
	if(length > 500)
		throw LengthLimit();
}

void Pdb::Visual::конкат(const char *s, Цвет ink)
{
	конкат(Ткст(s), ink);
}

Ткст Pdb::Visual::дайТкст() const
{
	Ткст r;
	for(int i = 0; i < part.дайСчёт(); i++)
		if((byte)*part[i].text >= 32)
			r << part[i].text;
	return r;
}

bool IsOk(const Ткст& q)
{
	const char *s = ~q;
	const char *e = q.стоп();
	while(s < e) {
		if((byte)*s < 32 && *s != '\t' && *s != '\r' && *s != '\n' && *s != '\f')
			return false;
		s++;
	}
	return true;
}

Pdb::Val Pdb::дайАтр(Pdb::Val record, int i)
{
	const Type& t = дайТип(record.тип);
	Val r;
	if(i < t.member.дайСчёт()) {
		r = t.member[i];
		r.address += record.address;
	}
	return r;
}

bool Pdb::HasAttr(Pdb::Val record, const Ткст& id)
{
	const Type& t = дайТип(record.тип);
	int q = t.member.найди(id);
	if(q >= 0)
		return true;
	for(int i = 0; i < t.base.дайСчёт(); i++) {
		Val b = t.base[i];
		b.address += record.address;
		Val v = дайАтр(b, id);
		if(v.тип != UNKNOWN)
			return true;
	}
	return false;
}

Pdb::Val Pdb::дайАтр(Pdb::Val record, const Ткст& id)
{
	const Type& t = дайТип(record.тип);
	int q = t.member.найди(id);
	if(q >= 0)
		return дайАтр(record, q);
	for(int i = 0; i < t.base.дайСчёт(); i++) {
		Val b = t.base[i];
		b.address += record.address;
		Val v = дайАтр(b, id);
		if(v.тип != UNKNOWN)
			return v;
	}
	return Val();
}

Pdb::Val Pdb::по(Pdb::Val val, int i)
{ // get n-th element relative to val
	if(val.ref)
		val = DeRef(val);
	val.address += i * размТипа(val.тип);
	val.array = false;
	return val;
}

Pdb::Val Pdb::по(Pdb::Val record, const char *id, int i)
{
	return по(дайАтр(record, id), i);
}

int Pdb::IntAt(Pdb::Val record, const char *id, int i)
{
	return (int)дайЦел(по(record, id, i));
}

void Pdb::CatInt(Visual& result, int64 val, dword flags)
{
	if(val < 0)
		result.конкат("-" + фмт64(-val), SRed);
	else
		result.конкат(фмт64(val), SRed);
	if(flags & MEMBER)
		return;
	if(val >= 32 && val < 128)
		result.конкат(Ткст() << " \'" << (char)val << '\'', SRed);
	result.конкат(" 0x" + фмт64Гекс(val), SMagenta);
}

void Pdb::Visualise(Visual& result, Pdb::Val val, dword flags)
{
	DR_LOG("Visualise");
	const int maxlen = 300;
	if(result.length > maxlen)
		return;
	if(val.ref > 0 || val.тип < 0) // if pointer or primitive тип, fetch it from the memory
		val = GetRVal(val);
	if(val.ref > 0) {
		if(!val.reference)
			result.конкат(Гекс(val.address), SLtMagenta);
		while(val.ref > 1) {
			val = GetRVal(DeRef(val));
			if(!val.reference) {
				result.конкат("->");
				result.конкат(Гекс(val.address), SLtMagenta);
			}
		}
		if((val.тип == UINT1 || val.тип == SINT1) && !val.reference) { // show string at [unsigned] char *
			if(Байт(val.address) < 0)
				result.конкат("??", SColorDisabled);
			else {
				int n = flags & MEMBER ? 10 : 200;
				Ткст x = читайТкст(val.address, n);
				Ткст dt;
				if(x.дайДлину() > n) {
					x.обрежь(x.дайДлину() - 1);
					dt = "..";
				}
				result.конкат(" ");
				result.конкат(FormatString(x), SRed);
				result.конкат(dt, SGray);
			}
			return;
		}
		if(!(flags & MEMBER) && val.тип != UNKNOWN && val.address) {
			if(!val.reference)
				result.конкат("->", SColorMark);
			int sz = размТипа(val.тип);
			int n = 40;
			Ткст dt = "..";
			if(val.reference) {
				n = 1;
				dt.очисть();
			}
			else
			if(val.reported_size > sz && sz > 0 && val.array) {
				n = val.reported_size / sz;
				if(n <= 40)
					dt.очисть();
				n = min(40, n);
			}
			for(int i = 0; i < n; i++) {
				if(i)
					result.конкат(", ", SGray);
				Visualise(result, DeRef(val), flags | (val.reference ? 0 : MEMBER));
				val.address += sz;
				if(Байт(val.address) < 0) {
					dt.очисть();
					break;
				}
			}
			result.конкат(dt, SGray);
		}
		return;
	}
	if(val.тип < 0) { // Дисплей primitive тип
		#define RESULTINT(x, тип) case x: CatInt(result, (тип)val.ival, flags); break;
		#define RESULTINTN(x, тип, t2) case x:  if(пусто_ли((t2)val.ival)) result.конкат("Null ", SCyan); CatInt(result, (тип)val.ival, flags); break;
		switch(val.тип) {
		RESULTINT(BOOL1, bool)
		RESULTINT(UINT1, byte)
		RESULTINT(SINT1, int8)
		RESULTINT(UINT2, uint16)
		RESULTINT(SINT2, int16)
		RESULTINT(UINT4, uint32)
		RESULTINT(UINT8, uint64)
		RESULTINTN(SINT4, int32, int)
		RESULTINTN(SINT8, int64, int64)
		case DBL:
		case FLT:
			if(пусто_ли(val.fval))
				result.конкат("Null", SCyan);
			else
			if(БЕСК_ли(val.fval))
				result.конкат("INF", SMagenta);
			else
			if(НЧ_ли(val.fval))
				result.конкат("NAN", SMagenta);
			else
				result.конкат(какТкст(val.fval), SRed);
			break;
		case PFUNC: {
			result.конкат(Гекс(val.address), SRed);
			FnInfo fi = GetFnInfo(val.address);
			if(!пусто_ли(fi.имя)) {
				result.конкат("->", SColorMark);
				result.конкат(fi.имя, SColorText);
			}
			break;
		}
		default:
			result.конкат("<void>", SColorMark);
		}
		return;
	}
	const Type& t = дайТип(val.тип);
	if(t.vtbl_typeindex == -2) {
		result.конкат(Nvl(GetFnInfo(val.address).имя, "??"), SColorText);
		return;
	}
	
	if(show_type)
		result.конкат(t.имя + ' ', SGreen);
	
	if(!(flags & RAW) && !raw)
		try {
			if(VisualisePretty(result, val, flags)) {
				if(flags & MEMBER)
					return;
				result.конкат(", raw: ", SGray);
			}
		}
		catch(СиПарсер::Ошибка e) {} // if failed, дисплей as raw data
	
	result.конкат("{ ", SColorMark);
	bool cm = false;
	for(int i = 0; i < t.member.дайСчёт(); i++) {
		if(cm)
			result.конкат(", ");
		cm = true;
		if(result.length > maxlen) {
			result.конкат("..");
			break;
		}
		result.конкат(t.member.дайКлюч(i));
		result.конкат("=", SColorMark);
		Val r = дайАтр(val, i);
		try {
			Visualise(result, r, flags | MEMBER);
		}
		catch(СиПарсер::Ошибка e) {
			result.конкат(e, SColorDisabled);
		}
	}
#if 0
	for(int i = 0; i < t.static_member.дайСчёт(); i++) {
		if(cm)
			result.конкат(", ");
		cm = true;
		if(result.length > maxlen) {
			result.конкат("..");
			break;
		}
		result.конкат(t.static_member.дайКлюч(i));
		result.конкат("=", SColorMark);
		try {
			Visualise(result, t.static_member[i], flags | MEMBER);
		}
		catch(СиПарсер::Ошибка e) {
			result.конкат(e, SColorDisabled);
		}
	}
#endif
	BaseFields(result, t, val, flags, cm, 0);
	result.конкат(" }", SColorMark);
}

void Pdb::BaseFields(Visual& result, const Type& t, Pdb::Val val, dword flags, bool& cm, int depth)
{
	for(int i = 0; i < t.base.дайСчёт(); i++) {
		const Val& b = t.base[i];
		if(b.тип >= 0) {
			adr_t adr = b.address + val.address;
			const Type& t = дайТип(b.тип);
			for(int i = 0; i < t.member.дайСчёт(); i++) {
				if(cm)
					result.конкат(", ");
				cm = true;
				if(result.length > 300) {
					result.конкат("..");
					break;
				}
				result.конкат(t.member.дайКлюч(i));
				result.конкат("=", SColorMark);
				Val r = t.member[i];
				r.address += adr;
				try {
					Visualise(result, r, flags | MEMBER);
				}
				catch(СиПарсер::Ошибка e) {
					result.конкат(e, SColorDisabled);
				}
			}
			if(depth < 30)
				BaseFields(result, t, val, flags, cm, depth + 1);
		}
	}
}

Размер Pdb::Visual::дайРазм() const
{
	int cx = 0;
	for(int i = 0; i < part.дайСчёт(); i++)
		cx += дайРазмТекста(part[i].text, StdFont()).cx;
	return Размер(cx, StdFont().Info().дайВысоту());
}

Pdb::Visual Pdb::Visualise(Val v, dword flags)
{
	Visual r;
	try {
		Visualise(r, v, flags);
	}
	catch(LengthLimit) {}
	catch(СиПарсер::Ошибка e) {
		r.конкат(e, SColorDisabled);
	}
	return r;
}

Pdb::Visual Pdb::Visualise(const Ткст& exp, dword flags)
{
	Visual r;
	try {
		СиПарсер p(exp);
		Val v = Exp(p);
		Visualise(r, v, 0);
	}
	catch(LengthLimit) {}
	catch(СиПарсер::Ошибка e) {
		r.конкат(e, SColorDisabled);
	}
	return r;
}

Размер Pdb::VisualPart::дайРазм() const
{
	return дайРазмТекста(*text && *text < 32 ? "MM" : ~text, StdFont());
}

Размер Pdb::VisualDisplay::дайСтдРазм(const Значение& q) const
{
	if(!IsType<Visual>(q))
		return StdDisplay().дайСтдРазм(q);
	Размер sz(0, GetStdFontCy());
	if(IsType<Visual>(q))
		for(const VisualPart& p : ValueTo<Visual>(q).part)
			sz.cx += p.дайРазм().cx;
	return sz;
}

void Pdb::VisualDisplay::рисуй(Draw& w, const Прям& r, const Значение& q,
                               Цвет ink, Цвет paper, dword style) const
{
	if(!IsType<Visual>(q)) {
		StdDisplay().рисуй(w, r, q, ink, paper, style);
		return;
	}
	int x = r.left;
	int y = r.top + (r.устВысоту() - Draw::GetStdFontCy()) / 2;
	bool blue = (style & (Дисплей::CURSOR|Дисплей::FOCUS)) == (Дисплей::CURSOR|Дисплей::FOCUS);
	for(const VisualPart& p : ValueTo<Visual>(q).part) {
		Размер sz = p.дайРазм();
		w.DrawRect(x, y, sz.cx, r.устВысоту(),
		           blue || !p.mark ? paper : HighlightSetup::GetHlStyle(HighlightSetup::PAPER_SELWORD).color);
		if(*p.text == '\1') { // Цвет support
			Прям r = RectC(x, y, sz.cx, sz.cy);
			r.дефлируй(DPI(1));
			w.DrawRect(r, SBlack);
			r.дефлируй(DPI(1));
			w.DrawRect(r, p.ink);
		}
		else
		if(*p.text == '\2') { // Рисунок support
			PrettyImage img;
			memcpy(&img, ~p.text + 1, sizeof(PrettyImage));
			Прям r = RectC(x, y, sz.cx, sz.cy);
			Рисунок m = DbgImg::Img();
			if(img.size.cx < 0 || img.size.cx > 10000 || img.size.cy < 0 || img.size.cy > 10000)
				m = DbgImg::ErrImg();
			else {
				int len = img.size.cx * img.size.cy;
				if(len < 70 * 70) {
					ImageBuffer ib(img.size);
					if(pdb->копируй(img.pixels, ~ib, len * 4)) {
						m = ib;
						m = Rescale(m, дайРазмСхождения(m.дайРазм(), sz));
					}
					else
						m = DbgImg::ErrImg();
				}
			}
			Размер isz = m.дайРазм();
			w.DrawImage(x + (sz.cx - isz.cx) / 2, y + (sz.cy - isz.cy) / 2, m);
		}
		else
			w.DrawText(x, y, p.text, StdFont(), blue ? ink : p.ink);
		x += sz.cx;
	}
	w.DrawRect(x, y, r.right - x, r.устВысоту(), paper);
}

#endif
