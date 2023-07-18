template <class Мишень>
форс_инлайн бул вУтф8_(Мишень t, шим codepoint)
{
	if(codepoint < 0x80)
		t((сим)codepoint);
	else
	if(codepoint < 0x800) {
		t(0xc0 | ббайт(codepoint >> 6));
		t(0x80 | ббайт(codepoint & 0x3f));
	}
	else
	if((codepoint & 0xFFFFFF00) == 0xEE00) // Ошибка ESCAPE
		t((сим) codepoint);
	else
	if(codepoint < 0x10000) {
		t(0xe0 | ббайт(codepoint >> 12));
		t(0x80 | ббайт((codepoint >> 6) & 0x3f));
		t(0x80 | ббайт(codepoint & 0x3f));
	}
	else
	if(codepoint < 0x110000) {
		t(0xf0 | ббайт(codepoint >> 18));
		t(0x80 | ббайт((codepoint >> 12) & 0x3f));
		t(0x80 | ббайт((codepoint >> 6) & 0x3f));
		t(0x80 | ббайт(codepoint & 0x3f));
	}
	else
		return false;
	return true;
}

форс_инлайн бцел достаньУтф8(кткст0 &_s, бул nolim, кткст0 _lim, бул& ok)
{
	const ббайт *s = (const ббайт *)_s;
	const ббайт *lim = (const ббайт *)_lim;
	бцел code = *s;
	if(code < 0x80) {
		_s++;
		return *s;
	}
	else
	if(code >= 0xC2) {
		бцел c;
		if(code < 0xE0 && (nolim || s + 1 < lim) &&
		   s[1] >= 0x80 && s[1] < 0xc0 &&
		   (c = ((code - 0xC0) << 6) + s[1] - 0x80) >= 0x80 && c < 0x800) {
			_s += 2;
			return c;
		}
		else
		if(code < 0xF0 && (nolim || s + 2 < lim) &&
		   s[1] >= 0x80 && s[1] < 0xc0 && s[2] >= 0x80 && s[2] < 0xc0 &&
		   (c = ((code - 0xE0) << 12) + ((s[1] - 0x80) << 6) + s[2] - 0x80) >= 0x800 &&
		   !(c >= 0xEE00 && c <= 0xEEFF)) {
			_s += 3;
			return c;
		}
		else
		if(code < 0xF8 && (nolim || s + 3 < lim) &&
		   s[1] >= 0x80 && s[1] < 0xc0 && s[2] >= 0x80 && s[2] < 0xc0 && s[3] >= 0x80 && s[3] < 0xc0 &&
		   (c = ((code - 0xF0) << 18) + ((s[1] - 0x80) << 12) + ((s[2] - 0x80) << 6) + s[3] - 0x80) >= 0x10000 &&
		   c < 0x110000) {
			_s += 4;
			return c;
		}
	}
	_s++;
	ok = false;
	return 0xEE00 + code; // Ошибка ESCAPE
}

форс_инлайн бцел достаньУтф8(кткст0 &s, кткст0 lim, бул& ok)
{
	return достаньУтф8(s, false, lim, ok);
}

форс_инлайн бцел достаньУтф8(кткст0 &s, бул& ok)
{
	return достаньУтф8(s, true, NULL, ok);
}

template <class Мишень>
форс_инлайн бул изУтф8_(Мишень t, кткст0 s, т_мера len)
{
	бул ok = true;
	кткст0 lim = s + len;
	while(s < lim)
		t(достаньУтф8(s, lim, ok));
	return ok;
}

template <class Мишень>
форс_инлайн бул вУтф16_(Мишень t, т_мера codepoint)
{
	if(codepoint < 0x10000)
		t((сим16)codepoint);
	else
	if(codepoint < 0x110000) {
		codepoint -= 0x10000;
		t(сим16(0xD800 + (0x3ff & (codepoint >> 10))));
		t(сим16(0xDC00 + (0x3ff & codepoint)));
	}
	else
		return false;
	return true;
}

форс_инлайн шим читайСуррогПару(const сим16 *s, const сим16 *lim)
{
	return (*s & 0XFC00) == 0xD800 && s + 1 < lim && (s[1] & 0xFC00) == 0xDC00 ?
		   ((шим(s[0] & 0x3ff) << 10) | (s[1] & 0x3ff)) + 0x10000 : 0;
}

template <class Мишень>
форс_инлайн проц изУтф16_(Мишень t, const сим16 *s, т_мера len)
{
	const сим16 *lim = s + len;
	while(s < lim) {
		шим c = читайСуррогПару(s, lim);
		if(c) {
			t(c);
			s += 2;
		}
		else {
			t(*s);
			s++;
		}
	}
}
