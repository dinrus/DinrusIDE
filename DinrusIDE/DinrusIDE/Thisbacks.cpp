#include "DinrusIDE.h"

bool IsCb(Ткст t) {
	int q = t.найди('<');
	if(q >= 0)
		t.обрежь(q);
	if(t.начинаетсяС("РНЦП::"))
		t = t.середина(5);
	return  t == "Событие<> " || t == "Событие<" || t == "Событие<> 2" || t == "Событие<> 3" ||
	        t == "Врата" || t == "Gate1" || t == "Врата";
}

struct ThisbacksDlg : WithThisbacksLayout<ТопОкно> {
	struct CbInfo {
		Ткст scope;
		Ткст тип;
		Ткст имя;
	};

	Индекс<Ткст> nname;
	Массив<CbInfo> cb;

	void GatherCallbacks(const Ткст& pfx, Индекс<Ткст>& done, const Ткст& scope, int access);
	void CbEdit(Один<Ктрл>& ctrl);
	void Generate(Ткст& ins, Ткст& clip, const Ткст& scope);

	typedef ThisbacksDlg ИМЯ_КЛАССА;

	ThisbacksDlg(const Ткст& scope);
};

int FilterId(int c)
{
	return iscid(c) ? c : 0;
}

void ThisbacksDlg::CbEdit(Один<Ктрл>& ctrl)
{
	ctrl.создай<EditString>().устФильтр(FilterId);
}

ThisbacksDlg::ThisbacksDlg(const Ткст& scope)
{
	CodeBaseLock __;
	CtrlLayoutOKCancel(*this, "События");
	list.добавьКолонку("Определено в");
	list.добавьКолонку("Тип");
	list.добавьКолонку("Событие<> ");
	list.добавьКолонку("Вставить").Ctrls<Опция>();
	list.добавьКолонку("Имя метода").Ctrls(THISBACK(CbEdit));
	list.SetLineCy(EditField::GetStdHeight());
	list.ColumnWidths("110 245 157 41 129");
	list.EvenRowColor();
	list.NoCursor();
	Sizeable().Zoomable();
	int q = CodeBase().найди(scope);
	if(q < 0)
		return;
	const Массив<CppItem>& n = CodeBase()[q];
	for(int i = 0; i < n.дайСчёт(); i++)
		nname.найдиДобавь(n[i].имя);
	Индекс<Ткст> done;
	GatherCallbacks("", done, scope, PRIVATE);
}

void ThisbacksDlg::GatherCallbacks(const Ткст& pfx, Индекс<Ткст>& done,
                                   const Ткст& scope, int access)
{
	CodeBaseLock __;
	if(пусто_ли(scope))
		return;
	Ткст h = pfx + scope;
	if(done.найди(h) >= 0 || list.дайСчёт() > 300)
		return;
	done.добавь(h);
	Вектор<Ткст> tparam;
	int q = CodeBase().найди(ParseTemplatedType(scope, tparam));
	if(q < 0)
		return;
	const Массив<CppItem>& n = CodeBase()[q];
	for(int i = 0; i < n.дайСчёт(); i++) {
		const CppItem& m = n[i];
		if(m.IsData() && IsCb(m.qtype) && m.access <= access) {
			Ткст n = m.имя;
			Ткст имя = pfx + '.' + n;
			if(*имя == '.')
				имя = имя.середина(1);
			if(имя == "WhenAction")
				n = "Action";
			else {
				Вектор<Ткст> ss = разбей(имя, '.');
				for(int i = 0; i < ss.дайСчёт(); i++) {
					Ткст& s = ss[i];
					if(s == "WhenAction")
						s.очисть();
					else {
						if(memcmp(s, "When", 4) == 0)
							s = s.середина(4);
						if(s.дайСчёт())
							s.уст(0, взаг(s[0]));
					}
				}
				n = Join(ss, "");
				while(nname.найди(n) >= 0)
					n = "On" + n;
			}
			list.добавь(scope, m.тип, имя, 0, n, m.имя);
		}
	}
	for(int i = 0; i < n.дайСчёт(); i++) {
		const CppItem& m = n[i];
		if(m.IsType() && m.access <= access) {
			Вектор<Ткст> b = разбей(m.qptype, ';');
			ResolveTParam(b, tparam);
			for(int i = 0; i < b.дайСчёт(); i++)
				GatherCallbacks(pfx, done, b[i], min(access, (int)PROTECTED));
		}
	}
	for(int i = 0; i < n.дайСчёт(); i++) {
		const CppItem& m = n[i];
		if(m.kind == INSTANCEVARIABLE && !IsCb(m.qtype)
		   && m.natural.найди('&') < 0 && m.natural.найди('*') < 0
		   && m.access <= access
		   && !m.имя.начинаетсяС("dv___"))
			GatherCallbacks(pfx + "." + m.имя, done, m.qtype, min(access, (int)PUBLIC));
	}
}

void ThisbacksDlg::Generate(Ткст& ins, Ткст& clip, const Ткст& scope)
{
	Ткст ac;
	for(int i = 0; i < list.дайСчёт(); i++) {
		if((bool)list.дай(i, 3)) {
			Ткст тип = list.дай(i, 1);
			Ткст имя = list.дай(i, 2);
			Ткст method = list.дай(i, 4);
			int q = имя.найди('.');
			if((Ткст)list.дай(i, 5) == "WhenAction" && q >= 0)
				ins << имя.середина(0, q) << " <<= THISFN(" << method;
			else
				ins << имя << " = THISFN(" << method;
			ins << ");\n";
			if(nname.найди(method) < 0) {
				Ткст param;
				q = тип.найди('<');
				if(q >= 0) {
					int qq = тип.найдирек('>');
					if(q < qq) {
						param = Ткст(~тип + q + 1, ~тип + qq);
						тип.обрежь(q);
					}
				}
				Ткст n = scope;
				if(n[0] == ':' && n[1] == ':')
					n = n.середина(2);
				Ткст t = (memcmp(тип, "Врата", 4) == 0 ? "bool " : "void ");
				clip << t << n << "::" << method << "(" << param << ")\n{\n}\n\n";
				ac << '\t' << t << method << "(" << param << ");\n";
			}
		}
	}
	clip << ac;
}

ИНИЦБЛОК
{
	региструйГлобКонфиг("ThisbacksDlg");
}

void AssistEditor::Thisbacks()
{
	ParserContext ctx;
	Контекст(ctx, дайКурсор32());
	if(пусто_ли(ctx.current_scope) || !ctx.IsInBody())
		return;
	ThisbacksDlg dlg(ctx.current_scope);
	грузиИзГлоба(dlg, "ThisbacksDlg");
	int c = dlg.пуск();
	сохраниВГлоб(dlg, "ThisbacksDlg");
	if(c != IDOK)
		return;
	Ткст a, b;
	dlg.Generate(a, b, ctx.current_scope);
	Paste(a.вШТкст());
	WriteClipboardText(b);
}
