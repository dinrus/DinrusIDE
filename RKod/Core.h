#ifndef _RKod_Core_h_
#define _RKod_Core_h_
#include <Core/Core.h>

#define ПИ_РНЦП     namespace РНЦПДинрус {
#define КОНЕЦ_ПИ_РНЦП }
#define РНЦП               РНЦПДинрус

namespace РНЦПДинрус {

#ifdef CPU_SIMD
Ткст какТкст(const f32x4& x);
Ткст какТкст(const i32x4& x);
Ткст какТкст(const i16x8& x);
Ткст какТкст(const i8x16& x);
#endif
}

void      RegisterTopic__(const char *topicfile, const char *topic, const char *title, const РНЦП::byte *данные, int len);

#ifdef PLATFORM_WIN32
typedef HMODULE УКДЛЛ;
#else
typedef void   *УКДЛЛ;
#endif

УКДЛЛ грузиДлл__(РНЦП::Ткст& фн, const char *const *names, void *const *procs);
void      освободиДлл__(УКДЛЛ dllhandle);






#endif
