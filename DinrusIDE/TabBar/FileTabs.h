class FileTabs : public БарТаб
{
private:
	bool stackedicons:1;
	bool greyedicons:1;
	Цвет filecolor;
	Цвет extcolor;
protected:
	// Overload this to change grouping behaviour
	virtual Ткст GetFileGroup(const Ткст &file);
	// Overload this to change stacking behaviour
	virtual Ткст GetStackId(const Вкладка &a);
	virtual hash_t GetStackSortOrder(const Вкладка &a);	
	
	virtual void ComposeTab(Вкладка& tab, const Шрифт &font, Цвет ink, int style);
	virtual void ComposeStackedTab(Вкладка& tab, const Вкладка& stacked_tab, const Шрифт &font, Цвет ink, int style);
	virtual Размер GetStackedSize(const Вкладка &t);
	
public:
	FileTabs();

	virtual void сериализуй(Поток& s);

	void  	AddFile(const ШТкст &file, bool make_active = true);
	void  	AddFile(const ШТкст &file, Рисунок img, bool make_active = true);
	void  	InsertFile(int ix, const ШТкст &file, bool make_active = true);
	void  	InsertFile(int ix, const ШТкст &file, Рисунок img, bool make_active = true);

	void 	AddFiles(const Вектор<Ткст> &files, bool make_active = true);
	void  	AddFiles(const Вектор<Ткст> &files, const Вектор<Рисунок> &img, bool make_active = true);
	void  	InsertFiles(int ix, const Вектор<Ткст> &files, bool make_active = true);
	void  	InsertFiles(int ix, const Вектор<Ткст> &files, const Вектор<Рисунок> &img, bool make_active = true);
	
	void 	RenameFile(const ШТкст &from, const ШТкст &to, Рисунок icon = Null);
	
	FileTabs& FileColor(Цвет c) 	{ filecolor = c; освежи(); return *this; }
	FileTabs& ExtColor(Цвет c) 	{ extcolor = c; освежи(); return *this; }
	
	FileTabs& FileIcons(bool normal = true, bool stacked = true, bool stacked_greyedout = true);
	
	Вектор<Ткст>	GetFiles() const;
	FileTabs& operator<<(const FileTabs &src);
};
