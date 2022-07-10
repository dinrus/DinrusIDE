#include "LayDes.h"

struct VisGenDlg : public WithVisGenLayout<ТопОкно> {
	LayoutData&  layout;
	Вектор<int>  sel;

	void освежи();
	bool HasItem(const char *id);
	void Type();
	Ткст дайИмя();

	typedef VisGenDlg ИМЯ_КЛАССА;

	VisGenDlg(LayoutData& layout, const Вектор<int>& cursor);
};

bool VisGenDlg::HasItem(const char *id)
{
	for(int i = 0; i < layout.элт.дайСчёт(); i++)
		if(layout.элт[i].variable == id)
			return true;
	return false;
}

Ткст VisGenDlg::дайИмя()
{
	Ткст n = layout.имя;
	int l = n.дайСчёт() - 6;
	if(l > 0 && n.середина(l) == "Выкладка")
		n = n.середина(0, l);
	return n + "Dlg";
}

Ткст IdInitCaps(const Ткст& src)
{
	Ткст h = иницШапки(src);
	h.замени("_", "");
	return h;
}

void VisGenDlg::освежи()
{
	Ткст s;
	int q = ~тип;
	parent_ctrl.вкл(q < 3);
	bool b = q >= 3 && q <= 4;
	pars.вкл(b);
	brackets.вкл(b);
	label1.вкл(b);
	toupper1.вкл(b);
	tolower1.вкл(b);
	initcaps1.вкл(b);
	name1.вкл(b);
	dname1.вкл(b);
	quotes1.вкл(b && !label1);
	b = q == 4;
	label2.вкл(b);
	toupper2.вкл(b);
	tolower2.вкл(b);
	initcaps2.вкл(b);
	name2.вкл(b);
	dname2.вкл(b);
	quotes2.вкл(b && !label2);
	Ткст oce = "\tCtrlLayout";
	bool ok = false;
	if(HasItem("ok")) {
		ok = true;
		oce << "OK";
	}
	if(HasItem("cancel"))
		oce << "Cancel";
	if(HasItem("exit"))
		oce << "выход";
	Ткст n = ~имя;
	if(пусто_ли(n))
		n = дайИмя();
	
	Ткст b1, b2, b3;
	if(lambdas) {
		for(int i = 0; i < layout.элт.дайСчёт(); i++) {
			Ткст bn = layout.элт[i].variable;
			if(layout.элт[i].тип == "Кнопка" && findarg(bn, "cancel", "ok", "exit") < 0) {
				if(b2.дайСчёт() == 0)
					b2 = "\n";
				b2 << '\t' << bn << " << [=] { };\n";
			}
		}
	}
	if(buttons) {
		for(int i = 0; i < layout.элт.дайСчёт(); i++) {
			Ткст bn = layout.элт[i].variable;
			if(layout.элт[i].тип == "Кнопка" && findarg(bn, "cancel", "ok", "exit") < 0) {
				Ткст mn = IdInitCaps(bn);
				mn.замени("_", "");
				b1 << '\t' << "void " << mn << "();\n";
				b2 << '\t' << bn << " << [=] { " << mn << "(); };\n";
				b3 << '\n' << "void " << n << "::" << mn << "()\n{\n}\n";
			}
		}
		if(b1.дайСчёт())
			b1 << "\n";
	}
	
	Ткст base = parent_ctrl ? "КтрлРодитель" : "ТопОкно";

	if(q == 0) {
		s << "class " << n << " : public With" << layout.имя << "<" << base << "> {\n"
		  << b1
		  << "public:\n"
		  << "\t" << n << "();\n"
		  << "};\n"
		  << "\n"
		  << n << "::" << n << "()\n"
		  << "{\n"
		  << oce << "(*this, \"\");\n"
		  << b2
		  << "}\n"
		  << b3;
	}
	else
	if(q == 1) {
		s << "struct " << n << " : With" << layout.имя << "<" << base << "> {\n"
		  << b1
		  << "\t" << n << "();\n"
		  << "};\n"
		  << "\n"
		  << n << "::" << n << "()\n"
		  << "{\n"
		  << oce << "(*this, \"\");\n"
		  << b2
		  << "}\n"
		  << b3;
	}
	else
	if(q == 2) {
		Ткст n = ~имя;
		if(пусто_ли(n))
			n = "dlg";
		s << "\tWith" << layout.имя << "<" << base << "> " << n << ";\n"
		  << oce << '(' << n << ", \"\");\n";
		if(ok)
			s << "\tif(" << n << ".выполни() != IDOK)\n\t\treturn;\n";
		else
			s << '\t' << n << ".выполни();\n";
	}
	else
		for(int i = 0; i < sel.дайСчёт(); i++) {
			Ткст id1 = layout.элт[sel[i]].variable;
			Ткст id2 = id1;
			int w = layout.элт[sel[i]].FindProperty("устНадпись");
			Ткст lbl;
			if(w >= 0 && ткст_ли(~layout.элт[sel[i]].property[w]))
				lbl = какТкстСи(вУтф8((ШТкст)~layout.элт[sel[i]].property[w]));
			if(q <= 4) {
				if(label1)
					id1 = lbl;
				if(label2)
					id2 = lbl;
			}
			if(пусто_ли(id1) && (q != 4 || пусто_ли(id2)))
				continue;
			if(q == 5)
				s << "\t" << id1 << " << [=] { };\n";
			else
			if(q == 6)
				s << "\t" << IdInitCaps(id1) << "();\n";
			else
			if(q == 7)
				s << "\t" << id1 << " << [=] { " << IdInitCaps(id1) << "(); };\n";
			else {
				if((pars || brackets) && !(name1 || name2 || dname1 || dname2))
					s << ~имя;
				if(pars)
					s << '(';
				Ткст ss;
				if(name1 && !пусто_ли(~имя))
					s << ~имя << '.';
				if(toupper1)
					ss << взаг(id1);
				else
				if(tolower1)
					ss << впроп(id1);
				else
				if(initcaps1)
					ss << IdInitCaps(id1);
				else
					ss << id1;
				if(quotes1 && !label1)
					ss = какТкстСи(ss);
				if(dname1 && !пусто_ли(~имя))
					ss << '.' << ~имя;
				if(brackets)
					s << '[';
				s << ss;
				if(brackets)
					s << ']';
				if(q == 4) {
					s << ", ";
					Ткст ss;
					if(name2 && !пусто_ли(~имя))
						ss << ~имя << '.';
					if(toupper2)
						ss << взаг(id2);
					else
					if(tolower2)
						ss << впроп(id2);
					else
					if(initcaps2)
						ss << IdInitCaps(id2);
					else
						ss << id2;
					if(quotes2 && !label2)
						ss = какТкстСи(ss);
					if(dname2 && !пусто_ли(~имя))
						ss << '.' << ~имя;
					if(brackets)
						s << '[';
					s << ss;
					if(brackets)
						s << ']';
				}
				if(pars)
					s << ')';
				if(semicolon)
					s << ';';
				s << '\n';
			}
		}
	view <<= s;
}

void VisGenDlg::Type()
{
	Ткст n = дайИмя();
	buttons.откл();
	switch((int)~тип) {
	case 0:
	case 1:
		имя <<= дайИмя();
		buttons.вкл();
		break;
	case 2:
		имя <<= "dlg";
		break;
	default:
		имя <<= "";
	}
	освежи();
}

VisGenDlg::VisGenDlg(LayoutData& layout, const Вектор<int>& cursor)
:	layout(layout)
{
	тип <<= 0;
	CtrlLayoutOKCancel(*this, "Code generator");
	тип <<= THISBACK(Type);

	// needs to be before освежи to maintain the proper order of action
	toupper1 << [=] { tolower1 <<= false; initcaps1 <<= false; };
	tolower1 << [=] { toupper1 <<= false; initcaps1 <<= false; };
	initcaps1 << [=] { toupper1 <<= false; tolower1 <<= false; };

	toupper2 << [=] { tolower2 <<= false; initcaps2 <<= false; };
	tolower2 << [=] { toupper2 <<= false; initcaps2 <<= false; };
	initcaps2 << [=] { toupper2 <<= false; tolower2 <<= false; };

	for(Ктрл *q = дайПервОтпрыск(); q; q = q->дайСледщ())
		if(dynamic_cast<Опция *>(q))
			*q << [=] { освежи(); };
			
	имя << [=] { освежи(); };
	

	освежи();
	view.Highlight("cpp");
	view.HideBar();
	view.устШрифт(CourierZ(12));
	if(cursor.дайСчёт())
		sel <<= cursor;
	else
		for(int i = 0; i < layout.элт.дайСчёт(); i++)
			sel.добавь(i);
}

void LayDes::VisGen()
{
	if(пусто_ли(currentlayout))
		return;
	VisGenDlg dlg(CurrentLayout(), cursor);
	if(dlg.пуск() == IDOK)
		WriteClipboardText(~dlg.view);
}
