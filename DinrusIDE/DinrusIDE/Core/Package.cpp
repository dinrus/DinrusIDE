#include "Core.h"

bool IsUppValueChar(int c)
{
	return c > ' ' && c != ',' && c != ';';
}

Ткст читайЗнач(СиПарсер& p)
{
	p.пробелы();
	if(p.ткст_ли())
		return p.читай1Ткст();
	ТкстБуф v;
	while(IsUppValueChar(p.подбериСим()))
		v.конкат(p.дайСим());
	p.пробелы();
	return Ткст(v);
}

static bool sMatchOr(СиПарсер& p, const Вектор<Ткст>& flag);

static bool sMatchFlag(СиПарсер& p, const Вектор<Ткст>& flag)
{
	if(p.сим('!'))
		return !sMatchFlag(p, flag);
	if(p.сим('(')) {
		bool b = sMatchOr(p, flag);
		p.передайСим(')');
		return b;
	}
	if(p.кф_ли())
		return true;
	return найдиИндекс(flag, p.читайИд()) >= 0;
}

static bool sMatchAnd(СиПарсер& p, const Вектор<Ткст>& flag)
{
	bool b = sMatchFlag(p, flag);
	while(p.ид_ли() || p.сим_ли('!') || p.сим_ли('(') || p.сим2('&', '&') || p.сим('&'))
		b = sMatchFlag(p, flag) && b;
	return b;
}

static bool sMatchOr(СиПарсер& p, const Вектор<Ткст>& flag)
{
	bool b = sMatchAnd(p, flag);
	while(p.сим2('|', '|') || p.сим('|'))
		b = sMatchFlag(p, flag) || b;
	return b;
}

bool MatchWhen_X(const Ткст& when, const Вектор<Ткст>& flag)
{
	СиПарсер p(when);
	bool b = sMatchOr(p, flag);
	if(!p.кф_ли())
		p.выведиОш("ожидался конец выражения");
	return b;
}

bool MatchWhen(const Ткст& when, const Вектор<Ткст>& flag)
{
	try {
		return MatchWhen_X(when, flag);
	}
	catch(СиПарсер::Ошибка e) {
		вКонсоль(Ткст().конкат()
		           << "Неверное выражение When: " << какТкстСи(when) << ": " << e);
		return false;
	}
}

Ткст ReadWhen(СиПарсер& p) {
	Ткст when;
	if(p.сим('(')) {
		if(p.ткст_ли())
			when = p.читайТкст();
		else {
			const char *b = p.дайУк();
			int lvl = 0;
			for(;;) {
				if(p.кф_ли() || lvl == 0 && p.сим_ли(')'))
					break;
				if(p.сим('('))
					lvl++;
				else
				if(p.сим(')'))
					lvl--;
				else
					p.пропустиТерм();
			}
			when = Ткст(b, p.дайУк());
			p.сим(')');
		}
	}
	return when;
}

Ткст AsStringWhen(const Ткст& when) {
	Ткст out;
	try {
		Вектор<Ткст> x;
		MatchWhen_X(when, x);
		out << '(' << when << ')';
	}
	catch(СиПарсер::Ошибка) {
		out << '(' << какТкстСи(when) << ')';
	}
	return out;
}

Ткст CustomStep::какТкст() const {
	return "custom" + AsStringWhen(when) + ' ' + какТкстСи(ext) + ",\n\t" +
					  какТкстСи(command, 70, "\t") + ",\n\t" +
					  какТкстСи(output, 70, "\t") + ";\n\n";
}

void CustomStep::грузи(СиПарсер& p)
{
	when = ReadWhen(p);
	ext = p.читайТкст();
	p.передайСим(',');
	command = p.читайТкст();
	p.передайСим(',');
	output = p.читайТкст();
	p.передайСим(';');
}

Ткст CustomStep::GetExt() const {
	return ext[0] != '.' ? "." + впроп(ext) : впроп(ext);
}

bool   CustomStep::MatchExt(const char *фн) const {
	return впроп(дайРасшф(фн)) == GetExt();
}

bool LoadOpt(СиПарсер& p, const char *ключ, Массив<OptItem>& v) {
	if(p.ид(ключ)) {
		Ткст when = ReadWhen(p);
		do {
			OptItem& m = v.добавь();
			m.when = when;
			m.text = читайЗнач(p);
		}
		while(p.сим(','));
		return true;
	}
	return false;
}

bool LoadFOpt(СиПарсер& p, const char *ключ, Массив<OptItem>& v) {
	if(p.ид(ключ)) {
		OptItem& m = v.добавь();
		m.when = ReadWhen(p);
		m.text = читайЗнач(p);
		return true;
	}
	return false;
}

void Пакет::переустанов()
{
	charset = 0;
	noblitz = nowarnings = false;
	bold = italic = false;
	ink = Null;
	spellcheck_comments = Null;
	tabsize = Null;
#ifdef PLATFORM_WIN32
	cr = true;
#else
	cr = false;
#endif
}

Пакет::Пакет()
{
	переустанов();
}

bool StdResolver(const Ткст& Ошибка, const Ткст& path, int line)
{
	вКонсоль("Неверный пакет: " + path);
	exit(1);
	return false;
}

static bool (*sResolve)(const Ткст& Ошибка, const Ткст& path, int line) = StdResolver;

void Пакет::SetPackageResolver(bool (*Resolve)(const Ткст& Ошибка, const Ткст& path, int line))
{
	sResolve = Resolve;
}

byte CharsetByNameX(const Ткст& s)
{
	return decode(s, "UTF-8-BOM", CHARSET_UTF8_BOM,
	                 "UTF-16-LE", CHARSET_UTF16_LE,
	                 "UTF-16-BE", CHARSET_UTF16_BE,
	                 "UTF-16-LE-BOM", CHARSET_UTF16_LE_BOM,
	                 "UTF-16-BE-BOM", CHARSET_UTF16_BE_BOM,
	                 набсимПоИмени(s));
}

void Пакет::Опция(bool& option, const char *имя)
{
	Файл& f = file.верх();
	for(int i = 0; i < f.option.дайСчёт(); i++) // Ugly BW compatibility hack
		if(f.option[i].text == имя) {
			f.option.удали(i);
			option = true;
			break;
		}
}

bool Пакет::грузи(const char *path)
{
	for(;;) {
		переустанов();
		library.очисть();
		static_library.очисть();
		target.очисть();
		flag.очисть();
		option.очисть();
		link.очисть();
		uses.очисть();
		include.очисть();
		pkg_config.очисть();
		accepts.очисть();
		file.очисть();
		config.очисть();
		custom.очисть();
		description.очисть();
		Ткст f = загрузиФайл(path);
		cr = f.найди('\r') >= 0;
		time = дайВремяф(path);
		if(пусто_ли(time))
			return false;
		СиПарсер p(f);
		p.безПропускаКомментов(); // allow file path like //home/user/project/something.cpp
		try {
			while(!p.кф_ли()) {
				if(!LoadOpt(p, "options", option) &&
				   !LoadOpt(p, "link", link) &&
				   !LoadOpt(p, "library", library) &&
				   !LoadOpt(p, "static_library", static_library) &&
				   !LoadOpt(p, "flags", flag) &&
				   !LoadOpt(p, "target", target) &&
				   !LoadOpt(p, "uses", uses) &&
				   !LoadOpt(p, "include", include) &&
				   !LoadOpt(p, "pkg_config", pkg_config)) {
					if(p.ид("charset"))
						charset = CharsetByNameX(p.читайТкст());
					else
					if(p.ид("tabsize"))
						tabsize = minmax(p.читайЦел(), 1, 20);
					else
					if(p.ид("description")) {
						description = p.читайТкст();
						const char *q = strchr(description, 255);
						ink = Null;
						bold = italic = false;
						if(q) {
							СиПарсер p(q + 1);
							bold = p.сим('B');
							italic = p.сим('I');
							if(p.число_ли()) {
								КЗСА c = чёрный();
								c.r = p.читайЦел();
								p.сим(',');
								if(p.число_ли())
									c.g = p.читайЦел();
								p.сим(',');
								if(p.число_ли())
									c.b = p.читайЦел();
								ink = c;
							}
							description = Ткст(~description, q);
						}
					}
					else
					if(p.ид("acceptflags")) {
						do
							accepts.добавь(читайЗнач(p));
						while(p.сим(','));
					}
					else
					if(p.ид("noblitz"))
					   noblitz = true;
					else
					if(p.ид("optimize_speed"))
						; // Legacy option ignored
					else
					if(p.ид("optimize_size"))
						; // Legacy option ignored
					else
					if(p.ид("file")) {
						do {
							Файл fv(читайЗнач(p));
							if(fv.дайСчёт()) {
								Файл& f = file.добавь();
								f = pick(fv);
								while(!p.сим_ли(',') && !p.сим_ли(';') && !p.кф_ли()) {
									if(!LoadFOpt(p, "options", f.option) &&
									   !LoadFOpt(p, "depends", f.depends)) {
										if(p.ид("optimize_speed"))
											; // Legacy option ignored
										else
										if(p.ид("optimize_size"))
											; // Legacy option ignored
										else
										if(p.ид("pch"))
											f.pch = true;
										else
										if(p.ид("nopch"))
											f.nopch = true;
										else
										if(p.ид("noblitz"))
											f.noblitz = true;
										else
										if(p.ид("readonly"))
											f.readonly = true;
										else
										if(p.ид("separator"))
											f.separator = true;
										else
										if(p.ид("charset"))
											f.charset = CharsetByNameX(p.читайТкст());
										else
										if(p.ид("tabsize"))
											f.tabsize = minmax(p.читайЦел(), 1, 20);
										else
										if(p.ид("font"))
											f.font = minmax(p.читайЦел(), 0, 3);
										else
										if(p.ид("highlight"))
											f.highlight = p.читайИд();
										else
										if(p.ид("spellcheck_comments"))
											f.spellcheck_comments = LNGFromText(p.читайТкст());
										else
											p.пропустиТерм();
									}
								}
								Опция(f.pch, "PCH");
								Опция(f.nopch, "NOPCH");
								Опция(f.noblitz, "NOBLITZ");
							}
						}
						while(p.сим(','));
					}
					else
					if(p.ид("mainconfig")) {
						do {
							Ткст c = p.читайТкст();
							p.сим('=');
							p.ид("external"); // Backward compatibility...
							p.ид("console");
							p.ид("remotelinux");
							p.ид("normal");
							Ткст f = p.читайТкст();
							Конфиг& cf = config.добавь();
							cf.имя = c;
							cf.param = f;
						}
						while(p.сим(','));
					}
					else
					if(p.ид("custom"))
						custom.добавь().грузи(p);
					else
					if(p.ид("spellcheck_comments"))
						spellcheck_comments = LNGFromText(p.читайТкст());
					else
						p.пропустиТерм();
				}
				p.сим(';');
			}
			for(int i = 0; i < option.дайСчёт(); i++)
				if(option[i].when == "BUILDER_OPTION" && option[i].text == "NOWARNINGS") {
					nowarnings = true;
					option.удали(i);
					break;
				}
			return true;
		}
		catch(СиПарсер::Ошибка Ошибка) {
			if(sResolve(Ошибка, path, p.дайСтроку() - 1))
				return false;
			сохрани(path);
			return true;
		}
	}
}

Ткст WriteValue(const Ткст& x) {
	for(const char *s = x; s < x.стоп(); s++)
		if(!IsUppValueChar(*s))
			return какТкстСи(x);
	return x;
}

void putopt(Поток& out, const char *ключ, const Массив<OptItem>& m) {
	bool was = false;
	for(int i = 0; i < m.дайСчёт(); i++)
		if(пусто_ли(m[i].when)) {
			if(was)
				out << ",\n\t";
			else
				out << ключ << "\n\t";
			out << WriteValue(m[i].text);
			was = true;
		}
	if(was)
		out << ";\n\n";
	for(int i = 0; i < m.дайСчёт(); i++)
		if(!пусто_ли(m[i].when))
			out << ключ << AsStringWhen(m[i].when) << ' ' << WriteValue(m[i].text) << ";\n\n";
}

void putp(Поток& out, const char *ключ, const Вектор<Ткст>& v)
{
	if(v.дайСчёт()) {
		out << ключ << "\n";
		for(int i = 0; i < v.дайСчёт(); i++) {
			if(i) out << ",\n";
			out << '\t' << WriteValue(v[i]);
		}
		out << ";\n\n";
	}
}

void putfopt(Поток& out, const char *ключ, const Массив<OptItem>& m)
{
	for(int i = 0; i < m.дайСчёт(); i++)
		out << "\n\t\t" << ключ << AsStringWhen(m[i].when) << ' ' << WriteValue(m[i].text);
}

Ткст исрИмяНабСима(byte charset) {
	return decode(charset, CHARSET_UTF8_BOM, "UTF-8-BOM",
		                   CHARSET_UTF16_LE, "UTF-16-LE",
		                   CHARSET_UTF16_BE, "UTF-16-BE",
		                   CHARSET_UTF16_LE_BOM, "UTF-16-LE-BOM",
		                   CHARSET_UTF16_BE_BOM, "UTF-16-BE-BOM",
		                   имяНабСим(charset));
}

void PutSpellCheckComments(ТкстПоток& out, int sc)
{
	if(!пусто_ли(sc))
		out << " spellcheck_comments " << какТкстСи(sc ? LNGAsText(sc) : "");
}

bool Пакет::сохрани(const char *path) const {
	ТкстПоток out;
	if(description.дайСчёт() || italic || bold || !пусто_ли(ink)) {
		Ткст d = description;
		d.конкат(255);
		if(bold)
			d << 'B';
		if(italic)
			d << 'I';
		if(!пусто_ли(ink))
			d << (int)ink.дайК() << ',' << (int)ink.дайЗ() << ',' << (int)ink.дайС();
		out << "description " << какТкстСи(d) << ";\n\n";
	}
	if(charset > 0)
		out << "charset " << какТкстСи(исрИмяНабСима(charset)) << ";\n\n";
	if(!пусто_ли(tabsize))
		out << "tabsize " << tabsize << ";\n\n";
	if(noblitz)
		out << "noblitz;\n\n";
	if(nowarnings)
		out << "options(BUILDER_OPTION) NOWARNINGS;\n\n";
	putp(out, "acceptflags", accepts);
	putopt(out, "flags", flag);
	putopt(out, "uses", uses);
	putopt(out, "target", target);
	putopt(out, "library", library);
	putopt(out, "static_library", static_library);
	putopt(out, "options", option);
	putopt(out, "link", link);
	putopt(out, "include", include);
	putopt(out, "pkg_config", pkg_config);
	if(file.дайСчёт()) {
		out << "file\n";
		int i;
		for(i = 0; i < file.дайСчёт(); i++) {
			if(i) out << ",\n";
			const Файл& f = file[i];
			out << '\t' << WriteValue(f);
			if(f.readonly)
				out << " readonly";
			if(f.separator)
				out << " separator";
			if(f.tabsize > 0)
				out << " tabsize " << f.tabsize;
			if(f.font > 0)
				out << " font " << f.font;
			if(f.pch)
				out << " options(BUILDER_OPTION) PCH";
			if(f.nopch)
				out << " options(BUILDER_OPTION) NOPCH";
			if(f.noblitz)
				out << " options(BUILDER_OPTION) NOBLITZ";
			if(f.charset > 0)
				out << " charset " << какТкстСи(исрИмяНабСима(f.charset));
			if(!пусто_ли(f.highlight))
				out << " highlight " << f.highlight;
			PutSpellCheckComments(out, f.spellcheck_comments);
			putfopt(out, "options", f.option);
			putfopt(out, "depends", f.depends);
		}
		out << ";\n\n";
	}
	if(config.дайСчёт()) {
		out << "mainconfig\n";
		for(int i = 0; i < config.дайСчёт(); i++) {
			const Конфиг& f = config[i];
			if(i) out << ",\n";
			out << '\t' << какТкстСи(f.имя) << " = " << какТкстСи(f.param);
		}
		out << ";\n\n";
	}
	PutSpellCheckComments(out, spellcheck_comments);
	for(int i = 0; i < custom.дайСчёт(); i++)
		out << custom[i].какТкст();
	Ткст content = out.дайРез();
	if(cr)
		content.замени("\n", "\r\n");
	return сохраниИзменёнФайл(path, content);
}