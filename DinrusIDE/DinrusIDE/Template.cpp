#include "DinrusIDE.h"

int ReadTemplateType(СиПарсер& p)
{
	static const char *nm[] = { "id", "filename", "option", "select", "text" };
	for(int i = 0; i < __countof(nm); i++)
		if(p.ид(nm[i]))
			return i;
	p.выведиОш("Неизвестный тип");
	return 0;
}

PackageTemplate ReadTemplate(const char *filename)
{
	PackageTemplate pt;
	ФайлВвод in(filename);
	Ткст header;
	Ткст l;
	for(;;) {
		if(in.кф_ли())
			break;
		l = in.дайСтроку();
		if(l[0] == '@' && l[1] == '@')
			break;
		header << l << '\n';
	}
	СиПарсер p(header, filename);
	p.передайИд("template");
	pt.имя = p.читайТкст();
	pt.main = pt.sub = false;
	for(;;) {
		if(p.ид("main"))
			pt.main = true;
		else
		if(p.ид("sub"))
			pt.sub = true;
		else
			break;
	}
	p.передайСим(';');
	while(!p.кф_ли()) {
		int тип = ReadTemplateType(p);
		TemplateItem& m = pt.элт.добавь();
		m.тип = тип;
		if(тип == TEMPLATEITEM_SELECT) {
			p.передайСим('(');
			do
				m.значение.добавь(p.читайТкст());
			while(p.сим(','));
			p.передайСим(')');
		}
		m.label = p.читайТкст();
		m.id = p.читайИд();
		if(p.сим('=')) {
			const char *b = p.дайУк();
			while(!p.сим_ли(';')) {
				if(p.кф_ли())
					p.выведиОш("неожиданный конец файла");
				p.пропустиТерм();
			}
			m.init = Ткст(b, p.дайУк());
		}
		p.передайСим(';');
	}
	for(;;) {
		pt.file.добавь();
		pt.file.верх().filename = l.середина(2);
		for(;;) {
			if(in.кф_ли())
				return pt;
			l = in.дайСтроку();
			if(l[0] == '@' && l[1] == '@')
				break;
			if(l[0] == '?' && l[1] == '?')
				pt.file.верх().condition = l.середина(2);
			else
				pt.file.верх().text << l << "\r\n";
		}
	}
}

int NoCr(int c)
{
	return c == '\r' ? 0 : c;
}

AppPreview::AppPreview()
{
	устФрейм(ViewFrame());
	добавьФрейм(sb);
	sb <<= THISBACK(промотай);
	NoWantFocus();
}

AppPreview::~AppPreview() {}

void AppPreview::колесоМыши(Точка p, int zdelta, dword keyflags) {
	sb.Wheel(zdelta);
}

void AppPreview::Выкладка()
{
	sb.устВсего(line.дайСчёт());
	sb.устСтраницу(дайРазм().cy / CourierZ(12).GetCy());
}

void AppPreview::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	int y = 0;
	int i = sb;
	while(y < sz.cy) {
		auto is_header = i < line.дайСчёт() && line[i].header;
		auto font = is_header ? ArialZ(12).Bold().Italic() : CourierZ(12);
		auto rect_color = is_header ? (IsDarkTheme() ? тёмнаяТема(светлоЦыан) : светлоЦыан) : SColorPaper;
		
		w.DrawRect(0, y, sz.cx, font.GetCy(), rect_color);
		if(i < line.дайСчёт())
			w.DrawText(0, y, line[i].text, font, SColorText);
		y += font.GetCy();
		i++;
	}
}

void AppPreview::добавь(const Ткст& data, bool header)
{
	Вектор<Ткст> l = разбей(фильтруй(data, NoCr), '\n', false);
	for(int i = 0; i < l.дайСчёт(); i++) {
		Строка& q = line.добавь();
		const char *s = l[i];
		while(*s == '\t') {
			q.text << "    ";
			s++;
		}
		q.text << s;
		q.header = header;
	}
	освежи();
	Выкладка();
}

int FilterPackageName(int c)
{
	return c == '_' || IsAlNum(c) || c == '/' ? c : c == '\\' ? '/' : 0;
}

TemplateDlg::TemplateDlg()
{
	CtrlLayoutOKCancel(*this, "Создать новый пакет");
	Sizeable().Zoomable();
	description <<= THISBACK(Preview);
	delay <<= THISBACK(Preview);
	delay.SetDelay(300);
	templist.добавьКолонку("Шаблон");
	templist.WhenEnterRow = THISBACK(EnterTemplate);
	ok.откл();
	package <<= THISBACK(EnableCreate);
	package.устФильтр(FilterPackageName);
	Иконка(IdeImg::CreatePackage());
}

TemplateDlg::~TemplateDlg() {}

const PackageTemplate& TemplateDlg::ActualTemplate()
{
	if(templist.дайКурсор() > 0)
		return pt[templist.дайКурсор() - 1];
	static PackageTemplate t;
	return t;
}

int IdFilter(int c)
{
	return c == '_' || IsAlNum(c) ? c : 0;
}

int FilenameFilter(int c)
{
	return c == '_' || IsAlNum(c) || c == '.' ? c : 0;
}

Ткст ToUpper_Caps(const Ткст& имя)
{
	Ткст output("");
	int str_len = имя.дайДлину();
	for (int i = 0; i < str_len; ++i)
	{
		int ch = имя[i];
		if (IsAlNum(ch))
		{
			output += взаг(ch);
			if (i < str_len - 1)
				if (заг_ли(ch) && проп_ли(имя[i + 1]))
					output += '_';
		}
		else
			output += '_';
	}
	return output;
}

МассивМап<Ткст, EscValue> TemplateDlg::MakeVars0()
{
	МассивМап<Ткст, EscValue> var;
	Ткст n = ~package;
	int q = n.найдирек('/');
	var.добавь("ПАКЕТ", q >= 0 ? n.середина(q + 1) : n);
	var.добавь("PACKAGE_TOUPPER", взаг(n));
	var.добавь("PACKAGE_TOUPPER_CAPS", ToUpper_Caps(n));
	var.добавь("DESCRIPTION", Ткст(~description));
	return var;
}

МассивМап<Ткст, EscValue> TemplateDlg::MakeVars()
{
	const PackageTemplate& tp = ActualTemplate();
	МассивМап<Ткст, EscValue> var = MakeVars0();
	for(int i = 0; i < tp.элт.дайСчёт(); i++)
		var.добавь(tp.элт[i].id, EscFromStdValue(~ctrl[i]));
	StdLib(var);
	return var;
}

void TemplateDlg::Preview()
{
	const PackageTemplate& tp = ActualTemplate();
	МассивМап<Ткст, EscValue> var = MakeVars();
	preview.очисть();
	for(int i = 0; i < tp.file.дайСчёт(); i++) {
		const FileTemplate& ft = tp.file[i];
		if(пусто_ли(ft.condition) || IsTrue(Evaluate(ft.condition, var))) {
			preview.добавь(расширь(ft.filename, var), true);
			preview.добавь(расширь(ft.text, var));
		}
	}
}

void TemplateDlg::создай()
{
	const PackageTemplate& tp = ActualTemplate();
	МассивМап<Ткст, EscValue> var = MakeVars();
	for(int i = 0; i < tp.file.дайСчёт(); i++) {
		const FileTemplate& ft = tp.file[i];
		if(пусто_ли(ft.condition) || IsTrue(Evaluate(ft.condition, var))) {
			Ткст фн = расширь(ft.filename, var);
			if(!пусто_ли(фн)) {
				фн = приставьИмяф(приставьИмяф(~nest, (Ткст)~package), фн);
				RealizePath(фн);
				сохраниФайл(фн, расширь(ft.text, var));
			}
		}
	}
	Пакет p;
	Ткст f = приставьИмяф(приставьИмяф(~nest, (Ткст)~package), (Ткст)~package + ".upp");
	p.грузи(f);
	p.description = ~description;
	p.сохрани(f);
}

void TemplateDlg::Inits()
{
	const PackageTemplate& tp = ActualTemplate();
	МассивМап<Ткст, EscValue> var = MakeVars0();
	for(int i = 0; i < tp.элт.дайСчёт(); i++) {
		Ткст init = tp.элт[i].init;
		if(!пусто_ли(init) && !ctrl[i].изменено()) {
			ctrl[i] <<= StdValueFromEsc(Evaluate(init, var));
			ctrl[i].ClearModify();
		}
	}
}

void TemplateDlg::EnterTemplate()
{
	const PackageTemplate& tp = ActualTemplate();
	МассивМап<Ткст, EscValue> var;
	var.добавь("ПАКЕТ", (Ткст)~package);
	StdLib(var);
	label.очисть();
	ctrl.очисть();
	Точка pos = templist.дайПрям().низЛево();
	pos.y += 8;
	int cx = template_end.дайПрям().right - pos.x;
	for(int i = 0; i < tp.элт.дайСчёт(); i++) {
		const TemplateItem& m = tp.элт[i];
		if(m.тип == TEMPLATEITEM_OPTION)
			ctrl.создай<Опция>().устНадпись(m.label);
		else {
			label.добавь().устНадпись(m.label).LeftPos(pos.x, cx).TopPos(pos.y);
			добавь(label.верх());
			pos.y += label.верх().дайМинРазм().cy;
			switch(m.тип) {
			case TEMPLATEITEM_ID: {
				EditField *e = new EditField;
				e->устФильтр(IdFilter);
				ctrl.добавь(e);
				break;
			}
			case TEMPLATEITEM_FILENAME: {
				EditField *e = new EditField;
				e->устФильтр(FilenameFilter);
				ctrl.добавь(e);
				break;
			}
			case TEMPLATEITEM_SELECT: {
				СписокБроса *d = new СписокБроса;
				for(int i = 0; i < m.значение.дайСчёт(); i++)
					d->добавь(i, m.значение[i]);
				if(m.значение.дайСчёт())
					d->SetIndex(0);
				ctrl.добавь(d);
				break;
			}
			case TEMPLATEITEM_TEXT: {
			    EditField *e = new EditField;
			    ctrl.добавь(e);
			    break;
			}
			}
		}
		добавь(ctrl.верх());
		ctrl.верх() << ~delay;
		ctrl.верх().LeftPos(pos.x, cx).TopPos(pos.y);
		pos.y += ctrl.верх().дайМинРазм().cy + 6;
		ctrl.верх().ClearModify();
	}
	Inits();
	Preview();
}

void TemplateDlg::LoadNest(const char *path, bool main, bool recurse)
{
	ФайлПоиск ff(приставьИмяф(path, "*.*"));
	while(ff) {
		Ткст p = приставьИмяф(path, ff.дайИмя());
		if(ff.папка_ли()) {
			if(recurse)
				LoadNest(p, main);
		}
		else
		if(впроп(дайРасшф(ff.дайИмя())) == ".upt") {
			try {
				PackageTemplate t = ReadTemplate(p);
				if(main && t.main || !main && t.sub)
					pt.добавь() = pick(t);
			}
			catch(СиПарсер::Ошибка e) {
				Exclamation("Шаблон пакета [* " + DeQtf(p) + "] повреждён&[* " + e);
			}
		}
		ff.следщ();
	}
}

void TemplateDlg::грузи(const Вектор<Ткст>& p, bool main)
{
	pt.очисть();
	for(int i = 0; i < p.дайСчёт(); i++) {
		LoadNest(p[i], main);
		nest.добавь(p[i]);
	}
	LoadNest(GetLocalDir(), main);
	LoadNest(дайПапкуФайла(конфигФайл("x")), main, false);
	сортируй(pt, FieldRelation(&PackageTemplate::имя, StdLess<Ткст>()));
	templist.очисть();
	templist.добавь("<empty>");
	for(int i = 0; i < pt.дайСчёт(); i++)
		templist.добавь(pt[i].имя);
	if(nest.дайСчёт())
		nest.SetIndex(0);
	templist.идиВНач();
	ActiveFocus(package);
}
