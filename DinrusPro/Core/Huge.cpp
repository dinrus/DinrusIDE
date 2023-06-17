#include <DinrusPro/DinrusCore.h>

Хьюдж::Хьюдж()
{
	size = 0;
}

ббайт *Хьюдж::добавьКусок()
{
	size += КУСОК;
	return данные.добавь().данные;
}

проц Хьюдж::финиш(цел last_chunk_size)
{
	size = size - КУСОК + last_chunk_size;
}

проц Хьюдж::дай(ук t_, т_мера pos, т_мера sz) const
{
	ПРОВЕРЬ(pos + sz <= size);
	цел blki = цел(pos / КУСОК);
	т_мера blkpos = pos & (КУСОК - 1);
	ббайт *t = (ббайт *)t_;
	
	while(sz > 0) {
		т_мера m = мин(sz, КУСОК - blkpos);
		копирпам8(t, данные[blki].данные + blkpos, m);
		t += m;
		sz -= m;
		blkpos = 0;
		blki++;
	}
}

Ткст Хьюдж::дай() const
{
	if(size >= INT_MAX)
		паника("Ткст is too big!");
	ТкстБуф sb((цел)size);
	дай(~sb, 0, size);
	return Ткст(sb);
}
