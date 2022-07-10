#include "CtrlLib.h"

namespace РНЦП {

#define LTIMING(x)  // DTIMING(x)

КтрлМассив::Колонка::Колонка() {
	convert = NULL;
	edit = NULL;
	дисплей = &StdDisplay();
	accel = NULL;
	margin = -1;
	cached = false;
	clickedit = true;
	Индекс = -1;
	order = NULL;
}

КтрлМассив::Колонка& КтрлМассив::Колонка::Кэш() {
	cached = true;
	return *this;
}

КтрлМассив::Колонка& КтрлМассив::Колонка::NoEdit()
{
	if(edit)
		edit->удали();
	edit = NULL;
	return *this;
}

КтрлМассив::Колонка& КтрлМассив::Колонка::Edit(Ктрл& e) {
	e.скрой();
	e.устФрейм(фреймПусто());
	edit = &e;
	arrayctrl->добавьОтпрыск(edit);
	return *this;
}

КтрлМассив::Колонка& КтрлМассив::Колонка::SetConvert(const Преобр& c) {
	convert = &c;
	очистьКэш();
	arrayctrl->освежи();
	arrayctrl->синхИнфо();
	return *this;
}

КтрлМассив::Колонка& КтрлМассив::Колонка::ConvertBy(Функция<Значение(const Значение&)> cv)
{
	convertby = cv;
	очистьКэш();
	arrayctrl->освежи();
	arrayctrl->синхИнфо();
	return *this;
}

КтрлМассив::Колонка& КтрлМассив::Колонка::устФормат(const char *fmt)
{
	ПреобрФормата::устФормат(fmt);
	return SetConvert(*this);
}

КтрлМассив::Колонка& КтрлМассив::Колонка::устДисплей(const Дисплей& d)
{
	дисплей = &d;
	очистьКэш();
	arrayctrl->SyncCtrls();
	arrayctrl->освежи();
	arrayctrl->синхИнфо();
	return *this;
}

КтрлМассив::Колонка& КтрлМассив::Колонка::Ctrls(Событие<int, Один<Ктрл>&> factory)
{
	return WithLined(factory);
}

КтрлМассив::Колонка& КтрлМассив::Колонка::WithLined(Событие<int, Один<Ктрл>&> f)
{
	factory = f;
	arrayctrl->hasctrls = arrayctrl->headerctrls = true;
	arrayctrl->SyncCtrls();
	arrayctrl->освежи();
	arrayctrl->синхИнфо();
	return *this;
}

КтрлМассив::Колонка& КтрлМассив::Колонка::With(Событие<Один<Ктрл>&> factory)
{
	return WithLined([=](int, Один<Ктрл>& x) { factory(x); });
}

КтрлМассив::Колонка& КтрлМассив::Колонка::Ctrls(Обрвыз1<Один<Ктрл>&> _factory)
{
	return Ctrls([=](int, Один<Ктрл>& x) { _factory(x); });
}

void КтрлМассив::Колонка::очистьКэш() {
	cache.очисть();
}

void КтрлМассив::Колонка::Sorts()
{
	HeaderTab().WhenAction = callback1(arrayctrl, &КтрлМассив::ToggleSortColumn, Индекс);
}

КтрлМассив::Колонка& КтрлМассив::Колонка::Sorting(const ПорядокЗнач& o)
{
	order = &o;
	сравни.очисть();
	line_order.очисть();
	Sorts();
	return *this;
}

КтрлМассив::Колонка& КтрлМассив::Колонка::SortingBy(Функция<int (const Значение& a, const Значение& b)> c)
{
	сравни = c;
	order = NULL;
	line_order.очисть();
	Sorts();
	return *this;
}

КтрлМассив::Колонка& КтрлМассив::Колонка::SortingLined(Врата<int, int> aorder)
{
	line_order = aorder;
	order = NULL;
	сравни.очисть();
	Sorts();
	return *this;
}

КтрлМассив::Колонка& КтрлМассив::Колонка::Sorting()
{
	return SortingBy(сравниСтдЗнач);
}

КтрлМассив::Колонка& КтрлМассив::Колонка::SortDefault(bool desc)
{
	if(!сравни && !order && !line_order)
		Sorting();
	arrayctrl->SetSortColumn(Индекс, desc);
	return *this;
}

void КтрлМассив::Колонка::инвалидируйКэш(int i) {
	if(i < 0) return;
	if(cache.является< Вектор<Ткст> >() && i < cache.дай< Вектор<Ткст> >().дайСчёт())
		cache.дай< Вектор<Ткст> >()[i] = Ткст::дайПроц();
	if(cache.является< Вектор<Значение> >() && i < cache.дай< Вектор<Значение> >().дайСчёт())
		cache.дай< Вектор<Значение> >()[i] = Значение();
}

void КтрлМассив::Колонка::вставьКэш(int i, int count) {
	if(i < 0) return;
	if(cache.является< Вектор<Ткст> >() && i < cache.дай< Вектор<Ткст> >().дайСчёт()) {
		Вектор<Ткст>& v = cache.дай< Вектор<Ткст> >();
		v.вставьН(i, count);
		while(count--)
			v[i++] = Ткст::дайПроц();
	}
	if(cache.является< Вектор<Значение> >() && i < cache.дай< Вектор<Значение> >().дайСчёт())
		cache.дай< Вектор<Значение> >().вставь(i);
}

void КтрлМассив::Колонка::удалиКэш(int i) {
	if(i < 0) return;
	if(cache.является< Вектор<Ткст> >() && i < cache.дай< Вектор<Ткст> >().дайСчёт())
		cache.дай< Вектор<Ткст> >().удали(i);
	if(cache.является< Вектор<Значение> >() && i < cache.дай< Вектор<Значение> >().дайСчёт())
		cache.дай< Вектор<Значение> >().удали(i);
}

КтрлМассив::Колонка& КтрлМассив::Колонка::вставьЗнач(const Значение& v) {
	arrayctrl->IndexInfo(arrayctrl->Поз(pos[0])).вставьЗнач(v);
	return *this;
}

КтрлМассив::Колонка& КтрлМассив::Колонка::вставьЗнач(ГенЗначения& g) {
	arrayctrl->IndexInfo(arrayctrl->Поз(pos[0])).вставьЗнач(g);
	return *this;
}

HeaderCtrl::Колонка& КтрлМассив::Колонка::HeaderTab() {
	return arrayctrl->header.Вкладка(arrayctrl->header.найдиИндекс(Индекс));
}

const HeaderCtrl::Колонка& КтрлМассив::Колонка::HeaderTab() const
{
	return arrayctrl->header.Вкладка(arrayctrl->header.найдиИндекс(Индекс));
}

void КтрлМассив::инвалидируйКэш(int ri)
{
	for(int i = 0; i < column.дайСчёт(); i++)
		column[i].инвалидируйКэш(ri);
}

void КтрлМассив::КтрлЯчейка::леваяВнизу(Точка, dword)
{
	if(ctrl->IsWantFocus())
		ctrl->устФокус();
}

void КтрлМассив::ИнфОЯчейке::освободи()
{
	if(ptr.дайБит()) {
		КтрлЯчейка *cc = (КтрлЯчейка *)ptr.дайУк();
		if(cc->owned)
			delete cc->ctrl;
		delete cc;
	}
}

void КтрлМассив::ИнфОЯчейке::уст(Ктрл *ctrl, bool owned, bool значение)
{
	освободи();
	КтрлЯчейка *cc = new КтрлЯчейка;
	cc->ctrl = ctrl;
	cc->добавь(*ctrl);
	cc->owned = owned;
	cc->значение = значение;
	ptr.уст1(cc);
}

КтрлМассив::ИнфОЯчейке::ИнфОЯчейке(ИнфОЯчейке&& s)
{
	ptr = s.ptr;
	const_cast<ИнфОЯчейке&>(s).ptr.устУк(NULL);
	const_cast<ИнфОЯчейке&>(s).ptr.устБит(0);
}

КтрлМассив::ИнфОЯчейке::~ИнфОЯчейке()
{
	освободи();
}

Ктрл& КтрлМассив::устКтрл(int i, int j, Ктрл *newctrl, bool owned, bool значение)
{
	if(значение)
		newctrl->устДанные(дайКолонку(i, j));
	hasctrls = true;
	RefreshRow(i);
	ИнфОЯчейке& ci = cellinfo.по(i).по(j);
	ci.уст(newctrl, owned, значение);
	Ктрл& c = ci.дайКтрл();
	if(newctrl->дайПоз().x.дайЛин() == LEFT && newctrl->дайПоз().x.дайС() == 0)
		newctrl->HSizePos().VCenterPos(STDSIZE);
	j--;
	for(;;) {
		while(j >= 0) {
			if(ктрл_ли(i, j)) {
				добавьОтпрыск(&c, &дайКтрлЯчейка(i, j));
				синхИнфо();
				return c;
			}
			j--;
		}
		i--;
		if(i < 0) {
			добавьОтпрыск(&c, NULL);
			синхИнфо();
			return c;
		}
		j = cellinfo[i].дайСчёт();
	}
	синхИнфо();
}

void  КтрлМассив::устКтрл(int i, int j, Ктрл& ctrl, bool значение)
{
	устКтрл(i, j, &ctrl, false, значение);
	SyncLineCtrls(i);
}

Ктрл *КтрлМассив::дайКтрл(int i, int col)
{
	SyncLineCtrls(i);
	if(ктрл_ли(i, col))
		return дайКтрлЯчейка(i, col).ctrl;
	return NULL;
}

bool  КтрлМассив::ктрл_ли(int i, int j) const
{
	return i < cellinfo.дайСчёт() && j < cellinfo[i].дайСчёт() && cellinfo[i][j].ктрл_ли();
}

КтрлМассив::КтрлЯчейка& КтрлМассив::дайКтрлЯчейка(int i, int j)
{
	LTIMING("дайКтрлЯчейка");
	return cellinfo[i][j].дайКтрл();
}

const КтрлМассив::КтрлЯчейка& КтрлМассив::дайКтрлЯчейка(int i, int j) const
{
	return cellinfo[i][j].дайКтрл();
}

void КтрлМассив::устДисплей(int i, int j, const Дисплей& d)
{
	cellinfo.по(i).по(j).уст(d);
	RefreshRow(i);
}

void КтрлМассив::SetRowDisplay(int i, const Дисплей& d)
{
	if(i >= 0 && i < дайСчёт())
		for (int j = 0 ; j < дайСчётКолонок(); j++)
			this->cellinfo.по(i).по(j).уст(d);
	RefreshRow(i);
}

void КтрлМассив::SetColumnDisplay(int j, const Дисплей& d)
{
	if(j >= 0 && j < дайСчётКолонок())
		for (int i = 0 ; i < дайСчёт(); i++) {
			this->cellinfo.по(i).по(j).уст(d);
			RefreshRow(i);
		}
}

const Дисплей& КтрлМассив::дайДисплей(int i, int j)
{
	if(i < cellinfo.дайСчёт() && j < cellinfo[i].дайСчёт() && cellinfo[i][j].дисплей_ли())
		return cellinfo[i][j].дайДисплей();
	return *column[j].дисплей;
}

const Дисплей& КтрлМассив::дайДисплей(int j)
{
	return *column[j].дисплей;
}

int КтрлМассив::Поз(int pos) const {
	if(pos >= 0) return pos;
	pos = idx.найди(id_ndx[-pos]);
	ПРОВЕРЬ(pos >= 0);
	return pos;
}

Значение КтрлМассив::Get0(int i, int ii) const {
	ПРОВЕРЬ(ii >= 0);
	if(hasctrls)
		for(int j = 0; j < column.дайСчёт(); j++)
			if(ктрл_ли(i, j)) {
				const Колонка& m = column[j];
				ПРОВЕРЬ(m.pos.дайСчёт() == 1);
				if(Поз(m.pos[0]) == ii) {
					const КтрлЯчейка& c = дайКтрлЯчейка(i, j);
					if(c.значение)
						return c.ctrl->дайДанные();
				}
			}
	if(i < array.дайСчёт()) {
		const Вектор<Значение>& v = array[i].line;
		if(ii < v.дайСчёт())
			return v[ii];
	}
	return Null;
}

Значение КтрлМассив::дай(int ii) const {
	ПРОВЕРЬ(курсор_ли());
	int i;
	for(i = 0; i < column.дайСчёт(); i++) {
		const Колонка& m = column[i];
		if(m.edit)
			for(int j = 0; j < m.pos.дайСчёт(); j++)
				if(Поз(m.pos[j]) == ii) {
					Значение v = m.edit->дайДанные();
					return m.pos.дайСчёт() > 1 && массивЗнач_ли(v) ? МассивЗнач(v)[j] : v;
				}
	}
	for(i = 0; i < control.дайСчёт(); i++) {
		const Контрол& c = control[i];
		if(Поз(c.pos) == ii)
			return c.ctrl->дайДанные();
	}
	return Get0(cursor, ii);
}

Значение КтрлМассив::дай(const Ид& id) const {
	return дай(дайПоз(id));
}

Значение КтрлМассив::дайОригинал(int ii) const {
	return Get0(cursor, ii);
}

Значение КтрлМассив::дайОригинал(const Ид& id) const {
	return дайОригинал(дайПоз(id));
}

Значение КтрлМассив::дайКлюч() const {
	return курсор_ли() ? дай(0) : Null;
}

Значение КтрлМассив::дайОригиналКлюч() const {
	return курсор_ли() ? дайОригинал(0) : Null;
}

bool КтрлМассив::изменено(int ii) const {
	ПРОВЕРЬ(курсор_ли());
	int i;
	if(ii < modify.дайСчёт() && modify[ii]) return true;
	for(i = 0; i < column.дайСчёт(); i++) {
		const Колонка& m = column[i];
		if(m.edit)
			for(int j = 0; j < m.pos.дайСчёт(); j++)
				if(Поз(m.pos[j]) == ii && m.edit->изменено())
					return true;
	}
	for(i = 0; i < control.дайСчёт(); i++) {
		const Контрол& c = control[i];
		if(Поз(c.pos) == ii && c.ctrl->изменено())
			return true;
	}
	return false;
}

bool КтрлМассив::изменено(const Ид& id) const {
	return изменено(дайПоз(id));
}

void КтрлМассив::ColEditSetData(int j) {
	ПРОВЕРЬ(курсор_ли());
	Колонка& m = column[j];
	if(m.edit)
		m.edit->устДанные(дайКолонку(cursor, j));
}

void КтрлМассив::устДанныеКтрл(int j) {
	ПРОВЕРЬ(курсор_ли());
	Контрол& c = control[j];
	c.ctrl->вкл();
	if(пусто_ли(c.pos))
		c.ctrl->устДанные(cursor);
	else
		c.ctrl->устДанные(дайОригинал(Поз(c.pos)));
}

void КтрлМассив::устЗначениеКтрл(int i, int ii, const Значение& v)
{
	if(hasctrls)
		for(int j = 0; j < column.дайСчёт(); j++)
			if(ктрл_ли(i, j)) {
				const Колонка& m = column[j];
				ПРОВЕРЬ(m.pos.дайСчёт() == 1);
				if(Поз(m.pos[0]) == ii) {
					const КтрлЯчейка& c = дайКтрлЯчейка(i, j);
					if(c.значение)
						c.ctrl->устДанные(v);
					return;
				}
			}
}

void КтрлМассив::уст(int ii, const Значение& v) {
	ПРОВЕРЬ(курсор_ли());
	array.по(cursor).line.по(ii) = v;
	int i;
	for(i = 0; i < column.дайСчёт(); i++) {
		Колонка& m = column[i];
		if(m.edit)
			for(int j = 0; j < m.pos.дайСчёт(); j++)
				if(Поз(m.pos[j]) == ii) {
					ColEditSetData(i);
					break;
				}
	}
	for(i = 0; i < control.дайСчёт(); i++) {
		const Контрол& c = control[i];
		if(Поз(c.pos) == ii)
			устДанныеКтрл(i);
	}
	устЗначениеКтрл(cursor, ii, v);
	modify.по(ii, false) = true;
	инвалидируйКэш(cursor);
	RefreshRow(cursor);
	WhenArrayAction();
}

void КтрлМассив::уст(const Ид& id, const Значение& v) {
	уст(дайПоз(id), v);
}

void КтрлМассив::уст0(int i, int ii, const Значение& v) {
	if(i == cursor)
		уст(ii, v);
	else
		устЗначениеКтрл(i, ii, v);
	array.по(i).line.по(ii) = v;
}

void  КтрлМассив::уст(int i, const Вектор<Значение>& v) {
	array.по(i).line.очисть();
	for(int ii = 0; ii < v.дайСчёт(); ii++)
		уст0(i, ii, v[ii]);
	AfterSet(i);
	RefreshRow(i);
	WhenArrayAction();
}

void  КтрлМассив::уст(int i, Вектор<Значение>&& v) {
	Вектор<Значение>& line = array.по(i).line;
	if(hasctrls || i == cursor) {
		line.очисть();
		for(int ii = 0; ii < v.дайСчёт(); ii++)
			уст0(i, ii, v[ii]);
	}
	else
		line = pick(v);
	AfterSet(i);
	RefreshRow(i);
	WhenArrayAction();
}

void КтрлМассив::AfterSet(int i, bool sync_ctrls)
{
	SetSb();
	освежи();
	синхИнфо();
	if(sync_ctrls)
		SyncLineCtrls(i);
	инвалидируйКэш(i);
}

void КтрлМассив::уст(int i, int ii, const Значение& v)
{
	уст0(i, ii, v);
	bool sync_ctrls = false;
	for(int col : найдиКолонкиСПоз(ii))
		if(ктрл_ли(i, col))
			sync_ctrls = true;
	AfterSet(i, sync_ctrls);
	WhenArrayAction();
}

void КтрлМассив::уст(int i, const Ид& id, const Значение& v) {
	уст(i, дайПоз(id), v);
}

Значение КтрлМассив::дай(int i, int ii) const {
	return i == cursor ? дай(ii) : Get0(i, ii);
}

Значение КтрлМассив::дай(int i, const Ид& id) const {
	return дай(i, дайПоз(id));
}

void  КтрлМассив::устСчёт(int n) {
	ПРОВЕРЬ(n >= 0);
	if(cursor >= n)
		CancelCursor();
	array.устСчёт(n);
	if(cellinfo.дайСчёт() > array.дайСчёт())
		cellinfo.устСчёт(array.дайСчёт());
	освежи();
	синхИнфо();
	SyncCtrls();
	SetSb();
	PlaceEdits();
	WhenArrayAction();
}

Значение КтрлМассив::дайКолонку(int row, int col) const {
	const Колонка& c = column[col];
	if(c.pos.дайСчёт() == 0) return row;
	if(c.pos.дайСчёт() == 1) return Get0(row, Поз(c.pos[0]));
	МапЗнач vm;
	for(int i = 0; i < c.pos.дайСчёт(); i++) {
		int ii = Поз(c.pos[i]);
		vm.добавь(ii < idx.дайСчёт() ? idx.дайКлюч(ii) : Ид(), Get0(row, ii));
	}
	return vm;
}

Значение КтрлМассив::GetConvertedColumn(int i, int col) const {
	LTIMING("GetConvertedColumn");
	const Колонка& m = column[col];
	Значение v = дайКолонку(i, col);
	if(!m.convert && !m.convertby) return v;
	if(m.cache.является< Вектор<Ткст> >() && i < m.cache.дай< Вектор<Ткст> >().дайСчёт()) {
		const Ткст& s = m.cache.дай< Вектор<Ткст> >()[i];
		if(!s.проц_ли()) return s;
	}
	if(m.cache.является< Вектор<Значение> >() && i < m.cache.дай< Вектор<Значение> >().дайСчёт()) {
		const Значение& v = m.cache.дай< Вектор<Значение> >()[i];
		if(!v.проц_ли()) return v;
	}
	if(m.convertby)
		v = m.convertby(v);
	if(m.convert)
		v = m.convert->фмт(v);
	if(m.cached) {
		if(m.cache.пустой())
			m.cache.создай< Вектор<Ткст> >();
		if(ткст_ли(v) && m.cache.является< Вектор<Ткст> >())
			m.cache.дай< Вектор<Ткст> >().по(i, Ткст::дайПроц()) = v;
		if(!ткст_ли(v) && m.cache.является< Вектор<Ткст> >())
			m.cache.создай< Вектор<Значение> >();
		if(m.cache.является< Вектор<Значение> >())
			m.cache.дай< Вектор<Значение> >().по(i) = v;
		ПРОВЕРЬ(m.pos.дайСчёт() || m.cache.пустой());
	}
	return v;
}

int КтрлМассив::дайСчёт() const {
	return max(virtualcount, array.дайСчёт());
}

void КтрлМассив::SetVirtualCount(int c) {
	virtualcount = c;
	освежи();
	синхИнфо();
	SetSb();
	if(cursor >= virtualcount)
		идиВКон();
}

void КтрлМассив::SetSb() {
	sb.устВсего(GetTotalCy() + IsInserting() * (GetLineCy() + horzgrid));
	sb.устСтраницу(дайРазм().cy);
	MinMaxLine();
}

void КтрлМассив::Выкладка() {
	SyncColumnsPos();
	SetSb();
	HeaderScrollVisibility();
	PlaceEdits();
	SyncCtrls();
}

void КтрлМассив::Reline(int i, int y)
{
	int i0 = i;
	if(WhenLineVisible)
		while(i < ln.дайСчёт()) {
			Ln& p = ln[i];
			p.y = y;
			if(строкаВидима(i))
				y += Nvl(p.cy, linecy) + horzgrid;
			i++;
		}
	else {
		while(i < ln.дайСчёт()) {
			Ln& p = ln[i];
			p.y = y;
			if(IsLineVisible0(i))
				y += Nvl(p.cy, linecy) + horzgrid;
			i++;
		}
	}
	SetSb();
	освежи();
	SyncCtrls(i0);
}

int  КтрлМассив::GetLineY(int i) const
{
	return i < ln.дайСчёт() ? ln[i].y
	                         : (ln.дайСчёт() ? ln.верх().y + строкаВидима(ln.дайСчёт() - 1) * (Nvl(ln.верх().cy, linecy) + horzgrid) : 0)
	                           + (linecy + horzgrid) * (i - ln.дайСчёт());
}

Прям КтрлМассив::GetScreenCellRect(int i, int col) const
{
	return GetCellRect(i, col).смещенец(GetScreenView().верхЛево());
}

Прям КтрлМассив::GetScreenCellRectM(int i, int col) const
{
	return GetCellRectM(i, col).смещенец(GetScreenView().верхЛево());
}

КтрлМассив& КтрлМассив::SetLineCy(int cy)
{
	linecy = cy;
	Reline(0, 0);
	sb.устСтроку(cy);
	освежи();
	return *this;
}

void КтрлМассив::SetLineCy(int i, int cy)
{
	int q = ln.дайСчёт();
	if(i < q) {
		ln[i].cy = cy;
		Reline(i, ln[i].y);
	}
	else {
		while(ln.дайСчёт() <= i)
			ln.добавь().cy = Null;
		ln[i].cy = cy;
		if(q > 0)
			Reline(q - 1, ln[q - 1].y);
		else
			Reline(0, 0);
	}
}

void КтрлМассив::SetLineColor(int i, Цвет c)
{
	Цвет& lc = array.по(i).paper;
	if(lc != c) {
		lc = c;
		RefreshRow(i);
	}
}

int  КтрлМассив::GetTotalCy() const
{
	int i = дайСчёт();
	return i ? GetLineY(i - 1) + GetLineCy(i - 1) + horzgrid: 0;
}

int  КтрлМассив::GetLineCy(int i) const {
	return i < ln.дайСчёт() ? Nvl(ln[i].cy, linecy) : linecy;
}

int  КтрлМассив::GetLineAt(int y) const {
	LTIMING("GetLineAt");
	if(!дайСчёт()) return Null;
	if(y < 0 || y >= GetTotalCy()) return Null;
	int l = 0;
	int h = дайСчёт();
	while(l < h) {
		int m = (l + h) / 2;
		int yy = GetLineY(m);
		if(yy == y) return m;
		if(yy < y)
			l = m + 1;
		else
			h = m;
	}
	return l > 0 ? l - 1 : l;
}

Ктрл *КтрлМассив::SyncLineCtrls(int i, Ктрл *p)
{
	if(!hasctrls)
		return NULL;
	Размер sz = дайРазм();
	bool visible = строкаВидима(i);
	for(int j = 0; j < column.дайСчёт(); j++) {
		bool ct = ктрл_ли(i, j);
		if(!ct && column[j].factory) {
			LTIMING("создай");
			Один<Ктрл> newctrl;
			column[j].factory(i, newctrl);
			newctrl->устДанные(дайКолонку(i, j));
			newctrl->WhenAction << прокси(WhenCtrlsAction);
			if(newctrl->дайПоз().x.дайЛин() == LEFT && newctrl->дайПоз().x.дайС() == 0)
				newctrl->HSizePos().VCenterPos(STDSIZE);
			ИнфОЯчейке& ci = cellinfo.по(i).по(j);
			ci.уст(newctrl.открепи(), true, true);
			ct = true;
		}
		if(ct) {
			LTIMING("PlaceCtrls");
			Ктрл& c = дайКтрлЯчейка(i, j);
			if(!c.HasFocusDeep() || c.дайРодителя() != this) {
				for(int ii = i - 1; ii >= 0 && !p; ii--) // find previous Ктрл for the correct order
					for(int j = column.дайСчёт() - 1; j >= 0; j--) {
						if(ктрл_ли(ii, j)) {
							p = &дайКтрлЯчейка(ii, j);
							break;
						}
						if(column[j].factory) { // was not created yet...
							for(int q = 0; q <= i; q++)
								p = SyncLineCtrls(q, p);
							return p;
						}
					}
				добавьОтпрыск(&c, p);
			}
			p = &c;
			Прям r;
			if(i < min_visible_line || i > max_visible_line || !visible)
				r.bottom = r.top = -1;
			else
				r = GetCellRectM(i, j);
			if(r.bottom < 0 || r.top > sz.cy) {
				if(c.дайПрям().top != -100000)
					c.устПрям(-1000, -100000, 1, 1);
			}
			else {
				c.устПрям(r);
				ctrl_low = min(ctrl_low, i);
				ctrl_high = max(ctrl_high, i);
			}
		}
	}
	return p;
}

void  КтрлМассив::SyncPageCtrls()
{
	LTIMING("SyncPageCtrls");
	if(!hasctrls)
		return;
	Ктрл *p = NULL;
	for(int i = max(ctrl_low, 0); i <= min(ctrl_high, дайСчёт() - 1); i++)
		p = SyncLineCtrls(i, p);
	ctrl_low = дайСчёт() - 1;
	ctrl_high = 0;
	p = NULL;
	for(int i = min_visible_line; i <= min(max_visible_line, дайСчёт() - 1); i++)
		p = SyncLineCtrls(i, p);
}

void  КтрлМассив::SyncCtrls(int i0)
{
	LTIMING("SyncCtrls");
	if(!hasctrls)
		return;
	ctrl_low = дайСчёт() - 1;
	ctrl_high = 0;
	Ктрл *p = NULL;
	for(int i = i0; i < дайСчёт(); i++)
		p = SyncLineCtrls(i, p);
}

Точка КтрлМассив::FindCellCtrl(Ктрл *ctrl)
{
	if(hasctrls)
		for(int i = 0; i < cellinfo.дайСчёт(); i++) {
			Вектор<ИнфОЯчейке>& ci = cellinfo[i];
			for(int j = 0; j < ci.дайСчёт(); j++)
				if(ктрл_ли(i, j)) {
					КтрлЯчейка& c = дайКтрлЯчейка(i, j);
					if(&c == ctrl || c.ctrl == ctrl || c.ctrl->HasChildDeep(ctrl))
						return Точка(j, i);
				}
		}
	return Null;
}

void КтрлМассив::отпрыскФок()
{
	if(cursor >= 0)
		RefreshRow(cursor);
	if(!acceptingrow) {
		Точка p = FindCellCtrl(GetFocusCtrl());
		if(!пусто_ли(p)) {
			if(nocursor)
				промотайДо(p.y);
			else
			if(p.y != cursor) // avoid setting cursor if it is already there - important for multiselect
				устКурсор(p.y);
		}
	}
	Ктрл::отпрыскФок();
}

void КтрлМассив::отпрыскРасфок()
{
	if(cursor >= 0)
		RefreshRow(cursor);
	Ктрл::отпрыскРасфок();
}

const Дисплей& КтрлМассив::GetCellInfo(int i, int j, bool f0,
                                      Значение& v, Цвет& fg, Цвет& bg, dword& st)
{
	st = 0;
	bool hasfocus = f0 && !isdrag;
	bool drop = i == dropline && (dropcol == DND_LINE || dropcol == j);
	if(толькочтен_ли())
		st |= Дисплей::READONLY;
	if(i < array.дайСчёт() && array[i].select)
		st |= Дисплей::SELECT;
	if(i == cursor && !nocursor)
		st |= Дисплей::CURSOR;
	if(drop) {
		hasfocus = true;
		st |= Дисплей::CURSOR;
	}
	if(hasfocus)
		st |= Дисплей::FOCUS;
	bg = i & 1 ? evenpaper : oddpaper;
	if(nobg)
		bg = Null;
	if(i < array.дайСчёт() && !пусто_ли(array[i].paper))
		bg = array[i].paper;
	fg = i & 1 ? evenink : oddink;
	if((st & Дисплей::SELECT) ||
	    !multiselect && (st & Дисплей::CURSOR) && !nocursor ||
	    drop) {
		fg = hasfocus ? SColorHighlightText : SColorText;
		bg = hasfocus ? SColorHighlight : смешай(SColorDisabled, SColorPaper);
	}
	v = ктрл_ли(i, j) && дайКтрлЯчейка(i, j).значение ? Null : GetConvertedColumn(i, j);
	return дайДисплей(i, j);
}

void КтрлМассив::SpanWideCell(const Дисплей& d, const Значение& q, int cm, int& cw, Прям& r, int i, int& j)
{
	int cx = d.дайСтдРазм(q).cx;
	while(cx > r.устШирину() - 2 * cm && j + 1 < column.дайСчёт()) {
		Значение v = GetConvertedColumn(i, j + 1);
		if(!пусто_ли(v))
			break;
		j++;
		cw += header.GetTabWidth(j);
		r.right = r.left + cw - vertgrid + (j == column.дайСчёт() - 1);
	}
}

Размер  КтрлМассив::DoPaint(Draw& w, bool sample) {
	LTIMING("рисуй");
	SyncColumnsPos();
	bool hasfocus0 = HasFocusDeep() || sample;
	Размер size = sample ? StdSampleSize() : дайРазм();
	Прям r;
	r.bottom = 0;
	int i = sample ? 0 : GetLineAt(sb);
	int xs = sample ? 0 : -header.дайПромотку();
	int js;
	for(js = 0; js < column.дайСчёт(); js++) {
		int cw = header.GetTabWidth(js);
		if ((xs + cw - vertgrid + (js == column.дайСчёт() - 1)) >= 0)
			break;
		xs += cw;
	}
	int sy = 0;
	if(!пусто_ли(i))
		while(i < дайСчёт()) {
			if((!sample || i == cursor || i < array.дайСчёт() && array[i].select) && строкаВидима(i)) {
				r.top = sample ? sy : GetLineY(i) - sb;
				if(r.top > size.cy)
					break;
				r.bottom = r.top + GetLineCy(i);
				int x = xs;
				dword st = 0;
				if(r.bottom > r.top)
					for(int j = js; j < column.дайСчёт(); j++) {
						int cw = header.GetTabWidth(j);
						int jj = header.GetTabIndex(j);
						int cm = column[jj].margin;
						if(cm < 0)
							cm = header.Вкладка(j).GetMargin();
						if(x > size.cx) break;
						r.left = x;
						r.right = x + cw - vertgrid + (jj == column.дайСчёт() - 1);
						dword st;
						Цвет fg, bg;
						Значение q;
						const Дисплей& d = GetCellInfo(i, jj, hasfocus0, q, fg, bg, st);
						if(sample || w.IsPainting(r)) {
							if(spanwidecells)
								SpanWideCell(d, q, cm, cw, r, i, j);
							
							if(cw < 2 * cm || editmode && i == cursor && column[jj].edit)
								d.PaintBackground(w, r, q, fg, bg, st);
							else {
								d.PaintBackground(w, RectC(r.left, r.top, cm, r.устВысоту()), q, fg, bg, st);
								r.left += cm;
								r.right -= cm;
								d.PaintBackground(w, RectC(r.right, r.top, cm, r.устВысоту()), q, fg, bg, st);
								w.Clip(r);
								d.рисуй(w, r, q, fg, bg, st);
								w.стоп();
							}
						}
						x += cw;
						if(vertgrid)
							w.DrawRect(x - 1, r.top, 1, r.устВысоту(), gridcolor);
					}
				if(horzgrid)
					w.DrawRect(0, r.bottom, size.cx, 1, gridcolor);
				r.left = 0;
				r.right = x;
				if(i == cursor && !nocursor && multiselect && (дайСчётВыделений() != 1 || !IsSel(i)) && hasfocus0 && !isdrag)
					DrawFocus(w, r, st & Дисплей::SELECT ? SColorPaper() : SColorText());
				r.bottom += horzgrid;
				r.left = x;
				r.right = size.cx;
				if(!nobg)
					w.DrawRect(r, SColorPaper);
				if(i == dropline && dropcol == DND_INSERTLINE)
					DrawHorzDrop(w, 0, r.top - (i > 0), size.cx);
				sy = r.bottom;
			}
			i++;
		}
	if(sample) return Размер(r.left, sy);
	int ldy = r.bottom;
	r.left = 0;
	r.right = size.cx;
	r.top = r.bottom;
	if(IsAppendLine() && !курсор_ли()) {
		r.bottom = r.top + linecy;
		w.DrawRect(r, естьФокус() ? SColorHighlight : SColorFace);
		r.top = r.bottom;
	}
	r.bottom = size.cy;
	if(!nobg)
		w.DrawRect(r, SColorPaper);
	if(дайСчёт() == dropline && dropcol == DND_INSERTLINE)
		DrawHorzDrop(w, 0, ldy - 1, size.cx);
	scroller.уст(Точка(header.дайПромотку(), sb));
	return Размер();
}

void КтрлМассив::рисуй(Draw& w)
{
	DoPaint(w, false);
}

Рисунок КтрлМассив::дайСэиплТяга()
{
	ImageDraw iw(StdSampleSize());
	Размер sz = DoPaint(iw, true);
	return Crop(iw, 0, 0, sz.cx, sz.cy);
}

void КтрлМассив::MinMaxLine()
{
	min_visible_line = Nvl(GetLineAt(sb), 0);
	max_visible_line = Nvl(GetLineAt((int)sb + sb.дайСтраницу()), дайСчёт());
}

void КтрлМассив::промотай() {
	SyncColumnsPos();
	MinMaxLine();
	SyncPageCtrls();
	PlaceEdits();
	scroller.промотай(*this, дайРазм(), Точка(header.дайПромотку(), sb));
	info.Cancel();
	ПриПромоте();
}

void КтрлМассив::SyncColumnsPos()
{
	int x = 0;
	column_pos.очисть();
	column_width.очисть();
	for(int i = 0; i < header.дайСчёт(); i++) {
		int w = header.GetTabWidth(i);
		int ii = header.GetTabIndex(i);
		column_pos.по(ii, 0) = x;
		column_width.по(ii, 0) = w;
		x += w;
	}
}

void КтрлМассив::HeaderLayout() {
	SyncColumnsPos();
	MinMaxLine();
	освежи();
	синхИнфо();
	SyncPageCtrls();
	PlaceEdits();
	WhenHeaderLayout();
}

void КтрлМассив::HeaderScroll()
{
	промотай();
	WhenHeaderLayout();
}

void КтрлМассив::HeaderScrollVisibility()
{
	scrollbox.устВысоту(размПромотБара());
	if(header.IsScroll())
		sb.устФрейм(scrollbox);
	else
		sb.устФрейм(фреймПусто());
}

void КтрлМассив::RefreshRow(int i)
{
	if(i >= 0 && i < дайСчёт())
		освежи(0, GetLineY(i) - sb, дайРазм().cx, GetLineCy(i) + horzgrid);
	if(IsAppendLine() || i == дайСчёт())
		освежи(0, GetLineY(дайСчёт()) - sb, дайРазм().cx, linecy + horzgrid);
	синхИнфо();
}

КтрлМассив::ИнфОбИд& КтрлМассив::IndexInfo(int i) {
	return idx[i];
}

КтрлМассив::ИнфОбИд& КтрлМассив::IndexInfo(const Ид& id) {
	return idx.дай(id);
}

КтрлМассив::ИнфОбИд& КтрлМассив::добавьИндекс(const Ид& id) {
	return idx.добавь(id);
}

КтрлМассив::ИнфОбИд& КтрлМассив::добавьИндекс() {
	return idx.добавь(Ид());
}

КтрлМассив::ИнфОбИд& КтрлМассив::устИд(int i, const Ид& id) {
	while(idx.дайСчёт() < i + 1)
		idx.добавь(Ид());
	idx.устКлюч(i, id);
	return idx[i];
}

КтрлМассив::Колонка& КтрлМассив::добавьКолонку(const char *text, int w) {
	добавьИндекс();
	return добавьКолонкуПо(idx.дайСчёт() - 1, text, w);
}

КтрлМассив::Колонка& КтрлМассив::добавьКолонку(const Ид& id, const char *text, int w) {
	добавьИндекс(id);
	return добавьКолонкуПо(idx.дайСчёт() - 1, text, w);
}

КтрлМассив::Колонка& КтрлМассив::добавьКолонкуПо(int pos, const char *text, int w) {
	header.добавь(text, w);
	Колонка& m = column.добавь();
	m.arrayctrl = this;
	m.Индекс = column.дайСчёт() - 1;
	m.добавь(pos);
	if(allsorting)
		m.Sorting();
	return m;
}

КтрлМассив::Колонка& КтрлМассив::добавьКолонкуПо(const Ид& id, const char *text, int w) {
	header.добавь(text, w);
	Колонка& m = column.добавь();
	m.arrayctrl = this;
	m.Индекс = column.дайСчёт() - 1;
	m.добавь(id);
	if(allsorting)
		m.Sorting();
	return m;
}

КтрлМассив::Колонка& КтрлМассив::AddRowNumColumn(const char *text, int w) {
	header.добавь(text, w);
	Колонка& m = column.добавь();
	m.arrayctrl = this;
	m.Индекс = column.дайСчёт() - 1;
	if(allsorting)
		m.Sorting();
	return m;
}

int КтрлМассив::найдиКолонкуСПоз(int pos) const
{
	for(int i = 0; i < column.дайСчёт(); i++) {
		const Mitor<int>& m = column[i].pos;
		for(int j = 0; j < m.дайСчёт(); j++)
			if(Поз(m[j]) == pos) return i;
	}
	return -1;
}

int КтрлМассив::найдиКолонкуСИд(const Ид& id) const
{
	return найдиКолонкуСПоз(дайПоз(id));
}

Вектор<int> КтрлМассив::найдиКолонкиСПоз(int pos) const
{
	Вектор<int> r;
	for(int i = 0; i < column.дайСчёт(); i++) {
		const Mitor<int>& m = column[i].pos;
		for(int j = 0; j < m.дайСчёт(); j++)
			if(Поз(m[j]) == pos)
				r.добавь(i);
	}
	return r;
}

Вектор<int> КтрлМассив::найдиКолонкиСИд(const Ид& id) const
{
	return найдиКолонкиСПоз(дайПоз(id));
}

КтрлМассив::ИнфОбИд& КтрлМассив::добавьКтрл(Ктрл& ctrl) {
	ИнфОбИд& f = добавьИндекс();
	добавьКтрлПо(idx.дайСчёт() - 1, ctrl);
	return f;
}

КтрлМассив::ИнфОбИд& КтрлМассив::добавьКтрл(const Ид& id, Ктрл& ctrl) {
	ИнфОбИд& f = добавьИндекс(id);
	добавьКтрлПо(idx.дайСчёт() - 1, ctrl);
	return f;
}

void КтрлМассив::добавьКтрлПо(int ii, Ктрл& ctrl) {
	Контрол& c = control.добавь();
	c.pos = ii;
	c.ctrl = &ctrl;
	ctrl.откл();
	ctrl <<= Null;
}

void КтрлМассив::добавьКтрлПо(const Ид& id, Ктрл& ctrl) {
	добавьКтрлПо(-AsNdx(id), ctrl);
}

void КтрлМассив::AddRowNumCtrl(Ктрл& ctrl) {
	добавьКтрлПо(Null, ctrl);
}

Прям КтрлМассив::GetCellRect(int i, int col) const
{
	LTIMING("GetCellRect");
	Прям r;
	r.top = GetLineY(i) - sb;
	r.bottom = r.top + GetLineCy(i);
	r.left = (col < column_pos.дайСчёт() ? column_pos[col] : 0) - header.дайПромотку();
	r.right = r.left + (col < column_width.дайСчёт() ? column_width[col] : 0) - vertgrid +
	          (col == column.дайСчёт() - 1);
	return r;
}

Прям КтрлМассив::GetCellRectM(int i, int col) const
{
	LTIMING("GetCellRectM");
	Прям r = GetCellRect(i, col);
	int cm = column[col].margin;
	if(cm < 0)
		cm = header.Вкладка(header.найдиИндекс(col)).GetMargin();
	r.left += cm;
	r.right -= cm;
	return r;
}

void КтрлМассив::удали0(int i) {
	array.удали(i);
	for(int j = 0; j < column.дайСчёт(); j++)
		column[j].удалиКэш(i);
	if(i < ln.дайСчёт()) {
		int y = ln[i].y;
		ln.удали(i);
		Reline(i, y);
	}
	if(i < cellinfo.дайСчёт())
		cellinfo.удали(i);
	if(virtualcount > 0) virtualcount--;
	освежи();
	PlaceEdits();
	SyncCtrls();
	синхИнфо();
	SetSb();
	selectiondirty = true;
	WhenArrayAction();
}

void КтрлМассив::отклКтрлы() {
	for(int i = 0; i < control.дайСчёт(); i++) {
		Ктрл& c = *control[i].ctrl;
		c <<= Null;
		c.откл();
	}
}

void КтрлМассив::RejectRow() {
	bool rm_cursor = false;
	if(курсор_ли()) {
		int i;
		for(i = 0; i < column.дайСчёт(); i++) {
			Колонка& m = column[i];
			if(m.edit) m.edit->отклони();
		}
		for(i = 0; i < control.дайСчёт(); i++)
			control[i].ctrl->отклони();
		if(IsInsert()) {
			удали0(cursor);
			отклКтрлы();
			cursor = -1;
			rm_cursor = true;
		}
		else
			RefreshRow(cursor);
	}
	EndEdit();
	WhenArrayAction();
	if(rm_cursor) {
		WhenCursor();
		WhenSel();
	}
	синхИнфо();
}

void КтрлМассив::отклони() {
	RejectRow();
}

void КтрлМассив::CancelCursor() {
	RejectRow();
	отклКтрлы();
	cursor = anchor = -1;
	WhenCursor();
	WhenSel();
	синхИнфо();
}

bool КтрлМассив::UpdateRow() {
	ПРОВЕРЬ(курсор_ли());
	bool iv = false;
	int i;
	for(i = 0; i < column.дайСчёт(); i++) {
		Колонка& m = column[i];
		if(m.edit && m.edit->изменено()) {
			Значение v = m.edit->дайДанные();
			if(m.pos.дайСчёт() == 1) {
				Значение& vv = array.по(cursor).line.по(Поз(m.pos[0]));
				if(vv != v) {
					iv = true;
					vv = v;
				}
			}
			else
			if(m.pos.дайСчёт() > 1) {
				МассивЗнач va = v;
				for(int j = 0; j < m.pos.дайСчёт(); j++) {
					Значение& vv = array.по(cursor).line.по(Поз(m.pos[j]));
					if(vv != va[j]) {
						iv = true;
						vv = va[j];
					}
				}
			}
		}
	}
	for(i = 0; i < control.дайСчёт(); i++) {
		Контрол& c = control[i];
		if(!пусто_ли(c.pos) && c.ctrl->изменено()) {
			Значение v = c.ctrl->дайДанные();
			Значение& vv = array.по(cursor).line.по(Поз(c.pos));
			if(vv != v) {
				iv = true;
				vv = v;
			}
		}
	}
	if(iv) {
		RefreshRow(cursor);
		инвалидируйКэш(cursor);
		WhenArrayAction();
	}
	WhenUpdateRow();
	return true;
}

void КтрлМассив::ClearModify() {
	int i;
	modify.уст(0, false, idx.дайСчёт());
	for(i = 0; i < column.дайСчёт(); i++)
		if(column[i].edit)
			column[i].edit->ClearModify();
	for(i = 0; i < control.дайСчёт(); i++)
		control[i].ctrl->ClearModify();
}

bool КтрлМассив::AcceptRow(bool endedit) {
	ПРОВЕРЬ(курсор_ли());
	if(acceptingrow) // prevent recursion
		return true;
	int i;
	for(i = 0; i < column.дайСчёт(); i++) {
		Колонка& m = column[i];
		if(m.edit && !m.edit->прими())
			return false;
		if(ктрл_ли(cursor, i) && !endedit) {
			Ктрл *c =  дайКтрлЯчейка(cursor, i).ctrl;
			acceptingrow++;
			bool b = c->прими();
			acceptingrow--;
			if(!b)
				return false;
		}
	}
	for(i = 0; i < control.дайСчёт(); i++)
		if(!control[i].ctrl->прими())
			return false;
	acceptingrow++;
	bool ar = WhenAcceptRow() && UpdateRow();
	acceptingrow--;
	if(!ar) {
		SetCursorEditFocus();
		return false;
	}
	bool b = editmode;
	EndEdit();
	if(!endedit)
		SetCtrls();
	ClearModify();
	if(b)
		WhenAcceptEdit();
	return true;
}

bool КтрлМассив::прими() {
	return курсор_ли() ? AcceptRow() : true;
}

bool КтрлМассив::KillCursor0() {
	if(курсор_ли()) {
		if(!AcceptRow()) return false;
		int c = cursor;
		отклКтрлы();
		WhenKillCursor();
		cursor = -1;
		RefreshRow(c);
	}
	return true;
}

bool КтрлМассив::анулируйКурсор() {
	bool b = KillCursor0();
	if(b) {
		WhenCursor();
		WhenSel();
		синхИнфо();
	}
	return b;
}

void КтрлМассив::SetCtrls() {
	int i;
	for(i = 0; i < column.дайСчёт(); i++)
		ColEditSetData(i);
	for(i = 0; i < control.дайСчёт(); i++)
		устДанныеКтрл(i);
}

void КтрлМассив::промотайДо(int line)
{
	if(line < 0)
		sb.старт();
	else
	if(line >= дайСчёт())
		sb.стоп();
	else
		sb.промотайДо(GetLineY(line), GetLineCy(line));
}

void КтрлМассив::ScrollIntoCursor()
{
	if(курсор_ли())
		промотайДо(cursor);
}

void КтрлМассив::SetCursorEditFocus()
{
	if(!IsEdit() && cursor >= 0 && hasctrls)
		for(int j = 0; j < column.дайСчёт(); j++)
			if(ктрл_ли(cursor, j) && дайКтрлЯчейка(cursor, j).ctrl->SetWantFocus())
				break;
}

bool КтрлМассив::устКурсор0(int i, bool dosel) {
	if(nocursor || дайСчёт() == 0 || !строкаАктивирована(i))
		return false;
	i = minmax(i, 0, дайСчёт() - 1);
	bool sel = false;
	if(i != cursor) {
		RefreshRow(cursor);
		if(cursor >= 0) {
			if(!KillCursor0()) return false;
		}
		cursor = i;
		ScrollIntoCursor();
		RefreshRow(cursor);
		SetCtrls();
		for(int j = 0; j < column.дайСчёт(); j++)
			if(ктрл_ли(cursor, j) && дайКтрлЯчейка(cursor, j).ctrl->естьФокус())
				goto nosetfocus;
		SetCursorEditFocus();
	nosetfocus:
		ClearModify();
		Action();
		WhenEnterRow();
		WhenCursor();
		sel = true;
	}
	if(dosel && multiselect) {
		очистьВыделение(false);
		anchor = cursor;
		выделиОдин(cursor, true, false);
		sel = true;
	}
	if(sel) {
		selectiondirty = true;
		WhenSelection();
		WhenSel();
	}
	синхИнфо();
	return true;
}

bool КтрлМассив::устКурсор(int i)
{
	return устКурсор0(i);
}

void КтрлМассив::ShowAppendLine() {
	sb.промотайДо(GetTotalCy(), GetLineCy());
}

void КтрлМассив::идиВНач() {
	if(nocursor)
		sb.старт();
	else {
		int q = FindEnabled(0, 1);
		if(q >= 0)
			устКурсор(q);
	}
}

void КтрлМассив::идиВКон() {
	if(nocursor)
		sb.стоп();
	else {
		int q = FindEnabled(дайСчёт() - 1, -1);
		if(q >= 0)
			устКурсор(q);
	}
}

int  КтрлМассив::GetCursorSc() const {
	return курсор_ли() ? GetLineY(cursor) - sb : 0;
}

void КтрлМассив::ScCursor(int a) {
	if(курсор_ли())
		sb = GetLineY(cursor) - max(a, 0);
}

void КтрлМассив::SyncSelection() const
{
	if(!selectiondirty)
		return;
	selectcount = 0;
	for(int i = 0; i < array.дайСчёт(); i++)
		if(array[i].select)
			selectcount++;
	selectiondirty = false;
}

int КтрлМассив::дайСчётВыделений() const
{
	SyncSelection();
	return selectcount;
}

void КтрлМассив::выделиОдин(int i, bool sel, bool raise)
{
	array.по(i).select = sel;
	selectiondirty = true;
	RefreshRow(i);
	if(raise) {
		WhenSelection();
		WhenSel();
	}
	синхИнфо();
}

void КтрлМассив::выдели(int i, int count, bool sel)
{
	while(count--) {
		array.по(i).select = sel;
		RefreshRow(i++);
	}
	selectiondirty = true;
	WhenSelection();
	WhenSel();
	синхИнфо();
}

void КтрлМассив::активируйСтроку(int i, bool en)
{
	array.по(i).enabled = en;
}

bool КтрлМассив::строкаАктивирована(int i) const
{
	bool b = i < 0 ? false : i < array.дайСчёт() ? array[i].enabled : true;
	WhenLineEnabled(i, b);
	return b;
}

void КтрлМассив::покажиСтроку(int i, bool e)
{
	int q = ln.дайСчёт();
	array.по(i).visible = e;
	ln.по(i);
	if(q > 0 && i > 0)
		Reline(i - 1, ln[i - 1].y);
	else
		Reline(0, 0);
}

bool КтрлМассив::строкаВидима(int i) const
{
	bool b = IsLineVisible0(i);
	WhenLineVisible(i, b);
	return b;
}

void КтрлМассив::очистьВыделение(bool raise)
{
	if(выделение_ли()) {
		for(int i = 0; i < array.дайСчёт(); i++)
			if(array[i].select) {
				RefreshRow(i);
				array[i].select = false;
			}
		selectiondirty = false;
		selectcount = 0;
		if(raise) {
			WhenSelection();
			WhenSel();
		}
		синхИнфо();
	}
}

bool КтрлМассив::IsSel(int i) const
{
	return multiselect ? выделен(i) : дайКурсор() == i;
}

Вектор<int> КтрлМассив::GetSelKeys() const
{
	Вектор<int> id;
	for(int i = 0; i < дайСчёт(); i++)
		if(IsSel(i))
			id.добавь(дай(i, 0));
	return id;
}

int  КтрлМассив::дайПромотку() const
{
	return sb;
}

void КтрлМассив::ScrollTo(int sc)
{
	sb = sc;
}

void КтрлМассив::курсорПоЦентру() {
	if(курсор_ли())
		ScCursor((дайРазм().cy - GetLineCy(cursor)) / 2);
}

int КтрлМассив::FindEnabled(int i, int dir)
{
	ПРОВЕРЬ(dir == -1 || dir == 1);
	while(i >= 0 && i < дайСчёт()) {
		if(строкаАктивирована(i))
			return i;
		i += dir;
	}
	return -1;
}

void КтрлМассив::Page(int q) {
	q = q * max(дайРазм().cy - (linecy + horzgrid), linecy + horzgrid);
	int a = GetCursorSc();
	if(курсор_ли()) {
		int i = FindEnabled(cursor + q / (linecy + horzgrid), зн(q));
		if(i >= 0) {
			устКурсор(i);
			ScCursor(a);
		}
	}
	else
	if(q > 0)
		sb.следщСтраница();
	else
		sb.предшСтраница();
}

void КтрлМассив::DoPoint(Точка p, bool dosel) {
	p.y += sb;
	if(p.y >= GetTotalCy() && IsAppendLine()) {
		if(IsMultiSelect())
			очистьВыделение();
		анулируйКурсор();
	}
	clickpos.y = GetLineAt(p.y);
	if(!пусто_ли(clickpos.y))
		устКурсор0(clickpos.y, dosel);
	else
	if(курсор_ли())
		AcceptRow(true); // true not to reenable ctrls
	if(!HasFocusDeep())
		SetWantFocus();
}

int  КтрлМассив::GetClickColumn(int ii, Точка p)
{
	for(int i = 0; i < column.дайСчёт(); i++)
		if(GetCellRect(ii, i).содержит(p))
			return i;
	return Null;
}

void КтрлМассив::ClickColumn(Точка p)
{
	clickpos.x = Null;
	if(clickpos.y >= 0)
		clickpos.x = GetClickColumn(clickpos.y, p);
}

void КтрлМассив::ClickSel(dword flags)
{
	if(cursor >= 0 && multiselect) {
		if(flags & K_CTRL) {
			выделиОдин(cursor, !выделен(cursor));
			anchor = cursor;
		}
		else {
			очистьВыделение(false);
			if((flags & K_SHIFT) && anchor >= 0)
				выдели(min(anchor, cursor), абс(anchor - cursor) + 1, true);
			else {
				anchor = cursor;
				выделиОдин(cursor, true);
			}
		}
		Action();
	}
}

void КтрлМассив::леваяВнизу(Точка p, dword flags)
{
	if(толькочтен_ли()) return;
	int q = header.GetSplit(p.x);
	if(q >= 0) {
		header.StartSplitDrag(q);
		return;
	}
	q = clickpos.y = GetLineAt(p.y + sb);
	ClickColumn(p);
	selclick = false;
	if(q >= 0 && q < дайСчёт() && IsSel(q) && (flags & (K_CTRL|K_SHIFT)) == 0) {
		selclick = true;
		return;
	}
	кликни(p, flags);
}

void КтрлМассив::кликни(Точка p, dword flags) {
	int c = cursor;
	bool b = естьФокус();
	DoPoint(p, !(flags & (K_CTRL|K_SHIFT)));
	ClickColumn(p);
	if(!пусто_ли(clickpos.x) && c == cursor && cursor >= 0 && b && column[clickpos.x].clickedit
	   && (flags & (K_CTRL|K_SHIFT)) == 0)
		StartEdit(clickpos.x);
	else
		ClickSel(flags);
	ПриЛевКлике();
}

void КтрлМассив::леваяВверху(Точка p, dword flags)
{
	if(selclick)
		кликни(p, flags);
	selclick = false;
}

void КтрлМассив::леваяДКлик(Точка p, dword flags)
{
	if(толькочтен_ли()) return;
	DoPoint(p, !(flags & (K_CTRL|K_SHIFT)));
	ClickColumn(p);
	ClickSel(flags);
	if((IsInserting() || IsAppending()) && IsAppendLine() && !курсор_ли())
		DoAppend();
	if(!multiselect || (flags & (K_CTRL|K_SHIFT)) == 0)
		WhenLeftDouble();
}

void КтрлМассив::леваяТяг(Точка p, dword)
{
	int ii = GetLineAt(p.y + sb);
	if(!пусто_ли(ii) && IsSel(ii))
		WhenDrag();
}

void КтрлМассив::синхИнфо()
{
	if((естьМышь() || info.естьМышь()) && !IsEdit() && popupex) {
		Точка p = GetMouseViewPos();
		Точка c;
		c.y = GetLineAt(p.y + sb);
		if(c.y >= 0) {
			for(c.x = 0; c.x < column.дайСчёт(); c.x++) {
				if(!ктрл_ли(c.y, c.x)) {
					Прям r = GetCellRect(c.y, c.x);
					int cm = column[c.x].margin;
					if(cm < 0)
						cm = header.Вкладка(header.найдиИндекс(c.x)).GetMargin();
					dword st;
					Цвет fg, bg;
					Значение q;
					const Дисплей& d = GetCellInfo(c.y, c.x, HasFocusDeep(), q, fg, bg, st);
					int cw = r.устШирину();
					if(spanwidecells)
						SpanWideCell(d, q, cm, cw, r, c.y, c.x);
					if(r.содержит(p)) {
						info.уст(this, r, q, &d, fg, bg, st, cm);
						return;
					}
				}
			}
		}
	}
	info.Cancel();
}

void КтрлМассив::двигМыши(Точка p, dword)
{
	int ii = Null;
	if(WhenMouseMove) {
		ii = GetLineAt(p.y + sb);
		if(пусто_ли(ii))
			WhenMouseMove(Null);
		else
			WhenMouseMove(Точка(GetClickColumn(ii, p), ii));
	}
	if(mousemove && !толькочтен_ли()) {
		if(пусто_ли(ii))
			ii = GetLineAt(p.y + sb);
		if(!пусто_ли(ii)) устКурсор(ii);
	}
	синхИнфо();
}

void КтрлМассив::выходМыши()
{
	WhenMouseMove(Null);
}

Рисунок КтрлМассив::рисКурсора(Точка p, dword)
{
	if(!пусто_ли(cursor_override))
		return cursor_override;
	return header.GetSplit(p.x) < 0 || header.дайРежим() == HeaderCtrl::FIXED ? Рисунок::Arrow()
	                                                                         : CtrlImg::HorzPos();
}

void КтрлМассив::праваяВнизу(Точка p, dword flags) {
	if((flags & (K_CTRL|K_SHIFT)) == 0) {
		DoPoint(p, false);
		ClickColumn(p);
		if(cursor >= 0 && multiselect) {
			if(!выделен(cursor)) {
				очистьВыделение();
				выделиОдин(anchor = cursor);
			}
			Action();
		}
	}
	БарМеню::выполни(WhenBar);
}

bool КтрлМассив::TestKey(int i, int ключ) {
	for(int j = 0; j < column.дайСчёт(); j++) {
		int (*af)(int) = column[j].accel;
		if(af) {
			int c = (*af)(ключ);
			if(c && (*af)(стдФормат(дайКолонку(i, j))[0]) == c) {
				устКурсор(i);
				return true;
			}
		}
	}
	for(int ii = 0; ii < idx.дайСчёт(); ii++) {
		int (*af)(int) = idx[ii].accel;
		if(af) {
			int c = (*af)(ключ);
			if(c && (*af)(стдФормат(дай(i, ii))[0]) == c) {
				устКурсор(i);
				return true;
			}
		}
	}
	return false;
}

int КтрлМассив::GetEditColumn() const {
	if(!IsEdit()) return -1;
	for(int i = 0; i < column.дайСчёт(); i++) {
		const Колонка& m = column[i];
		if(m.edit && m.edit->HasFocusDeep())
			return i;
	}
	return -1;
}

void КтрлМассив::KeyMultiSelect(int aanchor, dword ключ)
{
	if(!multiselect)
		return;
	очистьВыделение();
	if(ключ & K_SHIFT) {
		anchor = aanchor;
		if(anchor >= 0 && cursor >= 0)
			выдели(min(anchor, cursor), абс(anchor - cursor) + 1);
	}
	else
	if(cursor >= 0) {
		anchor = cursor;
		выделиОдин(anchor);
	}
}

bool КтрлМассив::Ключ(dword ключ, int) {
	if(толькочтен_ли()) return false;
	int aanchor = anchor;
	if(!editmode) {
		if(ключ > 32 && ключ < 256) {
			int cr = cursor >= 0 ? cursor + 1 : 0;
			for(int j = 0; j < column.дайСчёт(); j++)
				if(column[j].accel) {
					int i;
					for(i = cr; i < array.дайСчёт(); i++)
						if(TestKey(i, ключ)) return true;
					for(i = 0; i < cr; i++)
						if(TestKey(i, ключ)) return true;
					return true;
				}
		}
		switch(ключ) {
		case K_PAGEUP:
		case K_SHIFT_PAGEUP:
			Page(-1);
			KeyMultiSelect(aanchor, ключ);
			return true;
		case K_PAGEDOWN:
		case K_SHIFT_PAGEDOWN:
			if((IsInserting() || IsAppending()) && IsAppendLine() && cursor == дайСчёт() - 1)
				анулируйКурсор();
			else {
				Page(1);
				KeyMultiSelect(aanchor, ключ);
			}
			return true;
		case K_HOME:
		case K_CTRL_HOME:
		case K_CTRL_PAGEUP:
		case K_SHIFT_HOME:
		case K_SHIFT|K_CTRL_HOME:
		case K_SHIFT|K_CTRL_PAGEUP:
			идиВНач();
			KeyMultiSelect(aanchor, ключ);
			return true;
		case K_END:
		case K_CTRL_END:
		case K_CTRL_PAGEDOWN:
		case K_SHIFT|K_END:
		case K_SHIFT|K_CTRL_END:
		case K_SHIFT|K_CTRL_PAGEDOWN:
			идиВКон();
			KeyMultiSelect(aanchor, ключ);
			return true;
		case K_CTRL_A:
			if(multiselect) {
				идиВКон();
				anchor = 0;
				KeyMultiSelect(0, K_SHIFT);
			}
			return true;
		}
	}
	switch(ключ) {
	case K_UP:
	case K_SHIFT_UP:
		if(курсор_ли()) {
			int d = GetEditColumn();
			int i = FindEnabled(cursor - 1,  -1);
			if(i >= 0 && устКурсор0(i)) {
				if(d >= 0)
					StartEdit(d);
				else
					KeyMultiSelect(aanchor, ключ);
			}
		}
		else
		if((IsInserting() || IsAppending()) && IsAppendLine())
			устКурсор(дайСчёт() - 1);
		else
			sb.предшСтрочка();
		return true;
	case K_DOWN:
	case K_SHIFT_DOWN:
		if((IsInserting() || IsAppending()) && IsAppendLine() && cursor == дайСчёт() - 1 && !editmode) {
			if(IsMultiSelect())
				очистьВыделение();
			анулируйКурсор();
		}
		else
		if(курсор_ли()) {
			int d = GetEditColumn();
			int i = FindEnabled(cursor + 1, 1);
			if(i >= 0 && устКурсор0(i) && d >= 0)
				StartEdit(d);
			else
				KeyMultiSelect(aanchor, ключ);
		}
		else
			sb.следщСтрочка();
		return true;
	case K_ENTER:
		if(!курсор_ли() && (IsInserting() || IsAppending()) && IsAppendLine()) {
			DoAppend();
			return true;
		}
		if(editmode) {
			bool ins = IsInsert() && autoappend;
			if(AcceptEnter() && ins)
				DoAppend();
			return true;
		}
		if(WhenEnterKey && курсор_ли()) {
			WhenEnterKey();
			return true;
		}
		break;
	case K_ESCAPE:
		if(IsEdit()) {
			int c = cursor;
			CancelCursor();
			устКурсор(c);
			return true;
		}
		break;
	}
	return БарМеню::скан(WhenBar, ключ);
}

bool КтрлМассив::AcceptEnter() {
	if(editmode) {
		bool ins = IsInsert();
		if(!прими())
			return false;
		if(дайКурсор() == array.дайСчёт() - 1 &&
		(IsInserting() || IsAppending()) && IsAppendLine() && ins && !noinsertappend) {
			if(IsMultiSelect())
				очистьВыделение();
			анулируйКурсор();
			ShowAppendLine();
		}
	}
	return true;
}

void  КтрлМассив::добавь() {
	if(дайСчётИндексов()) {
		Вектор<Значение> x;
		for(int ii = 0; ii < idx.дайСчёт(); ii++)
			x.добавь(idx[ii].дайВставьЗнач());
		уст(array.дайСчёт(), x);
	}
	else {
		int i = array.дайСчёт();
		array.по(i);
		SetSb();
		освежи();
		инвалидируйКэш(cursor);
		инвалидируйКэш(i);
		RefreshRow(i);
	}
	WhenArrayAction();
}

void КтрлМассив::уст(int ii, const ВекторМап<Ткст, Значение>& m)
{
	for(int i = 0; i < m.дайСчёт(); i++) {
		int j = дайПоз(m.дайКлюч(i));
		if(j >= 0)
			уст(ii, j, m[i]);
	}
}

void КтрлМассив::устМап(int ii, const МапЗнач& m)
{
	for(int i = 0; i < m.дайСчёт(); i++) {
		int j = дайПоз((Ткст)m.дайКлюч(i));
		if(j >= 0)
			уст(ii, j, m.дайЗначение(i));
	}
}

void КтрлМассив::добавь(const ВекторМап<Ткст, Значение>& m)
{
	уст(array.дайСчёт(), m);
}

void КтрлМассив::добавьМап(const МапЗнач& m)
{
	устМап(array.дайСчёт(), m);
}

МапЗнач КтрлМассив::дайМап(int i) const
{
	МапЗнач m;
	for(int j = 0; j < дайСчётИндексов(); j++) {
		Ткст id = ~дайИд(j);
		if(id.дайСчёт())
			m(id, дай(i, j));
	}
	return m;
}

МассивЗнач КтрлМассив::дайМассив(int i) const
{
	return МассивЗнач(clone(дайСтроку(i)));
}

void КтрлМассив::устМассив(int i, const МассивЗнач& va)
{
	уст(i, va.дай());
}

void КтрлМассив::добавьМассив(const МассивЗнач& va)
{
	устМассив(array.дайСчёт(), va);
}

struct ArrayCtrlSeparatorDisplay : Дисплей {
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const {
		int y = r.top + r.дайВысоту() / 2;
		w.DrawRect(r, paper);
		w.DrawRect(r.left, y, r.дайШирину(), 1, SColorShadow());
		w.DrawRect(r.left, y + 1, r.дайШирину(), 1, SColorLight());
	}
	virtual Размер дайСтдРазм(const Значение& q) const {
		return Размер(0, 2);
	}
};

void КтрлМассив::добавьСепаратор()
{
	int ii = дайСчёт();
	добавь();
	SetLineCy(ii, Draw::GetStdFontCy() / 2);
	for(int i = 0; i < дайСчётКолонок(); i++)
		устДисплей(ii, i, Single<ArrayCtrlSeparatorDisplay>());
	дезактивируйСтроку(ii);
}

//$-
#define E__Addv(I)    уст0(q, I - 1, p##I)
#define E__AddF(I) \
void КтрлМассив::добавь(__List##I(E__Value)) { \
	int q = array.дайСчёт(); \
	__List##I(E__Addv); \
	AfterSet(q); \
}
__Expand(E__AddF)
//$+

void  КтрлМассив::вставь(int i, int count) {
	if(i < array.дайСчёт()) {
		array.вставьН(i, count);
		for(int j = 0; j < column.дайСчёт(); j++)
			column[j].вставьКэш(i, count);
	}
	else
		array.по(i + count - 1);
	if(i <= cursor) cursor++;
	if(i < cellinfo.дайСчёт())
		cellinfo.вставьН(i, count);
	if(i < ln.дайСчёт()) {
		int y = ln[i].y;
		ln.вставьН(i, count);
		Reline(i, y);
	}
	if(virtualcount >= 0) virtualcount += count;
	while(count--) {
		for(int ii = 0; ii < idx.дайСчёт(); ii++) {
			Значение v = idx[ii].дайВставьЗнач();
			if(!пусто_ли(v))
				уст(i, ii, v);
		}
		i++;
	}
	освежи();
	SetSb();
	PlaceEdits();
	SyncCtrls();
	синхИнфо();
	WhenArrayAction();
}

void  КтрлМассив::вставь(int i)
{
	вставь(i, 1);
}

void КтрлМассив::вставь(int i, const Вектор<Значение>& v) {
	вставь(i);
	уст(i, v);
}

void КтрлМассив::вставь(int i, const Вектор< Вектор<Значение> >& v) {
	вставь(i, v.дайСчёт());
	for(int ii = 0; ii < v.дайСчёт(); ii++)
		уст(i++, v[ii]);
}

void  КтрлМассив::удали(int i) {
	int c = cursor;
	if(i == cursor) CancelCursor();
	удали0(i);
	if(c >= 0)
		устКурсор0(c - (i < c), false);
	anchor = -1;
	PlaceEdits();
	SyncCtrls();
	SetSb();
	освежи();
	синхИнфо();
}

void КтрлМассив::PlaceEdits() {
	for(int i = 0; i < column.дайСчёт(); i++) {
		Колонка& m = column[i];
		if(m.edit) {
			m.edit->покажи(editmode && header.IsTabVisible(i));
			if(m.edit->видим_ли())
				m.edit->устПрям(GetCellRectM(cursor, i));
		}
	}
}

bool КтрлМассив::IsEditing() const {
	for(int i = 0; i < column.дайСчёт(); i++)
		if(column[i].edit) return true;
	return false;
}

bool КтрлМассив::StartEdit(int d) {
	if(толькочтен_ли()) return false;
	if(!IsEditing()) return false;
	editmode = true;
	PlaceEdits();
	for(int i = 0; i < column.дайСчёт(); i++) {
		Колонка& m = column[(i + d) % column.дайСчёт()];
		if(m.edit && (m.edit->SetWantFocus() || IterateFocusForward(m.edit, m.edit)))
			break;
	}
	NoWantFocus();
	RefreshRow(cursor);
	WhenStartEdit();
	синхИнфо();
	return true;
}

void КтрлМассив::EndEdit() {
	if(!IsEditing())
		return;
	bool f = HasFocusDeep();
	editmode = false;
	insertmode = false;
	PlaceEdits();
	WantFocus();
	if(f) SetWantFocus();
	синхИнфо();
}

void КтрлМассив::расфокусирован() {
	if(дайСчётВыделений() > 1)
		освежи();
	else
	if(курсор_ли())
		RefreshRow(cursor);
	синхИнфо();
}

void КтрлМассив::сфокусирован() {
	if(дайСчётВыделений() > 1)
		освежи();
	else
	if(курсор_ли())
		RefreshRow(cursor);
	else
	if(focussetcursor)
		идиВНач();
	SetCursorEditFocus();
	синхИнфо();
}

void КтрлМассив::DoEdit() {
	if(толькочтен_ли()) return;
	if(!editmode && курсор_ли())
		StartEdit();
}

void КтрлМассив::DoInsert(int c) {
	if(толькочтен_ли()) return;
	вставь(c);
	устКурсор(c);
	insertmode = true;
	DoEdit();
	if(!IsEdit())
		insertmode = false;
	WhenArrayAction();
}

void КтрлМассив::DoInsertBefore() {
	DoInsert(курсор_ли() ? дайКурсор() : array.дайСчёт());
}

void КтрлМассив::DoInsertAfter() {
	DoInsert(курсор_ли() ? дайКурсор() + 1 : array.дайСчёт());
}

void КтрлМассив::DoAppend() {
	DoInsert(array.дайСчёт());
}

void КтрлМассив::выделиВсе()
{
	выдели(0, дайСчёт());
}

Ткст КтрлМассив::фмтРяда(const char *s)
{
	return спринтф(s, ~row_name);
}

bool КтрлМассив::DoRemove()
{
	if(толькочтен_ли()) return false;
	if(!курсор_ли() || askremove && !PromptOKCancel(фмтРяда(t_("Do you really want to delete the selected %s ?"))))
		return false;
	if(multiselect) {
		Биты sel;
		for(int i = 0; i < дайСчёт(); i++)
			sel.уст(i, выделен(i));
		for(int i = дайСчёт() - 1; i >= 0; i--)
			if(sel[i])
				удали(i);
	}
	else
		удали(cursor);
	WhenArrayAction();
	return true;
}

void КтрлМассив::DoRemovem()
{
	DoRemove();
}

void КтрлМассив::DoDuplicate() {
	if(толькочтен_ли()) return;
	if(!курсор_ли()) return;
	int c = cursor;
	if(!анулируйКурсор()) return;
	Вектор<Значение> va = читайРяд(c);
	c = IsAppending() ? array.дайСчёт() : c + 1;
	вставь(c, va);
	устКурсор(c);
	DoEdit();
	WhenArrayAction();
}

void КтрлМассив::StdBar(Бар& menu)
{
	bool e = редактируем_ли();
	bool c = !IsEdit() && e;
	bool d = c && курсор_ли();
	if(inserting)
		menu.добавь(e, фмтРяда(t_("вставь %s")), THISBACK(DoInsertBefore))
			.Help(фмтРяда(t_("вставь a new %s into the table.")))
			.Ключ(K_INSERT);
	if(bains == 1) {
		menu.добавь(e, фмтРяда(t_("вставь %s before")), THISBACK(DoInsertBefore))
		    .Help(фмтРяда(t_("вставь a new %s into the table before current")))
		    .Ключ(K_INSERT);
		menu.добавь(e, фмтРяда(t_("вставь %s after")), THISBACK(DoInsertAfter))
		    .Help(фмтРяда(t_("вставь a new %s into the table after current")))
		    .Ключ(K_SHIFT_INSERT);
	}
	if(bains == 2) {
		menu.добавь(e, фмтРяда(t_("вставь %s after")), THISBACK(DoInsertAfter))
		    .Help(фмтРяда(t_("вставь a new %s into the table after current")))
		    .Ключ(K_INSERT);
		menu.добавь(e, фмтРяда(t_("вставь %s before")), THISBACK(DoInsertBefore))
		    .Help(фмтРяда(t_("вставь a new %s into the table before current")))
		    .Ключ(K_SHIFT_INSERT);
	}
	if(IsAppending())
		menu.добавь(c, фмтРяда(t_("приставь %s")), THISBACK(DoAppend))
			.Help(фмтРяда(t_("приставь a new %s at the end of the table.")))
			.Ключ(inserting ? (int)K_SHIFT_INSERT : (int)K_INSERT);
	if(duplicating)
		menu.добавь(d, фмтРяда(t_("Duplicate %s")), THISBACK(DoDuplicate))
			.Help(фмтРяда(t_("Duplicate current table %s.")))
			.Ключ(K_CTRL_INSERT);
	if(IsEditing())
		menu.добавь(d, фмтРяда(t_("Edit %s")), THISBACK(DoEdit))
			.Help(фмтРяда(t_("Edit active %s.")))
			.Ключ(K_CTRL_ENTER);
	if(removing)
		menu.добавь(d, фмтРяда(t_("Delete %s\tDelete")), THISBACK(DoRemovem))
			.Help(фмтРяда(t_("Delete active %s.")))
			.Ключ(K_DELETE);
	if(moving) {
		menu.добавь(дайКурсор() > 0, фмтРяда(t_("Move %s up")), THISBACK(SwapUp))
			.Help(фмтРяда(t_("разверни %s with previous thus moving it up.")))
			.Ключ(K_CTRL_UP);
		menu.добавь(дайКурсор() >= 0 && дайКурсор() < дайСчёт() - 1,
		         фмтРяда(t_("Move %s down")), THISBACK(SwapDown))
			.Help(фмтРяда(t_("разверни %s with next thus moving it down.")))
			.Ключ(K_CTRL_DOWN);
	}
	if(multiselect) {
		menu.добавь(дайСчёт() > 0, фмтРяда(t_("Выбрать все")), CtrlImg::select_all(), THISBACK(выделиВсе))
			.Help(t_("Выделить все ряды таблицы"))
			.Ключ(K_CTRL_A);
	}
}

int КтрлМассив::найди(const Значение& v, int ii, int i) const {
	ПРОВЕРЬ(ii >= 0);
	int n = дайСчёт();
	while(i < n) {
		if(строкаАктивирована(i) && дай(i, ii) == v) return i;
		i++;
	}
	return -1;
}

int  КтрлМассив::найди(const Значение& v, const Ид& id, int i) const {
	return найди(v, дайПоз(id), i);
}

bool КтрлМассив::FindSetCursor(const Значение& val, int ii, int i) {
	i = найди(val, ii, i);
	if(i < 0) return false;
	if(!устКурсор(i)) return false;
	return true;
}

bool КтрлМассив::FindSetCursor(const Значение& val, const Ид& id, int i) {
	return FindSetCursor(val, idx.найди(id), i);
}

void КтрлМассив::очистьКэш() {
	for(int i = 0; i < column.дайСчёт(); i++)
		column[i].очистьКэш();
}

struct КтрлМассив::ПредикатСортировки {
	const КтрлМассив::Порядок *order;
	bool operator()(const Строка& a, const Строка& b) const {
		return order->operator()(a.line, b.line);
	}
};

void КтрлМассив::SortA()
{
	if(hasctrls) {
		for(int i = 0; i < array.дайСчёт(); i++)
			for(int j = 0; j < column.дайСчёт(); j++)
				if(ктрл_ли(i, j)) {
					const Колонка& m = column[j];
					ПРОВЕРЬ(m.pos.дайСчёт() == 1);
					array[i].line.по(m.pos[0]) = дайКтрлЯчейка(i, j).ctrl->дайДанные();
				}
	}
}

void КтрлМассив::SortB(const Вектор<int>& o)
{
	Вектор<Строка> narray;
	narray.устСчёт(array.дайСчёт());
	Вектор<Ln> nln;
	Вектор< Вектор<ИнфОЯчейке> > ncellinfo;
	for(int i = 0; i < o.дайСчёт(); i++) {
		int oi = o[i];
		narray[i] = pick(array[oi]);
		if(oi < cellinfo.дайСчёт())
			ncellinfo.по(i) = pick(cellinfo[oi]);
		if(oi < ln.дайСчёт())
			nln.по(i) = ln[oi];
	}
	array = pick(narray);
	cellinfo = pick(ncellinfo);
	ln = pick(nln);
	Reline(0, 0);
	if(hasctrls) {
		for(int i = 0; i < array.дайСчёт(); i++)
			for(int j = 0; j < column.дайСчёт(); j++)
				if(ктрл_ли(i, j)) {
					const Колонка& m = column[j];
					ПРОВЕРЬ(m.pos.дайСчёт() == 1);
					array[i].line.по(m.pos[0]) = Null;
				}
		SyncCtrls();
		отпрыскФок();
	}
}

void КтрлМассив::ReArrange(const Вектор<int>& order)
{
	анулируйКурсор();
	очистьВыделение();
	очистьКэш();
	SortA();
	SortB(order);
	освежи();
	синхИнфо();
}

void КтрлМассив::сортируй(int from, int count, Врата<int, int> order)
{
	анулируйКурсор();
	очистьВыделение();
	очистьКэш();
	Вектор<int> h;
	for(int i = 0; i < array.дайСчёт(); i++)
		h.добавь(i);
	SortA();
	соСтабилСортируй(СубДиапазон(h, from, count).пиши(), order);
	SortB(h);
	освежи();
	синхИнфо();
}

void КтрлМассив::сортируй(Врата<int, int> order)
{
	if(sorting_from < array.дайСчёт())
		сортируй(sorting_from, array.дайСчёт() - sorting_from, order);
}

bool КтрлМассив::OrderPred(int i1, int i2, const КтрлМассив::Порядок *o)
{
	return (*o)(array[i1].line, array[i2].line);
}

void КтрлМассив::сортируй(int from, int count, const КтрлМассив::Порядок& order)
{
	сортируй(from, count, THISBACK1(OrderPred, &order));
}

void КтрлМассив::сортируй(const КтрлМассив::Порядок& order)
{
	if(sorting_from < array.дайСчёт())
		сортируй(sorting_from, array.дайСчёт() - sorting_from, order);
}

struct sAC_ColumnSort : public ПорядокЗнач {
	bool                                           descending;
	const ПорядокЗнач                              *order;
	Функция<int (const Значение& a, const Значение& b)> сравни;

	virtual bool operator()(const Значение& a, const Значение& b) const {
		return descending ? order ? (*order)(b, a) : сравни(b, a) < 0
		                  : order ? (*order)(a, b) : сравни(a, b) < 0;
	}
};

bool КтрлМассив::ColumnSortPred(int i1, int i2, int column, const ПорядокЗнач *o)
{
	return (*o)(GetConvertedColumn(i1, column), GetConvertedColumn(i2, column));
}

void КтрлМассив::ColumnSort(int column, Врата<int, int> order)
{
	Значение ключ = дайКлюч();
	CHECK(анулируйКурсор());
	if(columnsortsecondary)
		сортируй(*columnsortsecondary);
	сортируй(order);
	if(columnsortfindkey)
		FindSetCursor(ключ);
}

void КтрлМассив::ColumnSort(int column, const ПорядокЗнач& order)
{
	ColumnSort(column, THISBACK2(ColumnSortPred, column, &order));
}

void КтрлМассив::ColumnSort(int column, int (*compare)(const Значение& a, const Значение& b))
{
	sAC_ColumnSort cs;
	cs.descending = false;
	cs.order = NULL;
	cs.сравни = compare;
	if(!cs.сравни)
		cs.сравни = сравниСтдЗнач;
	ColumnSort(column, cs);
}

void КтрлМассив::SetSortColumn(int ii, bool desc)
{
	sortcolumn = ii;
	sortcolumndescending = desc;
	DoColumnSort();
}

void КтрлМассив::ToggleSortColumn(int ii)
{
	if(sortcolumn == ii)
		sortcolumndescending = !sortcolumndescending;
	else {
		sortcolumn = ii;
		sortcolumndescending = false;
	}
	DoColumnSort();
}

void КтрлМассив::DoColumnSort()
{
	if(sortcolumn < 0) {
		sortcolumndescending = false;
		for(int i = 0; i < column.дайСчёт(); i++)
			if(column[i].order || column[i].сравни) {
				sortcolumn = i;
				break;
			}
	}
	if(sortcolumn < 0)
		return;
	for(int i = 0; i < header.дайСчёт(); i++)
		header.Вкладка(i).SetRightImage(header.GetTabIndex(i) == sortcolumn ?
		                                 sortcolumndescending ? CtrlImg::SortUp()
		                                                      : CtrlImg::SortDown()
		                            : Рисунок());
	if(sortcolumn >= 0 && sortcolumn < column.дайСчёт()) {
		sAC_ColumnSort cs;
		const Колонка& c = column[sortcolumn];
		if(c.line_order)
			ColumnSort(sortcolumn, c.line_order);
		else {
			cs.descending = sortcolumndescending;
			cs.order = c.order;
			cs.сравни = c.сравни;
			if(!cs.order && !cs.сравни)
				cs.сравни = сравниСтдЗнач;
			ColumnSort(sortcolumn, cs);
		}
	}
	WhenColumnSorted();
}

КтрлМассив& КтрлМассив::AllSorting()
{
	allsorting = true;
	for(int i = 0; i < column.дайСчёт(); i++)
		column[i].Sorting();
	return *this;
}

struct КтрлМассив::ПорядокРядов : public КтрлМассив::Порядок {
	int (*compare)(const Вектор<Значение>& v1, const Вектор<Значение>& v2);

	virtual bool operator()(const Вектор<Значение>& row1, const Вектор<Значение>& row2) const {
		return (*compare)(row1, row2) < 0;
	}
};

void КтрлМассив::сортируй(int from, int count, int (*compare)(const Вектор<Значение>& v1, const Вектор<Значение>& v2))
{
	ПорядокРядов io;
	io.compare = compare;
	сортируй(from, count, io);
}

void КтрлМассив::сортируй(int (*compare)(const Вектор<Значение>& v1, const Вектор<Значение>& v2)) {
	сортируй(0, дайСчёт(), compare);
}

struct КтрлМассив::ПорядокЭлтов : public КтрлМассив::Порядок {
	int ii;
	int (*compare)(const Значение& v1, const Значение& v2);

	virtual bool operator()(const Вектор<Значение>& row1, const Вектор<Значение>& row2) const {
		return (*compare)(row1[ii], row2[ii]) < 0;
	}
};

void КтрлМассив::сортируй(int ii, int (*compare)(const Значение& v1, const Значение& v2)) {
	ПорядокЭлтов io;
	io.ii = ii;
	io.compare = compare;
	сортируй(io);
}

void КтрлМассив::сортируй(const Ид& id, int (*compare)(const Значение& v1, const Значение& v2)) {
	сортируй(idx.найди(id), compare);
}

void КтрлМассив::очисть() {
	EndEdit();
	int oc = cursor;
	if(cursor >= 0) {
		WhenKillCursor();
		cursor = -1;
		info.Cancel();
	}
	array.очисть();
	if(oc >= 0) {
		WhenCursor();
		WhenSel();
	}
	cellinfo.очисть();
	ln.очисть();
	очистьКэш();
	отклКтрлы();
	virtualcount = -1;
	anchor = -1;
	SetSb();
	освежи();
	синхИнфо();
}

void КтрлМассив::сожми() {
	array.сожми();
	cellinfo.сожми();
	ln.сожми();
}

void КтрлМассив::сериализуйНастройки(Поток& s)
{
	int version = 0;
	s / version;
	header.сериализуй(s);
	s % sortcolumn % sortcolumndescending;
	if(s.грузится())
		DoColumnSort();
}

void КтрлМассив::сериализуй(Поток& s)
{
}

void КтрлМассив::переустанов() {
	header.переустанов();
	idx.очисть();
	id_ndx.очисть();
	id_ndx.добавь(Ид());
	column.очисть();
	control.очисть();
	nocursor = false;
	vertgrid = horzgrid = true;
	mousemove = false;
	inserting = false;
	popupex = true;
	appending = false;
	removing = false;
	askremove = true;
	duplicating = false;
	moving = false;
	appendline = false;
	noinsertappend = false;
	editmode = false;
	insertmode = false;
	multiselect = false;
	selectiondirty = false;
	hasctrls = false;
	headerctrls = false;
	nobg = false;
	selectcount = 0;
	bains = 0;
	row_name = t_("row");
	gridcolor = SColorShadow;
	autoappend = false;
	focussetcursor = true;
	sortcolumn = -1;
	allsorting = false;
	acceptingrow = 0;
	columnsortfindkey = false;
	spanwidecells = false;
	linecy = Draw::GetStdFontCy();
	очисть();
	sb.устСтроку(linecy);
	columnsortsecondary = NULL;
	sorting_from = 0;
	min_visible_line = 0;
	max_visible_line = INT_MAX;
	ctrl_low = ctrl_high = 0;
}

void КтрлМассив::режимОтмены()
{
	isdrag = false;
	selclick = false;
	dropline = dropcol = -1;
	info.Cancel();
}

void КтрлМассив::колесоМыши(Точка p, int zdelta, dword keyflags) {
	sb.Wheel(zdelta);
}

Вектор<Значение> КтрлМассив::читайРяд(int i) const {
	Вектор<Значение> v;
	int n = max(idx.дайСчёт(), i < array.дайСчёт() ? array[i].line.дайСчёт() : 0);
	for(int j = 0; j < n; j++)
		v.добавь(дай(i, j));
	return v;
}

void КтрлМассив::Move(int dir) {
	int c = дайКурсор();
	int d = c + dir;
	if(c < 0 || c >= дайСчёт() || d < 0 || d >= дайСчёт()) return;
	Вектор<Значение> row(читайРяд(c), 0);
	уст(c, читайРяд(d));
	уст(d, row);
	устКурсор(d);
	WhenArrayAction();
}

void КтрлМассив::SwapUp() {
	Move(-1);
}

void КтрлМассив::SwapDown() {
	Move(1);
}

КтрлМассив& КтрлМассив::ColumnWidths(const char *s)
{
	Вектор<Ткст> w = разбей(s, ' ');
	for(int i = 0; i < min(w.дайСчёт(), header.дайСчёт()); i++) {
		int q = header.найдиИндекс(i);
		if(q >= 0)
			header.SetTabRatio(q, atoi(w[i]));
	}
	return *this;
}

Ткст КтрлМассив::GetColumnWidths()
{
	Ткст text;
	for(int i = 0; i < header.дайСчёт(); i++)
		text << фмт(i ? " %d" : "%d",
		              header.дайРежим() == HeaderCtrl::SCROLL ? (int)header[i].GetRatio()
		                                                     : header.GetTabWidth(i));
	return text;
}

КтрлМассив& КтрлМассив::OddRowColor(Цвет paper, Цвет ink)
{
	oddpaper = paper;
	oddink = ink;
	освежи();
	синхИнфо();
	return *this;
}

КтрлМассив& КтрлМассив::EvenRowColor(Цвет paper, Цвет ink)
{
	evenpaper = paper;
	evenink = ink;
	освежи();
	синхИнфо();
	return *this;
}

void КтрлМассив::освежиВыд()
{
	Размер sz = дайРазм();
	int i = GetLineAt(sb);
	if(i >= 0) {
		int y = GetLineY(i) - sb;
		while(y < sz.cy && i < array.дайСчёт()) {
			if(выделен(i))
				RefreshRow(i);
			if(строкаВидима(i))
				y += GetLineCy(i++);
		}
	}
}

void КтрлМассив::ТиБ(int line, int col)
{
	if(dropline != line || dropcol != col) {
		RefreshRow(dropline - 1);
		RefreshRow(dropline);
		dropline = line;
		dropcol = col;
		RefreshRow(dropline - 1);
		RefreshRow(dropline);
	}
}

bool КтрлМассив::вставьТиБ(int line, int py, PasteClip& d, int q)
{
	int y = GetLineY(line);
	int cy = GetLineCy();
	if(py < y + cy / q) {
		WhenDropInsert(line, d);
		if(d.IsAccepted()) {
			ТиБ(line, DND_INSERTLINE);
			return true;
		}
	}
	if(py >= y + (q - 1) * cy / q && line < дайСчёт()) {
		WhenDropInsert(line + 1, d);
		if(d.IsAccepted()) {
			ТиБ(line + 1, DND_INSERTLINE);
			return true;
		}
	}
	return false;
}

void КтрлМассив::тягИБрос(Точка p, PasteClip& d)
{
	if(толькочтен_ли())
		return;
	if(!isdrag) {
		isdrag = true;
		освежиВыд();
	}
	int py = p.y + sb;
	int line = GetLineAt(py);
	int col = -1;
	if(line >= 0) {
		for(int i = 0; i < column.дайСчёт(); i++)
			if(GetCellRect(line, i).содержит(p)) {
				col = i;
				break;
			}
		if(col >= 0) {
			WhenDropCell(line, col, d);
			if(d.IsAccepted()) {
				ТиБ(line, col);
				return;
			}
		}
		if(вставьТиБ(line, py, d, 4)) return;
		if(line >= 0) {
			WhenDropLine(line, d);
			if(d.IsAccepted()) {
				ТиБ(line, DND_LINE);
				return;
			}
		}
		if(вставьТиБ(line, py, d, 2)) return;
	}
	int ci = дайСчёт();
	if(py < GetLineY(ci) + GetLineCy(ci) / 4 || !WhenDrop) {
		WhenDropInsert(ci, d);
		if(d.IsAccepted()) {
			ТиБ(дайСчёт(), DND_INSERTLINE);
			return;
		}
	}
	WhenDrop(d);
	ТиБ(-1, -1);
}

void КтрлМассив::тягПовтори(Точка p)
{
	sb = sb + GetDragScroll(this, p, 16).y;
}

void КтрлМассив::тягПокинь()
{
	isdrag = false;
	освежиВыд();
	ТиБ(-1, -1);
}

void КтрлМассив::удалиВыделение()
{
	int ci = cursor;
	анулируйКурсор();
	for(int i = дайСчёт() - 1; i >= 0; i--)
		if(IsSel(i) || i == ci)
			удали(i); // Optimize!
}

void КтрлМассив::вставьБрос(int line, const Вектор< Вектор<Значение> >& data, PasteClip& d, bool self)
{
	if(data.дайСчёт() == 0)
		return;
	if(d.GetAction() == DND_MOVE && self) {
		if(IsMultiSelect())
			for(int i = дайСчёт() - 1; i >= 0; i--) {
				if(IsSel(i)) {
					удали(i); // Optimize!
					if(i < line)
						line--;
				}
			}
		else
		if(курсор_ли()) {
			if(дайКурсор() < line)
				line--;
			удали(дайКурсор());
		}
		анулируйКурсор();
		d.SetAction(DND_COPY);
	}
	вставь(line, data);
	очистьВыделение();
	устКурсор(line + data.дайСчёт() - 1);
	if(IsMultiSelect())
		выдели(line, data.дайСчёт());
}

void КтрлМассив::вставьБрос(int line, const Вектор<Значение>& data, PasteClip& d, bool self)
{
	Вектор< Вектор<Значение> > x;
	x.добавь() <<= data;
	вставьБрос(line, x, d, self);
}

void КтрлМассив::вставьБрос(int line, const Значение& data, PasteClip& d, bool self)
{
	Вектор<Значение> x;
	x.добавь(data);
	вставьБрос(line, x, d, self);
}

void КтрлМассив::вставьБрос(int line, const КтрлМассив& src, PasteClip& d)
{
	Вектор< Вектор<Значение> > data;
	for(int i = 0; i < src.дайСчёт(); i++)
		if(src.IsSel(i))
			data.добавь(src.дайСтроку(i));
	вставьБрос(line, data, d, &src == this);
}

void КтрлМассив::вставьБрос(int line, PasteClip& d)
{
	вставьБрос(line, GetInternal<КтрлМассив>(d), d);
}

Ткст КтрлМассив::AsText(Ткст (*формат)(const Значение&), bool sel,
                         const char *tab, const char *row,
                         const char *hdrtab, const char *hdrrow) const
{
	Ткст txt;
	if(hdrtab) {
		for(int i = 0; i < дайСчётКолонок(); i++) {
			if(i)
				txt << hdrtab;
			txt << (*формат)(HeaderTab(i).дайТекст());
		}
		if(hdrrow)
			txt << hdrrow;
	}
	bool next = false;
	for(int r = 0; r < дайСчёт(); r++)
		if(!sel || IsSel(r)) {
			if(next)
				txt << row;
			for(int i = 0; i < дайСчётКолонок(); i++) {
				if(i)
					txt << tab;
				txt << (*формат)(GetConvertedColumn(r, i));
			}
			next = true;
		}
	return txt;
}

void КтрлМассив::SetClipboard(bool sel, bool hdr) const
{
	ClearClipboard();
	AppendClipboardText(AsText(какТкст, sel, "\t", "\r\n", hdr ? "\t" : NULL, "\r\n"));
}

static Ткст sQtfFormat(const Значение& v)
{
	return "\1" + какТкст(v) + "\1";
}

Ткст КтрлМассив::AsQtf(bool sel, bool hdr)
{
	Ткст qtf;
	qtf << "{{";
	for(int i = 0; i < дайСчётКолонок(); i++) {
		if(i)
			qtf << ":";
		qtf << header.GetTabWidth(i);
	}
	if(hdr)
		qtf << "@L [*";
	return qtf << ' ' << AsText(sQtfFormat, sel, ":: ", ":: ", hdr ? ":: " : NULL, "::@W ]") << "}}";
}

static Ткст sCsvFormat(const Значение& v)
{
	return число_ли(v) ? какТкст(v) : CsvString(какТкст(v));
}

Ткст КтрлМассив::AsCsv(bool sel, int sep, bool hdr)
{
	char h[2] = { (char)sep, 0 };
	return AsText(sCsvFormat, sel, h, "\r\n", hdr ? h : NULL, "\r\n");
}

КтрлМассив::КтрлМассив() {
	cursor = -1;
	переустанов();
	добавьФрейм(sb);
	добавьФрейм(header);
	header.WhenLayout = THISBACK(HeaderLayout);
	header.ПриПромоте = THISBACK(HeaderScroll);
	sb.ПриПромоте = THISBACK(промотай);
	header.Moving();
	WhenAcceptRow = [] { return true; };
	WhenBar = THISBACK(StdBar);
	устФрейм(ViewFrame());
	oddpaper = evenpaper = SColorPaper;
	oddink = evenink = SColorText;
}

КтрлМассив::~КтрлМассив() {}

ArrayOption::ArrayOption()
{
	array = NULL;
	f = 0;
	t = 1;
	hswitch = vswitch = false;
	threestate = false;
	frame = true;
}

ArrayOption::~ArrayOption()
{
}

void ArrayOption::Click()
{
	if(!array)
		return;
	int c;
	for(c = Индекс.дайСчёт(); --c >= 0;)
		if(array->найдиКолонкуСПоз(Индекс[c]) == array->GetClickColumn())
			break;
	int cr = array->дайКурсор();
	if(c >= 0 && cr >= 0) {
		Значение v = array->дай(Индекс[c]);
		if(!пусто_ли(v) && v != t && v != f) {
			бипВосклицание();
			return;
		}
		if(hswitch)
			for(int i = 0; i < Индекс.дайСчёт(); i++)
				if(i != c)
					array->уст(Индекс[i], f);
		array->уст(Индекс[c], threestate && v == t
			? Значение() : v != t && !(threestate && пусто_ли(v)) ? t : f);
		if(vswitch) {
			for(int r = 0; r < array->дайСчёт(); r++)
				if(r != cr) {
					if(hswitch)
						for(int i = 0; i < Индекс.дайСчёт(); i++)
							if(i != c)
								array->уст(r, Индекс[i], f);
					array->уст(r, Индекс[c], f);
				}
		}
		WhenAction();
		array->Action();
		array->WhenArrayAction();
	}
}

void ArrayOption::Disconnect()
{
	array = NULL;
	Индекс.очисть();
}

void ArrayOption::Connect(КтрлМассив& a, int i)
{
	array = &a;
	if(Индекс.пустой()) {
		a.ПриЛевКлике << THISBACK(Click);
		a.WhenLeftDouble << THISBACK(Click);
	}
	Индекс.добавь(i);
}

КтрлМассив::Колонка& ArrayOption::добавьКолонку(КтрлМассив& ac, const char *text, int w)
{
	return добавьКолонку(ac, Ид(), text, w).NoClickEdit();
}

КтрлМассив::Колонка& ArrayOption::добавьКолонку(КтрлМассив& ac, const Ид& id, const char *text, int w)
{
	Connect(ac, ac.дайСчётИндексов());
	return ac.добавьКолонку(id, text, w).устДисплей(*this).вставьЗнач(f).NoClickEdit();
}

void ArrayOption::рисуй(Draw& w, const Прям& r, const Значение& q,
		                Цвет ink, Цвет paper, dword style) const
{
	bool gray = (array && !array->включен_ли());
	w.DrawRect(r, paper);

	int st = (gray ? CTRL_DISABLED : CTRL_NORMAL);
	int g = threestate && (q != t && q != f) ? CtrlsImg::I_O2 : q == t ? CtrlsImg::I_O1
		: CtrlsImg::I_O0;
		
	Рисунок img = CtrlsImg::дай(g + st);

	Размер crsz = min(img.дайРазм(), r.размер());
	Прям cr = r.центрПрям(crsz);

	Точка p = cr.позЦентра(img.дайРазм());
	w.DrawImage(p.x, p.y, img);
}

}
