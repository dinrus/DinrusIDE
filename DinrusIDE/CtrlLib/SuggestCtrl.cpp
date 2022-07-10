#include "CtrlLib.h"

namespace РНЦП {

void SuggestCtrl::режимОтмены()
{
	Cancel();
}

int SuggestCtrl::IsDelimiter(int c)
{
	if(delimiter_filter || delimiter_char) {
		if(delimiter_filter)
			c = (*delimiter_filter)(c);
		if(delimiter_char && delimiter_char != c)
			c = 0;
		return c;
	}
	return 0;
}

ШТкст SuggestCtrl::CF(const ШТкст& src)
{
	return compare_filter ? РНЦП::фильтруй(src, compare_filter) : src;
}

ШТкст SuggestCtrl::ReadLast(int& h)
{
	int l;
	дайВыделение(l, h);
	if(l != h)
		return Null;
	ШТкст text = дайТекст();
	ШТкст x;
	while(--l >= 0 && !IsDelimiter(text[l])) {
		h = l;
		x.вставь(0, text[l]);
	}
	return РНЦП::обрежьЛево(x);
}

void SuggestCtrl::выдели()
{
	if(list.курсор_ли()) {
		int h;
		ШТкст x = ReadLast(h);
		int q;
		if(дайВыделение(q, q))
			return;
		удали(h, q - h);
		x = list.дайКлюч();
		вставь(h, x);
		h += x.дайСчёт();
		устВыделение(h, h);
		Cancel();
		Action();
	}
}

bool SuggestCtrl::Ключ(dword ключ, int count)
{
	if(list.открыт()) {
		if(ключ == K_UP || ключ == K_PAGEUP || ключ == K_CTRL_PAGEDOWN) {
			if(list.курсор_ли())
				return list.Ключ(ключ, count);
			else {
				list.идиВКон();
				return true;
			}
		}
		if(ключ == K_DOWN || ключ == K_PAGEDOWN || ключ == K_CTRL_PAGEUP) {
			if(list.курсор_ли())
				return list.Ключ(ключ, count);
			else {
				list.идиВНач();
				return true;
			}
		}
	}
	if(ключ == K_ENTER && list.открыт() && list.курсор_ли()) {
		выдели();
		return true;
	}
	if(ключ == K_ESCAPE && list.открыт()) {
		закрой();
		return true;
	}
	int cc = IsDelimiter(ключ);
	if(cc)
		ключ = cc;
	if(EditString::Ключ(ключ, count)) {
		if(ключ >= 32 && ключ < K_CHAR_LIM || ключ == K_BACKSPACE || ключ == K_CTRL_SPACE) {
			int h;
			ШТкст x = CF(ReadLast(h));
			list.очисть();
			for(int i = 0; i < data.дайСчёт(); i++) {
				ШТкст h = CF(data[i]);
				if(just_start ? h.начинаетсяС(x) : h.найди(x) >= 0)
					list.добавь(data[i]);
			}
			if(list.дайСчёт() == 0) {
				Cancel();
				return true;
			}
			Прям cr = GetCaretRect(h) + GetScreenView().верхЛево();
			Прям wr = GetWorkArea();
			int c = droplines * Draw::GetStdFontCy();
			Прям r = Прям(cr.низЛево(), Размер(c, c));
			r.right = дайПрямЭкрана().right;
			if(r.bottom > wr.bottom) {
				r.top = cr.top - c;
				r.bottom = r.top + c;
			}
			if(r.right > wr.right) {
				r.left = cr.left - c;
				r.right = r.left + c;
			}
			list.устПрям(r);
			if(!list.открыт())
				list.Ктрл::PopUp(дайРодителя(), false, false, true);
		}
		else
			Cancel();
		return true;
	}
	return false;
}

void SuggestCtrl::Cancel()
{
	if(list.открыт())
		list.закрой();
}

void SuggestCtrl::расфокусирован()
{
	EditField::расфокусирован();
	Cancel();	
}

void SuggestCtrl::сфокусирован()
{
	EditField::сфокусирован();
	Move(дайДлину());
}

SuggestCtrl::SuggestCtrl()
{
	list.добавьКолонку();
	list.NoHeader().NoGrid();
	list.устФрейм(фреймЧёрный());
	list.MouseMoveCursor();
	list.ПриЛевКлике = THISBACK(выдели);
	list.NoWantFocus();
	delimiter_char = 0;
	delimiter_filter = NULL;
	droplines = 16;
	compare_filter = NULL;
	just_start = false;
}

}
