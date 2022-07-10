#include "CommandLineHandler.h"

#include "About.h"

#include <Draw/Draw.h>
#include <DinrusIDE/Common/CommandLineOptions.h>
#include <DinrusIDE/Debuggers/GdbUtils.h>

using namespace РНЦП;

ACommandLineHandler::ACommandLineHandler(const Вектор<Ткст>& args)
	: args(clone(args))
{}

BaseCommandLineHandler::BaseCommandLineHandler(const Вектор<Ткст>& args)
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
	if(args.пустой() || findarg(args[0], "-v", "--version") < 0)
		return false;
	
	Cout() << SplashCtrl::GenerateVersionInfo(' ') << "\n";
	
	return true;
}

bool BaseCommandLineHandler::HandleHelp() const
{
	if(args.пустой() || findarg(args[0], "?", "--help", "-h", "-?", "/?") < 0)
		return false;
		
	Cout() << "Использование:\n"
			  "    theide [файл..]             - открывает данный файл в режиме редактора (режим автодетекции).\n"
			  "    theide [сборка] [пакет] - открывает данный пакет из данной сборки.\n\n";
	
	Cout() << "Общие опции:\n"
	          "    -f $файл        - открывает указанный файл в режиме редактора.\n"
	          "    -v or --version - показывает информацию о версии.\n"
			  "    -h or --help    - показывает это сообщение.\n\n";
	
	Cout() << "Продвинутые опции:\n"
			  "    " << COMMAND_LINE_SCALE_OPTION << " $процент - масштабирование интерфейса на параметр \"процент\".\n\n";
	
	Cout() << "Внутренние опции (не для пользовательского вызова):\n"
	          "    " << COMMAND_LINE_GDB_DEBUG_BREAK_PROCESS_OPTION << " $пид - прерывает процесс отладки, представленный \"пид\".\n";
	
	return true;
}

bool BaseCommandLineHandler::HandleDebugBreakProcess() const
{
	if(args.дайСчёт() < 2 || !args[0].равен(COMMAND_LINE_GDB_DEBUG_BREAK_PROCESS_OPTION))
		return false;
	
	int pid = тктЦел(args[1]);
	if(пусто_ли(pid)) {
		Cout() << "ПИД должен быть числовым значением.\n";
		return true;
	}
	
	auto utils = GdbUtilsFactory().создай();
	auto Ошибка = utils->BreakRunning(pid);
	if(!Ошибка.пустой()) {
		Cout() << Ошибка << "\n";
		устКодВыхода(COMMAND_LINE_GDB_DEBUG_BREAK_PROCESS_FAILURE_STATUS);
	}
	
	return true;
}

MainCommandLineHandler::MainCommandLineHandler(const Вектор<Ткст>& args)
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
	if(args.дайСчёт() < 2 || !args[0].равен(COMMAND_LINE_SCALE_OPTION))
		return false;
	
	int scale = тктЦел(args[1]);
	if(пусто_ли(scale)) {
		Cout() << "Масштаб должен быть числовым значением.\n";
		return true;
	}
	
	Шрифт::SetStdFont(StdFont().устВысоту(GetStdFontCy() * minmax(scale, 50, 400) / 100));
	
	args.удали(0, 2);
	
	return false;
}
