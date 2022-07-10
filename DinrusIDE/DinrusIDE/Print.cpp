#include "DinrusIDE.h"

class PrinterDlg : public WithPrintLayout<ТопОкно> {
	typedef PrinterDlg ИМЯ_КЛАССА;

public:
	PrinterDlg();
};

PrinterDlg::PrinterDlg()
{
	CtrlLayoutOKCancel(*this, "Печать");
	columns <<= 1;
	orientation <<= 0;
	line <<= 100;
}

ИНИЦБЛОК {
	региструйГлобКонфиг("PrinterDlg");
}

struct Printer {
	Draw& w;
	Размер  page;
	int   cols;
	int   line;
	int   columncx;
	int   cx;
	Шрифт  fnt;
	int   fcx;
	int   fcy;
	int   lines;
	int   tabs;
	int   coli;
	int   y;
	
	void следщСтрочка() {
		y++;
		if(y >= lines) {
			y = 0;
			coli++;
			if(coli > cols) {
				coli = 0;
				w.EndPage();
				w.StartPage();
			}
		}
	}
	
	void PrintLine(const ШТкст& s) {
		int x = 0;
		for(int i = 0; i < s.дайСчёт(); i++) {
			wchar ch = s[i];
			if(x > line) {
				следщСтрочка();
				x = 0;
			}
			if(ch == '\t')
				x = (x + tabs) / tabs * tabs;
			else {
				if(ch != ' ')
					w.DrawText(x * fcx + coli * columncx, y * fcy, &ch, fnt, чёрный, 1);
				x++;
			}
		}
		следщСтрочка();
	}
	
	Printer(PrinterDlg& dlg, Draw& w, int tabs) : w(w), tabs(tabs) {
		cols = ~dlg.columns;
		line = ~dlg.line;
#ifdef SYSTEMDRAW
		page = w.GetPageSize();
#else
		page = w.GetPagePixels();
#endif
		columncx = page.cx / cols;
		cx = page.cx / cols - 100;
		int l = 0;
		int h = 1000;
		while(l < h) {
			int fh = (l + h) / 2;
			if(cx / Courier(fh).Info().дайШирину('x') > line)
				l = fh + 1;
			else
				h = fh;
		}
		fnt = Courier(h);
		FontInfo fi = fnt.Info();
		fcx = fi.дайШирину('x');
		fcy = fi.дайВысоту();
		lines = page.cy / fcy;
		y = coli = 0;
		w.StartPage();
	}
	~Printer() {
		w.EndPage();
	}
};

void Иср::Print()
{
	if(designer)
		return;
	PrinterDlg dlg;
	грузиИзГлоба(dlg, "PrinterDlg");
	int c = dlg.выполни();
	сохраниВГлоб(dlg, "PrinterDlg");
	if(c != IDOK)
		return;
	PrinterJob job;
	job.Landscape(dlg.orientation);
	if(!job.выполни())
		return;
	int l, h;
	if(editor.дайВыделение(l, h)) {
		l = editor.дайСтроку(l);
		h = editor.дайСтроку(h);
	}
	else {
		l = 0;
		h = editor.дайСчётСтрок() - 1;
	}
	Printer p(dlg, job, editor.GetTabSize());
	for(int i = l; i <= h; i++)
		p.PrintLine(editor.дайШСтроку(i));
}
