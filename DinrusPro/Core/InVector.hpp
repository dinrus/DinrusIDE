#define LLOG(x)   // DLOG(x)

дол NewInVectorSerial();

template <class T>
проц ВхоВектор<T>::иниц()
{
	serial = NewInVectorSerial();
	slave = 0;
	переустанов();
}

template <class T>
ВхоВектор<T>::ВхоВектор()
{
	иниц();
}

template <class T>
проц ВхоВектор<T>::переустанов()
{
	hcount = count = 0;
	устПарБлк();
}

template <class T>
проц ВхоВектор<T>::очисть()
{
	if(slave)
		Slave()->очисть();
	данные.очисть();
	индекс.очисть();
	переустанов();
	очистьКэш();
}

проц SetInvectorCache__(дол serial, цел blki, цел offset, цел end);
проц ClearInvectorCache__();
цел  FindInvectorCache__(дол serial, цел& pos, цел& off);

template <class T>
форс_инлайн проц ВхоВектор<T>::устКэш(цел blki, цел offset) const
{
#ifdef flagIVTEST
	Check(0, 0);
#endif
	SetInvectorCache__(serial, blki, offset, offset + данные[blki].дайСчёт());
}

template <class T>
форс_инлайн проц ВхоВектор<T>::очистьКэш() const
{
	ClearInvectorCache__();
}

template <class T>
форс_инлайн цел ВхоВектор<T>::найдиБлок(цел& pos, цел& off) const
{
	цел i = FindInvectorCache__(serial, pos, off);
	return i >= 0 ? i : найдиБлок0(pos, off);
}

template <class T>
цел ВхоВектор<T>::найдиБлок0(цел& pos, цел& off) const
{
	LLOG("найдиБлок " << pos);
	ПРОВЕРЬ(pos >= 0 && pos <= count);
	if(pos == count) {
		LLOG("Found last");
		pos = данные.верх().дайСчёт();
		off = count - pos;
		return данные.дайСчёт() - 1;
	}
	цел blki = 0;
	цел offset = 0;
	for(цел i = индекс.дайСчёт(); --i >= 0;) {
		цел n = индекс[i][blki];
		if(pos >= n) {
			blki++;
			pos -= n;
			offset += n;
		}
		blki += blki;
	}
	цел n = данные[blki].дайСчёт();
	if(pos >= n) {
		blki++;
		pos -= n;
		offset += n;
	}

	устКэш(blki, offset);

	off = offset;
	return blki;
}

template <class T>
форс_инлайн цел ВхоВектор<T>::найдиБлок(цел& pos) const
{
	цел h;
	return найдиБлок(pos, h);
}

template <class T>
const T& ВхоВектор<T>::operator[](цел i) const
{
	LLOG("operator[] " << i);
	ПРОВЕРЬ(i >= 0 && i < count);
	цел blki = найдиБлок(i);
	return данные[blki][i];
}

template <class T>
T& ВхоВектор<T>::operator[](цел i)
{
	LLOG("operator[] " << i);
	ПРОВЕРЬ(i >= 0 && i < count);
	цел blki = найдиБлок(i);
	return данные[blki][i];
}

template <class T>
проц ВхоВектор<T>::реиндексируй()
{
	LLOG("--- Reindexing");
	очистьКэш();
	устПарБлк();
	индекс.очисть();
	if(slave)
		Slave()->реиндексируй();
	hcount = 0;
	Вектор<T> *ds = данные.старт();
	Вектор<T> *dend = данные.стоп();
	цел n = данные.дайСчёт();
	if(n <= 2)
		return;
	Вектор<цел>& w = индекс.добавь();
	hcount = 2;
	w.устСчёт((n + 1) >> 1);
	цел *t = w.старт();
	while(ds != dend) {
		*t = (ds++)->дайСчёт();
		if(ds == dend)
			break;
		*t++ += (ds++)->дайСчёт();
	}
	цел *s = w.старт();
	цел *end = w.стоп();
	n = w.дайСчёт();
	while(n > 2) {
		Вектор<цел>& w = индекс.добавь();
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
проц ВхоВектор<T>::устПарБлк()
{
#if defined(_ОТЛАДКА) && defined(flagIVTEST)
	blk_high = 11;
	blk_low = 5;
#else
	цел n = 2500 + данные.дайСчёт() / 4;
	blk_high = минмакс(n / (цел)sizeof(T), 16, 65000);
	blk_low = минмакс(n / 3 / (цел)sizeof(T), 16, 65000);
#endif
}

template <class T>
проц ВхоВектор<T>::Индекс(цел q, цел n)
{
	for(цел i = 0; i < индекс.дайСчёт(); i++)
		индекс[i].по(q >>= 1, 0) += n;
}

template <class T>
T *ВхоВектор<T>::вставь0(цел ii, цел blki, цел pos, цел off, const T *знач)
{
	if(данные[blki].дайСчёт() > blk_high) {
		if(slave)
			Slave()->разбей(blki, данные[blki].дайСчёт() / 2);
		Вектор<T>& x = данные.вставь(blki + 1);
		x.вставьРазбей(0, данные[blki], данные[blki].дайСчёт() / 2);
		данные[blki].сожми();
		реиндексируй();
		pos = ii;
		blki = найдиБлок(pos, off);
	}
	LLOG("blki: " << blki << ", pos: " << pos);
	count++;
	if(slave) {
		Slave()->счёт(1);
		Slave()->Индекс(blki, 1);
		Slave()->вставь(blki, pos);
	}
	Индекс(blki, 1);
	if(знач)
		данные[blki].вставь(pos, *знач);
	else
		данные[blki].вставь(pos);
	устКэш(blki, off);
	return &данные[blki][pos];
}

template <class T>
T *ВхоВектор<T>::вставь0(цел ii, const T *знач)
{
	ПРОВЕРЬ(ii >= 0 && ii <= дайСчёт());
	if(данные.дайСчёт() == 0) {
		count++;
		очистьКэш();
		if(slave) {
			Slave()->Count(1);
			Slave()->добавьПервый();
		}
		if(знач) {
			данные.добавь().добавь(*знач);
			return &данные[0][0];
		}
		return &данные.добавь().добавь();
	}
	цел pos = ii;
	цел off;
	цел blki = найдиБлок(pos, off);
	return вставь0(ii, blki, pos, off, знач);
}

template <class T>
template <class Диапазон>
проц ВхоВектор<T>::Insert_(цел ii, const Диапазон& r, бул опр)
{
	цел n = r.дайСчёт();

	ПРОВЕРЬ(ii >= 0 && ii <= дайСчёт() && n >= 0 && !slave);

	if(n <= 0)
		return;
	
	auto s = r.begin();

	if(данные.дайСчёт() == 0) {
		цел m = (blk_high + blk_low) / 2;
		count = n;
		while(n > 0) {
			цел n1 = мин(m, n);
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

	цел pos = ii;
	цел off;
	цел blki = найдиБлок(pos, off);
	цел bc = данные[blki].дайСчёт();
	
	count += n;

	if(bc + n < blk_high) { // block will not be bigger than threshold after insert
		if(опр)
			данные[blki].вставьН(pos, n);
		else
			данные[blki].вставьДиапазон(pos, СубДиапазон(s, n));
		Индекс(blki, n);
		устКэш(blki, off);
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
		цел m = (blk_high + blk_low) / 2;
		цел bn = (n + m - 1) / m;
		цел ti;
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
		for(цел i = 0; i < bn; i++) {
			цел q = мин(m, n);
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
проц ВхоВектор<T>::соедини(цел blki)
{
	данные[blki].приставьПодбор(пикуй(данные[blki + 1]));
	данные.удали(blki + 1);
}

template <class T>
форс_инлайн бул ВхоВектор<T>::JoinSmall(цел blki)
{
	if(blki < данные.дайСчёт()) {
		цел n = данные[blki].дайСчёт();
		if(n == 0) {
			if(slave)
				Slave()->удалиБлок(blki, 1);
			данные.удали(blki);
			return true;
		}
		if(n < blk_low) {
			if(blki > 0 && данные[blki - 1].дайСчёт() + n <= blk_high) {
				if(slave)
					Slave()->соедини(blki - 1);
				соедини(blki - 1);
				return true;
			}
			if(blki + 1 < данные.дайСчёт() && n + данные[blki + 1].дайСчёт() <= blk_high) {
				if(slave)
					Slave()->соедини(blki);
				соедини(blki);
				return true;
			}
		}
	}
	return false;
}

template <class T>
проц ВхоВектор<T>::удали(цел pos, цел n)
{
	ПРОВЕРЬ(pos >= 0 && pos + n <= дайСчёт());
	if(n == 0)
		return;
	цел off;
	цел blki = найдиБлок(pos, off);
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
			устКэш(blki, off);
		}
	}
	else {
		цел b1 = blki;
		цел nn = мин(n, данные[b1].дайСчёт() - pos);
		if(slave)
			Slave()->удали(b1, pos, nn);
		данные[b1++].удали(pos, nn);
		n -= nn;
		цел b2 = b1;
		while(b2 < данные.дайСчёт() && n >= данные[b2].дайСчёт()) {
			n -= мин(n, данные[b2].дайСчёт());
			b2++;
		}
		if(slave)
			Slave()->удалиБлок(b1, b2 - b1);
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
проц ВхоВектор<T>::устСчёт(цел n)
{
	if(n < дайСчёт())
		обрежь(n);
	else
		вставьН(дайСчёт(), n - дайСчёт());
}

template <class T>
проц ВхоВектор<T>::сожми()
{
	for(цел i = 0; i < данные.дайСчёт(); i++)
		данные[i].сожми();
	данные.сожми();
	for(цел i = 0; i < индекс.дайСчёт(); i++)
		индекс[i].сожми();
	индекс.сожми();
}

template <class T>
проц ВхоВектор<T>::уст(цел i, const T& x, цел count)
{
	Обходчик it = дайОбх(i);
	while(count-- > 0)
		*it++ = x;
}

template <class T>
ВхоВектор<T>::ВхоВектор(const ВхоВектор<T>& v, цел)
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
проц ВхоВектор<T>::подбери(ВхоВектор&& v)
{
	данные = пикуй(v.данные);
	индекс = пикуй(v.индекс);
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
цел ВхоВектор<T>::найдиВерхнГран(const T& знач, const L& less, цел& off, цел& pos) const
{
	if(данные.дайСчёт() == 0) {
		pos = off = 0;
		return 0;
	}
	цел blki = 0;
	цел ii = 0;
	цел offset = 0;
	цел half = hcount;
	for(цел i = индекс.дайСчёт(); --i >= 0;) {
		цел m = blki + half;
		if(m - 1 < данные.дайСчёт() && !less(знач, данные[m - 1].верх())) {
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
		if(!less(знач, данные[blki].верх()))
			offset += данные[blki++].дайСчёт();
		if(blki < данные.дайСчёт()) {
			pos = найдиВерхнГран(данные[blki], знач, less);
			off = offset;
			устКэш(blki, offset);
			return blki;
		}
	}
	pos = данные.верх().дайСчёт();
	off = count - pos;
	blki--;
	устКэш(blki, off);
	return blki;
}

template <class T>
template <class L>
цел ВхоВектор<T>::найдиНижнГран(const T& знач, const L& less, цел& off, цел& pos) const
{
	if(данные.дайСчёт() == 0) {
		pos = off = 0;
		return 0;
	}
	цел blki = 0;
	цел ii = 0;
	цел offset = 0;
	цел half = hcount;
	for(цел i = индекс.дайСчёт(); --i >= 0;) {
		цел m = blki + half;
		if(m < данные.дайСчёт() && less(данные[m][0], знач)) {
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
		if(blki + 1 < данные.дайСчёт() && less(данные[blki + 1][0], знач))
			offset += данные[blki++].дайСчёт();
		if(blki < данные.дайСчёт()) {
			pos = найдиНижнГран(данные[blki], знач, less);
			off = offset;
			устКэш(blki, offset);
			return blki;
		}
	}
	pos = данные.верх().дайСчёт();
	off = count - pos;
	blki--;
	устКэш(blki, off);
	return blki;
}

template <class T>
template <class L>
цел ВхоВектор<T>::вставьВерхнГран(const T& знач, const L& less)
{
	if(данные.дайСчёт() == 0) {
		count++;
		очистьКэш();
		if(slave) {
			Slave()->счёт(1);
			Slave()->добавьПервый();
		}
		данные.добавь().вставь(0) = знач;
		return 0;
	}
	цел off;
	цел pos;
	цел blki = найдиВерхнГран(знач, less, off, pos);
	вставь0(off + pos, blki, pos, off, &знач);
	return off + pos;
}

template <class T>
template <class L>
цел ВхоВектор<T>::найди(const T& знач, const L& less) const
{
	цел i = найдиНижнГран(знач, less);
	return i < дайСчёт() && !less(знач, (*this)[i]) ? i : -1;
}


template <class T>
проц ВхоВектор<T>::устОбх(КонстОбходчик& it, цел ii) const
{
	if(count) {
		it.v = this;
		it.blki = найдиБлок(ii, it.offset);
		it.begin = данные[it.blki].старт();
		it.end = данные[it.blki].стоп();
		it.укз = it.begin + ii;
	}
	else
		устКон(it);
}

template <class T>
проц ВхоВектор<T>::устНач(КонстОбходчик& it) const
{
	if(count) {
		it.v = this;
		it.blki = 0;
		it.укз = it.begin = данные[0].старт();
		it.end = данные[0].стоп();
		it.offset = 0;
	}
	else
		устКон(it);
}

template <class T>
проц ВхоВектор<T>::устКон(КонстОбходчик& it) const
{
	if(count) {
		it.v = this;
		it.blki = данные.дайСчёт() - 1;
		it.begin = данные.верх().старт();
		it.укз = it.end = данные.верх().стоп();
		it.offset = count - данные.верх().дайСчёт();
	}
	else {
		it.v = this;
		it.blki = 0;
		it.укз = it.begin = it.end = NULL;
		it.offset = 0;		
	}
}

template <typename T>
форс_инлайн typename ВхоВектор<T>::КонстОбходчик& ВхоВектор<T>::КонстОбходчик::operator+=(цел d)
{
	if(d >= 0 ? d < end - укз : -d < укз - begin)
		укз += d;
	else
		v->устОбх(*this, дайИндекс() + d);
	ПРОВЕРЬ(end - begin == v->данные[blki].дайСчёт());
	return *this;
}

template <typename T>
проц ВхоВектор<T>::КонстОбходчик::следщБлк()
{
	ПРОВЕРЬ(end - begin == v->данные[blki].дайСчёт());
	if(blki + 1 < v->данные.дайСчёт()) {
		offset += v->данные[blki].дайСчёт();
		++blki;
		укз = begin = v->данные[blki].старт();
		end = v->данные[blki].стоп();
	}
}

template <typename T>
проц ВхоВектор<T>::КонстОбходчик::предшБлк()
{
	--blki;
	begin = v->данные[blki].старт();
	укз = end = v->данные[blki].стоп();
	offset -= v->данные[blki].дайСчёт();
}

template <typename T>
проц ВхоВектор<T>::разверни(ВхоВектор& b)
{
	разверни(данные, b.данные);
	разверни(индекс, b.индекс);
	разверни(count, b.count);
	разверни(hcount, b.hcount);
	разверни(serial, b.serial);
	разверни(blk_high, b.blk_high);
	разверни(blk_low, b.blk_low);
	разверни(slave, b.slave);
}

template <class T>
проц ВхоВектор<T>::вРяр(РярВВ& xio, кткст0 itemtag)
{
	контейнерВРяр(xio, itemtag, *this);
}

template <class T>
проц ВхоВектор<T>::вДжейсон(ДжейсонВВ& jio)
{
	джейсонируйМассив<ВхоВектор<T>>(jio, *this);
}

template <class T>
Ткст ВхоВектор<T>::вТкст() const
{
	return AsStringArray(*this);
}

template <class T>
проц ВхоВектор<T>::DumpIndex() const
{
	Ткст h;
	RLOG("------- ВхоВектор dump, count: " << дайСчёт() << ", индекс depth: " << индекс.дайСчёт());
	дол alloc = 0;
	for(цел i = 0; i < данные.дайСчёт(); i++) {
		if(i)
			h << ", ";
		h << данные[i].дайСчёт() << " (" << данные[i].дайРазмест() << ")";
		alloc += данные[i].дайРазмест();
	}
	RLOG("Данные blocks: " << данные.дайСчёт() << ", sizeof: " << данные.дайСчёт() * sizeof(Вектор<T>));
	RLOG("Total alloc: " << alloc);
	RLOG(h);
	for(цел j = 0; j < индекс.дайСчёт(); j++) {
		h.очисть();
		h << индекс[j].дайСчёт() << ": ";
		for(цел k = 0; k < индекс[j].дайСчёт(); k++) {
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
проц ВхоВектор<T>::Check(цел blki, цел offset) const
{
	цел off = 0;
	цел all = 0;
	for(цел i = 0; i < данные.дайСчёт(); i++) {
		if(i < blki)
			off += данные[i].дайСчёт();
		all += данные[i].дайСчёт();
	}
	ПРОВЕРЬ(off == offset);
	ПРОВЕРЬ(all == count);
}
#endif

template <class T>
проц ВхоМассив<T>::Delete(IVIter it, цел count)
{
	ПРОВЕРЬ(count >= 0);
	while(count--)
		delete (T *)*it++;
}

template <class T>
проц ВхоМассив<T>::Delete(цел i, цел count)
{
	Delete(iv.дайОбх(i), count);
}

template <class T>
проц ВхоМассив<T>::иниц(цел i, цел count)
{
	ПРОВЕРЬ(count >= 0);
	IVIter it = iv.дайОбх(i);
	while(count--)
		*it++ = new T;
}

template <class T>
проц ВхоМассив<T>::вставьН(цел i, цел count)
{
	iv.вставьН(i, count);
	иниц(i, count);
}

template <class T>
template <class Диапазон>
проц ВхоМассив<T>::вставьДиапазон(цел i, const Диапазон& r)
{
	цел count = r.дайСчёт();
	iv.вставьН(i, count);
	IVIter it = iv.begin() + i;
	auto s = r.begin();
	while(count--)
		*it++ = new T(*s++);
}

template <class T>
проц ВхоМассив<T>::удали(цел i, цел count)
{
	Delete(i, count);
	iv.удали(i, count);
}

template <class T>
проц ВхоМассив<T>::устСчёт(цел n)
{
	if(n < дайСчёт())
		обрежь(n);
	else
		вставьН(дайСчёт(), n - дайСчёт());
}

template <class T>
проц ВхоМассив<T>::очисть()
{
	освободи();
	iv.очисть();
}

template <class T>
проц ВхоМассив<T>::уст(цел i, const T& x, цел count)
{
	Обходчик it = дайОбх(i);
	while(count-- > 0)
		*it++ = x;
}

template <class T>
проц ВхоМассив<T>::устОбх(КонстОбходчик& it, цел ii) const
{
	it.it = iv.дайОбх(ii);
}

template <class T>
проц ВхоМассив<T>::устНач(КонстОбходчик& it) const
{
	it.it = iv.старт();
}

template <class T>
проц ВхоМассив<T>::устКон(КонстОбходчик& it) const
{
	it.it = iv.стоп();
}

template <class T>
ВхоМассив<T>::ВхоМассив(const ВхоМассив& v, цел)
{
	цел n = v.дайСчёт();
	iv.устСчёт(v.дайСчёт());
	КонстОбходчик s = v.старт();
	IVIter it = iv.старт();
	while(n--)
		*it++ = new T(клонируй(*s++));
}

#ifdef РНЦП
template <class T>
проц ВхоМассив<T>::вРяр(РярВВ& xio, кткст0 itemtag)
{
	контейнерВРяр(xio, itemtag, *this);
}

template <class T>
проц ВхоМассив<T>::вДжейсон(ДжейсонВВ& jio)
{
	джейсонируйМассив<ВхоМассив<T>>(jio, *this);
}

template <class T>
Ткст ВхоМассив<T>::вТкст() const
{
	return AsStringArray(*this);
}

#endif

#ifdef LLOG
#undef  LLOG
#endif