#include "Debuggers.h"

#ifdef PLATFORM_WIN32

#define LLOG(x) // LOG(x)

ВекторМап<Ткст, Значение> Pdb::DataMap(const КтрлМассив& a)
{
	ВекторМап<Ткст, Значение> r;
	for(int i = 0; i < a.дайСчёт(); i++)
		r.добавь(a.дай(i, 0), a.дай(i, 1));
	return r;
}

Значение Pdb::Vis(const Ткст& ключ, const ВекторМап<Ткст, Значение>& prev, Visual&& vis, bool& ch)
{
	int q = prev.найди(ключ);
	ch = false;
	for(int i = 0; i < vis.part.дайСчёт(); i++) {
		VisualPart& p = vis.part[i];
		p.mark = false;
		if(q >= 0 && IsType<Visual>(prev[q])) {
			const Visual& v = ValueTo<Visual>(prev[q]);
			if(v.part.дайСчёт() == vis.part.дайСчёт() && v.part[i].text != p.text)
				ch = p.mark = true;
		}
	}
	return RawPickToValue(pick(vis));
}

void Pdb::Vis(КтрлМассив& a, const Ткст& ключ, const ВекторМап<Ткст, Значение>& prev, Visual&& vis)
{
	bool ch;
	a.добавь(ключ, Vis(ключ, prev, pick(vis), ch));
}

void Pdb::Locals()
{
	ВекторМап<Ткст, Значение> prev = DataMap(locals);
	locals.очисть();
	int q = ~framelist;
	if(q >= 0 && q < frame.дайСчёт()) {
		Фрейм& f = frame[q];
		for(int i = 0; i < f.param.дайСчёт(); i++)
			Vis(locals, f.param.дайКлюч(i), prev, Visualise(f.param[i]));
		for(int i = 0; i < f.local.дайСчёт(); i++)
			Vis(locals, f.local.дайКлюч(i), prev, Visualise(f.local[i]));
	}
}

void Pdb::AddThis(const ВекторМап<Ткст, Val>& m, adr_t address, const ВекторМап<Ткст, Значение>& prev)
{
	for(int i = 0; i < m.дайСчёт() && self.дайСчёт() < 2000; i++) {
		Val mv = m[i];
		mv.address += address;
		Visual vis;
		try {
			vis = Visualise(mv);
		}
		catch(СиПарсер::Ошибка e) {
			vis.конкат(e, SColorDisabled);
		}
		Vis(self, m.дайКлюч(i), prev, pick(vis));
	}
}

void Pdb::AddThis(int тип, adr_t address, const ВекторМап<Ткст, Значение>& prev)
{
	const Type& t = дайТип(тип);
	AddThis(t.member, address, prev);
	AddThis(t.static_member, 0, prev);
	for(int i = 0; i < t.base.дайСчёт() && self.дайСчёт() < 2000; i++)
		AddThis(t.base[i].тип, t.base[i].address + address, prev);
}

void Pdb::This()
{
	ВекторМап<Ткст, Значение> prev = DataMap(locals);
	self.очисть();
	int q = ~framelist;
	if(q >= 0 && q < frame.дайСчёт()) {
		Фрейм& f = frame[q];
		for(int i = 0; i < f.local.дайСчёт(); i++) {
			if(f.local.дайКлюч(i) == "this") {
				Val val = f.local[i];
				if(val.ref > 0 || val.тип < 0)
					try {
						val = GetRVal(val);
					}
					catch(СиПарсер::Ошибка) {}
				AddThis(val.тип, val.address, prev);
				break;
			}
		}
	}
}

void Pdb::TryAuto(const Ткст& exp, const ВекторМап<Ткст, Значение>& prev)
{
	if(autos.найди(exp) < 0) {
		Visual r;
		try {
			СиПарсер p(exp);
			Val v = Exp(p);
			Visualise(r, v, 0);
		}
		catch(LengthLimit) {}
		catch(СиПарсер::Ошибка) {
			r.очисть();
		}
		if(r.part.дайСчёт())
			Vis(autos, exp, prev, pick(r));
	}
}

void Pdb::Autos()
{
	ВекторМап<Ткст, Значение> prev = DataMap(autos);
	autos.очисть();
	autotext.замени("//", "");
	try {
		СиПарсер p(autotext);
		TryAuto("this", prev);
		while(!p.кф_ли())
			if(p.ид_ли()) {
				Ткст exp = p.читайИд();
				TryAuto(exp, prev);
				for(;;) {
					if(p.сим('.') && p.ид_ли())
						exp << '.';
					else
					if(p.сим2('-', '>') && p.ид_ли())
						exp << "->";
					else
					if(p.сим2(':', ':') && p.ид_ли())
						exp << "::";
					else
						break;
					exp << p.читайИд();
					TryAuto(exp, prev);
				}
			}
			else
				p.пропустиТерм();
		autos.сортируй();
	}
	catch(LengthLimit) {}
	catch(СиПарсер::Ошибка) {}
}

void Pdb::Watches()
{
	ВекторМап<Ткст, Значение> prev = DataMap(watches);
	for(int i = 0; i < watches.дайСчёт(); i++) {
		bool ch;
		watches.уст(i, 1, Vis((Ткст)watches.дай(i, 0), prev, Visualise((Ткст)watches.дай(i, 0)), ch));
		if(ch)
			watches.устДисплей(i, 0, Single<RedDisplay>());
		else
			watches.устДисплей(i, 0, StdDisplay());
	}
}

bool Pdb::Подсказка(const Ткст& exp, РедакторКода::MouseTip& mt)
{
	DR_LOG("Pdb::Подсказка");
	Visual r;
	try {
		СиПарсер p(exp);
		Val v = Exp(p);
		Visualise(r, v, 0);
	}
	catch(LengthLimit) {}
	catch(СиПарсер::Ошибка) {
		DR_LOG("Pdb::Подсказка false");
		return false;
	}
	if(r.part.дайСчёт()) {
		mt.sz = r.дайРазм() + Размер(4, 4);
		mt.значение = RawPickToValue(pick(r));
		mt.дисплей = &visual_display;
		DR_LOG("Pdb::Подсказка true");
		return true;
	}
	return false;
}

void Pdb::Данные()
{
	switch(tab.дай()) {
	case TAB_AUTOS: Autos(); break;
	case TAB_LOCALS: Locals(); break;
	case TAB_THIS: This(); break;
	case TAB_WATCHES: Watches(); break;
	case TAB_MEMORY: memory.освежи(); break;
	case TAB_BTS: BTs(); break;
	}
	SetTree(tree_exp);
}

void Pdb::MemoryGoto(const Ткст& exp)
{
	try {
		СиПарсер p(exp);
		Val v = Exp(p);
		adr_t adr = 0;
		if(v.ref > 0)
			adr = GetRVal(v).address;
		else
		if(v.rvalue)
			adr = (adr_t)дайЦел64(v);
		else
			adr = v.address;
		memory.устКурсор(adr);
		memory.SetSc(adr);
		tab.уст(TAB_MEMORY);
	}
	catch(СиПарсер::Ошибка e) {
		Exclamation("Invalid expression!&" + DeQtf(e));
	}
}

void Pdb::CopyMenu(КтрлМассив& array, Бар& bar)
{
	auto CopyLine = [&](int i, КтрлМассив& array) {
		return Ткст() << array.дай(i, 0) << '=' << array.дай(i, 1).To<Visual>().дайТкст();
	};
	КтрлМассив *ap = &array;
	bar.добавь(array.курсор_ли(), "копируй", [=] {
		ClearClipboard();
		AppendClipboardText(CopyLine(ap->дайКурсор(), *ap));
	});
	bar.добавь(array.дайСчёт(), "копируй all", [=] {
		ClearClipboard();
		Ткст s;
		for(int i = 0; i < ap->дайСчёт(); i++)
			MergeWith(s, "\n", CopyLine(i, *ap));
		AppendClipboardText(s);
	});
	bar.Separator();
	bar.добавь("покажи тип", [=] { show_type = !show_type; Данные(); }).Check(show_type);
	bar.добавь("No pretty-printing", [=] { raw = !raw; Данные(); }).Check(raw);
}

void Pdb::MemMenu(КтрлМассив& array, Бар& bar, const Ткст& exp)
{
	if(bar.IsScanKeys())
		return;
	if(пусто_ли(exp))
		return;
	try {
		СиПарсер p(exp);
		Val v = Exp(p);
		bool sep = true;
		if(v.ref > 0 && !v.reference) {
			sep = false;
			bar.добавь("Memory at " + exp, THISBACK1(MemoryGoto, exp));
		}
		else
		if(v.rvalue) {
			sep = false;
			bar.добавь("Memory at 0x" + фмтЦелГекс((dword)дайЦел64(v)), THISBACK1(MemoryGoto, "&" + exp));
		}
		if(!v.rvalue) {
			sep = false;
			bar.добавь("Memory at &&" + exp, THISBACK1(MemoryGoto, "&" + exp));
		}
	}
	catch(СиПарсер::Ошибка e) {
	}
}

void Pdb::WatchMenu(Бар& bar, const Ткст& exp)
{
	bar.добавь(exp.дайСчёт(), "Watch", [=] { AddWatch(exp); }).Ключ(IK_DBL_CLICK);
}

void Pdb::DataMenu(КтрлМассив& array, Бар& bar)
{
	Ткст exp = array.дайКлюч();
	WatchMenu(bar, exp);
	MemMenu(array, bar, exp);
	bar.Separator();
	CopyMenu(autos, bar);
}

void Pdb::AddWatch(const Ткст& s)
{
	if(s.дайСчёт()) {
		SetTab(3);
		if(!watches.FindSetCursor(s))
			watches.добавь(s);
		Данные();
	}
}

void Pdb::AddWatch()
{
	Ткст s;
	if(EditPDBExpression("добавь watch", s, this))
		AddWatch(s);
}

void Pdb::EditWatch()
{
	Ткст s = watches.дайКлюч();
	if(EditPDBExpression("Edit watch", s, this)) {
		SetTab(2);
		watches.уст(0, s);
		Данные();
	}
}

void Pdb::WatchesMenu(Бар& bar)
{
	Ткст exp = watches.дайКлюч();
	if(!пусто_ли(exp))
		exp = "(" + exp + ")";
	MemMenu(watches, bar, exp);
	bar.Separator();
	CopyMenu(autos, bar);
	bar.Separator();
	bar.добавь(AK_ADDWATCH, [=] { AddWatch(); });
	bool b = watches.курсор_ли();
	bar.добавь(b, "Edit watch..", THISBACK(EditWatch));
	bar.добавь(b, "удали watch..", [=] {
		watches.DoRemove();
		Данные();
	});
	bar.добавь(b, "удали all watches..", [=] {
		if(PromptYesNo("удали all watches?")) { watches.очисть(); Данные(); }
	});
	bar.Separator();
	watches.StdBar(bar);
}

void Pdb::SetTab(int i)
{
	tab.уст(i);
	Вкладка();
}

void Pdb::ClearWatches()
{
	watches.очисть();
}

void Pdb::DropWatch(PasteClip& clip)
{
	Ткст s = ТкстПоток(дайТкст(clip)).дайСтроку();
	if(s.дайСчёт()) {
		watches.добавь(s);
		clip.SetAction(DND_COPY);
		Данные();
	}
}

#endif
