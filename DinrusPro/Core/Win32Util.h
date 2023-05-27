#ifdef PLATFORM_WIN32

#include <winreg.h>

#ifdef PLATFORM_WINCE
inline бул IsWinNT()    { return false; }
inline бул IsWinXP()    { return false; }
inline бул IsWin2K()    { return false; }
inline бул IsWinVista() { return false; }
inline бул IsWin7()     { return false; }
#else
inline бул IsWinNT() { return GetVersion() < 0x80000000; }
бул IsWin2K();
бул IsWinXP();
бул IsWinVista();
бул IsWin7();
#endif

HINSTANCE прилДайУк();
проц      прилУстУк(HINSTANCE dll_instance);

Ткст какТкст(const wchar_t *буфер);
Ткст какТкст(const wchar_t *буфер, цел count);
Ткст какТкст(const wchar_t *буфер, const wchar_t *end);

Ткст дайТкстВинРега(кткст0 значение, кткст0 path, HKEY base_key = HKEY_LOCAL_MACHINE, бцел wow = 0);
цел    дайЦелВинРега(кткст0 значение, кткст0 path, HKEY base_key = HKEY_LOCAL_MACHINE, бцел wow = 0);
бул   устТкстВинРега(const Ткст& string, кткст0 значение, кткст0 path, HKEY base_key = HKEY_LOCAL_MACHINE, бцел wow = 0);
бул   SetWinRegExpandString(const Ткст& string, кткст0 значение, кткст0 path, HKEY base_key, бцел wow = 0);
бул   устЦелВинРега(цел данные, кткст0 значение, кткст0 path, HKEY base_key = HKEY_LOCAL_MACHINE, бцел wow = 0);
проц   удалиВинРег(const Ткст& ключ, HKEY base = HKEY_LOCAL_MACHINE, бцел wow = 0);

ук дайФнДлл(кткст0 dll, кткст0 фн);

template <class T>
проц   фнДлл(T& x, кткст0 dll, кткст0 фн)
{
	x = (T)дайФнДлл(dll, фн);
}

бул Win32CreateProcess(кткст0 command, кткст0 envptr, STARTUPINFOW& si, PROCESS_INFORMATION& pi, кткст0 cd);

#ifndef PLATFORM_WINCE
Ткст GetSystemDirectory();
Ткст GetWindowsDirectory();
#endif
Ткст GetModuleFileName(HINSTANCE instance = прилДайУк());

#ifdef DEPRECATED
class СинхОбъект {
protected:
	HANDLE     handle;

public:
	бул       жди(цел time_ms);
	бул       жди();

	HANDLE     дайУк() const { return handle; }

	СинхОбъект();
	~СинхОбъект();
};

class СобытиеВин32 : public СинхОбъект {
public:
	проц       уст();

	СобытиеВин32();
};
#endif

#endif
