#include "Local.h"

NAMESPACE_UPP

struct DrawDragRectInfo {
	Прям  rect1, rect2, clip;
	int   n;
	int   тип;
	int   animation;
};

void DrawDragLine(SystemDraw& w, bool horz, int x, int y, int len, int n, const int *pattern, int animation)
{
	if(len <= 0)
		return;
	if(horz)
		w.Clip(x, y, len, n);
	else
		w.Clip(x, y, n, len);
	
	(horz ? x : y) -= animation;
	len += animation;
	bool ch = false;
	while(len > 0) {
		int segment = pattern[ch];
		int d = segment + pattern[2];
		if(horz) {
			w.DrawRect(x, y, segment, n, InvertColor());
			x += d;
		}
		else {
			w.DrawRect(x, y, n, segment, InvertColor());
			y += d;
		}
		len -= d;
		ch = !ch;
	}
	w.стоп();
}

void DrawDragFrame(SystemDraw& w, const Прям& r, int n, const int *pattern, int animation)
{
	DrawDragLine(w, true, r.left, r.top, r.дайШирину(), n, pattern, animation);
	DrawDragLine(w, false, r.left, r.top + n, r.дайВысоту() - 2 * n, n, pattern, animation);
	DrawDragLine(w, false, r.right - n, r.top + n, r.дайВысоту() - 2 * n, n, pattern, animation);
	DrawDragLine(w, true, r.left, r.bottom - n, r.дайШирину(), n, pattern, animation);
}

void DrawDragRect(Ктрл& q, const DrawDragRectInfo& f)
{
	SystemDraw& w = VirtualGuiPtr->BeginDraw();
	Ктрл::PaintScene(w);
	w.Clip(f.clip);
	static int dashes[3][3] = {
		{ 32, 32, 0 },
		{ 1, 1, 1 },
		{ 5, 1, 2 },
	};
	const int *dash = dashes[minmax(f.тип, 0, 2)];
	DrawDragFrame(w, f.rect1, f.n, dash, f.animation);
	DrawDragFrame(w, f.rect2, f.n, dash, f.animation);
	w.стоп();
	Ктрл::PaintCaretCursor(w);
	VirtualGuiPtr->CommitDraw();
}

void DrawDragRect(Ктрл& q, const Прям& rect1, const Прям& rect2, const Прям& clip, int n,
                  Цвет color, int тип, int animation)
{
	Ктрл *top = q.дайТопКтрл();
	if(top) {
		Точка off = q.GetScreenView().верхЛево();
		DrawDragRectInfo f;
		f.rect1 = rect1.смещенец(off);
		f.rect2 = rect2.смещенец(off);
		f.clip = (clip & q.дайРазм()).смещенец(off);
		f.n = n;
		f.тип = тип;
		f.animation = animation;
		DrawDragRect(*top, f);
	}
}

void FinishDragRect(Ктрл& q)
{
	SystemDraw& w = VirtualGuiPtr->BeginDraw();
	Ктрл::PaintScene(w);
	Ктрл::PaintCaretCursor(w);
	VirtualGuiPtr->CommitDraw();
}

END_UPP_NAMESPACE
