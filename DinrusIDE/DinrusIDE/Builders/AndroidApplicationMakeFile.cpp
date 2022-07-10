#include "Android.h"

namespace РНЦП {

AndroidApplicationMakeFile::AndroidApplicationMakeFile()
{
	
}

AndroidApplicationMakeFile::~AndroidApplicationMakeFile()
{
	
}

Ткст AndroidApplicationMakeFile::вТкст() const
{
	Ткст makeFile;
	
	AppendPlatform(makeFile);
	AppendArchitectures(makeFile);
	AppendCppRuntime(makeFile);
	AppendCppFlags(makeFile);
	AppendCFlags(makeFile);
	AppendOptim(makeFile);
	AppendToolchain(makeFile);
	
	return makeFile;
}

void AndroidApplicationMakeFile::SetPlatform(const Ткст& platform)
{
	this->platform = platform;
}

void AndroidApplicationMakeFile::SetArchitectures(const Вектор<Ткст>& architectures)
{
	this->architectures = clone(architectures);
}

void AndroidApplicationMakeFile::AddArchitecture(const Ткст& architecture)
{
	architectures.добавь(architecture);
}

void AndroidApplicationMakeFile::SetCppRuntime(const Ткст& cppRuntime)
{
	this->cppRuntime = cppRuntime;
}

void AndroidApplicationMakeFile::SetCppFlags(const Ткст& cppFlags)
{
	this->cppFlags = cppFlags;
}

void AndroidApplicationMakeFile::SetCFlags(const Ткст& cFlags)
{
	this->cFlags = cFlags;
}

void AndroidApplicationMakeFile::SetOptim(const Ткст& optim)
{
	this->optim = optim;
}

void AndroidApplicationMakeFile::SetToolchain(const Ткст& toolchain)
{
	this->toolchain = toolchain;
}

void AndroidApplicationMakeFile::AppendPlatform(Ткст& makeFile) const
{
	AndroidMakeFile::AppendString(makeFile, platform, "APP_PLATFORM");
}

void AndroidApplicationMakeFile::AppendArchitectures(Ткст& makeFile) const
{
	AndroidMakeFile::AppendStringVector(makeFile, architectures, "APP_ABI");
}

void AndroidApplicationMakeFile::AppendCppRuntime(Ткст& makeFile) const
{
	AndroidMakeFile::AppendString(makeFile, cppRuntime, "APP_STL");
}

void AndroidApplicationMakeFile::AppendCppFlags(Ткст& makeFile) const
{
	AndroidMakeFile::AppendString(makeFile, cppFlags, "APP_CPPFLAGS");
}

void AndroidApplicationMakeFile::AppendCFlags(Ткст& makeFile) const
{
	AndroidMakeFile::AppendString(makeFile, cFlags, "APP_CFLAGS");
}

void AndroidApplicationMakeFile::AppendOptim(Ткст& makeFile) const
{
	AndroidMakeFile::AppendString(makeFile, optim, "APP_OPTIM");
}

void AndroidApplicationMakeFile::AppendToolchain(Ткст& makeFile) const
{
	AndroidMakeFile::AppendString(makeFile, toolchain, "NDK_TOOLCHAIN_VERSION");
}

}
