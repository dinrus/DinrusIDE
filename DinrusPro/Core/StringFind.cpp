#include <DinrusPro/DinrusCore.h>

#ifdef CPU_UNALIGNED // Use unaligned access

#ifdef CPU_64
inline бул equal_back_8(кткст0 a, кткст0 b, цел len)
{
	while(len > 8) {
		len -= 8;
		if(подбери64лэ(a + len) != подбери64лэ(b + len))
			return false;
	}
	return true;
}
#else
форс_инлайн бул equal_back_4(кткст0 a, кткст0 b, цел len)
{
	while(len > 4) {
		len -= 4;
		if(подбери32лэ(a + len) != подбери32лэ(b + len))
			return false;
	}
	return true;
}
#endif

template <цел step> // Template parameter to be a constant
цел t_find(кткст0 укз, цел МДЛИН, кткст0 p, цел len, цел from)
{
	ПРОВЕРЬ(from >= 0 && from <= МДЛИН);
	цел l = МДЛИН - len - from;
	if(l < 0)
		return -1;
	кткст0 s = укз + from;
	кткст0 e = s + l;
	if(len <= 8) {
		if(len > 4) {
			len -= 4;
			цел p0 = подбери32лэ(p);
			цел p1 = подбери32лэ(p + len);
			while(s <= e) {
				if((цел)подбери32лэ(s) == p0 && (цел)подбери32лэ(s + len) == p1)
					return (цел)(s - укз);
				s += step;
			}
		}
		else
		if(len == 4) {
			цел p0 = подбери32лэ(p);
			while(s <= e) {
				if((цел)подбери32лэ(s) == p0)
					return (цел)(s - укз);
				s += step;
			}
		}
		else
		if(len == 3) {
			крат p0 = подбери16лэ(p);
			сим p1 = p[2];
			while(s <= e) {
				if((крат)подбери16лэ(s) == p0 && s[2] == p1)
					return (цел)(s - укз);
				s += step;
			}
		}
		else
		if(len == 2) {
			крат p0 = подбери16лэ(p);
			while(s <= e) {
				if((крат)подбери16лэ(s) == p0)
					return (цел)(s - укз);
				s += step;
			}
		}
		else
		if(len == 1) {
			сим p0 = p[0];
			while(s <= e) {
				if(*s == p0)
					return (цел)(s - укз);
				s += step;
			}
		}
		else
			return from;
	}
	else {
#ifdef CPU_64
		дол p0 = подбери64лэ(p);
		if(len <= 16) {
			len -= 8;
			дол p1 = подбери64лэ(p + len);
			while(s <= e) {
				if((дол)подбери64лэ(s) == p0 && (дол)подбери64лэ(s + len) == p1)
					return (цел)(s - укз);
				s += step;
			}
		}
		else
			while(s <= e) {
				if((дол)подбери64лэ(s) == p0 && equal_back_8(s, p, len))
					return (цел)(s - укз);
				s += step;
			}
#else
		цел p0 = подбери32лэ(p);
		while(s <= e) {
			if((цел)подбери32лэ(s) == p0 && equal_back_4(s, p, len))
				return (цел)(s - укз);
			s += step;
		}
#endif
	}
	return -1;
}

цел найди(кткст0 text, цел len, кткст0 needle, цел nlen, цел from)
{
	return t_find<1>(text, len, needle, nlen, from);
}

цел найди(const шим *text, цел len, const шим *needle, цел nlen, цел from)
{
	цел q = t_find<sizeof(шим)>((кткст0 )text, sizeof(шим) * len, (кткст0 )needle, sizeof(шим) * nlen, sizeof(шим) * from);
	return q < 0 ? q : q / sizeof(шим);
}

#else

template <class tchar>
форс_инлайн бул svo_равнпам(const tchar *a, const tchar *b, цел len)
{
	if(len > 11)
		return memcmp(a, b, len * sizeof(tchar)) == 0;
	switch(len) {
	case 11:
		if(a[10] != b[10]) return false;
	case 10:
		if(a[9] != b[9]) return false;
	case 9:
		if(a[8] != b[8]) return false;
	case 8:
		if(a[7] != b[7]) return false;
	case 7:
		if(a[6] != b[6]) return false;
	case 6:
		if(a[5] != b[5]) return false;
	case 5:
		if(a[4] != b[4]) return false;
	case 4:
		if(a[3] != b[3]) return false;
	case 3:
		if(a[2] != b[2]) return false;
	case 2:
		if(a[1] != b[1]) return false;
	case 1:
		if(a[0] != b[0]) return false;
	}
	return true;
}

template <class tchar>
цел t_find(const tchar *укз, цел plen, const tchar *s, цел len, цел from)
{
	ПРОВЕРЬ(from >= 0 && from <= plen);
	цел l = plen - len - from;
	if(l < 0)
		return -1;
	if(len == 0)
		return from;
	const tchar *p = укз + from;
	const tchar *e = p + l;
	if(len > 4) {
		tchar s0 = s[0];
		tchar s1 = s[1];
		tchar s2 = s[2];
		tchar sl = s[len - 1];
		цел l_1 = len - 1;
		while(p <= e) {
			if(s0 == p[0] && sl == p[l_1] && s1 == p[1] && s2 == p[2] &&
			   svo_равнпам(s + 3, p + 3, len - 4))
				return (цел)(p - укз);
			p++;
		}
	}
	else
	if(len == 4) {
		tchar s0 = s[0];
		tchar s1 = s[1];
		tchar s2 = s[2];
		tchar s3 = s[3];
		while(p <= e) {
			if(s0 == p[0] && s3 == p[3] && s1 == p[1] && s2 == p[2])
				return (цел)(p - укз);
			p++;
		}
	}
	else
	if(len == 3) {
		tchar s0 = s[0];
		tchar s1 = s[1];
		tchar s2 = s[2];
		while(p <= e) {
			if(s0 == p[0] && s2 == p[2] && s1 == p[1])
				return (цел)(p - укз);
			p++;
		}
	}
	else
	if(len == 2) {
		tchar s0 = s[0];
		tchar s1 = s[1];
		while(p <= e) {
			if(s0 == p[0] && s1 == p[1])
				return (цел)(p - укз);
			p++;
		}
	}
	else {
		tchar s0 = s[0];
		while(p <= e) {
			if(s0 == *p)
				return (цел)(p - укз);
			p++;
		}
	}
	return -1;
}

цел найди(кткст0 text, цел len, кткст0 needle, цел nlen, цел from)
{
	return t_find(text, len, needle, nlen, from);
}

цел найди(const шим *text, цел len, const шим *needle, цел nlen, цел from)
{
	return t_find(text, len, needle, nlen, from);
}
#endif
