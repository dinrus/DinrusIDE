#ifdef PLATFORM_MACOS
struct КЗСА : Движ<КЗСА> {
        ббайт a, r, g, b;
};
#else
struct КЗСА : Движ<КЗСА> {
	ббайт b, g, r, a;
};
#endif

inline Поток& operator%(Поток& s, КЗСА& c)
{
	return s % c.r % c.g % c.b % c.a;
}

template <> Ткст какТкст(const КЗСА& c);

template<> inline т_хэш дайХэшЗнач(const КЗСА& a) { return *(бцел *)&a; }

inline бул operator==(const КЗСА& a, const КЗСА& b)
{
	return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

inline бул operator!=(const КЗСА& a, const КЗСА& b)
{
	return !(a == b);
}

inline КЗСА обнулиКЗСА() { КЗСА c; c.r = c.g = c.b = c.a = 0; return c; }

//#ifndef PLATFORM_WIN32
inline цел        дайЗнК(бцел c)             { return (ббайт)(c >> 0); }
inline цел        дайЗнЗ(бцел c)             { return (ббайт)(c >> 8); }
inline цел        дайЗнС(бцел c)             { return (ббайт)(c >> 16); }
inline бцел      дайКЗС(ббайт r, ббайт g, ббайт b)    { return r | (g << 8) | (b << 16); }
//#endif

const цел COLOR_V = 39;

class Цвет : public ТипЗнач<Цвет, COLOR_V, Движ<Цвет> > {
protected:
	бцел    color;

	бцел дай() const;

public:
	бцел    дайСырой() const            { return color; }

	цел      дайК() const              { return дайЗнК(дай()); }
	цел      дайЗ() const              { return дайЗнЗ(дай()); }
	цел      дайС() const              { return дайЗнС(дай()); }

	проц     устПусто()                 { color = 0xffffffff; }
	бул     экзПусто_ли() const    { return color == 0xffffffff; }
	т_хэш   дайХэшЗнач() const      { return color; }
	бул     operator==(Цвет c) const { return color == c.color; }
	бул     operator!=(Цвет c) const { return color != c.color; }

	проц     сериализуй(Поток& s)      { s % color; }
	проц     вДжейсон(ДжейсонВВ& jio);
	проц     вРяр(РярВВ& xio);

	Цвет()                            { устПусто(); }
	Цвет(цел r, цел g, цел b)         { color = дайКЗС(r, g, b); }

	Цвет(const Обнул&)               { устПусто(); }

	operator Значение() const             { return SvoToValue(*this); }
	Цвет(const Значение& q)              { color = q.дай<Цвет>().color; }

	operator КЗСА() const;
	Цвет(КЗСА rgba);

	Цвет(Цвет (*фн)())               { color = (*фн)().color; }

	static Цвет изСырого(бцел co)     { Цвет c; c.color = co; return c; }
	static Цвет особый(цел n)        { Цвет c; c.color = 0x80000000 | n; return c; }
	
	цел  дайОсобый() const            { return color & 0x80000000 ? color & 0x7fffffff : -1; }

#ifdef PLATFORM_WIN32
	operator COLORREF() const          { return (COLORREF) дай(); }
	static  Цвет FromCR(COLORREF cr)  { Цвет c; c.color = (бцел)cr; return c; }
#else
	operator бцел() const             { return дай(); }
#endif

private:
	Цвет(цел);
};

КЗСА operator*(цел alpha, Цвет c);

inline Цвет прямойЦвет(КЗСА rgba)  { return Цвет(rgba.r, rgba.g, rgba.b); }

typedef Цвет (*ColorF)();

inline т_хэш   дайХэшЗнач(Цвет c)  { return c.дайХэшЗнач(); }
inline Цвет    Nvl(Цвет a, Цвет b)  { return пусто_ли(a) ? b : a; }

template<>
Ткст какТкст(const Цвет& c);


inline Цвет серыйЦвет(цел a = 128)    { return Цвет(a, a, a); }

inline Цвет чёрный()     { return Цвет(0, 0, 0); }
inline Цвет серый()      { return Цвет(128, 128, 128); }
inline Цвет светлоСерый()    { return Цвет(192, 192, 192); }
inline Цвет белоСерый() { return Цвет(224, 224, 224); }
inline Цвет белый()     { return Цвет(255, 255, 255); }

inline Цвет красный()       { return Цвет(128, 0, 0); }
inline Цвет зелёный()     { return Цвет(0, 128, 0); }
inline Цвет коричневый()     { return Цвет(128, 128, 0); }
inline Цвет синий()      { return Цвет(0, 0, 128); }
inline Цвет магента()   { return Цвет(128, 0, 255); }
inline Цвет цыан()      { return Цвет(0, 128, 128); }
inline Цвет жёлтый()    { return Цвет(255, 255, 0); }
inline Цвет светлоКрасный()     { return Цвет(255, 0, 0); }
inline Цвет светлоЗелёный()   { return Цвет(0, 255, 0); }
inline Цвет светлоЖёлтый()  { return Цвет(255, 255, 192); }
inline Цвет светлоСиний()    { return Цвет(0, 0, 255); }
inline Цвет светлоМагента() { return Цвет(255, 0, 255); }
inline Цвет светлоЦыан()    { return Цвет(0, 255, 255); }

проц   RGBtoHSV(дво r, дво g, дво b, дво& h, дво& s, дво& v);
проц   HSVtoRGB(дво h, дво s, дво v, дво& r, дво& g, дво& b);

Цвет  HsvColorf(дво h, дво s, дво v);

проц   CMYKtoRGB(дво c, дво m, дво y, дво k, дво& r, дво& g, дво& b);
проц   RGBtoCMYK(дво r, дво g, дво b, дво& c, дво& m, дво& y, дво& k);

Цвет  CmykColorf(дво c, дво m, дво y, дво k = 0);

Цвет  смешай(Цвет c1, Цвет c2, цел alpha = 128);

Ткст цветВГЯР(Цвет color);
Цвет  цветИзТекста(кткст0 s);

цел  Grayscale(const Цвет& c);
бул тёмен(Цвет c);
бул светел(Цвет c);

Цвет тёмнаяТема(Цвет c);
Цвет тёмнаяТемаИзКэша(Цвет c);

inline бул operator==(const Значение& v, Цвет x)        { return v == x.operator Значение(); }
inline бул operator==(Цвет x, const Значение& v)        { return v == x.operator Значение(); }
inline бул operator!=(const Значение& v, Цвет x)        { return v != x.operator Значение(); }
inline бул operator!=(Цвет x, const Значение& v)        { return v != x.operator Значение(); }

inline бул operator==(const Значение& v, Цвет (*x)())   { return v == (*x)(); }
inline бул operator==(Цвет (*x)(), const Значение& v)   { return v == (*x)(); }
inline бул operator!=(const Значение& v, Цвет (*x)())   { return v != (*x)(); }
inline бул operator!=(Цвет (*x)(), const Значение& v)   { return v != (*x)(); }

inline бул operator==(Цвет c, Цвет (*x)())          { return c == (*x)(); }
inline бул operator==(Цвет (*x)(), Цвет c)          { return c == (*x)(); }
inline бул operator!=(Цвет c, Цвет (*x)())          { return c != (*x)(); }
inline бул operator!=(Цвет (*x)(), Цвет c)          { return c != (*x)(); }
