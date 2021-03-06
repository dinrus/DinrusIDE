#include "Core.h"

namespace РНЦПДинрус {

//#BLITZ_APPROVE

ИНИЦБЛОК {
	Значение::регистрируй<Uuid>("Uuid");
}

void Uuid::сериализуй(Поток& s) {
	int версия = 0;
	s / версия % v[0] % v[1];
}

void Uuid::вДжейсон(ДжейсонВВ& jio)
{
	Ткст h;
	if(jio.сохраняется()) {
		h = фмт(*this);
		jio.уст(h);
	}
	else
		*this = ScanUuid((Ткст)jio.дай());
}

void Uuid::нов()
{
	do
		случ64(v, 2);
	while(экзПусто_ли());
}

Ткст фмт(const Uuid& ид) {
	return спринтф("%08X%08X%08X%08X", LODWORD(ид.v[0]), HIDWORD(ид.v[0]), LODWORD(ид.v[1]), HIDWORD(ид.v[1]));
}

Ткст FormatWithDashes(const Uuid& ид) {
	return спринтф("%08X-%04X-%04X-%04X-%04X%08X", LODWORD(ид.v[0]),
	               HIWORD(HIDWORD(ид.v[0])), LOWORD(HIDWORD(ид.v[0])),
	               HIWORD(LODWORD(ид.v[1])), LOWORD(LODWORD(ид.v[1])),
	               HIDWORD(ид.v[1]));
}

dword scanX(const char *s)
{
    dword r = 0;
    for(int i = 0; i < 8; i++) {
        r = (r << 4) | (*s >= '0' && *s <= '9' ?      *s - '0' :
                        *s >= 'A' && *s <= 'F' ? 10 + *s - 'A' :
                        *s >= 'a' && *s <= 'f' ? 10 + *s - 'a' : 0);
        s++;
    }
    return r;
}

Uuid ScanUuid(const char *s)
{
	Uuid ид;
	Ткст xu;
	while(*s) {
		if(IsXDigit(*s))
			xu.конкат(*s);
		s++;
	}
	if(xu.дайСчёт() < 32)
		return Null;
	ид.v[0] = MAKEQWORD(scanX(~xu), scanX(~xu + 8));
	ид.v[1] = MAKEQWORD(scanX(~xu + 16), scanX(~xu + 24));
	return ид;
}

void Uuid::вРяр(РярВВ& xio)
{
	Ткст h;
	if(xio.сохраняется())
		h = фмт(*this);
	xio.Атр("значение", h);
	if(xio.грузится())
		*this = ScanUuid(h);
}

Ткст Uuid::вТкст() const
{
	return фмт(*this);
}

Ткст Uuid::ToStringWithDashes() const
{
	return FormatWithDashes(*this);
}

Ткст дамп(const Uuid& ид) {
	return "UUID: " + фмт(ид);
}

struct UuidValueGenClass : ГенЗначения
{
	virtual Значение дай() {
		return фмт(Uuid::создай());
	}
};

ГенЗначения& UuidValueGen()
{
	return Single<UuidValueGenClass>();
}

}
