#ifndef _DinrusLLVM_Import_h_
#define _DinrusLLVM_Import_h_

#ifdef __cplusplus
extern "C" {
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>

#if defined(_WIN64)
typedef signed __int64 ssize_t;
typedef unsigned __int64 бдол;
#else
typedef signed int ssize_t;
#endif

#define ЛЛИМПОРТ __declspec(dllimport)

	typedef void проц;
	typedef void* ук;

	typedef bool бул;

	typedef  signed char байт;   ///int8_t
	typedef  unsigned char ббайт;  ///uint8_t

	typedef  short крат;  ///int16_t
	typedef  unsigned short бкрат; ///uint16_t

	typedef  int цел;  ///uint16_t
	typedef  unsigned int бцел; ///uint32_t

	typedef  long long дол;   ///int64_t
	typedef  unsigned long long бдол;  ///uint64_t

	typedef  size_t т_мера;

	typedef const char* ткст0;

    typedef цел ЛЛБул;
    typedef struct LLVMOpaqueMemoryBuffer* ЛЛБуферПамяти;
    typedef struct LLVMOpaqueContext* ЛЛКонтекст;
    typedef struct LLVMOpaqueModule* ЛЛМодуль;
    typedef struct LLVMOpaqueType* ЛЛТип;
    typedef struct LLVMOpaqueValue* ЛЛЗначение;
    typedef struct LLVMOpaqueBasicBlock* ЛЛБазовыйБлок;
    typedef struct LLVMOpaqueMetadata* ЛЛМетаданные;
    typedef struct LLVMOpaqueNamedMDNode* ЛЛИменованыйУзелМД;
    typedef struct LLVMOpaqueValueMetadataEntry ЛЛЗаписьМетаданныхЗначения;
    typedef struct LLVMOpaqueBuilder* ЛЛПостроитель;
    typedef struct LLVMOpaqueDIBuilder* ЛЛПостроительОИ;
    typedef struct LLVMOpaqueModuleProvider* ЛЛМодульПровайдер;
    typedef struct LLVMOpaquePassManager* ЛЛМенеджерПроходок;
    typedef struct LLVMOpaquePassRegistry* ЛЛРеестрПроходок;
    typedef struct LLVMOpaqueUse* ЛЛИспользование;
    typedef struct LLVMOpaqueAttributeRef* ЛЛАтрибут;
    typedef struct LLVMOpaqueDiagnosticInfo* ЛЛИнфоДиагностики;
    typedef struct LLVMComdat* ЛЛКомдат;
    typedef struct LLVMOpaqueModuleFlagEntry ЛЛЗаписьФлагаМодуля;
    typedef struct LLVMOpaqueJITEventListener* ЛЛДатчикСобытийДжит;
    typedef struct LLVMOpaqueBinary* ЛЛБинарник;

    typedef struct LLVMOpaqueError* ЛЛОшибка;
    typedef const void* ЛЛИдТипаОшибки;

    typedef struct LLVMOpaqueTargetData* ЛЛДанныеОЦели;
    typedef struct LLVMOpaqueTargetLibraryInfo* ЛЛИнфоЦелевойБиблиотеки;
    typedef void* ЛЛКонтекстДизасма;

    typedef struct LLVMOpaqueGenericValue* ЛЛГенерноеЗначение;
    typedef struct LLVMOpaqueExecutionEngine* ЛЛДвижокВыполнения;
    typedef struct LLVMOpaqueMCJITMemoryManager* ЛЛМенеджерПамятиМЦДжИТ;
    typedef struct LLVMMCJITCompilerOptions* ЛЛОпцииКомпиляцииМЦДжИТ;

    typedef struct LLVMOpaqueSectionIterator *ЛЛИтераторСекций;
    typedef struct LLVMOpaqueSymbolIterator *ЛЛСимвИтератор;
    typedef struct LLVMOpaqueRelocationIterator *ЛЛИтераторРелокаций;

    typedef struct LLVMRemarkOpaqueParser *ЛЛПарсерРемарок;
    typedef struct LLVMRemarkOpaqueEntry *ЛЛЗаписьРемарки;
    typedef struct LLVMRemarkOpaqueArg *ЛЛАргРемарки;

    typedef struct LLVMOpaquePassManagerBuilder *ЛЛПостроительМенеджеровПроходок;

    typedef struct LLVMOpaqueTargetMachine *ЛЛЦелеваяМашина;
    typedef struct LLVMTarget *ЛЛЦель;

	typedef struct LLVMRemarkOpaqueDebugLoc *ЛЛОтладЛокРемарки;

	typedef struct LLVMOpaqueObjectFile *ЛЛФайлОбъекта;

	typedef struct LLVMOrcOpaqueJITStack *LLVMOrcJITStackRef;
	typedef бдол LLVMOrcModuleHandle;
	typedef бдол LLVMOrcTargetAddress;
	typedef бдол (*LLVMOrcSymbolResolverFn)(const char *Name, void *LookupCtx);
	typedef бдол (*LLVMOrcLazyCompileCallbackFn)(LLVMOrcJITStackRef JITStack,
													 void *CallbackCtx);

	typedef struct LLVMOptRemarkOpaqueParser *ЛЛПарсерОптРемарок;
	typedef struct LLVMRemarkOpaqueString *ЛЛТкстРемарки;
	
	enum ППорядокБайт { БигЭнд, ЛитлЭнд };

	typedef struct LLVMOpaqueTargetData *ЛЛДанныеОЦели;
	typedef struct LLVMOpaqueTargetLibraryInfotData *ЛЛИнфоОБиблиотекеЦели;
	
	typedef struct {
  const char *Buffer;
  size_t Size;
} ЛЛБуферОбъектаОВК;

////////////////////////////
//Функции обратного вызова//////
///////////////////////////////////

	// LLVMFatalErrorHandler
    typedef void (*ЛЛОбработчикФатальнойОшибки)(ткст0 причина);

    typedef void (*ЛЛОбработчикДиагностики)(ЛЛИнфоДиагностики, ук);

    typedef void (*ЛЛОбрвызовЖни)(ЛЛКонтекст, ук);

    typedef int (*ЛЛОбрвызОпИнфо)(ук инфОДиз, бдол ПК,
        бдол смещ, бдол разм, цел типТэга, ук буфТэгов);

    typedef ткст0 (*ЛЛОбрвызПоискСимвола)(ук инфОДиз, бдол значСсыл,
        бдол* типСсыл, бдол сссылПК, ткст0* имяСсыл);

    //LLVMMemoryManagerAllocateCodeSectionCallback
    typedef ббайт* (*ЛЛОбрвызМенеджерПамРазместиСекциюКода)(
        ук опак, uintptr_t разм, бцел расклад, бцел идСекц,
        ткст0 имяСекц);

    //LLVMMemoryManagerAllocateDataSectionCallback
    typedef ббайт* (*ЛЛОбрвызМенеджерПамРазместиСекциюДанных)(
        ук опак, uintptr_t разм, бцел расклад, бцел идСекц,
        ткст0 имяСекц, ЛЛБул толькоЧтен_ли);

    //LLVMMemoryManagerFinalizeMemoryCallback
    typedef ЛЛБул(*ЛЛОбрвызМенеджерПамФинализуйПам)(ук опак, char** ошСооб);

    //LLVMMemoryManagerDestroyCallback
    typedef проц(*ЛЛОбрвызМенеджерПамРазрушь)(ук опак);
    
    class PassInfo;
    class StringRef;

extern "C" {
////////////////////////////////////////////////////////
//Инициализация
//////////////////////////////////////////////////////////
	ЛЛИМПОРТ проц ЛЛИницЯдро(ЛЛРеестрПроходок р);
	ЛЛИМПОРТ проц ЛЛИницТрансформУтил(ЛЛРеестрПроходок р);
	ЛЛИМПОРТ проц ЛЛИницСкалярОпц(ЛЛРеестрПроходок р);
	ЛЛИМПОРТ проц ЛЛИницОпцОбджСиАРЦ(ЛЛРеестрПроходок р);
	ЛЛИМПОРТ проц ЛЛИницВекторизацию(ЛЛРеестрПроходок р);
	ЛЛИМПОРТ проц ЛЛИНицИнстКомбин(ЛЛРеестрПроходок р);
	ЛЛИМПОРТ проц ЛЛИницАгрессивнИнстКомбайнер(ЛЛРеестрПроходок р);
	ЛЛИМПОРТ проц ЛЛИницМПО(ЛЛРеестрПроходок р);
	ЛЛИМПОРТ проц ЛЛИницИнструментацию(ЛЛРеестрПроходок р);
	ЛЛИМПОРТ проц ЛЛИницАнализ(ЛЛРеестрПроходок р);
	ЛЛИМПОРТ проц ЛЛИницМПА(ЛЛРеестрПроходок р);
	ЛЛИМПОРТ проц ЛЛИницКодГен(ЛЛРеестрПроходок р);
	ЛЛИМПОРТ проц ЛЛИницЦель(ЛЛРеестрПроходок р);
/////////////////////////////////////////////////////////////
//Анализ
////////////////////////////////////////////////////////////
typedef enum {
  ПрервиПроцесс, /* верификатор выведет в стдош и abort() */
  ВыведиСооб, /* верификатор выведет в стдош и вернёт 1 */
  ВерниСтатус  /* верификатор просто вернёт 1 */
} ПДействиеПриОш;

    ЛЛИМПОРТ  проц ЛЛИНицАнализ(ЛЛРеестрПроходок р) ;
    ЛЛИМПОРТ  ЛЛБул ЛЛВерифицируйМодуль(ЛЛМодуль м, ПДействиеПриОш действие,
        ткст0 * внМодessages);
    ЛЛИМПОРТ  ЛЛБул ЛЛВерифицируйФункцию(ЛЛЗначение фн, ПДействиеПриОш действие) ;
    ЛЛИМПОРТ  проц ЛЛПокажиКФГФункции(ЛЛЗначение фн) ;
    ЛЛИМПОРТ  проц ЛЛПокажиТолькоКФГФункции(ЛЛЗначение фн) ;
/////////////////////////////////////////////////////////////////
//Бит-Считыватель
//////////////////////////////////////////////////////////////////
    ЛЛИМПОРТ ЛЛБул ЛЛРазбериБиткод(ЛЛБуферПамяти буфПам, ЛЛМодуль* внешнМодуль,
        ткст0 * выхСооб) ;
    ЛЛИМПОРТ ЛЛБул ЛЛРазбериБиткод2(ЛЛБуферПамяти буфПам,
        ЛЛМодуль* внешнМодуль);
    ЛЛИМПОРТ ЛЛБул ЛЛРазбериБиткодВКонтексте(ЛЛКонтекст сКонтекст,
        ЛЛБуферПамяти буфПам, ЛЛМодуль* внешнМодуль,  ткст0 * выхСооб) ;
    ЛЛИМПОРТ ЛЛБул ЛЛРазбериБиткодВКонтексте2(ЛЛКонтекст сКонтекст,
        ЛЛБуферПамяти буфПам, ЛЛМодуль* внешнМодуль) ;
    ЛЛИМПОРТ ЛЛБул ЛЛДайБиткодМодульВКонтексте(ЛЛКонтекст сКонтекст,
        ЛЛБуферПамяти буфПам,
        ЛЛМодуль* внМод, ткст0 * выхСооб) ;
    ЛЛИМПОРТ ЛЛБул ЛЛДайБиткодМодульВКонтексте2(ЛЛКонтекст сКонтекст,
        ЛЛБуферПамяти буфПам,
        ЛЛМодуль* внМод);
    ЛЛИМПОРТ ЛЛБул ЛЛДайБиткодМодуль(ЛЛБуферПамяти буфПам, ЛЛМодуль* внМод,
        ткст0 * выхСооб) ;
    ЛЛИМПОРТ ЛЛБул ЛЛДайБиткодМодуль2(ЛЛБуферПамяти буфПам,
        ЛЛМодуль* внМод);
///////////////////////////////////////////////////////////////////
//Бит-Записыватель
//////////////////////////////////////////////////////////////////
     ЛЛИМПОРТ цел ЛЛПишиБиткодВФайл(ЛЛМодуль м, const ткст0 путь);
    ЛЛИМПОРТ цел ЛЛПишиБиткодВФД(ЛЛМодуль м, цел FD, цел закрыть_ли,
        цел небуферирвн) ;
    ЛЛИМПОРТ цел ЛЛПишиБиткодВФайлУк(ЛЛМодуль м, цел файлУк) ;
    ЛЛИМПОРТ ЛЛБуферПамяти ЛЛПишиБиткодВБуфПамяти(ЛЛМодуль м) ;
////////////////////////////////////////////////////////////////////
//Комдат
///////////////////////////////////////////////////////////////////
typedef enum {
  ЛюбойКомдат,        ///< Компонивщик может выбрать любой COMDAT.
  ТочноСовпадающийКомдат, ///< Данные, на которые указывает COMDAT,
                                     ///< должны быть одинаковыми.
  НаибольшийКомдат,    ///< Компоновщик выберет наибольший
                                     ///< COMDAT.
  КомдатБезДубликатов, ///< Никакой другой Модуль не может определять
                                       ///< этот COMDAT.
  КомдатТакогоЖеРазмера ///< Данные, на которые указывает COMDAT, должны быть
                                  ///< одинакового размера.
} ПВыборКомдата;

    ЛЛИМПОРТ ЛЛКомдат ЛЛДайИлиВставьКомдат(ЛЛМодуль м, const ткст0 имя);
    ЛЛИМПОРТ ЛЛКомдат ЛЛДайКомдат(ЛЛЗначение зн);
    ЛЛИМПОРТ проц ЛЛУстКомдат(ЛЛЗначение зн, ЛЛКомдат к);
    ЛЛИМПОРТ ПВыборКомдата ЛЛДайТипВыбораКомдат(ЛЛКомдат к);
    ЛЛИМПОРТ проц ЛЛУстТипВыбораКомдат(ЛЛКомдат к, ПВыборКомдата род) ;
///////////////////////////////////////////////////////////////////////////////
//------------------------------Ядро---------------------------------/////////
//////////////////////////////////////////////////////////////////////////////
typedef enum {
  /* Terminator Instructions */
  LLVMRet            = 1,
  LLVMBr             = 2,
  LLVMSwitch         = 3,
  LLVMIndirectBr     = 4,
  LLVMInvoke         = 5,
  /* removed 6 due to API changes */
  LLVMUnreachable    = 7,
  LLVMCallBr         = 67,

  /* Standard Unary Operators */
  LLVMFNeg           = 66,

  /* Standard Binary Operators */
  LLVMAdd            = 8,
  LLVMFAdd           = 9,
  LLVMSub            = 10,
  LLVMFSub           = 11,
  LLVMMul            = 12,
  LLVMFMul           = 13,
  LLVMUDiv           = 14,
  LLVMSDiv           = 15,
  LLVMFDiv           = 16,
  LLVMURem           = 17,
  LLVMSRem           = 18,
  LLVMFRem           = 19,

  /* Logical Operators */
  LLVMShl            = 20,
  LLVMLShr           = 21,
  LLVMAShr           = 22,
  LLVMAnd            = 23,
  LLVMOr             = 24,
  LLVMXor            = 25,

  /* Memory Operators */
  LLVMAlloca         = 26,
  LLVMLoad           = 27,
  LLVMStore          = 28,
  LLVMGetElementPtr  = 29,

  /* Cast Operators */
  LLVMTrunc          = 30,
  LLVMZExt           = 31,
  LLVMSExt           = 32,
  LLVMFPToUI         = 33,
  LLVMFPToSI         = 34,
  LLVMUIToFP         = 35,
  LLVMSIToFP         = 36,
  LLVMFPTrunc        = 37,
  LLVMFPExt          = 38,
  LLVMPtrToInt       = 39,
  LLVMIntToPtr       = 40,
  LLVMBitCast        = 41,
  LLVMAddrSpaceCast  = 60,

  /* Other Operators */
  LLVMICmp           = 42,
  LLVMFCmp           = 43,
  LLVMPHI            = 44,
  LLVMCall           = 45,
  LLVMSelect         = 46,
  LLVMUserOp1        = 47,
  LLVMUserOp2        = 48,
  LLVMVAArg          = 49,
  LLVMExtractElement = 50,
  LLVMInsertElement  = 51,
  LLVMShuffleVector  = 52,
  LLVMExtractValue   = 53,
  LLVMInsertValue    = 54,

  /* Atomic operators */
  LLVMFence          = 55,
  LLVMAtomicCmpXchg  = 56,
  LLVMAtomicRMW      = 57,

  /* Exception Handling Operators */
  LLVMResume         = 58,
  LLVMLandingPad     = 59,
  LLVMCleanupRet     = 61,
  LLVMCatchRet       = 62,
  LLVMCatchPad       = 63,
  LLVMCleanupPad     = 64,
  LLVMCatchSwitch    = 65
} ПКодОп;

typedef enum {
  LLVMVoidTypeKind,        /**< type with no size */
  LLVMHalfTypeKind,        /**< 16 bit floating point type */
  LLVMFloatTypeKind,       /**< 32 bit floating point type */
  LLVMDoubleTypeKind,      /**< 64 bit floating point type */
  LLVMX86_FP80TypeKind,    /**< 80 bit floating point type (X87) */
  LLVMFP128TypeKind,       /**< 128 bit floating point type (112-bit mantissa)*/
  LLVMPPC_FP128TypeKind,   /**< 128 bit floating point type (two 64-bits) */
  LLVMLabelTypeKind,       /**< Labels */
  LLVMIntegerTypeKind,     /**< Arbitrary bit width integers */
  LLVMFunctionTypeKind,    /**< Functions */
  LLVMStructTypeKind,      /**< Structures */
  LLVMArrayTypeKind,       /**< Arrays */
  LLVMPointerTypeKind,     /**< Pointers */
  LLVMVectorTypeKind,      /**< SIMD 'packed' format, or other vector type */
  LLVMMetadataTypeKind,    /**< мданн */
  LLVMX86_MMXTypeKind,     /**< X86 MMX */
  LLVMTokenTypeKind        /**< Tokens */
} ПТипаж;

typedef enum {
  LLVMExternalLinkage,    /**< Externally visible function */
  LLVMAvailableExternallyLinkage,
  LLVMLinkOnceAnyLinkage, /**< Keep one copy of function when linking (inline)*/
  LLVMLinkOnceODRLinkage, /**< Same, but only replaced by something
                            equivalent. */
  LLVMLinkOnceODRAutoHideLinkage, /**< Obsolete */
  LLVMWeakAnyLinkage,     /**< Keep one copy of function when linking (weak) */
  LLVMWeakODRLinkage,     /**< Same, but only replaced by something
                            equivalent. */
  LLVMAppendingLinkage,   /**< Special purpose, only applies to global arrays */
  LLVMInternalLinkage,    /**< Rename collisions when linking (static
                               functions) */
  LLVMPrivateLinkage,     /**< Like Internal, but omit from symbol table */
  LLVMDLLImportLinkage,   /**< Obsolete */
  LLVMDLLExportLinkage,   /**< Obsolete */
  LLVMExternalWeakLinkage,/**< ExternalWeak linkage description */
  LLVMGhostLinkage,       /**< Obsolete */
  LLVMCommonLinkage,      /**< Tentative definitions */
  LLVMLinkerPrivateLinkage, /**< Like Private, but linker removes. */
  LLVMLinkerPrivateWeakLinkage /**< Like LinkerPrivate, but is weak. */
} ПЛинкаж;

typedef enum {
  LLVMDefaultVisibility,  /**< The GV is visible */
  LLVMHiddenVisibility,   /**< The GV is hidden */
  LLVMProtectedVisibility /**< The GV is protected */
} ПВидимость;

typedef enum {
  LLVMNoUnnamedAddr,    /**< Address of the GV is significant. */
  LLVMLocalUnnamedAddr, /**< Address of the GV is locally insignificant. */
  LLVMGlobalUnnamedAddr /**< Address of the GV is globally insignificant. */
} ПАдрБезИмени;

typedef enum {
  ДефСохрКласс   = 0,
  ИмпортСохрКласс = 1, /**< Функция будет импортироваться из ДЛЛ. */
  ЭкспортСохрКласс = 2  /**< Функция станет доступной из ДЛЛ. */
} ПКлассСохрДЛЛ;

typedef enum {
  LLVMCCallConv             = 0,
  LLVMFastCallConv          = 8,
  LLVMColdCallConv          = 9,
  LLVMGHCCallConv           = 10,
  LLVMHiPECallConv          = 11,
  LLVMWebKitJSCallConv      = 12,
  LLVMAnyRegCallConv        = 13,
  LLVMPreserveMostCallConv  = 14,
  LLVMPreserveAllCallConv   = 15,
  LLVMSwiftCallConv         = 16,
  LLVMCXXFASTTLSCallConv    = 17,
  LLVMX86StdcallCallConv    = 64,
  LLVMX86FastcallCallConv   = 65,
  LLVMARMAPCSCallConv       = 66,
  LLVMARMAAPCSCallConv      = 67,
  LLVMARMAAPCSVFPCallConv   = 68,
  LLVMMSP430INTRCallConv    = 69,
  LLVMX86ThisCallCallConv   = 70,
  LLVMPTXKernelCallConv     = 71,
  LLVMPTXDeviceCallConv     = 72,
  LLVMSPIRFUNCCallConv      = 75,
  LLVMSPIRKERNELCallConv    = 76,
  LLVMIntelOCLBICallConv    = 77,
  LLVMX8664SysVCallConv     = 78,
  LLVMWin64CallConv         = 79,
  LLVMX86VectorCallCallConv = 80,
  LLVMHHVMCallConv          = 81,
  LLVMHHVMCCallConv         = 82,
  LLVMX86INTRCallConv       = 83,
  LLVMAVRINTRCallConv       = 84,
  LLVMAVRSIGNALCallConv     = 85,
  LLVMAVRBUILTINCallConv    = 86,
  LLVMAMDGPUVSCallConv      = 87,
  LLVMAMDGPUGSCallConv      = 88,
  LLVMAMDGPUPSCallConv      = 89,
  LLVMAMDGPUCSCallConv      = 90,
  LLVMAMDGPUKERNELCallConv  = 91,
  LLVMX86RegCallCallConv    = 92,
  LLVMAMDGPUHSCallConv      = 93,
  LLVMMSP430BUILTINCallConv = 94,
  LLVMAMDGPULSCallConv      = 95,
  LLVMAMDGPUESCallConv      = 96
} ПКонвВызова;

typedef enum {
  LLVMArgumentValueKind,
  LLVMBasicBlockValueKind,
  LLVMMemoryUseValueKind,
  LLVMMemoryDefValueKind,
  LLVMMemoryPhiValueKind,

  LLVMFunctionValueKind,
  LLVMGlobalAliasValueKind,
  LLVMGlobalIFuncValueKind,
  LLVMGlobalVariableValueKind,
  LLVMBlockAddressValueKind,
  LLVMConstantExprValueKind,
  LLVMConstantArrayValueKind,
  LLVMConstantStructValueKind,
  LLVMConstantVectorValueKind,

  LLVMUndefValueValueKind,
  LLVMConstantAggregateZeroValueKind,
  LLVMConstantDataArrayValueKind,
  LLVMConstantDataVectorValueKind,
  LLVMConstantIntValueKind,
  LLVMConstantFPValueKind,
  LLVMConstantPointerNullValueKind,
  LLVMConstantTokenNoneValueKind,

  LLVMMetadataAsValueValueKind,
  LLVMInlineAsmValueKind,

  LLVMInstructionValueKind,
} ПРодЗначения;

typedef enum {
  LLVMIntEQ = 32, /**< equal */
  LLVMIntNE,      /**< not equal */
  LLVMIntUGT,     /**< unsigned greater than */
  LLVMIntUGE,     /**< unsigned greater or equal */
  LLVMIntULT,     /**< unsigned less than */
  LLVMIntULE,     /**< unsigned less or equal */
  LLVMIntSGT,     /**< signed greater than */
  LLVMIntSGE,     /**< signed greater or equal */
  LLVMIntSLT,     /**< signed less than */
  LLVMIntSLE      /**< signed less or equal */
} ПЦелПредикат;

typedef enum {
  LLVMRealPredicateFalse, /**< Always false (always folded) */
  LLVMRealOEQ,            /**< True if ordered and equal */
  LLVMRealOGT,            /**< True if ordered and greater than */
  LLVMRealOGE,            /**< True if ordered and greater than or equal */
  LLVMRealOLT,            /**< True if ordered and less than */
  LLVMRealOLE,            /**< True if ordered and less than or equal */
  LLVMRealONE,            /**< True if ordered and operands are unequal */
  LLVMRealORD,            /**< True if ordered (no nans) */
  LLVMRealUNO,            /**< True if unordered: isnan(X) | isnan(Y) */
  LLVMRealUEQ,            /**< True if unordered or equal */
  LLVMRealUGT,            /**< True if unordered or greater than */
  LLVMRealUGE,            /**< True if unordered, greater than, or equal */
  LLVMRealULT,            /**< True if unordered or less than */
  LLVMRealULE,            /**< True if unordered, less than, or equal */
  LLVMRealUNE,            /**< True if unordered or not equal */
  LLVMRealPredicateTrue   /**< Always true (always folded) */
} ПРеалПредикат;

typedef enum {
  LLVMLandingPadCatch,    /**< A catch clause   */
  LLVMLandingPadFilter    /**< A filter clause  */
} LLVMLandingPadClauseTy;

typedef enum {
  LLVMNotThreadLocal = 0,
  LLVMGeneralDynamicTLSModel,
  LLVMLocalDynamicTLSModel,
  LLVMInitialExecTLSModel,
  LLVMLocalExecTLSModel
} ПРежимНителок;

typedef enum {
  LLVMAtomicOrderingNotAtomic = 0, /**< A load or store which is not atomic */
  LLVMAtomicOrderingUnordered = 1, /**< Lowest level of atomicity, guarantees
                                     somewhat sane results, lock free. */
  LLVMAtomicOrderingMonotonic = 2, /**< guarantees that if you take all the
                                     operations affecting a specific address,
                                     a consistent порядок exists */
  LLVMAtomicOrderingAcquire = 4, /**< Acquire provides a barrier of the sort
                                   necessary to acquire a lock to access other
                                   memory with normal loads and stores. */
  LLVMAtomicOrderingRelease = 5, /**< Release is similar to Acquire, but with
                                   a barrier of the sort necessary to release
                                   a lock. */
  LLVMAtomicOrderingAcquireRelease = 6, /**< provides both an Acquire and a
                                          Release barrier (for fences and
                                          operations which both read and write
                                           memory). */
  LLVMAtomicOrderingSequentiallyConsistent = 7 /**< provides Acquire semantics
                                                 for loads and Release
                                                 semantics for stores.
                                                 Additionally, it guarantees
                                                 that a total порядок exists
                                                 between all
                                                 SequentiallyConsistent
                                                 operations. */
} ПАтомичУпоряд;

typedef enum {
    LLVMAtomicRMWBinOpXchg, /**< Set the new знач and return the one old */
    LLVMAtomicRMWBinOpAdd, /**< Add a знач and return the old one */
    LLVMAtomicRMWBinOpSub, /**< Subtract a знач and return the old one */
    LLVMAtomicRMWBinOpAnd, /**< And a знач and return the old one */
    LLVMAtomicRMWBinOpNand, /**< Not-And a знач and return the old one */
    LLVMAtomicRMWBinOpOr, /**< OR a знач and return the old one */
    LLVMAtomicRMWBinOpXor, /**< Xor a знач and return the old one */
    LLVMAtomicRMWBinOpMax, /**< Sets the знач if it's greater than the
                             original using a signed comparison and return
                             the old one */
    LLVMAtomicRMWBinOpMin, /**< Sets the знач if it's Smaller than the
                             original using a signed comparison and return
                             the old one */
    LLVMAtomicRMWBinOpUMax, /**< Sets the знач if it's greater than the
                             original using an unsigned comparison and return
                             the old one */
    LLVMAtomicRMWBinOpUMin /**< Sets the знач if it's greater than the
                             original using an unsigned comparison  and return
                             the old one */
} LLVMAtomicRMWBinOp;

typedef enum {
    LLVMDSError,
    LLVMDSWarning,
    LLVMDSRemark,
    LLVMDSNote
} ПСтрогостьДиагностики;

typedef enum {
  LLVMInlineAsmDialectATT,
  LLVMInlineAsmDialectIntel
} LLVMInlineAsmDialect;

typedef enum {
  /**
   * Emits an error if two values disagree, otherwise the resulting знач is
   * that of the operands.
   *
   * @see Module::ModFlagBehavior::Error
   */
  LLVMModuleFlagBehaviorError,
  /**
   * Emits a warning if two values disagree. The result знач will be the
   * operand for the flag from the first module being linked.
   *
   * @see Module::ModFlagBehavior::Warning
   */
  LLVMModuleFlagBehaviorWarning,
  /**
   * Adds a requirement that another module flag be present and have a
   * specified знач after linking is performed. The знач must be a metadata
   * pair, where the first element of the pair is the ID of the module flag
   * to be restricted, and the second element of the pair is the знач the
   * module flag should be restricted to. This behavior can be used to
   * restrict the allowable results (via triggering of an error) of linking
   * IDs with the **Override** behavior.
   *
   * @see Module::ModFlagBehavior::Require
   */
  LLVMModuleFlagBehaviorRequire,
  /**
   * Uses the specified знач, regardless of the behavior or знач of the
   * other module. If both modules specify **Override**, but the values
   * differ, an error will be emitted.
   *
   * @see Module::ModFlagBehavior::Override
   */
  LLVMModuleFlagBehaviorOverride,
  /**
   * Appends the two values, which are required to be metadata nodes.
   *
   * @see Module::ModFlagBehavior::Append
   */
  LLVMModuleFlagBehaviorAppend,
  /**
   * Appends the two values, which are required to be metadata
   * nodes. However, duplicate entries in the second list are dropped
   * during the append operation.
   *
   * @see Module::ModFlagBehavior::AppendUnique
   */
  LLVMModuleFlagBehaviorAppendUnique,
} ППоведениеФлагаМодуля;

/**
 * Attribute index are either LLVMAttributeReturnIndex,
 * LLVMAttributeFunctionIndex or a parameter number from 1 to N.
 */
enum {
  LLVMAttributeReturnIndex = 0U,
  // ISO C restricts enumerator values to range of 'int'
  // (4294967295 is too large)
  // LLVMAttributeFunctionIndex = ~0U,
  LLVMAttributeFunctionIndex = -1,
};

typedef unsigned ЛЛИндексАтрибута;
///////////////////////////////////////////////////////////////////////////
    ЛЛИМПОРТ ЛЛЗначение ЛЛАргумент_ли(ЛЛЗначение зн);
    ЛЛИМПОРТ ЛЛЗначение ЛЛБазБлок_ли(ЛЛЗначение зн);
    ЛЛИМПОРТ ЛЛЗначение ЛЛИнлайнАсм_ли(ЛЛЗначение зн);
    ЛЛИМПОРТ ЛЛЗначение ЛЛПользователь_ли(ЛЛЗначение зн);
    ЛЛИМПОРТ ЛЛЗначение ЛЛКонст_ли(ЛЛЗначение зн);
    ЛЛИМПОРТ ЛЛЗначение ЛЛАдресБлока_ли(ЛЛЗначение зн);
    ЛЛИМПОРТ ЛЛЗначение ЛЛКонстАгрегатНоль_ли(ЛЛЗначение зн);
    ЛЛИМПОРТ ЛЛЗначение ЛЛМассивКонстант_ли(ЛЛЗначение зн);
    ЛЛИМПОРТ ЛЛЗначение ЛЛКонстПоследовательностьДанных_ли(ЛЛЗначение зн);
    ЛЛИМПОРТ ЛЛЗначение ЛЛКонстМассивДанных_ли(ЛЛЗначение зн);
    ЛЛИМПОРТ ЛЛЗначение ЛЛКонстВекторДанных_ли(ЛЛЗначение зн);
    ЛЛИМПОРТ ЛЛЗначение ЛЛКонстВыражение_ли(ЛЛЗначение зн);
    ЛЛИМПОРТ ЛЛЗначение ЛЛКонстПЗ_ли(ЛЛЗначение зн);
    ЛЛИМПОРТ ЛЛЗначение ЛЛКонстЦел_ли(ЛЛЗначение зн);
    ЛЛИМПОРТ ЛЛЗначение ЛЛКонстНуллУказатель_ли(ЛЛЗначение зн);
    ЛЛИМПОРТ ЛЛЗначение ЛЛКонстСтрукт_ли(ЛЛЗначение зн);
    ЛЛИМПОРТ ЛЛЗначение ЛЛКонстСемаНичто_ли(ЛЛЗначение зн);
    ЛЛИМПОРТ ЛЛЗначение ЛЛКонстВектор_ли(ЛЛЗначение зн);
    ЛЛИМПОРТ ЛЛЗначение ЛЛГлобЗначение_ли(ЛЛЗначение зн);
    ЛЛИМПОРТ ЛЛЗначение ЛЛГлобНик_ли(ЛЛЗначение зн);
    ЛЛИМПОРТ ЛЛЗначение ЛЛГлобИФункц_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛГлобОбъект_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛФункция_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛГлобПеременная_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛНеопрЗначение_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнструкция_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛБинарныйОператор_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрВызова_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнтринсикИнстр_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнтринсикОтладИнфо_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнтринсикОтладПеременная_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрДекларируйОтлад_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрОтладЯрлык_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛПамИнтринсик_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрКопируйПам_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрПереместиПам_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрУстПам_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрСравни_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрПСравни_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрЦСравни_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрИзвлекиЭлт_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрДайУкНаЭлт_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрВставьЭлемент_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрВставьЗначение_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрЛэндингПад_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛУзелПХИ_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрВыбор_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрШафлВектор_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрХрани_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрВетвь_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрНепрямБр_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрИнвок_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрВозврат_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрЩит_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрНедоступно_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрВозобнови_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрОчистьВозврат_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрЛовиВозврат_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрФунклетПад_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрЛовиПад_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрОчистьПад_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛУнарнаяИнструкция_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрРазместмас_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрКаст_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрАдрПрострКаст_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрБитКаст_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрПЗРасш_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрПЗвЗЦ_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрПЗвБЦ_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрОбрежьПЗ_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрЦелВУк_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрУкВЦел_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрЗРасш_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрЗЦвПЗ_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрОбрежь_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрБЦвПЗ_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрЗэдРасш_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрИзвлекиЗначение_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрГрузи_ли(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрАргВА_ли(ЛЛЗначение зн);
	///////////////////////////////////////////
		ЛЛИМПОРТ проц ЛЛШатдаун(проц);
	// Обработка ошибок
	ЛЛИМПОРТ ткст0 ЛЛCоздайCообщение(const ткст0 сооб);
	ЛЛИМПОРТ проц ЛЛВыместиСообщение(ткст0 сооб);
	// Операции над контекстом
	ЛЛИМПОРТ ЛЛКонтекст ЛЛКонтекстСоздай(проц);
	ЛЛИМПОРТ ЛЛКонтекст ЛЛДайГлобКонтекст(проц);
	ЛЛИМПОРТ проц ЛЛКонтекстУстОбработчикДиагностики(ЛЛКонтекст к,
	                                         ЛЛОбработчикДиагностики обраб,
	                                                         ук диагКонтекст);
	ЛЛИМПОРТ ЛЛОбработчикДиагностики ЛЛКонтекстДайОбработчикДиагностики(ЛЛКонтекст к);
	ЛЛИМПОРТ ук ЛЛКонтекстДайКонтекстДиагностики(ЛЛКонтекст к);
	ЛЛИМПОРТ проц ЛЛКонтекстУстОбрвызовЖни(ЛЛКонтекст к, ЛЛОбрвызовЖни обрвыз,
	                                                               ук опакУкз);
	ЛЛИМПОРТ ЛЛБул ЛЛКонтекстСбрасыватьИменаЗначений_ли(ЛЛКонтекст к);
	ЛЛИМПОРТ проц ЛЛКонтекстУстСбросИмёнЗначений(ЛЛКонтекст к, ЛЛБул Discard);
	ЛЛИМПОРТ проц ЛЛКонтекстВымести(ЛЛКонтекст к);
	ЛЛИМПОРТ бцел ЛЛДайИДТипаМДВКонтексте(ЛЛКонтекст к, const ткст0 имя,
	                                                              бцел длинТ);
	ЛЛИМПОРТ бцел ЛЛДайИДТипаМД(const ткст0 имя, бцел длинТ);
	ЛЛИМПОРТ бцел ЛЛДайТипАтрибутаПеречняДляИмени(const ткст0 имя, т_мера длинТ);
	ЛЛИМПОРТ бцел ЛЛДайТипПоследнАтрибутаПеречня(проц);
	ЛЛИМПОРТ ЛЛАтрибут ЛЛСоздайАтрибутПеречня(ЛЛКонтекст к, бцел идК, бдол знач);
	ЛЛИМПОРТ бцел ЛЛДайТипАтрибутаПеречня(ЛЛАтрибут A);
	ЛЛИМПОРТ бдол ЛЛДайЗначениеАтрибутаПеречня(ЛЛАтрибут A);
	ЛЛИМПОРТ ЛЛАтрибут ЛЛСоздайТкстАтрибут(ЛЛКонтекст к,
	                                           const ткст0 K, бцел длинаК,
	                                           const ткст0 зн, бцел длинаЗ);
	ЛЛИМПОРТ const ткст0 ЛЛДайТипТкстАтрибута(ЛЛАтрибут A, бцел* длина);
	ЛЛИМПОРТ const ткст0 ЛЛДайЗначениеТкстАтрибута(ЛЛАтрибут A,  бцел* длина);
	ЛЛИМПОРТ ЛЛБул ЛЛАтрибутПеречня_ли(ЛЛАтрибут A);
	ЛЛИМПОРТ ЛЛБул ЛЛТкстАтрибут_ли(ЛЛАтрибут A);
	ЛЛИМПОРТ ткст0 ЛЛДайОписаниеДиагИнфо(ЛЛИнфоДиагностики DI);
	ЛЛИМПОРТ ПСтрогостьДиагностики ЛЛДайСтрогостьДиагИнфо(ЛЛИнфоДиагностики DI);
	// Операции над модулями
	ЛЛИМПОРТ ЛЛМодуль ЛЛМодуль_СоздайСИменем(const ткст0 идМодуля) ;
	ЛЛИМПОРТ ЛЛМодуль ЛЛМодуль_СоздайСИменемВКонтексте(const ткст0 идМодуля,
	                                                               ЛЛКонтекст к);
	ЛЛИМПОРТ проц ЛЛВыместиМодуль(ЛЛМодуль м);
	ЛЛИМПОРТ const ткст0 ЛЛДайИдентификаторМодуля(ЛЛМодуль м, т_мера *длин);
	ЛЛИМПОРТ проц ЛЛУстИдентификаторМодуля(ЛЛМодуль м, const ткст0 идент, т_мера длин) ;
	ЛЛИМПОРТ const ткст0 ЛЛДайИмяИсходника(ЛЛМодуль м, т_мера *длин);
	ЛЛИМПОРТ проц ЛЛУстИмяИсходника(ЛЛМодуль м, const ткст0 имя, т_мера длин);
	// Раскладка данных
	ЛЛИМПОРТ const ткст0 ЛЛДайСтрРаскладкиДанных(ЛЛМодуль м);
	ЛЛИМПОРТ const ткст0 ЛЛДайРаскладкуДанных(ЛЛМодуль м);
	ЛЛИМПОРТ проц ЛЛУстРаскладкуДанных(ЛЛМодуль м, const ткст0 DataLayoutStr) ;
	//--Целевая триада
	ЛЛИМПОРТ const ткст0  ЛЛДайЦель(ЛЛМодуль м);
	ЛЛИМПОРТ проц ЛЛУстЦель(ЛЛМодуль м, const ткст0 триада) ;
	// Флаги модуля
	ЛЛИМПОРТ ЛЛЗаписьФлагаМодуля *ЛЛКопируйМетаданныеФлаговМодуля(ЛЛМодуль м, т_мера *длин) ;
	ЛЛИМПОРТ проц ЛЛВыместиМетаданныеФлаговМодуля(ЛЛЗаписьФлагаМодуля* записи);
	ЛЛИМПОРТ ППоведениеФлагаМодуля
	ЛЛЗаписиФлаговМодуля_ДайПоведениеФлага(ЛЛЗаписьФлагаМодуля *записи,
	                                     бцел инд);
	ЛЛИМПОРТ const ткст0 ЛЛЗаписиФлаговМодуля_ДайКлюч(ЛЛЗаписьФлагаМодуля* записи,
	                                                     бцел инд, т_мера* длин);
	ЛЛИМПОРТ ЛЛМетаданные ЛЛЗаписиФлаговМодуля_ДайМетаданные(ЛЛЗаписьФлагаМодуля *записи,
	                                                                   бцел инд);
	ЛЛИМПОРТ ЛЛМетаданные ЛЛДайФлагМодуля(ЛЛМодуль м,
	                                  const ткст0 ключ, т_мера длинКлюча);
	ЛЛИМПОРТ проц ЛЛДобавьФлагМодуля(ЛЛМодуль м, ППоведениеФлагаМодуля поведен,
	                                         const ткст0 ключ, т_мера длинКлюча,
	                                                                ЛЛМетаданные знач);
	// Вывод модулей
	    ЛЛИМПОРТ проц ЛЛДампМодуля(ЛЛМодуль м) ;
	ЛЛИМПОРТ ЛЛБул ЛЛВыведиМодульВФайл(ЛЛМодуль м, const ткст0 имяф,
	                                                          ткст0 *ошсооб);
	ЛЛИМПОРТ ткст0 ЛЛВыведиМодульВСтроку(ЛЛМодуль м);
	// Операции над инлайн-ассемблером
	ЛЛИМПОРТ проц ЛЛУстИнлайнАсмМодуля2(ЛЛМодуль м, const ткст0 Asm, т_мера длин);
	    ЛЛИМПОРТ проц ЛЛУстИнлайнАсмМодуля(ЛЛМодуль м, const ткст0 Asm);
	    ЛЛИМПОРТ проц ЛЛПриставьИнлайнАсмМодуля(ЛЛМодуль м, const ткст0 Asm, т_мера длин) ;
	ЛЛИМПОРТ const ткст0 ЛЛДайИнлайнАсмМодуля(ЛЛМодуль м, т_мера *длин) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайИнлайнАсм(ЛЛТип тип,
	                              ткст0 ткстАсма, т_мера размТкстАсма,
	                              ткст0 констрейнты, т_мера размКонстрейнов,
	                              ЛЛБул естьПобочка, ЛЛБул IsAlignStack,
	                              LLVMInlineAsmDialect диалект);
	// Операции над модульными контекстами
	ЛЛИМПОРТ ЛЛКонтекст ЛЛДайКонтекстМодуля(ЛЛМодуль м);
	// Операции над всеми типами (в основном)
	ЛЛИМПОРТ ПТипаж ЛЛДайРодТипа(ЛЛТип тип) ;
	ЛЛИМПОРТ ЛЛБул ЛЛТипСРазмером_ли(ЛЛТип тип);
	ЛЛИМПОРТ ЛЛКонтекст ЛЛДайКонтекстТипа(ЛЛТип тип);
	ЛЛИМПОРТ проц ЛЛДампТипа(ЛЛТип тип);
	ЛЛИМПОРТ ткст0 ЛЛВыведиТипВСтроку(ЛЛТип тип);
	// Операции над целочисленными типами
	ЛЛИМПОРТ ЛЛТип ЛЛТипЦел1ВКонтексте(ЛЛКонтекст к);
	ЛЛИМПОРТ ЛЛТип ЛЛТипЦел8ВКонтексте(ЛЛКонтекст к);
	ЛЛИМПОРТ ЛЛТип ЛЛТипЦел16ВКонтексте(ЛЛКонтекст к);
	ЛЛИМПОРТ ЛЛТип ЛЛТипЦел32Контексте(ЛЛКонтекст к);
	ЛЛИМПОРТ ЛЛТип ЛЛТипЦел64ВКонтексте(ЛЛКонтекст к);
	ЛЛИМПОРТ ЛЛТип ЛЛТипЦел128ВКонтексте(ЛЛКонтекст к);
	ЛЛИМПОРТ ЛЛТип ЛЛТипЦелВКонтексте(ЛЛКонтекст к, бцел члоБит);
	ЛЛИМПОРТ ЛЛТип ЛЛТипЦел1(проц);
	ЛЛИМПОРТ ЛЛТип ЛЛТипЦел8(проц) ;
	ЛЛИМПОРТ ЛЛТип  ЛЛТипЦел16(проц);
	ЛЛИМПОРТ ЛЛТип  ЛЛТипЦел32(проц);
	ЛЛИМПОРТ ЛЛТип  ЛЛТипЦел64(проц);
	ЛЛИМПОРТ ЛЛТип  ЛЛТипЦел128(проц) ;
	ЛЛИМПОРТ ЛЛТип  ЛЛТипЦел(бцел члоБит);
	ЛЛИМПОРТ бцел ЛЛДайШиринуЦелТипа(ЛЛТип целочисТип);
	// Операции над реальными типам
	ЛЛИМПОРТ ЛЛТип ЛЛПолутипВКонтексте(ЛЛКонтекст к);
	ЛЛИМПОРТ ЛЛТип ЛЛТипПлавВКонтексте(ЛЛКонтекст к);
	ЛЛИМПОРТ ЛЛТип ЛЛТипДвоВКонтексте(ЛЛКонтекст к);
	ЛЛИМПОРТ ЛЛТип ЛЛТипХ86ФП80ВКонтексте(ЛЛКонтекст к);
	ЛЛИМПОРТ ЛЛТип ЛЛТипФП128ВКонтексте(ЛЛКонтекст к);
	ЛЛИМПОРТ ЛЛТип ЛЛТипППЦФП128ВКонтексте(ЛЛКонтекст к) ;
	ЛЛИМПОРТ ЛЛТип ЛЛТипХ86ММХВКонтексте(ЛЛКонтекст к);
	ЛЛИМПОРТ ЛЛТип ЛЛПолутип(проц) ;
	ЛЛИМПОРТ ЛЛТип ЛЛТипПлав(проц) ;
	ЛЛИМПОРТ ЛЛТип ЛЛТипДво(проц);
	ЛЛИМПОРТ ЛЛТип ЛЛТипХ86ФП80(проц) ;
	ЛЛИМПОРТ ЛЛТип ЛЛТипФП128(проц);
	ЛЛИМПОРТ ЛЛТип ЛЛТипППЦФП128(проц);
	ЛЛИМПОРТ ЛЛТип ЛЛТипХ86ММХ(проц);
	// Операциями над типами функций
	ЛЛИМПОРТ ЛЛТип ЛЛТипФункция(ЛЛТип типВозврата,
	                             ЛЛТип *типыПарам, бцел счётПарам,
	                                                  ЛЛБул варарг_ли) ;
	ЛЛИМПОРТ ЛЛБул ЛЛВараргФункц_ли(ЛЛТип типФункц);
	ЛЛИМПОРТ ЛЛТип ЛЛДайТипВозврата(ЛЛТип типФункц);
	ЛЛИМПОРТ бцел ЛЛСчётТиповПарам(ЛЛТип типФункц);
	ЛЛИМПОРТ проц ЛЛДайТипыПарам(ЛЛТип типФункц, ЛЛТип *приёмник);
	// Операции над типами структур
	ЛЛИМПОРТ ЛЛТип ЛЛТипСтруктВКонтексте(ЛЛКонтекст к, ЛЛТип *типыЭлтов,
	                           бцел счётЭлтов, ЛЛБул упакован);
	ЛЛИМПОРТ ЛЛТип ЛЛТипСтрукт(ЛЛТип *типыЭлтов,
	                           бцел счётЭлтов, ЛЛБул упакован);
	ЛЛИМПОРТ ЛЛТип ЛЛСтруктСоздайСИменем(ЛЛКонтекст к, const ткст0 имя);
	ЛЛИМПОРТ const ткст0 ЛЛДайИмяСтрукт(ЛЛТип тип);
	ЛЛИМПОРТ проц ЛЛСтруктУстТело(ЛЛТип типСтрукт, ЛЛТип *типыЭлтов,
	                       бцел счётЭлтов, ЛЛБул упакован);
	ЛЛИМПОРТ бцел ЛЛПосчитайТипыЭлементовСтрукт(ЛЛТип типСтрукт);
	ЛЛИМПОРТ проц ЛЛДайТипыЭлементовСтрукт(ЛЛТип типСтрукт, ЛЛТип *приёмник);
	ЛЛИМПОРТ ЛЛТип ЛЛСтруктДайТипНаИндексе(ЛЛТип типСтрукт, бцел i) ;
	ЛЛИМПОРТ ЛЛБул ЛЛУпакованнаяСтруктура_ли(ЛЛТип типСтрукт) ;
	ЛЛИМПОРТ ЛЛБул ЛЛОпакСтрукт_ли(ЛЛТип типСтрукт) ;
	ЛЛИМПОРТ ЛЛБул ЛЛЛитералСтрукт_ли(ЛЛТип типСтрукт);
	ЛЛИМПОРТ ЛЛТип ЛЛДайТипПоИмени(ЛЛМодуль м, const ткст0 имя);
	// Операции над типами массивов, указателей и векторов (типами последовательностей)
	ЛЛИМПОРТ проц ЛЛДайПодтипы(ЛЛТип Tp, ЛЛТип *Arr);
	ЛЛИМПОРТ ЛЛТип ЛЛТипМассив(ЛЛТип типЭлт, бцел счётЭлтов);
	ЛЛИМПОРТ ЛЛТип ЛЛТипУказатель(ЛЛТип типЭлт, бцел адрПрострв);
	ЛЛИМПОРТ ЛЛТип ЛЛТипВектор(ЛЛТип типЭлт, бцел счётЭлтов);
	ЛЛИМПОРТ ЛЛТип ЛЛДайТипЭлемента(ЛЛТип WrappedTy) ;
	ЛЛИМПОРТ бцел ЛЛДайЧлоКонтТипов(ЛЛТип Tp);
	ЛЛИМПОРТ бцел ЛЛДайДлинуМассива(ЛЛТип типМассив);
	ЛЛИМПОРТ бцел ЛЛДАйАдрПрострУказателя(ЛЛТип типУказатель) ;
	ЛЛИМПОРТ бцел ЛЛДайРазмерВектора(ЛЛТип типВектор);
		// Операции над прочими типами
	ЛЛИМПОРТ ЛЛТип ЛЛТипПроцВКонтексте(ЛЛКонтекст к) ;
	ЛЛИМПОРТ ЛЛТип ЛЛТипЯрлыкВКонтексте(ЛЛКонтекст к);
	ЛЛИМПОРТ ЛЛТип ЛЛТипСемаВКонтексте(ЛЛКонтекст к);
	ЛЛИМПОРТ ЛЛТип ЛЛТипМетаданныеВКонтексте(ЛЛКонтекст к) ;
	ЛЛИМПОРТ ЛЛТип ЛЛТипПроц(проц) ;
	ЛЛИМПОРТ ЛЛТип ЛЛТипЯрлык(проц);
	// Операции над значениями
	// Операции над всеми значениями
	ЛЛИМПОРТ ЛЛТип ЛЛТипУ(ЛЛЗначение знач);
	ЛЛИМПОРТ ПРодЗначения ЛЛДайРодЗначения(ЛЛЗначение знач) ;
	ЛЛИМПОРТ const ткст0 ЛЛДайИмяЗначения2(ЛЛЗначение знач, т_мера *длина) ;
	ЛЛИМПОРТ проц ЛЛУстИмяЗначения2(ЛЛЗначение знач, const ткст0 имя, т_мера длинаИмени) ;
	ЛЛИМПОРТ const ткст0 ЛЛДайИмяЗначения(ЛЛЗначение знач);
	ЛЛИМПОРТ проц ЛЛУстИмяЗначения(ЛЛЗначение знач, const ткст0 имя);
	ЛЛИМПОРТ проц ЛЛЗначениеДампа(ЛЛЗначение знач);
	ЛЛИМПОРТ ткст0 ЛЛВыведиЗначениеВСтроку(ЛЛЗначение знач);
	ЛЛИМПОРТ проц ЛЛЗамениВсеИспользованияНа(ЛЛЗначение старЗнач, ЛЛЗначение новЗнач);
	ЛЛИМПОРТ цел ЛЛЕстьМетаданные_ли(ЛЛЗначение инст);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайМетаданные(ЛЛЗначение инст, бцел идК);
	ЛЛИМПОРТ проц ЛЛУстМетаданные(ЛЛЗначение инст, бцел идК, ЛЛЗначение знач);
	ЛЛИМПОРТ ЛЛЗаписьМетаданныхЗначения *
	ЛЛИнструкцияДайВсеМетаданныеКромеЛокОтлад(ЛЛЗначение знач, т_мера *чисЭлты);
	// Функции преобразования
	ЛЛИМПОРТ ЛЛЗначение ЛЛМДУзел_ли(ЛЛЗначение знач);
	ЛЛИМПОРТ ЛЛЗначение ЛЛМДТкст_ли(ЛЛЗначение знач) ;
	// Операции над использованиями
	ЛЛИМПОРТ ЛЛИспользование ЛЛДайПервоеИспользование(ЛЛЗначение знач) ;
	ЛЛИМПОРТ ЛЛИспользование ЛЛДайСледщИспользование(ЛЛИспользование U) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайПользователя(ЛЛИспользование U) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайИспользованноеЗначение(ЛЛИспользование U);
	// Операции над Пользователями
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайОперанд(ЛЛЗначение знач, бцел инд);
	ЛЛИМПОРТ ЛЛИспользование ЛЛДайИспользованиеОперанда(ЛЛЗначение знач, бцел инд);
	ЛЛИМПОРТ проц ЛЛУстОперанд(ЛЛЗначение знач, бцел инд, ЛЛЗначение Op);
	ЛЛИМПОРТ бцел ЛЛДайЧлоОперандовМДУзла(ЛЛЗначение зн) ;
	ЛЛИМПОРТ цел ЛЛДайЧлоОперандов(ЛЛЗначение знач);
	// Операции над константами любого типа
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстПусто(ЛЛТип тип);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстВсеЕд(ЛЛТип тип) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайНеопр(ЛЛТип тип) ;
	ЛЛИМПОРТ ЛЛБул ЛЛКонстанта_ли(ЛЛЗначение тип);
	ЛЛИМПОРТ ЛЛБул ЛЛПусто_ли(ЛЛЗначение знач);
	ЛЛИМПОРТ ЛЛБул ЛЛНеопр_ли(ЛЛЗначение знач);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстУкПусто(ЛЛТип тип);
	// Операции над узлами метаданных
	ЛЛИМПОРТ ЛЛМетаданные ЛЛМДТкстВКонтексте2(ЛЛКонтекст к, const ткст0 стр,
	                                                                 т_мера длинТ);
	ЛЛИМПОРТ ЛЛМетаданные ЛЛМДУзелВКонтексте2(ЛЛКонтекст к, ЛЛМетаданные *MDs,
	                                                                  т_мера счёт);
	ЛЛИМПОРТ ЛЛЗначение ЛЛМДТкстВКонтексте(ЛЛКонтекст к, const ткст0 стр,
	                                                             бцел длинТ);
	ЛЛИМПОРТ ЛЛЗначение ЛЛМДТкст(const ткст0 стр, бцел длинТ);
	ЛЛИМПОРТ ЛЛЗначение ЛЛМДУзелВКонтексте(ЛЛКонтекст к, ЛЛЗначение *значя,
	                                                              бцел счёт);
	ЛЛИМПОРТ ЛЛЗначение ЛЛМДУзел(ЛЛЗначение *значя, бцел счёт);
	ЛЛИМПОРТ ЛЛЗначение ЛЛМетаданныеКакЗначение(ЛЛКонтекст к, ЛЛМетаданные MD);
	ЛЛИМПОРТ ЛЛМетаданные ЛЛЗначениеКакМетаданные(ЛЛЗначение знач) ;
	ЛЛИМПОРТ const ткст0 ЛЛДайМДТкст(ЛЛЗначение зн, бцел *длина);
	ЛЛИМПОРТ ЛЛИменованыйУзелМД ЛЛДайПервыеИменованныеМетаданные(ЛЛМодуль м);
	ЛЛИМПОРТ ЛЛИменованыйУзелМД ЛЛДайПоследниеИменованныеМетаданные(ЛЛМодуль м);
	ЛЛИМПОРТ ЛЛИменованыйУзелМД ЛЛДайСледщИменованныеМетаданные(ЛЛИменованыйУзелМД NMD);
	ЛЛИМПОРТ ЛЛИменованыйУзелМД ЛЛДайПредшИменованныеМетаданные(ЛЛИменованыйУзелМД NMD);
	ЛЛИМПОРТ ЛЛИменованыйУзелМД ЛЛДайИменованныеМетаданные(ЛЛМодуль м,
	                                            const ткст0 имя, т_мера длинаИмени) ;
	ЛЛИМПОРТ ЛЛИменованыйУзелМД ЛЛДайИлиВставьИменованныеМетаданные(ЛЛМодуль м,
	                                                const ткст0 имя, т_мера длинаИмени);
	ЛЛИМПОРТ const ткст0 ЛЛДайИмяИменованныхМетаданных(ЛЛИменованыйУзелМД NMD,
	                                                                  т_мера *длинаИмени);
	ЛЛИМПОРТ проц ЛЛДайОперандыМДУзла(ЛЛЗначение зн, ЛЛЗначение *приёмник) ;
	ЛЛИМПОРТ бцел ЛЛДайЧлоОперандовИменованныхМетаданных(ЛЛМодуль м, const ткст0 имя);
	ЛЛИМПОРТ проц ЛЛДайОперандыИменованныхМетаданных(ЛЛМодуль м, const ткст0 имя,
	                                                               ЛЛЗначение *приёмник);
	ЛЛИМПОРТ проц ЛЛДобавьОперандИменованныхМетаданных(ЛЛМодуль м, const ткст0 имя,
	                                                                   ЛЛЗначение знач) ;
	ЛЛИМПОРТ const ткст0 ЛЛДайОтладЛокПапку(ЛЛЗначение знач, бцел *длина);
	ЛЛИМПОРТ const ткст0 ЛЛДайОтладЛокИмяф(ЛЛЗначение знач, бцел *длина);
	ЛЛИМПОРТ бцел ЛЛДайОтладЛокСтроку(ЛЛЗначение знач);
	ЛЛИМПОРТ бцел ЛЛДайОтладЛокКолонку(ЛЛЗначение знач);
	//Операции над скалярными константами
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстЦел(ЛЛТип типЦел, бдол N,
	                                                    ЛЛБул SignExtend) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстЦелПроизвольнойТочности(ЛЛТип типЦел,
	                                                      бцел NumWords,
	                                                          const бдол слова[]);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстЦелИзТкста(ЛЛТип типЦел, const char стр[],
	                                                          ббайт корень);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстЦелИзТкстаСРазмером(ЛЛТип типЦел, const char стр[],
	                                                     бцел длинТ, ббайт корень);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстРеал(ЛЛТип типРеал, double N);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстРеалИзТкста(ЛЛТип типРеал, const ткст0 Text);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстРеалИзТкстаСРазмером(ЛЛТип типРеал, const char стр[],
	                                                                   бцел длинТ);
	ЛЛИМПОРТ бдол ЛЛКонстЦелДайНРасшЗначение(ЛЛЗначение констЗнач);
	ЛЛИМПОРТ дол ЛЛКонстЦелДайЗРасшЗначение(ЛЛЗначение констЗнач);
	ЛЛИМПОРТ double ЛЛКонстРеалДайДво(ЛЛЗначение констЗнач, ЛЛБул *LosesInfo);
	// Операции над составными константами
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстТкстВКонтексте(ЛЛКонтекст к, const ткст0 стр,
	                                                          бцел длина,
	                                                        ЛЛБул неОканчиватьНулём);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстТкст(const ткст0 стр, бцел длина,
	                                                    ЛЛБул неОканчиватьНулём) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайЭлтКакКонст(ЛЛЗначение к, бцел инд) ;
	ЛЛИМПОРТ ЛЛБул ЛЛКонстТкст_ли(ЛЛЗначение к);
	ЛЛИМПОРТ const ткст0 ЛЛДайКакТкст(ЛЛЗначение к, т_мера *длина);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстМассив(ЛЛТип типЭлта,
	                            ЛЛЗначение *констЗначя, бцел длина);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстСтруктВКонтексте(ЛЛКонтекст к,
	                                      ЛЛЗначение *констЗначя,
	                                      бцел счёт, ЛЛБул упакован);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстСтрукт(ЛЛЗначение *констЗначя, бцел счёт,
	                                                            ЛЛБул упакован) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛИменованнаяКонстСтрукт(ЛЛТип типСтрукт,
	                                       ЛЛЗначение *констЗначя,
	                                                          бцел счёт) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстВектор(ЛЛЗначение *скалярнКонстЗначя, бцел разм) ;
	// Константные выражения
	ЛЛИМПОРТ ПКодОп ЛЛДайКонстОпкод(ЛЛЗначение констЗнач);
	ЛЛИМПОРТ ЛЛЗначение ЛЛРаскладУ(ЛЛТип тип);
	ЛЛИМПОРТ ЛЛЗначение ЛЛРазмерУ(ЛЛТип тип);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстОтр(ЛЛЗначение констЗнач);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстОтрНСВ(ЛЛЗначение констЗнач);
	ЛЛИМПОРТ ЛЛЗначение LLConstNUWNeg(ЛЛЗначение констЗнач);//
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстПОтриц(ЛЛЗначение констЗнач);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстНе(ЛЛЗначение констЗнач);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстДобавь(ЛЛЗначение леваяКонст, ЛЛЗначение праваяКонст);
	ЛЛИМПОРТ ЛЛЗначение LLConstNSWAdd(ЛЛЗначение леваяКонст, ЛЛЗначение праваяКонст);//
	ЛЛИМПОРТ ЛЛЗначение LLConstNUWAdd(ЛЛЗначение леваяКонст,  ЛЛЗначение праваяКонст);//
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстПСложи(ЛЛЗначение леваяКонст, ЛЛЗначение праваяКонст);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстОтними(ЛЛЗначение леваяКонст, ЛЛЗначение праваяКонст);
	ЛЛИМПОРТ ЛЛЗначение LLConstNSWSub(ЛЛЗначение леваяКонст, ЛЛЗначение праваяКонст);//
	ЛЛИМПОРТ ЛЛЗначение LLConstNUWSub(ЛЛЗначение леваяКонст, ЛЛЗначение праваяКонст);//
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстПОтними(ЛЛЗначение леваяКонст, ЛЛЗначение праваяКонст);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстУмножь(ЛЛЗначение леваяКонст, ЛЛЗначение праваяКонст) ;
	ЛЛИМПОРТ ЛЛЗначение LLConstNSWMul(ЛЛЗначение леваяКонст, ЛЛЗначение праваяКонст);//
	ЛЛИМПОРТ ЛЛЗначение LLConstNUWMul(ЛЛЗначение леваяКонст, ЛЛЗначение праваяКонст);//
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстПУмножь(ЛЛЗначение леваяКонст, ЛЛЗначение праваяКонст);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстБДели(ЛЛЗначение леваяКонст, ЛЛЗначение праваяКонст);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстТочноБДели(ЛЛЗначение леваяКонст, ЛЛЗначение праваяКонст);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстЗДели(ЛЛЗначение леваяКонст, ЛЛЗначение праваяКонст) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстТочноЗДели(ЛЛЗначение леваяКонст, ЛЛЗначение праваяКонст);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстПДели(ЛЛЗначение леваяКонст, ЛЛЗначение праваяКонст) ;
	ЛЛИМПОРТ ЛЛЗначение LLConstURem(ЛЛЗначение леваяКонст, ЛЛЗначение праваяКонст);//
	ЛЛИМПОРТ ЛЛЗначение LLConstSRem(ЛЛЗначение леваяКонст, ЛЛЗначение праваяКонст);//
	ЛЛИМПОРТ ЛЛЗначение LLConstFRem(ЛЛЗначение леваяКонст, ЛЛЗначение праваяКонст);//
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстИ(ЛЛЗначение леваяКонст, ЛЛЗначение праваяКонст) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстИли(ЛЛЗначение леваяКонст, ЛЛЗначение праваяКонст);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстИИли(ЛЛЗначение леваяКонст, ЛЛЗначение праваяКонст);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстЦСравн(ПЦелПредикат предикат,ЛЛЗначение леваяКонст,
	                                                               ЛЛЗначение праваяКонст);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстПСравн(ПРеалПредикат предикат, ЛЛЗначение леваяКонст,
	                                                             ЛЛЗначение праваяКонст);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстСдвл(ЛЛЗначение леваяКонст, ЛЛЗначение праваяКонст);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстСдвп(ЛЛЗначение леваяКонст, ЛЛЗначение праваяКонст);
	ЛЛИМПОРТ ЛЛЗначение LLConstAShr(ЛЛЗначение леваяКонст, ЛЛЗначение праваяКонст) ;//
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстУкНаЭлт(ЛЛЗначение констЗнач,
                  ЛЛЗначение *констИндексы, бцел числИндексы);
	ЛЛИМПОРТ ЛЛЗначение LLConstInBoundsGEP(ЛЛЗначение констЗнач,
	                                     ЛЛЗначение *констИндексы,
	                                                      бцел числИндексы);//
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстОбрежь(ЛЛЗначение констЗнач, ЛЛТип вТип);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстЗРасш(ЛЛЗначение констЗнач, ЛЛТип вТип);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстНРасш(ЛЛЗначение констЗнач, ЛЛТип вТип);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстПЗОбрежь(ЛЛЗначение констЗнач, ЛЛТип вТип) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстПЗРасш(ЛЛЗначение констЗнач, ЛЛТип вТип);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстБЦвПЗ(ЛЛЗначение констЗнач, ЛЛТип вТип);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстЗЦвПЗ(ЛЛЗначение констЗнач, ЛЛТип вТип);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстПЗвБЦ(ЛЛЗначение констЗнач, ЛЛТип вТип);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстПЗвЗЦ(ЛЛЗначение констЗнач, ЛЛТип вТип);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстУкзВЦел(ЛЛЗначение констЗнач, ЛЛТип вТип);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстЦелВУкз(ЛЛЗначение констЗнач, ЛЛТип вТип);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстБитКаст(ЛЛЗначение констЗнач, ЛЛТип вТип);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстАдрПрострКаст(ЛЛЗначение констЗнач, ЛЛТип вТип);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстНРасшИлиБитКаст(ЛЛЗначение констЗнач,ЛЛТип вТип) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстЗРасшИлиБитКаст(ЛЛЗначение констЗнач, ЛЛТип вТип);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстОбрежьИлиБитКаст(ЛЛЗначение констЗнач, ЛЛТип вТип);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстУказательКаст(ЛЛЗначение констЗнач, ЛЛТип вТип) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстЦелКаст(ЛЛЗначение констЗнач, ЛЛТип вТип,
	                                                             ЛЛБул соЗнаком_ли);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстПЗКаст(ЛЛЗначение констЗнач, ЛЛТип вТип);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстВыбор(ЛЛЗначение констУсловие, ЛЛЗначение констЕслиДа,
	                                                  ЛЛЗначение констЕслиНет) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстИзвлекиЭлемент(ЛЛЗначение векторнКонст,
	                                                  ЛЛЗначение индексКонст);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстВставьЭлемент(ЛЛЗначение векторнКонст,
	                                  ЛЛЗначение констЗначЭлта,
	                                                       ЛЛЗначение индексКонст);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстШафлВектор(ЛЛЗначение констВекторА,
	                                         ЛЛЗначение констВекторБ,
	                                                 ЛЛЗначение маскКонст);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстИзвлекиЗначение(ЛЛЗначение агрКонст, бцел *списИндов,
	                                                                  бцел чисИнд);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстВставьЗначение(ЛЛЗначение агрКонст,
	                                         ЛЛЗначение констЗначЭлта,
	                                             бцел *списИндов, бцел чисИнд);
	ЛЛИМПОРТ ЛЛЗначение ЛЛКонстИнлайнАсм(ЛЛТип тип, const ткст0 ткстАсма,
	                                        const ткст0 констрейнты,
	                                            ЛЛБул естьПобочка,
	                                                           ЛЛБул IsAlignStack);
	ЛЛИМПОРТ ЛЛЗначение ЛЛАдрБлока(ЛЛЗначение F, ЛЛБазовыйБлок бб);
	// Операции над глобальными переменными, функциями и псевдонимами (глобалы)
	ЛЛИМПОРТ ЛЛМодуль ЛЛДайГлобРодителя(ЛЛЗначение глоб);
	ЛЛИМПОРТ ЛЛБул ЛЛДекларация_ли(ЛЛЗначение глоб) ;
	ЛЛИМПОРТ ПЛинкаж ЛЛДайКомпоновку(ЛЛЗначение глоб);
	ЛЛИМПОРТ проц ЛЛУстКомпоновку(ЛЛЗначение глоб, ПЛинкаж ЛЛКомпоновка);
	ЛЛИМПОРТ const ткст0 ЛЛДайСекцию(ЛЛЗначение глоб) ;
	ЛЛИМПОРТ проц ЛЛУстСекцию(ЛЛЗначение глоб, const ткст0 Section);
	ЛЛИМПОРТ ПВидимость ЛЛДайВидимость(ЛЛЗначение глоб) ;
	ЛЛИМПОРТ проц ЛЛУстВидимость(ЛЛЗначение глоб, ПВидимость виз) ;
	ЛЛИМПОРТ ПКлассСохрДЛЛ ЛЛДайКлассХраненияДЛЛ(ЛЛЗначение глоб);
	ЛЛИМПОРТ проц ЛЛУстКлассХраненияДЛЛ(ЛЛЗначение глоб, ПКлассСохрДЛЛ класс);
	ЛЛИМПОРТ ПАдрБезИмени ЛЛДайБезымянныйАдрес(ЛЛЗначение глоб);
	ЛЛИМПОРТ проц ЛЛУстБезымянныйАдрес(ЛЛЗначение глоб, ПАдрБезИмени адрБезИмени);
	ЛЛИМПОРТ ЛЛБул ЛЛЕстьБезымянныйАдр(ЛЛЗначение глоб) ;
	ЛЛИМПОРТ проц ЛЛУстБезымянныйАдр(ЛЛЗначение глоб, ЛЛБул естьАдрБезИмени) ;
	ЛЛИМПОРТ ЛЛТип ЛЛГлобДайТипЗначения(ЛЛЗначение глоб);
	// Операции над глобальными переменными, инструкциями загрузки и хранения

	ЛЛИМПОРТ бцел ЛЛДайРаскладку(ЛЛЗначение зн) ;
	ЛЛИМПОРТ проц ЛЛУстРаскладку(ЛЛЗначение зн, бцел байты);
	ЛЛИМПОРТ ЛЛЗаписьМетаданныхЗначения *ЛЛГлоб_КопируйВсеМетаданные(ЛЛЗначение знач,
	                                                                  т_мера *чисЭлты);
	ЛЛИМПОРТ бцел ЛЛЗначение_ЗаписиМетаданных_ДайРод(ЛЛЗаписьМетаданныхЗначения *записи,
	                                                                    бцел инд);
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛЗначение_ЗаписиМетаданных_ДайМетаданные(ЛЛЗаписьМетаданныхЗначения *записи,
	                                                                   бцел инд) ;
	ЛЛИМПОРТ проц ЛЛВыместиЗаписиМетаданныхЗначения(ЛЛЗаписьМетаданныхЗначения *записи);
	ЛЛИМПОРТ проц ЛЛГлоб_УстановиМетаданные(ЛЛЗначение глоб, бцел род,
	                                                             ЛЛМетаданные MD);
	ЛЛИМПОРТ проц ЛЛГлоб_СотриМетаданные(ЛЛЗначение глоб, бцел род);
	ЛЛИМПОРТ проц ЛЛГлоб_СбросьМетаданные(ЛЛЗначение глоб);
	// Операции над глобальными переменными
	ЛЛИМПОРТ ЛЛЗначение ЛЛДобавьГлоб(ЛЛМодуль м, ЛЛТип тип, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДобавьГлобВАдрПрострво(ЛЛМодуль м, ЛЛТип тип,
	                                                    const ткст0 имя,
	                                                     бцел адрПрострв) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайИменованныйГлоб(ЛЛМодуль м, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайПервыйГлоб(ЛЛМодуль м) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайПоследнийГлоб(ЛЛМодуль м);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайСледщГлоб(ЛЛЗначение глобПерем);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайПредшГлоб(ЛЛЗначение глобПерем);
	ЛЛИМПОРТ проц ЛЛУдалиГлоб(ЛЛЗначение глобПерем) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайИнициализатор(ЛЛЗначение глобПерем);
	ЛЛИМПОРТ проц ЛЛУстИнициализатор(ЛЛЗначение глобПерем, ЛЛЗначение констЗнач);
	ЛЛИМПОРТ ЛЛБул ЛЛНителок_ли(ЛЛЗначение глобПерем) ;
	ЛЛИМПОРТ проц ЛЛУстНителок(ЛЛЗначение глобПерем, ЛЛБул нителок_ли);
	ЛЛИМПОРТ ЛЛБул ЛЛГлобКонст_ли(ЛЛЗначение глобПерем);
	ЛЛИМПОРТ проц ЛЛУстГлобКонст(ЛЛЗначение глобПерем, ЛЛБул конст_ли);
	ЛЛИМПОРТ ПРежимНителок ЛЛДайНителокРежим(ЛЛЗначение глобПерем);
	ЛЛИМПОРТ проц ЛЛУстНителокРежим(ЛЛЗначение глобПерем, ПРежимНителок режим);
	ЛЛИМПОРТ ЛЛБул ЛЛИзвнеИнициализуем_ли(ЛЛЗначение глобПерем);
	ЛЛИМПОРТ проц ЛЛУстИзвнеИнициализуем(ЛЛЗначение глобПерем, ЛЛБул IsExtInit);
	// Операции над псевдонимами
	ЛЛИМПОРТ ЛЛЗначение ЛЛДобавьНик(ЛЛМодуль м, ЛЛТип тип, ЛЛЗначение чейНик,
	                                                          const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайИменованГлобНик(ЛЛМодуль м,
	                                     const ткст0 имя, т_мера длинаИмени) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайПервыйГлобНик(ЛЛМодуль м) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайПоследнийГлобНик(ЛЛМодуль м);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайСледщГлобНик(ЛЛЗначение гн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайПредшГлобНик(ЛЛЗначение гн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛАлиас_ДайНики(ЛЛЗначение ник);
	ЛЛИМПОРТ проц ЛЛАлиас_УстНики(ЛЛЗначение ник, ЛЛЗначение чейНик);
	// Операции над функциями
	ЛЛИМПОРТ ЛЛЗначение ЛЛДобавьФункц(ЛЛМодуль м, const ткст0 имя,
	                                                   ЛЛТип типФункц);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайИменованФункц(ЛЛМодуль м, const ткст0 имя) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайПервФункц(ЛЛМодуль м);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайПоследнФункц(ЛЛМодуль м) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайСледщФункц(ЛЛЗначение фн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайПредшФункц(ЛЛЗначение фн);
	ЛЛИМПОРТ проц ЛЛУдалиФункц(ЛЛЗначение фн);
	ЛЛИМПОРТ ЛЛБул ЛЛЕстьПерсоналФн_ли(ЛЛЗначение фн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайПерсоналФн(ЛЛЗначение фн);
	ЛЛИМПОРТ проц ЛЛУстПерсоналФн(ЛЛЗначение фн, ЛЛЗначение персоналФн);
	ЛЛИМПОРТ бцел ЛЛДАйИнтринсикИД(ЛЛЗначение фн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайИнтринсикДекл(ЛЛМодуль мод,
	                                         бцел ид,
	                                         ЛЛТип *типыПарам,
	                                         т_мера счётПарам);
	ЛЛИМПОРТ const ткст0 ЛЛИнтринсик_ДайИмя(бцел ид, т_мера *длинаИмени);
	ЛЛИМПОРТ ЛЛТип ЛЛИнтринсик_ДайТип(ЛЛКонтекст контекст, бцел ид,
	                                 ЛЛТип *типыПарам, т_мера счётПарам) ;
	ЛЛИМПОРТ const ткст0 ЛЛИнтринсик_КопируйПерегруженИмя(бцел ид,
	                                            ЛЛТип *типыПарам,
	                                            т_мера счётПарам,
	                                            т_мера *длинаИмени);
	ЛЛИМПОРТ бцел ЛЛИщиИнтринсикИД(const ткст0 имя, т_мера длинаИмени);
	ЛЛИМПОРТ ЛЛБул ЛЛИнтринсик_Перегружен_ли(бцел ид);
	ЛЛИМПОРТ бцел ЛЛДайКонвВызФунции(ЛЛЗначение фн);
	ЛЛИМПОРТ проц ЛЛУстКонвВызФунции(ЛЛЗначение фн, бцел CC);
	ЛЛИМПОРТ const ткст0 ЛЛДайСМ(ЛЛЗначение фн);
	ЛЛИМПОРТ проц ЛЛУстСМ(ЛЛЗначение фн, const ткст0 GC);
	ЛЛИМПОРТ проц ЛЛДобавьАтрПоИндексу(ЛЛЗначение F, ЛЛИндексАтрибута инд,
	                                                                ЛЛАтрибут A);
	ЛЛИМПОРТ бцел ЛЛДайСчётАтровПоИндексу(ЛЛЗначение F, ЛЛИндексАтрибута инд);
	ЛЛИМПОРТ проц ЛЛДайАтрыПоИндексу(ЛЛЗначение F, ЛЛИндексАтрибута инд,
	                                                            ЛЛАтрибут *атры) ;
	ЛЛИМПОРТ ЛЛАтрибут ЛЛДайАтрПеречняПоИндексу(ЛЛЗначение F,
	                                             ЛЛИндексАтрибута инд,
	                                                         бцел идК);
	ЛЛИМПОРТ ЛЛАтрибут ЛЛДайТкстАтрПоИндексу(ЛЛЗначение F,
	                                               ЛЛИндексАтрибута инд,
	                                               const ткст0 K, бцел длинаК);
	ЛЛИМПОРТ проц ЛЛУдалиАтрПеречняПоИндексу(ЛЛЗначение F, ЛЛИндексАтрибута инд,
	                                                               бцел идК);
	ЛЛИМПОРТ проц ЛЛУдалиТкстАтрПоИндексу(ЛЛЗначение F, ЛЛИндексАтрибута инд,
	                                                 const ткст0 K, бцел длинаК);
	ЛЛИМПОРТ проц ЛЛДобавьЦелеЗависимАтрФции(ЛЛЗначение фн, const ткст0 A,
	                                                           const ткст0 зн);
	// Операции над параметрами
	ЛЛИМПОРТ бцел ЛЛПосчитайПарамы(ЛЛЗначение сФн) ;
	ЛЛИМПОРТ проц ЛЛДайПарамы(ЛЛЗначение сФн, ЛЛЗначение *ParamRefs);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайПарам(ЛЛЗначение сФн, бцел индекс);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайПредкаПарам(ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайПервПарам(ЛЛЗначение фн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайПоследнПарам(ЛЛЗначение фн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайСледщПарам(ЛЛЗначение арг);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайПредшПарам(ЛЛЗначение арг);
	ЛЛИМПОРТ проц ЛЛУстРаскладПарама(ЛЛЗначение арг, бцел align) ;
	// Операции над ifuncs
	ЛЛИМПОРТ ЛЛЗначение ЛЛДобавьГлобИФункц(ЛЛМодуль м,
	                                const ткст0 имя, т_мера длинаИмени,
	                                           ЛЛТип тип, бцел AddrSpace,
	                                                      ЛЛЗначение Resolver);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайИменованГлобИФункц(ЛЛМодуль м,
	                                     const ткст0 имя, т_мера длинаИмени) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайПервГлобИФункц(ЛЛМодуль м);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайПоследнГлобИФункц(ЛЛМодуль м);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайСледщГлобИФункц(ЛЛЗначение IFunc);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайПредшГлобИФункц(ЛЛЗначение IFunc);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайРезольверГлобИФункц(ЛЛЗначение IFunc);
	ЛЛИМПОРТ проц ЛЛУстРезольверГлобИФункц(ЛЛЗначение IFunc, ЛЛЗначение Resolver) ;
	ЛЛИМПОРТ проц ЛЛСотриГлобИФункц(ЛЛЗначение IFunc);
	ЛЛИМПОРТ проц ЛЛУдалиГлобИФункц(ЛЛЗначение IFunc);
	// Операции над базовыми блоками
	ЛЛИМПОРТ ЛЛЗначение ЛЛБазБлокКакЗначение(ЛЛБазовыйБлок бб);
	ЛЛИМПОРТ ЛЛБул ЛЛЗначение_БазБлок_ли(ЛЛЗначение знач);
	ЛЛИМПОРТ ЛЛБазовыйБлок ЛЛЗначениеКакБазБлок(ЛЛЗначение знач);
	ЛЛИМПОРТ const ткст0 ЛЛДайИмяБазБлока(ЛЛБазовыйБлок бб) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайРодителяБазБлока(ЛЛБазовыйБлок бб);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайТерминаторБазБлока(ЛЛБазовыйБлок бб);
	ЛЛИМПОРТ бцел ЛЛПосчитайБазБлоки(ЛЛЗначение сФн) ;
	ЛЛИМПОРТ проц ЛЛДайБазБлоки(ЛЛЗначение сФн, ЛЛБазовыйБлок *сББи);
	ЛЛИМПОРТ ЛЛБазовыйБлок ЛЛДайВводнБазБлок(ЛЛЗначение фн) ;
	ЛЛИМПОРТ ЛЛБазовыйБлок ЛЛДайПервБазБлок(ЛЛЗначение фн);
	ЛЛИМПОРТ ЛЛБазовыйБлок ЛЛДайПоследнБазБлок(ЛЛЗначение фн);
	ЛЛИМПОРТ ЛЛБазовыйБлок ЛЛДайСледщБазБлок(ЛЛБазовыйБлок бб) ;
	ЛЛИМПОРТ ЛЛБазовыйБлок ЛЛДайПредшБазБлок(ЛЛБазовыйБлок бб);
	ЛЛИМПОРТ ЛЛБазовыйБлок ЛЛСоздайБазБлокВКонтексте(ЛЛКонтекст к, const ткст0 имя);
	ЛЛИМПОРТ проц ЛЛВставьСущБазБлокПослеБлокаВставки(ЛЛПостроитель постр, ЛЛБазовыйБлок бб);
	ЛЛИМПОРТ проц ЛЛПриставьСущБазБлок(ЛЛЗначение фн,
	                                  ЛЛБазовыйБлок бб);
	ЛЛИМПОРТ ЛЛБазовыйБлок ЛЛПриставьБазБлокВКонтексте(ЛЛКонтекст к, ЛЛЗначение сФн,
	                                                                     const ткст0 имя);
	ЛЛИМПОРТ ЛЛБазовыйБлок ЛЛПриставьБазБлок(ЛЛЗначение сФн, const ткст0 имя);
	ЛЛИМПОРТ ЛЛБазовыйБлок ЛЛВставьБазБлокВКонтекст(ЛЛКонтекст к,
	                                                ЛЛБазовыйБлок сББ,
	                                                     const ткст0 имя);
	ЛЛИМПОРТ ЛЛБазовыйБлок ЛЛВставьБазБлок(ЛЛБазовыйБлок сББ,
	                                                  const ткст0 имя);
	ЛЛИМПОРТ проц ЛЛУдалиБазБлок(ЛЛБазовыйБлок сББ) ;
	ЛЛИМПОРТ проц ЛЛУдалиБазБлокИзРодителя(ЛЛБазовыйБлок сББ);
	ЛЛИМПОРТ проц ЛЛПоставьБазБлокПеред(ЛЛБазовыйБлок бб, ЛЛБазовыйБлок MovePos);
	ЛЛИМПОРТ проц ЛЛПоставьБазБлокПосле(ЛЛБазовыйБлок бб, ЛЛБазовыйБлок MovePos) ;
	// Операции над инструкциями
	ЛЛИМПОРТ ЛЛБазовыйБлок ЛЛДайРодителяИнстр(ЛЛЗначение инст);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайПервИнстр(ЛЛБазовыйБлок бб) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайПоследнИнстр(ЛЛБазовыйБлок бб);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайСледщИнстр(ЛЛЗначение инст);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайПредшИнстр(ЛЛЗначение инст);
	ЛЛИМПОРТ проц ЛЛИнструкция_УдалиИзРодителя(ЛЛЗначение инст);
	ЛЛИМПОРТ проц ЛЛИнструкция_СотриИзРодителя(ЛЛЗначение инст);
	ЛЛИМПОРТ ПЦелПредикат ЛЛДайПредикатЦСравн(ЛЛЗначение инст);
	ЛЛИМПОРТ ПРеалПредикат ЛЛДайПредикатПСравн(ЛЛЗначение инст);
	ЛЛИМПОРТ ПКодОп ЛЛДайОпкодИнстр(ЛЛЗначение инст) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнструкция_Клонируй(ЛЛЗначение инст) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛИнстрТерминатор_ли(ЛЛЗначение инст);
	ЛЛИМПОРТ бцел ЛЛДайЧлоАргОперандов(ЛЛЗначение инстр);
	// Вызов и выполнение инструкций
	ЛЛИМПОРТ бцел ЛЛДайКонвВызИнстр(ЛЛЗначение инстр);
	ЛЛИМПОРТ проц ЛЛУстКонвВызИнстр(ЛЛЗначение инстр, бцел CC);
	ЛЛИМПОРТ проц ЛЛУстРаскладПарамовИнстр(ЛЛЗначение инстр, бцел индекс,
	                                                            бцел align);
	ЛЛИМПОРТ проц ЛЛДобавьАтрМестаВызова(ЛЛЗначение к, ЛЛИндексАтрибута инд,
	                                                                ЛЛАтрибут A);
	ЛЛИМПОРТ бцел ЛЛДайЧлоАтровМестаВызова(ЛЛЗначение к,
	                                                ЛЛИндексАтрибута инд);
	ЛЛИМПОРТ проц ЛЛДайАтрыМестаВызова(ЛЛЗначение к, ЛЛИндексАтрибута инд,
	                                                           ЛЛАтрибут *атры);
	ЛЛИМПОРТ ЛЛАтрибут ЛЛДайАтрыПеречняМестаВызова(ЛЛЗначение к, ЛЛИндексАтрибута инд,
	                                                                    бцел идК);
	ЛЛИМПОРТ ЛЛАтрибут ЛЛДайТкстАтрыМестаВызова(ЛЛЗначение к, ЛЛИндексАтрибута инд,
	                                                      const ткст0 K, бцел длинаК);
	ЛЛИМПОРТ проц ЛЛУдалиАтрПеречняМестаВызова(ЛЛЗначение к, ЛЛИндексАтрибута инд,
	                                                                    бцел идК);
	ЛЛИМПОРТ проц ЛЛУдалиТкстАтрМестаВызова(ЛЛЗначение к, ЛЛИндексАтрибута инд,
	                                                       const ткст0 K, бцел длинаК);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайВызванноеЗнач(ЛЛЗначение инстр) ;
	ЛЛИМПОРТ ЛЛТип ЛЛДайТипВызваннойФункц(ЛЛЗначение инстр) ;
	// Операции над инструкциями вызова (только call)
	ЛЛИМПОРТ ЛЛБул ЛЛТейлВызов_ли(ЛЛЗначение вызов) ;
	ЛЛИМПОРТ проц ЛЛУстТейлВызов(ЛЛЗначение вызов, ЛЛБул тейлВыз_ли);
	// Операции над инструкциями выполнения (только invoke)
	ЛЛИМПОРТ ЛЛБазовыйБлок ЛЛДайНормальнПриёмник(ЛЛЗначение инвок);
	ЛЛИМПОРТ ЛЛБазовыйБлок ЛЛДайПриёмникОтмотки(ЛЛЗначение инвок) ;
	ЛЛИМПОРТ проц ЛЛУстНормальнПриёмник(ЛЛЗначение инвок, ЛЛБазовыйБлок б) ;
	ЛЛИМПОРТ проц ЛЛУстПриёмникОтмотки(ЛЛЗначение инвок, ЛЛБазовыйБлок б) ;
	// Операции над терминаторами
	ЛЛИМПОРТ бцел ЛЛДайЧлоПоследователей(ЛЛЗначение Term);
	ЛЛИМПОРТ ЛЛБазовыйБлок ЛЛДайПоследователь(ЛЛЗначение Term, бцел i);
	ЛЛИМПОРТ проц ЛЛУстПоследователь(ЛЛЗначение Term, бцел i, ЛЛБазовыйБлок блок);
	// Операции над инструкциями ветвления (только)
	ЛЛИМПОРТ ЛЛБул ЛЛУсловн_ли(ЛЛЗначение ветвь) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайУсловие(ЛЛЗначение ветвь);
	ЛЛИМПОРТ проц ЛЛУстУсловие(ЛЛЗначение ветвь, ЛЛЗначение услов);
	// Операции над инструкциями переключения (только)
	ЛЛИМПОРТ ЛЛБазовыйБлок ЛЛДайДефПриёмникРеле(ЛЛЗначение щит) ;
	// Операции над инструкциями alloca (только)
	ЛЛИМПОРТ ЛЛТип ЛЛДайРазмещТип(ЛЛЗначение аллока) ;
	// Операции над инструкциями gep (только)
	ЛЛИМПОРТ ЛЛБул ЛЛвПределах_ли(ЛЛЗначение укНаЭлт);
	ЛЛИМПОРТ проц ЛЛУстВПределах(ЛЛЗначение укНаЭлт, ЛЛБул InBounds);
	// Операции над узлами phi
	ЛЛИМПОРТ проц ЛЛДобавьВходящ(ЛЛЗначение PhiNode, ЛЛЗначение *IncomingValues,
	                                ЛЛБазовыйБлок *IncomingBlocks, бцел счёт);
	ЛЛИМПОРТ бцел ЛЛПосчитайВходящ(ЛЛЗначение PhiNode);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайВходящЗнач(ЛЛЗначение PhiNode, бцел инд);
	ЛЛИМПОРТ ЛЛБазовыйБлок ЛЛДайВходящБлок(ЛЛЗначение PhiNode, бцел инд);
	// Операции над узлами extractvalue и insertvalue
	ЛЛИМПОРТ бцел ЛЛДайЧлоИндексов(ЛЛЗначение инст);
	ЛЛИМПОРТ const бцел *ЛЛДайИндексы(ЛЛЗначение инст);
	// Построители инструкций
	ЛЛИМПОРТ ЛЛПостроитель ЛЛСоздайПостроительВКонтексте(ЛЛКонтекст к);
	ЛЛИМПОРТ ЛЛПостроитель ЛЛСоздайПостроитель(проц);
	ЛЛИМПОРТ проц ЛЛПостроительПозиции(ЛЛПостроитель постр, ЛЛБазовыйБлок б,
	                                                               ЛЛЗначение инстр) ;
	ЛЛИМПОРТ проц ЛЛПостроительПозицииПеред(ЛЛПостроитель постр, ЛЛЗначение инстр);
	ЛЛИМПОРТ проц ЛЛПостроительПозицииВКонце(ЛЛПостроитель постр, ЛЛБазовыйБлок б);
	ЛЛИМПОРТ ЛЛБазовыйБлок ЛЛДайБлокВставки(ЛЛПостроитель постр);
	ЛЛИМПОРТ проц ЛЛОчистиПозициюВставки(ЛЛПостроитель постр) ;
	ЛЛИМПОРТ проц ЛЛВставьВПостроитель(ЛЛПостроитель постр, ЛЛЗначение инстр);
	ЛЛИМПОРТ проц ЛЛВставьВПостроительСИменем(ЛЛПостроитель постр, ЛЛЗначение инстр,
	                                                               const ткст0 имя);
	ЛЛИМПОРТ проц ЛЛВыместиПостроитель(ЛЛПостроитель постр) ;
	// Построители метаданных
	ЛЛИМПОРТ ЛЛМетаданные ЛЛДайТекЛокОтладки2(ЛЛПостроитель постр) ;
	ЛЛИМПОРТ проц ЛЛУстТекЛокОтладки2(ЛЛПостроитель постр, ЛЛМетаданные лок);
	ЛЛИМПОРТ проц ЛЛУстТекЛокОтладки(ЛЛПостроитель постр, ЛЛЗначение L);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайТекЛокОтладки(ЛЛПостроитель постр) ;
	ЛЛИМПОРТ проц ЛЛУстТекЛокОтладкиИнстр(ЛЛПостроитель постр, ЛЛЗначение инст);
	ЛЛИМПОРТ проц ЛЛПостроитель_УстДефПЗМатТег(ЛЛПостроитель постр,
	                                            ЛЛМетаданные FPMathTag);
	ЛЛИМПОРТ ЛЛМетаданные ЛЛПостроитель_ДайДефПЗМатТег(ЛЛПостроитель постр);
	// Построители инструкций
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройВозврПроц(ЛЛПостроитель б) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройВозвр(ЛЛПостроитель б, ЛЛЗначение зн);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройАгрегатВозвр(ЛЛПостроитель б, ЛЛЗначение *RetVals, бцел N);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройБр(ЛЛПостроитель б, ЛЛБазовыйБлок приёмник);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройУсловнБр(ЛЛПостроитель б, ЛЛЗначение If,
	                             ЛЛБазовыйБлок Then, ЛЛБазовыйБлок Else);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройЩит(ЛЛПостроитель б, ЛЛЗначение зн,
	                             ЛЛБазовыйБлок Else, бцел NumCases);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройНепрямБр(ЛЛПостроитель б, ЛЛЗначение адр,
	                                                     бцел NumDests);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройИнвок(ЛЛПостроитель б, ЛЛЗначение фн,
	                                 ЛЛЗначение *аргs, бцел члоарг,
	                                     ЛЛБазовыйБлок Then, ЛЛБазовыйБлок Catch,
	                                                            const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройИнвок2(ЛЛПостроитель б, ЛЛТип тип, ЛЛЗначение фн,
	                                         ЛЛЗначение *аргs, бцел члоарг,
	                                      ЛЛБазовыйБлок Then, ЛЛБазовыйБлок Catch,
	                                                              const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтойЛэндингПад(ЛЛПостроитель б, ЛЛТип тип,
	                                 ЛЛЗначение PersFn, бцел NumClauses,
	                                                         const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройКэчПад(ЛЛПостроитель б, ЛЛЗначение ParentPad,
	                                     ЛЛЗначение *аргs, бцел члоарг,
	                                                           const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройОчистиПад(ЛЛПостроитель б, ЛЛЗначение ParentPad,
	                                         ЛЛЗначение *аргs, бцел члоарг,
	                                                             const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройВозобнови(ЛЛПостроитель б, ЛЛЗначение Exn);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройКэчЩит(ЛЛПостроитель б, ЛЛЗначение ParentPad,
	                       ЛЛБазовыйБлок UnwindBB, бцел NumHandlers, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройКэчВозвр(ЛЛПостроитель б, ЛЛЗначение CatchPad,
	                                                             ЛЛБазовыйБлок бб);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройОчистиВозвр(ЛЛПостроитель б, ЛЛЗначение CatchPad,
	                                                            ЛЛБазовыйБлок бб);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройНедоступный(ЛЛПостроитель б);
	ЛЛИМПОРТ проц ЛЛДобавьРеле(ЛЛЗначение щит, ЛЛЗначение OnVal,
	                                                ЛЛБазовыйБлок приёмник);
	ЛЛИМПОРТ проц ЛЛДобавьПриёмник(ЛЛЗначение IndirectBr, ЛЛБазовыйБлок приёмник);
	ЛЛИМПОРТ бцел ЛЛДайЧлоКлоз(ЛЛЗначение LandingPad);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайКлоз(ЛЛЗначение LandingPad, бцел инд);
	ЛЛИМПОРТ проц ЛЛДобавьКлоз(ЛЛЗначение LandingPad, ЛЛЗначение ClauseVal) ;
	ЛЛИМПОРТ ЛЛБул ЛЛОчистка_ли(ЛЛЗначение LandingPad) ;
	ЛЛИМПОРТ проц ЛЛУстОчистку(ЛЛЗначение LandingPad, ЛЛБул знач) ;
	ЛЛИМПОРТ проц ЛЛДобавьОбработчик(ЛЛЗначение CatchSwitch, ЛЛБазовыйБлок приёмник) ;
	ЛЛИМПОРТ бцел ЛЛДайЧлоОбработчиков(ЛЛЗначение CatchSwitch);
	ЛЛИМПОРТ проц ЛЛДайОбработчики(ЛЛЗначение CatchSwitch, ЛЛБазовыйБлок *Handlers);
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайРодительскКэчЩит(ЛЛЗначение CatchPad) ;
	ЛЛИМПОРТ проц ЛЛУстРодительскКэчЩит(ЛЛЗначение CatchPad, ЛЛЗначение CatchSwitch);
	// Функлеты
	ЛЛИМПОРТ ЛЛЗначение ЛЛДайАргОперанд(ЛЛЗначение функлет, бцел i);
	ЛЛИМПОРТ проц ЛЛУстАргОперанд(ЛЛЗначение функлет, бцел i, ЛЛЗначение знач) ;
	// Арифметика
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройСложи(ЛЛПостроитель б, ЛЛЗначение лев, ЛЛЗначение прав,
	                                                                    const ткст0 имя) ;
	ЛЛИМПОРТ ЛЛЗначение LLBuildNSWAdd(ЛЛПостроитель б, ЛЛЗначение лев, ЛЛЗначение прав,
	                                                                    const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение LLBuildNUWAdd(ЛЛПостроитель б, ЛЛЗначение лев, ЛЛЗначение прав,
	                                                                    const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройПСложи(ЛЛПостроитель б, ЛЛЗначение лев, ЛЛЗначение прав,
	                                                                    const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройОтними(ЛЛПостроитель б, ЛЛЗначение лев, ЛЛЗначение прав,
	                                                                  const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение LLBuildNSWSub(ЛЛПостроитель б, ЛЛЗначение лев, ЛЛЗначение прав,
	                                                                   const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение LLBuildNUWSub(ЛЛПостроитель б, ЛЛЗначение лев, ЛЛЗначение прав,
	                                                                    const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройПОтними(ЛЛПостроитель б, ЛЛЗначение лев, ЛЛЗначение прав,
	                                                                    const ткст0 имя) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройУмножь(ЛЛПостроитель б, ЛЛЗначение лев, ЛЛЗначение прав,
	                                                                     const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение LLBuildNSWMul(ЛЛПостроитель б, ЛЛЗначение лев, ЛЛЗначение прав,
	                                                                       const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение LLBuildNUWMul(ЛЛПостроитель б, ЛЛЗначение лев, ЛЛЗначение прав,
	                                                                     const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройПУмножь(ЛЛПостроитель б, ЛЛЗначение лев, ЛЛЗначение прав,
	                                                                      const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройБДели(ЛЛПостроитель б, ЛЛЗначение лев, ЛЛЗначение прав,
	                                                                     const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройТочноБДели(ЛЛПостроитель б, ЛЛЗначение лев,
	                                                    ЛЛЗначение прав, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройЗДели(ЛЛПостроитель б, ЛЛЗначение лев, ЛЛЗначение прав,
	                                                                    const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройТочноЗДели(ЛЛПостроитель б, ЛЛЗначение лев,
	                                                       ЛЛЗначение прав, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройПДели(ЛЛПостроитель б, ЛЛЗначение лев, ЛЛЗначение прав,
	                                                                     const ткст0 имя) ;
	ЛЛИМПОРТ ЛЛЗначение LLBuildURem(ЛЛПостроитель б, ЛЛЗначение лев, ЛЛЗначение прав,
	                                                                 const ткст0 имя) ;
	ЛЛИМПОРТ ЛЛЗначение LLBuildSRem(ЛЛПостроитель б, ЛЛЗначение лев, ЛЛЗначение прав,
	                                                                   const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение LLBuildFRem(ЛЛПостроитель б, ЛЛЗначение лев, ЛЛЗначение прав,
	                                                                   const ткст0 имя) ;
	ЛЛИМПОРТ ЛЛЗначение LLBuildShl(ЛЛПостроитель б, ЛЛЗначение лев, ЛЛЗначение прав,
	                                                                     const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение LLBuildLShr(ЛЛПостроитель б, ЛЛЗначение лев, ЛЛЗначение прав,
	                                                                   const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение LLBuildAShr(ЛЛПостроитель б, ЛЛЗначение лев, ЛЛЗначение прав,
	                                                                    const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройИ(ЛЛПостроитель б, ЛЛЗначение лев, ЛЛЗначение прав,
	                                                                     const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройИли(ЛЛПостроитель б, ЛЛЗначение лев, ЛЛЗначение прав,
	                                                                    const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройИИли(ЛЛПостроитель б, ЛЛЗначение лев, ЛЛЗначение прав,
	                                                                const ткст0 имя) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройБинОп(ЛЛПостроитель б, ПКодОп Op,
	                                  ЛЛЗначение лев, ЛЛЗначение прав,
	                                                    const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройОтриц(ЛЛПостроитель б, ЛЛЗначение зн, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение LLBuildNSWNeg(ЛЛПостроитель б, ЛЛЗначение зн,
	                                                    const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение LLBuildNUWNeg(ЛЛПостроитель б, ЛЛЗначение зн,
	                                                    const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройПОтриц(ЛЛПостроитель б, ЛЛЗначение зн, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройНе(ЛЛПостроитель б, ЛЛЗначение зн, const ткст0 имя);
	// Память
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройРазместПам(ЛЛПостроитель б, ЛЛТип тип,
	                                                  const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройРазместПамМасс(ЛЛПостроитель б, ЛЛТип тип,
	                                             ЛЛЗначение знач, const ткст0 имя) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройУстПам(ЛЛПостроитель б, ЛЛЗначение укз,
	                                       ЛЛЗначение знач, ЛЛЗначение длин,
	                                                          бцел расклад);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройКопирПам(ЛЛПостроитель б,
	                             ЛЛЗначение Dst, бцел DstAlign,
	                             ЛЛЗначение Src, бцел SrcAlign,
	                             ЛЛЗначение разм);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройПреместПам(ЛЛПостроитель б,
	                              ЛЛЗначение Dst, бцел DstAlign,
	                              ЛЛЗначение Src, бцел SrcAlign,
	                                                   ЛЛЗначение разм);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройАллока(ЛЛПостроитель б, ЛЛТип тип,
	                                                         const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройАллокаМасс(ЛЛПостроитель б, ЛЛТип тип,
	                                                        ЛЛЗначение знач, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройОсвободи(ЛЛПостроитель б, ЛЛЗначение значУкзателя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройЗагрузи(ЛЛПостроитель б, ЛЛЗначение значУкзателя,
	                                                                    const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройЗагрузи2(ЛЛПостроитель б, ЛЛТип тип,
	                                                    ЛЛЗначение значУкзателя, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройСохрани(ЛЛПостроитель б, ЛЛЗначение знач,
	                                                              ЛЛЗначение значУкзателя) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройЗабор(ЛЛПостроитель б, ПАтомичУпоряд упоряд,
	                                                            ЛЛБул isSingleThread, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройУкНаЭлт(ЛЛПостроитель б, ЛЛЗначение укз,
	                                         ЛЛЗначение *индексы, бцел числИндексы,
	                                                                   const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройУкНаЭлт2(ЛЛПостроитель б, ЛЛТип тип,
	                                         ЛЛЗначение укз, ЛЛЗначение *индексы,
	                                                       бцел числИндексы, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройУкНаЭлтВПределах(ЛЛПостроитель б, ЛЛЗначение укз,
	                                              ЛЛЗначение *индексы, бцел числИндексы,
	                                                                        const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройУкНаЭлтВПределах2(ЛЛПостроитель б, ЛЛТип тип,
	                                   ЛЛЗначение укз, ЛЛЗначение *индексы,
	                                                             бцел числИндексы, const ткст0 имя) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройУкНаЭлтСтрукт(ЛЛПостроитель б, ЛЛЗначение укз,
	                                                                бцел инд, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройУкНаЭлтСтрукт2(ЛЛПостроитель б, ЛЛТип тип,
	                                               ЛЛЗначение укз, бцел инд,
	                                                                 const ткст0 имя) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройГлобТкст(ЛЛПостроитель б, const ткст0 стр,
	                                                         const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройГлобТкстУкз(ЛЛПостроитель б, const ткст0 стр,
	                                                                const ткст0 имя);
	ЛЛИМПОРТ ЛЛБул ЛЛДайВолатил(ЛЛЗначение инстрДоступаКПам);
	ЛЛИМПОРТ проц ЛЛУстВолатил(ЛЛЗначение инстрДоступаКПам, ЛЛБул волатилен);
	ЛЛИМПОРТ ПАтомичУпоряд ЛЛДайПорядок(ЛЛЗначение инстрДоступаКПам);
	ЛЛИМПОРТ проц ЛЛУстПорядок(ЛЛЗначение инстрДоступаКПам, ПАтомичУпоряд упоряд);
	// Приведение к типу (касты)
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройОбрежь(ЛЛПостроитель б, ЛЛЗначение знач,
	                                               ЛЛТип типПриёмн, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройНРасш(ЛЛПостроитель б, ЛЛЗначение знач,
	                                               ЛЛТип типПриёмн, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройЗРасш(ЛЛПостроитель б, ЛЛЗначение знач,
	                                               ЛЛТип типПриёмн, const ткст0 имя) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройПЗвБЦ(ЛЛПостроитель б, ЛЛЗначение знач,
	                                               ЛЛТип типПриёмн, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройПЗвЗЦ(ЛЛПостроитель б, ЛЛЗначение знач,
	                                               ЛЛТип типПриёмн, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройБЦвПЗ(ЛЛПостроитель б, ЛЛЗначение знач,
	                                               ЛЛТип типПриёмн, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройЗЦвПЗ(ЛЛПостроитель б, ЛЛЗначение знач,
	                                               ЛЛТип типПриёмн, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройПЗОбрежь(ЛЛПостроитель б, ЛЛЗначение знач,
	                                               ЛЛТип типПриёмн, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройПЗРасш(ЛЛПостроитель б, ЛЛЗначение знач,
	                                               ЛЛТип типПриёмн, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройУкзВЦел(ЛЛПостроитель б, ЛЛЗначение знач,
	                                               ЛЛТип типПриёмн, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройЦелВУкз(ЛЛПостроитель б, ЛЛЗначение знач,
	                                               ЛЛТип типПриёмн, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройБитКаст(ЛЛПостроитель б, ЛЛЗначение знач,
	                                               ЛЛТип типПриёмн, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройАдрПрострКаст(ЛЛПостроитель б, ЛЛЗначение знач,
	                                               ЛЛТип типПриёмн, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройНРасшИлиБитКаст(ЛЛПостроитель б, ЛЛЗначение знач,
	                                               ЛЛТип типПриёмн, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройЗРасшИлиБитКаст(ЛЛПостроитель б, ЛЛЗначение знач,
	                                               ЛЛТип типПриёмн, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройОбрежьИлиБитКаст(ЛЛПостроитель б, ЛЛЗначение знач,
	                                                ЛЛТип типПриёмн, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройКаст(ЛЛПостроитель б, ПКодОп Op,
	                                     ЛЛЗначение знач, ЛЛТип типПриёмн, const ткст0 имя) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройУказательКаст(ЛЛПостроитель б, ЛЛЗначение знач,
	                                                        ЛЛТип типПриёмн, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройЦелКаст2(ЛЛПостроитель б, ЛЛЗначение знач,
	                                         ЛЛТип типПриёмн, ЛЛБул соЗнаком_ли,
	                                                                       const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройЦелКаст(ЛЛПостроитель б, ЛЛЗначение знач,
	                                                         ЛЛТип типПриёмн, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройПЗКаст(ЛЛПостроитель б, ЛЛЗначение знач,
	                                                         ЛЛТип типПриёмн, const ткст0 имя) ;
	// Сравнения
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройЦСравн(ЛЛПостроитель б, ПЦелПредикат Op,
	                                           ЛЛЗначение лев, ЛЛЗначение прав,
	                                                            const ткст0 имя) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройПСравн(ЛЛПостроитель б, ПРеалПредикат Op,
	                                            ЛЛЗначение лев, ЛЛЗначение прав,
	                                                             const ткст0 имя);
	// Различные инструкции
	ЛЛИМПОРТ ЛЛЗначение LLBuildPhi(ЛЛПостроитель б, ЛЛТип тип, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройВызов(ЛЛПостроитель б, ЛЛЗначение фн,
	                           ЛЛЗначение *аргs, бцел члоарг,
	                           const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройВызов2(ЛЛПостроитель б, ЛЛТип тип, ЛЛЗначение фн,
	                           ЛЛЗначение *аргs, бцел члоарг,
	                            const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройВыбери(ЛЛПостроитель б, ЛЛЗначение If,
	                             ЛЛЗначение Then, ЛЛЗначение Else,
	                             const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройВААрг(ЛЛПостроитель б, ЛЛЗначение список,
	                            ЛЛТип тип, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройИзвлекиЭлт(ЛЛПостроитель б, ЛЛЗначение векЗнач,
	                             ЛЛЗначение инд, const ткст0 имя) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройВставьЭлт(ЛЛПостроитель б, ЛЛЗначение векЗнач,
	                                    ЛЛЗначение значЭлта, ЛЛЗначение инд,
	                                    const ткст0 имя) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройШафлВектор(ЛЛПостроитель б, ЛЛЗначение V1,
	                                    ЛЛЗначение V2, ЛЛЗначение маска,
	                                    const ткст0 имя) ;
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройИзвлекиЗначение(ЛЛПостроитель б, ЛЛЗначение агрЗнач,
	                                   бцел инд, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройВставьЗначение(ЛЛПостроитель б, ЛЛЗначение агрЗнач,
	                                  ЛЛЗначение значЭлта, бцел инд,
	                                  const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройПусто(ЛЛПостроитель б, ЛЛЗначение знач,
	                             const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛЛСтройНеПусто(ЛЛПостроитель б, ЛЛЗначение знач,
	                                const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение ЛСтройУкзДифф(ЛЛПостроитель б, ЛЛЗначение лев,
	                              ЛЛЗначение прав, const ткст0 имя);
	ЛЛИМПОРТ ЛЛЗначение LLBuildAtomicRMW(ЛЛПостроитель б, LLVMAtomicRMWBinOp оп,
	                               ЛЛЗначение укз, ЛЛЗначение знач,
	                               ПАтомичУпоряд порядок,
	                               ЛЛБул одинПоток);
	ЛЛИМПОРТ ЛЛЗначение LLBuildAtomicCmpXchg(ЛЛПостроитель б, ЛЛЗначение укз,
	                                    ЛЛЗначение Cmp, ЛЛЗначение New,
	                                    ПАтомичУпоряд упорядУспех,
	                                    ПАтомичУпоряд упорядПровал,
	                                    ЛЛБул одинПоток);
	ЛЛИМПОРТ ЛЛБул ЛЛАтомнОднонить_ли(ЛЛЗначение атомнИнстр);
	ЛЛИМПОРТ проц ЛЛУстАтомнОднонить(ЛЛЗначение атомнИнстр, ЛЛБул новЗнач);
	ЛЛИМПОРТ ПАтомичУпоряд LLGetCmpXchgSuccessOrdering(ЛЛЗначение CmpXchgInst) ;
	ЛЛИМПОРТ проц LLSetCmpXchgSuccessOrdering(ЛЛЗначение CmpXchgInst,
	                                   ПАтомичУпоряд упоряд);
	ЛЛИМПОРТ ПАтомичУпоряд LLGetCmpXchgFailureOrdering(ЛЛЗначение CmpXchgInst);
	ЛЛИМПОРТ проц LLSetCmpXchgFailureOrdering(ЛЛЗначение CmpXchgInst,
	                                   ПАтомичУпоряд упоряд) ;
	// мод-провайдеры
	ЛЛИМПОРТ ЛЛМодульПровайдер
	                      ЛЛСоздайМодульПровайдерДляСущМодуля(ЛЛМодуль м);
	ЛЛИМПОРТ проц ЛЛВыместиМодульПровайдер(ЛЛМодульПровайдер MP) ;
	// Буферы памяти
	ЛЛИМПОРТ ЛЛБул ЛЛСоздайБуфПамССодержимымФайла(
	                          const ткст0 путь, ЛЛБуферПамяти *внМодemбуф, ткст0 *выхСооб);
	ЛЛИМПОРТ ЛЛБул ЛЛСоздайБуфПамСоСТДВХО(ЛЛБуферПамяти *внМодemбуф,
	                                                    ткст0 *выхСооб);
	ЛЛИМПОРТ ЛЛБуферПамяти ЛЛСоздайБуфПамСДиапазономПам(
	          const ткст0 вхоДанные,  т_мера длинаВхоДанных,  const ткст0 буфимя,
	                                                   ЛЛБул нужноОкончНулл);
	ЛЛИМПОРТ ЛЛБуферПамяти ЛЛСоздайБуфПамСКопиейДиапазонаПам(
	                 const ткст0 вхоДанные,  т_мера длинаВхоДанных, const ткст0 буфимя);
	ЛЛИМПОРТ const ткст0 ЛЛДайНачалоБуфера(ЛЛБуферПамяти буфПам);
	ЛЛИМПОРТ т_мера ЛЛДайРазмерБуфера(ЛЛБуферПамяти буфПам) ;
	ЛЛИМПОРТ проц ЛЛВыместиБуферПамяти(ЛЛБуферПамяти буфПам);
	// Реестр Проходок
	ЛЛИМПОРТ ЛЛРеестрПроходок ЛЛДайГлобРеестрПроходок(проц);
	// Менеджер Проходок
	ЛЛИМПОРТ ЛЛМенеджерПроходок ЛЛСоздайМенеджерПроходок(проц);
	ЛЛИМПОРТ ЛЛМенеджерПроходок ЛЛСоздайМенеджерФукнцПроходокДляМодуля(ЛЛМодуль м);
	ЛЛИМПОРТ ЛЛМенеджерПроходок ЛЛСоздайМенеджерФукнцПроходок(ЛЛМодульПровайдер п);
	ЛЛИМПОРТ ЛЛБул ЛЛЗапустиМенеджерПроходок(ЛЛМенеджерПроходок мп, ЛЛМодуль м);
	ЛЛИМПОРТ ЛЛБул ЛЛИнициализуйМенеджерФукнцПроходок(ЛЛМенеджерПроходок FPM) ;
	ЛЛИМПОРТ ЛЛБул ЛЛЗапустиМенеджерФукнцПроходок(ЛЛМенеджерПроходок FPM, ЛЛЗначение F) ;
	ЛЛИМПОРТ ЛЛБул ЛЛФинализуйМенеджерФукнцПроходок(ЛЛМенеджерПроходок FPM) ;
	ЛЛИМПОРТ проц ЛЛВыместиМенеджерПроходок(ЛЛМенеджерПроходок мп);
	// Управление потоками выполнения
	ЛЛИМПОРТ проц ЛЛСтопМультинить();
	ЛЛИМПОРТ ЛЛБул ЛЛМультинить_ли();
	ЛЛИМПОРТ ЛЛБул ЛЛСтартМультинить() ;
////////////////////////////////////////////////////////////////////////
//DebugInfo
/////////////////////////////////////////////////////////////////////////
/**
 * Debug info flags.
 */
typedef enum {
  LLVMDIFlagZero = 0,
  LLVMDIFlagPrivate = 1,
  LLVMDIFlagProtected = 2,
  LLVMDIFlagPublic = 3,
  LLVMDIFlagFwdDecl = 1 << 2,
  LLVMDIFlagAppleBlock = 1 << 3,
  LLVMDIFlagBlockByrefStruct = 1 << 4,
  LLVMDIFlagVirtual = 1 << 5,
  LLVMDIFlagArtificial = 1 << 6,
  LLVMDIFlagExplicit = 1 << 7,
  LLVMDIFlagPrototyped = 1 << 8,
  LLVMDIFlagObjcClassComplete = 1 << 9,
  LLVMDIFlagObjectPointer = 1 << 10,
  LLVMDIFlagVector = 1 << 11,
  LLVMDIFlagStaticMember = 1 << 12,
  LLVMDIFlagLValueReference = 1 << 13,
  LLVMDIFlagRValueReference = 1 << 14,
  LLVMDIFlagReserved = 1 << 15,
  LLVMDIFlagSingleInheritance = 1 << 16,
  LLVMDIFlagMultipleInheritance = 2 << 16,
  LLVMDIFlagVirtualInheritance = 3 << 16,
  LLVMDIFlagIntroducedVirtual = 1 << 18,
  LLVMDIFlagBitField = 1 << 19,
  LLVMDIFlagNoReturn = 1 << 20,
  LLVMDIFlagTypePassByValue = 1 << 22,
  LLVMDIFlagTypePassByReference = 1 << 23,
  LLVMDIFlagEnumClass = 1 << 24,
  LLVMDIFlagFixedEnum = LLVMDIFlagEnumClass, // Deprecated.
  LLVMDIFlagThunk = 1 << 25,
  LLVMDIFlagNonTrivial = 1 << 26,
  LLVMDIFlagBigEndian = 1 << 27,
  LLVMDIFlagLittleEndian = 1 << 28,
  LLVMDIFlagIndirectVirtualBase = (1 << 2) | (1 << 5),
  LLVMDIFlagAccessibility = LLVMDIFlagPrivate | LLVMDIFlagProtected |
                            LLVMDIFlagPublic,
  LLVMDIFlagPtrToMemberRep = LLVMDIFlagSingleInheritance |
                             LLVMDIFlagMultipleInheritance |
                             LLVMDIFlagVirtualInheritance
} ПФлагиОИ;

/**
 * Source languages known by DWARF.
 */
typedef enum {
  LLVMDWARFSourceLanguageC89,
  LLVMDWARFSourceLanguageC,
  LLVMDWARFSourceLanguageAda83,
  LLVMDWARFSourceLanguageC_plus_plus,
  LLVMDWARFSourceLanguageCobol74,
  LLVMDWARFSourceLanguageCobol85,
  LLVMDWARFSourceLanguageFortran77,
  LLVMDWARFSourceLanguageFortran90,
  LLVMDWARFSourceLanguagePascal83,
  LLVMDWARFSourceLanguageModula2,
  // New in DWARF v3:
  LLVMDWARFSourceLanguageJava,
  LLVMDWARFSourceLanguageC99,
  LLVMDWARFSourceLanguageAda95,
  LLVMDWARFSourceLanguageFortran95,
  LLVMDWARFSourceLanguagePLI,
  LLVMDWARFSourceLanguageObjC,
  LLVMDWARFSourceLanguageObjC_plus_plus,
  LLVMDWARFSourceLanguageUPC,
  LLVMDWARFSourceLanguageD,
  // New in DWARF v4:
  LLVMDWARFSourceLanguagePython,
  // New in DWARF v5:
  LLVMDWARFSourceLanguageOpenCL,
  LLVMDWARFSourceLanguageGo,
  LLVMDWARFSourceLanguageModula3,
  LLVMDWARFSourceLanguageHaskell,
  LLVMDWARFSourceLanguageC_plus_plus_03,
  LLVMDWARFSourceLanguageC_plus_plus_11,
  LLVMDWARFSourceLanguageOCaml,
  LLVMDWARFSourceLanguageRust,
  LLVMDWARFSourceLanguageC11,
  LLVMDWARFSourceLanguageSwift,
  LLVMDWARFSourceLanguageJulia,
  LLVMDWARFSourceLanguageDylan,
  LLVMDWARFSourceLanguageC_plus_plus_14,
  LLVMDWARFSourceLanguageFortran03,
  LLVMDWARFSourceLanguageFortran08,
  LLVMDWARFSourceLanguageRenderScript,
  LLVMDWARFSourceLanguageBLISS,
  // Vendor extensions:
  LLVMDWARFSourceLanguageMips_Assembler,
  LLVMDWARFSourceLanguageGOOGLE_RenderScript,
  LLVMDWARFSourceLanguageBORLAND_Delphi
} LLVMDWARFSourceLanguage;

/**
 * The amount of debug information to emit.
 */
typedef enum {
    LLVMDWARFEmissionNone = 0,
    LLVMDWARFEmissionFull,
    LLVMDWARFEmissionLineTablesOnly
} LLVMDWARFEmissionKind;

/**
 * The kind of metadata nodes.
 */
enum {
  LLVMMDStringMetadataKind,
  LLVMConstantAsMetadataMetadataKind,
  LLVMLocalAsMetadataMetadataKind,
  LLVMDistinctMDOperandPlaceholderMetadataKind,
  LLVMMDTupleMetadataKind,
  LLVMDILocationMetadataKind,
  LLVMDIExpressionMetadataKind,
  LLVMDIGlobalVariableExpressionMetadataKind,
  LLVMGenericDINodeMetadataKind,
  LLVMDISubrangeMetadataKind,
  LLVMDIEnumeratorMetadataKind,
  LLVMDIBasicTypeMetadataKind,
  LLVMDIDerivedTypeMetadataKind,
  LLVMDICompositeTypeMetadataKind,
  LLVMDISubroutineTypeMetadataKind,
  LLVMDIFileMetadataKind,
  LLVMDICompileUnitMetadataKind,
  LLVMDISubprogramMetadataKind,
  LLVMDILexicalBlockMetadataKind,
  LLVMDILexicalBlockFileMetadataKind,
  LLVMDINamespaceMetadataKind,
  LLVMDIModuleMetadataKind,
  LLVMDITemplateTypeParameterMetadataKind,
  LLVMDITemplateValueParameterMetadataKind,
  LLVMDIGlobalVariableMetadataKind,
  LLVMDILocalVariableMetadataKind,
  LLVMDILabelMetadataKind,
  LLVMDIObjCPropertyMetadataKind,
  LLVMDIImportedEntityMetadataKind,
  LLVMDIMacroMetadataKind,
  LLVMDIMacroFileMetadataKind,
  LLVMDICommonBlockMetadataKind
};
typedef unsigned ЛЛРодМетаданных;

/**
 * An LLVM DWARF type encoding.
 */
typedef unsigned LLVMDWARFTypeEncoding;

////////////////////////////////
    ЛЛИМПОРТ бцел ЛЛВерсияОтладМетадан(проц);
	ЛЛИМПОРТ бцел ЛЛДайВерсиюМодуляОтладМетадан(ЛЛМодуль мод);
	ЛЛИМПОРТ ЛЛБул ЛЛУдалиОтладИнфоВМодуле(ЛЛМодуль мод) ;
	ЛЛИМПОРТ ЛЛПостроительОИ ЛЛСоздайПостроительОИЗапрНеразр(ЛЛМодуль м);
	ЛЛИМПОРТ ЛЛПостроительОИ ЛЛСоздайПостроительОИ(ЛЛМодуль м) ;
	ЛЛИМПОРТ проц ЛЛВыместиПостроительОИ(ЛЛПостроительОИ постр) ;
	ЛЛИМПОРТ проц ЛЛПостроительОИ_Финализуй(ЛЛПостроительОИ постр);
	ЛЛИМПОРТ ЛЛМетаданные ЛЛПостроительОИ_СоздайЕдиницуКомпиляции(
	    ЛЛПостроительОИ постр, LLVMDWARFSourceLanguage яз,
	    ЛЛМетаданные сФайл, const ткст0 произв, т_мера длинаПроизв,
	    ЛЛБул оптимизирован, const ткст0 флаги, т_мера длинаФлагов,
	    бцел RuntimeVer, const ткст0 SplitName, т_мера SplitNameLen,
	    LLVMDWARFEmissionKind род, бцел DWOId, ЛЛБул SplitDebugInlining,
	    ЛЛБул DebugInfoForProfiling) ;
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛПостроительОИ_СоздайФайл(ЛЛПостроительОИ постр, const ткст0 имяф,
	                        т_мера длинаИмяф, const ткст0 дир,
	                        т_мера длинаДир) ;
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛПостроительОИ_СоздайМодуль(ЛЛПостроительОИ постр, ЛЛМетаданные ParentScope,
	                          const ткст0 имя, т_мера длинаИмени,
	                          const ткст0 ConfigMacros, т_мера ConfigMacrosLen,
	                          const ткст0 Includeпуть, т_мера IncludeпутьLen,
	                          const ткст0 ISysRoot, т_мера ISysRootLen) ;
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛПостроительОИ_СоздайПрострвоИмён(ЛЛПостроительОИ постр,
	                             ЛЛМетаданные ParentScope,
	                             const ткст0 имя, т_мера длинаИмени,
	                             ЛЛБул ExportSymbols) ;
	ЛЛИМПОРТ ЛЛМетаданные ЛЛПостроительОИ_СоздайФункц(
	    ЛЛПостроительОИ постр, ЛЛМетаданные масштаб, const ткст0 имя,
	    т_мера длинаИмени, const ткст0 LinkageName, т_мера LinkageNameLen,
	    ЛЛМетаданные файл, бцел номстр, ЛЛМетаданные тип,
	    ЛЛБул IsLocalToUnit, ЛЛБул IsDefinition,
	    бцел ScopeLine, ПФлагиОИ флаги, ЛЛБул IsOptimized);
	ЛЛИМПОРТ ЛЛМетаданные ЛЛПостроительОИ_СоздайЛексичБлок(
	    ЛЛПостроительОИ постр, ЛЛМетаданные масштаб,
	    ЛЛМетаданные файл, бцел строка, бцел графа) ;
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛПостроительОИ_СоздайФайлЛексичБлока(ЛЛПостроительОИ постр,
	                                    ЛЛМетаданные масштаб,
	                                    ЛЛМетаданные файл,
	                                    бцел Discriminator) ;
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛПостроительОИ_СоздайИмпортирМодульИзПрострваИмён(ЛЛПостроительОИ постр,
	                                               ЛЛМетаданные масштаб,
	                                               ЛЛМетаданные NS,
	                                               ЛЛМетаданные файл,
	                                               бцел строка) ;
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛПостроительОИ_СоздайИмпортирМодульИзАлиаса(ЛЛПостроительОИ постр,
	                                           ЛЛМетаданные масштаб,
	                                           ЛЛМетаданные ImportedEntity,
	                                           ЛЛМетаданные файл,
	                                           бцел строка);
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛПостроительОИ_СоздайИмпортирМодульИзМодуля(ЛЛПостроительОИ постр,
	                                            ЛЛМетаданные масштаб,
	                                            ЛЛМетаданные м,
	                                            ЛЛМетаданные файл,
	                                            бцел строка) ;
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛПостроительОИ_СоздайИмпортирДекларацию(ЛЛПостроительОИ постр,
	                                       ЛЛМетаданные масштаб,
	                                       ЛЛМетаданные декл,
	                                       ЛЛМетаданные файл,
	                                       бцел строка,
	                                       const ткст0 имя, т_мера длинаИмени);
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛПостроительОИ_СоздайЛокациюОтладки(ЛЛКонтекст контекст, бцел строка,
	                                 бцел графа, ЛЛМетаданные масштаб,
	                                 ЛЛМетаданные InlinedAt);
	ЛЛИМПОРТ бцел ЛЛЛокацОИ_ДайСтроку(ЛЛМетаданные локац) ;
	ЛЛИМПОРТ бцел ЛЛЛокацОИ_ДайСтолбец(ЛЛМетаданные локац);
	ЛЛИМПОРТ ЛЛМетаданные ЛЛЛокацОИ_ДайМасштаб(ЛЛМетаданные локац) ;
	ЛЛИМПОРТ ЛЛМетаданные ЛЛЛокацОИ_ДайИнлайнУ(ЛЛМетаданные локац) ;
	ЛЛИМПОРТ ЛЛМетаданные ЛЛМасштабОИ_ДайФайл(ЛЛМетаданные масштаб);
	ЛЛИМПОРТ const ткст0 ЛЛФайлОИ_ДайПапку(ЛЛМетаданные файл, бцел *длин) ;
	ЛЛИМПОРТ const ткст0 ЛЛФайлОИ_ДайИмяФ(ЛЛМетаданные файл, бцел *длин) ;
	ЛЛИМПОРТ const ткст0 ЛЛФайлОИ_ДайИсходник(ЛЛМетаданные файл, бцел *длин) ;
	ЛЛИМПОРТ ЛЛМетаданные ЛЛПостроительОИ_ДайИлиСоздайМассивТипа(ЛЛПостроительОИ постр,
	                                                  ЛЛМетаданные *данн,
	                                                  т_мера члоЭлтов);
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛПостроительОИ_СоздайТипПодпроцедуры(ЛЛПостроительОИ постр,
	                                  ЛЛМетаданные файл,
	                                  ЛЛМетаданные *типыПарамов,
	                                  бцел NumParameterTypes,
	                                  ПФлагиОИ флаги);
	ЛЛИМПОРТ ЛЛМетаданные ЛЛПостроительОИ_СоздайПеречислитель(ЛЛПостроительОИ постр,
	                                              const ткст0 имя, т_мера длинаИмени,
	                                              дол знач,
	                                              ЛЛБул безЗнака_ли) ;
	ЛЛИМПОРТ ЛЛМетаданные ЛЛПостроительОИ_СоздайТипПеречисления(
	    ЛЛПостроительОИ постр, ЛЛМетаданные масштаб, const ткст0 имя,
	    т_мера длинаИмени, ЛЛМетаданные файл, бцел номстр,
	    бдол размВБитах, бцел раскладВБитах, ЛЛМетаданные *элты,
	    бцел члоЭлтов, ЛЛМетаданные типКласс);
	ЛЛИМПОРТ ЛЛМетаданные ЛЛПостроительОИ_СоздайТипСоюз(
	    ЛЛПостроительОИ постр, ЛЛМетаданные масштаб, const ткст0 имя,
	    т_мера длинаИмени, ЛЛМетаданные файл, бцел номстр,
	    бдол размВБитах, бцел раскладВБитах, ПФлагиОИ флаги,
	    ЛЛМетаданные *элты, бцел члоЭлтов, бцел язРантайма,
	    const ткст0 уникИд, т_мера длинаУникИда);
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛПостроительОИ_СоздайТипМассив(ЛЛПостроительОИ постр, бдол разм,
	                             бцел раскладВБитах, ЛЛМетаданные тип,
	                             ЛЛМетаданные *Subscripts,
	                             бцел NumSubscripts);
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛПостроительОИ_СоздайТипВектор(ЛЛПостроительОИ постр, бдол разм,
	                              бцел раскладВБитах, ЛЛМетаданные тип,
	                              ЛЛМетаданные *Subscripts,
	                              бцел NumSubscripts) ;
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛПостроительОИ_СоздайНеукТип(ЛЛПостроительОИ постр, const ткст0 имя,
	                                   т_мера длинаИмени) ;
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛПостроительОИ_СоздайБазовыйТип(ЛЛПостроительОИ постр, const ткст0 имя,
	                             т_мера длинаИмени, бдол размВБитах,
	                             LLVMDWARFTypeEncoding кодировка,
	                             ПФлагиОИ флаги) ;
	ЛЛИМПОРТ ЛЛМетаданные ЛЛПостроительОИ_СоздайТипУказатель(
	    ЛЛПостроительОИ постр, ЛЛМетаданные типУказуемого,
	    бдол размВБитах, бцел раскладВБитах, бцел адрПрострв,
	    const ткст0 имя, т_мера длинаИмени) ;
	ЛЛИМПОРТ ЛЛМетаданные ЛЛПостроительОИ_СоздайТипСтрукт(
	    ЛЛПостроительОИ постр, ЛЛМетаданные масштаб, const ткст0 имя,
	    т_мера длинаИмени, ЛЛМетаданные файл, бцел номстр,
	    бдол размВБитах, бцел раскладВБитах, ПФлагиОИ флаги,
	    ЛЛМетаданные DerivedFrom, ЛЛМетаданные *элты,
	    бцел члоЭлтов, бцел язРантайма, ЛЛМетаданные виртабХолдер,
	    const ткст0 уникИд, т_мера длинаУникИда) ;
	ЛЛИМПОРТ ЛЛМетаданные ЛЛПостроительОИ_СоздайТипЧлен(
	    ЛЛПостроительОИ постр, ЛЛМетаданные масштаб, const ткст0 имя,
	    т_мера длинаИмени, ЛЛМетаданные файл, бцел номстр,
	    бдол размВБитах, бцел раскладВБитах, бдол смещВБитах,
	    ПФлагиОИ флаги, ЛЛМетаданные тип);
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛПостроительОИ_СоздайТипСтатичЧлен(
	    ЛЛПостроительОИ постр, ЛЛМетаданные масштаб, const ткст0 имя,
	    т_мера длинаИмени, ЛЛМетаданные файл, бцел номстр,
	    ЛЛМетаданные тип, ПФлагиОИ флаги, ЛЛЗначение констЗнач,
	    бцел раскладВБитах) ;
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛПостроительОИ_СоздайТипУкзНаЧлен(ЛЛПостроительОИ постр,
	                                     ЛЛМетаданные PointeeType,
	                                     ЛЛМетаданные ClassType,
	                                     бдол размВБитах,
	                                     бцел раскладВБитах,
	                                     ПФлагиОИ флаги) ;
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛПостроительОИ_СоздайЦВарОбджСи(ЛЛПостроительОИ постр,
	                            const ткст0 имя, т_мера длинаИмени,
	                            ЛЛМетаданные файл, бцел номстр,
	                            бдол размВБитах, бцел раскладВБитах,
	                            бдол смещВБитах, ПФлагиОИ флаги,
	                            ЛЛМетаданные тип, ЛЛМетаданные PropertyNode) ;
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛПостроительОИ_СоздайСвойствоОбджСи(ЛЛПостроительОИ постр,
	                                const ткст0 имя, т_мера длинаИмени,
	                                ЛЛМетаданные файл, бцел номстр,
	                                const ткст0 GetterName, т_мера GetterNameLen,
	                                const ткст0 SetterName, т_мера SetterNameLen,
	                                бцел PropertyAttributes,
	                                ЛЛМетаданные тип) ;
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛПостроительОИ_СоздайТипУкзНаОбъект(ЛЛПостроительОИ постр,
	                                     ЛЛМетаданные тип);
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛПостроительОИ_СоздайКвалифицированныйТип(ЛЛПостроительОИ постр, бцел Tag,
	                                 ЛЛМетаданные тип) ;
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛПостроительОИ_СоздайСсылочныйТип(ЛЛПостроительОИ постр, бцел Tag,
	                                 ЛЛМетаданные тип);
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛПостроительОИ_СоздайТипНуллУкз(ЛЛПостроительОИ постр) ;
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛПостроительОИ_СоздайТипдеф(ЛЛПостроительОИ постр, ЛЛМетаданные тип,
	                           const ткст0 имя, т_мера длинаИмени,
	                           ЛЛМетаданные файл, бцел номстр,
	                           ЛЛМетаданные масштаб) ;
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛПостроительОИ_СоздайНаследование(ЛЛПостроительОИ постр,
	                               ЛЛМетаданные тип, ЛЛМетаданные BaseTy,
	                               бдол BaseOffset, бцел VBPtrOffset,
	                               ПФлагиОИ флаги);
	ЛЛИМПОРТ ЛЛМетаданные ЛЛПостроительОИ_СоздайФорвардДекл(
	    ЛЛПостроительОИ постр, бцел Tag, const ткст0 имя,
	    т_мера длинаИмени, ЛЛМетаданные масштаб, ЛЛМетаданные файл, бцел строка,
	    бцел RuntimeLang, бдол размВБитах, бцел раскладВБитах,
	    const ткст0 уникИдент, т_мера длинаУникИдента);
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛПостроительОИ_СоздайПеремещаемыйСоставнойТип(
	    ЛЛПостроительОИ постр, бцел Tag, const ткст0 имя,
	    т_мера длинаИмени, ЛЛМетаданные масштаб, ЛЛМетаданные файл, бцел строка,
	    бцел RuntimeLang, бдол размВБитах, бцел раскладВБитах,
	    ПФлагиОИ флаги, const ткст0 уникИдент,
	    т_мера длинаУникИдента);
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛПостроительОИ_СоздайТипЧленПоля(ЛЛПостроительОИ постр,
	                                      ЛЛМетаданные масштаб,
	                                      const ткст0 имя, т_мера длинаИмени,
	                                      ЛЛМетаданные файл, бцел номстр,
	                                      бдол размВБитах,
	                                      бдол смещВБитах,
	                                      бдол смещХранаВБитах,
	                                      ПФлагиОИ флаги, ЛЛМетаданные тип);
	ЛЛИМПОРТ ЛЛМетаданные ЛЛПостроительОИ_СоздайТипКласс(ЛЛПостроительОИ постр,
	    ЛЛМетаданные масштаб, const ткст0 имя, т_мера длинаИмени,
	    ЛЛМетаданные файл, бцел номстр, бдол размВБитах,
	    бцел раскладВБитах, бдол смещВБитах, ПФлагиОИ флаги,
	    ЛЛМетаданные DerivedFrom,
	    ЛЛМетаданные *элты, бцел члоЭлтов,
	    ЛЛМетаданные виртабХолдер, ЛЛМетаданные узелШаблПарамов,
	    const ткст0 уникИдент, т_мера длинаУникИдента);
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛПостроительОИ_СоздайТипАртифициал(ЛЛПостроительОИ постр,
	                                  ЛЛМетаданные тип);
	ЛЛИМПОРТ const ткст0 ЛЛТипОИ_ДайИмя(ЛЛМетаданные отладТип, т_мера *длина);
	ЛЛИМПОРТ бдол ЛЛТипОИ_ДайРазмВБитах(ЛЛМетаданные отладТип);
	ЛЛИМПОРТ бдол ЛЛТипОИ_ДайСмещениеВБитах(ЛЛМетаданные отладТип);
	ЛЛИМПОРТ бцел ЛЛТипОИ_ДайРаскладкуВБитах(ЛЛМетаданные отладТип);
	ЛЛИМПОРТ бцел ЛЛТипОИ_ДайСтроку(ЛЛМетаданные отладТип);
	ЛЛИМПОРТ ПФлагиОИ ЛЛТипОИ_ДайФлаги(ЛЛМетаданные отладТип);
	ЛЛИМПОРТ ЛЛМетаданные ЛЛПостроительОИ_ДайИлиСоздайПоддиапазон(ЛЛПостроительОИ постр,
	                                                 дол LowerBound,
	                                                 дол счёт);
	ЛЛИМПОРТ ЛЛМетаданные ЛЛПостроительОИ_ДайИлиСоздайМассив(ЛЛПостроительОИ постр,
	                                              ЛЛМетаданные *данн,
	                                              т_мера члоЭлтов);
	ЛЛИМПОРТ ЛЛМетаданные ЛЛПостроительОИ_СоздайExpression(ЛЛПостроительОИ постр,
	                                              дол *адр, т_мера длина);
	ЛЛИМПОРТ ЛЛМетаданные
	ЛЛПостроительОИ_СОздайВыражениеКонстЗначения(ЛЛПостроительОИ постр,
	                                           дол знач);
	ЛЛИМПОРТ ЛЛМетаданные ЛЛПостроительОИ_СоздайВыражениеГлобПеременной(
	    ЛЛПостроительОИ постр, ЛЛМетаданные масштаб, const ткст0 имя,
	    т_мера длинаИмени, const ткст0 ЛЛКомпоновка, т_мера ссылДлина, ЛЛМетаданные файл,
	    бцел номстр, ЛЛМетаданные тип, ЛЛБул LocalToUnit,
	    ЛЛМетаданные Выр, ЛЛМетаданные декл, бцел раскладВБитах);
	ЛЛИМПОРТ ЛЛМетаданные ЛЛExprГлобПеременной_ДайПеременную(ЛЛМетаданные GVE);
	ЛЛИМПОРТ ЛЛМетаданные ЛЛВыражениеГлобПеременной_ДайВыражение(ЛЛМетаданные GVE);
	ЛЛИМПОРТ ЛЛМетаданные ЛЛПеременнаяОИ_ДайФайл(ЛЛМетаданные перем);
	ЛЛИМПОРТ ЛЛМетаданные ЛЛПеременнаяОИ_ДайМасштаб(ЛЛМетаданные перем);
	ЛЛИМПОРТ бцел ЛЛПеременнаяОИ_ДайСтроку(ЛЛМетаданные перем);
	ЛЛИМПОРТ ЛЛМетаданные ЛЛВременныйУзелМД(ЛЛКонтекст контекст, ЛЛМетаданные *данн,
	                                    т_мера члоЭлтов);
	ЛЛИМПОРТ проц ЛЛВыместиВременныйУзелМД(ЛЛМетаданные времУзел);
	ЛЛИМПОРТ проц ЛЛМетаданные_ЗамениВсеИспользованияНа(ЛЛМетаданные TempTargetMetadata,
	                                    ЛЛМетаданные Replacement);
	ЛЛИМПОРТ ЛЛМетаданные ЛЛПостроительОИ_СоздайВремФорвардДеклГлобПерем(
	    ЛЛПостроительОИ постр, ЛЛМетаданные масштаб, const ткст0 имя,
	    т_мера длинаИмени, const ткст0 ЛЛКомпоновка, т_мера ссылДлмна, ЛЛМетаданные файл,
	    бцел номстр, ЛЛМетаданные тип, ЛЛБул LocalToUnit,
	                                        ЛЛМетаданные декл, бцел раскладВБитах);
	ЛЛИМПОРТ ЛЛЗначение ЛЛПостроительОИ_ВставьДекларПеред(
	  ЛЛПостроительОИ постр, ЛЛЗначение Storage, ЛЛМетаданные перемИнф,
	                                           ЛЛМетаданные Выр, ЛЛМетаданные отладЛок, ЛЛЗначение инстр);
	ЛЛИМПОРТ ЛЛЗначение ЛЛПостроительОИ_ВставьДекларВКонце(
	                         ЛЛПостроительОИ постр, ЛЛЗначение Storage, ЛЛМетаданные перемИнф,
	                                                             ЛЛМетаданные Выр, ЛЛМетаданные отладЛок, ЛЛБазовыйБлок б);
	ЛЛИМПОРТ ЛЛЗначение ЛЛПостроительОИ_ВставьОтладЗначениеПеред(ЛЛПостроительОИ постр,
	                                               ЛЛЗначение знач,
	                                               ЛЛМетаданные перемИнф,
	                                               ЛЛМетаданные Выр,
	                                               ЛЛМетаданные отладЛок,
	                                                             ЛЛЗначение инстр);
	ЛЛИМПОРТ ЛЛЗначение ЛЛПостроительОИ_ВставьОтладЗначениеВКонце(ЛЛПостроительОИ постр,
	                                              ЛЛЗначение знач,
	                                              ЛЛМетаданные перемИнф,
	                                              ЛЛМетаданные Выр,
	                                              ЛЛМетаданные отладЛок,
	                                                              ЛЛБазовыйБлок б);
	ЛЛИМПОРТ ЛЛМетаданные ЛЛПостроительОИ_СоздайАвтоПеременную(
	                              ЛЛПостроительОИ постр, ЛЛМетаданные масштаб, const ткст0 имя,
	                                         т_мера длинаИмени, ЛЛМетаданные файл, бцел номстр, ЛЛМетаданные тип,
	                                                       ЛЛБул всегдаСохр, ПФлагиОИ флаги, бцел раскладВБитах);
	ЛЛИМПОРТ ЛЛМетаданные ЛЛПостроительОИ_СоздайПеременнуюПараметра(
	                                      ЛЛПостроительОИ постр, ЛЛМетаданные масштаб, const ткст0 имя,
	                                           т_мера длинаИмени, бцел аргNo, ЛЛМетаданные файл, бцел номстр,
	                                                       ЛЛМетаданные тип, ЛЛБул всегдаСохр, ПФлагиОИ флаги);
	ЛЛИМПОРТ ЛЛМетаданные ЛЛДайПодпрограмму(ЛЛЗначение функ);
	ЛЛИМПОРТ проц ЛЛУстПодпрограмму(ЛЛЗначение функ, ЛЛМетаданные сп);
	ЛЛИМПОРТ бцел ЛЛПодпрогаОИ_ДайСтроку(ЛЛМетаданные подпрога);
	ЛЛИМПОРТ ЛЛМетаданные ЛЛИнструкция_ДайОтладЛок(ЛЛЗначение инст);
	ЛЛИМПОРТ проц ЛЛИнструкция_УстОтладЛок(ЛЛЗначение инст, ЛЛМетаданные лок);
	ЛЛИМПОРТ ЛЛРодМетаданных ЛЛДайРодМетаданных(ЛЛМетаданные мданн) ;
////////////////////////////////////////////////////////////////////////////
//Disassembler
//////////////////////////////////////////////////////////////////////////
	ЛЛИМПОРТ  ЛЛКонтекстДизасма ЛЛСоздайДизасм(const ткст0 триадыИмя, ук дизИНфо,
	                                цел типТэга, ЛЛОбрвызОпИнфо дайОпИнфо,
	                                                ЛЛОбрвызПоискСимвола симпоиск) ;

	ЛЛИМПОРТ  ЛЛКонтекстДизасма ЛЛСоздайДизасмЦПБ(const ткст0 триада, const ткст0 CPU,
	                                              ук дизИНфо, цел типТэга,
	                                                     ЛЛОбрвызОпИнфо дайОпИнфо,
	                                                    ЛЛОбрвызПоискСимвола симпоиск);

	 ЛЛИМПОРТ  ЛЛКонтекстДизасма
	        ЛЛСоздайДизасмЦПБФичи(const ткст0 триада, const ткст0 CPU,
	                     const ткст0 Features, ук дизИНфо, цел типТэга,
	                                                 ЛЛОбрвызОпИнфо дайОпИнфо,
	                                                   ЛЛОбрвызПоискСимвола симпоиск);

	 ЛЛИМПОРТ    цел ЛЛУстОпцииДизасм(ЛЛКонтекстДизасма DC, бдол опции) ;

	ЛЛИМПОРТ     проц ЛЛВыместиДизасм(ЛЛКонтекстДизасма DC);

	 ЛЛИМПОРТ    т_мера ЛЛИнструкцияДизасм(ЛЛКонтекстДизасма DC, ббайт* байты,
	                                             бдол BytesSize, бдол PC,
	                                               ткст0 OutString, т_мера OutStringSize) ;
//////////////////////////////////////////////////////////////////////////////////
//Error
/////////////////////////////////////////////////////////////////////////////////

	ЛЛИМПОРТ ЛЛИдТипаОшибки ЛЛДайИдТипаОшибки(ЛЛОшибка Err);
	ЛЛИМПОРТ проц ЛЛКонсуммируйОш(ЛЛОшибка Err);
	ЛЛИМПОРТ ткст0 ЛЛДайОшСооб(ЛЛОшибка Err);
	ЛЛИМПОРТ проц ЛЛВыместиОшСооб(ткст0 ErrMsg);
	ЛЛИМПОРТ ЛЛИдТипаОшибки ЛЛДайТкстИдаТипаОш(проц);
	проц ЛЛУстановиОбрФатОш(ЛЛОбработчикФатальнойОшибки обраб);
	проц ЛЛСбросьОбрФатОш(проц) ;
	проц ЛЛАктивируйТрассировкуСтека(проц);
///////////////////////////////////////////////////////////////////////////////
//ExecutionEngine
//////////////////////////////////////////////////////////////////////////////
	 ЛЛИМПОРТ  проц LLLinkInMCJIT(проц);
	 ЛЛИМПОРТ    проц LLLinkInInterpreter(проц);
	    /*===-- Операции над генерными значениями --------------------------------------===*/
	 ЛЛИМПОРТ    ЛЛГенерноеЗначение ЛЛСоздайГенЗначЦел(ЛЛТип тип, бдол N, ЛЛБул соЗнаком_ли);
	 ЛЛИМПОРТ    ЛЛГенерноеЗначение ЛЛСоздайГенЗначУк(ук п);
	 ЛЛИМПОРТ   ЛЛГенерноеЗначение ЛЛСоздайГенЗначПлав(ЛЛТип тип, double N);
	 ЛЛИМПОРТ   бцел ЛЛШиринаГенЗначЦел(ЛЛГенерноеЗначение сГенЗнач);
	 ЛЛИМПОРТ   бдол ЛЛГенЗначВЦел(ЛЛГенерноеЗначение генЗнач,
	                                                   ЛЛБул соЗнаком_ли);
	 ЛЛИМПОРТ    ук ЛЛГенЗначВУк(ЛЛГенерноеЗначение генЗнач);
	 ЛЛИМПОРТ    double ЛЛГенЗначВПлав(ЛЛТип TyRef, ЛЛГенерноеЗначение генЗнач);
	 ЛЛИМПОРТ   проц ЛЛВыместиГенЗнач(ЛЛГенерноеЗначение генЗнач);
	    /*===-- Операции над движками выполнения -----------------------------------===*/
	 ЛЛИМПОРТ   ЛЛБул ЛЛСоздайДвижВыпДляМодуля(ЛЛДвижокВыполнения  *OutEE,
	                                                ЛЛМодуль м, ткст0 * вывОш);
	 ЛЛИМПОРТ   ЛЛБул ЛЛСоздайИнтерпретаторДляМодуля(ЛЛДвижокВыполнения  *OutInterp,
	                                                      ЛЛМодуль м, ткст0 * вывОш);
	 ЛЛИМПОРТ    ЛЛБул ЛЛСоздайДжИТКомпиляторДляМодуля(ЛЛДвижокВыполнения  *внешнДжит,
	                                   ЛЛМодуль м, бцел урОпц, ткст0 * вывОш);
	 ЛЛИМПОРТ    проц ЛЛИнициализуйОпцииМЦДжИТКомпилятора(
	                                       ЛЛОпцииКомпиляцииМЦДжИТ опции, т_мера размОпц);
	 ЛЛИМПОРТ    ЛЛБул ЛЛСоздайМЦДжИТКомпиляторДляМодуля(
	                       ЛЛДвижокВыполнения *внешнДжит, ЛЛМодуль м,
	                            ЛЛОпцииКомпиляцииМЦДжИТ опции, т_мера размОпц,
	                                                                  ткст0 * вывОш);
	 ЛЛИМПОРТ  проц ЛЛВыместиДвижВып(ЛЛДвижокВыполнения дв);
     ЛЛИМПОРТ     проц ЛЛВыполниСтатичКонструкторы(ЛЛДвижокВыполнения дв);
     ЛЛИМПОРТ     проц ЛЛВыполниСтатичДеструкторы(ЛЛДвижокВыполнения дв);
	 ЛЛИМПОРТ    цел ЛЛВыполниФункцКакГлавную(ЛЛДвижокВыполнения дв, ЛЛЗначение F,
	                                                бцел аргC, ткст0 const* аргV,
	                                                                ткст0 const* EnvP);
	 ЛЛИМПОРТ     ЛЛГенерноеЗначение ЛЛВыполниФункц(ЛЛДвижокВыполнения дв, ЛЛЗначение F,
	                                            бцел члоарг, ЛЛГенерноеЗначение * аргs);
	 ЛЛИМПОРТ   проц ЛЛОсвободиМашКодДляФункции(ЛЛДвижокВыполнения дв, ЛЛЗначение F);
	 ЛЛИМПОРТ  проц ЛЛДобавьМодуль(ЛЛДвижокВыполнения дв, ЛЛМодуль м);
	 ЛЛИМПОРТ    ЛЛБул ЛЛУдалиМодуль(ЛЛДвижокВыполнения дв, ЛЛМодуль м,
	                                         ЛЛМодуль * внМодod, ткст0 * вывОш);
	 ЛЛИМПОРТ   ЛЛБул ЛЛНайдиФункцию(ЛЛДвижокВыполнения дв, ткст0 имя,
	                                                     ЛЛЗначение * OutFn);
	 ЛЛИМПОРТ  ук ЛЛРекомпилИРекомпонуйФункц(ЛЛДвижокВыполнения дв,
	                                                        ЛЛЗначение фн);
	 ЛЛИМПОРТ   ЛЛДанныеОЦели ЛЛДайДанОЦелиДвижВыпа(ЛЛДвижокВыполнения дв);
     ЛЛИМПОРТ     ЛЛЦелеваяМашина  ЛЛДайЦелМашДвигВыпа(ЛЛДвижокВыполнения дв);
	 ЛЛИМПОРТ    проц ЛЛДобавьГлобМаппинг(ЛЛДвижокВыполнения дв, ЛЛЗначение глоб,
	                                                                             ук адр);
	 ЛЛИМПОРТ     ук ЛЛДайУкзНаГлоб(ЛЛДвижокВыполнения дв, ЛЛЗначение глоб);
	 ЛЛИМПОРТ     бдол ЛЛДайАдрГлобЗнач(ЛЛДвижокВыполнения дв, ткст0 имя);
	 ЛЛИМПОРТ    бдол ЛЛДайАдрФункц(ЛЛДвижокВыполнения дв, ткст0 имя);
	 ЛЛИМПОРТ   ЛЛМенеджерПамятиМЦДжИТ ЛЛСоздайПростойМенеджерПамМЦДжИТ(
	        ук Opaque,
	        ЛЛОбрвызМенеджерПамРазместиСекциюКода AllocateCodeSection,
	        ЛЛОбрвызМенеджерПамРазместиСекциюДанных AllocateDataSection,
	        ЛЛОбрвызМенеджерПамФинализуйПам FinalizeMemory,
	        ЛЛОбрвызМенеджерПамРазрушь Destroy);
	 ЛЛИМПОРТ     проц ЛЛВыместиМенеджерПамМЦДжИТ(ЛЛМенеджерПамятиМЦДжИТ MM);
	    /*===-- JIT Event Listener functions -------------------------------------===*/
	 ЛЛИМПОРТ   ЛЛДатчикСобытийДжит ЛЛСоздайДатчикРегистрацииГДБ(проц);
	 ЛЛИМПОРТ    ЛЛДатчикСобытийДжит ЛЛСоздайДатчикДжИТСобытийИнтел(проц);
	 ЛЛИМПОРТ   ЛЛДатчикСобытийДжит ЛЛСоздайДатчикДжИТСобытийОПрофайл(проц);
	 ЛЛИМПОРТ  ЛЛДатчикСобытийДжит ЛЛСоздайДатчикДжИТСобытийПерф(проц);
/////////////////////////////////////////////////////////////////////////////////
//IRReader
///////////////////////////////////////////////////////////////////////////////
	ЛЛИМПОРТ ЛЛБул ЛЛПарсируйППВКонтексте(ЛЛКонтекст сКонтекст,
	                              ЛЛБуферПамяти буфПам, ЛЛМодуль *внМод,
	                              ткст0 *выхСооб);
/////////////////////////////////////////////////////////////////
//Linker
////////////////////////////////////////////////////////////
	ЛЛИМПОРТ ЛЛБул ЛЛКомпонуйМодули2(ЛЛМодуль приёмник, ЛЛМодуль Src);
///////////////////////////////////////////////////////////////////
//LTO
/////////////////////////////////////////////////////////////////////
#ifndef __cplusplus
#if !defined(_MSC_VER)
#include <stdbool.h>
typedef bool т_булОВК;
#else
/* MSVC in particular does not have anything like _Bool or bool in C, but we can
   at least make sure the type is the same size.  The implementation side will
   use C++ bool. */
typedef unsigned char т_булОВК;
#endif
#else
typedef bool т_булОВК;
#endif

/**
 * @defgroup LLVMCLTO LTO
 * @ingroup LLVMC
 *
 * @{
 */

#define LTO_API_VERSION 24

/**
 * \since prior to LTO_API_VERSION=3
 */
typedef enum {
    LTO_SYMBOL_ALIGNMENT_MASK              = 0x0000001F, /* log2 of alignment */
    LTO_SYMBOL_PERMISSIONS_MASK            = 0x000000E0,
    LTO_SYMBOL_PERMISSIONS_CODE            = 0x000000A0,
    LTO_SYMBOL_PERMISSIONS_DATA            = 0x000000C0,
    LTO_SYMBOL_PERMISSIONS_RODATA          = 0x00000080,
    LTO_SYMBOL_DEFINITION_MASK             = 0x00000700,
    LTO_SYMBOL_DEFINITION_REGULAR          = 0x00000100,
    LTO_SYMBOL_DEFINITION_TENTATIVE        = 0x00000200,
    LTO_SYMBOL_DEFINITION_WEAK             = 0x00000300,
    LTO_SYMBOL_DEFINITION_UNDEFINED        = 0x00000400,
    LTO_SYMBOL_DEFINITION_WEAKUNDEF        = 0x00000500,
    LTO_SYMBOL_SCOPE_MASK                  = 0x00003800,
    LTO_SYMBOL_SCOPE_INTERNAL              = 0x00000800,
    LTO_SYMBOL_SCOPE_HIDDEN                = 0x00001000,
    LTO_SYMBOL_SCOPE_PROTECTED             = 0x00002000,
    LTO_SYMBOL_SCOPE_DEFAULT               = 0x00001800,
    LTO_SYMBOL_SCOPE_DEFAULT_CAN_BE_HIDDEN = 0x00002800,
    LTO_SYMBOL_COMDAT                      = 0x00004000,
    LTO_SYMBOL_ALIAS                       = 0x00008000
} ПАтрыСимволаОВК;

/**
 * \since prior to LTO_API_VERSION=3
 */
typedef enum {
    LTO_DEBUG_MODEL_NONE         = 0,
    LTO_DEBUG_MODEL_DWARF        = 1
} ПМодельОтладкиОВК;

/**
 * \since prior to LTO_API_VERSION=3
 */
typedef enum {
    LTO_CODEGEN_PIC_MODEL_STATIC         = 0,
    LTO_CODEGEN_PIC_MODEL_DYNAMIC        = 1,
    LTO_CODEGEN_PIC_MODEL_DYNAMIC_NO_PIC = 2,
    LTO_CODEGEN_PIC_MODEL_DEFAULT        = 3
} ПМодельКодгенаОВК;

typedef enum {
  LTO_DS_ERROR = 0,
  LTO_DS_WARNING = 1,
  LTO_DS_REMARK = 3, // Added in LTO_API_VERSION=10.
  LTO_DS_NOTE = 2
} lto_codegen_diagnostic_severity_t;
/** opaque reference to a loaded object module */
typedef struct LLVMOpaqueLTOModule *ЛЛМодульОВК;

/** opaque reference to a code generator */
typedef struct LLVMOpaqueLTOCodeGenerator *ЛЛКодгенОВК;

/** opaque reference to a thin code generator */
typedef struct LLVMOpaqueThinLTOCodeGenerator *ЛЛКодгенТОВК;

/**
 * Diagnostic handler type.
 * \p severity defines the severity.
 * \p diag is the actual diagnostic.
 * The diagnostic is not prefixed by any of severity keyword, e.g., 'error: '.
 * \p ctxt is used to pass the context set with the diagnostic handler.
 *
 * \since LTO_API_VERSION=7
 */
typedef void (*lto_diagnostic_handler_t)(
    lto_codegen_diagnostic_severity_t severity, const char *diag, void *ctxt);

/**
 * Set a diagnostic handler and the related context (void *).
 * This is more general than lto_get_error_message, as the diagnostic handler
 * can be called at anytime within lto.
 *
 * \since LTO_API_VERSION=7
 */
extern void lto_codegen_set_diagnostic_handler(ЛЛКодгенОВК,
                                               lto_diagnostic_handler_t,
                                               void *);
                                               
typedef struct LLVMOpaqueLTOInput *lto_input_t;
///////////////////////////////////////////
    ЛЛИМПОРТ const ткст0 ЛЛОВК_ДайВерсию();
    ЛЛИМПОРТ const ткст0 ЛЛОВК_ДайОшСооб() ;
    ЛЛИМПОРТ bool ЛЛОВКМодуль_ФайлОбъект_ли(const ткст0 path) ;
    ЛЛИМПОРТ bool ЛЛОВКМодуль_ФайлОбъектДляЦели_ли(const ткст0 path,
                                           const ткст0 target_triplet_prefix);
    ЛЛИМПОРТ bool ЛЛОВКМодуль_ЕстьКатегорияОБджСи_ли(const ук mem, т_мера length) ;
    ЛЛИМПОРТ bool ЛЛОВКМодуль_ФайлОбъектВПамяти_ли(const ук mem, т_мера length) ;
    ЛЛИМПОРТ bool
        ЛЛОВКМодуль_ФайлОбъектВПамятиДляЦели_ли(const ук mem,
                                                      т_мера length,
                                                const ткст0 target_triplet_prefix);
    ЛЛИМПОРТ ЛЛМодульОВК ЛЛОВКМодуль_Создай(const ткст0 path) ;
    ЛЛИМПОРТ ЛЛМодульОВК ЛЛОВКМодуль_СоздайИзФД(цел fd, const ткст0 path, т_мера size) ;
    ЛЛИМПОРТ ЛЛМодульОВК ЛЛОВКМодуль_СоздайИзФДПоСмещению(цел fd, const ткст0 path,
                                                            т_мера file_size,
                                                                   т_мера map_size,
                                                                              off_t offset) ;
    ЛЛИМПОРТ ЛЛМодульОВК ЛЛОВКМодуль_СоздайИзПамяти(const ук mem, т_мера length);
    ЛЛИМПОРТ ЛЛМодульОВК ЛЛОВКМодуль_СоздайИзПамятиСПутём(const ук mem,
                                                        т_мера length,
                                                                  const ткст0 path);
    ЛЛИМПОРТ ЛЛМодульОВК ЛЛОВКМодуль_СоздайВЛокКонтексте(const ук mem, т_мера length,
                                                                          const ткст0 path) ;
    ЛЛИМПОРТ ЛЛМодульОВК ЛЛОВКМодуль_СоздайВКонтекстеКодГена(const ук mem,
                                                      т_мера length,
                                                           const ткст0 path,
                                                                 ЛЛКодгенОВК cg) ;
    ЛЛИМПОРТ проц ЛЛОВКМодуль_Вымести(ЛЛМодульОВК mod) ;
    ЛЛИМПОРТ const ткст0 ЛЛОВКМодуль_ДайТриадуЦели(ЛЛМодульОВК mod) ;
    ЛЛИМПОРТ проц ЛЛОВКМодуль_УстТриадуЦели(ЛЛМодульОВК mod, const ткст0 триада);
    ЛЛИМПОРТ бцел ЛЛОВКМодуль_ДайЧлоСимволов(ЛЛМодульОВК mod) ;
    ЛЛИМПОРТ const ткст0 ЛЛОВКМодуль_ДайИмяСимвола(ЛЛМодульОВК mod, бцел индекс) ;
    ЛЛИМПОРТ ПАтрыСимволаОВК ЛЛОВКМодуль_ДайАтрибутыСимвола(ЛЛМодульОВК mod,
                                                                    бцел индекс) ;
    ЛЛИМПОРТ const ткст0 ЛЛОВКМодуль_ДайОпцииКомпоновщика(ЛЛМодульОВК mod) ;
    ЛЛИМПОРТ проц ЛЛОВККодГен_УстОбработчикДиагностики(ЛЛКодгенОВК cg,
                                          lto_diagnostic_handler_t diag_handler,
                                                                          ук ctxt);
    ЛЛИМПОРТ ЛЛКодгенОВК ЛЛОВККодГен_Создай(проц) ;
    ЛЛИМПОРТ ЛЛКодгенОВК ЛЛОВККодГен_СоздайВЛокКонтексте(проц);
    ЛЛИМПОРТ проц ЛЛОВККодГен_Вымести(ЛЛКодгенОВК cg);
    ЛЛИМПОРТ bool ЛЛОВККодГен_ДобавьМодуль(ЛЛКодгенОВК cg, ЛЛМодульОВК mod) ;
    ЛЛИМПОРТ проц ЛЛОВККодГен_УстМодуль(ЛЛКодгенОВК cg, ЛЛМодульОВК mod) ;
    ЛЛИМПОРТ bool ЛЛОВККодГен_УстМодельОтладки(ЛЛКодгенОВК cg, ПМодельОтладкиОВК debug);
    ЛЛИМПОРТ bool ЛЛОВККодГен_УстМодельПИК(ЛЛКодгенОВК cg, ПМодельКодгенаОВК model) ;
    ЛЛИМПОРТ проц ЛЛОВККодГен_УстЦПБ(ЛЛКодгенОВК cg, const ткст0 cpu) ;
    ЛЛИМПОРТ проц ЛЛОВККодГен_ДобавьСимволМастПрезерв(ЛЛКодгенОВК cg,
                                                          const ткст0 symbol) ;
    ЛЛИМПОРТ bool ЛЛОВККодГен_ПишиСлитноМодуль(ЛЛКодгенОВК cg, const ткст0 path);
    ЛЛИМПОРТ const ук ЛЛОВККодГен_Компилируй(ЛЛКодгенОВК cg, т_мера* length) ;
    ЛЛИМПОРТ bool ЛЛОВККодГен_Оптимизируй(ЛЛКодгенОВК cg) ;
    ЛЛИМПОРТ const ук ЛЛОВККодГен_КомпилируйОптимиз(ЛЛКодгенОВК cg, т_мера* length);
    ЛЛИМПОРТ bool ЛЛОВККодГен_КомпилируйВФайл(ЛЛКодгенОВК cg, const ткст0 * name) ;
    ЛЛИМПОРТ проц ЛЛОВККодГен_ОпцииОтладки(ЛЛКодгенОВК cg, const ткст0 opt) ;
    ЛЛИМПОРТ бцел ЛЛОВКАПИВерсия();
    ЛЛИМПОРТ проц ЛЛОВККодГен_УстСледуетИнтернализовать(ЛЛКодгенОВК cg,
                                                          bool ShouldInternalize);
    ЛЛИМПОРТ проц ЛЛОВККодГен_УстСледуетВнедритьСписокИспользований(ЛЛКодгенОВК cg,
                                                           т_булОВК ShouldEmbedUselists) ;
    ЛЛИМПОРТ ЛЛКодгенТОВК ЛЛОВК2_СоздайКодГен(проц) ;
    ЛЛИМПОРТ проц ЛЛОВК2_ВыместиКодГен(ЛЛКодгенТОВК cg) ;
    ЛЛИМПОРТ проц ЛЛОВК2_ДобавьМодуль(ЛЛКодгенТОВК cg, const ткст0 Identifier,
                                                          const ткст0 данн, цел длина);
    ЛЛИМПОРТ проц ЛЛОВК2КодГен_Обработай(ЛЛКодгенТОВК cg);
    ЛЛИМПОРТ бцел ЛЛОВК2Модуль_ДайЧлоОбъектов(ЛЛКодгенТОВК cg);
    ЛЛБуферОбъектаОВК ЛЛОВК2Модуль_ДайОбъект(ЛЛКодгенТОВК cg,
                                                       бцел индекс);
    ЛЛИМПОРТ бцел ЛЛОВК2Модуль_ДайЧлоОбъектФайлов(ЛЛКодгенТОВК cg);
    ЛЛИМПОРТ const ткст0 ЛЛОВК2Модуль_ДайОбъектФайл(ЛЛКодгенТОВК cg,
                                                             бцел индекс) ;
    ЛЛИМПОРТ проц ЛЛОВК2КодГен_ОтключиКодГен(ЛЛКодгенТОВК cg,
                                                      т_булОВК disable) ;
    ЛЛИМПОРТ проц ЛЛОВК2КодГен_УстТолькоКодГен(ЛЛКодгенТОВК cg,
                                                   т_булОВК CodeGenOnly);
    ЛЛИМПОРТ проц ЛЛОВК2_ОпцииОтладки(const ткст0 *options, цел number);
    ЛЛИМПОРТ т_булОВК ЛЛОВКМодуль_ОВК2_ли(ЛЛМодульОВК mod) ;
    ЛЛИМПОРТ проц ЛЛОВК2КодГен_ДобавьСимволМастПрезерв(ЛЛКодгенТОВК cg,
                                                   const ткст0 имя, цел длина) ;
    ЛЛИМПОРТ проц ЛЛОВК2КодГен_ДобавьКроссРефСимвол(ЛЛКодгенТОВК cg,
                                                 const ткст0 имя, цел длина) ;
    ЛЛИМПОРТ проц ЛЛОВК2КодГен_УстЦПБ(ЛЛКодгенТОВК cg, const ткст0 cpu) ;
    ЛЛИМПОРТ проц ЛЛОВК2КодГен_УстПапкуКэша(ЛЛКодгенТОВК cg,
                                                    const ткст0 cache_dir);
    ЛЛИМПОРТ проц ЛЛОВК2КодГен_УстИнтервалПрюнингаКэша(ЛЛКодгенТОВК cg,
                                                                  цел interval) ;
    ЛЛИМПОРТ проц ЛЛОВК2КодГен_УстЭкспирациюЗаписиКэша(ЛЛКодгенТОВК cg,
                                                            бцел expiration);
    ЛЛИМПОРТ проц ЛЛОВК2КодГен_УстФинальнРазКэшаОтносительноДоступнПрострву(
                                       ЛЛКодгенТОВК cg, бцел Percentage);
    ЛЛИМПОРТ проц ЛЛОВК2КодГен_УстРазмКэшаВБайтах(
                                     ЛЛКодгенТОВК cg, бцел MaxSizeBytes) ;
    ЛЛИМПОРТ проц ЛЛОВК2КодГен_УстРазмКэшаВМегаБайтах(
        ЛЛКодгенТОВК cg, бцел MaxSizeMegabytes) ;
    ЛЛИМПОРТ проц ЛЛОВК2КодГен_УстРазмКэшаВФайлах(
                                  ЛЛКодгенТОВК cg, бцел MaxSizeFiles) ;
    ЛЛИМПОРТ проц ЛЛОВК2КодГен_УстПапкуВремХран(ЛЛКодгенТОВК cg,
                                               const ткст0 save_temps_dir) ;
    ЛЛИМПОРТ проц ЛЛОВК2КодГен_УстПапкуСгенОбъектов(ЛЛКодгенТОВК cg,
                                                       const ткст0 save_temps_dir);
    ЛЛИМПОРТ т_булОВК ЛЛОВК2КодГен_УстМодельПИК(ЛЛКодгенТОВК cg,
                                                     ПМодельКодгенаОВК model);

    ЛЛИМПОРТ lto_input_t ЛЛОВКВвод_Создай(const ук buffer, т_мера buffer_size,
                                                                    const ткст0 path);
    ЛЛИМПОРТ проц ЛЛОВКВвод_Вымести(lto_input_t input) ;
    ЛЛИМПОРТ  бцел ЛЛОВКВвод_ДайЧлоЗависимыхБиб(lto_input_t input) ;
    ЛЛИМПОРТ  const ткст0 ЛЛОВКВвод_ДайЗависимБиб(lto_input_t input,
                                                       т_мера индекс,
                                                              т_мера* size);
////////////////////////////////////////////////////////////////////////////////////
//LTODisassembler
///////////////////////////////////////////////////
	ЛЛИМПОРТ проц ЛЛОВК_ИницДизасм();
//////////////////////////////////////////////////
//Object
typedef enum {
  LLVMBinaryTypeArchive,                /**< Archive file. */
  LLVMBinaryTypeMachOUniversalBinary,   /**< Mach-O Universal Binary file. */
  LLVMBinaryTypeCOFFImportFile,         /**< COFF Import file. */
  LLVMBinaryTypeIR,                     /**< LLVM IR. */
  LLVMBinaryTypeWinRes,                 /**< Windows resource (.res) file. */
  LLVMBinaryTypeCOFF,                   /**< COFF Object file. */
  LLVMBinaryTypeELF32L,                 /**< ELF 32-bit, little endian. */
  LLVMBinaryTypeELF32B,                 /**< ELF 32-bit, big endian. */
  LLVMBinaryTypeELF64L,                 /**< ELF 64-bit, little endian. */
  LLVMBinaryTypeELF64B,                 /**< ELF 64-bit, big endian. */
  LLVMBinaryTypeMachO32L,               /**< MachO 32-bit, little endian. */
  LLVMBinaryTypeMachO32B,               /**< MachO 32-bit, big endian. */
  LLVMBinaryTypeMachO64L,               /**< MachO 64-bit, little endian. */
  LLVMBinaryTypeMachO64B,               /**< MachO 64-bit, big endian. */
  LLVMBinaryTypeWasm,                   /**< Web Assembly. */
} LLVMBinaryType;
/////////////////////////////////////////////////
	ЛЛИМПОРТ ЛЛБинарник ЛЛСоздайБин(ЛЛБуферПамяти буфПам,
	                               ЛЛКонтекст Context,
	                               ткст0 *ошсооб);
	ЛЛИМПОРТ проц ЛЛВыместиБин(ЛЛБинарник BR);
	ЛЛИМПОРТ ЛЛБуферПамяти ЛЛБИинКопируйБуфПам(ЛЛБинарник BR);
	ЛЛИМПОРТ LLVMBinaryType ЛЛБинДайТип(ЛЛБинарник BR);
	ЛЛИМПОРТ ЛЛБинарник ЛЛМакхО_УнивБин_КопируйОбъДляАрх(ЛЛБинарник BR,
	                                                        const ткст0 Arch,
	                                                        т_мера ArchLen,
	                                                        ткст0 *ошсооб);
	ЛЛИМПОРТ ЛЛИтераторСекций ЛЛОбъФайл_КопируйИтераторВыборки(ЛЛБинарник BR);
	ЛЛИМПОРТ ЛЛБул ЛЛОбъФайл_ИтераторВыборкиВКонце_ли(ЛЛБинарник BR,
	                                              ЛЛИтераторСекций SI);
	ЛЛИМПОРТ ЛЛСимвИтератор ЛЛОбъФайл_КопируйСимвИтератор(ЛЛБинарник BR);
	ЛЛИМПОРТ ЛЛБул ЛЛОбъФайл_СимвИтераторВКонце_ли(ЛЛБинарник BR,
	                                             ЛЛСимвИтератор SI);
	ЛЛИМПОРТ проц ЛЛВыместиИтераторСекций(ЛЛИтераторСекций SI);
	ЛЛИМПОРТ проц ЛЛПереместисьКСледщСекции(ЛЛИтераторСекций SI);
	ЛЛИМПОРТ проц ЛЛПерместисьКСодержащСекции(ЛЛИтераторСекций Sect,
	                                                ЛЛСимвИтератор Sym);
	// ObjectFile Symbol iterators
	ЛЛИМПОРТ проц ЛЛВыместиСимвИтератор(ЛЛСимвИтератор SI);
	ЛЛИМПОРТ проц ЛЛПереместисьКСледщСимволу(ЛЛСимвИтератор SI);
	// SectionRef accessors
	ЛЛИМПОРТ const ткст0 ЛЛДайИмяСекции(ЛЛИтераторСекций SI);
	ЛЛИМПОРТ бдол ЛЛДАйРазмСекции(ЛЛИтераторСекций SI);
	ЛЛИМПОРТ const ткст0 ЛЛДайСодержимоеСекции(ЛЛИтераторСекций SI);
	ЛЛИМПОРТ бдол ЛЛДайАдресСекции(ЛЛИтераторСекций SI);
	ЛЛИМПОРТ ЛЛБул ЛЛСодержитСекцияСимвол_ли(ЛЛИтераторСекций SI,
	                                             ЛЛСимвИтератор Sym);
	// Section Relocation iterators
	ЛЛИМПОРТ ЛЛИтераторРелокаций ЛЛДайРелокации(ЛЛИтераторСекций Section);
	ЛЛИМПОРТ проц ЛЛВыместиИтераторРелокаций(ЛЛИтераторРелокаций RI);
	ЛЛИМПОРТ ЛЛБул ЛЛИтераторРелокацийВКонце_ли(ЛЛИтераторСекций Section,
	                                                  ЛЛИтераторРелокаций RI);
	ЛЛИМПОРТ проц ЛЛПереместисьКСледщРелокации(ЛЛИтераторРелокаций RI);
	// SymbolRef accessors
	ЛЛИМПОРТ const ткст0 ЛЛДайИмяСимвола(ЛЛСимвИтератор SI);
	ЛЛИМПОРТ бдол ЛЛДайАдресСимвола(ЛЛСимвИтератор SI);
	ЛЛИМПОРТ бдол ЛЛДайРазмСимвола(ЛЛСимвИтератор SI);
	// RelocationRef accessors
	ЛЛИМПОРТ бдол ЛЛДайСмещениеРелокации(ЛЛИтераторРелокаций RI);
	ЛЛИМПОРТ ЛЛСимвИтератор ЛЛДайСимволРелокации(ЛЛИтераторРелокаций RI);
	ЛЛИМПОРТ бдол ЛЛДайТипРелокации(ЛЛИтераторРелокаций RI);
	// NOTE: Caller takes ownership of returned string of the two
	// following functions.
	ЛЛИМПОРТ const ткст0 ЛЛДайИмяТипаРелокации(ЛЛИтераторРелокаций RI);
	ЛЛИМПОРТ const ткст0 ЛЛДайТкстЗначенияРелокации(ЛЛИтераторРелокаций RI);
	/** Deprecated: Use LLVMCreateBinary instead. */
	ЛЛИМПОРТ ЛЛФайлОбъекта ЛЛСоздайФайлОбъекта(ЛЛБуферПамяти буфПам);
	/** Deprecated: Use LLVMDisposeBinary instead. */
	ЛЛИМПОРТ проц ЛЛВыместиФайлОбъекта(ЛЛФайлОбъекта ObjectFile);
	/** Deprecated: Use LLVMObjectFileCopySectionIterator instead. */
	ЛЛИМПОРТ ЛЛИтераторСекций ЛЛДайСекции(ЛЛФайлОбъекта ObjectFile);
	/** Deprecated: Use LLVMObjectFileIsSectionIteratorAtEnd instead. */
	ЛЛИМПОРТ ЛЛБул ЛЛИтераторСекцииВКонце_ли(ЛЛФайлОбъекта ObjectFile,
	                                                   ЛЛИтераторСекций SI);
	/** Deprecated: Use LLVMObjectFileCopySymbolIterator instead. */
	ЛЛИМПОРТ ЛЛСимвИтератор ЛЛДайСимволы(ЛЛФайлОбъекта ObjectFile);
	/** Deprecated: Use LLVMObjectFileIsSymbolIteratorAtEnd instead. */
	ЛЛИМПОРТ ЛЛБул ЛЛСимвИтераторВКонце_ли(ЛЛФайлОбъекта ObjectFile,
	                                                   ЛЛСимвИтератор SI);
	/**
	 * @}
	 */
/////////////////////////////////////////////////////////////////////////
///OrcBindings
///////////////////////////////////////////////////////////////////////
	ЛЛИМПОРТ LLVMOrcJITStackRef ЛЛОрк_СоздайЭкземпляр(ЛЛЦелеваяМашина TM);
	ЛЛИМПОРТ const ткст0 ЛЛОрк_ДайОшСооб(LLVMOrcJITStackRef JITStack);
	ЛЛИМПОРТ проц ЛЛОрк_ДайДекорирСимвол(LLVMOrcJITStackRef JITStack, ткст0 *MangledSymbol,
	                                                                    const ткст0 Symbol);
	ЛЛИМПОРТ проц ЛЛОрк_ВыместиДекорирСимвол(ткст0 MangledSymbol);
	ЛЛИМПОРТ ЛЛОшибка ЛЛОрк_ОбрВызЛенивКомпиляции(
	    LLVMOrcJITStackRef JITStack, LLVMOrcTargetAddress *RetAddr,
	    LLVMOrcLazyCompileCallbackFn обрвыз, проц *CallbackCtx);
	ЛЛИМПОРТ ЛЛОшибка ЛЛОрк_СоздайНепрямСтаб(LLVMOrcJITStackRef JITStack,
	                                       const ткст0 StubName,
	                                       LLVMOrcTargetAddress InitAddr);
	ЛЛИМПОРТ ЛЛОшибка ЛЛОрк_УстУкзНаНепрямСтаб(LLVMOrcJITStackRef JITStack,
	                                           const ткст0 StubName,
	                                           LLVMOrcTargetAddress NewAddr);
	ЛЛИМПОРТ ЛЛОшибка ЛЛОрк_ДобавьАктивноКомпилирПП(LLVMOrcJITStackRef JITStack,
	                                         LLVMOrcModuleHandle *RetHandle,
	                                         ЛЛМодуль мод,
	                                         LLVMOrcSymbolResolverFn SymbolResolver,
	                                         проц *SymbolResolverCtx);
	ЛЛИМПОРТ ЛЛОшибка ЛЛОрк_ДобавьЛенивоКомпилирПП(LLVMOrcJITStackRef JITStack,
	                                        LLVMOrcModuleHandle *RetHandle,
	                                        ЛЛМодуль мод,
	                                        LLVMOrcSymbolResolverFn SymbolResolver,
	                                        проц *SymbolResolverCtx);
	ЛЛИМПОРТ ЛЛОшибка ЛЛОрк_ДобавьФайлОбъекта(LLVMOrcJITStackRef JITStack,
	                                  LLVMOrcModuleHandle *RetHandle,
	                                  ЛЛБуферПамяти Obj,
	                                  LLVMOrcSymbolResolverFn SymbolResolver,
	                                  проц *SymbolResolverCtx);
	ЛЛИМПОРТ ЛЛОшибка ЛЛОрк_УдалиМодуль(LLVMOrcJITStackRef JITStack,
	                                 LLVMOrcModuleHandle H);
	ЛЛИМПОРТ ЛЛОшибка ЛЛОрк_ДайАдресСимвола(LLVMOrcJITStackRef JITStack,
	                                     LLVMOrcTargetAddress *RetAddr,
	                                     const ткст0 SymbolName);
	ЛЛИМПОРТ ЛЛОшибка ЛЛОрк_ДайАдресСимволаЭкз(LLVMOrcJITStackRef JITStack,
	                                       LLVMOrcTargetAddress *RetAddr,
	                                       LLVMOrcModuleHandle H,
	                                       const ткст0 SymbolName);
	ЛЛИМПОРТ ЛЛОшибка ЛЛОрк_ВыместиЭкземпляр(LLVMOrcJITStackRef JITStack);
	ЛЛИМПОРТ проц ЛЛОрк_ЗарегистрируйДатчикСобытийДжИТ(LLVMOrcJITStackRef JITStack,
	                                             ЛЛДатчикСобытийДжит L);
	ЛЛИМПОРТ проц ЛЛОрк_ОтрегистрируйДатчикСобытийДжИТ(LLVMOrcJITStackRef JITStack,
	                                           ЛЛДатчикСобытийДжит L);
////////////////////////////////////////////////////////////////////////////////
//Ремарки
/**
 * The type of the emitted remark.
 */
enum LLVMRemarkType {
  LLVMRemarkTypeUnknown,
  LLVMRemarkTypePassed,
  LLVMRemarkTypeMissed,
  LLVMRemarkTypeAnalysis,
  LLVMRemarkTypeAnalysisFPCommute,
  LLVMRemarkTypeAnalysisAliasing,
  LLVMRemarkTypeFailure
};

////////////////////////////////////////////////////////////////////////////
	ЛЛИМПОРТ const ткст0 ЛЛТкстРемарки_ДайДанные(ЛЛТкстРемарки String);
	ЛЛИМПОРТ бцел ЛЛТкстРемарки_ДайДлину(ЛЛТкстРемарки String);
	ЛЛИМПОРТ ЛЛТкстРемарки
	ЛЛОтладЛокРемарки_ДайПутьКИсходнику(ЛЛОтладЛокРемарки DL);
	ЛЛИМПОРТ бцел ЛЛОтладЛокРемарки_ДайСтрокуИсходника(ЛЛОтладЛокРемарки DL);
	ЛЛИМПОРТ бцел ЛЛОтладЛокРемарки_ДайСтолбецИсходника(ЛЛОтладЛокРемарки DL);
	ЛЛИМПОРТ ЛЛТкстРемарки ЛЛАргРемарки_ДайКлюч(ЛЛАргРемарки арг);
	ЛЛИМПОРТ ЛЛТкстРемарки ЛЛАргРемарки_ДайЗначение(ЛЛАргРемарки арг);
	ЛЛИМПОРТ ЛЛОтладЛокРемарки ЛЛАргРемарки_ДайОтладЛок(ЛЛАргРемарки арг);
	ЛЛИМПОРТ проц ЛЛЗаписьРемарки_Вымести(ЛЛЗаписьРемарки ремарка);
	ЛЛИМПОРТ LLVMRemarkType ЛЛЗаписьРемарки_ДайТип(ЛЛЗаписьРемарки ремарка);
	ЛЛИМПОРТ ЛЛТкстРемарки
	ЛЛЗаписьРемарки_ДайИмяПроходки(ЛЛЗаписьРемарки ремарка);
	ЛЛИМПОРТ ЛЛТкстРемарки
	ЛЛЗаписьРемарки_ДайИмяРемарки(ЛЛЗаписьРемарки ремарка);
	ЛЛИМПОРТ ЛЛТкстРемарки
	ЛЛЗаписьРемарки_ДайИмяФункции(ЛЛЗаписьРемарки ремарка);
	ЛЛИМПОРТ ЛЛОтладЛокРемарки
	ЛЛЗаписьРемарки_ДайОтладЛок(ЛЛЗаписьРемарки ремарка);
	ЛЛИМПОРТ бдол ЛЛЗаписьРемарки_ДайАктуальность(ЛЛЗаписьРемарки ремарка);
	ЛЛИМПОРТ бцел ЛЛЗаписьРемарки_ДайЧлоАргов(ЛЛЗаписьРемарки ремарка);
	ЛЛИМПОРТ ЛЛАргРемарки ЛЛЗаписьРемарки_ДайПервАрг(ЛЛЗаписьРемарки ремарка);
	ЛЛИМПОРТ ЛЛАргРемарки ЛЛЗаписьРемарки_ДайСледщАрг(ЛЛАргРемарки It,
	                                                  ЛЛЗаписьРемарки ремарка);
	ЛЛИМПОРТ ЛЛПарсерРемарок ЛЛПарсерРемарок_СоздайЙАМЛ(const проц *буф,
	                                                      бдол разм);
	ЛЛИМПОРТ ЛЛЗаписьРемарки ЛЛПарсерРемарок_ДайСледщ(ЛЛПарсерРемарок парсер);
	ЛЛИМПОРТ ЛЛБул ЛЛПарсерРемарок_ЕстьОш_ли(ЛЛПарсерРемарок парсер);
	ЛЛИМПОРТ const ткст0 ЛЛПарсерРемарок_ДайОшСооб(ЛЛПарсерРемарок парсер);
	ЛЛИМПОРТ проц ЛЛПарсерРемарок_Вымести(ЛЛПарсерРемарок парсер);
///////////////////////////////////////////////////////////////////////
//Support
///////////////////////////////////////////////////
	ЛЛИМПОРТ ЛЛБул ЛЛГрузиБибПерманентно(const ткст0 имяф);
	ЛЛИМПОРТ проц ЛЛРазбериОпцКомСтроки(цел argc, const ткст0 *argv,
	                                                  const ткст0 Overview);
	ЛЛИМПОРТ проц *ЛЛНайдиАдресСимвола(const ткст0 имяСимв);
	ЛЛИМПОРТ проц ЛЛДобавьСимвол(const ткст0 имяСимв, проц *значСимв);
//////////////////////////////////////////////////////
//Target
///////////////////////////////////////////////////////
	ЛЛИМПОРТ проц ЛЛНициализуйВсеИнфОЦели(проц) ;
	ЛЛИМПОРТ проц ЛЛНициализуйВсеЦели(проц);
	ЛЛИМПОРТ проц ЛЛНициализуйВсеЦелевыеМК(проц);
	ЛЛИМПОРТ проц ЛЛНициализуйВсеАсмПринтеры(проц) ;
	ЛЛИМПОРТ проц ЛЛНициализуйВсеАсмПарсеры(проц);
	ЛЛИМПОРТ проц ЛЛНициализуйВсеДизассемблеры(проц) ;
	ЛЛИМПОРТ ЛЛБул ЛЛИнициализуйНативныйТаргет(проц);
	ЛЛИМПОРТ ЛЛБул ЛЛИнициализуйНативныйАсмПарсер(проц) ;
	ЛЛИМПОРТ ЛЛБул ЛЛИнициализуйНативныйАсмПринтер(проц) ;
	ЛЛИМПОРТ ЛЛБул ЛЛИнициализуйНативныйДизассемблер(проц);
	/*===-- Target данн -------------------------------------------------------===*/
	ЛЛИМПОРТ ЛЛДанныеОЦели ЛЛДайРаскладкуДанныхМодуля(ЛЛМодуль м) ;
	ЛЛИМПОРТ проц ЛЛУстРаскладкуДанныхМодуля(ЛЛМодуль м, ЛЛДанныеОЦели DL);
	ЛЛИМПОРТ ЛЛДанныеОЦели ЛЛСоздайДанОЦели(const ткст0 StringRep);
	ЛЛИМПОРТ проц ЛЛВыместиДанОЦели(ЛЛДанныеОЦели TD) ;
	ЛЛИМПОРТ проц ЛЛДобавьИнфОЦБиб(ЛЛИнфоЦелевойБиблиотеки TLI,
	                              ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ ткст0 ЛЛКопируйТкстПредстДанОЦели(ЛЛДанныеОЦели TD);
	ЛЛИМПОРТ enum ППорядокБайт ЛЛПорядокБайт(ЛЛДанныеОЦели TD) ;
	ЛЛИМПОРТ бцел ЛЛРазмУкз(ЛЛДанныеОЦели TD);
	ЛЛИМПОРТ бцел ЛЛРазмУкзДляАП(ЛЛДанныеОЦели TD, бцел AS) ;
	ЛЛИМПОРТ ЛЛТип ЛЛТипЦелУкз(ЛЛДанныеОЦели  TD) ;
	ЛЛИМПОРТ ЛЛТип ЛЛТипЦелУкзДляАП(ЛЛДанныеОЦели  TD, бцел AS) ;
	ЛЛИМПОРТ ЛЛТип ЛЛТипЦелУкзВКонтексте(ЛЛКонтекст к, ЛЛДанныеОЦели TD) ;
	ЛЛТип ЛЛТипЦелУкзДляАПВКонтексте(ЛЛКонтекст к, ЛЛДанныеОЦели TD,
	                                         бцел AS);
	ЛЛИМПОРТ бдол ЛЛРазмТипаВБитах(ЛЛДанныеОЦели  TD, ЛЛТип тип);
	ЛЛИМПОРТ бдол ЛЛРазмХранТипа(ЛЛДанныеОЦели TD, ЛЛТип тип);
	ЛЛИМПОРТ бдол ЛЛДИПРазмТипа(ЛЛДанныеОЦели TD, ЛЛТип тип);
	ЛЛИМПОРТ бцел ЛЛДИПРаскладкаТипа(ЛЛДанныеОЦели TD, ЛЛТип тип) ;
	ЛЛИМПОРТ бцел ЛЛРаскладкаФреймаВызДляТипа(ЛЛДанныеОЦели TD, ЛЛТип тип);
	ЛЛИМПОРТ бцел ЛЛПредпочтРаскладкаТипа(ЛЛДанныеОЦели TD,ЛЛТип тип);
	ЛЛИМПОРТ бцел ЛЛПредпочтРаскладкаГлоба(ЛЛДанныеОЦели  TD,
	                                        ЛЛЗначение глобПерем) ;
	ЛЛИМПОРТ бцел ЛЛЭлтПоСмещ(ЛЛДанныеОЦели TD, ЛЛТип типСтрукт,
	                             бдол Offset) ;
	ЛЛИМПОРТ бдол ЛЛСмещЭлта(ЛЛДанныеОЦели TD,
	                                       ЛЛТип типСтрукт, бцел Element) ;
	/**
	 * @}
	 */
	ЛЛИМПОРТ проц LLInitializeAArch64Target();
	ЛЛИМПОРТ проц LLInitializeAArch64TargetInfo();
	ЛЛИМПОРТ проц LLInitializeAArch64TargetMC() ;
	ЛЛИМПОРТ проц LLInitializeAArch64AsmPrinter() ;
	ЛЛИМПОРТ проц LLInitializeAArch64Asmпарсер();
	ЛЛИМПОРТ проц LLInitializeAArch64Disassembler();

	ЛЛИМПОРТ проц LLInitializeARMTarget() ;
	ЛЛИМПОРТ проц LLInitializeARMTargetInfo();
	ЛЛИМПОРТ проц LLInitializeARMTargetMC();
	ЛЛИМПОРТ проц LLInitializeARMAsmPrinter() ;
	ЛЛИМПОРТ проц LLInitializeARMAsmпарсер();
	ЛЛИМПОРТ проц LLInitializeARMDisassembler() ;

	ЛЛИМПОРТ проц LLInitializeHexagonTarget() ;
	ЛЛИМПОРТ проц LLInitializeHexagonTargetInfo() ;
	ЛЛИМПОРТ проц LLInitializeHexagonTargetMC() ;
	ЛЛИМПОРТ проц LLInitializeHexagonAsmPrinter() ;

	ЛЛИМПОРТ проц LLInitializeMSP430Target();
	ЛЛИМПОРТ проц LLInitializeMSP430TargetInfo() ;
	ЛЛИМПОРТ проц LLInitializeMSP430TargetMC() ;
	ЛЛИМПОРТ проц LLInitializeMSP430AsmPrinter() ;

	ЛЛИМПОРТ проц LLInitializeNVPTXTarget() ;
	ЛЛИМПОРТ проц LLInitializeNVPTXTargetInfo() ;
	ЛЛИМПОРТ проц LLInitializeNVPTXTargetMC() ;
	ЛЛИМПОРТ проц LLInitializeNVPTXAsmPrinter() ;

	ЛЛИМПОРТ проц LLInitializePowerPCTarget() ;
	ЛЛИМПОРТ проц LLInitializePowerPCTargetInfo() ;
	ЛЛИМПОРТ проц LLInitializePowerPCTargetMC() ;
	ЛЛИМПОРТ проц LLInitializePowerPCAsmPrinter() ;
	ЛЛИМПОРТ проц LLInitializePowerPCAsmпарсер() ;
	ЛЛИМПОРТ проц LLInitializePowerPCDisassembler() ;

	ЛЛИМПОРТ проц LLInitializeSystemZTarget();
	ЛЛИМПОРТ проц LLInitializeSystemZTargetInfo() ;
	ЛЛИМПОРТ проц LLInitializeSystemZTargetMC() ;
	ЛЛИМПОРТ проц LLInitializeSystemZAsmPrinter() ;
	ЛЛИМПОРТ проц LLInitializeSystemZAsmпарсер() ;
	ЛЛИМПОРТ проц LLInitializeSystemZDisassembler() ;

	ЛЛИМПОРТ проц LLInitializeX86Target() ;
	ЛЛИМПОРТ проц LLInitializeX86TargetInfo() ;
	ЛЛИМПОРТ проц LLInitializeX86TargetMC() ;
	ЛЛИМПОРТ проц LLInitializeX86AsmPrinter() ;
	ЛЛИМПОРТ проц LLInitializeX86Asmпарсер() ;
	ЛЛИМПОРТ проц LLInitializeX86Disassembler() ;

	ЛЛИМПОРТ проц LLInitializeXCoreTarget() ;
	ЛЛИМПОРТ проц LLInitializeXCoreTargetInfo() ;
	ЛЛИМПОРТ проц LLInitializeXCoreTargetMC() ;
	ЛЛИМПОРТ проц LLInitializeXCoreAsmPrinter() ;
	ЛЛИМПОРТ проц LLInitializeXCoreDisassembler() ;
////////////////////////////////////////////////////////////
//Целевая машина
typedef enum {
    LLVMCodeGenLevelNone,
    LLVMCodeGenLevelLess,
    LLVMCodeGenLevelDefault,
    LLVMCodeGenLevelAggressive
} LLVMCodeGenOptLevel;

typedef enum {
    LLVMRelocDefault,
    LLVMRelocStatic,
    LLVMRelocPIC,
    LLVMRelocDynamicNoPic,
    LLVMRelocROPI,
    LLVMRelocRWPI,
    LLVMRelocROPI_RWPI
} LLVMRelocMode;

typedef enum {
    LLVMCodeModelDefault,
    LLVMCodeModelJITDefault,
    LLVMCodeModelTiny,
    LLVMCodeModelSmall,
    LLVMCodeModelKernel,
    LLVMCodeModelMedium,
    LLVMCodeModelLarge
} LLVMCodeModel;

typedef enum {
    LLVMAssemblyFile,
    LLVMObjectFile
} LLVMCodeGenFileType;
//////////////////////////////////////////////////////////
	ЛЛИМПОРТ ЛЛЦель ЛЛДайПервЦель(проц);
	ЛЛИМПОРТ ЛЛЦель ЛЛДайСледщЦель(ЛЛЦель T);

	/*===-- Target ------------------------------------------------------------===*/
	ЛЛИМПОРТ ЛЛЦель ЛЛДайЦельИзИмени(const ткст0 имя);
	ЛЛИМПОРТ ЛЛБул ЛЛДайЦельИзТриады(const ткст0 триада, ЛЛЦель *T,
	                                                   ткст0 *ошсооб);
	ЛЛИМПОРТ const ткст0 ЛЛДайИмяЦели(ЛЛЦель T);
	ЛЛИМПОРТ const ткст0 ЛЛДайОписаниеЦели(ЛЛЦель T);
	ЛЛИМПОРТ ЛЛБул ЛЛЦель_ЕстьДжИТ_ли(ЛЛЦель T);
	ЛЛИМПОРТ ЛЛБул ЛЛЦель_ЕстьЦелМаш_ли(ЛЛЦель T);
	ЛЛИМПОРТ ЛЛБул ЛЛЦель_ЕстьАсмБэкэнд_ли(ЛЛЦель T);
	/*===-- Target Machine ----------------------------------------------------===*/
	ЛЛИМПОРТ ЛЛЦелеваяМашина ЛЛСоздайЦелМаш(ЛЛЦель T,
	        const ткст0 триада, const ткст0 CPU, const ткст0 Features,
	            LLVMCodeGenOptLevel Level, LLVMRelocMode Reloc, LLVMCodeModel CodeModel);
	ЛЛИМПОРТ проц ЛЛВыместиЦелМаш(ЛЛЦелеваяМашина T);
	ЛЛИМПОРТ ЛЛЦель ЛЛДайЦельЦелМаш(ЛЛЦелеваяМашина T);
	ЛЛИМПОРТ ткст0 ЛЛДайТриадуЦелМаш(ЛЛЦелеваяМашина T);
	ЛЛИМПОРТ ткст0 ЛЛДайЦПБЦелМаш(ЛЛЦелеваяМашина T);
	ЛЛИМПОРТ ткст0 ЛЛДайТкстФичЦелМаш(ЛЛЦелеваяМашина T);
	ЛЛИМПОРТ ЛЛДанныеОЦели ЛЛСоздайРаскладкуДанЦели(ЛЛЦелеваяМашина T);
	ЛЛИМПОРТ проц ЛЛУстЦелМашАсмВербозность(ЛЛЦелеваяМашина T,
	                                          ЛЛБул VerboseAsm);
	ЛЛИМПОРТ ЛЛБул ЛЛЦелМашГенерируйВФайл(ЛЛЦелеваяМашина T, ЛЛМодуль м,
	  ткст0 имяф, LLVMCodeGenFileType codegen, ткст0 *ошсооб);
	ЛЛИМПОРТ ЛЛБул ЛЛЦелМашГенерируйВБуфПам(ЛЛЦелеваяМашина T, ЛЛМодуль м,
	  LLVMCodeGenFileType codegen, ткст0 * ошсооб, ЛЛБуферПамяти *внМодemбуф);
	/*===-- триада ------------------------------------------------------------===*/
	ЛЛИМПОРТ ткст0 ЛЛДайДефТриадуЦели(проц);
	ЛЛИМПОРТ ткст0 ЛЛНормализуйТриадуЦели(const ткст0 триада);
	ЛЛИМПОРТ ткст0 ЛЛДайИмяЦПБХоста(проц);
	ЛЛИМПОРТ ткст0 ЛЛДайФичиЦПБХоста(проц);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуАнализа(ЛЛЦелеваяМашина T, ЛЛМенеджерПроходок мп);
///////////////////////////////////////////////////
//Реестр Проходок

/// llvm::PassRegistrationListener
///ДатчикРегистрацийПроходок
struct PassRegistrationListener {
  PassRegistrationListener() = default;
  virtual ~PassRegistrationListener() = default;

  /// Callback functions - These functions are invoked whenever a pass is loaded
  /// or removed from the current executable.
  virtual void passRegistered(const PassInfo *) {}

  /// enumeratePasses - Iterate over the registered passes, calling the
  /// passEnumerate callback on each PassInfo object.
  void enumeratePasses();

  /// passEnumerate - Callback function invoked when someone calls
  /// enumeratePasses on this PassRegistrationListener object.
  virtual void passEnumerate(const PassInfo *) {}
};
//////////////////////////////////////////
//////////class PassRegistry
    ///
    ЛЛИМПОРТ ЛЛРеестрПроходок LLPassRegistry_ctor();
    ЛЛИМПОРТ проц LLPassRegistry_dtor(ЛЛРеестрПроходок сам);
    ЛЛИМПОРТ ЛЛРеестрПроходок LLPassRegistry_getPassRegistry(ЛЛРеестрПроходок сам);
    ЛЛИМПОРТ const PassInfo* LLPassRegistry_getPassInfo(ЛЛРеестрПроходок сам, const ук TI);
    ЛЛИМПОРТ const PassInfo* LLPassRegistry_getPassInfo2(ЛЛРеестрПроходок сам, StringRef арг);
    ЛЛИМПОРТ  проц LLPassRegistry_registerPass(ЛЛРеестрПроходок сам, const PassInfo& PI,
                                                                    bool ShouldFree = false);
    ЛЛИМПОРТ проц LLPassRegistry_registerAnalysisGroup(ЛЛРеестрПроходок сам,
                          const ук InterfaceID, const ук PassID,
                           PassInfo& Registeree, bool isDefault, bool ShouldFree = false);
    ЛЛИМПОРТ проц LLPassRegistry_enumerateWith(ЛЛРеестрПроходок сам, ук L);
    ЛЛИМПОРТ проц LLPassRegistry_addRegistrationListener(ЛЛРеестрПроходок сам, ук L);
    ЛЛИМПОРТ проц LLPassRegistry_removeRegistrationListener(ЛЛРеестрПроходок сам,
                                                          PassRegistrationListener* L);
/////////// end 0f class PassRegistry
////////////////////////////////////////////
//Трансформы
///////////////////////////////////////////
////-----Агрессивное комбинирование инструкций
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуАгрессивИнстКомбайнера(ЛЛМенеджерПроходок мп);
////------Сопроцедуры
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуКороЁли(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуКороСплит(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуКороЭлайд(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуКороКлинап(ЛЛМенеджерПроходок мп);
////------Комбинирование инструкций
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуКомбинированияИнструкций(ЛЛМенеджерПроходок мп);
////------IPO
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуПродвиженияАргументов(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуМёрджКонстант(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуПропагацииВызванногоЗначения(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуИскорененияНенужныхАргов(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуАтрибутовФункций(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуИнлайнингаФункций(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуИнлайнВсегда(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуГлобДЦЕ(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуГлобОптимизатора(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуПропагацииИПКонстант(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуПрюнЕХ(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуИПСЦЦ(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуИнтернализации(ЛЛМенеджерПроходок мп, бцел AllButMain);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуОчисткиНенужныхПрототипов(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуОчисткиСимволов(ЛЛМенеджерПроходок мп);
////-----Построитель менеджера проходок
	ЛЛИМПОРТ ЛЛПостроительМенеджеровПроходок ЛЛПМП_Создай(проц);
	ЛЛИМПОРТ проц ЛЛПМП_Вымести(ЛЛПостроительМенеджеровПроходок PMB);
	ЛЛИМПОРТ проц
	ЛЛПМП_УстановиУровеньОпц(ЛЛПостроительМенеджеровПроходок PMB,
	                                                  бцел урОпц);
	ЛЛИМПОРТ проц
	ЛЛПМП_УстановиУровеньРазм(ЛЛПостроительМенеджеровПроходок PMB,
	                                                   бцел SizeLevel);
	ЛЛИМПОРТ проц
	ЛЛПМП_УстановиОтклЮнитВНВремя(ЛЛПостроительМенеджеровПроходок PMB,
	                                                           ЛЛБул Value);
	ЛЛИМПОРТ проц
	ЛЛПМП_УстОтклРазмоткуЦиклов(ЛЛПостроительМенеджеровПроходок PMB,
	                                                           ЛЛБул Value);
	ЛЛИМПОРТ проц
	ЛЛПМП_УстОтклУпроститьВызовБиб(ЛЛПостроительМенеджеровПроходок PMB,
	                                                            ЛЛБул Value);
	ЛЛИМПОРТ проц
	ЛЛПМП_ИспользуйИнлайнерСПорогом(ЛЛПостроительМенеджеровПроходок PMB,
	                                                        бцел Threshold);
	ЛЛИМПОРТ проц
	ЛЛПМП_НаполниМенеджерПроходокФункций(ЛЛПостроительМенеджеровПроходок PMB,
	                                                          ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц
	ЛЛПМП_НаполниМенеджерПроходокМодулей(ЛЛПостроительМенеджеровПроходок PMB,
	                                                           ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛПМП_НаполниМенеджерПроходокОВК(ЛЛПостроительМенеджеровПроходок PMB,
	                                                  ЛЛМенеджерПроходок мп,
	                                                             ЛЛБул Internalize,
	                                                                     ЛЛБул RunInliner);
////-----Скалярные
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуАгрессивДЦЕ(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуБитТрэкингДЦЕ(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуРаскладкаИзАссумпций(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуКФГУпрощения(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуУдаленияМёртвыхХранилищ(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуСкаляризатора(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуМёрдждЛоудСторМоушн(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуГВН(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуНовГВН(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуИндВарУпрощения(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуДжампПоточности(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуЛИЦМ(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуЛупДелешн(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуЛупИдиом(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуЛупРотейт(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуЛупРеролл(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуЛупАнролл(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуЛупАнроллЭндДжем(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуЛупАнСвитч(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуЛоверАтомик(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуКопирПамОпц(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуЧастичнИнлайнВызБиб(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуРеассоциации(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуСЦЦП(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуСкалярРеплАгрегаты(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьССАПроходкиСкалярРеплАгрегаты(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуСкалярРеплАгрегатыСПорогом(ЛЛМенеджерПроходок мп,
	                                                  цел Threshold);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуУпроститьВызовыБиб(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуИскоренениеТейлВызовов(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуПропагацияКонстант(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуДемотПамВРегистр(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуВерификатора(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуПропагацииКоррелирЗначений(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуЁлиЦСЕ(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуЁлиЦСЕПамССА(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуЛоверЭкпектИнтринсик(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуАнализАлиасаНаОвеТипа(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуМасштбнБезАлиасАА(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуБазовыйАнализАлиаса(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуУнификацииУзловВыходаИзФункц(ЛЛМенеджерПроходок мп);
////------Утилитные
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуЛоверСвитч(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуПамятьВРегистр(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуДобавкиДискриминаторов(ЛЛМенеджерПроходок мп);
////-----Векторизация
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуВекторизацЦикла(ЛЛМенеджерПроходок мп);
	ЛЛИМПОРТ проц ЛЛДобавьПроходкуВекторизацСЛП(ЛЛМенеджерПроходок мп);
//////////////////////////////////
//Инструменты
////////////////////////////////
    ЛЛИМПОРТ int ЛЛВхоФункцЛЛАр(ткст0 *арги); //llvm-ar
	ЛЛИМПОРТ int ЛЛОбщВхоФункцЛЛАр( ткст0 *арги);
	ЛЛИМПОРТ int ЛЛВхоФункцЛЛРанлиб(ткст0 *арги); //llvm-ranlib
	ЛЛИМПОРТ int ЛЛВхоФункцЛЛАс(ткст0 *арги); //llvm-as
	ЛЛИМПОРТ int ЛЛВхоФункцЛЛКат(ткст0 *арги); //llvm-cat
	ЛЛИМПОРТ int ЛЛВхоФункцЛЛКомпилятора(ткст0 *арги); //llc
	ЛЛИМПОРТ int ЛЛВхоФункцЛЛДиз(ткст0 *арги); //llvm-dis


}
#endif
#endif
