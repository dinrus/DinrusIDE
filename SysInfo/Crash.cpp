// SPDX-License-Identifier: Apache-2.0
// Copyright 2021 - 2022, the Anboto author and contributors
#include <Core/Core.h>
#include "Crash.h"
#include <signal.h>
#include <exception>
#include <fenv.h>
#if defined(PLATFORM_WIN32)
#include <psapi.h>
#include <rtcapi.h>
#include <shellapi.h>
    #ifdef COMPILER_MSC
    #include <verrsrc.h>
    #endif
#include <dbghelp.h>
#include <eh.h>
#include <memory>
#endif

namespace Upp {


#if defined (flagDEBUG) && defined(PLATFORM_WIN32)
#pragma float_control(except, on)
#endif

CrashHandler::CrashHandler() {
#if defined(PLATFORM_WIN32)
    _clearfp();
    _controlfp(_controlfp(0, 0) & ~(_EM_INVALID | _EM_ZERODIVIDE | _EM_OVERFLOW), _MCW_EM);

    SetUnhandledExceptionFilter(UnhandledHandler);
    _set_purecall_handler(PureCallHandler);
    _set_invalid_parameter_handler(InvalidParameterHandler);
    //_set_abort_behavior(_CALL_REPORTFAULT, _CALL_REPORTFAULT);
#else
    feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW);
#endif

    std::set_new_handler(NewHandler);
    std::set_terminate(TerminateHandler);
    //std::set_unexpected(UnexpectedHandler);

    signal(SIGABRT, SigabrtHandler);
    signal(SIGINT, SigintHandler);
    signal(SIGTERM, SigtermHandler);
    signal(SIGFPE, SigfpeHandler);
    signal(SIGILL, SigillHandler);
    signal(SIGSEGV, SigsegvHandler);

    //InstallPanicMessageBox(PanicMessage);
}

#if defined(PLATFORM_WIN32)
LONG WINAPI CrashHandler::UnhandledHandler(EXCEPTION_POINTERS *exceptionPtrs) {
    Panic("Дефолтное исключение");
    return EXCEPTION_EXECUTE_HANDLER;
}

void /*__cdecl*/ CrashHandler::SEHHandler(unsigned u, EXCEPTION_POINTERS* p) {
    switch(u) {
        case EXCEPTION_FLT_DIVIDE_BY_ZERO:
        case EXCEPTION_INT_DIVIDE_BY_ZERO:
        Panic("Исключение плавающей запятой");
        break;
    default:
        Panic("Исключение SEH");
    }
}
#endif

void /*__cdecl*/ CrashHandler::TerminateHandler() {
    Panic("Исключение Terminate");
}

void /*__cdecl*/ CrashHandler::UnexpectedHandler() {
    Panic("Неожиданное исключение");
}

void /*__cdecl*/ CrashHandler::PureCallHandler() {
    Panic("Вызов чисто виртуальной функции");
}

void /*__cdecl*/ CrashHandler::InvalidParameterHandler(const wchar_t* expression, const wchar_t *function,
    const wchar_t* file, unsigned int line, uintptr_t) {
    if (line == 0)
        Panic("Неверный параметр");
    else
        Panic(Format("Неверный параметр в in %s, функция %s, фвйл %s, строка %d", AsString(expression),
            AsString(function), AsString(file), int(line)));
}

void /*__cdecl*/ CrashHandler::NewHandler() {
    Panic("Нехватка доступной памяти");
}

void CrashHandler::SigabrtHandler(int) {
    Panic("SIGABRT: Процесс прерван");
}

void CrashHandler::SigfpeHandler(int) {
    Panic("SIGFPE: Ошибка плавающей запятой");
}

void CrashHandler::SigillHandler(int) {
    Panic("SIGILL: Исполнимый код, кажется, повреждён");
}

void CrashHandler::SigintHandler(int) {
    Panic("SIGINT: Процесс прерван пользователем");
}

void CrashHandler::SigsegvHandler(int) {
    Panic("SIGSEGV: Попытка чтения/записи из/в область памяти, к которой у процесса нет доступа");
}

void CrashHandler::SigtermHandler(int) {
    Panic("SIGTERM: Процесс прерван из другого приложения");
}


}