#include "CtrlLib.h"

namespace РНЦП {

struct KeyBinding : Движимое<KeyBinding> {
	ИнфОКлюче    *ключ;
	const char *id;
	dword       def[4];
};

static ВекторМап<Ткст, Вектор<KeyBinding> >& sKeys()
{
	static ВекторМап<Ткст, Вектор<KeyBinding> > k;
	return k;
}

void регистрируйПривязкуКлюча(const char *группа, const char *id, ИнфОКлюче& (*info)())
{
	ВекторМап<Ткст, Вектор<KeyBinding> >& keys = sKeys();
	KeyBinding& k = keys.дайДобавь(группа).добавь();
	k.id = id;
	ИнфОКлюче& f = (*info)();
	k.ключ = &f;
	memcpy(k.def, f.ключ, sizeof(f.ключ));
}

ИнфОКлюче& AK_NULL()
{
	static ИнфОКлюче x = { "" };
	return x;
}

void SetDefaultKeys() {
	ВекторМап<Ткст, Вектор<KeyBinding> >& g = sKeys();
	for(int i = 0; i < g.дайСчёт(); i++) {
		Вектор<KeyBinding>& k = g[i];
		for(int i = 0; i < k.дайСчёт(); i++) {
			KeyBinding& b = k[i];
			ИнфОКлюче& f = *b.ключ;
			memcpy(f.ключ, b.def, sizeof(f.ключ));
		}
	}
}

struct KeyDisplay : Дисплей {
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q,
	                   Цвет ink, Цвет paper, dword flags) const {
		w.DrawRect(r, paper);
		Ткст txt = GetKeyDesc(int(q));
		int tcy = дайРазмТекста(txt, StdFont()).cy;
		w.DrawText(r.left + 2, r.top + (r.устВысоту() - tcy) / 2, txt, StdFont(), ink);
	}
};

struct KeyCtrl : Ктрл {
	int ключ;

	virtual void  устДанные(const Значение& v) { ключ = v; освежи(); }
	virtual Значение дайДанные() const         { return ключ; }
	virtual void  сфокусирован()              { освежи(); }
	virtual void  расфокусирован()             { освежи(); }

	virtual void  леваяВнизу(Точка p, dword) { устФокус(); }

	virtual void рисуй(Draw& w) {
		Single<KeyDisplay>().рисуй(w, дайРазм(), ключ,
		                           естьФокус() ? SColorLight : SColorText,
		                           естьФокус() ? SColorHighlight : SColorPaper, 0);
	}

	virtual bool Ключ(dword _key, int) {
		if(_key == K_ENTER || _key == K_ESCAPE)
			return false;
		if((_key & K_KEYUP) || _key < K_CHAR_LIM || _key == K_SHIFT_KEY || _key == K_ALT_KEY || _key == K_CTRL_KEY)
			return true;
		if(_key == K_SPACE || _key == K_DELETE || _key == K_BACKSPACE)
			ключ = 0;
		else
			ключ = _key;
		UpdateActionRefresh();
		return true;
	}

	KeyCtrl() {
		Transparent();
		устФрейм(EditFieldFrame());
		ключ = 0;
	}
};

struct KeysDlg : WithKeysLayout<ТопОкно> {
	virtual bool  Ключ(dword ключ, int);

	void EnterGroup();
	void KeyEdit();
	void Defaults();
	void CopyKeys();

	void Perform();

	typedef KeysDlg ИМЯ_КЛАССА;

	KeysDlg() {
		CtrlLayoutOKCancel(*this, t_("Configure keyboard shortcuts"));
		группа.добавьКолонку(t_("Группа"));
		группа.WhenEnterRow = THISBACK(EnterGroup);
		группа.NoGrid();
		keys.добавьКолонку(t_("Action"));
		keys.добавьКолонку(t_("Primary")).Ctrls<KeyCtrl>();
		keys.добавьКолонку(t_("Secondary")).Ctrls<KeyCtrl>();
		keys.SetLineCy(EditField::GetStdHeight() + 2);
		keys.NoHorzGrid().NoCursor().EvenRowColor();
		keys.ColumnWidths("182 132 133");
		keys.WhenCtrlsAction = THISBACK(KeyEdit);
		defaults <<= THISBACK(Defaults);
	}
};

bool KeysDlg::Ключ(dword ключ, int count)
{
	if(ключ == K_F3) { // 'hidden trick' to retrieve the document
		Ткст qtf;
		for(int i = 0; i < sKeys().дайСчёт(); i++) {
			qtf << "&&&" << DeQtf(sKeys().дайКлюч(i)) << "&&{{1:1 ";
			const Вектор<KeyBinding>& b = sKeys()[i];
			for(int i = 0; i < b.дайСчёт(); i++) {
				const ИнфОКлюче& a = *b[i].ключ;
				const char *text = a.имя;
				if(*text == '\3')
					text++;
				if(a.ключ[0]) {
					if(i)
						qtf << ":: ";
					qtf << DeQtf(GetKeyDesc(a.ключ[0]));
					if(a.ключ[1])
						qtf << ", " << DeQtf(GetKeyDesc(a.ключ[1]));
					qtf << ":: " << DeQtf(text);
				}
			}
			qtf << "}}";
		}
		WriteClipboard("QTF", qtf);
		бипВосклицание();
		return true;
	}
	return ТопОкно::Ключ(ключ, count);
}

void KeysDlg::EnterGroup()
{
	keys.очисть();
	Ткст g = группа.дайКлюч();
	int q = sKeys().найди(g);
	if(q < 0)
		return;
	const Вектор<KeyBinding>& b = sKeys()[q];
	for(int i = 0; i < b.дайСчёт(); i++) {
		const ИнфОКлюче& a = *b[i].ключ;
		const char *text = a.имя;
		if(*text == '\3')
			text = t_GetLngString(text + 1);
		keys.добавь(text, (int)a.ключ[0], (int)a.ключ[1], (int)a.ключ[2]);
	}
	keys.идиВНач();
}

void KeysDlg::KeyEdit()
{
	Ткст g = группа.дайКлюч();
	int q = sKeys().найди(g);
	if(q < 0)
		return;
	for(int i = 0; i < keys.дайСчёт(); i++) {
		ИнфОКлюче& a = *(sKeys()[q][i].ключ);
		a.ключ[0] = (dword)(int)keys.дай(i, 1);
		a.ключ[1] = (dword)(int)keys.дай(i, 2);
	}
}

void KeysDlg::Defaults()
{
	if(PromptYesNo("Восстановить дефолтные ключи?")) {
		SetDefaultKeys();
		EnterGroup();
	}
}

void KeysDlg::Perform()
{
	Ткст bkup = сохраниКлючи();
	for(int i = 0; i < sKeys().дайСчёт(); i++)
		группа.добавь(sKeys().дайКлюч(i));
	группа.сортируй();
	группа.идиВНач();
	if(пуск() != IDOK)
		восстановиКлючи(bkup);
}

void редактируйКлючи()
{
	KeysDlg().Perform();
}

int CharFilterNoSpace(int c)
{
	return c == ' ' ? 0 : c;
}

Ткст сохраниКлючи()
{
	Ткст out;
	const ВекторМап<Ткст, Вектор<KeyBinding> >& g = sKeys();
	for(int i = 0; i < g.дайСчёт(); i++) {
		out << "- " << какТкстСи(g.дайКлюч(i)) << ";\r\n";
		const Вектор<KeyBinding>& k = g[i];
		for(int i = 0; i < k.дайСчёт(); i++) {
			const KeyBinding& b = k[i];
			const ИнфОКлюче& f = *b.ключ;
			for(int i = 0; i < 4; i++)
				if(f.ключ[i]) {
					out << фмт("%-25s", b.id);
					bool b = false;
					for(int i = 0; i < 4; i++)
						if(f.ключ[i]) {
							if(b)
								out << ", ";
							out << ' ' << фильтруй(GetKeyDesc(f.ключ[i]), CharFilterNoSpace);
							b = true;
						}
					out << ";\r\n";
					break;
				}
		}
		out << "\r\n";
	}
	return out;
}

dword разбериОписКлюча(СиПарсер& p)
{
	dword f = 0;
	for(;;) {
		if(p.ид("Ктрл"))
			f |= K_CTRL;
		else
		if(p.ид("Shift"))
			f |= K_SHIFT;
		else
		if(p.ид("Alt"))
			f |= K_ALT;
		else
			break;
		p.передайСим('+');
	}
	if(p.число_ли()) {
		uint32 q = p.читайЧисло(16);
		if(q <= 9)
			return f | (K_0 + q);
		return f | q;
	}
	if(p.сим('[')) {
		int q = p.дайСим();
		p.передайСим(']');
		switch(q) {
		case '`':  return f | K_CTRL_GRAVE;
		case '-':  return f | K_CTRL_MINUS;
		case '=':  return f | K_CTRL_EQUAL;
		case '\\': return f | K_CTRL_BACKSLASH;
		case ';':  return f | K_CTRL_SEMICOLON;
		case '\'': return f | K_CTRL_APOSTROPHE;
		case ',':  return f | K_CTRL_COMMA;
		case '.':  return f | K_CTRL_PERIOD;
		case '/':  return f | K_CTRL_SLASH;
		case '[':  return f | K_CTRL_LBRACKET;
		case ']':  return f | K_CTRL_RBRACKET;
		}
		throw СиПарсер::Ошибка("");
	}
	Ткст kid = p.читайИд();
	/*
	if(kid.дайДлину() == 1 && *kid >= 'A' && *kid <= 'Z')
		return f | (K_A + *kid - 'A');
	if(kid.дайДлину() == 2 && kid[0] == 'F' && цифра_ли(kid[1]) && kid[1] != '0')
		return f | (K_F1 + (kid[1] - '1'));
	static struct {
		int ключ;
		const char *имя;
	} nkey[] = {
		{ K_TAB, "Вкладка" }, { K_SPACE, "Space" }, { K_RETURN, "войди" }, { K_BACKSPACE, "Backspace" },
		{ K_CAPSLOCK, "Caps Замок" }, { K_ESCAPE, "Esc" },
		{ K_END, "стоп" }, { K_HOME, "Home" },
		{ K_LEFT, "лево" }, { K_UP, "Up" }, { K_RIGHT, "право" }, { K_DOWN, "Down" },
		{ K_INSERT, "вставь" }, { K_DELETE, "Delete" },
		{ K_ALT_KEY, "Alt" }, { K_SHIFT_KEY, "Shift" }, { K_CTRL_KEY, "Ктрл" },
		{ K_F10, "F10" }, { K_F11, "F11" }, { K_F12, "F12" }, { K_PAGEUP, "страницейВыше" }, { K_PAGEDOWN, "страницейНиже" },
		{ 0, NULL }
	};
	*/
	
	static ВекторМап<Ткст, int> map;
	ONCELOCK  {
		extern Кортеж<dword, const char *> KeyNames__[];
		for(int i = 0; KeyNames__[i].a; i++) {
			Ткст n = KeyNames__[i].b;
			int q = n.найди('\v');
			if(q)
				n = n.середина(q + 1);
			map.добавь(n, KeyNames__[i].a);
		}
	}
	int q = map.найди(kid);
	if(q >= 0)
		return f | map[q];
	if(kid == "Num") {
		p.передайСим('[');
		int q = p.дайСим();
		p.передайСим(']');
		if(q == '*') return f | K_MULTIPLY;
		if(q == '+') return f | K_ADD;
		if(q == '-') return f | K_SUBTRACT;
		if(q == '/') return f | K_DIVIDE;
		throw СиПарсер::Ошибка("");
	}
	return f | p.читайЧисло(16);
}

void восстановиКлючи(const Ткст& data)
{
	SetDefaultKeys();
	СиПарсер p(data);
	try {
		while(!p.кф_ли()) {
			try {
				p.передайСим('-');
				Ткст группа;
				if(p.ид_ли())
					группа = p.читайИд();
				else
					группа = p.читайТкст();
				p.передайСим(';');
				int q = sKeys().найди(группа);
				if(q < 0)
					return;
				const Вектор<KeyBinding>& b = sKeys()[q];
				while(!p.кф_ли() && p.ид_ли())
					try {
						Ткст id = p.читайИд();
						int i;
						for(i = 0; i < b.дайСчёт(); i++)
							if(b[i].id == id)
								break;
						int q = 0;
						do {
							dword ключ = разбериОписКлюча(p);
							if(i < b.дайСчёт() && q < 4)
								b[i].ключ->ключ[q++] = ключ;
						}
						while(p.сим(','));
						p.передайСим(';');
					}
					catch(СиПарсер::Ошибка) {
						while(!p.кф_ли() && !p.сим(';'))
							p.пропустиТерм();
					}
			}
			catch(СиПарсер::Ошибка) {
				while(!p.кф_ли() && !p.сим(';'))
					p.пропустиТерм();
			}
		}
	}
	catch(СиПарсер::Ошибка) {}
}

Ткст дайОпис(const ИнфОКлюче& f, bool parenthesis)
{
	return f.ключ[0] ? parenthesis ? "(" + GetKeyDesc(f.ключ[0]) + ")" : GetKeyDesc(f.ключ[0]) : Ткст();
}

bool сверь(const ИнфОКлюче& k, dword ключ)
{
	return findarg(ключ, k.ключ[0], k.ключ[1], k.ключ[2], k.ключ[3]) >= 0;
}

}
