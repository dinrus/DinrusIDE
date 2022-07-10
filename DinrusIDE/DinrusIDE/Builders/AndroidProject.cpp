#include "Android.h"

namespace РНЦП {

AndroidProject::AndroidProject(const Ткст& dir, bool debug)
	: dir(dir)
	, debug(debug)
{
}

Ткст AndroidProject::GetDir() const
{
	return this->dir;
}

Ткст AndroidProject::GetJavaDir() const
{
	return this->dir + DIR_SEPS + "java";
}

Ткст AndroidProject::GetJavaDir(const Ткст& package) const
{
	return GetJavaDir() + DIR_SEPS + package;
}

Ткст AndroidProject::GetJniDir() const
{
	return this->dir + DIR_SEPS + "jni";
}

Ткст AndroidProject::GetJniDir(const Ткст& package) const
{
	return GetJniDir() + DIR_SEPS + package;
}

Ткст AndroidProject::GetLibsDir() const
{
	return this->dir + DIR_SEPS + "libs";
}

Ткст AndroidProject::GetResDir() const
{
	return this->dir + DIR_SEPS + "res";
}

Ткст AndroidProject::GetBuildDir() const
{
	return this->dir + DIR_SEPS + "build";
}

Ткст AndroidProject::GetClassesDir() const
{
	return this->dir + DIR_SEPS + "classes";
}

Ткст AndroidProject::GetBinDir() const
{
	return this->dir + DIR_SEPS + "bin";
}

Ткст AndroidProject::GetObjDir() const
{
	return this->dir + DIR_SEPS + "obj";
}

Ткст AndroidProject::GetObjLocalDir() const
{
	return GetObjDir() + DIR_SEPS + "local";
}

// -------------------------------------------------------------------

Ткст AndroidProject::GetManifestPath() const
{
	return this->dir + DIR_SEPS + "AndroidManifest.xml";
}

Ткст AndroidProject::GetJniMakeFilePath() const
{
	return GetJniDir() + DIR_SEPS + "Android.mk";
}

Ткст AndroidProject::GetJniApplicationMakeFilePath() const
{
	return GetJniDir() + DIR_SEPS + "Application.mk";
}

// -------------------------------------------------------------------

bool AndroidProject::IsDebug() const
{
	return debug;
}

bool AndroidProject::IsRelease() const
{
	return !debug;
}

}
