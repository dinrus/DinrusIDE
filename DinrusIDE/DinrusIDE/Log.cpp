#include "DinrusIDE.h"

#define METHOD_NAME "Иср::" << UPP_FUNCTION_NAME << "(): "

Ткст Иср::GetIdeLogPath()
{
	return GetStdLogPath();
}

Ткст Иср::GetTargetLogPath()
{
	if(target.дайСчёт() == 0)
		return Null;
#ifdef PLATFORM_WIN32
	return форсируйРасш(target, ".log");
#else
	Ткст title = дайТитулф(target);
	Ткст pp;
	Ткст h = дайПапкуФайла(target);
	while(h.дайСчёт() > 1 && дирЕсть(h)) {
		Ткст p = приставьИмяф(h, ".config");
		ФайлПоиск ff(p);
		if(ff && ff.папка_ли() && ff.записываемый()) {
			pp = p;
			break;
		}
		h = дайПапкуФайла(h);
	}
	if(пусто_ли(pp))
		pp = дайСреду("XDG_CONFIG_HOME");
	if(пусто_ли(pp) || !дирЕсть(pp))
		pp = дайФайлИзДомПапки(".config");
	pp << "/u++/" << title << '/' << title << ".log";
	return файлЕсть(pp) ? pp : дайФайлИзДомПапки(".upp/" + title + "/" + title + ".log");
#endif
}

void Иср::OpenLog(const Ткст& logFilePath)
{
	Ткст normalizedPath = нормализуйПуть(logFilePath);
	if(!designer && normalizedPath == editfile) {
		History(-1);
		return;
	}
	AddHistory();
	if(!файлЕсть(logFilePath)) {
		Loge() << METHOD_NAME << "Следующего лог-файла не существует: \"" << logFilePath << "\".";
		return;
	}
	
	EditFile(logFilePath);
}
