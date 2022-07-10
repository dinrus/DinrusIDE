#include "CtrlLib.h"
		
#ifdef GUI_WIN
#ifndef PLATFORM_WINCE

#include <commdlg.h>
#pragma  comment(lib, "comdlg32.lib")

#endif
#else

#include <PdfDraw/PdfDraw.h>

#endif

namespace РНЦП {

#ifdef GUI_WIN

#ifndef PLATFORM_WINCE

#include <commdlg.h>
#pragma  comment(lib, "comdlg32.lib")

struct Win32PrintDlg_ : PRINTDLG {
	Win32PrintDlg_() {
		memset(this, 0, sizeof(PRINTDLG));
		lStructSize = sizeof(PRINTDLG);
	}
	~Win32PrintDlg_() {
		if(hDevMode)
			::GlobalFree(hDevMode);
		if(hDevNames)
			::GlobalFree(hDevNames);
	}
};

PrinterJob::PrinterJob(const char *_name)
{
	имя = _name;
	landscape = false;
	from = to = 1;
	current = 1;
}

PrinterJob::~PrinterJob()
{
}

bool PrinterJob::Execute0(bool dodlg)
{
	pdlg.создай<Win32PrintDlg_>();
	PRINTDLG& dlg = *pdlg;
	dlg.Flags = PD_DISABLEPRINTTOFILE|PD_NOSELECTION|PD_HIDEPRINTTOFILE|PD_RETURNDEFAULT;
	dlg.nFromPage = current;
	dlg.nToPage = current;
	dlg.nMinPage = from;
	dlg.nMaxPage = to;
	if(from != to)
		dlg.Flags |= PD_ALLPAGES;
	dlg.hwndOwner = 0;
	dlg.Flags |= PD_RETURNDEFAULT;
	dlg.nCopies = 1;
	if(!PrintDlg(&dlg)) return false;
	if(dlg.hDevMode) {
		DEVMODE *pDevMode = (DEVMODE*)::GlobalLock(dlg.hDevMode);
		pDevMode->dmOrientation = landscape ? DMORIENT_LANDSCAPE : DMORIENT_PORTRAIT;
		::GlobalUnlock(dlg.hDevMode);
	}
	int copies = 1;
	if(dodlg) {
		dlg.Flags = PD_DISABLEPRINTTOFILE|PD_NOSELECTION|PD_HIDEPRINTTOFILE;
		Вектор< Ук<Ктрл> > disabled = отклКтрлы(Ктрл::дайТопКтрлы());
		bool b = PrintDlg(&dlg);
		вклКтрлы(disabled);
		if(!b) return false;
		copies = dlg.nCopies;
		dlg.nCopies = 1; // because of buggy drivers for certain printers, we need to workaround copies problem
		if(dlg.hDevMode) {
			DEVMODE *pDevMode = (DEVMODE*)::GlobalLock(dlg.hDevMode);
			if(pDevMode->dmFields & DM_COPIES)
				copies = max((WORD)pDevMode->dmCopies, (WORD)copies);
			pDevMode->dmCopies = 1; // always set number of copies to 1 and deal with it by sending multiple pages to printer
			::GlobalUnlock(dlg.hDevMode);
		}
	}
	HDC hdc = NULL;
	if(dlg.hDevNames) {
		DEVNAMES *p = (DEVNAMES *)::GlobalLock(dlg.hDevNames);
		const char *driver = (const char *)p + p->wDriverOffset;
		const char *device = (const char *)p + p->wDeviceOffset;
		if(dlg.hDevMode && dlg.hDevNames) {
			DEVMODE *pDevMode = (DEVMODE*)::GlobalLock(dlg.hDevMode);
			hdc = CreateDC(driver, device, NULL, pDevMode);
			::GlobalUnlock(dlg.hDevMode);
		}
		else
			hdc = CreateDC(driver, device, NULL, NULL);
		::GlobalUnlock(dlg.hDevNames);
	}
	if(dlg.hDevMode) {
		::GlobalFree(dlg.hDevMode);
		dlg.hDevMode = NULL;
	}
	if(dlg.hDevNames) {
		::GlobalFree(dlg.hDevNames);
		dlg.hDevNames = NULL;
	}
		
	if(hdc) {
		draw = new PrintDraw(hdc, Nvl(имя, Ктрл::дайИмяПрил()));
		page.очисть();
		if(!(dlg.Flags & PD_PAGENUMS)) {
			dlg.nFromPage = dlg.nMinPage;
			dlg.nToPage = dlg.nMaxPage;
		}
		for(int n = 0; n < copies; n++)
			for(int i = dlg.nFromPage - 1; i <= dlg.nToPage - 1; i++)
				page.добавь(i);
		return true;
	}
	return false;
}

bool PrinterJob::выполни()
{
	return Execute0(true);
}

Draw& PrinterJob::GetDraw()
{
	if(!draw) {
		Execute0(false);
		if(!draw)
			draw = new NilDraw;
	}
	return *draw;
}

PrinterJob& PrinterJob::MinMaxPage(int minpage, int maxpage)
{
	from = minpage + 1;
	to = maxpage + 1;
	return *this;
}

PrinterJob& PrinterJob::CurrentPage(int i)
{
	current = i + 1;
	return *this;
}

#endif

#endif

#if (defined(PLATFORM_X11) || defined(PLATFORM_COCOA)) && !defined(VIRTUALGUI)

struct PageSizeName {
	const char *имя;
	int   cx, cy;
	
	Размер  GetDots() const { return Размер(6000 * cx / 254, 6000 * cy / 254); }
}
static const PageName2Size[] = {
	{ "A0", 841, 1189 },
	{ "A1", 594, 841 },
	{ "A2", 420, 594 },
	{ "A3", 297, 420 },
	{ "A4", 210, 297 },
	{ "A5", 148, 210 },
	{ "A6", 105, 148 },
	{ "A7", 74, 105 },
	{ "A8", 52, 74 },
	{ "A9", 37, 52 },
	{ "B0", 1030, 1456 },
	{ "B1", 728, 1030 },
	{ "B10", 32, 45 },
	{ "B2", 515, 728 },
	{ "B3", 364, 515 },
	{ "B4", 257, 364 },
	{ "B5", 182, 257 },
	{ "B6", 128, 182 },
	{ "B7", 91, 128 },
	{ "B8", 64, 91 },
	{ "B9", 45, 64 },
	{ "C5E", 163, 229 },
	{ "Comm10E", 105, 241 },
	{ "DLE", 110, 220 },
	{ "Executive", 191, 254 },
	{ "Folio", 210, 330 },
	{ "Ledger", 432, 279 },
	{ "Legal", 216, 356 },
	{ "Letter", 216, 279 },
	{ "Tabloid", 279, 432 }
};

const PageSizeName *FindPageSize(const Ткст& имя)
{
	for(int i = 0; i < __countof(PageName2Size); i++)
		if(PageName2Size[i].имя == имя)
			return &PageName2Size[i];
	return NULL;
}

Ткст System(const char *cmd, const Ткст& in)
{
	Ткст ofn = дайВремИмяф();
	Ткст ifn = дайВремИмяф();
	сохраниФайл(ifn, in);
	Ткст c = cmd;
	c << " >" << ofn;
	if(in.дайСчёт())
		c << " <" << ifn;
	Ткст q;
	if(system(c) >= 0)
		q = загрузиФайл(ofn);
	удалифл(ofn);
	удалифл(ifn);
	return q;
}

Ткст System(const char *cmd)
{
	return System(cmd, Null);
}

class PrinterDlg : public WithPrinterLayout<ТопОкно> {
	typedef PrinterDlg ИМЯ_КЛАССА;
	
public:
	void FillOpt(const Ткст& s, const char *id, СписокБроса& dl, bool pgsz);
	void SyncPrinterOptions();

	PrinterDlg();
	bool IsCanceled();

private:
	void StandardizePrinterName(Ткст& printerName);
	void OnOK();
	
private:
	bool canceled;
};

void PrinterDlg::FillOpt(const Ткст& s, const char *id, СписокБроса& dl, bool pgsz)
{
	int a = s.найди('/');
	int b = s.найди(':');
	if(a > 0 && b > a) {
		Ткст opt = впроп(s.середина(0, a));
		if(opt == id) {
			Вектор<Ткст> v = разбей(~s + b + 1, ' ');
			dl.вкл();
			for(int i = 0; i < v.дайСчёт(); i++) {
				Ткст o = v[i];
				if(o[0] == '*') {
					o = o.середина(1);
					dl <<= o;
				}
				if(!pgsz || FindPageSize(o))
					dl.добавь(o);
			}
		}
	}
}

void PrinterDlg::SyncPrinterOptions()
{
	Вектор<Ткст> l = разбей(System("lpoptions -d " + Ткст(~printer) + " -l"), '\n');
	paper.откл();
	paper.очисть();
	slot.откл();
	slot.очисть();
	for(int i = 0; i < l.дайСчёт(); i++) {
		FillOpt(l[i], "pagesize", paper, true);
		FillOpt(l[i], "inputslot", slot, false);
	}
}

PrinterDlg::PrinterDlg() : canceled(true)
{
	CtrlLayoutOKCancel(*this, "Print");
	printer <<= THISBACK(SyncPrinterOptions);
	ok <<= THISBACK(OnOK);
	npage.добавь(1);
	npage.добавь(2);
	npage.добавь(4);
	npage.добавь(6);
	npage.добавь(9);
	npage.добавь(16);
	npage <<= 1;
	copies <<= 1;
	landscape <<= 0;
	range <<= 0;
	Вектор<Ткст> l = разбей(System("lpstat -a"), '\n');
	for(int i = 0; i < l.дайСчёт(); i++) {
		Вектор<Ткст> w = разбей(l[i], ' ');
		if(w.дайСчёт())
			printer.добавь(w[0]);
	}
	Ткст h = System("lpstat -d");
	int q = h.найди(':');
	if(q >= 0) {
		Ткст p = h.середина(q + 1);
		StandardizePrinterName(p);
		if(printer.HasKey(p)) {
			printer <<= p;
			SyncPrinterOptions();
			return;
		}
	}
	if(printer.дайСчёт()) {
		printer.SetIndex(0);
		SyncPrinterOptions();
	}
}

bool PrinterDlg::IsCanceled()
{
	return canceled;
}

void PrinterDlg::StandardizePrinterName(Ткст& printerName)
{
	printerName.замени(" ", "");
	printerName.замени("\r", "");
	printerName.замени("\n", "");
}

void PrinterDlg::OnOK()
{
	canceled = false;
	закрой();
}

Размер PrinterJob::GetDefaultPageSize(Ткст *имя)
{
	Размер sz(6000 * 210 / 254, 6000 * 297 / 254);

	Вектор<Ткст> dpp = разбей(System("lpoptions -l"), '\n');

	for (int i = 0; i < dpp.дайСчёт(); i++){
		int pos = max(dpp[i].найдиПосле("Page Размер"), dpp[i].найдиПосле("PageSize"));
		if (pos >= 0){
			pos = dpp[i].найди('*', pos);
			//return A4 if there is not default page size
			if (pos < 0) return sz;
			//skip '*'
			pos++;
			int len = dpp[i].найди(' ', pos);
			if (len < 0) len = dpp[i].дайДлину();
			len -= pos;
			//page имя
			Ткст nm = dpp[i].середина(pos, len);
			if(имя)
				*имя = nm;
			const PageSizeName *p = FindPageSize(nm);
			if(p) {
				sz = p->GetDots();
				return sz;
			}
		}
	}
	//return A4 if there is not default page size
	return sz;
}

PrinterJob::PrinterJob(const char *_name)
{
	имя = _name;
	landscape = false;
	from = to = 0;
	current = 0;
	pgsz = GetDefaultPageSize();
	dlgSuccess = true;
}

PrinterJob::~PrinterJob()
{
}

bool PrinterJob::Execute0()
{
	PrinterDlg dlg;
	dlg.from <<= from + 1;
	dlg.to <<= to + 1;
	dlg.from.мин(from + 1).макс(to + 1);
	dlg.to.мин(from + 1).макс(to + 1);
	dlg.from.вкл(from != to);
	dlg.to.вкл(from != to);
	dlg.range.EnableCase(1, from != to);
	dlg.range.EnableCase(2, from != to);
	dlg.landscape <<= landscape;
	Ткст h;
	GetDefaultPageSize(&h);
	h.пустой() ? dlg.paper <<= "A4" : dlg.paper <<= h;
	
	dlg.пуск();
	if(dlg.IsCanceled())
		return false;
	
	options.очисть();
	options << "-d " << ~dlg.printer;
	options << " -o media=";
	dlg.paper.дайИндекс() < 0 ? options << ~dlg.slot : options << ~dlg.paper << "," << ~dlg.slot;
	landscape = dlg.landscape;
	options << " -o number-up=" << ~dlg.npage;
	options << " -n " << ~dlg.copies;
	if(dlg.collate)
		options << " -o Collate=True";
	page.очисть();
	switch(dlg.range) {
	case 0:
		for(int i = from; i <= to; i++)
			page.добавь(i);
		break;
	case 1:
		page.добавь(current);
		break;
	case 2:
		for(int i = (int)~dlg.from - 1; i <= (int)~dlg.to - 1; i++)
			page.добавь(i);
		break;
	}
	pgsz = Размер(5100, 6600);

	const PageSizeName *p = FindPageSize(~dlg.paper);
	if(p)
		pgsz = p->GetDots();

	return true;
}

bool PrinterJob::выполни()
{
	dlgSuccess = Execute0();
	return dlgSuccess;
}

struct PrinterDraw : PdfDraw {
	Ткст options;
	bool canceled;
	
	PrinterDraw(Размер sz) : PdfDraw(sz), canceled(true) {}
	~PrinterDraw() {
		if(!canceled && !пустой()) {
			System("lp " + options, финиш());
		}
	}
};

Draw& PrinterJob::GetDraw()
{
	if(!draw) {
		PrinterDraw *pd = new PrinterDraw(landscape ? Размер(pgsz.cy, pgsz.cx) : pgsz);
		pd->canceled = !dlgSuccess;
		pd->options = options;
		if(landscape)
			pd->options << " -o landscape";
		draw = pd;
	}
	return *draw;
}

PrinterJob& PrinterJob::MinMaxPage(int minpage, int maxpage)
{
	from = minpage;
	to = maxpage;
	return *this;
}

PrinterJob& PrinterJob::CurrentPage(int i)
{
	current = i;
	return *this;
}

#endif

}
