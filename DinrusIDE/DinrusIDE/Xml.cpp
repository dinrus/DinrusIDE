#include "DinrusIDE.h"

class XmlView : public ТопОкно {
	typedef XmlView ИМЯ_КЛАССА;

public:
	КтрлДерево              xml;
	СтрокРедакт              view;
	ФреймВерх<СтатичПрям>  errorbg;
	Надпись                 Ошибка;
	КтрлРодитель            data;
	
	XmlView();
	
	virtual bool Ключ(dword ключ, int count);
	virtual void закрой();

	void грузи(const Ткст& txt);
	void CopyPath();

	void сериализуй(Поток& s);

private:
	Ткст загрузи0(ПарсерРяр& p);
	void   загрузи0(int parent, ПарсерРяр& p);
	
	void   переустанов();
};

bool XmlView::Ключ(dword ключ, int count)
{
	if(ключ == K_ESCAPE) {
		закрой();
		return true;
	}
	return false;
}

XmlView::XmlView()
{
	Титул("XML view");
	Sizeable().Zoomable();
	Иконка(IdeCommonImg::xml());

	xml.NoRoot();

	Ошибка.устШрифт(Arial(20)).устЧернила(красный);
	errorbg.устВысоту(25).добавь(Ошибка.SizePos());
	view.устТолькоЧтен();
	view.устЦвет(СтрокРедакт::PAPER_READONLY, SColorPaper());

	добавь(xml.SizePos());
	добавь(view.SizePos());

	xml.WhenLeftDouble = THISBACK(CopyPath);
}

void XmlView::грузи(const Ткст& txt)
{
	переустанов();
	
	ПарсерРяр p(txt);
	Ткст parsingError = загрузи0(p);
	if(parsingError.дайСчёт() > 0) {
		parsingError.уст(0, впроп(parsingError[0]));
		
		Ошибка = "XML parsing Ошибка: " + parsingError + ".";
		добавьФрейм(errorbg);
		view.покажи();
		view <<= txt;
		view.устКурсор(view.дайПоз64(p.дайСтроку() - 1, p.дайКолонку() - 1));
		view.устФокус();
		return;
	}
	
	xml.покажи();
	xml.устФокус();
}

void XmlView::сериализуй(Поток& s)
{
	int version = 0;
	s / version;
	SerializePlacement(s);
}

void XmlView::CopyPath()
{
	Ткст path;
	int id = xml.дайКурсор();
	while(id >= 0) {
		Ткст tag = xml.дай(id);
		if(tag.дайСчёт())
			path = "[" + какТкстСи(tag) + "]" + path;
		id = xml.дайРодителя(id);
	}
	WriteClipboardText(path);
}

void XmlView::закрой()
{
	сохраниВГлоб(*this, "XML view");
	ТопОкно::закрой();
}

Ткст XmlView::загрузи0(ПарсерРяр& p) {
	Ткст parsingError;
	
	try {
		while(!p.кф_ли())
			загрузи0(0, p);
	}
	catch(const ОшибкаРяр& e) {
		parsingError = e;
	}
	
	if(!parsingError.дайСчёт() && !xml.GetChildCount(0))
		parsingError = "Not found any XML tags";
	
	return parsingError;
}

void XmlView::загрузи0(int parent, ПарсерРяр& p)
{
	if(p.тэг_ли()) {
		Ткст tag = p.читайТэг();
		Ткст txt = tag;
		for(int i = 0; i < p.дайСчётАтров(); i++)
			txt << ' ' << p.дайАтр(i) << "=\"" << p[i] << "\"";
		parent = xml.добавь(parent, IdeCommonImg::XmlTag(), tag, txt);
		while(!p.стоп()) {
			if(p.кф_ли())
				throw ОшибкаРяр("Unexpected end of text");
			загрузи0(parent, p);
		}
	}
	else
	if(p.текст_ли())
		xml.добавь(parent, IdeCommonImg::XmlText(), Null, нормализуйПробелы(p.читайТекст()));
	else
	if(p.IsPI())
		xml.добавь(parent, IdeCommonImg::XmlPI(), Null, нормализуйПробелы(p.ReadPI()));
	else
	if(p.декл_ли())
		xml.добавь(parent, IdeCommonImg::XmlDecl(), Null, нормализуйПробелы(p.читайДекл()));
	else
	if(p.коммент_ли())
		xml.добавь(parent, IdeCommonImg::XmlComment(), Null, нормализуйПробелы(p.читайКоммент()));
	else
		throw ОшибкаРяр("Unexpected input");
}

void XmlView::переустанов()
{
	удалиФрейм(errorbg);
	
	xml.очисть();
	xml.скрой();
	view.очисть();
	view.скрой();
}

void Иср::Xml()
{
	static XmlView dlg;
	dlg.грузи(editor.выделение_ли() ? editor.дайВыделение() : editor.дай());
	if(!dlg.открыт()) {
		грузиИзГлоба(dlg, "XMLview");
		dlg.откройГлавн();
	}
	else
		dlg.устПП();
}
