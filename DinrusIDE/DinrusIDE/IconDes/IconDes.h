#ifndef _ide_IconDes_IconDes_h_
#define _ide_IconDes_IconDes_h_

#include <DinrusIDE/Common/Common.h>
#include <IconDes/IconDes.h>

struct IdeIconDes : IdeDesigner, IconDes {
	virtual Ткст дайИмяф() const;
	virtual void   сохрани();
	virtual void   SaveEditPos();
	virtual void   RestoreEditPos();
	virtual void   EditMenu(Бар& menu);
	virtual Ктрл&  DesignerCtrl()             { return *this; }

	virtual void   сериализуй(Поток& s);

	int     формат;
	Ткст  filename;
	Время    filetime;
	Ткст  EOL = "\r\n";

	virtual void   ToolEx(Бар& bar);
	virtual void   ListMenuEx(Бар& bar);

	bool   грузи(const char *filename);
	void   FileProperties();
	void   CopyId(const Ткст& n);
	void   найдиИд(const Ткст& id);

	typedef IdeIconDes ИМЯ_КЛАССА;

	IdeIconDes()                              { формат = 0; }
};

struct IdeIconEditPos : IconDes::ПозРедакт, Движимое<IdeIconEditPos> {
	Время filetime;

	IdeIconEditPos() { filetime = Null; }
};

ИНИЦИАЛИЗУЙ(IconDes);

#endif
