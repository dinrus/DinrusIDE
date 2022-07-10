template <class T, class... Арги>
T& Single(Арги... арги) {
	static T o(арги...);
	return o;
}

template <class T> // Workaround for GCC bug - specialization needed...
T& Single() {
	static T o;
	return o;
}

int RegisterTypeNo__(const char *тип);

template <class T>
int StaticTypeNo() {
	static int typeno = RegisterTypeNo__(typeid(T).name());
	return typeno;
}

class Любое : Движимое<Любое> {
	struct ДанныеОснова {
		int      typeno;
		
		virtual ~ДанныеОснова() {}
	};

	template <class T>
	struct Данные : ДанныеОснова {
		T        данные;

		template <class... Арги>
		Данные(Арги&&... арги) : данные(std::forward<Арги>(арги)...) { typeno = StaticTypeNo<T>(); }
	};

	ДанныеОснова *ptr;

	void Chk() const                              { ПРОВЕРЬ(ptr != (void *)1); }
	void подбери(Любое&& s)                            { ptr = s.ptr; const_cast<Любое&>(s).ptr = NULL; }

public:
	template <class T, class... Арги> T& создай(Арги&&... арги) { очисть(); Данные<T> *x = new Данные<T>(std::forward<Арги>(арги)...); ptr = x; return x->данные; }
	template <class T> bool является() const            { return ptr && ptr->typeno == StaticTypeNo<T>(); }
	template <class T> T& дай()                   { ПРОВЕРЬ(является<T>()); Chk(); return ((Данные<T>*)ptr)->данные; }
	template <class T> const T& дай() const       { ПРОВЕРЬ(является<T>()); Chk(); return ((Данные<T>*)ptr)->данные; }

	void очисть()                                  { if(ptr) delete ptr; ptr = NULL; }

	bool пустой() const                          { return ptr == NULL; }

	void operator=(Любое&& s)                       { if(this != &s) { очисть(); подбери(pick(s)); } }
	Любое(Любое&& s)                                  { подбери(pick(s)); }
	
	Любое(const Любое& s) = delete;
	void operator=(const Любое& s) = delete;

	Любое()                                         { ptr = NULL; }
	~Любое()                                        { очисть(); }
};

class Биты : Движимое<Биты> {
	int         alloc;
	dword      *bp;
	
	void расширь(int q);
	void переразмести(int nalloc);
	int  дайПоследн() const;

public:
	void   очисть();
	void   уст(int i, bool b = true) { ПРОВЕРЬ(i >= 0 && alloc >= 0); int q = i >> 5; if(q >= alloc) расширь(q);
	                                   i &= 31; bp[q] = (bp[q] & ~(1 << i)) | (b << i); }
	bool   дай(int i) const        { ПРОВЕРЬ(i >= 0 && alloc >= 0); int q = i >> 5;
	                                 return q < alloc ? bp[q] & (1 << (i & 31)) : false; }
	bool   operator[](int i) const { return дай(i); }

	void   уст(int i, dword bits, int count);
	dword  дай(int i, int count);
	void   уст64(int i, uint64 bits, int count);
	uint64 дай64(int i, int count);

	void   устН(int i, bool b, int count);
	void   устН(int i, int count)         { устН(i, true, count); }
	
	void   резервируй(int nbits);
	void   сожми();
	
	Ткст вТкст() const;

	dword       *создайРяд(int n_dwords);
	const dword *дайРяд(int& n_dwords) const { n_dwords = alloc; return bp; }
	dword       *дайРяд(int& n_dwords)       { n_dwords = alloc; return bp; }
	
	void         сериализуй(Поток& s);

	Биты()                                { bp = NULL; alloc = 0; }
	~Биты()                               { очисть(); }

	Биты(Биты&& b)                        { alloc = b.alloc; bp = b.bp; b.bp = NULL; }
	void operator=(Биты&& b)              { if(this != &b) { очисть(); alloc = b.alloc; bp = b.bp; b.bp = NULL; } }
	
	Биты(const Биты&) = delete;
	void operator=(const Биты&) = delete;
};

//# System dependent
template <class T>
class Mitor : Движимое< Mitor<T> > {
	union {
		mutable unsigned   count;
		mutable Вектор<T> *vector;
	};
	byte elem0[sizeof(T)];

	T&        дай(int i) const;
	void      подбери(Mitor&& m);
	void      копируй(const Mitor& m);
	void      Chk() const               { ПРОВЕРЬ(count != 2); }

public:
	T&        operator[](int i)         { return дай(i); }
	const T&  operator[](int i) const   { return дай(i); }
	int       дайСчёт() const;
	T&        добавь();
	void      добавь(const T& x);
	void      очисть();
	void      сожми();

	Mitor(Mitor&& m)                    { подбери(pick(m)); }
	void operator=(Mitor&& m)           { if(this != &m) { очисть(); подбери(pick(m)); } }

	Mitor(Mitor& m, int)                { копируй(m); }

	Mitor()                             { count = 0; }
	~Mitor()                            { очисть(); }
};

template <class T>
T& Mitor<T>::дай(int i) const
{
	ПРОВЕРЬ(i >= 0 && i < дайСчёт());
	return i == 0 ? *(T*)elem0 : (*const_cast<Вектор<T>*>(vector))[i - 1];
}

template <class T>
void Mitor<T>::подбери(Mitor&& m)
{
	m.Chk();
	vector = m.vector;
	memcpy(&elem0, &m.elem0, sizeof(T));
	m.count = 2;
}

template <class T>
void Mitor<T>::копируй(const Mitor& m)
{
	m.Chk();
	if(m.count > 0)
		стройГлубКопию(elem0, (const T*)m.elem0);
	if(m.count > 1)
		vector = new Вектор<T>(*m.vector, 1);
}

template <class T>
int Mitor<T>::дайСчёт() const
{
	Chk();
	return count > 1 ? vector->дайСчёт() + 1 : count;
}

template <class T>
T& Mitor<T>::добавь()
{
	Chk();
	if(count == 0) {
		count = 1;
		return *new(elem0) T;
	}
	if(count == 1)
		vector = new Вектор<T>;
	return vector->добавь();
}

template <class T>
void Mitor<T>::добавь(const T& x)
{
	Chk();
	if(count == 0) {
		count = 1;
		new((T*) elem0) T(x);
	}
	else {
		if(count == 1)
			vector = new Вектор<T>;
		vector->добавь(x);
	}
}

template <class T>
void Mitor<T>::очисть()
{
	if(count > 2)
		delete vector;
	if(count && count != 2)
		((T*)elem0)->~T();
	count = 0;
}

template <class T>
void Mitor<T>::сожми()
{
	if(count > 2)
		vector->сожми();
}

//#
template <class T, int N = 1>
struct Линк {
	T *link_prev[N];
	T *link_next[N];

protected:
	void LPN(int i)                      { link_prev[i]->link_next[i] = link_next[i]->link_prev[i] = (T *)this; }

public:
	NOUBSAN	T *дайУк()                  { return (T *) this; }
	const T *дайУк() const              { return (const T *) this; }
	T       *дайСледщ(int i = 0)          { return link_next[i]; }
	T       *дайПредш(int i = 0)          { return link_prev[i]; }
	const T *дайСледщ(int i = 0) const    { return link_next[i]; }
	const T *дайПредш(int i = 0) const    { return link_prev[i]; }

	NOUBSAN	void линкуйся(int i = 0)     { link_next[i] = link_prev[i] = (T *)this; }
	void линкуйсяВесь()                   { for(int i = 0; i < N; i++) линкуйся(i); }
	void отлинкуй(int i = 0)               { link_next[i]->link_prev[i] = link_prev[i]; link_prev[i]->link_next[i] = link_next[i];
	                                       линкуйся(i); }
	void отлинкуйВсе()                     { for(int i = 0; i < N; i++) отлинкуй(i); }
	NOUBSAN	void линкПеред(Линк *n, int i = 0)  { link_next[i] = (T *)n; link_prev[i] = link_next[i]->link_prev[i]; LPN(i); }
	NOUBSAN	void линкПосле(Линк *p, int i = 0)   { link_prev[i] = (T *)p; link_next[i] = link_prev[i]->link_next[i]; LPN(i); }

	T   *вставьСледщ(int i = 0)           { T *x = new T; x->линкПосле(this, i); return x; }
	T   *вставьПредш(int i = 0)           { T *x = new T; x->линкПеред(this, i); return x; }

	void удалиСписок(int i = 0)           { while(link_next[i] != дайУк()) delete link_next[i]; }

	bool вСписке(int i = 0) const         { return link_next[i] != дайУк(); }
	bool пустой(int i = 0) const        { return !вСписке(i); }

	Линк()                               { линкуйсяВесь(); }
	~Линк()                              { отлинкуйВсе(); }

private:
	Линк(const Линк&);
	void operator=(const Линк&);

public:
#ifdef _ОТЛАДКА
	void дамп() {
		for(T *t = дайСледщ(); t != this; t = t->дайСледщ())
			LOG(t);
		LOG("-------------------------------------");
	}
#endif
};

template <class T, int N = 1>
class ЛинкВлад : public Линк<T, N> {
public:
	~ЛинкВлад()                         { Линк<T, N>::удалиСписок(); }
};

template <class T, class K = Ткст>
class LRUCache {
public:
	struct Делец {
		virtual K      Ключ() const = 0;
		virtual int    сделай(T& object) const = 0;
		virtual ~Делец() {}
	};

private:
	struct Элемент : Движимое<Элемент> {
		int    prev, next;
		int    size;
		Один<T> данные;
		bool   flag;
	};
	
	struct Ключ : Движимое<Ключ> {
		K            ключ;
		Ткст       тип;
		
		bool operator==(const Ключ& b) const { return ключ == b.ключ && тип == b.тип; }
		hash_t дайХэшЗнач() const { return комбинируйХэш(ключ, тип); }
	};

	Индекс<Ключ>   ключ;
	Вектор<Элемент> данные;
	int  head;

	int  size;
	int  count;

	int  foundsize;
	int  newsize;
	bool flag = false;
	
	const int InternalSize = 3 * (sizeof(Элемент) + sizeof(Ключ) + 24) / 2;

	void отлинкуй(int i);
	void линкуйГолову(int i);

public:
	int  дайРазм() const            { return size; }
	int  дайСчёт() const           { return count; }

	template <class P> void настройРазм(P getsize);

	T&       GetLRU();
	const K& GetLRUKey();
	void     DropLRU();
	void     сожми(int maxsize, int maxcount = 30000);
	void     сожмиСчёт(int maxcount = 30000)          { сожми(INT_MAX, maxcount); }

	template <class P> int  удали(P predicate);
	template <class P> bool удалиОдин(P predicate);

	T&   дай(const Делец& m);

	void очисть();

	void очистьСчётчики();
	int  дайНайденРазм() const       { return foundsize; }
	int  дайНовРазм() const         { return newsize; }

	LRUCache() { head = -1; size = 0; count = 0; очистьСчётчики(); }
};

template <class T, class K>
void LRUCache<T, K>::линкуйГолову(int i)
{
	Элемент& m = данные[i];
	if(head >= 0) {
		int tail = данные[head].prev;
		m.next = head;
		m.prev = tail;
		данные[head].prev = i;
		данные[tail].next = i;
	}
	else
		m.prev = m.next = i;
	head = i;
	count++;
}


template <class T, class K>
void LRUCache<T, K>::отлинкуй(int i)
{
	Элемент& m = данные[i];
	if(m.prev == i)
		head = -1;
	else {
		if(head == i)
			head = m.next;
		данные[m.next].prev = m.prev;
		данные[m.prev].next = m.next;
	}
	count--;
}

template <class T, class K>
T& LRUCache<T, K>::GetLRU()
{
	int tail = данные[head].prev;
	return *данные[tail].данные;
}

template <class T, class K>
const K& LRUCache<T, K>::GetLRUKey()
{
	int tail = данные[head].prev;
	return ключ[tail].ключ;
}

template <class T, class K>
void LRUCache<T, K>::DropLRU()
{
	if(head >= 0) {
		int tail = данные[head].prev;
		size -= данные[tail].size;
		данные[tail].данные.очисть();
		отлинкуй(tail);
		ключ.отлинкуй(tail);
	}
}

template <class T, class K>
template <class P>
void LRUCache<T, K>::настройРазм(P getsize)
{
	size = 0;
	count = 0;
	for(int i = 0; i < данные.дайСчёт(); i++)
		if(!ключ.отлинкован(i)) {
			int sz = getsize(*данные[i].данные);
			if(sz >= 0)
				данные[i].size = sz + InternalSize;
			size += данные[i].size;
			count++;
		}
}

template <class T, class K>
template <class P>
int LRUCache<T, K>::удали(P predicate)
{
	int n = 0;
	int i = 0;
	while(i < данные.дайСчёт())
		if(!ключ.отлинкован(i) && predicate(*данные[i].данные)) {
			size -= данные[i].size;
			отлинкуй(i);
			ключ.отлинкуй(i);
			n++;
		}
		else
			i++;
	return n;
}

template <class T, class K>
template <class P>
bool LRUCache<T, K>::удалиОдин(P predicate)
{
	int i = head;
	if(i >= 0)
		for(;;) {
			int next = данные[i].next;
			if(predicate(*данные[i].данные)) {
				size -= данные[i].size;
				отлинкуй(i);
				ключ.отлинкуй(i);
				return true;
			}
			if(i == next || next == head || next < 0)
				break;
			i = next;
		}
	return false;
}

template <class T, class K>
void LRUCache<T, K>::сожми(int maxsize, int maxcount)
{
	if(maxsize >= 0 && maxcount >= 0)
		while(count > maxcount || size > maxsize)
			DropLRU();
}

template <class T, class K>
void LRUCache<T, K>::очисть()
{
	head = -1;
	size = 0;
	count = 0;
	newsize = foundsize = 0;
	ключ.очисть();
	данные.очисть();
}

template <class T, class K>
void LRUCache<T, K>::очистьСчётчики()
{
	flag = !flag;
	newsize = foundsize = 0;
}

template <class T, class K>
T& LRUCache<T, K>::дай(const Делец& m)
{
	Ключ k;
	k.ключ = m.Ключ();
	k.тип = typeid(m).name();
	int q = ключ.найди(k);
	if(q < 0) {
		q = ключ.помести(k);
		Элемент& t = данные.по(q);
		t.size = m.сделай(t.данные.создай()) + InternalSize;
		size += t.size;
		newsize += t.size;
		t.flag = flag;
	}
	else {
		Элемент& t = данные[q];
		отлинкуй(q);
		if(t.flag != flag) {
			t.flag = flag;
			foundsize += t.size;
		}
	}
	линкуйГолову(q);
	return *данные[q].данные;
}
