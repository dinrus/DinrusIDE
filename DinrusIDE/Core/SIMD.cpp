#include "Core.h"

#ifdef CPU_SIMD

namespace РНЦПДинрус {

Ткст какТкст(const f32x4& x)
{
	float *f = (float *)&x;
	return фмт("%g %g %g %g", f[3], f[2], f[1], f[0]);
}

Ткст какТкст(const i32x4& x)
{
	int *f = (int *)&x;
	return фмт("%d %d %d %d", f[3], f[2], f[1], f[0]);
}

Ткст какТкст(const i16x8& x)
{
	int16 *f = (int16 *)&x;
	return фмт("%d %d %d %d  %d %d %d %d", f[7], f[6], f[5], f[4], f[3], f[2], f[1], f[0]);
}

Ткст какТкст(const i8x16& x)
{
	int8 *f = (int8 *)&x;
	return фмт("%d %d %d %d . %d %d %d %d . %d %d %d %d . %d %d %d %d",
	              f[15], f[14], f[13], f[12], f[11], f[10], f[9], f[8],
	              f[7], f[6], f[5], f[4], f[3], f[2], f[1], f[0]);
}

};

#endif
