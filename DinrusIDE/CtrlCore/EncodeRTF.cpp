#include "CtrlCore.h"

#define LLOG(x)

#include <plugin/png/png.h>

namespace РНЦП {

static int GetParaHeight(const Массив<RichPara::Part>& parts)
{
	int ht = 0;
	for(int i = 0; i < parts.дайСчёт(); i++) {
		int pht = 0;
		const RichPara::Part& part = parts[i];
		if(part.object)
			pht = part.object.дайРазм().cy;
		else if(part.field)
			pht = GetParaHeight(part.fieldpart);
		else
			pht = tabs(part.формат.дайВысоту());
		if(pht > ht)
			ht = pht;
	}
	return ht;
}

class RTFEncoder {
public:
	RTFEncoder(Поток& stream, const RichText& richtext, byte charset,
		Размер dot_page_size, const Прям& dot_margins, void *context);

	void            пуск();

private:
	void            FacesAddFormat(const RichPara::CharFormat& формат);
	void            GetFaces();
	void            GetTxtFaces(const RichTxt& txt);

	void            PutHeader();
	void            PutDocument();
	void            PutTxt(const RichTxt& txt, int nesting, int dot_width);
	void            PutTable(const RichTable& table, int nesting, int dot_width);
	void            PutParts(const Массив<RichPara::Part>& parts,
		RichPara::CharFormat& base, int bpart, int boff, int epart, int eoff);

	void            старт()                           { stream.помести('{'); }
	void            старт(const char *cmd)            { старт(); Command(cmd); }
	void            старт(const char *cmd, int param) { старт(); Command(cmd, param); }
	void            стоп()                             { stream.помести('}'); }

	void            Command(const char *cmd);
	void            Command(const char *cmd, int param);
	void            Space()                           { stream.помести(' '); }
	void            PutText(const wchar *text);
	void            PutObject(const RichObject& object);
	void            PutTabs(const Вектор<RichPara::Вкладка>& tabs);
	void            PutBinHex(const byte *data, int count);
	void            PutBinHex(const Ткст& s)        { PutBinHex(s, s.дайДлину()); }
	void            PutBorder(ТкстБуф& rowfmt, int wd, Цвет c, const char *clpar);

	bool            PutParaFormat(const RichPara::фмт& pf, const RichPara::фмт& difpf);
	bool            PutCharFormat(const RichPara::CharFormat& cf, const RichPara::CharFormat& difcf, bool pn);

	struct Группа;

	friend struct Группа;

	struct Группа {
		Группа(RTFEncoder *owner) : owner(owner) { owner->старт(); }
		Группа(RTFEncoder *owner, const char *cmd) : owner(owner) { owner->старт(cmd); }
		Группа(RTFEncoder *owner, const char *cmd, int param) : owner(owner) { owner->старт(cmd, param); }
		~Группа() { owner->стоп(); }
		RTFEncoder *owner;
	};

private:
	Поток&               stream;
	const RichText&       richtext;
	byte                  charset;
	Размер                  dot_page_size;
	Прям                  dot_margins;

	RichPara::CharFormat  charfmt;
	RichPara::фмт      parafmt;
	Uuid                  oldstyle;
	int                   old_ht;
	int                   para_ht;

	Индекс<int>            used_faces;
	enum { SYMBOL_INDEX = 1, WINGDINGS_INDEX = 2 };
	ВекторМап<Цвет, int> used_ink, used_paper;
	Индекс<Цвет>          phys_colors;
	Индекс<Uuid>           styleid;
};

void EncodeRTF(Поток& stream, const RichText& richtext, byte charset,
	Размер dot_page_size, const Прям& dot_margins, void *context)
{
	RTFEncoder(stream, richtext, charset, dot_page_size, dot_margins, context).пуск();
}

Ткст EncodeRTF(const RichText& richtext, byte charset, Размер dot_page_size, const Прям& dot_margins, void *context)
{
	ТкстПоток out;
	EncodeRTF(out, richtext, charset, dot_page_size, dot_margins, context);
	Ткст s = out.дайРез();
	LLOG("EncodeRTF <<<<<\n" << s << "\n>>>>> EncodeRTF");
	return s;
}

Ткст EncodeRTF(const RichText& richtext, byte charset, int dot_page_width)
{
	int dot_page_height = iscale(dot_page_width, 1414, 1000);
	Прям margins(472, 472, 472, 472);
	return EncodeRTF(richtext, charset, Размер(dot_page_width + margins.left + margins.right,
		dot_page_height + margins.top + margins.bottom), margins);
}

Ткст EncodeRTF(const RichText& richtext)
{
	return EncodeRTF(richtext, дайДефНабСим() == CHARSET_UTF8 ? GetLNGCharset(GetSystemLNG())
	                                                               : дайДефНабСим());
}

RTFEncoder::RTFEncoder(Поток& stream_, const RichText& richtext_, byte charset_,
	Размер dot_page_size_, const Прям& dot_margins_, void *context_)
: stream(stream_)
, richtext(richtext_)
, charset(charset_)
, dot_page_size(dot_page_size_)
, dot_margins(dot_margins_)
{
	for(int i = 0; i < richtext.GetStyleCount(); i++)
		styleid.добавь(richtext.GetStyleId(i));
}

void RTFEncoder::пуск()
{
	GetFaces();
	Группа docgrp(this, "rtf", 1);
	Command("ansi");
	PutHeader();
	PutDocument();
}

void RTFEncoder::FacesAddFormat(const RichPara::CharFormat& формат)
{
	used_faces.найдиДобавь(формат.GetFace());
	if(used_ink.найди(формат.ink) < 0) {
		Цвет i(формат.ink.дайК(), формат.ink.дайЗ(), формат.ink.дайС());
		int x = used_ink.дай(i, -1);
		if(x < 0)
			x = phys_colors.найдиДобавь(i);
		used_ink.добавь(формат.ink, x);
	}
	if(used_paper.найди(формат.paper) < 0) {
		Цвет p(формат.paper.дайК(), формат.paper.дайЗ(), формат.paper.дайС());
		int x = used_paper.дай(p, -1);
		if(x < 0)
			x = phys_colors.найдиДобавь(p);
		used_paper.добавь(формат.paper, x);
	}
}

void RTFEncoder::GetFaces()
{
	used_faces.добавь(Шрифт::ARIAL); // default font
#ifdef PLATFORM_WIN32
	used_faces.добавь(Шрифт::SYMBOL); // used for bullets
	used_faces.добавь(Шрифт::WINGDINGS); // used for bullets
#endif
	phys_colors.добавь(Null);
	used_ink.добавь(Null, 0);
	used_paper.добавь(Null, 0);
	used_ink.добавь(чёрный(), 0);
	used_paper.добавь(белый(), 0);
	GetTxtFaces(richtext);
	for(int i = 0; i < richtext.GetStyleCount(); i++)
		FacesAddFormat(richtext.дайСтиль(i).формат);
}

void RTFEncoder::GetTxtFaces(const RichTxt& rt)
{
	for(int i = 0; i < rt.GetPartCount(); i++) {
		if(rt.IsTable(i)) {
			const RichTable& table = rt.GetTable(i);
			const RichTable::фмт& tfmt = table.дайФормат();
			phys_colors.найдиДобавь(tfmt.framecolor);
			phys_colors.найдиДобавь(tfmt.gridcolor);
			for(int r = 0; r < table.GetRows(); r++)
				for(int c = 0; c < table.дайКолонки(); c++) {
					const RichCell& cell = table[r][c];
					phys_colors.найдиДобавь(cell.формат.color);
					phys_colors.найдиДобавь(cell.формат.bordercolor);
					GetTxtFaces(cell.text);
				}
		}
		else {
			const RichPara& para = rt.дай(i, richtext.GetStyles());
			FacesAddFormat(para.формат);
			for(int p = 0; p < para.part.дайСчёт(); p++) {
				const RichPara::Part& part = para.part[p];
				if(part.текст_ли())
					FacesAddFormat(part.формат);
			}
		}
	}
}

void RTFEncoder::Command(const char *cmd)
{
	stream.помести('\\');
	stream.помести(cmd);
}

void RTFEncoder::Command(const char *cmd, int param)
{
	stream.помести('\\');
	stream.помести(cmd);
	stream.помести(целТкт(param));
}

void RTFEncoder::PutText(const wchar *text)
{
	for(; *text; text++)
		if(*text >= 128) {
			if(*text >= 65536) {
				Вектор<char16> h = вУтф16(*text);
				stream << "\\uc0 \\u" << (unsigned)h[0] << "\\uc1 \\u" << (unsigned)h[1];
			}
			else
				stream.помести(фмт("\\u%d?", (int16)*text));
		}
		else {
			if(*text == '{' || *text == '}' || *text == '\\')
				stream.помести('\\');
			stream.помести((byte)*text);
		}
}

void RTFEncoder::PutBinHex(const byte *b, int count)
{
	enum { BLOCK = 32 };
	for(int l = count; l > 0; l -= BLOCK) {
		stream.PutCrLf();
		for(const byte *e = b + min<int>(l, BLOCK); b < e; b++) {
			static const char binhex[] = "0123456789abcdef";
			stream.помести(binhex[*b >> 4]);
			stream.помести(binhex[*b & 15]);
		}
	}
}

bool RTFEncoder::PutCharFormat(const RichPara::CharFormat& cf, const RichPara::CharFormat& difcf, bool pn)
{
	int64 pos = stream.дайПоз();
	int pn2 = (pn ? 0 : 2);
	bool f;
	int t;
	if(cf.GetFace() != difcf.GetFace())
		Command(("pnf") + pn2, used_faces.найди(cf.GetFace()));
	if((t = DotPoints(2 * tabs(cf.дайВысоту()))) != DotPoints(2 * tabs(difcf.дайВысоту())))
		Command(("pnfs") + pn2, t);
	if(!pn && dword(t = cf.sscript) != difcf.sscript)
		Command(t == 0 ? "nosupersub" : t == 1 ? "super" : "sub");
	if((f = cf.IsBold())          != difcf.IsBold())          Command((f ? "pnb" : "pnb0") + pn2);
	if((f = cf.IsItalic())        != difcf.IsItalic())        Command((f ? "pni" : "pni0") + pn2);
	if((f = cf.IsUnderline())     != difcf.IsUnderline())     Command((f ? "pnul" : "pnul0") + pn2);
	if((f = cf.IsStrikeout())     != difcf.IsStrikeout())     Command((f ? "pnstrike" : "pnstrike0") + pn2);
	if((f = cf.capitals)          != difcf.capitals)          Command((f ? "pncaps" : "pncaps0") + pn2);
	if((t = used_ink.дай(cf.ink)) != used_ink.дай(difcf.ink)) Command(("pncf") + pn2, t);
	if((t = used_paper.дай(cf.paper)) != used_paper.дай(difcf.paper)) Command(("pncb") + pn2, t);
#ifdef PLATFORM_WIN32
	if(!pn && cf.language         != difcf.language)          Command("lang", GetLanguageLCID(cf.language));
#endif
	// todo: link
	return stream.дайПоз() != pos;
}

void RTFEncoder::PutObject(const RichObject& object)
{
	Размер log_size = object.GetPhysicalSize(); // / 6;
	Размер out_size = object.дайРазм();
	Размер out_size_01mm = iscale(out_size, 254, 60);
	if(log_size.cx <= 0 || log_size.cy <= 0) log_size = out_size / 6;
	Группа pict_grp(this, "pict");
	Command("picw", out_size_01mm.cx);
	Command("pich", out_size_01mm.cy);
	Command("picwgoal", DotTwips(out_size.cx));
	Command("pichgoal", DotTwips(out_size.cy));
	Command("pngblip");
	PutBinHex(PNGEncoder().SaveString(object.ToImage(out_size)));
}

bool RTFEncoder::PutParaFormat(const RichPara::фмт& pf, const RichPara::фмт& difpf)
{
	int64 pos = stream.дайПоз();
	if(pf.align != difpf.align)
		switch(pf.align) {
		case ALIGN_NULL:
		case ALIGN_LEFT:    Command("ql"); break;
		case ALIGN_CENTER:  Command("qc"); break;
		case ALIGN_RIGHT:   Command("qr"); break;
		case ALIGN_JUSTIFY: Command("qj"); break;
		default:            NEVER();
		}
	int oind = difpf.indent, olm = difpf.lm;
	if(difpf.bullet != RichPara::BULLET_NONE) {
		olm += oind;
		oind = -oind;
	}
	int nind = pf.indent, nlm = pf.lm;
	if(pf.bullet != RichPara::BULLET_NONE) {
		nlm += nind;
		nind = -nind;
	}
//	if(nind      != oind)         Command("fi", DotTwips(nind));
	if(nlm       != olm)          Command("li", DotTwips(nlm));
	if(pf.rm     != difpf.rm)     Command("ri", DotTwips(pf.rm));
	if(pf.newpage)
		Command("pagebb");
	if(pf.before != difpf.before) Command("sb", DotTwips(pf.before));
	if(pf.after  != difpf.after)  Command("sa", DotTwips(pf.after));
	if(pf.tab    != difpf.tab)    PutTabs(pf.tab);
	Command("fi", DotTwips(nind)); // we always put number last as quick patch to "space at bullet" problem
	return stream.дайПоз() != pos;
}

void RTFEncoder::PutTabs(const Вектор<RichPara::Вкладка>& tabs)
{
	for(int i = 0; i < tabs.дайСчёт(); i++) {
		RichPara::Вкладка t = tabs[i];
		switch(t.align) {
		case ALIGN_NULL:
		case ALIGN_LEFT:   break;
		case ALIGN_CENTER: Command("tqc"); break;
		case ALIGN_RIGHT:  Command("tqr"); break;
		default: NEVER();
		}
		switch(t.fillchar) {
		case 0: break;
		case 1: Command("tldot"); break;
		case 2: Command("tlhyph"); break;
		case 3: Command("tlul"); break;
		default: NEVER();
		}
		Command("tx", DotTwips(t.pos));
	}
}

void RTFEncoder::PutParts(const Массив<RichPara::Part>& parts,
	RichPara::CharFormat& base, int bpart, int boff, int epart, int eoff)
{
	if(epart >= parts.дайСчёт()) {
		epart = parts.дайСчёт() - 1;
		eoff = INT_MAX;
	}
	ПРОВЕРЬ(boff >= 0 && bpart >= 0);
	for(int p = bpart; p <= epart; p++) {
		const RichPara::Part& part = parts[p];
		if(part.текст_ли()) {
			ШТкст px = part.text;
			if(p == epart && px.дайДлину() > eoff)
				px.обрежь(eoff);
			if(p == bpart)
				px.удали(0, boff);
			if(!px.пустой()) {
				if(PutCharFormat(part.формат, base, false) || p == 0)
					Space();
				base = part.формат;
				PutText(px);
			}
		}
		else if(part.object)
			PutObject(part.object);
	}

}

void RTFEncoder::PutHeader()
{
	struct CodePage {
		byte  charset;
		short ansicpg;
		short fontcpg;
	};
	
	static const CodePage ansicpg[] = {
		{ CHARSET_WIN1250, 1250, 238 },
		{ CHARSET_WIN1251, 1251, 204 },
		{ CHARSET_WIN1252, 1252,   0 },
		{ CHARSET_WIN1253, 1253, 161 },
		{ CHARSET_WIN1254, 1254, 162 },
		{ CHARSET_WIN1255, 1255, 177 },
		{ CHARSET_WIN1256, 1256, 178 },
		{ CHARSET_WIN1257, 1257, 186 },
		{ CHARSET_WIN1258, 1258, 163 },
	};
	int fontcpg = Null;
	for(int i = __countof(ansicpg); --i >= 0;)
		if(charset == ansicpg[i].charset) {
			Command("ansicpg", ansicpg[i].ansicpg);
			fontcpg = ansicpg[i].fontcpg;
			break;
		}

	Command("deff", 0);
	{
		Группа ftbl(this, "fonttbl");
		for(int i = 0; i < used_faces.дайСчёт(); i++) {
			Группа fnt(this, "f", i);
			int фн = used_faces[i];
			int cs = fontcpg;
			dword info = Шрифт::GetFaceInfo(фн);
#ifdef PLATFORM_WIN32
			if(фн == Шрифт::SYMBOL) {
				Command("ftech");
				cs = 2;
			}
			else
#endif
			if(info & Шрифт::FIXEDPITCH)
				Command("fmodern");
			else if(фн == Шрифт::ROMAN)
				Command("froman");
			else if(фн == Шрифт::ARIAL
#ifdef PLATFORM_WIN32
			|| фн == Шрифт::TAHOMA
#endif
			)
				Command("fswiss");
			if(!пусто_ли(fontcpg))
				Command("fcharset", cs);
			Space();
			stream.помести(Шрифт::GetFaceName(фн));
			stream.помести(';');
		}
	}

	if(phys_colors.дайСчёт() > 1) {
		Группа ctbl(this, "colortbl");
		stream.помести(';');
		for(int i = 1; i < phys_colors.дайСчёт(); i++) {
			Цвет rgb = phys_colors[i];
			Command("red",   rgb.дайК());
			Command("green", rgb.дайЗ());
			Command("blue",  rgb.дайС());
			stream.помести(';');
		}
	}

	старт("stylesheet");
		RichPara::CharFormat empcfmt;
		RichPara::фмт emppfmt;

		for(int i = 0; i < richtext.GetStyleCount(); i++) {
			const RichStyle& style = richtext.дайСтиль(i);
			старт("s", i);
				PutParaFormat(style.формат, emppfmt);
				PutCharFormat(style.формат, empcfmt, false);
				Command("sbasedon", 222);
				if(style.next != richtext.GetStyleId(i))
					Command("snext", styleid.найди(style.next));
				Space();
				for(const char *n = style.имя; *n; n++)
					stream.помести(*n == ';' ? '_' : *n);
				stream.помести(';');
			стоп();
		}
	стоп();
	
	Command("paperw", DotTwips(dot_page_size.cx));
	Command("paperh", DotTwips(dot_page_size.cy));
	Command("margl", DotTwips(dot_margins.left));
	Command("margr", DotTwips(dot_margins.right));
	Command("margt", DotTwips(dot_margins.top));
	Command("margb", DotTwips(dot_margins.bottom));
}

void RTFEncoder::PutTable(const RichTable& table, int nesting, int dot_width)
{
	Вектор<int> vspan_counts;
	const RichTable::фмт& tfmt = table.дайФормат();
	Вектор<int> column_pos;
	column_pos.устСчёт(tfmt.column.дайСчёт() + 1);
	column_pos[0] = tfmt.lm;
	int sum = 0;
	for(int c = 0; c < tfmt.column.дайСчёт(); c++)
		sum += tfmt.column[c];
	int rem_width = dot_width - tfmt.lm - tfmt.rm;
	for(int c = 0; c < tfmt.column.дайСчёт(); c++) {
		int part = iscale(tfmt.column[c], rem_width, max(sum, 1));
		sum -= tfmt.column[c];
		rem_width -= part;
		column_pos[c + 1] = column_pos[c] + part;
	}
	for(int r = 0; r < table.GetRows(); r++) {
		ТкстБуф rowfmt;
		if(nesting)
			rowfmt << "\\*\\nesttableprops";
		rowfmt << "\\trowd"
			"\\trleft" << DotTwips(tfmt.lm)
			// trbrdr[ltrbv]
			;
		Вектор<int> cellindex;
		Прям dflt_margin(600, 600, 600, 600);
		for(int c = 0; c < table.дайКолонки(); c++) {
			const RichCell& cell = table[r][c];
			dflt_margin.left = min(dflt_margin.left, cell.формат.margin.left);
			dflt_margin.top = min(dflt_margin.top, cell.формат.margin.top);
			dflt_margin.right = min(dflt_margin.right, cell.формат.margin.right);
			dflt_margin.bottom = min(dflt_margin.bottom, cell.формат.margin.bottom);
			c += cell.hspan;
		}
/*
		rowfmt <<
			"\\trpaddl" << DotTwips(dflt_margin.left) << "\\trpaddfl3"
			"\\trpaddt" << DotTwips(dflt_margin.top) << "\\trpaddft3"
			"\\trpaddr" << DotTwips(dflt_margin.right) << "\\trpaddfr3"
			"\\trpaddb" << DotTwips(dflt_margin.bottom) << "\\trpaddfb3";
*/
		bool istop = (r == 0);
		bool isbottom = (r == table.GetRows() - 1);
		for(int c = 0; c < table.дайКолонки(); c++) {
			const RichCell& cell = table[r][c];
/*
			if(cell.формат.margin.left != dflt_margin.left)
				rowfmt << "\\clpadl" << DotTwips(cell.формат.margin.left) << "\\clpadfl3";
			if(cell.формат.margin.top != dflt_margin.top)
				rowfmt << "\\clpadt" << DotTwips(cell.формат.margin.top) << "\\clpadft3";
			if(cell.формат.margin.right != dflt_margin.right)
				rowfmt << "\\clpadr" << DotTwips(cell.формат.margin.right) << "\\clpadfr3";
			if(cell.формат.margin.bottom != dflt_margin.bottom)
				rowfmt << "\\clpadb" << DotTwips(cell.формат.margin.bottom) << "\\clpadfb3";
*/
			switch(cell.формат.align) {
				case ALIGN_TOP: rowfmt << "\\clvertalt"; break;
				default:
				case ALIGN_CENTER: rowfmt << "\\clvertalc"; break;
				case ALIGN_BOTTOM: rowfmt << "\\clvertalb"; break;
			}
			bool isleft = (c == 0);
			cellindex.добавь(c);
			if(cell.hspan)
				c += cell.hspan;
			bool isright = (c + 1 == table.дайКолонки());
			int cell_end = column_pos[c + 1];
			if(cell.vspan) {
				vspan_counts.по(c, 0) = cell.vspan;
				rowfmt << "\\clvmgf";
			}
			else if(c < vspan_counts.дайСчёт() && vspan_counts[c] > 0)
				rowfmt << "\\clvmrg";
			if(!пусто_ли(cell.формат.color))
				rowfmt << "\\clcbpat" << phys_colors.найди(cell.формат.color);
			int lb, tb, rb, bb;
			Цвет lc, tc, rc, bc;
			const RichTable::фмт& tf = table.дайФормат();
			lc = tc = rc = bc = tf.gridcolor;
			lb = tb = rb = bb = (!пусто_ли(tf.gridcolor) ? tf.grid : 0);
			int fw = (!пусто_ли(tf.framecolor) ? tf.frame : 0);
			if(isleft) {
				lb = fw;
				lc = tf.framecolor;
			}
			if(isright) {
				rb = fw;
				rc = tf.framecolor;
			}
			if(istop) {
				tb = fw;
				tc = tf.framecolor;
			}
			if(isbottom) {
				bb = fw;
				bc = tf.framecolor;
			}
			if(!пусто_ли(cell.формат.bordercolor)) {
				if(cell.формат.border.left >= max(lb, 1)) {
					lb = cell.формат.border.left;
					lc = cell.формат.bordercolor;
				}
				if(cell.формат.border.top >= max(tb, 1)) {
					tb = cell.формат.border.top;
					tc = cell.формат.bordercolor;
				}
				if(cell.формат.border.right >= max(rb, 1)) {
					rb = cell.формат.border.right;
					rc = cell.формат.bordercolor;
				}
				if(cell.формат.border.bottom >= max(bb, 1)) {
					bb = cell.формат.border.bottom;
					bc = cell.формат.bordercolor;
				}
			}
			PutBorder(rowfmt, lb, lc, "clbrdrl");
			PutBorder(rowfmt, tb, tc, "clbrdrt");
			PutBorder(rowfmt, rb, rc, "clbrdrr");
			PutBorder(rowfmt, bb, bc, "clbrdrb");
			rowfmt << "\\cellx" << DotTwips(cell_end);
		}
		старт();
		Command("pard");
		Command("intbl");
		parafmt = RichPara::фмт();
		charfmt = RichPara::CharFormat();
		if(nesting)
			Command("itap", nesting + 1);
		Ткст fmtstr = rowfmt;
		if(!nesting)
			stream.помести(fmtstr);
		for(int c = 0; c < cellindex.дайСчёт(); c++) {
			int cx = cellindex[c];
			const RichCell& cell = table[r][cx];
			int cell_wd = column_pos[cx + cell.hspan + 1] - column_pos[cx];
			PutTxt(cell.text, nesting + 1, cell_wd);
			Command(nesting ? "nestcell" : "cell");
		}
		if(nesting)
			stream.помести(fmtstr);
		Command(nesting ? "nestrow" : "row");
		стоп();
	}
	oldstyle = Null;
}

void RTFEncoder::PutBorder(ТкстБуф& rowfmt, int wd, Цвет c, const char *clpar)
{
	if(wd > 0) { 
		rowfmt << "\\" << clpar << "\\brdrs\\brdrw" << wd << "\\brdrcf"
		<< phys_colors.найди(Nvl(c, чёрный())) << " ";
	}
}

void RTFEncoder::PutTxt(const RichTxt& txt, int nesting, int dot_width)
{
	for(int i = 0; i < txt.GetPartCount(); i++) {
		if(txt.IsTable(i))
			PutTable(txt.GetTable(i), nesting, dot_width);
		else {
			const RichPara& para = txt.дай(i, richtext.GetStyles());
			Uuid pstyle = txt.GetParaStyle(i);
			int new_para_ht = GetParaHeight(para.part);
			int first_part = 0, first_ind = 0;
			bool hdiff = (para.формат.bullet != RichPara::BULLET_NONE
				&& para.формат.bullet != RichPara::BULLET_TEXT && new_para_ht != para_ht);
			if(pstyle != oldstyle
			|| para.формат.bullet != parafmt.bullet || para.формат.bullet == RichPara::BULLET_TEXT
			|| hdiff || para.формат.tab != parafmt.tab || para.формат.newpage || parafmt.newpage) {
				Command("s", styleid.найди(pstyle));
				oldstyle = pstyle;
				parafmt = richtext.дайСтиль(pstyle).формат;
				Command("pard");
				if(nesting)
					Command("intbl");
				if(nesting > 1)
					Command("itap", nesting);
//				parafmt = RichPara::фмт();
//				bool isul = charfmt.IsUnderline();
//				charfmt = RichPara::CharFormat();
//				charfmt.Underline(isul);
				para_ht = 0;
			}
			if(para.формат.bullet == RichPara::BULLET_TEXT) {
				int epart = 0, eoff = 0;
				while(epart < para.part.дайСчёт() && (eoff = para.part[epart].text.найди('\t')) < 0)
					epart++;

				RichPara::CharFormat rcf = charfmt;
				rcf.устВысоту(0);
				старт("pntext");
					PutParts(para.part, rcf, 0, 0, epart, eoff);
					stream.помести('\t');
				стоп();

				старт("*\\pn");
					Command("pnhang");
					Command("pnql");
					rcf = charfmt;
					if(epart > 0 || eoff > 0)
						PutCharFormat(para.part[0].формат, rcf, true);
					старт("pntxta");
					стоп();
					старт("pntxtb");
						Space();
						PutParts(para.part, rcf, 0, 0, epart, eoff);
						stream.помести('\t');
					стоп();
				стоп();
			}
			else if(para.формат.bullet != RichPara::BULLET_NONE) {
				int sym_face = SYMBOL_INDEX;
				byte sym_char = 0xB7;

				switch(para.формат.bullet) {
				default:
					NEVER();

				case RichPara::BULLET_ROUND:      sym_face = SYMBOL_INDEX;    sym_char = 0xB7; break;
				case RichPara::BULLET_ROUNDWHITE: sym_face = WINGDINGS_INDEX; sym_char = 0xA1; break;
				case RichPara::BULLET_BOX:        sym_face = WINGDINGS_INDEX; sym_char = 'n'; break;
				case RichPara::BULLET_BOXWHITE:   sym_face = WINGDINGS_INDEX; sym_char = 'o'; break;
				}

				старт("pntext");
	//					Command("tqr");
	//					Command("tx", DotTwips(para.формат.indent));
	//					Command("qr");
					Command("f", sym_face);
					Command("fs", DotPoints(2 * new_para_ht));
					Space();
					stream.помести(sym_char);
					stream.помести('\t');
				стоп();

				старт("*\\pn");
					Command("pnlvlblt");
					Command("pnf", sym_face);
					Command("pnfs", DotPoints(2 * new_para_ht));
					Command("pnql");
					старт("pntxtb");
						Space();
						stream.помести(sym_char);
	//						stream.помести('\t');
					стоп();
					старт("pntxta");
					стоп();
				стоп();
			}
			PutParaFormat(para.формат, parafmt);
			para_ht = new_para_ht;
			parafmt = para.формат;
			PutParts(para.part, charfmt, first_part, first_ind, para.part.дайСчёт(), 0);
			if(i + 1 < txt.GetPartCount())
				Command("par");
		}
	}
}

void RTFEncoder::PutDocument()
{
	charfmt.устВысоту(PointDots(0));
	charfmt.language = 0;
	old_ht = DotPoints(2 * tabs(charfmt.дайВысоту()));
	para_ht = 0;
	oldstyle = Uuid::создай();
	PutTxt(richtext, 0, dot_page_size.cx - dot_margins.left - dot_margins.right);
}

}
