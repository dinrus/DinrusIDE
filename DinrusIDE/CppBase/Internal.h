#ifndef _CppBase_Internal_h_
#define _CppBase_Internal_h_

namespace РНЦП {
	
const char *SkipString(const char *s);

void RemoveComments(Ткст& l, bool& incomment);

const ВекторМап<Ткст, Ткст>& GetNamespaceMacros();
const Индекс<Ткст>&             GetNamespaceEndMacros();

struct CppMacro : Движимое<CppMacro> {
	Ткст        param;
	Ткст        body;
	byte          md5[16];
	
	Ткст Define(const char *s);
	void   SetUndef()                { body = "\x7f"; }
	bool   IsUndef() const           { return body[0] == '\x7f' && body[1] == '\0'; }

	Ткст расширь(const Вектор<Ткст>& p, const Вектор<Ткст>& ep) const;

	void   сериализуй(Поток& s);
	
	Ткст вТкст() const;
};

enum PPItemType {
	PP_DEFINES,
	PP_INCLUDE,
	PP_USING,
	PP_NAMESPACE,
	PP_NAMESPACE_END
};

struct PPItem {
	int      тип;
	Ткст   text;
	int      segment_id;
	
	void     сериализуй(Поток& s) { s % тип % text % segment_id; }
};

struct PPMacro : Движимое<PPMacro> {
	CppMacro  macro;
	int       segment_id;        // a группа of macros in single file, between other elements (include, namespace. using, undef...)
	int       line;              // line in file
	int       undef_segment_id;  // macro has matching undef in the same file within this segment
	
	void   сериализуй(Поток& s) { s % macro % segment_id % line % undef_segment_id; }
	Ткст вТкст() const     { return какТкст(macro) + " " + какТкст(segment_id); }
	
	PPMacro()                   { segment_id = undef_segment_id = 0; }
};

struct PPFile { // contains "macro extract" of file, only info about macros defined and namespaces
	Время           filetime;
	Массив<PPItem>  элт;
	Индекс<Ткст>  includes;
	Вектор<Ткст> keywords;
	Ткст         md5sum;

	void Parse(Поток& in);
	void сериализуй(Поток& s) { s % filetime % элт % includes % keywords % md5sum; }
	void Dump() const;

private:
	Вектор<int>    ppmacro;   // indicies of macros in sAllMacros

	void CheckEndNamespace(Вектор<int>& namespace_block, int level, Md5Stream& md5);
};

PPMacro            *FindPPMacro(const Ткст& id, Индекс<int>& segment_id, int& segmenti);
const     CppMacro *FindMacro(const Ткст& id, Индекс<int>& segment_id, int& segmenti);

Ткст GetIncludePath();

Ткст GetSegmentFile(int segment_id);

void  MakePP(const Индекс<Ткст>& paths); // this is the only place to change PPFile info, cannot be run concurrently with anything else

const PPFile& GetPPFile(const char *path); // only returns information created by MakePP

Ткст GetIncludePath(const Ткст& s, const Ткст& filedir);
bool   IncludesFile(const Ткст& parent_path, const Ткст& header_path);

struct FlatPP {
	Индекс<int>    segment_id;
	Индекс<Ткст> usings;
};

const FlatPP& GetFlatPPFile(const char *path); // with #includes resolved

struct Cpp {
	static Индекс<Ткст>        kw;

	bool                        incomment;
	bool                        done;
	
	Индекс<Ткст>               visited;

	Индекс<int>                  segment_id; // segments of included macros
	ВекторМап<Ткст, PPMacro>  macro; // macros defined
	int                         std_macros; // standard macros (keywords and trick - fixed)
	Индекс<Ткст>               notmacro; // accelerator / expanding helper
	Ткст                      prefix_macro; // for handling multiline macros

	Ткст                      output; // preprocessed file
//	Индекс<Ткст>               usedmacro;
	Индекс<Ткст>               namespace_using; // 'using namespace' up to start of file
	Вектор<Ткст>              namespace_stack; // namspace up to start of file
	
	Md5Stream                   md5;
	
	void   Define(const char *s);

	void   ParamAdd(Вектор<Ткст>& param, const char *b, const char *e);
	Ткст расширь(const char *s);
	void   DoFlatInclude(const Ткст& header_path);
	void   Do(const Ткст& sourcefile, Поток& in, const Ткст& currentfile, bool get_macros);

	bool   Preprocess(const Ткст& sourcefile, Поток& in, const Ткст& currentfile,
	                  bool just_get_macros = false);

	Ткст GetDependeciesMd5(const Вектор<Ткст>& m);
	
	typedef Cpp ИМЯ_КЛАССА;
};

enum tk_Keywords {
	Tmarker_before_first = 255,
#define CPPID(x)   tk_##x,
#include "keyword.i"
#undef  CPPID
};

enum t_Terms {
	t_eof,
	t_string = -200,
	t_integer,
	t_double,
	t_character,
	t_dblcolon,       // ::
	t_mulass,         // *=
	t_divass,         // /=
	t_modass,         // %=
	t_xorass,         // ^=
	t_neq,            // <>
	t_dot_asteriks,   // .*
	t_elipsis,        // ...
	t_inc,            // ++
	t_addass,         // +=
	t_dec,            // -
	t_arrow_asteriks, // ->*
	t_arrow,          // ->
	t_subass,         // -=
	t_and,            // &
	t_andass,         // &=
	t_or,             // |
	t_orass,          // |=
	t_eq,             // ==
	t_shl,            // <<
	t_shlass,         // <<=
	t_le,             // <=
	t_shr,            // >>
	t_shrass,         // >>=
	t_ge,             // >=
	te_integeroverflow,
	te_badcharacter,
	te_badstring,
};

class LexSymbolStat
{
public:
	LexSymbolStat();
	void переустанов(int minSymbol, int maxSymbol);
	void IncStat(int symbol);
	int  GetStat(int symbol) const;
	int  суммаStat(const Вектор<int> & symbols) const;
	void Merge(const LexSymbolStat & other);

private:
	Вектор<int> v;
	int minSymbol;
};


class Lex {
#ifdef _ОТЛАДКА
	const char *pp;
#endif
	const char *ptr;
	const char *pos;

	Индекс<Ткст> id;
	int           endkey;
	int           braceslevel;

	struct прекрати  : Движимое<прекрати>{
		const  char *ptr;
		int    code;
		Ткст text;
		double number;
		bool   grounding;
		
		прекрати() { grounding = false; }
	};

	bool statsCollected;
	LexSymbolStat symbolStat;
	БиВектор<прекрати> term;
	int            body;
	bool           grounding;

	bool сим(int c)                 { if(*ptr == c) { ptr++; return true; } else return false; }
	void добавьКод(int code)           { прекрати& tm = term.добавьХвост(); tm.code = code; tm.ptr = pos; tm.grounding = grounding; }
	void AssOp(int noass, int ass)   { добавьКод(char('=') ? ass : noass); }
	void следщ();
	bool Prepare(int pos);
	int  дайСимвол();
	void выведиОш(const char *e);

public:
	Событие<const Ткст&> WhenError;

	struct Grounding {};

	int         Code(int pos = 0);
	bool        ид_ли(int pos = 0);
	Ткст      Ид(int pos = 0);
	int         Цел(int pos = 0);
	double      Дво(int pos = 0);
	int         Chr(int pos = 0);
	Ткст      устТекст(int pos = 0);

	void        дай(int n = 1);
	int         дайКод()                   { int q = Code(); дай(); return q; }
	Ткст      дайИд()                     { Ткст q = Ид(); дай(); return q; }
	int         дайЦел()                    { int q = Цел(); дай(); return q; }
	double      дайДво()                 { double q = Дво(); дай(); return q; }
	int         GetChr()                    { int q = Chr(); дай(); return q; }
	Ткст      дайТекст()                   { Ткст q = устТекст(); дай(); return q; }

	int         Ид(const Ткст& s)         { return id.найдиДобавь(s) + 256; }

	int         GetBracesLevel() const      { return braceslevel; }
	void        ClearBracesLevel()          { braceslevel = 0; }
	
	void        BeginBody()                 { body++; }
	void        EndBody()                   { body--; }
	void        ClearBody()                 { body = 0; }
	bool        IsBody() const              { return body; }
	bool        IsGrounded()                { Code(); return term.дайСчёт() && term[0].grounding; }
	void        SkipToGrounding();

	const char *Поз(int pos = 0);
	int         operator[](int pos)         { return Code(pos); }
	operator    int()                       { return Code(0); }
	void        operator++()                { return дай(); }
	
	void        Dump(int pos);

	void        иниц(const char *s);
	void        StartStatCollection();
	const LexSymbolStat & FinishStatCollection();

	Lex();
};

struct SrcFile {
	SrcFile();
	rval_default(SrcFile);

	Ткст      text;
	Вектор<int> linepos;
	int preprocessorLinesRemoved;
	int blankLinesRemoved;
	int commentLinesRemoved;
};

struct Parser;

SrcFile PreProcess(Поток& in, Parser& parser);

Ткст CppItemKindAsString(int kind);

struct Parser : ParserContext {
	struct Decla {
		bool    s_static = false;
		bool    s_extern = false;
		bool    s_register = false;
		bool    s_mutable = false;
		bool    s_explicit = false;
		bool    s_virtual = false;
		Ткст  имя;
		bool    function = false;
		bool    type_def = false;
		bool    isfriend = false;
		bool    istemplate = false;
		bool    istructor = false;
		bool    isdestructor = false;
		bool    isptr = false;
		bool    nofn = false;
		bool    oper = false;
		bool    castoper = false;

		Ткст  tnames;
		Ткст  тип;
		Ткст  natural;
	};

	struct Decl : Decla {
		Массив<Decl> param;
		
		rval_default(Decl);
		Decl() {}
	};

	struct RecursionCounter
	{
		int change;
		int &count;
		RecursionCounter(int &count_i, int change_i = 1) :
		  change(change_i), count(count_i)
		{
		  count += change;
		}
		~RecursionCounter() { count -= change; }
	};

	SrcFile     file;
	Lex         lex;
	int         filei;
	byte        filetype;
	Ткст      title;
	int         struct_level;

	Событие<int, const Ткст&> err;

	int     lpos, line;

	CppBase *base;

	int    RPtr();

	bool   Ключ(int code);
	bool   EatBody();
	Ткст ResolveAutoType();
	void   TryLambda();
	bool   Skipars(int& q);

	void   Cv();
	Ткст TType();
	Ткст ReadType(Decla& d, const Ткст& tname, const Ткст& tparam);
	void   Qualifier(bool override_final = false);
	void   ParamList(Decl& d);
	void   Declarator(Decl& d, const char *p);
	void   EatInitializers();
	void   Vars(Массив<Decl>& r, const char *p, bool type_def, bool more);
	void   ReadMods(Decla& d);
	Массив<Decl> Declaration0(bool l0, bool more, const Ткст& tname, const Ткст& tparam);
	Массив<Decl> Declaration(bool l0/* = false*/, bool more/* = false*/, const Ткст& tname, const Ткст& tparam);
	bool   IsParamList(int q);
	void   Elipsis(Decl& d);
	Decl&  финиш(Decl& d, const char *p);
	void   AddNamespace(const Ткст& n, const Ткст& имя);
	bool   Scope(const Ткст& tp, const Ткст& tn);

	Ткст TemplateParams(Ткст& pnames);
	Ткст TemplateParams();
	Ткст TemplatePnames();
	Ткст Имя(Ткст& h, bool& castoper, bool& oper);
	Ткст Имя(bool& castoper, bool& oper);
	Ткст Constant();
	Ткст ReadOper(bool& castoper);

	int    дайСтроку(const char *pos);
	void   Строка();
	void   Check(bool b, const char *err);
	void   CheckKey(int c);

	void   ClassEnum(const Ткст& clss);
	bool   IsEnum(int i);
	bool   UsingNamespace();
	void   SetScopeCurrent();
	void   ScopeBody();
	void   DoNamespace();
	void   Do();
	Ткст AnonymousName();
	Ткст StructDeclaration(const Ткст& tp, const Ткст& tn);
	void   Enum(bool vars);

	CppItem& Элемент(const Ткст& scope, const Ткст& using_namespace, const Ткст& элт,
	              const Ткст& имя, bool impl);
	CppItem& Элемент(const Ткст& scope, const Ткст& using_namespace, const Ткст& элт,
	              const Ткст& имя);

	CppItem& Фн(const Decl& d, const Ткст& templ, bool body,
	            const Ткст& tname, const Ткст& tparam);

	struct Ошибка {};

	void   выведиОш(const Ткст& e);
	void   Resume(int bl);

	void   MatchPars();
	bool   VCAttribute();
	bool   TryDecl();
	void   Statement();
	void   Locals(const Ткст& тип);
	Ткст Tparam(int& q);
	bool   IsNamespace(const Ткст& scope);
	
	friend class Lex; // фиксируй to make Lex::выведиОш

	typedef Parser ИМЯ_КЛАССА;

public:
	void AddMacro(int lineno, const Ткст& macro, int kind = MACRO);

	bool                      dobody;
	Функция<Ткст(Ткст, Ткст, Ткст)> qualify; // used to qualify local variable names (needs main codebase and its mutex)
	int                       currentScopeDepth;
	int                       maxScopeDepth;
	
	Ткст                    namespace_info;

	const SrcFile &getPreprocessedFile() { return file; }

	void  Do(Поток& in, CppBase& _base, int file, int filetype,
	         const Ткст& title, Событие<int, const Ткст&> _err,
	         const Вектор<Ткст>& typenames,
	         const Вектор<Ткст>& namespace_stack,
	         const Индекс<Ткст>& namespace_using);

	Parser() : dobody(false) { lex.WhenError = THISBACK(выведиОш); }
};

void   QualifyTypes(CppBase& base, const Ткст& scope, CppItem& m);
Ткст QualifyKey(const CppBase& base, const Ткст& scope, const Ткст& тип, const Ткст& usings);

const Массив<CppItem>& GetTypeItems(const CppBase& codebase, const Ткст& тип);

Вектор<Ткст> MakeXP(const char *s);

};

#endif
