#include "CodeEditor.h"

namespace РНЦП {

HlStyle HighlightSetup::hl_style[HL_COUNT];
byte    HighlightSetup::hilite_scope = 0;
byte    HighlightSetup::hilite_ifdef = 1;
byte    HighlightSetup::hilite_bracket = 1;
bool    HighlightSetup::thousands_separator = true;

bool    HighlightSetup::indent_spaces = false;
bool    HighlightSetup::no_parenthesis_indent = false;
int     HighlightSetup::indent_amount = 4;

#define HL_COLOR(x, a, b)    #x,
static const char *s_hl_color[] = {
#include "hl_color.i"
};
#undef  HL_COLOR

#define HL_COLOR(a, x, b)    x,
static const char *s_hl_name[] = {
#include "hl_color.i"
};
#undef  HL_COLOR

#define HL_COLOR(a, b, x)    x,
static bool s_hl_font[] = {
#include "hl_color.i"
};
#undef  HL_COLOR

const HlStyle& HighlightSetup::GetHlStyle(int i)
{
	return hl_style[i];
}

const char *HighlightSetup::GetHlName(int i)
{
	return s_hl_name[i];
}

bool HighlightSetup::HasHlFont(int i)
{
	return s_hl_font[i];
}

void  HighlightSetup::SetHlStyle(int i, Цвет c, bool bold, bool italic, bool underline)
{
	HlStyle& st = hl_style[i];
	st.color = c;
	st.bold = bold;
	st.italic = italic;
	st.underline = underline;
}

void HighlightSetup::LoadHlStyles(const char *s)
{
	СиПарсер p(s);
	try {
		while(!p.кф_ли()) {
			Ткст id = p.читайИд();
			Цвет c = ReadColor(p);
			bool bold = false;
			bool italic = false;
			bool underline = false;
			for(;;)
				if(p.ид("bold"))
					bold = true;
				else
				if(p.ид("italic"))
					italic = true;
				else
				if(p.ид("underline"))
					underline = true;
				else
					break;
			for(int i = 0; i < HL_COUNT; i++)
				if(id == s_hl_color[i]) {
					SetHlStyle(i, c, bold, italic, underline);
					break;
				}
			p.передайСим(';');
		}
	}
	catch(СиПарсер::Ошибка) {
		DefaultHlStyles();
	}
}

Ткст HighlightSetup::StoreHlStyles()
{
	Ткст r;
	for(int i = 0; i < HL_COUNT; i++) {
		const HlStyle& s = GetHlStyle(i);
		r << фмт("%-25s", s_hl_color[i]) << ' ' << FormatColor(s.color);
		if(s.bold)
			r << " bold";
		if(s.italic)
			r << " italic";
		if(s.underline)
			r << " underline";
		r << ";\r\n";
	}
	return r;
}

void HighlightSetup::тёмнаяТема()
{
	SetHlStyle(INK_NORMAL,                белый);
	SetHlStyle(PAPER_NORMAL,              Цвет(1, 1, 1));
	SetHlStyle(INK_SELECTED,              Цвет(1, 1, 1));
	SetHlStyle(PAPER_SELECTED,            Цвет(97, 217, 255));
	SetHlStyle(INK_DISABLED,              Цвет(184, 184, 184));
	SetHlStyle(PAPER_READONLY,            Цвет(24, 24, 24));
	SetHlStyle(INK_COMMENT,               Цвет(173, 255, 173), false, true);
	SetHlStyle(INK_COMMENT_WORD,          Цвет(235, 235, 255), true, true);
	SetHlStyle(PAPER_COMMENT_WORD,        Цвет(99, 99, 0));
	SetHlStyle(INK_CONST_STRING,          Цвет(248, 162, 162));
	SetHlStyle(INK_CONST_STRINGOP,        Цвет(214, 214, 255));
	SetHlStyle(INK_RAW_STRING,            Цвет(235, 235, 255));
	SetHlStyle(INK_OPERATOR,              Цвет(214, 214, 255));
	SetHlStyle(INK_KEYWORD,               Цвет(214, 214, 255), true);
	SetHlStyle(INK_UPP,                   Цвет(108, 236, 236));
	SetHlStyle(PAPER_LNG,                 Цвет(13, 13, 0));
	SetHlStyle(INK_ERROR,                 Цвет(255, 185, 185));
	SetHlStyle(INK_PAR0,                  белый);
	SetHlStyle(INK_PAR1,                  Цвет(173, 255, 173));
	SetHlStyle(INK_PAR2,                  Цвет(255, 149, 255));
	SetHlStyle(INK_PAR3,                  Цвет(214, 214, 86));
	SetHlStyle(INK_CONST_INT,             Цвет(255, 137, 137));
	SetHlStyle(INK_CONST_FLOAT,           Цвет(255, 149, 255));
	SetHlStyle(INK_CONST_HEX,             Цвет(235, 235, 255));
	SetHlStyle(INK_CONST_OCT,             Цвет(235, 235, 255));
	SetHlStyle(PAPER_BRACKET0,            Цвет(26, 26, 0));
	SetHlStyle(PAPER_BRACKET,             Цвет(99, 99, 0), true);
	SetHlStyle(PAPER_BLOCK1,              Цвет(16, 16, 31));
	SetHlStyle(PAPER_BLOCK2,              Цвет(5, 20, 5));
	SetHlStyle(PAPER_BLOCK3,              Цвет(3, 3, 0));
	SetHlStyle(PAPER_BLOCK4,              Цвет(20, 5, 20));
	SetHlStyle(INK_MACRO,                 Цвет(255, 149, 255));
	SetHlStyle(PAPER_MACRO,               Цвет(11, 11, 0));
	SetHlStyle(PAPER_IFDEF,               Цвет(0, 18, 18));
	SetHlStyle(INK_IFDEF,                 Цвет(131, 131, 131));
	SetHlStyle(INK_UPPER,                 белый);
	SetHlStyle(INK_SQLBASE,               белый);
	SetHlStyle(INK_SQLFUNC,               белый);
	SetHlStyle(INK_SQLBOOL,               белый);
	SetHlStyle(INK_UPPMACROS,             Цвет(108, 236, 236));
	SetHlStyle(INK_UPPLOGS,               Цвет(173, 255, 173));
	SetHlStyle(INK_DIFF_FILE_INFO,        белый, true);
	SetHlStyle(INK_DIFF_HEADER,           Цвет(103, 202, 255));
	SetHlStyle(INK_DIFF_ADDED,            Цвет(182, 196, 255));
	SetHlStyle(INK_DIFF_REMOVED,          Цвет(255, 185, 185));
	SetHlStyle(INK_DIFF_COMMENT,          Цвет(173, 255, 173));
	SetHlStyle(PAPER_SELWORD,             Цвет(99, 99, 0));
	SetHlStyle(PAPER_ERROR,               Цвет(90, 40, 40));
	SetHlStyle(PAPER_WARNING,             Цвет(21, 21, 0));
	SetHlStyle(SHOW_LINE,                 Цвет(27, 75, 26));
	SetHlStyle(SHOW_COLUMN,               Цвет(56, 33, 29));
	SetHlStyle(WHITESPACE,                Цвет(68, 128, 176));
	SetHlStyle(WARN_WHITESPACE,           Цвет(206, 141, 141));
}

void HighlightSetup::WhiteTheme()
{
	SetHlStyle(INK_COMMENT, зелёный, false, true);
	SetHlStyle(PAPER_COMMENT_WORD, жёлтый, false, false);
	SetHlStyle(INK_COMMENT_WORD, синий, true, true);
	SetHlStyle(INK_CONST_STRING, красный);
	SetHlStyle(INK_RAW_STRING, синий);
	SetHlStyle(INK_CONST_STRINGOP, светлоСиний);
	SetHlStyle(INK_CONST_INT, красный);
	SetHlStyle(INK_CONST_FLOAT, магента);
	SetHlStyle(INK_CONST_HEX, синий);
	SetHlStyle(INK_CONST_OCT, синий);

	SetHlStyle(INK_OPERATOR, светлоСиний);
	SetHlStyle(INK_KEYWORD, светлоСиний, true);
	SetHlStyle(INK_UPP, цыан);
	SetHlStyle(PAPER_LNG, Цвет(255, 255, 224));
	SetHlStyle(INK_ERROR, светлоКрасный);
	SetHlStyle(INK_PAR0, чёрный);
	SetHlStyle(INK_PAR1, зелёный);
	SetHlStyle(INK_PAR2, магента);
	SetHlStyle(INK_PAR3, коричневый);

	SetHlStyle(INK_UPPER, чёрный);
	SetHlStyle(INK_SQLBASE, чёрный);
	SetHlStyle(INK_SQLFUNC, чёрный);
	SetHlStyle(INK_SQLBOOL, чёрный);
	SetHlStyle(INK_UPPMACROS, цыан);
	SetHlStyle(INK_UPPLOGS, зелёный);
	
	SetHlStyle(INK_DIFF_FILE_INFO, чёрный, true);
	SetHlStyle(INK_DIFF_HEADER, Цвет(28, 127, 200));
	SetHlStyle(INK_DIFF_ADDED, Цвет(28, 42, 255));
	SetHlStyle(INK_DIFF_REMOVED, Цвет(255, 0, 0));
	SetHlStyle(INK_DIFF_COMMENT, зелёный);

	SetHlStyle(PAPER_BLOCK1, смешай(светлоСиний, белый, 240));
	SetHlStyle(PAPER_BLOCK2, смешай(светлоЗелёный, белый, 240));
	SetHlStyle(PAPER_BLOCK3, смешай(светлоЖёлтый, белый, 240));
	SetHlStyle(PAPER_BLOCK4, смешай(светлоМагента, белый, 240));

	SetHlStyle(INK_MACRO, магента);
	SetHlStyle(PAPER_MACRO, Цвет(255, 255, 230));
	SetHlStyle(PAPER_IFDEF, Цвет(230, 255, 255));
	SetHlStyle(INK_IFDEF, Цвет(170, 170, 170));

	SetHlStyle(PAPER_BRACKET0, светлоЖёлтый);
	SetHlStyle(PAPER_BRACKET, жёлтый, true);

	SetHlStyle(INK_NORMAL, SColorText);
	SetHlStyle(INK_DISABLED, SColorDisabled);
	SetHlStyle(INK_SELECTED, SColorHighlightText);
	SetHlStyle(PAPER_NORMAL, SColorPaper);
	SetHlStyle(PAPER_READONLY, SColorFace);
	SetHlStyle(PAPER_SELECTED, SColorHighlight);
	
	SetHlStyle(PAPER_SELWORD, жёлтый);

	SetHlStyle(PAPER_ERROR, смешай(белый(), светлоКрасный(), 50));
	SetHlStyle(PAPER_WARNING, смешай(белый(), жёлтый(), 50));

	SetHlStyle(SHOW_LINE, Цвет(199, 247, 198));
	SetHlStyle(SHOW_COLUMN, Цвет(247, 224, 220));
	
	SetHlStyle(WHITESPACE, смешай(SColorLight, SColorHighlight));
	SetHlStyle(WARN_WHITESPACE, смешай(SColorLight, SRed));
}

void HighlightSetup::DefaultHlStyles()
{
	if(IsDarkTheme())
		тёмнаяТема();
	else
		WhiteTheme();
}

}
