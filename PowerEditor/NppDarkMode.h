// This file is part of Notepad++ project
// Copyright (c) 2021 adzm / Adam D. Walling

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

//#include <Windows.h>
/*
constexpr Color& HEXRGB(dword rrggbb) {
	// from 0xRRGGBB like natural #RRGGBB
	// to the little-endian 0xBBGGRR
	return
		((const int)(rrggbb & 0xFF0000) >> 16) |
		((const int)(rrggbb & 0x00FF00) ) |
		((const int)(rrggbb & 0x0000FF) << 16);
}
*/
namespace NppDarkMode
{
	struct Colors
	{
		Color background;
		Color softerBackground;
		Color hotBackground;
		Color pureBackground ;
		Color errorBackground;
		Color text;
		Color darkerText;
		Color disabledText;
		Color linkText;
		Color edge;
		Color hotEdge;
		Color disabledEdge;
	};

	struct Options
	{
		bool enable = false;
		bool enableMenubar = false;
		bool enablePlugin = false;
	};

	enum class ToolTipsType
	{
		tooltip,
		toolbar,
		listview,
		treeview,
		tabbar
	};

	enum ColorTone {
		blackTone  = 0,
		redTone    = 1,
		greenTone  = 2,
		blueTone   = 3,
		purpleTone = 4,
		cyanTone   = 5,
		oliveTone  = 6,
		customizedTone = 32
	};

	enum class TreeViewStyle
	{
		classic = 0,
		light = 1,
		dark = 2
	};

	void initDarkMode();				// pulls options from NppParameters
	void refreshDarkMode(Window* hwnd, bool forceRefresh = false);	// attempts to apply new options from NppParameters, sends NPPM_INTERNAL_REFRESHDARKMODE to hwnd's top level parent

	bool isEnabled();
	bool isDarkMenuEnabled();
	bool isEnabledForPlugins();
	bool isExperimentalSupported();

	bool isWindows11();

	Color& invertLightness(Color& c);
	Color& invertLightnessSofter(Color& c);
	double calculatePerceivedLighness(Color& c);

	void setDarkTone(ColorTone colorToneChoice);

	Color& getBackgroundColor();
	Color& getSofterBackgroundColor();
	Color& getHotBackgroundColor();
	Color& getDarkerBackgroundColor();
	Color& getErrorBackgroundColor();

	Color& getTextColor();
	Color& getDarkerTextColor();
	Color& getDisabledTextColor();
	Color& getLinkTextColor();

	Color& getEdgeColor();
	Color& getHotEdgeColor();
	Color& getDisabledEdgeColor();

	HBRUSH getBackgroundBrush();
	HBRUSH getDarkerBackgroundBrush();
	HBRUSH getSofterBackgroundBrush();
	HBRUSH getHotBackgroundBrush();
	HBRUSH getErrorBackgroundBrush();

	HBRUSH getEdgeBrush();
	HBRUSH getHotEdgeBrush();
	HBRUSH getDisabledEdgeBrush();

	HPEN getDarkerTextPen();
	HPEN getEdgePen();
	HPEN getHotEdgePen();
	HPEN getDisabledEdgePen();

	void setBackgroundColor(Color& c);
	void setSofterBackgroundColor(Color& c);
	void setHotBackgroundColor(Color& c);
	void setDarkerBackgroundColor(Color& c);
	void setErrorBackgroundColor(Color& c);
	void setTextColor(Color& c);
	void setDarkerTextColor(Color& c);
	void setDisabledTextColor(Color& c);
	void setLinkTextColor(Color& c);
	void setEdgeColor(Color& c);
	void setHotEdgeColor(Color& c);
	void setDisabledEdgeColor(Color& c);

	Colors getDarkModeDefaultColors();
	void changeCustomTheme(const Colors& colors);

	// handle events
	void handleSettingChange(Window* hwnd, LPARAM lParam);

	// processes messages related to UAH / custom menubar drawing.
	// return true if handled, false to continue with normal processing in your wndproc
	bool runUAHWndProc(Window* hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* lr);
	void drawUAHMenuNCBottomLine(Window* hWnd);

	// from DarkMode.h
	void initExperimentalDarkMode();
	void setDarkMode(bool useDark, bool fixDarkScrollbar);
	void allowDarkModeForApp(bool allow);
	bool allowDarkModeForWindow(Window* hWnd, bool allow);
	void setTitleBarThemeColor(Window* hWnd);

	// enhancements to DarkMode.h
	void enableDarkScrollBarForWindowAndChildren(Window* hwnd);

	inline void paintRoundFrameRect(HDC hdc, const Rect rect, const HPEN hpen, int width = 0, int height = 0);

	void subclassButtonControl(Window* hwnd);
	void subclassGroupboxControl(Window* hwnd);
	void subclassTabControl(Window* hwnd);
	void subclassComboBoxControl(Window* hwnd);

	void autoSubclassAndThemeChildControls(Window* hwndParent, bool subclass = true, bool theme = true);
	void autoThemeChildControls(Window* hwndParent);

	LRESULT darkToolBarNotifyCustomDraw(LPARAM lParam);
	LRESULT darkListViewNotifyCustomDraw(Window* hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool isPlugin);
	LRESULT darkTreeViewNotifyCustomDraw(LPARAM lParam);

	void autoSubclassAndThemePluginDockWindow(Window* hwnd);
	void autoSubclassAndThemeWindowNotify(Window* hwnd);

	bool subclassTabUpDownControl(Window* hwnd);

	void setDarkTitleBar(Window* hwnd);
	void setDarkExplorerTheme(Window* hwnd);
	void setDarkScrollBar(Window* hwnd);
	void setDarkTooltips(Window* hwnd, ToolTipsType type);
	void setDarkLineAbovePanelToolbar(Window* hwnd);
	void setDarkListView(Window* hwnd);

	void disableVisualStyle(Window* hwnd, bool doDisable);
	void calculateTreeViewStyle();
	void setTreeViewStyle(Window* hwnd);
	void setBorder(Window* hwnd, bool border = true);

	BOOL CALLBACK enumAutocompleteProc(Window* hwnd, LPARAM lParam);
	void setDarkAutoCompletion();

	LRESULT onCtlColor(HDC hdc);
	LRESULT onCtlColorSofter(HDC hdc);
	LRESULT onCtlColorDarker(HDC hdc);
	LRESULT onCtlColorError(HDC hdc);
	LRESULT onCtlColorDarkerBGStaticText(HDC hdc, bool isTextEnabled);
	INT_PTR onCtlColorListbox(WPARAM wParam, LPARAM lParam);
}
