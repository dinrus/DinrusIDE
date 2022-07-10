#include "RichEdit.h"

namespace РНЦП {

void RichEdit::FinishNF()
{
	cursor = clamp(cursor, 0, text.дайДлину());
	anchor = clamp(anchor, 0, text.дайДлину());
	anchorp = text.GetRichPos(anchor);
	cursorp = text.GetRichPos(cursor);
	tablesel = 0;
	begtabsel = false;
	if(anchor != cursor) {
		RichPos p = text.GetRichPos(cursor, anchorp.level);
		if(anchorp.level == 0 || anchorp.level < cursorp.level) {
			cursor = text.AdjustCursor(anchor, cursor);
			cursorp = text.GetRichPos(cursor);
		}
		else
		if(p.table != anchorp.table) {
			if(anchor == 0 && anchorp.level == 1 && text.GetRichPos(anchor, 1).table == 1 && anchor < cursor) {
				while(cursor > 0 && cursorp.level) // selection must be at plain text
					cursorp = text.GetRichPos(--cursor);
				begtabsel = true;
				anchor = 0;
			}
			else {
				tablesel = anchorp.table;
				if(cursor < anchor) {
					cells.left = 0;
					cells.right = anchorp.cell.x;
					cells.top = 0;
					cells.bottom = anchorp.cell.y;
				}
				else {
					cells.left = anchorp.cell.x;
					cells.right = anchorp.tabsize.cx - 1;
					cells.top = anchorp.cell.y;
					cells.bottom = anchorp.tabsize.cy - 1;
				}
				text.AdjustTableSel(tablesel, cells);
			}
		}
		else
		if(p.cell != anchorp.cell) {
			tablesel = anchorp.table;
			cells.left = min(anchorp.cell.x, p.cell.x);
			cells.right = max(anchorp.cell.x, p.cell.x);
			cells.top = min(anchorp.cell.y, p.cell.y);
			cells.bottom = max(anchorp.cell.y, p.cell.y);
			text.AdjustTableSel(tablesel, cells);
		}
	}
	cursorc = text.дайКаретку(cursor, pagesz);
	Размер sz = дайРазм();
	SetSb();
	Прям r = поместиКаретку();
	if(r.top == GetPosY(text.дайКаретку(0, pagesz)))
		sb = 0;
	else
		sb.промотайДо(r.top, r.устВысоту());
	sb.промотайДо(r.bottom, 1); // if r.устВысоту is bigger than view height, make sure we rather see the bottom
	SetZsc();
	PageY top, bottom;
	int sell = min(cursor, anchor);
	int selh = max(cursor, anchor);
	if(tablesel)
		освежи();
	else
	if(text.GetInvalid(top, bottom, pagesz, sell, selh, osell, oselh)) {
		int y = GetPosY(top);
		освежи(0, y - sb, sz.cx, GetPosY(bottom) - y);
		y = GetPosY(text.дайВысоту(pagesz)) - sb;
		if(y < sz.cy)
			освежи(0, y, sz.cx, sz.cy - y);
	}
	osell = sell;
	oselh = selh;
	text.Validate();
	FixObjectRect();
	SetupRuler();
	if(modified) {
		if(useraction)
			Action();
	}
	useraction = modified = false;
}

void RichEdit::финиш()
{
	FinishNF();
	ReadFormat();
}

void RichEdit::MoveNG(int newpos, bool select)
{
	if(newpos < 0) newpos = 0;
	if(newpos >= text.дайДлину() + select) newpos = text.дайДлину() + select;
	закройНайдиЗам();
	cursor = newpos;
	if(!select) {
		anchor = cursor;
		begtabsel = false;
	}
	objectpos = -1;
	финиш();
	if(select)
		SetSelectionSource(Ткст().конкат() << "text/QTF;Rich устТекст фмт;text/rtf;application/rtf;"
		                   << ClipFmtsText());
	CancelMyPreedit();
}

void RichEdit::Move(int newpos, bool select)
{
	MoveNG(newpos, select);
	gx = cursorc.left;
}

void RichEdit::MoveUpDown(int dir, bool select, int pg)
{
	Прям page = pagesz;
	if(dir > 0 && cursor >= дайДлину() && select) {
		Move(дайДлину() + 1, true);
		return;
	}
	if(dir < 0 && cursor > дайДлину()) {
		Move(дайДлину(), select);
		return;
	}
	int c = text.GetVertMove(min(дайДлину(), cursor), gx, page, dir);
	if(c >= 0)
		MoveNG(c, select);
	else
		Move(dir < 0 ? 0 : дайДлину(), select);
	if(pg) {
		RichCaret pr = text.дайКаретку(cursor, pagesz);
		PageY py;
		py.page = pr.page;
		py.y = pr.top + dir * pg;
		while(py.y > pagesz.cy) {
			py.y -= pagesz.cy;
			py.page++;
		}
		while(py.y < 0) {
			py.y += pagesz.cy;
			py.page--;
		}
		MoveNG(text.дайПоз(pr.left, py, pagesz), select);
	}
}

void RichEdit::MovePageUpDown(int dir, bool select)
{
	PageRect p = text.дайКаретку(cursor, pagesz);
	int q = GetPosY(p) - sb;
	MoveUpDown(dir, select, 4 * GetTextRect().устВысоту() / GetZoom() / 5);
	p = text.дайКаретку(cursor, pagesz);
	sb = GetPosY(p) - q;
}

void RichEdit::MoveHomeEnd(int dir, bool select)
{
	int c = cursor;
	while(c + dir >= 0 && c + dir <= text.дайДлину()) {
		PageRect p1 = text.дайКаретку(c + dir, pagesz);
		if(p1.page != cursorc.page || p1.top != cursorc.top)
			break;
		c += dir;
	}
	Move(c, select);
}

bool RichEdit::IsW(int c)
{
	return буква_ли(c) || цифра_ли(c) || c == '_';
}

void RichEdit::MoveWordRight(bool select)
{
	Move((int)GetNextWord(cursor), select);
}

void RichEdit::MoveWordLeft(bool select)
{
	Move((int)GetPrevWord(cursor), select);
}

bool RichEdit::SelBeg(bool select)
{
	if(выделение_ли() && !select) {
		Move(min(cursor, anchor), false);
		return true;
	}
	return false;
}

bool RichEdit::SelEnd(bool select)
{
	if(выделение_ли() && !select) {
		Move(max(cursor, anchor), false);
		return true;
	}
	return false;
}

void RichEdit::SelCell(int dx, int dy)
{
	Move(text.GetCellPos(tablesel, minmax(cursorp.cell.y + dy, 0, cursorp.tabsize.cy - 1),
	                               minmax(cursorp.cell.x + dx, 0, cursorp.tabsize.cx - 1)).pos, true);
}

bool RichEdit::CursorKey(dword ключ, int count)
{
	bool select = ключ & K_SHIFT;
	if(ключ == K_CTRL_ADD) {
		ZoomView(1);
		return true;
	}
	if(ключ == K_CTRL_SUBTRACT) {
		ZoomView(-1);
		return true;
	}
	if(select && tablesel)
		switch(ключ & ~K_SHIFT) {
		case K_LEFT:
			SelCell(-1, 0);
			break;
		case K_RIGHT:
			SelCell(1, 0);
			break;
		case K_UP:
			SelCell(0, -1);
			break;
		case K_DOWN:
			SelCell(0, 1);
			break;
		default:
			return false;
		}
	else {
		switch(ключ) {
		case K_CTRL_UP:
			sb.предшСтрочка();
			break;
		case K_CTRL_DOWN:
			sb.следщСтрочка();
			break;
		default:
			switch(ключ & ~K_SHIFT) {
			case K_LEFT:
				if(!SelBeg(select))
					Move(cursor - 1, select);
				break;
			case K_RIGHT:
				if(!SelEnd(select))
					Move(cursor + 1, select);
				break;
			case K_UP:
				if(!SelBeg(select))
					MoveUpDown(-1, select);
				break;
			case K_DOWN:
				if(!SelEnd(select))
					MoveUpDown(1, select);
				break;
			case K_PAGEUP:
				if(!SelBeg(select))
					MovePageUpDown(-1, select);
				break;
			case K_PAGEDOWN:
				if(!SelEnd(select))
					MovePageUpDown(1, select);
				break;
			case K_END:
				MoveHomeEnd(1, select);
				break;
			case K_HOME:
				MoveHomeEnd(-1, select);
				break;
			case K_CTRL_LEFT:
				if(!SelBeg(select))
					MoveWordLeft(select);
				break;
			case K_CTRL_RIGHT:
				if(!SelEnd(select))
					MoveWordRight(select);
				break;
			case K_CTRL_HOME:
			case K_CTRL_PAGEUP:
				Move(0, select);
				break;
			case K_CTRL_END:
			case K_CTRL_PAGEDOWN:
				Move(text.дайДлину(), select);
				break;
			case K_CTRL_A:
				Move(0, false);
				Move(text.дайДлину(), true);
				break;
			default:
				return false;
			}
		}
	}
	синх();
	return true;
}

bool RichEdit::выделение_ли() const
{
	return anchor != cursor;
}

bool RichEdit::дайВыделение(int& l, int& h) const
{
	if(выделение_ли()) {
		l = min(anchor, cursor);
		h = max(anchor, cursor);
		return true;
	}
	l = h = cursor;
	return false;
}

bool RichEdit::InSelection(int& c) const
{
	int sell, selh;
	if(дайВыделение(sell, selh) && c >= sell && c < selh) {
		c = sell;
		return true;
	}
	return false;
}

void RichEdit::CancelSelection()
{
	if(выделение_ли()) {
		tablesel = 0;
		anchor = cursor;
		begtabsel = false;
		found = notfoundfw = false;
		закройНайдиЗам();
		финиш();
	}
}

bool RichEdit::удалиВыделение(bool back)
{
	if(выделение_ли()) {
		if(tablesel) {
			NextUndo();
			SaveTable(tablesel);
			text.ClearTable(tablesel, cells);
			Move(text.GetCellPos(tablesel, cells.top, cells.left).pos);
		}
		else {
			BegSelTabFix();
			int c = min(cursor, anchor);
			удали(c, абс(cursor - anchor), back);
			found = notfoundfw = false;
			закройНайдиЗам();
			Move(c);
		}
		return true;
	}
	return false;
}

void    RichEdit::GetWordAtCursorPos(int& pos, int& count)
{
	ШТкст w;
	int c = cursor;
	pos = count = 0;
	if(буква_ли(text[c])) {
		while(c > 0 && буква_ли(text[c - 1]))
			c--;
		pos = c;
		while(w.дайДлину() < 64 && буква_ли(text[c]))
			c++;
		count = c - pos;
	}
}

ШТкст RichEdit::GetWordAtCursor()
{
	int pos, count;
	GetWordAtCursorPos(pos, count);
	ШТкст w;
	for(int i = 0; i < count; i++)
		w.конкат(text[i + pos]);
	return w;
}

void RichEdit::AddUserDict()
{
	if(выделение_ли()) return;
	ШТкст w = GetWordAtCursor();
	if(w.пустой()) return;
	SpellerAdd(w, fixedlang ? fixedlang : formatinfo.language);
	text.ClearSpelling();
	освежи();
}

void RichEdit::идиК()
{
	устФокус();
	if(gototable.курсор_ли())
	{
		Move(gototable.дай(1), false);
		Move(gototable.дай(2), true);
	}
}

void RichEdit::GotoType(int тип, Ктрл& l)
{
	Вектор<RichValPos> f = text.GetValPos(pagesz, тип);
	gototable.очисть();
	for(int i = 0; i < f.дайСчёт(); i++) {
		const RichValPos& q = f[i];
		int endpos = q.pos;
		if(тип == RichText::INDEXENTRIES) {
			ШТкст ie = text.GetRichPos(endpos).формат.indexentry;
			int l = text.дайДлину();
			while(endpos < l) {
				RichPos p = text.GetRichPos(++endpos);
				if(p.формат.indexentry != ie || p.chr == '\n')
					break;
			}
		}
		gototable.добавь(q.data, q.pos, endpos);
	}
	if(gototable.дайСчёт())
		gototable.PopUp(&l);
}

void RichEdit::GotoLbl()
{
	GotoType(RichText::LABELS, label);
}

void RichEdit::GotoEntry()
{
	GotoType(RichText::INDEXENTRIES, indexentry);
}

bool RichEdit::GotoLabel(const Ткст& lbl)
{
	Вектор<RichValPos> f = text.GetValPos(pagesz, RichText::LABELS);
	for(int i = 0; i < f.дайСчёт(); i++)
		if(f[i].data == ШТкст(lbl)) {
			Move(f[i].pos);
			return true;
		}
	return false;
}

void RichEdit::BeginPara()
{
	RichPos pos = text.GetRichPos(anchor);
	Move(cursor - pos.posinpara);
}

void RichEdit::NextPara()
{
	RichPos pos = text.GetRichPos(anchor);
	Move(cursor - pos.posinpara + pos.paralen + 1);
}

void RichEdit::PrevPara()
{
	RichPos pos = text.GetRichPos(anchor);
	Move(cursor - pos.posinpara - 1);
	BeginPara();
}

}
