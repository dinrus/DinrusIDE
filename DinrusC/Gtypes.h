template <class T> struct Точка_;
template <class T> struct Размер_;
template <class T> struct Прямоугольник_;

template <class T>
struct Размер_ : Движимое< Размер_<T> > {
	T             cx, cy;

	void          очисть()                      { cx = cy = 0; }
	bool          пустой() const              { return cx == 0 || cy == 0; }

	void          устПусто()                    { cx = cy = Null; }
	bool          экзПусто_ли() const       { return РНЦП::пусто_ли(cx); }

	Размер_&        operator+=(Размер_ p)          { cx  += p.cx; cy  += p.cy; return *this; }
	Размер_&        operator+=(T t)              { cx  += t;    cy  += t;    return *this; }
	Размер_&        operator-=(Размер_ p)          { cx  -= p.cx; cy  -= p.cy; return *this; }
	Размер_&        operator-=(T t)              { cx  -= t;    cy  -= t;    return *this; }
	Размер_&        operator*=(Размер_ p)          { cx  *= p.cx; cy  *= p.cy; return *this; }
	Размер_&        operator*=(T t)              { cx  *= t;    cy  *= t;    return *this; }
	Размер_&        operator/=(Размер_ p)          { cx  /= p.cx; cy  /= p.cy; return *this; }
	Размер_&        operator/=(T t)              { cx  /= t;    cy  /= t;    return *this; }
	Размер_&        operator<<=(int sh)          { cx <<= sh;   cy <<= sh;   return *this; }
	Размер_&        operator>>=(int sh)          { cx >>= sh;   cy >>= sh;   return *this; }

	Размер_&        operator++()                 { ++cx; ++cy; return *this; }
	Размер_&        operator--()                 { --cx; --cy; return *this; }

	friend Размер_  operator+(Размер_ s)           { return s; }
	friend Размер_  operator-(Размер_ s)           { return Размер_(-s.cx, -s.cy); }

	friend Размер_  operator+(Размер_ a, Размер_ b)  { return a += b; }
	friend Размер_  operator+(Размер_ a, T t)      { return a += t; }
	friend Размер_  operator+(T t, Размер_ b)      { return b += t; }
	friend Размер_  operator-(Размер_ a, Размер_ b)  { return a -= b; }
	friend Размер_  operator-(Размер_ a, T t)      { return a -= t; }
	friend Размер_  operator-(T t, Размер_ b)      { b = -b; return b += t; }
	friend Размер_  operator*(Размер_ a, Размер_ b)  { return a *= b; }
	friend Размер_  operator*(Размер_ a, T b)      { return a *= b; }
	friend Размер_  operator*(T a, Размер_ b)      { return b *= a; }
	friend Размер_  operator/(Размер_ a, Размер_ b)  { return a /= b; }
	friend Размер_  operator/(Размер_ a, T b)      { return a /= b; }
	friend Размер_  operator<<(Размер_ a, int sh)  { return a <<= sh; }
	friend Размер_  operator>>(Размер_ a, int sh)  { return a >>= sh; }

	friend bool   operator==(Размер_ a, Размер_ b) { return a.cx == b.cx && a.cy == b.cy; }
	friend bool   operator!=(Размер_ a, Размер_ b) { return !(a == b); }

	friend Размер_  min(Размер_ a, Размер_ b)        { return Размер_(min(a.cx, b.cx), min(a.cy, b.cy)); }
	friend Размер_  max(Размер_ a, Размер_ b)        { return Размер_(max(a.cx, b.cx), max(a.cy, b.cy)); }

	friend Размер_  Nvl(Размер_ a, Размер_ b)        { return пусто_ли(a) ? b : a; }

	friend T      ScalarProduct(Размер_ a, Размер_ b) { return a.cx * b.cx + a.cy * b.cy; }
	friend T      VectorProduct(Размер_ a, Размер_ b) { return a.cx * b.cy - a.cy * b.cx; }
	friend T      вКвадрате(Размер_ a)             { return a.cx * a.cx + a.cy * a.cy; }
	friend double длина(Размер_ a)              { return hypot(a.cx, a.cy); }

	hash_t        дайХэшЗнач() const         { return комбинируйХэш(cx, cy); }

	Ткст        вТкст() const;

	Размер_() {}
	Размер_(T cx, T cy) : cx(cx), cy(cy) {}

	Размер_(const Размер_<int>& sz)  : cx((T)sz.cx), cy((T)sz.cy) {}	
	Размер_(const Размер_<short>& sz)  : cx((T)sz.cx), cy((T)sz.cy) {}
	Размер_(const Размер_<double>& sz)  : cx((T)sz.cx), cy((T)sz.cy) {}
	Размер_(const Размер_<int64>& sz) : cx((T)sz.cx), cy((T)sz.cy) {}

	Размер_(const Точка_<T>& pt) : cx(pt.x), cy(pt.y) {}

	Размер_(const Обнул&)                       { cx = cy = Null; }

	operator Значение() const                     { return FitsSvoValue<Размер_>() ? SvoToValue(*this) : богатыйВЗнач(*this); }
	Размер_(const Значение& ист)                    { *this = ист.дай<Размер_>(); }

	operator Реф()                             { return какРеф(*this); }

	void сериализуй(Поток& s)                  { s % cx % cy; }
	void вДжейсон(ДжейсонВВ& jio)                  { jio("cx", cx)("cy", cy); }
	void вРяр(РярВВ& xio)                    { xio.Атр("cx", cx).Атр("cy", cy); }
	
	int  сравни(const Размер_&) const           { NEVER(); return 0; }
	int  полиСравни(const Значение&) const       { NEVER(); return 0; }

#ifdef PLATFORM_WIN32
	operator SIZE*()                           { ПРОВЕРЬ(sizeof(*this) == sizeof(SIZE)); return (SIZE*)this; }
	operator const SIZE*() const               { ПРОВЕРЬ(sizeof(*this) == sizeof(SIZE)); return (SIZE*)this; }
	operator SIZE() const                      { SIZE sz; sz.cx = cx; sz.cy = cy; return sz; }
	LONG     GetLONG() const                   { return MAKELONG(cx, cy); }

	Размер_(POINT pt) : cx((T)pt.x), cy((T)pt.y)  {}
	Размер_(SIZE sz) : cx((T)sz.cx), cy((T)sz.cy) {}
	explicit Размер_(LONG lParam) { cx = (T)(int16)LOWORD(lParam); cy = (T)(int16)HIWORD(lParam); }
#endif
};

template <class T>
Ткст Размер_<T>::вТкст() const {
	Ткст ss;
	return ss << '(' << cx << ", " << cy << ')';
}

template <class T>
struct Точка_ : Движимое< Точка_<T> > {
	T             x, y;

	typedef       Размер_<T> Sz;

	void          очисть()                           { x = y = 0; }
	bool          нуль_ли() const                    { return x == 0 && y == 0; }

	void          устПусто()                         { x = y = Null; }
	bool          экзПусто_ли() const            { return РНЦП::пусто_ли(x); }

	void          смещение(T dx, T dy)                { x += dx; y += dy; }

	Точка_&       operator+=(Sz p)                  { x  += p.cx; y  += p.cy; return *this; }
	Точка_&       operator+=(Точка_ p)              { x  += p.x;  y  += p.y; return *this; }
	Точка_&       operator+=(T t)                   { x  += t;    y  += t;    return *this; }
	Точка_&       operator-=(Sz p)                  { x  -= p.cx; y  -= p.cy; return *this; }
	Точка_&       operator-=(Точка_ p)              { x  -= p.x;  y  -= p.y; return *this; }
	Точка_&       operator-=(T t)                   { x  -= t;    y  -= t;    return *this; }
	Точка_&       operator*=(Sz p)                  { x  *= p.cx; y  *= p.cy; return *this; }
	Точка_&       operator*=(Точка_ p)              { x  *= p.x;  y  *= p.y; return *this; }
	Точка_&       operator*=(T t)                   { x  *= t;    y  *= t;    return *this; }
	Точка_&       operator/=(Sz p)                  { x  /= p.cx; y  /= p.cy; return *this; }
	Точка_&       operator/=(Точка_ p)              { x  /= p.x;  y  /= p.y; return *this; }
	Точка_&       operator/=(T t)                   { x  /= t;    y  /= t;    return *this; }
	Точка_&       operator<<=(int sh)               { x <<= sh;   y <<= sh;   return *this; }
	Точка_&       operator>>=(int sh)               { x >>= sh;   y >>= sh;   return *this; }

	Точка_&       operator++()                      { ++x; ++y; return *this; }
	Точка_&       operator--()                      { --x; --y; return *this; }

	friend Точка_ operator+(Точка_ p)               { return p; }
	friend Точка_ operator-(Точка_ p)               { return Точка_(-p.x, -p.y); }

	friend Точка_ operator+(Точка_ a, Sz b)         { return a += b; }
	friend Точка_ operator+(Точка_ a, Точка_ b)     { return a += b; }
	friend Точка_ operator+(Точка_ a, T t)          { return a += t; }
	friend Точка_ operator+(T t, Точка_ b)          { return b += t; }
	friend Sz     operator+(Sz a, Точка_ b)         { return Точка_(a) + b; }
	friend Sz&    operator+=(Sz& a, Точка_ b)       { return a = a + b; }
	friend Точка_ operator-(Точка_ a, Sz b)         { return a -= b; }
	friend Точка_ operator-(Точка_ a, T t)          { return a -= t; }
	friend Sz     operator-(Точка_ a, Точка_ b)     { return a -= b; }
	friend Sz     operator-(Sz a, Точка_ b)         { return Точка_(a) - b; }
	friend Sz&    operator-=(Sz& a, Точка_ b)       { return a = a - b; }
	friend Точка_ operator*(Точка_ a, Точка_ b)     { return a *= b; }
	friend Точка_ operator*(Sz a, Точка_ b)         { return a *= (Sz)b; }
	friend Точка_ operator*(Точка_ a, Sz b)         { return a *= (Точка_)b; }
	friend Точка_ operator*(Точка_ a, T b)          { return a *= b; }
	friend Точка_ operator*(T a, Точка_ b)          { return b *= a; }
	friend Точка_ operator/(Точка_ a, Sz b)         { return a /= b; }
	friend Точка_ operator/(Точка_ a, T b)          { return a /= b; }
	friend Точка_ operator<<(Точка_ a, int sh)      { return a <<= sh; }
	friend Точка_ operator>>(Точка_ a, int sh)      { return a >>= sh; }


	friend bool   operator==(Точка_ a, Точка_ b)    { return a.x == b.x && a.y == b.y; }
	friend bool   operator!=(Точка_ a, Точка_ b)    { return a.x != b.x || a.y != b.y; }

	friend Точка_ min(Точка_ a, Точка_ b)           { return Точка_(min(a.x, b.x), min(a.y, b.y)); }
	friend Точка_ max(Точка_ a, Точка_ b)           { return Точка_(max(a.x, b.x), max(a.y, b.y)); }

	friend Точка_ Nvl(Точка_ a, Точка_ b)           { return пусто_ли(a) ? b : a; }

	hash_t        дайХэшЗнач() const              { return комбинируйХэш(x, y); }

	Ткст        вТкст() const;

	Точка_() {}
	Точка_(T x, T y) : x(x), y(y) {}

	Точка_(const Точка_<int>& pt) : x((T)pt.x), y((T)pt.y) {}
	Точка_(const Точка_<short>& pt) : x((T)pt.x), y((T)pt.y) {}
	Точка_(const Точка_<double>& pt) : x((T)pt.x), y((T)pt.y) {}
	Точка_(const Точка_<int64>& pt) : x((T)pt.x), y((T)pt.y) {}

	Точка_(const Размер_<T>& sz)  : x(sz.cx), y(sz.cy) {}

	Точка_(const Обнул&)                           { x = y = Null; }

	operator Значение() const                          { return FitsSvoValue<Точка_>() ? SvoToValue(*this) : богатыйВЗнач(*this); }
	Точка_(const Значение& ист)                        { *this = ист.дай<Точка_>(); }

	operator Реф()                                  { return какРеф(*this); }

	void сериализуй(Поток& s)                       { s % x % y; }
	void вДжейсон(ДжейсонВВ& jio)                       { jio("x", x)("y", y); }
	void вРяр(РярВВ& xio)                         { xio.Атр("x", x).Атр("y", y); }

	int  сравни(const Точка_&) const               { NEVER(); return 0; }
	int  полиСравни(const Значение&) const            { NEVER(); return 0; }

#ifdef PLATFORM_WIN32
	operator POINT*()                               { ПРОВЕРЬ(sizeof(*this) == sizeof(POINT)); return (POINT*)this; }
	operator const POINT*() const                   { ПРОВЕРЬ(sizeof(*this) == sizeof(POINT)); return (POINT*)this; }
	operator POINT() const                          { POINT p; p.x = x; p.y = y; return p; }
	LONG     GetLONG() const                        { return MAKELONG(x, y); }

	Точка_(POINT pt) : x((T)pt.x), y((T)pt.y)       {}
	Точка_(SIZE sz) : x((T)sz.cx), y((T)sz.cy)      {}
	explicit Точка_(LONG lParam)                    { x = (T)(int16)LOWORD(lParam); y = (T)(int16)HIWORD(lParam); }
#endif
};

template <class T>
Ткст Точка_<T>::вТкст() const {
	Ткст ss;
	return ss << '[' << x << ", " << y << ']';
}

template <class T>
T GHalf_(T t) { return t >> 1; }

template <>
inline double GHalf_(double d) { return d / 2; }

template <class T>
struct Прямоугольник_ : Движимое< Прямоугольник_<T> > {
	T      left, top, right, bottom;

	typedef Точка_<T>  Pt;
	typedef Размер_<T>   Sz;

	void   очисть()                          { left = top = right = bottom = 0; }

	bool   пустой() const                  { return right <= left || bottom <= top; }
	void   устПусто()                        { left = right = top = bottom = Null; }
	bool   экзПусто_ли() const           { return пусто_ли(left); }

	Pt     верхЛево() const                  { return Pt(left, top); }
	Pt     верхЦентр() const                { return Pt(GHalf_(left + right), top); }
	Pt     верхПраво() const                 { return Pt(right, top); }
	Pt     центрЛево() const               { return Pt(left, GHalf_(top + bottom)); }
	Pt     центрТочка() const              { return Pt(GHalf_(left + right), GHalf_(top + bottom)); }
	Pt     центрПраво() const              { return Pt(right, GHalf_(top + bottom)); }
	Pt     низЛево() const               { return Pt(left, bottom); }
	Pt     низЦентр() const             { return Pt(GHalf_(left + right), bottom); }
	Pt     низПраво() const              { return Pt(right, bottom); }
	T      устШирину() const                    { return right - left; }
	T      устВысоту() const                   { return bottom - top; }
	Sz     размер() const                     { return Sz(right - left, bottom - top); }

	T      дайШирину() const                 { return right - left; }
	T      дайВысоту() const                { return bottom - top; }
	Sz     дайРазм() const                  { return Sz(right - left, bottom - top); }

	Pt     позЦентра(T cx, T cy) const;
	Pt     позЦентра(Sz sz) const           { return позЦентра(sz.cx, sz.cy); }
	Прямоугольник_  центрПрям(Sz sz) const          { return Прямоугольник_(позЦентра(sz), sz); }
	Прямоугольник_  центрПрям(T cx, T cy) const     { return центрПрям(Sz(cx, cy)); }

	void   уст(T l, T t, T r, T b)          { left = l; top = t; right = r; bottom = b; }
	void   уст(Pt a, Pt b)                  { left = a.x; top = a.y; right = b.x; bottom = b.y; }
	void   уст(Pt a, Sz sz)                 { уст(a, a + sz); }
	void   уст(const Прямоугольник_& r)              { уст(r.left, r.top, r.right, r.bottom); }

	void   устРазм(T cx, T cy)                  { right = left + cx; bottom = top + cy; }
	void   устРазм(Sz sz)                       { устРазм(sz.cx, sz.cy); }

	void   инфлируйГориз(T dx)                    { left -= dx; right += dx; }
	void   инфлируйВерт(T dy)                    { top -= dy; bottom += dy; }
	void   инфлируй(T dx, T dy)                  { инфлируйГориз(dx); инфлируйВерт(dy); }
	void   инфлируй(Sz sz)                       { инфлируй(sz.cx, sz.cy); }
	void   инфлируй(T dxy)                       { инфлируй(dxy, dxy); }
	void   инфлируй(T l, T t, T r, T b)          { left -= l; top -= t; right += r; bottom += b; }
	void   инфлируй(const Прямоугольник_& r)              { инфлируй(r.left, r.top, r.right, r.bottom); }

	void   дефлируйГориз(T dx)                    { инфлируйГориз(-dx); }
	void   дефлируйВерт(T dy)                    { инфлируйВерт(-dy); }
	void   дефлируй(T dx, T dy)                  { инфлируй(-dx, -dy); }
	void   дефлируй(Sz sz)                       { инфлируй(-sz); }
	void   дефлируй(T dxy)                       { инфлируй(-dxy); }
	void   дефлируй(T l, T t, T r, T b)          { инфлируй(-l, -t, -r, -b); }
	void   дефлируй(const Прямоугольник_& r)              { дефлируй(r.left, r.top, r.right, r.bottom); }

	Прямоугольник_  горизИнфлят(T dx) const             { Прямоугольник_ r = *this; r.инфлируйГориз(dx); return r; }
	Прямоугольник_  вертИнфлят(T dy) const             { Прямоугольник_ r = *this; r.инфлируйВерт(dy); return r; }
	Прямоугольник_  инфлят(T dx, T dy) const           { Прямоугольник_ r = *this; r.инфлируй(dx, dy); return r; }
	Прямоугольник_  инфлят(Sz sz) const                { Прямоугольник_ r = *this; r.инфлируй(sz); return r; }
	Прямоугольник_  инфлят(T dxy) const                { Прямоугольник_ r = *this; r.инфлируй(dxy); return r; }
	Прямоугольник_  инфлят(T l, T t, T r, T b) const   { Прямоугольник_ m = *this; m.инфлируй(l, t, r, b); return m; }
	Прямоугольник_  инфлят(const Прямоугольник_& q) const       { Прямоугольник_ r = *this; r.инфлируй(q); return r; }

	Прямоугольник_  горизДефлят(T dx) const             { Прямоугольник_ r = *this; r.дефлируйГориз(dx); return r; }
	Прямоугольник_  вертДефлят(T dy) const             { Прямоугольник_ r = *this; r.дефлируйВерт(dy); return r; }
	Прямоугольник_  дефлят(T dx, T dy) const           { Прямоугольник_ r = *this; r.дефлируй(dx, dy); return r; }
	Прямоугольник_  дефлят(Sz sz) const                { Прямоугольник_ r = *this; r.дефлируй(sz); return r; }
	Прямоугольник_  дефлят(T dxy) const                { Прямоугольник_ r = *this; r.дефлируй(dxy); return r; }
	Прямоугольник_  дефлят(T l, T t, T r, T b) const   { Прямоугольник_ m = *this; m.дефлируй(l, t, r, b); return m; }
	Прямоугольник_  дефлят(const Прямоугольник_& q) const       { Прямоугольник_ r = *this; r.дефлируй(q); return r; }

	void   смещениеГориз(T dx)                     { left += dx; right += dx; }
	void   смещениеВерт(T dy)                     { top += dy; bottom += dy; }
	void   смещение(T dx, T dy)                   { смещениеГориз(dx); смещениеВерт(dy); }
	void   смещение(Sz sz)                        { смещение(sz.cx, sz.cy); }
	void   смещение(Pt p)                         { смещение(p.x, p.y); }

	Прямоугольник_  горизСмещенец(T dx) const             { Прямоугольник_ r = *this; r.смещениеГориз(dx); return r; }
	Прямоугольник_  вертСмещенец(T dy) const             { Прямоугольник_ r = *this; r.смещениеВерт(dy); return r; }
	Прямоугольник_  смещенец(T dx, T dy) const           { Прямоугольник_ r = *this; r.смещение(dx, dy); return r; }
	Прямоугольник_  смещенец(Sz sz) const                { Прямоугольник_ r = *this; r.смещение(sz); return r; }
	Прямоугольник_  смещенец(Pt p) const                 { Прямоугольник_ r = *this; r.смещение(p); return r; }

	void   нормализуй();
	Прямоугольник_  нормализат() const                   { Прямоугольник_ r = *this; r.нормализуй(); return r; }

	void   союз(Pt p);
	void   союз(const Прямоугольник_& rc);
	void   пересек(const Прямоугольник_& rc);

	bool   содержит(T x, T y) const;
	bool   содержит(Pt p) const                 { return содержит(p.x, p.y); }
	bool   содержит(const Прямоугольник_& rc) const;
	bool   пересекается(const Прямоугольник_& rc) const;

	Pt     свяжи(Pt pt) const;

	Прямоугольник_& operator+=(Sz sz)                                { смещение(sz); return *this; }
	Прямоугольник_& operator+=(Pt p)                                 { смещение(p); return *this; }
	Прямоугольник_& operator+=(const Прямоугольник_& b);
	Прямоугольник_& operator-=(Sz sz)                                { смещение(-sz); return *this; }
	Прямоугольник_& operator-=(Pt p)                                 { смещение(-p); return *this; }
	Прямоугольник_& operator-=(const Прямоугольник_& b);
	Прямоугольник_& operator*=(T t)                                  { left *= t; right *= t; top *= t; bottom *= t; return *this; }
	Прямоугольник_& operator/=(T t)                                  { left /= t; right /= t; top /= t; bottom /= t; return *this; }

	Прямоугольник_& operator|=(Pt p)                                 { союз(p); return *this; }
	Прямоугольник_& operator|=(const Прямоугольник_& rc)                      { союз(rc); return *this; }
	Прямоугольник_& operator&=(const Прямоугольник_& rc)                      { пересек(rc); return *this; }

	bool   operator==(const Прямоугольник_& b) const;
	bool   operator!=(const Прямоугольник_& b) const                 { return !operator==(b); }

	friend Прямоугольник_ operator+(Прямоугольник_ a, Sz b)                   { return a += b; }
	friend Прямоугольник_ operator+(Sz a, Прямоугольник_ b)                   { return b += a; }
	friend Прямоугольник_ operator+(Прямоугольник_ a, Pt b)                   { return a += b; }
	friend Прямоугольник_ operator+(Pt a, Прямоугольник_ b)                   { return b += a; }
	friend Прямоугольник_ operator+(Прямоугольник_ a, const Прямоугольник_& b)         { return a += b; }
	friend Прямоугольник_ operator-(Прямоугольник_ a, Sz b)                   { return a -= b; }
	friend Прямоугольник_ operator-(Прямоугольник_ a, Pt b)                   { return a -= b; }
	friend Прямоугольник_ operator-(Прямоугольник_ a, const Прямоугольник_& b)         { return a -= b; }
	friend Прямоугольник_ operator*(Прямоугольник_ a, T t)                    { return a *= t; }
	friend Прямоугольник_ operator*(T t, Прямоугольник_ a)                    { return a *= t; }
	friend Прямоугольник_ operator/(Прямоугольник_ a, T t)                    { return a /= t; }
	friend Прямоугольник_ operator|(Прямоугольник_ a, Прямоугольник_ b)                { a.союз(b); return a; }
	friend Прямоугольник_ operator&(Прямоугольник_ a, Прямоугольник_ b)                { a.пересек(b); return a; }
	friend bool  operator&&(const Прямоугольник_& a, const Прямоугольник_& b) { return a.пересекается(b); }
	friend bool  operator>=(const Прямоугольник_& a, Pt b)           { return a.содержит(b); }
	friend bool  operator<=(Pt a, const Прямоугольник_& b)           { return b.содержит(a); }
	friend bool  operator<=(const Прямоугольник_& a, const Прямоугольник_& b) { return b.содержит(a); }
	friend bool  operator>=(const Прямоугольник_& b, const Прямоугольник_& a) { return a.содержит(b); }

	friend const Прямоугольник_& Nvl(const Прямоугольник_& a, const Прямоугольник_& b) { return пусто_ли(a) ? b : a; }

	hash_t дайХэшЗнач() const                             { return комбинируйХэш(left, top, right, bottom); }

	Ткст вТкст() const;

	Прямоугольник_()                                     {}
	Прямоугольник_(T l, T t, T r, T b)                   { уст(l, t, r, b); }
	Прямоугольник_(Pt a, Pt b)                           { уст(a, b); }
	Прямоугольник_(Pt a, Sz sz)                          { уст(a, sz); }
	Прямоугольник_(Sz sz)                                { уст(0, 0, sz.cx, sz.cy); }

	Прямоугольник_(const Прямоугольник_<int>& r) { уст((T)r.left, (T)r.top, (T)r.right, (T)r.bottom); }
	Прямоугольник_(const Прямоугольник_<short>& r) { уст((T)r.left, (T)r.top, (T)r.right, (T)r.bottom); }
	Прямоугольник_(const Прямоугольник_<double>& r) { уст((T)r.left, (T)r.top, (T)r.right, (T)r.bottom); }
	Прямоугольник_(const Прямоугольник_<int64>& r) { уст((T)r.left, (T)r.top, (T)r.right, (T)r.bottom); }

	Прямоугольник_(const Обнул&)             { устПусто(); }

	operator Значение() const           { return богатыйВЗнач(*this); }
	Прямоугольник_(const Значение& ист)          { *this = ист.дай<Прямоугольник_>(); }

	operator Реф()                   { return какРеф(*this); }

	void     сериализуй(Поток& s) { s % left % top % right % bottom; }
	void     вДжейсон(ДжейсонВВ& jio) { jio("left", left)("top", top)("right", right)("bottom", bottom); }
	void     вРяр(РярВВ& xio)   { xio.Атр("left", left).Атр("top", top).Атр("right", right).Атр("bottom", bottom); }

	int      сравни(const Прямоугольник_&) const           { NEVER(); return 0; }
	int      полиСравни(const Значение&) const       { NEVER(); return 0; }

#ifdef PLATFORM_WIN32
	operator const RECT*() const { ПРОВЕРЬ(sizeof(*this) == sizeof(RECT)); return (RECT*)this; }
	operator RECT*()             { ПРОВЕРЬ(sizeof(*this) == sizeof(RECT)); return (RECT*)this; }
	operator RECT&()             { ПРОВЕРЬ(sizeof(*this) == sizeof(RECT)); return *(RECT*)this; }
	operator RECT() const        { RECT r; r.top = top; r.bottom = bottom;
									       r.left = left; r.right = right; return r; }
	Прямоугольник_(const RECT& rc)        { уст((T)rc.left, (T)rc.top, (T)rc.right, (T)rc.bottom); }
#endif
};

template <class T>
Точка_<T> Прямоугольник_<T>::позЦентра(T cx, T cy) const {
	return Точка_<T>(left + GHalf_(устШирину() - cx), top + GHalf_(устВысоту() - cy));
}

template <class T>
void Прямоугольник_<T>::нормализуй() {
	if(left > right) разверни(left, right);
	if(top > bottom) разверни(top, bottom);
}

template <class T>
void Прямоугольник_<T>::союз(Pt p) {
	if(пусто_ли(p)) return;
	if(экзПусто_ли()) {
		right = 1 + (left = p.x);
		bottom = 1 + (top = p.y);
	}
	else
	{
		if(p.x >= right) right = p.x + 1;
		else if(p.x <  left) left = p.x;
		if(p.y >= bottom) bottom = p.y + 1;
		else if(p.y < top) top = p.y;
	}
}

template <>
inline void Прямоугольник_<double>::союз(Точка_<double> p) {
	if(пусто_ли(p)) return;
	if(экзПусто_ли())
	{
		left = right = p.x;
		top = bottom = p.y;
	}
	else
	{
		if(p.x < left) left = p.x;
		else if(p.x > right) right = p.x;
		if(p.y < top) top = p.y;
		else if(p.y > bottom) bottom = p.y;
	}
}

template <class T>
void Прямоугольник_<T>::союз(const Прямоугольник_<T>& r) {
	if(пусто_ли(r)) return;
	if(экзПусто_ли()) {
		уст(r);
		return;
	}
	if(r.left < left) left = r.left;
	if(r.top < top) top = r.top;
	if(r.right > right) right = r.right;
	if(r.bottom > bottom) bottom = r.bottom;
}

void прям_дво_союз(Прямоугольник_<double>& self, const Прямоугольник_<double>& r);

template <>
inline void Прямоугольник_<double>::союз(const Прямоугольник_<double>& r) {
	прям_дво_союз(*this, r);
}

template <class T>
void Прямоугольник_<T>::пересек(const Прямоугольник_<T>& r) {
	if(r.left > left) left = r.left;
	if(r.top > top) top = r.top;
	if(right < left) right = left;
	if(r.right < right) right = r.right;
	if(r.bottom < bottom) bottom = r.bottom;
	if(bottom < top) bottom = top;
}

template <class T>
bool Прямоугольник_<T>::содержит(T x, T y) const {
	return x >= left && x < right && y >= top && y < bottom;
}

template <>
inline bool Прямоугольник_<double>::содержит(double x, double y) const {
	return x >= left && x <= right && y >= top && y <= bottom;
}

template <class T>
bool Прямоугольник_<T>::содержит(const Прямоугольник_<T>& r) const {
	return r.left >= left && r.top >= top && r.right <= right && r.bottom <= bottom;
}

template <class T>
bool Прямоугольник_<T>::пересекается(const Прямоугольник_<T>& r) const {
	if(пустой() || r.пустой()) return false;
	return r.right > left && r.bottom > top && r.left < right && r.top < bottom;
}

bool прям_дво_пересекается(const Прямоугольник_<double>& self, const Прямоугольник_<double>& r);

template <>
inline bool Прямоугольник_<double>::пересекается(const Прямоугольник_<double>& r) const {
	return прям_дво_пересекается(*this, r);
}

template <class T>
Точка_<T> Прямоугольник_<T>::свяжи(Точка_<T> pt) const {
	return Точка_<T>(pt.x < left ? left : pt.x >= right ? right - 1 : pt.x,
		             pt.y < top ? top : pt.y >= bottom ? bottom - 1 : pt.y);
}

Точка_<double> прям_дво_свяжи(const Прямоугольник_<double>& self, Точка_<double> pt);

template <>
inline Точка_<double> Прямоугольник_<double>::свяжи(Точка_<double> pt) const {
	return прям_дво_свяжи(*this, pt);
}

template <class T>
Прямоугольник_<T>& Прямоугольник_<T>::operator-=(const Прямоугольник_<T>& b) {
	left -= b.left;
	top -= b.top;
	right -= b.right;
	bottom -= b.bottom;
	return *this;
}

template <class T>
Прямоугольник_<T>& Прямоугольник_<T>::operator+=(const Прямоугольник_<T>& b) {
	left += b.left;
	top += b.top;
	right += b.right;
	bottom += b.bottom;
	return *this;
}

template <class T>
bool Прямоугольник_<T>::operator==(const Прямоугольник_& b) const {
	return top == b.top && bottom == b.bottom && left == b.left && right == b.right;
}

template <class T>
Ткст Прямоугольник_<T>::вТкст() const
{
	Ткст str;
	return str << какТкст(верхЛево()) << " - " << какТкст(низПраво())
	           << " : " << какТкст(размер());
}

typedef Размер_<int>     Размер;
typedef Точка_<int>    Точка;
typedef Прямоугольник_<int>     Прям;

typedef Размер_<int16>   Размер16;
typedef Точка_<int16>  Точка16;
typedef Прямоугольник_<int16>   Прям16;

typedef Размер_<double>  РазмерПЗ;
typedef Точка_<double> ТочкаПЗ;
typedef Прямоугольник_<double>  ПрямПЗ;

typedef Размер_<int64>   Размер64;
typedef Точка_<int64>  Точка64;
typedef Прямоугольник_<int64>   Прям64;

const int SIZE_V   = 70;
const int SIZE16_V = 71;
const int SIZEF_V  = 72;
const int SIZE64_V = 79;

template<> inline dword номерТипаЗнач(const Размер*)   { return SIZE_V; }
template<> inline dword номерТипаЗнач(const Размер16*) { return SIZE16_V; }
template<> inline dword номерТипаЗнач(const Размер64*) { return SIZE64_V; }
template<> inline dword номерТипаЗнач(const РазмерПЗ*)  { return SIZEF_V; }

const int POINT_V   = 73;
const int POINT16_V = 74;
const int POINTF_V  = 75;
const int POINT64_V = 80;

template<> inline dword номерТипаЗнач(const Точка*)   { return POINT_V; }
template<> inline dword номерТипаЗнач(const Точка16*) { return POINT16_V; }
template<> inline dword номерТипаЗнач(const Точка64*) { return POINT64_V; }
template<> inline dword номерТипаЗнач(const ТочкаПЗ*)  { return POINTF_V; }

const int RECT_V   = 76;
const int RECT16_V = 77;
const int RECTF_V  = 78;
const int RECT64_V = 81;

template<> inline dword номерТипаЗнач(const Прям*)   { return RECT_V; }
template<> inline dword номерТипаЗнач(const Прям16*) { return RECT16_V; }
template<> inline dword номерТипаЗнач(const Прям64*) { return RECT64_V; }
template<> inline dword номерТипаЗнач(const ПрямПЗ*)  { return RECTF_V; }

Прям    RectC(int x, int y, int cx, int cy);
Прям16  Rect16C(int16 x, int16 y, int16 cx, int16 cy);
ПрямПЗ   RectfC(double x, double y, double cx, double cy);

inline Размер& operator*=(Размер& sz, double a)
{
	sz.cx = int(sz.cx * a);
	sz.cy = int(sz.cy * a);
	return sz;
}

inline Размер& operator/=(Размер& sz, double a)
{
	sz.cx = int(sz.cx / a);
	sz.cy = int(sz.cy / a);
	return sz;
}

inline Размер& operator*=(Размер& sz, РазмерПЗ a)
{
	sz.cx = int(sz.cx * a.cx);
	sz.cy = int(sz.cy * a.cy);
	return sz;
}

inline Размер& operator/=(Размер& sz, РазмерПЗ a)
{
	sz.cx = int(sz.cx / a.cx);
	sz.cy = int(sz.cy / a.cy);
	return sz;
}

inline РазмерПЗ operator*(Размер sz, double a)   { return РазмерПЗ(sz.cx * a, sz.cy * a); }
inline РазмерПЗ operator*(double a, Размер sz)   { return РазмерПЗ(sz.cx * a, sz.cy * a); }
inline РазмерПЗ operator/(Размер sz, double a)   { return РазмерПЗ(sz.cx / a, sz.cy / a); }
inline РазмерПЗ operator*(Размер sz, РазмерПЗ a)    { return РазмерПЗ(sz.cx * a.cx, sz.cy * a.cy); }
inline РазмерПЗ operator*(РазмерПЗ a, Размер sz)    { return РазмерПЗ(sz.cx * a.cx, sz.cy * a.cy); }
inline РазмерПЗ operator/(Размер sz, РазмерПЗ a)    { return РазмерПЗ(sz.cx / a.cx, sz.cy / a.cy); }

inline Размер16& operator*=(Размер16& sz, double a)
{
	sz.cx = int16(sz.cx * a);
	sz.cy = int16(sz.cy * a);
	return sz;
}

inline Размер16& operator/=(Размер16& sz, double a)
{
	sz.cx = int16(sz.cx / a);
	sz.cy = int16(sz.cy / a);
	return sz;
}

inline Размер16& operator*=(Размер16& sz, РазмерПЗ a)
{
	sz.cx = int16(sz.cx * a.cx);
	sz.cy = int16(sz.cy * a.cy);
	return sz;
}

inline Размер16& operator/=(Размер16& sz, РазмерПЗ a)
{
	sz.cx = int16(sz.cx / a.cx);
	sz.cy = int16(sz.cy / a.cy);
	return sz;
}

inline РазмерПЗ operator*(Размер16 sz, double a)   { return РазмерПЗ(sz.cx * a, sz.cy * a); }
inline РазмерПЗ operator*(double a, Размер16 sz)   { return РазмерПЗ(sz.cx * a, sz.cy * a); }
inline РазмерПЗ operator/(Размер16 sz, double a)   { return РазмерПЗ(sz.cx / a, sz.cy / a); }
inline РазмерПЗ operator*(Размер16 sz, РазмерПЗ a)  { return РазмерПЗ(sz.cx * a.cx, sz.cy * a.cy); }
inline РазмерПЗ operator*(РазмерПЗ a, Размер16 sz)  { return РазмерПЗ(sz.cx * a.cx, sz.cy * a.cy); }
inline РазмерПЗ operator/(Размер16 sz, РазмерПЗ a)  { return РазмерПЗ(sz.cx / a.cx, sz.cy / a.cy); }

inline Прям RectC(int x, int y, int cx, int cy) {
	return Прям(x, y, x + cx, y + cy);
}

inline Прям16  Rect16C(int16 x, int16 y, int16 cx, int16 cy) {
	return Прям16(x, y, x + cx, y + cy);
}

inline ПрямПЗ   RectfC(double x, double y, double cx, double cy) {
	return ПрямПЗ(x, y, x + cx, y + cy);
}

inline Прям  RectSort(Точка a, Точка b)    { return Прям(min(a, b), max(a, b) + 1); }
inline ПрямПЗ RectfSort(ТочкаПЗ a, ТочкаПЗ b) { return ПрямПЗ(min(a, b), max(a, b)); }

Поток& пакуй16(Поток& s, Точка& p);
Поток& пакуй16(Поток& s, Размер& sz);
Поток& пакуй16(Поток& s, Прям& r);

Размер iscale(Размер a,  int b, int c);
Размер iscalefloor(Размер a,  int b, int c);
Размер iscaleceil(Размер a,  int b, int c);
Размер idivfloor(Размер a,  int b);
Размер idivceil(Размер a,  int b);
Размер iscale(Размер a,  Размер b, Размер c);
Размер iscalefloor(Размер a,  Размер b, Размер c);
Размер iscaleceil(Размер a,  Размер b, Размер c);
Размер idivfloor(Размер a,  Размер b);
Размер idivceil(Размер a,  Размер b);

enum Alignment {
	ALIGN_NULL,
	ALIGN_LEFT,
	ALIGN_TOP = ALIGN_LEFT,
	ALIGN_RIGHT,
	ALIGN_BOTTOM = ALIGN_RIGHT,
	ALIGN_CENTER,
	ALIGN_JUSTIFY,
};

Размер        дайСоотношение(Размер stdsize, int cx, int cy);
Размер        дайРазмСхождения(Размер objsize, int cx, int cy);
inline Размер дайРазмСхождения(Размер objsize, Размер intosize) { return дайРазмСхождения(objsize, intosize.cx, intosize.cy); }

РазмерПЗ дайРазмСхождения(РазмерПЗ sz, double cx, double cy);
inline РазмерПЗ дайРазмСхождения(РазмерПЗ objsize, РазмерПЗ intosize) { return дайРазмСхождения(objsize, intosize.cx, intosize.cy); }

double вКвадрате(const ТочкаПЗ& p);
double длина(const ТочкаПЗ& p);
double направление(const ТочкаПЗ& p);
double расстояние(const ТочкаПЗ& p1, const ТочкаПЗ& p2);
double растояниеВКвадрате(const ТочкаПЗ& p1, const ТочкаПЗ& p2);
ТочкаПЗ середина(const ТочкаПЗ& a, const ТочкаПЗ& b);
ТочкаПЗ ортогональ(const ТочкаПЗ& p);
ТочкаПЗ нормализуй(const ТочкаПЗ& p);
ТочкаПЗ поляр(double a);
ТочкаПЗ поляр(const ТочкаПЗ& p, double r, double a);


// deprecated because of confusing имя:
	
inline double Bearing(const ТочкаПЗ& p) { return направление(p); }
