template <цел N, typename... T>
struct КортежН;

template <цел I>
struct IndexI__ {};

template <typename V, typename T, typename I>
const V& дайИзКортежа(const T& t, const I&);

template <typename... T>
struct Кортеж;

template <typename A>
struct КортежН<1, A>
{
	A a;

	бул  operator==(const КортежН& x) const   { return a == x.a; }
	цел   сравни(const КортежН& x) const      { return сравниЗнак(a, x.a); }

	проц  вХэш(КомбХэш& h) const        { h << a; }
	проц  вТкст(Ткст& r) const           { r << какТкст(a); }
	
	проц  сериализуй(Поток& s)                { s % a; }
	
	цел   дайСчёт() const                    { return 1; }

	Значение дай(цел i) const                    { ПРОВЕРЬ(i == 0); return a; }
	проц  уст(цел i, const Значение& v)          { ПРОВЕРЬ(i == 0); a = v; }

	КортежН(const A& a) : a(a) {}
	КортежН() {}
	
	template <typename AA>
	operator Кортеж<AA>()                      { Кортеж<AA> t; t.a = (AA)a; return t; }
};

#define TUPLE_N_METHODS(M, I) \
	бул operator==(const КортежН& x) const    { return Основа::operator==(x) && M == x.M; } \
	цел  сравни(const КортежН& x) const       { цел q = Основа::сравни(x); return q ? q : сравниЗнак(M, x.M); } \
 \
	проц вХэш(КомбХэш& h) const         { Основа::вХэш(h); h << M; } \
	проц вТкст(Ткст& r) const            { Основа::вТкст(r); r << ", " << M; } \
	 \
	проц сериализуй(Поток& s)                 { Основа::сериализуй(s); s % M; } \
	 \
	цел   дайСчёт() const                    { return I + 1; } \
 \
	Значение дай(цел i) const                    { if(i == I) return M; return Основа::дай(i); } \
	проц  уст(цел i, const Значение& v)          { if(i == I) M = v; else Основа::уст(i, v); } \
 \
	КортежН() {} \


template <typename A, typename B>
struct КортежН<2, A, B> : public КортежН<1, A> {
	typedef КортежН<1, A> Основа;
	B b;
	
	TUPLE_N_METHODS(b, 1);

	КортежН(const A& a, const B& b) : Основа(a), b(b) {}

	template <typename AA, typename BB>
	operator Кортеж<AA, BB>()                  { Кортеж<AA, BB> t; t.a = (AA)Основа::a; t.b = b; return t; }
};

template <typename A, typename B, typename C>
struct КортежН<3, A, B, C> : public КортежН<2, A, B>
{
	typedef КортежН<2, A, B> Основа;
	C c;

	TUPLE_N_METHODS(c, 2);

	КортежН(const A& a, const B& b, const C& c) : Основа(a, b), c(c) {}

	template <typename AA, typename BB, typename CC>
	operator Кортеж<AA, BB, CC>() { Кортеж<AA, BB, CC> t; t.a = (AA)Основа::a; t.b = (BB)Основа::b; t.c = (CC)c; return t; }
};

template <typename A, typename B, typename C, typename D>
struct КортежН<4, A, B, C, D> : public КортежН<3, A, B, C>
{
	typedef КортежН<3, A, B, C> Основа;
	D d;

	TUPLE_N_METHODS(d, 3);
	
	КортежН(const A& a, const B& b, const C& c, const D& d) : Основа(a, b, c), d(d) {}

	template <typename AA, typename BB, typename CC, typename DD>
	operator Кортеж<AA, BB, CC, DD>() { Кортеж<AA, BB, CC, DD> t; t.a = (AA)Основа::a; t.b = (BB)Основа::b; t.c = (CC)Основа::c; t.d = (DD)d; return t; }
};


#define ДАЙ_ИЗ_КОРТЕЖА(M, I) \
 \
template <typename T> \
auto дайИзКортежа(const T& t, const IndexI__<I>&) -> decltype(t.M)& \
{ \
	return const_cast<T&>(t).M; \
} \
 \
template <typename T> \
auto дайИзКортежаПоТипу(const T& t, decltype(t.M)*, const IndexI__<I>* = NULL) -> decltype(t.M)& \
{ \
	return const_cast<T&>(t).M; \
}

ДАЙ_ИЗ_КОРТЕЖА(a, 0)
ДАЙ_ИЗ_КОРТЕЖА(b, 1)
ДАЙ_ИЗ_КОРТЕЖА(c, 2)
ДАЙ_ИЗ_КОРТЕЖА(d, 3)

template <typename... Арги>
struct Кортеж : public КортежН<sizeof...(Арги), Арги...> {
private:
	typedef КортежН<sizeof...(Арги), Арги...> Основа;

	friend проц подтвердиДвиж0(Кортеж *) {}

public:
	template <цел I>
	const auto& дай() const { return дайИзКортежа(*this, IndexI__<I>()); }
	template <цел I>
	auto& дай() { return дайИзКортежа(*this, IndexI__<I>()); }
	
	template <typename T> const T& дай() const { return дайИзКортежаПоТипу(*this, (T*)NULL); }
	template <typename T> T& дай()             { return дайИзКортежаПоТипу(*this, (T*)NULL); }

	цел  дайСчёт() const                      { return Основа::дайСчёт(); }

	бул operator==(const Кортеж& x) const     { return Основа::operator==(x); }
	бул operator!=(const Кортеж& x) const     { return !operator==(x); }
	
	цел  сравни(const Кортеж& x) const        { return Основа::сравни(x); }
	бул operator<=(const Кортеж& x) const     { return сравни(x) <= 0; }
	бул operator>=(const Кортеж& x) const     { return сравни(x) >= 0; }
	бул operator<(const Кортеж& x) const      { return сравни(x) < 0; }
	бул operator>(const Кортеж& x) const      { return сравни(x) > 0; }

	т_хэш дайХэшЗнач() const               { КомбХэш h; Основа::вХэш(h); return h; }
	
	проц сериализуй(Поток& s)                 { Основа::сериализуй(s); }

	Ткст вТкст() const                   { Ткст h = "("; Основа::вТкст(h); h << ")"; return h; }
	
	Значение дай(цел i) const                    { return Основа::дай(i); }
	проц  уст(цел i, const Значение& v)          { return Основа::уст(i, v); }
	МассивЗнач дайМассив() const               { МассивЗнач va; for(цел i = 0; i < дайСчёт(); i++) va.добавь(дай(i)); return va; }
	проц  устМассив(const МассивЗнач& va)      { for(цел i = 0; i < va.дайСчёт(); i++) уст(i, va[i]); }
	
	Кортеж() {}
	Кортеж(const Арги... арги) : Основа(арги...) {};
};

template <typename... Арги>
Кортеж<Арги...> сделайКортеж(const Арги... арги) {
	return Кортеж<Арги...>(арги...);
}

template <typename T, typename U>
inline T *найдиКортеж(T *x, цел n, const U& ключ) {
	while(n--) {
		if(x->a == ключ)
			return x;
		x++;
	}
	return NULL;
}

template <typename A, typename B>
struct Tie2 {
	A& a;
	B& b;
	
	проц operator=(const Кортеж<A, B>& s)  { a = s.a; b = s.b; }

	Tie2(A& a, B& b) : a(a), b(b) {}
};

template <typename A, typename B>
Tie2<A, B> Tie(A& a, B& b) { return Tie2<A, B>(a, b); }

template <typename A, typename B, typename C>
struct Tie3 {
	A& a;
	B& b;
	C& c;
	
	проц operator=(const Кортеж<A, B, C>& s) { a = s.a; b = s.b; c = s.c; }

	Tie3(A& a, B& b, C& c) : a(a), b(b), c(c) {}
};

template <typename A, typename B, typename C>
Tie3<A, B, C> Tie(A& a, B& b, C& c) { return Tie3<A, B, C>(a, b, c); }

template <typename A, typename B, typename C, typename D>
struct Tie4 {
	A& a;
	B& b;
	C& c;
	D& d;
	
	проц operator=(const Кортеж<A, B, C, D>& s) { a = s.a; b = s.b; c = s.c; d = s.d; }

	Tie4(A& a, B& b, C& c, D& d) : a(a), b(b), c(c), d(d) {}
};

template <typename A, typename B, typename C, typename D>
Tie4<A, B, C, D> Tie(A& a, B& b, C& c, D& d) { return Tie4<A, B, C, D>(a, b, c, d); }

// Backward compatibility

template <typename A, typename B> using Tuple2 = Кортеж<A, B>;
template <typename A, typename B, typename C> using Tuple3 = Кортеж<A, B, C>;
template <typename A, typename B, typename C, typename D> using Tuple4 = Кортеж<A, B, C, D>;
