#ifndef _DinrusPro_Windows_h_
#define _DinrusPro_Windows_h_

#include <DinrusPro/Windows.h>
#include <intrin.h>

	#if defined(COMPILER_MSC) && defined(CPU_X86)
		#pragma warning(disable: 4035)
	#else
		#ifndef __NOASSEMBLY__
			#define __NOASSEMBLY__
		#endif
	#endif

	#if defined(COMPILER_MINGW)
		#if !defined(WINVER)
			#define WINVER 0xFFFF
		#endif
		#include <float.h>
	#endif

	#define DIR_SEP  '\\'
	#define DIR_SEPS "\\"
	#define PLATFORM_PATH_HAS_CASE 0
	#ifndef PLATFORM_WINCE
		#include <io.h>
	#endif
	#ifndef PLATFORM_MFC // just mini Windows заголовки
		#ifdef COMPILER_MSC
			#ifndef CPU_ARM
				#ifndef CPU_AMD64
					#ifndef _X86_
						#define _X86_
					#endif
				#else
					#ifndef _AMD64_
						#define _AMD64_
					#endif
					#ifndef __NOASSEMBLY__
						#define __NOASSEMBLY__
					#endif
					#ifndef WIN64
						#define WIN64
					#endif
				#endif
			#endif
			#ifndef _WINDOWS_
				#define _WINDOWS_
			#endif
			#ifndef _INC_WINDOWS
				#define _INC_WINDOWS
			#endif
			#ifndef _STRUCT_NAME
				#define _STRUCT_NAME(x)
				#define DUMMYSTRUCTNAME
				#define DUMMYSTRUCTNAME2
				#define DUMMYSTRUCTNAME3
			#endif
			#ifndef NO_STRICT
				#ifndef STRICT
					#define STRICT 1
				#endif
			#endif
			#include <stdarg.h>
			#include <windef.h>
			#include <winbase.h>
			#include <wingdi.h>
			#include <winuser.h>
			#include <Wincon.h>
			#include <плав.h>
			#include <mmsystem.h>
		#define ббайт win32_byte_ // RpcNdr defines ббайт -> class with ббайт
		#define CY win32_CY_
			#include <objidl.h>
			#include <winnetwk.h>
		#undef ббайт
		#undef CY
			typedef DWORD LCTYPE;
			#define W_P(w, p) w
			#include <winsock2.h>
			#include <ws2tcpip.h>
			typedef цел socklen_t;
		#else
			#define W_P(w, p) w
			#if !defined(PLATFORM_CYGWIN)
			#include <winsock2.h>
			#include <ws2tcpip.h>
			#endif
			typedef цел socklen_t;
			#define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */
			#include <windows.h>
			#include <stdint.h>
		#endif
		#include <process.h>
	#endif

#endif

#define КРИТСЕКЦ RTL_CRITICAL_SECTION