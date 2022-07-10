#include "Designers.h"

void XmlViewDes::CopyPath()
{
	Ткст path;
	int id = tree.дайКурсор();
	while(id >= 0) {
		Ткст tag = tree.дай(id);
		if(tag.дайСчёт())
			path = "[" + какТкстСи(tag) + "]" + path;
		id = tree.дайРодителя(id);
	}
	WriteClipboardText(path);
}

Ткст XmlViewDes::загрузи0(const Ткст& data) {
	ПарсерРяр p(data);

	Ткст parsingError;
	
	try {
		while(!p.кф_ли())
			загрузи0(0, p);
	}
	catch(const ОшибкаРяр& e) {
		parsingError = e;
	}
	
	if(!parsingError.дайСчёт() && !tree.GetChildCount(0))
		parsingError = "Не найдено никаких тегов XML";
	
	return parsingError;
}

void XmlViewDes::загрузи0(int parent, ПарсерРяр& p)
{
	if(p.тэг_ли()) {
		Ткст tag = p.читайТэг();
		Ткст txt = tag;
		for(int i = 0; i < p.дайСчётАтров(); i++)
			txt << ' ' << p.дайАтр(i) << "=\"" << p[i] << "\"";
		parent = tree.добавь(parent, IdeCommonImg::XmlTag(), tag, txt);
		while(!p.стоп()) {
			if(p.кф_ли())
				throw ОшибкаРяр("Неожиданный конец текста");
			загрузи0(parent, p);
		}
	}
	else
	if(p.текст_ли())
		tree.добавь(parent, IdeCommonImg::XmlText(), Null, нормализуйПробелы(p.читайТекст()));
	else
	if(p.IsPI())
		tree.добавь(parent, IdeCommonImg::XmlPI(), Null, нормализуйПробелы(p.ReadPI()));
	else
	if(p.декл_ли())
		tree.добавь(parent, IdeCommonImg::XmlDecl(), Null, нормализуйПробелы(p.читайДекл()));
	else
	if(p.коммент_ли())
		tree.добавь(parent, IdeCommonImg::XmlComment(), Null, нормализуйПробелы(p.читайКоммент()));
	else
		throw ОшибкаРяр("Неожиданный ввод");
}

struct XmlDesModule : public МодульИСР {
	virtual Ткст       GetID() { return "XmlDesModule"; }

	virtual bool         AcceptsFile(const char *path) {
		return впроп(дайРасшф(path)) == ".xml";
	}

	virtual IdeDesigner *CreateDesigner(const char *path, byte) {
		if(!AcceptsFile(path))
			return NULL;
		XmlViewDes *d = new XmlViewDes;
		if(d->грузи(path))
			return d;
		delete d;
		return NULL;
	}
};

ИНИЦИАЛИЗАТОР(XmlViewDes)
{
	регМодульИСР(Single<XmlDesModule>());
}
