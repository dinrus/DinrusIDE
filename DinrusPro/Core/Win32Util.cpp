#include <DinrusPro/DinrusPro.h>

namespace ДинрусРНЦП {

#ifdef PLATFORM_WIN32

static HINSTANCE app_instance;

HINSTANCE прилДайУк()
{
	if(!app_instance)
		app_instance = GetModuleHandle(NULL);
	return app_instance;
}

проц прилУстУк(HINSTANCE dll_instance) { app_instance = dll_instance; }

#ifndef PLATFORM_WINCE
бул IsWin2K()
{
	OSVERSIONINFO of;
	of.dwOSVersionInfoSize = sizeof(of);
	GetVersionEx(&of);
	return of.dwMajorVersion >= 5;
}

бул IsWinXP()
{
	OSVERSIONINFO of;
	of.dwOSVersionInfoSize = sizeof(of);
	GetVersionEx(&of);
	return of.dwMajorVersion > 5 || of.dwMajorVersion == 5 && of.dwMinorVersion >= 1;
}

бул IsWinVista()
{
	OSVERSIONINFO of;
	of.dwOSVersionInfoSize = sizeof(of);
	GetVersionEx(&of);
	return of.dwMajorVersion >= 6;
}

бул IsWin7()
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
	return какТкст(буфер, (цел)wcslen(буфер));
}

Ткст какТкст(const wchar_t *буфер, цел count) { // Преобр with code page...
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
	return какТкст(буфер, (цел)(end - буфер));
}

Ткст дайТкстВинРега(кткст0 значение, кткст0 path, HKEY base_key, бцел wow) {
	HKEY ключ = 0;
	if(RegOpenKeyEx(base_key, path, 0, KEY_READ|wow, &ключ) != ERROR_SUCCESS)
		return Ткст::дайПроц();
	бцел тип, len;
	if(RegQueryValueEx(ключ, значение, 0, &тип, NULL, &len) != ERROR_SUCCESS)
	{
		RegCloseKey(ключ);
		return Ткст::дайПроц();
	}
	ТкстБуф raw_len(len);
	if(RegQueryValueEx(ключ, значение, 0, 0, (ббайт *)~raw_len, &len) != ERROR_SUCCESS)
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

цел дайЦелВинРега(кткст0 значение, кткст0 path, HKEY base_key, бцел wow) {
	HKEY ключ = 0;
	if(RegOpenKeyEx(base_key, path, 0, KEY_READ|wow, &ключ) != ERROR_SUCCESS)
		return Null;
	цел данные;
	бцел тип, length = sizeof(данные);
	if(RegQueryValueEx(ключ, значение, 0, &тип, (ббайт *)&данные, &length) != ERROR_SUCCESS)
		данные = Null;
	RegCloseKey(ключ);
	return данные;
}

бул устТкстВинРега(const Ткст& string, кткст0 значение, кткст0 path, HKEY base_key, бцел wow) {
	HKEY ключ = 0;
	if(RegCreateKeyEx(base_key, path, 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS|wow, NULL, &ключ, NULL) != ERROR_SUCCESS)
		return false;
	бул ok = (RegSetValueEx(ключ, значение, 0,	REG_SZ, string, string.дайДлину() + 1) == ERROR_SUCCESS);
	RegCloseKey(ключ);
	return ok;
}

бул SetWinRegExpandString(const Ткст& string, кткст0 значение, кткст0 path, HKEY base_key, бцел wow) {
	HKEY ключ = 0;
	if(RegCreateKeyEx(base_key, path, 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS|wow, NULL, &ключ, NULL) != ERROR_SUCCESS)
		return false;
	бул ok = (RegSetValueEx(ключ, значение, 0,	REG_EXPAND_SZ, string, string.дайДлину() + 1) == ERROR_SUCCESS);
	RegCloseKey(ключ);
	return ok;
}

бул устЦелВинРега(цел данные, кткст0 значение, кткст0 path, HKEY base_key, бцел wow)
{
	HKEY ключ = 0;
	if(RegCreateKeyEx(base_key, path, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS|wow, NULL, &ключ, NULL) != ERROR_SUCCESS)
		return false;
	бул ok = (RegSetValueEx(ключ, значение, 0, REG_DWORD, (const ббайт *)&данные, sizeof(данные)) == ERROR_SUCCESS);
	RegCloseKey(ключ);
	return ok;
}

проц удалиВинРег(const Ткст& ключ, HKEY base, бцел wow) {
	HKEY hkey;
	if(RegOpenKeyEx(base, ключ, 0, KEY_READ|wow, &hkey) != ERROR_SUCCESS)
		return;
	Вектор<Ткст> subkeys;
	char temp[_MAX_PATH];
	бцел len;
	for(бцел dw = 0; len = sizeof(temp), RegEnumKeyEx(hkey, dw, temp, &len, 0, 0, 0, 0) == ERROR_SUCCESS; dw++)
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

ук дайФнДлл(кткст0 dll, кткст0 фн)
{
	if(HMODULE hDLL = LoadLibrary(dll))
		return (проц *)GetProcAddress(hDLL, фн);
	return NULL;
}

Ткст GetModuleFileName(HINSTANCE instance) {
	WCHAR h[_MAX_PATH];
	GetModuleFileNameW(instance, h, _MAX_PATH);
	return изСисНабсимаШ(h);
}

#ifdef DEPRECATED
бул СинхОбъект::жди(цел ms)
{
	return WaitForSingleObject(handle, ms);
}

бул СинхОбъект::жди()
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

проц СобытиеВин32::уст()
{
	SetEvent(handle);
}
#endif

#endif

}
