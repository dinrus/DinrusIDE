template <class T, class Less>
цел SortedIndex<T, Less>::найдиДобавь(const T& ключ)
{
	цел i = найдиНижнГран(ключ);
	if(i == дайСчёт() || Less()(ключ, iv[i]))
		iv.вставь(i, ключ);
	return i;
}

template <class T, class Less>
цел SortedIndex<T, Less>::найдиСледщ(цел i) const
{
	return i + 1 < iv.дайСчёт() && !Less()(iv[i], iv[i + 1]) ? i + 1 : -1;
}

template <class T, class Less>
цел SortedIndex<T, Less>::найдиПоследн(const T& x) const
{
	цел i = iv.найдиВерхнГран(x, Less());
	return i > 0 && !Less()(iv[i - 1], x) ? i - 1 : -1;
}

template <class T, class Less>
цел SortedIndex<T, Less>::найдиПредш(цел i) const
{
	return i > 0 && !Less()(iv[i - 1], iv[i]) ? i - 1 : -1;
}

template <class T, class Less>
цел SortedIndex<T, Less>::удалиКлюч(const T& x)
{
	цел l = найдиНижнГран(x);
	цел count = найдиВерхнГран(x) - l;
	удали(l, count);
	return count;
}

template <class T, class Less>
Ткст SortedIndex<T, Less>::вТкст() const
{
	return AsStringArray(*this);
}

template <class T>
проц Slaved_InVector__<T>::вставь(цел blki, цел pos)
{
	данные.данные[blki].вставь(pos);
	res = &данные.данные[blki][pos];
}

template <class T>
проц Slaved_InVector__<T>::разбей(цел blki, цел nextsize)
{
	Вектор<T>& x = данные.данные.вставь(blki + 1);
	x.вставьРазбей(0, данные.данные[blki], nextsize);
	данные.данные[blki].сожми();
}

template <class T>
проц Slaved_InVector__<T>::AddFirst()
{
	данные.данные.добавь().добавь();
	res = &данные.данные[0][0];
}

template <class K, class T, class Less, class Данные>
Ткст SortedAMap<K, T, Less, Данные>::вТкст() const
{
	Ткст r;
	r = "{";
	for(цел i = 0; i < дайСчёт(); i++) {
		if(i)
			r << ", ";
		r << дайКлюч(i) << ": " << (*this)[i];
	}
	r << '}';
	return r;
}

template <class K, class T, class Less>
SortedVectorMap<K, T, Less>::SortedVectorMap(SortedVectorMap&& s)
{
	B::ключ = pick(s.ключ);
	B::значение.данные = pick(s.значение.данные);
	B::SetSlave();
}

template <class K, class T, class Less>
SortedVectorMap<K, T, Less>& SortedVectorMap<K, T, Less>::operator=(SortedVectorMap&& s)
{
	B::ключ = pick(s.ключ);
	B::значение.данные = pick(s.значение.данные);
	B::SetSlave();
	return *this;
}

template <class K, class T, class Less>
SortedVectorMap<K, T, Less>::SortedVectorMap(const SortedVectorMap& s, цел)
{
	B::ключ = клонируй(s.ключ);
	B::значение.данные = клонируй(s.значение.данные);
	B::SetSlave();
}

template <class K, class T, class Less>
цел SortedVectorMap<K, T, Less>::найдиДобавь(const K& k, const T& init)
{
	B::значение.res = NULL;
	цел q = B::ключ.найдиДобавь(k);
	if(B::значение.res)
		*B::значение.res = init;
	return q;
}

template <class K, class T, class Less>
проц SortedVectorMap<K, T, Less>::разверни(SortedVectorMap& x)
{
	B::значение.данные.разверни(x.значение.данные);
	B::ключ.разверни(x.B::ключ);
	B::SetSlave();
	x.SetSlave();
}
               
template <class T>
проц Slaved_InArray__<T>::вставь(цел blki, цел pos)
{
	if(!res)
		res = new T;
	данные.iv.данные[blki].вставь(pos, res);
}

template <class T>
проц Slaved_InArray__<T>::разбей(цел blki, цел nextsize)
{
	Вектор< typename InArray<T>::ТипУказатель >& x = данные.iv.данные.вставь(blki + 1);
	x.вставьРазбей(0, данные.iv.данные[blki], nextsize);
}

template <class T>
проц Slaved_InArray__<T>::удали(цел blki, цел pos, цел n)
{
	Вектор< typename InArray<T>::ТипУказатель >& b = данные.iv.данные[blki];
	for(цел i = 0; i < n; i++)
		if(b[i + pos])
			delete (T *)b[i + pos];
	b.удали(pos, n);
}

template <class T>
проц Slaved_InArray__<T>::AddFirst()
{
	if(!res)
		res = new T;
	данные.iv.данные.добавь().добавь(res);
}

template <class K, class T, class Less>
цел SortedArrayMap<K, T, Less>::найдиДобавь(const K& k, const T& init)
{
	B::значение.res = NULL;
	цел q = B::ключ.найдиДобавь(k);
	if(B::значение.res)
		*B::значение.res = init;
	return q;
}

template <class K, class T, class Less>
SortedArrayMap<K, T, Less>::SortedArrayMap(SortedArrayMap&& s)
{
	B::ключ = pick(s.ключ);
	B::значение.данные = pick(s.значение.данные);
	B::SetSlave();
}

template <class K, class T, class Less>
SortedArrayMap<K, T, Less>& SortedArrayMap<K, T, Less>::operator=(SortedArrayMap&& s)
{
	B::ключ = pick(s.ключ);
	B::значение.данные = pick(s.значение.данные);
	B::SetSlave();
	return *this;
}

template <class K, class T, class Less>
SortedArrayMap<K, T, Less>::SortedArrayMap(const SortedArrayMap& s, цел)
{
	B::ключ = клонируй(s.ключ);
	B::значение.данные = клонируй(s.значение.данные);
	B::SetSlave();
}

template <class K, class T, class Less>
проц SortedArrayMap<K, T, Less>::разверни(SortedArrayMap& x)
{
	B::значение.данные.разверни(x.значение.данные);
	B::ключ.разверни(x.B::ключ);
	B::SetSlave();
	x.SetSlave();
}

#ifdef РНЦП
template <class K, class T>
проц StreamSortedMap(Поток& s, T& cont)
{
	цел n = cont.дайСчёт();
	s / n;
	if(n < 0) {
		s.загрузиОш();
		return;
	}
	if(s.грузится()) {
		cont.очисть();
		while(n--) {
			K ключ;
			s % ключ;
			s % cont.добавь(ключ);
		}
	}
	else
		for(цел i = 0; i < cont.дайСчёт(); i++) {
			K ключ = cont.дайКлюч(i);
			s % ключ;
			s % cont[i];
		}
}

template <class K, class T, class Less>
проц SortedVectorMap<K, T, Less>::сериализуй(Поток& s) {
	StreamSortedMap<K, SortedVectorMap<K, T, Less> >(s, *this);
}

template <class K, class T, class Less>
проц SortedVectorMap<K, T, Less>::вРяр(РярВВ& xio)
{
	XmlizeSortedMap<K, T, SortedVectorMap<K, T, Less> >(xio, "ключ", "значение", *this);
}

template <class K, class T, class Less>
проц SortedVectorMap<K, T, Less>::вДжейсон(ДжейсонВВ& jio)
{
	JsonizeSortedMap<SortedVectorMap<K, T, Less>, K, T>(jio, *this, "ключ", "значение");
}

template <class K, class T, class Less>
проц SortedArrayMap<K, T, Less>::сериализуй(Поток& s) {
	StreamSortedMap<K, SortedArrayMap<K, T, Less> >(s, *this);
}

template <class K, class T, class Less>
проц SortedArrayMap<K, T, Less>::вРяр(РярВВ& xio)
{
	XmlizeSortedMap<K, T, SortedArrayMap<K, T, Less> >(xio, "ключ", "значение", *this);
}

template <class K, class T, class Less>
проц SortedArrayMap<K, T, Less>::вДжейсон(ДжейсонВВ& jio)
{
	JsonizeSortedMap<SortedArrayMap<K, T, Less>, K, T>(jio, *this, "ключ", "значение");
}

#endif
