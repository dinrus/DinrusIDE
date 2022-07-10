#include "Report.h"

namespace РНЦП {

#define LLOG(x)  // DLOG(x)

Report::Report()
{
	mg.x = mg.y = Null;
	очисть();
}

Report::Report(int cx, int cy)
{
	mg.x = mg.y = Null;
	SetPageSize(cx, cy);
}

Report::Report(const Размер &sz)
{
	mg.x = mg.y = Null;
	SetPageSize(sz);
}

Report& Report::SetPageSize(Размер sz)
{
	page.очисть();
	header = footer = Null;
	headercy = footercy = headerspc = footerspc = 0;
	pagei = -1;
	создай(sz);
	Page(0);
	создай(sz);
	StartPage(0);
	return *this;
}

void Report::очисть()
{
	SetPageSize(3968, 6074);
}

Прям Report::GetPageRect()
{
	Прям r = дайРазм();
	r.top += headercy + headerspc;
	r.bottom -= footercy + footerspc;
	return r;
}

void Report::PaintHF(Draw& w, int y, const char *qtf, int i)
{
	RichText txt = ParseQTF(FormatHF(qtf, i));
	PaintInfo pi; // need pi so that darktheme is false
	txt.рисуй(w, 0, y, дайРазм().cx, pi);
}

void Report::слей()
{
	if(pagei >= 0) {
		Чертёж dw = дайРез();
		page.по(pagei).приставь(dw);
		создай(дайРазм());
	}
}

void Report::StartPage(int i)
{
	DrawingDraw dw(дайРазм());
	page.по(i) = dw;
	LLOG("старт page " << i);
	создай(дайРазм());
	WhenPage();
	LLOG("рисуй header");
	PaintHF(*this, 0, header, i);
	LLOG("рисуй footer");
	PaintHF(*this, дайРазм().cy - footercy, footer, i);
	y = GetPageRect().top;
}

Draw& Report::Page(int i)
{
	ПРОВЕРЬ(i >= 0);
	if(i != pagei) {
		слей();
		pagei = i;
		while(page.дайСчёт() <= pagei)
			StartPage(page.дайСчёт());
		y = GetPageRect().top;
	}
	return *this;
}

Ткст  Report::FormatHF(const char *s, int pageno)
{
	Ткст result;
	while(*s) {
		if(s[0] == '$' && s[1] == '$') {
			if(s[2] == 'P') {
				result << pageno + 1;
				s += 3;
			}
			else
			if(s[2] == 'D') {
				result.конкат(фмт(дайСисДату()));
				s += 3;
			}
		}
		result.конкат(*s++);
	}
	return result;
}

int Report::GetHeightHF(const char *s)
{
	RichText txt = ParseQTF(FormatHF(s, 9999));
	return txt.дайВысоту(дайРазм().cx);
}

Report& Report::Header(const char *qtf, int spc)
{
	header = qtf;
	headerspc = spc;
	headercy = qtf ? GetHeightHF(qtf) : 0;
	RestartPage();
	return *this;
}

Report& Report::Footer(const char *qtf, int spc)
{
	footer = qtf;
	footerspc = spc;
	footercy = qtf ? GetHeightHF(qtf) : 0;
	RestartPage();
	return *this;
}

Report& Report::OnPage(Callback whenpage)
{
	WhenPage = whenpage;
	RestartPage();
	return *this;
}

void Report::RestartPage()
{
	page.устСчёт(pagei + 1);
	StartPage(pagei);
}

Report& Report::Landscape()
{
	Размер sz = дайРазм();
	SetPageSize(sz.cy, sz.cx);
	return *this;
}

void Report::помести(const RichText& txt, void *context)
{
	PageY py(pagei, y);
	LLOG("помести RichText, py: " << py << ", pagerect: " << GetPageRect());
	PaintInfo paintinfo;
	paintinfo.top = PageY(0, 0);
	paintinfo.bottom = PageY(INT_MAX, INT_MAX);
	paintinfo.indexentry = Null;
	paintinfo.hyperlink = Null;
	paintinfo.context = context;
	txt.рисуй(*this, py, GetPageRect(), paintinfo);
	py = txt.дайВысоту(py, GetPageRect());
	LLOG("Final pos: " << py);
	Page(py.page);
	y = py.y;
}

void Report::помести(const char *qtf)
{
	помести(ParseQTF(qtf));
}

bool Report::ChoosePrinter(const char *jobname)
{
	printerjob.создай();
	printerjob->Имя(jobname);
	if(!printerjob->выполни()) {
		printerjob.очисть();
		return false;
	}
	SetPageSize(printerjob->GetDraw().GetPageSize());
	return true;
}

bool Report::ChooseDefaultPrinter(const char *jobname)
{
	printerjob.создай();
	printerjob->Имя(jobname);
	Размер sz = printerjob->GetDraw().GetPageSize();
	if(sz.cx == 0 || sz.cy == 0)
		return false;
	SetPageSize(sz);
	return true;
}

}
