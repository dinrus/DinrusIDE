#ifndef DOCPP_H
#define DOCPP_H

#include <Core/Core.h>

namespace РНЦП {
	
// These functions can only but executed by single thread, unless said otherwise
	
bool   IsCPPFile(const Ткст& file);
bool   IsHFile(const Ткст& path);

Ткст GetStdDefs();

void   SetPPDefs(const Ткст& defs);

void   InvalidatePPCache();

void   PPSync(const Ткст& include_path);

enum Kind {
	STRUCT,
	STRUCTTEMPLATE,
	TYPEDEF,
	CONSTRUCTOR,
	DESTRUCTOR,
	FUNCTION,
	INSTANCEFUNCTION,
	CLASSFUNCTION,
	FUNCTIONTEMPLATE,
	INSTANCEFUNCTIONTEMPLATE,
	CLASSFUNCTIONTEMPLATE,
	INLINEFRIEND,
	VARIABLE,
	INSTANCEVARIABLE,
	CLASSVARIABLE,
	ENUM,
	MACRO,
	FRIENDCLASS,
	NAMESPACE,
	FLAGTEST,
};

inline bool IsCppType(int i)
{
	return i >= STRUCT && i <= TYPEDEF || i == FRIENDCLASS;
}

inline bool IsCppCode(int i) {
	return i >= CONSTRUCTOR && i <= INLINEFRIEND;
};

inline bool IsCppData(int i) {
	return i >= VARIABLE && i <= ENUM;
}

inline bool IsCppMacro(int i) {
	return i == MACRO;
}

inline bool IsCppTemplate(int i) {
	return i == STRUCTTEMPLATE || i >= FUNCTIONTEMPLATE && i <= CLASSFUNCTIONTEMPLATE;
}

void   CleanPP();
void   SerializePPFiles(Поток& s);
void   SweepPPFiles(const Индекс<Ткст>& keep);

void InvalidateFileTimeCache();
void InvalidateFileTimeCache(const Ткст& path);
Время GetFileTimeCached(const Ткст& path);

Ткст               NormalizeSourcePath(const Ткст& path, const Ткст& currdir);
Ткст               NormalizeSourcePath(const Ткст& path);

void                 ClearSources();
const Индекс<Ткст>& GetAllSources();
void                 GatherSources(const Ткст& path);
Ткст               GetMasterFile(const Ткст& file);
const ВекторМап<Ткст, Ткст>& GetAllSourceMasters();

const char **CppKeyword();

enum CppAccess {
	PUBLIC,
	PROTECTED,
	PRIVATE,
};

enum FileTypeEnum {
	FILE_H,
	FILE_HPP,
	FILE_CPP,
	FILE_C,
	FILE_OTHER,
};

struct CppItem {
	Ткст         qitem;
	Ткст         элт;
	Ткст         имя;
	Ткст         uname;
	Ткст         natural;
	Ткст         тип;
	Ткст         qtype;
	Ткст         tparam; // complete template parameters list, like '<class T>'
	Ткст         param;
	Ткст         pname;
	Ткст         ptype; // фн: types of parameters, struct: base classes
	Ткст         qptype;
	Ткст         tname;
	Ткст         ctname;
	Ткст         using_namespaces;
	byte           access = PUBLIC;
	byte           kind = STRUCT;
	int16          at = 0;
	bool           virt = false;

	bool           decla = false;
	bool           lvalue = false;
	bool           isptr = false;
	
	byte           filetype = FILE_OTHER;
	bool           impl = false;
	int            file = 0;
	int            line = 0;

	bool           qualify = true;

	bool           IsType() const      { return IsCppType(kind); }
	bool           IsCode() const      { return IsCppCode(kind); }
	bool           IsData() const      { return IsCppData(kind); }
	bool           IsMacro() const     { return IsCppMacro(kind); }
	bool           IsFlagTest() const  { return kind == FLAGTEST; }
	bool           IsTemplate() const  { return IsCppTemplate(kind); }
	
	void           сериализуй(Поток& s);
	
	void           Dump(Поток& s) const;
	Ткст         вТкст() const;
};

int FindItem(const Массив<CppItem>& x, const Ткст& qitem);
int FindName(const Массив<CppItem>& x, const Ткст& имя, int i = 0);

struct CppBase : МассивМап<Ткст, Массив<CppItem> > {
	Ткст         types_md5;
	Индекс<Ткст>  namespaces;

	bool           IsType(int i) const;
	void           смети(const Индекс<int>& file, bool keep = true);
	void           RemoveFiles(const Индекс<int>& remove_file) { смети(remove_file, false); }
	void           RemoveFile(int filei);
	void           приставь(CppBase&& base);

	void           Dump(Поток& s);
};

class ScopeInfo { // information about scope
	bool           bvalid; // baselist is valid
	bool           nvalid; // scopes is valid
	Вектор<Ткст> baselist; // list of all base classes of scope
	Вектор<Ткст> scopes; // list of scopes (РНЦП::Ткст::иниц::, РНЦП::Ткст::, РНЦП::)
	int            scopei; // Индекс of this scope in base
	Ткст         usings; // using namespaces contained in scopes
	
	void           Bases(int i, Вектор<int>& g);
	void           иниц();

public:
	const CppBase&            base;
	ВекторМап<Ткст, Ткст> cache;

	const Вектор<Ткст>& GetBases();
	const Вектор<Ткст>& GetScopes(const Ткст& usings);
	int                   GetScope() const              { return scopei; }
	void                  NoBases()                     { baselist.очисть(); bvalid = true; }

	ScopeInfo(const CppBase& base, int scopei = -1);
	ScopeInfo(int scopei, const CppBase& base);
	ScopeInfo(const CppBase& base, const Ткст& scope);
	ScopeInfo(const ScopeInfo& f);
};

struct ParserContext {
	struct Контекст {
		Ткст         ns;
		Ткст         scope;
		Ткст         ctname;
		Вектор<int>    tparam;
		Индекс<int>     typenames;
		int            access;
		Ткст         namespace_using;

		void operator<<=(const Контекст& t);

		Ткст  Dump() const;
		
		Контекст() {}
		rval_default(Контекст);
	};

	Контекст     context;
	Ткст      current_scope;
	Ткст      current_key;
	Ткст      current_name;
	CppItem     current;
	bool        inbody;

	struct Local : Движимое<Local> {
		Ткст тип;
		bool   isptr;
		int    line;
	};

	ВекторМап<Ткст, Local>  local;

	Вектор<Ткст> GetNamespaces() const;
	bool           IsInBody() const                   { return inbody; }
};

// Parse CAN be run in parallel
void Parse(CppBase& base, const Ткст& src, int file, int filetype, const Ткст& path,
           Событие<int, const Ткст&> Ошибка, const Вектор<Ткст>& namespace_stack,
           const Индекс<Ткст>& namespace_using);


Ткст NoTemplatePars(const Ткст& тип);

// PreprocessParse CAN be run in parallel
void PreprocessParse(CppBase& base, Поток& in, int file, const Ткст& path,
                     Событие<int, const Ткст&> Ошибка);
Ткст PreprocessCpp(const Ткст& src, const Ткст& path);

ParserContext AssistParse(const Ткст& src, const Ткст& path_, Событие<int, const Ткст&> Ошибка,
                          Функция<Ткст(Ткст, Ткст, Ткст)> qualify);

void SimpleParse(CppBase& cpp, const Ткст& txt, const Ткст& cls);

void   Qualify(CppBase& base);

Ткст Qualify(const CppBase& base, const Ткст& scope, const Ткст& тип, const Ткст& usings);

Ткст ResolveTParam(const CppBase& codebase, const Ткст& тип, const Вектор<Ткст>& tparam);
void   ResolveTParam(const CppBase& codebase, Вектор<Ткст>& тип, const Вектор<Ткст>& tparam);

Ткст ParseTemplatedType(const Ткст& тип, Вектор<Ткст>& tparam);

bool HasCPPFileKeyword(const Ткст& path, const Ткст& id);

Ткст GetPPMD5(const Ткст& фн);

// GetDependeciesMD5 CAN be run in parallel
Ткст GetDependeciesMD5(const Ткст& path, Индекс<Ткст>& visited);

Индекс<Ткст> GetExpressionType(const CppBase& codebase, const ParserContext& parser, const Вектор<Ткст>& xp);

}

#endif
