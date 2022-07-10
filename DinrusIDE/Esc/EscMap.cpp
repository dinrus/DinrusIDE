#include "Esc.h"

namespace РНЦП {

#define LTIMING(x) // RTIMING(x)

ВекторМап<EscValue, EscValue>& EscValue::CloneMap()
{
	LTIMING("CloneMap");
	ПРОВЕРЬ(IsMap());
	if(map->refcount != 1) {
		EscMap *c = new EscMap;
		c->map <<= map->map;
		map->отпусти();
		map = c;
	}
	hash = 0;
	return map->map;
}

const ВекторМап<EscValue, EscValue>& EscValue::дайМап() const
{
	ПРОВЕРЬ(IsMap());
	return map->map;
}

void  EscValue::SetEmptyMap()
{
	освободи();
	тип = ESC_MAP;
	hash = 0;
	map = new EscMap;
}

EscValue EscValue::MapGet(EscValue ключ) const
{
	LTIMING("MapGet");
	return дайМап().дай(ключ, EscValue());
}

void EscValue::MapSet(EscValue ключ, EscValue значение)
{
	LTIMING("MapSet");
	if(проц_ли())
		SetEmptyMap();
	ВекторМап<EscValue, EscValue>& m = CloneMap();
	int q = m.найди(ключ);
	if(q >= 0) {
		if(значение.проц_ли()) {
			m.отлинкуй(q);
			map->count--;
		}
		else
			m[q] = значение;
	}
	else
		if(!значение.проц_ли()) {
			map->count++;
			m.добавь(ключ, значение);
		}
}

bool EscValue::HasNumberField(const char *id) const
{
	return IsMap() && дайМап().найди(id) >= 0;
}

int  EscValue::GetFieldInt(const char *id) const
{
	return HasNumberField(id) ? MapGet(id).дайЦел() : 0;
}

}
