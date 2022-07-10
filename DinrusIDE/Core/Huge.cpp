#include "Core.h"

namespace РНЦПДинрус {

Huge::Huge()
{
	size = 0;
}

byte *Huge::AddChunk()
{
	size += CHUNK;
	return данные.добавь().данные;
}

void Huge::финиш(int last_chunk_size)
{
	size = size - CHUNK + last_chunk_size;
}

void Huge::дай(void *t_, size_t pos, size_t sz) const
{
	ПРОВЕРЬ(pos + sz <= size);
	int blki = int(pos / CHUNK);
	size_t blkpos = pos & (CHUNK - 1);
	byte *t = (byte *)t_;
	
	while(sz > 0) {
		size_t m = min(sz, CHUNK - blkpos);
		memcpy8(t, данные[blki].данные + blkpos, m);
		t += m;
		sz -= m;
		blkpos = 0;
		blki++;
	}
}

Ткст Huge::дай() const
{
	if(size >= INT_MAX)
		паника("Ткст is too big!");
	ТкстБуф sb((int)size);
	дай(~sb, 0, size);
	return Ткст(sb);
}

}