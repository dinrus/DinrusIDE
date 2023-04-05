template <class T> class Ук;

class PteBase {
protected:
	struct Prec {
		PteBase *ptr;
		Атомар   n;
	};

	volatile Prec  *prec;

	Prec           *добавьУк();
	static void     отпустиУк(Prec *prec);
	static Prec    *добавьУк(const Uuid& uuid);

	PteBase();
	~PteBase();

	friend class УкОснова;
};

class УкОснова {
protected:
	PteBase::Prec *prec;
	void уст(PteBase *p);
	void отпусти();
	void присвой(PteBase *p);

public:
	~УкОснова();
};

template <class T>
class Pte : public PteBase {
	friend class Ук<T>;
};

template <class T>
class Ук : public УкОснова, Движимое< Ук<T> > {
	T   *дай() const                          { return prec ? static_cast<T *>(prec->ptr) : NULL; }

public:
	T       *operator->() const               { return дай(); }
	T       *operator~() const                { return дай(); }
	operator T*() const                       { return дай(); }

	Ук& operator=(T *ptr)                    { присвой(ptr); return *this; }
	Ук& operator=(const Ук& ptr)            { присвой(ptr.дай()); return *this; }

	Ук()                                     { prec = NULL; }
	Ук(T *ptr)                               { уст(ptr); }
	Ук(const Ук& ptr)                       { уст(ptr.дай()); }

	Ткст вТкст() const;

	friend bool operator==(const Ук& a, const T *b)   { return a.дай() == b; }
	friend bool operator==(const T *a, const Ук& b)   { return a == b.дай(); }
	friend bool operator==(const Ук& a, const Ук& b) { return a.prec == b.prec; }

	friend bool operator==(const Ук& a, T *b)         { return a.дай() == b; }
	friend bool operator==(T *a, const Ук& b)         { return a == b.дай(); }

	friend bool operator!=(const Ук& a, const T *b)   { return a.дай() != b; }
	friend bool operator!=(const T *a, const Ук& b)   { return a != b.дай(); }
	friend bool operator!=(const Ук& a, const Ук& b) { return a.prec != b.prec; }

	friend bool operator!=(const Ук& a, T *b)         { return a.дай() != b; }
	friend bool operator!=(T *a, const Ук& b)         { return a != b.дай(); }
};

template <class T>
Ткст Ук<T>::вТкст() const
{
	return prec ? фмтУк(дай()) : Ткст("0x0");
}
