#include "Core.h"

ВекторМап<Ткст, Построитель *(*)()>& BuilderMap()
{
	static ВекторМап<Ткст, Построитель *(*)()> h;
	return h;
}

void RegisterBuilder(const char *имя, Построитель *(*create)())
{
	ПРОВЕРЬ(BuilderMap().найди(имя) < 0);
	BuilderMap().добавь(имя, create);
}

Ткст FindInDirs(const Вектор<Ткст>& dir, const Ткст& file)
{
	if(!полнпуть_ли(file))
		for(int i = 0; i < dir.дайСчёт(); i++) {
			Ткст ef = CatAnyPath(dir[i], file);
			if(файлЕсть(ef))
				return ef;
		}
	return file;
}

Ткст FindCommand(const Вектор<Ткст>& exedir, const Ткст& cmdline)
{
	Ткст app;
	const char *s = cmdline;
	while(*s && (byte)*s <= ' ')
		s++;
	if(*s == '\"')
	{
		const char *b = ++s;
		while(*s && *s != '\"')
			s++;
		app = Ткст(b, s);
		if(*s)
			s++;
	}
	else
	{
		const char *b = s;
		while(*s && (byte)*s > ' ')
			s++;
		app = Ткст(b, s);
	}
	Ткст tail = s;
	Ткст фн = FindInDirs(exedir, app);
	if(!файлЕсть(фн))
#ifdef PLATFORM_WIN32
		фн = FindInDirs(exedir, форсируйРасш(app, ".exe"));
#endif
#ifdef PLATFORM_POSIX
		фн = FindInDirs(exedir, app);
#endif
	if(фн.найди(' ') >= 0)
		фн = '\"' + фн + '\"';
	return фн + tail;
}

Ткст GetMakePath(Ткст фн, bool win32)
{
	фн = UnixPath(фн);
	Ткст cd = UnixPath(дайТекДир());
	int q = cd.дайСчёт();
	if(фн.начинаетсяС(cd) && фн[q] == '/') {
		фн.удали(0, q);
		фн.вставь(0, '.');
	}
	return фн;
}

Ткст AdjustMakePath(const char *фн)
{
	Ткст out;
	for(; *фн; фн++)
		if(*фн == '$')
			out << "$$";
		else
			out << *фн;
	return out;
}

bool IsHeaderExt(const Ткст& ext)
{
	return findarg(ext, ".h", ".hpp", ".hh", ".hxx") >= 0;
}

bool сохраниИзменёнФайл(const char *path, Ткст data, bool delete_empty)
{
	if(загрузиФайл(path) == data && (файлЕсть(path) || delete_empty))
		return true;
	if(delete_empty && пусто_ли(data))
		return удалифл(path);
	else
		return сохраниФайл(path, data);
}

static int sReadCharc(СиПарсер& p)
{
	p.передайСим('\'');
	if(!IsAlpha(p.подбериСим()))
		p.выведиОш("ожидался алфавитный символ языкового кода");
	char c = p.дайСим();
	p.передайСим('\'');
	return c;
}

static void sReadLNG(СиПарсер& p, char *c)
{
	p.передайСим('(');
	c[0] = sReadCharc(p);
	p.передайСим(',');
	c[1] = sReadCharc(p);
	p.передайСим(',');
	c[2] = sReadCharc(p);
	p.передайСим(',');
	c[3] = sReadCharc(p);
}

int читайЯЗ(СиПарсер& p) {
	char c[4];
	if(p.ид("LNG_CZECH"))
		return LNG_CZECH;
	else
	if(p.ид("LNG_ENGLISH"))
		return LNG_ENGLISH;
	else
	if(p.ид("LNG_")) {
		sReadLNG(p, c);
		p.передайСим(')');
		return LNG_(c[0], c[1], c[2], c[3]);
	}
	else
	if(p.ид("LNGC_")) {
		sReadLNG(p, c);
		p.передайСим(',');
		dword l = LNGC_(c[0], c[1], c[2], c[3], p.читайЦел());
		p.передайСим(')');
		return l;
	}
	else
		p.выведиОш("неверный языковой код");
	return 0;
}

Ткст делайЯЗ(int lang)
{
	int cs = GetLNGCharset(lang);
	Ткст str;
	if(cs)
		return str << "LNGC_('"
		       << char(((lang >> 15) & 31) + 'A' - 1) << "', '"
		       << char(((lang >> 10) & 31) + 'A' - 1) << "', '"
		       << char(((lang >>  5) & 31) + 'A' - 1) << "', '"
		       << char((lang & 31) + 'A' - 1) << "', " << cs << ")";
	else
		return str << "LNG_('"
		       << char(((lang >> 15) & 31) + 'A' - 1) << "', '"
		       << char(((lang >> 10) & 31) + 'A' - 1) << "', '"
		       << char(((lang >>  5) & 31)+ 'A' - 1) << "', '"
		       << char((lang & 31) + 'A' - 1) << "')";
}

Ткст PrintTime(int time) {
	int q = time % 1000 / 10;
	time /= 1000;
	return фмт("(%d:%02d.%02d)", time / 60, time % 60, q);
}

Точка ReadNums(СиПарсер& p) {
	Точка pt;
	pt.x = p.читайЦел();
	p.передайСим(',');
	pt.y = p.читайЦел();
	return pt;
}

Точка ReadPoint(СиПарсер& p)
{
	Точка pt;
	p.передайСим('(');
	pt = ReadNums(p);
	p.передайСим(')');
	return pt;
}

bool OldLang() {
	static int q = -1;
	if(q < 0)
		q = файлЕсть(конфигФайл("oldlang"));
	return q;
}

int CharFilterCid(int c)
{
	return IsAlNum(c) || c == '_' ? c : 0;
}


bool док_ли(Ткст s)
{
	s = впроп(s);
	if(s.найди("svn") >= 0)
		return false;
	return s.найди("readme") >= 0 || s.найди("copying") >= 0 || s.найди("license") >= 0 ||
	       s.найди("authors") >= 0;
}

static void WriteByteArray(ТкстБуф& fo, const Ткст& data)
{
	int pos = 0;
	for(int p = 0; p < data.дайДлину(); p++) {
		if(pos >= 70) {
			fo << '\n';
			pos = 0;
		}
		if(pos == 0)
			fo << '\t';
		Ткст part = фмтЦел((byte)data[p]);
		fo << part << ", ";
		pos += part.дайДлину() + 2;
	}
}

Ткст BrcToC(СиПарсер& binscript, Ткст basedir)
{
	BinObjInfo info;
	info.Parse(binscript, basedir);
	ТкстБуф fo;
	for(int i = 0; i < info.blocks.дайСчёт(); i++) {
		Ткст ident = info.blocks.дайКлюч(i);
		МассивМап<int, BinObjInfo::Block>& belem = info.blocks[i];
		if(belem[0].flags & (BinObjInfo::Block::FLG_ARRAY | BinObjInfo::Block::FLG_MASK)) {
			int count = макс(belem.дайКлючи()) + 1;
			Вектор<BinObjInfo::Block *> blockref;
			blockref.устСчёт(count, 0);
			for(int a = 0; a < belem.дайСчёт(); a++) {
				BinObjInfo::Block& b = belem[a];
				blockref[b.Индекс] = &b;
			}
			for(int i = 0; i < blockref.дайСчёт(); i++)
				if(blockref[i]) {
					BinObjInfo::Block& b = *blockref[i];
					fo << "static unsigned char " << ident << "_" << i << "[] = {\n";
					Ткст data = ::загрузиФайл(b.file);
					if(data.проц_ли())
						throw Искл(фмт("Ошибка при чтении файла '%s'", b.file));
					if(data.дайДлину() != b.length)
						throw Искл(фмт("длина файла '%s' изменилась (%d -> %d) в ходе создания объекта",
							b.file, b.length, data.дайДлину()));
					b.Compress(data);
					b.length = data.дайДлину();
					data.конкат('\0');
					WriteByteArray(fo, data);
					fo << "\n};\n\n";
//					fo << какТкстСи(data, 70, "\t", ASCSTRING_OCTALHI | ASCSTRING_SMART) << ";\n\n";
				}

			fo << "int " << ident << "_count = " << blockref.дайСчёт() << ";\n\n"
			"int " << ident << "_length[] = {\n";
			for(int i = 0; i < blockref.дайСчёт(); i++)
				fo << '\t' << (blockref[i] ? blockref[i]->length : -1) << ",\n";
			fo << "};\n\n"
			"unsigned char *" << ident << "[] = {\n";
			for(int i = 0; i < blockref.дайСчёт(); i++)
				if(blockref[i])
					fo << '\t' << ident << '_' << i << ",\n";
				else
					fo << "\t0,\n";
			fo << "};\n\n";
			if(belem[0].flags & BinObjInfo::Block::FLG_MASK) {
				fo << "char *" << ident << "_files[] = {\n";
				for(int i = 0; i < blockref.дайСчёт(); i++)
					fo << '\t' << какТкстСи(blockref[i] ? дайИмяф(blockref[i]->file) : Ткст(Null)) << ",\n";
				fo << "\n};\n\n";
			}
		}
		else {
			BinObjInfo::Block& b = belem[0];
			fo << "static unsigned char " << ident << "_[] = {\n";
			Ткст data = ::загрузиФайл(b.file);
			if(data.проц_ли())
				throw Искл(фмт("Ошибка при чтении файла '%s'", b.file));
			if(data.дайДлину() != b.length)
				throw Искл(фмт("длина файла '%s' изменена (%d -> %d) в ходе создания объекта",
					b.file, b.length, data.дайДлину()));
			b.Compress(data);
			int b_length = data.дайДлину();
			data.конкат('\0');
			WriteByteArray(fo, data);
			fo << "\n};\n\n"
			<< "unsigned char *" << ident << " = " << ident << "_;\n\n"
//			fo << какТкстСи(data, 70) << ";\n\n"
			"int " << ident << "_length = " << b_length << ";\n\n";
		}
	}
	return Ткст(fo);
}

void копируйФайл(const Ткст& d, const Ткст& s, bool brc)
{
	ФайлПоиск ff(s);
	if(ff) {
		Ткст ext = впроп(дайРасшф(s));
		if(brc && ext == ".brc") {
			try {
				Ткст brcdata = загрузиФайл(s);
				if(brcdata.проц_ли())
					throw Искл(фмт("ошибка при чтении файла '%s'", s));
				СиПарсер parser(brcdata, s);
				РНЦП::сохраниФайл(d + "c", BrcToC(parser, дайДиректориюФайла(s)));
			}
			catch(Искл e) {
				вКонсоль(e);
			}
		}
		сохраниФайл(d, загрузиФайл(s));
		устФВремя(d, ff.дайВремяПоследнЗаписи());
	}
}

void копируйПапку(const char *_dst, const char *_src, Индекс<Ткст>& used, bool all, bool brc)
{
	if(дайИмяф(_src) == ".svn")
		return;
	Ткст dst = NativePath(_dst);
	Ткст src = NativePath(_src);
	if(dst == src)
		return;
	ФайлПоиск ff(приставьИмяф(src, "*"));
	bool realize = true;
	while(ff) {
		Ткст s = приставьИмяф(src, ff.дайИмя());
		Ткст d = приставьИмяф(dst, ff.дайИмя());
		if(ff.папка_ли())
			копируйПапку(d, s, used, all, brc);
		else
		if(ff.файл_ли() && (all || док_ли(s) || used.найди(s) >= 0)) {
			if(realize) {
				реализуйДир(dst);
				realize = false;
			}
			копируйФайл(d, s, brc);
		}
		ff.следщ();
	}
}

Ткст Join(const Ткст& a, const Ткст& b, const char *sep)
{
	Ткст h = a;
	if(a.дайСчёт() && b.дайСчёт())
		h << sep;
	h << b;
	return h;
}

Ткст GetExeExt()
{
#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64)
	return ".exe";
#else
	return "";
#endif
}

Ткст NormalizeExePath(Ткст exePath)
{
	if(exePath.найди(" ") >= 0) {
	#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64)
		exePath = "\"" + exePath + "\"";
	#else
		exePath.замени(" ", "\\ ");
	#endif
	}
	
	return exePath;
}

Ткст NormalizePathSeparator(Ткст path)
{
	#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64)
		path.замени("/", DIR_SEPS);
	#else
		path.замени("\\", DIR_SEPS);
	#endif
	
	return path;
}

Индекс<Ткст> pch_files;

void RegisterPCHFile(const Ткст& pch_file)
{
	pch_files.найдиДобавь(pch_file);
}

void DeletePCHFiles()
{
	for(int i = 0; i < pch_files.дайСчёт(); i++)
		удалифл(pch_files[i]);
}

Ткст GetLineEndings(const Ткст& data, const Ткст& default_eol)
{
	int q = data.найдирек("\n");
	if(q >= 0) {
		if(data.середина(q - 1, 1) == "\r")
			return "\r\n";
		else
			return "\n";
	}
	return default_eol;
}
