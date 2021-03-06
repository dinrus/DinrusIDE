#include "CodeEditor.h"

namespace РНЦП {

void Renumber(LineInfo& lf)
{
	LineInfo tf;
	int l = 0;
	if(lf.дайСчёт()) {
		LineInfoRecord& t = tf.добавь();
		t.breakpoint = lf[0].breakpoint;
		t.lineno = 0;
		t.count = lf[0].count;
		t.Ошибка = lf[0].Ошибка;
		t.firstedited = lf[0].firstedited;
		t.edited = lf[0].edited;
		l += t.count;
	}
	for(int i = 1; i < lf.дайСчёт(); i++) {
		LineInfoRecord& r = lf[i];
		if(r.breakpoint.пустой() && r.Ошибка == 0 && r.edited == 0 &&
			tf.верх().breakpoint.пустой() && tf.верх().Ошибка == 0 && tf.верх().edited == 0)
			tf.верх().count += r.count;
		else {
			LineInfoRecord& t = tf.добавь();
			t.breakpoint = r.breakpoint;
			t.Ошибка = r.Ошибка;
			t.firstedited = r.firstedited;
			t.edited = r.edited;
			t.count = r.count;
			t.lineno = l;
		}
		l += r.count;
	}
	lf = pick(tf);
}

void ClearBreakpoints(LineInfo& lf)
{
	for(int i = 0; i < lf.дайСчёт(); i++)
		lf[i].breakpoint.очисть();
}

void ValidateBreakpoints(LineInfo& lf)
{
	for(int i = 0; i < lf.дайСчёт(); i++)
		if(lf[i].breakpoint[0] == 0xe)
			lf[i].breakpoint = "1";
}

void EditorBar::sPaintImage(Draw& w, int y, int fy, const Рисунок& img)
{
	w.DrawImage(0, y + (fy - img.дайРазм().cy) / 2, img);
}

void EditorBar::рисуй(Draw& w)
{
	Цвет bg = IsDarkTheme() ? серыйЦвет(70) : SColorLtFace();
	Размер sz = дайРазм();
	w.DrawRect(0, 0, sz.cx, sz.cy, bg);
	if(!editor) return;
	int fy = editor->GetFontSize().cy;
	int hy = fy >> 1;
	int y = 0;
	int i = editor->дайПозПромота().y;
	int cy = дайРазм().cy;
	Ткст hl = editor->дайПодсвет();
	bool hi_if = (hilite_if_endif && findarg(hl, "cpp", "cs", "java") >= 0);
	Вектор<IfState> previf;
	if(hi_if)
		previf <<= editor->GetIfStack(i);
	int ptri[2];
	for(int q = 0; q < 2; q++)
		ptri[q] = ptrline[q] >= 0 ? дайНомСтр(ptrline[q]) : -1;
	while(y < cy) {
		Ткст b;
		int err = 0;
		int edit = 0;
		Ткст ann;
		Рисунок  icon;
		if(i < li.дайСчёт()) {
			const LnInfo& l = li[i];
			b = l.breakpoint;
			err = l.Ошибка;
			edit = l.edited;
			icon = l.icon;
			ann = l.annotation;
		}
		if(editor->дайКаретку().top == y && editor->barline)
			w.DrawRect(0, y, sz.cx, fy, смешай(SColorHighlight(), bg, 200));
		if(line_numbers && i < editor->дайСчётСтрок()) {
			Ткст n = какТкст((i + 1) % 1000000);
			Шрифт fnt = editor->дайШрифт();
			Размер tsz = дайРазмТекста(n, fnt);
			w.DrawText(sz.cx - Zx(4 + 12) - tsz.cx, y + (fy - tsz.cy) / 2, n, fnt, SBrown());
		}
		if(hi_if) {
			Вектор<IfState> nextif;
			if(i < li.дайСчёт())
				nextif <<= editor->GetIfStack(i + 1);
			int pifl = previf.дайСчёт(), nifl = nextif.дайСчёт();
			int dif = max(pifl, nifl);
			if(--dif >= 0) {
				char p = (dif < pifl ? previf[dif].state : 0);
				char n = (dif < nifl ? nextif[dif].state : 0);
				int wd = min(2 * (dif + 1), sz.cx);
				int x = sz.cx - wd;
				Цвет cn = EditorSyntax::IfColor(n);
				if(p == n)
					w.DrawRect(x, y, 1, fy, cn);
				else {
					Цвет cp = EditorSyntax::IfColor(p);
					w.DrawRect(x, y, 1, hy, cp);
					w.DrawRect(x, y + hy, wd, 1, Nvl(cn, cp));
					w.DrawRect(x, y + hy, 1, fy - hy, cn);
					if(--dif >= 0) {
						x = sz.cx - min(2 * (dif + 1), sz.cx);
						if(!p)
							w.DrawRect(x, y, 1, hy, EditorSyntax::IfColor(dif < pifl ? previf[dif].state : 0));
						if(!n)
							w.DrawRect(x, y + hy, 1, fy - hy, EditorSyntax::IfColor(dif < nifl ? nextif[dif].state : 0));
					}
				}
			}
			previf = pick(nextif);
		}
		if(editor->GetMarkLines()) {
			int width = CodeEditorImg::Breakpoint().дайШирину() >> 1;
			if(edit)
			{
				int age = (int)(log((double)(editor->GetUndoCount() + 1 - edit)) * 30);
				w.DrawRect(0, y, width, fy, смешай(SLtBlue(), bg, min(220, age)));
			}
			if(err)
				w.DrawRect(width, y, width, fy, err == 1 ? светлоКрасный() : (err == 2 ? Цвет(255, 175, 0) : SGreen()));
		}

		if(!b.пустой())
			sPaintImage(w, y, fy, b == "1"   ? CodeEditorImg::Breakpoint() :
			                      b == "\xe" ? CodeEditorImg::InvalidBreakpoint() :
			                                   CodeEditorImg::CondBreakpoint());
		for(int q = 0; q < 2; q++)
			if(ptri[q] == i)
				sPaintImage(w, y, fy, ptrimg[q]);

		if(annotations && !пусто_ли(icon))
			w.DrawImage(sz.cx - annotations, y + (fy - icon.дайРазм().cy) / 2, icon);

		y += fy;
		i++;
	}
}

void EditorBar::двигМыши(Точка p, dword flags)
{
	int pa = active_annotation;
	if(p.x > дайРазм().cx - annotations)
		active_annotation = p.y / editor->дайШрифт().Info().дайВысоту() + editor->дайПозПромота().y;
	else
		active_annotation = -1;
	if(active_annotation >= editor->дайСчётСтрок())
		active_annotation = -1;
	if(pa != active_annotation)
		WhenAnnotationMove();
	if(editor)
		editor->двигМыши(Точка(0, p.y), flags);
}

void EditorBar::выходМыши()
{
	int pa = active_annotation;
	active_annotation = -1;
	if(pa != active_annotation)
		WhenAnnotationMove();
}

void EditorBar::леваяВнизу(Точка p, dword flags)
{
	if(p.x > дайРазм().cx - annotations)
		WhenAnnotationClick();
	else
	if(editor) {
		editor->леваяВнизу(Точка(0, p.y), flags);
		ReleaseCtrlCapture();
	}
}

Ткст& EditorBar::PointBreak(int& y)
{
	y = minmax(y / editor->дайШрифт().Info().дайВысоту()
		+ editor->дайПозПромота().y, 0, editor->дайСчётСтрок());
	static Ткст e;
	return y < 100000 ? li.по(y).breakpoint : e;
}

void EditorBar::леваяДКлик(Точка p, dword flags)
{
	if(!editor || !bingenabled) return;
	Ткст& b = PointBreak(p.y);
	if(b.пустой())
		b = "1";
	else
		b.очисть();
	WhenBreakpoint(p.y);
	освежи();
}

void EditorBar::праваяВнизу(Точка p, dword flags)
{
	if(p.x > дайРазм().cx - annotations)
		WhenAnnotationRightClick();
}

void EditorBar::колесоМыши(Точка p, int zdelta, dword keyflags)
{
	if(editor) {
		int i = editor->дайПозПромота().y;
		editor->колесоМыши(p, zdelta, keyflags);
		if(i != editor->дайПозПромота().y)
			двигМыши(p, keyflags);
	}
}

void EditorBar::вставьСтроки(int i, int count)
{
	li.вставьН(minmax(i + 1, 0, li.дайСчёт()), max(count, 0));
	if(editor->GetCheckEdited()) {
		if(editor->IsUndoOp() && li_removed.дайСчёт() >= count) {
			for(int t = 0; t < count; t++) {
				li.по(i + t).firstedited = li_removed[li_removed.дайСчёт() - count + t].firstedited;
				li[i + t].edited = li_removed[li_removed.дайСчёт() - count + t].edited;
			}
			li_removed.сбрось(count);
			SetEdited(i + count, 1);
			ignored_next_edit = true;
		}
		else {
			if (li[i].firstedited == 0) {
				bool fe = li[i].firstedited;
				li.по(i + count).firstedited = fe;
			}
			SetEdited(i + 1, count);
		}
	}
	освежи();
}

void EditorBar::удалиСтроки(int i, int count)
{
	if(editor->GetCheckEdited() && !editor->IsUndoOp()) {
		for(int t = i - 1; t < i + count - 1; t++) {
			LineInfoRemRecord& rm = li_removed.добавь();
			rm.firstedited = li[t].firstedited;
			rm.edited = li[t].edited;
		}
		if(li.по(i + count - 1).firstedited)
			next_age = li[i + count - 1].firstedited;
		else
			next_age = editor->GetUndoCount();
	}
	i = minmax(i, 0, li.дайСчёт());
	li.удали(i, minmax(count, 0, li.дайСчёт() - i));
	освежи();
}

void EditorBar::очистьСтроки()
{
	li.очисть();
	li.сожми();
	li_removed.очисть();
	li_removed.сожми();
	освежи();
}

LineInfo EditorBar::GetLineInfo() const
{
	LineInfo lf;
	int l = -2;
	for(int i = 0; i < li.дайСчёт(); i++) {
		const LnInfo& ln = li[i];
		if(!ln.breakpoint.пустой() || ln.Ошибка || ln.edited) {
			LineInfoRecord& r = lf.добавь();
			r.lineno = ln.lineno;
			r.count = 1;
			r.breakpoint = ln.breakpoint;
			r.Ошибка = ln.Ошибка;
			r.firstedited = ln.firstedited;
			r.edited = ln.edited;
			l = -2;
		}
		else
		if(ln.lineno != l) {
			LineInfoRecord& r = lf.добавь();
			r.lineno = l = ln.lineno;
			r.count = 1;
		}
		else
			lf.верх().count++;
		if(l >= 0) l++;
	}
	return lf;
}

void EditorBar::SetLineInfo(const LineInfo& lf, int total)
{
	li.очисть();
	if(lf.дайСчёт() == 0) {
		for(int i = 0; i < total; i++)
			li.добавь().lineno = i;
	}
	else {
		for(int i = 0; i < lf.дайСчёт() && (total < 0 || li.дайСчёт() < total); i++) {
			const LineInfoRecord& r = lf[i];
			int l = r.lineno;
			for(int j = r.count; j-- && li.дайСчёт() < total;) {
				LnInfo& ln = li.добавь();
				ln.lineno = l;
				ln.breakpoint = r.breakpoint;
				ln.Ошибка = r.Ошибка;
				ln.firstedited = r.firstedited;
				ln.edited = r.edited;
				if(l >= 0) l++;
			}
		}
		while(li.дайСчёт() < total)
			li.добавь().lineno = -1;
	}
}

void EditorBar::Renumber(int linecount)
{
	li.устСчёт(linecount);
	for(int i = 0; i < linecount; i++)
		li[i].lineno = i;
}

void EditorBar::ClearBreakpoints()
{
	for(int i = 0; i < li.дайСчёт(); i++)
		li[i].breakpoint.очисть();
	освежи();
}

void EditorBar::ValidateBreakpoints()
{
	for(int i = 0; i < li.дайСчёт(); i++)
		if(li[i].breakpoint[0] == 0xe)
			li[i].breakpoint = "1";
	освежи();
}

Ткст EditorBar::GetBreakpoint(int ln)
{
	return ln < li.дайСчёт() ? li[ln].breakpoint : Null;
}

void EditorBar::ClearAnnotations()
{
	for(int i = 0; i < li.дайСчёт(); i++) {
		li[i].icon.очисть();
		li[i].annotation.очисть();
	}
}

void EditorBar::SetAnnotation(int line, const Рисунок& img, const Ткст& ann)
{
	if(line >= 0 && line < li.дайСчёт()) {
		li[line].icon = img;
		li[line].annotation = ann;
	}
	освежи();
}

Ткст EditorBar::GetAnnotation(int line) const
{
	return line >= 0 && line < li.дайСчёт() ? li[line].annotation : Ткст();
}

void EditorBar::SetBreakpoint(int ln, const Ткст& s)
{
	li.по(ln).breakpoint = s;
	WhenBreakpoint(ln);
}

void EditorBar::SetEdited(int ln, int count)
{
	if(ignored_next_edit) {
		ignored_next_edit = false;
		return;
	}
	int age = editor->GetUndoCount() + 1;
	bool undo = editor->IsUndoOp();
	for(int i = 0; i < count; i++) {
		if(undo) {
			if (li.по(ln + i).firstedited >= age - 1) {
				li[ln + i].firstedited = 0;
				li[ln + i].edited = 0;
			}
		}
		else {
			if(next_age) {
				li[ln + i].firstedited = next_age;
				li[ln + i].edited = age;
				next_age = 0;
			}
			else {
				if(li.по(ln + i).firstedited == 0)
					li[ln + i].firstedited = age;
				li[ln + i].edited = age;
			}
		}
	}
	освежи();
}

void EditorBar::ClearEdited()
{
	for(int i = 0; i < li.дайСчёт(); i++) {
		li.по(i).firstedited = 0;
		li[i].edited = 0;
	}
	li_removed.очисть();
	li_removed.сожми();
	освежи();
}

void EditorBar::устОш(int ln, int err)
{
	li.по(ln).Ошибка = err;
}

void EditorBar::очистьОшибки(int line)
{
	int count;
	if(line < 0) {
		line = 0;
		count = li.дайСчёт();
	}
	else
	if(line >= li.дайСчёт())
		return;
	else
		count = line + 1;

	for(int i = line; i < count; i++)
		li[i].Ошибка = 0;
}

int  EditorBar::дайНомСтр(int lineno) const {
	for(int i = 0; i < li.дайСчёт(); i++) {
		if(lineno <= li[i].lineno)
			return i;
	}
	return lineno;
}

int  EditorBar::GetNoLine(int line) const {
	int n = 0;
	for(int i = 0; i < li.дайСчёт(); i++) {
		if(li[i].lineno >= 0)
			n = li[i].lineno;
		if(i == line) return n;
	}
	return n;
}

void EditorBar::устУк(int line, const Рисунок& img, int i)
{
	ПРОВЕРЬ(i >= 0 && i < 2);
	ptrline[i] = line;
	ptrimg[i] = img;
	освежи();
}

void EditorBar::HidePtr()
{
	ptrline[0] = ptrline[1] = -1;
	освежи();
}

void EditorBar::SyncSize()
{
	int n = editor ? editor->дайСчётСтрок() : 0;
	int i = 0;
	while(n) {
		i++;
		n /= 10;
	}
	int w = (line_numbers && editor ? editor->дайШрифт()['0'] * i : 0) + Zx(12 + 4) + annotations;
	if(w != дайШирину())
		устШирину(w);
	освежи();
}

void EditorBar::LineNumbers(bool b)
{
	line_numbers = b;
	SyncSize();
}

void EditorBar::Annotations(int width)
{
	annotations = width;
	SyncSize();
}

EditorBar::EditorBar()
{
	editor = NULL;
	line_numbers = false;
	bingenabled = true;
	hilite_if_endif = true;
	line_numbers = false;
	annotations = 0;
	ignored_next_edit = false;
	next_age = 0;
	active_annotation = -1;
	SyncSize();
}

EditorBar::~EditorBar()
{
}

void очистьОшибки(LineInfo& li)
{
	for(int i = 0; i < li.дайСчёт(); i++)
		li[i].Ошибка = 0;
}

void устОш(LineInfo& li, int line, int err)
{
	li.по(line).Ошибка = err;
}

}
