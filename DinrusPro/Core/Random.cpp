#include <DinrusPro/DinrusCore.h>

/*  Written in 2018 by David Blackman and Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

/* This is xoshiro256** 1.0, our all-purpose, rock-solid generator. It has
   excellent (sub-ns) speed, a state (256 bits) that is large enough for
   any parallel application, and it passes all tests we are aware of.

   For generating just floating-point numbers, xoshiro256+ is even faster.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */

static форс_инлайн uint64_t s_rotl(const uint64_t x, цел k) {
	return (x << k) | (x >> (64 - k)); // GCC/CLANG/MSC happily optimize this
}

форс_инлайн
static бдол sNext(бдол *s)
{
	const uint64_t result_starstar = s_rotl(s[1] * 5, 7) * 9;
	const uint64_t t = s[1] << 17;

	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];

	s[2] ^= t;

	s[3] = s_rotl(s[3], 45);

	return result_starstar;
}

никогда_inline
static проц sSeed(бдол *s)
{
#ifdef PLATFORM_POSIX
	цел fd = open("/dev/urandom", O_RDONLY);
	if(fd != -1) {
		ИГНОРРЕЗ(
			read(fd, s, 4 * sizeof(бдол))
		);
		close(fd);
	}
#else
	for(цел pass = 0; pass < 4; pass++) {
		for(цел i = 0; i < 4; i++) {
			КомбХэш h[2];
			for(цел j = 0; j < 2; j++) {
				h[j] << дайСисВремя().дай() << usecs() << msecs() << j << pass << i;
				for(цел p = 0; p < 2; p++) {
					Ууид uuid;
					CoCreateGuid((GUID *)&uuid); // GUID is basically a random number...
					h[j] << uuid.v[0] << uuid.v[1];
				}
			}
			s[i] ^= СДЕЛАЙБДОЛ(h[0], h[1]);
		}
	}
#endif
}

форс_инлайн
static бдол *sState()
{
	thread_local бдол *s;
	if(!s) {
		thread_local бдол state[4];
		s = state;
		sSeed(s);
#ifdef PLATFORM_POSIX
		if(Нить::главная_ли()) // non-main threads do not work with fork anyway
			pthread_atfork(NULL, NULL, [] { sSeed(s); }); // reseed random generator after fork
#endif
	}
	return s;
}

бцел случ()
{
	return (бцел)sNext(sState());
}

бдол случ64()
{
	return sNext(sState());
}

проц случ64(бдол *t, цел n) {
	бдол *s = sState();
	while(n-- > 0)
		*t++ = sNext(s);
}

бцел случ(бцел n) {
	ПРОВЕРЬ(n);
	бдол *s = sState();
	бцел mask = (1 << значимыеБиты(n)) - 1;
	бцел r;
	do
		r = (бцел)sNext(s) & mask;
	while(r >= n);
	return r;
}

бдол случ64(бдол n) {
	ПРОВЕРЬ(n);
	бдол *s = sState();
	бдол mask = ((бдол)1 << значимыеБиты64(n)) - 1;
	бдол r;
	do
		r = sNext(s) & mask;
	while(r >= n);
	return r;
}

дво случПЗ()
{
	return (sNext(sState()) >> 11) * (1. / (UINT64_C(1) << 53));
}

проц засейСлуч()
{
	sSeed(sState());
}

проц засейСлуч(бцел seed) {
	бдол *s = sState();
	for(цел i = 0; i < 4; i++)
		s[i] = 12345678 + seed + i; // xoshiro does not work well if all is zero
}
