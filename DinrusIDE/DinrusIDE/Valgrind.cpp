#include "DinrusIDE.h"

bool Иср::IsValgrind()
{
	static bool is;
	ONCELOCK {
		is = system("which valgrind") == 0;
	}
	return is;
}

void Иср::Valgrind()
{
	if(!IsValgrind())
		return;
	if(!Build())
		return;
	Хост h;
	CreateHostRunDir(h);
	h.ChDir(Nvl(rundir, дайПапкуФайла(target)));
	Ткст cmdline;
	Ткст фн = дайВремИмяф();
	cmdline << "valgrind --xml=yes --num-callers=40 --xml-file=" << фн << ' ';
	Ткст ValgSupp = конфигФайл("valgrind.supp");
	if(!пусто_ли(загрузиФайл(ValgSupp)))
		cmdline << "--suppressions=" << ValgSupp << ' ';
	cmdline << '\"' << target << "\" ";
	cmdline << runarg;
	ConsoleClear();
	вКонсоль("Valgrind..");
	if(пусто_ли(h.выполни(cmdline))) {
		вКонсоль("Ошибка executing valgrind");
		return;
	}
	вКонсоль("Parsing valgrind output..");
	синх();
	Ткст txt = загрузиФайл(фн);
	DeleteFile(фн);
	
	ВекторМап<Ткст, bool> file_exists;
	auto FileExistsCached = [&] (const Ткст& s) {
		int ii = file_exists.найди(s);
		if(ii < 0) {
			ii = file_exists.дайСчёт();
			file_exists.добавь(s, файлЕсть(s));
		}
		return file_exists[ii];
	};

	ClearErrorsPane();
	
	try {
		ПарсерРяр p(txt);
		while(!p.тэг_ли())
			p.пропусти();
		p.передайТэг("valgrindoutput");
		while(!p.стоп()) {
			if(p.Тэг("Ошибка")) {
				Ткст hdr = "Ошибка (missing description)";
				МассивЗнач notes;
				Ткст     path;
				int        lineno = Null;
				while(!p.стоп()) {
					if(p.Тэг("what")) {
						hdr = p.читайТекст();
						p.пропустиКонец();
					}
					else
					if(p.Тэг("stack")) {
						while(!p.стоп()) {
							Ткст obj;
							Ткст фн;
							Ткст dir;
							Ткст file;
							int    line = Null;
							if(p.Тэг("frame")) {
								while(!p.стоп()) {
									if(p.Тэг("obj")) {
										obj = p.читайТекст();
										p.пропустиКонец();
									}
									else
									if(p.Тэг("фн")) {
										фн = p.читайТекст();
										p.пропустиКонец();
									}
									else
									if(p.Тэг("dir")) {
										dir = p.читайТекст();
										p.пропустиКонец();
									}
									else
									if(p.Тэг("file")) {
										file = p.читайТекст();
										p.пропустиКонец();
									}
									else
									if(p.Тэг("line")) {
										line = тктЦел(p.читайТекст());
										p.пропустиКонец();
									}
									else
										p.пропусти();
								}
								if(dir.дайСчёт() && file.дайСчёт() && !пусто_ли(line)) {
									ErrorInfo f;
									Ткст p = приставьИмяф(dir, file);
									f.lineno = Null;
									if(FileExistsCached(p)) {
										f.file = p;
										f.lineno = line;
										if(пусто_ли(path)) {
											path = p;
											lineno = line;
										}
									}
									f.linepos = 0;
									f.kind = 0;
									f.message << фн << ' ' << obj;
									notes.добавь(RawToValue(f));
								}
							}
							else
								p.пропусти();
						}
					}
					else
						p.пропусти();
				}
				ErrorInfo f;
				f.file = path;
				f.lineno = lineno;
				f.linepos = 0;
				f.kind = 0;
				f.message = hdr;
				int linecy;
				Ошибка.добавь(f.file, f.lineno,
				          AttrText(FormatErrorLine(f.message, linecy)).NormalPaper(HighlightSetup::GetHlStyle(HighlightSetup::PAPER_ERROR).color),
				          RawToValue(f));
				Ошибка.уст(Ошибка.дайСчёт() - 1, "ЗАМЕТКИ", notes);
				Ошибка.SetLineCy(Ошибка.дайСчёт() - 1, linecy);
			}
			else
				p.пропусти();
		}
	}
	catch(ОшибкаРяр) {
		вКонсоль("Ошибка parsing valgrind output");
		return;
	}
	if(Ошибка.дайСчёт()) {
		вКонсоль("Готово.");
		устНиз(BERRORS);
	}
	else
		вКонсоль("Done - no errors found.");
}
