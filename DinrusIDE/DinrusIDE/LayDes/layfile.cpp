#include "LayDes.h"

МассивМап<Ткст, LayDesEditPos>& LayFileDataVar()
{
	static МассивМап<Ткст, LayDesEditPos> x;
	return x;
}

void SerializeLayEditPos(Поток& s)
{
	МассивМап<Ткст, LayDesEditPos>& filedata = LayFileDataVar();
	int dummy = 0;
	if(s.сохраняется()) {
		for(int i = 0; i < filedata.дайСчёт(); i++) {
			Ткст фн = filedata.дайКлюч(i);
			if(!фн.пустой()) {
				ФайлПоиск ff(фн);
				LayDesEditPos& ep = filedata[i];
				if(ff && ff.дайВремяПоследнЗаписи() == ep.filetime) {
					s % фн;
					s % ep.filetime;
					s % ep.layouti;
					s % dummy;
				}
			}
		}
		Ткст h;
		s % h;
	}
	else {
		Ткст фн;
		filedata.очисть();
		for(;;) {
			s % фн;
			if(фн.пустой()) break;
			LayDesEditPos& ep = filedata.дайДобавь(фн);
			ep.undo.очисть();
			ep.redo.очисть();
			ep.cursor.очисть();
			s % ep.filetime;
			s % ep.layouti;
			s % dummy;
		}
	}

}

void LayDes::SaveEditPos()
{
	LayDesEditPos& p = LayFileDataVar().дайДобавь(filename);
	p.filetime = filetime;
	p.undo.устСчёт(layout.дайСчёт());
	p.redo.устСчёт(layout.дайСчёт());
	for(int i = 0; i < layout.дайСчёт(); i++) {
		p.undo[i] = pick(layout[i].undo);
		p.redo[i] = pick(layout[i].redo);
	}
	p.layouti = list.дайКлюч();
	p.cursor = pick(cursor);
}

void LayDes::RestoreEditPos()
{
	if(пусто_ли(filetime)) return;
	int q = LayFileDataVar().найди(filename);
	if(q < 0)
		return;
	LayDesEditPos& p = LayFileDataVar()[q];
	if(p.filetime != filetime) return;
	if(layout.дайСчёт() == p.undo.дайСчёт()) {
		for(int i = 0; i < layout.дайСчёт(); i++) {
			layout[i].undo = pick(p.undo[i]);
			layout[i].redo = pick(p.redo[i]);
		}
	}
	if(p.layouti >= 0 && p.layouti < layout.дайСчёт()) {
		GoTo(p.layouti);
		LayoutCursor();
	}
	cursor = pick(p.cursor);
	SyncItems();
}

void LayDes::FindLayout(const Ткст& имя, const Ткст& item_name)
{
	for(int i = 0; i < layout.дайСчёт(); i++)
		if(layout[i].имя == имя) {
			GoTo(i);
			if(!пусто_ли(item_name)) {
				int q = элт.найди(item_name, 1);
				if(q >= 0)
					выделиОдин(q, 0);
			}
		}
}

Ткст LayDes::GetLayoutName() const
{
	return currentlayout >= 0 && currentlayout < layout.дайСчёт() ? layout[currentlayout].имя
	                                                               : Ткст();
}

bool LayDes::грузи(const char *file, byte _charset)
{
	charset = _charset;
	if(charset == CHARSET_UTF8_BOM)
		charset = CHARSET_UTF8;
	layout.очисть();
	filename = file;
	ФайлВвод in(file);
	if(in) {
		layfile = загрузиПоток(in);
		newfile = false;
		filetime = in.дайВремя();
		fileerror.очисть();
		EOL = GetLineEndings(layfile);
		try {
			СиПарсер p(layfile);
			if(p.сим('#') && p.ид("ifdef")) {
				if(!p.ид("LAYOUTFILE"))
					p.ид("LAYOUT");
			}
			while(!p.кф_ли()) {
				LayoutData& ld = layout.добавь();
				ld.устНабсим(charset);
				ld.читай(p);
				if(p.сим('#'))
					p.ид("endif");
			}
		}
		catch(СиПарсер::Ошибка& e) {
			вКонсоль(file + e);
			return false;
		}
	}
	else {
		newfile = true;
		filetime = Null;
		EOL = "\r\n";
	}
	search <<= Null;
	SyncLayoutList();
	if(list.дайСчёт()) {
		list.устКурсор(0);
		LayoutCursor();
	}
	RestoreEditPos();
	return true;
}

void LayDes::сохрани()
{
	сохраниВГлоб(*this, "laydes-ctrl");

	if(!пусто_ли(fileerror))
		return;
	Ткст r;
	for(int i = 0; i < layout.дайСчёт(); i++) {
		layout[i].устНабсим(charset);
		r << layout[i].сохрани(0, EOL) << EOL;
	}
	layfile = r;
	if(!SaveChangedFileFinish(filename, r))
		return;
	ФайлПоиск ff(filename);
	if(ff) {
		newfile = false;
		filetime = ff.дайВремяПоследнЗаписи();
	}
}
