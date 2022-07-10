#include "Designers.h"
#include <plugin/md/Markdown.h>

struct IdeMDEditPos : Движимое<IdeMDEditPos> {
	Время               filetime = Null;
	СтрокРедакт::ПозРедакт  editpos;
	СтрокРедакт::ОтмениДанные undodata;
};

static ВекторМап<Ткст, IdeMDEditPos>& sEPmd()
{
	static ВекторМап<Ткст, IdeMDEditPos> x;
	return x;
}

IdeMDDes::IdeMDDes()
{
	editor << [=] { delay.глушиУст(250, [=] { Preview(); }); };
	splitter.гориз(editor, preview);
	добавь(splitter.SizePos());
	if(TheIde())
		editor.устШрифт(((Иср *)TheIde())->editorfont);
}

void IdeMDDes::Preview()
{
	int sc = preview.GetSb();
	preview <<= MarkdownConverter().Tables().ToQtf(editor.дай());
	preview.SetSb(sc);
}

void IdeMDDes::SaveEditPos()
{
	if(filename.дайСчёт()) {
		IdeMDEditPos& p = sEPmd().дайДобавь(filename);
		p.filetime = дайВремяф(filename);;
		p.undodata = editor.PickUndoData();
		p.editpos = editor.GetEditPos();
	}
}

bool IdeMDDes::грузи(const char *filename_)
{
	filename = filename_;
	ФайлВвод in(filename);
	if(in) {
		editor.грузи(in, CHARSET_UTF8);
		IdeMDEditPos& ep = sEPmd().дайДобавь(filename);
		if(ep.filetime == дайВремяф(filename)) {
			editor.SetEditPos(ep.editpos);
			editor.SetPickUndoData(pick(ep.undodata));
		}
		Preview();
		return true;
	}
	return false;
}

void IdeMDDes::сохрани()
{
	ФайлВывод out(filename);
	editor.сохрани(out, CHARSET_UTF8);
}

void IdeMDDes::EditMenu(Бар& menu)
{
//	EditTools(menu);
}

void IdeMDDes::сфокусирован()
{
	editor.устФокус();
}

void IdeMDDes::сериализуй(Поток& s)
{
}

void SerializeMDDesPos(Поток& s)
{
	ВекторМап<Ткст, IdeMDEditPos>& filedata = sEPmd();
	s.Magic();
	s.Magic(0);
	if(s.сохраняется()) {
		for(int i = 0; i < filedata.дайСчёт(); i++) {
			Ткст фн = filedata.дайКлюч(i);
			if(!фн.пустой()) {
				ФайлПоиск ff(фн);
				IdeMDEditPos& ep = filedata[i];
				if(ff && ff.дайВремяПоследнЗаписи() == ep.filetime) {
					s % фн;
					s % ep.filetime;
					s % ep.editpos;
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
			IdeMDEditPos& ep = filedata.дайДобавь(фн);
			s % ep.filetime;
			s % ep.editpos;
		}
	}
}

bool IsMDFile(const char *path)
{
	return впроп(дайРасшф(path)) == ".md";
}

struct MDDesModule : public МодульИСР {
	virtual Ткст       GetID() { return "MDDesModule"; }
	virtual void CleanUsc() {}
	virtual bool ParseUsc(СиПарсер& p) { return false; }

	virtual Рисунок FileIcon(const char *path) {
		return IsMDFile(path) ? IdeCommonImg::MD() : Null;
	}

	virtual IdeDesigner *CreateDesigner(const char *path, byte) {
		if(IsMDFile(path)) {
			IdeMDDes *d = new IdeMDDes;
			грузиИзГлоба(*d, "mddes-ctrl");
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
		SerializeMDDesPos(s);
	}
};

ИНИЦИАЛИЗАТОР(MD)
{
	регМодульИСР(Single<MDDesModule>());
	региструйГлобКонфиг("mddes-ctrl");
}
