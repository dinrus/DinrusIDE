// These are Ткст methods which are best inlined in heap allocator

проц Ткст0::бУст(const Ткст0& s)
{
	w[2] = s.w[2];
	w[3] = s.w[3];
	if(s.реф_ли()) {
		укз = s.укз;
		if(укз != (char *)(voidptr + 1))
			атомнИнк(s.Реф()->refcount);
	}
	else {
		укз = (char *)разместиПам32_i();
		memcpy(qptr, s.qptr, 32); // optimizes to movups
	}
}

проц Ткст0::бОсвободи()
{
	if(реф_ли()) {
		if(укз != (char *)(voidptr + 1)) {
			Rc *rc = Реф();
			ПРОВЕРЬ(rc->refcount > 0);
			if(атомнДек(rc->refcount) == 0) освободиПам(rc);
		}
	}
	else
		освободиПам32_i(укз);
}

force_inline
char *Ткст0::размести_(цел count, char& kind)
{
	if(count < 32) {
		kind = СРЕДНИЙ;
		return (char *)разместиПам32_i();
	}
	т_мера sz = sizeof(Rc) + count + 1;
	Rc *rc = (Rc *)разместиПамТн(sz);
	rc->alloc = count == INT_MAX ? INT_MAX : (цел)sz - sizeof(Rc) - 1;
	rc->refcount = 1;
	kind = min(rc->alloc, 255);
	return rc->дайУк();
}

char *Ткст0::размести(цел count, char& kind)
{
	return размести_(count, kind);
}

проц Ткст0::устБ(кткст0 s, цел len)
{
	укз = размести_(len, chr[РОД]);
	memcpy8(укз, s, len);
	укз[len] = 0;
	бДлин() = len;
	мДлин() = 15;
}

проц Ткст0::бКат(цел c)
{
	if(малый()) {
		qword *x = (qword *)разместиПам32_i();
		x[0] = q[0];
		x[1] = q[1];
		бДлин() = МДЛИН();
		мДлин() = 15;
		chr[РОД] = СРЕДНИЙ;
		qptr = x;
	}
	цел l = БДЛИН();
	if(реф_ли() ? !совместный() && l < (цел)Реф()->alloc : l < 31) {
		укз[l] = c;
		укз[бДлин() = l + 1] = 0;
	}
	else {
		char *s = вставь(l, 1, NULL);
		s[0] = c;
		s[1] = 0;
	}
}

char *ТкстБуф::размести(цел count, цел& alloc)
{
	if(count <= 31) {
		char *s = (char *)разместиПам32_i();
		alloc = 31;
		return s;
	}
	else {
		т_мера sz = sizeof(Rc) + count + 1;
		Rc *rc = (Rc *)разместиПам(sz);
		alloc = rc->alloc = (цел)min((т_мера)INT_MAX, sz - sizeof(Rc) - 1);
		rc->refcount = 1;
		return (char *)(rc + 1);
	}
}

проц ТкстБуф::уст(Ткст& s)
{
	s.UnShare();
	цел l = s.дайДлину();
	if(s.дайРазмест() == 14) {
		pbegin = (char *)разместиПам32_i();
		limit = pbegin + 31;
		memcpy8(pbegin, s.старт(), l);
		pend = pbegin + l;
	}
	else {
		pbegin = s.укз;
		pend = pbegin + l;
		limit = pbegin + s.дайРазмест();
	}
	s.обнули();
}

проц ТкстБуф::освободи()
{
	if(pbegin == буфер)
		return;
	цел all = (цел)(limit - pbegin);
	if(all == 31)
		освободиПам32_i(pbegin);
	if(all > 31)
		освободиПам((Rc *)pbegin - 1);
}
