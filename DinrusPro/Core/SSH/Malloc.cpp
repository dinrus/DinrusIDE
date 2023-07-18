#include "SSH.h"

#define MLOG(x)  //  LOG(x)

#ifdef КУЧА_РНЦП

static std::atomic<дол> РНЦП_SSH_alloc;

static ук ssh_malloc(т_мера size, проц **abstract)
{
	MLOG("размести " << size);
	т_мера alloc = size + sizeof(дол);
	дол *aptr = (дол *)разместиПамТн(alloc);
	*aptr++ = (дол)alloc;
	РНЦП_SSH_alloc += alloc;
	MLOG("РНЦП_SSH_MALLOC(" << (дол)size << ", alloc " << alloc << ") -> " << фмтЦелГекс(aptr) << ", total = " << (дол) РНЦП_SSH_alloc);
	return aptr;
}

static проц ssh_free(ук укз, проц **abstract)
{
	if(!укз)
		return;
	дол *aptr = (дол *)укз - 1;
	РНЦП_SSH_alloc -= *aptr;
	MLOG("РНЦП_SSH_FREE(" << укз << ", alloc " << *aptr << "), total = " << (дол) РНЦП_SSH_alloc);
	освободиПам(aptr);
}

static ук ssh_realloc(ук укз, т_мера size, проц** abstract)
{
	if(!укз)
		return NULL;
	дол *aptr = (дол *)укз - 1;
	if((дол)(size + sizeof(дол)) <= *aptr) {
		MLOG("РНЦП_SSH_REALLOC(" << укз << ", " << (дол)size << ", alloc " << *aptr << ") -> keep same block");
		return укз;
	}
	т_мера newalloc = size + sizeof(дол);
	дол *newaptr = (дол *)разместиПамТн(newalloc);
	if(!newaptr) {
		MLOG("РНЦП_SSH_REALLOC(" << укз << ", " << (дол)size << ", alloc " << newalloc << ") -> fail");
		return NULL;
	}
	*newaptr++ = newalloc;
	memcpy(newaptr, укз, мин<цел>((цел)(*aptr - sizeof(дол)), (цел)size));
	РНЦП_SSH_alloc += newalloc - *aptr;
	MLOG("РНЦП_SSH_REALLOC(" << укз << ", " << (дол)size << ", alloc " << newalloc << ") -> "
		<< фмтЦелГекс(newaptr) << ", total = " << (дол) РНЦП_SSH_alloc);
	освободиПам(aptr);
	return newaptr;
}

#endif
