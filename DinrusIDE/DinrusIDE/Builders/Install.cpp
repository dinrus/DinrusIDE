#include "Builders.h"

#ifdef PLATFORM_POSIX

#ifdef PLATFORM_OSX

const char *clang_bm =
R"(BUILDER = "CLANG";
COMPILER = "clang++";
COMMON_OPTIONS = "-mmacosx-version-min=10.13";
COMMON_CPP_OPTIONS = "-std=c++14 -Wall -Wno-logical-op-parentheses";
COMMON_C_OPTIONS = "";
COMMON_LINK = "";
COMMON_FLAGS = "";
DEBUG_INFO = "2";
DEBUG_BLITZ = "1";
DEBUG_LINKMODE = "1";
DEBUG_OPTIONS = "-O0";
DEBUG_FLAGS = "";
DEBUG_LINK = "";
RELEASE_BLITZ = "1";
RELEASE_LINKMODE = "1";
RELEASE_OPTIONS = "-O3 -ffunction-sections -fdata-sections";
RELEASE_FLAGS = "";
RELEASE_LINK = "";
DEBUGGER = "gdb";
ALLOW_PRECOMPILED_HEADERS = "0";
DISABLE_BLITZ = "0";
PATH = "";
INCLUDE = "/opt/local/include;/usr/include";
LIB = "/opt/local/lib;/usr/lib";
LINKMODE_LOCK = "0";)";

#elif PLATFORM_SOLARIS

const char *gcc_bm =
R"(BUILDER = "GCC";
COMPILER = "";
COMMON_OPTIONS = "";
COMMON_CPP_OPTIONS = "-std=c++14";
COMMON_C_OPTIONS = "";
COMMON_LINK = "";
COMMON_FLAGS = "";
DEBUG_INFO = "2";
DEBUG_BLITZ = "1";
DEBUG_LINKMODE = "1";
DEBUG_OPTIONS = "-O0";
DEBUG_FLAGS = "";
DEBUG_LINK = "";
RELEASE_BLITZ = "1";
RELEASE_LINKMODE = "1";
RELEASE_OPTIONS = "-O3 -ffunction-sections -fdata-sections";
RELEASE_FLAGS = "";
RELEASE_LINK = "";
DEBUGGER = "gdb";
ALLOW_PRECOMPILED_HEADERS = "0";
DISABLE_BLITZ = "0";
PATH = "";
INCLUDE = "";
LIB = "";
LINKMODE_LOCK = "0";)";

const char *clang_bm =
R"(BUILDER = "CLANG";
COMPILER = "clang++";
COMMON_OPTIONS = "";
COMMON_CPP_OPTIONS = "-std=c++14 -Wno-logical-op-parentheses";
COMMON_C_OPTIONS = "";
COMMON_LINK = "";
COMMON_FLAGS = "";
DEBUG_INFO = "2";
DEBUG_BLITZ = "1";
DEBUG_LINKMODE = "1";
DEBUG_OPTIONS = "-O0";
DEBUG_FLAGS = "";
DEBUG_LINK = "";
RELEASE_BLITZ = "1";
RELEASE_LINKMODE = "1";
RELEASE_OPTIONS = "-O3 -ffunction-sections -fdata-sections";
RELEASE_FLAGS = "";
RELEASE_LINK = "";
DEBUGGER = "gdb";
ALLOW_PRECOMPILED_HEADERS = "0";
DISABLE_BLITZ = "0";
PATH = "";
INCLUDE = "";
LIB = "";
LINKMODE_LOCK = "0";)";

#else

const char *gcc_bm =
R"(BUILDER = "GCC";
COMPILER = "";
COMMON_OPTIONS = "";
COMMON_CPP_OPTIONS = "-std=c++14";
COMMON_C_OPTIONS = "";
COMMON_LINK = "";
COMMON_FLAGS = "";
DEBUG_INFO = "2";
DEBUG_BLITZ = "1";
DEBUG_LINKMODE = "1";
DEBUG_OPTIONS = "-O0";
DEBUG_FLAGS = "";
DEBUG_LINK = "";
RELEASE_BLITZ = "1";
RELEASE_LINKMODE = "1";
RELEASE_OPTIONS = "-O3 -ffunction-sections -fdata-sections";
RELEASE_FLAGS = "";
RELEASE_LINK = "-Wl,--gc-sections";
DEBUGGER = "gdb";
ALLOW_PRECOMPILED_HEADERS = "0";
DISABLE_BLITZ = "0";
PATH = "";
INCLUDE = "";
LIB = "";
LINKMODE_LOCK = "0";)";

const char *clang_bm =
R"(BUILDER = "CLANG";
COMPILER = "clang++";
COMMON_OPTIONS = "";
COMMON_CPP_OPTIONS = "-std=c++14 -Wno-logical-op-parentheses";
COMMON_C_OPTIONS = "";
COMMON_LINK = "";
COMMON_FLAGS = "";
DEBUG_INFO = "2";
DEBUG_BLITZ = "1";
DEBUG_LINKMODE = "1";
DEBUG_OPTIONS = "-O0";
DEBUG_FLAGS = "";
DEBUG_LINK = "";
RELEASE_BLITZ = "1";
RELEASE_LINKMODE = "1";
RELEASE_OPTIONS = "-O3 -ffunction-sections -fdata-sections";
RELEASE_FLAGS = "";
RELEASE_LINK = "-Wl,--gc-sections";
DEBUGGER = "gdb";
ALLOW_PRECOMPILED_HEADERS = "0";
DISABLE_BLITZ = "0";
PATH = "";
INCLUDE = "";
LIB = "";
LINKMODE_LOCK = "0";)";

#endif

void CreateBuildMethods()
{
#ifdef PLATFORM_COCOA
	Ткст bm = конфигФайл("CLANG.bm");
	if(пусто_ли(загрузиФайл(bm)))
		сохраниФайл(bm, clang_bm);
#else
	bool openbsd = впроп(Sys("uname")).найди("openbsd") >= 0;
	auto фиксируй = [=](const char *s) {
		Ткст r = s;
		if(openbsd) {
			r.замени("INCLUDE = \"\";", "INCLUDE = \"/usr/local/include\";");
			r.замени("LIB = \"\";", "LIB = \"/usr/local/lib\";");
		}
		return r;
	};

	Ткст bm = конфигФайл("GCC.bm");
	if(пусто_ли(загрузиФайл(bm)))
		сохраниФайл(bm, фиксируй(gcc_bm));

	if(Sys("clang --version").дайСчёт()) {
		Ткст bm = конфигФайл("CLANG.bm");
		if(пусто_ли(загрузиФайл(bm)))
			сохраниФайл(bm, фиксируй(clang_bm));
	}
#endif
}

Ткст GetDefaultUppOut()
{
	Ткст out;
	Ткст p = дайПапкуИсп();
	while(p.дайСчёт() > 1 && дирЕсть(p)) {
		Ткст h = приставьИмяф(p, ".cache");
		if(дирЕсть(h)) {
			out = h;
			break;
		}
		p = дайПапкуФайла(p);
	}
	
	out = Nvl(out, дайФайлИзДомПапки(".cache")) + "/upp.out";
	
	реализуйДир(out);
	return out;
}

#else

Ткст GetDefaultUppOut()
{
	return конфигФайл("_out");
}

#endif
