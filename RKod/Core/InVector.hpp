#define LLOG(x)   // DLOG(x)

int64 NewInVectorSerial();

template <class T>
void InVector<T>::иниц()
{
	serial = NewInVectorSerial();
	slave = 0;
	переустанов();
}

template <class T>
InVector<T>::InVector()
{
	иниц();
}

template <class T>
void InVector<T>::переустанов()
{
	hcount = count = 0;
	SetBlkPar();
}

template <class T>
void InVector<T>::очисть()
{
	if(slave)
		Slave()->очисть();
	данные.очисть();
	индекс.очисть();
	переустанов();
	очистьКэш();
}

void SetInvectorCache__(int64 serial, int blki, int offset, int end);
void ClearInvectorCache__();
int  FindInvectorCache__(int64 serial, int& pos, int& off);

template <class T>
force_inline void InVector<T>::SetCache(int blki, int offset) const
{
#ifdef flagIVTEST
	Check(0, 0);
#endif
	SetInvectorCache__(serial, blki, offset, offset + данные[blki].дайСчёт());
}

template <class T>
force_inline void InVector<T>::очистьКэш() const
{
	ClearInvectorCache__();
}

template <class T>
force_inline int InVector<T>::FindBlock(int& pos, int& off) const
{
	int i = FindInvectorCache__(serial, pos, off);
	return i >= 0 ? i : FindBlock0(pos, off);
}

template <class T>
int InVector<T>::FindBlock0(int& pos, int& off) const
{
	LLOG("FindBlock " << pos);
	ПРОВЕРЬ(pos >= 0 && pos <= count);
	if(pos == count) {
		LLOG("Found last");
		pos = данные.верх().дайСчёт();
		off = count - pos;
		return данные.дайСчёт() - 1;
	}
	int blki = 0;
	int offset = 0;
	for(int i = индекс.дайСчёт(); --i >= 0;) {
		int n = индекс[i][blki];
		if(pos >= n) {
			blki++;
			pos -= n;
			offset += n;
		}
		blki += blki;
	}
	int n = данные[blki].дайСчёт();
	if(pos >= n) {
		blki++;
		pos -= n;
		offset += n;
	}

	SetCache(blki, offset);

	off = offset;
	return blki;
}

template <class T>
force_inline int InVector<T>::FindBlock(int& pos) const
{
	int h;
	return FindBlock(pos, h);
}

template <class T>
const T& InVector<T>::operator[](int i) const
{
	LLOG("operator[] " << i);
	ПРОВЕРЬ(i >= 0 && i < count);
	int blki = FindBlock(i);
	return данные[blki][i];
}

template <class T>
T& InVector<T>::operator[](int i)
{
	LLOG("operator[] " << i);
	ПРОВЕРЬ(i >= 0 && i < count);
	int blki = FindBlock(i);
	return данные[blki][i];
}

template <class T>
void InVector<T>::реиндексируй()
{
	LLOG("--- Reindexing");
	очистьКэш();
	SetBlkPar();
	индекс.очисть();
	if(slave)
		Slave()->реиндексируй();
	hcount = 0;
	Вектор<T> *ds = данные.старт();
	Вектор<T> *dend = данные.стоп();
	int n = данные.дайСчёт();
	if(n <= 2)
		return;
	Вектор<int>& w = индекс.добавь();
	hcount = 2;
	w.устСчёт((n + 1) >> 1);
	int *t = w.старт();
	while(ds != dend) {
		*t = (ds++)->дайСчёт();
		if(ds == dend)
			break;
		*t++ += (ds++)->дайСчёт();
	}
	int *s = w.старт();
	int *end = w.стоп();
	n = w.дайСчёт();
	while(n > 2) {
		Вектор<int>& w = индекс.добавь();
		hcount += hcount;
		w.устСчёт((n + 1) >> 1);
		t = w.старт();
		while(s != end) {
			*t = *s++;
			if(s == end)
				break;
			*t++ += *s++;
		}
		s = w.старт();
		end = w.стоп();
		n = w.дайСчёт();
	}
#ifdef flagIVTEST
	Check(0, 0);
#endif
}

template <class T>
void InVector<T>::SetBlkPar()
{
#if defined(_ОТЛАДКА) && defined(flagIVTEST)
	blk_high = 11;
	blk_low = 5;
#else
	int n = 2500 + данные.дайСчёт() / 4;
	blk_high = minmax(n / (int)sizeof(T), 16, 65000);
	blk_low = minmax(n / 3 / (int)sizeof(T), 16, 65000);
#endif
}

template <class T>
void InVector<T>::Индекс(int q, int n)
{
	for(int i = 0; i < индекс.дайСчёт(); i++)
		индекс[i].по(q >>= 1, 0) += n;
}

template <class T>
T *InVector<T>::вставь0(int ii, int blki, int pos, int off, const T *val)
{
	if(данные[blki].дайСчёт() > blk_high) {
		if(slave)
			Slave()->разбей(blki, данные[blki].дайСчёт() / 2);
		Вектор<T>& x = данные.вставь(blki + 1);
		x.вставьРазбей(0, данные[blki], данные[blki].дайСчёт() / 2);
		данные[blki].сожми();
		реиндексируй();
		pos = ii;
		blki = FindBlock(pos, off);
	}
	LLOG("blki: " << blki << ", pos: " << pos);
	count++;
	if(slave) {
		Slave()->счёт(1);
		Slave()->Индекс(blki, 1);
		Slave()->вставь(blki, pos);
	}
	Индекс(blki, 1);
	if(val)
		данные[blki].вставь(pos, *val);
	else
		данные[blki].вставь(pos);
	SetCache(blki, off);
	return &данные[blki][pos];
}

template <class T>
T *InVector<T>::вставь0(int ii, const T *val)
{
	ПРОВЕРЬ(ii >= 0 && ii <= дайСчёт());
	if(данные.дайСчёт() == 0) {
		count++;
		очистьКэш();
		if(slave) {
			Slave()->Count(1);
			Slave()->AddFirst();
		}
		if(val) {
			данные.добавь().добавь(*val);
			return &данные[0][0];
		}
		return &данные.добавь().добавь();
	}
	int pos = ii;
	int off;
	int blki = FindBlock(pos, off);
	return вставь0(ii, blki, pos, off, val);
}

template <class T>
template <class Диапазон>
void InVector<T>::Insert_(int ii, const Диапазон& r, bool опр)
{
	int n = r.дайСчёт();

	ПРОВЕРЬ(ii >= 0 && ii <= дайСчёт() && n >= 0 && !slave);

	if(n <= 0)
		return;
	
	auto s = r.begin();

	if(данные.дайСчёт() == 0) {
		int m = (blk_high + blk_low) / 2;
		count = n;
		while(n > 0) {
			int n1 = min(m, n);
			if(опр)
				данные.добавь().устСчёт(n1);
			else
				данные.добавь().приставьДиапазон(СубДиапазон(s, n1));
			s += n1;
			n -= n1;
		}
		реиндексируй();
		return;
	}

	int pos = ii;
	int off;
	int blki = FindBlock(pos, off);
	int bc = данные[blki].дайСчёт();
	
	count += n;

	if(bc + n < blk_high) { // block will not be bigger than threshold after insert
		if(опр)
			данные[blki].вставьН(pos, n);
		else
			данные[blki].вставьДиапазон(pos, СубДиапазон(s, n));
		Индекс(blki, n);
		SetCache(blki, off);
	}
	else
	if(bc - pos + n < blk_high) { // splitting into 2 blocks is enough
		Вектор<T>& t = данные.вставь(blki + 1);
		if(опр)
			t.вставьН(0, n);
		else
			t.вставьДиапазон(0, СубДиапазон(s, n));
		t.вставьРазбей(n, данные[blki], pos);
		данные[blki].сожми();
		реиндексируй();
	}
	else { // need to insert several blocks
		int m = (blk_high + blk_low) / 2;
		int bn = (n + m - 1) / m;
		int ti;
		if(pos) { // need to split first block
			ti = blki + 1; // TODO should add some of данные to splitted blocks
			данные.вставьН(ti, bn + 1);
			данные[ti + bn].вставьРазбей(0, данные[blki], pos);
			данные[blki].сожми();
		}
		else {
			ti = blki;
			данные.вставьН(ti, bn);
		}
		for(int i = 0; i < bn; i++) {
			int q = min(m, n);
			if(опр)
				данные[ti + i].устСчёт(q);
			else
				данные[ti + i].приставьДиапазон(СубДиапазон(s, q));
			s += q;
			n -= q;
		}
		ПРОВЕРЬ(n == 0);
		реиндексируй();
	}
#ifdef flagIVTEST
	Check(0, 0);
#endif
}

template <class T>
void InVector<T>::Join(int blki)
{
	данные[blki].приставьПодбор(pick(данные[blki + 1]));
	данные.удали(blki + 1);
}

template <class T>
force_inline bool InVector<T>::JoinSmall(int blki)
{
	if(blki < данные.дайСчёт()) {
		int n = данные[blki].дайСчёт();
		if(n == 0) {
			if(slave)
				Slave()->RemoveBlk(blki, 1);
			данные.удали(blki);
			return true;
		}
		if(n < blk_low) {
			if(blki > 0 && данные[blki - 1].дайСчёт() + n <= blk_high) {
				if(slave)
					Slave()->Join(blki - 1);
				Join(blki - 1);
				return true;
			}
			if(blki + 1 < данные.дайСчёт() && n + данные[blki + 1].дайСчёт() <= blk_high) {
				if(slave)
					Slave()->Join(blki);
				Join(blki);
				return true;
			}
		}
	}
	return false;
}

template <class T>
void InVector<T>::удали(int pos, int n)
{
	ПРОВЕРЬ(pos >= 0 && pos + n <= дайСчёт());
	if(n == 0)
		return;
	int off;
	int blki = FindBlock(pos, off);
	count -= n;
	if(slave)
		Slave()->Count(-n);
	if(pos + n < данные[blki].дайСчёт()) {
		if(slave)
			Slave()->удали(blki, pos, n);
		данные[blki].удали(pos, n);
		if(JoinSmall(blki))
			реиндексируй();
		else {
			if(slave)
				Slave()->Индекс(blki, -n);
			Индекс(blki, -n);
			SetCache(blki, off);
		}
	}
	else {
		int b1 = blki;
		int nn = min(n, данные[b1].дайСчёт() - pos);
		if(slave)
			Slave()->удали(b1, pos, nn);
		данные[b1++].удали(pos, nn);
		n -= nn;
		int b2 = b1;
		while(b2 < данные.дайСчёт() && n >= данные[b2].дайСчёт()) {
			n -= min(n, данные[b2].дайСчёт());
			b2++;
		}
		if(slave)
			Slave()->RemoveBlk(b1, b2 - b1);
		данные.удали(b1, b2 - b1);
		if(b1 < данные.дайСчёт()) {
			if(slave)
				Slave()->удали(b1, 0, n);
			данные[b1].удали(0, n);
		}
		JoinSmall(blki + 1);
		JoinSmall(blki);
		реиндексируй();
	}
#ifdef flagIVTEST
	Check(0, 0);
#endif
}

template <class T>
void InVector<T>::устСчёт(int n)
{
	if(n < дайСчёт())
		обрежь(n);
	else
		вставьН(дайСчёт(), n - дайСчёт());
}

template <class T>
void InVector<T>::сожми()
{
	for(int i = 0; i < данные.дайСчёт(); i++)
		данные[i].сожми();
	данные.сожми();
	for(int i = 0; i < индекс.дайСчёт(); i++)
		индекс[i].сожми();
	индекс.сожми();
}

template <class T>
void InVector<T>::уст(int i, const T& x, int count)
{
	Обходчик it = дайОбх(i);
	while(count-- > 0)
		*it++ = x;
}

template <class T>
InVector<T>::InVector(const InVector<T>& v, int)
{
	данные <<= v.данные;
	индекс <<= v.индекс;
	count = v.count;
	hcount = v.hcount;
	blk_high = v.blk_high;
	blk_low = v.blk_low;
	serial = NewInVectorSerial();
	slave = v.slave;
}

template <class T>
void InVector<T>::подбери(InVector&& v)
{
	данные = pick(v.данные);
	индекс = pick(v.индекс);
	count = v.count;
	hcount = v.hcount;
	blk_high = v.blk_high;
	blk_low = v.blk_low;
	serial = v.serial;
	slave = v.slave;

	v.иниц();
}


template <class T>
template <class L>
int InVector<T>::найдиВерхнГран(const T& val, const L& less, int& off, int& pos) const
{
	if(данные.дайСчёт() == 0) {
		pos = off = 0;
		return 0;
	}
	int blki = 0;
	int ii = 0;
	int offset = 0;
	int half = hcount;
	for(int i = индекс.дайСчёт(); --i >= 0;) {
		int m = blki + half;
		if(m - 1 < данные.дайСчёт() && !less(val, данные[m - 1].верх())) {
			blki = m;
			offset += индекс[i][ii];
			ii++;
		}
		ii += ii;
		half >>= 1;
	}
#ifdef flagIVTEST
	Check(blki, offset);
#endif
	if(blki < данные.дайСчёт()) {
		if(!less(val, данные[blki].верх()))
			offset += данные[blki++].дайСчёт();
		if(blki < данные.дайСчёт()) {
			pos = РНЦПДинрус::найдиВерхнГран(данные[blki], val, less);
			off = offset;
			SetCache(blki, offset);
			return blki;
		}
	}
	pos = данные.верх().дайСчёт();
	off = count - pos;
	blki--;
	SetCache(blki, off);
	return blki;
}

template <class T>
template <class L>
int InVector<T>::найдиНижнГран(const T& val, const L& less, int& off, int& pos) const
{
	if(данные.дайСчёт() == 0) {
		pos = off = 0;
		return 0;
	}
	int blki = 0;
	int ii = 0;
	int offset = 0;
	int half = hcount;
	for(int i = индекс.дайСчёт(); --i >= 0;) {
		int m = blki + half;
		if(m < данные.дайСчёт() && less(данные[m][0], val)) {
			blki = m;
			offset += индекс[i][ii];
			ii++;
		}
		ii += ii;
		half >>= 1;
	}
#ifdef flagIVTEST
	Check(blki, offset);
#endif
	if(blki < данные.дайСчёт()) {
		if(blki + 1 < данные.дайСчёт() && less(данные[blki + 1][0], val))
			offset += данные[blki++].дайСчёт();
		if(blki < данные.дайСчёт()) {
			pos = РНЦПДинрус::найдиНижнГран(данные[blki], val, less);
			off = offset;
			SetCache(blki, offset);
			return blki;
		}
	}
	pos = данные.верх().дайСчёт();
	off = count - pos;
	blki--;
	SetCache(blki, off);
	return blki;
}

template <class T>
template <class L>
int InVector<T>::InsertUpperBound(const T& val, const L& less)
{
	if(данные.дайСчёт() == 0) {
		count++;
		очистьКэш();
		if(slave) {
			Slave()->счёт(1);
			Slave()->AddFirst();
		}
		данные.добавь().вставь(0) = val;
		return 0;
	}
	int off;
	int pos;
	int blki = найдиВерхнГран(val, less, off, pos);
	вставь0(off + pos, blki, pos, off, &val);
	return off + pos;
}

template <class T>
template <class L>
int InVector<T>::найди(const T& val, const L& less) const
{
	int i = найдиНижнГран(val, less);
	return i < дайСчёт() && !less(val, (*this)[i]) ? i : -1;
}


template <class T>
void InVector<T>::SetIter(КонстОбходчик& it, int ii) const
{
	if(count) {
		it.v = this;
		it.blki = FindBlock(ii, it.offset);
		it.begin = данные[it.blki].старт();
		it.end = данные[it.blki].стоп();
		it.ptr = it.begin + ii;
	}
	else
		SetEnd(it);
}

template <class T>
void InVector<T>::SetBegin(КонстОбходчик& it) const
{
	if(count) {
		it.v = this;
		it.blki = 0;
		it.ptr = it.begin = данные[0].старт();
		it.end = данные[0].стоп();
		it.offset = 0;
	}
	else
		SetEnd(it);
}

template <class T>
void InVector<T>::SetEnd(КонстОбходчик& it) const
{
	if(count) {
		it.v = this;
		it.blki = данные.дайСчёт() - 1;
		it.begin = данные.верх().старт();
		it.ptr = it.end = данные.верх().стоп();
		it.offset = count - данные.верх().дайСчёт();
	}
	else {
		it.v = this;
		it.blki = 0;
		it.ptr = it.begin = it.end = NULL;
		it.offset = 0;		
	}
}

template <typename T>
force_inline typename InVector<T>::КонстОбходчик& InVector<T>::КонстОбходчик::operator+=(int d)
{
	if(d >= 0 ? d < end - ptr : -d < ptr - begin)
		ptr += d;
	else
		v->SetIter(*this, дайИндекс() + d);
	ПРОВЕРЬ(end - begin == v->данные[blki].дайСчёт());
	return *this;
}

template <typename T>
void InVector<T>::КонстОбходчик::NextBlk()
{
	ПРОВЕРЬ(end - begin == v->данные[blki].дайСчёт());
	if(blki + 1 < v->данные.дайСчёт()) {
		offset += v->данные[blki].дайСчёт();
		++blki;
		ptr = begin = v->данные[blki].старт();
		end = v->данные[blki].стоп();
	}
}

template <typename T>
void InVector<T>::КонстОбходчик::PrevBlk()
{
	--blki;
	begin = v->данные[blki].старт();
	ptr = end = v->данные[blki].стоп();
	offset -= v->данные[blki].дайСчёт();
}

template <typename T>
void InVector<T>::разверни(InVector& b)
{
	РНЦПДинрус::разверни(данные, b.данные);
	РНЦПДинрус::разверни(индекс, b.индекс);
	РНЦПДинрус::разверни(count, b.count);
	РНЦПДинрус::разверни(hcount, b.hcount);
	РНЦПДинрус::разверни(serial, b.serial);
	РНЦПДинрус::разверни(blk_high, b.blk_high);
	РНЦПДинрус::разверни(blk_low, b.blk_low);
	РНЦПДинрус::разверни(slave, b.slave);
}

template <class T>
void InVector<T>::вРяр(РярВВ& xio, const char *itemtag)
{
	контейнерВРяр(xio, itemtag, *this);
}

template <class T>
void InVector<T>::вДжейсон(ДжейсонВВ& jio)
{
	JsonizeArray<InVector<T>>(jio, *this);
}

template <class T>
Ткст InVector<T>::вТкст() const
{
	return AsStringArray(*this);
}

template <class T>
void InVector<T>::DumpIndex() const
{
	Ткст h;
	RLOG("------- InVector dump, count: " << дайСчёт() << ", индекс depth: " << индекс.дайСчёт());
	int64 alloc = 0;
	for(int i = 0; i < данные.дайСчёт(); i++) {
		if(i)
			h << ", ";
		h << данные[i].дайСчёт() << " (" << данные[i].дайРазмест() << ")";
		alloc += данные[i].дайРазмест();
	}
	RLOG("Данные blocks: " << данные.дайСчёт() << ", sizeof: " << данные.дайСчёт() * sizeof(Вектор<T>));
	RLOG("Total alloc: " << alloc);
	RLOG(h);
	for(int j = 0; j < индекс.дайСчёт(); j++) {
		h.очисть();
		h << индекс[j].дайСчёт() << ": ";
		for(int k = 0; k < индекс[j].дайСчёт(); k++) {
			if(k)
				h << ", ";
			h << индекс[j][k];
		}
		RLOG(h);
	}
	RLOG(".");
}

#ifdef flagIVTEST
template <class T>
void InVector<T>::Check(int blki, int offset) const
{
	int off = 0;
	int all = 0;
	for(int i = 0; i < данные.дайСчёт(); i++) {
		if(i < blki)
			off += данные[i].дайСчёт();
		all += данные[i].дайСчёт();
	}
	ПРОВЕРЬ(off == offset);
	ПРОВЕРЬ(all == count);
}
#endif

template <class T>
void InArray<T>::Delete(IVIter it, int count)
{
	ПРОВЕРЬ(count >= 0);
	while(count--)
		delete (T *)*it++;
}

template <class T>
void InArray<T>::Delete(int i, int count)
{
	Delete(iv.дайОбх(i), count);
}

template <class T>
void InArray<T>::иниц(int i, int count)
{
	ПРОВЕРЬ(count >= 0);
	IVIter it = iv.дайОбх(i);
	while(count--)
		*it++ = new T;
}

template <class T>
void InArray<T>::вставьН(int i, int count)
{
	iv.вставьН(i, count);
	иниц(i, count);
}

template <class T>
template <class Диапазон>
void InArray<T>::вставьДиапазон(int i, const Диапазон& r)
{
	int count = r.дайСчёт();
	iv.вставьН(i, count);
	IVIter it = iv.begin() + i;
	auto s = r.begin();
	while(count--)
		*it++ = new T(*s++);
}

template <class T>
void InArray<T>::удали(int i, int count)
{
	Delete(i, count);
	iv.удали(i, count);
}

template <class T>
void InArray<T>::устСчёт(int n)
{
	if(n < дайСчёт())
		обрежь(n);
	else
		вставьН(дайСчёт(), n - дайСчёт());
}

template <class T>
void InArray<T>::очисть()
{
	освободи();
	iv.очисть();
}

template <class T>
void InArray<T>::уст(int i, const T& x, int count)
{
	Обходчик it = дайОбх(i);
	while(count-- > 0)
		*it++ = x;
}

template <class T>
void InArray<T>::SetIter(КонстОбходчик& it, int ii) const
{
	it.it = iv.дайОбх(ii);
}

template <class T>
void InArray<T>::SetBegin(КонстОбходчик& it) const
{
	it.it = iv.старт();
}

template <class T>
void InArray<T>::SetEnd(КонстОбходчик& it) const
{
	it.it = iv.стоп();
}

template <class T>
InArray<T>::InArray(const InArray& v, int)
{
	int n = v.дайСчёт();
	iv.устСчёт(v.дайСчёт());
	КонстОбходчик s = v.старт();
	IVIter it = iv.старт();
	while(n--)
		*it++ = new T(clone(*s++));
}

#ifdef РНЦП
template <class T>
void InArray<T>::вРяр(РярВВ& xio, const char *itemtag)
{
	контейнерВРяр(xio, itemtag, *this);
}

template <class T>
void InArray<T>::вДжейсон(ДжейсонВВ& jio)
{
	JsonizeArray<InArray<T>>(jio, *this);
}

template <class T>
Ткст InArray<T>::вТкст() const
{
	return AsStringArray(*this);
}

#endif

#ifdef LLOG
#undef  LLOG
#endif