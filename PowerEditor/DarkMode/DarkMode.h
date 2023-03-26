#pragma once

extern bool g_darkModeSupported;
extern bool g_darkModeEnabled;


bool ShouldAppsUseDarkMode();
bool AllowDarkModeForWindow(Upp::Ctrl* hWnd, bool allow);
bool IsHighContrast();
void RefreshTitleBarThemeColor(Upp::Ctrl* hWnd);
void SetTitleBarThemeColor(Upp::Ctrl* hWnd, BOOL dark);
bool IsColorSchemeChangeMessage(LPARAM lParam);
bool IsColorSchemeChangeMessage(UINT message, LPARAM lParam);
void AllowDarkModeForApp(bool allow);
void EnableDarkScrollBarForWindowAndChildren(Upp::Ctrl* hwnd);
void InitDarkMode();
void SetDarkMode(bool useDarkMode, bool fixDarkScrollbar);
bool IsWindows11();
