//#BLITZ_APPROVE

//$-

#define IMAGE_META(k, v)
#define IMAGE_SCAN(s)
#define IMAGE_PACKED(n, d)

#define PREMULTIPLIED
#define IMAGE_BEGIN_DATA
#define IMAGE_DATA(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa,ab,ac,ad,ae,af,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,ba,bb,bc,bd,be,bf)
#define IMAGE_END_DATA(n, c)

class IMAGECLASS {
public:
#define IMAGE_BEGIN(n) I_##n,
#define IMAGE_ID(n) I_##n,
	enum {
#include IMAGEFILE
		COUNT
	};
#undef  IMAGE_BEGIN
#undef  IMAGE_ID

public:
	static РНЦП::Iml&   Iml();

	static void        Register__()                { регистрируй(КАКТКСТ(IMAGECLASS), Iml()); }

	static int         найди(const РНЦП::Ткст& s);
	static int         найди(const char *s);
	static int         дайСчёт()                  { return Iml().дайСчёт(); }
	static РНЦП::Ткст дайИд(int i)                { return Iml().дайИд(i); }

	static РНЦП::Рисунок  дай(int i);
	static РНЦП::Рисунок  дай(const char *s);
	static РНЦП::Рисунок  дай(const РНЦП::Ткст& s);

	static void   уст(int i, const РНЦП::Рисунок& m);
	static void   уст(const char *s, const РНЦП::Рисунок& m);

	static void   переустанов()                     { Iml().переустанов(); }

#define IMAGE_BEGIN(n) static РНЦП::Рисунок n() { return дай(I_##n); }
#define IMAGE_ID(n) static РНЦП::Рисунок n() { return дай(I_##n); }
#include IMAGEFILE
#undef  IMAGE_BEGIN
#undef  IMAGE_ID

};

#undef  IMAGE_SCAN
#undef  IMAGE_PACKED
#undef  IMAGE_META

#undef  IMAGE_BEGIN_DATA
#undef  IMAGE_END_DATA
#undef  IMAGE_DATA

#ifndef IMAGE_KEEP
#undef  IMAGECLASS
#undef  IMAGEFILE
#endif
