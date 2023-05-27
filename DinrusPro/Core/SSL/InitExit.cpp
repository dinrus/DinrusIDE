#include "SSL.h"

#define LLOG(x) // DLOG(x)

namespace ДинрусРНЦП {

#ifdef КУЧА_РНЦП

static дол РНЦП_SSL_alloc = 0;

#if OPENSSL_VERSION_NUMBER >= 0x10100000L
static ук SslAlloc(т_мера size, кткст0 file, цел line)
#else
static ук SslAlloc(т_мера size)
#endif
{
	т_мера alloc = size + sizeof(дол);
	дол *aptr = (дол *)разместиПамТн(alloc);
	*aptr++ = (дол)alloc;
	РНЦП_SSL_alloc += alloc;
	LLOG("РНЦП_SSL_MALLOC(" << (дол)size << ", size " << size
         << ", " << file << " " << line
	     << ") -> " << aptr << ", MemorySize: " << дайРазмБлокаПам(aptr)
	     << ", total = " << РНЦП_SSL_alloc << ", thread: " << Нить::дайИдТекущ());
	return aptr;
}

#if OPENSSL_VERSION_NUMBER >= 0x10100000L
static проц SslFree(ук укз, кткст0 file, цел line)
#else
static проц SslFree(ук укз)
#endif
{
	if(!укз)
		return;
	дол *aptr = (дол *)укз - 1;
	РНЦП_SSL_alloc -= *aptr;
	LLOG("РНЦП_SSL_FREE(" << укз << ", size " << *aptr
	                     << ", " << file << " " << line
	                     << "), MemorySize: " << дайРазмБлокаПам(aptr) << ", total = " << РНЦП_SSL_alloc << ", thread: " << Нить::дайИдТекущ());
	if(*aptr !=  дайРазмБлокаПам(aptr))
		паника("SslFree corrupted");
	освободиПам(aptr);
}

#if OPENSSL_VERSION_NUMBER >= 0x10100000L
static ук SslRealloc(ук укз, т_мера size, кткст0 file, цел line)
#else
static ук SslRealloc(ук укз, т_мера size)
#endif
{
	LLOG("SslRealloc " << укз << ", " << size);
	if(!укз) {
		LLOG("РНЦП_SSL_REALLOC by размести:");
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
		return SslAlloc(size, file, line);
#else
		return SslAlloc(size);
#endif
	}
	дол *aptr = (дол *)укз - 1;
	if((дол)(size + sizeof(дол)) <= *aptr) { // TODO: делай we really want this?
		LLOG("РНЦП_SSL_REALLOC(" << укз << ", " << (дол)size << ", alloc " << *aptr << ") -> keep same block" << ", thread: " << Нить::дайИдТекущ());
		return укз;
	}
	т_мера newalloc = size + sizeof(дол);
	дол *newaptr = (дол *)разместиПамТн(newalloc);
	if(!newaptr) {
		LLOG("РНЦП_SSL_REALLOC(" << укз << ", " << (дол)size << ", alloc " << newalloc << ") -> fail" << ", thread: " << Нить::дайИдТекущ());
		return NULL;
	}
	*newaptr++ = newalloc;
	memcpy(newaptr, укз, мин<цел>((цел)(*aptr - sizeof(дол)), (цел)size));
	РНЦП_SSL_alloc += newalloc - *aptr;
	LLOG("РНЦП_SSL_REALLOC(" << укз << ", " << (дол)size << ", alloc " << newalloc
	     << ", " << file << " " << line
	     << ") -> " << newaptr << ", total = " << РНЦП_SSL_alloc << ", thread: " << Нить::дайИдТекущ());
	освободиПам(aptr);
	return newaptr;
}

#endif

проц TcpSocketИниt();

ИНИЦИАЛИЗАТОР(SSL)
{
	MemoryIgnoreLeaksBlock __;
	LLOG("SslИниt");
	TcpSocketИниt();
#ifdef КУЧА_РНЦП
	CRYPTO_set_mem_functions(SslAlloc, SslRealloc, SslFree);
#endif
	SSL_library_init();
	SSL_load_error_strings();
}

ЭКЗИТБЛОК
{
	MemoryIgnoreLeaksBlock __;
	CONF_modules_unload(1);
	EVP_cleanup();
	ENGINE_cleanup();
	CRYPTO_cleanup_all_ex_data();
#if OPENSSL_VERSION_NUMBER < 0x10100000L
	STACK_OF(SSL_COMP) *pCOMP = SSL_COMP_get_compression_methods();
	if(pCOMP)
		sk_SSL_COMP_free( pCOMP );
	ERR_remove_state(0);
#else
//	ERR_remove_thread_state(NULL);
	OPENSSL_thread_stop();
#endif
	ERR_free_strings();
}


static thread_local бул sThreadИниt;
static thread_local проц (*sPrevExit)();

static проц sslExitThread()
{
#if OPENSSL_VERSION_NUMBER < 0x10100000L
	ERR_remove_state(0);
#else
//	ERR_remove_thread_state(NULL);
	OPENSSL_thread_stop();
#endif
	if(sPrevExit)
		(*sPrevExit)();
}

проц SslИниtThread()
{
	MemoryIgnoreLeaksBlock __;
	if(sThreadИниt || Нить::главная_ли())
		return;
	sThreadИниt = true;
	sPrevExit = Нить::приВыходе(sslExitThread);
}

}
