#include <DinrusPro/DinrusCore.h>

#include "lib/xxhash.h"


ххХэшПоток::ххХэшПоток(бцел seed)
{
	STATIC_ASSERT(sizeof(context) >= sizeof(XXH32_state_t));
	переустанов(seed);
}

проц ххХэшПоток::переустанов(бцел seed)
{
	XXH32_reset((XXH32_state_t *)context, seed);
}

проц ххХэшПоток::выведи(кук данные, бцел size)
{
	XXH32_update((XXH32_state_t *)context, данные, size);
}

цел ххХэшПоток::финиш()
{
	слей();
	return XXH32_digest((XXH32_state_t *)context);
}

цел ххХэш(кук данные, т_мера len)
{
	ххХэшПоток h;
	h.помести64(данные, len);
	return h.финиш();
}

цел ххХэш(const Ткст& s)
{
	return ххХэш(~s, s.дайСчёт());
}

ххХэш64Поток::ххХэш64Поток(бцел seed)
{
	STATIC_ASSERT(sizeof(context) >= sizeof(XXH64_state_t));
	переустанов(seed);
}

проц ххХэш64Поток::переустанов(бцел seed)
{
	XXH64_reset((XXH64_state_t *)context, seed);
}

проц ххХэш64Поток::выведи(кук данные, бцел size)
{
	XXH64_update((XXH64_state_t *)context, данные, size);
}

дол ххХэш64Поток::финиш()
{
	слей();
	return XXH64_digest((XXH64_state_t *)context);
}

дол ххХэш64(кук данные, т_мера len)
{
	ххХэш64Поток h;
	h.помести64(данные, len);
	return h.финиш();
}

дол ххХэш64(const Ткст& s)
{
	return ххХэш64(~s, s.дайСчёт());
}

