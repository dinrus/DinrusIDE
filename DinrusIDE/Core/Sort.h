template <class I, class Less>
force_inline
void OrderIter2__(I a, I b, const Less& less)
{
	if(less(*b, *a))
		IterSwap(a, b);
}

template <class I, class Less>
force_inline // 2-3 compares, 0-2 swaps
void OrderIter3__(I x, I y, I z, const Less& less)
{
    if(less(*y, *x)) {
	    if(less(*z, *y))
	        IterSwap(x, z);
	    else {
		    IterSwap(x, y);
		    if(less(*z, *y))
		        IterSwap(y, z);
	    }
    }
    else
    if(less(*z, *y)) {
        IterSwap(y, z);
        if(less(*y, *x))
            IterSwap(x, y);
    }
}

template <class I, class Less>
force_inline // 3-6 compares, 0-5 swaps
void OrderIter4__(I x, I y, I z, I u, const Less& less)
{
	OrderIter3__(x, y, z, less);
	if(less(*z, *u)) return;
	IterSwap(z, u);
	if(less(*y, *z)) return;
	IterSwap(y, z);
	if(less(*x, *y)) return;
	IterSwap(x, y);
}

template <class I, class Less>
force_inline // 4-10 compares, 0-9 swaps
void OrderIter5__(I x, I y, I z, I u, I v, const Less& less)
{
	OrderIter4__(x, y, z, u, less);
	if(less(*u, *v)) return;
	IterSwap(u, v);
	if(less(*z, *u)) return;
	IterSwap(z, u);
	if(less(*y, *z)) return;
	IterSwap(y, z);
	if(less(*x, *y)) return;
	IterSwap(x, y);
}

template <class I, class Less>
force_inline // 9-15 compares, 0-14 swaps
void OrderIter6__(I x, I y, I z, I u, I v, I w, const Less& less)
{
	OrderIter5__(x, y, z, u, v, less);
	if(less(*v, *w)) return;
	IterSwap(v, w);
	if(less(*u, *v)) return;
	IterSwap(u, v);
	if(less(*z, *u)) return;
	IterSwap(z, u);
	if(less(*y, *z)) return;
	IterSwap(y, z);
	if(less(*x, *y)) return;
	IterSwap(x, y);
}

template <class I, class Less>
force_inline // 16-26 compares, 0-20 swaps
void OrderIter7__(I x, I y, I z, I u, I v, I w, I q, const Less& less)
{
	OrderIter6__(x, y, z, u, v, w, less);
	if(less(*w, *q)) return;
	IterSwap(w, q);
	if(less(*v, *w)) return;
	IterSwap(v, w);
	if(less(*u, *v)) return;
	IterSwap(u, v);
	if(less(*z, *u)) return;
	IterSwap(z, u);
	if(less(*y, *z)) return;
	IterSwap(y, z);
	if(less(*x, *y)) return;
	IterSwap(x, y);
}

dword  ัะปัั(dword n);

template <class I, class Less>
void Sort__(I l, I h, const Less& less)
{
	int count = int(h - l);
	I middle = l + (count >> 1);        // get the middle element
	for(;;) {
		switch(count) {
		case 0:
		case 1: return;
		case 2: OrderIter2__(l, l + 1, less); return;
		case 3: OrderIter3__(l, l + 1, l + 2, less); return;
		case 4: OrderIter4__(l, l + 1, l + 2, l + 3, less); return;
		case 5: OrderIter5__(l, l + 1, l + 2, l + 3, l + 4, less); return;
		case 6: OrderIter6__(l, l + 1, l + 2, l + 3, l + 4, l + 5, less); return;
		case 7: OrderIter7__(l, l + 1, l + 2, l + 3, l + 4, l + 5, l + 6, less); return;
		}
		if(count > 1000) { // median of 5, 2 of them random elements
			middle = l + (count >> 1); // iterators cannot point to the same object!
			I q = l + 1 + (int)ัะปัั((count >> 1) - 2);
			I w = middle + 1 + (int)ัะปัั((count >> 1) - 2);
			OrderIter5__(l, q, middle, w, h - 1, less);
		}
		else // median of 3
			OrderIter3__(l, middle, h - 1, less);

		I pivot = h - 2;
		IterSwap(pivot, middle); // move median pivot to h - 2
		I i = l;
		I j = h - 2; // l, h - 2, h - 1 already sorted above
		for(;;) { // Hoareโs partition (modified):
			while(less(*++i, *pivot));
			do
				if(!(i < j)) goto done;
			while(!less(*--j, *pivot));
			IterSwap(i, j);
		}
	done:
		IterSwap(i, h - 2);                 // put pivot back in between partitions

		I ih = i;
		while(ih + 1 != h && !less(*i, *(ih + 1))) // ะฝะฐะนะดะธ middle range of elements equal to pivot
			++ih;

		int count_l = int(i - l);
		if(count_l == 1) // this happens if there are many elements equal to pivot, filter them out
			for(I q = ih + 1; q != h; ++q)
				if(!less(*i, *q))
					IterSwap(++ih, q);

		int count_h = int(h - ih) - 1;

		if(count_l < count_h) {       // recurse on smaller partition, tail on larger
			Sort__(l, i, less);
			l = ih + 1;
			count = count_h;
		}
		else {
			Sort__(ih + 1, h, less);
			h = i;
			count = count_l;
		}

		if(count > 8 && min(count_l, count_h) < (max(count_l, count_h) >> 2)) // If unbalanced, randomize the next step
			middle = l + 1 + (int)ัะปัั(count - 2); // ัะปัั pivot selection
		else
			middle = l + (count >> 1); // the middle is probably still the best guess otherwise
	}
}

template <class ะะธะฐะฟะฐะทะพะฝ, class Less>
void ัะพััะธััะน(ะะธะฐะฟะฐะทะพะฝ&& c, const Less& less)
{
	Sort__(c.begin(), c.end(), less);
}

template <class ะะธะฐะฟะฐะทะพะฝ>
void ัะพััะธััะน(ะะธะฐะฟะฐะทะพะฝ&& c)
{
	Sort__(c.begin(), c.end(), std::less<ัะธะฟะะฝะฐัะฃ<ะะธะฐะฟะฐะทะพะฝ>>());
}

template <class T>
struct StableSortItem__ {
	const T& ะทะฝะฐัะตะฝะธะต;
	int      ะธะฝะดะตะบั;

	StableSortItem__(const T& ะทะฝะฐัะตะฝะธะต, int ะธะฝะดะตะบั) : ะทะฝะฐัะตะฝะธะต(ะทะฝะฐัะตะฝะธะต), ะธะฝะดะตะบั(ะธะฝะดะตะบั) {}
};

template <class II, class T>
struct StableSortIterator__ {
	II          ii;
	int        *vi;

	typedef StableSortIterator__<II, T> ะะฑั;

	ะะฑั&       operator ++ ()               { ++ii; ++vi; return *this; }
	ะะฑั&       operator -- ()               { --ii; --vi; return *this; }
	ะะฑั        operator +  (int i) const    { return ะะฑั(ii + i, vi + i); }
	ะะฑั        operator -  (int i) const    { return ะะฑั(ii - i, vi - i); }
	int         operator -  (ะะฑั b) const   { return (int)(ii - b.ii); }
	bool        operator == (ะะฑั b) const   { return ii == b.ii; }
	bool        operator != (ะะฑั b) const   { return ii != b.ii; }
	bool        operator <  (ะะฑั b) const   { return ii <  b.ii; }
	bool        operator <= (ะะฑั b) const   { return ii <=  b.ii; }

	StableSortItem__<T> operator*() const    { return StableSortItem__<T>(*ii, *vi); }

	friend void IterSwap(ะะฑั a, ะะฑั b)     { IterSwap(a.ii, b.ii); IterSwap(a.vi, b.vi); }

	StableSortIterator__(II ii, int *vi) : ii(ii), vi(vi) {}
};

template <class T, class Less>
struct StableSortLess__ {
	const Less& less;

	bool operator()(const StableSortItem__<T>& a, const StableSortItem__<T>& b) const {
		if(less(a.ะทะฝะฐัะตะฝะธะต, b.ะทะฝะฐัะตะฝะธะต)) return true;
		return less(b.ะทะฝะฐัะตะฝะธะต, a.ะทะฝะฐัะตะฝะธะต) ? false : a.ะธะฝะดะตะบั < b.ะธะฝะดะตะบั;
	}

	StableSortLess__(const Less& less) : less(less) {}
};

template <class ะะธะฐะฟะฐะทะพะฝ, class Less>
void StableSort(ะะธะฐะฟะฐะทะพะฝ&& r, const Less& less)
{
	auto begin = r.begin();
	auto end = r.end();
	typedef ัะธะฟะะฝะฐัะฃ<ะะธะฐะฟะฐะทะพะฝ> VT;
	typedef decltype(begin) I;
	int count = (int)(uintptr_t)(end - begin);
	ะััะตั<int> h(count);
	for(int i = 0; i < count; i++)
		h[i] = i;
	Sort__(StableSortIterator__<I, VT>(begin, ~h), StableSortIterator__<I, VT>(end, ~h + count),
	       StableSortLess__<VT, Less>(less));
}

template <class ะะธะฐะฟะฐะทะพะฝ>
void StableSort(ะะธะฐะฟะฐะทะพะฝ&& r)
{
	StableSort(r, std::less<ัะธะฟะะฝะฐัะฃ<ะะธะฐะฟะฐะทะพะฝ>>());
}

template <class II, class VI, class K>
struct ะะฝะดะตะบัะกะพััะะฑัะพะดัะธะบ__
{
	typedef ะะฝะดะตะบัะกะพััะะฑัะพะดัะธะบ__<II, VI, K> ะะฑั;

	ะะฝะดะตะบัะกะพััะะฑัะพะดัะธะบ__(II ii, VI vi) : ii(ii), vi(vi) {}

	ะะฑั&       operator ++ ()               { ++ii; ++vi; return *this; }
	ะะฑั&       operator -- ()               { --ii; --vi; return *this; }
	const K&    operator *  () const         { return *ii; }
	ะะฑั        operator +  (int i) const    { return ะะฑั(ii + i, vi + i); }
	ะะฑั        operator -  (int i) const    { return ะะฑั(ii - i, vi - i); }
	int         operator -  (ะะฑั b) const   { return (int)(ii - b.ii); }
	bool        operator == (ะะฑั b) const   { return ii == b.ii; }
	bool        operator != (ะะฑั b) const   { return ii != b.ii; }
	bool        operator <  (ะะฑั b) const   { return ii <  b.ii; }
	bool        operator <= (ะะฑั b) const   { return ii <=  b.ii; }
	friend void IterSwap    (ะะฑั a, ะะฑั b) { IterSwap(a.ii, b.ii); IterSwap(a.vi, b.vi); }

	II          ii;
	VI          vi;
};

template <class ะะฐััะตัะะธะฐะฟะฐะทะพะฝ, class ะะธะฐะฟะฐะทะพะฝ2, class Less>
void IndexSort(ะะฐััะตัะะธะฐะฟะฐะทะพะฝ&& r, ะะธะฐะฟะฐะทะพะฝ2&& r2, const Less& less)
{
	ะะ?ะะะะ?ะฌ(r.ะดะฐะนะกััั() == r2.ะดะฐะนะกััั());
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef ัะธะฟะะฝะฐัะฃ<ะะฐััะตัะะธะฐะฟะฐะทะพะฝ> VT;
	if(r.ะดะฐะนะกััั() == 0)
		return;
	Sort__(ะะฝะดะตะบัะกะพััะะฑัะพะดัะธะบ__<I, I2, VT>(r.begin(), r2.begin()),
	       ะะฝะดะตะบัะกะพััะะฑัะพะดัะธะบ__<I, I2, VT>(r.end(), r2.end()),
		   less);
}

template <class ะะฐััะตัะะธะฐะฟะฐะทะพะฝ, class ะะธะฐะฟะฐะทะพะฝ2>
void IndexSort(ะะฐััะตัะะธะฐะฟะฐะทะพะฝ&& r, ะะธะฐะฟะฐะทะพะฝ2&& r2)
{
	IndexSort(r, r2, std::less<ัะธะฟะะฝะฐัะฃ<ะะฐััะตัะะธะฐะฟะฐะทะพะฝ>>());
}

template <class ะะฐััะตัะะธะฐะฟะฐะทะพะฝ, class ะะธะฐะฟะฐะทะพะฝ2, class Less>
void StableIndexSort(ะะฐััะตัะะธะฐะฟะฐะทะพะฝ&& r, ะะธะฐะฟะฐะทะพะฝ2&& r2, const Less& less)
{
	ะะ?ะะะะ?ะฌ(r.ะดะฐะนะกััั() == r2.ะดะฐะนะกััั());
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef ัะธะฟะะฝะฐัะฃ<ะะฐััะตัะะธะฐะฟะฐะทะพะฝ> VT;
	if(r.ะดะฐะนะกััั() == 0)
		return;
	StableSort(ะกัะฑะะธะฐะฟะฐะทะพะฝ(ะะฝะดะตะบัะกะพััะะฑัะพะดัะธะบ__<I, I2, VT>(r.begin(), r2.begin()),
		                ะะฝะดะตะบัะกะพััะะฑัะพะดัะธะบ__<I, I2, VT>(r.end(), r2.end())).ะฟะธัะธ(),
	           less);
}

template <class ะะฐััะตัะะธะฐะฟะฐะทะพะฝ, class ะะธะฐะฟะฐะทะพะฝ2>
void StableIndexSort(ะะฐััะตัะะธะฐะฟะฐะทะพะฝ&& r, ะะธะฐะฟะฐะทะพะฝ2&& r2)
{
	StableIndexSort(r, r2, std::less<ัะธะฟะะฝะฐัะฃ<ะะฐััะตัะะธะฐะฟะฐะทะพะฝ>>());
}

template <class II, class VI, class WI, class K>
struct ะะฝะดะตะบัะกะพัั2ะะฑัะพะดัะธะบ__
{
	typedef ะะฝะดะตะบัะกะพัั2ะะฑัะพะดัะธะบ__<II, VI, WI, K> ะะฑั;

	ะะฝะดะตะบัะกะพัั2ะะฑัะพะดัะธะบ__(II ii, VI vi, WI wi) : ii(ii), vi(vi), wi(wi) {}

	ะะฑั&       operator ++ ()               { ++ii; ++vi; ++wi; return *this; }
	ะะฑั&       operator -- ()               { --ii; --vi; --wi; return *this; }
	const K&    operator *  () const         { return *ii; }
	ะะฑั        operator +  (int i) const    { return ะะฑั(ii + i, vi + i, wi + i); }
	ะะฑั        operator -  (int i) const    { return ะะฑั(ii - i, vi - i, wi - i); }
	int         operator -  (ะะฑั b) const   { return (int)(ii - b.ii); }
	bool        operator == (ะะฑั b) const   { return ii == b.ii; }
	bool        operator != (ะะฑั b) const   { return ii != b.ii; }
	bool        operator <  (ะะฑั b) const   { return ii <  b.ii; }
	bool        operator <= (ะะฑั b) const   { return ii <=  b.ii; }
	friend void IterSwap    (ะะฑั a, ะะฑั b) { IterSwap(a.ii, b.ii); IterSwap(a.vi, b.vi); IterSwap(a.wi, b.wi); }

	II          ii;
	VI          vi;
	WI          wi;
};

template <class ะะฐััะตัะะธะฐะฟะฐะทะพะฝ, class ะะธะฐะฟะฐะทะพะฝ2, class ะะธะฐะฟะฐะทะพะฝ3, class Less>
void IndexSort2(ะะฐััะตัะะธะฐะฟะฐะทะพะฝ&& r, ะะธะฐะฟะฐะทะพะฝ2&& r2, ะะธะฐะฟะฐะทะพะฝ3&& r3, const Less& less)
{
	ะะ?ะะะะ?ะฌ(r.ะดะฐะนะกััั() == r2.ะดะฐะนะกััั());
	ะะ?ะะะะ?ะฌ(r.ะดะฐะนะกััั() == r3.ะดะฐะนะกััั());
	if(r.ะดะฐะนะกััั() == 0)
		return;
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef decltype(r3.begin()) I3;
	typedef ัะธะฟะะฝะฐัะฃ<ะะฐััะตัะะธะฐะฟะฐะทะพะฝ> VT;
	Sort__(ะะฝะดะตะบัะกะพัั2ะะฑัะพะดัะธะบ__<I, I2, I3, VT>(r.begin(), r2.begin(), r3.begin()),
	       ะะฝะดะตะบัะกะพัั2ะะฑัะพะดัะธะบ__<I, I2, I3, VT>(r.end(), r2.end(), r3.end()),
		   less);
}

template <class ะะฐััะตัะะธะฐะฟะฐะทะพะฝ, class ะะธะฐะฟะฐะทะพะฝ2, class ะะธะฐะฟะฐะทะพะฝ3>
void IndexSort2(ะะฐััะตัะะธะฐะฟะฐะทะพะฝ&& r, ะะธะฐะฟะฐะทะพะฝ2&& r2, ะะธะฐะฟะฐะทะพะฝ3&& r3)
{
	IndexSort2(r, r2, r3, std::less<ัะธะฟะะฝะฐัะฃ<ะะฐััะตัะะธะฐะฟะฐะทะพะฝ>>());
}

template <class ะะฐััะตัะะธะฐะฟะฐะทะพะฝ, class ะะธะฐะฟะฐะทะพะฝ2, class ะะธะฐะฟะฐะทะพะฝ3, class Less>
void StableIndexSort2(ะะฐััะตัะะธะฐะฟะฐะทะพะฝ&& r, ะะธะฐะฟะฐะทะพะฝ2&& r2, ะะธะฐะฟะฐะทะพะฝ3&& r3, const Less& less)
{
	ะะ?ะะะะ?ะฌ(r.ะดะฐะนะกััั() == r2.ะดะฐะนะกััั());
	ะะ?ะะะะ?ะฌ(r.ะดะฐะนะกััั() == r3.ะดะฐะนะกััั());
	if(r.ะดะฐะนะกััั() == 0)
		return;
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef decltype(r3.begin()) I3;
	typedef ัะธะฟะะฝะฐัะฃ<ะะฐััะตัะะธะฐะฟะฐะทะพะฝ> VT;
	StableSort(ะกัะฑะะธะฐะฟะฐะทะพะฝ(ะะฝะดะตะบัะกะพัั2ะะฑัะพะดัะธะบ__<I, I2, I3, VT>(r.begin(), r2.begin(), r3.begin()),
	                    ะะฝะดะตะบัะกะพัั2ะะฑัะพะดัะธะบ__<I, I2, I3, VT>(r.end(), r2.end(), r3.end())).ะฟะธัะธ(),
		       less);
}

template <class ะะฐััะตัะะธะฐะฟะฐะทะพะฝ, class ะะธะฐะฟะฐะทะพะฝ2, class ะะธะฐะฟะฐะทะพะฝ3>
void StableIndexSort2(ะะฐััะตัะะธะฐะฟะฐะทะพะฝ&& r, ะะธะฐะฟะฐะทะพะฝ2&& r2, ะะธะฐะฟะฐะทะพะฝ3&& r3)
{
	StableIndexSort2(r, r2, r3, std::less<ัะธะฟะะฝะฐัะฃ<ะะฐััะตัะะธะฐะฟะฐะทะพะฝ>>());
}

template <class II, class VI, class WI, class XI, class K>
struct ะะฝะดะตะบัะกะพัั3ะะฑัะพะดัะธะบ__
{
	typedef ะะฝะดะตะบัะกะพัั3ะะฑัะพะดัะธะบ__<II, VI, WI, XI, K> ะะฑั;

	ะะฝะดะตะบัะกะพัั3ะะฑัะพะดัะธะบ__(II ii, VI vi, WI wi, XI xi) : ii(ii), vi(vi), wi(wi), xi(xi) {}

	ะะฑั&       operator ++ ()               { ++ii; ++vi; ++wi; ++xi; return *this; }
	ะะฑั&       operator -- ()               { --ii; --vi; --wi; --xi; return *this; }
	const K&    operator *  () const         { return *ii; }
	ะะฑั        operator +  (int i) const    { return ะะฑั(ii + i, vi + i, wi + i, xi + i); }
	ะะฑั        operator -  (int i) const    { return ะะฑั(ii - i, vi - i, wi - i, xi - i); }
	int         operator -  (ะะฑั b) const   { return (int)(ii - b.ii); }
	bool        operator == (ะะฑั b) const   { return ii == b.ii; }
	bool        operator != (ะะฑั b) const   { return ii != b.ii; }
	bool        operator <  (ะะฑั b) const   { return ii <  b.ii; }
	bool        operator <= (ะะฑั b) const   { return ii <=  b.ii; }
	friend void IterSwap    (ะะฑั a, ะะฑั b) { IterSwap(a.ii, b.ii); IterSwap(a.vi, b.vi); IterSwap(a.wi, b.wi); IterSwap(a.xi, b.xi); }

	II          ii;
	VI          vi;
	WI          wi;
	XI          xi;
};

template <class ะะฐััะตัะะธะฐะฟะฐะทะพะฝ, class ะะธะฐะฟะฐะทะพะฝ2, class ะะธะฐะฟะฐะทะพะฝ3, class ะะธะฐะฟะฐะทะพะฝ4, class Less>
void IndexSort3(ะะฐััะตัะะธะฐะฟะฐะทะพะฝ&& r, ะะธะฐะฟะฐะทะพะฝ2&& r2, ะะธะฐะฟะฐะทะพะฝ3&& r3, ะะธะฐะฟะฐะทะพะฝ4&& r4, const Less& less)
{
	ะะ?ะะะะ?ะฌ(r.ะดะฐะนะกััั() == r2.ะดะฐะนะกััั());
	ะะ?ะะะะ?ะฌ(r.ะดะฐะนะกััั() == r3.ะดะฐะนะกััั());
	ะะ?ะะะะ?ะฌ(r.ะดะฐะนะกััั() == r4.ะดะฐะนะกััั());
	if(r.ะดะฐะนะกััั() == 0)
		return;
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef decltype(r3.begin()) I3;
	typedef decltype(r4.begin()) I4;
	typedef ัะธะฟะะฝะฐัะฃ<ะะฐััะตัะะธะฐะฟะฐะทะพะฝ> VT;
	Sort__(ะะฝะดะตะบัะกะพัั3ะะฑัะพะดัะธะบ__<I, I2, I3, I4, VT>(r.begin(), r2.begin(), r3.begin(), r4.begin()),
	       ะะฝะดะตะบัะกะพัั3ะะฑัะพะดัะธะบ__<I, I2, I3, I4, VT>(r.end(), r2.end(), r3.end(), r4.end()),
		   less);
}

template <class ะะฐััะตัะะธะฐะฟะฐะทะพะฝ, class ะะธะฐะฟะฐะทะพะฝ2, class ะะธะฐะฟะฐะทะพะฝ3, class ะะธะฐะฟะฐะทะพะฝ4>
void IndexSort3(ะะฐััะตัะะธะฐะฟะฐะทะพะฝ&& r, ะะธะฐะฟะฐะทะพะฝ2&& r2, ะะธะฐะฟะฐะทะพะฝ3&& r3, ะะธะฐะฟะฐะทะพะฝ4&& r4)
{
	IndexSort3(r, r2, r3, r4, std::less<ัะธะฟะะฝะฐัะฃ<ะะฐััะตัะะธะฐะฟะฐะทะพะฝ>>());
}

template <class ะะฐััะตัะะธะฐะฟะฐะทะพะฝ, class ะะธะฐะฟะฐะทะพะฝ2, class ะะธะฐะฟะฐะทะพะฝ3, class ะะธะฐะฟะฐะทะพะฝ4, class Less>
void StableIndexSort3(ะะฐััะตัะะธะฐะฟะฐะทะพะฝ&& r, ะะธะฐะฟะฐะทะพะฝ2&& r2, ะะธะฐะฟะฐะทะพะฝ3&& r3, ะะธะฐะฟะฐะทะพะฝ4&& r4, const Less& less)
{
	ะะ?ะะะะ?ะฌ(r.ะดะฐะนะกััั() == r2.ะดะฐะนะกััั());
	ะะ?ะะะะ?ะฌ(r.ะดะฐะนะกััั() == r3.ะดะฐะนะกััั());
	ะะ?ะะะะ?ะฌ(r.ะดะฐะนะกััั() == r4.ะดะฐะนะกััั());
	if(r.ะดะฐะนะกััั() == 0)
		return;
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef decltype(r3.begin()) I3;
	typedef decltype(r4.begin()) I4;
	typedef ัะธะฟะะฝะฐัะฃ<ะะฐััะตัะะธะฐะฟะฐะทะพะฝ> VT;
	StableSort(ะกัะฑะะธะฐะฟะฐะทะพะฝ(ะะฝะดะตะบัะกะพัั3ะะฑัะพะดัะธะบ__<I, I2, I3, I4, VT>(r.begin(), r2.begin(), r3.begin(), r4.begin()),
	                    ะะฝะดะตะบัะกะพัั3ะะฑัะพะดัะธะบ__<I, I2, I3, I4, VT>(r.end(), r2.end(), r3.end(), r4.end())).ะฟะธัะธ(),
		       less);
}

template <class ะะฐััะตัะะธะฐะฟะฐะทะพะฝ, class ะะธะฐะฟะฐะทะพะฝ2, class ะะธะฐะฟะฐะทะพะฝ3, class ะะธะฐะฟะฐะทะพะฝ4>
void StableIndexSort3(ะะฐััะตัะะธะฐะฟะฐะทะพะฝ&& r, ะะธะฐะฟะฐะทะพะฝ2&& r2, ะะธะฐะฟะฐะทะพะฝ3&& r3, ะะธะฐะฟะฐะทะพะฝ4&& r4)
{
	StableIndexSort3(r, r2, r3, r4, std::less<ัะธะฟะะฝะฐัะฃ<ะะฐััะตัะะธะฐะฟะฐะทะพะฝ>>());
}

template <class I, class V>
struct ะะฑัะพะดัะธะบะะพััะดะบะฐะกะพััะฐ__ : PostfixOps< ะะฑัะพะดัะธะบะะพััะดะบะฐะกะพััะฐ__<I, V> >
{
	typedef ะะฑัะพะดัะธะบะะพััะดะบะฐะกะพััะฐ__<I, V> ะะฑั;

	ะะฑัะพะดัะธะบะะพััะดะบะฐะกะพััะฐ__(int *ii, I vi) : ii(ii), vi(vi) {}

	ะะฑั&       operator ++ ()               { ++ii; return *this; }
	ะะฑั&       operator -- ()               { --ii; return *this; }
	const V&    operator *  () const         { return *(vi + *ii); }
	ะะฑั        operator +  (int i) const    { return ะะฑั(ii + i, vi); }
	ะะฑั        operator -  (int i) const    { return ะะฑั(ii - i, vi); }
	int         operator -  (ะะฑั b) const   { return int(ii - b.ii); }
	bool        operator == (ะะฑั b) const   { return ii == b.ii; }
	bool        operator != (ะะฑั b) const   { return ii != b.ii; }
	bool        operator <  (ะะฑั b) const   { return ii < b.ii; }
	bool        operator <= (ะะฑั b) const   { return ii <=  b.ii; }
	friend void IterSwap    (ะะฑั a, ะะฑั b) { IterSwap(a.ii, b.ii); }

	int        *ii;
	I           vi;
};

template <class ะะธะฐะฟะฐะทะพะฝ, class Less>
ะะตะบัะพั<int> GetSortOrder(const ะะธะฐะฟะฐะทะพะฝ& r, const Less& less)
{
	auto begin = r.begin();
	ะะตะบัะพั<int> ะธะฝะดะตะบั;
	ะธะฝะดะตะบั.ัััะกััั(r.ะดะฐะนะกััั());
	for(int i = ะธะฝะดะตะบั.ะดะฐะนะกััั(); --i >= 0; ะธะฝะดะตะบั[i] = i)
		;
	typedef ะะฑัะพะดัะธะบะะพััะดะบะฐะกะพััะฐ__<decltype(begin), ัะธะฟะะฝะฐัะฃ<ะะธะฐะฟะฐะทะพะฝ>> It;
	Sort__(It(ะธะฝะดะตะบั.begin(), begin), It(ะธะฝะดะตะบั.end(), begin), less);
	return ะธะฝะดะตะบั;
}

template <class ะะธะฐะฟะฐะทะพะฝ>
ะะตะบัะพั<int> GetSortOrder(const ะะธะฐะฟะฐะทะพะฝ& r)
{
	return GetSortOrder(r, std::less<ัะธะฟะะฝะฐัะฃ<ะะธะฐะฟะฐะทะพะฝ>>());
}

template <class I, class T>
struct ะะฑัะพะดัะธะบะะพััะดะบะฐะกัะฐะฑะธะปะกะพััะฐ__ : PostfixOps< ะะฑัะพะดัะธะบะะพััะดะบะฐะกัะฐะฑะธะปะกะพััะฐ__<I, T> >
{
	typedef ะะฑัะพะดัะธะบะะพััะดะบะฐะกัะฐะฑะธะปะกะพััะฐ__<I, T> ะะฑั;

	ะะฑัะพะดัะธะบะะพััะดะบะฐะกัะฐะฑะธะปะกะพััะฐ__(int *ii, I vi) : ii(ii), vi(vi) {}

	ะะฑั&       operator ++ ()               { ++ii; return *this; }
	ะะฑั&       operator -- ()               { --ii; return *this; }
	ะะฑั        operator +  (int i) const    { return ะะฑั(ii + i, vi); }
	ะะฑั        operator -  (int i) const    { return ะะฑั(ii - i, vi); }
	int         operator -  (ะะฑั b) const   { return int(ii - b.ii); }
	bool        operator == (ะะฑั b) const   { return ii == b.ii; }
	bool        operator != (ะะฑั b) const   { return ii != b.ii; }
	bool        operator <  (ะะฑั b) const   { return ii < b.ii; }
	bool        operator <= (ะะฑั b) const   { return ii <=  b.ii; }
	friend void IterSwap    (ะะฑั a, ะะฑั b) { IterSwap(a.ii, b.ii); }

	StableSortItem__<T> operator*() const    { return StableSortItem__<T>(*(vi + *ii), *ii); }

	int        *ii;
	I           vi;
};

template <class ะะธะฐะฟะฐะทะพะฝ, class Less>
ะะตะบัะพั<int> GetStableSortOrder(const ะะธะฐะฟะฐะทะพะฝ& r, const Less& less)
{
	ะะตะบัะพั<int> ะธะฝะดะตะบั;
	ะธะฝะดะตะบั.ัััะกััั(r.ะดะฐะนะกััั());
	for(int i = ะธะฝะดะตะบั.ะดะฐะนะกััั(); --i >= 0; ะธะฝะดะตะบั[i] = i)
		;
	auto begin = r.begin();
	typedef ัะธะฟะะฝะฐัะฃ<ะะธะฐะฟะฐะทะพะฝ> VT;
	typedef ะะฑัะพะดัะธะบะะพััะดะบะฐะกัะฐะฑะธะปะกะพััะฐ__<decltype(begin), VT> It;
	Sort__(It(ะธะฝะดะตะบั.begin(), begin), It(ะธะฝะดะตะบั.end(), begin), StableSortLess__<VT, Less>(less));
	return ะธะฝะดะตะบั;
}

template <class ะะธะฐะฟะฐะทะพะฝ>
ะะตะบัะพั<int> GetStableSortOrder(const ะะธะฐะฟะฐะทะพะฝ& r)
{
	return GetStableSortOrder(r, std::less<ัะธะฟะะฝะฐัะฃ<ะะธะฐะฟะฐะทะพะฝ>>());
}

template <class ะฒะะฐััั, class Less>
void SortByKey(ะฒะะฐััั& map, const Less& less)
{
	typename ะฒะะฐััั::ะะพะฝัะตะนะฝะตัะะปััะตะน k = map.ะฟะพะดะฑะตัะธะะปััะธ();
	typename ะฒะะฐััั::ะะพะฝัะตะนะฝะตัะะฝะฐัะตะฝะธะน v = map.ะฟะพะดะฑะตัะธะะฝะฐัะตะฝะธั();
	IndexSort(k, v, less);
	map = ะฒะะฐััั(pick(k), pick(v));
}

template <class ะฒะะฐััั>
void SortByKey(ะฒะะฐััั& map)
{
	SortByKey(map, std::less<typename ะฒะะฐััั::ะขะธะฟะะปััะฐ>());
}

template <class ะฒะะฐััั, class Less>
void SortByValue(ะฒะะฐััั& map, const Less& less)
{
	typename ะฒะะฐััั::ะะพะฝัะตะนะฝะตัะะปััะตะน k = map.ะฟะพะดะฑะตัะธะะปััะธ();
	typename ะฒะะฐััั::ะะพะฝัะตะนะฝะตัะะฝะฐัะตะฝะธะน v = map.ะฟะพะดะฑะตัะธะะฝะฐัะตะฝะธั();
	IndexSort(v, k, less);
	map = ะฒะะฐััั(pick(k), pick(v));
}

template <class ะฒะะฐััั>
void SortByValue(ะฒะะฐััั& map)
{
	SortByValue(map, std::less<ัะธะฟะะฝะฐัะฃ<ะฒะะฐััั>>());
}

template <class ะฒะะฐััั, class Less>
void StableSortByKey(ะฒะะฐััั& map, const Less& less)
{
	typename ะฒะะฐััั::ะะพะฝัะตะนะฝะตัะะปััะตะน k = map.ะฟะพะดะฑะตัะธะะปััะธ();
	typename ะฒะะฐััั::ะะพะฝัะตะนะฝะตัะะฝะฐัะตะฝะธะน v = map.ะฟะพะดะฑะตัะธะะฝะฐัะตะฝะธั();
	StableIndexSort(k, v, less);
	map = ะฒะะฐััั(pick(k), pick(v));
}

template <class ะฒะะฐััั>
void StableSortByKey(ะฒะะฐััั& map)
{
	StableSortByKey(map, std::less<typename ะฒะะฐััั::ะขะธะฟะะปััะฐ>());
}

template <class ะฒะะฐััั, class Less>
void StableSortByValue(ะฒะะฐััั& map, const Less& less)
{
	typename ะฒะะฐััั::ะะพะฝัะตะนะฝะตัะะปััะตะน k = map.ะฟะพะดะฑะตัะธะะปััะธ();
	typename ะฒะะฐััั::ะะพะฝัะตะนะฝะตัะะฝะฐัะตะฝะธะน v = map.ะฟะพะดะฑะตัะธะะฝะฐัะตะฝะธั();
	StableIndexSort(v, k, less);
	map = ะฒะะฐััั(pick(k), pick(v));
}

template <class ะฒะะฐััั>
void StableSortByValue(ะฒะะฐััั& map)
{
	StableSortByValue(map, std::less<ัะธะฟะะฝะฐัะฃ<ะฒะะฐััั>>());
}

template <class ะะฝะดะตะบั, class Less>
void SortIndex(ะะฝะดะตะบั& ะธะฝะดะตะบั, const Less& less)
{
	typename ะะฝะดะตะบั::ะะพะฝัะตะนะฝะตัะะฝะฐัะตะฝะธะน k = ะธะฝะดะตะบั.ะฟะพะดะฑะตัะธะะปััะธ();
	ัะพััะธััะน(k, less);
	ะธะฝะดะตะบั = ะะฝะดะตะบั(pick(k));
}

template <class ะะฝะดะตะบั>
void SortIndex(ะะฝะดะตะบั& ะธะฝะดะตะบั)
{
	SortIndex(ะธะฝะดะตะบั, std::less<ัะธะฟะะฝะฐัะฃ<ะะฝะดะตะบั>>());
}

template <class ะะฝะดะตะบั, class Less>
void StableSortIndex(ะะฝะดะตะบั& ะธะฝะดะตะบั, const Less& less)
{
	typename ะะฝะดะตะบั::ะะพะฝัะตะนะฝะตัะะฝะฐัะตะฝะธะน k = ะธะฝะดะตะบั.ะฟะพะดะฑะตัะธะะปััะธ();
	StableSort(k, less);
	ะธะฝะดะตะบั = ะะฝะดะตะบั(pick(k));
}

template <class ะะฝะดะตะบั>
void StableSortIndex(ะะฝะดะตะบั& ะธะฝะดะตะบั)
{
	StableSortIndex(ะธะฝะดะตะบั, std::less<ัะธะฟะะฝะฐัะฃ<ะะฝะดะตะบั>>());
}
