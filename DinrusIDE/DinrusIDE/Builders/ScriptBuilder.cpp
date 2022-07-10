#include "Builders.h"

#include <Esc/Esc.h>

EscValue ScriptBuilder::ExecuteIf(const char *фн, Вектор<EscValue>& args)
{
	CheckParse();
	EscValue out;
	int f = globals.найди(фн);
	if(f < 0)
		return out;
	try
	{
		out = ::выполни(globals, NULL, globals[f], args, 50000);
	}
	catch(Искл e)
	{
		script_error = true;
		вКонсоль(e);
	}
	return out;
}

EscValue ScriptBuilder::ExecuteIf(const char *фн)
{
	Вектор<EscValue> args;
	return ExecuteIf(фн, args);
}

EscValue ScriptBuilder::ExecuteIf(const char *фн, EscValue arg)
{
	Вектор<EscValue> args;
	args.добавь(arg);
	return ExecuteIf(фн, args);
}

EscValue ScriptBuilder::ExecuteIf(const char *фн, EscValue arg1, EscValue arg2)
{
	Вектор<EscValue> args;
	args.добавь(arg1);
	args.добавь(arg2);
	return ExecuteIf(фн, args);
}

EscValue ScriptBuilder::ExecuteIf(const char *фн, EscValue arg1, EscValue arg2, EscValue arg3)
{
	Вектор<EscValue> args;
	args.добавь(arg1);
	args.добавь(arg2);
	args.добавь(arg3);
	return ExecuteIf(фн, args);
}

void ScriptBuilder::ESC_Execute(EscEscape& e)
{
	e = выполни(Ткст(e[0])) ? 0 : 1;
}

void ScriptBuilder::ESC_PutConsole(EscEscape& e)
{
	вКонсоль(Ткст(e[0]));
}

void ScriptBuilder::ESC_PutVerbose(EscEscape& e)
{
	PutVerbose(Ткст(e[0]));
}

void ScriptBuilder::CheckParse()
{
	if(is_parsed)
		return;
	script_error = false;
	is_parsed = true;
	StdLib(globals);
	Escape(globals, "выполни(cmdline)", THISBACK(ESC_Execute));
	Escape(globals, "вКонсоль(text)", THISBACK(ESC_PutConsole));
	Escape(globals, "PutVerbose(text)", THISBACK(ESC_PutVerbose));
	EscValue inclist;
	inclist.SetEmptyArray();
	for(int i = 0; i < include.дайСчёт(); i++)
		inclist.ArrayAdd(GetPathQ(include[i]));
	globals.дайДобавь("INCLUDE") = inclist;
	EscValue liblist;
	liblist.SetEmptyArray();
	for(int i = 0; i < libpath.дайСчёт(); i++)
		liblist.ArrayAdd(GetPathQ(libpath[i]));
	globals.дайДобавь("LIBPATH") = liblist;

	try
	{
		Ткст sdata = загрузиФайл(script);
		if(пусто_ли(sdata))
			throw Искл(фмт("%s: not found or empty", script));
		СиПарсер parser(sdata, script, 1);
		while(!parser.кф_ли()) {
			Ткст id = parser.читайИд();
			globals.дайДобавь(id) = ReadLambda(parser);
		}
	}
	catch(Искл e)
	{
		script_error = true;
		вКонсоль(e);
	}
}

bool ScriptBuilder::постройПакет(const Ткст& package, Вектор<Ткст>& linkfile, Вектор<Ткст>&, Ткст& linkoptions,
	const Вектор<Ткст>& all_uses, const Вектор<Ткст>& all_libraries, int)
{
	int i;
	Ткст packagepath = PackagePath(package);
	Пакет pkg;
	pkg.грузи(packagepath);
	Ткст packagedir = дайПапкуФайла(packagepath);
	ChDir(packagedir);
	PutVerbose("cd " + packagedir);
	Вектор<Ткст> obj;
	script_error = false;

	Ткст gfl = Gather(pkg.option, config.дайКлючи());

	Вектор<Ткст> sfile;
	Вектор<Ткст> soptions;
	bool           Ошибка = false;

	for(i = 0; i < pkg.дайСчёт(); i++) {
		if(!строитсяИср())
			return false;
		if(!pkg[i].separator) {
			Ткст gop = Gather(pkg[i].option, config.дайКлючи());
			Вектор<Ткст> srcfile = CustomStep(pkg[i], package, Ошибка);
			if(srcfile.дайСчёт() == 0)
				Ошибка = true;
			for(int j = 0; j < srcfile.дайСчёт(); j++) {
				Ткст фн = srcfile[j];
				Ткст ext = впроп(дайРасшф(фн));
				if(ext == ".c" || ext == ".cpp" || ext == ".cc" || ext == ".cxx" ||
				   (ext == ".rc" && естьФлаг("WIN32"))) {
					sfile.добавь(фн);
					soptions.добавь(gfl + " " + gop);
				}
				else
				if(ext == ".o")
					obj.добавь(фн);
				else
				if(ext == ".a" || ext == ".so")
					linkfile.добавь(фн);
			}
		}
	}

/*
	if(естьФлаг("BLITZ")) {
		Blitz b = BlitzStep(sfile, soptions, obj, ".o");
		if(b.build) {
			вКонсоль("BLITZ:" + b.info);
			int time = msecs();
			if(выполни(cc + " " + GetPathQ(b.path) + " -o " + GetPathQ(b.object)) == 0)
				поместиВремяКомпиляции(time, b.count);
			else
				Ошибка = true;
		}
	}
*/
	int time = msecs();
	int ccount = 0;
	for(i = 0; i < sfile.дайСчёт() && !script_error; i++) {
		if(!строитсяИср())
			return false;
		Ткст фн = sfile[i];
		Ткст ext = впроп(дайРасшф(фн));
//		bool rc = ext == ".rc";
		Ткст objfile = ExecuteIf("objectfile", фн);
		if(script_error)
			return false;
		if(пусто_ли(objfile))
			objfile = CatAnyPath(outdir, дайТитулф(фн) + ".o");
		if(HdependFileTime(фн) > дайФВремя(objfile)) {
			вКонсоль(дайИмяф(фн));
			int time = msecs();
			if(!ExecuteIf("compile", GetPathQ(фн), GetPathQ(objfile), soptions[i]).GetNumber()) {
				DeleteFile(objfile);
				Ошибка = true;
			}
			PutVerbose("compiled in " + GetPrintTime(time));
			ccount++;
		}
		obj.добавь(objfile);
	}
	if(ccount)
		поместиВремяКомпиляции(time, ccount);

	if(Ошибка || script_error)
		return false;

	linkoptions << Gather(pkg.link, config.дайКлючи());

	Вектор<Ткст> libs = разбей(Gather(pkg.library, config.дайКлючи()), ' ');
	linkfile.приставь(libs);

	time = msecs();
	if(!естьФлаг("MAIN")) {
		if(естьФлаг("NOLIB")) {
			linkfile.приставь(obj);
			return true;
		}
		Ткст product = ExecuteIf("libraryfile", package);
		if(пусто_ли(product))
			product = CatAnyPath(outdir, GetAnyFileName(package) + ".a");
		Время producttime = дайФВремя(product);
		if(obj.дайСчёт())
			linkfile.добавь("*" + product); //!! ugly
		for(int i = 0; i < obj.дайСчёт(); i++)
			if(дайФВремя(obj[i]) > producttime) {
				вКонсоль("Creating library...");
				DeleteFile(product);
				EscValue objlist;
				objlist.SetEmptyArray();
				for(int i = 0; i < obj.дайСчёт(); i++)
					objlist.ArrayAdd(GetPathQ(obj[i]));
				if(!ExecuteIf("library", objlist, product).GetNumber()) {
					DeleteFile(product);
					Ошибка = true;
					return false;
				}
				вКонсоль(Ткст().конкат() << product << " (" << дайИнфОФайле(product).length
				           << " B) created in " << GetPrintTime(time));
				break;
			}
		return true;
	}

	obj.приставь(linkfile);
	linkfile = pick(obj);
	return true;
}

bool ScriptBuilder::Link(const Вектор<Ткст>& linkfile, const Ткст& linkoptions, bool)
{
	PutLinking();
	int time = msecs();
	for(int i = 0; i < linkfile.дайСчёт(); i++)
		if(дайФВремя(linkfile[i]) >= targettime) {
			EscValue objlist;
			objlist.SetEmptyArray();
			EscValue liblist;
			liblist.SetEmptyArray();
			for(i = 0; i < linkfile.дайСчёт(); i++)
				if(*linkfile[i] == '*')
					liblist.ArrayAdd(GetPathQ(linkfile[i].середина(1)));
				else
					objlist.ArrayAdd(GetPathQ(linkfile[i]));
			Вектор<EscValue> linkargs;
			linkargs.добавь(objlist);
			linkargs.добавь(liblist);
			linkargs.добавь(GetPathQ(target));
			linkargs.добавь(linkoptions);
			вКонсоль("Linking...");
			bool Ошибка = false;
			CustomStep(".pre-link", Null, Ошибка);
			if(!Ошибка && !ExecuteIf("link", linkargs).GetNumber()) {
				DeleteFile(target);
				return false;
			}
			CustomStep(".post-link", Null, Ошибка);
			вКонсоль(Ткст().конкат() << target << " (" << дайИнфОФайле(target).length
				<< " B) linked in " << GetPrintTime(time));
			return !Ошибка;
		}
	вКонсоль(Ткст().конкат() << target << " (" << дайИнфОФайле(target).length
	           << " B) is up to date.");
	return true;
}

bool ScriptBuilder::Preprocess(const Ткст& package, const Ткст& file, const Ткст& target, bool)
{
	return ExecuteIf("preprocess", file, target).GetNumber();
}

Построитель *CreateScriptBuilder()
{
	return new ScriptBuilder;
}

ИНИЦИАЛИЗАТОР(ScriptBuilder)
{
	RegisterBuilder("SCRIPT", CreateScriptBuilder);
}

/*
EscValue LayoutItem::CreateEsc() const
{
	EscValue ctrl;
	Ткст tp = тип;
	Ткст tm;
	if(ParseTemplate(tp, tm)) {
		CreateMethods(ctrl, tp, true);
		ctrl("CtrlPaint") = ctrl("рисуй");
		CreateMethods(ctrl, tm, false);
	}
	else
		CreateMethods(ctrl, tp, false);
	for(int q = 0; q < property.дайСчёт(); q++) {
		EscValue& w = ctrl(property[q].имя);
		const Значение& v = ~property[q];
		if(IsType<Шрифт>(v))
			w = EscFont(v);
		if(ткст_ли(v))
			w = (ШТкст)v;
		if(число_ли(v))
			w = (double)v;
		if(IsType<Цвет>(v))
			w = EscColor(v);
	}
	ctrl("тип") = (ШТкст)тип;
	ctrl("дайРазм") = ReadLambda(фмт("() { return Размер(%d, %d); }",
	                                    csize.cx, csize.cy));
	ctrl("дайПрям") = ReadLambda(фмт("() { return Прям(0, 0, %d, %d); }",
	                                    csize.cx, csize.cy));
	return ctrl;
}

EscValue LayoutItem::ExecuteMethod(const char *method, Вектор<EscValue>& arg) const
{
	try {
		EscValue self = CreateEsc();
		return ::выполни(UscGlobal(), &self, method, arg, 50000);
	}
	catch(СиПарсер::Ошибка& e) {
		вКонсоль(e + "\n");
	}
	return EscValue();
}

EscValue LayoutItem::ExecuteMethod(const char *method) const
{
	Вектор<EscValue> arg;
	return ExecuteMethod(method, arg);
}

Размер LayoutItem::дайМинРазм()
{
	return SizeEsc(ExecuteMethod("дайМинРазм"));
}
*/
