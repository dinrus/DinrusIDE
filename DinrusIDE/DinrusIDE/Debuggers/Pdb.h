#include <winver.h>
#include <dbghelp.h>
#include <psapi.h>
#include "cvconst.h"

#include <plugin/ndisasm/ndisasm.h>

using namespace PdbKeys;

struct Pdb : Отладчик, КтрлРодитель {
	virtual void стоп();
	virtual bool окончен();
	virtual bool Ключ(dword ключ, int count);
	virtual void DebugBar(Бар& bar);
	virtual bool SetBreakpoint(const Ткст& filename, int line, const Ткст& bp);
	virtual bool RunTo();
	virtual void пуск();
	virtual bool Подсказка(const Ткст& exp, РедакторКода::MouseTip& mt);
	virtual void сериализуй(Поток& s);

	struct ИнфОМодуле : Движимое<ИнфОМодуле> {
		adr_t  base;
		dword  size;
		Ткст path;
		bool   symbols;

		ИнфОМодуле() { base = size = 0; symbols = false; }
	};

	struct FilePos {
		Ткст path;
		int    line;
		adr_t  address;

		operator bool() const                              { return !пусто_ли(path); }

		FilePos()                                          { line = 0; address = 0; }
		FilePos(const Ткст& p, int l) : path(p), line(l) { address = 0; }
	};

	enum CpuRegisterKind {
		REG_L, REG_H, REG_X, REG_E, REG_R
	};
	
	struct РегистрЦпб : Движимое<РегистрЦпб> {
		int         sym;    // DbgHelp register symbol
		const char *имя;   // NULL: Do not list (e.g. al, as it is printed as EAX or RAX
		int         kind;   // CpuRegisterKind
		dword       flags;  // Unused
	};
	
	struct MemPg : Движимое<MemPg> {
		char data[1024];
	};

	struct PdbHexView : ГексОбзор {
		Pdb *pdb;

		virtual int Байт(int64 addr)            { return pdb ? pdb->Байт((adr_t)addr) : 0; }

		PdbHexView()                            { pdb = NULL; }
	}
	memory;

	struct FnInfo : Движимое<FnInfo> {
		Ткст имя;
		adr_t  address;
		dword  size;
		dword  pdbtype;

		FnInfo() { address = size = pdbtype = 0; }
	};

	enum { UNKNOWN = -99, BOOL1, SINT1, UINT1, SINT2, UINT2, SINT4, UINT4, SINT8, UINT8, FLT, DBL, PFUNC };

	struct Контекст {
	#ifdef CPU_64
		union {
			CONTEXT context64;
			WOW64_CONTEXT context32;
		};
	#else
		CONTEXT context32;
	#endif
	};
	
	struct TypeInfo : Движимое<TypeInfo> {
		int    тип = UNKNOWN;
		int    ref = 0; // this is pointer or reference
		bool   reference = false; // this is reference
	};

	struct Val : Движимое<Val, TypeInfo> {
		bool   array = false;
		bool   rvalue = false; // data is loaded from debugee (if false, data pointed to by address)
		bool   udt = false; // user defined тип (e.g. struct..)
		byte   bitpos = 0;
		byte   bitcnt = 0;
		int    reported_size = 0; // size of symbol, can be 0 - unknown, useful for C fixed size arrays
		union {
			adr_t  address;
			int64  ival;
			double fval;
		};
		Контекст *context = NULL; // needed to retrieve register variables

#ifdef _ОТЛАДКА
		Ткст вТкст() const;
#endif

		Val() { address = 0; }
	};

	struct NamedVal : Движимое<NamedVal> {
		Ткст имя;
		Val    val;
		Val    ключ;
		int64  from = 0;
		bool   exp = false;
	};

	struct Type : Движимое<Type> {
		Type() : size(-1), vtbl_typeindex(-1) {}

		adr_t  modbase;

		Ткст имя;
		int    size;

		int    vtbl_typeindex;
		int    vtbl_offset;

		Вектор<Val>            base;
		ВекторМап<Ткст, Val> member;
		ВекторМап<Ткст, Val> static_member;
	};

	struct Фрейм : Движимое<Фрейм> {
		adr_t                  pc, frame, stack;
		FnInfo                 фн;
		ВекторМап<Ткст, Val> param;
		ВекторМап<Ткст, Val> local;
		Ткст                 text;
	};

	struct VisualPart : Движимое<VisualPart> {
		Ткст  text;
		Цвет   ink;
		bool    mark;
		
		Размер    дайРазм() const;
	};

	struct Visual {
		int                length;
		Вектор<VisualPart> part;

		void   конкат(const Ткст& text, Цвет ink = SColorText);
		void   конкат(const char *text, Цвет ink = SColorText);
		Ткст дайТкст() const;
		void   очисть() { part.очисть(); length = 0; }
		Размер   дайРазм() const;

		Visual()       { length = 0; }
	};

	struct VisualDisplay : Дисплей {
		Pdb *pdb;

		virtual Размер дайСтдРазм(const Значение& q) const;
		virtual void рисуй(Draw& w, const Прям& r, const Значение& q,
	                       Цвет ink, Цвет paper, dword style) const;

		VisualDisplay(Pdb *pdb) : pdb(pdb) {}
	} visual_display;

	struct PrettyImage {
		Размер  size;
		adr_t pixels;
	};
	
	struct Нить : Контекст {
		HANDLE  hThread;
		adr_t   sp;
	};
	
	int                         lock;
	bool                        running;
	bool                        stop;
	HANDLE                      hProcess;
	HANDLE                      mainThread;
	DWORD                       processid;
	DWORD                       hProcessId;
	DWORD                       mainThreadId;
	МассивМап<dword, Нить>     threads;
	bool                        terminated;
	bool                        refreshmodules;
	Вектор<ИнфОМодуле>          module;
	DEBUG_EVENT                 event;
	DWORD                       debug_threadid;
	HWND                        hWnd;
	ВекторМап<adr_t, byte>      bp_set; // breakpoints active for single RunToException

	bool                        clang; // we are in clang toolchain
	bool                        win64; // debugee is 64-bit, always false in 32-bit exe

	Контекст                     context;

	Индекс<adr_t>                invalidpage;
	ВекторМап<adr_t, MemPg>     mempage;

	Индекс<adr_t>                breakpoint;
	Вектор<Ткст>              breakpoint_cond;

	МассивМап<int, Type>         тип; // maps pdb pSym->TypeIndex to тип data
	ВекторМап<Ткст, int>      type_name; // maps the имя of тип to above 'тип' Индекс
	Индекс<adr_t>                type_bases; // Индекс of modbases for types (usually one)
	ВекторМап<Ткст, int>      type_index; // maps the имя of тип to TypeIndex, loaded by SymEnumTypes

	Ткст                      disas_name;

	Массив<Фрейм>                frame;
	Фрейм                      *current_frame;
	Ткст                      autotext;
	
	ВекторМап<adr_t, FnInfo>    fninfo_cache;

	DbgDisas           disas;

	EditString         watchedit;
	
	enum { // Порядок in this enum has to be same as order of tab.добавь
		TAB_AUTOS, TAB_LOCALS, TAB_THIS, TAB_WATCHES, TAB_CPU, TAB_MEMORY, TAB_BTS
	};

	КтрлВкладка            tab;
	СписокБроса           threadlist;
	СписокБроса           framelist;
	Кнопка             frame_up, frame_down;
	Надпись              dlock;
	КтрлМассив          locals;
	КтрлМассив          self;
	КтрлМассив          watches;
	КтрлМассив          autos;
	СписокКолонок         cpu;
	EditString         expexp;
	Кнопка             exback, exfw;
	СтатичПрям         explorer_pane;
	СтатичПрям         pane, rpane;
	КтрлДерево           tree;
	Ткст             tree_exp;
	bool               first_exception = true;
	КтрлДерево           bts; // backtraces of all threads

	ВекторМап<Ткст, Ткст> treetype;
	int restoring_tree = 0;

	Вектор<Ткст>      exprev, exnext;

	Индекс<Ткст>          noglobal;
	ВекторМап<Ткст, Val> global;

		
	adr_t                       current_modbase; // so that we do not need to pass it as parameter to GetTypeInfo
	ВекторМап<Ткст, TypeInfo> typeinfo_cache;
	
	enum { SINGLE_VALUE, TEXT, CONTAINER };
	
	struct LengthLimit {};

	struct Pretty {
		int            kind; // VARIABLE, TEXT or CONTAINER
		int64          data_count = 0; // number of entries
		Вектор<Ткст> data_type; // тип of data items (usually type_param)
		Вектор<adr_t>  data_ptr; // pointer to items (data_count.дайСчёт() * data_type.дайСчёт() items)
		Visual         text;
		bool           has_data = true; // do дисплей the data for SINGLE_VALUE (false for e.g. void Значение)

		void           устТекст(const char *s, Цвет color = SRed)   { text.конкат(s, color); has_data = false; }
		void           устТекст(const Ткст& s, Цвет color = SRed) { text.конкат(s, color); has_data = false; }
		void           устПусто()                                 { устТекст("Null", SCyan); has_data = false; }
	};

	ВекторМап<Ткст, Кортеж<int, Событие<Val, const Вектор<Ткст>&, int64, int, Pdb::Pretty&>>> pretty;
	
	bool       break_running; // Needed for Wow64 BreakRunning to avoid ignoring breakpoint
	
	bool       show_type = false;
	bool       raw = false;
	
	int        bc_lvl = 0; // For coloring { } in pretty container дисплей

	void       Ошибка(const char *s = NULL);

	Ткст     Гекс(adr_t);

// CPU registers
	uint32 дайРегистр32(const Контекст& ctx, int sym);
	uint64 дайРегистр64(const Контекст& ctx, int sym);

	const ВекторМап<int, РегистрЦпб>& дайСписокРегистров();
	uint64     дайРегистрЦпб(const Контекст& ctx, int sym);

// debug
	Контекст   читайКонтекст(HANDLE hThread);
	void       пишиКонтекст(HANDLE hThread, Контекст& context);
	void       LoadModuleInfo();
	int        FindModuleIndex(adr_t base);
	void       UnloadModuleSymbols();
	void       добавьНить(dword dwThreadId, HANDLE hThread);
	void       RemoveThread(dword dwThreadId);
	void       блокируй();
	void       разблокируй();
	void       ToForeground();
	bool       RunToException();
	bool       AddBp(adr_t address);
	bool       RemoveBp(adr_t address);
	bool       RemoveBp();
	bool       IsBpSet(adr_t address) const { return bp_set.найди(address) >= 0; }
	bool       Continue();
	bool       SingleStep();
	void       BreakRunning();
	bool       ConditionalPass();
	void       SetBreakpoints();
	void       SaveForeground();
	void       RestoreForeground();
	void       SyncFrameButtons();

	adr_t      GetIP();

	void       пишиКонтекст();

// mem
	int        Байт(adr_t addr);
	bool       копируй(adr_t addr, void *ptr, int count);
	Ткст     читайТкст(adr_t addr, int maxlen, bool allowzero = false);
	ШТкст    читайШТкст(adr_t addr, int maxlen, bool allowzero = false);

// sym
	struct LocalsCtx;
	static BOOL CALLBACK  EnumLocals(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext);
	static BOOL CALLBACK  EnumGlobals(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext);
	void                  TypeVal(Pdb::Val& v, int typeId, adr_t modbase);
	Ткст                GetSymName(adr_t modbase, dword typeindex);
	dword                 GetSymInfo(adr_t modbase, dword typeindex, IMAGEHLP_SYMBOL_TYPE_INFO info);
	const Type&           дайТип(int ti);
	int                   GetTypeIndex(adr_t modbase, dword typeindex);
	Val                   GetGlobal(const Ткст& имя);

	adr_t                 GetAddress(FilePos p);
	FilePos               GetFilePos(adr_t address);
	FnInfo                GetFnInfo0(adr_t address);
	FnInfo                GetFnInfo(adr_t address);
	void                  GetLocals(Фрейм& frame, Контекст& context,
	                                ВекторМап<Ткст, Pdb::Val>& param,
	                                ВекторМап<Ткст, Pdb::Val>& local);
	Ткст                TypeAsString(int ti, bool deep = true);
	int                   FindType(adr_t modbase, const Ткст& имя);

	Ткст                TypeInfoAsString(TypeInfo tf);
	TypeInfo              GetTypeInfo(adr_t modbase, const Ткст& имя);
	TypeInfo              GetTypeInfo(const Ткст& имя) { return GetTypeInfo(current_modbase, имя); } // only in Pretty...
	
	static Ткст FormatString(const Ткст& x) { return какТкстСи(x, INT_MAX, NULL, проверьУтф8(x) ? 0 : ASCSTRING_OCTALHI); }

// exp
	Val        делайЗнач(const Ткст& тип, adr_t address);
	void       выведиОш(const char *s);
	int        размТипа(int ti);
	int        размТипа(const Ткст& имя);
	adr_t      подбериУк(adr_t address);
	byte       PeekByte(adr_t address);
	word       PeekWord(adr_t address);
	dword      PeekDword(adr_t address);
	Val        GetRVal(Val v);
	Val        DeRef(Val v);
	Val        Реф(Val v);
	int64      дайЦел64(Val v);
	int        дайЦел(Val v)                                 { return (int)дайЦел64(v); }
	double     GetFlt(Val v);
	void       ZeroDiv(double x);
	Val        Compute(Val v1, Val v2, int oper);
	Val        RValue(int64 v);
	Val        Field0(Pdb::Val v, const Ткст& field);
	Val        Field(Pdb::Val v, const Ткст& field);
	Val        прекрати(СиПарсер& p);
	Val        пост(СиПарсер& p);
	Val        Unary(СиПарсер& p);
	Val        Additive(СиПарсер& p);
	Val        Multiplicative(СиПарсер& p);
	Val        сравни(Val v, СиПарсер& p, int r1, int r2);
	void       GetBools(Val v1, Val v2, bool& a, bool& b);
	Val        LogAnd(СиПарсер& p);
	Val        LogOr(СиПарсер& p);
	Val        Comparison(СиПарсер& p);
	Val        Exp0(СиПарсер& p);
	Val        Exp(СиПарсер& p);

	bool       HasAttr(Pdb::Val record, const Ткст& id);
	Val        дайАтр(Pdb::Val record, int i);
	Val        дайАтр(Pdb::Val record, const Ткст& id);
	int64      GetInt64Attr(Pdb::Val v, const char *a);
	int        GetIntAttr(Pdb::Val v, const char *a)         { return (int)GetInt64Attr(v, a); }
	byte       GetByteAttr(Pdb::Val v, const char *a)        { return (byte)GetInt64Attr(v, a); }
	Val        по(Pdb::Val val, int i);
	Val        по(Pdb::Val record, const char *id, int i);
	int        IntAt(Pdb::Val record, const char *id, int i);
	void       CatInt(Visual& result, int64 val, dword flags = 0);

	enum       { MEMBER = 1, RAW = 2 };
	void       BaseFields(Visual& result, const Type& t, Pdb::Val val, dword flags, bool& cm, int depth);
	void       Visualise(Visual& result, Pdb::Val val, dword flags);
	Visual     Visualise(Val v, dword flags = 0);
	Visual     Visualise(const Ткст& rexp, dword flags = 0);

	bool       VisualisePretty(Visual& result, Pdb::Val val, dword flags);

	bool       PrettyVal(Pdb::Val val, int64 from, int count, Pretty& p);
	void       PrettyString(Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pretty& p);
	void       PrettyWString(Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pretty& p);
	void       PrettyVector(Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pretty& p);
	void       PrettyArray(Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pretty& p);
	void       PrettyBiVector(Pdb::Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p);
	void       PrettyBiArray(Pdb::Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p);
	void       PrettyIndex(Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pretty& p);
	void       PrettyMap(Pretty& p, Pretty& ключ, Pretty& значение);
	void       PrettyVectorMap(Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pretty& p);
	void       PrettyArrayMap(Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pretty& p);
	void       PrettyDate(Pdb::Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p);
	void       PrettyTime(Pdb::Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p);
	void       PrettyColor(Pdb::Val val, const Вектор<Ткст>&, int64 from, int count, Pdb::Pretty& p);
	void       PrettyRGBA(Pdb::Val val, const Вектор<Ткст>&, int64 from, int count, Pdb::Pretty& p);
	void       PrettyImageBuffer(Pdb::Val val, const Вектор<Ткст>&, int64 from, int count, Pdb::Pretty& p);
	void       PrettyImg(Pdb::Val val, const Вектор<Ткст>&, int64 from, int count, Pdb::Pretty& p);
	void       PrettyFont(Pdb::Val val, const Вектор<Ткст>&, int64 from, int count, Pdb::Pretty& p);
	void       PrettyValueArray_(adr_t a, Pdb::Pretty& p);
	void       PrettyValueArray(Pdb::Val val, const Вектор<Ткст>&, int64 from, int count, Pdb::Pretty& p);
	void       PrettyValue(Pdb::Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pretty& p);
	void       PrettyValueMap_(adr_t a, Pdb::Pretty& p, int64 from, int count);
	void       PrettyValueMap(Pdb::Val val, const Вектор<Ткст>&, int64 from, int count, Pdb::Pretty& p);

	void       PrettyStdVector(Pdb::Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p);
	void       PrettyStdString(Pdb::Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p);
	void       TraverseTree(bool set, Val head, Val node, int64& from, int& count, Pdb::Pretty& p, int depth);
	void       TraverseTreeClang(bool set, int nodet, Val node, int64& from, int& count, Pdb::Pretty& p, int depth);
	void       PrettyStdTree(Pdb::Val val, bool set, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p);
	void       PrettyStdListM(Pdb::Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p, bool map = false);
	void       PrettyStdList(Pdb::Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p);
	void       PrettyStdForwardList(Pdb::Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p);
	void       PrettyStdDeque(Pdb::Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p);
	void       PrettyStdUnordered(Pdb::Val val, bool set, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p);

// code
	Нить&    Current();
	Массив<Фрейм> Backtrace(Нить& ctx, bool single_frame = false, bool thread_info = false);
	int        дизассемблируй(adr_t ip);
	bool       фреймВалиден(adr_t eip);
	void       Sync0(Нить& ctx);
	void       синх();
	void       устНить();
	void       FrameUpDown(int dir);
	void       устФрейм();
	bool       Step(bool over);
	void       Trace(bool over);
	void       StepOut();
	void       DoRunTo() { RunTo(); }
	adr_t      CursorAdr();
	void       SetIp();

	void       Break();

// data
	static ВекторМап<Ткст, Значение> DataMap(const КтрлМассив& a);
	static Значение Vis(const Ткст& ключ, const ВекторМап<Ткст, Значение>& prev,
	                 Visual&& vis, bool& ch);
	static void  Vis(КтрлМассив& a, const Ткст& ключ,
	                 const ВекторМап<Ткст, Значение>& prev, Visual&& vis);

	void      DisasCursor() {}
	void      DisasFocus() {}
	void      Locals();
	void      Watches();
	void      TryAuto(const Ткст& exp, const ВекторМап<Ткст, Значение>& prev);
	void      Autos();
	void      AddThis(const ВекторМап<Ткст, Val>& m, adr_t address, const ВекторМап<Ткст, Значение>& prev);
	void      AddThis(int тип, adr_t address, const ВекторМап<Ткст, Значение>& prev);
	void      This();
	void      Данные();
	void      ClearWatches();
	void      DropWatch(PasteClip& clip);
	void      AddWatch();
	void      AddWatch(const Ткст& s);
	void      EditWatch();
	void      BTs();

	void      SetTab(int i);

	void      SetTree(const Ткст& exp);
	void      SetTreeA(КтрлМассив *data);
	void      PrettyTreeNode(int parent, Pdb::Val val, int64 from = 0);
	bool      TreeNode(int parent, const Ткст& имя, Val val, int64 from = 0, Цвет ink = SColorText(), bool exp = false);
	bool      TreeNodeExp(int parent, const Ткст& имя, Val val, int64 from = 0, Цвет ink = SColorText());
	void      TreeExpand(int node);
	Ткст    GetTreeText(int id);
	void      GetTreeText(Ткст& r, int id, int depth);
	void      TreeMenu(Бар& bar);
	void      TreeWatch();
	void      StoreTree(ТкстБуф& r, int parent);
	void      SaveTree();
	int       FindChild(int parent, Ткст id);
	void      ExpandTreeType(int parent, СиПарсер& p);

	void      копируйСтэк();
	void      копируйВесьСтэк();
	void      копируйДизас();
	void      CopyModules();

	void      MemoryGoto(const Ткст& exp);
	
	void      CopyMenu(КтрлМассив& array, Бар& bar);
	void      MemMenu(КтрлМассив& array, Бар& bar, const Ткст& exp);
	void      DataMenu(КтрлМассив& array, Бар& bar);
	void      WatchMenu(Бар& bar, const Ткст& exp);
	void      WatchesMenu(Бар& bar);
	
	void      SyncTreeDisas();

	void      Вкладка();

	bool      создай(Хост& local, const Ткст& exefile, const Ткст& cmdline, bool clang_);

	void      SerializeSession(Поток& s);

	typedef Pdb ИМЯ_КЛАССА;

	Pdb();
	virtual ~Pdb();


	void LoadGlobals(DWORD64 base);
};

bool EditPDBExpression(const char *title, Ткст& brk, Pdb *pdb);
