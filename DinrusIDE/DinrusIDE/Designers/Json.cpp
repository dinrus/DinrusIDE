#include "Designers.h"

JsonViewDes::JsonViewDes()
{
	tree.устДисплей(QTFDisplay());
}

void JsonViewDes::CopyPath()
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

Ткст JsonViewDes::загрузи0(const Ткст& json)
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

int JsonViewDes::AddNode(int parent_id, const Значение& id, const Ткст& имя, const Значение& v)
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

struct JsonDesModule : public МодульИСР {
	virtual Ткст       GetID() { return "JsonDesModule"; }

	virtual bool         AcceptsFile(const char *path) {
		return впроп(дайРасшф(path)) == ".json";
	}

	virtual IdeDesigner *CreateDesigner(const char *path, byte) {
		if(!AcceptsFile(path))
			return NULL;
		JsonViewDes *d = new JsonViewDes;
		if(d->грузи(path))
			return d;
		delete d;
		return NULL;
	}
};

ИНИЦИАЛИЗАТОР(JsonViewDes)
{
	регМодульИСР(Single<JsonDesModule>());
}
