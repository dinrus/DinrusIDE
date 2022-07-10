#include "DinrusIDE.h"

struct ДисплейОпции : Дисплей {
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink,
	                   Цвет paper, dword style) const
	{
		ШТкст txt = q;
		w.DrawRect(r, paper);
		w.Clipoff(r.left + Zx(10), r.top, r.устШирину() - Zx(20), r.устВысоту());
		int tcy = GetTLTextHeight(txt, StdFont());
		DrawTLText(w, 0, max((r.устВысоту() - tcy) / 2, 0), r.устШирину(), txt, StdFont(), ink);
		w.стоп();
	}
	virtual Размер дайСтдРазм(const Значение& q) const
	{
		ШТкст txt = q;
		Размер tsz = GetTLTextSize(txt, StdFont());
		tsz.cx += Zx(20);
		return tsz;
	}
};

struct ДисплейЗагаОпции : Дисплей {
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink,
	                   Цвет paper, dword style) const
	{
		ШТкст txt = q;
		w.DrawRect(r, AdjustIfDark(Цвет(255, 255, 204)));
		w.Clipoff(r.left + Zx(5), r.top, r.устШирину() - Zx(10), r.устВысоту());
		int tcy = GetTLTextHeight(txt, StdFont().Bold());
		DrawTLText(w, 0, max((r.устВысоту() - tcy) / 2, 0), r.устШирину(), txt,
		           StdFont().Bold(), SColorText());
		w.стоп();
	}
	virtual Размер дайСтдРазм(const Значение& q) const
	{
		ШТкст txt = q;
		Размер tsz = GetTLTextSize(txt, StdFont());
		tsz.cx += Zx(10);
		return tsz;
	}
};

struct ДисплейЗависимостей : Дисплей {
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink,
	                   Цвет paper, dword style) const
	{
		ШТкст txt = q;
		w.DrawRect(r, paper);
		w.Clipoff(r.left + Zx(10), r.top, r.устШирину() - Zx(20), r.устВысоту());
		Рисунок img = IdeFileImage(q, false, false);
		Размер isz = img.дайРазм();
		w.DrawImage(0, (r.устВысоту() - isz.cy) / 2, img);
		int tcy = GetTLTextHeight(txt, StdFont());
		DrawTLText(w, isz.cx + Zx(4), max((r.устВысоту() - tcy) / 2, 0), r.устШирину(), txt, StdFont(), ink);
		w.стоп();
	}
	virtual Размер дайСтдРазм(const Значение& q) const
	{
		ШТкст txt = q;
		Размер sz = GetTLTextSize(txt, StdFont());
		sz.cx += Zx(24);
		Размер isz = IdeFileImage(q, false, false).дайРазм();
		sz.cy = max(isz.cy, sz.cy);
		sz.cx += isz.cx;
		return sz;
	}
};

struct ДисплейИспользований : Дисплей {
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink,
	                   Цвет paper, dword style) const
	{
		ШТкст txt = q;
		w.DrawRect(r, paper);
		w.Clipoff(r.left + Zx(10), r.top, r.устШирину() - Zx(20), r.устВысоту());
		Размер isz = IdeImg::Package().дайРазм();
		w.DrawImage(0, (r.устВысоту() - isz.cy) / 2, IdeImg::Package());
		int tcy = GetTLTextHeight(txt, StdFont());
		DrawTLText(w, isz.cx + Zx(4), max((r.устВысоту() - tcy) / 2, 0), r.устШирину(), txt, StdFont(), ink);
		w.стоп();
	}
	virtual Размер дайСтдРазм(const Значение& q) const
	{
		ШТкст txt = q;
		Размер sz = GetTLTextSize(txt, StdFont());
		sz.cx += Zx(24);
		Размер isz = IdeImg::Package().дайРазм();
		sz.cy = max(isz.cy, sz.cy);
		sz.cx += isz.cx;
		return sz;
	}
};

int FlagFilter(int c) {
	return IsAlNum(c) || c == '_' || c == ' ' ? взаг(c) : 0;
}

int FlagFilterM(int c) {
	return c == '.' ? '.' : FlagFilter(c);
}

int CondFilter(int c) {
	return c == '!' || c == '(' || c == ')' || c == '&' || c == '|' ? c : FlagFilter(c);
}

bool ДлгИспользований::нов()
{
	Ткст s = SelectPackage("Выберите пакет");
	text <<= s;
	return !пусто_ли(s);
}

ДлгИспользований::ДлгИспользований()
{
	CtrlLayoutOKCancel(*this, "Использования");
	when.устФильтр(CondFilter);
	text.устДисплей(Single<ДисплейИспользований>());
	text.WhenPush = [=] { нов(); };
}

void РедакторПакета::SaveOptions() {
	if(!actualpackage.пустой()) {
		actual.description = ~description;
		actual.ink = ~ink;
		actual.bold = ~bold;
		actual.italic = ~italic;
		actual.charset = (byte)(int)~charset;
		actual.tabsize = ~tabsize;
		actual.spellcheck_comments = ~spellcheck_comments;
		actual.accepts = разбей(accepts.дайТекст().вТкст(), ' ');
		actual.noblitz = noblitz;
		actual.nowarnings = nowarnings;
		if(IsActiveFile()) {
			Пакет::Файл& f = ActiveFile();
			f.pch = pch_file;
			f.nopch = nopch_file;
			f.noblitz = noblitz_file;
		}
		SavePackage();
	}
}

bool РедакторПакета::Ключ(dword ключ, int count)
{
	if(ключ == K_ESCAPE) {
		закрой();
		return true;
	}
	return ТопОкно::Ключ(ключ, count);
}

void РедакторПакета::Empty()
{
	FileEmpty();
	charset.откл();
	tabsize.откл();
	spellcheck_comments.откл();
	noblitz.откл();
	nowarnings.откл();
	description.откл();
	ink.откл();
	italic.откл();
	bold.откл();
	filelist.очисть();
	filelist.откл();
	option.очисть();
	option.откл();
}

void РедакторПакета::FileEmpty()
{
	fileoption.очисть();
	fileoption.откл();
	pch_file = false;
	pch_file.откл();
	nopch_file = false;
	nopch_file.откл();
	noblitz_file = false;
	noblitz_file.откл();
	includeable_file = false;
	includeable_file.откл();
}

void РедакторПакета::OptionAdd(КтрлМассив& option, int тип, const char *title, const Массив<OptItem>& o)
{
	if(o.дайСчёт() == 0)
		return;
	option.добавь(-1, -1, тип == INCLUDE ? "" : "when", title);
	option.устДисплей(option.дайСчёт() - 1, 0, Single<ДисплейЗагаОпции>());
	option.устДисплей(option.дайСчёт() - 1, 1, Single<ДисплейЗагаОпции>());
	for(int i = 0; i < o.дайСчёт(); i++) {
		option.добавь(тип, i, o[i].when, o[i].text);
		if(тип == USES)
			option.устДисплей(option.дайСчёт() - 1, 1, Single<ДисплейИспользований>());
		if(тип == FILEDEPENDS)
			option.устДисплей(option.дайСчёт() - 1, 1, Single<ДисплейЗависимостей>());
	}
}

void РедакторПакета::OptionAdd(int тип, const char *title, const Массив<OptItem>& o)
{
	OptionAdd(option, тип, title, o);
}

void РедакторПакета::PackageCursor()
{
	WorkspaceWork::PackageCursor();
	if(пусто_ли(actualpackage))
		Empty();
	else {
		description <<= actual.description;
		ink <<= actual.ink;
		bold <<= actual.bold;
		italic <<= actual.italic;
		charset <<= (int)actual.charset;
		tabsize <<= actual.tabsize;
		spellcheck_comments <<= actual.spellcheck_comments;
		noblitz = actual.noblitz;
		nowarnings = actual.nowarnings;
		Ткст s;
		for(int i = 0; i < actual.accepts.дайСчёт(); i++) {
			if(i) s << ' ';
			s << actual.accepts[i];
		}
		accepts <<= s.вШТкст();
		description.вкл();
		ink.вкл();
		bold.вкл();
		italic.вкл();
		charset.вкл();
		tabsize.вкл();
		spellcheck_comments.вкл();
		noblitz.вкл();
		nowarnings.вкл();
		accepts.вкл();
		option.вкл();
		option.очисть();
		for(int i = FLAG; i <= PKG_LAST; i++)
			OptionAdd(i, opt_name[i], *opt[i]);
	}
}

void РедакторПакета::AdjustOptionCursor(КтрлМассив& option)
{
	int q = option.дайКурсор();
	if(q < 0)
		return;
	if((int)option.дай(q, 0) < 0 && q + 1 < option.дайСчёт())
		option.устКурсор(q + 1);
}

void РедакторПакета::AdjustPackageOptionCursor()
{
	AdjustOptionCursor(option);
}

void РедакторПакета::FindOpt(КтрлМассив& option, int тип, const Ткст& when, const Ткст& text)
{
	for(int i = 0; i < option.дайСчёт(); i++)
		if((int)option.дай(i, 0) == тип &&
		   (Ткст)option.дай(i, 2) == when && (Ткст)option.дай(i, 3) == text) {
			option.устКурсор(i);
			option.устФокус();
			break;
		 }
}

void РедакторПакета::FindOpt(КтрлМассив& option, int тип, int ii)
{
	for(int i = 0; i < option.дайСчёт(); i++)
		if((int)option.дай(i, 0) == тип && (int)option.дай(i, 1) == ii) {
			option.устКурсор(i);
			option.устФокус();
			break;
		 }
}

void РедакторПакета::SetOpt(КтрлМассив& opt, int тип, OptItem& m, const Ткст& when, const Ткст& text)
{
	m.when = when;
	m.text = text;
	SaveLoadPackage();
	FindOpt(opt, тип, when, text);
}

int FlagFilterR(int c)
{
	return c == '-' ? c : FlagFilter(c);
}

void РедакторПакета::Prepare(WithUppOptDlg<ТопОкно>& dlg, int тип)
{
	CtrlLayoutOKCancel(dlg, opt_name[тип]);
	dlg.when.устФильтр(CondFilter);
	dlg.when.вкл(тип != INCLUDE);
	if(тип == FLAG) {
		dlg.text.устФильтр(FlagFilterR);
		dlg.info.устНадпись("Используйте префикс '-', чтобы удалить этот флаг");
	}
}

void РедакторПакета::AddOption(int тип)
{
	if(пусто_ли(actualpackage))
		return;
	if(тип == USES) {
		ДлгИспользований dlg;
		if(dlg.нов() && dlg.пуск() == IDOK)
			SetOpt(option, USES, actual.uses.добавь(), ~dlg.when, ~dlg.text);
		return;
	}
	WithUppOptDlg<ТопОкно> dlg;
	Prepare(dlg, тип);
	if(dlg.пуск() != IDOK)
		return;
	SetOpt(option, тип, opt[тип]->добавь(), ~dlg.when, ~dlg.text);
}

void РедакторПакета::EditOption(bool duplicate)
{
	if(!option.курсор_ли() || пусто_ли(actualpackage))
		return;
	int тип = option.дай(0);
	if(тип == USES) {
		Массив<OptItem>& m = *opt[тип];
		int i = option.дай(1);
		if(i >= 0 && i < m.дайСчёт()) {
			ДлгИспользований dlg;
			if(duplicate)
				dlg.Титул(дайТитул().вТкст() + " - duplicate");
			dlg.when <<= m[i].when;
			dlg.text <<= m[i].text;
			if(dlg.пуск() == IDOK)
				SetOpt(option, USES, duplicate ? actual.uses.добавь() : m[i], ~dlg.when, ~dlg.text);
		}
		return;
	}
	if(тип >= FLAG && тип <= PKG_LAST) {
		Массив<OptItem>& m = *opt[тип];
		int i = option.дай(1);
		if(i >= 0 && i < m.дайСчёт()) {
			WithUppOptDlg<ТопОкно> dlg;
			Prepare(dlg, тип);
			if(duplicate)
				dlg.Титул(дайТитул().вТкст() + " - duplicate");
			dlg.when <<= m[i].when;
			dlg.text <<= m[i].text;
			if(dlg.пуск() != IDOK)
				return;
			SetOpt(option, тип, duplicate ? opt[тип]->добавь() : m[i], ~dlg.when, ~dlg.text);
		}
	}
}

void РедакторПакета::RemoveOption()
{
	if(!option.курсор_ли() || пусто_ли(actualpackage))
		return;
	int тип = option.дай(0);
	if(тип >= FLAG && тип <= PKG_LAST) {
		Массив<OptItem>& m = *opt[тип];
		int i = option.дай(1);
		if(i >= 0 && i < m.дайСчёт())
			m.удали(i);
		SaveLoadPackage();
	}
}

void РедакторПакета::MoveOption(int d)
{
	if(!option.курсор_ли() || пусто_ли(actualpackage))
		return;
	int тип = option.дай(0);
	if(тип >= FLAG && тип <= PKG_LAST) {
		Массив<OptItem>& m = *opt[тип];
		int i = option.дай(1);
		if(min(i, i + d) >= 0 && max(i, i + d) < m.дайСчёт()) {
			::разверни(m[i], m[i + d]);
			SaveLoadPackage();
			FindOpt(option, тип, i + d);
		}
	}
}

void РедакторПакета::OptionMenu(Бар& bar)
{
	bool b = !пусто_ли(actualpackage);
	bar.добавь(b, "Добавить пакет..", IdeImg::package_add(), THISBACK1(AddOption, USES));
	for(int j = FLAG; j <= PKG_LAST; j++)
		if(j != USES)
			bar.добавь(b, "Новый " + opt_name[j] + "..", THISBACK1(AddOption, j));
	bar.Separator();
	b = option.курсор_ли() && (int)option.дай(0) >= 0;
	bar.добавь(b, "Редактировать..", [=] { EditOption(false); })
		.Ключ(K_CTRL_ENTER);
	bar.добавь(b, "Дублткат..", [=] { EditOption(true); })
	   .Ключ(K_CTRL_D);
	bar.добавь(b, "Удалить", THISBACK(RemoveOption))
	   .Ключ(K_DELETE);
	bar.Separator();
	int тип = option.курсор_ли() ? (int)option.дай(0) : -1;
	int i = -1;
	Массив<OptItem> *m = NULL;
	if(тип >= FLAG && тип <= PKG_LAST) {
		m = opt[тип];
		i = option.дай(1);
	}
	bar.добавь(i >= 0 && min(i, i - 1) >= 0,
	        "Поднять", THISBACK1(MoveOption, -1))
	   .Ключ(K_CTRL_UP);
	bar.добавь(m && i >= 0 && max(i, i + 1) < m->дайСчёт(),
	        "Опустить", THISBACK1(MoveOption, 1))
	   .Ключ(K_CTRL_DOWN);
}

void РедакторПакета::FileCursor()
{
	WorkspaceWork::FileCursor();
	if(IsActiveFile()) {
		Пакет::Файл& f = ActiveFile();
		if(!f.separator) {
			Ткст p = GetActiveFilePath();
			Ткст ext = впроп(дайРасшф(p));
			bool tpp = ext == ".tpp" && папка_ли(p);
			bool hdr = IsHeaderExt(ext) && !tpp;
			pch_file.вкл(hdr);
			pch_file <<= f.pch;
			nopch_file.вкл(!hdr);
			nopch_file <<= f.nopch;
			noblitz_file.вкл(!tpp);
			noblitz_file <<= f.noblitz;
			includeable_file.вкл(tpp);
			includeable_file <<= файлЕсть(приставьИмяф(p, "all.i"));
			fileoption.вкл();
			fileoption.очисть();
			OptionAdd(fileoption, FILEOPTION, "Дополнтельные опции компилятора для этого файла",
			          f.option);
			OptionAdd(fileoption, FILEDEPENDS, "Дополнительные зависимости для этого файла",
			          f.depends);
			return;
		}
	}
	FileEmpty();
}

void РедакторПакета::AdjustFileOptionCursor()
{
	AdjustOptionCursor(fileoption);
}

void РедакторПакета::FileOptionMenu(Бар& bar)
{
	bool b = IsActiveFile();
	bar.добавь(b, "Добавить флаги компилятора..", THISBACK(AddFileOption)).Ключ(K_INSERT);
	bar.добавь(b, "Добавить зависимость..", THISBACK1(AddDepends, false)).Ключ(K_CTRL_INSERT);
	bar.добавь(b, "Жлбавить внешнюю зависимость..", THISBACK1(AddDepends, true)).Ключ(K_SHIFT_INSERT);
	bar.Separator();
	b = fileoption.курсор_ли() && (int)fileoption.дай(0) >= 0;
	int тип = b ? (int)fileoption.дай(0) : -1;
	bar.добавь(b, "Редактировать..", THISBACK(EditFileOption)).Ключ(K_CTRL_ENTER);
	bar.добавь(b, "Удалить", THISBACK(RemoveFileOption)).Ключ(K_DELETE);
	bar.Separator();
	int i = -1;
	Массив<OptItem> *m = NULL;
	if(тип >= 0) {
		m = тип == FILEDEPENDS ? &ActiveFile().depends : &ActiveFile().option;
		i = fileoption.дай(1);
	}
	bar.добавь(i >= 0 && min(i, i - 1) >= 0,
	        "Поднять", THISBACK1(MoveFileOption, -1))
	   .Ключ(K_CTRL_UP);
	bar.добавь(m && i >= 0 && max(i, i + 1) < m->дайСчёт(),
	        "Опустить", THISBACK1(MoveFileOption, 1))
	   .Ключ(K_CTRL_DOWN);
}

void РедакторПакета::MoveFileOption(int d)
{
	if(!fileoption.курсор_ли() || !IsActiveFile())
		return;
	int тип = fileoption.дай(0);
	if(тип < 0)
		return;
	Массив<OptItem>& m = тип == FILEDEPENDS ? ActiveFile().depends : ActiveFile().option;
	int i = fileoption.дай(1);
	if(min(i, i + d) >= 0 && max(i, i + d) < m.дайСчёт()) {
		::разверни(m[i], m[i + d]);
		SaveLoadPackage();
		FindOpt(fileoption, тип, i + d);
	}
}

void DependsDlg::нов()
{
	FileSel *fs;
	if(!пусто_ли(package)) {
		fs = &BasedSourceFs();
		fs->BaseDir(дайПапкуФайла(PackagePath(package)));
		fs->Multi(false);
	}
	else {
		fs = &OutputFs();
		fs->Multi(false);
	}
	if(fs->ExecuteOpen("Дополнительная зависимость файла"))
		text <<= ~*fs;
	fs->Multi();
}

DependsDlg::DependsDlg()
{
	CtrlLayoutOKCancel(*this, "Дополнительная зависимость файла");
	when.устФильтр(CondFilter);
	text.устДисплей(Single<ДисплейЗависимостей>());
	text.WhenPush = THISBACK(нов);
}

void РедакторПакета::AddDepends(bool extdep)
{
	if(!IsActiveFile())
		return;
	DependsDlg dlg;
	if(!extdep)
		dlg.package = GetActivePackage();
	dlg.нов();
	if(dlg.пуск() == IDOK)
		SetOpt(fileoption, FILEDEPENDS, ActiveFile().depends.добавь(), ~dlg.when, ~dlg.text);
}

void РедакторПакета::AddFileOption()
{
	if(!IsActiveFile())
		return;
	WithUppOptDlg<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, "Опции компилятора для этого файла");
	dlg.when.устФильтр(CondFilter);
	if(dlg.пуск() == IDOK)
		SetOpt(fileoption, FILEDEPENDS, ActiveFile().option.добавь(), ~dlg.when, ~dlg.text);
}

void РедакторПакета::EditFileOption()
{
	if(!IsActiveFile() || !fileoption.курсор_ли())
		return;
	int ii = (int)fileoption.дай(1);
	Пакет::Файл& f = ActiveFile();
	if(ii < 0 || ii >= f.option.дайСчёт())
		return;
	OptItem& m = f.option[(int)fileoption.дай(1)];
	WithUppOptDlg<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, "Опции компилятора для этого файла");
	dlg.when.устФильтр(CondFilter);
	dlg.when <<= m.when;
	dlg.text <<= m.text;
	if(dlg.пуск() != IDOK)
		return;
	m.when = ~dlg.when;
	m.text = ~dlg.text;
	SaveLoadPackage();
	FindOpt(fileoption, FILEOPTION, ~dlg.when, ~dlg.text);
}

void РедакторПакета::RemoveFileOption()
{
	if(!IsActiveFile() || !fileoption.курсор_ли())
		return;
	int ii = (int)fileoption.дай(1);
	Пакет::Файл& f = ActiveFile();
	int тип = fileoption.дай(0);
	if(тип == FILEOPTION && ii >= 0 && ii < f.option.дайСчёт())
		f.option.удали(ii);
	if(тип == FILEDEPENDS && ii >= 0 && ii < f.depends.дайСчёт())
		f.depends.удали(ii);
	SaveLoadPackage();
}

void РедакторПакета::сериализуй(Поток& s) {
	int version = 3;
	s / version;
	if(version >= 3) {
		filelist.сериализуйНастройки(s);
		package.сериализуйНастройки(s);
	}
	else {
		s % filelist;
		s % package % package;
	}
	SerializePlacement(s);
	if(version >= 1 && version <= 2) {
		Сплиттер dummy;
		s % dummy % dummy % dummy % dummy % dummy % dummy;
	}
}

void РедакторПакета::добавь(const char *имя, Массив<OptItem>& m)
{
	opt.добавь(&m);
	opt_name.добавь(имя);
}

void РедакторПакета::иниц(КтрлМассив& option)
{
	option.добавьИндекс();
	option.добавьИндекс();
	option.добавьКолонку("").устДисплей(Single<ДисплейОпции>()).Margin(0);
	option.добавьКолонку("", 2).устДисплей(Single<ДисплейОпции>()).Margin(0);
	option.NoHeader();
	option.SetLineCy(max(IdeImg::Package().дайРазм().cy, Draw::GetStdFontCy()));
}

void РедакторПакета::SaveOptionsLoad()
{
	SaveOptions();
	SaveLoadPackage();
}

void РедакторПакета::Description()
{
	WithDescriptionLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, "Описание пакета");
	dlg.text <<= ~description;
	if(dlg.пуск() != IDOK)
		return;
	description <<= ~dlg.text;
	SaveOptions();
}

РедакторПакета::РедакторПакета()
{
	organizer = true;
	CtrlLayoutOKCancel(*this, "Органайзер пакета");
	description.откл();
	description <<= THISBACK(Description);
	
	spellcheck_comments.добавь(Null, "Дефолт");
	DlSpellerLangs(spellcheck_comments);
	DlCharsetD(charset);
	charset.откл();
	filelist.откл();
	spellcheck_comments.откл();
	accepts.устФильтр(FlagFilter);
	accepts ^= spellcheck_comments ^= charset ^= tabsize ^= THISFN(SaveOptions);
	noblitz <<=
	nowarnings <<=
	pch_file <<=
	nopch_file <<=
	noblitz_file <<= THISBACK(SaveOptionsLoad);
	
	includeable_file <<= THISBACK(ToggleIncludeable);

	добавь("Добавить/удалить флаги", actual.flag);
	добавь("Использования", actual.uses);
	добавь("Цели", actual.target);
	добавь("Библиотеки", actual.library);
	добавь("Статические библиотеки", actual.static_library);
	добавь("Опции компоновки", actual.link);
	добавь("Опции компилятора", actual.option);
	добавь("Внутренние включения", actual.include);
	добавь("pkg-config", actual.pkg_config);

	иниц(option);
	option.WhenCursor = THISBACK(AdjustPackageOptionCursor);
	option.WhenBar = THISBACK(OptionMenu);
	option.WhenLeftDouble << [=] { EditOption(false); };

	иниц(fileoption);
	fileoption.WhenCursor = THISBACK(AdjustFileOptionCursor);
	fileoption.WhenBar = THISBACK(FileOptionMenu);
	fileoption.WhenLeftDouble = THISBACK(EditFileOption);
	
	ink.NullText("(std)");
	ink <<=
	bold <<=
	italic <<= THISBACK(SaveOptionsLoad);
	
	merge << [=] {
		const РОбласть& wspc = GetIdeWorkspace();
		if(wspc.дайСчёт() == 0)
			return;
		Вектор<Ткст> nests = GetUppDirs();
		Ткст app_nest = нормализуйПуть(nests[0]);
		Progress pi;
		Вектор<Ткст> tocopy, tocopy_nest;
		for(int i = 0; i < wspc.дайСчёт(); i++) {
			Ткст pkg_name = wspc[i];
			Ткст pkg_dir = дайПапкуФайла(PackagePath(pkg_name));
			Ткст pkg_nest = GetPackagePathNest(pkg_dir);
			if(нормализуйПуть(GetPackagePathNest(pkg_dir)) != app_nest) {
				tocopy.добавь(pkg_name);
				tocopy_nest.добавь(pkg_nest);
			}
		}
		if(PromptOKCancel("Следующие пакеты будут скопированы в [* \1" + app_nest + "\1]:&&\1" + Join(tocopy, "\n"))) {
			Progress pi;
			for(int i = 0; i < tocopy.дайСчёт(); i++)
				копируйПапку(приставьИмяф(app_nest, tocopy[i]), приставьИмяф(tocopy_nest[i], tocopy[i]), &pi);
		}
		ScanWorkspace();
		SyncWorkspace();
	};

	FileCursor();
}

void EditPackages(const char *main, const char *startwith, Ткст& cfg) {
	InvalidatePackageCache();
	РедакторПакета pe;
	pe.Sizeable();
	ТкстПоток in(cfg);
	pe.сериализуй(in);
	pe.SetMain(main);
	pe.ScanWorkspace();
	pe.SyncWorkspace();
	pe.FindSetPackage(startwith);
	if(pe.пуск() != IDOK)
		pe.RestoreBackup();
	ТкстПоток out;
	pe.сериализуй(out);
	cfg = out;
}
