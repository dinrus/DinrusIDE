#include "CtrlLib.h"

namespace РНЦП {

#define LLOG(x) // RLOG(x)

// #define SLOWANIMATION

#define IMAGECLASS CtrlImg
#define IMAGEFILE  <CtrlLib/Ctrl.iml>
#include <Draw/iml_source.h>

void анимируй(Ктрл& c, const Прям& target, int тип)
{
	if(тип < 0)
		тип = GUI_PopUpEffect();
	Прям r0 = c.дайПрям();
	dword time0 = msecs();
	int anitime = 150;
#ifdef SLOWANIMATION
	anitime = 1500;
#endif
	if(тип)
		for(;;) {
			int t = int(msecs() - time0);
			if(t > anitime)
				break;
			if(тип == GUIEFFECT_SLIDE) {
				Прям r = r0;
				if(r.left > target.left)
				   r.left -= ((r.left - target.left)* t) / anitime;
				if(r.top > target.top)
				   r.top -= ((r.top - target.top) * t) / anitime;
				if(r.right < target.right)
				   r.right += ((target.right - r.right) * t) / anitime;
				if(r.bottom < target.bottom)
				   r.bottom += ((target.bottom - r.bottom) * t) / anitime;
				if(r.дайШирину() > target.дайШирину())
				   r.right = r.left + target.дайШирину();
				if(r.дайВысоту() > target.дайВысоту())
				   r.bottom = r.top + target.дайВысоту();
				c.устПрям(r);
				if(r == target)
					break;
			}
			else
			if(тип == GUIEFFECT_FADE)
				c.SetAlpha((byte)(255 * t / anitime));
			else
				break;
			c.синх();
			Sleep(0);
#ifdef SLOWANIMATION
			спи(100);
#endif
		}
	c.устПрям(target);
	c.SetAlpha(255);
}

void анимируй(Ктрл& c, int x, int y, int cx, int cy, int тип)
{
	анимируй(c, RectC(x, y, cx, cy), тип);
}

bool CtrlLibDisplayError(const Значение& e) {
	if(!e.ошибка_ли())
		return false;
	Ткст s = дайТекстОш(e);
	if(s.пустой())
		s = t_("Неверные данные.");
	Exclamation(s);
	return true;
}

ИНИЦБЛОК
{
	DisplayErrorFn() = &CtrlLibDisplayError;
}
/*
Ткст SaveCtrlLayout(Ктрл::ПозЛога p, const Ткст& classname, const Ткст& variable,
					  const Ткст& label, const Ткст& help) {
	Ткст out;
	if(classname.пустой())
		out << "\tUNTYPED(";
	else
		out << "\tITEM(" << classname << ", ";
	out << variable << ", ";
	switch(p.x.дайЛин()) {
	case Ктрл::LEFT:   out << фмт("LeftPos(%d, %d).", p.x.GetA(), p.x.дайС()); break;
	case Ктрл::RIGHT:  out << фмт("RightPos(%d, %d).", p.x.GetA(), p.x.дайС()); break;
	case Ктрл::SIZE:   out << фмт("HSizePos(%d, %d).", p.x.GetA(), p.x.дайС()); break;
	case Ктрл::CENTER: out << фмт("HCenterPos(%d, %d).", p.x.дайС(), p.x.GetA()); break;
	}
	switch(p.y.дайЛин()) {
	case Ктрл::TOP:    out << фмт("TopPos(%d, %d)", p.y.GetA(), p.y.дайС()); break;
	case Ктрл::BOTTOM: out << фмт("BottomPos(%d, %d)", p.y.GetA(), p.y.дайС()); break;
	case Ктрл::SIZE:   out << фмт("VSizePos(%d, %d)", p.y.GetA(), p.y.дайС()); break;
	case Ктрл::CENTER: out << фмт("VCenterPos(%d, %d)", p.y.дайС(), p.y.GetA()); break;
	}
	if(!label.пустой()) {
		out << ".устНадпись(\"";
		for(const char *s = label; *s; s++)
			if(*s == '\n')
				out.конкат("\\n");
			else
				out.конкат(*s);
		out << "\")";
	}
	if(!help.пустой()) {
		out << ".HelpC(\"";
		for(const char *s = help; *s; s++)
			if(*s == '\n')
				out.конкат("\\n");
			else
				out.конкат(*s);
		out << "\")";
	}
	out << ")\n";
	return out;
}
*/
void покажи2(Ктрл& ctrl1, Ктрл& ctrl2, bool show) {
	ctrl1.покажи(show);
	ctrl2.покажи(show);
}

void скрой2(Ктрл& ctrl1, Ктрл& ctrl2) {
	покажи2(ctrl1, ctrl2, false);
}

void DelayCallback::Invoke() {
	глушиОбрвызВремени(this);
	устОбрвызВремени(delay, target, this);
}

void sSyncLabel(Надпись *lbl, const char *label, EditString *text)
{
	lbl->устНадпись(Ткст().конкат() << label << " (" << text->дайДлину() << "/" << text->дайМаксДлин() << ")");
}

bool редактируйТекст(Ткст& s, const char *title, const char *label, int (*f)(int), int maxlen, bool notnull)
{
	WithEditStringLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, title);
	dlg.lbl = label;
	dlg.text <<= s.вШТкст();
	dlg.text.неПусто(notnull);
	dlg.text.устФильтр(f);
	if(maxlen) {
		dlg.text.максдлин(maxlen);
		dlg.text <<= callback3(sSyncLabel, &dlg.lbl, label, &dlg.text);
		dlg.text.WhenAction();
	}
	if(dlg.выполни() == IDOK) {
		s = ~dlg.text;
		return true;
	}
	return false;
}

bool редактируйТекст(Ткст& s, const char *title, const char *label, int (*filter)(int), int maxlen)
{
	return редактируйТекст(s, title, label, filter, maxlen, false);
}

bool редактируйТекст(Ткст& s, const char *title, const char *label, int maxlen)
{
	return редактируйТекст(s, title, label, CharFilterUnicode, maxlen, false);
}

bool EditTextNotNull(Ткст& s, const char *title, const char *label, int (*filter)(int), int maxlen)
{
	return редактируйТекст(s, title, label, filter, maxlen, true);
}

bool EditTextNotNull(Ткст& s, const char *title, const char *label, int maxlen)
{
	return редактируйТекст(s, title, label, CharFilterUnicode, maxlen, true);
}

bool редактируйТекст(ШТкст& s, const char *title, const char *label, int (*f)(int), int maxlen, bool notnull)
{
	Ткст ss = s.вТкст();
	if(редактируйТекст(ss, title, label, f, maxlen, notnull)) {
		s = ss.вШТкст();
		return true;
	}
	return false;
}

bool редактируйТекст(ШТкст& s, const char *title, const char *label, int (*filter)(int), int maxlen)
{
	return редактируйТекст(s, title, label, filter, maxlen, false);
}

bool редактируйТекст(ШТкст& s, const char *title, const char *label, int maxlen)
{
	return редактируйТекст(s, title, label, CharFilterUnicode, maxlen, false);
}

bool EditTextNotNull(ШТкст& s, const char *title, const char *label, int (*filter)(int), int maxlen)
{
	return редактируйТекст(s, title, label, filter, maxlen, true);
}

bool EditTextNotNull(ШТкст& s, const char *title, const char *label, int maxlen)
{
	return редактируйТекст(s, title, label, CharFilterUnicode, maxlen, true);
}

bool EditNumber(int& n, const char *title, const char *label, int min, int max, bool notnull)
{
	WithEditIntLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, title);
	dlg.lbl = label;
	dlg.number <<= n;
	dlg.number.минмакс(min, max);
	dlg.number.неПусто(notnull);
	if(dlg.выполни() == IDOK) {
		n = ~dlg.number;
		return true;
	}
	return false;
}

bool EditNumber(double& n, const char *title, const char *label, double min, double max, bool notnull)
{
	WithEditDoubleLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, title);
	dlg.lbl = label;
	dlg.number <<= n;
	dlg.number.минмакс(min, max);
	dlg.number.неПусто(notnull);
	if(dlg.выполни() == IDOK) {
		n = ~dlg.number;
		return true;
	}
	return false;
}

bool EditDateDlg(Дата& d, const char *title, const char *label, Дата min, Дата max, bool notnull)
{
	WithEditDateLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, title);
	dlg.lbl = label;
	dlg.date <<= d;
	dlg.date.минмакс(min, max);
	dlg.date.неПусто(notnull);
	if(dlg.выполни() == IDOK) {
		d = ~dlg.date;
		return true;
	}
	return false;
}

Событие<> CtrlRetriever::operator^=(Событие<> cb)
{
	for(int i = 0; i < элт.дайСчёт(); i++) {
		CtrlItem0 *m = dynamic_cast<CtrlItem0 *>(&элт[i]);
		if(m)
			m->ctrl->WhenAction = cb;
	}
	return cb;
}

Событие<> CtrlRetriever::operator<<(Событие<> cb)
{
	for(int i = 0; i < элт.дайСчёт(); i++) {
		CtrlItem0 *m = dynamic_cast<CtrlItem0 *>(&элт[i]);
		if(m)
			m->ctrl->WhenAction << cb;
	}
	return cb;
}

void CtrlRetriever::уст()
{
	for(int i = 0; i < элт.дайСчёт(); i++)
		элт[i].уст();
}

void CtrlRetriever::Retrieve()
{
	for(int i = 0; i < элт.дайСчёт(); i++)
		элт[i].Retrieve();
}

void ИдКтрлы::добавь(Ид id, Ктрл& ctrl)
{
	Элемент& m = элт.добавь();
	m.id = id;
	m.ctrl = &ctrl;
}

МапЗнач ИдКтрлы::дай() const
{
	МапЗнач m;
	for(int i = 0; i < элт.дайСчёт(); i++)
		m.добавь(элт[i].id, элт[i].ctrl->дайДанные());
	return m;
}

void ИдКтрлы::уст(const МапЗнач& m)
{
	for(int i = 0; i < элт.дайСчёт(); i++)
		элт[i].ctrl->устДанные(m[элт[i].id]);
}

bool ИдКтрлы::прими()
{
	for(int i = 0; i < элт.дайСчёт(); i++)
		if(!элт[i].ctrl->прими()) return false;
	return true;
}

void ИдКтрлы::ClearModify() {
	for(int i = 0; i < элт.дайСчёт(); i++)
		элт[i].ctrl->ClearModify();
}

bool ИдКтрлы::изменено() {
	for(int i = 0; i < элт.дайСчёт(); i++)
		if(элт[i].ctrl->изменено()) return true;
	return false;
}

void ИдКтрлы::вкл(bool b)
{
	for(int i = 0; i < элт.дайСчёт(); i++)
		элт[i].ctrl->вкл(b);
}

void ИдКтрлы::устПусто()
{
	for(int i = 0; i < элт.дайСчёт(); i++)
		элт[i].ctrl->устДанные(Null);
}

void уст(КтрлМассив& array, int ii, ИдКтрлы& m)
{
	for(int i = 0; i < m.дайСчёт(); i++)
		array.уст(ii, m(i), ~m[i]);
}

void дай(КтрлМассив& array, int ii, ИдКтрлы& m)
{
	for(int i = 0; i < m.дайСчёт(); i++)
		m[i] <<= array.дай(ii, m(i));
}

Событие<> ИдКтрлы::operator<<(Событие<> action)
{
	for(int i = 0; i < элт.дайСчёт(); i++)
		элт[i].ctrl->WhenAction << action;
	return action;
}

Событие<> ИдКтрлы::operator^=(Событие<> action)
{
	for(int i = 0; i < элт.дайСчёт(); i++)
		элт[i].ctrl->WhenAction = action;
	return action;
}

Ткст sProfile(const ПрофильПамяти& mem)
{
	return какТкст(mem);
}

void MemoryProfileInfo() {
	ПрофильПамяти mem;
	Ткст text = sProfile(mem);
	const ПрофильПамяти *peak = PeakMemoryProfile();
	if(peak)
		text << "\r\n=== Выберите профиль памяти\r\n" << sProfile(*peak);
	PromptOK("[C " + DeQtfLf(text));
};

FileSelButton::FileSelButton(MODE mode, const char *title)
: title(title), mode(mode)
{
	button.NoWantFocus();
	button.устРисунок(mode == MODE_DIR ? CtrlImg::DirSmall() : CtrlImg::FileSmall());
	button << [=] { OnAction(); };
}

void FileSelButton::прикрепи(Ктрл& parent)
{
	button.устШирину(DPI(20));
	bool not_mb = true;
	for(int i = 0; i < parent.дайСчётФреймов(); i++)
		if(dynamic_cast<MultiButtonFrame *>(&parent.дайФрейм(i))) {
			not_mb = false;
			break;
		}
	if(not_mb && dynamic_cast<EditField *>(&parent)) {
		parent.вставьФрейм(0, button);
		parent.вставьФрейм(1, фреймПравПроёма());
	}
	else
		parent.добавьФрейм(button);
}

void FileSelButton::OnAction()
{
	Ктрл *owner = button.дайРодителя();
	ПРОВЕРЬ(owner);
	Ткст old = ~*owner;
	if(mode == MODE_DIR) {
		for(int i = 0; i < 4; i++) {
			if(дирЕсть(old))
				break;
			old = дайПапкуФайла(old);
		}
		ActiveDir(old);
	}
	else
		уст(old);
	if(mode == MODE_OPEN ? ExecuteOpen(title) : mode == MODE_SAVE ? ExecuteSaveAs(title) : ExecuteSelectDir(title))
	{
		*owner <<= дай();
		owner->Action();
		WhenSelected();
	}
}

void FileSelButton::открепи()
{
	Ктрл *p = button.дайРодителя();
	if(p) p->удалиФрейм(button);
}

}
