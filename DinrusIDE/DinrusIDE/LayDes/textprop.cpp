#include "LayDes.h"

struct EditStringNE : EditString {
	virtual bool Ключ(dword ключ, int rep) {
		return ключ == K_ENTER ? true : EditString::Ключ(ключ, rep);
	}
};

struct LTProperty : public ItemProperty, public Преобр {
	EditStringNE           lid;
	ButtonOption           context, id;
	byte                   charset;

	void ид();
	void Контекст();
	void EditAction();
	void SyncLid();

	virtual void     устНабсим(byte charset);
	virtual void     читай(СиПарсер& p);
	virtual Ткст   сохрани() const;
	virtual void     уст(const ШТкст& text) = 0;
	virtual ШТкст  дай() const = 0;
	virtual int      фильтруй(int chr) const;
	virtual Значение    фмт(const Значение& q) const      { return q; }
	virtual Значение    скан(const Значение& text) const     { return text; }

	typedef LTProperty ИМЯ_КЛАССА;

	LTProperty();
};

int FilterOutCr(int c)
{
	return c == '\r' ? 0 : c;
}

Ткст NoCr(const Ткст& s)
{
	return фильтруй(s, FilterOutCr);
}

int  LTProperty::фильтруй(int chr) const
{
	return charset == CHARSET_UTF8 ? chr : изЮникода(chr, charset) == DEFAULTCHAR ? 0 : chr;
}

void LTProperty::EditAction()
{
	UpdateActionRefresh();
}

LTProperty::LTProperty()
{
	context = LayImg::Context();
	context.WhenAction = THISBACK(Контекст);
	id = LayImg::Id();
	id.WhenAction = THISBACK(ид);
	charset = CHARSET_UTF8; //!! not good, but better than a crash; TRC 06/04/10//TODO
}

void LTProperty::SyncLid()
{
	lid.вкл(context || id);
}

void LTProperty::ид()
{
	if(id)
		context = false;
	SyncLid();
	if(lid.включен_ли())
		lid.устФокус();
}

void LTProperty::Контекст()
{
	if(context)
		id = false;
	SyncLid();
	if(lid.включен_ли())
		lid.устФокус();
}

void LTProperty::устНабсим(byte cs)
{
	charset = cs;
}

void LTProperty::читай(СиПарсер& p)
{
	lid <<= Null;
	if(p.ид("t_")) {
		p.передайСим('(');
		Ткст q = p.читайТкст();
		p.передайСим(')');
		for(const char *s = ~q; *s; s++) {
			if(*s == '\v' && s[1] != '\v') {
				context = true;
				lid <<= Ткст(~q, s);
				уст(вЮникод(s + 1, charset));
				SyncLid();
				return;
			}
			if(*s == '\a' && s[1] != '\a') {
				id = true;
				lid <<= Ткст(~q, s);
				уст(вЮникод(s + 1, charset));
				SyncLid();
				return;
			}
		}
		уст(вЮникод(q, charset));
	}
	else
	if(p.ткст_ли())
		уст(вЮникод(NoCr(p.читайТкст()), CHARSET_WIN1250));
	else
		уст(Null);
	SyncLid();
}

Ткст LTProperty::сохрани() const
{
	Ткст px = ~lid;
	Ткст txt = изЮникода(дай(), charset);
	if(id)
		return "t_(" + какТкстСи(px + "\a" + txt) + ')';
	else
	if(context)
		return "t_(" + какТкстСи(px + "\v" + txt) + ')';
	else
		return "t_(" + какТкстСи(txt) + ')';
}

template <class Editor>
struct TextEditProperty : public LTProperty {
	virtual Значение    дайДанные() const                { return дай(); }
	virtual ШТкст  дай() const                    { return ~editor; }
	virtual void     уст(const ШТкст& text)       { editor <<= text; }

	virtual bool     PlaceFocus(dword k, int c)     { editor.устФокус(); return editor.Ключ(k, c); }

protected:
	Editor     editor;
	ФреймНиз<КтрлРодитель> p;

	TextEditProperty() {
		editor.WhenAction = callback(this, &TextEditProperty::EditAction);
		int c = EditField::GetStdHeight();
		p.устВысоту(c);
		editor.добавьФрейм(p);
		p.добавь(lid.HSizePos(0, 2 * c).TopPos(0, c));
		p.добавь(id.TopPos(0, c).RightPos(0, c));
		p.добавь(context.TopPos(0, c).RightPos(c, c));
	}
};

template <class Editor>
struct SmartTextEditProperty : public TextEditProperty<Editor> {
	typedef TextEditProperty<Editor> B;
	virtual ШТкст  дай() const {
		ШТкст text = ~B::editor;
		ШТкст r;
		for(const wchar *s = text; *s; s++) {
			if(*s == '\\' && s[1] == '1') {
				r.конкат('\1');
				s++;
			}
			else
//			if(*s == '\\' && s[1] == 'b') {
//				r.конкат('\b');
//				s++;
//			}
//			else
			if(*s == '\n' || *s >= ' ')
				r.конкат(*s);
		}
		return r;
	}
	virtual void уст(const ШТкст& text) {
		ШТкст r;
		for(const wchar *s = text; *s; s++) {
			if(*s == '\1')
				r.конкат("\\1");
			else
//			if(*s == '\b')
//				r.конкат("\\b");
//			else
			if(*s == '\n' || *s >= ' ')
				r.конкат(*s);
		}
		B::editor.устНабсим(B::charset);
		B::editor <<= r;
	}
};

struct TextProperty : public SmartTextEditProperty<EditString>
{
	virtual int      дайВысоту() const {
		return 2 * EditField::GetStdHeight() + 6;
	}

	TextProperty() {
		добавь(editor.HSizePosZ(100, 2).TopPos(2));
		editor.SetConvert(*this);
	}

	static ItemProperty *создай() { return new TextProperty; }
};

struct DocProperty : public SmartTextEditProperty<ДокРедакт>
{
	Кнопка  large;

	virtual int      дайВысоту() const {
		return EditField::GetStdHeight() + 6 + Zy(120) + 1;
	}

	void LargeEdit();

	typedef DocProperty ИМЯ_КЛАССА;

	DocProperty() {
		editor.UpDownLeave();
		добавь(editor.HSizePosZ(2, 2).TopPos(EditField::GetStdHeight() + 2, Zy(120)));
		добавь(large.RightPosZ(2, DPI(16)).TopPos(2, DPI(16)));
		large.устНадпись("...");
		large <<= THISBACK(LargeEdit);
	}

	static ItemProperty *создай() { return new DocProperty; }
};

Прям s_texteditorpos;

void DocProperty::LargeEdit()
{
	editor.устФокус();
	ТопОкно w;
	w.NoCenter();
	w.Sizeable();
	w.устПрям(s_texteditorpos);
	w.Титул(имя.вШТкст());
	ДокРедакт edit;
	edit.устШрифт(LayFont2());
	edit <<= дайДанные();
	w.добавь(edit.SizePos());
	w.пуск();
	editor.устДанные(~edit);
	s_texteditorpos = w.дайПрям();
}

struct QtfProperty : public TextEditProperty<RichTextView>
{
	Кнопка  large;

	virtual int    дайВысоту() const           { return EditField::GetStdHeight() + 6 + 120 + 1; }

	void LargeEdit();

	typedef QtfProperty ИМЯ_КЛАССА;

	QtfProperty() {
		добавь(editor.HSizePosZ(2, 2).TopPos(EditField::GetStdHeight() + 2, Zy(120)));
		добавь(large.RightPosZ(2, 16).TopPos(2, Zy(16)));
		large.устНадпись("...");
		large <<= THISBACK(LargeEdit);
		editor.SetZoom(Zoom(1, 7));
		defval = ~editor;
	}

	static ItemProperty *создай() { return new QtfProperty; }
};

void QtfProperty::LargeEdit()
{
	large.устФокус();
	ТопОкно w;
	w.NoCenter();
	w.Sizeable();
	w.Титул(имя.вШТкст());
	w.устПрям(s_texteditorpos);
	RichEditWithToolBar edit;
	edit <<= дайДанные();
	w.добавь(edit.SizePos());
	w.пуск();
	editor.устДанные(~edit);
	s_texteditorpos = w.дайПрям();
}

void RegisterTextProperties() {
	ItemProperty::регистрируй("устТекст", TextProperty::создай);
	ItemProperty::регистрируй("Qtf", QtfProperty::создай);
	ItemProperty::регистрируй("Doc", DocProperty::создай);
}
