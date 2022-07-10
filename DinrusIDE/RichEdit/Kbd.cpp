#include "RichEdit.h"

namespace РНЦП {

bool HasNumbering(const RichPara::фмт& f)
{
	if(f.after_number.дайСчёт() || f.before_number.дайСчёт())
		return true;
	for(int i = 0; i < 8; i++)
		if(f.number[i] != RichPara::NUMBER_NONE)
			return true;
	return false;
}

bool RichEdit::RemoveBullet(bool backspace)
{
	RichPos p = text.GetRichPos(cursor);
	if((backspace ? p.posinpara : p.paralen) == 0 &&
	   (p.формат.bullet != RichPara::BULLET_NONE || HasNumbering(p.формат))) {
	    Стиль();
		RichText::FormatInfo nobullet;
		nobullet.paravalid = RichText::NUMBERING|RichText::BULLET;
		nobullet.charvalid = 0;
		ApplyFormatInfo(nobullet);
		return true;
	}
	return false;
}

bool RichEdit::Ключ(dword ключ, int count)
{
	useraction = true;
	NextUndo();
	if(CursorKey(ключ, count))
		return true;
	if(толькочтен_ли())
		return false;
	switch(ключ) {
	case K_CTRL_BACKSPACE:
		if(удалиВыделение(true)) return true;
		if(cursor > 0 && IsW(text[cursor - 1])) {
			int c = cursor;
			ReadFormat();
			MoveWordLeft(false);
			if(InvalidRange(cursor, c))
				return true;
			удали(cursor, c - cursor);
			objectpos = -1;
			FinishNF();
			return true;
		}
	case K_BACKSPACE:
	case K_SHIFT_BACKSPACE:
		if(удалиВыделение(true)) return true;
		if(RemoveBullet(true)) break;
		if(cursor <= 0 || RemoveSpecial(cursor, cursor - 1, true))
			return true;
		anchor = --cursor;
		begtabsel = false;
		if(cursor > 0) {
			RichPos p = text.GetRichPos(cursor - 1);
			if(p.формат.bullet != RichPara::BULLET_NONE || HasNumbering(p.формат)) {
				удали(cursor, 1, true);
				break;
			}
		}
		удали(cursor, 1);
		break;
	case K_DELETE:
		if(удалиВыделение()) return true;
		if(cursor < text.дайДлину() && !RemoveSpecial(cursor, cursor + 1, false))
			удали(cursor, 1, true);
		break;
	case K_INSERT:
		overwrite = !overwrite;
		поместиКаретку();
		break;
	case K_CTRL_DELETE:
		if(удалиВыделение()) return true;
		if(cursor < text.дайДлину()) {
			int c = cursor;
			if(IsW(text[c]))
				MoveWordRight(false);
			else
				cursor++;
			if(InvalidRange(cursor, c))
				return true;
			удали(c, cursor - c);
			cursor = anchor = c;
			begtabsel = false;
			break;
		}
		break;
	case K_CTRL_Z:
		Undo();
		return true;
	case K_SHIFT_CTRL_Z:
		Redo();
		return true;
	case K_ENTER: {
			if(singleline)
				return false;
			if(!удалиВыделение() && InsertLineSpecial())
				return true;
			if(RemoveBullet(false))
				break;
			RichText::FormatInfo f = formatinfo;
			InsertLine();
			formatinfo = f;
			ShowFormat();
			FinishNF();
		}
		return true;
	case K_CTRL_ENTER:
		{
			int c = дайКурсор(), l = дайДлину();
			RichObject object;
			while(c < l) {
				RichPos p = text.GetRichPos(c);
				object = p.object;
				if(object || p.chr > ' ')
					break;
				c++;
			}
			if(object) {
				NextUndo();
				objectpos = c;
				RichObject o = object;
				o.DefaultAction(context);
				if(o.GetSerialId() != object.GetSerialId())
					ReplaceObject(o);
				return true;
			}
		}
		return false;
	case K_F9:
		EvaluateFields();
		break;
	case K_F3:
		найди();
		break;
	case K_CTRL_H:
		Hyperlink();
		break;
	case K_CTRL_Q:
		IndexEntry();
		break;
	case K_ESCAPE:
		закройНайдиЗам();
		return false;
	case K_CTRL_C:
	case K_CTRL_INSERT:
		копируй();
		return true;
	case K_CTRL_X:
	case K_SHIFT_DELETE:
		вырежь();
		return true;
	case K_CTRL_V:
	case K_SHIFT_INSERT:
		Paste();
		return true;
	case K_SHIFT_CTRL_SPACE:
	case K_CTRL_SPACE:
		ключ = 160;
	case K_TAB:
		if(cursorp.table && cursorp.posintab == cursorp.tablen) {
			TableInsertRow();
			return true;
		}
		if(cursorp.table && cursorp.posincell == cursorp.celllen) {
			cursor = anchor = cursor + 1;
			begtabsel = false;
			break;
		}
	default:
		if(ключ >= K_ALT_0 && ключ <= K_ALT_9) {
			ApplyStyleKey(ключ - K_ALT_0);
			return true;
		}
		if(ключ >= (K_SHIFT|K_ALT_0) && ключ <= (K_SHIFT|K_ALT_9)) {
			ApplyStyleKey(ключ - (K_SHIFT|K_ALT_0) + 10);
			return true;
		}
		if(ключ == K_SHIFT_SPACE)
			ключ = ' ';
		if(ключ == 9 || ключ >= 32 && ключ < K_CHAR_LIM) {
			RichPara::фмт f;
			if(выделение_ли()) {
				f = text.GetRichPos(min(cursor, anchor)).формат;
				удалиВыделение();
			}
			else
				f = formatinfo;
			RichPara p;
			p.формат = f;
			p.конкат(ШТкст(ключ, count), f);
			RichText txt;
			txt.SetStyles(text.GetStyles());
			txt.конкат(p);
			if(overwrite) {
				RichPos p = text.GetRichPos(cursor);
				if(p.posinpara < p.paralen)
					удали(cursor, 1);
			}
			фильтруй(txt);
			вставь(cursor, txt, true);
			Move(cursor + count, false);
			break;
		}
		return false;
	}
	objectpos = -1;
	финиш();
	return true;
}

}
