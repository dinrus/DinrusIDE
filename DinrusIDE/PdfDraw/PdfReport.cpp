#include "PdfDraw.h"

namespace РНЦП {

Ткст Pdf(const Массив<Чертёж>& report, Размер sz, int margin, bool pdfa,
           const PdfSignatureInfo *sign)
{
	PdfDraw w(sz + 2 * margin, pdfa);
	for(int i = 0; i < report.дайСчёт(); i++) {
		w.StartPage();
		w.DrawDrawing(margin, margin, sz.cx, sz.cy, report[i]);
		w.EndPage();
	}
	return w.финиш(sign);
}

ИНИЦИАЛИЗАТОР(PdfDraw)
{
	SetDrawingToPdfFn(Pdf, DrawJPEG);
}

}
