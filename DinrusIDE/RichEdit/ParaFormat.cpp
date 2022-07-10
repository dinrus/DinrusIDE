#include "RichEdit.h"

namespace РНЦП {

struct ParaFormatDlg : public WithParaFormatLayout<ТопОкно> {
	ParaFormatting para;

	ParaFormatDlg() {
		CtrlLayoutOKCancel(*this, t_("Paragraph формат"));
		ActiveFocus(para.before);
	}
};

void RichEdit::ParaFormat()
{
	ParaFormatDlg d;
	d.para.уст(unit, formatinfo, !выделение_ли() && cursorp.level == 0);
	if(d.выполни() != IDOK || !d.para.IsChanged())
		return;
	dword v = d.para.дай(formatinfo);
	if(v) ApplyFormat(0, v);
}

struct sCompareLess {
	bool operator()(const Значение& a, const Значение& b) const {
		return CompareNoCase(Ткст(a), Ткст(b)) < 0;
	}
};

void RichEdit::ReadStyles()
{
	int i;
	style.очисть();
	Вектор<Uuid> id;
	Вектор<Ткст> имя;
	for(i = 0; i < text.GetStyleCount(); i++) {
		id.добавь(text.GetStyleId(i));
		имя.добавь(text.дайСтиль(i).имя);
	}
	IndexSort(имя, id, sCompareLess());
	for(i = 0; i < id.дайСчёт(); i++)
		style.добавь(id[i], имя[i]);
}

int RichEdit::CompareStyle(const Значение& a, const Значение& b)
{
	return CompareNoCase(Ткст(a), Ткст(b));
}

void RichEdit::устСтиль()
{
	if(!выделение_ли()) {
		NextUndo();
		WithSetStyleLayout<ТопОкно> d;
		CtrlLayoutOKCancel(d, t_("уст style"));
		d.newstyle <<= d.Breaker(IDYES);
		d.style.добавьКлюч();
		d.style.добавьКолонку();
		d.style.NoHeader().NoGrid();
		for(int i = 0; i < text.GetStyleCount(); i++)
			d.style.добавь(text.GetStyleId(i), text.дайСтиль(i).имя);
		d.style.сортируй(1, CompareStyle);
		int q = d.style.найди(RichStyle::GetDefaultId());
		if(q >= 0)
			d.style.устДисплей(q, 0, Single<DisplayDefault>());
		d.style.FindSetCursor(formatinfo.styleid);
		RichStyle cs;
		cs.формат = formatinfo;
		cs.формат.sscript = 0;
		cs.формат.link.очисть();
		cs.формат.indexentry.очисть();
		cs.формат.language = LNG_ENGLISH;
		cs.формат.label.очисть();

		Uuid id;
		switch(d.пуск()) {
		case IDCANCEL:
			return;
		case IDOK:
			if(d.style.курсор_ли()) {
				id = d.style.дайКлюч();
				const RichStyle& st = text.дайСтиль(id);
				cs.имя = st.имя;
				cs.next = st.next;
				SaveStyleUndo(id);
				break;
			}
			return;
		case IDYES:
			Ткст newname;
			if(редактируйТекст(newname, фмт(t_("нов style no. %d"), text.GetStyleCount()),
			            "Имя", CharFilterAscii128)) {
				cs.имя = newname;
				id = Uuid::создай();
				cs.next = id;
				SaveStylesUndo();
				break;
			}
			return;
		}
		text.устСтиль(id, cs);
		ReadStyles();
		formatinfo.styleid = id;
		SaveFormat(дайКурсор(), 0);
		text.ReStyle(дайКурсор(), id);
		финиш();
	}
}

void RichEdit::Styles()
{
	NextUndo();
	StyleManager s;
	s.настрой(ffs, unit);
	s.уст(text, formatinfo.styleid);
	if(s.выполни() != IDOK || !s.IsChanged())
		return;
	SaveStylesUndo();
	SetModify();
	s.дай(text);
	ReadStyles();
	финиш();
}

void RichEdit::ApplyStylesheet(const RichText& r)
{
	NextUndo();
	SaveStylesUndo();
	text.OverrideStyles(r.GetStyles(), false, false);
	ReadStyles();
	финиш();
}

}
