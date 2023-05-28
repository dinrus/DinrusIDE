class ВхоФильтрПоток : public Поток {
public:
	virtual   бул  открыт() const;

protected:
	virtual   цел   _прекрати();
	virtual   цел   _получи();
	virtual   бцел _получи(ук данные, бцел size);

	Вектор<ббайт> буфер;
	бул         eof;
	цел          buffersize = 4096;
	Буфер<цел>  inbuffer;
	ббайт        *t; // target pointer for block _получи
	бцел        todo; // target count

	проц   иниц();
	проц   фетч();
	проц   SetRd();

private:
	проц   устРазм(дол size)  { НИКОГДА(); } // removed
	дол  дайРазм() const      { НИКОГДА(); return 0; }

public:
	Поток                      *in;
	Событие<const проц *, цел>     фильтруй;
	Врата<>                       FilterEof;
	Событие<>                      стоп;
	Врата<>                       ещё;
	проц                         выведи(const ук укз, цел size);
	
	Событие<>                      WhenOut;
	
	template <class F>
	проц уст(Поток& in_, F& фильтр) {
		иниц();
		in = &in_;
		фильтр.WhenOut = [=](const ук укз, цел size) { выведи(укз, size); };
		фильтруй = [&фильтр](const ук укз, цел size) { фильтр.помести(укз, size); };
		стоп = [&фильтр] { фильтр.стоп(); };
	}
	
	проц устРазмБуф(цел size) { buffersize = size; inbuffer.очисть(); }
	
	ВхоФильтрПоток();
	template <class F> ВхоФильтрПоток(Поток& in, F& фильтр) { уст(in, фильтр); }
};

class ВыхФильтрПоток : public Поток {
public:
	virtual   проц  открой();
	virtual   бул  открыт() const;

protected:
	virtual   проц  _помести(цел w);
	virtual   проц  _помести(const ук данные, бцел size);

	Буфер<ббайт> буфер;
	дол        count;

	проц   излей();
	бцел  Avail()               { return бцел(4096 - (укз - ~буфер)); }
	проц   иниц();

public:
	Поток                      *out;
	Событие<const проц *, цел>     фильтруй;
	Событие<>                      стоп;
	проц                         выведи(const ук укз, цел size);
	
	дол                        дайСчёт() const             { return count; }
	
	Событие<дол>                 WhenPos;

	template <class F>
	проц уст(Поток& out_, F& фильтр) {
		иниц();
		out = &out_;
		фильтр.WhenOut = обрвыз(this, &ВыхФильтрПоток::выведи);
		фильтруй = обрвыз<F, F, const проц *, цел>(&фильтр, &F::помести);
		стоп = обрвыз(&фильтр, &F::стоп);
		count = 0;
	}
	
	ВыхФильтрПоток();
	template <class F> ВыхФильтрПоток(Поток& in, F& фильтр) { уст(in, фильтр); }
	~ВыхФильтрПоток();
};
