#include "Android.h"

namespace РНЦП {

Adb::Adb(const Ткст& path)
	: path(path)
{
}

int Adb::GetPid(const Ткст& packageName) const
{
	int pid = -1;
	
	Ткст out;
	if(Sys(MakeGetAllProcessesCmd(), out) == 0) {
		Вектор<Ткст> lines = разбей(out, "\n");
		for(int i = 0; i < lines.дайСчёт(); i++) {
			if(lines[i].найди(packageName) >= 0) {
				Вектор<Ткст> parts = разбей(lines[i], " ");
				if(parts.дайСчёт() >= 2)
					pid = тктЦел(parts[1]);
			}
		}
	}
	
	return pid;
}

Ткст Adb::MakeCmd() const
{
	Ткст cmd = NormalizeExePath(path);
	if(!serial.пустой())
		cmd << " -s " << serial;
	
	return cmd;
}

Ткст Adb::MakeInstallCmd(const Ткст& apkPath) const
{
	return MakeCmd() + " install -r " + apkPath;
}

Ткст Adb::MakeInstallOnDefaultDeviceCmd(const Ткст& apkPath) const
{
	return NormalizeExePath(path) + " -d install -r " + apkPath;
}

Ткст Adb::MakeInstallOnDefaultEmulatorCmd(const Ткст& apkPath) const
{
	return NormalizeExePath(path) + " -e install -r " + apkPath;
}

Ткст Adb::MakeLaunchOnDeviceCmd(const Ткст& packageName, const Ткст& activityName) const
{
	Ткст cmd = NormalizeExePath(path);
	cmd << " shell am start";
	cmd << " -n " << packageName << "/" << activityName;
	
	return cmd;
}

Ткст Adb::MakeGetAllProcessesCmd() const
{
	return MakeCmd() + " shell ps";
}

}
