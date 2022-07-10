template <typename CHAR, typename BYTE>
force_inline
void пропустиПробелы__(const CHAR *&s)
{
	while(*s && (BYTE)*s <= ' ') s++;
}

template <typename CHAR, typename BYTE>
force_inline
int сканЗнак__(const CHAR *&s)
{
	int sign = 1;
	if(*s == '+')
		s++;
	if(*s == '-') {
		sign = -1;
		s++;
	}
	пропустиПробелы__<CHAR, BYTE>(s);
	return sign;
}

template <typename CHAR, typename BYTE, typename UINT, int base>
force_inline
const CHAR *сканБцел0(UINT& result, const CHAR *s, bool& overflow)
{
	auto дай = [&] (int i) -> int {
		int c = (BYTE)s[i];
		if(base > 10) {
			if(c <= '9')
				return c - '0';
			if(c <= 'Z')
				return c - 'A' + 10;
			return c - 'a' + 10;
		}
		return c - '0';
	};
	int c1 = дай(0);
	if(c1 < 0 || c1 >= base)
		return NULL;
	UINT n = 0;
	const int base2 = base * base;
	const int base3 = base * base2;
	const int base4 = base * base3;
	for(;;) {
		UINT n0 = n;
		auto CheckOverflow = [&](UINT n1) { if(n0 > n1) overflow = true; };
		if(c1 < 0 || c1 >= base) {
			result = n;
			return s;
		}
		int c2 = дай(1);
		if(c2 < 0 || c2 >= base) {
			result = base * n + c1;
			CheckOverflow(result);
			return s + 1;
		}
		int c3 = дай(2);
		if(c3 < 0 || c3 >= base) {
			result = base2 * n + base * c1 + c2;
			CheckOverflow(result);
			return s + 2;
		}
		int c4 = дай(3);
		if(c4 < 0 || c4 >= base) {
			result = base3 * n + base2 * c1 + base * c2 + c3;
			CheckOverflow(result);
			return s + 3;
		}
		n = base4 * n + base3 * c1 + base2 * c2 + base * c3 + c4;
		CheckOverflow(n);
		s += 4;
		c1 = дай(0);
	}
}

template <typename CHAR, typename BYTE, typename UINT, int base>
force_inline
const CHAR *сканБцел(UINT& result, const CHAR *s, bool& overflow)
{
	пропустиПробелы__<CHAR, BYTE>(s);
	return сканБцел0<CHAR, BYTE, UINT, base>(result, s, overflow);
}

template <typename CHAR, typename BYTE, typename UINT, int base>
force_inline
const CHAR *сканБцел(UINT& result, const char *s)
{
	bool overflow = false;
	s = сканБцел<CHAR, BYTE, UINT, base>(result, s, overflow);
	return overflow ? NULL : s;
}

template <typename CHAR, typename BYTE, typename UINT, typename INT, int base>
force_inline
const CHAR *сканЦел(INT& result, const CHAR *s, bool& overflow)
{
	пропустиПробелы__<CHAR, BYTE>(s);
	int зн = сканЗнак__<CHAR, BYTE>(s);
	UINT uresult;
	s = сканБцел0<CHAR, BYTE, UINT, base>(uresult, s, overflow);
	if(s) {
		if(зн < 0) {
			result = INT(зн * uresult);
			if(result > 0) overflow = true;
		}
		else {
			result = INT(uresult);
			if(result < 0) overflow = true;
		}
	}
	return s;
}

template <typename CHAR, typename BYTE, typename UINT, typename INT, int base>
force_inline
const CHAR *сканЦел(INT& result, const CHAR *s)
{
	bool overflow = false;
	s = сканЦел<CHAR, BYTE, UINT, INT, base>(result, s, overflow);
	return overflow ? NULL : s;
}

inline
Ткст фмтБцел(dword w)
{
	return Ткст::сделай(12, [&](char *s) { return utoa32(w, s); });
}

inline
Ткст фмтЦел(int i)
{
	return Ткст::сделай(12, [&](char *s) {
		if(пусто_ли(i))
			return 0;
		if(i < 0) {
			*s++ = '-';
			return utoa32(-i, s) + 1;
		}
		return utoa32(i, s);
	});
}
