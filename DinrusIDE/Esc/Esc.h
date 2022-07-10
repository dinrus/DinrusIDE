#ifndef _ESC_ESC_H_
#define _ESC_ESC_H_

#include <Core/Core.h>


namespace РНЦП {

enum EscTypeKind { ESC_VOID, ESC_DOUBLE, ESC_ARRAY, ESC_MAP, ESC_LAMBDA, ESC_INT64 };

Ткст EscTypeName(int sv_type);

struct EscEscape;
class  EscLambda;
struct EscHandle;
struct Esc;

class EscValue : Движимое<EscValue> {
	struct RefCount {
		Атомар   refcount;
		RefCount()              { refcount = 1; }
	};

	int              тип;
	mutable hash_t   hash;

	struct EscMap;
	struct EscArray;

	union {
		double         number;
		int64          i64;
		EscArray      *array;
		EscMap        *map;
		EscLambda     *lambda;
	};

	void                  освободи();
	void                  присвой(const EscValue& s);

	void                  InitString(const ШТкст& w);
	Вектор<EscValue>&     CloneArray();

	ВекторМап<EscValue, EscValue>& CloneMap();

	static int             total;
	static int             max_total;

public:
	static int             GetTotalCount();
	static void            SetMaxTotalCount(int n);
	static int             GetMaxTotalCount();

	bool проц_ли() const                          { return тип == ESC_VOID; }
	EscValue();

	bool                   число_ли() const      { return findarg(тип, ESC_DOUBLE, ESC_INT64) >= 0; }
	double                 GetNumber() const;
	bool                   IsInt64() const       { return тип == ESC_INT64; }
	int64                  дайЦел64() const;
	bool                   цел_ли() const;
	int                    дайЦел() const;
	EscValue(double n);
	EscValue(int64 n);
	EscValue(int n);

	bool                    IsArray() const      { return тип == ESC_ARRAY; }
	const Вектор<EscValue>& дайМассив() const;
	EscValue                ArrayGet(int i) const;
	EscValue                ArrayGet(int i, int n) const;
	bool                    ArraySet(int i, EscValue val);
	bool                    замени(int i, int n, EscValue a);
	void                    SetEmptyArray();
	void                    ArrayAdd(EscValue val);
	bool                    приставь(EscValue a);

	operator ШТкст() const;
	operator Ткст() const                     { return operator ШТкст().вТкст(); }

	EscValue(const char *s)                     { InitString(ШТкст(s)); }
	EscValue(const ШТкст& s)                  { InitString(s); }
	EscValue(const Ткст& s)                   { InitString(s.вШТкст()); }

	bool                                 IsMap() const         { return тип == ESC_MAP; }
	const ВекторМап<EscValue, EscValue>& дайМап() const;
	EscValue                             MapGet(EscValue ключ) const;
	void                                 MapSet(EscValue ключ, EscValue значение);
	void                                 SetEmptyMap();

	bool                                 IsLambda() const     { return тип == ESC_LAMBDA; }
	const EscLambda&                     GetLambda() const;
	EscLambda&                           CreateLambda();


	void    Escape(const char *method, Событие<EscEscape&> escape);
	void    Escape(const char *method, EscHandle *h, Событие<EscEscape&> escape);
	bool    HasNumberField(const char *id) const;
	int     GetFieldInt(const char *id) const;

	int     дайТип() const                                   { return тип; }
	Ткст  GetTypeName() const                               { return РНЦП::EscTypeName(тип); }

	int     дайСчёт() const;

	hash_t   дайХэшЗнач() const;
	bool     operator==(const EscValue& a) const;
	bool     operator!=(const EscValue& a) const              { return !(*this == a); }

	Ткст вТкст(int maxlen = INT_MAX, int indent_step = 4, bool hex = false, int indent = 0) const;

	EscValue& operator=(const EscValue& s);
	EscValue(const EscValue& src);

	~EscValue();
};

struct EscValue::EscArray : EscValue::RefCount {
	Вектор<EscValue> array;

	void     Retain()        { атомнИнк(refcount); }
	void     отпусти()       { if(атомнДек(refcount) == 0) delete this; }
};

struct EscValue::EscMap : EscValue::RefCount {
	ВекторМап<EscValue, EscValue> map;
	int                           count;

	void     Retain()        { атомнИнк(refcount); }
	void     отпусти()       { if(атомнДек(refcount) == 0) delete this; }

	EscMap()                 { count = 0; }
};

struct EscHandle {
	Атомар   refcount;

	void       Retain()        { атомнИнк(refcount); }
	void       отпусти()       { if(атомнДек(refcount) == 0) delete this; }

	EscHandle()                { refcount = 0; }
	virtual ~EscHandle()       {}
};

class EscLambda {
	Атомар   refcount;

	void     Retain()        { атомнИнк(refcount); }
	void     отпусти()       { if(атомнДек(refcount) == 0) delete this; }

	EscLambda()                 { refcount = 1; varargs = false; handle = NULL; }
	~EscLambda()                { if(handle) handle->отпусти(); }

	friend class EscValue;

public:
	Вектор<Ткст>        arg;
	Вектор<Ткст>        def;
	Вектор<bool>          inout;
	Ткст                code;
	EscHandle            *handle;
	Событие<EscEscape&>     escape;
	bool                  varargs;
	Ткст                filename;
	int                   line;

private:
	EscLambda(const EscLambda&);
	void operator=(const EscLambda&);
};

template <>
inline hash_t дайХэшЗнач(const EscValue& v)
{
	return v.дайХэшЗнач();
}

bool     IsTrue(const EscValue& a);

void     SkipBlock(СиПарсер& p);
EscValue ReadLambda(СиПарсер& p);
EscValue ReadLambda(const char *s);

struct Esc : public СиПарсер {
	struct SRVal : Движимое<SRVal> {
		EscValue *lval;
		EscValue  rval;
		EscValue  sbs;

		SRVal()                    { lval = NULL; }
		SRVal(const EscValue& v)   { lval = NULL; rval = v; }
		SRVal(double n)            { lval = NULL; rval = n; }
		SRVal(int64 n)             { lval = NULL; rval = n; }
		SRVal(uint64 n)            { lval = NULL; rval = (int64)n; }
		SRVal(bool n)              { lval = NULL; rval = (int64)n; }
	};

	МассивМап<Ткст, EscValue>& global;
	EscValue                    self;
	МассивМап<Ткст, EscValue>  var;

	int      skipexp;
	int      loop;
	bool     no_break, no_return, no_continue;
	int&     op_limit;
	int      r_stack_level;
	EscValue return_value;

	static int stack_level;

	void       OutOfMemory();

	void       TestLimit();
	double     DoCompare(const EscValue& a, const EscValue& b, const char *op);
	double     DoCompare(const SRVal& a, const char *op);
	Ткст     ReadName();
	EscValue   ExecuteLambda(const Ткст& id, EscValue lambda, SRVal self, Вектор<SRVal>& arg);

	void       присвой(EscValue& val, const Вектор<EscValue>& sbs, int si, const EscValue& src);

	EscValue   дай(const SRVal& val);
	void       присвой(const SRVal& val, const EscValue& src);

	EscValue   GetExp();

	double Number(const EscValue& a, const char *oper);
	int64  Цел(const EscValue& a, const char *oper);
	double Number(const SRVal& a, const char *oper);
	int64  Цел(const SRVal& a, const char *oper);

	EscValue   MulArray(EscValue array, EscValue times);

	void  Subscript(SRVal& r, SRVal _self, Ткст id);
	void  Subscript(SRVal& r);
	void  прекрати(SRVal& r);
	void  Unary(SRVal& r);
	void  Mul(SRVal& r);
	void  добавь(SRVal& r);
	void  Shift(SRVal& r);
	void  сравни(SRVal& r);
	void  Equal(SRVal& r);
	void  BinAnd(SRVal& r);
	void  BinXor(SRVal& r);
	void  BinOr(SRVal& r);
	void  And(SRVal& r);
	void  Or(SRVal& r);
	void  Cond(SRVal& r);
	void  присвой(SRVal& r);
	void  Exp(SRVal& r);

	void  пропустиТерм();
	void  SkipStatement();
	void  SkipExp();
	bool  PCond();
	void  FinishSwitch();
	void  DoStatement();

	void  пуск();

	Esc(МассивМап<Ткст, EscValue>& global, const char *s, int& oplimit,
	    const Ткст& фн, int line = 1)
	: СиПарсер(s, фн, line), global(global), op_limit(oplimit)
	{ r_stack_level = stack_level;  skipexp = false; }
	~Esc() { stack_level = r_stack_level; }
};

struct EscEscape {
	Esc&             esc;
	EscValue         self;
	Массив<EscValue>& arg;
	EscValue         ret_val;
	Ткст           id;

	EscValue&    operator[](int i)             { return arg[i]; }
	int          дайСчёт() const              { return arg.дайСчёт(); }
	void         operator=(const EscValue& v)  { ret_val = v; }
	operator     EscValue&()                   { return ret_val; }
	void         выведиОш(const char *s)     { esc.выведиОш(s); }

	Ткст       DumpType(int i);
	Ткст       InCall();

	void         CheckNumber(int i);
	void         CheckArray(int i);
	void         CheckMap(int i);

	double       Number(int i);
	int          Цел(int i);

	EscEscape(Esc& esc, EscValue self, Массив<EscValue>& arg)
	  : esc(esc), self(self), arg(arg) {}
};

void Escape(МассивМап<Ткст, EscValue>& globals, const char *function, void (*escape)(EscEscape& e));

inline // resolve overloading with Функция...
void Escfn(МассивМап<Ткст, EscValue>& globals, const char *function, void (*escape)(EscEscape& e))
{
	return Escape(globals, function, escape);
}

void Escape(МассивМап<Ткст, EscValue>& globals, const char *function, Событие<EscEscape&> escape);

void скан(МассивМап<Ткст, EscValue>& global, const char *code, const char *filename = "");

void StdLib(МассивМап<Ткст, EscValue>& global);

void     LambdaArgs(СиПарсер& p, EscLambda& l);

EscValue выполни(МассивМап<Ткст, EscValue>& global, EscValue *self,
                 const EscValue& lambda, Вектор<EscValue>& arg, int oplimit = 50000);
EscValue выполни(МассивМап<Ткст, EscValue>& global, EscValue *self,
                 const char *имя, Вектор<EscValue>& arg, int oplimit = 50000);
EscValue выполни(МассивМап<Ткст, EscValue>& global, const char *имя, int oplimit = 50000);

EscValue Evaluatex(const char *expression, МассивМап<Ткст, EscValue>& global, int oplimit = 50000);
EscValue Evaluate(const char *expression, МассивМап<Ткст, EscValue>& global, int oplimit = 50000);

EscValue EscFromStdValue(const Значение& v);
Значение    StdValueFromEsc(const EscValue& v);
void     StdValueLib(МассивМап<Ткст, EscValue>& global);

bool     IsDate(const EscValue& v);
bool     IsTime(const EscValue& v);

Ткст   расширь(const Ткст& doc, МассивМап<Ткст, EscValue>& global,
                int oplimit = 50000, Ткст (*формат)(const Значение& v) = стдФормат);

}

#endif
