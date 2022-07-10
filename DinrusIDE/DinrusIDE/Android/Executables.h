#ifndef _ide_Android_Executable_h_
#define _ide_Android_Executable_h_

#include <Core/Core.h>

#include <DinrusIDE/Core/Core.h>

namespace РНЦП {

class LogCat;

class Adb : public Движимое<Adb> {
public:
	Adb(const Ткст& path);
	virtual ~Adb() {}
	
public:
	Ткст дайПуть() const               { return this->path; }
	Ткст GetSerial() const             { return this->serial; }
	
	void SetPath(const Ткст& path)     { this->path = path; }
	void SetSerial(const Ткст& serial) { this->serial = serial; }
	
public:
	int GetPid(const Ткст& packageName) const;
	
public:
	Ткст MakeListDevicesCmd() const;
	
	Ткст MakeCmd() const;
	
	Ткст MakeInstallCmd(const Ткст& apkPath) const;
	Ткст MakeInstallOnDefaultDeviceCmd(const Ткст& apkPath) const;
	Ткст MakeInstallOnDefaultEmulatorCmd(const Ткст& apkPath) const;
	
	Ткст MakeLaunchOnDeviceCmd(const Ткст& packageName, const Ткст& activityName) const;
	
	Ткст MakeGetAllProcessesCmd() const;
	
private:
	Ткст path;
	Ткст serial;
};

class LogCat : public Движимое<LogCat> {
public:
	LogCat();
	virtual ~LogCat();

public:
	Ткст MakeCmdByTag(const Ткст& packageName) const;
	Ткст MakeCmdByTag(const Adb& adb, const Ткст& packageName) const;
	
private:
	
};

class NDKBuild {
public:
	NDKBuild(const Ткст& path);
	virtual ~NDKBuild() {}
	
	void SetJobs(int jobs)                       { this->jobs = jobs; }
	void SetWorkingDir(const Ткст& workingDir) { this->workingDir = workingDir; }
	void EnableVerbose(bool verbose = true)      { this->verbose = verbose; }
	
	Ткст MakeCmd() const;
	
private:
	Ткст path;
	Ткст workingDir;
	int    jobs;
	bool   verbose;
};

}

#endif
