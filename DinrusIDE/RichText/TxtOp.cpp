#include "RichText.h"

namespace РНЦП {

void RichTxt::GetAllLanguages(Индекс<int>& all) const
{
	for(int i = 0; i < part.дайСчёт(); i++) {
		if(IsTable(i)) {
			const RichTable& tab = part[i].дай<RichTable>();
			for(int i = 0; i < tab.GetRows(); i++)
				for(int j = 0; j < tab.дайКолонки(); j++)
					if(tab(i, j))
						tab[i][j].text.GetAllLanguages(all);
		}
		else {
			RichPara p = дай(i, RichStyle::GetDefault());
			all.найдиДобавь(p.формат.language);
			for(int i = 0; i < p.дайСчёт(); i++)
				all.найдиДобавь(p[i].формат.language);
		}
	}
}

Вектор<int> RichTxt::GetAllLanguages() const
{
	Индекс<int> all;
	GetAllLanguages(all);
	return all.подбериКлючи();
}

bool RichTxt::Update(ParaOp& op)
{
	bool val = false;
	for(int i = 0; i < part.дайСчёт(); i++)
		if(IsTable(i)) {
			RichTable& tab = part[i].дай<RichTable>();
			for(int i = 0; i < tab.GetRows(); i++)
				for(int j = 0; j < tab.дайКолонки(); j++)
					if(tab(i, j))
						if(tab[i][j].text.Update(op)) {
							tab.InvalidateRefresh(i, j);
							val = true;
						}
		}
		else
		if(op(part[i].дай<Para>()))
			val = true;
	RefreshAll();
	return val;
}

RichTxt& RichTxt::GetText0(int& pos, bool update)
{
	if(update)
		инивалидируй();
	int p = pos;
	int pi = FindPart(p);
	if(IsTable(pi)) {
		RichTable& tab = part[pi].дай<RichTable>();
		Точка cl = tab.FindCell(p);
		if(update) {
			tab.InvalidateRefresh(cl);
			SetRefresh(pi);
		}
		pos = p;
		return tab[cl].text.GetText0(pos, update);
	}
	return *this;
}

RichTxt& RichTxt::GetUpdateText(int& pos)
{
	return GetText0(pos, true);
}

const RichTxt& RichTxt::GetConstText(int& pos) const
{
	return const_cast<RichTxt *>(this)->GetText0(pos, false);
}

RichTable& RichTxt::GetTable0(int table, bool update)
{
	if(update)
		инивалидируй();
	for(int i = 0;; i++)
		if(IsTable(i)) {
			table--;
			RichTable& tab = part[i].дай<RichTable>();
			if(table <= tab.GetTableCount()) {
				if(update)
					SetRefresh(i);
				if(table == 0)
					return tab;
				for(int i = 0; i < tab.GetRows(); i++)
					for(int j = 0; j < tab.дайКолонки(); j++)
						if(tab(i, j)) {
							RichTxt& txt = tab[i][j].text;
							if(table <= txt.GetTableCount()) {
								if(update)
									tab.InvalidateRefresh(i, j);
								return txt.GetTable0(table, update);
							}
							table -= txt.GetTableCount();
						}
				NEVER();
			}
			else
				table -= tab.GetTableCount();
		}
}

RichTable& RichTxt::GetUpdateTable(int table)
{
	return GetTable0(table, true);
}

const RichTable& RichTxt::GetConstTable(int table) const {
	return const_cast<RichTxt *>(this)->GetTable0(table, false);
}

void RichTxt::CombineFormat(FormatInfo& fi, int pi, int pi2, bool& first, const RichStyles& style) const
{
	while(pi < pi2) {
		if(IsTable(pi)) {
			const RichTable& tab = part[pi].дай<RichTable>();
			for(int i = 0; i < tab.GetRows(); i++)
				for(int j = 0; j < tab.дайКолонки(); j++)
					if(tab(i, j)) {
						const RichTxt& txt = tab[i][j].text;
						txt.CombineFormat(fi, 0, txt.GetPartCount(), first, style);
					}
		}
		else {
			RichPara pa = дай(pi, style);
			if(first) {
				fi.уст(pa.формат);
				if(pa.дайСчёт())
					fi.уст(pa[0].формат);
				first = false;
			}
			else
				fi.Combine(pa.формат);
			for(int i = first; i < pa.дайСчёт(); i++)
				fi.Combine(pa[i].формат);
		}
		pi++;
	}
}

void RichTxt::ApplyStyle(const RichText::FormatInfo& fi, RichPara& pa, const RichStyles& style)
{
	if(fi.paravalid & STYLE) {
		int q = style.найди(fi.styleid);
		if(q >= 0) {
			pa.ApplyStyle(style[q].формат);
			pa.формат.styleid = fi.styleid;
		}
	}
}

void RichTxt::Apply(const RichText::FormatInfo& fi, RichPara& pa, const RichStyles& style)
{
	ApplyStyle(fi, pa, style);
	for(int i = 0; i < pa.дайСчёт(); i++)
		fi.ApplyTo(pa[i].формат);
	fi.ApplyTo(pa.формат);
}

void RichTxt::ApplyFormat(const FormatInfo& fi, int pi, int pi2, const RichStyles& style)
{
	while(pi < pi2) {
		if(IsTable(pi)) {
			RichTable& tab = part[pi].дай<RichTable>();
			for(int i = 0; i < tab.GetRows(); i++)
				for(int j = 0; j < tab.дайКолонки(); j++)
					if(tab(i, j)) {
						RichTxt& txt = tab[i][j].text;
						tab.InvalidateRefresh(i, j);
						txt.ApplyFormat(fi, 0, txt.GetPartCount(), style);
					}
		}
		else {
			RichPara pa;
			if(fi.paravalid & RichText::STYLE)
				pa = RichTxt::дай(pi, fi.styleid, style);
			else
				pa = дай(pi, style);
			Apply(fi, pa, style);
			помести(pi, pa, style);
		}
		pi++;
	}
}

void RichTxt::SaveFormat(Formating& r, int p1, int p2, const RichStyles& style) const
{
	Массив<RichObject> dummy;
	for(int i = p1; i <= p2; i++)
		if(IsTable(i)) {
			const RichTable& tab = part[i].дай<RichTable>();
			for(int i = 0; i < tab.GetRows(); i++)
				for(int j = 0; j < tab.дайКолонки(); j++)
					if(tab(i, j)) {
						const RichTxt& txt = tab[i][j].text;
						txt.SaveFormat(r, 0, txt.GetPartCount() - 1, style);
					}
		}
		else {
			RichPara pa = дай(i, style);
			for(int i = 0; i < pa.дайСчёт(); i++) {
				RichPara::Part& p = pa[i];
				int q = p.дайДлину();
				p.field = Ид();
				p.object = RichObject();
				ШТкст h;
				while(q) {
					int c = min(q, 50000);
					h.конкат(c + 32);
					q -= c;
				}
				p.text = h;
			}
			r.styleid.добавь(pa.формат.styleid);
			r.формат.добавь(pa.Pack(дайСтиль(style, pa.формат.styleid).формат, dummy));
		}
}

void RichTxt::RestoreFormat(int pi, const Formating& info, int& ii, const RichStyles& style)
{
	Массив<RichObject> dummy;
	while(ii < info.формат.дайСчёт() && pi < GetPartCount()) {
		if(IsTable(pi)) {
			RichTable& tab = part[pi].дай<RichTable>();
			for(int i = 0; i < tab.GetRows(); i++)
				for(int j = 0; j < tab.дайКолонки(); j++) {
					if(tab(i, j)) {
						if(ii >= info.формат.дайСчёт())
							return;
						tab.InvalidateRefresh(i, j);
						tab[i][j].text.RestoreFormat(0, info, ii, style);
					}
				}
			pi++;
		}
		else {
			RichPara pa = дай(pi, style);
			RichPara pf;
			pf.Unpack(info.формат[ii], dummy, дайСтиль(style, info.styleid[ii]).формат);
			RichPara t;
			t.формат = pf.формат;
			int si = 0;
			int sp = 0;
			for(int j = 0; j < pf.дайСчёт(); j++) {
				const RichPara::Part& q = pf[j];
				for(int k = 0; k < q.text.дайДлину(); k++) {
					int len = q.text[k] - 32;
					t.part.добавь().формат = q.формат;
					while(len) {
						const RichPara::Part& p = pa[si];
						if(p.текст_ли()) {
							int l = min(len, p.дайДлину() - sp);
							t.part.верх().text.конкат(p.text.середина(sp, l));
							sp += l;
							len -= l;
							ПРОВЕРЬ(sp <= p.дайДлину());
							if(sp >= p.дайДлину()) {
								sp = 0;
								si++;
							}
						}
						else {
							ПРОВЕРЬ(sp == 0);
							(t.part.добавь() = pa[si++]).формат = q.формат;
							len--;
							sp = 0;
						}
					}
				}
			}
			ПРОВЕРЬ(si == pa.дайСчёт() && sp == 0);
			помести(pi, t, style);
			ii++;
			pi++;
		}
	}
}

ШТкст RichTxt::GetPlainText(bool withcr) const {
	ШТкст clip;
	for(int pi = 0; pi < GetPartCount(); pi++) {
		if(pi) {
			if(withcr)
				clip.конкат('\r');
			clip.конкат('\n');
		}
		if(IsTable(pi)) {
			const RichTable& tab = part[pi].дай<RichTable>();
			for(int i = 0; i < tab.GetRows(); i++)
				for(int j = 0; j < tab.дайКолонки(); j++)
					if(tab(i, j)) {
						if(i || j) {
							if(withcr)
								clip.конкат('\r');
							clip.конкат('\n');
						}
						clip << tab[i][j].text.GetPlainText(withcr);
					}
		}
		else
			clip.конкат(дай(pi, RichStyle::GetDefault()).дайТекст());
	}
	return clip;
}

RichTxt& RichTxt::GetTableUpdateText(int table, const RichStyles& style, int& pi)
{
	инивалидируй();
	for(int i = 0;; i++)
		if(IsTable(i)) {
			table--;
			RichTable& tab = part[i].дай<RichTable>();
			if(table <= tab.GetTableCount()) {
				SetRefresh(i);
				if(table == 0) {
					pi = i;
					return *this;
				}
				for(int i = 0; i < tab.GetRows(); i++)
					for(int j = 0; j < tab.дайКолонки(); j++)
						if(tab(i, j)) {
							RichTxt& txt = tab[i][j].text;
							if(table <= txt.GetTableCount()) {
								tab.InvalidateRefresh(i, j);
								return txt.GetTableUpdateText(table, style, pi);
							}
							table -= txt.GetTableCount();
						}
				NEVER();
			}
			else
				table -= tab.GetTableCount();
		}
	NEVER();
}

void RichTxt::нормализуй()
{
	RichPara pa;
	if(GetPartCount() && IsTable(0)) {
		part.вставь(0);
		помести(0, pa, RichStyle::GetDefault());
		инивалидируй();
	}
	if(GetPartCount() == 0 || IsTable(GetPartCount() - 1)) {
		помести(GetPartCount(), pa, RichStyle::GetDefault());
		инивалидируй();
	}
}

}
