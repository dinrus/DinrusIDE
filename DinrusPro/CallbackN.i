#define MP_
#define P_p_
#define p_
#define обрвызн обрвыз
#define птеобрн  птеобр

#include "CallbackNP.i"

#define classP    class P1, class MP1
#define MP_       MP1
#define P_p_      , P1 p1
#define p_        p1
#define обрвызн обрвыз1
#define птеобрн  птеобр1

#include "CallbackNP.i"

#define classP    class P1, class P2, class MP1, class MP2
#define MP_       MP1, MP2
#define P_p_      , P1 p1, P2 p2
#define p_        p1, p2
#define обрвызн обрвыз2
#define птеобрн  птеобр2

#include "CallbackNP.i"

#define classP    class P1, class P2, class P3, class MP1, class MP2, class MP3
#define MP_       MP1, MP2, MP3
#define P_p_      , P1 p1, P2 p2, P3 p3
#define p_        p1, p2, p3
#define обрвызн обрвыз3
#define птеобрн  птеобр3

#include "CallbackNP.i"

#define classP    class P1, class P2, class P3, class P4, class MP1, class MP2, class MP3, class MP4
#define MP_       MP1, MP2, MP3, MP4
#define P_p_      , P1 p1, P2 p2, P3 p3, P4 p4
#define p_        p1, p2, p3, p4
#define обрвызн обрвыз4
#define птеобрн  птеобр4

#include "CallbackNP.i"

#define classP    class P1, class P2, class P3, class P4, class P5, class MP1, class MP2, class MP3, class MP4, class MP5
#define MP_       MP1, MP2, MP3, MP4, MP5
#define P_p_      , P1 p1, P2 p2, P3 p3, P4 p4, P5 p5
#define p_        p1, p2, p3, p4, p5
#define обрвызн обрвыз5
#define птеобрн  птеобр5

#include "CallbackNP.i"

#ifdef classA
	#undef classA
#endif
#undef A_
#undef A_a_
#undef a_
