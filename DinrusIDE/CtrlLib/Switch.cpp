#include "CtrlLib.h"

namespace РНЦП {

#define LDUMP(x) // DUMP(x)

Switch& Switch::уст(int i, const Значение& val) {
	cs.по(i).значение = val;
	освежи();
	return *this;
}

Switch& Switch::уст(int i, const Значение& val, const char *text, int gap) {
	Case& v = cs.по(i);
	if(значение.проц_ли())
		значение = val;
	устНадпись(i, text, gap);
	v.значение = val;
	освежи();
	return *this;
}

Switch& Switch::добавь(const Значение& val, const char *text, int gap) {
	уст(cs.дайСчёт(), val, text);
	return *this;
}

Switch& Switch::добавь(const char *text, int gap) {
	уст(cs.дайСчёт(), cs.дайСчёт(), text, gap);
	return *this;
}

int Switch::дайИндекс() const {
	for(int i = 0; i < cs.дайСчёт(); i++)
		if(cs[i].значение == значение)
			return i;
	return -1;
}

Switch&  Switch::устНадпись(int i, const char *text, int gap) {
	if(i >= cs.дайСчёт())
		cs.по(i).значение = i;
	Case& v = cs[i];
	v.accesskey = ExtractAccessKey(text, v.label);
	v.gap = gap;
	освежи();
	return *this;
}

dword Switch::дайКлючиДоступа() const
{
	dword keys = 0;
	for(int i = 0; i < cs.дайСчёт(); i++)
		keys |= AccessKeyBit(cs[i].accesskey);
	return keys;
}

void  Switch::присвойКлючиДоступа(dword used)
{
	for(int i = 0; i < cs.дайСчёт(); i++) {
		Case& v = cs[i];
		if(!v.accesskey) {
			v.accesskey = ChooseAccessKey(v.label, used);
			if(v.accesskey) освежи();
			used |= AccessKeyBit(v.accesskey);
		}
	}
	Ктрл::присвойКлючиДоступа(used);
}

Switch& Switch::устНадпись(const char *text) {
	const char *q = text;
	int i = 0;
	const char *s = text;
	int gap = 0;
	while(*s) {
		if(*s == '^') {
			if(s[1] == '^')
				s += 2;
			else {
				gap = atoi(Ткст(q, s));
				q = ++s;
			}
		}
		else
		if(*s == '|') {
			if(s[1] == '|')
				s += 2;
			else {
				gap = atoi(Ткст(q, s)) | GAP_SEPARATOR;
				q = ++s;
			}
		}
		else
		if(*s == '\r') {
			устНадпись(i++, Ткст(q, s), gap);
			gap = 0;
			q = ++s;
			if(*q == '\n')
				q = ++s;
		}
		else
		if(*s == '\n') {
			устНадпись(i++, Ткст(q, s), gap);
			gap = 0;
			q = ++s;
		}
		else
			s++;
	}
	устНадпись(i++, Ткст(q, s), gap);
	cs.устСчёт(i);
	return *this;
}

void  Switch::устДанные(const Значение& val) {
	if(val != значение) {
		значение = val;
		Update();
		освежи();
	}
}

Значение Switch::дайДанные() const {
	return значение;
}

void Switch::EnableCase(int i, bool enable) {
	cs.по(i).enabled = enable;
	освежи();
}

void Switch::EnableValue(const Значение& val, bool enable) {
	for(int i = 0; i < cs.дайСчёт(); i++)
		if(cs[i].значение == val)
			EnableCase(i, enable);
}

void Switch::сфокусирован() {
	освежи();
}

void Switch::расфокусирован() {
	освежи();
}

void Switch::рисуй(Draw& w) {
	if(cs.дайСчёт() == 0) return;
	Размер sz = дайРазм();
	if(!IsTransparent())
		w.DrawRect(0, 0, sz.cx, sz.cy, SColorFace);
	int tcy = дайРазмТекста("W", font).cy;
	Размер isz = CtrlsImg::S0().дайРазм();
	linecy = max(mincy, max(isz.cy + DPI(2), tcy));
	bool horz = false;
	if(direction == 1) horz = true;
	for(int pass = !!direction; pass < 2; pass++) { // first pass to decide horz, second to actually draw
		int y = 0;
		int x = 0;
		
		if(horz)
			linecy = sz.cy;
	
		int ty = (linecy - tcy) / 2;
		bool ds = !IsShowEnabled();
		int i;
		light = -1;
		for(i = 0; i < cs.дайСчёт(); i++) {
			Case& v = cs[i];

			if(v.gap) {
				int gsz = (v.gap & 255) * tcy / 4;
				if(pass && (v.gap & GAP_SEPARATOR)) {
					if(horz)
						w.DrawRect(x + y + gsz / 2, y, DPI(1), linecy, SColorDisabled());
					else
						w.DrawRect(x, y + gsz / 2, sz.cx, DPI(1), SColorDisabled());
				}
				(horz ? x : y) += gsz;
			}
			
			bool dv = ds || !v.enabled;
	
			Размер tsz = GetSmartTextSize(v.label, font);
			int iy = (linecy - isz.cy) / 2;
			int width = horz ? tsz.cx + isz.cx + DPI(4) : sz.cx;
			Прям hr = RectC(x, y, width, linecy);
			bool mousein = HasMouseIn(hr);
			if(mousein)
				light = i;
			if(pass) {
				Рисунок img;
				int q = dv ? CTRL_DISABLED :
				        pushindex == i ? CTRL_PRESSED :
				        mousein ? CTRL_HOT :
				        CTRL_NORMAL;
				img = CtrlsImg::дай((v.значение == значение ? CtrlsImg::I_S1 : CtrlsImg::I_S0) + q);
				w.DrawImage(x, y + iy, img);
				DrawSmartText(w, x + isz.cx + DPI(4), y + ty, sz.cx, v.label, font,
				              dv || толькочтен_ли() ? SColorDisabled : GetLabelTextColor(this),
				              VisibleAccessKeys() ? v.accesskey : 0);
				if(естьФокус() && (pushindex == i || v.значение == значение && pushindex < 0))
					DrawFocus(w, RectC(x + isz.cx + DPI(2), y + ty - DPI(1), tsz.cx + DPI(3), tsz.cy + DPI(2)) & sz);
			}
			v.rect = hr;
			if(horz)
				x += hr.устШирину() + tcy / 2;
			else
				y += linecy;
		}
		
		if(y > sz.cy)
			horz = true;
	}
	
}

int Switch::дайИндекс(Точка p) {
	for(int i = 0; i < cs.дайСчёт(); i++) {
		if(cs[i].rect.содержит(p) && cs[i].enabled)
			return i;
	}
	return -1;
}

void Switch::двигМыши(Точка p, dword keyflags) {
	if(толькочтен_ли()) return;
	int i = дайИндекс(p);
	int a = -1;
	if(keyflags & K_MOUSELEFT)
		a = i;
	if(pushindex != a) {
		pushindex = a;
		освежи();
	}
	if(i != light)
		освежи();
}

void Switch::леваяВнизу(Точка p, dword keyflags) {
	if(толькочтен_ли()) return;
	if(Ктрл::ClickFocus()) SetWantFocus();
	pushindex = дайИндекс();
	двигМыши(p, keyflags);
}

void Switch::леваяВверху(Точка p, dword keyflags) {
	if(толькочтен_ли()) return;
	if(pushindex >= 0 && pushindex < cs.дайСчёт()) {
		const Значение& v = cs[pushindex].значение;
		if(v != значение) {
			значение = v;
			UpdateAction();
		}
		освежи();
	}
	pushindex = -1;
}

void Switch::выходМыши() {
	pushindex = -1;
	освежи();
}

bool Switch::Ключ(dword ключ, int) {
	if(толькочтен_ли()) return false;
	int Индекс, i;
	Индекс = i = дайИндекс();
	if(ключ == K_UP || ключ == K_LEFT) {
		for(int t = 0; t < cs.дайСчёт(); t++) {
			if(--Индекс < 0) Индекс = cs.дайСчёт() - 1;
			if(cs[Индекс].enabled)
				break;
		}
	}
	else
	if(ключ == K_DOWN || ключ == K_RIGHT) {
		for(int t = 0; t < cs.дайСчёт(); t++) {
			if(++Индекс >= cs.дайСчёт()) Индекс = 0;
			if(cs[Индекс].enabled)
				break;
		}
	}
	else
		return false;
	if(i != Индекс && cs[Индекс].enabled) {
		значение = cs[Индекс].значение;
		UpdateActionRefresh();
	}
	return true;
}

bool Switch::горячаяКлав(dword ключ) {
	if(толькочтен_ли()) return false;
	for(int i = 0; i < cs.дайСчёт(); i++) {
		if(cs[i].enabled && CompareAccessKey(cs[i].accesskey, ключ)) {
			значение = cs[i].значение;
			UpdateActionRefresh();
			return true;
		}
	}
	return false;
}

void Switch::режимОтмены() {
	pushindex = -1;
}

Switch::Switch() {
	direction = 0;
	linecy = 16;
	mincy = 0;
	Transparent();
	NoInitFocus();
	font = StdFont();
}

Switch::~Switch() {}

}
