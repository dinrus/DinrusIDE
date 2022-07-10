#include "CodeEditor.h"

namespace РНЦП {

HighlightOutput::HighlightOutput(Вектор<СтрокРедакт::Highlight>& v)
:	v(v)
{
	pos = 0;
	def = v[0];
	def.chr = ' ';
	v.верх().chr = ' ';
}

HighlightOutput::~HighlightOutput()
{
	for(int i = 0; i < v.дайСчёт(); i++)
		if(v[i].chr == '_') {
			v[i].font.NoBold();
			v[i].font.NoItalic();
		}
}

void HighlightOutput::уст(int pos, int count, const HlStyle& ink)
{
	if(pos + count > v.дайСчёт())
		v.по(pos + count - 1, def);
	while(count-- > 0) {
		СтрокРедакт::Highlight& x = v[pos++];
		x.ink = ink.color;
		if(ink.bold)
			x.font.Bold();
		if(ink.italic)
			x.font.Italic();
		if(ink.underline)
			x.font.Underline();
	}
}

void HighlightOutput::устШрифт(int pos, int count, const HlStyle& f)
{
	if(pos + count > v.дайСчёт())
		v.по(pos + count - 1, def);
	while(count-- > 0) {
		СтрокРедакт::Highlight& x = v[pos++];
		if(f.bold)
			x.font.Bold();
		if(f.italic)
			x.font.Italic();
		if(f.underline)
			x.font.Underline();
	}
}

void HighlightOutput::SetPaper(int pos, int count, Цвет paper)
{
	if(pos + count > v.дайСчёт())
		v.по(pos + count - 1, def);
	while(count-- > 0)
		v[pos++].paper = paper;
}

void HighlightOutput::SetFlags(int pos, int count, word flags)
{
	if(pos + count > v.дайСчёт())
		v.по(pos + count - 1, def);
	while(count-- > 0)
		v[pos++].flags = flags;
}

void HighlightOutput::устЧернила(int pos, int count, Цвет ink)
{
	if(pos + count > v.дайСчёт())
		v.по(pos + count - 1, def);
	while(count-- > 0)
		v[pos++].ink = ink;
}

void HighlightOutput::помести(int count, const HlStyle& ink, Цвет paper)
{
	уст(pos, count, ink);
	SetPaper(pos, count, paper);
	pos += count;
}

void HighlightOutput::помести(int count, const HlStyle& ink, const HlStyle& paper)
{
	помести(count, ink, paper.color);
}

const wchar *HighlightOutput::CString(const wchar *p)
{
	помести(hl_style[INK_CONST_STRING]);
	const wchar delim = *p;
	p++;
	while(*p && *p != delim)
		if(*p == '\\') {
			const wchar *t = p++;
			if(*p == 'x' || *p == 'X') {
				p++;
				if(IsXDigit(*p))
					p++;
				if(IsXDigit(*p))
					p++;
				помести(int(p - t), hl_style[INK_CONST_STRINGOP]);
			}
			else
			if(*p >= '0' && *p <= '7') {
				p++;
				if(*p >= '0' && *p <= '7') p++;
				if(*p >= '0' && *p <= '7') p++;
				помести(int(p - t), hl_style[INK_CONST_STRINGOP]);
			}
			else {
				помести(hl_style[INK_CONST_STRINGOP]);
				if(*p) {
					помести(hl_style[INK_CONST_STRINGOP]);
					p++;
				}
			}
		}
		else
		if(*p == '%')
			if(p[1] == '%') {
				помести(2, hl_style[INK_CONST_STRING]);
				p += 2;
			}
			else {
				const wchar *t = p++;
				while(!IsAlpha(*p) && *p && *p != '`' && *p != '\"' && *p != '\'' && *p != '\\')
					p++;
				while(IsAlpha(*p) && *p)
					p++;
				помести(int(p - t), hl_style[INK_CONST_STRINGOP]);
			}
		else {
			помести(hl_style[INK_CONST_STRING]);
			p++;
		}
	if(*p == delim)	{
		помести(hl_style[INK_CONST_STRING]);
		p++;
	}
	return p;
}

}
