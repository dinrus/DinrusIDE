#ifndef _ide_Debuggers_GdbUtils_h_
#define _ide_Debuggers_GdbUtils_h_

#include <Core/Core.h>

namespace РНЦП {

class IGdbUtils {
public:
	virtual ~IGdbUtils() = default;
	
	virtual Ткст BreakRunning(int pid) = 0;
};

class GdbUtilsFactory final {
public:
	Один<IGdbUtils> создай();
};

#if defined(PLATFORM_WIN32)

class GdbWindowsUtils final : public IGdbUtils {
public: /* IGdbUtils */
	virtual Ткст BreakRunning(int pid) override;
	
private:
	bool Is64BitIde() const;
	bool Is64BitProcess(HANDLE handle) const;
};

#elif defined(PLATFORM_POSIX)

class GdbPosixUtils final : public IGdbUtils {
public: /* IGdbUtils */
	virtual Ткст BreakRunning(int pid) override;
};

#endif

}

#endif
