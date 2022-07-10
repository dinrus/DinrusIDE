#ifndef BUILDERS_H
#define BUILDERS_H

#include <DinrusIDE/Core/Core.h>
#include <plugin/bz2/bz2.h>

#include "Android.h"
#include "BuilderComponents.h"
#include "Build.h"

void поместиВремяКомпиляции(int time, int count);

Ткст BrcToC(СиПарсер& binscript, Ткст basedir);

Вектор<Ткст> RepoInfo(const Ткст& package);

Ткст MakeIdent(const char *имя);

struct ПостроительСиПП : Построитель {
	virtual Ткст GetTargetExt() const;
	virtual void   очистьПакет(const Ткст& package, const Ткст& outdir);

	const РОбласть& wspc;
	Время             targettime;
	Вектор<Ткст>   CFLAGS;
	Вектор<Ткст>   CXXFLAGS;

	Ткст                 GetSharedLibPath(const Ткст& package) const;
	int                    разместиСлот();
	bool                   пуск(const char *cmdline, int slot, Ткст ключ, int blitz_count);
	bool                   пуск(const char *cmdline, Поток& out, int slot, Ткст ключ, int blitz_count);
	bool                   жди();
	bool                   жди(int slot);
	void                   OnFinish(Событие<>  cb);
	bool                   Cp(const Ткст& cmd, const Ткст& package, bool& Ошибка);
	bool                   Cd(const Ткст& cmd);
	Вектор<Ткст>         CustomStep(const Ткст& path, const Ткст& package, bool& Ошибка);

	Ткст                 Includes(const char *sep, const Ткст& package, const Пакет& pkg);
	void                   SaveBuildInfo(const Ткст& package);
	Ткст                 DefinesTargetTime(const char *sep, const Ткст& package, const Пакет& pkg);
	Ткст                 IncludesDefinesTargetTime(const Ткст& package, const Пакет& pkg);
	bool                   HasAnyDebug() const;

	Ткст                 GetMakePath(Ткст фн) const;
	Точка                  ExtractVersion() const;

	// POSIX lib files has names in form of libXXXXXX.so.ver.minver(.rel)
	// so we can't simply get file extension
	Ткст                 GetSrcType(Ткст фн) const;

	// from complete lib имя/path (libXXX.so.ver.minVer) gets the soname (libXXX.so.ver)
	Ткст                 GetSoname(Ткст libName) const;

	// from complete lib имя/path (libXXX.so.ver.minVer) gets the link имя (libXXX.so)
	Ткст                 GetSoLinkName(Ткст libName) const;

	void                   ShowTime(int count, int start_time);

	virtual void           AddMakeFile(MakeFile& makefile, Ткст package,
		const Вектор<Ткст>& all_uses, const Вектор<Ткст>& all_libraries,
		const Индекс<Ткст>& common_config, bool exporting);

	virtual void AddCCJ(MakeFile& mfinfo, Ткст package,
		const Индекс<Ткст>& common_config, bool exporting, bool last_ws);

	ПостроительСиПП() : wspc(GetIdeWorkspace()) {}
};

Ткст SourceToObjName(const Ткст& package, const Ткст& srcfile_);

struct GccBuilder : ПостроительСиПП {
	virtual void   добавьФлаги(Индекс<Ткст>& cfg);
	virtual bool   постройПакет(const Ткст& package, Вектор<Ткст>& linkfile, Вектор<Ткст>& immfile,
	    Ткст& linkoptions, const Вектор<Ткст>& all_uses, const Вектор<Ткст>& all_libraries, int optimize);
	virtual bool   Link(const Вектор<Ткст>& linkfile, const Ткст& linkoptions, bool createmap);
	virtual bool   Preprocess(const Ткст& package, const Ткст& file, const Ткст& target, bool asmout);

	Ткст CompilerName() const;
	Ткст CmdLine(const Ткст& package, const Пакет& pkg);
	void BinaryToObject(Ткст objfile, СиПарсер& binscript, Ткст basedir, const Ткст& package, const Пакет& pkg);
	void   CocoaAppBundle();
	bool   CreateLib(const Ткст& product, const Вектор<Ткст>& obj,
	                 const Вектор<Ткст>& all_uses, const Вектор<Ткст>& all_libraries,
	                 const Ткст& link_options);

	Ткст Info_plist; // apple bundle Info.plist
};

struct OwcBuilder : ПостроительСиПП {
	typedef OwcBuilder ИМЯ_КЛАССА;

	virtual void   добавьФлаги(Индекс<Ткст>& cfg);
	virtual bool   постройПакет(const Ткст& package, Вектор<Ткст>& linkfile, Вектор<Ткст>& immfile,
	    Ткст& linkoptions, const Вектор<Ткст>& all_uses, const Вектор<Ткст>& all_libraries, int optimize);
	virtual bool   Link(const Вектор<Ткст>& linkfile, const Ткст& linkoptions, bool createmap);
	virtual bool   Preprocess(const Ткст& package, const Ткст& file, const Ткст& target, bool asmout);

	Ткст         IncludesDefinesTargetTime(const Ткст& package, const Пакет& pkg);

	Ткст CompilerName(bool isCpp = true) const;
	Ткст LinkerName() const;
	Ткст CmdLine(const Ткст& package, const Пакет& pkg);
	void   BinObjConsole(Ткст c) { вКонсоль(c); }
};

struct MscBuilder : ПостроительСиПП {
	typedef MscBuilder ИМЯ_КЛАССА;

	virtual void   добавьФлаги(Индекс<Ткст>& cfg);
	virtual bool   постройПакет(const Ткст& package, Вектор<Ткст>& linkfile, Вектор<Ткст>& immfile,
	    Ткст& linkoptions, const Вектор<Ткст>& all_uses, const Вектор<Ткст>& all_libraries, int optimize);
	virtual bool   Link(const Вектор<Ткст>& linkfile, const Ткст& linkoptions, bool createmap);
	virtual bool   Preprocess(const Ткст& package, const Ткст& file, const Ткст& target, bool asmout);

	Ткст CmdLine(const Ткст& package, const Пакет& pkg);
	Ткст MachineName() const;
	Ткст LinkerName() const;
	Ткст Pdb(Ткст package, int slot, bool separate_pdb) const;
	void   BinObjConsole(Ткст c) { вКонсоль(c); }
	bool   IsMsc89() const;
	bool   IsMsc86() const;
	bool   IsMscArm() const;
	bool   IsMsc64() const;
	bool   CreateLib(const Ткст& product, const Вектор<Ткст>& obj,
	                 const Вектор<Ткст>& all_uses, const Вектор<Ткст>& all_libraries,
	                 const Ткст& link_options);

	void BinaryToObject(Ткст objfile, СиПарсер& binscript, Ткст basedir, const Ткст& package, const Пакет& pkg);
};

struct JavaBuilder : ПостроительСиПП {
	virtual bool   постройПакет(const Ткст& package, Вектор<Ткст>& linkfile, Вектор<Ткст>&, Ткст& linkoptions,
		const Вектор<Ткст>& all_uses, const Вектор<Ткст>& all_libraries, int optimize);
	virtual bool   Link(const Вектор<Ткст>& linkfile, const Ткст& linkoptions, bool createmap);
	virtual bool   Preprocess(const Ткст& package, const Ткст& file, const Ткст& target, bool asmout);

	bool           PreprocessJava(Ткст file, Ткст target, Ткст options, Ткст package, const Пакет& pkg);
	Время           AddClassDeep(Ткст& link, Ткст dir, Ткст reldir);

	Ткст         JavaLine();
	Ткст         JarLine();
};

struct ScriptBuilder : ПостроительСиПП {
public:
	typedef ScriptBuilder ИМЯ_КЛАССА;
	ScriptBuilder() : is_parsed(false), script_error(false) {}

	virtual bool   постройПакет(const Ткст& package, Вектор<Ткст>& linkfile, Вектор<Ткст>&, Ткст& linkoptions,
		const Вектор<Ткст>& all_uses, const Вектор<Ткст>& all_libraries, int optimize);
	virtual bool   Link(const Вектор<Ткст>& linkfile, const Ткст& linkoptions, bool createmap);
	virtual bool   Preprocess(const Ткст& package, const Ткст& file, const Ткст& target, bool asmout);

	Ткст         CmdLine();

private:
	void           CheckParse();

	EscValue       ExecuteIf(const char *фн, Вектор<EscValue>& args);
	EscValue       ExecuteIf(const char *фн);
	EscValue       ExecuteIf(const char *фн, EscValue arg);
	EscValue       ExecuteIf(const char *фн, EscValue arg1, EscValue arg2);
	EscValue       ExecuteIf(const char *фн, EscValue arg1, EscValue arg2, EscValue arg3);

	void           ESC_Execute(EscEscape& e);
	void           ESC_PutConsole(EscEscape& e);
	void           ESC_PutVerbose(EscEscape& e);

private:
	МассивМап<Ткст, EscValue> globals;
	bool is_parsed;
	bool script_error;
};

void DeletePCHFile(const Ткст& pch_file);

#ifdef PLATFORM_POSIX
void   CreateBuildMethods();
#endif

Ткст GetDefaultUppOut();

ИНИЦИАЛИЗУЙ(GccBuilder)
ИНИЦИАЛИЗУЙ(MscBuilder)
ИНИЦИАЛИЗУЙ(JavaBuilder)
ИНИЦИАЛИЗУЙ(AndroidBuilder)
ИНИЦИАЛИЗУЙ(ScriptBuilder)

#endif