#include "Designers.h"

void IdePngDes::сохрани()
{
	if(дайСчёт())
		SaveChangedFileFinish(filename, PNGEncoder().SaveString(GetImage(0)));
	filetime = дайВремяф(filename);
}

void IdePngDes::грузи(const char *_filename)
{
	очисть();
	filename = _filename;
	filetime = дайВремяф(filename);
	Рисунок m = StreamRaster::LoadFileAny(filename);
	добавьРисунок(filename, m, false);
	SingleMode();
}

void IdePngDes::создай(const char *_filename)
{
	очисть();
	filename = _filename;
	filetime = дайСисВремя();
	Рисунок m = CreateImage(Размер(16, 16), Null);
	добавьРисунок(filename, m, false);
	SingleMode();
}
