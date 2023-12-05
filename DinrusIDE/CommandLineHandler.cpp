#include "CommandLineHandler.h"

#include "About.h"

#include <Draw/Draw.h>
#include <DinrusIDE/Common/CommandLineOptions.h>
#include <DinrusIDE/Debuggers/GdbUtils.h>

using namespace Upp;

ACommandLineHandler::ACommandLineHandler(const Vector<String>& args)
	: args(clone(args))
{}

BaseCommandLineHandler::BaseCommandLineHandler(const Vector<String>& args)
	: ACommandLineHandler(args)
{}

bool BaseCommandLineHandler::Handle()
{
	if(HandleVersion())
		return true;
	if(HandleHelp())
		return true;
	
	if(HandleDebugBreakProcess())
		return true;
	
	return false;
}

bool BaseCommandLineHandler::HandleVersion() const
{
	if(args.IsEmpty() || findarg(args[0], "-v", "--version") < 0)
		return false;
	
	Cout() << SplashCtrl::GenerateVersionInfo(' ') << "\n";
	
	return true;
}

bool BaseCommandLineHandler::HandleHelp() const
{
	if(args.IsEmpty() || findarg(args[0], "?", "--help", "-h", "-?", "/?") < 0)
		return false;
		
	Cout() << "Использование:\n"
			  "    DinrusIDE [файл..]             - открывает данный файл в режиме редактора (режим автодетекции).\n"
			  "    DinrusIDE [сборка] [пакет] - открывает данный пакет из данной сборки.\n\n";
	
	Cout() << "Общие опции:\n"
	          "    -f $файл        - открывает указанный файл в режиме редактора.\n"
	          "    -v или --version - показывает информацию о версии.\n"
			  "    -h или --help    - показывает это сообщение.\n\n";
	
	Cout() << "Продвинутые опции:\n"
			  "    " << COMMAND_LINE_SCALE_OPTION << " $процент - масштабирование интерфейса на параметр \"процент\".\n\n";
	
	Cout() << "Внутренние опции (не для пользовательского вызова):\n"
	          "    " << COMMAND_LINE_GDB_DEBUG_BREAK_PROCESS_OPTION << " $пид - прерывает процесс отладки, представленный \"пид\".\n";
	
	return true;
}

bool BaseCommandLineHandler::HandleDebugBreakProcess() const
{
	if(args.GetCount() < 2 || !args[0].IsEqual(COMMAND_LINE_GDB_DEBUG_BREAK_PROCESS_OPTION))
		return false;
	
	int pid = StrInt(args[1]);
	if(IsNull(pid)) {
		Cout() << "ПИД должен быть числовым значением.\n";
		return true;
	}
	
	auto utils = GdbUtilsFactory().Create();
	auto error = utils->BreakRunning(pid);
	if(!error.IsEmpty()) {
		Cout() << error << "\n";
		SetExitCode(COMMAND_LINE_GDB_DEBUG_BREAK_PROCESS_FAILURE_STATUS);
	}
	
	return true;
}

MainCommandLineHandler::MainCommandLineHandler(const Vector<String>& args)
	: ACommandLineHandler(args)
{}

bool MainCommandLineHandler::Handle()
{
	if(HandleManipulators())
		return true;
	
	return false;
}

bool MainCommandLineHandler::HandleManipulators()
{
	if(HandleScale())
		return true;
	
	return false;
}

bool MainCommandLineHandler::HandleScale()
{
	if(args.GetCount() < 2 || !args[0].IsEqual(COMMAND_LINE_SCALE_OPTION))
		return false;
	
	int scale = StrInt(args[1]);
	if(IsNull(scale)) {
		Cout() << "Масштаб должен быть числовым значением.\n";
		return true;
	}
	
	Font::SetStdFont(StdFont().Height(GetStdFontCy() * minmax(scale, 50, 400) / 100));
	
	args.Remove(0, 2);
	
	return false;
}
