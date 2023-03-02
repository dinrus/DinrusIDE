template <class C, class L>
C& AppendSorted(C& dest, const C& ист, const L& less)
{
	if(ист.пустой())
		return dest;
	if(dest.пустой())
	{
		dest <<= ист;
		return dest;
	}
	if(!less(*ист, dest.верх()))
	{
		dest.приставь(ист);
		return dest;
	}
	if(!less(*dest, ист.верх()))
	{
		dest.вставь(0, ист);
		return dest;
	}
	int dc = dest.дайСчёт();
	int sc = ист.дайСчёт();
	dest.устСчёт(dc + sc);
	typename C::Обходчик de = dest.стоп();
	typename C::КонстОбходчик se = ист.стоп(), pe = dest.дайОбх(dc);
	--se;
	--pe;
	for(;;)
	{
		if(less(*se, *pe))
		{
			*--de = *pe;
			if(pe == dest.старт())
			{ // dest items are finished
				*--de = *se;
				while(se != ист.старт())
					*--de = *--se;
				return dest;
			}
			--pe;
		}
		else
		{
			*--de = *se;
			if(se == ист.старт())
				return dest; // ист items are finished, dest items are in place
			--se;
		}
	}
	return dest;
}

template <class C>
C& AppendSorted(C& dest, const C& ист)
{
	typedef типЗначУ<C> VT;
	return AppendSorted(dest, ист, std::less<VT>());
}

template <class C, class L>
C& UnionSorted(C& dest, const C& ист, const L& less)
{
	if(ист.пустой())
		return dest;
	if(dest.пустой())
	{
		dest <<= ист;
		return dest;
	}
	if(less(dest.верх(), *ист))
	{
		dest.приставь(ист);
		return dest;
	}
	if(less(ист.верх(), *dest))
	{
		dest.вставь(0, ист);
		return dest;
	}
	int dc = dest.дайСчёт();
	int sc = ист.дайСчёт();
	dest.устСчёт(dc + sc);
	typename C::Обходчик de = dest.стоп();
	typename C::КонстОбходчик se = ист.стоп(), pe = dest.дайОбх(dc);
	--se;
	--pe;
	for(;;)
	{
		if(less(*se, *pe))
		{
			*--de = *pe;
			if(pe == dest.старт())
			{ // dest items are finished
				*--de = *se;
				while(se != ист.старт())
					*--de = *--se;
				dest.удали(0, dest.дайИндекс(*de));
				return dest;
			}
			--pe;
		}
		else
		{
			*--de = *se;
			if(!less(*pe, *se))
			{
				if(pe == dest.старт())
				{
					while(se != ист.старт())
						*--de = *--se;
					dest.удали(0, dest.дайИндекс(*de));
					return dest;
				}
				--pe;
			}
			if(se == ист.старт())
			{
				int pi = (pe - dest.старт()) + 1;
				dest.удали(pi, (de - dest.старт()) - pi);
				return dest; // ист items are finished, dest items are in place
			}
			--se;
		}
	}
	return dest;
}

template <class C>
C& UnionSorted(C& dest, const C& ист)
{
	typedef типЗначУ<C> VT;
	return UnionSorted(dest, ист, std::less<VT>());
}

template <class C, class L>
C& RemoveSorted(C& from, const C& what, const L& less)
{
	if(from.пустой() || what.пустой() ||
	   less(from.верх(), *what.старт()) || less(what.верх(), *from.старт()))
		return from;
	typename C::КонстОбходчик we = what.стоп(), wp = BinFind(what, from[0], less);
	if(wp == we)
		return from;
	typename C::Обходчик fp = from.старт() + BinFindIndex(from, *wp), fe = from.стоп(), fd = fp;
	if(fp == fe)
		return from;
	for(;;)
	{
		while(less(*fp, *wp))
		{
			*fd = *fp;
			++fd;
			if(++fp == fe)
			{
				from.устСчёт(fd - from.старт());
				return from;
			}
		}
		if(less(*wp, *fp))
		{
			do
				if(++wp == we)
				{
					копируй(fd, fp, fe);
					fd += (fe - fp);
					from.устСчёт(fd - from.старт());
					return from;
				}
			while(less(*wp, *fp));
		}
		else
		{
			const типЗначУ<C>& значение = *fp;
			while(!less(значение, *fp))
				if(++fp == fe)
				{
					from.устСчёт(fd - from.старт());
					return from;
				}
			do
				if(++wp == we)
				{
					копируй(fd, fp, fe);
					fd += (fe - fp);
					from.устСчёт(fd - from.старт());
					return from;
				}
			while(!less(значение, *wp));
		}
	}
}

template <class C>
C& RemoveSorted(C& from, const C& what)
{
	typedef типЗначУ<C> VT;
	return RemoveSorted(from, what, std::less<VT>());
}

template <class D, class S, class L>
D& IntersectSorted(D& dest, const S& ист, const L& less)
{
	if(dest.пустой())
		return dest;
	if(ист.пустой() || less(dest.верх(), ист[0]) || less(ист.верх(), dest[0]))
	{ // empty source set or disjunct intervals
		dest.очисть();
		return dest;
	}
	typename S::КонстОбходчик ss = BinFind(ист, dest[0], less), se = ист.стоп();
	if(ss == se)
	{
		dest.очисть();
		return dest;
	}
	typename D::КонстОбходчик ds = BinFind(dest, ист[0], less), de = dest.стоп();
	if(ds == de)
	{
		dest.очисть();
		return dest;
	}
	typename D::Обходчик d = dest.старт();
	int count = 0;
	for(;;)
	{
		if(less(*ss, *ds))
		{
			if(++ss == se)
				break;
		}
		else
		{
			if(!less(*ds, *ss))
			{
				*d = *ds;
				++d;
				count++;
			}
			if(++ds == de)
				break;
		}
	}
	dest.устСчёт(count);
	return dest;
}

template <class D, class S>
D& IntersectSorted(D& dest, const S& ист)
{
	typedef типЗначУ<D> VT;
	return IntersectSorted(dest, ист, std::less<VT>());
}
