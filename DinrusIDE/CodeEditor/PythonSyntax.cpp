#include "CodeEditor.h"

namespace РНЦП {

void PythonSyntax::Highlight(const wchar *s, const wchar *end, HighlightOutput& hls, РедакторКода *editor, int line, int64 pos)
{
	const HlStyle& ink = hl_style[INK_NORMAL];
	while(s < end) {
		int c = *s;
		dword pair = MAKELONG(s[0], s[1]);
		if(c == '#') {
			hls.помести(int(end - s), hl_style[INK_COMMENT]);
			return;
		}
		else
		if(findarg(pair, MAKELONG('0', 'x'), MAKELONG('0', 'X'), MAKELONG('0', 'b'), MAKELONG('0', 'B'),
		                 MAKELONG('0', 'o'), MAKELONG('0', 'O')) >= 0)
			s = HighlightHexBin(hls, s, 2, thousands_separator);
		else
		if(цифра_ли(c))
			s = HighlightNumber(hls, s, thousands_separator, false, false);
		else
		if(c == '\'' || c == '\"') {
			const wchar *s0 = s;
			s++;
			for(;;) {
				int c1 = *s;
				if(s >= end || c1 == c) {
					s++;
					hls.помести((int)(s - s0), hl_style[INK_CONST_STRING]);
					break;
				}
				s += 1 + (c1 == '\\' && s[1] == c);
			}
		}
		else
		if(IsAlpha(c) || c == '_') {
			static Индекс<Ткст> kws = { "False", "await", "else", "import", "pass", "None", "break",
			                             "except", "in", "raise", "True", "class", "finally", "is",
			                             "return", "and", "continue", "for", "lambda", "try", "as",
			                             "def", "from", "nonlocal", "while", "assert", "del", "global",
			                             "not", "with", "async", "elif", "if", "or", "yield" };
			static Индекс<Ткст> sws = { "self", "NotImplemented", "Ellipsis", "__debug__", "__file__", "__name__" };
			Ткст w;
			while(s < end && IsAlNum(*s) || *s == '_')
				w.конкат(*s++);
			hls.помести(w.дайСчёт(), kws.найди(w) >= 0 ? hl_style[INK_KEYWORD] :
			                      sws.найди(w) >= 0 ? hl_style[INK_UPP] :
			                      ink);
		}
		else
		if(c == '\\' && s[1]) {
			hls.помести(2, ink);
			s += 2;
		}
		else {
			bool hl = findarg(c, '[', ']', '(', ')', ':', '-', '=', '{', '}', '/', '<', '>', '*',
			                     '#', '@', '\\', '.') >= 0;
			hls.помести(1, hl ? hl_style[INK_OPERATOR] : ink);
			s++;
		}
	}
}

void PythonSyntax::IndentInsert(РедакторКода& editor, int chr, int count)
{
	if(chr == '\n') {
		while(count--) {
			ШТкст cursorLine = editor.дайШСтроку(editor.GetCursorLine());
			editor.InsertChar('\n', 1);
			
			Identation::Type idType = FindIdentationType(editor, cursorLine);
			char idChar = GetIdentationByType(idType);
			int mult = 1;
			if(idType == Identation::Space)
				mult = CalculateSpaceIndetationSize(editor);
			if(LineHasColon(cursorLine))
				editor.InsertChar(idChar, mult);
			editor.InsertChar(idChar, CalculateLineIndetations(cursorLine, idType));
		}
	}
	if(count > 0)
		editor.InsertChar(chr, count);
}

bool PythonSyntax::LineHasColon(const ШТкст& line)
{
	for(int i = line.дайДлину() - 1; i >= 0; i--) {
		if(line[i] == ':')
			return true;
	}
	return false;
}

int PythonSyntax::CalculateLineIndetations(const ШТкст& line, Identation::Type тип)
{
	int count = 0;
	for(int i = 0; i < line.дайДлину(); i++) {
		if(тип == Identation::Вкладка && line[i] == '\t')
			count++;
		else
		if(тип == Identation::Space && line[i] == ' ')
			count++;
		else
			break;
	}
	return count;
}

PythonSyntax::Identation::Type PythonSyntax::FindIdentationType(РедакторКода& editor, const ШТкст& line)
{
	Identation::Type тип = Identation::Unknown;
	if(line.начинаетсяС("\t"))
		тип = Identation::Вкладка;
	else
	if(line.начинаетсяС(" "))
		тип = Identation::Space;
	else {
		for(int i = 0; i < editor.дайСчётСтрок(); i++) {
			ШТкст cLine = editor.дайШСтроку(i);
			if(cLine.начинаетсяС("\t")) {
				тип = Identation::Вкладка;
				break;
			}
			else
			if(cLine.начинаетсяС(" ")) {
				тип = Identation::Space;
				break;
			}
		}
	}
	return тип;
}

int PythonSyntax::CalculateSpaceIndetationSize(РедакторКода& editor)
{
	int current = 0;
	for(int i = 0; i < editor.дайСчётСтрок(); i++) {
		ШТкст line = editor.дайШСтроку(i);
		for(int j = 0; j < line.дайДлину(); j++) {
			if(line[j] == ' ')
				current++;
			else
				break;
		}
		
		if(current > 0)
			break;
	}
	
	// TODO: 4 is magic number - try to find the way to get this number from ide constants
	return current > 0 ? current : 4;
}

char PythonSyntax::GetIdentationByType(Identation::Type тип)
{
	if(тип == Identation::Space)
		return ' ';
	return '\t';
}

}
