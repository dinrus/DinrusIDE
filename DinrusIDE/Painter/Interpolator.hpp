inline
void LinearInterpolator::Dda2::уст(int p1, int p2, int len)
{
	count = len <= 0 ? 1 : len;
	lift = (p2 - p1) / count;
	rem = (p2 - p1) % count;
	mod = rem;
	p = p1;
	if(mod <= 0) {
		mod += count;
		rem += count;
		lift--;
    }
	mod -= count;
}

inline
int LinearInterpolator::Dda2::дай()
{
	int pp = p;
	mod += rem;
	p += lift;
	if(mod > 0) {
		mod -= count;
		p++;
	}
	return pp;
}

inline
void LinearInterpolator::старт(int x, int y, int len)
{
	ТочкаПЗ p1 = xform.Transform(ТочкаПЗ(x, y));
	ТочкаПЗ p2 = xform.Transform(ТочкаПЗ(x + len, y));
	ddax.уст(Q8(p1.x), Q8(p2.x), len);
	dday.уст(Q8(p1.y), Q8(p2.y), len);
}

inline
Точка LinearInterpolator::дай()
{
	return Точка(ddax.дай(), dday.дай());
}
