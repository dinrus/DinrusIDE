#include "RichText.h"

namespace РНЦП {

extern Цвет (*QTFColor[])();
extern int   QTFFontHeight[];

void SeparateNumber(Ткст& s)
{
	if(*s.последний() >= '0' && *s.последний() <= '9')
		s.конкат(';');
}

Ткст QtfFormat(Цвет c)
{
	if(пусто_ли(c)) return "N";
	for(int i = 0; i < 10; i++)
		if((*QTFColor[i])() == c)
			return Ткст(i + '0', 1);
	if(c.дайК() == c.дайЗ() && c.дайЗ() == c.дайС())
		return спринтф("(%d)", c.дайК());
	return спринтф("(%d.%d.%d)", c.дайК(), c.дайЗ(), c.дайС());
}

void LngFmt(Ткст& fmt, dword l, dword lang)
{
	if(lang != (dword)l) {
		if(l == 0)
			fmt << "%-";
		else
		if(l == LNG_ENGLISH)
			fmt << "%%";
		else
			fmt << "%" << LNGAsText(l);
	}
}

void CharFmt(Ткст& fmt, const RichPara::CharFormat& a, const RichPara::CharFormat& b)
{
	if(a.IsBold() != b.IsBold()) fmt.конкат('*');
	if(a.IsItalic() != b.IsItalic()) fmt.конкат('/');
	if(a.IsUnderline() != b.IsUnderline()) fmt.конкат('_');
	if(a.IsStrikeout() != b.IsStrikeout()) fmt.конкат('-');
	if(a.IsNonAntiAliased() != b.IsNonAntiAliased()) fmt.конкат('t');
	if(a.capitals != b.capitals) fmt.конкат('c');
	if(a.dashed != b.dashed) fmt.конкат('d');
	if(a.sscript != b.sscript)
		fmt.конкат(b.sscript == 0 ? a.sscript == 1 ? '`' : ',' :
		        b.sscript == 1 ? '`' : ',');
	if(a.GetFace() != b.GetFace())
		switch(b.GetFace()) {
		case Шрифт::ARIAL:     fmt.конкат('A'); break;
		case Шрифт::ROMAN:     fmt.конкат('R'); break;
		case Шрифт::COURIER:   fmt.конкат('C'); break;
		case Шрифт::STDFONT:   fmt.конкат('G'); break;
	#ifdef PLATFORM_WIN32
		case Шрифт::SYMBOL:    fmt.конкат('S'); break;
	#endif
		default:
			fmt << "!" << b.GetFaceName() << "!";
		}
	if(a.link != b.link)
		fmt << '^' << DeQtf(b.link) << '^';
	if(a.indexentry != b.indexentry)
		fmt << 'I' << DeQtf(вУтф8(b.indexentry)) << ';';
	if(a.ink != b.ink)
		fmt << "@" << QtfFormat(b.ink);
	if(a.paper != b.paper)
		fmt << "$" << QtfFormat(b.paper);
	if(a.дайВысоту() != b.дайВысоту()) {
		for(int i = 0; i < 10; i++)
			if(b.дайВысоту() == QTFFontHeight[i]) {
				SeparateNumber(fmt);
				fmt.конкат('0' + i);
				return;
			}
		fmt.конкат(фмт("+%d", b.дайВысоту()));
	}
}

void FmtNumber(Ткст& qtf, char c, int former, int current)
{
	qtf << (former != current ? фмт("%c%d;", c, current) : Ткст());
}

void QTFEncodeParaFormat(Ткст& qtf, const RichPara::фмт& формат, const RichPara::фмт& style)
{
	if(формат.align != style.align)
		switch(формат.align) {
		case ALIGN_LEFT:    qtf << '<'; break;
		case ALIGN_RIGHT:   qtf << '>'; break;
		case ALIGN_CENTER:  qtf << '='; break;
		case ALIGN_JUSTIFY: qtf << '#'; break;
		}
	FmtNumber(qtf, 'l', style.lm, формат.lm);
	FmtNumber(qtf, 'r', style.rm, формат.rm);
	FmtNumber(qtf, 'i', style.indent, формат.indent);
	FmtNumber(qtf, 'H', style.ruler, формат.ruler);
	if(style.rulerink != формат.rulerink)
		qtf << "h" << QtfFormat(формат.rulerink);
	FmtNumber(qtf, 'L', style.rulerstyle, формат.rulerstyle);
	FmtNumber(qtf, 'b', style.before, формат.before);
	FmtNumber(qtf, 'a', style.after, формат.after);
	if(style.newpage != формат.newpage)
		qtf << 'P';
	if(style.firstonpage != формат.firstonpage)
		qtf << 'F';
	if(style.keep != формат.keep)
		qtf << 'k';
	if(style.keepnext != формат.keepnext)
		qtf << 'K';
	if(style.orphan != формат.orphan)
		qtf << 'Q';
	if(style.linespacing != формат.linespacing)
		switch(формат.linespacing) {
		case RichPara::LSP115: qtf << "pw"; break;
		case RichPara::LSP15: qtf << "ph"; break;
		case RichPara::LSP20: qtf << "pd"; break;
		default:              qtf << "po"; break;
		}
	if(style.bullet != формат.bullet) {
		qtf << 'O';
		switch(формат.bullet) {
		case RichPara::BULLET_NONE:        qtf << '_'; break;
		case RichPara::BULLET_ROUNDWHITE:  qtf << '1'; break;
		case RichPara::BULLET_BOX:         qtf << '2'; break;
		case RichPara::BULLET_BOXWHITE:    qtf << '3'; break;
		case RichPara::BULLET_TEXT:        qtf << '9'; break;
		default:                           qtf << '0'; break;
		}
		qtf << ';';
	}
	if(!пустой(формат.label))
		qtf << ':' << DeQtf(формат.label) << ':';
	if(style.newhdrftr != формат.newhdrftr)
		qtf << "tP" << формат.header_qtf << "^^" << формат.footer_qtf << "^^";

	if(NumberingDiffers(style, формат)) {
		if(формат.before_number != style.before_number) {
			qtf << "n";
			qtf << DeQtf(формат.before_number);
			qtf << ';';
		}
		if(формат.after_number != style.after_number) {
			qtf << "m";
			qtf << DeQtf(формат.after_number);
			qtf << ";";
		}
		int l;
		for(l = 7; l >= 0; --l)
			if(формат.number[l])
				break;
		qtf << "N";
		for(int i = 0; i <= l; i++) {
			static char h[] = { '-', '1', '0', 'a', 'A', 'i', 'I' };
			if(формат.number[i] <= RichPara::NUMBER_I)
				qtf << h[формат.number[i]];
			else
				qtf << '-';
			}
		if(формат.reset_number)
			qtf << '!';
		else
			qtf << ';';
	}
	FmtNumber(qtf, 't', style.tabsize, формат.tabsize);
	if(style.tab != формат.tab) {
		qtf << "~~";
		int i;
		for(i = 0; i < формат.tab.дайСчёт(); i++) {
			RichPara::Вкладка tab = формат.tab[i];
			qtf << '~';
			if(tab.align == ALIGN_RIGHT)
				qtf << '>';
			if(tab.align == ALIGN_CENTER)
				qtf << '=';
			if(tab.fillchar == 1)
				qtf << '.';
			if(tab.fillchar == 2)
				qtf << '-';
			if(tab.fillchar == 3)
				qtf << '_';
			qtf << tab.pos;
		}
		if(i)
			qtf << ';';
	}
}

bool s_nodeqtf[128];

void QTFEncodePara(Ткст& qtf, const RichPara& p, const RichPara::фмт& style, byte charset, dword lang, bool crlf)
{
	int d = qtf.дайДлину();
	QTFEncodeParaFormat(qtf, p.формат, style);
	if(p.part.дайСчёт()) {
		dword l = p.part.верх().формат.language;
		LngFmt(qtf, l, lang);
		lang = l;
	}
	else {
		CharFmt(qtf, style, p.формат);
		LngFmt(qtf, p.формат.language, lang);
	}
	qtf.конкат(' ');
	d = qtf.дайДлину() - d;
	for(int i = 0; i < p.part.дайСчёт(); i++) {
		const RichPara::Part& part = p.part[i];
		Ткст cf;
		LngFmt(cf, part.формат.language, lang);
		CharFmt(cf, style, part.формат);
		if(!cf.пустой()) {
			qtf << '[' << cf << ' ';
			d += cf.дайДлину();
		}
		if(part.field) {
			if(crlf)
				qtf << "\r\n";
			qtf << "{:" + DeQtf(part.field.вТкст()) + ":" + DeQtf(part.fieldparam) + ":}";
			if(crlf)
				qtf << "\r\n";
			d = 0;
		}
		else
		if(part.object) {
			const RichObject& object = part.object;
			Размер sz = object.дайРазм();
			if(crlf)
				qtf << "\r\n";
			qtf << "@@" << object.GetTypeName() << ':' << sz.cx
			    << (object.IsKeepRatio() ? '&' : '*') << sz.cy;
			if(object.GetYDelta())
				qtf << '/' << object.GetYDelta();
			Ткст data = object.пиши();
			const char *q = data.старт();
			const char *slim = data.стоп();
			qtf.резервируй(8 * data.дайДлину() / 7);
			if(object.текст_ли()) {
				qtf << "`";
				while(q < slim) {
					ПРОВЕРЬ((byte)*q >= 32);
					if(*q == '`')
						data.конкат('`');
					qtf.конкат(*q++);
				}
				qtf << "`";
			}
			else {
				if(crlf)
					qtf << "\r\n";
#if 1
				qtf << '(' << Base64Encode(q, slim) << ')';
#else
				while(q < slim - 7) {
					byte data[8];
					data[0] = ((q[0] & 0x80) >> 7) |
					          ((q[1] & 0x80) >> 6) |
					          ((q[2] & 0x80) >> 5) |
					          ((q[3] & 0x80) >> 4) |
					          ((q[4] & 0x80) >> 3) |
					          ((q[5] & 0x80) >> 2) |
					          ((q[6] & 0x80) >> 1) |
					          0x80;
					data[1] = q[0] | 0x80;
					data[2] = q[1] | 0x80;
					data[3] = q[2] | 0x80;
					data[4] = q[3] | 0x80;
					data[5] = q[4] | 0x80;
					data[6] = q[5] | 0x80;
					data[7] = q[6] | 0x80;
					qtf.конкат(data, 8);
					if(crlf && ++n % 10 == 0)
						qtf << "\r\n";
					q += 7;
				}
				while(q < slim) {
					byte seven = 0;
					const char *lim = slim;
					const char *s;
					for(s = q; s < lim; s++)
						seven = (seven >> 1) | (*s & 0x80);
					seven >>= 8 - (lim - q);
					qtf.конкат(seven | 0x80);
					for(s = q; s < lim; s++)
						qtf.конкат(*s | 0x80);
					if(crlf && ++n % 10 == 0)
						qtf << "\r\n";
					q += 7;
				}
#endif
			}
			if(crlf)
				qtf << "\r\n";
			d = 0;
		}
		else {
			for(const wchar *s = part.text.старт(); s != part.text.стоп(); s++) {
				int c = *s;
				if(c < 128) {
					if(s_nodeqtf[c]) {
						qtf.конкат(c);
						d++;
					}
					else
					if(c == 9) {
						qtf.конкат("-|");
						d++;
					}
					else
					if(c == ':' && s[1] != ':')
						qtf.конкат(':');
					else {
						qtf.конкат('`');
						qtf.конкат(c);
						d += 2;
					}
					if(crlf && d > 60 && c == ' ') {
						qtf.конкат("\r\n");
						d = 0;
					}
				}
				else {
					if(c == 160) {
						qtf.конкат("_");
						d++;
					}
					else
					if(charset == CHARSET_UTF8) {
						Ткст q = вУтф8(c);
						d += q.дайДлину();
						qtf << q;
					}
					else {
						int ch = изЮникода(c, charset, 0);
						if(ch)
							qtf << (char)ch;
						else
							qtf << "@$" << фмт("%04X", ch) << ';';
						d++;
					}
				}
				if(crlf && d > 80) {
					qtf.конкат("\r\n");
					d = 0;
				}
			}
		}
		if(!cf.пустой()) {
			d++;
			qtf << ']';
		}
	}
}

void FmtNumber2(Ткст& qtf, char c, int da, int a, int db, int b)
{
	if(da != a || db != b) {
		qtf << c;
		if(da != a)
			qtf << a;
		qtf << '/';
		qtf << b;
	}
}


void QTFEncodeTxt(Ткст& qtf, const RichTxt& text, const RichStyles& styles, const RichStyle& defstyle,
                  dword options, const Индекс<Uuid>& sm, byte charset, dword lang)
{
	qtf << '[';
	for(int i = 0; i < text.GetPartCount(); i++) {
		if(i) {
			qtf << "&]";
			if(options & QTF_CRLF)
				qtf << "\r\n";
			qtf << '[';
		}
		if(text.IsTable(i)) {
			qtf << ' ';

			const RichTable& t = text.GetTable(i);
			const RichTable::фмт& f = t.дайФормат();
			int nx = f.column.дайСчёт();
			int ny = t.GetRows();
			qtf << "{{";
			for(int i = 0; i < nx; i++) {
				if(i)
					qtf << ':';
				qtf << f.column[i];
			}
			const RichTable::фмт& d = Single<RichTable::фмт>();
			FmtNumber(qtf, '<', d.lm, f.lm);
			FmtNumber(qtf, '>', d.rm, f.rm);
			FmtNumber(qtf, 'B', d.before, f.before);
			FmtNumber(qtf, 'A', d.after, f.after);
			FmtNumber(qtf, 'f', d.frame, f.frame);
			if(f.keep)
				qtf << 'K';
			if(f.newpage)
				qtf << 'P';
			if(f.newhdrftr)
				qtf << 'T' << f.header_qtf << "^^" << f.footer_qtf << "^^";
			if(f.framecolor != d.framecolor)
				qtf << 'F' << QtfFormat(f.framecolor);
			FmtNumber(qtf, 'g', d.grid, f.grid);
			if(f.gridcolor != d.gridcolor)
				qtf << 'G' << QtfFormat(f.gridcolor);
			FmtNumber(qtf, 'h', d.header, f.header);
			RichCell::фмт cf = Single<RichCell::фмт>();
			for(int i = 0; i < ny; i++) {
				const Массив<RichCell>& r = t[i];
				for(int j = 0; j < r.дайСчёт(); j++) {
					const RichCell& c = r[j];
					if(i || j) {
						if(options & QTF_CRLF)
							qtf << "\r\n";
						qtf << "::";
					}
					const RichCell::фмт& f = c.формат;
					if(f.align != cf.align)
						switch(f.align) {
						case ALIGN_TOP:    qtf << '^'; break;
						case ALIGN_CENTER: qtf << '='; break;
						case ALIGN_BOTTOM: qtf << 'v'; break;
						}
					FmtNumber2(qtf, 'l', cf.border.left, f.border.left, cf.margin.left, f.margin.left);
					FmtNumber2(qtf, 'r', cf.border.right, f.border.right, cf.margin.right, f.margin.right);
					FmtNumber2(qtf, 't', cf.border.top, f.border.top, cf.margin.top, f.margin.top);
					FmtNumber2(qtf, 'b', cf.border.bottom, f.border.bottom, cf.margin.bottom, f.margin.bottom);
					FmtNumber(qtf, 'H', cf.minheight, f.minheight);
					if(f.color != cf.color)
						qtf << '@' << QtfFormat(f.color);
					if(f.bordercolor != cf.bordercolor)
						qtf << 'R' << QtfFormat(f.bordercolor);
					cf = f;
					if(c.hspan)
						qtf << '-' << c.hspan;
					if(c.vspan)
						qtf << '|' << c.vspan;
					if(f.keep)
						qtf << "k";
					if(f.round)
						qtf << "o";
					qtf << ' ';
					QTFEncodeTxt(qtf, c.text, styles, defstyle, options, sm, charset, lang);
				}
			}
			qtf << "}}";
		}
		else {
			RichPara p = text.дай(i, styles);
			int si = sm.найди(text.GetParaStyle(i));
			if(!(options & QTF_NOSTYLES))
				qtf << "s" << si;
			SeparateNumber(qtf);
			const RichStyle& s = si < 0 ? defstyle : дайСтиль(styles, sm[si]);
			QTFEncodePara(qtf, p, options & QTF_NOSTYLES ? defstyle.формат : s.формат, charset, lang,
			              (options & QTF_CRLF));
		}
	}
	qtf << ']';
}

void init_s_nodeqtf()
{
	ONCELOCK {
		for(int c = 1; c < 128; c++)
			s_nodeqtf[c] = IsAlNum(c) || strchr(".,;!?%()/<># ", c);
	}
}

Ткст   AsQTF(const RichText& text, byte charset, dword options)
{
	int i;
	Ткст qtf;

	init_s_nodeqtf();

	RichPara::фмт dpf;
	dpf.Face(Шрифт::ARIAL);
	dpf.устВысоту(100);
	dpf.language = LNG_ENGLISH;

	bool crlf = options & QTF_CRLF;

	Индекс<Uuid> sm;
//	if(options & QTF_ALL_STYLES) //!!!! problem -> GetUsedStyles!!!!
		for(i = 0; i < text.GetStyleCount(); i++)
			sm.найдиДобавь(text.GetStyleId(i));
//	else
//		for(i = 0; i < text.GetPartCount(); i++)
//			sm.найдиДобавь(text.GetParaStyle(i));

	Ткст hdr = text.GetHeaderQtf();
	if(hdr.дайСчёт())
		qtf << "^H" << hdr << "^^\r\n";

	Ткст ftr = text.GetFooterQtf();
	if(ftr.дайСчёт())
		qtf << "^F" << ftr << "^^\r\n";

	if(!(options & QTF_NOSTYLES))
		for(int j = 1; j < sm.дайСчёт() + 1; j++) { // we need to store default style last as it immediately sets
			int i = j % sm.дайСчёт();
			Uuid id = sm[i];
			const RichStyle& s = text.дайСтиль(id);
			qtf << '[';
			QTFEncodeParaFormat(qtf, s.формат, dpf);
			CharFmt(qtf, dpf, s.формат);
			qtf << ' ';
			qtf << "$$" << i << ',' << max(sm.найди(s.next), 0)
			    << '#' << фмт(id)
			    << ':' << DeQtf(s.имя) << "]";
			if(crlf)
			    qtf << "\r\n";
		}

	if(text.GetPartCount() && (options & QTF_BODY)) {
		ВекторМап<dword, int> lngc;
		int m = min(text.GetPartCount(), 30);
		for(int i = 0; i < m; i++)
			if(text.IsPara(i)) {
				RichPara p = text.дай(i);
				lngc.дайДобавь(p.формат.language, 0)++;
			}
		dword lang = lngc.дайСчёт() ? lngc.дайКлюч(найдиМакс(lngc.дайЗначения())) : 0;
		qtf << "[";
		if(!(options & QTF_NOCHARSET)) {
			qtf << "{";
			if(charset == CHARSET_UTF8)
				qtf << "_";
			else
			if(charset >= CHARSET_WIN1250 && charset <= CHARSET_WIN1258)
				qtf << (char)('0' + charset - CHARSET_WIN1250);
			else
			if(charset >= CHARSET_ISO8859_1 && charset <= CHARSET_ISO8859_16)
				qtf << (char)('A' + charset - CHARSET_ISO8859_1);
			else
				qtf << имяНабСим(charset);
			qtf << "}";
		}
		if(lang && !(options & QTF_NOLANG))
			qtf << "%" << LNGAsText(SetLNGCharset(lang, CHARSET_DEFAULT));
		qtf << " ";
		if(crlf)
			qtf << "\r\n";
		RichStyle defstyle;
		defstyle.формат.устВысоту(100);

		QTFEncodeTxt(qtf, text, text.GetStyles(), defstyle, options, sm, charset, lang);

		qtf << "]";
	}
	return qtf;
}

Ткст DeQtf(const char *s) {
	ТкстБуф r;
	for(; *s; s++) {
		if(*s == '\n')
			r.конкат('&');
		else {
			if((byte)*s > ' ' && !цифра_ли(*s) && !IsAlpha(*s) && (byte)*s < 128)
				r.конкат('`');
			r.конкат(*s);
		}
	}
	return Ткст(r);
}

Ткст DeQtfLf(const char *s) {
	ТкстБуф r;
	while(*s) {
		if((byte)*s > ' ' && !цифра_ли(*s) && !IsAlpha(*s) && (byte)*s < 128)
			r.конкат('`');
		if((byte)*s >= ' ')
			r.конкат(*s);
		else
		if(*s == '\n')
			r.конкат('&');
		else
		if(*s == '\t')
			r.конкат("-|");
		s++;
	}
	return Ткст(r);
}

RichText AsRichText(const RichObject& obj)
{
	RichText x;
	RichPara p;
	RichPara::фмт fmt;
	p.конкат(obj, fmt);
	x.конкат(p);
	return x;
}


Ткст AsQTF(const RichObject& obj)
{
	return AsQTF(AsRichText(obj), CHARSET_UTF8, QTF_NOSTYLES|QTF_BODY);
}

}
