#ifndef _Java_Java_h_
#define _Java_Java_h_

#include <Core/Core.h>
#include <DinrusIDE/Core/Core.h>

namespace РНЦП {

class JavaVersion {
public:
	JavaVersion();
	JavaVersion(int major, int minor);
	
	bool IsGreate(int major) const;
	
	bool IsGreaterOrEqual(int major) const;
	bool IsGreaterOrEqual(int major, int minor) const;
	
	Ткст вТкст() { return целТкт(mMajor) + "." + целТкт(mMinor); }
	
private:
	int mMajor;
	int mMinor;
};

class Java {
public:
	Java() = delete;
	Java(const Java&) = delete;
	Java& operator=(const Java&) = delete;
	virtual ~Java() = delete;
	
	static Ткст GetDelimiter();
};

class Jdk {
public:
	static Ткст GetDownloadUrl();
	
public:
	Jdk(const Ткст& path, Хост* host);
	virtual ~Jdk() {}
	
	bool Validate() const;
	
public:
	const JavaVersion& дайВерсию() const  { return version; }
	
	Ткст GetBinDir() const               { return path + DIR_SEPS + "bin"; }
	Ткст GetIncludeDir() const           { return path + DIR_SEPS + "include"; }
	
	Ткст GetJarPath() const              { return GetBinDir() + DIR_SEPS + "jar" + GetExeExt(); }
	Ткст GetJavacPath() const            { return GetBinDir() + DIR_SEPS + "javac" + GetExeExt(); }
	Ткст GetJavadocPath() const          { return GetBinDir() + DIR_SEPS + "javadoc" + GetExeExt(); }
	Ткст GetJavahPath() const            { return GetBinDir() + DIR_SEPS + "javah" + GetExeExt(); }
	Ткст GetJavapPath() const            { return GetBinDir() + DIR_SEPS + "javap" + GetExeExt(); }
	Ткст GetJdbPath() const              { return GetBinDir() + DIR_SEPS + "jdb" + GetExeExt(); }
	
	Ткст GetJarsignerPath() const        { return GetBinDir() + DIR_SEPS + "jarsigner" + GetExeExt(); }
	Ткст GetKeytoolPath() const          { return GetBinDir() + DIR_SEPS + "keytool" + GetExeExt(); }
	
	Ткст дайПуть() const                 { return this->path; }

public:
	void SetPath(const Ткст& path) { this->path = path; }
	void устВерсию(const Ткст& version);
	
private:
	void FindVersion(Хост* host);
	
private:
	Ткст       path;
	JavaVersion  version;
};

}

#endif
