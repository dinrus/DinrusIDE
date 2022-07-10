#include "Executables.h"

namespace РНЦП {

NDKBuild::NDKBuild(const Ткст& path)
	: path(path)
	, jobs(0)
	, verbose(false)
{
}

Ткст NDKBuild::MakeCmd() const
{
	Ткст cmd = NormalizeExePath(path);
	if(verbose)
		cmd << " V=1 ";
	if(!workingDir.пустой())
		cmd << " -C " << workingDir;
	if(jobs > 0)
		cmd << " -j " << jobs;
	
	return cmd;
}

}
