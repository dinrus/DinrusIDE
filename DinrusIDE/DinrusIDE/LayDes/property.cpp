#include "LayDes.h"

void ItemProperty::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	w.DrawRect(sz, SColorLtFace);
	w.DrawRect(0, дайВысоту() - 1, sz.cx, 1, SColorText);
	w.DrawText(2,
	           (EditField::GetStdHeight() + 6 - дайРазмТекста(имя, StdFont()).cy) / 2, имя,
	           дайДанные() == defval ? StdFont()() : StdFont().Bold());
}

int ItemProperty::дайВысоту() const
{
	return EditField::GetStdHeight() + 5;
}

bool ItemProperty::PlaceFocus(dword, int) { return false; }

void ItemProperty::устНабсим(byte charset)
{
}

void ItemProperty::читай(СиПарсер& p)
{
}

Ткст ItemProperty::сохрани() const
{
	return Null;
}

ВекторМап<Ткст, ItemProperty *(*)()>& ItemProperty::Factory()
{
	return Single< ВекторМап<Ткст, ItemProperty *(*)()> >();
}

void ItemProperty::регистрируй(const char *имя, ItemProperty *(*cr)())
{
	ПРОВЕРЬ(Factory().найди(имя) < 0);
	Factory().добавь(имя, cr);
}

Ткст ReadPropertyParam(СиПарсер& p)
{
	const char *b = p.дайУк();
	int level = 0;
	while(!p.кф_ли()) {
		if(p.сим_ли(';'))
			break;
		if(p.сим_ли(')') && level == 0)
			break;
		if(p.сим(')')) {
			if(level == 0)
				break;
			level--;
		}
		else
		if(p.сим('('))
			level++;
		else
			p.пропустиТерм();
	}
	return обрежьЛево(обрежьПраво(Ткст(b, p.дайУк())));
}

ItemProperty *ItemProperty::создай(const Ткст& имя)
{
	int q = Factory().найди(имя);
	if(q >= 0)
		return (*Factory()[q])();
	return NULL;
}

void RawProperty::читай(СиПарсер& p)
{
	editor.устДанные(вЮникод(ReadPropertyParam(p), CHARSET_WIN1252));
}

Ткст RawProperty::сохрани() const
{
	Ткст s = ~editor;
	СиПарсер p(s);
	int level = 0;
	try {
		while(!p.кф_ли()) {
			if(p.сим('('))
				level++;
			else
			if(p.сим(')'))
				level--;
			else
				p.пропустиТерм();
		}
	}
	catch(СиПарсер::Ошибка) {}
	if(level < 0)
		s = Ткст('(', -level) + s;
	else
		s = Ткст(')', level) + s;
	return s;
}

struct IntProperty : public EditorProperty<EditInt> {
	virtual void   читай(СиПарсер& p) {
		if(p.ид("Null"))
			editor.устДанные(Null);
		else
			editor.устДанные(p.читайЦел());
	}
	virtual Ткст сохрани() const {
		int q = ~editor;
		return пусто_ли(q) ? "Null" : какТкст(q);
	}

	IntProperty() {
		добавь(editor.HSizePosZ(100, 2).TopPos(2));
	}

	static ItemProperty *создай() { return new IntProperty; }
};

struct DoubleProperty : public EditorProperty<EditDouble> {
	virtual void   читай(СиПарсер& p) {
		if(p.ид("Null"))
			editor.устДанные(Null);
		else
			editor.устДанные(p.читайДво());
	}
	virtual Ткст сохрани() const {
		return фмтДво(~editor, 10);
	}

	DoubleProperty() {
		добавь(editor.HSizePosZ(100, 2).TopPos(2));
	}

	static ItemProperty *создай() { return new DoubleProperty; }
};

struct StringProperty : public EditorProperty<EditString> {
	virtual void     читай(СиПарсер& p) {
		if(p.ид("Null"))
			editor.устДанные(Null);
		else
			editor.устДанные(p.читайТкст());
	}
	virtual Ткст   сохрани() const {
		return какТкстСи(~editor);
	}
	StringProperty() {
		добавь(editor.HSizePosZ(100, 2).TopPos(2));
	}

	static ItemProperty *создай() { return new StringProperty; }
};

struct BoolProperty : public EditorProperty<Опция> {
	virtual void читай(СиПарсер& p) {
		if(p.ид("true"))
			editor.устДанные(1);
		else {
			p.ид("false");
			editor.устДанные(0);
		}
	}
	virtual Ткст   сохрани() const {
		return (int)~editor ? "true" : "false";
	}
	BoolProperty() {
		добавь(editor.HSizePosZ(100, 2).TopPos(3));
	}

	static ItemProperty *создай() { return new BoolProperty; }
};

struct ColorProperty : public EditorProperty<ColorPusher> {
	virtual Ткст   сохрани() const           { return "РНЦП::" + FormatColor(~editor); }
	virtual void     читай(СиПарсер& p);

	void FontChanged();
	void Perform();

	typedef ColorProperty ИМЯ_КЛАССА;

	ColorProperty() {
		добавь(editor.HSizePosZ(100, 2).TopPos(2));
		editor.WithText().SColors().NullText("Null").Track();
	}

	static ItemProperty *создай() { return new ColorProperty; }
};

void EatUpp(СиПарсер& p)
{
	p.ид("РНЦП");
	p.сим2(':', ':');
}

void ColorProperty::читай(СиПарсер& p)
{
//	EatUpp(p);
	if(p.ид("Null")) {
		editor.устДанные(Null);
		return;
	}
	p.сим(':');
	editor.устДанные(ReadColor(p));
}

ИНИЦБЛОК
{
	ItemProperty::регистрируй("raw", RawProperty::создай);
	ItemProperty::регистрируй("int", IntProperty::создай);
	ItemProperty::регистрируй("double", DoubleProperty::создай);
	ItemProperty::регистрируй("Ткст", StringProperty::создай);
	ItemProperty::регистрируй("bool", BoolProperty::создай);
	ItemProperty::регистрируй("Цвет", ColorProperty::создай);

	void RegisterTextProperties(); RegisterTextProperties();
	void RegisterFontProperty(); RegisterFontProperty();
}
