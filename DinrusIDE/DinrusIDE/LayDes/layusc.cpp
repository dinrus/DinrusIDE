#include "LayDes.h"

ВекторМап<Ткст, LayoutType>& LayoutTypes()
{
	static ВекторМап<Ткст, LayoutType> q;
	return q;
}

ВекторМап<Ткст, ВекторМап<Ткст, Ткст> >& LayoutEnums()
{
	static ВекторМап<Ткст, ВекторМап<Ткст, Ткст> > q;
	return q;
}

Ткст CurrentNamespace; // this is ugly hack, but better than rewrite everything

void ReadClass(СиПарсер& p, int kind)
{
	LayoutType& m = LayoutTypes().дайДобавь(p.читайИд());
	m.kind = kind;
	m.name_space = CurrentNamespace;
	p.передайСим('{');
	while(!p.сим('}')) {
		if(p.сим('>')) {
			Ткст n = p.читайИд();
			m.property.добавь().имя = n;
			if(пусто_ли(m.группа)) {
				int q = LayoutTypes().найди(n);
				if(q >= 0)
					m.группа = LayoutTypes()[q].группа;
			}
			p.передайСим(';');
		}
		else
		if(p.ид("группа")) {
			m.группа = p.читайТкст();
			p.передайСим(';');
		}
		else {
			Ткст id = p.читайИд();
			if(p.сим_ли('('))
				m.methods.дайДобавь(id) = ReadLambda(p);
			else {
				TypeProperty& r = m.property.добавь();
				r.тип = id;
				r.имя = p.читайИд();
				if(p.сим('=')) {
					const char *b = p.дайУк();
					while(!p.сим_ли(';') && !p.сим_ли('?') && !p.сим_ли('@'))
						p.пропустиТерм();
					r.defval = Ткст(b, p.дайУк());
				}
				if(p.сим('@'))
					r.level = p.читайЦел();
				if(p.сим('?'))
					r.help = p.читайТкст();
				p.передайСим(';');
			}
		}
	}
	p.сим(';');
}

void  LayLib();

void LayUscClean()
{
	CurrentNamespace.очисть();
	LayoutEnums().очисть();
	LayoutTypes().очисть();
	LayLib();
}

bool LayUscParse(СиПарсер& p)
{
	if(p.ид("namespace")) {
		CurrentNamespace.очисть();
		for(;;)
			if(p.ид_ли())
				CurrentNamespace << p.читайИд();
			else
			if(p.сим2(':', ':'))
				CurrentNamespace << "::";
			else
				break;
		p.сим(';');
	}
	else
	if(p.ид("ctrl"))
		ReadClass(p, LAYOUT_CTRL);
	else
	if(p.ид("subctrl"))
		ReadClass(p, LAYOUT_SUBCTRL);
	else
	if(p.ид("template"))
		ReadClass(p, LAYOUT_TEMPLATE);
	else
	if(p.ид("enum_property")) {
		ВекторМап<Ткст, Ткст>& e = LayoutEnums().дайДобавь(p.читайИд());
		p.передайСим('{');
		for(;;) {
			Ткст ключ = p.ткст_ли() ? p.читайТкст() : p.читайИд();
			Ткст text = ключ;
			if(p.сим(':'))
				text = p.ткст_ли() ? p.читайТкст() : p.читайИд();
			e.добавь(ключ, text);
			if(p.сим('}'))
				break;
			p.передайСим(',');
		}
		p.сим(';');
	}
	else
		return false;
	return true;
}
