#include "RichText.h"

namespace РНЦП {

const RichStyle& RichStyle::GetDefault()
{
	return Single<RichStyle>();
}

Uuid  RichStyle::GetDefaultId()
{
	Uuid id;
	id.устПусто();
	return id;
}

const RichStyle& дайСтиль(const RichStyles& s, const Uuid& id)
{
	return s.дай(id, RichStyle::GetDefault());
}

int FindStyleWithName(const RichStyles& style, const Ткст& имя)
{
	for(int j = 0; j < style.дайСчёт(); j++)
		if(style[j].имя == имя)
			return j;
	return -1;
}

struct RichText::StyleChangeOp : RichTxt::ParaOp {
	Uuid id, nid;

	virtual bool operator()(RichTxt::Para& p) {
		if(p.styleid == id) {
			p.styleid = nid;
			p.инивалидируй();
			return true;
		}
		return false;
	}
};

void RichText::устСтиль(const Uuid& id, const RichStyle& _style)
{
	инивалидируй();
	RichStyle& s = style.дайДобавь(id);
	s = _style;
	s.формат.styleid = id;
	s.формат.language = 0;
	s.формат.newpage = false;
	StyleChangeOp op;
	op.nid = op.id = id;
	Update(op);
}

struct RichText::SetStylesOp : RichTxt::ParaOp {
	RichStyles *style;

	virtual bool operator()(RichTxt::Para& p) {
		p.инивалидируй();
		if(style->найди(p.styleid) < 0)
			p.styleid = RichStyle::GetDefaultId();
		return true;
	}
};

void RichText::SetStyles(const RichStyles& _style)
{
	инивалидируй();
	style <<= _style;
	SetStylesOp op;
	op.style = &style;
	Update(op);
}

Uuid RichText::GetStyleId(const Ткст& имя) const
{
	int i = FindStyleWithName(style, имя);
	return i >= 0 ? style.дайКлюч(i) : RichStyle::GetDefaultId();
}

void  RichText::RemoveStyle(const Uuid& id)
{
	ПРОВЕРЬ(id != RichStyle::GetDefaultId());
	инивалидируй();
	StyleChangeOp op;
	op.id = id;
	op.nid = RichStyle::GetDefaultId();
	Update(op);
	style.удалиКлюч(id);
}

struct RichText::MergeStylesOp : RichTxt::ParaOp {
	RichText       *text;
	const RichText *stxt;

	virtual bool operator()(RichTxt::Para& p) {
		if(text->style.найди(p.styleid) < 0) {
			RichStyle& s = text->style.дайДобавь(p.styleid);
			s = stxt->дайСтиль(p.styleid);
			s.формат.styleid = p.styleid;
		}
		return false;
	}
};

void RichText::MergeStyles(const RichText& text)
{
	инивалидируй();
	MergeStylesOp op;
	op.stxt = &text;
	op.text = this;
	const_cast<RichText&>(text).Update(op);
}

struct RichText::OverrideStylesOp : RichTxt::ParaOp {
	ВекторМап<Uuid, Uuid> cs;

	virtual bool operator()(RichTxt::Para& p) {
		p.styleid = cs.дай(p.styleid, RichStyle::GetDefaultId());
		p.инивалидируй();
		return true;
	}
};

void  RichText::OverrideStyles(const RichStyles& ms, bool matchname, bool addmissing)
{
	инивалидируй();
	RichStyles orig = pick(style);
	style <<= ms;
	OverrideStylesOp op;
	for(int i = 0; i < orig.дайСчёт(); i++) {
		Uuid pid = orig.дайКлюч(i);
		int q;
		if(ms.найди(pid) >= 0)
			op.cs.добавь(pid, pid);
		else
		if(matchname && (q = FindStyleWithName(style, orig[i].имя)) >= 0)
			op.cs.добавь(pid, style.дайКлюч(q));
		else
		if(addmissing) {
			устСтиль(pid, orig[i]);
			op.cs.добавь(pid, pid);
		}
	}

	Update(op);
}

}
