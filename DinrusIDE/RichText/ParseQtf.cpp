#include "RichText.h"

namespace РНЦП {

Цвет (*QTFColor[])() = {
	чёрный, светлоСерый, белый, красный, зелёный, синий, светлоКрасный, белоСерый, светлоЦыан, жёлтый
};

Цвет NullColorF()
{
	return Null;
}

static Цвет (*QTFColorl[])() = {
	/*a*/белый, /*b*/синий, /*c*/цыан, /*d*/белый, /*e*/белый, /*f*/белый, /*g*/ зелёный, /*h*/белый,
	/*i*/белый, /*j*/белый, /*k*/чёрный, /*l*/светлоСерый, /*m*/магента, /*n*/NullColorF, /*o*/коричневый, /*p*/белый,
	/*q*/белый, /*r*/красный, /*s*/белый, /*t*/белый, /*u*/белый, /*v*/белый, /*w*/белоСерый, /*x*/белый,
	/*y*/жёлтый, /*z*/ белый
};

static Цвет (*QTFColorL[])() = {
	/*A*/белый, /*B*/светлоСиний, /*C*/светлоЦыан, /*D*/белый, /*E*/белый, /*F*/белый, /*G*/светлоЗелёный, /*H*/белый,
	/*I*/белый, /*J*/белый, /*K*/серый, /*L*/белоСерый, /*M*/светлоМагента, /*N*/NullColorF, /*O*/коричневый, /*P*/белый,
	/*Q*/белый, /*R*/светлоКрасный, /*S*/белый, /*T*/белый, /*U*/белый, /*V*/белый, /*W*/белый, /*X*/белый,
	/*Y*/светлоЖёлтый, /*Z*/белый
};

int QTFFontHeight[] = {
	50, 67, 84, 100, 134, 167, 200, 234, 300, 400
};

class RichQtfParser {
	void       *context;
	const char *term;
	ШТкст     text;
	RichPara    paragraph;
	RichTable   tablepart;
	bool        istable;
	bool        breakpage;
	int         accesskey;

	struct PFormat : public RichPara::фмт {
		byte                  charset;
	};

	struct Вкладка {
		RichCell::фмт формат;
		int              vspan, hspan;
		RichTxt          text;
		RichTable        table;
		int              cell;
		Вектор<int>      rown;

		void Old()                    { RichTable::фмт fmt; fmt.grid = 10; table.устФормат(fmt); }

		Вкладка()                         { cell = 0; vspan = hspan = 0; }
	};

	PFormat          формат;
	Массив<PFormat>   fstack;
	Вектор<Uuid>     styleid;
	Вектор<int>      stylenext;
	Массив<Вкладка>       table;
	bool             oldtab;

	bool   Ключ(int c)                 { if(*term == c) { term++; return true; } return false; }
	bool   Key2(int c, int d);
	bool   Key2(int c)                { return Key2(c, c); }
	int    GetNumber();
	int    читайЧисло();
	Ткст дайТекст(int delim);
	Ткст GetText2(int delim1, int delim2);
	Цвет  дайЦвет();
	void   Number2(int& a, int& b);

	void   слей();
	void   устФормат();
	void   FlushStyles();
	void   Ошибка(const char *s);

	void   ReadObject();

	RichTable& Table();
	void       TableFormat(bool bw = false);
	void       FinishCell();
	void       FinishTable();
	void       FinishOldTable();
	void       S(int& x, int a);

	void       EndPart();

	void       конкат(int chr);

public:
	struct Искл {};

	RichText target;

	void     Parse(const char *qtf, int accesskey);

	RichQtfParser(void *context);
};

void init_s_nodeqtf();

RichQtfParser::RichQtfParser(void *context_)
: context(context_)
{
	формат.Face(Шрифт::ARIAL);
	формат.устВысоту(100);
	формат.charset = дайДефНабСим();
	формат.language = 0;
	breakpage = false;
	istable = false;
	oldtab = false;
	init_s_nodeqtf();
}

bool RichQtfParser::Key2(int c, int d)
{
	if(term[0] == c && term[1] == d) {
		term += 2;
		return true;
	}
	return false;
}

int  RichQtfParser::GetNumber()
{
	int n = 0;
	int зн = 1;
	if(*term == '-') {
		зн = -1;
		term++;
	}
	while(цифра_ли(*term))
		n = n * 10 + *term++ - '0';
	return зн * n;
}

Ткст RichQtfParser::дайТекст(int delim) {
	Ткст s;
	for(;;) {
		if(*term == '\0') return s;
		if(*term == '`') {
			term++;
			if(*term == '\0') return s;
			s.конкат(*term++);
		}
		else
		if(*term == delim) {
			term++;
			return s;
		}
		else
			s.конкат(*term++);
	}
}

Ткст RichQtfParser::GetText2(int delim1, int delim2) {
	Ткст s;
	for(;;) {
		if(*term == '\0') return s;
		if(*term == '`') {
			term++;
			if(*term == '\0') return s;
			s.конкат(*term++);
		}
		else
		if(term[0] == delim1 && term[1] == delim2) {
			term += 2;
			return s;
		}
		else
			s.конкат(*term++);
	}
}

int RichQtfParser::читайЧисло()
{
	if(!цифра_ли(*term))
		Ошибка("Expected number");
	return GetNumber();
}

void RichQtfParser::Number2(int& a, int& b)
{
	a = -1;
	b = -1;
	if(цифра_ли(*term))
		a = GetNumber();
	if(*term == '/') {
		term++;
		b = GetNumber();
	}
}

Цвет RichQtfParser::дайЦвет()
{
	int c = *term++;
	if(c == '(') {
		byte r = GetNumber();
		if(Ключ(')')) {
			r &= 255;
			return Цвет(r, r, r);
		}
		Ключ('.');
		byte g = GetNumber();
		Ключ('.');
		byte b = GetNumber();
		Ключ(')');
		return Цвет(r & 255, g & 255, b & 255);
	}
	else
	if(c >= '0' && c <= '9')
		return QTFColor[c - '0']();
	else
	if(c >= 'a' && c <= 'z')
		return QTFColorl[c - 'a']();
	else
	if(c >= 'A' && c <= 'Z')
		return QTFColorL[c - 'A']();
	else
		return красный;
}

void RichQtfParser::устФормат()
{
	paragraph.формат = формат;
}

void RichQtfParser::слей() {
	if(text.дайДлину()) {
		ПРОВЕРЬ(!istable);
		paragraph.конкат(text, формат);
		text.очисть();
	}
}

void RichQtfParser::EndPart()
{
	if(istable) {
		if(paragraph.дайСчёт() == 0 && text.дайСчёт() == 0)
			if(table.дайСчёт())
				table.верх().text.CatPick(pick(tablepart));
			else
				target.CatPick(pick(tablepart));
		else {
			paragraph.part.очисть();
			text.очисть();
		}
	}
	else {
		слей();
		if(table.дайСчёт())
			table.верх().text.конкат(paragraph, target.GetStyles());
		else {
			if(breakpage)
				paragraph.формат.newpage = true;
			target.конкат(paragraph);
		}
		paragraph.part.очисть();
		устФормат();
		breakpage = false;
	}
	istable = false;
}

void RichQtfParser::ReadObject()
{
	слей();
	RichObject obj;
	if(*term == '#') {
		term++;
	#ifdef CPU_64
		obj = *(RichObject *)stou64(term, &term);
	#else
		obj = *(RichObject *)stou(term, &term);
	#endif
		term++;
	}
	else {
		Ткст тип;
		while(IsAlNum(*term) || *term == '_')
			тип.конкат(*term++);
		Размер sz;
		Ключ(':');
		sz.cx = читайЧисло();
		bool keepratio = false;
		if(Ключ('&'))
			keepratio = true;
		else
			Ключ('*');
		sz.cy = читайЧисло();
		int yd = 0;
		if(Ключ('/'))
			yd = GetNumber();
		while(*term && (byte)*term < 32)
			term++;
		Ткст odata;
		if(Ключ('`'))
			while(*term) {
				if(*term == '`') {
					term++;
					if(*term == '`')
						odata.конкат('`');
					else
						break;
				}
				else
				if((byte)*term >= 32)
					odata.конкат(*term);
				term++;
			}
		else
		if(Ключ('(')) {
			const char *b = term;
			while(*term && *term != ')')
				term++;
			odata = Base64Decode(b, term);
			if(*term == ')')
				term++;
		}
		else {
			ТкстБуф data;
			for(;;) {
				while(*term < 32 && *term > 0) term++;
				if((byte)*term >= ' ' && (byte)*term <= 127 || *term == '\0') break;
				byte seven = *term++;
				for(int i = 0; i < 7; i++) {
					while((byte)*term < 32 && (byte)*term > 0) term++;
					if((byte)*term >= ' ' && (byte)*term <= 127 || *term == '\0') break;
					data.конкат((*term++ & 0x7f) | ((seven << 7) & 0x80));
					seven >>= 1;
				}
			}
			odata = data;
		}
		obj.читай(тип, odata, sz, context);
		obj.KeepRatio(keepratio);
		obj.SetYDelta(yd);
	}
	paragraph.конкат(obj, формат);
}

int NoLow(int c) {
	return (byte)c >= 32 ? c : 0;
}

void RichQtfParser::Ошибка(const char *s) {
	RichPara::CharFormat ef;
	(Шрифт&) ef = Arial(84).Bold().Underline();
	ef.ink = красный;
	ШТкст e;
	e << "Ошибка: " << s;
	if(*term)
		e << ": " << фильтруй(Ткст(term, min<int>((int)strlen(term), 20)), NoLow).вШТкст();
	else
		e << ".";
	paragraph.конкат(e, ef);
	target.конкат(paragraph);
	FlushStyles();
	throw Искл();
}

void RichQtfParser::FlushStyles()
{
	for(int i = 0; i < styleid.дайСчёт(); i++)
		if(stylenext[i] >= 0 && stylenext[i] < styleid.дайСчёт()) {
			RichStyle s = target.дайСтиль(styleid[i]);
			s.next = styleid[stylenext[i]];
			target.устСтиль(styleid[i], s);
		}
}

RichTable& RichQtfParser::Table()
{
	if(table.дайСчёт() == 0)
		Ошибка("Not in table");
	return table.верх().table;
}

void RichQtfParser::S(int& x, int a)
{
	if(a >= 0)
		x = a;
}

void RichQtfParser::TableFormat(bool bw)
{
	RichTable& tab = Table();
	RichTable::фмт tabformat = tab.дайФормат();
	Вкладка& t = table.верх();
	int a, b;
	for(;;) {
		if(bw && цифра_ли(*term)) {
			t.hspan = GetNumber();
		}
		else
		if(*term == '\0')
			Ошибка("Unexpected end of text in cell формат");
		else
		switch(*term++) {
		case ' ':
			tab.устФормат(tabformat);
			return;
		case ';': break;
		case '<': tabformat.lm = читайЧисло(); break;
		case '>': tabformat.rm = читайЧисло(); break;
		case 'B': tabformat.before = читайЧисло(); break;
		case 'A': tabformat.after = читайЧисло(); break;
		case 'f': tabformat.frame = читайЧисло(); break;
		case '_':
		case 'F': tabformat.framecolor = дайЦвет(); break;
		case 'g': tabformat.grid = читайЧисло(); break;
		case 'G': tabformat.gridcolor = дайЦвет(); break;
		case 'h': tabformat.header = GetNumber(); break;
		case '~': tabformat.frame = tabformat.grid = 0; break;
		case '^': t.формат.align = ALIGN_TOP; break;
		case '=': t.формат.align = ALIGN_CENTER; break;
		case 'v': t.формат.align = ALIGN_BOTTOM; break;
		case 'l': Number2(a, b); S(t.формат.border.left, a); S(t.формат.margin.left, b); break;
		case 'r': Number2(a, b); S(t.формат.border.right, a); S(t.формат.margin.right, b); break;
		case 't': Number2(a, b); S(t.формат.border.top, a); S(t.формат.margin.top, b); break;
		case 'b': Number2(a, b); S(t.формат.border.bottom, a); S(t.формат.margin.bottom, b); break;
		case 'H': t.формат.minheight = читайЧисло(); break;
		case '@': t.формат.color = дайЦвет(); break;
		case 'R': t.формат.bordercolor = дайЦвет(); break;
		case '!': t.формат = RichCell::фмт(); break;
		case 'o': t.формат.round = true; break;
		case 'k': t.формат.keep = true; break;
		case 'K': tabformat.keep = true; break;
		case 'P': tabformat.newpage = true; break;
		case 'T':
			tabformat.newhdrftr = true;
			tabformat.header_qtf = GetText2('^', '^');
			tabformat.footer_qtf = GetText2('^', '^');
			break;
		case 'a':
			Number2(a, b);
			if(a >= 0)
				t.формат.border.left = t.формат.border.right = t.формат.border.top = t.формат.border.bottom = a;
			if(b >= 0)
				t.формат.margin.left = t.формат.margin.right = t.формат.margin.top = t.формат.margin.bottom = b;
			break; //!!cell all lines
		case '*':
			tabformat.frame = tabformat.grid =
			t.формат.border.left = t.формат.border.right = t.формат.border.top = t.формат.border.bottom =
			t.формат.margin.left = t.формат.margin.right = t.формат.margin.top = t.формат.margin.bottom = 0;
			break;
		case '-': t.hspan = GetNumber(); break;
		case '+':
		case '|': t.vspan = GetNumber(); break;
		default:
			Ошибка("Invalid cell формат");
		}
	}
}

void RichQtfParser::FinishCell()
{
	EndPart();
	RichTable& t = Table();
	Вкладка& b = table.верх();
	int i, j;
	if(oldtab) {
		i = b.rown.дайСчёт() - 1;
		j = b.rown.верх();
		b.rown.верх()++;
	}
	else {
		i = b.cell / t.дайКолонки();
		j = b.cell % t.дайКолонки();
	}
	t.SetPick(i, j, pick(b.text));
	b.text.очисть();
	t.устФормат(i, j, b.формат);
	t.SetSpan(i, j, b.vspan, b.hspan);
	if(oldtab && b.rown.дайСчёт() > 1 && j + 1 < b.rown[0])
		b.формат = t.дайФормат(0, j + 1);
	else {
		b.cell++;
		b.vspan = 0;
		b.hspan = oldtab;
	}
	b.формат.keep = false;
	b.формат.round = false;
}

void RichQtfParser::FinishTable()
{
	FinishCell();
	while(table.верх().cell % Table().дайКолонки())
		FinishCell();
	tablepart = pick(Table());
	istable = true;
	table.сбрось();
}

void RichQtfParser::FinishOldTable()
{
	FinishCell();
	Индекс<int>  pos;
	Вектор<int> srow;
	RichTable& t = Table();
	Вкладка& b = table.верх();
	for(int i = 0; i < t.GetRows(); i++) {
		int& s = srow.добавь();
		s = 0;
		int nx = b.rown[i];
		for(int j = 0; j < nx; j++)
			s += t.GetSpan(i, j).cx;
		int xn = 0;
		for(int j = 0; j < nx; j++) {
			pos.найдиДобавь(xn * 10000 / s);
			xn += t.GetSpan(i, j).cx;
		}
	}
	Вектор<int> h = pos.подбериКлючи();
	if(h.дайСчёт() == 0)
		Ошибка("table");
	сортируй(h);
	pos = pick(h);
	pos.добавь(10000);
	RichTable tab;
	tab.устФормат(t.дайФормат());
	for(int i = 0; i < pos.дайСчёт() - 1; i++) {
		tab.добавьКолонку(pos[i + 1] - pos[i]);
	}
	for(int i = 0; i < t.GetRows(); i++) {
		int s = srow[i];
		int nx = b.rown[i];
		int xn = 0;
		int xi = 0;
		for(int j = 0; j < nx; j++) {
			Размер span = t.GetSpan(i, j);
			xn += span.cx;
			int nxi = pos.найди(xn * 10000 / s);
			tab.SetPick(i, xi, t.GetPick(i, j));
			tab.устФормат(i, xi, t.дайФормат(i, j));
			tab.SetSpan(i, xi, max(span.cy - 1, 0), nxi - xi - 1);
			xi = nxi;
		}
	}
	table.сбрось();
	if(table.дайСчёт())
		table.верх().text.CatPick(pick(tab));
	else
		target.CatPick(pick(tab));
	oldtab = false;
}

void RichQtfParser::конкат(int chr)
{
	if(accesskey && взаг(вАски(chr)) == LOBYTE(accesskey)) {
		слей();
		формат.Underline(!формат.IsUnderline());
		text.конкат(chr);
		слей();
		формат.Underline(!формат.IsUnderline());
		accesskey = 0;
	}
	else if(chr >= ' ') {
		text.конкат(chr);
	}
}

extern bool s_nodeqtf[128];

int GetRichTextScreenStdFontHeight()
{
	static int gh = 67;
	ONCELOCK {
		for(int i = 0; i < 1000; i++) {
			int h = GetRichTextStdScreenZoom() * i;
			if(h > 0 && StdFont(h).GetCy() == StdFont().GetCy()) {
				gh = i;
				break;
			}
		}
	}
	return gh;
}

void RichQtfParser::Parse(const char *qtf, int _accesskey)
{
	accesskey = _accesskey;
	term = qtf;
	while(*term) {
		if(Ключ('[')) {
			слей();
			fstack.добавь(формат);
			for(;;) {
				int c = *term;
				if(!c)
					Ошибка("Unexpected end of text");
				term++;
				if(c == ' ' || c == '\n') break;
				switch(c) {
				case 's': {
					Uuid id;
					c = *term;
					if(Ключ('\"') || Ключ('\''))
						id = target.GetStyleId(дайТекст(c));
					else {
						int i = читайЧисло();
						if(i >= 0 && i < styleid.дайСчёт())
							id = styleid[i];
						else
							id = RichStyle::GetDefaultId();
					}
					const RichStyle& s = target.дайСтиль(id);
					bool p = формат.newpage;
					int lng = формат.language;
					(RichPara::фмт&) формат = s.формат;
					формат.styleid = id;
					формат.language = lng;
					формат.newpage = p;
					break;
				}
				case '/': формат.Italic(!формат.IsItalic()); break;
				case '*': формат.Bold(!формат.IsBold()); break;
				case '_': формат.Underline(!формат.IsUnderline()); break;
				case 'T': формат.NonAntiAliased(!формат.IsNonAntiAliased()); break;
				case '-': формат.Strikeout(!формат.IsStrikeout()); break;
				case 'c': формат.capitals = !формат.capitals; break;
				case 'd': формат.dashed = !формат.dashed; break;
				case '`': формат.sscript = формат.sscript == 1 ? 0 : 1; break;
				case ',': формат.sscript = формат.sscript == 2 ? 0 : 2; break;
				case '^': формат.link = дайТекст('^'); break;
				case 'I': формат.indexentry = вУтф32(дайТекст(';')); break;
				case '+': формат.устВысоту(GetNumber()); break;
				case '@': формат.ink = дайЦвет(); break;
				case '$': формат.paper = дайЦвет(); break;
				case 'A': формат.Face(Шрифт::ARIAL); break;
				case 'R': формат.Face(Шрифт::ROMAN); break;
				case 'C': формат.Face(Шрифт::COURIER); break;
				case 'G': формат.Face(Шрифт::STDFONT); break;
				case 'S':
#ifdef PLATFORM_WIN32
					формат.Face(Шрифт::SYMBOL);
#endif
					break;
				case '.': {
					int n = GetNumber();
					if(n >= Шрифт::GetFaceCount())
						Ошибка("Invalid face number");
					формат.Face(n); break;
				}
				case '!': {
						Ткст фн = дайТекст('!');
						int i = Шрифт::FindFaceNameIndex(фн);
						if(i < 0)
							i = Шрифт::ARIAL;
						формат.Face(i);
					}
					break;
				case '{': {
						Ткст cs = дайТекст('}');
						if(cs.дайДлину() == 1) {
							int c = *cs;
							if(c == '_')
								формат.charset = CHARSET_UTF8;
							if(c >= '0' && c <= '8')
								формат.charset = c - '0' + CHARSET_WIN1250;
							if(c >= 'A' && c <= 'Z')
								формат.charset = c - '0' + CHARSET_ISO8859_1;
						}
						else {
							for(int i = 0; i < счётНабСим(); i++)
								if(stricmp(имяНабСим(i), cs) == 0) {
									формат.charset = i;
									break;
								}
						}
						break;
					}
				case '%': {
						Ткст h;
						if(*term == '-') {
							формат.language = 0;
							term++;
						}
						else
						if(*term == '%') {
							формат.language = LNG_ENGLISH;
							term++;
						}
						else {
							while(*term && h.дайДлину() < 5)
								h.конкат(*term++);
							формат.language = LNGFromText(h);
						}
						break;
					}
				case 'g':
					формат.Face(Шрифт::STDFONT);
					формат.устВысоту(GetRichTextScreenStdFontHeight());
					break;
				default:
					if(c >= '0' && c <= '9') {
						формат.устВысоту(QTFFontHeight[c - '0']);
						break;
					}
					switch(c) {
					case ':': формат.label = дайТекст(':'); break;
					case '<': формат.align = ALIGN_LEFT; break;
					case '>': формат.align = ALIGN_RIGHT; break;
					case '=': формат.align = ALIGN_CENTER; break;
					case '#': формат.align = ALIGN_JUSTIFY; break;
					case 'l': формат.lm = GetNumber(); break;
					case 'r': формат.rm = GetNumber(); break;
					case 'i': формат.indent = GetNumber(); break;
					case 'b': формат.before = GetNumber(); break;
					case 'a': формат.after = GetNumber(); break;
					case 'P': формат.newpage = !формат.newpage; break;
					case 'F': формат.firstonpage = !формат.firstonpage; break;
					case 'k': формат.keep = !формат.keep; break;
					case 'K': формат.keepnext = !формат.keepnext; break;
					case 'H': формат.ruler = GetNumber(); break;
					case 'h': формат.rulerink = дайЦвет(); break;
					case 'L': формат.rulerstyle = GetNumber(); break;
					case 'Q': формат.orphan = !формат.orphan; break;
					case 'n': формат.before_number = дайТекст(';'); break;
					case 'm': формат.after_number = дайТекст(';'); break;
					case 'N': {
						memset8(формат.number, 0, sizeof(формат.number));
						формат.reset_number = false;
						int i = 0;
						while(i < 8) {
							int c;
							if(Ключ('-'))
								c = RichPara::NUMBER_NONE;
							else
							if(Ключ('1'))
								c = RichPara::NUMBER_1;
							else
							if(Ключ('0'))
								c = RichPara::NUMBER_0;
							else
							if(Ключ('a'))
								c = RichPara::NUMBER_a;
							else
							if(Ключ('A'))
								c = RichPara::NUMBER_A;
							else
							if(Ключ('i'))
								c = RichPara::NUMBER_i;
							else
							if(Ключ('I'))
								c = RichPara::NUMBER_I;
							else
								break;
							формат.number[i++] = c;
						}
						if(Ключ('!'))
							формат.reset_number = true;
						break;
					}
					case 'o': формат.bullet = RichPara::BULLET_ROUND;
					          формат.indent = 150; break;
					case 'O':
						if(Ключ('_'))
							формат.bullet = RichPara::BULLET_NONE;
						else {
							int c = *term++;
							if(!c)
								Ошибка("Unexpected end of text");
							формат.bullet =
							                c == '1' ? RichPara::BULLET_ROUNDWHITE :
							                c == '2' ? RichPara::BULLET_BOX :
							                c == '3' ? RichPara::BULLET_BOXWHITE :
							                c == '9' ? RichPara::BULLET_TEXT :
							                           RichPara::BULLET_ROUND;
						}
						break;
					case 'p':
						switch(*term++) {
						case 0:   Ошибка("Unexpected end of text");
						case 'h': формат.linespacing = RichPara::LSP15; break;
						case 'd': формат.linespacing = RichPara::LSP20; break;
						case 'w': формат.linespacing = RichPara::LSP115; break;
						default:  формат.linespacing = RichPara::LSP10;
						}
						break;
					case 't':
						if(*term == 'P') {
							term++;
							формат.newhdrftr = true;
							формат.header_qtf = GetText2('^', '^');
							формат.footer_qtf = GetText2('^', '^');
						}
						else
						if(цифра_ли(*term))
							формат.tabsize = читайЧисло();
						break;
					case '~': {
							if(Ключ('~'))
								формат.tab.очисть();
							else {
								RichPara::Вкладка tab;
								Ключ('<');
								if(Ключ('>'))
									tab.align = ALIGN_RIGHT;
								if(Ключ('='))
									tab.align = ALIGN_CENTER;
								if(Ключ('.'))
									tab.fillchar = 1;
								if(Ключ('-'))
									tab.fillchar = 2;
								if(Ключ('_'))
									tab.fillchar = 3;
								int rightpos = Ключ('>') ? RichPara::TAB_RIGHTPOS : 0;
								tab.pos = rightpos | читайЧисло();
								формат.tab.добавь(tab);
							}
						}
						break;
					default:
						continue;
					}
				}
			}
			устФормат();
		}
		else
		if(Ключ(']')) {
			слей();
			if(fstack.дайСчёт()) {
				формат = fstack.верх();
				fstack.сбрось();
			}
			else
				Ошибка("Unmatched ']'");
		}
		else
		if(Key2('{')) {
			if(oldtab)
				Ошибка("{{ in ++ table");
			if(text.дайДлину() || paragraph.дайСчёт())
				EndPart();
			table.добавь();
			int r = цифра_ли(*term) ? читайЧисло() : 1;
			Table().добавьКолонку(r);
			while(Ключ(':'))
				Table().добавьКолонку(читайЧисло());
			if(breakpage) {
				RichTable& tab = Table();
				RichTable::фмт tabformat = tab.дайФормат();
				tabformat.newpage = true;
				tab.устФормат(tabformat);
				breakpage = false;
			}
			TableFormat();
			устФормат();
		}
		else
		if(Key2('}')) {
			if(oldtab)
				Ошибка("}} in ++ table");
			FinishTable();
		}
		else
		if(Key2('+'))
			if(oldtab)
				FinishOldTable();
			else {
				слей();
				if(text.дайДлину() || paragraph.дайСчёт())
					EndPart();
				Вкладка& b = table.добавь();
				b.rown.добавь(0);
				b.hspan = 1;
				b.Old();
				oldtab = true;
			}
		else
		if(Key2('|'))
			FinishCell();
		else
		if(Key2('-')) {
			FinishCell();
			table.верх().rown.добавь(0);
		}
		else
		if(Key2(':')) {
			if(!oldtab)
				FinishCell();
			TableFormat(oldtab);
		}
		else
		if(Key2('^')) {
			EndPart();
			breakpage = true;
		}
		else
		if(Key2('@')) {
			ReadObject();
		}
		else
		if(Key2('@', '$')) {
			Ткст xu;
			while(isxdigit(*term))
				xu.конкат(*term++);
			int c = stou(~xu, NULL, 16);
			if(c >= 32)
				конкат(c);
			if(*term == ';')
				term++;
			устФормат();
		}
		else
		if(Key2('^', 'H'))
			target.SetHeaderQtf(GetText2('^', '^'));
		else
		if(Key2('^', 'F'))
			target.SetFooterQtf(GetText2('^', '^'));
		else
		if(Key2('{', ':')) {
			слей();
			Ткст field = дайТекст(':');
			Ткст param = дайТекст(':');
			Ид fid(field);
			if(RichPara::fieldtype().найди(fid) >= 0)
				paragraph.конкат(fid, param, формат);
			Ключ('}');
		}
		else
		if(Ключ('&')) {
			устФормат();
			EndPart();
		}
		else
		if(Key2('$')) {
			слей();
			int i = GetNumber();
			Uuid id;
			RichStyle style;
			style.формат = формат;
			if(Ключ(','))
				stylenext.по(i, 0) = GetNumber();
			else
				stylenext.по(i, 0) = i;
			if(Ключ('#')) {
				Ткст xu;
				while(isxdigit(*term))
					xu.конкат(*term++);
				if(xu.дайДлину() != 32)
					Ошибка("Invalid UUID !");
				id = ScanUuid(xu);
			}
			else
				if(i)
					id = Uuid::создай();
				else
					id = RichStyle::GetDefaultId();
			if(Ключ(':'))
				style.имя = дайТекст(']');
			if(fstack.дайСчёт()) {
				формат = fstack.верх();
				fstack.сбрось();
			}
			target.устСтиль(id, style);
			styleid.по(i, RichStyle::GetDefaultId()) = id;
			if(id == RichStyle::GetDefaultId()) {
				bool p = формат.newpage;
				int lng = формат.language;
				(RichPara::фмт&) формат = style.формат;
				формат.styleid = id;
				формат.language = lng;
				формат.newpage = p;
			}
		}
		else
		if(*term == '_') {
			устФормат();
			text.конкат(160);
			term++;
		}
		else
		if(Key2('-', '|')) {
			устФормат();
			text.конкат(9);
		}
		else
		if(*term == '\1') {
			if(istable)
				EndPart();
			устФормат();
			const char *b = ++term;
			for(; *term && *term != '\1'; term++) {
				if((byte)*term == '\n') {
					text.конкат(вЮникод(b, (int)(term - b), формат.charset));
					EndPart();
					b = term + 1;
				}
				if((byte)*term == '\t') {
					text.конкат(вЮникод(b, (int)(term - b), формат.charset));
					text.конкат(9);
					b = term + 1;
				}
			}
			text.конкат(вЮникод(b, (int)(term - b), формат.charset));
			if(*term == '\1')
				term++;
		}
		else {
			if(!Ключ('`')) Ключ('\\');
			if((byte)*term >= ' ') {
				устФормат();
				do {
					if(istable)
						EndPart();
					if(формат.charset == CHARSET_UTF8) {
						bool ok = true;
						wchar c = достаньУтф8(term, ok);
						if(ok)
							конкат(c);
						else
							Ошибка("Invalid UTF-8 sequence");
					}
					else
						конкат(вЮникод((byte)*term++, формат.charset));
				}
				while((byte)*term >= 128 || s_nodeqtf[(byte)*term]);
			}
			else
			if(*term)
				term++;
		}
	}
	if(oldtab)
		FinishOldTable();
	else
		while(table.дайСчёт())
			FinishTable();
	EndPart();
	FlushStyles();
}

bool ParseQTF(RichText& txt, const char *qtf, int accesskey, void *context)
{
	RichQtfParser p(context);
	try {
		p.Parse(qtf, accesskey);
	}
	catch(RichQtfParser::Искл) {
		return false;
	}
	txt = pick(p.target);
	return true;
}

RichText ParseQTF(const char *qtf, int accesskey, void *context)
{
	RichQtfParser p(context);
	try {
		p.Parse(qtf, accesskey);
	}
	catch(RichQtfParser::Искл) {}
	return pick(p.target);
}

Ткст QtfRichObject::вТкст() const
{
	return Ткст("@@#").конкат() << uintptr_t(&obj) << ";";
}

QtfRichObject::QtfRichObject(const RichObject& o)
	: obj(o)
{}

}
