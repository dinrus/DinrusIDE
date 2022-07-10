#include "DinrusIDE.h"

void AssistEditor::SyncParamInfo()
{
	Ткст qtf;
	Ктрл *p = дайТопКтрл();
	int mpar = INT_MAX;
	int pos = 0;
	if(p && p->HasFocusDeep()) {
		for(int q = 0; q < PARAMN; q++) {
			ParamInfo& m = param[q];
			int i = GetCursorLine();
			if(m.line >= 0 && m.line < дайСчётСтрок() && i >= m.line && i < m.line + 10
			   && m.editfile == theide->editfile && дайШСтроку(m.line).начинаетсяС(m.test)) {
				int c = дайКурсор32();
				i = дайПоз32(m.line) + m.test.дайСчёт();
				if(c >= i) {
					int par = 0;
					int pari = 0;
					int str = 0;
					bool esc = false;
					for(;;) {
						int ch = Ch(i++);
						if(i > c) {
							if(par < mpar) {
								qtf = "[A1  " + DecoratedItem(m.элт.имя, m.элт, m.элт.natural, pari);
								mpar = par;
								pos = m.pos;
							}
							break;
						}
						if(findarg(ch, ')', ']', '}') >= 0) {
							if(par <= 0)
								break;
							par--;
						}
						else
						if(findarg(ch, '(', '[', '{') >= 0)
							par++;
						else
						if(findarg(ch, '\"', '\'') >= 0 && !esc)
							str = str ? 0 : ch;
						else
						if(ch == ',' && par == 0 && str == 0)
							pari++;
						esc = ch == '\\';
					}
				}
			}
		}
	}
	if(param_qtf != qtf)
		param_info.SetQTF(qtf);
	Прям r = GetLineScreenRect(GetCursorLine());
	int cx = max(дайРазм().cx - 30, 300);
	int h = param_info.дайВысоту(cx);
	h = min(h, 550);
	int y = r.top - h - 6;
	if(y < GetWorkArea().top)
		y = r.bottom;
	r = RectC(r.left, y, min(param_info.дайШирину(), cx) + 8, h + 6);
	r.смещениеГориз(GetColumnLine(pos).x * GetFontSize().cx);
	r.смещениеГориз(min(GetWorkArea().right - r.right, 0));
	if(param_qtf != qtf || r != param_info.дайПрям()) {
		param_qtf = qtf;
		if(пусто_ли(qtf)) {
			if(param_info.открыт())
				param_info.закрой();
		}
		else {
			param_info.устПрям(r);
			if(!param_info.открыт() && !выделение_ли())
				param_info.Ктрл::PopUp(this, false, false, false);
		}
	}
}

void AssistEditor::StartParamInfo(const CppItem& m, int pos)
{
	int x = дайКурсор32();
	ParamInfo& f = param[parami];
	f.line = дайПозСтроки32(x);
	f.test = дайШСтроку(f.line).середина(0, x);
	f.элт = m;
	f.editfile = theide->editfile;
	f.pos = pos;
	SyncParamInfo();
	parami = (parami + 1) % PARAMN;
}

void AssistEditor::State(int reason)
{
	if(reason == FOCUS)
		SyncParamInfo();
	РедакторКода::State(reason);
}

int AssistEditor::GetCurrentLine()
{
	return дайСтроку(дайКурсор64());
}
