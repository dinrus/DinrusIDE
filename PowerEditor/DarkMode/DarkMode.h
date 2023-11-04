#pragma once

extern bool g_darkModeSupported;
extern bool g_darkModeEnabled;


bool ShouldAppsUseDarkMode();
bool AllowDarkModeForWindow(Window* hWnd, bool allow);
bool IsHighContrast();
void RefreshTitleBarThemeColor(Window* hWnd);
void SetTitleBarThemeColor(Window* hWnd, BOOL dark);
bool IsColorSchemeChangeMessage(LPARAM lParam);
bool IsColorSchemeChangeMessage(UINT message, LPARAM lParam);
void AllowDarkModeForApp(bool allow);
void EnableDarkScrollBarForWindowAndChildren(Window* hwnd);
void InitDarkMode();
void SetDarkMode(bool useDarkMode, bool fixDarkScrollbar);
bool IsWindows11();
