#include "RichEdit.h"

namespace РНЦП {

Размер RichEdit::GetPhysicalSize(const RichObject& obj)
{
	if(ignore_physical_size)
		return 600 * obj.GetPixelSize() / 96;
	return obj.GetPhysicalSize();
}

void RichEdit::режимОтмены()
{
	tabmove.table = 0;
	selclick = false;
	dropcaret.очисть();
}

void RichEdit::колесоМыши(Точка p, int zdelta, dword keyflags)
{
	if(keyflags == K_CTRL) {
		if(пусто_ли(floating_zoom))
			ZoomView(зн(zdelta));
		else {
			floating_zoom = minmax(floating_zoom + zdelta / 480.0, 0.5, 10.0);
			RefreshLayoutDeep();
		}
	}
	else
		sb.Wheel(zdelta);
}

RichHotPos RichEdit::GetHotPos(Точка p)
{
	int x;
	PageY py;
	GetPagePoint(p, py, x);
	return text.GetHotPos(x, py, 4 / GetZoom(), pagesz);
}

void RichEdit::SetObjectPercent(int p)
{
	if(objectpos >= 0) {
		RichObject obj = GetObject();
		Размер sz = GetPhysicalSize(obj) * p / 100;
		if(sz.cx > 0 && sz.cy > 0) {
			obj.устРазм(sz);
			obj.KeepRatio(true);
			ReplaceObject(obj);
		}
	}
}

void RichEdit::SetObjectYDelta(int pt)
{
	if(objectpos >= 0) {
		RichObject obj = GetObject();
		obj.SetYDelta(pt * 25 / 3);
		ReplaceObject(obj);
	}
}

void RichEdit::SetObjectPos(int pos)
{
	Прям r = GetObjectRect(cursor);
	Прям rr = r.смещенец(GetTextRect().left, -sb);
	objectrect = GetObjectRect(pos);
	objectpos = cursor;
	поместиКаретку();
	освежи(rr);
	ReadFormat();
}

void RichEdit::леваяВнизу(Точка p, dword flags)
{
	useraction = true;
	NextUndo();
	устФокус();
	selclick = false;
	tabmove = GetHotPos(p);
	if(tabmove.table && tabmove.column >= -2) {
		SaveTableFormat(tabmove.table);
		SetCapture();
		Move(text.GetCellPos(tabmove.table, 0, max(tabmove.column, 0)).pos);
		return;
	}
	int c = GetHotSpot(p);
	if(c >= 0 && objectpos >= 0) {
		int pos = objectpos;
		ПрямТрэкер tracker(*this);
		RichObject obj = text.GetRichPos(pos).object;
		tracker.минРазм(Размер(16, 16))
		       .максРазм(GetZoom() * pagesz)
		       .Animation()
		       .Dashed()
		       .KeepRatio(obj.IsKeepRatio());
		int tx, ty;
		switch(c) {
		case 1:
			tracker.SetCursorImage(Рисунок::SizeVert());
			tx = ALIGN_CENTER; ty = ALIGN_BOTTOM;
			break;
		case 2:
			tracker.SetCursorImage(Рисунок::SizeHorz());
			tx = ALIGN_RIGHT; ty = ALIGN_CENTER;
			break;
		default:
			tracker.SetCursorImage(Рисунок::SizeBottomRight());
			tx = ALIGN_RIGHT; ty = ALIGN_RIGHT;
			break;
		}
		double zoom = GetZoom().AsDouble();
		Размер sz = obj.дайРазм();
		sz.cx = int(zoom * sz.cx + 0.5);
		sz.cy = int(zoom * sz.cy + 0.5);
		sz = tracker.Track(Прям(objectrect.смещенец(GetTextRect().left, -sb).верхЛево(), sz), tx, ty).размер();
		sz.cx = int(sz.cx / zoom + 0.5);
		sz.cy = int(sz.cy / zoom + 0.5);
		obj.устРазм(sz);
		ReplaceObject(obj);
	}
	else {
		c = дайПозМыши(p);
		if(c >= 0) {
			if(InSelection(c)) {
				selclick = true;
				return;
			}
			Move(c, flags & K_SHIFT);
			mpos = c;
			SetCapture();
			if(cursorp.object && GetObjectRect(cursor).смещенец(GetTextRect().left, -sb).содержит(p))
				SetObjectPos(cursor);
		}
	}
}

void RichEdit::леваяВверху(Точка p, dword flags)
{
	useraction = true;
	NextUndo();
	int c = дайПозМыши(p);
	int d = c;
	if(!HasCapture() && InSelection(d) && selclick) {
		CancelSelection();
		Move(c);
	}
	selclick = false;
}

void RichEdit::двигМыши(Точка p, dword flags)
{
	useraction = true;
	if(HasCapture() && (flags & K_MOUSELEFT)) {
		if(tabmove.table && tabmove.column >= 0) {
			RichTable::фмт fmt = text.GetTableFormat(tabmove.table);
			if(tabmove.column >= fmt.column.дайСчёт() - 1)
				return;
			int sum = сумма(fmt.column);
			int nl = 0;
			for(int i = 0; i < tabmove.column; i++)
				nl += fmt.column[i];
			int xn = fmt.column[tabmove.column] + fmt.column[tabmove.column + 1];
			int xl = tabmove.left + tabmove.cx * nl / sum + 12;
			int xh = tabmove.left + tabmove.cx * (nl + xn) / sum - 12;
			if(xl >= xh)
				return;
			int xx = minmax(GetSnapX(p.x) - tabmove.delta, xl, xh) - tabmove.left;
			fmt.column[tabmove.column] = xx * sum / tabmove.cx - nl;
			fmt.column[tabmove.column + 1] = xn - fmt.column[tabmove.column];
			text.SetTableFormat(tabmove.table, fmt);
			финиш();
		}
		else
		if(tabmove.table && tabmove.column == RICHHOT_LM) {
			RichTable::фмт fmt = text.GetTableFormat(tabmove.table);
			fmt.rm = clamp(fmt.rm, 0, tabmove.textcx - fmt.lm - 120);
			fmt.lm = clamp(GetSnapX(p.x) - tabmove.textleft, 0, max(tabmove.textcx - fmt.rm - 120, 0));
			text.SetTableFormat(tabmove.table, fmt);
			финиш();
		}
		else
		if(tabmove.table && tabmove.column == RICHHOT_RM) {
			RichTable::фмт fmt = text.GetTableFormat(tabmove.table);
			fmt.lm = clamp(fmt.lm, 0, max(tabmove.textcx - fmt.rm - 120, 0));
			fmt.rm = minmax(tabmove.textcx - GetSnapX(p.x) + tabmove.textleft, 0, tabmove.textcx - fmt.lm - 120);
			text.SetTableFormat(tabmove.table, fmt);
			финиш();
		}
		else {
			PageY py = GetPageY(p.y + sb);
			int c;
			if(py > text.дайВысоту(pagesz))
				c = дайДлину() + 1;
			else
				c = GetNearestPos(дайХ(p.x), py);
			if(c != mpos) {
				mpos = -1;
				Move(c, true);
			}
		}
	}
}

static bool IsObjectPercent(РазмерПЗ percent, int p)
{
	return fabs(percent.cx - p) < 1 && fabs(percent.cy - p) < 1;
}

static bool IsObjectDelta(int delta, int d)
{
	return d * 25 / 3 == delta;
}

void RichEdit::StdBar(Бар& menu)
{
	int l, h;
	Ид field;
	Ткст fieldparam;
	Ткст ofieldparam;
	RichObject object;
	if(дайВыделение(l, h)) {
		CutTool(menu);
		CopyTool(menu);
		PasteTool(menu);
	}
	else {
		if(objectpos >= 0) {
			bar_object = GetObject();
			if(!bar_object) return;
			bar_object.Menu(menu, context);
			if(!menu.пустой())
				menu.Separator();
			Размер sz = GetPhysicalSize(bar_object);
			РазмерПЗ percent = bar_object.дайРазм();
			percent = 100.0 * percent / РазмерПЗ(sz);
			bool b = sz.cx || sz.cy;
			menu.добавь(t_("Object position.."), THISBACK(AdjustObjectSize));
			menu.Separator();
			menu.добавь(b, "20 %", THISBACK1(SetObjectPercent, 20)).Check(IsObjectPercent(percent, 20));
			menu.добавь(b, "30 %", THISBACK1(SetObjectPercent, 30)).Check(IsObjectPercent(percent, 30));
			menu.добавь(b, "40 %", THISBACK1(SetObjectPercent, 40)).Check(IsObjectPercent(percent, 40));
			menu.добавь(b, "50 %", THISBACK1(SetObjectPercent, 50)).Check(IsObjectPercent(percent, 50));
			menu.добавь(b, "60 %", THISBACK1(SetObjectPercent, 60)).Check(IsObjectPercent(percent, 60));
			menu.добавь(b, "70 %", THISBACK1(SetObjectPercent, 70)).Check(IsObjectPercent(percent, 70));
			menu.добавь(b, "80 %", THISBACK1(SetObjectPercent, 80)).Check(IsObjectPercent(percent, 80));
			menu.добавь(b, "90 %", THISBACK1(SetObjectPercent, 90)).Check(IsObjectPercent(percent, 90));
			menu.добавь(b, "100 %", THISBACK1(SetObjectPercent, 100)).Check(IsObjectPercent(percent, 100));
			menu.Break();
			int delta = bar_object.GetYDelta();
			menu.добавь(t_("3 pt up"), THISBACK1(SetObjectYDelta, -3)).Check(IsObjectDelta(delta, -3));
			menu.добавь(t_("2 pt up"), THISBACK1(SetObjectYDelta, -2)).Check(IsObjectDelta(delta, -2));
			menu.добавь(t_("1 pt up"), THISBACK1(SetObjectYDelta, -1)).Check(IsObjectDelta(delta, -1));
			menu.добавь(t_("Baseline"), THISBACK1(SetObjectYDelta, 0)).Check(IsObjectDelta(delta, 0));
			menu.добавь(t_("1 pt down"), THISBACK1(SetObjectYDelta, 1)).Check(IsObjectDelta(delta, 1));
			menu.добавь(t_("2 pt down"), THISBACK1(SetObjectYDelta, 2)).Check(IsObjectDelta(delta, 2));
			menu.добавь(t_("3 pt down"), THISBACK1(SetObjectYDelta, 3)).Check(IsObjectDelta(delta, 3));
			menu.Separator();
			CopyTool(menu);
			CutTool(menu);
		}
		else {
			RichPos p = cursorp;
			field = p.field;
			bar_fieldparam = p.fieldparam;
			RichPara::FieldType *ft = RichPara::fieldtype().дай(field, NULL);
			if(ft) {
				ft->Menu(menu, &bar_fieldparam);
				if(!menu.пустой())
					menu.Separator();
				CopyTool(menu);
				CutTool(menu);
			}
			else {
				ШТкст w = GetWordAtCursor();
				if(!w.пустой() && !SpellWord(w, w.дайДлину(),
				                              fixedlang ? fixedlang : formatinfo.language)) {
					if(true) {
						Вектор<Ткст> h = SpellerFindCloseWords(fixedlang ? fixedlang : formatinfo.language, w.вТкст(), 10);
						for(Ткст s : h)
							menu.добавь(s, [=] {
								int pos, count;
								GetWordAtCursorPos(pos, count);
								if(count) {
									удали(pos, count);
									ШТкст h = s.вШТкст();
									вставь(pos, AsRichText(h, formatinfo));
									Move(pos + h.дайСчёт());
									финиш();
								}
							});
					}
					menu.добавь(t_("добавь to user dictionary"), THISBACK(AddUserDict));
					menu.Separator();
				}
				PasteTool(menu);
				ObjectTool(menu);
			}
		}
		LoadImageTool(menu);
	}
}

void RichEdit::праваяВнизу(Точка p, dword flags)
{
	useraction = true;
	NextUndo();
	БарМеню menu;
	int l, h;
	Прям ocr = GetCaretRect();
	int fieldpos = -1;
	Ид field;
	Ткст ofieldparam;
	RichObject o;
	bar_object.очисть();
	bar_fieldparam = Null;
	if(!дайВыделение(l, h)) {
		леваяВнизу(p, flags);
		if(objectpos >= 0)
			o = bar_object = GetObject();
		else {
			RichPos p = cursorp;
			field = p.field;
			bar_fieldparam = p.fieldparam;
			RichPara::FieldType *ft = RichPara::fieldtype().дай(field, NULL);
			if(ft) {
				ofieldparam = bar_fieldparam;
				fieldpos = cursor;
			}
		}
	}
	WhenBar(menu);
	Прям r = GetCaretRect();
	освежи(r);
	освежи(ocr);
	paintcarect = true;
	menu.выполни();
	paintcarect = false;
	освежи(r);
	if(bar_object && o && o.GetSerialId() != bar_object.GetSerialId())
		ReplaceObject(bar_object);
	if(fieldpos >= 0 && bar_fieldparam != ofieldparam) {
		RichText::FormatInfo f = text.GetFormatInfo(fieldpos, 1);
		удали(fieldpos, 1);
		RichPara p;
		p.конкат(field, bar_fieldparam, f);
		RichText clip;
		clip.конкат(p);
		вставь(fieldpos, clip, false);
		финиш();
	}
	bar_object.очисть();
	bar_fieldparam = Null;
}

void RichEdit::леваяДКлик(Точка p, dword flags)
{
	NextUndo();
	int c = дайПозМыши(p);
	if(c >= 0) {
		if(objectpos == c) {
			RichObject object = GetObject();
			if(!object) return;
			RichObject o = object;
			o.DefaultAction(context);
			if(object.GetSerialId() != o.GetSerialId())
				ReplaceObject(o);
		}
		else {
			RichPos rp = cursorp;
			RichPara::FieldType *ft = RichPara::fieldtype().дай(rp.field, NULL);
			if(ft) {
				int fieldpos = cursor;
				ft->DefaultAction(&rp.fieldparam);
				RichText::FormatInfo f = text.GetFormatInfo(fieldpos, 1);
				удали(fieldpos, 1);
				RichPara p;
				p.конкат(rp.field, rp.fieldparam, f);
				RichText clip;
				clip.конкат(p);
				вставь(fieldpos, clip, false);
				финиш();
			}
			else {
				int64 l, h;
				if(GetWordSelection(c, l, h))
					устВыделение((int)l, (int)h);
			}
		}
	}
}

void RichEdit::LeftTriple(Точка p, dword flags)
{
	NextUndo();
	int c = дайПозМыши(p);
	if(c >= 0 && c != objectpos) {
		RichPos rp = text.GetRichPos(c);
		выдели(c - rp.posinpara, rp.paralen + 1);
	}
}

Рисунок RichEdit::рисКурсора(Точка p, dword flags)
{
	if(tablesel)
		return Рисунок::Arrow();

	switch(GetHotSpot(p)) {
	case 0:
		return Рисунок::SizeBottomRight();
	case 1:
		return Рисунок::SizeVert();
	case 2:
		return Рисунок::SizeHorz();
	default:
		if(text.GetRichPos(дайПозМыши(p)).object) {
			return Рисунок::Arrow();
		}
		else
		if(HasCapture() && tabmove.table && tabmove.column >= -2)
			return Рисунок::SizeHorz();
		else {
			RichHotPos hp = GetHotPos(p);
			if(hp.table > 0)
				return Рисунок::SizeHorz();
			else {
				int c = дайПозМыши(p);
				return InSelection(c) && !HasCapture() ? Рисунок::Arrow() : Рисунок::IBeam();
			}
		}
	}
	return Рисунок::Arrow();
}

void RichEdit::леваяПовтори(Точка p, dword flags)
{
	NextUndo();
	if(HasCapture() && (flags & K_MOUSELEFT)) {
		if(p.y < 0)
			MoveUpDown(-1, true);
		if(p.y > дайРазм().cy)
			MoveUpDown(1, true);
	}
}

}
