//#BLITZ_APPROVE

#define DLISTR__(s) #s
#define DLISTR_(s)  DLISTR__(s)
#define ТКТДЛИ(s)   DLISTR_(s)

#define FN(retval, фн, арги)           FN_CN(retval, DLLCALL, фн, арги, ТКТДЛИ(фн))
#define FN_C(retval, call, фн, арги)   FN_CN(retval, call, фн, арги, ТКТДЛИ(фн))
#define FN_N(retval, фн, арги, имя)   FN_CN(retval, DLLCALL, фн, арги, имя)

#define FN0(retval, фн, арги)          FN0_CN(retval, DLLCALL, фн, арги, ТКТДЛИ(фн))
#define FN0_C(retval, call, фн, арги)  FN0_CN(retval, call, фн, арги, ТКТДЛИ(фн))
#define FN0_N(retval, фн, арги, имя)  FN0_CN(retval, DLLCALL, фн, арги, имя)

#ifndef ТИПДЛЛ
#define ТИПДЛЛ   КОМБИНИРУЙ(T_, МОДУЛЬДЛИ)
#endif

#ifndef DLLCALL
#ifdef PLATFORM_WIN32
#define DLLCALL //__cdecl
#else
#define DLLCALL
#endif
#endif

#ifndef ДЛЛСТРУКТ
#define ДЛЛСТРУКТ МОДУЛЬДЛИ
#endif

// ---------

#if !defined(DLI_HEADER) && !defined(DLI_SOURCE)
#define DLI_HEADER
#define DLI_SOURCE
#endif

#ifdef DLI_HEADER
#undef DLI_HEADER

#define FN_CN(retval, call, фн, арги, имя)\
	typedef retval (call фн##_type) арги; фн##_type *фн;
#define FN0_CN FN_CN

#define DATA(тип, имя) тип *имя;

struct ТИПДЛЛ{
	ТИПДЛЛ();

	Ткст       дайИмяБиб() const        { return libname; }
	проц              устИмяБиб(кткст0 n) { if(handle) освободи(); libname = n; checked = false; }

	бул              грузи();
	бул              грузи(кткст0 n)       { устИмяБиб(n); return грузи(); }
	проц              усиль();
	проц              освободи();

	operator          бул ()      { return handle; }

#include DLIHEADER
private:
	Ткст       libname;
	УКДЛЛ         handle;
	бул              checked;
};

#undef FN_CN
#undef FN0_CN
#undef DATA

// ---------

ТИПДЛЛ& МОДУЛЬДЛИ();
ТИПДЛЛ& КОМБИНИРУЙ(МОДУЛЬДЛИ, _)();

#endif

#ifdef DLI_SOURCE
#undef DLI_SOURCE

ТИПДЛЛ& КОМБИНИРУЙ(МОДУЛЬДЛИ, _)()
{
	static ТИПДЛЛ out;
	return out;
}

ТИПДЛЛ& МОДУЛЬДЛИ()
{
	ТИПДЛЛ& out = КОМБИНИРУЙ(МОДУЛЬДЛИ, _)();
	out.грузи();
	return out;
}

ТИПДЛЛ::ТИПДЛЛ()
{
	checked = false;
	handle = 0;
	libname =
#ifdef ФИМЯДЛЛ
		ФИМЯДЛЛ;
#else
		ТКТДЛИ(ДЛЛСТРУКТ);
#endif
}

бул ТИПДЛЛ::грузи()
{
	if(!checked)
	{
		checked = true;

#define FN_CN(retval, call, фн, арги, имя)  имя,
#define FN0_CN(retval, call, фн, арги, имя) "?" имя,
#define DATA(тип, имя) #имя,

		кткст0 имя[] =
		{
		#include DLIHEADER
			0
		};

#undef FN_CN
#undef FN0_CN
#undef DATA

// --- FnPtr map


#define FN_CN(retval, call, фн, арги, имя)  &фн,
#define FN0_CN FN_CN
#define DATA(тип, имя) &имя,

		ук proc[] =
		{
		#include DLIHEADER
			0
		};

#undef FN_CN
#undef FN0_CN
#undef DATA

		// --- Proc pointers


//		if(handle = LoadDll(file, имя, proc))
//			atexit(&освободи);
		handle = грузиДлл__(libname, имя, proc);
	}
	return handle;
}

проц ТИПДЛЛ::усиль()
{
	if(!ТИПДЛЛ::грузи()) {
//		RLOG("Failed to load required dynamic library '" << LibName() << "'");
		exit(1);
	}
}

проц ТИПДЛЛ::освободи() {
	if(handle) {
		освободиДлл__(handle);
		handle = 0;
	}
	checked = false;
}
#endif

#undef FN
#undef FN_C
#undef FN_N

#undef FN0
#undef FN0_C
#undef FN0_N

#undef DLISTR__
#undef DLISTR_
#undef ТКТДЛИ

#ifdef ФИМЯДЛЛ
#undef ФИМЯДЛЛ
#endif

#ifdef DLLAUTOLOAD
#undef DLLAUTOLOAD
#endif

#undef ТИПДЛЛ
#undef МОДУЛЬДЛИ
#undef DLIHEADER
#undef DLLCALL

#ifdef ФИМЯДЛЛ
#undef ФИМЯДЛЛ
#endif

#ifdef DLLAUTOLOAD
#undef DLLAUTOLOAD
#endif
