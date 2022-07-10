#include "Core.h"

#include "lib/xxhash.h"

namespace РНЦПДинрус {

ххХэшПоток::ххХэшПоток(dword seed)
{
	STATIC_ASSERT(sizeof(context) >= sizeof(XXH32_state_t));
	переустанов(seed);
}

void ххХэшПоток::переустанов(dword seed)
{
	XXH32_reset((XXH32_state_t *)context, seed);
}

void ххХэшПоток::выведи(const void *данные, dword size)
{
	XXH32_update((XXH32_state_t *)context, данные, size);
}

int ххХэшПоток::финиш()
{
	слей();
	return XXH32_digest((XXH32_state_t *)context);
}

int xxHash(const void *данные, size_t len)
{
	ххХэшПоток h;
	h.помести64(данные, len);
	return h.финиш();
}

int xxHash(const Ткст& s)
{
	return xxHash(~s, s.дайСчёт());
}

ххХэш64Поток::ххХэш64Поток(dword seed)
{
	STATIC_ASSERT(sizeof(context) >= sizeof(XXH64_state_t));
	переустанов(seed);
}

void ххХэш64Поток::переустанов(dword seed)
{
	XXH64_reset((XXH64_state_t *)context, seed);
}

void ххХэш64Поток::выведи(const void *данные, dword size)
{
	XXH64_update((XXH64_state_t *)context, данные, size);
}

int64 ххХэш64Поток::финиш()
{
	слей();
	return XXH64_digest((XXH64_state_t *)context);
}

int64 xxHash64(const void *данные, size_t len)
{
	ххХэш64Поток h;
	h.помести64(данные, len);
	return h.финиш();
}

int64 xxHash64(const Ткст& s)
{
	return xxHash64(~s, s.дайСчёт());
}

};