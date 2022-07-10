#include "RichText.h"

namespace РНЦП {

void RichTxt::Para::инивалидируй()
{
	INTERLOCKED {
		static int64 ss;
		updateserial = ++ss;
		ccx = -1;
	}
}

RichTxt::Para::Para(const Para& src, int)
: object(src.object, 1)
{
	length = src.length;
	styleid = src.styleid;
	content = src.content;
	haspos = src.haspos;
	if(src.number)
		number.создай<RichPara::NumberFormat>(*src.number);
	инивалидируй();
	checked = false;
}

RichTxt::Part::Part(const Part& src, int) {
	if(src.является<Para>())
		создай<Para>() <<= src.дай<Para>();
	else
		создай<RichTable>() <<= src.дай<RichTable>();
}

RichTxt::Part::Part() {}

void RichTxt::инивалидируй()
{
	length = -1;
	tabcount = -1;
}

int RichTxt::GetPartLength(int pi) const
{
	return part[pi].является<RichTable>() ? part[pi].дай<RichTable>().дайДлину() : part[pi].дай<Para>().length;
}

bool             RichTxt::IsTable(int i) const  { return part[i].является<RichTable>(); }
const RichTable& RichTxt::GetTable(int i) const { return part[i].дай<RichTable>(); }


int RichTxt::FindPart(int& pos) const
{
	int pi = 0;
	while(pi < part.дайСчёт()) {
		int l = GetPartLength(pi) + 1;
		if(pos < l)
			break;
		pos -= l;
		pi++;
	}
	if(pi >= part.дайСчёт())
		pos = 0;
	return pi;
}

int  RichTxt::GetPartPos(int pi) const
{
	int pos = 0;
	for(int i = 0; i < pi; i++)
		pos += GetPartLength(i) + 1;
	return pos;
}

void RichTxt::SetRefresh(int parti)
{
	switch(r_type) {
	case NONE:
		r_parti = parti;
		r_type = PARA;
		if(IsPara(parti)) {
			Para& pp = part[parti].дай<Para>();
			if(pp.ccx >= 0) {
				r_paraocx = pp.ccx;
				r_paraocy = сумма(pp.linecy, 0) + pp.before + pp.after;
				r_keep = pp.keep;
				r_keepnext = pp.keepnext;
				r_firstonpage = pp.firstonpage;
				r_newpage = pp.newpage || pp.header_qtf.дайСчёт() || pp.footer_qtf.дайСчёт();
				break;
			}
			else
				r_type = FROM;
		}
		break;
	case PARA:
		if(parti == r_parti) break;
	case FROM:
		r_parti = min(parti, r_parti);
		r_type = FROM;
		break;
	}
}

void RichTxt::SetRefreshFrom(int parti)
{
	r_type = FROM;
	if(r_type == NONE)
		r_parti = parti;
	else
		r_parti = min(parti, r_parti);
}

void RichTxt::помести(int i, const RichPara& p, const RichStyle& s)
{
	if(i >= part.дайСчёт() || !IsPara(i))
		part.по(i).создай<Para>();
	Para& pp = part[i].дай<Para>();
	int numbering = p.формат.GetNumberLevel();
	if(pp.numbering != numbering)
		SetRefreshFrom(i);
	else
		SetRefresh(i);
	pp.number.очисть();
	pp.content = p.Pack(s.формат, pp.object);
	pp.инивалидируй();
	pp.checked = false;
	pp.styleid = p.формат.styleid;
	pp.length = p.дайДлину();
	pp.numbering = numbering;
	pp.spellerrors.очисть();
	pp.haspos = p.HasPos();
	if(numbering >= 0 || p.формат.reset_number)
		pp.number.создай() = p.формат;
}

void RichTxt::помести(int i, const RichPara& p, const RichStyles& s)
{
	помести(i, p, дайСтиль(s, p.формат.styleid));
}

void RichTxt::уст(int i, const RichPara& p, const RichStyles& s)
{
	помести(i, p, s);
	инивалидируй();
}

void RichTxt::вставь(int i, const RichPara& p, const RichStyles& s)
{
	part.вставь(i);
	уст(i, p, s);
}

void RichTxt::RemovePart(int parti)
{
	part.удали(parti);
	инивалидируй();
}

void RichTxt::SetPick(int i, RichTable&& p)
{
	const_cast<RichTable&>(p).нормализуй();
	part.по(i).создай<RichTable>() = pick(p);
	инивалидируй();
	SetRefresh(i);
}

RichPara RichTxt::дай(int parai, const RichStyle& style, bool usecache) const
{
	ПРОВЕРЬ(part[parai].является<Para>());
	const Para& pp = part[parai].дай<Para>();
	RichPara p;
	if(usecache)
		p.CacheId(pp.updateserial);
	p.Unpack(pp.content, pp.object, style.формат);
	return p;
}

RichPara RichTxt::дай(int parai, const Uuid& styleid, const RichStyles& s, bool usecache) const
{
	RichPara p = дай(parai, дайСтиль(s, styleid), usecache);
	p.формат.styleid = styleid;
	return p;
}

RichPara RichTxt::дай(int parti, const RichStyles& s, bool usecache) const
{
	return дай(parti, part[parti].дай<Para>().styleid, s, usecache);
}

RichPara RichTxt::дай(int parai, const RichStyle& style) const
{
	return дай(parai, style, false);
}

RichPara RichTxt::дай(int parai, const Uuid& styleid, const RichStyles& s) const
{
	return дай(parai, styleid, s, false);
}

RichPara RichTxt::дай(int i, const RichStyles& s) const
{
	return дай(i, s, false);
}

bool RichTxt::пустой() const
{
	return part.пустой() || part.дайСчёт() == 1 && ComputeLength() == 0;
}

int RichTxt::ComputeLength() const
{
	int length = part.дайСчёт() - 1;
	for(int i = 0; i < part.дайСчёт(); i++)
		length += GetPartLength(i);
	return length;
}

int RichTxt::дайДлину() const
{ // expects int to be atomic, worst 'race' is that it gets computed more times...
	if(length < 0)
		length = ComputeLength();
	return length;
}

int  RichTxt::GetTableCount() const
{ // expects int to be atomic, worst 'race' is that it gets computed more times...
	if(tabcount < 0) {
		int n = 0;
		for(int i = 0; i < part.дайСчёт(); i++)
			if(IsTable(i))
				n += GetTable(i).GetTableCount() + 1;
		tabcount = n;
	}
	return tabcount;
}

bool RichTxt::EvaluateFields(const RichStyles& s, ВекторМап<Ткст, Значение>& vars)
{
	bool b = false;
	for(int i = 0; i < part.дайСчёт(); i++)
		if(IsTable(i)) {
			RichTable& tab = part[i].дай<RichTable>();
			for(int i = 0; i < tab.GetRows(); i++)
				for(int j = 0; j < tab.дайКолонки(); j++)
					if(tab(i, j) && tab[i][j].text.EvaluateFields(s, vars)) {
						tab.InvalidateRefresh(i, j);
						b = true;
					}
		}
		else {
			RichPara p = дай(i, s);
			if(p.EvaluateFields(vars)) {
				b = true;
				уст(i, p, s);
			}
		}
	return b;
}

void RichTxt::ClearSpelling()
{
	for(int i = 0; i < part.дайСчёт(); i++) {
		if(IsTable(i)) {
			RichTable& tab = part[i].дай<RichTable>();
			for(int i = 0; i < tab.GetRows(); i++)
				for(int j = 0; j < tab.дайКолонки(); j++)
					if(tab(i, j)) {
						tab[i][j].text.ClearSpelling();
						tab.InvalidateRefresh(i, j);
					}
		}
		else {
			Para& p = part[i].дай<Para>();
			p.spellerrors.очисть();
			p.checked = false;
		}
	}
}

void RichTxt::SetParaStyle(int i, const Uuid& id)
{
	ПРОВЕРЬ(IsPara(i));
	Para& p = part[i].дай<Para>();
	p.styleid = id;
	p.инивалидируй();
	SetRefreshFrom(i);
}

void RichTxt::GetRichPos(int pos, RichPos& rp, int ti, int maxlevel, const RichStyles& st) const
{
	int p = pos;
	int pti = ti;
	for(int i = 0; i < part.дайСчёт(); i++) {
		int l = GetPartLength(i) + 1;
		if(pos < l) {
			if(IsTable(i)) {
				const RichTable& tab = GetTable(i);
				rp.level++;
				rp.tabtextparti = i;
				rp.tabtextpartcount = part.дайСчёт();
				rp.tabtextlen = дайДлину();
				rp.tabposintabtext = p - pos;
				rp.posintab = pos;
				rp.cell = tab.FindCell(pos);
				rp.posincell = pos;
				rp.tabsize = tab.дайРазм();
				rp.tablen = tab.дайДлину();
				const RichTxt& ct = tab[rp.cell].text;
				rp.celllen = ct.дайДлину();
				rp.parenttab = pti;
				rp.table = ti + 1;
				if(rp.level < maxlevel)
					ct.GetRichPos(pos, rp, ti + 1 + tab.GetTableCount(rp.cell), maxlevel, st);
				return;
			}
			else {
				rp.posinpara = pos;
				rp.parai = i;
				rp.partcount = part.дайСчёт();
				rp.paralen = l - 1;
				дай(i, st, true).GetRichPos(rp, pos);
				return;
			}
		}
		pos -= l;
		if(IsTable(i))
			ti += 1 + GetTable(i).GetTableCount();
	}
	rp.parai = part.дайСчёт();
	rp.posinpara = 0;
	rp.partcount = part.дайСчёт();
	rp.paralen = 0;
}

RichPara::фмт RichTxt::GetFirstFormat(const RichStyles& style) const
{
	if(part.пустой())
		return RichPara::фмт();
	if(IsTable(0)) {
		const RichTable& tab = GetTable(0);
		return tab[0][0].text.GetFirstFormat(style);
	}
	RichPos rp;
	дай(0, style).GetRichPos(rp, 0);
	return rp.формат;
}

RichCellPos RichTxt::GetCellPos(int table, int row, int column) const
{
	int pos = 0;
	for(int i = 0;; i++) {
		if(IsTable(i)) {
			table--;
			const RichTable& tab = part[i].дай<RichTable>();
			if(table <= tab.GetTableCount()) {
				if(table == 0) {
					RichCellPos p;
					p.tabsize = tab.дайРазм();
					p.tablen = tab.дайДлину();
					p.cellpos = tab.GetCellPos(row, column);
					p.pos = p.tabpos = pos;
					p.pos += p.cellpos;
					p.textlen = дайДлину();
					p.level = 1;
					return p;
				}
				for(int i = 0; i < tab.GetRows(); i++)
					for(int j = 0; j < tab.дайКолонки(); j++)
						if(tab(i, j)) {
							const RichTxt& txt = tab[i][j].text;
							if(table <= txt.GetTableCount()) {
								RichCellPos p = txt.GetCellPos(table, row, column);
								p.pos += pos + tab.GetCellPos(i, j);
								p.level++;
								return p;
							}
							table -= txt.GetTableCount();
						}
				NEVER();
			}
			else
				table -= tab.GetTableCount();
		}
		pos += GetPartLength(i) + 1;
	}
	NEVER();
	return RichCellPos();
}

RichCellPos RichTxt::GetCellPos(int table, Точка p) const
{
	return GetCellPos(table, p.y, p.x);
}

bool RichTxt::Iterate(UpdateIterator& r, int gpos, const RichStyles& s)
{
	for(int pi = 0; pi < part.дайСчёт(); pi++)
		if(IsTable(pi)) {
			RichTable& tab = part[pi].дай<RichTable>();
			for(int i = 0; i < tab.GetRows(); i++)
				for(int j = 0; j < tab.дайКолонки(); j++)
					if(tab(i, j)) {
						if(tab[i][j].text.Iterate(r, gpos, s))
							return true;
						gpos += tab[i][j].text.дайДлину() + 1;
					}
		}
		else {
			RichPara p = дай(pi, s);
			int q = r(gpos, p);
			if(q & UpdateIterator::UPDATE)
				уст(pi, p, s);
			if(q & UpdateIterator::STOP)
				return true;
			gpos += GetPartLength(pi) + 1;
		}
	return false;
}

struct sIter__ : RichTxt::UpdateIterator {
	RichTxt::Обходчик *iter;
	virtual int operator()(int pos, RichPara& para) {
		return iter->operator()(pos, para) ? STOP : CONTINUE;
	}
};

bool RichTxt::Iterate(Обходчик& r, int gpos, const RichStyles& s) const
{
	sIter__ it;
	it.iter = &r;
	return const_cast<RichTxt *>(this)->Iterate(it, gpos, s);
}

void RichTxt::иниц()
{
	r_type = ALL;
	r_parti = 0;
	r_paraocx = r_paraocy = -1;	
	tabcount = length = 0;
}

void RichTxt::очисть()
{
	part.очисть();
	иниц();
}

RichTxt::RichTxt()
{
	иниц();
}

RichTxt::RichTxt(const RichTxt& src, int)
{
	иниц();
	part <<= src.part;
	length = src.length;
	tabcount = src.tabcount;
}

#ifdef _ОТЛАДКА

#define DMP(x)   s << #x << "=" << x << ", "

Ткст RichPos::вТкст() const
{
	Ткст s;
	DMP(tabtextparti);
	DMP(tabtextpartcount);
	DMP(tabposintabtext);
	DMP(tabtextlen);
	DMP(table);
	DMP(tabsize);
	DMP(cell);
	DMP(tablen);
	DMP(posintab);
	DMP(celllen);
	DMP(posincell);
	DMP(parai);
	DMP(partcount);
	DMP(posinpara);
	DMP(paralen);
	DMP(level);
	DMP(parenttab);
	s << "char: " << (char)chr;
	return s;
}

Ткст RichCellPos::вТкст() const
{
	Ткст s;
	s << "pos: " << pos << ", textlen: " << textlen << ", size: " << tabsize << ", tabpos: " << tabpos
	  << ", tablen: " << tablen << ", cellpos: " << cellpos << ", celllen: " << celllen << ", level: " << level;
	return s;
}

#endif

}
