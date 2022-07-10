#include "CppBase.h"
#include "Internal.h"

namespace РНЦП {

bool IsCPPFile(const Ткст& path)
{
	return findarg(впроп(дайРасшф(path)) , ".c", ".cpp", ".cc" , ".cxx", ".icpp") >= 0;
}

bool IsHFile(const Ткст& path)
{
	return findarg(впроп(дайРасшф(path)) , ".h", ".hpp", ".hxx" , ".hh") >= 0;
}

void SetSpaces(Ткст& l, int pos, int count)
{
	ТкстБуф s(l);
	memset(~s + pos, ' ', count);
	l = s;
}

const char *SkipString(const char *s)
{
	СиПарсер p(s);
	try {
		p.читай1Ткст(*s);
	}
	catch(СиПарсер::Ошибка) {}
	s = p.дайУк();
	while((byte)*(s - 1) <= ' ')
		s--;
	return s;
}

void RemoveComments(Ткст& l, bool& incomment)
{
	int q = -1;
	int w = -1;
	if(incomment)
		q = w = 0;
	else {
		const char *s = l;
		while(*s) {
			if(*s == '\"')
				s = SkipString(s);
			else
			if(s[0] == '/' && s[1] == '/') {
				q = int(s - ~l);
				SetSpaces(l, q, l.дайСчёт() - q);
				return;
			}
			else
			if(s[0] == '/' && s[1] == '*') {
				q = int(s - ~l);
				break;
			}
			else
				s++;
		}
		if(q >= 0)
			w = q + 2;
	}
	while(q >= 0) {
		int eq = l.найди("*/", w);
		if(eq < 0) {
			incomment = true;
			SetSpaces(l, q, l.дайСчёт() - q);
			return;
		}
		SetSpaces(l, q, eq + 2 - q);
		incomment = false;
		q = l.найди("/*");
		w = q + 2;
	}
}

};