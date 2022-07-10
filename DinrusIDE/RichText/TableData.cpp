#include "RichText.h"

namespace РНЦП {

void RichTable::инивалидируй()
{
	clayout.py.page = -1;
	length = tabcount = -1;
}

void RichTable::InvalidateRefresh(int i, int j)
{
	if(i < формат.header)
		r_row = -1;
	else
	if(r_row != i || r_column != j) {
		if(r_row == -2 && clayout.sz == дайРазм()) {
			r_row = i;
			r_column = j;
			r_py = clayout.py;
			r_pyy = clayout[min(GetRows() - 1, i + cell[i][j].vspan)].pyy;
			r_first_page = clayout.first_page;
			r_next_page = clayout.next_page;
		}
		else
			r_row = -1;
	}
	инивалидируй();
}

int  RichTable::GetInvalid(PageY& top, PageY& bottom, RichContext rc) const
{
	if(r_row == -2)
		return -1;
	const TabLayout& tab = Realize(rc);
	Прям first_page, next_page;
	Reduce(rc, first_page, next_page);
	if(r_row >= 0 && r_first_page == first_page && r_next_page == next_page
	   && r_py == rc.py && tab[min(GetRows() - 1, r_row + cell[r_row][r_column].vspan)].pyy == r_pyy) {
		const PaintRow& pr = tab[r_row];
		const RichCell& cl = cell[r_row][r_column];
		top = pr.py;
		bottom = tab[min(cell.дайСчёт() - 1, r_row + cl.vspan)].pyy;
		return 0;
	}
	return 1;
}

void RichTable::Normalize0()
{
	int nx = формат.column.дайСчёт();
	int ny = cell.дайСчёт();

	for(int i = 0; i < ny; i++)
		cell[i].устСчёт(nx);

	инивалидируй();
	r_py.page = -1;
	r_row = -1;

	ci.размести(ny);
	for(int i = 0; i < ny; i++)
		ci[i].размести(nx);
	for(int i = 0; i < ny; i++)
		for(int j = 0; j < nx; j++)
			if(ci[i][j].valid) {
				RichCell& c = cell[i][j];
				int vs = i < формат.header ? min(формат.header - 1, i + c.vspan) : min(ny - 1, i + c.vspan);
				int hs = min(nx - 1, j + c.hspan);
				c.vspan = vs - i;
				c.hspan = hs - j;
				for(int ii = i; ii <= vs; ii++)
					for(int jj = j; jj <= hs; jj++) {
						if((i != ii || j != jj) && ii < ny && jj < nx) {
							ИнфОЯчейке& f = ci[ii][jj];
							f.valid = false;
							cell[ii][jj].очисть();
							f.master = Точка(j, i);
							cell[ii][jj].hspan = hs - jj;
						}
					}
			}
}

bool RichTable::IsRowEmpty(int row)
{
	for(int i = 0; i < дайКолонки(); i++)
		if(ci[row][i].valid && cell[row][i].vspan == 0)
			return false;
	return true;
}

bool RichTable::IsColumnEmpty(int column)
{
	for(int i = 0; i < GetRows(); i++)
		if(ci[i][column].valid)
			return false;
	return true;
}

void RichTable::RemoveRow0(int rowi)
{
	if(rowi < формат.header)
		формат.header--;
	for(int i = 0; i < дайКолонки(); i++) {
		ИнфОЯчейке& cf = ci[rowi][i];
		if(!cf.valid && cf.master.x == i)
			cell[cf.master.y][cf.master.x].vspan--;
	}
	cell.удали(rowi);
}

void RichTable::RemoveColumn0(int column)
{
	for(int i = 0; i < cell.дайСчёт(); i++) {
		ИнфОЯчейке& cf = ci[i][column];
		if(!cf.valid && cf.master.y == i)
			cell[cf.master.y][cf.master.x].hspan--;
		cell[i].удали(column);
	}
	формат.column.удали(column);
}

void RichTable::нормализуй()
{
	Normalize0();
	int i = 1;
	while(i < GetRows())
		if(IsRowEmpty(i)) {
			Массив<RichCell> r(cell[i], 1);
			RemoveRow0(i);
			for(int j = 0; j < дайКолонки(); j++)
				if(ci[i][j].valid) {
					cell[i][j] <<= r[j];
					cell[i][j].vspan--;
				}
			Normalize0();
		}
		else
			i++;
	int j = 1;
	while(j < дайКолонки())
		if(IsColumnEmpty(j)) {
			Массив<RichCell> r;
			for(int i = 0; i < GetRows(); i++)
				r.добавь() <<= cell[i][j];
			int c = формат.column[j];
			RemoveColumn0(j);
			формат.column[min(дайКолонки() - 1, j - 1)] += c;
			for(int i = 0; i < GetRows(); i++)
				if(ci[i][j].valid)
					cell[i][j] <<= r[i];
			Normalize0();
		}
		else
			j++;
	int sum = сумма(формат.column);
	if(sum != 10000) {
		r_row = -1;
		if(формат.column.дайСчёт()) {
			if(формат.column.дайСчёт() > 1) {
				int q = 0;
				for(int i = 0; i < формат.column.дайСчёт() - 1; i++)
					q += sum <= 0 ? (формат.column[i] = 10000 / формат.column.дайСчёт())
					              : (формат.column[i] = формат.column[i] * 10000 / sum);
				формат.column[формат.column.дайСчёт() - 1] = 10000 - q;
			}
			else
				формат.column[0] = 10000;
		}
	}
}

int RichTable::дайДлину() const
{
	if(length < 0) {
		length = 0;
		for(int i = 0; i < cell.дайСчёт(); i++)
			for(int j = 0; j < формат.column.дайСчёт(); j++)
				if(ci[i][j].valid)
					length += cell[i][j].text.дайДлину() + 1;
	}
	return length ? length - 1 : 0;
}

Точка   RichTable::FindCell(int& pos) const
{
	for(int i = 0; i < cell.дайСчёт(); i++)
		for(int j = 0; j < формат.column.дайСчёт(); j++)
			if(ci[i][j].valid) {
				int l = cell[i][j].text.дайДлину() + 1;
				if(pos < l)
					return Точка(j, i);
				pos -= l;
			}
	NEVER();
	return Точка();
}

int RichTable::GetCellPos(int ii, int jj) const
{
	int pos = 0;
	for(int i = 0; i < ii; i++)
		for(int j = 0; j < формат.column.дайСчёт(); j++)
			if(ci[i][j].valid)
				pos += cell[i][j].text.дайДлину() + 1;
	for(int j = 0; j < jj; j++)
		if(ci[ii][j].valid)
			pos += cell[ii][j].text.дайДлину() + 1;
	return pos;
}

Точка RichTable::GetMasterCell(int i, int j) const
{
	i = min(GetRows() - 1, i);
	j = min(дайКолонки() - 1, j);
	const ИнфОЯчейке& cf = ci[i][j];
	return cf.valid ? Точка(j, i) : cf.master;
}

const RichCell& RichTable::GetMaster(int i, int j) const
{
	Точка p = GetMasterCell(i, j);
	return cell[p.y][p.x];
}

int RichTable::GetTableCount(int ii, int jj) const
{
	int ti = 0;
	for(int i = 0; i < ii; i++)
		for(int j = 0; j < формат.column.дайСчёт(); j++)
			if(ci[i][j].valid)
				ti += cell[i][j].text.GetTableCount();
	for(int j = 0; j < jj; j++)
		if(ci[ii][j].valid)
			ti += cell[ii][j].text.GetTableCount();
	return ti;
}


void RichTable::ClearSpelling()
{
	for(int i = 0; i < cell.дайСчёт(); i++)
		for(int j = 0; j < формат.column.дайСчёт(); j++)
			if(ci[i][j].valid)
				cell[i][j].text.ClearSpelling();
}

int RichTable::GetTableCount() const
{
	if(tabcount < 0) {
		tabcount = 0;
		for(int i = 0; i < cell.дайСчёт(); i++) {
			for(int j = 0; j < формат.column.дайСчёт(); j++)
				if(ci[i][j].valid)
					tabcount += cell[i][j].text.GetTableCount();
		}
	}
	return tabcount;
}

void RichTable::Validate()
{
	r_row = -2;
}

void RichTable::добавьКолонку(int cx)
{
	формат.column.добавь(cx);
}

void RichTable::SetPick(int i, int j, RichTxt&& text)
{
	cell.по(i).по(j).text = pick(text);
}

RichTxt RichTable::GetPick(int i, int j)
{
	return pick(cell[i][j].text);
}

void RichTable::SetQTF(int i, int j, const char *qtf)
{
	SetPick(i, j, ParseQTF(qtf));
}

void RichTable::устФормат(int i, int j, const RichCell::фмт& fmt)
{
	cell.по(i).по(j).формат = fmt;
}

void RichTable::SetSpan(int i, int j, int vspan, int hspan)
{
	RichCell& c = cell.по(i).по(j);
	c.vspan = vspan;
	c.hspan = hspan;
}

Размер RichTable::GetSpan(int i, int j) const
{
	const RichCell& c = cell[i][j];
	return Размер(c.hspan, c.vspan);
}

void RichTable::устФормат(const фмт& fmt)
{
	формат = fmt;
	РНЦП::SetQTF(header, fmt.header_qtf);
	РНЦП::SetQTF(footer, fmt.footer_qtf);
}

RichTable RichTable::копируй(const Прям& sel) const
{
	RichTable r;
	r.формат = формат;
	r.формат.header = max(0, формат.header - sel.top);
	r.формат.column.удали(0, sel.left);
	r.формат.column.устСчёт(sel.right - sel.left + 1);
	for(int i = sel.top; i <= sel.bottom; i++)
		for(int j = sel.left; j <= sel.right; j++)
			r.cell.по(i - sel.top).по(j - sel.left) <<= cell[i][j];
	r.нормализуй();
	return r;
}

void RichTable::RemoveRow(int rowi)
{
	RemoveRow0(rowi);
	нормализуй();
}

void RichTable::InsertRow(int rowi, const RichStyles& style)
{
	if(rowi < формат.header)
		формат.header++;
	int si;
	if(rowi < GetRows()) {
		for(int i = 0; i < дайКолонки(); i++) {
			ИнфОЯчейке& cf = ci[rowi][i];
			if(!cf.valid && cf.master.x == i)
				cell[cf.master.y][cf.master.x].vspan++;
		}
		si = rowi + 1;
	}
	else
		si = rowi - 1;
	cell.вставь(rowi).устСчёт(дайКолонки());
	if(si >= 0)
		for(int i = 0; i < дайКолонки(); i++) {
			RichCell& c = cell[rowi][i];
			const RichCell& sc = cell[si][i];
			c.формат = sc.формат;
			DUMP(sc.hspan);
			c.hspan = sc.hspan;
			c.ClearText(sc.text.GetFirstFormat(style), style);
		}
	нормализуй();
}

void RichTable::RemoveColumn(int column)
{
	RemoveColumn0(column);
	нормализуй();
}

void RichTable::InsertColumn(int column, const RichStyles& style)
{
	int sci = column < дайКолонки() ? column + 1 : column - 1;
	for(int i = 0; i < cell.дайСчёт(); i++) {
		ИнфОЯчейке& cf = ci[i][column];
		if(!cf.valid && cf.master.y == i)
			cell[cf.master.y][cf.master.x].hspan++;
		RichCell& c = cell[i].вставь(column);
		if(sci >= 0) {
			const RichCell& sc = cell[i][sci];
			c.формат = sc.формат;
			c.vspan = sc.vspan;
			c.ClearText(sc.text.GetFirstFormat(style), style);
		}
	}
	int c = формат.column[min(column, дайКолонки() - 1)];
	формат.column.вставь(column, c);
	нормализуй();
}

void  RichTable::SplitCell(Точка cl, Размер sz, const RichStyles& style)
{
	const RichCell& sc = cell[cl.y][cl.x];
	int exty = sz.cy - cell[cl.y][cl.x].vspan - 1;
	int extx = sz.cx - cell[cl.y][cl.x].hspan - 1;
	int ny = cell.дайСчёт() + exty;
	int nx = формат.column.дайСчёт() + extx;
	if(ny < 0 || ny > 20000 || nx < 0 || nx > 200)
		return;
	if(exty > 0) {
		cell.вставьН(cl.y + 1, exty);
		if(cl.y < формат.header)
			формат.header += exty;
		for(int i = 0; i < exty; i++) {
			cell[cl.y + 1 + i].устСчёт(дайКолонки());
			for(int j = 0; j < дайКолонки(); j++) {
				RichCell& c = cell[cl.y + 1 + i][j];
				const RichCell& sc = cell[cl.y][j];
				c.формат = sc.формат;
				c.hspan = sc.hspan;
				c.ClearText(sc.text.GetFirstFormat(style), style);
			}
		}
		for(int i = 0; i < дайКолонки(); i++) {
			ИнфОЯчейке& cf = ci[cl.y][i];
			if(cf.valid)
				cell[cl.y][i].vspan += exty;
			else
			if(cf.master.x == i)
				cell[cf.master.y][cf.master.x].vspan += exty;
		}
	}

	cell[cl.y][cl.x].vspan = 0;
	if(exty < 0)
		cell[cl.y + sz.cy - 1][cl.x].vspan = -exty;
	for(int i = 1; i < sz.cy; i++) {
		RichCell& c = cell[cl.y + i][cl.x];
		c.формат = sc.формат;
		c.hspan = sc.hspan;
		c.ClearText(sc.text.GetFirstFormat(style), style);
	}

	Normalize0();
	// ext = sz.cx - cell[cl.y][cl.x].hspan - 1;
	if(extx > 0) {
		int clx = 0;
		for(int i = 0; i <= cell[cl.y][cl.x].hspan; i++)
			clx += формат.column[cl.x + i];
		формат.column.вставьН(cl.x, extx);
		int q = clx / sz.cx;
		for(int i = 1; i < sz.cx; i++)
			формат.column[cl.x + i] = q;
		формат.column[cl.x] = clx - (sz.cx - 1) * q;
		for(int i = 0; i < cell.дайСчёт(); i++) {
			cell[i].вставьН(cl.x + 1, extx);
			for(int q = 0; q < extx; q++) {
				RichCell& c = cell[i][cl.x + 1 + q];
				const RichCell& sc = cell[i][cl.x];
				c.формат = sc.формат;
				c.vspan = sc.vspan;
				c.ClearText(sc.text.GetFirstFormat(style), style);
			}
			ИнфОЯчейке& cf = ci[i][cl.x];
			if(cf.valid)
				cell[i][cl.x].hspan += extx;
			else
			if(cf.master.y == i)
				cell[cf.master.y][cf.master.x].hspan += extx;
		}
	}
	for(int i = 0; i < sz.cy; i++)
		for(int j = 0; j < sz.cx; j++) {
			RichCell& c = cell[cl.y + i][cl.x + j];
			if(j < sz.cx - 1)
				c.hspan = 0;
			if(j) {
				const RichCell& sc = cell[cl.y + i][cl.x];
				c.формат = sc.формат;
				c.vspan = sc.vspan;
				c.ClearText(sc.text.GetFirstFormat(style), style);
			}
		}
	нормализуй();
}

void sMatchRect(Прям& t, const Прям& s)
{
	if(t.left != s.left)
		t.left = Null;
	if(t.top != s.top)
		t.top = Null;
	if(t.right != s.right)
		t.right = Null;
	if(t.bottom != s.bottom)
		t.bottom = Null;
}

RichCell::фмт RichTable::GetCellFormat(const Прям& sel) const
{
	RichCell::фмт fmt;
	for(int i = sel.top; i <= sel.bottom; i++)
		for(int j = sel.left; j <= sel.right; j++)
			if(i == sel.top && j == sel.left)
				fmt = cell[i][j].формат;
			else
			if(ci[i][j].valid) {
				const RichCell::фмт& f = cell[i][j].формат;
				sMatchRect(fmt.border, f.border);
				sMatchRect(fmt.margin, f.margin);
				if(fmt.align != f.align)
					fmt.align = Null;
				if(fmt.color != f.color)
					fmt.color = VoidColor;
				if(fmt.bordercolor != f.bordercolor)
					fmt.bordercolor = VoidColor;
			}
	return fmt;
}

void sSetRect(Прям& t, const Прям& s)
{
	if(!пусто_ли(s.left))
		t.left = s.left;
	if(!пусто_ли(s.top))
		t.top = s.top;
	if(!пусто_ли(s.right))
		t.right = s.right;
	if(!пусто_ли(s.bottom))
		t.bottom = s.bottom;
}

void  RichTable::SetCellFormat(const Прям& sel, const RichCell::фмт& fmt,
                               bool setkeep, bool setround)
{
	for(int i = sel.top; i <= sel.bottom; i++)
		for(int j = sel.left; j <= sel.right; j++)
			if(ci[i][j].valid) {
				RichCell::фмт& f = cell[i][j].формат;
				sSetRect(f.border, fmt.border);
				sSetRect(f.margin, fmt.margin);
				if(!пусто_ли(fmt.align))
					f.align = fmt.align;
				if(fmt.color != VoidColor)
					f.color = fmt.color;
				if(fmt.bordercolor != VoidColor)
					f.bordercolor = fmt.bordercolor;
				if(!пусто_ли(fmt.minheight))
					f.minheight = fmt.minheight;
				if(setkeep)
					f.keep = fmt.keep;
				if(setround)
					f.round = fmt.round;
			}

	нормализуй();
}

void  RichTable::Paste(Точка pos, const RichTable& tab)
{
	for(int i = 0; i < tab.GetRows(); i++)
		for(int j = 0; j < min(дайКолонки() - pos.x, tab.дайКолонки()); j++)
			cell.по(i + pos.y, cell[cell.дайСчёт() - 1]).по(j + pos.x) <<= tab[i][j];
	нормализуй();
}

void RichTable::AdjustSel(Прям& sel) const
{
again:
	for(int i = sel.top; i <= sel.bottom; i++)
		for(int j = sel.left; j <= sel.right; j++) {
			Точка p = GetMasterCell(i, j);
			const RichCell& c = cell[p.y][p.x];
			Точка pp = p + Точка(c.hspan, c.vspan);
			if(p.x < sel.left) {
				sel.left = p.x;
				goto again;
			}
			if(pp.x > sel.right) {
				sel.right = pp.x;
				goto again;
			}
			if(p.y < sel.top) {
				sel.top = p.y;
				goto again;
			}
			if(pp.y > sel.bottom) {
				sel.bottom = pp.y;
				goto again;
			}
		}
}

RichTable::RichTable(const RichTable& src, int)
{
	формат = src.формат;
	cell <<= src.cell;
	r_row = -1;
	нормализуй();
}

RichTable::RichTable()
{
	clayout.py.page = -1;
	r_row = -1;
	инивалидируй();
}

}
