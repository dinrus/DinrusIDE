#include "DinrusIDE.h"

struct AssistItemInfo : CppItem {
	Ткст defined;
	Ткст overed;
	Ткст имя;
};

void GatherVirtuals(МассивМап<Ткст, AssistItemInfo>& элт, const Ткст& scope,
                    Индекс<Ткст>& done)
{
	CodeBaseLock __;
	if(пусто_ли(scope))
		return;
	if(done.найди(scope) >= 0)
		return;
	done.добавь(scope);
	Вектор<Ткст> tparam;
	int q = CodeBase().найди(ParseTemplatedType(scope, tparam));
	if(q < 0)
		return;
	const Массив<CppItem>& m = CodeBase()[q];
	for(int i = 0; i < m.дайСчёт(); i++) {
		const CppItem& im = m[i];
		if(im.IsType()) {
			Вектор<Ткст> b = разбей(im.qptype, ';');
			ResolveTParam(b, tparam);
			for(int i = 0; i < b.дайСчёт(); i++)
				GatherVirtuals(элт, b[i], done);
		}
	}
	for(int i = 0; i < m.дайСчёт(); i++) {
		const CppItem& im = m[i];
		if(im.IsCode()) {
			Ткст k = im.qitem;
			if(im.IsCode()) {
				int q = элт.найди(k);
				if(q >= 0)
					элт[q].overed = scope;
				else
				if(im.virt) {
					AssistItemInfo& f = элт.дайДобавь(k);
					f.defined = f.overed = scope;
					f.имя = im.имя;
					(CppItem&)f = im;
				}
			}
		}
	}
}

struct VirtualsDlg : public WithVirtualsLayout<ТопОкно> {
	МассивМап<Ткст, AssistItemInfo> элт;

	virtual bool Ключ(dword ключ, int count)
	{
		if(ключ == K_CTRL_K) {
			find.устФокус();
			return true;
		}
		return горячаяКлав(ключ) || find.Ключ(ключ, count);
	}

	void синх() {
		Ткст имя = впроп((Ткст)~find);
		Ткст k = list.дайКлюч();
		list.очисть();
		for(int i = 0; i < элт.дайСчёт(); i++)
			if(впроп(элт[i].имя).найди(имя) >= 0) {
				CppItemInfo f;
				(CppItem&)f = элт[i];
				f.virt = false;
				f.имя = элт[i].имя;
				list.добавь(элт.дайКлюч(i), f.natural, RawToValue(f),
					элт[i].defined, элт[i].overed);
			}
		if(!list.FindSetCursor(k))
			list.идиВНач();
	}

	void DblClk() {
		if(list.курсор_ли()) {
			list.уст(5, 1);
			Break(IDOK);
		}
	}

	void сериализуй(Поток& s)
	{
		SerializePlacement(s);
		list.сериализуйЗаг(s);
		s % add_virtual % add_override;
	}

	typedef VirtualsDlg ИМЯ_КЛАССА;

	VirtualsDlg(const Ткст& scope) {
		Индекс<Ткст> done;
		GatherVirtuals(элт, scope, done);
		CtrlLayoutOKCancel(*this, "Виртуальные методы");
		list.добавьИндекс();
		list.добавьИндекс();
		list.добавьКолонку("Виртуальные функции").устДисплей(Single<CppItemInfoDisplay>());
		list.добавьКолонку("Определено в");
		list.добавьКолонку("Последний оверрайд");
		list.SetLineCy(Arial(Zy(11)).Info().дайВысоту() + DPI(3));
		list.ColumnWidths("500 100 100");
		list.WhenLeftDouble = THISBACK(DblClk);
		list.MultiSelect();
		list.EvenRowColor();
		Sizeable().Zoomable();
		find.NullText("Поиск (Ктрл+K)");
		find.устФильтр(SearchItemFilter);
		find <<= THISBACK(синх);
		add_override <<= true;
		add_virtual <<= false;
		синх();
	}
};

ИНИЦБЛОК
{
	региструйГлобКонфиг("VirtualsDlg");
}

void AssistEditor::Virtuals()
{
	ParserContext ctx;
	Контекст(ctx, дайКурсор32());
	if(пусто_ли(ctx.current_scope) || ctx.current_scope == "::" || ctx.IsInBody())
		return;
	VirtualsDlg dlg(ctx.current_scope);
	грузиИзГлоба(dlg, "VirtualsDlg");
	int c = dlg.пуск();
	сохраниВГлоб(dlg, "VirtualsDlg");
	if(c != IDOK)
		return;
	Ткст text;
	Ткст ctext;
	if(!dlg.list.выделение_ли() && dlg.list.курсор_ли())
		dlg.list.выдели(dlg.list.дайКурсор());
	for(int i = 0; i < dlg.list.дайСчёт(); i++)
		if(dlg.list.выделен(i)) {
			Ткст n = (Ткст)dlg.list.дай(i, 1);
			text << "\t";
			if(dlg.add_virtual)
				text << "virtual ";
			text << RemoveDefPar(n);
			if(dlg.add_override)
				text << " override";
			text << ";\r\n";
			ctext << MakeDefinition(ctx.current_scope, n) << "\n{\n}\n\n";
		}
	Paste(text.вШТкст());
	WriteClipboardText(ctext);
}
