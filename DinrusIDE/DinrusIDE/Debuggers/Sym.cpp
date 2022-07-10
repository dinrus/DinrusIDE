#include "Debuggers.h"

#ifdef PLATFORM_WIN32

#define LLOG(x)  // DLOG(x)

#ifdef _ОТЛАДКА

Ткст SymTagAsString(int n) {
	static ВекторМап<int, Ткст> tagmap = {
		{ SymTagNull, "SymTagNull" },
		{ SymTagExe, "SymTagExe" },
		{ SymTagCompiland, "SymTagCompiland" },
		{ SymTagCompilandDetails, "SymTagCompilandDetails" },
		{ SymTagCompilandEnv, "SymTagCompilandEnv" },
		{ SymTagFunction, "SymTagFunction" },
		{ SymTagBlock, "SymTagBlock" },
		{ SymTagData, "SymTagData" },
		{ SymTagAnnotation, "SymTagAnnotation" },
		{ SymTagLabel, "SymTagLabel" },
		{ SymTagPublicSymbol, "SymTagPublicSymbol" },
		{ SymTagUDT, "SymTagUDT" },
		{ SymTagEnum, "SymTagEnum" },
		{ SymTagFunctionType, "SymTagFunctionType" },
		{ SymTagPointerType, "SymTagPointerType" },
		{ SymTagArrayType, "SymTagArrayType" },
		{ SymTagBaseType, "SymTagBaseType" },
		{ SymTagTypedef, "SymTagTypedef" },
		{ SymTagBaseClass, "SymTagBaseClass" },
		{ SymTagFriend, "SymTagFriend" },
		{ SymTagFunctionArgType, "SymTagFunctionArgType" },
		{ SymTagFuncDebugStart, "SymTagFuncDebugStart" },
		{ SymTagFuncDebugEnd, "SymTagFuncDebugEnd" },
		{ SymTagUsingNamespace, "SymTagUsingNamespace" },
		{ SymTagVTableShape, "SymTagVTableShape" },
		{ SymTagVTable, "SymTagVTable" },
		{ SymTagCustom, "SymTagCustom" },
		{ SymTagThunk, "SymTagThunk" },
		{ SymTagCustomType, "SymTagCustomType" },
		{ SymTagManagedType, "SymTagManagedType" },
		{ SymTagDimension, "SymTagDimension" },
	};
	return tagmap.дай(n, "");
}

const char * BaseTypeAsString( DWORD baseType )
{
	switch ( baseType )
	{
	case btNoType: return "btNoType";
	case btVoid: return "btVoid";
	case btChar: return "btChar";
	case btWChar: return "btWChar";
	case btInt: return "btInt";
	case btUInt: return "btUInt";
	case btFloat: return "btFloat";
	case btBCD: return "btBCD";
	case btBool: return "btBool";
	case btLong: return "btLong";
	case btULong: return "btULong";
	case btCurrency: return "btCurrency";
	case btDate: return "btDate";
	case btVariant: return "btVariant";
	case btComplex: return "btComplex";
	case btBit: return "btBit";
	case btBSTR: return "btBSTR";
	case btHresult: return "btHresult";
	default: return "???";
	}
}
#endif

adr_t Pdb::GetAddress(FilePos p)
{
	LONG dummy;
	IMAGEHLP_LINE ln;
	ln.SizeOfStruct = sizeof(ln);
	char h[MAX_PATH];
	strcpy(h, p.path);
	if(SymGetLineFromName(hProcess, NULL, h, p.line + 1, &dummy, &ln)) {
		LLOG("GetAddress " << p.path << "(" << p.line << "): " << Гекс(ln.Address));
		return ln.Address;
	}
	LLOG("GetAddress " << p.path << "(" << p.line << "): ??");
	return 0;
}

Pdb::FilePos Pdb::GetFilePos(adr_t address)
{
	FilePos fp;
	DWORD dummy;
	IMAGEHLP_LINE ln;
	ln.SizeOfStruct = sizeof(ln);
	fp.address = address;
	if(SymGetLineFromAddr(hProcess, (uintptr_t)address, &dummy, &ln) && файлЕсть(ln.FileName)) {
		fp.line = ln.LineNumber - 1;
		fp.path = ln.FileName;
		fp.address = ln.Address;
	}
	LLOG("GetFilePos(" << Гекс(address) << "): " << fp.path << ": " << fp.line);
	return fp;
}

#define MAX_SYMB_NAME 1024

Pdb::FnInfo Pdb::GetFnInfo0(adr_t address)
{
	DWORD64 h;

	ULONG64 буфер[(sizeof(SYMBOL_INFO) + MAX_SYMB_NAME + sizeof(ULONG64) - 1) / sizeof(ULONG64)];
	SYMBOL_INFO *f = (SYMBOL_INFO*)буфер;

	f->SizeOfStruct = sizeof(SYMBOL_INFO);
	f->MaxNameLen = MAX_SYMB_NAME;

	LLOG("GetFnInfo " << фмт64Гекс(address));
	FnInfo фн;
	if(SymFromAddr(hProcess, address, &h, f)) {
		LLOG("GetFnInfo " << f->Имя
		     << ", тип Index: " << f->TypeIndex
		     << ", Flags: " << фмтЦелГекс(f->Flags)
		     << ", Address: " << Гекс((dword)f->Address)
		     << ", Size: " << фмтЦелГекс((dword)f->Размер)
		     << ", Тэг: " << SymTagAsString(f->Тэг));
		фн.имя = f->Name;
		фн.address = (adr_t)f->Address;
		фн.size = f->Size;
		фн.pdbtype = f->TypeIndex;
	}
	return фн;
}

Pdb::FnInfo Pdb::GetFnInfo(adr_t address)
{
	int q = fninfo_cache.найди(address);
	if(q >= 0)
		return fninfo_cache[q];
	if(fninfo_cache.дайСчёт() > 100)
		fninfo_cache.очисть();
	FnInfo f = GetFnInfo0(address);
	fninfo_cache.добавь(address, f);
	return f;
}

void Pdb::TypeVal(Pdb::Val& v, int typeId, adr_t modbase)
{
	adr_t tag;

	BOOL reference;
	dword dw = 0;
#ifdef COMPILER_MINGW
	SymGetTypeInfo(hProcess, modbase, typeId, (IMAGEHLP_SYMBOL_TYPE_INFO)31, &reference);
#else
	SymGetTypeInfo(hProcess, modbase, typeId, TI_GET_IS_REFERENCE, &reference);
#endif
	v.reference = reference;

	for(;;) {
		tag = GetSymInfo(modbase, typeId, TI_GET_SYMTAG);
		if(tag == SymTagPointerType)
			v.ref++;
		else
		if(tag == SymTagArrayType)
			v.array = true;
		else {
			if(tag == SymTagUDT)
				v.udt = true;
			break;
		}
		typeId = GetSymInfo(modbase, typeId, TI_GET_TYPE); // follow pointer(s) to base тип
	}
	v.тип = UNKNOWN;
	if(tag == SymTagUDT)
		v.тип = GetTypeIndex(modbase, typeId);
	else {
		ULONG64 sz = 0;
		SymGetTypeInfo(hProcess, modbase, typeId, TI_GET_LENGTH, &sz);
		dword size = (dword)sz;
		if(tag == SymTagEnum)
			v.тип = size == 8 ? SINT8 : size == 4 ? SINT4 : size == 2 ? SINT2 : SINT1;
		else {
			switch(GetSymInfo(modbase, typeId, TI_GET_BASETYPE)) {
			case btBool:
				v.тип = BOOL1;
				break;
			case btChar:
			case btWChar:
			case btInt:
			case btLong:
				v.тип = size == 8 ? SINT8 : size == 4 ? SINT4 : size == 2 ? SINT2 : SINT1;
				break;
			case btUInt:
			case btULong:
				v.тип = size == 8 ? UINT8 : size == 4 ? UINT4 : size == 2 ? UINT2 : UINT1;
				break;
			case btFloat:
				v.тип = size == 8 ? DBL : FLT;
				break;
			}
		}
	}
}

struct Pdb::LocalsCtx {
	adr_t                       frame;
	ВекторМап<Ткст, Pdb::Val> param;
	ВекторМап<Ткст, Pdb::Val> local;
	Pdb                        *pdb;
	Контекст                    *context;
};

BOOL CALLBACK Pdb::EnumLocals(PSYMBOL_INFO pSym, ULONG SymbolSize, PVOID UserContext)
{
	LocalsCtx& c = *(LocalsCtx *)UserContext;

	if(pSym->Tag == SymTagFunction)
		return TRUE;

	bool param = pSym->Flags & IMAGEHLP_SYMBOL_INFO_PARAMETER;
	Val& v = (param ? c.param : c.local).добавь(pSym->Name);
	v.address = (adr_t)pSym->Address;
	if(pSym->Flags & IMAGEHLP_SYMBOL_INFO_REGISTER)
		v.address = pSym->Register;
	else
	if(pSym->Flags & IMAGEHLP_SYMBOL_INFO_REGRELATIVE) {
		if(pSym->Register == CV_ALLREG_VFRAME) {
		#ifdef CPU_64
			if(c.pdb->win64)
				v.address += c.pdb->дайРегистрЦпб(*c.context, CV_AMD64_RBP);
			else
		#endif
			{
				adr_t ebp = (adr_t)c.pdb->дайРегистрЦпб(*c.context, CV_REG_EBP);
				if(c.pdb->clang)
					ebp &= ~(adr_t)7;  // Workaround for supposed clang/win32 issue
				v.address += ebp;
			}
		}
		else
			v.address += (adr_t)c.pdb->дайРегистрЦпб(*c.context, pSym->Register);
	}
	else
	if(pSym->Flags & IMAGEHLP_SYMBOL_INFO_FRAMERELATIVE)
		v.address += c.frame;
	
	c.pdb->TypeVal(v, pSym->TypeIndex, (adr_t)pSym->ModBase);
	if(param && v.udt && v.ref == 0 && c.pdb->win64) { // dbghelp.dll incorrectly does not report pointer for (copied) значение struct params
		v.ref++;
		v.reference = true;
	}
	v.reported_size = pSym->Size;
	v.context = c.context;
#if 0
	DLOG("------");
	DDUMP(pSym->Имя);
	DLOG("TYPE: " << (v.тип >= 0 ? c.pdb->дайТип(v.тип).имя : "primitive"));
	DDUMPHEX(pSym->Flags);
	DDUMP(pSym->Flags & IMAGEHLP_SYMBOL_INFO_PARAMETER);
	DDUMP(pSym->Flags & IMAGEHLP_SYMBOL_INFO_REGISTER);
	DDUMP(pSym->Flags & IMAGEHLP_SYMBOL_INFO_REGRELATIVE);
	DDUMP(pSym->Flags & IMAGEHLP_SYMBOL_INFO_FRAMERELATIVE);
	DDUMP(pSym->регистрируй == CV_ALLREG_VFRAME);
	DDUMP(pSym->регистрируй);
	DDUMP(pSym->Scope);
	DDUMP(pSym->Значение);
	DDUMP(pSym->ModBase);
	DDUMPHEX((adr_t)pSym->Address);
#endif

	LLOG("LOCAL " << c.pdb->дайТип(v.тип).имя << " " << pSym->Имя << ": " << фмт64Гекс(v.address));
	return TRUE;
}

void Pdb::GetLocals(Фрейм& frame, Контекст& context, ВекторМап<Ткст, Pdb::Val>& param,
                    ВекторМап<Ткст, Pdb::Val>& local)
{
	LLOG("GetLocals *****************");
	static IMAGEHLP_STACK_FRAME f;
	f.InstructionOffset = frame.pc;
	SymSetContext(hProcess, &f, 0);
	LocalsCtx c;
	c.frame = frame.frame;
	c.pdb = this;
	c.context = &context;
	SymEnumSymbols(hProcess, 0, 0, &EnumLocals, &c);
	param = pick(c.param);
	local = pick(c.local);
	LLOG("===========================");
}

BOOL CALLBACK Pdb::EnumGlobals(PSYMBOL_INFO pSym, ULONG SymbolSize, PVOID UserContext)
{
	LocalsCtx& c = *(LocalsCtx *)UserContext;

	if(pSym->Tag != SymTagData)
		return TRUE;

	LLOG("GLOBAL: " << pSym->Имя << " " << фмт64Гекс(pSym->Address));

	Val& v = c.pdb->global.дайДобавь(pSym->Name);
	v.address = (adr_t)pSym->Address;
	v.reported_size = pSym->Size;
	c.pdb->TypeVal(v, pSym->TypeIndex, (adr_t)pSym->ModBase);
	return TRUE;
}

void Pdb::LoadGlobals(DWORD64 base)
{
	LocalsCtx c;
	c.pdb = this;
	c.context = &context;
	SymEnumSymbols(hProcess, base, NULL, &EnumGlobals, &c);
}

Pdb::Val Pdb::GetGlobal(const Ткст& имя)
{
	return global.дай(имя, Val());
}

Ткст Pdb::GetSymName(adr_t modbase, dword typeindex)
{
    WCHAR *pwszTypeName;
    if(SymGetTypeInfo(hProcess, modbase, typeindex, TI_GET_SYMNAME, &pwszTypeName)) {
		ШТкст w = pwszTypeName;
		LocalFree(pwszTypeName);
		return w.вТкст();
    }
    return Null;
}

dword Pdb::GetSymInfo(adr_t modbase, dword typeindex, IMAGEHLP_SYMBOL_TYPE_INFO info)
{
	dword dw = 0;
	SymGetTypeInfo(hProcess, modbase, typeindex, info, &dw);
	return dw;
}

int Pdb::GetTypeIndex(adr_t modbase, dword typeindex)
{
	int q = тип.найди(typeindex);
	if(q < 0) {
		q = тип.дайСчёт();
		тип.добавь(typeindex).modbase = modbase;
	}
	return q;
}

const Pdb::Type& Pdb::дайТип(int ti)
{
	if(ti < 0 || ti >= тип.дайСчёт())
		выведиОш("Неверный тип");
	Type& t = тип[ti];
	int typeindex = тип.дайКлюч(ti);
	if(t.size < 0) {
		t.имя = GetSymName(t.modbase, typeindex);
		type_name.дайДобавь(t.имя) = ti;
		ULONG64 sz = 0;
		SymGetTypeInfo(hProcess, t.modbase, typeindex, TI_GET_LENGTH, &sz);
		t.size = (dword)sz;
		dword count = GetSymInfo(t.modbase, typeindex, TI_GET_CHILDRENCOUNT);
		if(count) {
			Буфер<byte> b(sizeof(TI_FINDCHILDREN_PARAMS) + sizeof(ULONG) * count);
			TI_FINDCHILDREN_PARAMS *children = (TI_FINDCHILDREN_PARAMS *) ~b;
			children->Count = count;
			children->Start = 0;
			if(SymGetTypeInfo(hProcess, t.modbase, typeindex, TI_FINDCHILDREN, children)) {
				for(dword i = 0; i < count; i++) {
					dword ch = children->ChildId[i];
					dword tag = GetSymInfo(t.modbase, ch, TI_GET_SYMTAG);
					dword kind = GetSymInfo(t.modbase, ch, TI_GET_DATAKIND);
					if(tag == SymTagData) {
						Ткст имя = GetSymName(t.modbase, ch);
						if(kind == DataIsMember) {
							Val& v = t.member.добавь(имя);
							TypeVal(v, GetSymInfo(t.modbase, ch, TI_GET_TYPEID), t.modbase);
							v.address = GetSymInfo(t.modbase, ch, TI_GET_OFFSET);
							ULONG64 bitcnt = 0;
							SymGetTypeInfo(hProcess, t.modbase, ch, TI_GET_LENGTH, &bitcnt);
							if(bitcnt) {
								v.bitcnt = (byte)bitcnt;
								v.bitpos = (byte)GetSymInfo(t.modbase, ch, TI_GET_BITPOSITION);
							}
						}
						if(kind == DataIsStaticMember || kind == DataIsGlobal) {
							Val& v = t.static_member.добавь(имя);
							TypeVal(v, GetSymInfo(t.modbase, ch, TI_GET_TYPEID), t.modbase);
							ULONG64 adr = 0;
							SymGetTypeInfo(hProcess, t.modbase, ch, TI_GET_ADDRESS, &adr);
							v.address = (adr_t)adr;
						}
					}
					else
					if(tag == SymTagBaseClass) {
						Val& v = t.base.добавь();
						TypeVal(v, GetSymInfo(t.modbase, ch, TI_GET_TYPEID), t.modbase);
						v.address = GetSymInfo(t.modbase, ch, TI_GET_OFFSET);
					}
					else
					if(tag == SymTagVTable) {
						t.vtbl_offset = GetSymInfo(t.modbase, ch, TI_GET_OFFSET);
						dword typeId = GetSymInfo(t.modbase, ch, TI_GET_TYPEID);
						while(GetSymInfo(t.modbase, typeId, TI_GET_SYMTAG) == SymTagPointerType)
							typeId = GetSymInfo(t.modbase, typeId, TI_GET_TYPE);
						if((t.vtbl_typeindex = тип.найди(typeId)) < 0) {
							t.vtbl_typeindex = тип.дайСчёт();
							Type& vt = тип.добавь(typeId);
							vt.modbase = t.modbase;
							vt.size = 0;
							vt.vtbl_typeindex = -2;
						}
					}
	            }
		    }
		}
	}
	return t;
}

static int CALLBACK sSymEnum(PSYMBOL_INFO pSym, ULONG SymbolSize, PVOID UserContext)
{
	auto type_index = (ВекторМап<Ткст, int> *)UserContext;
	type_index->дайДобавь(pSym->Name) = pSym->TypeIndex;
	return TRUE;
}

int Pdb::FindType(adr_t modbase, const Ткст& имя)
{
	static ВекторМап<Ткст, int> primitive = {
		{ "bool", BOOL1 },
		{ "char", SINT1 },
		{ "unsigned char", UINT1 },
		{ "short", SINT2 },
		{ "unsigned short", UINT2 },
		{ "wchar_t", UINT2 },
		{ "int", SINT4 },
		{ "unsigned int", UINT4 },
		{ "float", FLT },
		{ "double", DBL },
		{ "int64", SINT8 },
		{ "uint64", UINT8 },
		{ "__int64", SINT8 },
		{ "unsigned __int64", UINT8 },
	};
	
	int q = primitive.дай(имя, Null);
	if(!пусто_ли(q))
		return q;
	q = type_name.дай(имя, Null);
	if(!пусто_ли(q))
		return q;
	if(type_bases.найди(modbase) < 0) {
		type_bases.добавь(modbase);
		SymEnumTypes(hProcess, current_modbase, sSymEnum, &type_index);
		// DDUMPM(type_index);
	}
	int ndx = type_index.дай(имя, Null);
	if(пусто_ли(ndx))
		return Null;
	return GetTypeIndex(modbase, ndx);
}

Ткст Pdb::TypeInfoAsString(TypeInfo tf)
{
	static ВекторМап<int, Ткст> primitive = { // todo: UINT8
		{ BOOL1, "bool" },
		{ SINT1, "char" },
		{ UINT1, "unsigned char" },
		{ SINT2, "short" },
		{ UINT2, "unsigned short" },
		{ SINT4, "int" },
		{ UINT4, "unsigned int"  },
		{ SINT8, "int64" },
		{ UINT8, "uint64"  },
		{ FLT, "float" },
		{ DBL, "double" },
	};
	
	Ткст r = primitive.дай(tf.тип, Null);
	if(пусто_ли(r))
		r = дайТип(tf.тип).имя;

	while(tf.ref > 0) {
		r << "*";
		tf.ref--;
	}
	
	return r;
}

Pdb::TypeInfo Pdb::GetTypeInfo(adr_t modbase, const Ткст& имя)
{
	int q = typeinfo_cache.найди(имя);
	if(q >= 0)
		return typeinfo_cache[q];
	
	TypeInfo r;
	Ткст tp = имя;
	bool spc = false;
	for(;;)
		if(tp.обрежьКонец("*") || tp.обрежьКонец("&"))
			r.ref++;
		else
		if(!tp.обрежьКонец(" ") && !tp.обрежьКонец("const"))
			break;
	r.тип = FindType(modbase, обрежьОба(tp));
	typeinfo_cache.добавь(имя, r);
	return r;
}

#ifdef _ОТЛАДКА

Ткст Pdb::TypeAsString(int ti, bool deep)
{
	Ткст r;
	#define sTYPE(x)     case x: return #x;
	switch(ti) {
	sTYPE(BOOL1)
	sTYPE(UINT1)
	sTYPE(SINT1)
	sTYPE(UINT2)
	sTYPE(SINT2)
	sTYPE(UINT4)
	sTYPE(SINT4)
	sTYPE(UINT8)
	sTYPE(SINT8)
	sTYPE(FLT)
	sTYPE(DBL)
	sTYPE(UNKNOWN)
	}
	if(ti < 0)
		return r;
	const Type& t = дайТип(ti);
	r << t.имя << "(sizeof = " << t.size << ") ";
	if(!deep)
		return r;
	if(t.member.дайСчёт()) {
		r << "{ ";
		for(int i = 0; i < t.member.дайСчёт(); i++) {
			if(i)
				r << ", ";
			r << t.member.дайКлюч(i) << " +" << t.member[i].address;
			if(t.member[i].ref)
				r << TypeAsString(t.member[i].тип, false) << Ткст('*', t.member[i].ref);
			else
				r << ": " << TypeAsString(t.member[i].тип);
		}
		r << " }";
	}
	if(t.base.дайСчёт()) {
		r << " BASE: ";
		for(int i = 0; i < t.base.дайСчёт(); i++) {
			if(i)
				r << ", ";
			r << " +" << t.base[i].address;
			r << TypeAsString(t.base[i].тип);
		}
	}
	return r;
}

#endif

#endif
