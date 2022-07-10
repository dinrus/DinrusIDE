template <class B>
force_inline
void АТкст<B>::вставь(int pos, const char *s)
{
	вставь(pos, s, strlen__(s));
}

template <class B>
void АТкст<B>::конкат(int c, int count)
{
	tchar *s = B::вставь(дайДлину(), count, NULL);
	while(count--)
		*s++ = c;
}

template <class B>
force_inline
void АТкст<B>::конкат(const tchar *s)
{
	конкат(s, strlen__(s));
}

template <class B>
int АТкст<B>::сравни(const tchar *b) const
{
	const tchar *a = B::старт();
	const tchar *ae = стоп();
	for(;;) {
		if(a >= ae)
			return *b == 0 ? 0 : -1;
		if(*b == 0)
			return 1;
		int q = cmpval__(*a++) - cmpval__(*b++);
		if(q)
			return q;
	}
}

template <class B>
typename АТкст<B>::Ткст АТкст<B>::середина(int from, int count) const
{
	int l = дайДлину();
	if(from > l) from = l;
	if(from < 0) from = 0;
	if(count < 0)
		count = 0;
	if(from + count > l)
		count = l - from;
	return Ткст(B::старт() + from, count);
}

template <class B>
int АТкст<B>::найди(int chr, int from) const
{
	ПРОВЕРЬ(from >= 0 && from <= дайДлину());
	const tchar *e = стоп();
	const tchar *ptr = B::старт();
	for(const tchar *s = ptr + from; s < e; s++)
		if(*s == chr)
			return (int)(s - ptr);
	return -1;
}

template <class B>
int АТкст<B>::найдирек(int chr, int from) const
{
	ПРОВЕРЬ(from >= 0 && from <= дайДлину());
	if(from < дайДлину()) {
		const tchar *ptr = B::старт();
		for(const tchar *s = ptr + from; s >= ptr; s--)
			if(*s == chr)
				return (int)(s - ptr);
	}
	return -1;
}

template <class B>
int АТкст<B>::найдирек(int len, const tchar *s, int from) const
{
	ПРОВЕРЬ(from >= 0 && from <= дайДлину());
	if(from < дайДлину()) {
		const tchar *ptr = B::старт();
		const tchar *p = ptr + from - len + 1;
		len *= sizeof(tchar);
		while(p >= ptr) {
			if(*s == *p && memcmp(s, p, len) == 0)
				return (int)(p - ptr);
			p--;
		}
	}
	return -1;
}

template <class B>
int АТкст<B>::найдирекПосле(int len, const tchar *s, int from) const
{
	int q = найдирек(len, s, from);
	return q >= 0 ? q + len : -1;
}

template <class B>
void АТкст<B>::замени(const tchar *find, int findlen, const tchar *replace, int replacelen)
{
	if(findlen == 0)
		return;
	Ткст r;
	int i = 0;
	const tchar *p = B::старт();
	for(;;) {
		int j = найди(findlen, find, i);
		if(j < 0)
			break;
		r.конкат(p + i, j - i);
		r.конкат(replace, replacelen);
		i = j + findlen;
	}
	r.конкат(p + i, B::дайСчёт() - i);
	B::освободи();
	B::уст0(r);
}

template <class B>
int АТкст<B>::найдирек(const tchar *s, int from) const
{
	return найдирек(strlen__(s), s, from);
}

template <class B>
int АТкст<B>::найдирекПосле(const tchar *s, int from) const
{
	return найдирекПосле(strlen__(s), s, from);
}

template <class B>
int АТкст<B>::найдирек(int chr) const
{
	return B::дайСчёт() ? найдирек(chr, B::дайСчёт() - 1) : -1;
}

template <class B>
void АТкст<B>::замени(const Ткст& find, const Ткст& replace)
{
	замени(~find, find.дайСчёт(), ~replace, replace.дайСчёт());
}

template <class B>
force_inline
void АТкст<B>::замени(const tchar *find, const tchar *replace)
{
	замени(find, (int)strlen__(find), replace, (int)strlen__(replace));
}

template <class B>
force_inline
void АТкст<B>::замени(const Ткст& find, const tchar *replace)
{
	замени(~find, find.дайСчёт(), replace, (int)strlen__(replace));
}

template <class B>
force_inline
void АТкст<B>::замени(const tchar *find, const Ткст& replace)
{
	замени(find, (int)strlen__(find), ~replace, replace.дайСчёт());
}

template <class B>
bool АТкст<B>::начинаетсяС(const tchar *s, int len) const
{
	if(len > дайДлину()) return false;
	return memcmp(s, B::старт(), len * sizeof(tchar)) == 0;
}

template <class B>
force_inline
bool АТкст<B>::начинаетсяС(const tchar *s) const
{
	return начинаетсяС(s, strlen__(s));
}

template <class B>
bool АТкст<B>::заканчиваетсяНа(const tchar *s, int len) const
{
	int l = дайДлину();
	if(len > l) return false;
	return memcmp(s, B::старт() + l - len, len * sizeof(tchar)) == 0;
}

template <class B>
force_inline
bool АТкст<B>::заканчиваетсяНа(const tchar *s) const
{
	return заканчиваетсяНа(s, strlen__(s));
}

template <class B>
int АТкст<B>::найдиПервыйИз(int len, const tchar *s, int from) const
{
	ПРОВЕРЬ(from >= 0 && from <= дайДлину());
	const tchar *ptr = B::старт();
	const tchar *e = B::стоп();
	const tchar *se = s + len;
	if(len == 1) {
		tchar c1 = s[0];
		for(const tchar *bs = ptr + from; bs < e; bs++) {
			if(*bs == c1)
				return (int)(bs - ptr);
		}
		return -1;
	}
	if(len == 2) {
		tchar c1 = s[0];
		tchar c2 = s[1];
		for(const tchar *bs = ptr + from; bs < e; bs++) {
			tchar ch = *bs;
			if(ch == c1 || ch == c2)
				return (int)(bs - ptr);
		}
		return -1;
	}
	if(len == 3) {
		tchar c1 = s[0];
		tchar c2 = s[1];
		tchar c3 = s[2];
		for(const tchar *bs = ptr + from; bs < e; bs++) {
			tchar ch = *bs;
			if(ch == c1 || ch == c2 || ch == c3)
				return (int)(bs - ptr);
		}
		return -1;
	}
	if(len == 4) {
		tchar c1 = s[0];
		tchar c2 = s[1];
		tchar c3 = s[2];
		tchar c4 = s[3];
		for(const tchar *bs = ptr + from; bs < e; bs++) {
			tchar ch = *bs;
			if(ch == c1 || ch == c2 || ch == c3 || ch == c4)
				return (int)(bs - ptr);
		}
		return -1;
	}
	for(const tchar *bs = ptr + from; bs < e; bs++)
		for(const tchar *ss = s; ss < se; ss++)
			if(*bs == *ss)
				return (int)(bs - ptr);
	return -1;
}

force_inline
void Ткст0::уст(const char *s, int len)
{
	очисть();
	if(len <= 14) {
		memcpy8(chr, s, len);
		SLen() = len;
		Dsyn();
		return;
	}
	SetL(s, len);
	Dsyn();
}

force_inline
void Ткст0::уст0(const char *s, int len)
{
	обнули();
	if(len <= 14) {
		SLen() = len;
		memcpy8(chr, s, len);
		Dsyn();
		return;
	}
	SetL(s, len);
	Dsyn();
}

inline
bool Ткст0::равен(const char *s) const
{ // This is optimized for comparison with string literals...
	size_t len = strlen(s);
	const void *p;
	if(смолл_ли()) {
		if(len > 14 || len != (size_t)chr[SLEN]) return false; // len > 14 fixes issue with GCC warning, might improves performance too
		p = chr;
	}
	else {
		if(len != (size_t)w[LLEN]) return false;
		p = ptr;
	}
	return memcmp(p, s, len) == 0; // compiler is happy to optimize memcmp out with up to 64bit comparisons for literals...
}

inline
int Ткст0::сравни(const Ткст0& s) const
{
#ifdef CPU_LE
	if((chr[KIND] | s.chr[KIND]) == 0) { // both strings are small, len is the MSB
	#ifdef CPU_64
		uint64 a = SwapEndian64(q[0]);
		uint64 b = SwapEndian64(s.q[0]);
		if(a != b)
			return a < b ? -1 : 1;
		a = SwapEndian64(q[1]);
		b = SwapEndian64(s.q[1]);
		return a == b ? 0 : a < b ? -1 : 1;
	#else
		uint64 a = эндианРазворот32(w[0]);
		uint64 b = эндианРазворот32(s.w[0]);
		if(a != b)
			return a < b ? -1 : 1;
		a = эндианРазворот32(w[1]);
		b = эндианРазворот32(s.w[1]);
		if(a != b)
			return a < b ? -1 : 1;
		a = эндианРазворот32(w[2]);
		b = эндианРазворот32(s.w[2]);
		if(a != b)
			return a < b ? -1 : 1;
		a = эндианРазворот32(w[3]);
		b = эндианРазворот32(s.w[3]);
		return a == b ? 0 : a < b ? -1 : 1;
	#endif
	}
#endif
	return CompareL(s);
}

force_inline
Ткст& Ткст::operator=(const char *s)
{
	присвойДлин(s, strlen__(s));
	return *this;
}

force_inline
Ткст::Ткст(const char *s)
{
	Ткст0::уст0(s, strlen__(s));
}

force_inline
void Ткст0::разверни(Ткст0& b)
{
	qword a0 = q[0]; // Explicit code (instead of разверни) seems to emit some SSE2 code with CLANG
	qword a1 = q[1];
	qword b0 = b.q[0];
	qword b1 = b.q[1];
	q[0] = b0;
	q[1] = b1;
	b.q[0] = a0;
	b.q[1] = a1;
	Dsyn(); b.Dsyn();
}

template <class Делец>
inline
Ткст Ткст::сделай(int alloc, Делец m)
{
	Ткст s;
	if(alloc <= 14) {
		int len = m(s.chr);
		ПРОВЕРЬ(len <= alloc);
		s.SLen() = len;
		s.Dsyn();
	}
	else {
		if(alloc < 32) {
			s.chr[KIND] = MEDIUM;
			s.ptr = (char *)MemoryAlloc32();
		}
		else
			s.ptr = s.размести(alloc, s.chr[KIND]);
		int len = m(s.ptr);
		ПРОВЕРЬ(len <= alloc);
		s.ptr[len] = 0;
		s.LLen() = len;
		s.SLen() = 15;
		if(alloc >= 32 && alloc > 2 * len)
			s.сожми();
	}
	return s;
}

force_inline
void ТкстБуф::длинткт()
{
	устДлину((int)strlen__(pbegin));
}

inline
void ТкстБуф::конкат(const char *s, int l)
{
	if(pend + l > limit)
		ReallocL(s, l);
	else {
		memcpy8(pend, s, l);
		pend += l;
	}
}

inline
void ТкстБуф::конкат(int c, int l)
{
	if(pend + l > limit)
		ReallocL(NULL, l);
	memset8(pend, c, l);
	pend += l;
}

force_inline
void ТкстБуф::конкат(const char *s)
{
	конкат(s, (int)strlen__(s));
}
