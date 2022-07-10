#include "RichEdit.h"

namespace РНЦП {

Точка RichEdit::GetPreedit()
{
	Прям r = GetCaretRect();
	if(formatinfo.sscript == 2) {
		Точка p = r.низПраво();
		p.y -= 3 * r.дайВысоту() / 5;
		return p;
	}
	return r.верхПраво();
}
	
Шрифт RichEdit::GetPreeditFont()
{
	Шрифт fnt = formatinfo;
	int h = абс(fnt.дайВысоту());
	if(formatinfo.sscript)
		h = 3 * h / 5;
	return fnt(max(GetZoom() * абс(h), 1));
}

void RichEdit::ApplyFormat(dword charvalid, dword paravalid)
{
	if(толькочтен_ли())
		return;
	RichText::FormatInfo f = formatinfo;
	f.charvalid = charvalid;
	f.paravalid = paravalid;
	if(objectpos >= 0) {
		ModifyFormat(objectpos, f, 1);
		финиш();
	}
	if(выделение_ли()) {
		if(tablesel) {
			NextUndo();
			SaveTable(tablesel);
			text.ApplyTableFormatInfo(tablesel, cells, f);
		}
		else {
			int l = min(cursor, anchor);
			int h = max(cursor, anchor);
			RichPos rp = text.GetRichPos(h);
			if(rp.posinpara == 0 && h > l) {
				RichPos rp1 = text.GetRichPos(h - 1);
				if(InSameTxt(rp, rp1))
					h--;
			}
			ModifyFormat(l, f, h - l);
		}
		финиш();
	}
	else
	if(cursorp.paralen == 0) {
		ModifyFormat(cursor, f, 0);
		финиш();
	}
	else
	if(f.paravalid) {
		ModifyFormat(cursor, f, 0);
		финиш();
	}
	else
		RefreshBar();
}

void RichEdit::ApplyFormatInfo(const RichText::FormatInfo& fi)
{
	fi.ApplyTo(formatinfo);
	formatinfo.charvalid |= fi.charvalid;
	formatinfo.paravalid |= fi.paravalid;
	ApplyFormat(fi.charvalid, fi.paravalid);
}

void RichEdit::Bold()
{
	NextUndo();
	formatinfo.Bold(!(formatinfo.IsBold() && (formatinfo.charvalid & RichText::BOLD)));
	ApplyFormat(RichText::BOLD);
}

void RichEdit::Italic()
{
	NextUndo();
	formatinfo.Italic(!(formatinfo.IsItalic() && (formatinfo.charvalid & RichText::ITALIC)));
	ApplyFormat(RichText::ITALIC);
}

void RichEdit::Underline()
{
	NextUndo();
	formatinfo.Underline(!(formatinfo.IsUnderline() && (formatinfo.charvalid & RichText::UNDERLINE)));
	ApplyFormat(RichText::UNDERLINE);
}

void RichEdit::Strikeout()
{
	NextUndo();
	formatinfo.Strikeout(!(formatinfo.IsStrikeout() && (formatinfo.charvalid & RichText::STRIKEOUT)));
	ApplyFormat(RichText::STRIKEOUT);
}

void RichEdit::Capitals()
{
	NextUndo();
	formatinfo.capitals = !formatinfo.capitals && (formatinfo.charvalid & RichText::CAPITALS);
	ApplyFormat(RichText::CAPITALS);
}

void RichEdit::SetScript(int i)
{
	NextUndo();
	formatinfo.sscript = i;
	ApplyFormat(RichText::SSCRIPT);
}

void RichEdit::SetFace()
{
	NextUndo();
	formatinfo.Face(~face);
	ApplyFormat(RichText::FACE);
	устФокус();
}

void RichEdit::SetHeight()
{
	NextUndo();
	formatinfo.устВысоту(PtToDot(~height));
	ApplyFormat(RichText::HEIGHT);
	устФокус();
}

void RichEdit::устЧернила()
{
	NextUndo();
	formatinfo.ink = ~ink;
	ApplyFormat(RichText::INK);
	устФокус();
}

void RichEdit::SetPaper()
{
	NextUndo();
	formatinfo.paper = ~paper;
	ApplyFormat(RichText::PAPER);
	устФокус();
}

void RichEdit::SetLanguage()
{
	NextUndo();
	formatinfo.language = (int)~language;
	ApplyFormat(RichText::LANGUAGE);
	устФокус();
}

void RichEdit::Language()
{
	WithRichLanguageLayout<ТопОкно> d;
	CtrlLayoutOKCancel(d, t_("Language"));
	d.lang <<= ~language;
	if(d.пуск() != IDOK)
		return;
	formatinfo.language = (int)~d.lang;
	ApplyFormat(RichText::LANGUAGE);
	устФокус();
	if(!language.HasKey((int)~d.lang)) {
		Вектор<int> h;
		for(int i = 0; i < language.дайСчёт(); i++)
			h.добавь(language.дайКлюч(i));
		h.добавь(~d.lang);
		SetupLanguage(pick(h));
	}
}

void RichEdit::IndentMark()
{
	RichRuler::Marker m;
	int l = formatinfo.lm;
	int r = cursorc.textpage.устШирину() - formatinfo.rm;
	m.pos = l + formatinfo.indent;
	m.minpos = max(l, 0);
	m.maxpos = max(r - 120, 0);
	m.top = true;
	m.image = formatinfo.paravalid & RichText::INDENT ? RichEditImg::Indent()
	                                                  : RichEditImg::IndentMixed();
	ruler.уст(2, m);
}

void RichEdit::ReadFormat()
{
	if(objectpos >= 0)
		formatinfo = text.GetFormatInfo(objectpos, 1);
	else
	if(выделение_ли())
		if(tablesel)
			formatinfo = text.GetTableFormatInfo(tablesel, cells);
		else
			formatinfo = text.GetFormatInfo(min(cursor, anchor), абс(cursor - anchor));
	else {
		RichPos p = cursorp;
		if(cursor && p.posinpara)
			p = text.GetRichPos(cursor - 1);
		formatinfo.уст(p.формат);
	}
	ShowFormat();
}

void RichEdit::ShowFormat()
{
	RefreshBar();

	if(formatinfo.charvalid & RichText::FACE)
		face <<= formatinfo.GetFace();
	else
		face <<= Null;

	if(formatinfo.charvalid & RichText::HEIGHT)
		height <<= DotToPt(formatinfo.дайВысоту());
	else
		height <<= Null;

	if(formatinfo.charvalid & RichText::LINK)
		hyperlink <<= formatinfo.link;
	else
		hyperlink <<= Null;

	if(formatinfo.charvalid & RichText::INDEXENTRY)
		indexentry <<= formatinfo.indexentry;
	else
		indexentry <<= Null;

	if(formatinfo.charvalid & RichText::INK)
		ink <<= formatinfo.ink;
	else
		ink <<= Null;

	if(formatinfo.charvalid & RichText::PAPER)
		paper <<= formatinfo.paper;
	else
		paper <<= Null;

	if(formatinfo.charvalid & RichText::LANG)
		language <<= (int)formatinfo.language;
	else
		language <<= Null;

	if(выделение_ли())
		label <<= Null;
	else
		label <<= formatinfo.label;

	int l = formatinfo.lm;
	int r = cursorc.textpage.устШирину() - formatinfo.rm;

	RichRuler::Marker m;
	m.pos = l;
	m.minpos = 0;
	m.maxpos = max(r - formatinfo.indent - 120, 0);
	m.image = formatinfo.paravalid & RichText::LM ? RichEditImg::Margin() : RichEditImg::MarginMixed();
	ruler.уст(0, m);

	m.pos = r;
	m.minpos = max(l + formatinfo.indent + 120, 0);
	m.maxpos = cursorc.textpage.устШирину();
	m.image = formatinfo.paravalid & RichText::RM ? RichEditImg::Margin() : RichEditImg::MarginMixed();
	ruler.уст(1, m);
	IndentMark();

	int maxpos = 0;
	m.minpos = 0;
	m.deletable = true;
	if(formatinfo.paravalid & RichText::TABS) {
		for(int i = 0; i < formatinfo.tab.дайСчёт(); i++) {
			RichPara::Вкладка tab = formatinfo.tab[i];
			m.pos = tab.pos;
			if(tab.pos > maxpos)
				maxpos = tab.pos;
			switch(tab.align) {
			case ALIGN_LEFT:
				m.image = RichEditImg::LeftTab();
				break;
			case ALIGN_RIGHT:
				m.image = RichEditImg::RightTab();
				break;
			case ALIGN_CENTER:
				m.image = RichEditImg::CenterTab();
				break;
			}
			ruler.уст(i + 3, m);
		}
		ruler.SetTabs(maxpos, formatinfo.tabsize);
		ruler.устСчёт(formatinfo.tab.дайСчёт() + 3);
	}
	else {
		ruler.SetTabs(INT_MAX / 2, 1);
		ruler.устСчёт(3);
	}

	if(formatinfo.paravalid & RichText::STYLE)
		style <<= formatinfo.styleid;
	else
		style <<= Null;
	setstyle->вкл(!выделение_ли());
}

void RichEdit::HighLightTab(int r)
{
	RichRuler::Marker m = ruler[r + 3];
	RichPara::Вкладка tab = formatinfo.tab[r];
	m.image = tab.align == ALIGN_RIGHT  ? RichEditImg::RightTabTrack() :
	          tab.align == ALIGN_CENTER ? RichEditImg::CenterTabTrack() :
	                                      RichEditImg::LeftTabTrack();
	ruler.уст(r + 3, m);
}

void RichEdit::Hyperlink()
{
	Ткст s = formatinfo.link;
	if(!выделение_ли() && !пусто_ли(s) && cursorp.формат.link == s && text[cursor] != '\n') {
		int l = cursor - 1;
		while(l >= 0 && text[l] != '\n' && text.GetRichPos(l).формат.link == s)
			l--;
		l++;
		int h = cursor;
		while(h < text.дайДлину() && text[h] != '\n' && text.GetRichPos(h).формат.link == s)
			h++;
		if(l < h)
			выдели(l, h - l);
	}
	ШТкст linktext;
	WhenHyperlink(s, linktext);
	if(s != formatinfo.link || linktext.дайДлину()) {
		formatinfo.link = s;
		hyperlink <<= s;
		NextUndo();
		ApplyFormat(RichText::LINK);
		if(linktext.дайДлину()) {
			удалиВыделение();
			RichPara p;
			p.формат = formatinfo;
			p.конкат(linktext, formatinfo);
			RichText txt;
			txt.SetStyles(text.GetStyles());
			txt.конкат(p);
			вставь(cursor, txt, true);
			Move(cursor + linktext.дайСчёт(), false);
		}
	}
	устФокус();
}

void RichEdit::Надпись()
{
	if(выделение_ли()) return;
	Ткст s = formatinfo.label;
	WhenLabel(s);
	if(s != formatinfo.label) {
		formatinfo.label = s;
		NextUndo();
		ApplyFormat(0, RichText::LABEL);
		устФокус();
	}
}

void RichEdit::IndexEntry()
{
	Ткст s = formatinfo.indexentry.вТкст();
	Ткст s0 = s;
	WhenIndexEntry(s);
	if(s != s0) {
		formatinfo.indexentry = s.вШТкст();
		ApplyFormat(RichText::INDEXENTRY);
		NextUndo();
		устФокус();
	}
}

void RichEdit::BeginRulerTrack()
{
	NextUndo();
	SaveFormat();
	int r = ruler.GetTrack();
	if(r < 0) return;
	RichRuler::Marker m = ruler[r];
	switch(r) {
	case 0:
	case 1:
		m.image = RichEditImg::MarginTrack();
		break;
	case 2:
		m.image = RichEditImg::IndentTrack();
		break;
	default:
		HighLightTab(r - 3);
		return;
	}
	ruler.уст(r, m);
}

void RichEdit::SetParaFormat(dword paravalid)
{
	RichText::FormatInfo f = formatinfo;
	f.charvalid = 0;
	f.paravalid = paravalid;
	if(выделение_ли())
		if(tablesel)
			text.ApplyTableFormatInfo(tablesel, cells, f);
		else
			text.ApplyFormatInfo(min(cursor, anchor), f, абс(cursor - anchor));
	else
		text.ApplyFormatInfo(cursor, f, 0);
}

void RichEdit::RulerTrack()
{
	int r = ruler.GetTrack();
	if(r < 0) return;
	RichRuler::Marker m = ruler[r];
	switch(r) {
	case 0:
		formatinfo.lm = m.pos;
		SetParaFormat(RichText::LM);
		IndentMark();
		break;
	case 1:
		formatinfo.rm = cursorc.textpage.устШирину() - m.pos;
		SetParaFormat(RichText::RM);
		break;
	case 2:
		formatinfo.indent = m.pos - formatinfo.lm;
		SetParaFormat(RichText::INDENT);
		break;
	default:
		formatinfo.tab[r - 3].pos = m.pos;
		SetParaFormat(RichText::TABS);
		int maxpos = 0;
		for(int i = 0; i < formatinfo.tab.дайСчёт(); i++) {
			RichPara::Вкладка tab = formatinfo.tab[i];
			if(tab.pos > maxpos)
				maxpos = tab.pos;
		}
		ruler.SetTabs(maxpos, formatinfo.tabsize);
		break;
	}
	FinishNF();
}

void RichEdit::TabAdd(int align)
{
	RichPara::Вкладка tab;
	tab.pos = ruler.дайПоз();
	tab.align = align;
	if(formatinfo.tab.дайСчёт() > 30000 || tab.pos < 0 || tab.pos >= cursorc.textpage.устШирину()) return;
	formatinfo.tab.добавь(tab);
	SetParaFormat(RichText::TABS);
	финиш();
}

void RichEdit::AddTab()
{
	NextUndo();
	SaveFormat();
	TabAdd(ruler.GetNewTabAlign());
}

void RichEdit::TabMenu()
{
	NextUndo();
	int r = ruler.GetTrack() - 3;
	if(r >= 0)
		HighLightTab(r);
	CallbackArgTarget<int> align;
	CallbackArgTarget<int> fill;
	БарМеню menu;
	menu.добавь(t_("лево"), RichEditImg::LeftTab(), align[ALIGN_LEFT]);
	menu.добавь(t_("право"), RichEditImg::RightTab(), align[ALIGN_RIGHT]);
	menu.добавь(t_("Center"), RichEditImg::CenterTab(), align[ALIGN_CENTER]);
	if(r >= 0) {
		int f = formatinfo.tab[r].fillchar;
		menu.Separator();
		menu.добавь(t_("No fill"), fill[0])
		    .Radio(f == 0);
		menu.добавь(t_("Fill with ...."), fill[1])
		    .Radio(f == 1);
		menu.добавь(t_("Fill with ----"), fill[2])
		    .Radio(f == 2);
		menu.добавь(t_("Fill with __"), fill[3])
		    .Radio(f == 3);
		menu.Separator();
		menu.добавь(t_("удали"), fill[-1]);
	}
	menu.выполни();
	if(!пусто_ли(align)) {
		SaveFormat();
		if(r >= 0) {
			formatinfo.tab[r].align = (int)align;
			SetParaFormat(RichText::TABS);
		}
		else
			TabAdd(align);
	}
	if(!пусто_ли(fill) && r >= 0) {
		SaveFormat();
		if(r >= 0) {
			if(fill == -1)
				formatinfo.tab[r].pos = Null;
			else
				formatinfo.tab[r].fillchar = (int)fill;
			SetParaFormat(RichText::TABS);
		}
	}
	финиш();
}

void RichEdit::AlignLeft()
{
	NextUndo();
	formatinfo.align = ALIGN_LEFT;
	ApplyFormat(0, RichText::ALIGN);
}

void RichEdit::AlignRight()
{
	NextUndo();
	formatinfo.align = ALIGN_RIGHT;
	ApplyFormat(0, RichText::ALIGN);
}

void RichEdit::AlignCenter()
{
	NextUndo();
	formatinfo.align = ALIGN_CENTER;
	ApplyFormat(0, RichText::ALIGN);
}

void RichEdit::AlignJustify()
{
	NextUndo();
	formatinfo.align = ALIGN_JUSTIFY;
	ApplyFormat(0, RichText::ALIGN);
}

void  RichEdit::SetBullet(int bullet)
{
	NextUndo();
	if((formatinfo.paravalid & RichText::BULLET) && formatinfo.bullet == bullet) {
		formatinfo.bullet = RichPara::BULLET_NONE;
		formatinfo.indent = formatinfo.paravalid & RichText::STYLE ?
		                       text.дайСтиль(formatinfo.styleid).формат.indent : 0;
	}
	else {
		formatinfo.bullet = bullet;
		formatinfo.indent = bullet_indent;
	}
	ApplyFormat(0, RichText::INDENT|RichText::BULLET);
}

void RichEdit::Стиль()
{
	NextUndo();
	SaveFormat(cursor, 0);
	formatinfo.уст(text.дайСтиль((Uuid)~style).формат);
	ApplyFormat(0, RichText::STYLE);
	устФокус();
	финиш();
}

void RichEdit::AdjustObjectSize()
{
	NextUndo();
	RichObject obj = cursorp.object;
	if(!obj) return;
	WithObjectSizeLayout<ТопОкно> d;
	CtrlLayoutOKCancel(d, t_("Object position"));
	Размер sz = obj.дайРазм();
	Размер psz = GetPhysicalSize(obj);
	if(psz.cx == 0) psz.cx = 2000;
	if(psz.cy == 0) psz.cy = 2000;
	d.width.уст(unit, sz.cx);
	d.height.уст(unit, sz.cy);
	d.widthp.SetInc(5).образец("%.1f");
	d.widthp <<= 100.0 * sz.cx / psz.cx;
	d.heightp.SetInc(5).образец("%.1f");
	d.heightp <<= 100.0 * sz.cy / psz.cy;
	d.keepratio = obj.IsKeepRatio();
	d.width <<= d.height <<= d.widthp <<= d.heightp <<= d.Breaker(IDYES);
	d.ydelta.WithSgn().уст(unit, obj.GetYDelta());
	d.keepratio <<= d.Breaker(IDNO);
	for(;;) {
		switch(d.пуск()) {
		case IDCANCEL:
			return;
		case IDYES:
			if(d.width.естьФокус() && !пусто_ли(d.width)) {
				d.widthp <<= 100 * (double)~d.width / psz.cx;
				if(d.keepratio) {
					d.height <<= psz.cy * (double)~d.width / psz.cx;
					d.heightp <<= ~d.widthp;
				}
			}
			if(d.height.естьФокус() && !пусто_ли(d.height)) {
				d.heightp <<= 100 * (double)~d.height / psz.cy;
				if(d.keepratio) {
					d.width <<= psz.cx * (double)~d.height / psz.cy;
					d.widthp <<= ~d.heightp;
				}
			}
			if(d.widthp.естьФокус() && !пусто_ли(d.widthp)) {
				d.width <<= psz.cx * (double)~d.widthp / 100;
				if(d.keepratio) {
					d.height <<= psz.cy * (double)~d.width / psz.cx;
					d.heightp <<= ~d.widthp;
				}
			}
			if(d.heightp.естьФокус() && !пусто_ли(d.heightp)) {
				d.height <<= psz.cy * (double)~d.heightp / 100;
				if(d.keepratio) {
					d.width <<= psz.cx * (double)~d.height / psz.cy;
					d.widthp <<= ~d.heightp;
				}
			}
			break;
		case IDNO:
			if(d.keepratio && !пусто_ли(d.width)) {
				d.widthp <<= 100 * (double)~d.width / psz.cx;
				if(d.keepratio) {
					d.height  <<= psz.cy * (double)~d.width / psz.cx;
					d.heightp <<= ~d.widthp;
				}
			}
			break;
		case IDOK:
			if(!пусто_ли(d.width) && (int)~d.width > 0)
				sz.cx = ~d.width;
			if(!пусто_ли(d.height) && (int)~d.height > 0)
				sz.cy = ~d.height;
			obj.устРазм(sz);
			if(!пусто_ли(d.ydelta))
				obj.SetYDelta(~d.ydelta);
			obj.KeepRatio(d.keepratio);
			ReplaceObject(obj);
			return;
		}
	}
}

}
