#include "IconDes.h"

static ВекторМап<Ткст, IdeIconEditPos>& sEP()
{
	static ВекторМап<Ткст, IdeIconEditPos> x;
	return x;
}

void IdeIconDes::SaveEditPos()
{
	IdeIconEditPos& ep = sEP().дайДобавь(filename);
	ep.filetime = filetime;
	(ПозРедакт&)ep = GetEditPos();
}

void SerializeIconDesPos(Поток& s)
{
	ВекторМап<Ткст, IdeIconEditPos>& filedata = sEP();
	if(s.сохраняется()) {
		for(int i = 0; i < filedata.дайСчёт(); i++) {
			Ткст фн = filedata.дайКлюч(i);
			if(!фн.пустой()) {
				ФайлПоиск ff(фн);
				IdeIconEditPos& ep = filedata[i];
				if(ff && ff.дайВремяПоследнЗаписи() == ep.filetime) {
					s % фн;
					s % ep.filetime;
					s % ep.cursor;
					s % ep.sc;
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
			IdeIconEditPos& ep = filedata.дайДобавь(фн);
			s % ep.filetime;
			s % ep.cursor;
			s % ep.sc;
		}
	}
}

void IdeIconDes::RestoreEditPos()
{
	IdeIconEditPos& ep = sEP().дайДобавь(filename);
	if(ep.filetime == filetime)
		SetEditPos(ep);
}

bool IsImlFile(const char *path)
{
	return впроп(дайРасшф(path)) == ".iml";
}

struct IconDesModule : public МодульИСР {
	virtual Ткст       GetID() { return "IconDesModule"; }
	virtual void CleanUsc() {}
	virtual bool ParseUsc(СиПарсер& p) { return false; }
	virtual Рисунок FileIcon(const char *path) {
		return IsImlFile(path) ? IconDesImg::FileIcon() : Null;
	}
	virtual IdeDesigner *CreateDesigner(const char *path, byte) {
		if(IsImlFile(path)) {
			IdeIconDes *d = new IdeIconDes;
			грузиИзГлоба(*d, "icondes-ctrl");
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
		SerializeIconDesPos(s);
	}
};

void RegisterIconDes()
{
	регМодульИСР(Single<IconDesModule>());
}

void IdeIconDes::CopyId(const Ткст& n)
{
	ClearClipboard();
	AppendClipboardText(n);
}

void IdeIconDes::найдиИд(const Ткст& id)
{
	FindName(id);
}

void IdeIconDes::ListMenuEx(Бар& bar)
{
	if(IsSingleMode())
		return;
	Ткст n = GetCurrentName();
	Ткст c = дайТитулф(filename);
	c.уст(0, взаг(c[0]));
	c = c.заканчиваетсяНа("Img") ? c : c + "Img";
	c << "::" << n << "()";
	bar.Separator();
	bar.добавь(n.дайСчёт(), "Копировать '" + c + '\'', CtrlImg::copy(), THISBACK1(CopyId, c));
}

ИНИЦИАЛИЗАТОР(IconDes) {
	региструйГлобКонфиг("icondes-ctrl");
}
