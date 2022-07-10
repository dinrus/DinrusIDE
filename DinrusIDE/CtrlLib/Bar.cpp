#include "CtrlLib.h"

namespace РНЦП {

#define LLOG(x) // LOG(x)

void ПаноБара::леваяВнизу(Точка pt, dword keyflags)
{
	ПриЛевКлике();
}

void ПаноБара::PaintBar(Draw& w, const КтрлСепаратор::Стиль& ss, const Значение& pane,
                       const Значение& iconbar, int iconsz)
{
	Размер sz = дайРазм();
	Прям r = дайРазм();
	for(int i = 0; i < breakpos.дайСчёт(); i++)
		if(horz) {
			int y = breakpos[i];
			Прям rr = r;
			rr.bottom = y;
			ChPaint(w, rr, pane);
			if(!пусто_ли(ss.l1))
				ChPaint(w, 0, y++, sz.cx, 1, ss.l1);
			if(!пусто_ли(ss.l2))
				ChPaint(w, 0, y++, sz.cx, 1, ss.l2);
			r.top = y;
		}
		else {
			int x = breakpos[i];
			Прям rr = r;
			rr.right = x;
			if(!пусто_ли(iconbar)) {
				Прям r2 = rr;
				r2.right = r2.left + iconsz;
				ChPaint(w, r2, iconbar);
				rr.left = r2.right;
			}
			ChPaint(w, rr, pane);
			if(!пусто_ли(ss.l1))
				ChPaint(w, x++, 0, 1, sz.cy, ss.l1);
			if(!пусто_ли(ss.l2))
				ChPaint(w, x++, 0, 1, sz.cy, ss.l2);
			r.left = x;
		}
	if(!horz && !пусто_ли(iconbar)) {
		Прям r2 = r;
		r2.right = r2.left + iconsz;
		ChPaint(w, r2, iconbar);
		r.left = r2.right;
	}
	ChPaint(w, r, pane);
}

Размер ПаноБара::LayOut(bool horz, int maxsize, bool repos)
{
	auto HoVe = [&](Размер& sz)->int& { return horz ? sz.cx : sz.cy; };
	auto VeHo = [&](Размер& sz)->int& { return horz ? sz.cy : sz.cx; };
	maxsize -= 2 * (horz ? hmargin : vmargin);
	if(repos) {
		this->horz = horz;
		breakpos.очисть();
	}
	int bmargin = horz ? vmargin : hmargin;
	Размер asz(0, 0);
	VeHo(asz) = bmargin;
	auto q = элт.begin();
	while(q != элт.end()) {
		Размер psz(0, 0);
		auto f = q;
		while(q != элт.end()) {
			if(q->ctrl == NULL && пусто_ли(q->gapsize)) {
				q++;
				break;
			}
			int gapsize = q->gapsize;
			if(gapsize == INT_MAX && q->ctrl == NULL) {
				if(maxsize == INT_MAX) {
					q++;
					break;
				}
				int rsz = 0;
				auto w = q + 1;
				while(w < элт.end()) {
					if(!w->ctrl) break;
					Размер sz = w->ctrl->дайМинРазм();
//					if(HoVe(psz) + gapsize + HoVe(sz) > maxsize) break;
					rsz += HoVe(sz);
					w++;
				}
				if(rsz > maxsize)
					gapsize = 0;
				else {
					gapsize = maxsize - rsz - HoVe(psz);
					if(gapsize < 0)
						gapsize = maxsize - rsz;
				}
			}
			Размер sz = q->ctrl ? q->ctrl->дайМинРазм()
			                  : Размер(horz ? gapsize : 0, horz ? 0 : gapsize);
			if(HoVe(sz) == INT_MAX)
				HoVe(sz) = maxsize - HoVe(psz);
			if(HoVe(psz) + HoVe(sz) > maxsize && HoVe(psz)) {
				while(q < элт.end() && q->ctrl == NULL && !пусто_ли(q->gapsize) && q->gapsize != INT_MAX)
					q++;
				break;
			}
			VeHo(psz) = max(VeHo(psz), VeHo(sz));
			if(q->ctrl) {
				if(horz)
					q->ctrl->LeftPos(psz.cx + hmargin, sz.cx);
				else
					q->ctrl->TopPos(psz.cy + vmargin, sz.cy);
			}
			HoVe(psz) += HoVe(sz);
			q++;
		}
		int& bp = VeHo(asz);
		if(bp > bmargin) {
			if(repos)
				breakpos.добавь(bp + bmargin);
			bp += 2 + 2 * bmargin;
		}
		if(repos)
			while(f != q) {
				if(f->ctrl) {
					if(horz)
						f->ctrl->TopPos(bp, psz.cy);
					else
						f->ctrl->LeftPos(bp, psz.cx);
				}
				f++;
			}
		bp += VeHo(psz);
		HoVe(asz) = max(HoVe(asz), HoVe(psz));
	}
	VeHo(asz) += bmargin;
	HoVe(asz) += horz ? 2 * hmargin : 2 * vmargin;
	LLOG("Repos " << Имя() << " " << asz);
	return asz;
}


Размер ПаноБара::Repos(bool horz, int maxsize)
{
	return LayOut(horz, maxsize, true);
}

Размер ПаноБара::GetPaneSize(bool _horz, int maxsize) const
{
	return const_cast<ПаноБара *>(this)->LayOut(horz, maxsize, false);
}

void ПаноБара::IClear()
{
	breakpos.очисть();
	элт.очисть();
}

void ПаноБара::очисть()
{
	while(дайПервОтпрыск())
		удалиОтпрыск(дайПервОтпрыск());
	IClear();
}

void ПаноБара::добавь(Ктрл *ctrl, int gapsize)
{
	Элемент& m = элт.добавь();
	m.ctrl = ctrl;
	m.gapsize = gapsize;
	if(ctrl)
		добавьОтпрыск(ctrl);
}

void ПаноБара::двигМыши(Точка p, dword)
{
	Ктрл *q = Ктрл::GetVisibleChild(this, p, false);
	if(q != phelpctrl) {
		phelpctrl = q;
		КтрлБар::SendHelpLine(q);
	}
}

ПаноБара::ПаноБара()
{
	NoWantFocus();
	phelpctrl = NULL;
	hmargin = vmargin = 0;
	menu = false;
}

ПаноБара::~ПаноБара() {}

Бар::Элемент::Элемент() {}
Бар::Элемент::~Элемент() {}

Бар::Элемент& Бар::Элемент::устТекст(const char *text)        { return *this; }
Бар::Элемент& Бар::Элемент::Ключ(dword ключ)                { return *this; }
Бар::Элемент& Бар::Элемент::повтори(bool b)                { return *this; }
Бар::Элемент& Бар::Элемент::Рисунок(const РНЦП::Рисунок& img)  { return *this; }
Бар::Элемент& Бар::Элемент::Bold(bool bold)               { return *this; }
Бар::Элемент& Бар::Элемент::вкл(bool _enable)          { return *this; }
Бар::Элемент& Бар::Элемент::Подсказка(const char *tip)          { return *this; }
Бар::Элемент& Бар::Элемент::Help(const char *help)        { return *this; }
Бар::Элемент& Бар::Элемент::Topic(const char *topic)      { return *this; }
Бар::Элемент& Бар::Элемент::Description(const char *desc) { return *this; }
Бар::Элемент& Бар::Элемент::Check(bool check)             { return *this; }
Бар::Элемент& Бар::Элемент::Radio(bool check)             { return *this; }
void       Бар::Элемент::FinalSync() {}

Бар::Элемент& Бар::Элемент::Надпись(const char *text)
{
	ToolButton *b = dynamic_cast<ToolButton *>(this);
	if(b) b->Надпись(text, ToolButton::BOTTOMLABEL);
	return *this;
}

Бар::Элемент& Бар::Элемент::RightLabel(const char *text)
{
	ToolButton *b = dynamic_cast<ToolButton *>(this);
	if(b) b->Надпись(text, ToolButton::RIGHTLABEL);
	return *this;
}

Бар::Элемент& Бар::Элемент::Ключ(ИнфОКлюче& (*ключ)()) {
	ИнфОКлюче& k = (*ключ)();
	return Ключ(k.ключ[0]).Ключ(k.ключ[1]).Ключ(k.ключ[2]).Ключ(k.ключ[3]);
}

Бар::Бар() {}
Бар::~Бар() {}

Бар::Элемент& Бар::NilItem()
{
	static Элемент m;
	return m;
}

void Бар::AddNC(Ктрл& ctrl)
{
	добавьКтрл(&ctrl, (int)Null);
}

Бар::Элемент&  Бар::добавь(bool enable, const char *text, const РНЦП::Рисунок& image, const Callback& callback)
{
	return добавьЭлт(callback).устТекст(text).Рисунок(image).вкл(enable);
}

Бар::Элемент&  Бар::добавь(bool enable, ИнфОКлюче& (*ключ)(), const РНЦП::Рисунок& image, const Callback& callback)
{
	const char *text = (*ключ)().имя;
	if(*text == '\3')
		text = t_GetLngString(text + 1);
	return добавь(enable, text, image, callback).Ключ(ключ);
}

Бар::Элемент&  Бар::добавь(const char *text, const РНЦП::Рисунок& image, const Callback& callback)
{
	return добавьЭлт(callback).устТекст(text).Рисунок(image);
}
/*
Бар::Элемент& Бар::добавь(const Ткст& text, const РНЦП::Рисунок& image, const Событие<> & callback)
{
	return добавь(~text, image, callback);
}
*/
Бар::Элемент&  Бар::добавь(ИнфОКлюче& (*ключ)(), const РНЦП::Рисунок& image, const Callback& callback)
{
	const char *text = (*ключ)().имя;
	if(*text == '\3')
		text = t_GetLngString(text + 1);
	return добавь(text, image, callback).Ключ(ключ);
}

Бар::Элемент&  Бар::добавь(bool enable, const char *text, const Callback & callback)
{ return барМеню_ли() ? добавь(enable, text, Рисунок(), callback) : NilItem(); }

Бар::Элемент&  Бар::добавь(const char *text, const Callback & callback)
{ return барМеню_ли() ? добавь(true, text, Рисунок(), callback) : NilItem(); }

Бар::Элемент&  Бар::добавь(bool enable, ИнфОКлюче& (*ключ)(), const Callback & callback)
{ return барМеню_ли() ? добавь(enable, ключ, Рисунок(), callback) : NilItem(); }

Бар::Элемент&  Бар::добавь(ИнфОКлюче& (*ключ)(), const Callback & callback)
{ return барМеню_ли() ? добавь(true, ключ, Рисунок(), callback) : NilItem(); }

Бар::Элемент& Бар::добавь(bool enable, const char *text, const РНЦП::Рисунок& image, const РНЦП::Функция<void ()>& фн)
{
	return добавь(enable, text, image, Callback() << фн);
}

Бар::Элемент& Бар::добавь(bool enable, ИнфОКлюче& (*ключ)(), const РНЦП::Рисунок& image, const Функция<void ()>& фн)
{
	return добавь(enable, ключ, image, Callback() << фн);
}

Бар::Элемент& Бар::добавь(const char *text, const РНЦП::Рисунок& image, const Функция<void ()>& фн)
{
	return добавь(text, image, Callback() << фн);
}

/*
Бар::Элемент& Бар::добавь(const Ткст& text, const РНЦП::Рисунок& image, const Функция<void ()>& фн)
{
	return добавь(text, image, Событие<> () << фн);
}
*/

Бар::Элемент& Бар::добавь(ИнфОКлюче& (*ключ)(), const РНЦП::Рисунок& image, const Функция<void ()>& фн)
{
	return добавь(ключ, image, Callback() << фн);
}

Бар::Элемент& Бар::добавь(bool enable, const char *text, const Функция<void ()>& фн)
{
	return добавь(enable, text, Callback() << фн);
}

Бар::Элемент& Бар::добавь(bool enable, ИнфОКлюче& (*ключ)(), const Функция<void ()>& фн)
{
	return добавь(enable, ключ, Callback() << фн);
}

Бар::Элемент& Бар::добавь(const char *text, const Функция<void ()>& фн)
{
	return добавь(text, Callback() << фн);
}

Бар::Элемент& Бар::добавь(ИнфОКлюче& (*ключ)(), const Функция<void ()>& фн)
{
	return добавь(ключ, Callback() << фн);
}

void Бар::Break()
{
	добавьКтрл(NULL, (int)Null);
}

void Бар::Gap(int size)
{
	добавьКтрл(NULL, size);
}

void   Бар::MenuSeparator()              { if(барМеню_ли()) Separator(); }
void   Бар::MenuBreak()                  { if(барМеню_ли()) Break(); }
void   Бар::MenuGap(int size)            { if(барМеню_ли()) Gap(size); }
void   Бар::добавьМеню(Ктрл& ctrl)          { if(барМеню_ли()) добавь(ctrl); }
void   Бар::добавьМеню(Ктрл& ctrl, Размер sz) { if(барМеню_ли()) добавь(ctrl, sz); }

Бар::Элемент&  Бар::добавьМеню(bool enable, ИнфОКлюче& (*ключ)(), const РНЦП::Рисунок& image, const Callback & callback)
{ return барМеню_ли() ? добавь(enable, ключ, image, callback) : NilItem(); }

Бар::Элемент&  Бар::добавьМеню(ИнфОКлюче& (*ключ)(), const РНЦП::Рисунок& image, const Callback & callback)
{ return барМеню_ли() ? добавь(ключ, image, callback) : NilItem(); }

Бар::Элемент&  Бар::добавьМеню(bool enable, const char *text, const РНЦП::Рисунок& image, const Callback & callback)
{ return барМеню_ли() ? добавь(enable, text, image, callback) : NilItem(); }

Бар::Элемент&  Бар::добавьМеню(const char *text, const РНЦП::Рисунок& image, const Callback & callback)
{ return барМеню_ли() ? добавь(text, image, callback) : NilItem(); }


Бар::Элемент& Бар::добавьМеню(bool enable, const char *text, const РНЦП::Рисунок& image, const Функция<void ()>& фн)
{
	return добавьМеню(enable, text, image, Callback() << фн);
}

Бар::Элемент& Бар::добавьМеню(bool enable, ИнфОКлюче& (*ключ)(), const РНЦП::Рисунок& image, const Функция<void ()>& фн)
{
	return добавьМеню(enable, ключ, image, Callback () << фн);
}

Бар::Элемент& Бар::добавьМеню(const char *text, const РНЦП::Рисунок& image, const Функция<void ()>& фн)
{
	return добавьМеню(text, image, Callback () << фн);
}

Бар::Элемент& Бар::добавьМеню(ИнфОКлюче& (*ключ)(), const РНЦП::Рисунок& m, const Функция<void ()>& фн)
{
	return добавьМеню(ключ, m, Callback () << фн);
}

Бар::Элемент& Бар::добавь(bool enable, const char *text, const Обрвыз1<Бар&>& proc)
{ return добавьСубМеню(proc).устТекст(text).вкл(enable); }

Бар::Элемент& Бар::добавь(const char *text, const Обрвыз1<Бар&>& proc)
{ return добавь(true, text, proc); }

Бар::Элемент& Бар::добавь(bool enable, const char *text, const РНЦП::Рисунок& image, const Обрвыз1<Бар&>& proc)
{ return добавь(enable, text, proc).Рисунок(image); }

Бар::Элемент& Бар::добавь(const char *text, const РНЦП::Рисунок& image, const Обрвыз1<Бар&>& proc)
{ return добавь(text, proc).Рисунок(image); }

Бар::Элемент& Бар::Sub(bool enable, const char *text, const Функция<void (Бар&)>& submenu)
{
	return добавь(enable, text, Обрвыз1<Бар&>() << submenu);
}

Бар::Элемент& Бар::Sub(const char *text, const Функция<void (Бар&)>& submenu)
{
	return добавь(text, Обрвыз1<Бар&>() << submenu);
}

Бар::Элемент& Бар::Sub(bool enable, const char *text, const РНЦП::Рисунок& image, const Функция<void (Бар&)>& submenu)
{
	return добавь(enable, text, image, Обрвыз1<Бар&>() << submenu);
}

Бар::Элемент& Бар::Sub(const char *text, const РНЦП::Рисунок& image, const Функция<void (Бар&)>& submenu)
{
	return добавь(text, image, Обрвыз1<Бар&>() << submenu);
}

void   Бар::ToolSeparator()                { if(барИнстр_ли()) Separator(); }
void   Бар::ToolBreak()                    { if(барИнстр_ли()) Break(); }
void   Бар::ToolGap(int size)              { if(барИнстр_ли()) Gap(size); }

void   Бар::добавьИнстр(Ктрл& ctrl)            { if(барИнстр_ли()) добавь(ctrl); }
void   Бар::добавьИнстр(Ктрл& ctrl, Размер sz)   { if(барИнстр_ли()) добавь(ctrl, sz); }

void   Бар::добавьКлюч(dword ключ, Событие<>  cb) {}

void Бар::добавьКлюч(ИнфОКлюче& (*ключ)(), Событие<>  cb)
{
	ИнфОКлюче& k = (*ключ)();
	добавьКлюч(k.ключ[0], cb);
	добавьКлюч(k.ключ[1], cb);
	добавьКлюч(k.ключ[2], cb);
	добавьКлюч(k.ключ[3], cb);
}

Бар::Элемент&  Бар::добавь(bool enable, const РНЦП::Рисунок& image, Событие<>  cb)
{ return барИнстр_ли() ? добавьЭлт(cb).Рисунок(image).вкл(enable) : NilItem(); }

Бар::Элемент&  Бар::добавь(const РНЦП::Рисунок& image, Событие<>  cb)
{ return барИнстр_ли() ? добавь(true, image, cb) : NilItem(); }

class Бар::ScanKeys : public Бар {
	struct KeyItem : public Бар::Элемент {
		bool          enabled;
		Вектор<dword> ключ;
		Событие<>       action;

		virtual Элемент& устТекст(const char *)             { return *this; }
		virtual Элемент& Check(bool check)              { return *this; }
		virtual Элемент& Radio(bool check)              { return *this; }
		virtual Элемент& Ключ(dword _key)                { if(_key) ключ.добавь(_key); return *this; }
		virtual Элемент& Рисунок(const РНЦП::Рисунок& img)   { return *this;  }
		virtual Элемент& вкл(bool _enable = true)    { enabled = _enable; return *this; }
		virtual Элемент& Подсказка(const char *tip)           { return *this; }
		virtual Элемент& Help(const char *help)         { return *this; }
		virtual Элемент& Topic(const char *help)        { return *this; }
		virtual Элемент& Description(const char *help)  { return *this; }
	};

	Массив<KeyItem> ключ;
	KeyItem        nilitem;
	int            level;

protected:
	virtual Элемент&  добавьЭлт(Событие<>  cb);
	virtual Элемент&  добавьСубМеню(Событие<Бар&> proc);
	virtual void   добавьКтрл(Ктрл *ctrl, int gapsize) {};
	virtual void   добавьКтрл(Ктрл *ctrl, Размер sz) {};

public:
	virtual bool   барМеню_ли() const                 { return true; }
	virtual bool   барИнстр_ли() const                 { return true; }
	virtual bool   IsScanKeys() const                { return true; }

	virtual bool   пустой() const                   { return ключ.пустой(); }
	virtual void   Separator()                       {}
	virtual void   добавьКлюч(dword ключ, Событие<> cb);

	bool Do(dword ключ);

	ScanKeys()                                       { level = 0; }
};

Бар::Элемент& Бар::ScanKeys::добавьЭлт(Событие<>  cb)
{
	KeyItem& m = ключ.добавь();
	m.action = cb;
	m.enabled = true;
	return m;
}

Бар::Элемент&  Бар::ScanKeys::добавьСубМеню(Событие<Бар&> proc)
{
	level++;
	if(level > 5)
		return NilItem();;
	proc(*this);
	level--;
	return ключ.дайСчёт() ? ключ.верх() : nilitem;
}

void   Бар::ScanKeys::добавьКлюч(dword _key, Событие<>  cb)
{
	if(_key) {
		KeyItem& m = ключ.добавь();
		m.action = cb;
		m.enabled = true;
		m.ключ.добавь(_key);
	}
}

bool   Бар::ScanKeys::Do(dword k)
{
	for(int i = 0; i < ключ.дайСчёт(); i++) {
		KeyItem& m = ключ[i];
		if(m.enabled && найдиИндекс(m.ключ, k) >= 0) {
			m.action();
			return true;
		}
	}
	return false;
}

bool   Бар::скан(Событие<Бар&> proc, dword ключ)
{
	ScanKeys keys;
	proc(keys);
	return keys.Do(ключ);
}

bool КтрлБар::пустой() const
{
	return pane.пустой();
}

void КтрлБар::Separator()
{
	AddNC(separator.по(sii++).Margin(lsepm, rsepm).устСтиль(*sepstyle).устРазм(ssize));
}

void КтрлБар::добавьКтрл(Ктрл *ctrl, Размер sz)
{
	КтрлРазмер& m = sizer.индексируй(zii++);
	while(m.дайПервОтпрыск())
		m.удалиОтпрыск(m.дайПервОтпрыск());
	ПозЛога p = ctrl->дайПоз();
	if(p.x.пустой()) {
		if(sz.cx == INT_MAX)
			ctrl->HSizePos();
		else
			ctrl->HCenterPos(sz.cx ? sz.cx : MINSIZE);
	}
	if(p.y.пустой()) {
		if(sz.cy == INT_MAX)
			ctrl->VSizePos();
		else
			ctrl->VCenterPos(sz.cy ? sz.cy : MINSIZE);
	}
	LLOG("добавьКтрл Ктрл::дайРазм: " << ctrl->дайРазм());
	m.устРазм(sz);
	m.добавьОтпрыск(ctrl);
	AddNC(m);
	LLOG("AddCtrl2 Ктрл::дайРазм: " << ctrl->дайРазм());
}

Размер КтрлБар::КтрлРазмер::дайМинРазм() const
{
	Размер sz = size;
	Размер chsz = Размер(8, 8);
	if(дайПервОтпрыск())
		chsz = дайПервОтпрыск()->дайСтдРазм();
	sz.cx = sz.cx <= 0 ? chsz.cx : sz.cx;
	sz.cy = sz.cy <= 0 ? chsz.cy : sz.cy;
	return sz;
}

Значение КтрлБар::дайФон() const
{
	return Null;
}

void КтрлБар::очисть()
{
	separator.очисть();
	sizer.очисть();
	pane.очисть();
}

void КтрлБар::IClear()
{
	sii = zii = 0;
	pane.IClear();
}

void КтрлБар::IFinish()
{
	separator.устСчёт(sii);
	sizer.устСчёт(zii);
}

КтрлБар *КтрлБар::GetBarCtrlParent(Ктрл *q)
{
	while(q) {
		КтрлБар *w = dynamic_cast<КтрлБар *>(q);
		if(w) return w;
		q = q->дайРодителя();
	}
	return NULL;
}

void КтрлБар::SendHelpLine(Ктрл *q)
{
	Ткст s = q->GetHelpLine();
	if(!РНЦП::пустой(s)) {
		КтрлБар *b = GetBarCtrlParent(q);
		if(b) b->WhenHelp(s);
	}
}

void КтрлБар::ClearHelpLine(Ктрл *q)
{
	КтрлБар *b = GetBarCtrlParent(q);
	if(b) b->WhenHelp(Null);
}

void КтрлБар::выложиФрейм(Прям& r)
{
	switch(align) {
	case BAR_LEFT:
		выложиФреймСлева(r, this, pane.Repos(false, r.устВысоту()).cx);
		break;
	case BAR_RIGHT:
		выложиФреймСправа(r, this, pane.Repos(false, r.устВысоту()).cx);
		break;
	case BAR_BOTTOM:
		выложиФреймСнизу(r, this, pane.Repos(true, r.устШирину()).cy);
		break;
	default:
		выложиФреймСверху(r, this, pane.Repos(true, r.устШирину()).cy);
		break;
	}
}

void КтрлБар::добавьРазмФрейма(Размер& sz)
{
	if(align == BAR_LEFT || align == BAR_RIGHT)
		sz.cx += pane.GetPaneSize(true, sz.cy).cx;
	else
		sz.cy += pane.GetPaneSize(true, sz.cx).cy;
}

void КтрлБар::добавьКФрейму(Ктрл& parent)
{
	parent.добавь(*this);
}

void КтрлБар::удалиФрейм()
{
	удали();
}

void КтрлБар::добавьКтрл(Ктрл *ctrl, int gapsize)
{
	pane.добавь(ctrl, gapsize);
}

КтрлБар& КтрлБар::Align(int al) {
	align = al;
	SyncBar();
	return *this;
}

void КтрлБар::SyncBar()
{
	if(отпрыск_ли() && InFrame())
		UpdateParentLayout();
	else
		Выкладка();
}

void КтрлБар::Выкладка()
{
	LLOG("КтрлБар::Выкладка");
	if(отпрыск_ли()) {
		bool dowrap = InFrame() && wrap >= 0 || wrap > 0;
		if(дайЛин() == BAR_LEFT || дайЛин() == BAR_RIGHT)
			pane.Repos(false, dowrap ? дайРазм().cy : INT_MAX);
		else
			pane.Repos(true, dowrap ? дайРазм().cx : INT_MAX);
	}
}

void КтрлБар::PaintBar(Draw& w, const КтрлСепаратор::Стиль& ss, const Значение& p, const Значение& iconbar, int iconsz)
{
	pane.PaintBar(w, ss, p, iconbar, iconsz);
}

КтрлБар::КтрлБар() {
	align = BAR_TOP;
	Ктрл::добавь(pane.SizePos());
	lsepm = rsepm = 2;
	ssize = 8;
	sii = zii = 0;
	NoWantFocus();
	pane.ПриЛевКлике = прокси(ПриЛевКлике);
	wrap = 0;
	sepstyle = &КтрлСепаратор::дефСтиль();
}

КтрлБар::~КтрлБар() {}

void LRUList::сериализуй(Поток& stream)
{
	StreamContainer(stream, lru);
}

void LRUList::выдели(Ткст f, Событие<const Ткст&> WhenSelect)
{
	WhenSelect(f);
}

void LRUList::operator()(Бар& bar, Событие<const Ткст&> WhenSelect, int count, int from)
{
	if(bar.барМеню_ли() && !lru.пустой()) {
		bar.Separator();
		char n = '1';
		for(int i = from; i < min(from + count, lru.дайСчёт()); i++) {
			bar.добавь(Ткст("&") + n + ' ' + дайИмяф(lru[i]),
			        THISBACK2(выдели, lru[i], WhenSelect));
			n = n == '9' ? 'A' : n + 1;
		}
	}
}

void LRUList::NewEntry(const Ткст& path)
{
	LruAdd(lru, path, limit);
}

void LRUList::RemoveEntry(const Ткст& path)
{
	int q = найдиИндекс(lru, path);
	if(q >= 0)
		lru.удали(q);
}

}
