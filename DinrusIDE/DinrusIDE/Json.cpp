#include "DinrusIDE.h"

class JsonView : public ТопОкно {
	typedef JsonView ИМЯ_КЛАССА;

public:
	КтрлДерево              tree;
	СтрокРедакт              view;
	ФреймВерх<СтатичПрям>  errorbg;
	Надпись                 Ошибка;
	
public:
	JsonView();
	
	virtual bool Ключ(dword ключ, int count);
	virtual void закрой();

	void грузи(const Ткст& json);
	void CopyPath();

	void сериализуй(Поток& s);

private:
	void   переустанов();
	Ткст загрузи0(const Ткст& json);
	
	int AddNode(int parent_id, const Значение& id, const Ткст& имя, const Значение& v);
};

JsonView::JsonView()
{
	Титул("JSON view");
	Sizeable().Zoomable();
	Иконка(IdeCommonImg::json());
	
	добавь(tree.SizePos());
	добавь(view.SizePos());
	
	Ошибка.устШрифт(Arial(20)).устЧернила(красный);
	errorbg.устВысоту(25).добавь(Ошибка.SizePos());
	view.устТолькоЧтен();
	view.устЦвет(СтрокРедакт::PAPER_READONLY, SColorPaper());
	tree.устДисплей(QTFDisplay());
	tree.NoRoot();
	tree.WhenLeftDouble = THISBACK(CopyPath);
}

bool JsonView::Ключ(dword ключ, int count)
{
	if(ключ == K_ESCAPE) {
		закрой();
		return true;
	}
	return false;
}

void JsonView::грузи(const Ткст& json)
{
	переустанов();
	
	Ткст parsingError = загрузи0(json);
	if(parsingError.дайСчёт() > 0) {
		parsingError.уст(0, впроп(parsingError[0]));
		
		Ошибка = "Json parsing Ошибка: \"" + parsingError + "\".";
		добавьФрейм(errorbg);
		view.покажи();
		view <<= json;
		
		return;
	}
	
	tree.покажи();
	tree.устФокус();
}

void JsonView::CopyPath()
{
	int id = tree.дайКурсор();
	Ткст path;
	while(id) {
		Значение k = tree.дай(id);
		if(!пусто_ли(k)) {
			if(число_ли(k))
				path = "[" + какТкст(k) + "]" + path;
			if(ткст_ли(k))
				path = "[" + какТкстСи(Ткст(k)) + "]" + path;
		}
		id = tree.дайРодителя(id);
	}
	WriteClipboardText(path);
}

void JsonView::сериализуй(Поток& s)
{
	int version = 0;
	s / version;
	SerializePlacement(s);
}

void JsonView::закрой()
{
	сохраниВГлоб(*this, "JSONview");
	ТопОкно::закрой();
}

void JsonView::переустанов()
{
	удалиФрейм(errorbg);
	view.очисть();
	view.скрой();
	tree.очисть();
	tree.скрой();
}

Ткст JsonView::загрузи0(const Ткст& json)
{
	Ткст parsingError;
	
	СиПарсер p(json);
	try {
		tree.открой(AddNode(0, Null, "JSON", ParseJSON(p)));
	}
	catch(const СиПарсер::Ошибка& e) {
		parsingError << e;
	}
	
	return parsingError;
}

int JsonView::AddNode(int parent_id, const Значение& id, const Ткст& имя, const Значение& v)
{
	if(ошибка_ли(v)) {
		// TODO: замени with JsonExc or something that is more accurate in this situation.
		Ткст errorText = дайТекстОш(v);
		errorText.удали(0, errorText.найди(" ") + 1);
		throw Искл(errorText);
	}
	else
	if(v.является<МапЗнач>()) {
		МапЗнач m = v;
		parent_id = tree.добавь(parent_id, IdeCommonImg::JsonStruct(), id, "[G1 [* " + имя);
		for(int i = 0; i < m.дайСчёт(); i++)
			AddNode(parent_id, m.дайКлюч(i), "[@B \1" + Ткст(m.дайКлюч(i)) + "\1:]", m.дайЗначение(i));
	}
	else
	if(v.является<МассивЗнач>()) {
		parent_id = tree.добавь(parent_id, IdeCommonImg::JsonArray(), id, "[G1 [* " + имя);
		for(int i = 0; i < v.дайСчёт(); i++)
			AddNode(parent_id, i, "[@c " + какТкст(i) + ":]", v[i]);
	}
	else {
		Ткст qtf = "[G1 [* " + имя + "]";
		Рисунок img = IdeCommonImg::JsonNumber();
		if(ткст_ли(v)) {
			img = IdeCommonImg::JsonString();
			if(пусто_ли(v))
				qtf << "[*@g  Null";
			else
				qtf << "[@r \1 " + какТкстСи(Ткст(v));
		}
		else {
			if(v.является<bool>())
				img = IdeCommonImg::JsonBool();
			if(пусто_ли(v))
				qtf << "[*@g  Null";
			else
				qtf << "\1 " + какТкст(v);
		}
		parent_id = tree.добавь(parent_id, img, id, qtf);
	}
	return parent_id;
}

void Иср::Json()
{
	static JsonView dlg;
	dlg.грузи(editor.выделение_ли() ? editor.дайВыделение() : editor.дай());
	if(!dlg.открыт()) {
		грузиИзГлоба(dlg, "JSONview");
		dlg.откройГлавн();
	}
	else
		dlg.устПП();
}
