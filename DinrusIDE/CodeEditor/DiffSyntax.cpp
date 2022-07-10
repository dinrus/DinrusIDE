#include "CodeEditor.h"

namespace РНЦП {

DiffSyntax::DiffSyntax()
{
	hout = NULL;
}

void DiffSyntax::Highlight(const wchar *start, const wchar *end, HighlightOutput& hls, РедакторКода *editor, int line, int64 pos)
{
	hout = &hls;
	Do(start, end, editor, line, editor ? editor->GetTabSize() : 4, pos);
	hout = NULL;
}

void DiffSyntax::помести(int ink, int n, int paper)
{
	if(hout)
		hout->помести(n, hl_style[ink], hl_style[paper]);
}

void DiffSyntax::Do(const wchar *ln, const wchar *end, РедакторКода *editor, int line, int tabsize, int64 pos)
{
	const int lineLength = FindTheNumberOfCharsToLineEnd(ln, end);
	
	if(IsPattern(ln, end, "---") || IsPattern(ln, end, "***")) {
		const wchar *lnPrefix = end - 4;
		
		if(lnPrefix >= ln) {
			if (IsPattern(lnPrefix, end, "----") || IsPattern(lnPrefix, end, "****")) {
				помести(INK_DIFF_HEADER, lineLength);
				return;
			}
		}
		
		помести(INK_DIFF_FILE_INFO, lineLength);
	}
	else
	if(IsPattern(ln, end, "====") || IsPattern(ln, end, "Индекс"))
		помести(INK_DIFF_FILE_INFO, lineLength);
	else
	if(IsPattern(ln, end, "+++"))
		помести(INK_DIFF_HEADER, lineLength);
	else
	if(IsPattern(ln, end, "@@"))
		помести(INK_DIFF_HEADER, lineLength);
	else
	if(цифра_ли(*ln))
		помести(INK_DIFF_HEADER, lineLength);
	else
	if(*ln == '+' || *ln == '>')
		помести(INK_DIFF_ADDED, lineLength);
	else
	if(*ln == '-' || *ln == '<' || *ln == '!')
		помести(INK_DIFF_REMOVED, lineLength);
	else
	if(*ln == ' ')
		помести(INK_NORMAL, lineLength);
	else
		помести(INK_DIFF_COMMENT, lineLength);
}

int DiffSyntax::FindTheNumberOfCharsToLineEnd(const wchar *current, const wchar *end) const
{
	int counter = 0;
	
	while(current < end) {
		current++;
		counter++;
	}
	
	return counter;
}

bool DiffSyntax::IsPattern(const wchar *current, const wchar *end, Ткст pattern) const
{
	bool containing = true;
	
	int i = 0;
	while((current < end) && (i < pattern.дайСчёт())) {
		if((int)впроп(*current) != впроп(pattern[i])) {
			containing = false;
			break;
		}
		
		current++;
		i++;
	}
	
	return (containing && (i == pattern.дайСчёт()));
}

}
