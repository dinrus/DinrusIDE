#ifndef _ide_Designers_Designers_h_
#define _ide_Designers_Designers_h_

#include <HexView/HexView.h>
#include <DinrusIDE/Common/Common.h>
#include <DinrusIDE/IconDes/IconDes.h>
#include <DinrusIDE/DinrusIDE.h>

struct IdeImgView : IdeDesigner, Ктрл {
	Размер   img_sz;
	Рисунок  img;
	Ткст filename;
	
	virtual void   рисуй(Draw& w);

	virtual Ткст дайИмяф() const        { return filename; }
	virtual void   EditMenu(Бар& menu);
	virtual void   сохрани()                     {}
	virtual Ктрл&  DesignerCtrl()             { return *this; }

	typedef IdeImgView ИМЯ_КЛАССА;
};

struct IdePngDes : IdeIconDes {
	virtual void   сохрани();

	void   грузи(const char *filename);
	void   создай(const char *filename);
};

ИНИЦИАЛИЗУЙ(Img)

struct FileHexView : public IdeDesigner, public ГексОбзор, private LRUCache<Ткст, int64>::Делец {
	virtual bool   Ключ(dword ключ, int count)  { return ГексОбзор::Ключ(ключ, count); } // silence CLANG warning

public:
	virtual int    Байт(int64 addr);

	virtual int64  Ключ() const;
	virtual int    сделай(Ткст& object) const;

	virtual Ткст дайИмяф() const        { return filename; }
	virtual void   сохрани()                     {}
	virtual void   EditMenu(Бар& menu)        { стдМеню(menu); }
	virtual Ктрл&  DesignerCtrl()             { return *this; }
	virtual void   RestoreEditPos();
	virtual void   SaveEditPos();

	Ткст  filename;

	ФайлВвод  file;
	int64   blk;
	
	LRUCache<Ткст, int64> cache;

	static ВекторМап<Ткст, Tuple2<int64, int64>> pos;
	
	enum { BLKSHIFT = 14, BLKSIZE = 1 << BLKSHIFT };
	
	void    сериализуй(Поток& s)              { сериализуйНастройки(s); }

	void открой(const char *path);
	
	FileHexView();
	~FileHexView();
};

ИНИЦИАЛИЗУЙ(ГексОбзор)

struct IdeQtfDes : IdeDesigner, RichEditWithToolBar {
	Ткст filename;

	virtual Ткст дайИмяф() const        { return filename; }
	virtual void   сохрани();
	virtual void   SaveEditPos();
	virtual void   EditMenu(Бар& menu);
	virtual Ктрл&  DesignerCtrl()             { return *this; }

	virtual void   сериализуй(Поток& s);

	bool   грузи(const char *filename);
	void   FileProperties();
	void   CopyId(const Ткст& n);

	typedef IdeQtfDes ИМЯ_КЛАССА;
	
	IdeQtfDes() { Extended(); }
};

ИНИЦИАЛИЗУЙ(Qtf)

struct IdeMDDes : IdeDesigner, КтрлРодитель {
	Ткст       filename;
	ДокРедакт      editor;
	RichTextView preview;
	Сплиттер     splitter;
	ОбрвызВремени delay;

	virtual void сфокусирован();
	virtual Ткст дайИмяф() const        { return filename; }
	virtual void   сохрани();
	virtual void   SaveEditPos();
	virtual void   EditMenu(Бар& menu);
	virtual Ктрл&  DesignerCtrl()             { return *this; }
	
	virtual void   сериализуй(Поток& s);
	
	void    Preview();

	bool   грузи(const char *filename);

	IdeMDDes();
};

ИНИЦИАЛИЗУЙ(MD)

bool  FileIsBinary(const char *path);

struct TreeDesPos {
	Время               filetime = Null;
	Вектор<int>        openid;
	int                cursor = -1;
	Точка              scroll;

	void сериализуй(Поток& s) { s % filetime % openid % cursor % scroll; }
	void сохрани(Время filetime, КтрлДерево& tree);
	void Restore(Время filetime, КтрлДерево& tree);
};

void сохрани(МассивМап<Ткст, TreeDesPos>& pos, const Ткст& filename, Время filetime, КтрлДерево& tree);
void Restore(МассивМап<Ткст, TreeDesPos>& pos, const Ткст& filename, Время filetime, КтрлДерево& tree);
void сериализуй(Поток& s, МассивМап<Ткст, TreeDesPos>& pos);

struct TreeViewDes : IdeDesigner, Ктрл {
	virtual Ткст дайИмяф() const        { return filename; }
	virtual void   сохрани()                     {}
	virtual void   EditMenu(Бар& menu);
	virtual Ктрл&  DesignerCtrl()             { return *this; }
	virtual void   RestoreEditPos();
	virtual void   SaveEditPos();

	static МассивМап<Ткст, TreeDesPos> pos;

	Ткст                filename;
	Время                  filetime;
	КтрлДерево              tree;
	ФреймВерх<СтатичПрям>  errorbg;
	Надпись                 Ошибка;
	КтрлРодитель            data;
	
	TreeViewDes();
	
	void ошибка(const char *e);
	bool грузи(const Ткст& txt);

	virtual Ткст дайИд() = 0;
	virtual void   CopyPath() = 0;
	virtual Ткст загрузи0(const Ткст& data) = 0;
};

ИНИЦИАЛИЗУЙ(TreeViewDes)

struct XmlViewDes : TreeViewDes {
	virtual Ткст дайИд() { return "XML"; }
	virtual void   CopyPath();
	virtual Ткст загрузи0(const Ткст& data);

	void   загрузи0(int parent, ПарсерРяр& p);
};

ИНИЦИАЛИЗУЙ(XmlViewDes)

struct JsonViewDes : TreeViewDes {
	virtual Ткст дайИд() { return "JSON"; }
	virtual Ткст загрузи0(const Ткст& json);
	virtual void   CopyPath();
	
	int AddNode(int parent_id, const Значение& id, const Ткст& имя, const Значение& v);
	
	JsonViewDes();
};

ИНИЦИАЛИЗУЙ(JsonViewDes)

#endif
