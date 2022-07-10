#include "RichEdit.h"

namespace РНЦП {

class StyleKeysDlg : public WithStyleKeysLayout<ТопОкно> {
	typedef StyleKeysDlg ИМЯ_КЛАССА;
	
	Массив<СписокБроса>    style;
	Массив<СписокБроса>    face;
	Массив<СписокБроса>    height;
	Массив<ColorPusher> ink;
	Массив<ColorPusher> paper;

public:
	void иниц(const RichEdit& edit, RichEdit::StyleKey *ключ);
	void Retrieve(RichEdit::StyleKey *k);

	StyleKeysDlg();
};

StyleKeysDlg::StyleKeysDlg()
{
	CtrlLayoutOKCancel(*this, "Styling keys");
	
	list.добавьКолонку("Ключ");
	list.добавьКолонку("Paragraph style");
	list.добавьКолонку("Шрифт");
	list.добавьКолонку("устВысоту");
	list.добавьКолонку("Ink");
	list.добавьКолонку("Paper");
	list.SetLineCy(EditField::GetStdHeight() + 4);
	list.NoHorzGrid().EvenRowColor().NoCursor();
	list.ColumnWidths("117 160 160 75 90 90");
}
	
void StyleKeysDlg::иниц(const RichEdit& edit, RichEdit::StyleKey *ключ)
{
	const RichText& text = edit.text;
	for(int i = 0; i < 20; i++) {
		RichEdit::StyleKey& k = ключ[i];
		list.добавь((i >= 10 ? "Shift+Alt+" : "Alt+") + какТкст(i % 10));

		СписокБроса& st = style.по(i);
		st.добавь(Null);
		const RichStyles& ts = text.GetStyles();
		for(int j = 0; j < ts.дайСчёт(); j++) {
			st.добавь(ts.дайКлюч(j), ts[j].имя);
		}
		if(st.найдиКлюч(k.styleid) < 0)
			st.добавь(k.styleid, k.stylename);
		st <<= k.styleid;
		list.устКтрл(i, 1, st, false);
		
		СписокБроса& fc = face.по(i);
		fc.добавь(Null);
		for(int j = 0; j < edit.face.дайСчёт(); j++)
			fc.добавь(Шрифт::GetFaceName(edit.face.дайКлюч(j)));
		if(fc.найди(k.face) < 0)
			fc.добавь(k.face);
		fc <<= k.face;
		list.устКтрл(i, 2, fc, false);

		СписокБроса& hg = height.по(i);
		hg.добавь(Null);
		for(int j = 0; j < edit.height.дайСчёт(); j++)
			hg.добавь(edit.height.дай(j));
		if(hg.найди(k.face) < 0)
			hg.добавь(k.face);
		list.устКтрл(i, 3, hg, false);
		
		ColorPusher& n = ink.по(i);
		n.NullText("");
		n <<= k.ink;
		list.устКтрл(i, 4, n, false);
		
		ColorPusher& p = paper.по(i);
		p.NullText("");
		p <<= k.paper;
		list.устКтрл(i, 5, p, false);
	}
}

void StyleKeysDlg::Retrieve(RichEdit::StyleKey *ключ)
{
	for(int i = 0; i < 20; i++) {
		RichEdit::StyleKey& k = ключ[i];
		k.styleid = ~style[i];
		k.stylename = style[i].дайЗначение();
		k.face = ~face[i];
		k.height = ~height[i];
		k.ink = ~ink[i];
		k.paper = ~paper[i];
	}
}

void RichEdit::StyleKeys()
{
	StyleKeysDlg dlg;
	dlg.иниц(*this, stylekey);
	if(dlg.пуск() == IDOK)
		dlg.Retrieve(stylekey);
}

void RichEdit::ApplyStyleKey(int i)
{
	if(i < 0 || i >= 20)
		return;
	const StyleKey& k = stylekey[i];
	if(!пусто_ли(k.styleid)) {
		int q = style.найдиКлюч(k.styleid);
		if(q >= 0) {
			style.SetIndex(q);
			Стиль();
		}
	}
	if(!пусто_ли(k.face)) {
		int q = face.найдиЗначение(k.face);
		if(q >= 0) {
			face.SetIndex(q);
			SetFace();
		}
	}
	if(!пусто_ли(k.height)) {
		height <<= k.height;
		SetHeight();
	}
	if(!пусто_ли(k.ink)) {
		ink <<= k.ink;
		устЧернила();
	}
	if(!пусто_ли(k.paper)) {
		paper <<= k.paper;
		SetPaper();
	}
}

RichEdit::StyleKey::StyleKey()
{
	styleid = Null;
	height = Null;
	ink = Null;
	paper = Null;
}

}
