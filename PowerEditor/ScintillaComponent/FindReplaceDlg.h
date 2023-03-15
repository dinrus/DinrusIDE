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

#include <map>
#include "FindReplaceDlg_rc.h"
#include <PowerEditor/ScintillaComponent/ScintillaEditView.h>
#include <PowerEditor/WinControls/DockingWnd/DockingDlgInterface.h>
#include <Scintilla/BoostRegexSearch.h>
#include <PowerEditor/WinControls/StatusBar/StatusBar.h>

#define FIND_RECURSIVE 1
#define FIND_INHIDDENDIR 2

#define FINDREPLACE_MAXLENGTH 2048

enum DIALOG_TYPE {FIND_DLG, REPLACE_DLG, FINDINFILES_DLG, FINDINPROJECTS_DLG, MARK_DLG};

#define DIR_DOWN true
#define DIR_UP false

//#define FIND_REPLACE_STR_MAX 256

enum InWhat{ALL_OPEN_DOCS, FILES_IN_DIR, CURRENT_DOC, CURR_DOC_SELECTION, FILES_IN_PROJECTS};

struct FoundInfo {
	FoundInfo(intptr_t start, intptr_t end, size_t lineNumber, const char *fullPath)
		: _lineNumber(lineNumber), _fullPath(fullPath) {
		_ranges.push_back(std::pair<intptr_t, intptr_t>(start, end));
	};
	std::vector<std::pair<intptr_t, intptr_t>> _ranges;
	size_t _lineNumber = 0;
	String _fullPath;
};

struct TargetRange {
	int targetStart;
	int targetEnd;
};

enum SearchIncrementalType { NotIncremental, FirstIncremental, NextIncremental };
enum SearchType { FindNormal, FindExtended, FindRegex };
enum ProcessOperation { ProcessFindAll, ProcessReplaceAll, ProcessCountAll, ProcessMarkAll, ProcessMarkAll_2, ProcessMarkAll_IncSearch, ProcessMarkAllExt, ProcessFindInFinder };

struct FindOption
{
	bool _isWholeWord = true;
	bool _isMatchCase = true;
	bool _isWrapAround = true;
	bool _whichDirection = DIR_DOWN;
	SearchIncrementalType _incrementalType = NotIncremental;
	SearchType _searchType = FindNormal;
	bool _doPurge = false;
	bool _doMarkLine = false;
	bool _isInSelection = false;
	String _str2Search;
	String _str4Replace;
	String _filters;
	String _directory;
	bool _isRecursive = true;
	bool _isInHiddenDir = false;
	bool _isProjectPanel_1 = false;
	bool _isProjectPanel_2 = false;
	bool _isProjectPanel_3 = false;
	bool _dotMatchesNewline = false;
	bool _isMatchLineNumber = true; // only for Find in Folder
};

//This class contains generic search functions as static functions for easy access
class Searching {
public:
	static int convertExtendedToString(const char * query, char * result, int length);
	static TargetRange t;
	static int buildSearchFlags(const FindOption * option) {
		return	(option->_isWholeWord ? SCFIND_WHOLEWORD : 0) |
				(option->_isMatchCase ? SCFIND_MATCHCASE : 0) |
				(option->_searchType == FindRegex ? SCFIND_REGEXP|SCFIND_POSIX : 0) |
				((option->_searchType == FindRegex && option->_dotMatchesNewline) ? SCFIND_REGEXP_DOTMATCHESNL : 0);
	};
	static void displaySectionCentered(size_t posStart, size_t posEnd, ScintillaEditView * pEditView, bool isDownwards = true);

private:
	static bool readBase(const char * str, int * value, int base, int size);

};

//Finder: Dockable window that contains search results
class Finder : public DockingDlgInterface {
friend class FindReplaceDlg;
public:

	Finder() : DockingDlgInterface(IDD_FINDRESULT) {
		_markingsStruct._length = 0;
		_markingsStruct._markings = NULL;
	};

	~Finder() {
		_scintView.destroy();
	}
	void init(HINSTANCE hInst, HWND hPere, ScintillaEditView **ppEditView) {
		DockingDlgInterface::init(hInst, hPere);
		_ppEditView = ppEditView;
	};

	void addSearchLine(const char *searchName);
	void addFileNameTitle(const char * fileName);
	void addFileHitCount(int count);
	void addSearchHitCount(int count, int countSearched, bool isMatchLines, bool searchedEntireNotSelection);
	void add(FoundInfo fi, SearchResultMarkingLine mi, const char* foundline, size_t totalLineNumber);
	void setFinderStyle();
	void removeAll();
	void openAll();
	void wrapLongLinesToggle();
	void purgeToggle();
	void copy();
	void copyPathnames();
	void beginNewFilesSearch();
	void finishFilesSearch(int count, int searchedCount, bool isMatchLines, bool searchedEntireNotSelection);
	
	void gotoNextFoundResult(int direction);
	std::pair<intptr_t, intptr_t> gotoFoundLine(size_t nOccurrence = 0); // value 0 means this argument is not used
	void deleteResult();
	Vector<String> getResultFilePaths() const;
	bool canFind(const char *fileName, size_t lineNumber) const;
	void setVolatiled(bool val) { _canBeVolatiled = val; };
	String getHitsString(int count) const;

protected :
	virtual intptr_t CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
	bool notify(SCNotification *notification);

private:
	enum { searchHeaderLevel = SC_FOLDLEVELBASE, fileHeaderLevel, resultLevel };

	enum CurrentPosInLineStatus { pos_infront, pos_between, pos_inside, pos_behind };

	struct CurrentPosInLineInfo {
		CurrentPosInLineStatus _status;
		intptr_t auxiliaryInfo = -1; // according the status
	};

	ScintillaEditView **_ppEditView = nullptr;
	std::vector<FoundInfo> _foundInfos1;
	std::vector<FoundInfo> _foundInfos2;
	std::vector<FoundInfo>* _pMainFoundInfos = &_foundInfos1;
	std::vector<SearchResultMarkingLine> _markings1;
	std::vector<SearchResultMarkingLine> _markings2;
	std::vector<SearchResultMarkingLine>* _pMainMarkings = &_markings1;
	SearchResultMarkings _markingsStruct;
	intptr_t _previousLineNumber = -1;

	ScintillaEditView _scintView;
	unsigned int _nbFoundFiles = 0;

	intptr_t _lastFileHeaderPos = 0;
	intptr_t _lastSearchHeaderPos = 0;

	bool _canBeVolatiled = true;
	bool _longLinesAreWrapped = false;
	bool _purgeBeforeEverySearch = false;

	String _prefixLineStr;

	void setFinderReadOnly(bool isReadOnly) {
		_scintView.execute(SCI_SETREADONLY, isReadOnly);
	};

	bool isLineActualSearchResult(const String & s) const;
	String & prepareStringForClipboard(String & s) const;

	static FoundInfo EmptyFoundInfo;
	static SearchResultMarkingLine EmptySearchResultMarking;

	CurrentPosInLineInfo getCurrentPosInLineInfo(intptr_t currentPosInLine, const SearchResultMarkingLine& markingLine) const;
	void anchorWithNoHeaderLines(intptr_t& currentL, intptr_t initL, intptr_t minL, intptr_t maxL, int direction);
};


enum FindStatus { FSFound, FSNotFound, FSTopReached, FSEndReached, FSMessage, FSNoMessage};

enum FindNextType {
	FINDNEXTTYPE_FINDNEXT,
	FINDNEXTTYPE_REPLACENEXT,
	FINDNEXTTYPE_FINDNEXTFORREPLACE
};

struct FindReplaceInfo
{
	const char *_txt2find = nullptr;
	const char *_txt2replace = nullptr;
	intptr_t _startRange = -1;
	intptr_t _endRange = -1;
};

struct FindersInfo
{
	Finder *_pSourceFinder = nullptr;
	Finder *_pDestFinder = nullptr;
	const char *_pFileName = nullptr;

	FindOption _findOption;
};

class FindInFinderDlg : public StaticDialog
{
public:
	void init(HINSTANCE hInst, HWND hPere) {
		Window::init(hInst, hPere);
	};
	void doDialog(Finder *launcher, bool isRTL = false);
	FindOption & getOption() { return _options; }

private:
	Finder  *_pFinder2Search = nullptr;
	FindOption _options;
	
	virtual intptr_t CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
	void initFromOptions();
	void writeOptions();
};

class FindReplaceDlg : public StaticDialog
{
friend class FindIncrementDlg;
public :
	static FindOption _options;
	static FindOption* _env;
	FindReplaceDlg() {
		_uniFileName = new char[(_fileNameLenMax + 3) * 2];
		_winVer = (NppParameters::getInstance()).getWinVersion();
		_env = &_options;
	};

	~FindReplaceDlg();

	void init(HINSTANCE hInst, HWND hPere, ScintillaEditView **ppEditView) {
		Window::init(hInst, hPere);
		if (!ppEditView)
			throw std::runtime_error("FindIncrementDlg::init : ppEditView is null.");
		_ppEditView = ppEditView;
	};

	virtual void create(int dialogID, bool isRTL = false, bool msgDestParent = true);
	
	void initOptionsFromDlg();

	void doDialog(DIALOG_TYPE whichType, bool isRTL = false, bool toShow = true);
	bool processFindNext(const char *txt2find, const FindOption *options = NULL, FindStatus *oFindStatus = NULL, FindNextType findNextType = FINDNEXTTYPE_FINDNEXT);
	bool processReplace(const char *txt2find, const char *txt2replace, const FindOption *options = NULL);

	int markAll(const char *txt2find, int styleID);
	int markAllInc(const FindOption *opt);
	

	int processAll(ProcessOperation op, const FindOption *opt, bool isEntire = false, const FindersInfo *pFindersInfo = nullptr, int colourStyleID = -1);
	int processRange(ProcessOperation op, FindReplaceInfo & findReplaceInfo, const FindersInfo *pFindersInfo, const FindOption *opt = nullptr, int colourStyleID = -1, ScintillaEditView *view2Process = nullptr);

	void replaceAllInOpenedDocs();
	void findAllIn(InWhat op);
	void setSearchText(char * txt2find);

	void gotoNextFoundResult(int direction = 0) {if (_pFinder) _pFinder->gotoNextFoundResult(direction);};

	void putFindResult(int result) {
		_findAllResult = result;
	};
	const char * getDir2Search() const {return _env->_directory.Begin();};

	void getPatterns(Vector<String> & patternVect);
	void getAndValidatePatterns(Vector<String> & patternVect);

	void launchFindInFilesDlg() {
		doDialog(FINDINFILES_DLG);
	};

	void launchFindInProjectsDlg() {
		doDialog(FINDINPROJECTS_DLG);
	};

	void setFindInFilesDirFilter(const char *dir, const char *filters);
	void setProjectCheckmarks(FindHistory *findHistory, int Msk);
	void enableProjectCheckmarks();

	String getText2search() const {
		return _env->_str2Search;
	};

	const String & getFilters() const {return _env->_filters;};
	const String & getDirectory() const {return _env->_directory;};
	const FindOption & getCurrentOptions() const {return *_env;};
	bool isRecursive() const { return _env->_isRecursive; };
	bool isInHiddenDir() const { return _env->_isInHiddenDir; };
	bool isProjectPanel_1() const { return _env->_isProjectPanel_1; };
	bool isProjectPanel_2() const { return _env->_isProjectPanel_2; };
	bool isProjectPanel_3() const { return _env->_isProjectPanel_3; };
	void saveFindHistory();
	void changeTabName(DIALOG_TYPE index, const char *name2change) {
		TCITEM tie;
		tie.mask = TCIF_TEXT;
		tie.pszText = (char *)name2change;
		TabCtrl_SetItem(_tab.getHSelf(), index, &tie);

		char label[MAX_PATH];
		_tab.getCurrentTitle(label, MAX_PATH);
		::SetWindowText(_hSelf, label);
	}
	void beginNewFilesSearch()
	{
		_pFinder->beginNewFilesSearch();
		_pFinder->addSearchLine(getText2search().Begin());
	}

	void finishFilesSearch(int count, int searchedCount, bool searchedEntireNotSelection)
	{
		const bool isMatchLines = false;
		_pFinder->finishFilesSearch(count, searchedCount, isMatchLines, searchedEntireNotSelection);
	}

	void focusOnFinder() {
		// Show finder and set focus
		if (_pFinder) 
		{
			_pFinder->display();
			_pFinder->_scintView.getFocus();
		}
	};

	HWND getHFindResults() {
		if (_pFinder)
			return _pFinder->_scintView.getHSelf();
		return NULL;
	}

	void updateFinderScintilla() {
		if (_pFinder && _pFinder->isCreated() && _pFinder->isVisible())
		{
			_pFinder->setFinderStyle();
		}
	};

	void execSavedCommand(int cmd, uptr_t intValue, const String& stringValue);
	void clearMarks(const FindOption& opt);
	void setStatusbarMessage(const String & msg, FindStatus staus, char const *pTooltipMsg = NULL);
	String getScopeInfoForStatusBar(FindOption const *pFindOpt) const;
	Finder * createFinder();
	bool removeFinder(Finder *finder2remove);
	DIALOG_TYPE getCurrentStatus() {return _currentStatus;};

protected :
	void resizeDialogElements(LONG newWidth);
	virtual intptr_t CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
	static LONG_PTR originalFinderProc;
	static LONG_PTR originalComboEditProc;

	static LRESULT FAR PASCAL comboEditProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	// Window procedure for the finder
	static LRESULT FAR PASCAL finderProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

    void combo2ExtendedMode(int comboID);

private :
	RECT _initialWindowRect = {};
	LONG _deltaWidth = 0;
	LONG _initialClientWidth = 0;

	DIALOG_TYPE _currentStatus = DIALOG_TYPE::FIND_DLG;
	RECT _findClosePos, _replaceClosePos, _findInFilesClosePos, _markClosePos;
	RECT _countInSelFramePos, _replaceInSelFramePos;
	RECT _countInSelCheckPos, _replaceInSelCheckPos;

	ScintillaEditView **_ppEditView = nullptr;
	Finder  *_pFinder = nullptr;
	String _findResTitle;

	std::vector<Finder*> _findersOfFinder{};

	HWND _shiftTrickUpTip = nullptr;
	HWND _2ButtonsTip = nullptr;
	HWND _filterTip = nullptr;

	bool _isRTL = false;

	int _findAllResult;
	char _findAllResultStr[1024] = {'\0'};

	int _fileNameLenMax = 1024;
	char *_uniFileName = nullptr;

	TabBar _tab;
	winVer _winVer = winVer::WV_UNKNOWN;
	StatusBar _statusBar;
	FindStatus _statusbarFindStatus;

	String _statusbarTooltipMsg;
	HWND _statusbarTooltipWnd = nullptr;
	HICON _statusbarTooltipIcon = nullptr;
	int _statusbarTooltipIconSize = 0;

	HFONT _hMonospaceFont = nullptr;
	HFONT _hLargerBolderFont = nullptr;

	std::map<int, bool> _controlEnableMap;

	void enableFindDlgItem(int dlgItemID, bool isEnable = true);
	void showFindDlgItem(int dlgItemID, bool isShow = true);

	void enableReplaceFunc(bool isEnable);
	void enableFindInFilesControls(bool isEnable, bool projectPanels);
	void enableFindInFilesFunc();
	void enableFindInProjectsFunc();
	void enableMarkAllControls(bool isEnable);
	void enableMarkFunc();

	void setDefaultButton(int nID) {
		SendMessage(_hSelf, DM_SETDEFID, nID, 0L);
	};

	void gotoCorrectTab() {
		auto currentIndex = _tab.getCurrentTabIndex();
		if (currentIndex != _currentStatus)
			_tab.activateAt(_currentStatus);
	};
	
	FindStatus getFindStatus() {
		return this->_statusbarFindStatus;
	}

	void updateCombos();
	void updateCombo(int comboID);
	void fillFindHistory();
    void fillComboHistory(int id, const Vector<String> & strings);
	int saveComboHistory(int id, int maxcount, Vector<String> & strings, bool saveEmpty);
	static const int FR_OP_FIND = 1;
	static const int FR_OP_REPLACE = 2;
	static const int FR_OP_FIF = 4;
	static const int FR_OP_GLOBAL = 8;
	static const int FR_OP_FIP = 16;
	void saveInMacro(size_t cmd, int cmdType);
	void drawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	bool replaceInFilesConfirmCheck(String directory, String fileTypes);
	bool replaceInProjectsConfirmCheck();
	bool replaceInOpenDocsConfirmCheck(void);
};

//FindIncrementDlg: incremental search dialog, docked in rebar
class FindIncrementDlg : public StaticDialog
{
public :
	FindIncrementDlg() = default;
	void init(HINSTANCE hInst, HWND hPere, FindReplaceDlg *pFRDlg, bool isRTL = false);
	virtual void destroy();
	virtual void display(bool toShow = true) const;

	void setSearchText(const char* txt2find, bool) {
		::SendDlgItemMessage(_hSelf, IDC_INCFINDTEXT, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(txt2find));
	};

	void setFindStatus(FindStatus iStatus, int nbCounted);
	
	FindStatus getFindStatus() {
		return _findStatus;
	}

	void addToRebar(ReBar* rebar);
private :
	bool _isRTL = false;
	FindReplaceDlg *_pFRDlg = nullptr;
	FindStatus _findStatus = FSFound;

	ReBar* _pRebar = nullptr;
	REBARBANDINFO _rbBand = {};

	virtual intptr_t CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
	void markSelectedTextInc(bool enable, FindOption *opt = NULL);
};


class Progress
{
public:
	explicit Progress(HINSTANCE hInst);
	~Progress();

	// Disable copy construction and operator=
	Progress(const Progress&) = delete;
	const Progress& operator=(const Progress&) = delete;

	HWND open(HWND hCallerWnd, const char* header = NULL);
	void close();

	bool isCancelled() const
	{
		if (_hwnd)
			return (::WaitForSingleObject(_hActiveState, 0) != WAIT_OBJECT_0);
		return false;
	}

	void setInfo(const char *info) const
	{
		if (_hwnd)
			::SendMessage(_hPText, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(info));
	}

	void setPercent(unsigned percent, const char *fileName) const;

private:
	static const char cClassName[];
	static const char cDefaultHeader[];
	static const int cBackgroundColor;
	static const int cPBwidth;
	static const int cPBheight;
	static const int cBTNwidth;
	static const int cBTNheight;

	static volatile LONG refCount;

	static DWORD WINAPI threadFunc(LPVOID data);
	static LRESULT APIENTRY wndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

	int thread();
	int createProgressWindow();

	HINSTANCE _hInst = nullptr;
	volatile HWND _hwnd = nullptr;
	HWND _hCallerWnd = nullptr;
	char _header[128] = {'\0'};
	HANDLE _hThread = nullptr;
	HANDLE _hActiveState = nullptr;
	HWND _hPText = nullptr;
	HWND _hPBar = nullptr;
	HWND _hBtn = nullptr;
};

