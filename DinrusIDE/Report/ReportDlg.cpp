#include "Report.h"

namespace РНЦП {

#define LLOG(x) // LOG(x)

#define  TFILE <Report/Report.t>
#include <Core/t.h>

void Print(Report& r, PrinterJob& pd)
{
	Draw& w = pd;
	Размер sz = w.GetPageSize();
	Точка mg = r.GetMargins();
	Размер pgsz = r.дайСтраницу(0).дайРазм();
	int	x = Nvl(mg.x, (sz.cx - pgsz.cx) / 2);
	int y = Nvl(mg.y, (sz.cy - pgsz.cy) / 2);
	for(int i = 0; i < pd.GetPageCount(); i++) {
		Чертёж iw = r.дайСтраницу(pd[i]);
		Размер sz = iw.дайРазм();
		w.StartPage();
		w.DrawDrawing(x, y, sz.cx, sz.cy, iw);
		w.EndPage();
	}
}

bool Print0(Report& r, int i, const char *_name, bool dodlg) {
	if(r.GetPrinterJob()) {
		Print(r, *r.GetPrinterJob());
		return true;
	}
	PrinterJob pd(_name);
	pd.CurrentPage(i);
	pd.MinMaxPage(0, r.дайСчёт() - 1);
	Размер pgsz = r.дайСтраницу(0).дайРазм();
	pd.Landscape(pgsz.cx > pgsz.cy);
	if(dodlg && !pd.выполни())
		return false;
	Print(r, pd);
	return true;
}

bool Print(Report& r, int i, const char *_name)
{
	return Print0(r, i, _name, true);
}

bool DefaultPrint(Report& r, int i, const char *_name)
{
	return Print0(r, i, _name, false);
}

void ReportView::колесоМыши(Точка, int zdelta, dword) {
	sb.Wheel(zdelta);
}

ReportView::ReportView() {
	report = NULL;
	vsize = 0;
	pagesize.cx = pagesize.cy = 0;
	sb.ПриПромоте = callback(this, &ReportView::Sb);
	sb.устСтроку(16);
	добавьФрейм(sb);
	устФрейм(фреймПоле());
	numbers = false;
	pages = PG1;
}

void ReportView::уст(Report& p) {
	иниц();
	report = &p;
	освежи();
	Выкладка();
}

Размер ReportView::GetReportSize()
{
	return report->дайСтраницу(0).дайРазм();
}

bool ReportView::Ключ(dword ключ, int) {
	return sb.вертКлюч(ключ);
}

void ReportView::леваяВнизу(Точка p, dword) {
	if(!report || pagesize.cy == 0 || pagesize.cx == 0) return;
	int pg = (sb + p.y) / pagesize.cy * pvn + min(pvn - 1, p.x / pagesize.cx);
	WhenGoPage();
	Выкладка();
	sb = pg * pagesize.cy;
}

void ReportView::Sb() {
	освежи();
	WhenAction();
}

void ReportView::иниц() {
	for(int i = 0; i < 64; i++) {
		page[i].очисть();
		pagei[i] = -1;
	}
}

void ReportView::Выкладка() {
	Размер sz = дайРазм();
	if(report && report->дайСчёт()) {
		ПРОВЕРЬ(pages >= PG1 && pages <= PG16);
		pvn = 1 << pages;
		static int h[] = { 1, 3, 15, 63 };
		pm = h[pages];
		Размер sr = report->дайСтраницу(0).дайРазм();
		pagesize.cx = sz.cx / pvn;
		pagesize.cy = sr.cy * pagesize.cx / sr.cx;
		vsize = (report->дайСчёт() + pvn - 1) / pvn * pagesize.cy;
		sb.устСтраницу(sz.cy);
		sb.устВсего(vsize);
		иниц();
		освежи();
	}
	else {
		vsize = 0;
		sb.уст(0, 0, 0);
		return;
	}
}

Рисунок ReportView::дайСтраницу(int i) {
	ПРОВЕРЬ(report);
	int ii = i & pm;
	if(pagei[ii] != i) {
		pagei[ii] = i;
		Размер sz = Размер(max(pagesize.cx - 2, 1), max(pagesize.cy - 2, 1));
		if(HasPainter()) {
			ImageBuffer ib(sz);
			Fill(~ib, белый(), ib.дайДлину());
			PaintImageBuffer(ib, report->дайСтраницу(i));
			page[ii] = ib;
		}
		else {
			ImageDraw iw(sz);
			iw.DrawRect(sz, белый);
			iw.DrawDrawing(0, 0, sz.cx, sz.cy, report->дайСтраницу(i));
			page[ii] = iw;
		}
	}
	return page[ii];
}

void ReportView::промотайДо(int toppage, int top, int bottompage, int bottom) {
	sb.промотайДо(toppage * pagesize.cy + top * pagesize.cy / GetReportSize().cy,
	              (bottompage - toppage) * pagesize.cy +
	                (bottom - top) * pagesize.cy / GetReportSize().cy);
}

void ReportView::рисуй(Draw& w) {
	Размер sz = дайРазм();
	if(sz.cx <= 0 || sz.cy <= 0) return;
	if(!vsize || !report) {
		w.DrawRect(0, 0, sz.cx, sz.cy, SGray);
		return;
	}
	int i = sb / pagesize.cy;
	int y = i * pagesize.cy - sb;
	i *= pvn;
	while(y < sz.cy) {
		int x = 0;
		for(int j = pvn; j--;) {
			if(i < report->дайСчёт()) {
				w.DrawImage(x + 1, y + 1, дайСтраницу(i));
				if(j == 0) {
					w.DrawRect(x + pagesize.cx - 1, y, sz.cx - x - pagesize.cx, pagesize.cy, белый);
					DrawFrame(w, x, y, sz.cx - x, pagesize.cy, белый, светлоСерый);
				}
				else
					DrawFrame(w, x, y, pagesize.cx, pagesize.cy, белый, светлоСерый);
				if(numbers) {
					Ткст n = фмт("%d", i + 1);
					Размер tsz = дайРазмТекста(n, StdFont());
					tsz += Размер(8, 4);
					int tx = x + pagesize.cx - tsz.cx;
					DrawFrame(w, tx, y, tsz.cx, tsz.cy, чёрный, чёрный);
					w.DrawRect(tx + 1, y + 1, tsz.cx - 2, tsz.cy - 2, жёлтый);
					w.DrawText(tx + 4, y + 2, n);
				}
			}
			else {
				w.DrawRect(x, y, sz.cx - x, pagesize.cy, серый);
				break;
			}
			x += pagesize.cx;
			i++;
		}
		y += pagesize.cy;
	}
}

ReportWindow::ReportWindow()
{
	CtrlLayoutCancel(*this, "");
	sw <<= ReportView::PG1;
	sw <<= THISBACK(Pages);
	numbers <<= THISBACK(Numbers);
	pg.WhenGoPage = THISBACK(GoPage);
	lbl.устШрифт(ArialZ(20).Italic());
	lbl.устЛин(ALIGN_CENTER);
	pg.WhenAction = THISBACK(ShowPage);
	Sizeable();
	MaximizeBox();
	Иконка(CtrlImg::smallreporticon());
	SetButton(0, t_("&Print"), 999995);
	pdf.покажи(GetDrawingToPdfFn());
	pdf <<= THISBACK(Pdf);
}

Ткст Pdf(Report& report, bool pdfa, const PdfSignatureInfo *зн)
{
	return GetDrawingToPdfFn() && report.дайСчёт() ?
	      (*GetDrawingToPdfFn())(report.GetPages(), report.дайСтраницу(0).дайРазм(),
	                             Nvl(report.GetMargins().x, 200), pdfa, зн)
	      : Ткст();
}

void ReportWindow::Pdf()
{
	static FileSel fs;
	static bool b;
	if(!b) {
		fs.Type(t_("PDF file"), "*.pdf");
		fs.AllFilesType();
	}
	if(!fs.ExecuteSaveAs(t_("Output PDF file")))
		return;
	сохраниФайл(~fs, РНЦП::Pdf(*report));
}

void ReportWindow::SetButton(int i, const char *label, int id)
{
	ПозЛога p = cancel.дайПоз();
	Кнопка& b = button.по(i);
	b.устНадпись(label);
	b.RightPos(p.x.GetA(), p.x.дайС()).BottomPos(p.y.GetA() + (p.y.дайС() + 6) * (i + 1), p.y.дайС());
	b <<= Breaker(id);
	for(int i = 0; i < button.дайСчёт(); i++)
		button[i].удали();
	for(int i = 0; i < button.дайСчёт(); i++)
		добавьОтпрыскПеред(&button[i], &cancel);
}

void ReportWindow::ShowPage()
{
	if(pg.дай())
		lbl = фмт("%d / %d", pg.дайПерв() + 1, pg.дай()->дайСчёт());
}

int ReportWindow::Perform(Report& report, int zoom, const char *caption)
{
	this->report = &report;
//	if(report.dortf)
//		WriteClipboard(GetClipboardFormatCode("Rich text формат"), report.rtf.дай());
	pg.уст(report);
	Размер sz;
	Прям area = Ктрл::GetWorkArea();
	sz = area.размер() * 4 * абс(zoom) / 500;
	устПрям(sz);

	ActiveFocus(pg);
	Титул(caption);
	открой();
	for(;;) {
		ShowPage();
		int c = пуск();
		switch(c) {
		case IDCANCEL:
			return false;
		case 999995:
			return Print(report, pg.дайПерв(), caption);
		}
		if(c > 0)
			return c;
	}
}

bool Perform(Report& r, const char *caption)
{
	return ReportWindow().Perform(r, 100, caption);
}

bool QtfReport0(const Ткст& qtf, const char *имя, bool pagenumbers, Размер pagesize)
{
	Report r;
	if(!пусто_ли(pagesize))
		r.SetPageSize(pagesize);
	if(pagenumbers) {
		Report rr;
		rr.Footer("[1> $$P]");
		rr << qtf;
		r.Footer("[1> $$P/" + какТкст(rr.дайСчёт()) + "]");
	}
	r << qtf;
	return Perform(r, имя);
}

bool QtfReport(const Ткст& qtf, const char *имя, bool pagenumbers)
{
	return QtfReport0(qtf, имя, pagenumbers, Null);
}

bool QtfReport(Размер pagesize, const Ткст& qtf, const char *имя, bool pagenumbers)
{
	return QtfReport0(qtf, имя, pagenumbers, pagesize);
}


}
