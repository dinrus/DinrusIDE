#include "RichEdit.h"
#include <Painter/Painter.h>

namespace РНЦП {

void RichEdit::InsertImage()
{
	if(!imagefs.ExecuteOpen(t_("открой image from file")))
		return;
	Ткст фн = ~imagefs;
	if(дайДлинуф(фн) > 17000000) {
		Exclamation("Рисунок is too large!");
		return;
	}
	Ткст data = загрузиФайл(фн);
	ТкстПоток ss(data);
	if(!StreamRaster::OpenAny(ss) && !IsSVG(data)) {
		Exclamation(фмт(t_("Unsupported image формат in file [* \1%s\1]."), ~imagefs));
		return;
	}
	RichText clip;
	RichPara p;
	p.конкат(CreateRawImageObject(data), formatinfo);
	clip.конкат(p);
	ClipPaste(clip, "image/raw");
}

bool RichEdit::прими(PasteClip& d, RichText& clip, Ткст& fmt)
{
	if(толькочтен_ли())
		return false;
	if(AcceptFiles(d)) {
		Вектор<Ткст> s = GetFiles(d);
		if(s.дайСчёт()) {
			Ткст фн = s[0];
			Ткст ext = впроп(дайРасшф(фн));
			if(findarg(ext, ".png", ".jpg", ".jpeg", ".gif", ".tif", ".tiff", ".svg") >= 0) {
				if(d.прими() && дайДлинуф(фн) < 17000000) {
					Ткст data = загрузиФайл(фн);
					ТкстПоток ss(data);
					if(StreamRaster::OpenAny(ss) || ext == ".svg" && IsSVG(загрузиФайл(фн))) {
						RichPara p;
						p.конкат(CreateRawImageObject(data), formatinfo);
						clip.конкат(p);
						fmt = "files";
					}
					return true;
				}
				return false;
			}
		}
		d.отклони();
	}
	if(d.прими("image/x-inkscape-svg")) {
		RichPara p;
		p.конкат(CreateRawImageObject(~d), formatinfo);
		clip.конкат(p);
		fmt = "files";
	}
	if(d.прими("text/QTF")) {
		fmt = "text/QTF";
		clip = ParseQTF(~d, 0, context);
		return true;
	}
	if(d.прими(ClipFmtsRTF())) {
		fmt = ClipFmtsRTF();
		clip = ParseRTF(~d);
		return true;
	}
	for(int i = 0; i < RichObject::GetTypeCount(); i++) {
		RichObjectType& rt = RichObject::дайТип(i);
		if(rt.прими(d)) {
			Значение data = rt.читай(d);
			if(!пусто_ли(data)) {
				RichPara p;
				RichObject o = RichObject(&rt, data, pagesz);
				p.конкат(o, formatinfo);
				clip.конкат(p);
				fmt = o.GetTypeName();
			}
			return true;
		}
	}
	if(AcceptText(d)) {
		fmt = "text/plain";
		clip = AsRichText(дайШТкст(d), formatinfo);
		return true;
	}
	return false;
}

void RichEdit::ClipPaste(RichText& clip, const Ткст& fmt)
{
	clip.ApplyZoom(clipzoom.Reciprocal());
	PasteFilter(clip, fmt);
	NextUndo();
	if(clip.GetPartCount() == 1 && clip.IsTable(0)) {
		CancelSelection();
		if(cursorp.table) {
			NextUndo();
			SaveTable(cursorp.table);
			text.PasteTable(cursorp.table, cursorp.cell, clip.GetTable(0));
			финиш();
			return;
		}
	}
	clip.нормализуй();
	PasteText(clip);
}

void RichEdit::тягИБрос(Точка p, PasteClip& d)
{
	int dropcursor = дайПозМыши(p);
	if(dropcursor >= 0) {
		RichText clip;
		Ткст fmt;
		if(прими(d, clip, fmt)) {
			NextUndo();
			int a = sb;
			int c = dropcursor;
			if(InSelection(c)) {
				if(!толькочтен_ли())
					удалиВыделение();
				if(IsDragAndDropSource())
					d.SetAction(DND_COPY);
			}
			int sell, selh;
			if(дайВыделение(sell, selh) && d.GetAction() == DND_MOVE && IsDragAndDropSource()) {
				if(c > sell)
					c -= selh - sell;
				if(!толькочтен_ли())
					удалиВыделение();
				d.SetAction(DND_COPY);
			}
			Move(c);
			clip.нормализуй();
			ClipPaste(clip, fmt);
			sb = a;
			выдели(c, clip.дайДлину());
			устФокус();
			Action();
			return;
		}
	}
	if(!d.IsAccepted())
		dropcursor = -1;
	Прям r = Null;
	if(dropcursor >= 0 && dropcursor < text.дайДлину()) {
		RichCaret pr = text.дайКаретку(dropcursor, pagesz);
		Zoom zoom = GetZoom();
		Прям tr = GetTextRect();
		r = RectC(pr.left * zoom + tr.left - 1,
		          GetPosY(pr) + (pr.lineascent - pr.caretascent) * zoom,
		          2, (pr.caretascent + pr.caretdescent) * zoom);
	}
	if(r != dropcaret) {
		RefreshDropCaret();
		dropcaret = r;
		RefreshDropCaret();
	}
}

void RichEdit::тягПовтори(Точка p)
{
	sb = (int)sb + GetDragScroll(this, p, 16).y;
}

void RichEdit::RefreshDropCaret()
{
	освежи(dropcaret.вертСмещенец(-sb));
}

void RichEdit::Paste()
{
	if(толькочтен_ли())
		return;
	RichText clip;
	PasteClip d = Clipboard();
	Ткст fmt;
	if(!прими(d, clip, fmt))
		return;
	ClipPaste(clip, fmt);
}

void RichEdit::тягПокинь()
{
	RefreshDropCaret();
	dropcaret.очисть();
}

static Ткст sRTF(const Значение& data)
{
	const RichText& txt = ValueTo<RichText>(data);
	return EncodeRTF(txt);
}

static Ткст sQTF(const Значение& data)
{
	const RichText& txt = ValueTo<RichText>(data);
	return AsQTF(txt);
}

void RichEdit::ZoomClip(RichText& text) const
{
	text.ApplyZoom(clipzoom);
}

void AppendClipboard(RichText&& txt)
{
	AppendClipboardUnicodeText(txt.GetPlainText());
	Значение clip = RawPickToValue(pick(txt));
	AppendClipboard("text/QTF", clip, sQTF);
	AppendClipboard(ClipFmtsRTF(), clip, sRTF);
}

void RichEdit::копируй()
{
	RichText txt;
	if(выделение_ли())
		txt = дайВыделение();
	else if(objectpos >= 0)
		txt = text.копируй(cursor, 1);
	else {
		бипВосклицание();
		return;
	}
	ZoomClip(txt);
	ClearClipboard();
	AppendClipboard(pick(txt));
	if(objectpos >= 0) {
		RichObject o = GetObject();
		Вектор<Ткст> v = разбей(o.дайТип().GetClipFmts(), ';');
		for(int i = 0; i < v.дайСчёт(); i++)
			AppendClipboard(v[i], o.дайТип().GetClip(o.дайДанные(), v[i]));
	}
}

Ткст RichEdit::GetSelectionData(const Ткст& fmt) const
{
	Ткст f = fmt;
	if(выделение_ли()) {
		RichText clip = дайВыделение();
		ZoomClip(clip);
		if(f == "text/QTF")
			return AsQTF(clip);
		if(InScList(f, ClipFmtsRTF()))
			return EncodeRTF(clip);
		return GetTextClip(clip.GetPlainText(), fmt);
	}
/*	else
	if(objectpos >= 0) {
		RichObject o = GetObject();
		if(InScList(fmt, o.дайТип().GetClipFmts()))
			return o.дайТип().GetClip(o.дайДанные(), fmt);
	}*/
	return Null;
}

void RichEdit::леваяТяг(Точка p, dword flags)
{
	int c = дайПозМыши(p);
	Размер ssz = StdSampleSize();
	if(!HasCapture() && InSelection(c)) {
		RichText sample = дайВыделение(5000);
		sample.ApplyZoom(Zoom(1, 8));
		ImageDraw iw(ssz);
		iw.DrawRect(0, 0, ssz.cx, ssz.cy, белый);
		sample.рисуй(iw, 0, 0, 128);
		NextUndo();
		if(DoDragAndDrop(Ткст().конкат() << "text/QTF;" << ClipFmtsRTF() << ";" << ClipFmtsText(),
		                 ColorMask(iw, белый)) == DND_MOVE && !толькочтен_ли()) {
			удалиВыделение();
			Action();
		}
	}
/*	else
	if(objectpos >= 0 && c == objectpos) {
		ReleaseCapture();
		RichObject o = GetObject();
		Размер sz = o.GetPhysicalSize();
		NextUndo();
		if(DoDragAndDrop(o.дайТип().GetClipFmts(),
		                 o.ToImage(Размер(ssz.cx, sz.cy * ssz.cx / sz.cx))) == DND_MOVE
		   && objectpos >= 0) {
			if(droppos > objectpos)
				droppos--;
			удали(objectpos, 1);
		}
		Move(droppos);
		SetObjectPos(droppos);
	}*/
}

void  RichEdit::MiddleDown(Точка p, dword flags)
{
	RichText clip;
	if(толькочтен_ли())
		return;
	Ткст fmt;
	if(прими(Selection(), clip, fmt)) {
		selclick = false;
		леваяВнизу(p, flags);
		ClipPaste(clip, fmt);
	}
}

}
