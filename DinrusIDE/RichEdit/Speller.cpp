#include "RichEdit.h"

namespace РНЦП {

void RichEdit::SpellerAdd(const ШТкст& w, int lang)
{
	РНЦП::SpellerAdd(w, lang);
}

int RichEdit::fixedlang;

Биты RichEdit::SpellParagraph(const RichPara& para)
{
	int len = para.дайДлину();
	Буфер<wchar> text(len);
	Буфер<int> lang(len);
	wchar *s = text;
	int *g = lang;
	for(int i = 0; i < para.дайСчёт(); i++) {
		const RichPara::Part& p = para[i];
		if(p.текст_ли()) {
			int l = p.text.дайДлину();
			memcpy(s, p.text, l * sizeof(wchar));
			Fill(g, g + l, fixedlang ? fixedlang : p.формат.language);
			s += l;
			g += l;
		}
		else {
			*s++ = 127;
			*g++ = 0;
		}
	}
	Биты e;
	s = text;
	wchar *end = text + len;
	while(s < end) {
		if(буква_ли(*s)) {
			const wchar *q = s;
			while(s < end && буква_ли(*s) || s + 1 < end && *s == '\'' && буква_ли(s[1]))
				s++;
			if(!SpellWord(q, int(s - q), lang[q - text]))
				e.устН(int(q - text), int(s - q));
		}
		else
			s++;
	}
	return e;
}

}
