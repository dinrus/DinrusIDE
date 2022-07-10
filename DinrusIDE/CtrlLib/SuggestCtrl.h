class SuggestCtrl : public EditString {
	virtual bool  Ключ(dword ключ, int count);
	virtual void  режимОтмены();
	virtual void  расфокусирован();
	virtual void  сфокусирован();

private:
	Вектор<ШТкст> data;
	КтрлМассив       list;
	int             droplines;
	int             delimiter_char;
	int           (*delimiter_filter)(int c);
	int           (*compare_filter)(int c);
	bool            just_start;

	void            Cancel();
	void            выдели();
	ШТкст         ReadLast(int& h);
	int             IsDelimiter(int c);
	ШТкст         CF(const ШТкст& src);

	typedef SuggestCtrl ИМЯ_КЛАССА;

public:
	void очистьСписок()                                  { data.очисть(); }
	void добавьСписок(const ШТкст& s)                    { data.добавь(s); }
	void добавьСписок(const Ткст& s)                     { data.добавь(s.вШТкст()); }
	void добавьСписок(const char *s)                       { data.добавь(s); }
	void найдиДобавьСписок(const ШТкст& s)                { if(найдиИндекс(data,s) < 0) data.добавь(s); }
	void найдиДобавьСписок(const Ткст& s)                 { найдиДобавьСписок(s.вШТкст()); }
	void найдиДобавьСписок(const char *s)                   { найдиДобавьСписок(Ткст(s)); }

	const Вектор<ШТкст>& GetList()                  { return data; }
	void подбери(Вектор<ШТкст>&& list)                 { data = pick(list); }
	
	SuggestCtrl& SetDropLines(int n)                  { droplines = n; return *this; }
	SuggestCtrl& Delimiter(int chr)                   { delimiter_char = chr; return *this; }
	SuggestCtrl& Delimiter(int (*filter)(int c))      { delimiter_filter = filter; return *this; }
	SuggestCtrl& сравниФильтр(int (*filter)(int c))  { compare_filter = filter; return *this; }
	SuggestCtrl& JustStart(bool b = true)             { just_start = b; return *this; }
		
	SuggestCtrl();
};
