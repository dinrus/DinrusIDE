#include "DinrusIDE.h"

FileSel& sSD()
{
	static bool b;
	static FileSel fs;
	if(!b) {
		fs.AllFilesType();
		b = true;
	}
	return fs;
}

void Иср::SerializeFindInFiles(Поток& s) {
	int version = 7;
	s / version;
	s % ff.files;
	ff.files.SerializeList(s);
	s % ff.folder;
	ff.folder.SerializeList(s);
	if(version >= 2)
	{
		s % ff.replace;
		ff.replace.SerializeList(s);
		s % ff.style;
	}
	if(version >= 1)
		s % sSD();
	if(version >= 3 && version < 7) {
		СписокБроса dummy;
		s % dummy;
	}

	if(version >= 4)
		s % ff.samecase;
	if(version >= 5)
		s % ff.regexp;
	if(version >= 6)
		s % ff.workspace;
}

void SearchForFiles(Индекс<Ткст>& files, Ткст dir, Ткст mask, int readonly, Время since, Progress& pi) {
	ФайлПоиск ff(приставьИмяф(dir, "*.*"));
	while(ff) {
		if(ff.папка_ли() && *ff.дайИмя() != '.')
			SearchForFiles(files, приставьИмяф(dir, ff.дайИмя()), mask, readonly, since, pi);
		else
		if(ff.файл_ли() && PatternMatchMulti(mask, ff.дайИмя())) {
			if((пусто_ли(readonly) || !!readonly == !!ff.толькочтен_ли()) &&
			   (пусто_ли(since) || ff.дайВремяПоследнЗаписи() >= since)) {
				if(pi.StepCanceled()) return;
				files.найдиДобавь(приставьИмяф(dir, ff.дайИмя()));
			}
		}
		ff.следщ();
	}
}

enum {
	WILDANY = 16,
	WILDONE,
	WILDSPACE,
	WILDNUMBER,
	WILDID,
};

bool сверь(const char *f, const char *s, bool we, bool ignorecase, int& count) {
	const char *b = s;
	while(*f) {
		if(*f == WILDANY) {
			f++;
			for(;;) {
				if(сверь(f, s, we, ignorecase, count)) {
					count += int(s - b);
					return true;
				}
				if(!*s++) break;
			}
			return false;
		}
		else
		if(*f == WILDONE) {
			if(!*s++) return false;
		}
		else
		if(*f == WILDSPACE) {
			if(*s != ' ' && *s != '\t') return false;
			s++;
			while(*s == ' ' || *s == '\t')
				s++;
		}
		else
		if(*f == WILDNUMBER) {
			if(*s < '0' || *s > '9') return false;
			s++;
			while(*s >= '0' && *s <= '9')
				s++;
		}
		else
		if(*f == WILDID) {
			if(!iscib(*s)) return false;
			s++;
			while(iscid(*s)) s++;
		}
		else {
			if(ignorecase ? взаг(*s) != взаг(*f) : *s != *f) return false;
			s++;
		}
		f++;
	}
	count = int(s - b);
	return we && iscid(*s) ? false : true;
}

void Иср::AddFoundFile(const Ткст& фн, int ln, const Ткст& line, int pos, int count)
{
	ErrorInfo f;
	f.file = фн;
	f.lineno = ln;
	f.linepos = pos + 1;
	f.len = count;
	f.kind = 0;
	f.message = "\1" + EditorSyntax::GetSyntaxForFilename(фн) + "\1" +
	            какТкст(pos) + "\1" + какТкст(count) + "\1" + (line.дайСчёт() > 300 ? line.середина(0, 300) : line);
	FFound().добавь(фн, ln, f.message, RawToValue(f));
}

bool Иср::SearchInFile(const Ткст& фн, const Ткст& pattern, bool wholeword, bool ignorecase,
                       int& n, RegExp *regexp) {
	ФайлВвод in(фн);
	if(!in) return true;
	int ln = 1;
	bool wb = wholeword ? iscid(*pattern) : false;
	bool we = wholeword ? iscid(*pattern.последний()) : false;
	int infile = 0;
	bool sync = false;
	while(!in.кф_ли()) {
		Ткст line = in.дайСтроку();
		bool bw = true;
		int  count;
		if(regexp) {
			if(regexp->сверь(line)) {
				AddFoundFile(фн, ln, line, regexp->дайСмещ(), regexp->дайДлину());
				sync = true;
			}
		}
		else
			for(const char *s = line; *s; s++) {
				if(bw && сверь(pattern, s, we, ignorecase, count)) {
					AddFoundFile(фн, ln, line, int(s - line), count);
					sync = true;
					infile++;
					n++;
					break;
				}
				if(wb) bw = !iscid(*s);
			}
		ln++;
	}

	if(sync)
		FFound().синх();

	in.закрой();
	int ffs = ~ff.style;
	if(infile && ffs != STYLE_NO_REPLACE)
	{
		EditFile(фн);
		if(!editor.толькочтен_ли()) {
			bool doit = true;
			if(ffs == STYLE_CONFIRM_REPLACE)
			{
				editor.устКурсор(0);
				editor.найди(false, true);
				switch(PromptYesNoCancel(фмт("Заменить %d строки в [* \1%s\1]?", infile, фн)))
				{
				case 1:  break;
				case 0:  doit = false; break;
				case -1: return false;
				}
			}
			if(doit)
			{
				editor.выбериВсе();
				editor.BlockReplace();
				сохраниФайл();
				FFound().добавь(фн, Null, какТкст(infile) + " замен сделано");
				FFound().синх();
			}
		}
	}

	return true;
}

void Иср::FindInFiles(bool replace) {
	РедакторКода::FindReplaceData d = editor.GetFindReplaceData();
	CtrlRetriever rf;
	ff.output <<= ffoundi_next;
	rf(ff.find, d.find)
	  (ff.replace, d.replace)
	  (ff.ignorecase, d.ignorecase)
	  (ff.samecase, d.samecase)
	  (ff.wholeword, d.wholeword)
	  (ff.wildcards, d.wildcards)
	  (ff.regexp, d.regexp)
	;
	WriteList(ff.find, d.find_list);
	WriteList(ff.replace, d.replace_list);
	ff.синх();
	if(пусто_ли(~ff.folder))
		ff.folder <<= GetUppDir();
	ff.style <<= STYLE_NO_REPLACE;
	ff.синх();
	ff.itext = editor.GetI();
	ff.настрой(replace);
	
	int c = ff.выполни();

	ff.find.AddHistory();
	ff.replace.AddHistory();

	rf.Retrieve();
	d.find_list = ReadList(ff.find);
	d.replace_list = ReadList(ff.replace);
	editor.SetFindReplaceData(d);
	
	if(c == IDOK) {
		сохраниФайл();

		SetFFound(~ff.output);

		FFound().HeaderTab(2).устТекст("Строка исходника");
		Renumber();
		ff.find.AddHistory();
		ff.files.AddHistory();
		ff.folder.AddHistory();
		ff.replace.AddHistory();
		Progress pi("Найдено %d файлов для поиска.");
		pi.AlignText(ALIGN_LEFT);
		Индекс<Ткст> files;
		Время since = Null;
		if(!пусто_ли(ff.recent))
			since = воВремя(дайСисДату() - (int)~ff.recent);
		if(ff.workspace) {
			const РОбласть& wspc = GetIdeWorkspace();
			for(int i = 0; i < wspc.дайСчёт(); i++)
				SearchForFiles(files, дайПапкуФайла(PackagePath(wspc[i])),
					           ~ff.files, ~ff.readonly, since, pi);
		}
		else
			SearchForFiles(files, нормализуйПуть(~~ff.folder, GetUppDir()), ~ff.files,
			               ~ff.readonly, since, pi);
		if(!pi.Canceled()) {
			Ткст pattern;
			RegExp rx, *regexp = NULL;
			if(ff.regexp) {
				rx.SetPattern(~ff.find);
				regexp = &rx;
				pattern = "dummy";
			}
			else
			if(ff.wildcards) {
				Ткст q = ~ff.find;
				for(const char *s = q; *s; s++)
					if(*s == '\\') {
						s++;
						if(*s == '\0') break;
						pattern.конкат(*s);
					}
					else
					switch(*s) {
					case '*': pattern.конкат(WILDANY); break;
					case '?': pattern.конкат(WILDONE); break;
					case '%': pattern.конкат(WILDSPACE); break;
					case '#': pattern.конкат(WILDNUMBER); break;
					case '$': pattern.конкат(WILDID); break;
					default:  pattern.конкат(*s);
					}
			}
			else
				pattern = ~ff.find;
			pi.устВсего(files.дайСчёт());
			FFound().очисть();
			pi.устПоз(0);
			int n = 0;
			for(int i = 0; i < files.дайСчёт(); i++) {
				pi.устТекст(files[i]);
				if(pi.StepCanceled()) break;
				if(!пусто_ли(pattern)) {
					if(!SearchInFile(files[i], pattern, ff.wholeword, ff.ignorecase, n, regexp))
						break;
				}
				else {
					ErrorInfo f;
					f.file = files[i];
					f.lineno = 1;
					f.linepos = 0;
					f.kind = 0;
					f.message = files[i];
					FFound().добавь(f.file, 1, f.message, RawToValue(f));
					FFound().синх();
					n++;
				}
			}
			FFound().HeaderTab(2).устТекст(фмт("Строка исходника (%d)", FFound().дайСчёт()));
			if(!пусто_ли(pattern))
				FFound().добавь(Null, Null, какТкст(n) + " случай(ев)обнаружен(о).");
			else
				FFound().добавь(Null, Null, какТкст(n) + "  файл(ов) с совпадением был(о) обнаруЖен(о).");
		}
	}
}


void Иср::FindFileAll(const Вектор<Кортеж<int64, int>>& f)
{
	SetFFound(ffoundi_next);
	FFound().очисть();
	for(auto pos : f) {
		editor.CachePos(pos.a);
		int linei = editor.дайПозСтроки64(pos.a);
		ШТкст ln = editor.дайШСтроку(linei);
		AddFoundFile(editfile, linei + 1, ln.вТкст(), длинаУтф8(~ln, (int)pos.a), длинаУтф8(~ln + pos.a, pos.b));
	}
	FFound().HeaderTab(2).устТекст(фмт("Строка исходника (%d)", FFound().дайСчёт()));
	FFound().добавь(Null, Null, какТкст(f.дайСчёт()) + " случай(ев)обнаружен(о).");
}
	
void Иср::найдиТкст(bool back)
{
	if(!editor.найдиТкст(back))
		BeepMuteExclamation();
}

void Иср::TranslateString()
{
	if(editor.толькочтен_ли()) return;
	int l, h;
	if(editor.дайВыделение(l, h)) {
		editor.вставь(l, "t_(");
		editor.вставь(h + 3, ")");
		editor.устКурсор(h + 4);
		найдиТкст(false);
	}
}

void Иср::InsertWildcard(const char *s) {
	iwc = s;
}

void Иср::FindWildcard() {
	int l, h;
	ff.find.дайВыделение(l, h);
	iwc = 0;
	FindWildcardMenu(THISBACK(InsertWildcard), ff.find.GetPushScreenRect().верхПраво(), false, NULL, ff.regexp);
	if(iwc.дайСчёт()) {
		ff.wildcards = true;
		ff.find.устФокус();
		ff.find.устВыделение(l, h);
		ff.find.удалиВыделение();
		ff.find.вставь(iwc);
	}
}

void Иср::найдиУстСтдПап(Ткст n)
{
	ff.folder <<= n;
}

void Иср::найдиСтдПап()
{
	Ткст n = дайПапкуФайла(editfile);
	БарМеню menu;
	if(!пусто_ли(n))
		menu.добавь(n, THISBACK1(найдиУстСтдПап, n));
	Ткст hub = GetHubDir();
	Вектор<Ткст> pd;
	const РОбласть& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.дайСчёт(); i++)
		pd.добавь(PackageDirectory(wspc[i]));
	for(Ткст d : GetUppDirs())
		if(!IsHubDir(d) || найдиСовпадение(pd, [&](const Ткст& q) { return q.начинаетсяС(d); }) >= 0)
			menu.добавь(d, [=] { найдиУстСтдПап(d); });
	menu.добавь(GetHubDir(), [=] { найдиУстСтдПап(GetHubDir()); });
	menu.выполни(&ff.folder, ff.folder.GetPushScreenRect().низЛево());
}

void Иср::найдиПапку()
{
	if(!sSD().ExecuteSelectDir()) return;
	ff.folder <<= ~sSD();
}

void Иср::SyncFindInFiles()
{
	ff.samecase.вкл(ff.ignorecase);
}

void Иср::ConstructFindInFiles() {
	ff.find.AddButton().SetMonoImage(CtrlImg::smallright()).Подсказка("Wildcard") <<= THISBACK(FindWildcard);
	static const char *defs = "*.cpp *.h *.hpp *.c *.m *.C *.M *.cxx *.cc *.mm *.MM *.icpp *.sch *.lay *.rc";
	ff.files <<= Ткст(defs);
	ff.files.добавьСписок((Ткст)defs);
	ff.files.добавьСписок((Ткст)"*.txt");
	ff.files.добавьСписок((Ткст)"*.*");
	ff.folder.AddButton().SetMonoImage(CtrlImg::smalldown()).Подсказка("Относящиеся тома") <<= THISBACK(найдиСтдПап);
	ff.folder.AddButton().SetMonoImage(CtrlImg::smallright()).Подсказка("Выберите том") <<= THISBACK(найдиПапку);
	editor.PutI(ff.find);
	editor.PutI(ff.replace);
	CtrlLayoutOKCancel(ff, "Найти в Файлах");
	ff.ignorecase <<= THISBACK(SyncFindInFiles);
	ff.samecase <<= true;
	SyncFindInFiles();
}

void FindInFilesDlg::синх()
{
	replace.вкл((int)~style);
	bool b = !regexp;
	wildcards.вкл(b);
	ignorecase.вкл(b);
	wholeword.вкл(b);
	folder.вкл(!workspace);
	folder_lbl.вкл(!workspace);
}

FindInFilesDlg::FindInFilesDlg()
{
	regexp <<= style <<= THISBACK(синх);
	readonly.добавь(Null, "Все файлы");
	readonly.добавь(0, "Записываемый");
	readonly.добавь(1, "Только чтение");
	readonly <<= Null;
	recent <<= Null;
	recent.добавь(Null, "Все");
	recent.добавь(1, "1 День");
	recent.добавь(3, "3 Дня");
	recent.добавь(7, "7 Дней");
	recent.добавь(14, "14 Дней");
	recent.добавь(32, "28 Дней");
	workspace <<= THISBACK(синх);
}

void FindInFilesDlg::настрой(bool replacing)
{
	Титул(replacing ? "Найти и заменить в файлах" : "Найти в файлах");
	replace_lbl.покажи(replacing);
	style.покажи(replacing);
	replace_lbl2.покажи(replacing);
	replace.покажи(replacing);
	Размер sz = GetLayoutSize();
	if(!replacing)
		sz.cy -= replace.дайПрям().bottom - folder.дайПрям().bottom;
	Прям r = дайПрям();
	r.устРазм(sz);
	устПрям(r);
	ActiveFocus(find);
}

bool FindInFilesDlg::Ключ(dword ключ, int count)
{
	if(ключ == K_CTRL_I) {
		if(find.естьФокус()) {
			find <<= itext;
			return true;
		}
		if(replace.естьФокус()) {
			replace <<= itext;
			return true;
		}
	}
	return ТопОкно::Ключ(ключ, count);
}

void Иср::SetFFound(int ii)
{
	ii = clamp(ii, 0, 2);
	устНиз(BFINDINFILES1 + ii);
	ffoundi_next = (ii + 1) % 3;
}
	
КтрлМассив& Иср::FFound()
{
	int i = btabs.дайКурсор() - BFINDINFILES1;
	return i >= 0 && i < 3 ? ffound[i] : ffound[0];
}

void Иср::CopyFound(bool all)
{
	Ткст txt;
	for(int i = 0; i < FFound().дайСчёт(); i++) {
		if(all)
			txt << FFound().дай(i, 0) << " (" << FFound().дай(i, 1) << "): ";
		Ткст h = FFound().дай(i, 2);
		if(*h == '\1')
			h = разбей(~h + 1, '\1', false).верх();
		txt << h << "\r\n";
	}
	WriteClipboardText(txt);
}

void Иср::FFoundMenu(Бар& bar)
{
	bar.добавь("Копировать текст", THISBACK1(CopyFound, false));
	bar.добавь("Копировать всё", THISBACK1(CopyFound, true));
}
