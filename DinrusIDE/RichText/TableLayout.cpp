#include "RichText.h"

namespace РНЦП {

bool RichTable::Reduce(Прям& r) const
{
	Прям br = r;
	r.left += формат.lm;
	r.right -= формат.rm;
	r.дефлируй(формат.frame);
	if(r.пустой()) {
		r = br;
		return false;
	}
	return true;
}

bool RichTable::Reduce(RichContext rc, Прям& first_page, Прям& next_page) const
{
	RichContext rc2 = rc;
	rc2.Page();
	first_page = rc.page;
	next_page = rc2.page;
	return Reduce(first_page) && Reduce(next_page);
}

Прям RichTable::GetPageRect(PageY py) const
{
	return py.page == clayout.py.page ? clayout.first_page : clayout.next_page;
}

void RichTable::следщСтраница(PageY& py, bool header) const
{
	py.y = header ? clayout.header_page.top : clayout.next_page.top;
	py.page++;
}

RichContext RichTable::MakeRichContext(RichContext rc, PageY py, bool header) const
{
	if(header)
		rc.уст(py, clayout.header_page, clayout.header_page, py);
	else
		rc.уст(clayout.py, clayout.first_page, clayout.next_page, py);
	return rc;
}

const RichTable::TabLayout& RichTable::Realize(RichContext rc) const
{ // создай page layout with header
	Прям first_page, next_page;
	Reduce(rc, first_page, next_page);
	if(rc.py != clayout.py || first_page != clayout.first_page || next_page != clayout.next_page) {
		clayout.sz = дайРазм();
		clayout.py = rc.py;
		clayout.first_page = first_page;
		clayout.next_page = next_page;
		clayout.header_page = clayout.next_page;
		PageY py = rc.py;
		py.y += формат.before;
		if(py.y > clayout.first_page.bottom)
			следщСтраница(py, false);
		clayout.hasheader = false;
		if(формат.header && cell.дайСчёт()) {
			RichContext nextpage_rc = rc;
			PageY hpy(0, clayout.next_page.top); // RowPaint will need page zero here to add real page
			nextpage_rc.уст(hpy, clayout.next_page, clayout.next_page, hpy);
			int hy = min(формат.header, cell.дайСчёт());
			clayout.header = Realize(hpy, nextpage_rc, hy, true); // realize header as if first on next page
			if(clayout.header[0].py.page == clayout.header[hy - 1].pyy.page) { // header fits single page
				Выкладка x = Realize(py, MakeRichContext(rc, py, false), cell.дайСчёт(), false); // TODO: check this
				if(cell.дайСчёт() > hy && py.page != x[hy].py.page) // first header would break the end of page
					следщСтраница(py, false);
				clayout.hasheader = true; // if it fits, we repeat header on each new page
				clayout.header_page.bottom = clayout.header[hy - 1].pyy.y;
				clayout.next_page.top = clayout.header_page.bottom + формат.grid ;// so have to reduce the page size for nonheader rows
			}
		}
		(Выкладка&)clayout = Realize(py, MakeRichContext(rc, py, false), cell.дайСчёт(), false);
		if(формат.keep && cell.дайСчёт()) {
			if(clayout[0].py.page != clayout[cell.дайСчёт() - 1].pyy.page)
				следщСтраница(py, false);
			(Выкладка&)clayout = Realize(py, MakeRichContext(rc, py, false), cell.дайСчёт(), false);
		}
	}
	return clayout;
}

RichContext RichTable::PaintCell::MakeRichContext(RichContext rc) const
{
	rc.page.left = page_left;
	rc.page.right = page_right;
	return rc;
}

RichTable::Выкладка RichTable::Realize(PageY py, RichContext arc, int ny, bool header) const
{ // create layout for first ny rows
	Выкладка tab;
	
	int nx = формат.column.дайСчёт();
	tab.row.размести(ny);
	for(int i = 0; i < ny; i++)
		tab[i].cell.размести(nx);
	
	int sum = 0;
	for(int i = 0; i < nx; i++)
		sum += формат.column[i];

	Буфер<int> column_left, column_right;
	column_left.размести(nx);
	column_right.размести(nx);

	int x = 0;
	int xx = arc.page.left;
	int dcx = arc.page.устШирину();
	for(int i = 0; i < nx; i++) {
		column_left[i] = xx;
		x += формат.column[i];
		xx = column_right[i] = x * dcx / sum + arc.page.left;
	}

	int f2 = формат.grid / 2;
	int ff2 = формат.grid - f2;

	py.y += формат.frame;
	for(int i = 0; i < ny; i++) {
		const Массив<RichCell>& row = cell[i];
		PaintRow& pr = tab[i];
		pr.first = i == 0;
		pr.gpy = py;
		if(i)
			py.y += формат.grid;
		for(int j = 0; j < nx;) {
			PaintCell& pc = pr[j];
			const RichCell& cell = row[j];
			if(pc.top) {
				pc.page_left = pc.left = column_left[j];
				pc.page_right = pc.right = column_right[min(nx - 1, j + cell.hspan)];
				if(j)
					pc.page_left += f2;
				if(j + cell.hspan < nx - 1)
					pc.page_right -= ff2;
				pc.bottom = false;
				int ms = min(ny - 1, i + cell.vspan);
				for(int k = i + 1; k <= ms; k++) {
					PaintCell& pc = tab[k][j];
					pc.top = pc.bottom = false;
				}
				tab[ms][j].bottom = true;
			}
			j += cell.hspan + 1;
		}
		bool keep = i < формат.header || формат.keep;
		for(int j = 0; j < nx;) {
			const RichCell& cell = row[j];
			if(pr[j].top && cell.формат.keep) {
				keep = true;
				break;
			}
			j += cell.hspan + 1;
		}
		bool span = false;
		for(int j = 0; j < nx;) {
			const RichCell& cell = row[j];
			if(!pr[j].top) {
				span = true;
				break;
			}
			j += cell.hspan + 1;
		}
		if(!span) // check whether we should break page before this row
			for(int j = 0; j < nx;) {
				const RichCell& cell = row[j];
				PaintCell& pc = pr[j];
				if(pc.top) {
					RichContext trc = pc.MakeRichContext(MakeRichContext(arc, py, header));
					PageY ty = cell.GetTop(trc);
					PageY ky = py;
					if(keep)
						ky = cell.дайВысоту(trc);
					if(ty.page != py.page || ky.page != py.page) {
						следщСтраница(py, header);
						pr.gpy = py;
						pr.first = true;
						break;
					}
				}
				j += cell.hspan + 1;
			}
		pr.py = py;
		for(int j = 0; j < nx;) {
			const RichCell& cell = row[j];
			PaintCell& pc = pr[j];
			if(pc.top)
				tab[min(ny - 1, i + cell.vspan)][j].hy = cell.дайВысоту(pc.MakeRichContext(MakeRichContext(arc, py, header)));
			j += cell.hspan + 1;
		}
		for(int j = 0; j < nx;) {
			const RichCell& cell = row[j];
			if(pr[j].bottom)
				py = max(pr[j].hy, py);
			j += cell.hspan + 1;
		}
		tab.pyy = pr.pyy = py;
	}
	return tab;
}

}
