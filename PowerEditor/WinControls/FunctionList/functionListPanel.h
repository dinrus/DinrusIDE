// This file is part of Notepad++ project
// Copyright (C)2021 Don HO <don.h@free.fr>

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

#include <PowerEditor/WinControls/DockingWnd/DockingDlgInterface.h>
#include "functionListPanel_rc.h"
#include "functionParser.h"
#include <PowerEditor/WinControls/TreeView/TreeView.h>

#define FL_PANELTITLE     TEXT("Function List")
#define FL_FUCTIONLISTROOTNODE "FunctionList"

#define FL_SORTLOCALNODENAME        "SortTip"
#define FL_RELOADLOCALNODENAME      "ReloadTip"
#define FL_PREFERENCESLOCALNODENAME "PreferencesTip"

#define FL_PREFERENCE_INITIALSORT "PreferencesInitialSort"

class ScintillaEditView;

/*
1. global function + object + methods: Tree view of 2 levels - only the leaf contains the position info
root
|
|---leaf
|
|---node
|    |
|    |---leaf
|    |
|    |---leaf
|
|---node
     |
     |---leaf

2. each rule associates with file kind. For example, c_def (for *.c), cpp_def (for *.cpp) cpp_header (for *h) java_def (for *.java)...etc.



*/

struct SearchParameters {
	String _text2Find;
	bool _doSort = false;

	bool hasParams()const{
		return (_text2Find != TEXT("") || _doSort);
	};
};

struct TreeParams {
	TreeStateNode _treeState;
	SearchParameters _searchParameters;
};

class FunctionListPanel : public DockingDlgInterface {
public:
	FunctionListPanel(): DockingDlgInterface(IDD_FUNCLIST_PANEL), _pTreeView(&_treeView) {};
	~FunctionListPanel();

	void init(Ctrl& hInst, Upp::Ctrl* hPere, ScintillaEditView **ppEditView);

    virtual void display(bool toShow = true) const {
        DockingDlgInterface::display(toShow);
    };

	virtual void setBackgroundColor(Color& bgColour) {
		TreeView_SetBkColor(_treeView.getHSelf(), bgColour);
		TreeView_SetBkColor(_treeViewSearchResult.getHSelf(), bgColour);
    };
	virtual void setForegroundColor(Color& fgColour) {
		TreeView_SetTextColor(_treeView.getHSelf(), fgColour);
		TreeView_SetTextColor(_treeViewSearchResult.getHSelf(), fgColour);
    };

    void setParent(Upp::Ctrl* parent2set){
        _hParent = parent2set;
    };
	
	// functionalities
	static int CALLBACK categorySortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM /*lParamSort*/);
	void sortOrUnsort();
	void reload();
	void markEntry();
	bool serialize(const String & outputFilename = TEXT(""));
	void addEntry(const char *node, const char *displayText, size_t pos);
	void removeAllEntries();
	void searchFuncAndSwitchView();

protected:
	virtual intptr_t CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
	Menu* _hPreferencesMenu = Null;

private:
	Upp::Ctrl* _hToolbarMenu = nullptr;
	Upp::Ctrl* _hSearchEdit = nullptr;

	TreeView *_pTreeView = nullptr;
	TreeView _treeView;
	TreeView _treeViewSearchResult;

	SCROLLINFO si = {};
	long _findLine = -1;
	long _findEndLine = -1;
	HTREEITEM _findItem = nullptr;

	String _sortTipStr = TEXT("Sort");
	String _reloadTipStr = TEXT("Reload");
	String _preferenceTipStr = TEXT("Preferences");

	std::vector<foundInfo> _foundFuncInfos;

	std::vector<String*> _posStrs;

	ScintillaEditView **_ppEditView = nullptr;
	FunctionParsersManager _funcParserMgr;
	std::vector< std::pair<int, int> > _skipZones;
	std::vector<TreeParams> _treeParams;
	HIMAGELIST _hTreeViewImaLst = nullptr;

	String parseSubLevel(size_t begin, size_t end, std::vector< String > dataToSearch, intptr_t& foundPos);
	size_t getBodyClosePos(size_t begin, const char *bodyOpenSymbol, const char *bodyCloseSymbol);
	void notified(LPNMHDR notification);
	void addInStateArray(TreeStateNode tree2Update, const char *searchText, bool isSorted);
	TreeParams* getFromStateArray(String fullFilePath);
	bool openSelection(const TreeView &treeView);
	bool shouldSort();
	void setSort(bool isEnabled);
	void findMarkEntry(HTREEITEM htItem, LONG line);
	void initPreferencesMenu();
	void showPreferencesMenu();
};

