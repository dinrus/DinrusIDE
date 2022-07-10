#include "RichText.h"

namespace РНЦП {

RichTable::фмт::фмт()
{
	grid = 4;
	gridcolor = чёрный;
	frame = 10;
	framecolor = чёрный;
	before = after = lm = rm = 0;
	header = 0;
	keep = newpage = newhdrftr = false;
}

#include "RichText.h"

void RichTable::ExpandFrr(ВекторМап<int, Прям>& frr, int pi, int l, int r, int t, int b)
{
	Прям& fr = frr.дайДобавь(pi, Прям(INT_MAX, INT_MAX, INT_MIN, INT_MIN));
	fr.left = min(fr.left, l);
	fr.right = max(fr.right, r);
	fr.top = min(fr.top, t);
	fr.bottom = max(fr.bottom, b);
}

bool RichTable::RowPaint(PageDraw& pw, RichContext rc, const Выкладка& tab, bool header,
                          int i, int ny, Прям pg, Прям npg,
                          ВекторМап<int, Прям>& frr,
                          PaintInfo& pi, int pd, bool sel) const
{
	const Массив<RichCell>& row = cell[i];
	const PaintRow& pr = tab[i];
	if(pw.tracer)
		pw.tracer->TableRow(GetPageRect(pr.py), rc.py, i, *this);
	int gridln = LineZoom(pi.zoom, формат.grid);
	int ff2ln = gridln - gridln / 2;
	Цвет gc = формат.gridcolor;
	if(gridln == 0 && !пусто_ли(pi.showcodes)) {
		gridln = 1;
		gc = pi.showcodes;
	}
	PageY py = pr.py;
	py.page += pd;
	if(py.page != clayout.py.page) // We are on the next page already
		pg = npg;
	for(int j = 0; j < формат.column.дайСчёт();) {
		const RichCell& cell = row[j];
		const PaintCell& pc = pr[j];
		if(pc.top) {
			PageY pyy = tab[min(ny - 1, i + cell.vspan)].pyy;
			pyy.page += pd; // move header line
			bool paint = pyy > pi.top;
			Прям xpg = pg; // on first page
			Прям nxpg = npg; // on next page
			int y;
			int ny = pi.zoom * pyy.y;
			y = pi.zoom * pr.gpy.y;
			nxpg.left = xpg.left = pi.zoom * pc.left;
			xpg.right = pi.zoom * pc.right;
			if(j) { // need to draw vertical grid line on the left
				nxpg.left = xpg.left = xpg.left - ff2ln; // move half of vertical grid width left
				if(py.page == pyy.page)
					pw.Page(py.page).DrawRect(xpg.left, y, gridln, ny - y, gc);
				else {
					pw.Page(py.page).DrawRect(xpg.left, y, gridln, pg.bottom - y, gc);
					for(int i = py.page + 1; i < pyy.page; i++)
						pw.Page(i).DrawRect(nxpg.left, npg.top, gridln, npg.bottom - npg.top, gc);
					pw.Page(pyy.page).DrawRect(nxpg.left, npg.top, gridln, ny - npg.top, gc);
				}
				nxpg.left = xpg.left = xpg.left + gridln;
			}
			if(j + cell.hspan < формат.column.дайСчёт() - 1)
				xpg.right = pi.zoom * pc.right - ff2ln; // make place for the next grid line
			nxpg.right = xpg.right;
			if(!pr.first) { // Draw horizontal grid line
				if(paint)
					pw.Page(py.page).DrawRect(xpg.left, y, xpg.устШирину(), gridln, gc);
				y += gridln;
			}
			if(paint) {
				if(pw.tracer)
					pw.tracer->TableCell(rc.page, rc.py, i, j, *this, pyy);
				row[j].рисуй(pw, pc.MakeRichContext(MakeRichContext(rc, py, header)), pyy, xpg, nxpg, y, ny, pi,
				             sel && j >= pi.cells.left && i >= pi.cells.top &&
				             j + cell.hspan <= pi.cells.right && i + cell.vspan <= pi.cells.bottom);
				if(pw.tracer)
					pw.tracer->EndTableCell(pyy);
			}
			if(pyy.page == py.page)
				ExpandFrr(frr, py.page, xpg.left, xpg.right, y, ny);
			else {
				ExpandFrr(frr, py.page, xpg.left, xpg.right, y, xpg.bottom);
				for(int i = py.page + 1; i < pyy.page; i++)
					ExpandFrr(frr, i, nxpg.left, nxpg.right, nxpg.top, nxpg.bottom);
				ExpandFrr(frr, pyy.page, nxpg.left, nxpg.right, nxpg.top, ny);
			}
			int l = cell.text.дайДлину() + 1;
			pi.sell -= l;
			pi.selh -= l;
			pi.tablesel -= cell.text.GetTableCount();
		}
		j += cell.hspan + 1;
	}
	if(pw.tracer)
		pw.tracer->EndTableRow(rc.py);
	return rc.py >= pi.bottom;
}

void RichTable::рисуй(PageDraw& pw, RichContext rc, const PaintInfo& _pi, bool baselevel) const
{
	if(pw.tracer)
		pw.tracer->Table(rc.page, rc.py, *this);
	const TabLayout& tab = Realize(rc);
	PaintInfo pi = _pi;
	int frameln = LineZoom(pi.zoom, формат.frame);
	int gridln = LineZoom(pi.zoom, формат.grid);
	Прям pg[2];
	Прям hpg(0, 0, 0, 0);
	RichContext hrc = rc;
	int hy = min(формат.header, cell.дайСчёт());
	for(int pass = 0;; pass++) {
		Прям& pgr = pg[pass];
		pgr = pass ? tab.next_page : tab.first_page;
		pgr.left += формат.lm;
		pgr.right -= формат.rm;
		pgr.left = pi.zoom * pgr.left;
		pgr.right = pi.zoom * pgr.right;
		pgr.top = pi.zoom * pgr.top;
		pgr.bottom = pi.zoom * pgr.bottom;
		pgr.дефлируй(LineZoom(pi.zoom, формат.frame));
		if(pass)
			break;
		hpg = pgr;
		if(tab.hasheader) {
			hpg.bottom = pi.zoom * tab.header[hy - 1].pyy.y;
			pgr.top = hpg.bottom + LineZoom(pi.zoom, формат.grid);
		}
		hrc.Page();
	}
	
	bool allsel = false;
	if(pi.sell < 0 && pi.selh >= 0) {
		pi.sell = pi.selh = 0;
		allsel = true;
	}
	bool sel = pi.tablesel == 0;
	int ny = cell.дайСчёт();

	if(формат.newhdrftr && ny && baselevel) {
		Рисунок img = RichTextImg::HdrFtr();
		Размер isz = img.дайРазм();
		pw.Page(tab[0].py.page).DrawImage(-7 - isz.cx, pi.zoom * tab[0].py.y, img, pi.showcodes);
	}

	ВекторМап<int, Прям> frr; // page -> table rectangle
	for(int i = 0; i < ny; i++)
		if(RowPaint(pw, rc, tab, false, i, ny, pg[0], pg[1], frr, pi, 0, sel))
			break;

	Цвет gc = pi.ResolveInk(формат.gridcolor);
	Цвет fc = pi.ResolveInk(формат.framecolor);
	int fl = frameln;
	if(!пусто_ли(pi.showcodes)) {
		if(fl == 0 && !пусто_ли(pi.showcodes)) {
			fl = 1;
			fc = pi.showcodes;
		}
		if(gridln == 0) {
			gridln = 1;
			gc = pi.showcodes;
		}
	}
	for(int i = 0; i < frr.дайСчёт(); i++) { // add headers on subsequent pages and outer border
		pi.tablesel = 0;
		pi.sell = pi.selh = -1;
		int pgi = frr.дайКлюч(i);
		Прям r = frr[i];
		if(pgi > tab.py.page && tab.hasheader) // need to paint header here...
			for(int i = 0; i < hy; i++) {
				RowPaint(pw, rc, tab.header, true, i, hy, hpg, hpg, frr, pi, pgi, false);
				pw.Page(pgi).DrawRect(r.left, hpg.bottom, r.устШирину(), gridln, формат.gridcolor);
			}
		r = frr[i].инфлят(fl);
		Draw& w = pw.Page(pgi);
		if(!r.пустой()) {
			DrawFatFrame(w, r, fc, fl);
			if(allsel)
				pi.DrawSelection(w, r.left, r.top, r.устШирину(), r.устВысоту());
		}
	}
	if(pw.tracer)
		pw.tracer->EndTable(rc.py);
}

int RichTable::дайШирину(const RichStyles& st) const
{
	Размер sz = дайРазм();
	Буфер<int> col(sz.cx, 0);
	
	for(int x = 0; x < sz.cx; x++)
		for(int y = 0; y < sz.cy; y++) {
			const RichCell::фмт& fmt = дайФормат(y, x);
			col[x] = max(col[x], дай(y, x).дайШирину(st)
			         + fmt.border.left + fmt.border.right + fmt.margin.left + fmt.margin.right);
		}
	
	int sum = 0;
	for(int i = 0; i < sz.cx; i++)
		sum += формат.column[i];

	int maxcx = 0;
	for(int i = 0; i < sz.cx; i++)
		maxcx = max(maxcx, (col[i] * sum + формат.column[i] - 1) / формат.column[i]);
	
	return maxcx + (sz.cx + 1) * формат.grid + формат.lm + формат.rm;
}

PageY RichTable::дайВысоту(RichContext rc) const
{
	PageY pyy = Realize(rc).pyy;
	pyy.y += формат.frame;
	pyy.y += формат.after;
	if(pyy.y > GetPageRect(pyy).bottom)
		следщСтраница(pyy);
	return pyy;
}

PageY RichTable::GetTop(RichContext rc) const
{
	if(cell.дайСчёт() == 0)
		return rc.py;
	PageY py = Realize(rc)[0].py;
	py.y -= формат.frame;
	return py;
}

RichCaret RichTable::дайКаретку(int pos, RichContext rc) const
{
	ПРОВЕРЬ(pos >= 0);
	int nx = формат.column.дайСчёт();
	int ny = cell.дайСчёт();
	int ti = 0;
	const TabLayout& tab = Realize(rc);
	for(int i = 0; i < ny; i++) {
		const PaintRow& pr = tab[i];
		PageY pyy;
		for(int j = 0; j < nx; j++) {
			if(ci[i][j].valid) {
				const RichCell& cl = cell[i][j];
				pyy = tab[min(ny - 1, i + cl.vspan)].pyy;
				int l = cl.text.дайДлину() + 1;
				if(pos < l)
					return cl.дайКаретку(pos, pr[j].MakeRichContext(MakeRichContext(rc, pr.py)), pyy);
				ti += cl.text.GetTableCount();
				pos -= l;
			}
		}
	}
	NEVER();
	return RichCaret();
}

int  RichTable::дайПоз(int x, PageY y, RichContext rc) const
{
	int nx = формат.column.дайСчёт();
	int ny = cell.дайСчёт();
	const TabLayout& tab = Realize(rc);
	int pos = 0;
	for(int i = 0; i < ny; i++) {
		const PaintRow& pr = tab[i];
		rc.py = pr.py;
		for(int j = 0; j < nx; j++)
			if(ci[i][j].valid) {
				const RichCell& cl = cell[i][j];
				const PaintCell& pc = pr[j];
				PageY pyy = tab[min(ny - 1, i + cl.vspan)].pyy;
				if(y < pyy
				   && (j == 0 || x >= pc.page_left - формат.grid)
				   && (j == nx - 1 || x < pc.page_right))
					return cl.дайПоз(x, y, pr[j].MakeRichContext(MakeRichContext(rc, pr.py)), pyy) + pos;
				pos += cl.text.дайДлину() + 1;
			}
	}
	return pos ? pos - 1 : 0;
}

RichHotPos  RichTable::GetHotPos(int x, PageY y, int tolerance, RichContext rc) const
{
	RichHotPos hp;
	hp.textleft = rc.page.left;
	hp.textcx = rc.page.устШирину();
	int nx = формат.column.дайСчёт();
	int ny = cell.дайСчёт();
	const TabLayout& tab = Realize(rc);
	int left = tab.first_page.left;
	int right = tab.first_page.right;
	if(абс(x - left) <= tolerance) {
		hp.table = 0;
		hp.column = RICHHOT_LM;
		hp.delta = x - left;
		return hp;
	}
	if(абс(x - right) <= tolerance) {
		hp.table = 0;
		hp.column = RICHHOT_RM;
		hp.delta = x - right;
		return hp;
	}
	int ti = 0;
	for(int i = 0; i < ny; i++) {
		const PaintRow& pr = tab[i];
		for(int j = 0; j < nx; j++) {
			if(ci[i][j].valid) {
				const RichCell& cl = cell[i][j];
				const PaintCell& pc = pr[j];
				PageY pyy = tab[min(ny - 1, i + cl.vspan)].pyy;
				if(y < pyy) {
					if(j < nx - 1 && абс(x - pc.right) <= tolerance) {
						hp.table = 0;
						hp.column = j + cl.hspan;
						hp.delta = x - pc.right;
						hp.left = left;
						hp.cx = right - left;
						return hp;
					}
					if((j == 0 || x >= pc.left - формат.grid) && (j == nx - 1 || x < pc.right)) {
						hp = cl.GetHotPos(x, y, tolerance, pr[j].MakeRichContext(rc), pyy);
						hp.table += ti;
						return hp;
					}
				}
				ti += cl.text.GetTableCount();
			}
		}
	}
	return RichHotPos();
}

int RichTable::GetVertMove(int pos, int gx, RichContext rc, int dir) const
{
	if(cell.дайСчёт() == 0)
		return -1;
	int nx = формат.column.дайСчёт();
	int ny = cell.дайСчёт();
	const TabLayout& tab = Realize(rc);
	Точка cp;
	if(pos >= 0)
		cp = FindCell(pos);
	else {
		cp.y = dir > 0 ? 0 : cell.дайСчёт() - 1;
		cp.x = 0;
		const PaintRow& pr = tab[cp.y];
		for(int j = 0; j < nx; j++)
			if(ci[cp.y][cp.x].valid && gx < pr[j].page_right) {
				cp.x = j;
				break;
			}
		pos = -1;
	}
	for(;;) {
		RichContext rc1 = tab[cp.y][cp.x].MakeRichContext(rc);
		const RichCell& c = cell[cp.y][cp.x];
		if(c.Reduce(rc1)) {
			int q = cell[cp.y][cp.x].text.GetVertMove(pos, gx, rc1, dir);
			if(q >= 0)
				return q + GetCellPos(cp);
		}
		pos = -1;
		cp.y += dir;
		if(cp.y < 0 || cp.y >= ny)
			return -1;
	}
}

void RichTable::GatherValPos(Вектор<RichValPos>& f, RichContext rc, int pos, int тип) const
{
	ПРОВЕРЬ(pos >= 0);
	int nx = формат.column.дайСчёт();
	int ny = cell.дайСчёт();
	const TabLayout& tab = Realize(rc);
	for(int i = 0; i < ny; i++) {
		const PaintRow& pr = tab[i];
		rc.py = pr.py;
		for(int j = 0; j < nx; j++)
			if(ci[i][j].valid) {
				const RichCell& cl = cell[i][j];
				cl.GatherValPos(f, pr[j].MakeRichContext(rc), tab[min(ny - 1, i + cl.vspan)].pyy, pos, тип);
				pos += cl.text.дайДлину() + 1;
			}
	}
}

void RichTable::ApplyZoom(Zoom z, const RichStyles& ostyle, const RichStyles& zstyle)
{
	инивалидируй();
	r_row = -1;
	int nx = формат.column.дайСчёт();
	int ny = cell.дайСчёт();
	формат.frame = LineZoom(z, формат.frame);
	формат.grid = LineZoom(z, формат.grid);
	формат.before *= z;
	формат.lm *= z;
	формат.rm *= z;
	формат.after *= z;
	for(int i = 0; i < ny; i++)
		for(int j = 0; j < nx; j++)
			if(ci[i][j].valid) {
				RichCell& c = cell[i][j];
				c.формат.border *= z;
				c.формат.margin *= z;
				c.формат.minheight *= z;
				c.text.ApplyZoom(z, ostyle, zstyle);
			}
}

}
