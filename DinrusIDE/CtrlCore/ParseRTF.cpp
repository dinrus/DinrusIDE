#include "CtrlCore.h"

namespace РНЦП {

#define LLOG(x) // DLOG(x)

static int TwipDotsLim(int twips) { return minmax<int>(TwipDots(twips), 0, MAX_DOTS); }

static Ткст FromCString(const char *p, const char **endptr = NULL)
{
	if(endptr) {
		const char *e = p;
		if(*e == '\"')
			e++;
		while(*e && *e != '\"')
			if(*e++ == '\\' && *e)
				e++;
		if(*e == '\"')
			e++;
		*endptr = e;
	}

	try {
		СиПарсер parser(p);
		return parser.читай1Ткст();
	}
	catch(Искл e) {
		return Null;
	}
}

class RTFParser
{
public:
	RTFParser(const char *rtf);

	RichText      пуск();

private:
	enum TOKEN { T_EOF, T_TEXT, T_COMMAND, T_GROUP, T_END_GROUP };

	void          слей(bool force, int itap);
	void          OpenTable(int level);
	void          FlushTable(int level);
	TOKEN         Fetch();
	void          пропусти();
	TOKEN         Token()                  { if(!is_full) Fetch(); return token; }
	bool          PassIf(bool c)           { is_full &= !c; return c; }
	bool          PassText()               { return PassIf(Token() == T_TEXT); }
	bool          PassGroup()              { return PassIf(Token() == T_GROUP); }
	bool          PassEndGroup()           { return PassIf(Token() == T_END_GROUP || token == T_EOF); }
	bool          PassEndGroup(int level);
	bool          PassCmd(const char *cmd) { return PassIf(Token() == T_COMMAND && command == cmd); }
	bool          PassQ(const char *cmd)   { return PassIf(command == cmd); }

	void          SkipGroup()              { SkipGroup(stack.дайСчёт()); }
	void          SkipGroup(int level);
	int           Level() const            { return stack.дайСчёт(); }

	void          ReadItem();
	void          ReadItemGroup(int level);
	void          ReadItemGroup()          { ReadItemGroup(Level()); }
	void          читайТекст();
	void          читайТекст(const ШТкст& text);
	void          ReadChar(word ch)        { читайТекст(ШТкст(ch, 1)); }
	void          ReadCommand();

	void          ReadHeader();
		void          ReadFaceTable();
		void          ReadColorTable();
		void          ReadCharSet();

	void          ReadMisc();
		void          ReadField();
		bool          ReadField(const char *def);
		void          ReadPict();
		void          ReadShape();

	void          ReadParaStyle();
	void          ReadTableStyle();
	void          DefaultParaStyle();

	void          ReadCharStyle();
	void          ReadCellBorder(int& width);

	Ткст        ReadBinHex(char& odd) const;

private:
	const char   *rtf;

	TOKEN         token;
	bool          is_full;
	bool          next_command;
	ШТкст       text;
	Ткст        command;
	int           command_arg;

	struct ИнфОЯчейке {
		ИнфОЯчейке();
		
		RichCell::фмт формат;
		Прям cellmarginunits;
		int shading;
		Цвет shading_fore;
		Цвет shading_back;
		int end_dots;
	};

	struct Cell {
		Cell();

		ИнфОЯчейке info;
		RichTxt text;
		bool merge_first;
		bool merge;
		int nbegin;
		Размер span;
	};
	
	struct Face : Движимое<Face> {
		int  face;
		byte charset;
	};

	struct TableState {
		TableState() : textcol(0), stylecol(0) { cells.добавь(); }

		RichTable::фмт tableformat;
		Вектор< Массив<Cell> > cells;
		int textcol;
		int stylecol;
	};

	ИнфОЯчейке&     CellInfoAt(int i);
	Cell&         CellAt(TableState& ts, int i);
	void          SetCellMargin(Cell& cell, int Прям::*mbr);

	struct State {
		Ткст               dest;
		RichPara::фмт     формат;
		RichPara::CharFormat charformat;
		WithDeepCopy< Массив<ИнфОЯчейке> > cellinfo;
		int                  trgaph;
		Прям                 rowmargin;
		Прям                 rowmarginunits;
		Прям                 rowspacing;
		Прям                 rowspacingunits;
		Прям                 cellmarginunits;
		int                  uc_value;
		int                  left_margin;
		int                  right_margin;
		int                  first_indent;
		bool                 in_table;
		int                  itap;
		bool                 nestprop;
		bool                 new_dest;
		byte                 charset;
	};

	Массив<State>  stack;
	Массив<TableState> table_stack;
	State         state;
	RichPara::CharFormat plain_format;
	RichPara::фмт pard_format;
	ИнфОЯчейке      std_cell_info;
	byte          plain_charset;
	byte          default_charset;
	int           default_font;
	Alignment     tab_align;
	byte          tab_fill;
	Вектор<Face>  face_table;
	Вектор<Цвет> color_table;
	int           paper_width;
	int           left_margin;
	int           right_margin;

	RichText      output;
	RichPara      para;
};

RichText ParseRTF(const char *rtf) { return RTFParser(rtf).пуск(); }

RTFParser::ИнфОЯчейке::ИнфОЯчейке()
: cellmarginunits(0, 0, 0, 0)
, shading(0)
, shading_fore(чёрный())
, shading_back(белый())
, end_dots(0)
{
}

RTFParser::Cell::Cell()
: merge_first(false)
, merge(false)
, nbegin(0)
, span(0, 0)
{
}

RTFParser::RTFParser(const char *rtf)
:	rtf(rtf)
{
#ifdef _DEBUG0
	сохраниФайл(конфигФайл("rtfparser.rtf"), rtf);
	LOG(rtf);
#endif
	is_full = false;
	next_command = false;
	default_font = 0;
	plain_charset = default_charset = state.charset = CHARSET_WIN1250;
	state.uc_value = 1;
	state.new_dest = false;
	plain_format.Face(Шрифт::ARIAL).устВысоту(100);
	std_cell_info.формат.align = ALIGN_TOP;
	std_cell_info.формат.margin = Прям(25, 25, 25, 25);
	DefaultParaStyle();
	state.charformat = plain_format;
	tab_align = ALIGN_LEFT;
	tab_fill = 0;
	paper_width = 5100;
	left_margin = right_margin = 750;
}

RichText RTFParser::пуск()
{
	if(!PassGroup() || !PassCmd("rtf") || command_arg != 1 && !пусто_ли(command_arg))
		return pick(output);
	while(Token() != T_EOF)
		ReadItem();
	слей(false, 1);
	FlushTable(0);
	return pick(output);
}

void RTFParser::FlushTable(int level)
{
	while(table_stack.дайСчёт() > level) {
		TableState& child = table_stack.верх();
		while(!child.cells.пустой() && child.cells.верх().пустой())
			child.cells.сбрось();
		if(child.cells.пустой()) {
			table_stack.сбрось();
			continue;
		}
		Индекс<int> dot_index;
//		int pos = child.tableformat.lm;
		dot_index.добавь(child.tableformat.lm);
		for(int r = 0; r < child.cells.дайСчёт(); r++) {
			Массив<Cell>& rw = child.cells[r];
			for(int c = 0; c < rw.дайСчёт(); c++)
				dot_index.найдиДобавь(rw[c].info.end_dots);
		}
		Вектор<int> dot_order = dot_index.подбериКлючи();
		сортируй(dot_order);
		RichTable table;
		if(table_stack.дайСчёт() == 1)
			child.tableformat.rm = max(paper_width - left_margin - right_margin - dot_order.верх(), 0);
//		child.tableformat.before = state.формат.before;
//		child.tableformat.after = state.формат.after;
		table.устФормат(child.tableformat);
		for(int c = 1; c < dot_order.дайСчёт(); c++)
			table.добавьКолонку(dot_order[c] - dot_order[c - 1]);
		dot_index = pick(dot_order);
		int tbl_border = Null, tbl_grid = Null;
		Цвет clr_border = Null, clr_grid = Null;
		for(int r = 0; r < child.cells.дайСчёт(); r++) {
			Массив<Cell>& rw = child.cells[r];
			int pos = child.tableformat.lm;
			for(int c = 0; c < rw.дайСчёт(); c++) {
				Cell& cell = rw[c];
				if(cell.merge) {
					pos = cell.info.end_dots;
					continue;
				}
				cell.span.cy = 0;
				if(cell.merge_first) {
					for(int m = r + 1; m < child.cells.дайСчёт(); m++) {
						const Массив<Cell>& mrw = child.cells[m];
						int mc = mrw.дайСчёт();
						while(--mc >= 0 && mrw[mc].info.end_dots > cell.info.end_dots)
							;
						if(mc >= 0 && mrw[mc].info.end_dots == cell.info.end_dots && mrw[mc].merge)
							cell.span.cy++;
						else
							break;
					}
				}
				cell.nbegin = dot_index.найди(pos);
				cell.span.cx = max(0, dot_index.найди(pos = cell.info.end_dots) - cell.nbegin - 1);
				if(cell.span.cx < 0) {
					cell.merge = true;
					continue;
				} 
				bool outer_border[] = {
					cell.nbegin == 0,
					r == 0,
					cell.nbegin + cell.span.cx + 2 >= dot_index.дайСчёт(),
					r + cell.span.cy + 1 >= child.cells.дайСчёт(),
				};
				int border_width[] = {
					cell.info.формат.border.left,
					cell.info.формат.border.top,
					cell.info.формат.border.right,
					cell.info.формат.border.bottom,
				};
				for(int b = 0; b < __countof(border_width); b++) {
					int& out_wd = (outer_border[b] ? tbl_border : tbl_grid);
					Цвет& out_co = (outer_border[b] ? clr_border : clr_grid);
					if(пусто_ли(cell.info.формат.bordercolor) || border_width[b] <= 0
					|| !пусто_ли(out_co) && out_co != cell.info.формат.bordercolor)
						out_wd = 0;
					else if(пусто_ли(out_wd) || border_width[b] < out_wd) {
						out_wd = border_width[b];
						out_co = cell.info.формат.bordercolor;
					}
				}
				if(cell.info.shading > 0) {
					Цвет zero = белый();
					Цвет one = Nvl(cell.info.shading_fore, чёрный());
					int r = zero.дайК() + iscale(one.дайК() - zero.дайК(), cell.info.shading, 10000);
					int g = zero.дайЗ() + iscale(one.дайЗ() - zero.дайЗ(), cell.info.shading, 10000);
					int b = zero.дайС() + iscale(one.дайС() - zero.дайС(), cell.info.shading, 10000);
					cell.info.формат.color = Цвет(r, g, b);
				}
			}
		}
		RichTable::фмт tf = table.дайФормат();
		tf.frame = Nvl(tbl_border, 0);
		tf.framecolor = (tf.frame > 0 ? clr_border : Цвет(Null));
		tf.grid = Nvl(tbl_grid, 0);
		tf.gridcolor = (tf.grid > 0 ? clr_grid : Цвет(Null));
		table.устФормат(tf);
		for(int r = 0; r < child.cells.дайСчёт(); r++) {
			Массив<Cell>& rw = child.cells[r];
//			int pos = child.tableformat.lm;
			for(int c = 0; c < rw.дайСчёт(); c++) {
				Cell& cell = rw[c];
				if(cell.merge)
					continue;
				if(cell.span.cx || cell.span.cy)
					table.SetSpan(r, cell.nbegin, cell.span.cy, cell.span.cx);
				bool outer_border[] = {
					cell.nbegin == 0,
					r == 0,
					cell.nbegin + cell.span.cx + 2 >= dot_index.дайСчёт(),
					r + cell.span.cy + 1 >= child.cells.дайСчёт(),
				};
				int *border_width[] = {
					&cell.info.формат.border.left,
					&cell.info.формат.border.top,
					&cell.info.формат.border.right,
					&cell.info.формат.border.bottom,
				};
				for(int b = 0; b < __countof(border_width); b++) {
					int tbl_wd = (outer_border[b] ? tbl_border : tbl_grid);
//					Цвет tbl_co = (outer_border[b] ? clr_border : clr_grid);
					if(*border_width[b] <= tbl_wd)
						*border_width[b] = 0;
				}
				table.устФормат(r, cell.nbegin, cell.info.формат);
				cell.text.нормализуй();
				table.SetPick(r, cell.nbegin, pick(cell.text));
			}
		}
		table.нормализуй();
		table_stack.сбрось();
		if(table_stack.пустой())
			output.CatPick(pick(table));
		else {
			TableState& par = table_stack.верх();
			CellAt(par, par.textcol).text.CatPick(pick(table));
		}
	}
}

void RTFParser::слей(bool force, int itap)
{
	if(!para.part.пустой() || force) {
		int fi = state.first_indent, li = state.left_margin, ri = state.right_margin;
		if(state.формат.bullet != RichPara::BULLET_NONE) {
			разверни(li, fi);
//			li += fi;
//			fi = -fi;
		}
		state.формат.indent = minmax<int>(fi, 0, MAX_DOTS);
		state.формат.lm = minmax<int>(li, 0, MAX_DOTS);
		state.формат.rm = minmax<int>(ri, 0, MAX_DOTS);
		para.формат = state.формат;
		if(state.in_table) {
			FlushTable(itap);
			OpenTable(itap);
			TableState& ts = table_stack[itap - 1];
			CellAt(ts, ts.textcol).text.конкат(para, output.GetStyles());
		}
		else {
			FlushTable(0);
			output.конкат(para);
		}
		para.part.очисть();
	}
	else
		FlushTable(itap);
}

RTFParser::TOKEN RTFParser::Fetch()
{
	is_full = true;
	text.очисть();
	if(next_command)
	{
		next_command = false;
		return token = T_COMMAND;
	}

	command = Null;
	command_arg = Null;

	int skip = 0;
	while(*rtf && *rtf != '{' && *rtf != '}')
	{
		int c = 0, nskip = max(skip - 1, 0);
		if((byte)*rtf < ' ')
			rtf++;
		else if(*rtf != '\\')
			c = вЮникод(*rtf++, state.charset);
		else
			switch(rtf++, *rtf++)
			{
				case 0: {
					rtf--;
					break;
				}

				case '{':
				case '}':
				case '\\': {
					c = rtf[-1];
					break;
				}

				case '~': {
					c = 160;
					break;
				}

				case '|':
				case '-':
				case '_':
				case ':': {
					command = Ткст(rtf - 1, 1);
					LLOG("Command " << command);
					if(text.пустой())
						return token = T_COMMAND;
					next_command = true;
					return token = T_TEXT;
				}

				case '\'': {
					int c1 = ctoi(*rtf);
					if(c1 < 16) {
						int c2 = ctoi(*++rtf);
						if(c2 < 16) {
							c1 = c1 * 16 + c2;
							rtf++;
						}
						c = вЮникод(c1, state.charset);
					}
					break;
				}

				default: {
					if(IsAlpha(*--rtf) || *rtf == '*' && rtf[1] == '\\' && IsAlpha(rtf[2])) {
						if(*rtf == '*') {
							rtf += 2;
							state.new_dest = true;
							LLOG("NewDest");
						}
						const char *b = rtf;
						while(IsAlpha(*++rtf))
							;
						command = Ткст(b, rtf);
						if(цифра_ли(*rtf) || *rtf == '-')
							command_arg = strtol(rtf, (char **)&rtf, 10);
						if(*rtf == ' ')
							rtf++;
						if(command == "uc")
							state.uc_value = command_arg;
						else if(command == "u") {
							c = command_arg;
							nskip = state.uc_value;
						}
						else { // command - quit reading text
							LLOG("Command " << command);
							if(text.пустой())
								return token = T_COMMAND;
							next_command = true;
							return token = T_TEXT;
						}
					}
					break;
				}
			}
		if(c && !skip) {
			text.конкат(c);
			if(text.дайСчёт() >= 2) {
				char16 h[2];
				h[0] = text[text.дайСчёт() - 2];
				h[1] = text[text.дайСчёт() - 1];
				wchar c = читайСуррогПару(h, h + 2);
				if(c) {
					text.обрежьПоследн(2);
					text.конкат(c);
				}
			}
		}
		skip = nskip;
	}

	if(!text.пустой()) {
		LLOG("TEXT, size: " << text.дайСчёт());
		return token = T_TEXT;
	}

	if(*rtf == '{') {
		stack.добавь(state);
		rtf++;
		return token = T_GROUP;
	}

	if(*rtf == '}') {
		if(!stack.пустой()) {
			state = stack.верх();
			stack.сбрось();
		}
		rtf++;
		return token = T_END_GROUP;
	}

	return token = T_EOF;
}

bool RTFParser::PassEndGroup(int level)
{
	if(Token() == T_EOF)
		return true;
	if(token != T_END_GROUP)
		return false;
	is_full = false;
	return Level() < level;
}

void RTFParser::пропусти()
{
	LLOG("пропусти");
	bool is_group = (token == T_GROUP || token == T_COMMAND && state.new_dest);
	is_full = false;
	if(is_group)
		SkipGroup();
}

void RTFParser::SkipGroup(int level)
{
	while(!PassEndGroup(level))
		is_full = false;
}

void RTFParser::ReadItem()
{
	const char *p = rtf;
	if(token == T_COMMAND)
		ReadCommand();
	else if(token == T_TEXT)
		читайТекст();
	if(rtf == p && is_full) {
		is_full = false;
		if(token == T_COMMAND && state.new_dest && command != "shppict") {
			LLOG("SkipGroup new_dest " << command);
			SkipGroup();
		}
		state.new_dest = false;
	}
}

void RTFParser::ReadItemGroup(int level)
{
	while(!PassEndGroup(level))
		ReadItem();
}

void RTFParser::читайТекст()
{
	if(!пусто_ли(text))
		читайТекст(text);
}

void RTFParser::читайТекст(const ШТкст& text)
{
	if(!пусто_ли(state.dest))
		return;
	LLOG("Output text: <" << изЮникода(text, state.charset) << ">, " << state.charformat);
	para.конкат(text, state.charformat);
}

void RTFParser::ReadCommand()
{
	if(Token() == T_COMMAND) ReadHeader();
	if(Token() == T_COMMAND) ReadMisc();
	if(Token() == T_COMMAND) ReadParaStyle();
	if(Token() == T_COMMAND) ReadTableStyle();
	if(Token() == T_COMMAND) ReadCharStyle();
}

void RTFParser::ReadHeader()
{
	if(PassCmd("deff"))
		default_font = command_arg;
	else if(PassQ("fonttbl")) {
		state.dest = command;
		ReadFaceTable();
	}
	else if(PassQ("colortbl")) {
		state.dest = command;
		ReadColorTable();
	}
	else if(PassQ("stylesheet") || PassQ("list") || PassQ("listoverride") || PassQ("info")) {
		state.dest = command;
		SkipGroup();
	}
	else if(Token() == T_COMMAND)
		ReadCharSet();
}

void RTFParser::ReadCharSet()
{
	if(PassQ("ansi")) {}
	else if(PassQ("mac")) {}
	else if(PassQ("pc")) {}
	else if(PassQ("pca")) {}
	else if(PassQ("ansicpg")) {
		static const struct {
			int  ansicpg;
			byte charset;
		}
		charsets[] =
		{
			{ 1250, CHARSET_WIN1250 },
			{ 1251, CHARSET_WIN1251 },
			{ 1252, CHARSET_WIN1252 },
			{ 1253, CHARSET_WIN1253 },
			{ 1254, CHARSET_WIN1254 },
			{ 1255, CHARSET_WIN1255 },
			{ 1256, CHARSET_WIN1256 },
			{ 1257, CHARSET_WIN1257 },
		};
		for(int c = 0; c < __countof(charsets); c++)
			if(charsets[c].ansicpg == command_arg) {
				default_charset = state.charset = charsets[c].charset;
				break;
			}
	}
}

void RTFParser::ReadFaceTable()
{
	int fx = 0;
	while(!PassEndGroup()) {
		if(!PassGroup()) {
			пропусти();
			continue;
		}
		Face n;
		n.face = Шрифт::ARIAL;
		n.charset = default_charset;
		while(!PassEndGroup()) {
			if(PassCmd("f"))
				fx = command_arg;
			else if(PassCmd("fnil"))
				;
			else if(PassCmd("froman"))
				n.face = Шрифт::ROMAN;
			else if(PassCmd("fswiss"))
				n.face = Шрифт::ARIAL;
			else if(PassCmd("fmodern"))
				n.face = Шрифт::ARIAL;
			else if(PassCmd("ftech"))
#ifdef PLATFORM_WIN32
				n.face = Шрифт::SYMBOL;
#else
				n.face = Шрифт::ARIAL;
#endif
			else if(PassCmd("fcharset")) {
				switch(command_arg) {
					case 0: n.charset = CHARSET_WIN1252; break; // ANSI
					case 1: n.charset = default_charset; break; // дефолт
					case 2: n.charset = CHARSET_WIN1252; break; // Symbol
					case 3: break; // Invalid
					case 77: break; // Mac
					case 128: break; // Shift Jis
					case 129: break; // Hangul
					case 130: break; // Johab
					case 134: break; // GB2312
					case 136: break; // Big5
					case 161: n.charset = CHARSET_WIN1253; break; // Greek
					case 162: n.charset = CHARSET_WIN1254; break; // Turkish
					case 163: break; // Vietnamese
					case 177: n.charset = CHARSET_WIN1255; break; // Hebrew
					case 178: break; // Arabic
					case 179: break; // Arabic Traditional
					case 180: break; // Arabic user
					case 181: break; // Hebrew user
					case 186: break; // Baltic
					case 204: n.charset = CHARSET_WIN1251; break; // Russian
					case 222: break; // Thai
					case 238: n.charset = CHARSET_WIN1250; break; // Eastern European
					case 254: break; // PC 437
					case 255: n.charset = CHARSET_WIN1252; break; // OEM
				}
			}
/*			else if(PassText()) {
				Ткст s = изЮникода(text, charset);
				if(!s.пустой() && *s.последний() == ';')
					s.обрежь(s.дайДлину() - 1);
				if(!s.пустой())
					f = Шрифт::FindFaceNameIndex(s);
			}
			else if(PassGroup()) {
				int level = Level();
				if(PassCmd("falt") && PassText() && f < 0)
					f = Шрифт::FindFaceNameIndex(изЮникода(text, charset));
				SkipGroup(level);
			}*/ //Cxl 2005-11-29 - "Arial CE" makes mess here!
			else
				пропусти();
		}
		if(fx >= 0 && fx < MAX_FONTS) {
//			if(f < 0) // Cxl 2005-11-29
			if(default_font == fx) {
				plain_format.Face(n.face);
				plain_charset = n.charset;
			}
			Face dflt;
			dflt.face = Шрифт::ARIAL;
			dflt.charset = default_charset;
			face_table.по(fx++, dflt) = n;
		}
	}
}

void RTFParser::ReadColorTable()
{
	int r = Null, g = Null, b = Null;
	for(; !PassEndGroup(); пропусти())
		if(PassCmd("red"))
			r = command_arg;
		else if(PassCmd("green"))
			g = command_arg;
		else if(PassCmd("blue"))
			b = command_arg;
		else if(PassText())
		{
			Цвет c = Null;
			if(!пусто_ли(r) || !пусто_ли(g) || !пусто_ли(b))
				c = Цвет(Nvl(r, 0), Nvl(g, 0), Nvl(b, 0));
			color_table.добавь(c);
		}
}

void RTFParser::ReadMisc()
{
	if(PassQ("field"))
		ReadField();
	else if(PassQ("nonshppict"))
		SkipGroup();
	else if(PassQ("pict"))
		ReadPict();
	else if(PassQ("shpinst"))
		ReadShape();
	else if(PassQ("endash"))
		ReadChar(0x2013);
	else if(PassQ("emdash"))
		ReadChar(0x2014);
	else if(PassQ("tab"))
		читайТекст(ШТкст(9, 1));
	else if(PassQ("enspace"))
		читайТекст(ШТкст(" ")); // todo
	else if(PassQ("emspace"))
		читайТекст(ШТкст(" ")); // todo
	else if(PassQ("bullet"))
		ReadChar(0x2022);
	else if(PassQ("lquote"))
		ReadChar(0x2018);
	else if(PassQ("rquote"))
		ReadChar(0x2019);
	else if(PassQ("ldblquote"))
		ReadChar(0x201C);
	else if(PassQ("rdblquote"))
		ReadChar(0x201D);
}

void RTFParser::ReadField()
{
	bool ign_rslt = false;
	int level = Level();
	while(!PassEndGroup(level))
		if(PassGroup() && Level() == level + 1) {
			if(PassCmd("fldinst")) {
				ШТкст source;
				for(; !PassEndGroup(); пропусти())
					if(PassText())
						source.конкат(text);
				if(ReadField(изЮникода(source, state.charset)))
					ign_rslt = true;
				continue;
			}
			else if(PassCmd("fldrslt")) {
				if(!ign_rslt)
					ReadItemGroup();
			}
		}
		else
			пропусти();
}

bool RTFParser::ReadField(const char *p)
{
	Индекс<Ткст> symdef;
	while(*p)
		if((byte)*p <= ' ')
			p++;
		else if(*p == '\"')
			symdef.добавь(FromCString(p, &p));
		else {
			const char *b = p;
			while(*++p && *p != ' ')
				;
			symdef.добавь(Ткст(b, p));
		}
	if(symdef.пустой())
		return false;
	if(symdef[0] == "SYMBOL" && symdef.дайСчёт() >= 2 && цифра_ли(*symdef[1])) {
		int code = atoi(symdef[1]);
		int face = -1;
		int height = 0;
		int f = symdef.найди("\\f");
		if(f >= 0 && f + 1 < symdef.дайСчёт())
			face = Шрифт::FindFaceNameIndex(symdef[f + 1]);
		f = symdef.найди("\\s");
		if(f >= 0 && f + 1 < symdef.дайСчёт())
			height = PointDots(fround(2 * Atof(symdef[f + 1]))) >> 1;
		if(face < 0)
#ifdef PLATFORM_WIN32
			face = Шрифт::SYMBOL;
#else
			face = Шрифт::ARIAL;
#endif
		if(height <= 0 || height >= MAX_DOT_HEIGHT)
			height = state.charformat.дайВысоту();
		if(code >= 0 && code < 255) {
			state.charformat.Face(face).устВысоту(height);
			читайТекст(ШТкст(вЮникод(code, state.charset), 1));
			return true;
		}
	}
	return false;
}

void RTFParser::DefaultParaStyle()
{
	state.формат = pard_format;
	state.first_indent = state.left_margin = state.right_margin = 0;
//	state.cellformat = std_cell_format;
	state.in_table = false;
	state.itap = 1;
	state.nestprop = false;
	state.trgaph = 2;
	state.rowmargin = Прям(25, 25, 25, 25);
	state.cellmarginunits = state.rowmarginunits = Прям(0, 0, 0, 0);
	state.rowspacing = Прям(0, 0, 0, 0);
	state.rowspacingunits = Прям(0, 0, 0, 0);
	state.charset = plain_charset;
}

void RTFParser::ReadParaStyle()
{
	if(PassQ("par"))
		слей(true, state.itap);
	else if(PassQ("cell")) {
		слей(false, 1);
		if(!table_stack.пустой())
			table_stack[0].textcol++;
 	}
 	else if(PassQ("nestcell")) {
 		слей(false, state.itap);
 		if(state.itap <= table_stack.дайСчёт())
 			table_stack[state.itap - 1].textcol++;
 	}
	else if(PassQ("pard"))
		DefaultParaStyle();
	else if(PassQ("pntext"))
		SkipGroup();
	else if(PassQ("pn")) {
		SkipGroup();
		state.формат.bullet = RichPara::BULLET_ROUND;
	}
	else if(PassQ("pagebb"))
		state.формат.newpage = (command_arg != 0);
	else if(PassQ("ql"))
		state.формат.align = ALIGN_LEFT;
	else if(PassQ("qc"))
		state.формат.align = ALIGN_CENTER;
	else if(PassQ("qr"))
		state.формат.align = ALIGN_RIGHT;
	else if(PassQ("qj"))
		state.формат.align = ALIGN_JUSTIFY;
	else if(PassQ("fi"))
		state.first_indent = TwipDotsLim(command_arg);
	else if(PassQ("li"))
		state.left_margin = TwipDotsLim(command_arg);
	else if(PassQ("ri"))
		state.right_margin = TwipDotsLim(command_arg);
	else if(PassQ("sb"))
		state.формат.before = TwipDotsLim(command_arg);
	else if(PassQ("sa"))
		state.формат.after = TwipDotsLim(command_arg);
	else if(PassQ("widctlpar"))
		state.формат.orphan = true;
	else if(PassQ("nowidctlpar"))
		state.формат.orphan = false;
	else if(PassQ("tql"))
		tab_align = ALIGN_LEFT;
	else if(PassQ("tqc"))
		tab_align = ALIGN_CENTER;
	else if(PassQ("tqr"))
		tab_align = ALIGN_RIGHT;
	else if(PassQ("tqdec"))
		tab_align = ALIGN_RIGHT; // todo
	else if(PassQ("tldot"))
		tab_fill = 0;
	else if(PassQ("tlhyph"))
		tab_fill = 0;
	else if(PassQ("tlul"))
		tab_fill = 0;
	else if(PassQ("tlth"))
		tab_fill = 0;
	else if(PassQ("tleq"))
		tab_fill = 0;
	else if(PassQ("tx") || PassQ("tb")) { // todo: bar tab ?
		int pos = TwipDotSize(command_arg);
		RichPara::Вкладка& tab = state.формат.tab.добавь();
		tab.align = tab_align;
		tab.fillchar = tab_fill;
		tab.pos = pos;
		state.формат.сортируйТабы();
	}
	else if(PassQ("intbl"))
		state.in_table = true;
	else if(PassQ("itap")) {
		state.itap = minmax(command_arg, 1, 10);
		if(table_stack.дайСчёт() < state.itap)
			OpenTable(state.itap);
	}
}

void RTFParser::ReadCharStyle()
{
	if(PassQ("plain")) {
		state.charformat = plain_format;
		state.charset = plain_charset;
	}
	else if(PassQ("b"))
		state.charformat.Bold(command_arg != 0);
	else if(PassQ("i"))
		state.charformat.Italic(command_arg != 0);
	else if(PassQ("ul") || PassQ("uld") || PassQ("uldb")
	|| PassQ("uldash") || PassQ("uldashd") || PassQ("uldashdd")
	|| PassQ("ulth") || PassQ("ulw") || PassQ("ulwave"))
		state.charformat.Underline(command_arg != 0);
	else if(PassQ("ulnone"))
		state.charformat.Underline(false);
	else if(PassQ("strike") || PassQ("strikedl"))
		state.charformat.Strikeout(command_arg != 0);
	else if(PassQ("caps") || PassQ("scaps"))
		state.charformat.capitals = (command_arg != 0);
	else if(PassQ("super") || PassQ("up"))
		state.charformat.sscript = 1;
	else if(PassQ("sub") || PassQ("dn"))
		state.charformat.sscript = 2;
	else if(PassQ("nosupersub"))
		state.charformat.sscript = 0;
	else if(PassQ("f") && command_arg >= 0 && command_arg < face_table.дайСчёт()) {
		LLOG("font = " << command_arg << ", face = " << face_table[command_arg].face
			<< ", charset = " << face_table[command_arg].charset);
		state.charformat.Face(face_table[command_arg].face);
		state.charset = face_table[command_arg].charset;
	}
	else if(PassQ("fs"))
		state.charformat.устВысоту(PointDotHeight(command_arg));
	else if(PassQ("cf") && command_arg >= 0 && command_arg < color_table.дайСчёт())
		state.charformat.ink = Nvl(color_table[command_arg], чёрный);
	else if(PassQ("cb") && command_arg >= 0 && command_arg < color_table.дайСчёт())
		state.charformat.paper = color_table[command_arg];
	else if(PassQ("lang"))
	{} // state.language = ...
}

void RTFParser::ReadShape()
{
	int level = Level();
	while(!PassEndGroup(level))
		if(PassCmd("shppict")) {
			LLOG("* shppict");
			state.new_dest = false;
			ReadItemGroup();
		}
		else
			is_full = false;
}

void RTFParser::ReadPict()
{
	LLOG("* ReadPict");
	Размер log_size(1, 1), out_size(1, 1), scaling(100, 100);
	Прям crop(0, 0, 0, 0);
	enum BLIPTYPE { UNK_BLIP, WMF_BLIP, PNG_BLIP, JPEG_BLIP, DIB_BLIP };
	BLIPTYPE blip_type = UNK_BLIP;
	Ткст blip_data;
	char odd = 0;
	while(!PassEndGroup()) {
		if(PassText())
			blip_data.конкат(ReadBinHex(odd));
		else if(Token() == T_COMMAND) {
			if(PassQ("picw"))            log_size.cx = minmax<int>(command_arg, 0, 30000);
			else if(PassQ("pich"))       log_size.cy = minmax<int>(command_arg, 0, 30000);
			else if(PassQ("picwgoal"))   out_size.cx = TwipDotSize(command_arg);
			else if(PassQ("pichgoal"))   out_size.cy = TwipDotSize(command_arg);
			else if(PassQ("picscalex"))  scaling.cx = minmax<int>(command_arg, 1, 1000);
			else if(PassQ("picscaley"))  scaling.cy = minmax<int>(command_arg, 1, 1000);
			else if(PassQ("piccropl"))   crop.left   = TwipDotSize(command_arg);
			else if(PassQ("piccropt"))   crop.top    = TwipDotSize(command_arg);
			else if(PassQ("piccropr"))   crop.right  = TwipDotSize(command_arg);
			else if(PassQ("piccropb"))   crop.bottom = TwipDotSize(command_arg);
			else if(PassQ("emfblip"))    blip_type    = WMF_BLIP;
			else if(PassQ("pngblip"))    blip_type    = PNG_BLIP;
			else if(PassQ("jpegblip"))   blip_type    = JPEG_BLIP;
			else if(PassQ("wmetafile"))  blip_type    = WMF_BLIP;
			else if(PassQ("dibitmap"))   blip_type    = DIB_BLIP;
			else {
				LLOG("Command skip " << command);
				пропусти();
			}
		}
		else {
			LLOG("Non command skip");
			пропусти();
		}
	}
	Размер final_size = minmax(iscale(out_size, scaling, Размер(100, 100)), Размер(1, 1), Размер(30000, 30000));
	Размер drawing_size;
	DrawingDraw dd;
	RichObject object;
	LLOG("Pict формат " << (int)blip_type << ", data size: " << blip_data.дайСчёт());
	if(blip_data.пустой())
		return;
#ifdef GUI_WIN
#ifndef PLATFORM_WINCE
	if(blip_type == WMF_BLIP) {
		log_size = min(log_size, дайРазмСхождения(log_size, final_size));
		dd.создай(drawing_size = log_size);
		ВинМетаФайл wmf(blip_data);
		wmf.рисуй(dd, log_size);
		object = CreateDrawingObject(dd, out_size, final_size);
	}
	else
#endif
#endif
	if(blip_type == DIB_BLIP || blip_type == PNG_BLIP || blip_type == JPEG_BLIP) {
		Рисунок image = StreamRaster::LoadStringAny(blip_data);
		LLOG("Рисунок size: " << image.дайРазм());
		object = CreatePNGObject(image, out_size, final_size);
	}
	if(object) {
		LLOG("object (" << object.GetTypeName() << ", " << object.пиши().дайДлину() << " B), pixel size "
			<< object.GetPixelSize() << ", final size " << object.дайРазм());
		para.конкат(object, state.charformat);
	}
}

Ткст RTFParser::ReadBinHex(char& odd) const
{
	int t = odd;
	byte v = ctoi(odd);
	Ткст out;
	for(const wchar *s = text.старт(); *s; s++) {
		byte w = (byte)(*s >= '0' && *s <= '9' ? *s - '0'
			: *s >= 'A' && *s <= 'F' ? *s - 'A' + 10
			: *s >= 'a' && *s <= 'f' ? *s - 'a' + 10
			: 255);
		if(w < 16) {
			if(v >= 16) {
				t = *s;
				v = w;
			}
			else
			{
				out.конкат(16 * v + w);
				v = 255;
			}
		}
	}
	odd = (v < 16 ? t : 0);
	return out;
}

void RTFParser::OpenTable(int level)
{
	if(table_stack.дайСчёт() < level) {
		TableState& ts = table_stack.по(level - 1);
		ts.stylecol = 0;
//		state.cellformat = std_cell_format;
	}
}

void RTFParser::ReadCellBorder(int& width)
{
	if(Token() == T_COMMAND && !memcmp(command, "brdr", 4))
		is_full = false;
	if(PassCmd("brdrw"))
		width = TwipDots(command_arg);
}

RTFParser::Cell& RTFParser::CellAt(TableState& ts, int i)
{
	Массив<Cell>& top = ts.cells.верх();
	int p = top.дайСчёт();
	if(p <= i)
		top.устСчётР(i + 1);
	for(int n = p; n <= i; n++)
		top[n].info = CellInfoAt(n);
	return top[i];
}

RTFParser::ИнфОЯчейке& RTFParser::CellInfoAt(int i)
{
	return state.cellinfo.по(i, std_cell_info);
}

void RTFParser::SetCellMargin(Cell& out, int Прям::*mbr)
{
	if(out.info.cellmarginunits.*mbr == 0) {
		out.info.формат.margin.*mbr = state.trgaph;
		if(state.rowmarginunits.*mbr == 3)
			out.info.формат.margin.*mbr = state.rowmargin.*mbr;
		if(state.rowspacingunits.*mbr == 3)
			out.info.формат.margin.*mbr += state.rowspacing.*mbr;
	}
}

void RTFParser::ReadTableStyle()
{
	if(PassQ("nesttableprops")) {
		state.nestprop = true;
		return;
	}
	if(PassQ("nonesttables")) {
		SkipGroup();
		return;
	}
	int itap = (state.nestprop ? state.itap : 1);
	if(PassQ("trowd")) {
		OpenTable(itap);
		table_stack[itap - 1].stylecol = 0;
		return;
	}
	if(PassQ("row") && table_stack.дайСчёт() >= 1) {
		TableState& ts0 = table_stack[0];
		ts0.textcol = ts0.stylecol = 0;
		ts0.cells.добавь();
		return;
	}
	if(PassQ("nestrow") && table_stack.дайСчёт() >= state.itap) {
		TableState& ts = table_stack[state.itap - 1];
		ts.textcol = ts.stylecol = 0;
		ts.cells.добавь();
		return;
	}
	if(itap > table_stack.дайСчёт())
		return;
	TableState& ts = table_stack[itap - 1];
	if(PassQ("trgaph"))
		state.trgaph = TwipDotsLim(command_arg);
	else if(PassQ("trql")) {}
	else if(PassQ("trqr")) {}
	else if(PassQ("trqc")) {}
	else if(PassQ("trleft")) {
		ts.tableformat.lm = TwipDotsLim(command_arg);
	}
	else if(PassQ("trbrdrl")) {}
	else if(PassQ("trbrdrt")) {}
	else if(PassQ("trbrdrr")) {}
	else if(PassQ("trbrdrb")) {}
	else if(PassQ("trbrdrv")) {}
	else if(PassQ("trftsWidth")) {}
	else if(PassQ("trautofit")) {}
	else if(PassQ("trpaddl"))
		state.rowmargin.left = TwipDotsLim(command_arg);
	else if(PassQ("trpaddt"))
		state.rowmargin.top = TwipDotsLim(command_arg);
	else if(PassQ("trpaddr"))
		state.rowmargin.right = TwipDotsLim(command_arg);
	else if(PassQ("trpaddb"))
		state.rowmargin.bottom = TwipDotsLim(command_arg);
	else if(PassQ("trpaddfl"))
		state.rowmarginunits.left = command_arg;
	else if(PassQ("trpaddft"))
		state.rowmarginunits.top = command_arg;
	else if(PassQ("trpaddfr"))
		state.rowmarginunits.right = command_arg;
	else if(PassQ("trpaddfb"))
		state.rowmarginunits.bottom = command_arg;
	else if(PassQ("trspdl"))
		state.rowspacing.left = TwipDotsLim(command_arg);
	else if(PassQ("trspdt"))
		state.rowspacing.top = TwipDotsLim(command_arg);
	else if(PassQ("trspdr"))
		state.rowspacing.right = TwipDotsLim(command_arg);
	else if(PassQ("trspdb"))
		state.rowspacing.bottom = TwipDotsLim(command_arg);
	else if(PassQ("trspdfl"))
		state.rowspacingunits.left = command_arg;
	else if(PassQ("trspdft"))
		state.rowspacingunits.top = command_arg;
	else if(PassQ("trspdfr"))
		state.rowspacingunits.right = command_arg;
	else if(PassQ("trspdfb"))
		state.rowspacingunits.bottom = command_arg;
	else if(PassQ("clpadl"))
		CellInfoAt(ts.stylecol).формат.margin.left = TwipDotsLim(command_arg);
	else if(PassQ("clpadt"))
		CellInfoAt(ts.stylecol).формат.margin.top = TwipDotsLim(command_arg);
	else if(PassQ("clpadr"))
		CellInfoAt(ts.stylecol).формат.margin.right = TwipDotsLim(command_arg);
	else if(PassQ("clpadb"))
		CellInfoAt(ts.stylecol).формат.margin.bottom = TwipDotsLim(command_arg);
	else if(PassQ("clpadfl"))
		state.cellmarginunits.left = command_arg;
	else if(PassQ("clpadft"))
		state.cellmarginunits.top = command_arg;
	else if(PassQ("clpadfr"))
		state.cellmarginunits.right = command_arg;
	else if(PassQ("clpadfb"))
		state.cellmarginunits.bottom = command_arg;
	else if(PassQ("clbrdrl"))
		ReadCellBorder(CellInfoAt(ts.stylecol).формат.border.left);
	else if(PassQ("clbrdrt"))
		ReadCellBorder(CellInfoAt(ts.stylecol).формат.border.top);
	else if(PassQ("clbrdrr"))
		ReadCellBorder(CellInfoAt(ts.stylecol).формат.border.right);
	else if(PassQ("clbrdrb"))
		ReadCellBorder(CellInfoAt(ts.stylecol).формат.border.bottom);
	else if(PassQ("cltxlrtb")) {}
	else if(PassQ("clshdng"))
		CellInfoAt(ts.stylecol).shading = command_arg;
	else if(PassQ("clcbpat")) {
		if(command_arg >= 0 && command_arg < color_table.дайСчёт())
			CellInfoAt(ts.stylecol).формат.color = color_table[command_arg];
	}
	else if(PassQ("clvmrg"))
		CellAt(ts, ts.stylecol).merge = true;
	else if(PassQ("clvmgf"))
		CellAt(ts, ts.stylecol).merge_first = true;
	else if(PassQ("clftsWidth")) {}
	else if(PassQ("clwWidth")) {}
	else if(PassQ("cellx")) {
		int sx = ts.stylecol++;
		Cell& newcell = CellAt(ts, sx);
		newcell.info.end_dots = TwipDotsLim(command_arg);
		SetCellMargin(newcell, &Прям::left);
		SetCellMargin(newcell, &Прям::top);
		SetCellMargin(newcell, &Прям::right);
		SetCellMargin(newcell, &Прям::bottom);
		CellInfoAt(sx) = newcell.info;
		//CellFormat(sx) = std_cell_format;
	}
	else if(PassQ("clvertalt"))
		CellInfoAt(ts.stylecol).формат.align = ALIGN_TOP;
	else if(PassQ("clvertalc"))
		CellInfoAt(ts.stylecol).формат.align = ALIGN_CENTER;
	else if(PassQ("clvertalb"))
		CellInfoAt(ts.stylecol).формат.align = ALIGN_BOTTOM;
}

}
