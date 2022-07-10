#ifndef _Report_Report_h
#define _Report_Report_h

#include <CtrlLib/CtrlLib.h>
#include <RichText/RichText.h>

namespace РНЦП {

#define  LAYOUTFILE <Report/Report.lay>
#include <CtrlCore/lay.h>

class Report : public DrawingDraw, public PageDraw {
public:
	virtual Draw& Page(int i);
	virtual Размер  GetPageSize() const    { return DrawingDraw::GetPageSize(); } // avoid CLANG warning
	virtual void  StartPage()            { DrawingDraw::StartPage(); } // avoid CLANG warning

private:
	Массив<Чертёж>      page;
	int                 pagei;
	int                 y;
	Ткст              header, footer;
	int                 headercy, headerspc, footercy, footerspc;
	Точка               mg;
	Один<PrinterJob>     printerjob;

	void    слей();
	Ткст  FormatHF(const char *s, int pageno);
	int     GetHeightHF(const char *s);
	void    PaintHF(Draw& w, int y, const char *qtf, int i);
	void    StartPage(int i);
	void    RestartPage();

	Callback              WhenPage;
	
public:

	int                   дайСчёт()                  { слей(); return page.дайСчёт(); }
	Чертёж               дайСтраницу(int i)              { слей(); return page[i]; }
	Чертёж               operator[](int i)           { return дайСтраницу(i); }
	const Массив<Чертёж>& GetPages()                  { слей(); return page; }

	void                  очисть();

	Прям                  GetPageRect();
	Размер                  GetPageSize()               { return GetPageRect().размер(); }

	void                  устУ(int _y)                { y = _y; }
	int                   дайУ() const                { return y; }

	void                  NewPage()                   { Page(pagei + 1); }
	void                  RemoveLastPage()            { if(page.дайСчёт()) page.сбрось(); pagei = -1; }

	void                  помести(const RichText& txt, void *context = NULL);
	void                  помести(const char *qtf);
	Report&               operator<<(const char *qtf) { помести(qtf); return *this; }

	void                  SetRichTextLayoutTracer(RichTextLayoutTracer& l) { tracer = &l; }

	Точка                 GetMargins() const          { return mg; }

	bool                  ChoosePrinter(const char *jobname = t_("Report"));
	bool                  ChooseDefaultPrinter(const char *jobname = t_("Report"));
	PrinterJob           *GetPrinterJob()             { return ~printerjob; }

	Report&               SetPageSize(Размер sz);
	Report&               SetPageSize(int cx, int cy) { return SetPageSize(Размер(cx, cy)); }
	Report&               Landscape();
	Report&               Margins(int top, int left)  { mg.y = top; mg.x = left; return *this; }
	Report&               Margins(int m)              { return Margins(m, m); }
	Report&               Header(const char *qtf, int spc = 150);
	Report&               Footer(const char *qtf, int spc = 150);
	Report&               OnPage(Callback whenpage);
	Report&               NoHeader()                  { return Header(NULL, 0); }
	Report&               NoFooter()                  { return Footer(NULL, 0); }

	Report();
	Report(int cx, int cy);
	Report(const Размер &sz);
};

class ReportView : public Ктрл {
public:
	virtual void рисуй(Draw& w);
	virtual void Выкладка();
	virtual bool Ключ(dword ключ, int);
	virtual void леваяВнизу(Точка p, dword);
	virtual void колесоМыши(Точка, int zdelta, dword);

protected:
	ПромотБар sb;
	Report   *report;
	Рисунок     page[64];
	int       pagei[64];
	Размер      pagesize;
	int       vsize;

	int       pm;
	int       pvn;
	bool      numbers;
	int       pages;

	Рисунок     дайСтраницу(int i);

	void      иниц();
	void      Sb();
	void      Numbers()               { освежи(); }
	Размер      GetReportSize();

public:
	Callback  WhenGoPage;

	enum Pages {
		PG1, PG2, PG4, PG16
	};
	ReportView& Pages(int pags)       { pages = pags; Выкладка(); return *this; }
	ReportView& Numbers(bool nums)    { numbers = nums; освежи(); return *this; }

	void      уст(Report& report);
	Report   *дай()                   { return report; }
	int       дайПерв() const        { return sb / pagesize.cy * pvn; }

	void      промотайДо(int toppage, int top, int bottompage, int bottom);

	typedef ReportView ИМЯ_КЛАССА;

	ReportView();
};

class ReportWindow : public WithReportWindowLayout<ТопОкно> {
	void Pages()   { pg.Pages(~sw); }
	void Numbers() { pg.Numbers(numbers); }
	void GoPage()  { sw <<= ReportView::PG1; Pages(); }
	void Pdf();
	void ShowPage();

	Массив<Кнопка>         button;
	Report               *report;

public:
	ReportView pg;

	typedef ReportWindow ИМЯ_КЛАССА;

	static void SetPdfRoutine(Ткст (*pdf)(const Report& report, int margin));

	void SetButton(int i, const char *label, int id);

	int  Perform(Report& report, int zoom = 100, const char *caption = t_("Report"));

	ReportWindow();
};

Ткст Pdf(Report& report, bool pdfa = false, const PdfSignatureInfo *sign = NULL);
void   Print(Report& r, PrinterJob& pd);
bool   DefaultPrint(Report& r, int i, const char *_name = t_("Report"));
bool   Print(Report& r, int i, const char *имя = t_("Report"));
bool   Perform(Report& r, const char *имя = t_("Report"));
bool   QtfReport(const Ткст& qtf, const char *имя = "", bool pagenumbers = false);
bool   QtfReport(Размер pagesize, const Ткст& qtf, const char *имя = "", bool pagenumbers = false);

}

#endif
