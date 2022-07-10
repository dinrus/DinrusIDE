#include "Designers.h"

struct IdeQtfEditPos : Движимое<IdeQtfEditPos> {
	Время               filetime;
	RichEdit::PosInfo  pos;
	RichEdit::UndoInfo uf;

	IdeQtfEditPos()           { filetime = Null; }
};

static ВекторМап<Ткст, IdeQtfEditPos>& sEP()
{
	static ВекторМап<Ткст, IdeQtfEditPos> x;
	return x;
}

void IdeQtfDes::SaveEditPos()
{
	if(filename.дайСчёт()) {
		IdeQtfEditPos& p = sEP().дайДобавь(filename);
		p.filetime = дайВремяф(filename);;
		p.pos = GetPosInfo();
		p.uf = PickUndoInfo();
	}
}

bool IdeQtfDes::грузи(const char *_filename)
{
	RichText txt;
	if(ParseQTF(txt, загрузиФайл(_filename))) {
		filename = _filename;
		подбери(pick(txt));
		IdeQtfEditPos& ep = sEP().дайДобавь(filename);
		if(ep.filetime == дайВремяф(filename)) {
			SetPosInfo(ep.pos);
			SetPickUndoInfo(pick(ep.uf));
			ep.uf.очисть();
		}
		return true;
	}
	return false;
}

void IdeQtfDes::сохрани()
{
	сохраниФайл(filename, GetQTF());
}

void IdeQtfDes::EditMenu(Бар& menu)
{
	EditTools(menu);
}

void IdeQtfDes::сериализуй(Поток& s)
{
	сериализуйНастройки(s);
}

void SerializeQtfDesPos(Поток& s)
{
	ВекторМап<Ткст, IdeQtfEditPos>& filedata = sEP();
	s.Magic();
	s.Magic(0);
	if(s.сохраняется()) {
		for(int i = 0; i < filedata.дайСчёт(); i++) {
			Ткст фн = filedata.дайКлюч(i);
			if(!фн.пустой()) {
				ФайлПоиск ff(фн);
				IdeQtfEditPos& ep = filedata[i];
				if(ff && ff.дайВремяПоследнЗаписи() == ep.filetime) {
					s % фн;
					s % ep.filetime;
					s % ep.pos;
				}
			}
		}
		Ткст end;
		s % end;
	}
	else {
		Ткст фн;
		filedata.очисть();
		for(;;) {
			s % фн;
			if(фн.пустой()) break;
			IdeQtfEditPos& ep = filedata.дайДобавь(фн);
			s % ep.filetime;
			s % ep.pos;
		}
	}
}

bool IsQtfFile(const char *path)
{
	return впроп(дайРасшф(path)) == ".qtf";
}

struct QtfDesModule : public МодульИСР {
	virtual Ткст       GetID() { return "QtfDesModule"; }
	virtual void CleanUsc() {}
	virtual bool ParseUsc(СиПарсер& p) { return false; }

	virtual Рисунок FileIcon(const char *path) {
		return IsQtfFile(path) ? IdeCommonImg::Qtf() : Null;
	}

	virtual IdeDesigner *CreateDesigner(const char *path, byte) {
		if(IsQtfFile(path)) {
			IdeQtfDes *d = new IdeQtfDes;
			грузиИзГлоба(*d, "qtfdes-ctrl");
			if(d->грузи(path)) {
				return d;
			}
			delete d;
			return NULL;
		}
		return NULL;
	}

	virtual void сериализуй(Поток& s) {
		int version = 0;
		s / version;
		SerializeQtfDesPos(s);
	}
};

ИНИЦИАЛИЗАТОР(Qtf)
{
	регМодульИСР(Single<QtfDesModule>());
	региструйГлобКонфиг("qtfdes-ctrl");
}
