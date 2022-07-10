#include "RichText.h"

namespace РНЦП {

void  RichText::CatPick(RichText&& p)
{
	MergeStyles(p);
	int c = part.дайСчёт();
	part.приставьПодбор(pick(p.part));
	for(int i = c; i < part.дайСчёт(); i++)
		if(IsPara(i))
			part[i].дай<Para>().инивалидируй();
	RefreshAll();
}

RichPos::RichPos()
{
	tabtextparti = tabtextpartcount = tabposintabtext = tabtextlen = table = tablen = posintab =
	celllen = posincell = parai = partcount = posinpara = paralen = 0;
	parenttab = Null;

	tabsize = Размер(0, 0);
	cell = Точка(0, 0);
	chr = '?';
}

RichPos RichText::GetRichPos(int pos, int maxlevel) const
{
	Стопор::Замок __(mutex);
	RichPos rp;
	rp.level = 0;
	RichTxt::GetRichPos(pos, rp, 0, maxlevel, style);
	if(пусто_ли(rp.parenttab))
		rp.parenttab = 0;
	return rp;
}

int  RichText::AdjustCursor(int anchor, int cursor) const
{
	Стопор::Замок __(mutex);
	int d = anchor;
	const RichTxt& txt = GetConstText(anchor);
	d -= anchor;
	if(!(cursor == txt.дайДлину() && !txt.IsTable(txt.GetPartCount() - 1)))
		cursor = minmax(cursor - d, 0, txt.дайДлину());
	int c = cursor;
	int pi = txt.FindPart(c);
	if(txt.IsTable(pi)) {
		if(cursor < anchor) {
			while(--pi >= 0)
				if(txt.IsPara(pi))
					return d + txt.GetPartPos(pi) + txt.GetPartLength(pi);
			while(++pi < txt.GetPartCount())
				if(txt.IsPara(pi))
					return d + txt.GetPartPos(pi);
			return d + anchor;
		}
		else {
			while(++pi < txt.GetPartCount())
				if(txt.IsPara(pi))
					return d + txt.GetPartPos(pi);
			while(--pi >= 0)
				if(txt.IsPara(pi))
					return d + txt.GetPartPos(pi) + txt.GetPartLength(pi);
			return d + anchor;
		}
	}
	return d + cursor;
}

void RichText::удали(int pos, int count)
{
	дайДлину();
	int p = pos;
	int l = GetConstText(p).дайДлину();
	count = min(l - p, count);
	RichTxt& txt = GetUpdateText(pos);
	int pos2 = pos + count;
	int pi = txt.FindPart(pos);
	int pi2 = txt.FindPart(pos2);
	if(pi == pi2) {
		RichPara pa, pa1;
		pa = txt.дай(pi, style);
		pa1 <<= pa;
		pa.обрежь(pos);
		pa1.середина(pos2);
		pa.приставь(pa1);
		txt.помести(pi, pa, style);
	}
	else {
		RichPara pa2 = txt.дай(pi2, style);
		RichPara pa = txt.дай(pi, pa2.формат.styleid, style);
		txt.part.удали(pi, pi2 - pi);
		pa.обрежь(pos);
		pa2.середина(pos2);
		pa.приставь(pa2);
		pa.формат = pa2.формат;
		txt.помести(pi, pa, style);
		txt.SetRefreshFrom(pi);
	}
}

void RichText::вставь(int pos, const RichText& p)
{
	MergeStyles(p);
	if(p.дайДлину() == 0)
		return;
	ПРОВЕРЬ(pos >= 0 && pos <= дайДлину());
	RichTxt& txt = GetUpdateText(pos);
	int pi = txt.FindPart(pos);
	int n = p.part.дайСчёт() - 1;
	if(n) {
		txt.part.вставь(pi + 1, p.part, 1, n);
		RichPara pa1, pa2;
		pa1 = txt.дай(pi, style);
		pa2 = txt.дай(pi + n, pa1.формат.styleid, style);
		pa1.середина(pos);
		pa2.приставь(pa1);
		pa2.формат = pa1.формат;
		txt.помести(pi + n, pa2, style);
		pa2 = p.RichTxt::дай(0, p.дайСтиль(p.GetParaStyle(0)));
		pa1 = txt.дай(pi, pa2.формат.styleid, style);
		pa1.обрежь(pos);
		pa1.приставь(pa2);
		pa1.формат = pa2.формат;
		txt.помести(pi, pa1, style);
		txt.SetRefreshFrom(pi);
	}
	else {
		RichPara pa0, pa2;
		pa0 = txt.дай(pi, style);
		pa2 <<= pa0;
		pa0.обрежь(pos);
		pa2.середина(pos);
		pa0.приставь(p.RichTxt::дай(0, p.дайСтиль(pa0.формат.styleid)));
		pa0.приставь(pa2);
		txt.помести(pi, pa0, style);
	}
}

RichText RichText::копируй(int pos, int count) const
{
	Стопор::Замок __(mutex);

	RichText r;
	r.SetStyles(style);

	const RichTxt& txt = GetConstText(pos);
	bool addp = false;
	if(count > txt.дайДлину() - pos) {
		count = txt.дайДлину() - pos;
		addp = true;
	}
	int pos2 = pos + count;
	int pi = txt.FindPart(pos);
	int pi2 = txt.FindPart(pos2);

	if(pi == pi2) {
		RichPara pa = txt.дай(pi, style);
		pa.обрежь(pos2);
		pa.середина(pos);
		r.конкат(pa);
	}
	else {
		RichPara pa = txt.дай(pi++, style);
		pa.середина(pos);
		r.конкат(pa);
		while(pi < pi2) {
			if(txt.IsPara(pi))
				r.конкат(txt.дай(pi, style));
			else {
				RichTable tab(txt.GetTable(pi), 1);
				r.CatPick(pick(tab));
			}
			pi++;
		}
		pa = txt.дай(pi2, style);
		pa.обрежь(pos2);
		r.конкат(pa);
	}
	if(addp)
		r.конкат(RichPara());
	return r;
}

RichText::FormatInfo RichText::GetFormatInfo(int pos, int count) const
{
	Стопор::Замок __(mutex);

	const RichTxt& txt = GetConstText(pos);

	count = min(txt.дайДлину() - pos, count);

	int pos2 = pos + count;
	int pi = txt.FindPart(pos);
	int pi2 = txt.FindPart(pos2);

	FormatInfo fi;
	RichPara::фмт fmt;
	RichPara pa = txt.дай(pi, style);
	fmt = pa.формат;
	int i = pa.FindPart(pos);
	if(i < pa.дайСчёт())
		(RichPara::CharFormat&)fmt = pa[i].формат;
	fi.уст(fmt);
	i++;
	if(pi == pi2) {
		int i2 = pa.FindPart(pos2);
		while(i < i2)
			fi.Combine(pa[i++].формат);
		if(pos2 && i2 < pa.дайСчёт())
			fi.Combine(pa[i2].формат);
	}
	else {
		while(i < pa.дайСчёт())
			fi.Combine(pa[i++].формат);
		fi.Combine(pa.формат);
		pi++;
		bool first = false;
		txt.CombineFormat(fi, pi, pi2, first, style);
		pa = txt.дай(pi2, style);
		int i2 = pa.FindPart(pos2);
		for(i = 0; i < i2; i++)
			fi.Combine(pa[i].формат);
		if(pos2 && i2 < pa.дайСчёт())
			fi.Combine(pa[i2].формат);
	}
	return fi;
}

void RichText::ApplyFormatInfo(int pos, const FormatInfo& fi, int count)
{
	RichTxt& txt = GetUpdateText(pos);

	int pos2 = min(txt.дайДлину(), pos + count);
	int pi = txt.FindPart(pos);
	int pi2 = txt.FindPart(pos2);

	ПРОВЕРЬ(txt.IsPara(pi) && txt.IsPara(pi2));

	if(pi == pi2) {
		RichPara pa, pa1, pa2;
		pa = txt.дай(pi, style);
		ApplyStyle(fi, pa, style);
		pa1 <<= pa;
		pa2 <<= pa;
		pa.обрежь(pos);
		pa2.середина(pos2);
		pa1.обрежь(pos2);
		pa1.середина(pos);
		Apply(fi, pa1, style);
		pa.приставь(pa1);
		pa.приставь(pa2);
		fi.ApplyTo(pa.формат);
		txt.помести(pi, pa, style);
	}
	else {
		RichPara pa, pa1;
//		if(fi.paravalid & RichText::STYLE)
//			pa = txt.дай(pi, fi.styleid, style);
//		else
			pa = txt.дай(pi, style);
		ApplyStyle(fi, pa, style);
		pa1 <<= pa;
		pa.обрежь(pos);
		pa1.середина(pos);
		Apply(fi, pa1, style);
		fi.ApplyTo(pa.формат);
		pa.приставь(pa1);
		txt.помести(pi, pa, style);
		pi++;
		txt.ApplyFormat(fi, pi, pi2, style);
//		if(fi.paravalid & RichText::STYLE)
//			pa = txt.дай(pi2, fi.styleid, style);
//		else
			pa = txt.дай(pi2, style);
		ApplyStyle(fi, pa, style);
		pa1 <<= pa;
		pa.обрежь(pos2);
		pa1.середина(pos2);
		Apply(fi, pa, style);
		pa.приставь(pa1);
		txt.помести(pi2, pa, style);
	}
}

void RichText::ReplaceStyle(int pos, const Uuid& id)
{
	RichTxt& txt = GetUpdateText(pos);
	int p = txt.FindPart(pos);
	RichPara pa = txt.дай(p, id, style);
	txt.уст(p, pa, style);
}

void RichText::ReStyle(int pos, const Uuid& id)
{
	RichTxt& txt = GetUpdateText(pos);
	int p = txt.FindPart(pos);
	RichPara pa = txt.дай(p, style);
	pa.формат.styleid = id;
	txt.уст(p, pa, style);
}

RichText::Formating RichText::SaveFormat(int pos, int count) const
{
	Стопор::Замок __(mutex);
	const RichTxt& txt = GetConstText(pos);
	count += pos;
	Formating r;
	txt.SaveFormat(r, txt.FindPart(pos), txt.FindPart(count), style);
	return r;
}

void RichText::RestoreFormat(int pos, const RichText::Formating& info)
{
	RichTxt& txt = GetUpdateText(pos);
	int ii = 0;
	txt.RestoreFormat(txt.FindPart(pos), info, ii, style);
}

void RichText::иниц()
{
	RichTxt::иниц();
	RichStyle& s = style.добавь(RichStyle::GetDefaultId());
	s.имя = "дефолт";
	nolinks = false;
}

void RichText::очисть()
{
	RichTxt::очисть();
	style.очисть();
	иниц();
}

RichText::RichText(const RichText& x, int)
   : RichTxt(x, 1), style(x.style, 1)
{
	nolinks = x.nolinks;
	footer_hack = x.footer_hack;
}

RichText::RichText(RichText&& x)
   : RichTxt(pick(x)), style(pick(x.style))
{
	nolinks = x.nolinks;
	footer_hack = x.footer_hack;
}

RichText& RichText::operator=(RichText&& x)
{
	(RichTxt&)(*this) = pick(x);
	style = pick(x.style);
	nolinks = x.nolinks;
	footer_hack = x.footer_hack;
	return *this;
}

RichText::RichText(RichTxt&& x, RichStyles&& st)
   : RichTxt(pick(x)), style(pick(st))
{
	nolinks = false;
}

void RichTextLayoutTracer::Paragraph(const Прям& page, PageY y, const RichPara& para) {}
void RichTextLayoutTracer::EndParagraph(PageY y) {}
void RichTextLayoutTracer::Table(const Прям& page, PageY y, const RichTable& table) {}
void RichTextLayoutTracer::EndTable(PageY y) {}
void RichTextLayoutTracer::TableRow(const Прям& page, PageY y, int i, const RichTable& table) {}
void RichTextLayoutTracer::EndTableRow(PageY y) {}
void RichTextLayoutTracer::TableCell(const Прям& page, PageY y, int i, int j, const RichTable& table, PageY npy) {}
void RichTextLayoutTracer::EndTableCell(PageY y) {}

}
