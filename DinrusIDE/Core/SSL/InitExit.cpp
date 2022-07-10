#include "SSL.h"

#define LLOG(x) // DLOG(x)

namespace РНЦПДинрус {

#ifdef КУЧА_РНЦП

static int64 РНЦП_SSL_alloc = 0;

#if OPENSSL_VERSION_NUMBER >= 0x10100000L
static void *SslAlloc(size_t size, const char *file, int line)
#else
static void *SslAlloc(size_t size)
#endif
{
	size_t alloc = size + sizeof(int64);
	int64 *aptr = (int64 *)MemoryAllocSz(alloc);
	*aptr++ = (int64)alloc;
	РНЦП_SSL_alloc += alloc;
	LLOG("РНЦП_SSL_MALLOC(" << (int64)size << ", size " << size
         << ", " << file << " " << line
	     << ") -> " << aptr << ", MemorySize: " << GetMemoryBlockSize(aptr)
	     << ", total = " << РНЦП_SSL_alloc << ", thread: " << Нить::дайИдТекущ());
	return aptr;
}

#if OPENSSL_VERSION_NUMBER >= 0x10100000L
static void SslFree(void *ptr, const char *file, int line)
#else
static void SslFree(void *ptr)
#endif
{
	if(!ptr)
		return;
	int64 *aptr = (int64 *)ptr - 1;
	РНЦП_SSL_alloc -= *aptr;
	LLOG("РНЦП_SSL_FREE(" << ptr << ", size " << *aptr
	                     << ", " << file << " " << line
	                     << "), MemorySize: " << GetMemoryBlockSize(aptr) << ", total = " << РНЦП_SSL_alloc << ", thread: " << Нить::дайИдТекущ());
	if(*aptr !=  GetMemoryBlockSize(aptr))
		паника("SslFree corrupted");
	MemoryFree(aptr);
}

#if OPENSSL_VERSION_NUMBER >= 0x10100000L
static void *SslRealloc(void *ptr, size_t size, const char *file, int line)
#else
static void *SslRealloc(void *ptr, size_t size)
#endif
{
	LLOG("SslRealloc " << ptr << ", " << size);
	if(!ptr) {
		LLOG("РНЦП_SSL_REALLOC by размести:");
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
		return SslAlloc(size, file, line);
#else
		return SslAlloc(size);
#endif
	}
	int64 *aptr = (int64 *)ptr - 1;
	if((int64)(size + sizeof(int64)) <= *aptr) { // TODO: делай we really want this?
		LLOG("РНЦП_SSL_REALLOC(" << ptr << ", " << (int64)size << ", alloc " << *aptr << ") -> keep same block" << ", thread: " << Нить::дайИдТекущ());
		return ptr;
	}
	size_t newalloc = size + sizeof(int64);
	int64 *newaptr = (int64 *)MemoryAllocSz(newalloc);
	if(!newaptr) {
		LLOG("РНЦП_SSL_REALLOC(" << ptr << ", " << (int64)size << ", alloc " << newalloc << ") -> fail" << ", thread: " << Нить::дайИдТекущ());
		return NULL;
	}
	*newaptr++ = newalloc;
	memcpy(newaptr, ptr, min<int>((int)(*aptr - sizeof(int64)), (int)size));
	РНЦП_SSL_alloc += newalloc - *aptr;
	LLOG("РНЦП_SSL_REALLOC(" << ptr << ", " << (int64)size << ", alloc " << newalloc
	     << ", " << file << " " << line
	     << ") -> " << newaptr << ", total = " << РНЦП_SSL_alloc << ", thread: " << Нить::дайИдТекущ());
	MemoryFree(aptr);
	return newaptr;
}

#endif

void TcpSocketИниt();

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


static thread_local bool sThreadИниt;
static thread_local void (*sPrevExit)();

static void sslExitThread()
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

void SslИниtThread()
{
	MemoryIgnoreLeaksBlock __;
	if(sThreadИниt || Нить::главная_ли())
		return;
	sThreadИниt = true;
	sPrevExit = Нить::приВыходе(sslExitThread);
}

}
