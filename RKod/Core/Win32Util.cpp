#include "Core.h"

namespace РНЦПДинрус {

#ifdef PLATFORM_WIN32

static HINSTANCE app_instance;

HINSTANCE прилДайУк()
{
	if(!app_instance)
		app_instance = GetModuleHandle(NULL);
	return app_instance;
}

void прилУстУк(HINSTANCE dll_instance) { app_instance = dll_instance; }

#ifndef PLATFORM_WINCE
bool IsWin2K()
{
	OSVERSIONINFO of;
	of.dwOSVersionInfoSize = sizeof(of);
	GetVersionEx(&of);
	return of.dwMajorVersion >= 5;
}

bool IsWinXP()
{
	OSVERSIONINFO of;
	of.dwOSVersionInfoSize = sizeof(of);
	GetVersionEx(&of);
	return of.dwMajorVersion > 5 || of.dwMajorVersion == 5 && of.dwMinorVersion >= 1;
}

bool IsWinVista()
{
	OSVERSIONINFO of;
	of.dwOSVersionInfoSize = sizeof(of);
	GetVersionEx(&of);
	return of.dwMajorVersion >= 6;
}

bool IsWin7()
{
	OSVERSIONINFO of;
	of.dwOSVersionInfoSize = sizeof(of);
	GetVersionEx(&of);
	return of.dwMajorVersion >= 6 && of.dwMinorVersion >= 1;
}
#endif

Ткст какТкст(const wchar_t *буфер) {
	if(!буфер)
		return Null;
	return какТкст(буфер, (int)wcslen(буфер));
}

Ткст какТкст(const wchar_t *буфер, int count) { // Преобр with code page...
	if(!буфер)
		return Null;
	ТкстБуф temp(count);
	for(char *p = temp, *e = p + count; p < e;)
		*p++ = (char)*буфер++;
	return Ткст(temp);
}

Ткст какТкст(const wchar_t *буфер, const wchar_t *end) {
	if(!буфер)
		return Null;
	return какТкст(буфер, (int)(end - буфер));
}

Ткст дайТкстВинРега(const char *значение, const char *path, HKEY base_key, dword wow) {
	HKEY ключ = 0;
	if(RegOpenKeyEx(base_key, path, 0, KEY_READ|wow, &ключ) != ERROR_SUCCESS)
		return Ткст::дайПроц();
	dword тип, len;
	if(RegQueryValueEx(ключ, значение, 0, &тип, NULL, &len) != ERROR_SUCCESS)
	{
		RegCloseKey(ключ);
		return Ткст::дайПроц();
	}
	ТкстБуф raw_len(len);
	if(RegQueryValueEx(ключ, значение, 0, 0, (byte *)~raw_len, &len) != ERROR_SUCCESS)
	{
		RegCloseKey(ключ);
		return Ткст::дайПроц();
	}
	if(len > 0 && (тип == REG_SZ || тип == REG_EXPAND_SZ))
		len--;
	raw_len.устДлину(len);
	RegCloseKey(ключ);
	return Ткст(raw_len);
}

int дайЦелВинРега(const char *значение, const char *path, HKEY base_key, dword wow) {
	HKEY ключ = 0;
	if(RegOpenKeyEx(base_key, path, 0, KEY_READ|wow, &ключ) != ERROR_SUCCESS)
		return Null;
	int данные;
	dword тип, length = sizeof(данные);
	if(RegQueryValueEx(ключ, значение, 0, &тип, (byte *)&данные, &length) != ERROR_SUCCESS)
		данные = Null;
	RegCloseKey(ключ);
	return данные;
}

bool устТкстВинРега(const Ткст& string, const char *значение, const char *path, HKEY base_key, dword wow) {
	HKEY ключ = 0;
	if(RegCreateKeyEx(base_key, path, 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS|wow, NULL, &ключ, NULL) != ERROR_SUCCESS)
		return false;
	bool ok = (RegSetValueEx(ключ, значение, 0,	REG_SZ, string, string.дайДлину() + 1) == ERROR_SUCCESS);
	RegCloseKey(ключ);
	return ok;
}

bool SetWinRegExpandString(const Ткст& string, const char *значение, const char *path, HKEY base_key, dword wow) {
	HKEY ключ = 0;
	if(RegCreateKeyEx(base_key, path, 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS|wow, NULL, &ключ, NULL) != ERROR_SUCCESS)
		return false;
	bool ok = (RegSetValueEx(ключ, значение, 0,	REG_EXPAND_SZ, string, string.дайДлину() + 1) == ERROR_SUCCESS);
	RegCloseKey(ключ);
	return ok;
}

bool устЦелВинРега(int данные, const char *значение, const char *path, HKEY base_key, dword wow)
{
	HKEY ключ = 0;
	if(RegCreateKeyEx(base_key, path, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS|wow, NULL, &ключ, NULL) != ERROR_SUCCESS)
		return false;
	bool ok = (RegSetValueEx(ключ, значение, 0, REG_DWORD, (const byte *)&данные, sizeof(данные)) == ERROR_SUCCESS);
	RegCloseKey(ключ);
	return ok;
}

void удалиВинРег(const Ткст& ключ, HKEY base, dword wow) {
	HKEY hkey;
	if(RegOpenKeyEx(base, ключ, 0, KEY_READ|wow, &hkey) != ERROR_SUCCESS)
		return;
	Вектор<Ткст> subkeys;
	char temp[_MAX_PATH];
	dword len;
	for(dword dw = 0; len = sizeof(temp), RegEnumKeyEx(hkey, dw, temp, &len, 0, 0, 0, 0) == ERROR_SUCCESS; dw++)
		subkeys.добавь(temp);
	RegCloseKey(hkey);
	while(!subkeys.пустой())
		удалиВинРег(ключ + '\\' + subkeys.вынь(), base);

	static LONG (WINAPI *RegDeleteKeyEx)(HKEY, LPCTSTR, REGSAM, DWORD);
	фнДлл(RegDeleteKeyEx, "Advapi32.dll", "RegDeleteKeyExA");

	if(wow && RegDeleteKeyEx)
		RegDeleteKeyEx(base, ключ, wow, 0);
	else
		RegDeleteKey(base, ключ);
}

Ткст GetSystemDirectory() {
	WCHAR temp[MAX_PATH];
	*temp = 0;
	::GetSystemDirectoryW(temp, sizeof(temp));
	return изСисНабсимаШ(temp);
}

Ткст GetWindowsDirectory() {
	WCHAR temp[MAX_PATH];
	*temp = 0;
	GetWindowsDirectoryW(temp, sizeof(temp));
	return изСисНабсимаШ(temp);
}

void *дайФнДлл(const char *dll, const char *фн)
{
	if(HMODULE hDLL = LoadLibrary(dll))
		return (void *)GetProcAddress(hDLL, фн);
	return NULL;
}

Ткст GetModuleFileName(HINSTANCE instance) {
	WCHAR h[_MAX_PATH];
	GetModuleFileNameW(instance, h, _MAX_PATH);
	return изСисНабсимаШ(h);
}

#ifdef DEPRECATED
bool СинхОбъект::жди(int ms)
{
	return WaitForSingleObject(handle, ms);
}

bool СинхОбъект::жди()
{
	return жди(INFINITE);
}

СинхОбъект::СинхОбъект()
{
	handle = NULL;
}

СинхОбъект::~СинхОбъект()
{
	if(handle) CloseHandle(handle);
}

СобытиеВин32::СобытиеВин32()
{
	handle = CreateEvent(NULL, FALSE, FALSE, NULL);
}

void СобытиеВин32::уст()
{
	SetEvent(handle);
}
#endif

#endif

}
