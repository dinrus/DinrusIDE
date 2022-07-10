#ifdef PLATFORM_MACOS
struct КЗСА : Движимое<КЗСА> {
        byte a, r, g, b;
};
#else
struct КЗСА : Движимое<КЗСА> {
	byte b, g, r, a;
};
#endif

inline Поток& operator%(Поток& s, КЗСА& c)
{
	return s % c.r % c.g % c.b % c.a;
}

template <> Ткст какТкст(const КЗСА& c);

template<> inline hash_t дайХэшЗнач(const КЗСА& a) { return *(dword *)&a; }

inline bool operator==(const КЗСА& a, const КЗСА& b)
{
	return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

inline bool operator!=(const КЗСА& a, const КЗСА& b)
{
	return !(a == b);
}

inline КЗСА обнулиКЗСА() { КЗСА c; c.r = c.g = c.b = c.a = 0; return c; }

//#ifndef PLATFORM_WIN32
inline int        дайЗнК(dword c)             { return (byte)(c >> 0); }
inline int        дайЗнЗ(dword c)             { return (byte)(c >> 8); }
inline int        дайЗнС(dword c)             { return (byte)(c >> 16); }
inline dword      дайКЗС(byte r, byte g, byte b)    { return r | (g << 8) | (b << 16); }
//#endif

const int COLOR_V = 39;

class Цвет : public ТипЗнач<Цвет, COLOR_V, Движимое<Цвет> > {
protected:
	dword    color;

	dword дай() const;

public:
	dword    дайСырой() const            { return color; }

	int      дайК() const              { return дайЗнК(дай()); }
	int      дайЗ() const              { return дайЗнЗ(дай()); }
	int      дайС() const              { return дайЗнС(дай()); }

	void     устПусто()                 { color = 0xffffffff; }
	bool     экзПусто_ли() const    { return color == 0xffffffff; }
	hash_t   дайХэшЗнач() const      { return color; }
	bool     operator==(Цвет c) const { return color == c.color; }
	bool     operator!=(Цвет c) const { return color != c.color; }

	void     сериализуй(Поток& s)      { s % color; }
	void     вДжейсон(ДжейсонВВ& jio);
	void     вРяр(РярВВ& xio);

	Цвет()                            { устПусто(); }
	Цвет(int r, int g, int b)         { color = дайКЗС(r, g, b); }

	Цвет(const Обнул&)               { устПусто(); }

	operator Значение() const             { return SvoToValue(*this); }
	Цвет(const Значение& q)              { color = q.дай<Цвет>().color; }

	operator КЗСА() const;
	Цвет(КЗСА rgba);

	Цвет(Цвет (*фн)())               { color = (*фн)().color; }

	static Цвет изСырого(dword co)     { Цвет c; c.color = co; return c; }
	static Цвет особый(int n)        { Цвет c; c.color = 0x80000000 | n; return c; }
	
	int  дайОсобый() const            { return color & 0x80000000 ? color & 0x7fffffff : -1; }

#ifdef PLATFORM_WIN32
	operator COLORREF() const          { return (COLORREF) дай(); }
	static  Цвет FromCR(COLORREF cr)  { Цвет c; c.color = (dword)cr; return c; }
#else
	operator dword() const             { return дай(); }
#endif

private:
	Цвет(int);
};

КЗСА operator*(int alpha, Цвет c);

inline Цвет прямойЦвет(КЗСА rgba)  { return Цвет(rgba.r, rgba.g, rgba.b); }

typedef Цвет (*ColorF)();

inline hash_t   дайХэшЗнач(Цвет c)  { return c.дайХэшЗнач(); }
inline Цвет    Nvl(Цвет a, Цвет b)  { return пусто_ли(a) ? b : a; }

template<>
Ткст какТкст(const Цвет& c);


inline Цвет серыйЦвет(int a = 128)    { return Цвет(a, a, a); }

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

void   RGBtoHSV(double r, double g, double b, double& h, double& s, double& v);
void   HSVtoRGB(double h, double s, double v, double& r, double& g, double& b);

Цвет  HsvColorf(double h, double s, double v);

void   CMYKtoRGB(double c, double m, double y, double k, double& r, double& g, double& b);
void   RGBtoCMYK(double r, double g, double b, double& c, double& m, double& y, double& k);

Цвет  CmykColorf(double c, double m, double y, double k = 0);

Цвет  смешай(Цвет c1, Цвет c2, int alpha = 128);

Ткст цветВГЯР(Цвет color);
Цвет  цветИзТекста(const char *s);

int  Grayscale(const Цвет& c);
bool тёмен(Цвет c);
bool светел(Цвет c);

Цвет тёмнаяТема(Цвет c);
Цвет тёмнаяТемаИзКэша(Цвет c);

inline bool operator==(const Значение& v, Цвет x)        { return v == x.operator Значение(); }
inline bool operator==(Цвет x, const Значение& v)        { return v == x.operator Значение(); }
inline bool operator!=(const Значение& v, Цвет x)        { return v != x.operator Значение(); }
inline bool operator!=(Цвет x, const Значение& v)        { return v != x.operator Значение(); }

inline bool operator==(const Значение& v, Цвет (*x)())   { return v == (*x)(); }
inline bool operator==(Цвет (*x)(), const Значение& v)   { return v == (*x)(); }
inline bool operator!=(const Значение& v, Цвет (*x)())   { return v != (*x)(); }
inline bool operator!=(Цвет (*x)(), const Значение& v)   { return v != (*x)(); }

inline bool operator==(Цвет c, Цвет (*x)())          { return c == (*x)(); }
inline bool operator==(Цвет (*x)(), Цвет c)          { return c == (*x)(); }
inline bool operator!=(Цвет c, Цвет (*x)())          { return c != (*x)(); }
inline bool operator!=(Цвет (*x)(), Цвет c)          { return c != (*x)(); }
