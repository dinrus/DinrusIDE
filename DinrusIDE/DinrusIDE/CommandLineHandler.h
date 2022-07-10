#ifndef _ide_Command_Line_Handler_h_
#define _ide_Command_Line_Handler_h_

#include <Core/Core.h>

// TODO: All TheIDE command line arguments should be consume in this file.

namespace РНЦП {

class ACommandLineHandler {
public:
	ACommandLineHandler(const Вектор<Ткст>& args);
	
	Вектор<Ткст> GetArgs() { return clone(args); }
	
public:
	virtual bool Handle() = 0;
	
protected:
	Вектор<Ткст> args;
};

class BaseCommandLineHandler final : public ACommandLineHandler {
public:
	BaseCommandLineHandler(const Вектор<Ткст>& args);
	
public:
	bool Handle() override;
	
private:
	bool HandleVersion() const;
	bool HandleHelp() const;
	
	bool HandleDebugBreakProcess() const;
};

class MainCommandLineHandler final : public ACommandLineHandler {
public:
	MainCommandLineHandler(const Вектор<Ткст>& args);
	
public:
	bool Handle() override;
	
private:
	bool HandleManipulators();
	bool HandleScale();
};

}

#endif
