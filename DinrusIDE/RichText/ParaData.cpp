#include "RichText.h"

namespace РНЦП {

СтатическийСтопор RichPara::cache_lock;

Массив<RichPara>& RichPara::Кэш()
{
	static Массив<RichPara> x;
	return x;
}

void RichPara::CacheId(int64 id)
{
	ПРОВЕРЬ(cacheid == 0);
	ПРОВЕРЬ(part.дайСчёт() == 0);
	cacheid = id;
}

RichPara::RichPara()
{
	cacheid = 0;
	incache = false;
}

RichPara::~RichPara()
{
	if(cacheid && part.дайСчёт() && !incache) {
		Стопор::Замок __(cache_lock);
		Массив<RichPara>& cache = Кэш();
		incache = true;
		cache.вставьПодбор(0, pick(*this));
		int total = 0;
		for(int i = 1; i < cache.дайСчёт(); i++) {
			total += cache[i].дайДлину();
			if(total > 10000 || i > 64) {
				cache.устСчёт(i);
				break;
			}
		}
	}
}

PaintInfo::PaintInfo()
{
	sell = selh = 0;
	tablesel = 0;
	top = PageY(0, 0);
	bottom = PageY(INT_MAX, INT_MAX);
	hyperlink = SColorMark();
	usecache = false;
	sizetracking = false;
	showcodes = Null;
	spellingchecker = NULL;
	highlightpara = -1;
	highlight = жёлтый();
	indexentry = светлоЗелёный();
	indexentrybg = false;
	darktheme = false;
	context = NULL;
	showlabels = false;
	shrink_oversized_objects = false;
	textcolor = Null;
	DrawSelection = [] (Draw& w, int x, int y, int cx, int cy) {
		w.DrawRect(x, y, cx, cy, InvertColor);
	};
}

Ткст RichPara::Number::AsText(const RichPara::NumberFormat& формат) const
{
	Ткст result;
	for(int i = 0; i < 8; i++)
		if(формат.number[i]) {
			if(result.дайДлину())
				result.конкат('.');
			int q = n[i];
			switch(формат.number[i]) {
			case NUMBER_1:
				result << какТкст(q);
				break;
			case NUMBER_0:
				result << какТкст(q - 1);
				break;
			case NUMBER_a:
				result << фмтЦелАльфа(q, false);
				break;
			case NUMBER_A:
				result << фмтЦелАльфа(q, true);
				break;
			case NUMBER_i:
				result << фмтЦелРим(q, false);
				break;
			case NUMBER_I:
				result << фмтЦелРим(q, true);
				break;
			}
		}
	return формат.before_number + result + формат.after_number;
}

void   RichPara::Number::TestReset(const RichPara::NumberFormat& fmt)
{
	if(fmt.reset_number) {
		bool done = false;
		for(int i = 7; i >= 0; --i)
			if(fmt.number[i]) {
				n[i] = 0;
				done = true;
				break;
			}
		if(!done && !пусто_ли(n[0]))
			n[0] = 0;
	}
}

void   RichPara::Number::следщ(const RichPara::NumberFormat& fmt)
{
	for(int i = 7; i >= 0; --i)
		if(fmt.number[i]) {
			n[i++]++;
			while(i <= 7)
				n[i++] = 0;
			break;
		}
}

RichPara::Number::Number()
{
	memset8(n, 0, sizeof(n));
}

bool RichPara::NumberFormat::IsNumbered() const
{
	if(before_number.дайДлину() || after_number.дайДлину())
		return true;
	for(int i = 0; i < 8; i++)
		if(number[i])
		   return true;
	return false;
}

int RichPara::NumberFormat::GetNumberLevel() const
{
	for(int i = 7; i >= 0; i--)
		if(number[i])
		   return i + 1;
	if(before_number.дайДлину() || after_number.дайДлину())
		return 0;
	return -1;
}

bool NumberingDiffers(const RichPara::фмт& fmt1, const RichPara::фмт& fmt2)
{
	return fmt1.before_number != fmt2.before_number ||
	       fmt1.after_number != fmt2.after_number ||
	       fmt1.reset_number != fmt2.reset_number ||
	       memcmp(fmt1.number, fmt2.number, sizeof(fmt1.number));
}

RichPara::CharFormat::CharFormat()
{
	(Шрифт &)*this = Arial(100);
	ink = чёрный;
	paper = Null;
	language = 0;
	link = Null;
	sscript = 0;
	capitals = dashed = false;
}

RichPara::фмт::фмт()
{
	align = ALIGN_LEFT;
	ruler = before = lm = rm = indent = after = 0;
	rulerink = чёрный;
	rulerstyle = RULER_SOLID;
	bullet = 0;
	keep = newpage = firstonpage = keepnext = orphan = newhdrftr = false;
	tabsize = 296;
	memset8(number, 0, sizeof(number));
	reset_number = false;
	linespacing = 0;
	tab.очисть();
	styleid = RichStyle::GetDefaultId();
}

void RichPara::Charformat(Поток& out, const RichPara::CharFormat& o,
                          const RichPara::CharFormat& n, const RichPara::CharFormat& s)
{
	if(o.IsBold() != n.IsBold())
		out.помести(n.IsBold() == s.IsBold() ? BOLDS : BOLD0 + n.IsBold());
	if(o.IsItalic() != n.IsItalic())
		out.помести(n.IsItalic() == s.IsItalic() ? ITALICS
		                                     : ITALIC0 + n.IsItalic());
	if(o.IsUnderline() != n.IsUnderline())
		out.помести(n.IsUnderline() == s.IsUnderline() ? UNDERLINES
		                                           : UNDERLINE0 + n.IsUnderline());
	if(o.IsStrikeout() != n.IsStrikeout())
		out.помести(n.IsStrikeout() == s.IsStrikeout() ? STRIKEOUTS
		                                           : STRIKEOUT0 + n.IsStrikeout());
	if(o.IsNonAntiAliased() != n.IsNonAntiAliased()) {
		out.помести(EXT);
		out.помести(n.IsNonAntiAliased() == s.IsNonAntiAliased() ? NONAAS
		                                                     : NONAA0 + n.IsNonAntiAliased());
	}
	if(o.capitals != n.capitals)
		out.помести(n.capitals == s.capitals ? CAPITALSS
		                                 : CAPITALS0 + n.capitals);
	if(o.dashed != n.dashed)
		out.помести(n.dashed == s.dashed ? DASHEDS
		                             : DASHED0 + n.dashed);
	if(o.sscript != n.sscript) {
		out.помести(SSCRIPT);
		out.помести(n.sscript);
	}
	if(o.GetFace() != n.GetFace()) {
		out.помести(FACE);
		out.помести16(n.GetFace() == s.GetFace() ? 0xffff : n.GetFace());
	}
	if(o.дайВысоту() != n.дайВысоту()) {
		out.помести(HEIGHT);
		out.помести16(n.дайВысоту() == s.дайВысоту() ? 0xffff : n.дайВысоту());
	}
	if(o.link != n.link) {
		out.помести(LINK);
		Ткст s = n.link;
		out % s;
	}
	if(o.ink != n.ink) {
		out.помести(INK);
		Цвет c = n.ink;
		c.сериализуй(out);
	}
	if(o.paper != n.paper) {
		out.помести(PAPER);
		Цвет c = n.paper;
		c.сериализуй(out);
	}
	if(o.language != n.language) {
		out.помести(LANGUAGE);
		out.помести32(n.language);
	}
	if(o.indexentry != n.indexentry) {
		out.помести(INDEXENTRY);
		ШТкст s = n.indexentry;
		out % s;
	}
}

void RichPara::конкат(const ШТкст& s, const RichPara::CharFormat& f)
{
	cacheid = 0;
	part.добавь();
	part.верх().text = s;
	part.верх().формат = f;
}

void RichPara::конкат(const char *s, const CharFormat& f)
{
	конкат(ШТкст(s), f);
}

void RichPara::конкат(const RichObject& o, const RichPara::CharFormat& f)
{
	cacheid = 0;
	part.добавь();
	part.верх().object = o;
	part.верх().формат = f;
}

void RichPara::конкат(Ид field, const Ткст& param, const RichPara::CharFormat& f)
{
	cacheid = 0;
	Part& p = part.добавь();
	p.field = field;
	p.fieldparam = param;
	p.формат = f;
	ВекторМап<Ткст, Значение> dummy;
	FieldType *ft = fieldtype().дай(field, NULL);
	if(ft)
		p.fieldpart = pick(ft->Evaluate(param, dummy, f));
}

struct TabLess {
	bool operator () (const RichPara::Вкладка& a, const RichPara::Вкладка& b) const {
		return a.pos == b.pos ? a.align < b.align : a.pos < b.pos;
	}
};

void RichPara::фмт::сортируйТабы()
{
	сортируй(tab, TabLess());
}

void RichPara::PackParts(Поток& out, const RichPara::CharFormat& chrstyle,
                         const Массив<RichPara::Part>& part, CharFormat& cf,
                         Массив<RichObject>& obj) const
{
	for(int i = 0; i < part.дайСчёт(); i++) {
		const Part& p = part[i];
		Charformat(out, cf, p.формат, chrstyle);
		cf = p.формат;
		if(p.field) {
			out.помести(FIELD);
			Ткст s = ~p.field;
			out % s;
			s = p.fieldparam;
			out % s;
			ТкстПоток oout;
			CharFormat subf = cf;
			PackParts(oout, chrstyle, p.fieldpart, subf, obj);
			s = oout;
			out % s;
		}
		else
		if(p.object) {
			obj.добавь(p.object);
			out.помести(OBJECT);
		}
		else
			out.помести(вУтф8(p.text));
	}
}

Ткст RichPara::Pack(const RichPara::фмт& style, Массив<RichObject>& obj) const
{
	ТкстПоток out;
	dword pattr = 0;
	if(формат.align != style.align)             pattr |= 1;
	if(формат.before != style.before)           pattr |= 2;
	if(формат.lm != style.lm)                   pattr |= 4;
	if(формат.indent != style.indent)           pattr |= 8;
	if(формат.rm != style.rm)                   pattr |= 0x10;
	if(формат.after != style.after)             pattr |= 0x20;
	if(формат.bullet != style.bullet)           pattr |= 0x40;
	if(формат.keep != style.keep)               pattr |= 0x80;
	if(формат.newpage != style.newpage)         pattr |= 0x100;
	if(формат.tabsize != style.tabsize)         pattr |= 0x200;
	if(!пусто_ли(формат.label))                   pattr |= 0x400;
	if(формат.keepnext != style.keepnext)       pattr |= 0x800;
	if(формат.orphan != style.orphan)           pattr |= 0x1000;
	if(NumberingDiffers(формат, style))         pattr |= 0x2000;
	if(формат.linespacing != style.linespacing) pattr |= 0x4000;
	if(формат.tab != style.tab)                 pattr |= 0x8000;
	if(формат.ruler != style.ruler)             pattr |= 0x10000;
	if(формат.rulerink != style.rulerink)       pattr |= 0x20000;
	if(формат.rulerstyle != style.rulerstyle)   pattr |= 0x40000;
	if(формат.newhdrftr != style.newhdrftr)     pattr |= 0x80000;
	if(формат.firstonpage != style.firstonpage) pattr |= 0x100000;
	
	out.помести32(pattr);
	if(pattr & 1)      out.помести16(формат.align);
	if(pattr & 2)      out.помести16(формат.before);
	if(pattr & 4)      out.помести16(формат.lm);
	if(pattr & 8)      out.помести16(формат.indent);
	if(pattr & 0x10)   out.помести16(формат.rm);
	if(pattr & 0x20)   out.помести16(формат.after);
	if(pattr & 0x40)   out.помести16(формат.bullet);
	if(pattr & 0x80)   out.помести(формат.keep);
	if(pattr & 0x100)  out.помести(формат.newpage);
	if(pattr & 0x200)  out.помести16(формат.tabsize);
	if(pattr & 0x400)  { Ткст t = формат.label; out % t; }
	if(pattr & 0x800)  out.помести(формат.keepnext);
	if(pattr & 0x1000) out.помести(формат.orphan);
	if(pattr & 0x2000) {
		Ткст b = формат.before_number, a = формат.after_number;
		out % b % a;
		out.помести(формат.reset_number);
		out.помести(формат.number, 8);
	}
	if(pattr & 0x4000)
		out.помести(формат.linespacing);
	if(pattr & 0x8000) {
		int c = 0;
		int i;
		for(i = 0; i < формат.tab.дайСчёт(); i++) {
			if(!пусто_ли(формат.tab[i].pos))
				c++;
		}
		out.помести16(c);
		for(i = 0; i < формат.tab.дайСчёт(); i++) {
			const RichPara::Вкладка& w = формат.tab[i];
			if(!пусто_ли(w.pos)) {
				out.помести32(w.pos);
				out.помести(w.align);
				out.помести(w.fillchar);
			}
		}
	}
	if(pattr & 0x10000)
		out.помести16(формат.ruler);
	if(pattr & 0x20000) {
		Цвет c = формат.rulerink;
		c.сериализуй(out);
	}
	if(pattr & 0x40000)
		out.помести16(формат.rulerstyle);

	if(pattr & 0x80000) {
		out.помести(формат.newhdrftr);
		if(формат.newhdrftr) {
			Ткст t = формат.header_qtf;
			Ткст f = формат.footer_qtf;
			out % t % f;
		}
	}
	if(pattr & 0x100000)  out.помести(формат.firstonpage);

	obj.очисть();
	CharFormat cf = style;
	if(part.дайСчёт())
		PackParts(out, style, part, cf, obj);
	else
		Charformat(out, style, формат, cf);
	Ткст r = out;
	r.сожми();
	return r;
}

void RichPara::UnpackParts(Поток& in, const RichPara::CharFormat& chrstyle,
                           Массив<RichPara::Part>& part, const Массив<RichObject>& obj,
                           int& oi) {
	part.добавь();
	part.верх().формат = формат;
	int c;
	while((c = in.прекрати()) >= 0)
		if(c < 31 && c != 9 && c != FIELD) {
			do
				switch(in.дай()) {
				case BOLD0:
					формат.NoBold();
					break;
				case BOLD1:
					формат.Bold();
					break;
				case BOLDS:
					формат.Bold(chrstyle.IsBold());
					break;
				case ITALIC0:
					формат.NoItalic();
					break;
				case ITALIC1:
					формат.Italic();
					break;
				case ITALICS:
					формат.Italic(chrstyle.IsItalic());
					break;
				case UNDERLINE0:
					формат.NoUnderline();
					break;
				case UNDERLINE1:
					формат.Underline();
					break;
				case UNDERLINES:
					формат.Underline(chrstyle.IsUnderline());
					break;
				case STRIKEOUT0:
					формат.NoStrikeout();
					break;
				case STRIKEOUT1:
					формат.Strikeout();
					break;
				case STRIKEOUTS:
					формат.Strikeout(chrstyle.IsStrikeout());
					break;
				case CAPITALS0:
					формат.capitals = false;
					break;
				case CAPITALS1:
					формат.capitals = true;
					break;
				case CAPITALSS:
					формат.capitals = chrstyle.capitals;
					break;
				case DASHED0:
					формат.dashed = false;
					break;
				case DASHED1:
					формат.dashed = true;
					break;
				case DASHEDS:
					формат.dashed = chrstyle.dashed;
					break;
				case SSCRIPT:
					формат.sscript = in.дай();
					if(формат.sscript == 3)
						формат.sscript = chrstyle.sscript;
					break;
				case FACE:
					c = in.дай16();
					формат.Face(c == 0xffff ? chrstyle.GetFace() : c);
					break;
				case HEIGHT:
					c = in.дай16();
					формат.устВысоту(c == 0xffff ? chrstyle.дайВысоту() : c);
					break;
				case LINK:
					in % формат.link;
					break;
				case INDEXENTRY:
					in % формат.indexentry;
					break;
				case INK:
					in % формат.ink;
					break;
				case PAPER:
					in % формат.paper;
					break;
				case LANGUAGE:
					формат.language = in.дай32();
					break;
				case EXT:
					switch(in.дай()) {
					case NONAA0:
						формат.NonAntiAliased(false);
						break;
					case NONAA1:
						формат.NonAntiAliased(true);
						break;
					case NONAAS:
						формат.NonAntiAliased(chrstyle.IsNonAntiAliased());
						break;
					}
				}
			while((c = in.прекрати()) < 31 && c != 9 && c != FIELD && c >= 0);
			if(part.верх().text.дайДлину())
				part.добавь();
			part.верх().формат = формат;
		}
		else
		if(in.прекрати() == FIELD) {
			RichPara::фмт pformat = формат;
			if(part.верх().text.дайДлину()) {
				part.добавь();
				part.верх().формат = pformat;
			}
			in.дай();
			Part& p = part.верх();
			Ткст id;
			in % id;
			p.field = id;
			in % p.fieldparam;
			Ткст s;
			in % s;
			ТкстПоток sn(s);
			UnpackParts(sn, chrstyle, p.fieldpart, obj, oi);
			part.добавь();
			part.верх().формат = формат = pformat;
		}
		else
		if(in.прекрати() == OBJECT) {
			if(part.верх().text.дайДлину()) {
				part.добавь();
				part.верх().формат = формат;
			}
			part.верх().object = obj[oi++];
			part.верх().формат = формат;
			part.добавь();
			part.верх().формат = формат;
			in.дай();
		}
		else {
			ТкстБуф b;
			b.резервируй(512);
			while(in.прекрати() >= 32 || in.прекрати() == 9)
				b.конкат(in.дай());
			part.верх().text.конкат(вУтф32(~b));
		}
	if(part.верх().text.дайДлину() == 0 && part.верх().текст_ли())
		part.сбрось();
}

void RichPara::Unpack(const Ткст& data, const Массив<RichObject>& obj,
                      const RichPara::фмт& style)
{
	part.очисть();
	формат = style;
	
	if(cacheid) {
		Стопор::Замок __(cache_lock);
		Массив<RichPara>& cache = Кэш();
		for(int i = 0; i < cache.дайСчёт(); i++)
			if(cache[i].cacheid == cacheid) {
				*this = pick(cache[i]);
				incache = false;
				cache.удали(i);
				return;
			}
	}

	ТкстПоток in(data);
	dword pattr = in.дай32();

	if(pattr & 1)      формат.align = in.дай16();
	if(pattr & 2)      формат.before = in.дай16();
	if(pattr & 4)      формат.lm = in.дай16();
	if(pattr & 8)      формат.indent = in.дай16();
	if(pattr & 0x10)   формат.rm = in.дай16();
	if(pattr & 0x20)   формат.after = in.дай16();
	if(pattr & 0x40)   формат.bullet = in.дай16();
	if(pattr & 0x80)   формат.keep = in.дай();
	if(pattr & 0x100)  формат.newpage = in.дай();
	if(pattr & 0x200)  формат.tabsize = in.дай16();
	if(pattr & 0x400)  in % формат.label;
	if(pattr & 0x800)  формат.keepnext = in.дай();
	if(pattr & 0x1000) формат.orphan = in.дай();
	if(pattr & 0x2000) {
		in % формат.before_number;
		in % формат.after_number;
		формат.reset_number = in.дай();
		in.дай(формат.number, 8);
	}
	if(pattr & 0x4000) {
		формат.linespacing = (int8)in.дай();
	}
	if(pattr & 0x8000) {
		формат.tab.очисть();
		int n = in.дай16();
		формат.tab.резервируй(n);
		for(int i = 0; i < n; i++) {
			RichPara::Вкладка& w = формат.tab.добавь();
			w.pos = in.дай32();
			w.align = in.дай();
			w.fillchar = in.дай();
		}
	}
	if(pattr & 0x10000)
		формат.ruler = in.дай16();
	if(pattr & 0x20000)
		формат.rulerink.сериализуй(in);
	if(pattr & 0x40000)
		формат.rulerstyle = in.дай16();

	if(pattr & 0x80000) {
		формат.newhdrftr = in.дай();
		if(формат.newhdrftr)
			in % формат.header_qtf % формат.footer_qtf;
	}

	if(pattr & 0x100000) формат.firstonpage = in.дай();

	part.очисть();
	int oi = 0;
	UnpackParts(in, style, part, obj, oi);
}

bool RichPara::пустой() const
{
	return part.дайСчёт() == 0 || дайДлину() == 0;
}

int RichPara::дайДлину() const
{
	int n = 0;
	for(int i = 0; i < part.дайСчёт(); i++)
		n += part[i].дайДлину();
	return n;
}

ШТкст RichPara::дайТекст() const
{
	ШТкст r;
	for(int i = 0; i < part.дайСчёт(); i++)
		if(part[i].текст_ли())
			r.конкат(part[i].text);
		else
			r.конкат(127);
	return r;
}

ВекторМап<Ид, RichPara::FieldType *>& RichPara::fieldtype0()
{
	static ВекторМап<Ид, RichPara::FieldType *> h;
	return h;
}

void RichPara::регистрируй(Ид id, FieldType& ft)
{
	проверьНаОН();
	fieldtype0().дайДобавь(id, &ft);
}

bool RichPara::EvaluateFields(ВекторМап<Ткст, Значение>& vars)
{
	bool b = false;
	for(int i = 0; i < дайСчёт(); i++) {
		Part& p = part[i];
		if(p.field) {
			FieldType *f = fieldtype().дай(p.field, NULL);
			if(f) {
				p.fieldpart = pick(f->Evaluate(p.fieldparam, vars, p.формат));
				b = true;
			}
		}
	}
	return b;
}

bool RichPara::HasPos() const
{
	if(!формат.label.пустой()) return true;
	for(int i = 0; i < part.дайСчёт(); i++)
		if(!part[i].формат.indexentry.пустой())
			return true;
	return false;
}

int  RichPara::FindPart(int& pos) const
{
	int pi = 0;
	while(pi < part.дайСчёт() && pos >= part[pi].дайДлину()) {
		pos -= part[pi].дайДлину();
		pi++;
	}
	return pi;
}

void RichPara::обрежь(int pos)
{
	int i = FindPart(pos);
	if(pos) {
		ПРОВЕРЬ(part[i].текст_ли());
		part[i].text.обрежь(pos);
		part.устСчёт(i + 1);
	}
	else
		part.устСчёт(i);
	cacheid = 0;
}

void RichPara::середина(int pos)
{
	int i = FindPart(pos);
	part.удали(0, i);
	if(pos) {
		ПРОВЕРЬ(part[0].текст_ли());
		part[0].text = part[0].text.середина(pos);
	}
	cacheid = 0;
}

void ApplyCharStyle(RichPara::CharFormat& формат, const RichPara::CharFormat& f0,
                    const RichPara::CharFormat& newstyle) {
	if(формат.IsBold() == f0.IsBold())
		формат.Bold(newstyle.IsBold());
	if(формат.IsUnderline() == f0.IsUnderline())
		формат.Underline(newstyle.IsUnderline());
	if(формат.IsItalic() == f0.IsItalic())
		формат.Italic(newstyle.IsItalic());
	if(формат.IsStrikeout() == f0.IsStrikeout())
		формат.Strikeout(newstyle.IsStrikeout());
	if(формат.IsNonAntiAliased() == f0.IsNonAntiAliased())
		формат.NonAntiAliased(newstyle.IsNonAntiAliased());
	if(формат.capitals == f0.capitals)
		формат.capitals = newstyle.capitals;
	if(формат.dashed == f0.dashed)
		формат.dashed = newstyle.dashed;
	if(формат.sscript == f0.sscript)
		формат.sscript = newstyle.sscript;
	if(формат.GetFace() == f0.GetFace())
		формат.Face(newstyle.GetFace());
	if(формат.дайВысоту() == f0.дайВысоту())
		формат.устВысоту(newstyle.дайВысоту());
	if(формат.ink == f0.ink)
		формат.ink = newstyle.ink;
	if(формат.paper == f0.paper)
		формат.paper = newstyle.paper;
}

void RichPara::ApplyStyle(const фмт& newstyle)
{
	CharFormat f0 = part.дайСчёт() ? part[0].формат : формат;
	for(int i = 0; i < part.дайСчёт(); i++)
		ApplyCharStyle(part[i].формат, f0, newstyle);
	CharFormat h = формат;
	ApplyCharStyle(h, f0, newstyle);
	формат = newstyle;
	(CharFormat&)формат = h;
	cacheid = 0;
}

#ifdef _ОТЛАДКА
void RichPara::Dump()
{
	LOG("RichPara dump" << LOG_BEGIN);
	LOG("RULER: " << формат.ruler << " " << формат.rulerink << " " << формат.rulerstyle);
	LOG("BEFORE: " << формат.before);
	LOG("INDENT: " << формат.indent);
	LOG("LM: " << формат.lm);
	LOG("RM: " << формат.rm);
	LOG("AFTER: " << формат.after);
	LOG("KEEP: " << формат.keep);
	LOG("NEWPAGE: " << формат.newpage);
	LOG("BULLET: " << формат.bullet);
	int i;
	for(i = 0; i < формат.tab.дайСчёт(); i++)
		LOG("TAB " << формат.tab[i].pos << " : " << формат.tab[i].align);
	for(i = 0; i < part.дайСчёт(); i++)
		LOG("Part[" << i << "] = \"" << part[i].text << "\" "
		    << part[i].формат);
	LOG(LOG_END << "---------");
}


Ткст RichPara::CharFormat::вТкст() const
{
	Ткст out;
	out << Шрифт(*this) << ", ink " << ink;
	if(!РНЦП::пусто_ли(paper))
		out << ", paper " << paper;
	switch(sscript)
	{
	case 0:  break;
	case 1:  out << ", superscript"; break;
	case 2:  out << ", subscript"; break;
	default: out << ", sscript(" << (int)sscript << ")"; break;
	}
	out << ", lang " << LNGAsText(language);
	if(!РНЦП::пусто_ли(link))
		out << ", link " << link;
	if(capitals)
		out << ", capitals";
	if(dashed)
		out << ", dashed";
	return out;
}

Ткст RichPara::фмт::вТкст() const
{
	Ткст out;
	if(!РНЦП::пусто_ли(label))
		out << "label <" << label << ">: ";
	out
	<< align << ", left " << lm << ", right " << rm
	<< ", indent " << indent << ", before " << before << ", after " << after
	<< ", tabsize " << tabsize << ", bullet " << bullet
	<< (newpage  ? ", newpage" : "")
	<< (firstonpage  ? ", firstonpage" : "")
	<< (keep     ? ", keep" : "")
	<< (keepnext ? ", keepnext" : "")
	<< (orphan   ? ", orphan" : "");
	int i;
	for(i = 0; i < tab.дайСчёт(); i++)
		out << (i ? "\n" : ", ")
		<< "tab[" << i << "] = " << tab[i].pos << ", align " << tab[i].align
		<< ", fill " << фмтЦелГекс(tab[i].fillchar, 2);
	out << "\n";
	out << "before_number " << before_number << ", after_number " << after_number
	<< (reset_number ? ", reset_number" : "");
	for(i = 0; i < __countof(number); i++)
		if(number[i] != RichPara::NUMBER_NONE)
			out << " num[" << i << "] = " << (int)number[i];
	out << "\n";
	return out;
}

#endif

}
