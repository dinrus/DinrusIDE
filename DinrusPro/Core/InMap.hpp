template <class T, class Less>
цел ОтсортИндекс<T, Less>::найдиДобавь(const T& ключ)
{
	цел i = найдиНижнГран(ключ);
	if(i == дайСчёт() || Less()(ключ, iv[i]))
		iv.вставь(i, ключ);
	return i;
}

template <class T, class Less>
цел ОтсортИндекс<T, Less>::найдиСледщ(цел i) const
{
	return i + 1 < iv.дайСчёт() && !Less()(iv[i], iv[i + 1]) ? i + 1 : -1;
}

template <class T, class Less>
цел ОтсортИндекс<T, Less>::найдиПоследн(const T& x) const
{
	цел i = iv.найдиВерхнГран(x, Less());
	return i > 0 && !Less()(iv[i - 1], x) ? i - 1 : -1;
}

template <class T, class Less>
цел ОтсортИндекс<T, Less>::найдиПредш(цел i) const
{
	return i > 0 && !Less()(iv[i - 1], iv[i]) ? i - 1 : -1;
}

template <class T, class Less>
цел ОтсортИндекс<T, Less>::удалиКлюч(const T& x)
{
	цел l = найдиНижнГран(x);
	цел count = найдиВерхнГран(x) - l;
	удали(l, count);
	return count;
}

template <class T, class Less>
Ткст ОтсортИндекс<T, Less>::вТкст() const
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
проц Slaved_InVector__<T>::добавьПервый()
{
	данные.данные.добавь().добавь();
	res = &данные.данные[0][0];
}

template <class K, class T, class Less, class Данные>
Ткст ОтсортАМап<K, T, Less, Данные>::вТкст() const
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
ОтсортВекторМап<K, T, Less>::ОтсортВекторМап(ОтсортВекторМап&& s)
{
	B::ключ = пикуй(s.ключ);
	B::значение.данные = пикуй(s.значение.данные);
	B::SetSlave();
}

template <class K, class T, class Less>
ОтсортВекторМап<K, T, Less>& ОтсортВекторМап<K, T, Less>::operator=(ОтсортВекторМап&& s)
{
	B::ключ = пикуй(s.ключ);
	B::значение.данные = пикуй(s.значение.данные);
	B::SetSlave();
	return *this;
}

template <class K, class T, class Less>
ОтсортВекторМап<K, T, Less>::ОтсортВекторМап(const ОтсортВекторМап& s, цел)
{
	B::ключ = клонируй(s.ключ);
	B::значение.данные = клонируй(s.значение.данные);
	B::SetSlave();
}

template <class K, class T, class Less>
цел ОтсортВекторМап<K, T, Less>::найдиДобавь(const K& k, const T& init)
{
	B::значение.res = NULL;
	цел q = B::ключ.найдиДобавь(k);
	if(B::значение.res)
		*B::значение.res = init;
	return q;
}

template <class K, class T, class Less>
проц ОтсортВекторМап<K, T, Less>::разверни(ОтсортВекторМап& x)
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
	Вектор< typename ВхоМассив<T>::ТипУказатель >& x = данные.iv.данные.вставь(blki + 1);
	x.вставьРазбей(0, данные.iv.данные[blki], nextsize);
}

template <class T>
проц Slaved_InArray__<T>::удали(цел blki, цел pos, цел n)
{
	Вектор< typename ВхоМассив<T>::ТипУказатель >& b = данные.iv.данные[blki];
	for(цел i = 0; i < n; i++)
		if(b[i + pos])
			delete (T *)b[i + pos];
	b.удали(pos, n);
}

template <class T>
проц Slaved_InArray__<T>::добавьПервый()
{
	if(!res)
		res = new T;
	данные.iv.данные.добавь().добавь(res);
}

template <class K, class T, class Less>
цел ОтсортМассивМап<K, T, Less>::найдиДобавь(const K& k, const T& init)
{
	B::значение.res = NULL;
	цел q = B::ключ.найдиДобавь(k);
	if(B::значение.res)
		*B::значение.res = init;
	return q;
}

template <class K, class T, class Less>
ОтсортМассивМап<K, T, Less>::ОтсортМассивМап(ОтсортМассивМап&& s)
{
	B::ключ = пикуй(s.ключ);
	B::значение.данные = пикуй(s.значение.данные);
	B::SetSlave();
}

template <class K, class T, class Less>
ОтсортМассивМап<K, T, Less>& ОтсортМассивМап<K, T, Less>::operator=(ОтсортМассивМап&& s)
{
	B::ключ = пикуй(s.ключ);
	B::значение.данные = пикуй(s.значение.данные);
	B::SetSlave();
	return *this;
}

template <class K, class T, class Less>
ОтсортМассивМап<K, T, Less>::ОтсортМассивМап(const ОтсортМассивМап& s, цел)
{
	B::ключ = клонируй(s.ключ);
	B::значение.данные = клонируй(s.значение.данные);
	B::SetSlave();
}

template <class K, class T, class Less>
проц ОтсортМассивМап<K, T, Less>::разверни(ОтсортМассивМап& x)
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
проц ОтсортВекторМап<K, T, Less>::сериализуй(Поток& s) {
	StreamSortedMap<K, ОтсортВекторМап<K, T, Less> >(s, *this);
}

template <class K, class T, class Less>
проц ОтсортВекторМап<K, T, Less>::вРяр(РярВВ& xio)
{
	XmlizeSortedMap<K, T, ОтсортВекторМап<K, T, Less> >(xio, "ключ", "значение", *this);
}

template <class K, class T, class Less>
проц ОтсортВекторМап<K, T, Less>::вДжейсон(ДжейсонВВ& jio)
{
	джейсонируйОтсортМап<ОтсортВекторМап<K, T, Less>, K, T>(jio, *this, "ключ", "значение");
}

template <class K, class T, class Less>
проц ОтсортМассивМап<K, T, Less>::сериализуй(Поток& s) {
	StreamSortedMap<K, ОтсортМассивМап<K, T, Less> >(s, *this);
}

template <class K, class T, class Less>
проц ОтсортМассивМап<K, T, Less>::вРяр(РярВВ& xio)
{
	XmlizeSortedMap<K, T, ОтсортМассивМап<K, T, Less> >(xio, "ключ", "значение", *this);
}

template <class K, class T, class Less>
проц ОтсортМассивМап<K, T, Less>::вДжейсон(ДжейсонВВ& jio)
{
	джейсонируйОтсортМап<ОтсортМассивМап<K, T, Less>, K, T>(jio, *this, "ключ", "значение");
}

#endif
