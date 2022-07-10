#include <RichEdit/RichEdit.h>

namespace РНЦП {

void RichEdit::FindReplaceAddHistory() {
	if(!Ткст(~findreplace.find).пустой())
		findreplace.find.AddHistory();
	if(!Ткст(~findreplace.replace).пустой())
		findreplace.replace.AddHistory();
}

void RichEdit::закройНайдиЗам()
{
	if(!persistent_findreplace && findreplace.открыт())
		findreplace.закрой();
}

bool compare3(const wchar *s, const wchar *a, const wchar *b, int len)
{
	const wchar *e = s + len;
	while(s < e) {
		if(*s != *a && *s != *b)
			return false;
		s++;
		a++;
		b++;
	}
	return true;
}

struct RichFindIterator : RichText::Обходчик {
	int cursor;
	int fpos;
	ШТкст upperw, lowerw;
	bool ww;
	int  len;

	virtual bool operator()(int pos, const RichPara& para)
	{
		ШТкст ptext = para.дайТекст();
		if(pos + ptext.дайДлину() > cursor && ptext.дайДлину() >= len) {
			const wchar *q = ptext;
			const wchar *e = ptext.стоп() - len;
			if(cursor >= pos)
				q += cursor - pos;
			while(q <= e) {
				if(compare3(q, upperw, lowerw, len) &&
				   (!ww || (q + len == e || !буква_ли(q[len])) &&
				           (q == ptext || !буква_ли(q[-1])))) {
					fpos = int(q - ~ptext + pos);
					return true;
				}
				q++;
			}
		}
		return false;
	}
};

int  RichEdit::FindPos()
{
	RichFindIterator fi;
	ШТкст w = findreplace.find.дайТекст();
	if(findreplace.ignorecase) {
		fi.upperw = взаг(w);
		fi.lowerw = впроп(w);
	}
	else
		fi.upperw = fi.lowerw = w;
	fi.len = w.дайДлину();
	fi.ww = findreplace.wholeword;
	if(w.дайДлину()) {
		fi.cursor = cursor;
		if(text.Iterate(fi))
			return fi.fpos;
	}
	return -1;
}

void RichEdit::найди()
{
	CancelSelection();
	FindReplaceAddHistory();
	if(notfoundfw)
		Move(0, false);
	found = notfoundfw = false;
	int pos = FindPos();
	if(pos >= 0) {
		anchor = pos;
		cursor = pos + findreplace.find.дайТекст().дайДлину();
		финиш();
		found = true;
		Размер sz = findreplace.дайРазм();
		Прям sw = GetScreenView();
		Прям r = sw.центрПрям(sz);
		Прям cr = GetCaretRect();
		if(cr.top < sz.cy + 2 * cr.устВысоту()) {
			r.bottom = sw.bottom - 8;
			r.top = r.bottom - sz.cy;
		}
		else {
			r.top = sw.top + 24;
			r.bottom = r.top + sz.cy;
		}
		findreplace.устПрям(r);
		if(!findreplace.открыт()) {
			findreplace.открой();
		}
		устФокус();
	}
	else {
		CancelSelection();
		закройНайдиЗам();
		notfoundfw = true;
	}
}

RichText RichEdit::ReplaceText()
{
	RichText clip;
	RichPara p;
	formatinfo.ApplyTo(p.формат);
	p.part.добавь();
	formatinfo.ApplyTo(p[0].формат);
	p.part.верх().text = findreplace.replace.дайТекст();
	clip.конкат(p);
	return clip;
}

void RichEdit::замени()
{
	NextUndo();
	if(выделение_ли() && found) {
		FindReplaceAddHistory();
		int c = min(cursor, anchor);
		удали(c, абс(cursor - anchor));
		anchor = cursor = c;
		вставь(cursor, ReplaceText(), false);
		cursor += findreplace.replace.дайТекст().дайДлину();
		anchor = cursor;
		финиш();
		найди();
	}
}

void RichEdit::OpenFindReplace()
{
	NextUndo();
	if(!findreplace.открыт()) {
		Размер sz = findreplace.дайРазм();
		findreplace.устПрям(GetScreenView().центрПрям(sz));
		int l, h;
		if(дайВыделение(l, h)) {
			findreplace.amend.скрой();
			findreplace.ok.устНадпись(t_("замени"));
			findreplace.Титул(t_("замени in selection"));
			findreplace.cancel <<= findreplace.Breaker(IDCANCEL);
			findreplace.ok <<= findreplace.Breaker(IDOK);
			if(findreplace.выполни() == IDOK) {
				int len = findreplace.find.дайТекст().дайДлину();
				int rlen = findreplace.replace.дайТекст().дайДлину();
				RichText rtext = ReplaceText();
				cursor = l;
				for(;;) {
					int pos = FindPos();
					if(pos < 0 || pos + len >= h)
						break;
					выдели(pos, len);
					удали(pos, len);
					вставь(pos, ReplaceText(), false);
					cursor += pos + rlen;
					h += rlen - len;
				}
				CancelSelection();
				Move(h, false);
			}
			FindReplaceAddHistory();
			findreplace.amend.покажи();
			findreplace.ok.устНадпись(t_("найди"));
			findreplace.Титул(t_("найди / замени"));
			findreplace.cancel <<= callback(&findreplace, &ТопОкно::закрой);
			findreplace.ok <<= THISBACK(найди);
		}
		else {
			findreplace.открой();
			findreplace.find.устФокус();
		}
	}
}

}
