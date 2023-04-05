#ifdef PLATFORM_WIN32

#include <winreg.h>

#ifdef PLATFORM_WINCE
inline bool IsWinNT()    { return false; }
inline bool IsWinXP()    { return false; }
inline bool IsWin2K()    { return false; }
inline bool IsWinVista() { return false; }
inline bool IsWin7()     { return false; }
#else
inline bool IsWinNT() { return GetVersion() < 0x80000000; }
bool IsWin2K();
bool IsWinXP();
bool IsWinVista();
bool IsWin7();
#endif

HINSTANCE прилДайУк();
void      прилУстУк(HINSTANCE dll_instance);

Ткст какТкст(const wchar_t *буфер);
Ткст какТкст(const wchar_t *буфер, int count);
Ткст какТкст(const wchar_t *буфер, const wchar_t *end);

Ткст дайТкстВинРега(const char *значение, const char *path, HKEY base_key = HKEY_LOCAL_MACHINE, dword wow = 0);
int    дайЦелВинРега(const char *значение, const char *path, HKEY base_key = HKEY_LOCAL_MACHINE, dword wow = 0);
bool   устТкстВинРега(const Ткст& string, const char *значение, const char *path, HKEY base_key = HKEY_LOCAL_MACHINE, dword wow = 0);
bool   SetWinRegExpandString(const Ткст& string, const char *значение, const char *path, HKEY base_key, dword wow = 0);
bool   устЦелВинРега(int данные, const char *значение, const char *path, HKEY base_key = HKEY_LOCAL_MACHINE, dword wow = 0);
void   удалиВинРег(const Ткст& ключ, HKEY base = HKEY_LOCAL_MACHINE, dword wow = 0);

void  *дайФнДлл(const char *dll, const char *фн);

template <class T>
void   фнДлл(T& x, const char *dll, const char *фн)
{
	x = (T)дайФнДлл(dll, фн);
}

bool Win32CreateProcess(const char *command, const char *envptr, STARTUPINFOW& si, PROCESS_INFORMATION& pi, const char *cd);

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
	bool       жди(int time_ms);
	bool       жди();

	HANDLE     дайУк() const { return handle; }

	СинхОбъект();
	~СинхОбъект();
};

class СобытиеВин32 : public СинхОбъект {
public:
	void       уст();

	СобытиеВин32();
};
#endif

#endif
