#ifndef _DinrusPro_Linux_h_
#define _DinrusPro_Linux_h_

#if defined(PLATFORM_POSIX)
	#ifndef __USE_FILE_OFFSET64
		#define __USE_FILE_OFFSET64
	#endif
	#define DIR_SEP  '/'
	#define DIR_SEPS "/"
	#define PLATFORM_PATH_HAS_CASE 1

	#include <errno.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/time.h>
	#include <sys/file.h>
	#include <time.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <pthread.h>
	#include <semaphore.h>
	#include <memory.h>
	#include <dirent.h>
	#include <signal.h>
	#include <syslog.h>
	#include <float.h>
	#include <fenv.h>
	#ifdef PLATFORM_SOLARIS
		#include <inttypes.h>
	#else
		#include <stdint.h>
	#endif
	#ifdef PLATFORM_OSX
		#include <dispatch/dispatch.h>
	#endif
	#define LOFF_T_      off_t
	#define LSEEK64_     lseek
	#define FTRUNCATE64_ ftruncate
	
	#define W_P(w, p) p
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netdb.h>
	#include <netinet/in.h>
	//#include <libiberty.h>
	enum
	{
		INVALID_SOCKET = -1,
		TCP_NODELAY    = 1,
		SD_RECEIVE     = 0,
		SD_SEND        = 1,
		SD_BOTH        = 2,
	};
	typedef цел SOCKET;
#endif

#ifdef PLATFORM_LINUX
#undef  LOFF_T_
#define LOFF_T_      loff_t
#undef  LSEEK64_
#define LSEEK64_     lseek64
#undef  FTRUNCATE64_
#define FTRUNCATE64_ ftruncate64
#endif

#endif
