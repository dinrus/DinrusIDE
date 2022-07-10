#include "Builders.h"

#include "coff.h"

Ткст MaxLenString(const byte *b, int maxlen)
{
	const byte *e = b + maxlen;
	while(e > b && e[-1] == 0)
		e--;
	return Ткст(b, int(e - b));
}

Ткст COFFSymbolName(const COFF_IMAGE_SYMBOL& sym, const char *strtbl)
{
	Ткст имя;
	if(sym.N.Имя.Short)
		имя = MaxLenString(sym.N.ShortName, 8);
	else
		имя = strtbl + sym.N.Имя.Long;
	return имя;
}

#ifdef PLATFORM_WIN32
static bool HasTail(Ткст s, const char *tail)
{
	int tl = (int)strlen(tail);
	int sl = s.дайДлину();
	if(sl < tl)
		return false;
	for(const char *p = s.дайОбх(sl - tl); *p; p++, tail++)
		if(*tail != '*' && *tail != *p)
			return false;
	return *tail == 0;
}
#endif

static void AddObjectExports(const char *path, Индекс<Ткст>& out)
{
#ifdef PLATFORM_WIN32
	КартФайл mapping;
	if(!mapping.открой(path))
		return;
	const byte *begin = mapping.старт();
	if(!begin)
		return;
	const COFF_IMAGE_FILE_HEADER *hdr = (const COFF_IMAGE_FILE_HEADER *)begin;
	if(hdr->Machine != COFF_IMAGE_FILE_MACHINE_I386)
		return;
	const COFF_IMAGE_SECTION_HEADER *sechdr = (const COFF_IMAGE_SECTION_HEADER *)(begin
		+ sizeof(COFF_IMAGE_FILE_HEADER) + hdr->SizeOfOptionalHeader);
	Индекс<int> code_sections;
	for(int i = 0; i < hdr->NumberOfSections; i++)
		if(sechdr[i].Characteristics & COFF_IMAGE_SCN_CNT_CODE)
			code_sections.добавь(i + 1);
	const COFF_IMAGE_SYMBOL *symtbl = (const COFF_IMAGE_SYMBOL *)(begin + hdr->PointerToSymbolTable);
	const char *strtbl = (const char *)(symtbl + hdr->NumberOfSymbols);
	for(int i = 0; i < (int)hdr->NumberOfSymbols; i++)
	{
		const COFF_IMAGE_SYMBOL& sym = symtbl[i];
		if(sym.StorageClass == COFF_IMAGE_SYM_CLASS_EXTERNAL && code_sections.найди(sym.SectionNumber) >= 0)
		{
			Ткст имя = COFFSymbolName(sym, strtbl);
			if(!HasTail(имя, "AEPAXI@Z"))
			{
				if(*имя == '_' && имя.найди('@') < 0)
					имя.удали(0, 1);
				out.найдиДобавь(имя);
			}
		}
		i += sym.NumberOfAuxSymbols;
	}
#endif
}

void   MscBuilder::добавьФлаги(Индекс<Ткст>& cfg)
{
	cfg.найдиДобавь("MSC");
}

Ткст MscBuilder::CmdLine(const Ткст& package, const Пакет& pkg)
{
	Ткст cc;
	if(естьФлаг("ARM"))
		cc = "clarm";
	else
	if(естьФлаг("MIPS"))
		cc = "clmips";
	else
	if(естьФлаг("SH3"))
		cc = "shcl /Qsh3";
	else
	if(естьФлаг("SH4"))
		cc = "shcl /Qsh4";
	else
	if(естьФлаг("MSC8ARM"))
		cc = "cl -GS- ";
	else
		cc = естьФлаг("INTEL") ? "icl" : "cl";
// TRC 080605-documentation says Wp64 works in 32-bit compilation only
//	cc << (IsMsc64() ? " -nologo -Wp64 -W3 -GR -c" : " -nologo -W3 -GR -c");
	cc << " -nologo -W" << (pkg.nowarnings ? "0" : "3") << " -GR -c";
	cc << IncludesDefinesTargetTime(package, pkg);

	return cc;
}

Ткст MscBuilder::MachineName() const
{
	if(естьФлаг("ARM"))     return "ARM";
	if(естьФлаг("MIPS"))    return "MIPS";
	if(естьФлаг("SH3"))     return "SH3";
	if(естьФлаг("SH4"))     return "SH4";
	if(IsMscArm())         return "ARM";
	if(IsMsc64())          return "x64";
	if(естьФлаг("WIN32"))   return "I386";
	return "IX86";
}

bool MscBuilder::IsMsc89() const
{
	return IsMsc86() || IsMsc64() || IsMscArm();
}

bool MscBuilder::IsMsc86() const
{
	return естьФлаг("MSC8") || естьФлаг("MSC9") || естьФлаг("MSC10") || естьФлаг("MSC11")
		|| естьФлаг("MSC12") || естьФлаг("MSC15") || естьФлаг("MSC14") || естьФлаг("MSC17")
		|| естьФлаг("MSC19") || естьФлаг("MSC22");
}

bool MscBuilder::IsMscArm() const
{
	return естьФлаг("MSC8ARM") || естьФлаг("MSC9ARM");
}

bool MscBuilder::IsMsc64() const
{
	return естьФлаг("MSC8X64") || естьФлаг("MSC9X64") || естьФлаг("MSC10X64") || естьФлаг("MSC11X64")
		|| естьФлаг("MSC12X64") || естьФлаг("MSC14X64") || естьФлаг("MSC15X64") || естьФлаг("MSC17X64")
		|| естьФлаг("MSC19X64") || естьФлаг("MSC22X64");
}

Ткст MscBuilder::LinkerName() const
{
	if(естьФлаг("ULD")) return "uld";
	if(естьФлаг("INTEL")) return "xilink";
	return "link";
}

static bool sContainsPchOptions(const Ткст& x)
{
	Индекс<Ткст> a(разбей(x, ' '));
	return  a.найди("-GL") >= 0 || a.найди("/GL") >= 0 || a.найди("-Y-") >= 0 || a.найди("/Y-") >= 0
	     || a.найди("-Yc") >= 0 || a.найди("/Yc") >= 0 || a.найди("-Yd") >= 0 || a.найди("/Yd") >= 0
	     || a.найди("-Yl") >= 0 || a.найди("/Yl") >= 0 || a.найди("-Yu") >= 0 || a.найди("/Yu") >= 0
	     || a.найди("-YX") >= 0 || a.найди("/YX") >= 0;
}

Ткст MscBuilder::Pdb(Ткст package, int slot, bool separate_pdb) const
{
	Ткст pdb_name = GetAnyFileName(package);
	if(separate_pdb)
		pdb_name << '-' << (slot + 1);
	return " -Gy -Fd" + GetPathQ(CatAnyPath(outdir, pdb_name + ".pdb"));
}

void DeletePCHFile(const Ткст& pch_file)
{
	DeleteFile(pch_file);
	PutVerbose("Deleting precompiled header: " + pch_file);
}

bool MscBuilder::постройПакет(const Ткст& package, Вектор<Ткст>& linkfile, Вектор<Ткст>& immfile,
    Ткст& linkoptions, const Вектор<Ткст>& all_uses, const Вектор<Ткст>& all_libraries, int opt)
{
	if(естьФлаг("MAKE_MLIB") && !естьФлаг("MAIN"))
		return true;
	
	SaveBuildInfo(package);

	int i;
	Ткст packagepath = PackagePath(package);
	Пакет pkg;
	pkg.грузи(packagepath);
	Ткст packagedir = дайПапкуФайла(packagepath);
	ChDir(packagedir);
	PutVerbose("cd " + packagedir);
	начниГруппуКонсИср(package);
	Вектор<Ткст> obj;

	bool is_shared = естьФлаг("SO"),
		is_clr = естьФлаг("CLR");

	Вектор<Ткст> sfile, isfile;
	Вектор<Ткст> soptions, isoptions;
	Вектор<Ткст> sobjfile;
	bool           Ошибка = false;

	Ткст pch_header;
	
	Индекс<Ткст> nopch, noblitz;

	bool blitz = естьФлаг("BLITZ");
	bool release = !естьФлаг("DEBUG");

	for(i = 0; i < pkg.дайСчёт(); i++) {
		if(!строитсяИср())
			return false;
		if(!pkg[i].separator) {
			Ткст gop = Gather(pkg[i].option, config.дайКлючи());
			Вектор<Ткст> srcfile = CustomStep(pkg[i], package, Ошибка);
			if(srcfile.дайСчёт() == 0)
				Ошибка = true;
			for(int j = 0; j < srcfile.дайСчёт(); j++) {
				Ткст фн = нормализуйПуть(srcfile[j]);
				Ткст ext = впроп(дайРасшф(фн));
				if(findarg(ext, ".c", ".cpp", ".cc", ".cxx", ".rc", ".brc") >= 0 ||
				   (!release && blitz && ext == ".icpp")) {
					if(найдиИндекс(sfile, фн) < 0) {
						sfile.добавь(фн);
						soptions.добавь(gop);
					}
				}
				else
				if(ext == ".icpp") {
					isfile.добавь(фн);
					isoptions.добавь(gop);
				}
				else
				if(ext == ".obj")
					obj.добавь(фн);
				else
				if(ext == ".lib")
					linkfile.добавь(фн);
				else
				if(IsHeaderExt(ext) && pkg[i].pch && allow_pch && IsMsc89() && release && !HasAnyDebug() && !blitz) {
					if(pch_header.дайСчёт())
						вКонсоль(дайИмяф(фн) + ": multiple PCHs are not allowed. Check your package configuration");
					else
						pch_header = фн;
				}
				if(pkg[i].nopch) {
					nopch.добавь(фн);
					if(allow_pch && release)
						noblitz.добавь(фн);
				}
				if(pkg[i].noblitz)
					noblitz.добавь(фн);
				if(ext == ".c")
					nopch.добавь(фн);
			}
		}
	}

	Ткст cc = CmdLine(package, pkg);
	if(естьФлаг("EVC")) {
		if(!естьФлаг("SH3") && !естьФлаг("SH4"))
			cc << " -Gs8192"; // disable stack checking
		cc << " -GF" // read-only string pooling
		      " -GX-"; // turn off exception handling
	}
	else
	if(is_clr)
		cc << " -EHac";
	else
	if(IsMsc89())
		cc << " -EHsc";
	else
		cc << " -GX";
//	Ткст pdb = GetPathQ(CatAnyPath(outdir, GetAnyFileName(package) + ".pdb"));
//	Ткст pch;
//	if(!естьФлаг("MSC8")) // MSC8 does not support automatic precompiled headers...
//		pch << " -YX -Fp" << GetPathQ(CatAnyPath(outdir, GetAnyFileName(package) + ".pch")) << ' ';
//	cc << " -Gy -Fd" << pdb;
//	if(естьФлаг("SSE2") && !IsMsc64())
//		cc << " /arch:SSE2";
	if(естьФлаг("DEBUG_MINIMAL"))
		cc << " -Zd";
	if(естьФлаг("DEBUG_FULL"))
		cc << " -Zi";
	cc << ' ' << Gather(pkg.option, config.дайКлючи());
	cc << (естьФлаг("SHARED") || is_shared || is_clr ? " -MD" : " -MT");

	Ткст cc_size = cc;
	Ткст cc_speed = cc;

	if(release)
		cc << ' ' << release_options;
	else
		cc << "d " << debug_options;
	
	int recompile = 0;
	Blitz b;
	if(blitz) {
		BlitzBuilderComponent bc(this);
		b = bc.MakeBlitzStep(sfile, soptions, obj, immfile, ".obj", noblitz, package);
		recompile = b.build;
	}

	for(i = 0; i < sfile.дайСчёт(); i++) {
		Ткст фн = sfile[i];
		Ткст ext = впроп(дайРасшф(фн));
		if(findarg(ext, ".rc", ".brc", ".c") < 0 && HdependFileTime(sfile[i]) > дайФВремя(CatAnyPath(outdir, дайТитулф(фн) + ".obj")))
			recompile++;
	}

	Ткст pch_use;
	Ткст pch_file;
	
	if(pch_header.дайСчёт()) {
		Ткст pch_obj = CatAnyPath(outdir, дайТитулф(pch_header) + "$pch.obj");
		pch_file = CatAnyPath(outdir, дайТитулф(pch_header) + ".pch");
		RegisterPCHFile(pch_file);
		Ткст pch_common = GetPathQ(pch_header) + " -Fp" + GetPathQ(pch_file) + " -FI" + GetPathQ(pch_header);
		
		if(blitz) // enable MK__s macros
			pch_common.конкат(" -DBLITZ_INDEX__=FPCH");

		if(recompile > 0 || !файлЕсть(pch_file)) {
			int pch_slot = разместиСлот();
			ТкстБуф sb;
			sb << Join(cc, cpp_options) << Pdb(package, pch_slot, false) << " -Yc" << pch_common
			   << " -Tp " << GetPathQ(pch_header) << " -Fo" + GetPathQ(pch_obj);
			вКонсоль("Прекомпилирование заголовка: " + дайИмяф(pch_header));
			if(pch_slot < 0 || !пуск(~sb, pch_slot, pch_obj, 1))
				Ошибка = true;
			жди();
		}

		pch_use = " -Yu" + pch_common;
		obj.добавь(pch_obj);
	}

	if(blitz && b.build) {
		вКонсоль("BLITZ:" + b.info);
		int slot = разместиСлот();
		Ткст c = Join(cc, cpp_options);
		if(HasAnyDebug())
			c << Pdb(package, slot, false);
		if(slot < 0 ||
		   !пуск(c + " -Tp " + GetPathQ(b.path) + " -Fo" + GetPathQ(b.object),
		        slot, b.object, b.count))
			Ошибка = true;
	}

	int first_ifile = sfile.дайСчёт();
	sfile.приставьПодбор(pick(isfile));
	soptions.приставьПодбор(pick(isoptions));

	int ccount = 0;

	for(i = 0; i < sfile.дайСчёт(); i++) {
		if(!строитсяИср())
			return false;
		Ткст фн = sfile[i];
		Ткст ext = впроп(дайРасшф(фн));
		bool rc = (ext == ".rc");
		bool brc = (ext == ".brc");
		bool init = (i >= first_ifile);
		Ткст objfile = CatAnyPath(outdir, SourceToObjName(package, фн) + (rc ? "$rc.obj" : brc ? "$brc.obj" : ".obj"));
		if(HdependFileTime(фн) > дайФВремя(objfile)) {
			int time = msecs();
			bool execerr = false;
			if(rc) {
				вКонсоль(дайПозИмяф(фн));
				int slot = разместиСлот();
				if(slot < 0 || !пуск("rc /fo" + GetPathQ(objfile) + Includes(" /i", package, pkg)
				    + DefinesTargetTime(" /d", package, pkg) + (естьФлаг("DEBUG")?" /d_DEBUG":"")
					+ ' ' + GetPathQ(фн), slot, objfile, 1))
					execerr = true;
			}
			else
			if(brc) {
				try {
//					Ткст hfn = GetHostPath(фн);
					Ткст brcdata = загрузиФайл(фн);
					if(brcdata.проц_ли())
						throw Искл(фмт("Ошибка при чтении файла '%s'", фн));
					СиПарсер parser(brcdata, фн);
					Ткст fo = BrcToC(parser, дайДиректориюФайла(фн));
					Ткст tmpfile = форсируйРасш(objfile, ".c");
					сохраниФайл(tmpfile, fo);
					int slot = разместиСлот();
					ТкстБуф cmdline;
					cmdline << cc << Pdb(package, slot, false)
					        << " -Tc " << GetPathQ(tmpfile) << " -Fo" << GetPathQ(objfile);
					if(slot < 0 || !пуск(Ткст(cmdline), slot, objfile, 1))
						throw Искл(фмт("Ошибка при компилировании двоичного объекта '%s'.", objfile));
				}
				catch(Искл e) {
					вКонсоль(e);
					execerr = true;
				}
			}
			else {
				Ткст c = cc;
				int slot = разместиСлот();
				if(HasAnyDebug())
					c << Pdb(package, slot, !sContainsPchOptions(cc) && !sContainsPchOptions(soptions[i]));
				c << " " + soptions[i] + (ext == ".c" ? Join(c_options, " -Tc") : Join(cpp_options, " -Tp")) + ' '
				     + GetPathQ(фн) + " -Fo" + GetPathQ(objfile);
				if(nopch.найди(фн) < 0)
					c << pch_use;
				if(slot < 0 || !пуск(c, slot, objfile, 1))
					execerr = true;
			}
			if(execerr)
				DeleteFile(objfile);
			Ошибка |= execerr;
			PutVerbose("скомпилировано за " + GetPrintTime(time));
			ccount++;
		}
		if(init)
			linkfile.добавь(objfile);
		else
			obj.добавь(objfile);
		immfile.добавь(objfile);
	}
	if(Ошибка) {
		завершиГруппуКонсИср();
		return false;
	}

	bool making_lib = естьФлаг("MAKE_LIB") || естьФлаг("MAKE_MLIB");

	if(!making_lib) {
		Вектор<Ткст> pkglibs = разбей(Gather(pkg.library, config.дайКлючи()), ' ');
		for(int i = 0; i < pkglibs.дайСчёт(); i++) {
			Ткст libfile = приставьРасш(pkglibs[i], ".lib");
			if(!полнпуть_ли(libfile)) {
				for(int p = 0; p < libpath.дайСчёт(); p++) {
					Ткст nf = нормализуйПуть(libfile, libpath[p]);
					if(файлЕсть(nf)) {
						libfile = nf;
						break;
					}
				}
			}
			linkfile.добавь(libfile);
		}
	}
	linkoptions << ' ' << Gather(pkg.link, config.дайКлючи());
	
//	if(pch_file.дайСчёт())
//		OnFinish(callback1(DeletePCHFile, pch_file));

	if(!естьФлаг("MAIN")) {
		if(естьФлаг("BLITZ") || естьФлаг("NOLIB") || making_lib) {
			linkfile.приставь(obj);
//			ShowTime(ccount, time);
			завершиГруппуКонсИср();
			return true;
		}
		Ткст product;
		if(is_shared)
			product = GetSharedLibPath(package);
		else
			product = CatAnyPath(outdir, GetAnyFileName(package) + ".lib");
		Время producttime = дайФВремя(product);
		if(obj.дайСчёт()) {
			Ткст h = форсируйРасш(product, ".lib");
			linkfile.добавь(h);
			immfile.добавь(h);
		}
		if(!жди()) {
			завершиГруппуКонсИср();
			return false;
		}
		Вектор<Хост::ИнфОФайле> objinfo = host->дайИнфОФайле(obj);
		for(int i = 0; i < obj.дайСчёт(); i++)
			if(objinfo[i] > producttime)
				return CreateLib(product, obj, all_uses, all_libraries, Gather(pkg.link, config.дайКлючи()));
		return true;
	}

	завершиГруппуКонсИср();
	obj.приставь(linkfile);
	linkfile = pick(obj);
	return true;
}

bool MscBuilder::CreateLib(const Ткст& product, const Вектор<Ткст>& obj,
                           const Вектор<Ткст>& all_uses, const Вектор<Ткст>& all_libraries,
                           const Ткст& link_options)
{
	int linktime = msecs();
	bool isgemsc10 = естьФлаг("MSC10") || естьФлаг("MSC10X64")
	    || естьФлаг("MSC11") || естьФлаг("MSC11X64")
		|| естьФлаг("MSC12") || естьФлаг("MSC12X64")
		|| естьФлаг("MSC14") || естьФлаг("MSC14X64")
		|| естьФлаг("MSC15") || естьФлаг("MSC15X64")
		|| естьФлаг("MSC17") || естьФлаг("MSC17X64")
		|| естьФлаг("MSC19") || естьФлаг("MSC19X64")
		|| естьФлаг("MSC22") || естьФлаг("MSC22X64")
	;
	bool is_shared = естьФлаг("SO");
	Ткст linker, lib;
	if(is_shared) {
		linker << LinkerName() << " -dll -nologo ";
		lib << "-machine:" << MachineName()
			<< " -pdb:" << GetPathQ(форсируйРасш(product, ".pdb"))
			<< " -out:" << GetPathQ(product);
		if(!isgemsc10)
			lib << " -incremental:no";
		if(HasAnyDebug())
			lib << " -debug -OPT:NOREF";
		else
			lib << " -release -OPT:REF,ICF";
		if(IsMscArm())
			lib <<  " -subsystem:windowsce,4.20 /ARMPADCODE";
		else
		if(естьФлаг("GUI"))
			lib << (естьФлаг("WIN32") ? " -subsystem:windows"
			                         : " -subsystem:windowsce");
		else
			lib << " -subsystem:console";
		Индекс<Ткст> exports;
		for(int o = 0; o < obj.дайСчёт(); o++)
			AddObjectExports(obj[o], exports);
		Ткст def;
		def << "LIBRARY " << какТкстСи(дайИмяф(product)) << "\n\n"
			"EXPORTS\n";
		for(int o = 0; o < exports.дайСчёт(); o++)
			def << '\t' << exports[o] << "\n"; //" @" << (o + 1) << "\n";
		Ткст deffile = форсируйРасш(product, ".def");
		if(!сохраниИзменёнФайл(deffile, def))
		{
			вКонсоль(фмт("%s: Ошибка при сохранении файла", deffile));
			return false;
		}
		lib << " -def:" << GetPathQ(deffile);
		for(int i = 0; i < libpath.дайСчёт(); i++)
			lib << " -LIBPATH:" << GetPathQ(libpath[i]);
		lib << ' ' << link_options;
		for(int i = 0; i < all_uses.дайСчёт(); i++)
			lib << ' ' << GetPathQ(форсируйРасш(GetSharedLibPath(all_uses[i]), ".lib"));
		for(int i = 0; i < all_libraries.дайСчёт(); i++) {
			Ткст libfile = приставьРасш(all_libraries[i], ".lib");
			if(!полнпуть_ли(libfile)) {
				for(int p = 0; p < libpath.дайСчёт(); p++) {
					Ткст nf = нормализуйПуть(libfile, libpath[p]);
					if(файлЕсть(nf)) {
						libfile = nf;
						break;
					}
				}
			}
			lib << ' ' << GetPathQ(libfile);
		}
	}
	else{
		linker << (естьФлаг("INTEL") ? "xilib" : "link /lib") << " -nologo ";
		lib << " -out:" << GetPathQ(product) << ' ' << link_options;
	}
	for(int i = 0; i < obj.дайСчёт(); i++)
		lib << ' ' << GetPathQ(obj[i]);
	вКонсоль("Создаётся библиотека...");
	завершиГруппуКонсИср();
	DeleteFile(product);
	Ткст tmpFileName;
	if(linker.дайСчёт() + lib.дайСчёт() >= 8192)
	{
		tmpFileName = дайВремИмяф();
		// we can't simply put all data on a single line
		// as it has a limit of around 130000 chars too, so we split
		// in multiple lines
		ФайлВывод f(tmpFileName);
		while(lib != "")
		{
			int found = 0;
			bool quotes = false;
			int lim = min(8192, lib.дайСчёт());
			for(int i = 0; i < lim; i++)
			{
				char c = lib[i];
				if(isspace(c) && !quotes)
					found = i;
				else if(c == '"')
					quotes = !quotes;
			}
			if(!found)
				found = lib.дайСчёт();
			f.PutLine(lib.лево(found));
			lib.удали(0, found);
		}
		f.закрой();
		linker << "@" << tmpFileName;
	}
	else
		linker << lib;
	bool res = выполни(linker);
	if(tmpFileName != "")
		удалифл(tmpFileName);
	if(res) {
		DeleteFile(product);
		return false;
	}
	else
	if((IsMsc86() || IsMsc64()) && is_shared) {
		Ткст mt("mt -nologo -manifest ");
		mt << GetPathQ(product) << ".manifest -outputresource:" << GetPathQ(product) << ";2";
		выполни(mt);
	}
	вКонсоль(Ткст().конкат() << product << " (" << дайИнфОФайле(product).length
	           << " B) создан(о) за " << GetPrintTime(linktime));
	return true;
}

bool MscBuilder::Link(const Вектор<Ткст>& linkfile, const Ткст& linkoptions, bool createmap)
{
	int time = msecs();
	if(!жди())
		return false;

	PutLinking();

	if(естьФлаг("MAKE_MLIB") || естьФлаг("MAKE_LIB"))
		return CreateLib(форсируйРасш(target, ".lib"), linkfile, Вектор<Ткст>(), Вектор<Ткст>(), linkoptions);

	bool isgemsc10 = естьФлаг("MSC10") || естьФлаг("MSC10X64")
	    || естьФлаг("MSC11") || естьФлаг("MSC11X64")
	    || естьФлаг("MSC12") || естьФлаг("MSC12X64")
	    || естьФлаг("MSC14") || естьФлаг("MSC14X64")
		|| естьФлаг("MSC15") || естьФлаг("MSC15X64")
		|| естьФлаг("MSC17") || естьФлаг("MSC17X64")
		|| естьФлаг("MSC19") || естьФлаг("MSC19X64")
		|| естьФлаг("MSC22") || естьФлаг("MSC22X64")
	;
	for(int i = 0; i < linkfile.дайСчёт(); i++)
		if(дайФВремя(linkfile[i]) > targettime) {
			Ткст link, lib;
			link << LinkerName() << " -nologo -machine:" << MachineName()
			<< " -pdb:" << GetPathQ(форсируйРасш(target, ".pdb"))
			<< " -out:" << GetPathQ(target);
			if(!isgemsc10)
				if(HasAnyDebug())
					link << " -incremental:yes -debug -OPT:NOREF";
				else
					link << " -incremental:no -release -OPT:REF,ICF";
			else
				if(HasAnyDebug())
					link << " -debug -OPT:NOREF";
				else
					link << " -release -OPT:REF,ICF";
			if(IsMscArm())
				link << " -subsystem:windowsce,4.20 /ARMPADCODE -NODEFAULTLIB:\"oldnames.lib\" ";
			else
			if(естьФлаг("GUI") || IsMscArm())
				link << " -subsystem:windows";
			else
				link << " -subsystem:console";
			if(!IsMsc64())
				link << ",5.01"; //,5.01 needed to support WindowsXP
			if(createmap)
				link << " -MAP";
			if(естьФлаг("DLL"))
				link << " -DLL";

			for(i = 0; i < libpath.дайСчёт(); i++)
				link << " -LIBPATH:\"" << libpath[i] << '\"';
			link << ' ' << linkoptions << ' ';
			for(i = 0; i < linkfile.дайСчёт(); i++)
				lib << ' ' << GetPathQ(приставьРасш(linkfile[i], ".lib"));
			вКонсоль("Компоновка...");
			bool Ошибка = false;

			Ткст tmpFileName;
			if(link.дайСчёт() + lib.дайСчёт() >= 8192)
			{
				tmpFileName = дайВремИмяф();
				// we can't simply put all data on a single line
				// as it has a limit of around 130000 chars too, so we split
				// in multiple lines
				ФайлВывод f(tmpFileName);
				while(lib != "")
				{
					int found = 0;
					bool quotes = false;
					int lim = min(8192, lib.дайСчёт());
					for(int i = 0; i < lim; i++)
					{
						char c = lib[i];
						if(isspace(c) && !quotes)
							found = i;
						else if(c == '"')
							quotes = !quotes;
					}
					if(!found)
						found = lib.дайСчёт();
					f.PutLine(lib.лево(found));
					lib.удали(0, found);
				}
				f.закрой();
				link << "@" << tmpFileName;
			}
			else
				link << lib;

			CustomStep(".pre-link", Null, Ошибка);
			if(!Ошибка && выполни(link) == 0) {
				CustomStep(".post-link", Null, Ошибка);
				if((IsMsc86() || IsMsc64()) && естьФлаг("SHARED")) {
					Ткст mt("mt -nologo -manifest ");
					mt << GetPathQ(target) << ".manifest -outputresource:" << GetPathQ(target)
				           << (естьФлаг("DLL") ? ";2" : ";1");
				   выполни(mt);
				}
				вКонсоль(Ткст().конкат() << target << " (" << дайИнфОФайле(target).length
				           << " B) скомпоновано за " << GetPrintTime(time));
			}
			else {
				DeleteFile(target);
				Ошибка = true;
			}
			if(tmpFileName != "")
				удалифл(tmpFileName);
			return !Ошибка;
		}
	вКонсоль(Ткст().конкат() << target << " (" << дайИнфОФайле(target).length
	           << " B) ещё свежий.");
	return true;
}

bool MscBuilder::Preprocess(const Ткст& package, const Ткст& file, const Ткст& target, bool)
{
	ФайлВывод out(target);
	Ткст packagepath = PackagePath(package);
	Пакет pkg;
	pkg.грузи(packagepath);
	return выполни(CmdLine(package, pkg) + " -E " + file, out);
}

Построитель *CreateMscBuilder()
{
	return new MscBuilder;
}

ИНИЦИАЛИЗАТОР(MscBuilder)
{
	RegisterBuilder("MSC71", CreateMscBuilder);
	RegisterBuilder("MSC8", CreateMscBuilder);
	RegisterBuilder("MSC8X64", CreateMscBuilder);
	RegisterBuilder("MSC8ARM", CreateMscBuilder);
	RegisterBuilder("MSC9", CreateMscBuilder);
	RegisterBuilder("MSC9X64", CreateMscBuilder);
	RegisterBuilder("MSC9ARM", CreateMscBuilder);
	RegisterBuilder("MSC10", CreateMscBuilder);
	RegisterBuilder("MSC10X64", CreateMscBuilder);
	RegisterBuilder("MSC11", CreateMscBuilder);
	RegisterBuilder("MSC11X64", CreateMscBuilder);
	RegisterBuilder("MSC12", CreateMscBuilder);
	RegisterBuilder("MSC12X64", CreateMscBuilder);
	RegisterBuilder("MSC14", CreateMscBuilder);
	RegisterBuilder("MSC14X64", CreateMscBuilder);
	RegisterBuilder("MSC15", CreateMscBuilder);
	RegisterBuilder("MSC15X64", CreateMscBuilder);
	RegisterBuilder("MSC17", CreateMscBuilder);
	RegisterBuilder("MSC17X64", CreateMscBuilder);
	RegisterBuilder("MSC19", CreateMscBuilder);
	RegisterBuilder("MSC19X64", CreateMscBuilder);
	RegisterBuilder("MSC22", CreateMscBuilder);
	RegisterBuilder("MSC22X64", CreateMscBuilder);
	RegisterBuilder("EVC_ARM", CreateMscBuilder);
	RegisterBuilder("EVC_MIPS", CreateMscBuilder);
	RegisterBuilder("EVC_SH3", CreateMscBuilder);
	RegisterBuilder("EVC_SH4", CreateMscBuilder);
	RegisterBuilder("INTEL", CreateMscBuilder);
}
