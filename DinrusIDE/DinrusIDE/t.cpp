#include "DinrusIDE.h"

struct FileLine : Движимое<FileLine> {
	Ткст file;
	int    line;
};

struct LngEntry : Движимое<LngEntry, DeepCopyOption<LngEntry> > {
	bool                   added;
	ВекторМап<int, Ткст> text;
	Вектор<FileLine>       fileline;

	LngEntry(const LngEntry& e, int) { added = e.added; text <<= e.text; fileline <<= e.fileline; }
	LngEntry() {}

	void AddFileLine(СиПарсер& p) {
		FileLine& fl = fileline.добавь();
		fl.file = p.дайИмяф();
		fl.line = p.дайСтроку();
	}
};

struct TFile : Движимое<TFile> {
	bool                        dirty;
	bool                        java;
	Ткст                      package, file;
	ВекторМап<Ткст, LngEntry> map;
	Вектор<int>                 ls;

	void MakeLS() {
		Индекс<int> lngset;
		lngset.добавь(LNG_enUS);
		for(int i = 0; i < map.дайСчёт(); i++)
			for(int j = 0; j < map[i].text.дайСчёт(); j++)
				lngset.найдиДобавь(map[i].text.дайКлюч(j));
		ls = lngset.подбериКлючи();
		сортируй(ls);
	}

	rval_default(TFile);
	TFile() { dirty = false; }
};

void t_error(СиПарсер& p)
{
	вКонсоль(p.дайИмяф() + фмт("(%d): t_/tt_ работает только с простыми текстовыми литералами", p.дайСтроку()));
}

void LngParseCFile(const Ткст& фн, ВекторМап<Ткст, LngEntry>& lng)
{
	Ткст data = загрузиФайл(фн);
	СиПарсер p(data, фн);
	try {
		while(!p.кф_ли()) {
			if((p.ид("t_") || p.ид("tt_")) && p.сим('('))
				if(p.ткст_ли()) {
					Ткст tid = p.читайТкст();
					LngEntry& le = lng.дайДобавь(tid);
					le.text.дайДобавь(LNG_enUS) = GetENUS(tid);
					le.AddFileLine(p);
					le.added = true;
					if(!p.сим(')'))
						t_error(p);
				}
				else
					t_error(p);
			else
				p.пропустиТерм();
		}
	}
	catch(СиПарсер::Ошибка e) {
		вКонсоль(e);
		ShowConsole();
	}
}

const Индекс<int>& LngIndex()
{
	static Индекс<int> l;
	if(l.дайСчёт() == 0) {
		const int *x = GetAllLanguages();
		while(*x)
			l.добавь(*x++);
	}
	return l;
}

bool LngParseTFile(const Ткст& фн, ВекторМап<Ткст, LngEntry>& lng)
{
	Ткст data = загрузиФайл(фн);
	СиПарсер p(data, фн);
	try {
		if(p.сим('#'))
			while(!p.кф_ли()) {
				if(p.сим2_ли('T', '_'))
					break;
				else
					p.пропустиТерм();
			}
		Ткст id;
		while(!p.кф_ли()) {
			if(p.ид("T_")) {
				p.передайСим('(');
				id = Null;
				do
					id.конкат(p.читайТкст());
				while(p.сим('+'));
				p.передайСим(')');
				lng.дайДобавь(id).added = false;
			}
			else {
				if(пусто_ли(id))
					p.выведиОш("missing T_");
				Ткст lngs = p.читайИд();
				p.передайСим('(');
				if(lngs.дайДлину() == 4) {
					int lang = LNG_(взаг(lngs[0]), взаг(lngs[1]), взаг(lngs[2]), взаг(lngs[3]));
					if(LngIndex().найди(lang) >= 0) {
						Ткст lt;
						do
							lt.конкат(p.читайТкст());
						while(p.сим('+'));
						lng.дайДобавь(id).text.дайДобавь(lang) = lt;
						p.передайСим(')');
						continue;
					}
				}
				p.выведиОш("неверный язык");
			}
		}
	}
	catch(СиПарсер::Ошибка e) {
		вКонсоль(e);
		ShowConsole();
		return false;
	}
	return true;
}

Ткст CreateTFile(const ВекторМап<Ткст, LngEntry>& map, const Вектор<int>& lngset, bool rep, bool obsolete, bool java)
{
	const char *linepfx = (java ? "   + " : "     ");
	int ascflags = (java ? 0 : ASCSTRING_OCTALHI) | ASCSTRING_SMART;

	Ткст out;
	Ткст cfile;
	out << "#ifdef _MSC_VER\r\n#pragma setlocale(\"C\")\r\n#endif";

	for(int i = 0; i < map.дайСчёт(); i++) {
		if(i) out << "\r\n";
		const LngEntry& e = map[i];
		Ткст nc;
		if(e.fileline.дайСчёт())
			nc = e.fileline[0].file;
		if(nc != cfile) {
			cfile = nc;
			if(!пусто_ли(cfile) && !rep)
				out << "\r\n// " << дайИмяф(cfile) << "\r\n\r\n";
			if(пусто_ли(cfile) && obsolete)
				out << "\r\n// Obsolete\r\n\r\n";
		}
		if(!пусто_ли(cfile) || rep || obsolete) {
			Ткст id = map.дайКлюч(i);
			out << "T_(" << какТкстСи(id, 70, linepfx, ascflags) << ")\r\n";
			for(int j = 0; j < lngset.дайСчёт(); j++) {
				int lang = lngset[j];
				if(rep || lang != LNG_enUS) {
					int q = e.text.найди(lang);
					if(!rep || q >= 0 && !пусто_ли(e.text[q])) {
						int c = (lang >> 15) & 31;
						if(c) {
							out.конкат(c + 'a' - 1);
							c = (lang >> 10) & 31;
							if(c) {
								out.конкат(c + 'a' - 1);
								c = (lang >> 5) & 31;
								if(c) {
									out.конкат(c + 'A' - 1);
									c = lang & 31;
									if(c) out.конкат(c + 'A' - 1);
								}
							}
						}
						out << '(' << какТкстСи(q >= 0 ? e.text[q] : Ткст(), 70,
							linepfx, ascflags) << ")\r\n";
					}
				}
			}
		}
	}
	return out;
}

struct LangDlg : WithLangLayout<ТопОкно> {
	void сериализуй(Поток& s);

	Вектор<TFile>& tfile;

	void AddLang();
	void RemoveLang();

	void AddLangAll();
	void RemoveLangAll();

	void EnterFile();
	void EnterText();
	void ToggleWork();

	void LangMenu(Бар& bar);

	bool ShouldWrite(int i)        { return file.дай(i, 1); }

	LangDlg(Вектор<TFile>& map);
};

void LangDlg::сериализуй(Поток& s)
{
	SerializePlacement(s);
	Ткст f = file.дайКлюч();
	int l = lang.дайКлюч();
	Ткст t = text.дайКлюч();
	s % f % l % t;
	file.FindSetCursor(f);
	lang.FindSetCursor(l);
	text.FindSetCursor(t);
}

void LangDlg::AddLang()
{
	if(!file.курсор_ли())
		return;
	WithAddLangLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, "Добавить язык");
	dlg.lang <<= dlg.Breaker(999);
	TFile& tf = tfile[file.дайКурсор()];
	Вектор<int>& ls = tf.ls;
	for(;;) {
		int l = ~dlg.lang;
		dlg.ok.вкл((l & 31) && найдиИндекс(ls, l) < 0);
		switch(dlg.пуск()) {
		case IDOK:
			l = ~dlg.lang;
			if(найдиИндекс(ls, l) < 0) {
				ls.добавь(l);
				tf.dirty = true;
				сортируй(ls);
				EnterFile();
				lang.FindSetCursor(l);
				return;
			}
			break;
		case IDCANCEL:
			return;
		}
	}
}

void LangDlg::AddLangAll()
{
	WithAddLangLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, "Добавить ко всем");
	if(dlg.пуск() != IDOK)
		return;
	int l = ~dlg.lang;
	for(int i = 0; i < tfile.дайСчёт(); i++) {
		TFile& tf = tfile[i];
		Вектор<int>& ls = tf.ls;
		if(найдиИндекс(ls, l) < 0) {
			ls.добавь(l);
			tf.dirty = true;
			сортируй(ls);
		}
	}
	EnterFile();
	lang.FindSetCursor(l);
}

void LangDlg::RemoveLang()
{
	if(file.курсор_ли() && lang.курсор_ли() && (int)lang.дайКлюч() != LNG_enUS
	   && PromptOKCancel("Удалить выбранную версию языка?")) {
		TFile& tf = tfile[file.дайКурсор()];
		tf.ls.удали(lang.дайКурсор());
		tf.dirty = true;
		EnterFile();
	}
}

void LangDlg::RemoveLangAll()
{
	WithAddLangLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, "Удалить из всех");
	if(file.курсор_ли() && lang.курсор_ли())
		dlg.lang <<= tfile[file.дайКурсор()].ls[lang.дайКурсор()];
	if(dlg.пуск() != IDOK)
		return;
	int l = ~dlg.lang;
	for(int i = 0; i < tfile.дайСчёт(); i++) {
		TFile& tf = tfile[i];
		Вектор<int>& ls = tf.ls;
		int q = найдиИндекс(ls, l);
		if(q >= 0) {
			ls.удали(q);
			tf.dirty = true;
			сортируй(ls);
		}
	}
	EnterFile();
	lang.FindSetCursor(l);
}

void LangDlg::LangMenu(Бар& bar)
{
	bar.добавь(file.курсор_ли(), "Добавить..", [=] { AddLang(); });
	bar.добавь(lang.курсор_ли(), "Удалить", [=] { RemoveLang(); });
	bar.Separator();
	bar.добавь(file.курсор_ли(), "Добавить ко всем..", [=] { AddLangAll(); });
	bar.добавь(lang.курсор_ли(), "Удалить из всех..", [=] { RemoveLangAll(); });
}

struct FontAndColorDisplay : Дисплей {
	Шрифт  font;
	Цвет color;

	void рисуй(Draw& w, const Прям& r, const Значение& q,
						Цвет ink, Цвет paper, dword s) const {
		w.DrawRect(r, paper);
		Ткст vt = q;
		const char *txt = GetENUS(vt);
		int x = r.left;
		if(txt != ~vt) {
			Ткст id(~vt, txt - 1);
			w.DrawText(x, r.top, id, font, txt[-1] == '\a' ? светлоКрасный : магента);
			x += дайРазмТекста(id, font).cx + 6;
		}
		w.DrawText(x, r.top, какТкстСи(txt), font, ink);
	}

	FontAndColorDisplay(Шрифт f, Цвет c) : font(f), color(c) {}
};

void LangDlg::EnterFile()
{
	TFile& tf = tfile[file.дайКурсор()];
	сортируй(tf.ls);

	lang.очисть();
	for(int i = 0; i < tf.ls.дайСчёт(); i++)
		lang.добавь(tf.ls[i], LNGAsText(tf.ls[i]));
	lang.идиВНач();

	static FontAndColorDisplay normal(ArialZ(11)(), Null);
	static FontAndColorDisplay added(ArialZ(11)().Bold(), Null);
	static FontAndColorDisplay obsolete(ArialZ(11)().Italic(), Null);

	text.очисть();
	for(int i = 0; i < tf.map.дайСчёт(); i++) {
		text.добавь(tf.map.дайКлюч(i));
		LngEntry& e = tf.map[i];
		Дисплей *d;
		if(e.fileline.дайСчёт() == 0)
			d = &obsolete;
		else
		if(e.added)
			d = &added;
		else
			d = &normal;
		text.устДисплей(text.дайСчёт() - 1, 0, *d);
	}
	text.идиВНач();
}

void LangDlg::EnterText()
{
	source.очисть();
	ВекторМап<Ткст, LngEntry>& map = tfile[file.дайКурсор()].map;
	int q = text.дайКурсор();
	if(q < 0 || q >= map.дайСчёт())
		return;
	LngEntry& e = map[q];
	for(int i = 0; i < e.fileline.дайСчёт(); i++) {
		FileLine& f = e.fileline[i];
		source.добавь(f.file, f.line, дайИмяф(f.file) + " (" + какТкст(f.line) + ")");
	}
}

void LangDlg::ToggleWork()
{
	if(file.дайСчёт()) {
		bool q = !(int)file.дай(0, 1);
		for(int i = 0; i < file.дайСчёт(); i++)
			file.уст(i, 1, q);
	}
}

LangDlg::LangDlg(Вектор<TFile>& tfile)
:	tfile(tfile)
{
	CtrlLayoutOKCancel(*this, "Файлы переводов");
	file.добавьКолонку("Файл");
	HeaderCtrl::Колонка& m = file.добавьКолонку().Ctrls<Опция>().HeaderTab();
	m.устРисунок(IdeImg::work());
	m.WhenAction = [=] { ToggleWork(); };
	file.WhenEnterRow = [=] { EnterFile(); };
	file.ColumnWidths("144 27");

	lang.добавьИндекс();
	lang.добавьКолонку("Версии");
	lang.WhenBar = [=] (Бар& bar) { LangMenu(bar); };

	text.добавьКолонку("Текст");
	text.WhenEnterRow = [=] { EnterText(); };
	
	help << [=] { запустиВебБраузер("https://www.ultimatepp.org/srcdoc$Core$i18n_en-us.html"); };
	
	source.добавьИндекс();
	source.добавьИндекс();
	source.добавьКолонку("Исходник");
	source.WhenLeftDouble = Breaker(IDYES);

	for(int i = 0; i < tfile.дайСчёт(); i++)
		file.добавь(tfile[i].package + '/' + tfile[i].file, 1);
	file.идиВНач();

	Иконка(IdeImg::Language());
	Sizeable().Zoomable();
}

class ExportTrDlg : public WithExportTrLayout<ТопОкно> {
	SelectDirButton of;
	typedef ExportTrDlg ИМЯ_КЛАССА;

public:
	ExportTrDlg();
};

ExportTrDlg::ExportTrDlg()
{
	CtrlLayoutOKCancel(*this, "Эксортировать в файл .tr");

	DlCharsetD(charset);
	of.прикрепи(folder);

	charset <<= CHARSET_UTF8;
	lang <<= LNG_ENGLISH;
}

Ткст ExportTr(const Вектор<TFile>& tfile, int& cs)
{
	ExportTrDlg dlg;
	грузиИзГлоба(dlg, "tr-export");
	int c = dlg.пуск();
	сохраниВГлоб(dlg, "tr-export");
	if(c != IDOK)
		return Null;
	int lng = ~dlg.lang;
	cs = ~dlg.charset;
	Ткст фн = приставьИмяф(~dlg.folder, LNGAsText(lng) + "." + имяНабСим(cs) + ".tr");
	ФайлВывод out(фн);
	if(!out) {
		Exclamation("Не удаётся открыть файл вывода:&[* \1" + фн);
		return Null;
	}
	out << "LANGUAGE " << какТкстСи(LNGAsText(SetLNGCharset(lng, cs))) << ";\r\n";
	for(int i = 0; i < tfile.дайСчёт(); i++) {
		const TFile& m = tfile[i];
		out << "// " << m.package << "/" << m.file << "\r\n";
		for(int j = 0; j < m.map.дайСчёт(); j++)
			out << какТкстСи(m.map.дайКлюч(j), 70) << ",\r\n"
			    << "\t" << какТкстСи(вНабсим(cs, m.map[j].text.дай(lng, ""), CHARSET_UTF8),
			                         60, "\t", ASCSTRING_SMART)
			    << ";\r\n\r\n";
	}
	return фн;
}

void Иср::SyncT(int kind)
{
	console.очисть();
	Ткст filepath = GetActiveFilePath();
	сохраниФайл();

	TFile repository;
	LngParseTFile(конфигФайл("repository.t"), repository.map);

	if(kind == 1) {
		FileSel fs;
		fs.ActiveDir(AnySourceFs().GetActiveDir());
		fs.Type("Файл перевода рантаймный (*.tr)", "*.tr");
		fs.AllFilesType();
		грузиИзГлоба(fs, "fs-tr");
		bool b = fs.ExecuteOpen("Импольтировать файл перевода");
		сохраниВГлоб(fs, "fs-tr");
		if(!b) return;
		Ткст tr = загрузиФайл(~fs);
		СиПарсер p(tr);
		try {
			while(!p.кф_ли()) {
				p.передайИд("LANGUAGE");
				int lang = LNGFromText(p.читайТкст());
				if(!lang)
					p.выведиОш("Неверный Язык");
				p.передайСим(';');
				byte cs = GetLNGCharset(lang);
				lang &= LNGC_(255, 255, 255, 255, 0);
				while(p.ткст_ли()) {
					Ткст id = p.читайТкст();
					p.передайСим(',');
					repository.map.дайДобавь(id).text.дайДобавь(lang) = вНабсим(CHARSET_UTF8, p.читайТкст(), cs);
					p.передайСим(';');
				}
			}
		}
		catch(СиПарсер::Ошибка& e) {
			вКонсоль(e);
			Exclamation("Неудача: \1" + e);
			return;
		}
	}

	Вектор<TFile> tfile;
	Вектор<int> mainsT;

	Progress pi;
	const РОбласть& wspc = роблИср();
	pi.устВсего(wspc.дайСчёт());
	for(int iPackage = 0; iPackage < wspc.дайСчёт(); iPackage++) {
		const Пакет& pk = wspc.дайПакет(iPackage);
		Ткст n = wspc[iPackage];
		pi.устТекст("Скануруется " + n);
		if(pi.StepCanceled()) return;
		ВекторМап<Ткст, LngEntry> pmap;
		for(int iFile = 0; iFile < pk.file.дайСчёт(); iFile++) {
			Ткст file = SourcePath(n, pk.file[iFile]);
			LngParseCFile(SourcePath(GetActivePackage(), file), pmap);
		}
		bool localT = false;
		for(int iFile = 0; iFile < pk.file.дайСчёт(); iFile++) {
			Ткст file = SourcePath(n, pk.file[iFile]);
			Ткст ext = дайРасшф(file);
			if(ext == ".t" || ext == ".jt") {
				ВекторМап<Ткст, LngEntry> tmap(pmap, 0);
				if(LngParseTFile(file, tmap)) {
					TFile& tf = tfile.добавь();
					tf.java = (ext == ".jt");
					tf.package = n;
					tf.file = pk.file[iFile];
					tf.map = pick(tmap);
					tf.MakeLS();
					// mark that we've found a local translation file
					localT = true;
					
					// store Индекс of main package translation(s) file(s)
					if(iPackage == 0)
						mainsT.добавь(tfile.дайСчёт() - 1);
				}
			}
		}
		// if no local translation file(s), we append translation to
		// main package one(s), if any
		if(!localT && iPackage > 0)
		{
			for(int i = 0; i < mainsT.дайСчёт(); i++)
			{
				ВекторМап<Ткст, LngEntry> &map = tfile[mainsT[i]].map;
				for(int iEntry = 0; iEntry < pmap.дайСчёт(); iEntry++)
				{
					int idx = map.найди(pmap.дайКлюч(iEntry));
					if(idx < 0)
					{
						map.добавь(pmap.дайКлюч(iEntry), clone(pmap[iEntry]));
						idx = map.дайСчёт() - 1;
					}
					LngEntry &entry = map[idx];
					if(!entry.fileline.дайСчёт())
					{
						LngEntry &pEntry = pmap[iEntry];
						for(int iLc = 0; iLc < pEntry.fileline.дайСчёт(); iLc++)
						{
							FileLine &fl = entry.fileline.добавь();
							Ткст file = дайИмяф(pEntry.fileline[iLc].file);
							file = "ПАКЕТ '" + дайИмяф(n) + "' FILE '" + file + "'";
							fl.file = file;
							fl.line = pEntry.fileline[iLc].line;
						}
					}
				}
			}
		}
	}

	if(kind == 2) {
		for(int i = 0; i < tfile.дайСчёт(); i++) {
			TFile& tf = tfile[i];
			for(int ii = 0; ii < tf.map.дайСчёт(); ii++) {
				LngEntry& tle = tf.map[ii];
				LngEntry& rle = repository.map.дайДобавь(tf.map.дайКлюч(ii));
				for(int l = 0; l < tle.text.дайСчёт(); l++)
					if(!пусто_ли(tle.text[l]))
						rle.text.дайДобавь(tle.text.дайКлюч(l)) = tle.text[l];
			}
		}
	
		int cs;
		Ткст фн = ExportTr(tfile, cs);
		if(!пусто_ли(фн)) {
			EditFile(фн);
			int c = filelist.дайКурсор();
			FlushFile();
			ActiveFile().charset = (byte)cs;
			SavePackage();
			PackageCursor();
			if(c >= 0)
				filelist.устКурсор(c);
			editor.ClearUndo();
		}
		return;
	}

	LangDlg dlg(tfile);
	LoadFromWorkspace(dlg, "Tdlg");
	switch(dlg.выполни()) {
	case IDCANCEL:
		return;
	case IDYES:
		if(dlg.source.курсор_ли()) {
			EditFile(dlg.source.дай(0));
			editor.устКурсор(editor.дайПоз64(editor.дайНомСтр((int)dlg.source.дай(1) - 1)));
			editor.курсорПоЦентру();
			editor.устФокус();
			StoreToWorkspace(dlg, "Tdlg");
			return;
		}
	}
	StoreToWorkspace(dlg, "Tdlg");

	FlushFile();

	for(int i = 0; i < tfile.дайСчёт(); i++) {
		TFile& tf = tfile[i];
		for(int ii = 0; ii < tf.map.дайСчёт(); ii++) {
			int q = repository.map.найди(tf.map.дайКлюч(ii));
			if(q >= 0) {
				LngEntry& tle = tf.map[ii];
				LngEntry& rle = repository.map[q];
				for(int l = 0; l < tf.ls.дайСчёт(); l++) {
					q = rle.text.найди(tf.ls[l]);
					if(q >= 0)
						tle.text.дайДобавь(tf.ls[l]) = rle.text[q];
				}
			}
		}
	}

	pi.устВсего(tfile.дайСчёт());
	for(int i = 0; i < tfile.дайСчёт(); i++) {
		TFile& tf = tfile[i];
		Ткст td = CreateTFile(tf.map, tf.ls, false, !dlg.remove, tf.java);
		Ткст fp = SourcePath(tf.package, tf.file);
		pi.устТекст("Сохранение " + tf.package + '/' + tf.file);
		if(pi.StepCanceled())
			break;
		if(dlg.ShouldWrite(i) && загрузиФайл(fp) != td)
			::сохраниФайл(fp, td);
	}

	repository.MakeLS();
	::сохраниФайл(конфигФайл("repository.t"), CreateTFile(repository.map, repository.ls, true, false, false));
	FileCursor();
}

void Иср::ConvertST()
{
	if(!PromptYesNo("Преобразовать проект в переводы t_?"))
		return;

	FlushFile();

	TFile repository;
	LngParseTFile(конфигФайл("repository.t"), repository.map);

	Progress pi;
	const РОбласть& wspc = роблИср();
	pi.устВсего(wspc.дайСчёт());
	for(int i = 0; i < wspc.дайСчёт(); i++) {
		const Пакет& pk = wspc.дайПакет(i);
		Ткст n = wspc[i];
		pi.устТекст("Преобразуется " + n);
		if(pi.StepCanceled()) return;
		ВекторМап<Ткст, LngEntry> pmap;
		for(int i = 0; i < pk.file.дайСчёт(); i++) {
			Ткст file = SourcePath(n, pk.file[i]);
			Ткст data = загрузиФайл(file);
			СиПарсер p(data);
			Ткст r;
			const char *b = data;
			while(!p.кф_ли()) {
				p.пробелы();
				const char *w = p.дайУк();
				if(p.ид("s_") && p.сим('(') && p.ид_ли()) {
					Ткст id = p.читайИд();
					int q = repository.map.найди(id);
					if(q >= 0) {
						LngEntry& e = repository.map[q];
						q = e.text.найди(LNG_enUS);
						if(q >= 0) {
							r.конкат(b, w);
							r.конкат("t_(");
							r.конкат(какТкстСи(e.text[q]));
							b = p.дайУк();
						}
					}
				}
				else
					p.пропустиТерм();
			}
			r.конкат(b);
			if(r != data)
				::сохраниФайл(file, r);
		}
	}

	FileCursor();
}
