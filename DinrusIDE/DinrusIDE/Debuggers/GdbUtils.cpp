#include "GdbUtils.h"

#include <memory>
#include <DinrusIDE/Core/Logger.h>
#include <DinrusIDE/Common/CommandLineOptions.h>

using namespace РНЦП;

Один<IGdbUtils> GdbUtilsFactory::создай()
{
#if defined(PLATFORM_WIN32)
	return сделайОдин<GdbWindowsUtils>();
#elif defined(PLATFORM_POSIX)
	return сделайОдин<GdbPosixUtils>();
#endif
}

#if defined(PLATFORM_WIN32)

#define METHOD_NAME UPP_METHOD_NAME("GdbWindowsUtils")

using DeleteHandleFun = std::function<void(HANDLE)>;

static void DeleteHandle(HANDLE handle)
{
	if (handle)
	{
		CloseHandle(handle);
	}
}

Ткст GdbWindowsUtils::BreakRunning(int pid)
{
	std::unique_ptr<void, DeleteHandleFun> handle(OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid), &DeleteHandle);
	if(!handle)
		return Ткст().конкат() << "Не удалось открыть процесс, ассоциированный с ПИД " << pid <<" .";
	
	if(Is64BitIde() && !Is64BitProcess(handle.get())) {
		auto path = Ткст().конкат() << дайПапкуИсп() << "\\" << дайТитулИсп() << "32.exe";
		auto cmd = Ткст().конкат() << path << " " << COMMAND_LINE_GDB_DEBUG_BREAK_PROCESS_OPTION << " " << pid;
		
		Ткст out;
		if(Sys(cmd, out) == COMMAND_LINE_GDB_DEBUG_BREAK_PROCESS_FAILURE_STATUS) {
			Logd() << METHOD_NAME << cmd;
			
			return Ткст().конкат() << "Не удалось прервать процесс через 32-битную ИСР. Вывод из команды: \"" << out << "\".";
		}
		
		return "";
	}
	
	if (!DebugBreakProcess(handle.get()))
		return Ткст().конкат() << "Не удалось остановить процесс, ассоциированный с ПИД " << pid << " .";
	
	return "";
}

bool GdbWindowsUtils::Is64BitIde() const
{
	return sizeof(void*) == 8;
}

bool GdbWindowsUtils::Is64BitProcess(HANDLE handle) const
{
	BOOL is_wow_64 = FALSE;
	if(!IsWow64Process(handle, &is_wow_64)) {
		Loge() << METHOD_NAME << "Не удалось проверить, находится ли процесс под слоем эмуляции wow64.";
	}
	
	return !is_wow_64;
}

#undef METHOD_NAME

#elif defined(PLATFORM_POSIX)

Ткст GdbPosixUtils::BreakRunning(int pid)
{
	if(kill(pid, SIGINT) == -1)
		return Ткст().конкат() << "Не удалось прервать процесс, ассоциированный с ПИД " << pid << " .";
	
	return "";
}

#endif
