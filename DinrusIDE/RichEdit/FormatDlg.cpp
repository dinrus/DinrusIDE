#include "RichEdit.h"

namespace РНЦП {

RichPara::NumberFormat ParaFormatting::GetNumbering()
{
	RichPara::NumberFormat f;
	f.before_number = ~before_number;
	f.after_number = ~after_number;
	f.reset_number = ~reset_number;
	for(int i = 0; i < 8; i++)
		f.number[i] = Nvl((int)~n[i]);
	return f;
}

bool ParaFormatting::IsNumbering()
{
	if(!пусто_ли(before_number) || !пусто_ли(after_number) || reset_number)
		return true;
	for(int i = 0; i < 8; i++)
		if(!пусто_ли(n[i]))
		   return true;
	return false;
}

void ParaFormatting::EnableNumbering()
{
	bool b = !пусто_ли(bullet) && !(int)~bullet;
	before_number.вкл(b);
	after_number.вкл(b);
	for(int i = 0; i < 8; i++)
		n[i].вкл(b);
}

int  ParaFormatting::ComputeIndent()
{
	if(!пусто_ли(bullet) && (int)~bullet)
		return 150;
	if(IsNumbering()) {
		RichPara::NumberFormat f = GetNumbering();
		RichPara::Number n;
		static byte n0[] = { 0, 37, 38, 48, 48, 37, 37 };
		static byte n1[] = { 0, 17, 18, 12, 12, 17, 17 };
		if(f.number[0] && f.number[0] < 8)
			n.n[0] = n0[f.number[0]];
		if(f.number[1] && f.number[1] < 8)
			n.n[1] = n1[f.number[1]];
		for(int i = 2; i < 8; i++) {
			static byte nn[] = { 0,  7,  8,  1,  1,  7,  7 };
			if(f.number[i] && f.number[i] < 8)
				n.n[i] = nn[f.number[i]];
		}
		Ткст s = n.AsText(f);
		s.конкат(' ');
		return дайРазмТекста(s, font).cx;
	}
	return 0;
}

void ParaFormatting::SetupIndent()
{
	EnableNumbering();
	if(indent.изменено() || keepindent) return;
	indent <<= ComputeIndent();
	indent.ClearModify();
}

void ParaFormatting::EditHdrFtr()
{
	if(EditRichHeaderFooter(header_qtf, footer_qtf))
		modified = true;
}

void ParaFormatting::NewHdrFtr()
{
	SyncHdrFtr();
	if(newhdrftr)
		EditHdrFtr();
}

void ParaFormatting::SyncHdrFtr()
{
	hdrftr.вкл(newhdrftr && newhdrftr.включен_ли());
}

void ParaFormatting::уст(int unit, const RichText::FormatInfo& formatinfo, bool baselevel)
{
	newhdrftr.вкл(baselevel);
	hdrftr.вкл(baselevel);
	font = formatinfo;
	ruler.уст(unit, RichText::RULER & formatinfo.paravalid ? formatinfo.ruler : Null);
	before.уст(unit, RichText::BEFORE & formatinfo.paravalid ? formatinfo.before : Null);
	lm.уст(unit, RichText::LM & formatinfo.paravalid ? formatinfo.lm : Null);
	indent.уст(unit, RichText::INDENT & formatinfo.paravalid ? formatinfo.indent : Null);
	rm.уст(unit, RichText::RM & formatinfo.paravalid ? formatinfo.rm : Null);
	after.уст(unit, RichText::AFTER & formatinfo.paravalid ? formatinfo.after : Null);
	if(RichText::ALIGN & formatinfo.paravalid)
		align <<= formatinfo.align == ALIGN_LEFT    ? 0 :
		          formatinfo.align == ALIGN_CENTER  ? 1 :
		          formatinfo.align == ALIGN_RIGHT   ? 2 :
		                                            3;
	if(RichText::NEWHDRFTR & formatinfo.paravalid) {
		newhdrftr = formatinfo.newhdrftr;
		header_qtf = formatinfo.header_qtf;
		footer_qtf = formatinfo.footer_qtf;
	}
	else {
		newhdrftr = Null;
		newhdrftr.ThreeState();
	}
	if(RichText::FIRSTONPAGE & formatinfo.paravalid)
		firstonpage = formatinfo.firstonpage;
	else {
		firstonpage = Null;
		firstonpage.ThreeState();
	}
	if(RichText::NEWPAGE & formatinfo.paravalid)
		page = formatinfo.newpage;
	else {
		page = Null;
		page.ThreeState();
	}
	if(RichText::KEEP & formatinfo.paravalid)
		keep = formatinfo.keep;
	else {
		keep = Null;
		keep.ThreeState();
	}
	if(RichText::KEEPNEXT & formatinfo.paravalid)
		keepnext = formatinfo.keepnext;
	else {
		keepnext = Null;
		keepnext.ThreeState();
	}
	if(RichText::ORPHAN & formatinfo.paravalid)
		orphan = formatinfo.orphan;
	else {
		orphan = Null;
		orphan.ThreeState();
	}
	if(RichText::RULERINK & formatinfo.paravalid)
		rulerink <<= formatinfo.rulerink;
	else
		rulerink <<= Null;
	if(RichText::RULERSTYLE & formatinfo.paravalid)
		rulerstyle <<= formatinfo.rulerstyle;
	else
		rulerstyle <<= Null;
	tabpos.SetUnit(unit);
	if(RichText::BULLET & formatinfo.paravalid)
		bullet <<= formatinfo.bullet;
	else
		bullet <<= Null;
	if(RichText::SPACING & formatinfo.paravalid)
		linespacing <<= formatinfo.linespacing;
	else
		linespacing <<= Null;
	if(RichText::NUMBERING & formatinfo.paravalid) {
		before_number <<= formatinfo.before_number.вШТкст();
		after_number <<= formatinfo.after_number.вШТкст();
		reset_number <<= formatinfo.reset_number;
		for(int i = 0; i < 8; i++)
			n[i] = formatinfo.number[i];
	}
	else {
		before_number <<= Null;
		after_number <<= Null;
		reset_number <<= Null;
		reset_number.ThreeState();
		for(int i = 0; i < 8; i++)
			n[i] = Null;
	}
	tabs.очисть();
	if(RichText::TABS & formatinfo.paravalid)
		for(int i = 0; i < formatinfo.tab.дайСчёт(); i++)
			tabs.добавь(formatinfo.tab[i].pos, formatinfo.tab[i].align, formatinfo.tab[i].fillchar);
	tabsize.уст(unit, RichText::TABSIZE & formatinfo.paravalid ? formatinfo.tabsize : Null);
	keepindent = formatinfo.indent != ComputeIndent();
	SetupIndent();
	ClearModify();
	SyncHdrFtr();
	modified = false;
}

dword ParaFormatting::дай(RichText::FormatInfo& formatinfo)
{
	dword v = 0;
	if(!пусто_ли(before)) {
		formatinfo.before = ~before;
		v |= RichText::BEFORE;
	}
	if(!пусто_ли(lm)) {
		formatinfo.lm = ~lm;
		v |= RichText::LM;
	}
	if(!пусто_ли(indent)) {
		formatinfo.indent = ~indent;
		v |= RichText::INDENT;
	}
	if(!пусто_ли(rm)) {
		formatinfo.rm = ~rm;
		v |= RichText::RM;
	}
	if(!пусто_ли(after)) {
		formatinfo.after = ~after;
		v |= RichText::AFTER;
	}
	if(!пусто_ли(align)) {
		static int sw[] = { ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT, ALIGN_JUSTIFY };
		formatinfo.align = sw[(int)~align];
		v |= RichText::ALIGN;
	}
	if(!пусто_ли(page)) {
		formatinfo.newpage = page;
		v |= RichText::NEWPAGE;
	}
	if(!пусто_ли(firstonpage)) {
		formatinfo.firstonpage = firstonpage;
		v |= RichText::FIRSTONPAGE;
	}
	if(!пусто_ли(newhdrftr)) {
		formatinfo.newhdrftr = newhdrftr;
		v |= RichText::NEWHDRFTR;
		if(formatinfo.newhdrftr) {
			formatinfo.header_qtf = header_qtf;
			formatinfo.footer_qtf = footer_qtf;
		}
		else
			formatinfo.header_qtf = formatinfo.footer_qtf = Null;
	}
	if(!пусто_ли(keep)) {
		formatinfo.keep = keep;
		v |= RichText::KEEP;
	}
	if(!пусто_ли(keepnext)) {
		formatinfo.keepnext = keepnext;
		v |= RichText::KEEPNEXT;
	}
	if(!пусто_ли(orphan)) {
		formatinfo.orphan = orphan;
		v |= RichText::ORPHAN;
	}
	if(!пусто_ли(bullet)) {
		formatinfo.bullet = ~bullet;
		v |= RichText::BULLET;
	}
	if(!пусто_ли(linespacing)) {
		formatinfo.linespacing = ~linespacing;
		v |= RichText::SPACING;
	}
	if(IsNumbering()) {
		(RichPara::NumberFormat&)formatinfo = GetNumbering();
		v |= RichText::NUMBERING;
	}
	if((RichText::TABS & formatinfo.paravalid) || tabs.дайСчёт()) {
		formatinfo.tab.очисть();
		for(int i = 0; i < tabs.дайСчёт(); i++) {
			RichPara::Вкладка tab;
			tab.pos = tabs.дай(i, 0);
			tab.align = (int)tabs.дай(i, 1);
			tab.fillchar = (int)tabs.дай(i, 2);
			formatinfo.tab.добавь(tab);
		}
		v |= RichText::TABS;
	}
	if(!пусто_ли(tabsize)) {
		formatinfo.tabsize = ~tabsize;
		v |= RichText::TABSIZE;
	}
	if(!пусто_ли(ruler)) {
		formatinfo.ruler = ~ruler;
		v |= RichText::RULER;
	}
	if(!пусто_ли(rulerink)) {
		formatinfo.rulerink = ~rulerink;
		v |= RichText::RULERINK;
	}
	if(!пусто_ли(rulerstyle)) {
		formatinfo.rulerstyle = ~rulerstyle;
		v |= RichText::RULERSTYLE;
	}
	return v;
}

struct RulerStyleDisplay : Дисплей {
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const
	{
		w.DrawRect(r, paper);
		if(!пусто_ли(q))
			RichPara::DrawRuler(w, r.left, (r.top + r.bottom) / 2 - 1, r.устШирину(), 2, ink, q);
	}
};

ParaFormatting::ParaFormatting()
{
	CtrlLayout(*this);
	tabtype.добавь(ALIGN_LEFT, t_("Слева"));
	tabtype.добавь(ALIGN_RIGHT, t_("Справа"));
	tabtype.добавь(ALIGN_CENTER, t_("По центру"));
	tabfill.добавь(0, t_("Отсутствует"));
	tabfill.добавь(1, t_("...."));
	tabfill.добавь(2, t_("----"));
	tabfill.добавь(3, t_("__"));
	tabs.добавьКолонку(t_("Положение вкладки"), 2).Edit(tabpos).SetConvert(tabpos);
	tabs.добавьКолонку(t_("Тип"), 2).Edit(tabtype).SetConvert(tabtype).вставьЗнач(ALIGN_LEFT);
	tabs.добавьКолонку(t_("Заполнение"), 1).Edit(tabfill).SetConvert(tabfill).вставьЗнач(0);
	tabs.ColumnWidths("103 89 78");
	tabs.Appending().Removing().NoAskRemove();
	tabs.WhenAcceptEdit = tabs.WhenArrayAction = THISBACK(SetMod);
	linespacing.добавь(RichPara::LSP10, "1.0");
	linespacing.добавь(RichPara::LSP115, "1.15");
	linespacing.добавь(RichPara::LSP15, "1.5");
	linespacing.добавь(RichPara::LSP20, "2.0");
	bullet.добавь(RichPara::BULLET_NONE, RichEditImg::NoneBullet());
	bullet.добавь(RichPara::BULLET_ROUND, RichEditImg::RoundBullet());
	bullet.добавь(RichPara::BULLET_ROUNDWHITE, RichEditImg::RoundWhiteBullet());
	bullet.добавь(RichPara::BULLET_BOX, RichEditImg::BoxBullet());
	bullet.добавь(RichPara::BULLET_BOXWHITE, RichEditImg::BoxWhiteBullet());
	bullet.добавь(RichPara::BULLET_TEXT, RichEditImg::TextBullet());
	bullet.устДисплей(CenteredHighlightImageDisplay());
	bullet.SetLineCy(RichEditImg::RoundBullet().дайВысоту() + Zy(2));
	for(int i = 0; i < 8; i++) {
		СписокБроса& list = n[i];
		list.добавь(Null);
		list.добавь(RichPara::NUMBER_NONE, " - ");
		list.добавь(RichPara::NUMBER_1, "1, 2, 3");
		list.добавь(RichPara::NUMBER_0, "0, 1, 2");
		list.добавь(RichPara::NUMBER_a, "a, b, c");
		list.добавь(RichPara::NUMBER_A, "A, B, C");
		list.добавь(RichPara::NUMBER_i, "i, ii, iii");
		list.добавь(RichPara::NUMBER_I, "I, II, III");
		list <<= THISBACK(SetupIndent);
	}
	before_number <<=
	after_number <<=
	reset_number <<=
	bullet <<= THISBACK(SetupIndent);
	
	newhdrftr <<= THISBACK(NewHdrFtr);
	hdrftr <<= THISBACK(EditHdrFtr);
	SyncHdrFtr();
	
	EnableNumbering();
	rulerink.NullText("---");
	rulerstyle.устДисплей(Single<RulerStyleDisplay>());
	rulerstyle.добавь(Null);
	rulerstyle.добавь(RichPara::RULER_SOLID);
	rulerstyle.добавь(RichPara::RULER_DOT);
	rulerstyle.добавь(RichPara::RULER_DASH);
}

void StyleManager::EnterStyle()
{
	RichText::FormatInfo f;
	const RichStyle& s = style.дай(list.дайКлюч());
	f.уст(s.формат);
	para.уст(unit, f);
	height <<= RichEdit::DotToPt(s.формат.дайВысоту());
	face <<= s.формат.GetFace();
	bold = s.формат.IsBold();
	italic = s.формат.IsItalic();
	underline = s.формат.IsUnderline();
	strikeout = s.формат.IsStrikeout();
	capitals = s.формат.capitals;
	ink <<= s.формат.ink;
	paper <<= s.формат.paper;
	next <<= s.next;
	ClearModify();
	SetupFont0();
	para.EnableNumbering();
}

void StyleManager::дайШрифт(Шрифт& font)
{
	if(!пусто_ли(face))
		font.Face(~face);
	if(!пусто_ли(height))
		font.устВысоту(RichEdit::PtToDot(~height));
	font.Bold(bold);
	font.Italic(italic);
	font.Underline(underline);
	font.Strikeout(strikeout);
}

void StyleManager::SetupFont0()
{
	Шрифт font = Arial(120);
	дайШрифт(font);
	para.устШрифт(font);
}

void StyleManager::SetupFont()
{
	SetupFont0();
	para.SetupIndent();
}

void StyleManager::SaveStyle()
{
	if(list.курсор_ли()) {
		Uuid id = list.дайКлюч();
		RichStyle& s = style.дай(list.дайКлюч());
		if(Ктрл::IsModifiedDeep() || para.IsChanged()) {
			dirty.найдиДобавь(id);
			RichText::FormatInfo f;
			para.дай(f);
			s.формат = f;
			дайШрифт(s.формат);
			s.формат.capitals = capitals;
			s.формат.ink = ~ink;
			s.формат.paper = ~paper;
		}
		if(Ткст(list.дай(1)) != s.имя) {
			dirty.найдиДобавь(id);
			s.имя = list.дай(1);
		}
		if((Uuid)~next != s.next) {
			dirty.найдиДобавь(id);
			s.next = ~next;
		}
	}
}

void StyleManager::создай()
{
	Uuid id = Uuid::создай();
	style.добавь(id, style.дай(list.дайКлюч()));
	style.верх().next = id;
	dirty.найдиДобавь(id);
	list.добавь(id);
	list.идиВКон();
	list.StartEdit();
}

void StyleManager::удали()
{
	if(list.дайСчёт() > 1 && (Uuid)list.дайКлюч() != RichStyle::GetDefaultId()) {
		dirty.найдиДобавь(list.дайКлюч());
		style.удали(list.дайКурсор());
		list.удали(list.дайКурсор());
	}
}

void StyleManager::Menu(Бар& bar)
{
	bar.добавь(t_("Создать новый стиль.."), THISBACK(создай))
	   .Ключ(K_INSERT);
	bar.добавь(t_("Удалить стиль"), THISBACK(удали))
	   .Ключ(K_DELETE);
	bar.добавь(t_("Переименовать.."), callback(&list, &КтрлМассив::DoEdit))
	   .Ключ(K_CTRL_ENTER);
}

void StyleManager::ReloadNextStyles()
{
	next.очистьСписок();
	for(int i = 0; i < list.дайСчёт(); i++)
		next.добавь(list.дай(i, 0), list.дай(i, 1));
}

void RichEdit::DisplayDefault::рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const
{
	Ткст text = q;
	w.DrawRect(r, paper);
	DrawSmartText(w, r.left, r.top, r.устШирину(), text, StdFont().Bold(), ink);
}

void StyleManager::уст(const RichText& text, const Uuid& current)
{
	list.очисть();
	int i;
	for(i = 0; i < text.GetStyleCount(); i++)
		list.добавь(text.GetStyleId(i), text.дайСтиль(i).имя);
	list.сортируй(1, RichEdit::CompareStyle);
	for(i = 0; i < text.GetStyleCount(); i++) {
		Uuid id = list.дай(i, 0);
		style.добавь(id, text.дайСтиль(id));
	}
	int q = list.найди(RichStyle::GetDefaultId());
	if(q >= 0)
		list.устДисплей(q, 0, Single<RichEdit::DisplayDefault>());
	ReloadNextStyles();
	list.FindSetCursor(current);
}

void StyleManager::уст(const char *qtf)
{
	RichText txt = ParseQTF(qtf);
	уст(txt);
}

bool StyleManager::IsChanged() const
{
	return dirty.дайСчёт() || IsModifiedDeep();
}

void StyleManager::дай(RichText& text)
{
	SaveStyle();
	for(int i = 0; i < dirty.дайСчёт(); i++) {
		Uuid id = dirty[i];
		int q = style.найди(id);
		if(q >= 0)
			text.устСтиль(id, style.дай(id));
		else
			text.RemoveStyle(id);
	}
}

RichText StyleManager::дай()
{
	RichText output;
	output.SetStyles(style);
	return output;
}

Ткст StyleManager::GetQTF()
{
	return AsQTF(дай());
}

void StyleManager::настрой(const Вектор<int>& faces, int aunit)
{
	unit = aunit;
	height.очисть();
	for(int i = 0; RichEdit::fh[i]; i++)
		height.добавьСписок(RichEdit::fh[i]);
	face.очистьСписок();
	SetupFaceList(face);
	for(int i = 0; i < faces.дайСчёт(); i++)
		face.добавь(faces[i]);
}

StyleManager::StyleManager()
{
	CtrlLayoutOKCancel(*this, t_("Стили"));
	list.NoHeader().NoGrid();
	list.добавьКлюч();
	list.добавьКолонку().Edit(имя);
	list.WhenEnterRow = THISBACK(EnterStyle);
	list.WhenKillCursor = THISBACK(SaveStyle);
	list.WhenBar = THISBACK(Menu);
	list.WhenAcceptEdit = THISBACK(ReloadNextStyles);
	ink.неПусто();
	face <<= height <<= italic <<= bold <<= underline <<= strikeout <<= THISBACK(SetupFont);
	Вектор<int> ffs;
	Вектор<int> ff;
	ff.добавь(Шрифт::ARIAL);
	ff.добавь(Шрифт::ROMAN);
	ff.добавь(Шрифт::COURIER);
	настрой(ff, UNIT_DOT);
	para.page.откл();
}

}
