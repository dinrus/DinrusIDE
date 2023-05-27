#include <DinrusPro/DinrusPro.h>

namespace ДинрусРНЦП {

Huge::Huge()
{
	size = 0;
}

ббайт *Huge::AddChunk()
{
	size += CHUNK;
	return данные.добавь().данные;
}

проц Huge::финиш(цел last_chunk_size)
{
	size = size - CHUNK + last_chunk_size;
}

проц Huge::дай(ук t_, т_мера pos, т_мера sz) const
{
	ПРОВЕРЬ(pos + sz <= size);
	цел blki = цел(pos / CHUNK);
	т_мера blkpos = pos & (CHUNK - 1);
	ббайт *t = (ббайт *)t_;
	
	while(sz > 0) {
		т_мера m = мин(sz, CHUNK - blkpos);
		копирпам8(t, данные[blki].данные + blkpos, m);
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
	ТкстБуф sb((цел)size);
	дай(~sb, 0, size);
	return Ткст(sb);
}

}