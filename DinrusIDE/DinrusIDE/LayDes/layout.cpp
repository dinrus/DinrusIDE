#include "LayDes.h"

void LayoutUndo::сунь(const Ткст& state)
{
	if(stack.дайСчёт()) {
		if(state != stack.верх()) {
			stack.верх() = DiffPack(state, stack.верх());
			stack.добавь(state);
		}
	}
	else
		stack.добавь(state);
}

Ткст LayoutUndo::вынь()
{
	ПРОВЕРЬ(stack.дайСчёт());
	Ткст state = stack.вынь();
	if(stack.дайСчёт())
		stack.верх() = DiffUnpack(state, stack.верх());
	return state;
}

Ткст ReadVar(СиПарсер& p) {
	Ткст var;
	for(;;) {
		if(p.ид_ли())
			var << p.читайИд();
		else
		if(p.цел_ли())
			var << p.читайЦел();
		else
		if(p.сим('.'))
			var << '.';
		else
		if(p.сим('['))
			var << '[';
		else
		if(p.сим(']'))
			var << ']';
		else
			return var;
	}
}

Массив<LayoutItem> ReadItems(СиПарсер& p, byte charset)
{
	Массив<LayoutItem> items;
	for(;;) {
		Ткст тип;
		if(p.ид("ЭЛТ")) {
			p.передайСим('(');
			for(;;)
				if(p.сим2(':', ':'))
					тип << "::";
				else
				if(p.ид_ли())
					тип << p.ReadIdt();
				else
					break;
			p.передайСим(',');
		}
		else
		if(p.ид("UNTYPED"))
			p.передайСим('(');
		else
			return items;
		LayoutItem& m = items.добавь();
		int q = тип.найдирек(':');
		if(q >= 0)
			тип = тип.середина(q + 1);
		m.создай(тип);
		m.устНабсим(charset);
		m.variable = ReadVar(p);
		if(strncmp(m.variable, "dv___", 5) == 0)
			m.variable.очисть();
		p.передайСим(',');
		m.ReadProperties(p, charset);
		p.передайСим(')');
	}
}

void LayoutData::устНабсим(byte cs)
{
	charset = cs;
	for(int i = 0; i < элт.дайСчёт(); i++)
		элт[i].устНабсим(charset);
}

void  LayoutData::читай(СиПарсер& p)
{
	p.передайИд("LAYOUT");
	p.передайСим('(');
	имя = p.читайИд();
	p.передайСим(',');
	size.cx = p.читайЦел();
	p.передайСим(',');
	size.cy = p.читайЦел();
	p.передайСим(')');
	элт = ReadItems(p, charset);
	p.передайИд("END_LAYOUT");
}

Ткст LayoutData::сохрани(int y, const Ткст& eol)
{
	Ткст out;
	out << "LAYOUT(" << имя << ", " << size.cx << ", " << size.cy << ")" << eol;
	for(int i = 0; i < элт.дайСчёт(); i++) {
		out << элт[i].сохрани(i, y, eol);
	}
	out << "END_LAYOUT" << eol;
	return out;
}

Ткст LayoutData::сохрани(const Вектор<int>& sel, int y, const Ткст& eol)
{
	Вектор<int> cs(sel, 1);
	сортируй(cs);
	Ткст out;
	out << "LAYOUT(" << имя << ", " << size.cx << ", " << size.cy << ")" << eol;
	for(int i = 0; i < cs.дайСчёт(); i++)
		out << элт[cs[i]].сохрани(cs[i], y, eol);
	out << "END_LAYOUT" << eol;
	return out;
}

Ткст LayoutData::MakeState()
{
	byte cs = charset;
	устНабсим(CHARSET_UTF8);
	Ткст out = сохрани(0, "\r\n");
	устНабсим(cs);
	return out;
}

void LayoutData::LoadState(const Ткст& s)
{
	СиПарсер p(s);
	byte cs = charset;
	устНабсим(CHARSET_UTF8);
	читай(p);
	устНабсим(cs);
}

void LayoutData::SaveState()
{
	undo.сунь(MakeState());
	redo.очисть();
}

bool LayoutData::IsUndo()
{
	return undo && (undo.дайСчёт() > 1 || undo.верх() != MakeState());
}

bool LayoutData::IsRedo()
{
	return redo && (redo.дайСчёт() > 1 || redo.верх() != MakeState());
}

void LayoutData::Do(LayoutUndo& u1, LayoutUndo& u2)
{
	Ткст s = MakeState();
	u2.сунь(s);
	Ткст q = u1.вынь();
	if(q == s && u1)
		q = u1.вынь();
	LoadState(q);
}

void LayoutData::Undo()
{
	Do(undo, redo);
}

void LayoutData::Redo()
{
	Do(redo, undo);
}
