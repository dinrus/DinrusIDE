#include "DinrusIDE.h"

bool Ide::IsValgrind()
{
	static bool is;
	ONCELOCK {
		is = system("which valgrind") == 0;
	}
	return is;
}

void Ide::Valgrind()
{
	if(!IsValgrind())
		return;
	if(!Build())
		return;
	Host h;
	CreateHostRunDir(h);
	h.ChDir(Nvl(rundir, GetFileFolder(target)));
	String cmdline;
	String fn = GetTempFileName();
	cmdline << "valgrind --xml=yes --num-callers=40 --xml-file=" << fn << ' ';
	String ValgSupp = ConfigFile("valgrind.supp");
	if(!IsNull(LoadFile(ValgSupp)))
		cmdline << "--suppressions=" << ValgSupp << ' ';
	cmdline << '\"' << target << "\" ";
	cmdline << runarg;
	ConsoleClear();
	PutConsole("Valgrind..");
	if(IsNull(h.Execute(cmdline))) {
		PutConsole("Ошибка при выполнении valgrind");
		return;
	}
	PutConsole("Разбор вывода от valgrind..");
	Sync();
	String txt = LoadFile(fn);
	DeleteFile(fn);
	
	VectorMap<String, bool> file_exists;
	auto FileExistsCached = [&] (const String& s) {
		int ii = file_exists.Find(s);
		if(ii < 0) {
			ii = file_exists.GetCount();
			file_exists.Add(s, FileExists(s));
		}
		return file_exists[ii];
	};

	ClearErrorsPane();
	
	try {
		XmlParser p(txt);
		while(!p.IsTag())
			p.Skip();
		p.PassTag("valgrindoutput");
		while(!p.End()) {
			if(p.Tag("error")) {
				String hdr = "Ошибка (описание отсутствует)";
				ValueArray notes;
				String     path;
				int        lineno = Null;
				while(!p.End()) {
					if(p.Tag("what")) {
						hdr = p.ReadText();
						p.SkipEnd();
					}
					else
					if(p.Tag("stack")) {
						while(!p.End()) {
							String obj;
							String fn;
							String dir;
							String file;
							int    line = Null;
							if(p.Tag("frame")) {
								while(!p.End()) {
									if(p.Tag("obj")) {
										obj = p.ReadText();
										p.SkipEnd();
									}
									else
									if(p.Tag("fn")) {
										fn = p.ReadText();
										p.SkipEnd();
									}
									else
									if(p.Tag("dir")) {
										dir = p.ReadText();
										p.SkipEnd();
									}
									else
									if(p.Tag("file")) {
										file = p.ReadText();
										p.SkipEnd();
									}
									else
									if(p.Tag("line")) {
										line = StrInt(p.ReadText());
										p.SkipEnd();
									}
									else
										p.Skip();
								}
								if(dir.GetCount() && file.GetCount() && !IsNull(line)) {
									ErrorInfo f;
									String p = AppendFileName(dir, file);
									f.lineno = Null;
									if(FileExistsCached(p)) {
										f.file = p;
										f.lineno = line;
										if(IsNull(path)) {
											path = p;
											lineno = line;
										}
									}
									f.linepos = 0;
									f.kind = 0;
									f.message << fn << ' ' << obj;
									notes.Add(RawToValue(f));
								}
							}
							else
								p.Skip();
						}
					}
					else
						p.Skip();
				}
				ErrorInfo f;
				f.file = path;
				f.lineno = lineno;
				f.linepos = 0;
				f.kind = 0;
				f.message = hdr;
				int linecy;
				error.Add(f.file, f.lineno,
				          AttrText(FormatErrorLine(f.message, linecy)).NormalPaper(HighlightSetup::GetHlStyle(HighlightSetup::PAPER_ERROR).color),
				          RawToValue(f));
				error.Set(error.GetCount() - 1, "ЗАМЕТКИ", notes);
				error.SetLineCy(error.GetCount() - 1, linecy);
			}
			else
				p.Skip();
		}
	}
	catch(XmlError) {
		PutConsole("Ошибка при разборе вывода от valgrind");
		return;
	}
	if(error.GetCount()) {
		PutConsole("Готово.");
		SetBottom(BERRORS);
	}
	else
		PutConsole("Готово - ошибок не найдено.");
}