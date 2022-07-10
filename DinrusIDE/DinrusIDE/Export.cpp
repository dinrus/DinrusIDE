#include "DinrusIDE.h"

void Иср::ExportMakefile(const Ткст& ep)
{
	SaveMakeFile(приставьИмяф(ep, "Makefile"), true);
}

void Иср::ExportProject(const Ткст& ep, bool all, bool gui, bool deletedir)
{
	сохраниФайл(false);
	::РОбласть wspc;
	wspc.скан(main);
	Индекс<Ткст> used;
	HdependClearDependencies();
	for(int i = 0; i < wspc.дайСчёт(); i++) {
		const Пакет& p = wspc.дайПакет(i);
		Ткст pn = wspc[i];
		for(int j = 0; j < p.дайСчёт(); j++) {
			const Пакет::Файл& f = p[j];
			if(!f.separator) {
				Ткст p = SourcePath(pn, f);
				used.найдиДобавь(p);
				Вектор<Ткст> d = HdependGetDependencies(p);
				for(int q = 0; q < d.дайСчёт(); q++)
					used.найдиДобавь(d[q]);
				for(int q = 0; q < f.depends.дайСчёт(); q++)
					used.найдиДобавь(SourcePath(pn, f.depends[q].text));
			}
		}
	}
	if(файлЕсть(ep)) {
		if(gui && !PromptYesNo(DeQtf(ep) + " существующий файл.&"
		                "Хотите удалить его?")) return;
		удалифл(ep);
	}
	if(deletedir && дирЕсть(ep)) {
		if(gui && !PromptYesNo(DeQtf(ep) + " существубщая директория.&"
		                "Хотите заменить её?")) return;
		DeleteFolderDeep(ep);
	}

	Progress pi("Проект экспортируется");
	pi.устВсего(wspc.дайСчёт());
	for(int i = 0; i < wspc.дайСчёт(); i++) {
		if(gui && pi.StepCanceled())
			return;
		копируйПапку(приставьИмяф(ep, wspc[i]), PackageDirectory(wspc[i]), used, all, true);
	}
	Вектор<Ткст> upp = GetUppDirs();
	for(int i = 0; i < upp.дайСчёт(); i++) {
		if(gui && pi.StepCanceled())
			return;
		Ткст d = upp[i];
		ФайлПоиск ff(приставьИмяф(d, "*"));
		while(ff) {
			if(ff.файл_ли()) {
				Ткст фн = ff.дайИмя();
				Ткст path = приставьИмяф(d, фн);
				if(all || used.найди(path) >= 0)
					копируйФайл(приставьИмяф(ep, фн), path, true);
			}
			ff.следщ();
		}
		копируйПапку(приставьИмяф(ep, wspc[i]), PackageDirectory(wspc[i]), used, all, true);
	}
	ExportMakefile(ep);
}
