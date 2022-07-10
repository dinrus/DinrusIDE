#include "Builders.h"
#include "BuilderUtils.h"

void   DinrusBuilder::добавьФлаги(Индекс<Ткст>& cfg)
{
}

Ткст DinrusBuilder::CompilerName() const
{
	if(!пусто_ли(compiler)) return compiler;
	return "drc";
}

Ткст DinrusBuilder::CmdLine(const Ткст& package, const Пакет& pkg)
{
	Ткст cc = CompilerName();
	cc << " -c";
	for(Ткст s : pkg_config)
		cc << " `pkg-config --cflags " << s << "`";
	cc << ' ' << IncludesDefinesTargetTime(package, pkg);
	return cc;
}

void DinrusBuilder::BinaryToObject(Ткст objfile, СиПарсер& binscript, Ткст basedir,
                                const Ткст& package, const Пакет& pkg)
{
	Ткст fo = BrcToC(binscript, basedir);
	Ткст tmpfile = форсируйРасш(objfile, ".d");
	сохраниФайл(tmpfile, fo);
	Ткст cc = CmdLine(package, pkg);
	cc << " -c -o " << GetPathQ(objfile) << " -x c " << GetPathQ(tmpfile);
	int slot = разместиСлот();
	if(slot < 0 || !пуск(cc, slot, objfile, 1))
		throw Искл(фмт("Ошибка при компиляции бинарного объекта '%s'.", objfile));
}

bool DinrusBuilder::постройПакет(const Ткст& package, Вектор<Ткст>& linkfile, Вектор<Ткст>& immfile,
	Ткст& linkoptions, const Вектор<Ткст>& all_uses, const Вектор<Ткст>& all_libraries,
	int opt)
{
	if(естьФлаг("MAKE_MLIB") && !естьФлаг("MAIN"))
		return true;

	if(естьФлаг("OSX") && естьФлаг("GUI")) {
		Ткст folder;
		Ткст имя = дайИмяф(target);
		if(дайРасшф(target) == ".app")
			target = target + "/Contents/MacOS/" + дайТитулф(target);
		else
			target = target + ".app/Contents/MacOS/" + дайИмяф(target);
		RealizePath(target);
	}

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

	bool is_shared = естьФлаг("SO");
	Ткст shared_ext = (естьФлаг("WIN32") ? ".dll" : ".so");

	Вектор<Ткст> sfile, isfile;
	Вектор<Ткст> soptions, isoptions;
	bool           Ошибка = false;

	Ткст pch_header;
	
	Индекс<Ткст> nopch, noblitz;

	bool blitz = естьФлаг("BLITZ");
	bool release = !естьФлаг("DEBUG");
	bool objectivec = естьФлаг("OBJC");
	
	if(естьФлаг("OSX"))
		objectivec = true;

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
				Ткст ext = GetSrcType(фн);
				if(findarg(ext, ".c", ".cpp", ".cc", ".cxx", ".brc", ".s", ".ss") >= 0 ||
				   objectivec && findarg(ext, ".mm", ".m") >= 0 ||
				   (!release && blitz && ext == ".icpp") ||
				   ext == ".rc" && естьФлаг("WIN32")) {
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
				if(ext == ".o")
					obj.добавь(фн);
				else
				if(ext == ".a" || ext == ".so")
					linkfile.добавь(фн);
				else
				if(IsHeaderExt(ext) && pkg[i].pch && allow_pch && !blitz) {
					if(pch_header.дайСчёт())
						вКонсоль(дайИмяф(фн) + ": несколько PCHs не допускается. Проверьте файл конфигурации.");
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

//	if(вербозно_ли())
//		cc << " -v";
	if(естьФлаг("WIN32")/* && естьФлаг("MT")*/)
		cc << " -mthreads";

	if(естьФлаг("DEBUG_MINIMAL") || естьФлаг("DEBUG_FULL")) {
		cc << (естьФлаг("WIN32") && естьФлаг("CLANG") ? " -gcodeview -fno-limit-debug-info" : " -ggdb");
		cc << (естьФлаг("DEBUG_FULL") ? " -g2" : " -g1");
	}
	Ткст fuse_cxa_atexit;
	if(is_shared /*&& !естьФлаг("MAIN")*/) {
		cc << " -shared -fPIC";
		fuse_cxa_atexit = " -fuse-cxa-atexit";
	}
	if(!естьФлаг("SHARED") && !is_shared)
		cc << " -static ";
//	else if(!естьФлаг("WIN32")) // TRC 05/03/08: dynamic fPIC doesn't seem to work in MinGW
//		cc << " -dynamic -fPIC "; // TRC 05/03/30: dynamic fPIC doesn't seem to work in GCC either :-)
	cc << ' ' << Gather(pkg.option, config.дайКлючи());
	cc << " -fexceptions";

#if 0
	if (естьФлаг("OSX")) {
	  if (естьФлаг("POWERPC"))
		cc << " -arch ppc";
	  if (естьФлаг("X86"))
		cc << " -arch i386";
	}
#endif
//	if(естьФлаг("SSE2")) {
//		cc << " -msse2";
//		if(!естьФлаг("CLANG"))
//			cc << " -mfpmath=sse";
//	}

	if(!release)
		cc << " -D_DEBUG " << debug_options;
	else
		cc << ' ' << release_options;

	if(pkg.nowarnings)
		cc << " -w";

	int recompile = 0;
	Blitz b;
	if(blitz) {
		BlitzBuilderComponent bc(this);
		b = bc.MakeBlitzStep(sfile, soptions, obj, immfile, ".o", noblitz, package);
		recompile = b.build;
	}

	for(i = 0; i < sfile.дайСчёт(); i++) {
		Ткст фн = sfile[i];
		Ткст ext = впроп(дайРасшф(фн));
		if(findarg(ext, ".rc", ".brc", ".c") < 0 && HdependFileTime(sfile[i]) > дайФВремя(CatAnyPath(outdir, дайТитулф(фн) + ".o")))
			recompile++;
	}

	Ткст pch_use;
	Ткст pch_file;
	
	if(recompile > 2 && pch_header.дайСчёт()) {
		Ткст pch_header2 = CatAnyPath(outdir, дайТитулф(pch_header) + "$pch.h");
		pch_file = pch_header2 + ".gch";
		сохраниФайл(pch_header2, "#include <" + pch_header + ">"); // CLANG needs a copy of header
		
		int pch_slot = разместиСлот();
		ТкстБуф sb;

		sb << Join(cc, cpp_options) << " -x c++-header " << GetPathQ(pch_header) << " -o " << GetPathQ(pch_file);

		вКонсоль("Прекомпилирование заголовка: " + дайИмяф(pch_header));
		if(pch_slot < 0 || !пуск(~sb, pch_slot, pch_file, 1))
			Ошибка = true;
		жди();

		pch_use = " -I" + GetPathQ(outdir) + " -include " + дайИмяф(pch_header2) + " -Winvalid-pch ";
	}

	if(blitz && b.build) {
		вКонсоль("БЛИЦ:" + b.info);
		int slot = разместиСлот();
		if(slot < 0 || !пуск(Ткст().конкат() << Join(cc, cpp_options) << ' '
		                    << GetPathQ(b.path)
		                    << " -o " << GetPathQ(b.object), slot, b.object, b.count))
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
		Ткст objfile = CatAnyPath(outdir, SourceToObjName(package, фн)) + (rc ? "$rc.o" : brc ? "$brc.o" : ".o");
		if(дайИмяф(фн) == "Info.plist")
			Info_plist = загрузиФайл(фн);
		if(HdependFileTime(фн) > дайФВремя(objfile)) {
			вКонсоль(дайИмяф(фн));
			int time = msecs();
			bool execerr = false;
			if(rc) {
				Ткст exec;
				Ткст windres = "windres.exe";
				int q = compiler.найдирек('-'); // clang32 windres имя is i686-w64-mingw32-windres.exe
				if(q > 0)
					windres = compiler.середина(0, q + 1) + windres;
				exec << FindInDirs(host->GetExecutablesDirs(), windres) << " -i " << GetPathQ(фн);
				if(cc.найди(" -m32 ") >= 0)
					exec << " --target=pe-i386 ";
				exec << " -o " << GetPathQ(objfile) << Includes(" --include-dir=", package, pkg)
				     << DefinesTargetTime(" -D", package, pkg) + (естьФлаг("DEBUG")?" -D_DEBUG":"");
				PutVerbose(exec);
				int slot = разместиСлот();
				execerr = (slot < 0 || !пуск(exec, slot, objfile, 1));
			}
			else if(brc) {
				try {
					Ткст brcdata = загрузиФайл(фн);
					if(brcdata.проц_ли())
						throw Искл(фмт("ошибка при чтении файла '%s'", фн));
					СиПарсер parser(brcdata, фн);
					BinaryToObject(objfile, parser, дайДиректориюФайла(фн), package, pkg);
				}
				catch(Искл e) {
					вКонсоль(e);
					execerr = true;
				}
			}
			else {
				Ткст exec = cc;
				if(ext == ".c")
					exec << Join(" -x c", c_options) << ' ';
				else if(ext == ".s" || ext == ".S")
					exec << " -x assembler-with-cpp ";
				else
				if (ext == ".m")
					exec << fuse_cxa_atexit << " -x objective-c ";
				else
				if (ext == ".mm")
					exec << fuse_cxa_atexit << Join(" -x objective-c++ ", cpp_options) << ' ';
				else {
					exec << fuse_cxa_atexit << Join(" -x c++", cpp_options) << ' ';
					exec << pch_use;
				}
				exec << GetPathQ(фн)  << " " << soptions[i] << " -o " << GetPathQ(objfile);
				PutVerbose(exec);
				int slot = разместиСлот();
				execerr = (slot < 0 || !пуск(exec, slot, objfile, 1));
			}
			if(execerr)
				DeleteFile(objfile);
			Ошибка |= execerr;
			PutVerbose("скомпилировано за " + GetPrintTime(time));
			ccount++;
		}
		immfile.добавь(objfile);
		if(init)
			linkfile.добавь(objfile);
		else
			obj.добавь(objfile);
	}

	if(Ошибка) {
//		if(ccount)
//			поместиВремяКомпиляции(time, ccount);
		завершиГруппуКонсИср();
		return false;
	}

	MergeWith(linkoptions, " ", Gather(pkg.link, config.дайКлючи()));
	if(linkoptions.дайСчёт())
		linkoptions << ' ';
	
	bool making_lib = естьФлаг("MAKE_LIB") || естьФлаг("MAKE_MLIB");

	if(!making_lib) {
		Вектор<Ткст> libs = разбей(Gather(pkg.library, config.дайКлючи()), ' ');
		linkfile.приставь(libs);
	}

	if(pch_file.дайСчёт())
		OnFinish(callback1(DeletePCHFile, pch_file));

	if(!естьФлаг("MAIN")) {
		if(естьФлаг("BLITZ") && !естьФлаг("SO") || естьФлаг("NOLIB") || making_lib) {
			linkfile.приставь(obj); // Simply link everything as .o files...
			завершиГруппуКонсИср();
//			if(ccount)
//				поместиВремяКомпиляции(time, ccount);
			return true;
		}
		завершиГруппуКонсИср();
		if(!жди())
			return false;
		Ткст product;
		if(is_shared)
			product = GetSharedLibPath(package);
		else
			product = CatAnyPath(outdir, GetAnyFileName(package) + ".a");
		Ткст hproduct = product;
		Время producttime = дайФВремя(hproduct);
		if(obj.дайСчёт()) {
			linkfile.добавь(product);
			immfile.добавь(product);
		}
		for(int i = 0; i < obj.дайСчёт(); i++)
			if(дайФВремя(obj[i]) > producttime)
				return CreateLib(product, obj, all_uses, all_libraries, Gather(pkg.link, config.дайКлючи()));
		return true;
	}

	завершиГруппуКонсИср();
	obj.приставь(linkfile);
	linkfile = pick(obj);
	return true;
}

bool DinrusBuilder::CreateLib(const Ткст& product, const Вектор<Ткст>& obj,
                           const Вектор<Ткст>& all_uses, const Вектор<Ткст>& all_libraries,
                           const Ткст& link_options)
{
	int libtime = msecs();
	Ткст hproduct = product;
	Ткст lib;
	bool is_shared = естьФлаг("SO");
	if(is_shared) {
		lib = CompilerName();
		lib << " -shared -fPIC -fuse-cxa-atexit";
		if(естьФлаг("GCC32"))
			lib << " -m32";
		Точка p = ExtractVersion();
		if(!пусто_ли(p.x) && естьФлаг("WIN32")) {
			lib << " -Xlinker --major-image-version -Xlinker " << p.x;
			if(!пусто_ли(p.y))
				lib << " -Xlinker --minor-image-version -Xlinker " << p.y;
		}
		lib << ' ' << link_options;
		lib << " -o ";
	}
	else
		lib = "ar -sr ";
	lib << GetPathQ(product);

	Ткст llib;
	for(int i = 0; i < obj.дайСчёт(); i++)
		llib << ' ' << GetPathQ(obj[i]);
	вКонсоль("Создаётся библиотека...");
	DeleteFile(hproduct);
	if(is_shared) {
		for(int i = 0; i < libpath.дайСчёт(); i++)
			llib << " -L" << GetPathQ(libpath[i]);
		for(int i = 0; i < all_uses.дайСчёт(); i++)
			llib << ' ' << GetPathQ(GetSharedLibPath(all_uses[i]));
		for(int i = 0; i < all_libraries.дайСчёт(); i++)
			llib << " -l" << GetPathQ(all_libraries[i]);
		
		if(естьФлаг("POSIX"))
			llib << " -Wl,-soname," << GetSoname(product);
	}

	Ткст tmpFileName;
	if(естьФлаг("LINUX") || естьФлаг("WIN32")) {
		if(lib.дайСчёт() + llib.дайСчёт() >= 8192)
		{
			tmpFileName = дайВремИмяф();
			// we can't simply put all data on a single line
			// as it has a limit of around 130000 chars too, so we split
			// in multiple lines
			ФайлВывод f(tmpFileName);
			while(llib != "")
			{
				int found = 0;
				bool quotes = false;
				int lim = min(8192, llib.дайСчёт());
				for(int i = 0; i < lim; i++)
				{
					char c = llib[i];
					if(isspace(c) && !quotes)
						found = i;
					else if(c == '"')
						quotes = !quotes;
				}
				if(!found)
					found = llib.дайСчёт();

				// replace all '\' with '/'`
				llib = UnixPath(llib);
				
				f.PutLine(llib.лево(found));
				llib.удали(0, found);
			}
			f.закрой();
			lib << " @" << tmpFileName;
		}
		else
			lib << llib;
	}
	else
		lib << llib;

	int res = выполни(lib);
	if(tmpFileName.дайСчёт())
		удалифл(tmpFileName);
	Ткст folder, libF, soF, linkF;
	if(естьФлаг("POSIX")) {
		if(is_shared)
		{
			folder = дайПапкуФайла(hproduct);
			libF = дайИмяф(hproduct);
			soF = приставьИмяф(folder, GetSoname(hproduct));
			linkF = приставьИмяф(folder, GetSoLinkName(hproduct));
		}
	}
	if(res) {
		DeleteFile(hproduct);
		if(естьФлаг("POSIX")) {
			if(is_shared) {
				DeleteFile(libF);
				DeleteFile(linkF);
			}
		}
		return false;
	}
#ifdef PLATFORM_POSIX // we do not have symlink in Win32....
	if(естьФлаг("POSIX")) {
		if(is_shared)
		{
			int r;
			r = symlink(libF, soF);
			r = symlink(libF, linkF);
			(void)r;
		}
	}
#endif
	вКонсоль(Ткст().конкат() << hproduct << " (" << дайИнфОФайле(hproduct).length
	           << " B) создано за " << GetPrintTime(libtime));
	return true;
}

bool DinrusBuilder::Link(const Вектор<Ткст>& linkfile, const Ткст& linkoptions, bool createmap)
{
	if(!жди())
		return false;
	PutLinking();
	
	if(естьФлаг("MAKE_MLIB") || естьФлаг("MAKE_LIB"))
		return CreateLib(форсируйРасш(target, ".a"), linkfile, Вектор<Ткст>(), Вектор<Ткст>(), linkoptions);

	int time = msecs();
#ifdef PLATFORM_OSX
	CocoaAppBundle();
#endif
	for(int i = 0; i < linkfile.дайСчёт(); i++)
		if(дайФВремя(linkfile[i]) > targettime) {
			Вектор<Ткст> lib;
			Ткст lnk = CompilerName();
//			if(вербозно_ли())
//				lnk << " -v";
			if(естьФлаг("GCC32"))
				lnk << " -m32";
			if(естьФлаг("DLL"))
				lnk << " -shared";
			if(!естьФлаг("SHARED") && !естьФлаг("SO"))
				lnk << " -static";
			if(естьФлаг("WINCE"))
				lnk << " -mwindowsce";
			else if(естьФлаг("WIN32")) {
				lnk << " -mthreads";
				if(естьФлаг("CLANG")) {
					if(естьФлаг("GUI"))
						lnk << " -mwindows";
					else
						lnk << " -mconsole";
				}
				else {
					lnk << " -mwindows";
					// if(естьФлаг("MT"))
					if(!естьФлаг("GUI"))
						lnk << " -mconsole";
				}
			}
			lnk << " -o " << GetPathQ(target);
			if(createmap)
				lnk << " -Wl,-вКарту," << GetPathQ(дайДиректориюФайла(target) + дайТитулф(target) + ".map");
			if(естьФлаг("DEBUG_MINIMAL") || естьФлаг("DEBUG_FULL"))
				lnk << (естьФлаг("CLANG") && естьФлаг("WIN32") ? " -Wl,-pdb=" : " -ggdb");
			else
				lnk << (!естьФлаг("OSX") ? " -Wl,-s" : "");
			for(i = 0; i < libpath.дайСчёт(); i++)
				lnk << " -L" << GetPathQ(libpath[i]);
			MergeWith(lnk, " ", linkoptions);
			Ткст lfilename;
			if(естьФлаг("OBJC")) {
				Ткст lfilename;
				Ткст linklist;
				for(i = 0; i < linkfile.дайСчёт(); i++)
					if(впроп(дайРасшф(linkfile[i])) == ".o" || впроп(дайРасшф(linkfile[i])) == ".a")
						linklist << linkfile[i] << '\n';

				Ткст linklistM = "Producing link file list ...\n";
				Ткст odir = дайДиректориюФайла(linkfile[0]);
				lfilename << дайПапкуФайла(linkfile[0]) << ".LinkFileList";
					
				linklistM  << lfilename;
				РНЦП::сохраниФайл(lfilename, linklist);
				lnk << " -L" << GetPathQ(odir)
				    << " -F" << GetPathQ(odir)
				          << " -filelist " << lfilename << " ";
				вКонсоль( linklistM );
			}
			else
				for(i = 0; i < linkfile.дайСчёт(); i++) {
					if(впроп(дайРасшф(linkfile[i])) == ".o")
						lnk  << ' ' << GetPathQ(linkfile[i]);
					else
						lib.добавь(linkfile[i]);
				}
			if(!естьФлаг("SOLARIS") && !естьФлаг("OSX") && !естьФлаг("OBJC"))
				lnk << " -Wl,--start-группа ";
			for(Ткст s : pkg_config)
				lnk << " `pkg-config --libs " << s << "`";
			for(int pass = 0; pass < 2; pass++) {
				for(i = 0; i < lib.дайСчёт(); i++) {
					Ткст ln = lib[i];
					Ткст ext = впроп(дайРасшф(ln));
					
					// unix shared libs shall have version number AFTER .so (sic)
					// so we shall find the true extension....
					if(естьФлаг("POSIX") && ext != ".so")
					{
						const char *c = ln.последний();
						while(--c >= ~ln)
							if(!цифра_ли(*c) && *c != '.')
								break;
						int pos = int(c - ~ln - 2);
						if(pos >= 0 && впроп(ln.середина(pos, 3)) == ".so")
							ext = ".so";
					}
							
					if(pass == 0) {
						if(ext == ".a")
							lnk << ' ' << GetPathQ(FindInDirs(libpath, lib[i]));
					}
					else
						if(ext != ".a") {
							if(ext == ".so" || ext == ".dll" || ext == ".lib")
								lnk << ' ' << GetPathQ(FindInDirs(libpath, lib[i]));
							else
								lnk << " -l" << ln;
						}
				}
				if(pass == 1 && !естьФлаг("SOLARIS") && !естьФлаг("OSX"))
					lnk << " -Wl,--end-группа";
			}
			вКонсоль("Компоновка...");
			bool Ошибка = false;
			CustomStep(".pre-link", Null, Ошибка);
			if(!Ошибка && выполни(lnk) == 0) {
				CustomStep(".post-link", Null, Ошибка);
				вКонсоль(Ткст().конкат() << target << " (" << дайИнфОФайле(target).length
				           << " B) скомпоновано за " << GetPrintTime(time));
				return !Ошибка;
			}
			else {
				DeleteFile(target);
				return false;
			}
		}

	вКонсоль(Ткст().конкат() << target << " (" << дайИнфОФайле(target).length
	           << " B) в свежем состоянии.");
	return true;
}

bool DinrusBuilder::Preprocess(const Ткст& package, const Ткст& file, const Ткст& target, bool asmout)
{
	Пакет pkg;
	Ткст packagePath = PackagePath(package);
	pkg.грузи(packagePath);
	Ткст packageDir = дайПапкуФайла(packagePath);
	ChDir(packageDir);
	PutVerbose("cd " + packageDir);

	Ткст cmd = CmdLine(package, pkg);
	cmd << " " << Gather(pkg.option, config.дайКлючи());
	cmd << " -o " << target;
	cmd << (asmout ? " -S " : " -E ") << GetPathQ(file);
	if(BuilderUtils::IsCFile(file))
		cmd << " " << c_options;
	else
	if(BuilderUtils::IsCppFile(file))
		cmd << " " << cpp_options;
	return выполни(cmd);
}

Построитель *CreateGccBuilder()
{
	return new DinrusBuilder;
}

ИНИЦИАЛИЗАТОР(DinrusBuilder)
{
	RegisterBuilder("GCC", CreateGccBuilder);
	RegisterBuilder("CLANG", CreateGccBuilder);
}
