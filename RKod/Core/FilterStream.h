class ВхоФильтрПоток : public Поток {
public:
	virtual   bool  открыт() const;

protected:
	virtual   int   _прекрати();
	virtual   int   _получи();
	virtual   dword _получи(void *данные, dword size);

	Вектор<byte> буфер;
	bool         eof;
	int          buffersize = 4096;
	Буфер<int>  inbuffer;
	byte        *t; // target pointer for block _получи
	dword        todo; // target count

	void   иниц();
	void   фетч();
	void   SetRd();

private:
	void   устРазм(int64 size)  { НИКОГДА(); } // removed
	int64  дайРазм() const      { НИКОГДА(); return 0; }

public:
	Поток                      *in;
	Событие<const void *, int>     фильтруй;
	Врата<>                       FilterEof;
	Событие<>                      стоп;
	Врата<>                       ещё;
	void                         выведи(const void *ptr, int size);
	
	Событие<>                      WhenOut;
	
	template <class F>
	void уст(Поток& in_, F& filter) {
		иниц();
		in = &in_;
		filter.WhenOut = [=](const void *ptr, int size) { выведи(ptr, size); };
		фильтруй = [&filter](const void *ptr, int size) { filter.помести(ptr, size); };
		стоп = [&filter] { filter.стоп(); };
	}
	
	void устРазмБуф(int size) { buffersize = size; inbuffer.очисть(); }
	
	ВхоФильтрПоток();
	template <class F> ВхоФильтрПоток(Поток& in, F& filter) { уст(in, filter); }
};

class ВыхФильтрПоток : public Поток {
public:
	virtual   void  открой();
	virtual   bool  открыт() const;

protected:
	virtual   void  _помести(int w);
	virtual   void  _помести(const void *данные, dword size);

	Буфер<byte> буфер;
	int64        count;

	void   излей();
	dword  Avail()               { return dword(4096 - (ptr - ~буфер)); }
	void   иниц();

public:
	Поток                      *out;
	Событие<const void *, int>     фильтруй;
	Событие<>                      стоп;
	void                         выведи(const void *ptr, int size);
	
	int64                        дайСчёт() const             { return count; }
	
	Событие<int64>                 WhenPos;

	template <class F>
	void уст(Поток& out_, F& filter) {
		иниц();
		out = &out_;
		filter.WhenOut = обрвыз(this, &ВыхФильтрПоток::выведи);
		фильтруй = обрвыз<F, F, const void *, int>(&filter, &F::помести);
		стоп = обрвыз(&filter, &F::стоп);
		count = 0;
	}
	
	ВыхФильтрПоток();
	template <class F> ВыхФильтрПоток(Поток& in, F& filter) { уст(in, filter); }
	~ВыхФильтрПоток();
};
