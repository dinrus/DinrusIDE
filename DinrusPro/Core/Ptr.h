template <class T> class Ук;

class PteBase {
protected:
	struct Prec {
		PteBase *укз;
		Атомар   n;
	};

	volatile Prec  *prec;

	Prec           *добавьУк();
	static проц     отпустиУк(Prec *prec);
	static Prec    *добавьУк(const Uuid& uuid);

	PteBase();
	~PteBase();

	friend class УкОснова;
};

class УкОснова {
protected:
	PteBase::Prec *prec;
	проц уст(PteBase *p);
	проц отпусти();
	проц присвой(PteBase *p);

public:
	~УкОснова();
};

template <class T>
class Pte : public PteBase {
	friend class Ук<T>;
};

template <class T>
class Ук : public УкОснова, Движ< Ук<T> > {
	T   *дай() const                          { return prec ? static_cast<T *>(prec->укз) : NULL; }

public:
	T       *operator->() const               { return дай(); }
	T       *operator~() const                { return дай(); }
	operator T*() const                       { return дай(); }

	Ук& operator=(T *укз)                    { присвой(укз); return *this; }
	Ук& operator=(const Ук& укз)            { присвой(укз.дай()); return *this; }

	Ук()                                     { prec = NULL; }
	Ук(T *укз)                               { уст(укз); }
	Ук(const Ук& укз)                       { уст(укз.дай()); }

	Ткст вТкст() const;

	friend бул operator==(const Ук& a, const T *b)   { return a.дай() == b; }
	friend бул operator==(const T *a, const Ук& b)   { return a == b.дай(); }
	friend бул operator==(const Ук& a, const Ук& b) { return a.prec == b.prec; }

	friend бул operator==(const Ук& a, T *b)         { return a.дай() == b; }
	friend бул operator==(T *a, const Ук& b)         { return a == b.дай(); }

	friend бул operator!=(const Ук& a, const T *b)   { return a.дай() != b; }
	friend бул operator!=(const T *a, const Ук& b)   { return a != b.дай(); }
	friend бул operator!=(const Ук& a, const Ук& b) { return a.prec != b.prec; }

	friend бул operator!=(const Ук& a, T *b)         { return a.дай() != b; }
	friend бул operator!=(T *a, const Ук& b)         { return a != b.дай(); }
};

template <class T>
Ткст Ук<T>::вТкст() const
{
	return prec ? фмтУк(дай()) : Ткст("0x0");
}
