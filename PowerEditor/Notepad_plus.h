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
#include <Core/Core.h>
#include <PowerEditor/ScintillaComponent/ScintillaComponent.h>
#include <PowerEditor/WinControls/SplitterContainer/SplitterContainer.h>
#include <PowerEditor/WinControls/AboutDlg/AboutDlg.h>
#include <PowerEditor/WinControls/StaticDialog/RunDlg/RunDlg.h>
#include <PowerEditor/WinControls/StatusBar/StatusBar.h>
#include <PowerEditor/lastRecentFileList.h>
#include <PowerEditor/WinControls/FindCharsInRange/FindCharsInRange.h>
#include <PowerEditor/WinControls/ColourPicker/WordStyleDlg.h>
#include <PowerEditor/WinControls/TrayIcon/trayIconControler.h>
#include <PowerEditor/MISC/PluginsManager/PluginsManager.h>
#include <PowerEditor/WinControls/Preference/preferenceDlg.h>
#include <PowerEditor/WinControls/WindowsDlg/WindowsDlg.h>
#include <PowerEditor/WinControls/shortcut/RunMacroDlg.h>
#include <PowerEditor/WinControls/DockingWnd/DockingManager.h>
#include <PowerEditor/MISC/Process/Processus.h>
#include "lesDlgs.h"
#include <PowerEditor/WinControls/PluginsAdmin/pluginsAdmin.h>
#include <PowerEditor/localization.h>
#include <PowerEditor/WinControls/DocumentMap/documentSnapshot.h>
#include <PowerEditor/MISC/md5/md5Dlgs.h>
#include <vector>
#include <iso646.h>


#define MENU 0x01
#define TOOLBAR 0x02

enum FileTransferMode {
	TransferClone		= 0x01,
	TransferMove		= 0x02
};

enum WindowStatus {	//bitwise mask
	WindowMainActive	= 0x01,
	WindowSubActive		= 0x02,
	WindowBothActive	= 0x03,	//little helper shortcut
	WindowUserActive	= 0x04,
	WindowMask			= 0x07
};

enum trimOp {
	lineHeader = 0,
	lineTail = 1,
	lineEol = 2
};

enum spaceTab {
	tab2Space = 0,
	space2TabLeading = 1,
	space2TabAll = 2
};

struct TaskListInfo;


struct VisibleGUIConf final
{
	bool _isPostIt = false;
	bool _isFullScreen = false;
	bool _isDistractionFree = false;

	//Used by postit & fullscreen
	bool _isMenuShown = true;
	DWORD_PTR _preStyle = (WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN);

	//used by postit
	bool _isTabbarShown = true;
	bool _isAlwaysOnTop = false;
	bool _isStatusbarShown = true;

	//used by fullscreen
	WINDOWPLACEMENT _winPlace = {};

	//used by distractionFree
	bool _was2ViewModeOn = false;
	std::vector<DockingCont*> _pVisibleDockingContainers;
};

struct QuoteParams
{
	enum Speed { slow = 0, rapid, speedOfLight };

	QuoteParams() {};
	QuoteParams(const wchar_t* quoter, Speed speed, bool shouldBeTrolling, int encoding, LangType lang, const wchar_t* quote) :
		_quoter(quoter), _speed(speed), _shouldBeTrolling(shouldBeTrolling), _encoding(encoding), _lang(lang), _quote(quote) {}

	void reset() {
		_quoter = nullptr;
		_speed = rapid;
		_shouldBeTrolling = false;
		_encoding = SC_CP_UTF8;
		_lang = L_TEXT;
		_quote = nullptr;
	};

	const wchar_t* _quoter = nullptr;
	Speed _speed = rapid;
	bool _shouldBeTrolling = false;
	int _encoding = SC_CP_UTF8;
	LangType _lang = L_TEXT;
	const wchar_t* _quote = nullptr;
};

class CustomFileDialog;
class Notepad_plus_Window;
class AnsiCharPanel;
class ClipboardHistoryPanel;
class VerticalFileSwitcher;
class ProjectPanel;
class DocumentMap;
class FunctionListPanel;
class FileBrowser;
struct QuoteParams;

class Notepad_plus final
{
friend class Notepad_plus_Window;
friend class FileManager;

public:
	Notepad_plus();
	~Notepad_plus();

	LRESULT init(Window* hwnd);
	LRESULT process(Window* hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
	void killAllChildren();

	enum comment_mode {cm_comment, cm_uncomment, cm_toggle};

	void setTitle();
	void getTaskListInfo(TaskListInfo *tli);
	size_t getNbDirtyBuffer(int view);
	// For filtering the modeless Dialog message

	//! \name File Operations
	//@{
	//The doXXX functions apply to a single buffer and dont need to worry about views, with the excpetion of doClose, since closing one view doesnt have to mean the document is gone
	BufferID doOpen(const char* fileName, bool isRecursive = false, bool isReadOnly = false, int encoding = -1, const char *backupFileName = nullptr, FILETIME fileNameTimestamp = {});
	bool doReload(BufferID id, bool alert = true);
	bool doSave(BufferID, const char * filename, bool isSaveCopy = false);
	void doClose(BufferID, int whichOne, bool doDeleteBackup = false);
	//bool doDelete(const char *fileName) const {return ::DeleteFile(fileName) != 0;};

	void fileOpen();
	void fileNew();
    bool fileReload();
	bool fileClose(BufferID id = BUFFER_INVALID, int curView = -1);	//use curView to override view to close from
	bool fileCloseAll(bool doDeleteBackup, bool isSnapshotMode = false);
	bool fileCloseAllButCurrent();
	bool fileCloseAllGiven(const std::vector<int>& krvecBufferIndexes);
	bool fileCloseAllToLeft();
	bool fileCloseAllToRight();
	bool fileCloseAllUnchanged();
	bool fileSave(BufferID id = BUFFER_INVALID);
	bool fileSaveAllConfirm();
	bool fileSaveAll();
	bool fileSaveSpecific(const char* fileNameToSave);
	bool fileSaveAs(BufferID id = BUFFER_INVALID, bool isSaveCopy = false);
	bool fileDelete(BufferID id = BUFFER_INVALID);
	bool fileRename(BufferID id = BUFFER_INVALID);

	bool switchToFile(BufferID buffer);			//find buffer in active view then in other view.
	//@}

	bool isFileSession(const char * filename);
	bool isFileWorkspace(const char * filename);
	void filePrint(bool showDialog);
	void saveScintillasZoom();

	bool saveGUIParams();
	bool saveProjectPanelsParams();
	bool saveFileBrowserParam();
	void saveDockingParams();
    void saveUserDefineLangs();
    void saveShortcuts();
	void saveSession(const Session & session);
	void saveCurrentSession();
	void saveFindHistory();

	void getCurrentOpenedFiles(Session& session, bool includUntitledDoc = false);

	bool fileLoadSession(const char* fn = nullptr);
	const char * fileSaveSession(size_t nbFile, char ** fileNames, const char *sessionFile2save, bool includeFileBrowser = false);
	const char * fileSaveSession(size_t nbFile = 0, char** fileNames = nullptr);

	bool doBlockComment(comment_mode currCommentMode);
	bool doStreamComment();
	bool undoStreamComment(bool tryBlockComment = true);

	bool addCurrentMacro();
	void macroPlayback(Macro);

    void loadLastSession();
	bool loadSession(Session & session, bool isSnapshotMode = false, bool shouldLoadFileBrowser = false);

	void prepareBufferChangedDialog(SciBuffer * buffer);
	void notifyBufferChanged(SciBuffer * buffer, int mask);
	bool findInFinderFiles(FindersInfo *findInFolderInfo);

	bool createFilelistForFiles(Vector<String> & fileNames);
	bool createFilelistForProjects(Vector<String> & fileNames);
	bool findInFiles();
	bool findInProjects();
	bool findInFilelist(Vector<String> & fileList);
	bool replaceInFiles();
	bool replaceInProjects();
	bool replaceInFilelist(Vector<String> & fileList);

	void setFindReplaceFolderFilter(const char *dir, const char *filters);
	Vector<String> addNppComponents(const char *destDir, const char *extFilterName, const char *extFilter);
	Vector<String> addNppPlugins(const char *extFilterName, const char *extFilter);
    int getHtmlXmlEncoding(const char *fileName) const;

	HACCEL getAccTable() const{
		return _accelerator.getAccTable();
	};

	bool emergency(const char* emergencySavedDir);

	SciBuffer* getCurrentBuffer()	{
		return _pEditView->getCurrentBuffer();
	};

	void launchDocumentBackupTask();
	int getQuoteIndexFrom(const wchar_t* quoter) const;
	void showQuoteFromIndex(int index) const;
	void showQuote(const QuoteParams* quote) const;

	String getPluginListVerStr() const {
		return _pluginsAdminDlg.getPluginListVerStr();
	};

	void minimizeDialogs();
	void restoreMinimizeDialogs();

	void refreshDarkMode(bool resetStyle = false);

private:
	Notepad_plus_Window* _pPublicInterface = nullptr;
    Window* _pMainWindow = nullptr;
	DockingManager _dockingManager;
	std::vector<int> _internalFuncIDs;

	AutoCompletion _autoCompleteMain;
	AutoCompletion _autoCompleteSub; // each Scintilla has its own autoComplete

	SmartHighlighter _smartHighlighter;
    NativeLangSpeaker _nativeLangSpeaker;
    DocTabView _mainDocTab;
    DocTabView _subDocTab;
    DocTabView* _pDocTab = nullptr;
	DocTabView* _pNonDocTab = nullptr;

    ScintillaEditView _subEditView;
    ScintillaEditView _mainEditView;
	ScintillaEditView _invisibleEditView; // for searches
	ScintillaEditView _fileEditView;      // for FileManager
    ScintillaEditView* _pEditView = nullptr;
	ScintillaEditView* _pNonEditView = nullptr;

    SplitterContainer* _pMainSplitter = nullptr;
    SplitterContainer _subSplitter;

    ContextMenu _tabPopupMenu;
	ContextMenu _tabPopupDropMenu;
	ContextMenu _fileSwitcherMultiFilePopupMenu;

	ToolBar	_toolBar;
	IconList _docTabIconList;
	IconList _docTabIconListAlt;
	IconList _docTabIconListDarkMode;

    StatusBar _statusBar;
	bool _toReduceTabBar = false;
	ReBar _rebarTop;
	ReBar _rebarBottom;

	// Dialog
	FindReplaceDlg _findReplaceDlg;
	FindInFinderDlg _findInFinderDlg;

	FindIncrementDlg _incrementFindDlg;
    AboutDlg _aboutDlg;
	DebugInfoDlg _debugInfoDlg;
	RunDlg _runDlg;
	HashFromFilesDlg _md5FromFilesDlg;
	HashFromTextDlg _md5FromTextDlg;
	HashFromFilesDlg _sha2FromFilesDlg;
	HashFromTextDlg _sha2FromTextDlg;
    GoToLineDlg _goToLineDlg;
	ColumnEditorDlg _colEditorDlg;
	WordStyleDlg _configStyleDlg;
	PreferenceDlg _preference;
	FindCharsInRangeDlg _findCharsInRangeDlg;
	PluginsAdminDlg _pluginsAdminDlg;
	DocumentPeeker _documentPeeker;

	// a handle list of all the Notepad++ dialogs
	std::vector<Window*> _hModelessDlgs;

	LastRecentFileList _lastRecentFileList;

	WindowsMenu _windowsMenu;
	Menu* _mainMenuHandle = nullptr;

	bool _sysMenuEntering = false;

	// make sure we don't recursively call doClose when closing the last file with -quitOnEmpty
	bool _isAttemptingCloseOnQuit = false;

	// For FullScreen/PostIt/DistractionFree features
	VisibleGUIConf	_beforeSpecialView;
	void fullScreenToggle();
	void postItToggle();
	void distractionFreeToggle();

	// Keystroke macro recording and playback
	Macro _macro;
	bool _recordingMacro = false;
	bool _playingBackMacro = false;
	bool _recordingSaved = false;
	RunMacroDlg _runMacroDlg;

	// For conflict detection when saving Macros or RunCommands
	ShortcutMapper* _pShortcutMapper = nullptr;

	// For hotspot
	bool _linkTriggered = true;
	bool _isFolding = false;

	//For Dynamic selection highlight
	Sci_CharacterRange _prevSelectedRange;

	//Synchronized Scolling
	struct SyncInfo final
	{
		intptr_t _line = 0;
		intptr_t _column = 0;
		bool _isSynScollV = false;
		bool _isSynScollH = false;

		bool doSync() const {return (_isSynScollV || _isSynScollH); }
	}
	_syncInfo;

	bool _isUDDocked = false;

	trayIconControler* _pTrayIco = nullptr;
	intptr_t _zoomOriginalValue = 0;

	Accelerator _accelerator;
	ScintillaAccelerator _scintaccelerator;

	PluginsManager _pluginsManager;
    ButtonDlg _restoreButton;

	bool _isFileOpening = false;
	bool _isAdministrator = false;

	bool _isEndingSessionButNotReady = false; // If Windows 10 update needs to restart
                                              // and Notepad++ has one (some) dirty document(s)
                                              // and "Enable session snapshot and periodic backup" is not enabled
                                              // then WM_ENDSESSION is send with wParam == FALSE
                                              // in this case this boolean is set true, so Notepad++ will quit and its current session will be saved
	ScintillaCtrls _scintillaCtrls4Plugins;

	std::vector<std::pair<int, int> > _hideLinesMarks;
	StyleArray _hotspotStyles;

	AnsiCharPanel* _pAnsiCharPanel = nullptr;
	ClipboardHistoryPanel* _pClipboardHistoryPanel = nullptr;
	VerticalFileSwitcher* _pDocumentListPanel = nullptr;
	ProjectPanel* _pProjectPanel_1 = nullptr;
	ProjectPanel* _pProjectPanel_2 = nullptr;
	ProjectPanel* _pProjectPanel_3 = nullptr;

	FileBrowser* _pFileBrowser = nullptr;

	DocumentMap* _pDocMap = nullptr;
	FunctionListPanel* _pFuncList = nullptr;

	std::vector<Window*> _sysTrayHiddenHwnd;

	BOOL notify(SCNotification *notification);
	void command(int id);

//Document management
	UCHAR _mainWindowStatus = 0; //For 2 views and user dialog if docked
	int _activeView = MAIN_VIEW;

	//User dialog docking
	void dockUserDlg();
    void undockUserDlg();

	//View visibility
	void showView(int whichOne);
	bool viewVisible(int whichOne);
	void hideView(int whichOne);
	void hideCurrentView();
	bool bothActive() { return (_mainWindowStatus & WindowBothActive) == WindowBothActive; };
	bool reloadLang();
	bool loadStyles();

	int currentView() {
		return _activeView;
	}

	int otherView() {
		return (_activeView == MAIN_VIEW?SUB_VIEW:MAIN_VIEW);
	}

	int otherFromView(int whichOne) {
		return (whichOne == MAIN_VIEW?SUB_VIEW:MAIN_VIEW);
	}

	bool canHideView(int whichOne);	//true if view can safely be hidden (no open docs etc)

	bool isEmpty(); // true if we have 1 view with 1 clean, untitled doc

	int switchEditViewTo(int gid);	//activate other view (set focus etc)

	void docGotoAnotherEditView(FileTransferMode mode);	//TransferMode
	void docOpenInNewInstance(FileTransferMode mode, int x = 0, int y = 0);

	void loadBufferIntoView(BufferID id, int whichOne, bool dontClose = false);		//Doesnt _activate_ the buffer
	bool removeBufferFromView(BufferID id, int whichOne);	//Activates alternative of possible, or creates clean document if not clean already

	bool activateBuffer(BufferID id, int whichOne, bool forceApplyHilite = false);			//activate buffer in that view if found
	void notifyBufferActivated(BufferID bufid, int view);
	void performPostReload(int whichOne);
//END: Document management

	int doSaveOrNot(const char *fn, bool isMulti = false);
	int doReloadOrNot(const char *fn, bool dirty);
	int doCloseOrNot(const char *fn);
	int doDeleteOrNot(const char *fn);
	int doSaveAll();

	void enableMenu(int cmdID, bool doEnable) const;
	void enableCommand(int cmdID, bool doEnable, int which) const;
	void checkClipboard();
	void checkDocState();
	void checkUndoState();
	void checkMacroState();
	void checkSyncState();
	void setupColorSampleBitmapsOnMainMenuItems();
	void dropFiles(HDROP hdrop);
	void checkModifiedDocument(bool bCheckOnlyCurrentBuffer);

    void getMainClientRect(Rect & rc) const;
	void staticCheckMenuAndTB() const;
	void dynamicCheckMenuAndTB() const;
	void enableConvertMenuItems(EolType f) const;
	void checkUnicodeMenuItems() const;

	String getLangDesc(LangType langType, bool getName = false);

	void setLangStatus(LangType langType);

	void setDisplayFormat(EolType f);
	void setUniModeText();
	void checkLangsMenu(int id) const ;
    void setLanguage(LangType langType);
	LangType menuID2LangType(int cmdID);

	BOOL processIncrFindAccel(MSG *msg) const;
	BOOL processFindAccel(MSG *msg) const;

	void checkMenuItem(int itemID, bool willBeChecked) const {
		::CheckMenuItem(_mainMenuHandle, itemID, MF_BYCOMMAND | (willBeChecked?MF_CHECKED:MF_UNCHECKED));
	}

	bool isConditionExprLine(intptr_t lineNumber);
	intptr_t findMachedBracePos(size_t startPos, size_t endPos, char targetSymbol, char matchedSymbol);
	void maintainIndentation(char ch);

	void addHotSpot(ScintillaEditView* view = nullptr);

    void bookmarkAdd(intptr_t lineno) const {
		if (lineno == -1)
			lineno = _pEditView->getCurrentLineNumber();
		if (!bookmarkPresent(lineno))
			_pEditView->execute(SCI_MARKERADD, lineno, MARK_BOOKMARK);
	}

    void bookmarkDelete(size_t lineno) const {
		if (lineno == -1)
			lineno = _pEditView->getCurrentLineNumber();
		while (bookmarkPresent(lineno))
			_pEditView->execute(SCI_MARKERDELETE, lineno, MARK_BOOKMARK);
	}

    bool bookmarkPresent(intptr_t lineno) const {
		if (lineno == -1)
			lineno = _pEditView->getCurrentLineNumber();
		LRESULT state = _pEditView->execute(SCI_MARKERGET, lineno);
		return ((state & (1 << MARK_BOOKMARK)) != 0);
	}

    void bookmarkToggle(intptr_t lineno) const {
		if (lineno == -1)
			lineno = _pEditView->getCurrentLineNumber();

		if (bookmarkPresent(lineno))
			bookmarkDelete(lineno);
		else
			bookmarkAdd(lineno);
	}

    void bookmarkNext(bool forwardScan);
	void bookmarkClearAll() const {
		_pEditView->execute(SCI_MARKERDELETEALL, MARK_BOOKMARK);
	}

	void copyMarkedLines();
	void cutMarkedLines();
	void deleteMarkedLines(bool isMarked);
	void pasteToMarkedLines();
	void deleteMarkedline(size_t ln);
	void inverseMarks();
	void replaceMarkedline(size_t ln, const char *str);
	String getMarkedLine(size_t ln);
    void findMatchingBracePos(intptr_t& braceAtCaret, intptr_t& braceOpposite);
    bool braceMatch();

    void activateNextDoc(bool direction);
	void activateDoc(size_t pos);

	void updateStatusBar();
	size_t getSelectedCharNumber(UniMode);
	size_t getCurrentDocCharCount(UniMode u);
	size_t getSelectedAreas();
	size_t getSelectedBytes();
	bool isFormatUnicode(UniMode);
	int getBOMSize(UniMode);

	void showAutoComp();
	void autoCompFromCurrentFile(bool autoInsert = true);
	void showFunctionComp();
	void showPathCompletion();

	//void changeStyleCtrlsLang(Window* hDlg, int *idArray, const char **translatedText);
	void setCodePageForInvisibleView(SciBuffer const* pBuffer);
	bool replaceInOpenedFiles();
	bool findInOpenedFiles();
	bool findInCurrentFile(bool isEntireDoc);

	void getMatchedFileNames(const char *dir, size_t level, const Vector<String> & patterns, Vector<String> & fileNames, bool isRecursive, bool isInHiddenDir);
	void doSynScorll(Window* hW);
	void setWorkingDir(const char *dir);
	bool str2Cliboard(const String& str2cpy);

	bool getIntegralDockingData(tTbData & dockData, int & iCont, bool & isVisible);
	int getLangFromMenuName(const char * langName);
	String getLangFromMenu(const SciBuffer * buf);

    String exts2Filters(const char* exts, int maxExtsLen = -1) const; // maxExtsLen default value -1 makes no limit of whole exts length
	int setFileOpenSaveDlgFilters(CustomFileDialog & fDlg, bool showAllExt, int langType = -1); // showAllExt should be true if it's used for open file dialog - all set exts should be used for filtering files
	Style * getStyleFromName(const char *styleName);
	bool dumpFiles(const char * outdir, const char * fileprefix = TEXT(""));	//helper func
	void drawTabbarColoursFromStylerArray();
	void drawAutocompleteColoursFromTheme(Color& fgColor, Color& bgColor);
	void drawDocumentMapColoursFromStylerArray();

	Vector<String> loadCommandlineParams(const char * commandLine, const CmdLineParams * pCmdParams) {
		const CmdLineParamsDTO dto = CmdLineParamsDTO::FromCmdLineParams(*pCmdParams);
		return loadCommandlineParams(commandLine, &dto);
	}
	Vector<String> loadCommandlineParams(const char * commandLine, const CmdLineParamsDTO * pCmdParams);
	bool noOpenedDoc() const;
	bool goToPreviousIndicator(int indicID2Search, bool isWrap = true) const;
	bool goToNextIndicator(int indicID2Search, bool isWrap = true) const;
	int wordCount();

	void wsTabConvert(spaceTab whichWay);
	void doTrim(trimOp whichPart);
	void removeEmptyLine(bool isBlankContained);
	void removeDuplicateLines();
	void launchAnsiCharPanel();
	void launchClipboardHistoryPanel();
	void launchDocumentListPanel();
	void checkProjectMenuItem();
	void launchProjectPanel(int cmdID, ProjectPanel ** pProjPanel, int panelID);
	void launchDocMap();
	void launchFunctionList();
	void launchFileBrowser(const Vector<String> & folders, const char* selectedItemPath, bool fromScratch = false);
	void showAllQuotes() const;
	static dword WINAPI threadTextPlayer(void *text2display);
	static dword WINAPI threadTextTroller(void *params);
	static int getRandomAction(int ranNum);
	static bool deleteBack(ScintillaEditView *pCurrentView, BufferID targetBufID);
	static bool deleteForward(ScintillaEditView *pCurrentView, BufferID targetBufID);
	static bool selectBack(ScintillaEditView *pCurrentView, BufferID targetBufID);

	static int getRandomNumber(int rangeMax = -1) {
		int randomNumber = rand();
		if (rangeMax == -1)
			return randomNumber;
		return (rand() % rangeMax);
	}

	static dword WINAPI backupDocument(void *params);

	static dword WINAPI monitorFileOnChange(void * params);
	struct MonitorInfo final {
		MonitorInfo(SciBuffer *buf, Window* nppHandle) :
			_buffer(buf), _nppHandle(nppHandle) {};
		SciBuffer *_buffer = nullptr;
		Window* _nppHandle = nullptr;
	};

	void monitoringStartOrStopAndUpdateUI(SciBuffer* pBuf, bool isStarting);
	void createMonitoringThread(SciBuffer* pBuf);
	void updateCommandShortcuts();
};
