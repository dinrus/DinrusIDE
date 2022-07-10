#include "GridCtrl.h"

namespace РНЦП {

int GridCtrl::ItemRect::sortCol;
int GridCtrl::ItemRect::sortMode;

/*---------------------------------------------------*/

void GridCtrl::Элемент::устКтрл(Ктрл& c, bool owned)
{
	ctrl = &c;
	ctrl->скрой();
	ctrl->устФрейм(фреймПусто());
	ctrl->WantFocus();
	ctrl_flag = IC_INIT | IC_MANUAL;
	if(owned)
		ctrl_flag |= IC_OWNED;
}

void GridCtrl::Элемент::ClearCtrl()
{
	ctrl = NULL;
}

void GridCtrl::Элемент::устДисплей(GridDisplay& gd)
{
	дисплей = &gd;
}

void GridCtrl::Элемент::NoDisplay()
{
	дисплей = NULL;
}

ВекторМап<Ид, int> *GridCtrl::ItemRect::aliases = NULL;

/*---------------------------------------------------*/

GridCtrl::ItemRect& GridCtrl::ItemRect::Alias(Ид _id)
{
	aliases->устКлюч(id, _id);
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Alias(const char * s)
{
	aliases->устКлюч(id, s);
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Имя(Ткст &s)
{
	(*items)[0][id].val = s;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Hidden(bool b)
{
	hidden = b;
	if(hidden)
		size = 0;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Имя(const char * s)
{
	(*items)[0][id].val = s;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Размер(int n, bool hv)
{
	hidden = n == 0;
	
	if(!hidden)
	{
		n = hv ? Ктрл::HorzLayoutZoom(n) : Ктрл::VertLayoutZoom(n);
		if(n < min) { n = min; ismin = true; }
		if(n > max) { n = max; ismax = true; }
	}

	size = nsize = n;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::устШирину(int n)
{
	return Размер(n, true);
}

GridCtrl::ItemRect& GridCtrl::ItemRect::устВысоту(int n)
{
	return Размер(n, false);
}

GridCtrl::ItemRect& GridCtrl::ItemRect::мин(int n)
{
	if(hidden)
		return *this;
	min = n;
	if(size < min)
		size = min;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::макс(int n)
{
	if(hidden)
		return *this;
	max = n;
	if(size > max)
		size = max;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::фиксирован(int n)
{
	n = Ктрл::VertLayoutZoom(n);
	return мин(n).макс(n);
}

GridCtrl::ItemRect& GridCtrl::ItemRect::FixedAuto()
{
	int n = дайРазмТекста((Ткст)(*items)[0][id].val, StdFont()).cx + 15;
	return мин(n).макс(n);
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Edit(Ктрл &ctrl, bool b)
{
	if(b)
	{
		ctrl.скрой();
		ctrl.устФрейм(фреймПусто());
		(*edits)[id].ctrl = &ctrl;
		parent->holder.добавьОтпрыск(&ctrl);
	}
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::EditInsert(bool b /* = true*/)
{
	edit_insert = b;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::EditUpdate(bool b /* = true*/)
{
	edit_update = b;
	return *this;
}

void GridCtrl::ItemRect::ChangeSortMode(bool idsort)
{
	if(++sortmode > 2) sortmode = (int) !idsort;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::SetConvert(Преобр &c)
{
	(*edits)[id].convert = &c;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::NoConvert()
{
	(*edits)[id].convert = NULL;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::NoConvertion()
{
	convertion = false;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::дефолт(Значение v)
{
	defval = v;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Индекс(bool b)
{
	Размер(0);
	Индекс = b;
	return *this;
}

Ткст GridCtrl::ItemRect::дайИмя() const
{
	return (*items)[0][id].val;
}

Ид GridCtrl::ItemRect::GetAlias() const
{
	return aliases->дайКлюч(id);
}

GridCtrl::ItemRect& GridCtrl::ItemRect::устФормат(const char *fmt)
{
	//(*items)[0][id].convert = &c;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::устДисплей(GridDisplay &gd)
{
	дисплей = &gd;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::IgnoreDisplay()
{
	ignore_display = true;
	return *this;
}

void GridCtrl::ItemRect::очисть()
{
	fnt = StdFont();
	bg = Null;
	fg = Null;
}

void GridCtrl::ItemRect::сериализуй(Поток &s)
{
	s % balign % halign % calign;
	s % fg % bg;
	s % fnt;
	s % id;
	s % size;
	s % prop;
	s % hidden;
}

static void MakeOption(Один<Ктрл>& ctrl)
{
	ctrl.создай<Опция>().ShowLabel(false);
	ctrl->устДанные(0);
	ctrl->WantFocus();
}

static void MakeThreeStateOption(Один<Ктрл>& ctrl)
{
	ctrl.создай<Опция>().ThreeState().ShowLabel(false);
	ctrl->устДанные(0);
	ctrl->WantFocus();
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Ctrls(Обрвыз1<Один<Ктрл>&> _factory)
{
	if(!(*edits)[id].factory)
		++parent->genr_ctrls;
	(*edits)[id].factory = _factory;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::NoCtrls()
{
	(*edits)[id].factory.очисть();
	--parent->genr_ctrls;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Опция()
{
	return Ctrls(MakeOption).CtrlAlignHorzPos().CtrlAlignVertPos();
}

GridCtrl::ItemRect& GridCtrl::ItemRect::ThreeStateOption()
{
	return Ctrls(MakeThreeStateOption).CtrlAlignHorzPos().CtrlAlignVertPos();
}

GridCtrl::Элемент& GridCtrl::Элемент::Editable(bool b)
{
	editable = b;
	return *this;
}

GridCtrl::Элемент& GridCtrl::Элемент::NoEditable()
{
	return Editable(false);
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Editable(bool b)
{
	editable = b;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::NoEditable()
{
	return Editable(false);
}

Значение GridCtrl::ItemRect::ExtractValue(int r, int c) const
{
	const Значение& v = (*items)[r][c].val;
	return IsType<AttrText>(v) ? Значение(ValueTo<AttrText>(v).text) : v;
}

}
