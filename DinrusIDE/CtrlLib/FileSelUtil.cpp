#include "CtrlLib.h"

namespace РНЦП {

FileSel& GFileSel()
{
	static FileSel fs;
	ONCELOCK {
		fs.ActiveDir(дайДомПапку());
	}
	return fs;
}

ИНИЦБЛОК {
	региструйГлобКонфиг("GlobalFileSelector");
}

Ткст SelectFileOpen(const char *types)
{
	FileSel& fs = GFileSel();
	грузиИзГлоба(fs, "GlobalFileSelector");
	fs.ClearTypes();
	fs.Types(types);
	bool b = fs.ExecuteOpen();
	сохраниВГлоб(fs, "GlobalFileSelector");
	return b ? ~fs : Ткст::дайПроц();
}

Ткст SelectFileSaveAs(const char *types)
{
	FileSel& fs = GFileSel();
	грузиИзГлоба(fs, "GlobalFileSelector");
	fs.ClearTypes();
	fs.Types(types);
	bool b = fs.ExecuteSaveAs();
	сохраниВГлоб(fs, "GlobalFileSelector");
	return b ? ~fs : Ткст::дайПроц();
}

Ткст SelectDirectory()
{
	FileSel fs;
	fs.ActiveDir(дайДомПапку());
	грузиИзГлоба(fs, "GlobalDirSelector");
	bool b = fs.ExecuteSelectDir();
	сохраниВГлоб(fs, "GlobalDirSelector");
	return b ? ~fs : Ткст::дайПроц();
}

SelectFileIn::SelectFileIn(const char *types)
{
	for(;;) {
		Ткст p = SelectFileOpen(types);
		if(p.дайСчёт() == 0 || открой(p))
			return;
		Exclamation(t_("Не удаётся открыть [* \1") + p + t_("\1] для чтения!"));
	}
}

SelectFileOut::SelectFileOut(const char *types)
{
	for(;;) {
		Ткст p = SelectFileSaveAs(types);
		if(p.дайСчёт() == 0 || открой(p))
			return;
		Exclamation(t_("Не удаётся открыть [* \1") + p + t_("\1] для записи!"));
	}
}

Ткст SelectLoadFile(const char *types)
{
	Ткст p = SelectFileOpen(types);
	return p.дайСчёт() ? загрузиФайл(p) : Ткст::дайПроц();
}

bool SelectSaveFile(const char *types, const Ткст& data)
{
	Ткст p = SelectFileSaveAs(types);
	if(p.дайСчёт() == 0)
		return false;
	if(!сохраниФайл(p, data)) {
		Exclamation(t_("Ошибка при сохранении файла!"));
		return false;
	}
	return true;
}

}
