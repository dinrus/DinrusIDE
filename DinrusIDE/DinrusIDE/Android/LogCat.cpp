#include "Executables.h"

namespace РНЦП {

LogCat::LogCat()
{
	
}

LogCat::~LogCat()
{
	
}

Ткст LogCat::MakeCmdByTag(const Ткст& packageName) const
{
	Ткст cmd;
	
	cmd << "logcat " << packageName << ":I" << " *:S";
	
	return cmd;
}

Ткст LogCat::MakeCmdByTag(const Adb& adb, const Ткст& packageName) const
{
	return adb.MakeCmd() + " " + MakeCmdByTag(packageName);
}

}
