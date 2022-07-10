// These are Ткст methods which are best inlined in heap allocator

void Ткст0::LSet(const Ткст0& s)
{
	w[2] = s.w[2];
	w[3] = s.w[3];
	if(s.реф_ли()) {
		ptr = s.ptr;
		if(ptr != (char *)(voidptr + 1))
			атомнИнк(s.Реф()->refcount);
	}
	else {
		ptr = (char *)MemoryAlloc32_i();
		memcpy(qptr, s.qptr, 32); // optimizes to movups
	}
}

void Ткст0::LFree()
{
	if(реф_ли()) {
		if(ptr != (char *)(voidptr + 1)) {
			Rc *rc = Реф();
			ПРОВЕРЬ(rc->refcount > 0);
			if(атомнДек(rc->refcount) == 0) MemoryFree(rc);
		}
	}
	else
		MemoryFree32_i(ptr);
}

force_inline
char *Ткст0::Alloc_(int count, char& kind)
{
	if(count < 32) {
		kind = MEDIUM;
		return (char *)MemoryAlloc32_i();
	}
	size_t sz = sizeof(Rc) + count + 1;
	Rc *rc = (Rc *)MemoryAllocSz(sz);
	rc->alloc = count == INT_MAX ? INT_MAX : (int)sz - sizeof(Rc) - 1;
	rc->refcount = 1;
	kind = min(rc->alloc, 255);
	return rc->дайУк();
}

char *Ткст0::размести(int count, char& kind)
{
	return Alloc_(count, kind);
}

void Ткст0::SetL(const char *s, int len)
{
	ptr = Alloc_(len, chr[KIND]);
	memcpy8(ptr, s, len);
	ptr[len] = 0;
	LLen() = len;
	SLen() = 15;
}

void Ткст0::LCat(int c)
{
	if(смолл_ли()) {
		qword *x = (qword *)MemoryAlloc32_i();
		x[0] = q[0];
		x[1] = q[1];
		LLen() = SLen();
		SLen() = 15;
		chr[KIND] = MEDIUM;
		qptr = x;
	}
	int l = LLen();
	if(реф_ли() ? !IsShared() && l < (int)Реф()->alloc : l < 31) {
		ptr[l] = c;
		ptr[LLen() = l + 1] = 0;
	}
	else {
		char *s = вставь(l, 1, NULL);
		s[0] = c;
		s[1] = 0;
	}
}

char *ТкстБуф::размести(int count, int& alloc)
{
	if(count <= 31) {
		char *s = (char *)MemoryAlloc32_i();
		alloc = 31;
		return s;
	}
	else {
		size_t sz = sizeof(Rc) + count + 1;
		Rc *rc = (Rc *)MemoryAlloc(sz);
		alloc = rc->alloc = (int)min((size_t)INT_MAX, sz - sizeof(Rc) - 1);
		rc->refcount = 1;
		return (char *)(rc + 1);
	}
}

void ТкстБуф::уст(Ткст& s)
{
	s.UnShare();
	int l = s.дайДлину();
	if(s.дайРазмест() == 14) {
		pbegin = (char *)MemoryAlloc32_i();
		limit = pbegin + 31;
		memcpy8(pbegin, s.старт(), l);
		pend = pbegin + l;
	}
	else {
		pbegin = s.ptr;
		pend = pbegin + l;
		limit = pbegin + s.дайРазмест();
	}
	s.обнули();
}

void ТкстБуф::освободи()
{
	if(pbegin == буфер)
		return;
	int all = (int)(limit - pbegin);
	if(all == 31)
		MemoryFree32_i(pbegin);
	if(all > 31)
		MemoryFree((Rc *)pbegin - 1);
}
