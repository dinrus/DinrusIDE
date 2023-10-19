#include <DinrusPro/DinrusCore.h>

// iscale: computes x * y / z.

цел iscale(цел x, цел y, цел z)
{
#if CPU_32 && COMPILER_MSC
	__asm {
		mov		eax, [x]
		imul	[y]
		idiv	[z]
	}
#else
	дол res = x;
	res *= y;
	res /= z;
	return (цел)res;
#endif
}

// iscalefloor: computes x * y / z, rounded towards -infty.

цел iscalefloor(цел x, цел y, цел z)
{
#if CPU_32 && COMPILER_MSC
	__asm {
		mov		eax, [x]
		imul	[y]
		idiv	[z]
		and		edx, edx
		jge		__1
		dec		eax
	__1:
	}
#else
	дол res = x;
	дол mulres = res * y;
	res = mulres / z;
	if(res * z != mulres)
		res--;
	return (цел)res;
#endif
}

// iscaleceil: computes x * y / z, rounded towards +infty.

цел iscaleceil(цел x, цел y, цел z)
{
#if CPU_32 && COMPILER_MSC
	__asm {
		mov		eax, [x]
		imul	[y]
		idiv	[z]
		and		edx, edx
		jle		__1
		inc		eax
	__1:
	}
#else
	дол res = x;
	дол mulres = res * y;
	res = mulres / z;
	if(res * z != mulres)
		res++;
	return (цел)res;
#endif
}

#ifdef COMPILER_MSC
#pragma warning(default: 4035)
#endif

// idivfloor: returns x / y, truncated towards -infinity.

цел idivfloor(цел x, цел y)
{
	return x / y - (x % y < 0);
}

// idivceil: returns x / y, truncated towards +infinity.

цел idivceil(цел x, цел y)
{
	return x / y + (x % y > 0);
}

// itimesfloor: moves x to nearest lower multiple of y.

цел itimesfloor(цел x, цел y)
{
	return y * idivfloor(x, y);
}

// itimesceil: moves x to nearest higher multiple of y.

цел itimesceil(цел x, цел y)
{
	return y * idivceil(x, y);
}

// fround: converts дво to nearest integer. Checks integer limits.

цел fround(дво значение)
{
	return значение >= INT_MAX ? INT_MAX : значение <= INT_MIN ? INT_MIN
		: (цел)floor(значение + 0.5);
}

// ffloor: converts дво to integer rounding towards -infinity. Checks integer limits.

цел ffloor(дво значение)
{
	return значение >= INT_MAX ? INT_MAX : значение <= INT_MIN ? INT_MIN
		: (цел)floor(значение);
}

// fceil: converts дво to integer rounding towards +infinity. Checks integer limits.

цел fceil(дво значение)
{
	return значение >= INT_MAX ? INT_MAX : значение <= INT_MIN ? INT_MIN
		: (цел)ceil(значение);
}

// fround: converts дво to nearest integer. Checks integer limits.

дол fround64(дво значение)
{
	return значение >= (дво)ЦЕЛ64_МАКС ? ЦЕЛ64_МАКС : значение <= (дво)ЦЕЛ64_МИН ? ЦЕЛ64_МИН
		: (дол)floor(значение + 0.5);
}

// ffloor: converts дво to integer rounding towards -infinity. Checks integer limits.

дол ffloor64(дво значение)
{
	return значение >= (дво)ЦЕЛ64_МАКС ? ЦЕЛ64_МАКС : значение <= (дво)ЦЕЛ64_МИН ? ЦЕЛ64_МИН
		: (дол)floor(значение);
}

// fceil: converts дво to integer rounding towards +infinity. Checks integer limits.

дол fceil64(дво значение)
{
	return значение >= (дво)ЦЕЛ64_МАКС ? ЦЕЛ64_МАКС : значение <= (дво)ЦЕЛ64_МИН ? ЦЕЛ64_МИН
		: (дол)ceil(значение);
}

// какТкст: converts дво numbers to strings.

Ткст какТкст(дво x, цел digits)
{
	return фмтГ(x, digits);
}

// modulo: working версия of math function `fmod'.

дво modulo(дво x, дво y)
{
	return y ? (x - y * floor(x / y)) : x;
}

цел ilog10(дво d)
{
	if(пусто_ли(d) || d == 0)
		return Null;
	return fceil(log10(fabs(d)));
}

дво ipow10(цел i)
{
	static const дво pow_tbl[] =
	{
		1e00,
		1e01, 1e02, 1e03, 1e04, 1e05, 1e06, 1e07, 1e08, 1e09, 1e10,
		1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19, 1e20,
		1e21, 1e22, 1e23, 1e24, 1e25, 1e26, 1e27, 1e28, 1e29, 1e30,
		1e31, 1e32, 1e33, 1e34, 1e35, 1e36, 1e37, 1e38, 1e39, 1e40,
		1e41, 1e42, 1e43, 1e44, 1e45, 1e46, 1e47, 1e48, 1e49, 1e50,
	};
	if(i >= 0 && i < __количество(pow_tbl))
		return pow_tbl[i];
	else if(i < 0 && i > -__количество(pow_tbl))
		return 1 / pow_tbl[-i];
	return pow(10.0, (дво)i);
}

дво normalize(дво d, цел& exp)
{
	if(пусто_ли(d) || d == 0)
	{
		exp = Null;
		return d;
	}
	бул sign = (d < 0);
	if(sign) d = -d;
	exp = минмакс<цел>(ilog10(d), -308, +308); // 8-ббайт дво!
	d /= ipow10(exp);
	if(d >= 10) { d /= 10; exp++; }
	if(d < 1)   { d *= 10; exp--; }
	return sign ? -d : d;
}

дво roundr(дво d, цел digits)
{
	цел i = ilog10(d);
	if(пусто_ли(i))
		return d;
	дво fac = ipow10(i - digits);
	return floor(d / fac + 0.5) * fac;
}

дво floorr(дво d, цел digits)
{
	цел i = ilog10(d);
	if(пусто_ли(i))
		return d;
	дво fac = ipow10(i - digits);
	return floor(d / fac) * fac;
}

дво ceilr(дво d, цел digits)
{
	цел i = ilog10(d);
	if(пусто_ли(i))
		return d;
	дво fac = ipow10(i - digits);
	return ceil(d / fac) * fac;
}

цел SolveQuadraticEquation(дво a, дво b, дво c, дво *r)
{
	if(a == 0) {
		if(b == 0)
			return 0;
		r[0] = r[1] = -c / b;
		return 1;
    }
	дво d = b * b - 4 * a * c;
	if (d < 0)
		return 0;
	a *= 2;
	if (d == 0) {
		r[0] = r[1] = -b / a;
		return 1;
	}
	d = sqrt(d);
	if(b == 0) {
		r[0] = d / a;
		r[1] = -r[0];
    }
	else {
		r[0] = (-b + d) / a;
		r[1] = (-b - d) / a;
    }
	return 2;
}
