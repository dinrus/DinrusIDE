#pragma once

#include <Core/Core.h>
#include <CtrlLib/CtrlLib.h>
#include <plugin/TinyXml/tinyxml.h>
#include <Scintilla/Scintilla.h>
#include <map>
#include <Scintilla/ILexer.h>
#include <Lexilla/Lexilla.h>
//#include <PowerEditor/ScintillaComponent/ScintillaComponent.h>
//#include <PowerEditor/WinControls/ToolBar/ToolBar.h>
//#include <PowerEditor/WinControls/shortcut/shortcut.h>
//#include <PowerEditor/WinControls/ContextMenu/ContextMenu.h>
//#include <PowerEditor/dpiManager.h>
//#include <PowerEditor/NppDarkMode.h>
//#include <assert.h>
//#include <char.h>
//#include <cwchar>


using namespace Upp;

#ifdef _WIN64

#ifdef _M_ARM64
#define ARCH_TYPE IMAGE_FILE_MACHINE_ARM64
#else
#define ARCH_TYPE IMAGE_FILE_MACHINE_AMD64
#endif

#else
#define ARCH_TYPE IMAGE_FILE_MACHINE_I386

#endif

#define CMD_INTERPRETER TEXT("%COMSPEC%")

class NativeLangSpeaker;

const bool POS_VERTICAL = true;
const bool POS_HORIZOTAL = false;

const int UDD_SHOW   = 1; // 0000 0001
const int UDD_DOCKED = 2; // 0000 0010

// 0 : 0000 0000 hide & undocked
// 1 : 0000 0001 show & undocked
// 2 : 0000 0010 hide & docked
// 3 : 0000 0011 show & docked

const int TAB_DRAWTOPBAR = 1;      //0000 0000 0001
const int TAB_DRAWINACTIVETAB = 2; //0000 0000 0010
const int TAB_DRAGNDROP = 4;       //0000 0000 0100
const int TAB_REDUCE = 8;          //0000 0000 1000
const int TAB_CLOSEBUTTON = 16;    //0000 0001 0000
const int TAB_DBCLK2CLOSE = 32;    //0000 0010 0000
const int TAB_VERTICAL = 64;       //0000 0100 0000
const int TAB_MULTILINE = 128;     //0000 1000 0000
const int TAB_HIDE = 256;          //0001 0000 0000
const int TAB_QUITONEMPTY = 512;   //0010 0000 0000
const int TAB_ALTICONS = 1024;     //0100 0000 0000

enum LangType {L_TEXT, L_PHP , L_C, L_CPP, L_CS, L_OBJC, L_JAVA, L_RC,\
               L_HTML, L_XML, L_MAKEFILE, L_PASCAL, L_BATCH, L_INI, L_ASCII, L_USER,\
               L_ASP, L_SQL, L_VB, L_JS, L_CSS, L_PERL, L_PYTHON, L_LUA, \
               L_TEX, L_FORTRAN, L_BASH, L_FLASH, L_NSIS, L_TCL, L_LISP, L_SCHEME,\
               L_ASM, L_DIFF, L_PROPS, L_PS, L_RUBY, L_SMALLTALK, L_VHDL, L_KIX, L_AU3,\
               L_CAML, L_ADA, L_VERILOG, L_MATLAB, L_HASKELL, L_INNO, L_SEARCHRESULT,\
               L_CMAKE, L_YAML, L_COBOL, L_GUI4CLI, L_D, L_POWERSHELL, L_R, L_JSP,\
               L_COFFEESCRIPT, L_JSON, L_JAVASCRIPT, L_FORTRAN_77, L_BAANC, L_SREC,\
               L_IHEX, L_TEHEX, L_SWIFT,\
               L_ASN1, L_AVS, L_BLITZBASIC, L_PUREBASIC, L_FREEBASIC, \
               L_CSOUND, L_ERLANG, L_ESCRIPT, L_FORTH, L_LATEX, \
               L_MMIXAL, L_NIM, L_NNCRONTAB, L_OSCRIPT, L_REBOL, \
               L_REGISTRY, L_RUST, L_SPICE, L_TXT2TAGS, L_VISUALPROLOG, L_TYPESCRIPT,\
               // Don't use L_JS, use L_JAVASCRIPT instead
               // The end of enumated language type, so it should be always at the end
               L_EXTERNAL};


enum class EolType: std::uint8_t
{
	windows,
	macos,
	unix,

	// special values
	unknown, // can not be the first value for legacy code
	osdefault = windows,
};

/*!
** \brief Convert an int into a FormatType
** \param value An arbitrary int
** \param defvalue The default value to use if an invalid value is provided
*/
EolType convertIntToFormatType(int value, EolType defvalue = EolType::osdefault);


enum UniMode {uni8Bit=0, uniUTF8=1, uni16BE=2, uni16LE=3, uniCookie=4, uni7Bit=5, uni16BE_NoBOM=6, uni16LE_NoBOM=7, uniEnd};
enum ChangeDetect { cdDisabled = 0x0, cdEnabledOld = 0x01, cdEnabledNew = 0x02, cdAutoUpdate = 0x04, cdGo2end = 0x08 };
enum BackupFeature {bak_none = 0, bak_simple = 1, bak_verbose = 2};
enum OpenSaveDirSetting {dir_followCurrent = 0, dir_last = 1, dir_userDef = 2};
enum MultiInstSetting {monoInst = 0, multiInstOnSession = 1, multiInst = 2};
enum writeTechnologyEngine {defaultTechnology = 0, directWriteTechnology = 1};
enum urlMode {urlDisable = 0, urlNoUnderLineFg, urlUnderLineFg, urlNoUnderLineBg, urlUnderLineBg,
              urlMin = urlDisable,
              urlMax = urlUnderLineBg};

const int LANG_INDEX_INSTR = 0;
const int LANG_INDEX_INSTR2 = 1;
const int LANG_INDEX_TYPE = 2;
const int LANG_INDEX_TYPE2 = 3;
const int LANG_INDEX_TYPE3 = 4;
const int LANG_INDEX_TYPE4 = 5;
const int LANG_INDEX_TYPE5 = 6;
const int LANG_INDEX_TYPE6 = 7;
const int LANG_INDEX_TYPE7 = 8;

const int COPYDATA_PARAMS = 0;
const int COPYDATA_FILENAMESA = 1;
const int COPYDATA_FILENAMESW = 2;
const int COPYDATA_FULL_CMDLINE = 3;

#define PURE_LC_NONE	0
#define PURE_LC_BOL	 1
#define PURE_LC_WSP	 2

#define DECSEP_DOT	  0
#define DECSEP_COMMA	1
#define DECSEP_BOTH	 2


#define DROPBOX_AVAILABLE 1
#define ONEDRIVE_AVAILABLE 2
#define GOOGLEDRIVE_AVAILABLE 4

const char fontSizeStrs[][3] = {TEXT(""), TEXT("5"), TEXT("6"), TEXT("7"), TEXT("8"), TEXT("9"), TEXT("10"), TEXT("11"), TEXT("12"), TEXT("14"), TEXT("16"), TEXT("18"), TEXT("20"), TEXT("22"), TEXT("24"), TEXT("26"), TEXT("28")};

const char localConfFile[] = TEXT("doLocalConf.xml");
const char notepadStyleFile[] = TEXT("asNotepad.xml");

// issue xml/log file name
const char nppLogNetworkDriveIssue[] = TEXT("nppLogNetworkDriveIssue");
const char nppLogNulContentCorruptionIssue[] = TEXT("nppLogNulContentCorruptionIssue");

void cutString(const char *str2cut, Vector<String> & patternVect);
void cutStringBy(const char *str2cut, Vector<String> & patternVect, char byChar, bool allowEmptyStr);


struct Position
{
	intptr_t _firstVisibleLine = 0;
	intptr_t _startPos = 0;
	intptr_t _endPos = 0;
	intptr_t _xOffset = 0;
	intptr_t _selMode = 0;
	intptr_t _scrollWidth = 1;
	intptr_t _offset = 0;
	intptr_t _wrapCount = 0;
};


struct MapPosition
{
private:
	intptr_t _maxPeekLenInKB = 512; // 512 KB
public:
	intptr_t _firstVisibleDisplayLine = -1;

	intptr_t _firstVisibleDocLine = -1; // map
	intptr_t _lastVisibleDocLine = -1;  // map
	intptr_t _nbLine = -1;              // map
	intptr_t _higherPos = -1;           // map
	intptr_t _width = -1;
	intptr_t _height = -1;
	intptr_t _wrapIndentMode = -1;

	intptr_t _KByteInDoc = _maxPeekLenInKB;

	bool _isWrap = false;
	bool isValid() const { return (_firstVisibleDisplayLine != -1); };
	bool canScroll() const { return (_KByteInDoc < _maxPeekLenInKB); }; // _nbCharInDoc < _maxPeekLen : Don't scroll the document for the performance issue
};


struct sessionFileInfo : public Position
{
	sessionFileInfo(const char *fn, const char *ln, int encoding, bool userReadOnly, const Position& pos, const char *backupFilePath, FILETIME originalFileLastModifTimestamp, const MapPosition & mapPos) :
		_isUserReadOnly(userReadOnly), _encoding(encoding), Position(pos), _originalFileLastModifTimestamp(originalFileLastModifTimestamp), _mapPos(mapPos)
	{
		if (fn) _fileName = fn;
		if (ln)	_langName = ln;
		if (backupFilePath) _backupFilePath = backupFilePath;
	}

	sessionFileInfo(String fn) : _fileName(fn) {}

	String _fileName;
	String	_langName;
	std::vector<size_t> _marks;
	std::vector<size_t> _foldStates;
	int	_encoding = -1;
	bool _isUserReadOnly = false;
	bool _isMonitoring = false;

	String _backupFilePath;
	FILETIME _originalFileLastModifTimestamp = {};

	MapPosition _mapPos;
};


struct Session
{
	size_t nbMainFiles() const {return _mainViewFiles.size();};
	size_t nbSubFiles() const {return _subViewFiles.size();};
	size_t _activeView = 0;
	size_t _activeMainIndex = 0;
	size_t _activeSubIndex = 0;
	bool _includeFileBrowser = false;
	String _fileBrowserSelectedItem;
	std::vector<sessionFileInfo> _mainViewFiles;
	std::vector<sessionFileInfo> _subViewFiles;
	Vector<String> _fileBrowserRoots;
};

/*
struct CmdLineParams
{
	bool _isNoPlugin = false;
	bool _isReadOnly = false;
	bool _isNoSession = false;
	bool _isNoTab = false;
	bool _isPreLaunch = false;
	bool _showLoadingTime = false;
	bool _alwaysOnTop = false;
	intptr_t _line2go   = -1;
	intptr_t _column2go = -1;
	intptr_t _pos2go = -1;

	POINT _point = {};
	bool _isPointXValid = false;
	bool _isPointYValid = false;

	bool _isSessionFile = false;
	bool _isRecursive = false;
	bool _openFoldersAsWorkspace = false;
	bool _monitorFiles = false;

	LangType _langType = L_EXTERNAL;
	String _localizationPath;
	String _udlName;
	String _pluginMessage;

	String _easterEggName;
	unsigned char _quoteType = 0;
	int _ghostTypingSpeed = -1; // -1: initial value  1: slow  2: fast  3: speed of light

	CmdLineParams()
	{
		_point.x = 0;
		_point.y = 0;
	}

	bool isPointValid() const
	{
		return _isPointXValid && _isPointYValid;
	}
};

// A POD class to send CmdLineParams through WM_COPYDATA and to Notepad_plus::loadCommandlineParams
struct CmdLineParamsDTO
{
	bool _isReadOnly = false;
	bool _isNoSession = false;
	bool _isSessionFile = false;
	bool _isRecursive = false;
	bool _openFoldersAsWorkspace = false;
	bool _monitorFiles = false;

	intptr_t _line2go = 0;
	intptr_t _column2go = 0;
	intptr_t _pos2go = 0;

	LangType _langType = L_EXTERNAL;
	wchar _udlName[MAX_PATH];
	wchar _pluginMessage[MAX_PATH];

	static CmdLineParamsDTO FromCmdLineParams(const CmdLineParams& params)
	{
		CmdLineParamsDTO dto;
		dto._isReadOnly = params._isReadOnly;
		dto._isNoSession = params._isNoSession;
		dto._isSessionFile = params._isSessionFile;
		dto._isRecursive = params._isRecursive;
		dto._openFoldersAsWorkspace = params._openFoldersAsWorkspace;
		dto._monitorFiles = params._monitorFiles;

		dto._line2go = params._line2go;
		dto._column2go = params._column2go;
		dto._pos2go = params._pos2go;

		dto._langType = params._langType;
		std::wcsncpy(dto._udlName, (const wchar*) params._udlName.Begin(), MAX_PATH);
		std::wcsncpy(dto._pluginMessage,(const wchar*) params._pluginMessage.Begin(), MAX_PATH);
		return dto;
	}
};
*/
struct FloatingWindowInfo
{
	int _cont = 0;
	Rect _pos = {};

	FloatingWindowInfo(int cont, int x, int y, int w, int h)
		: _cont(cont)
	{
		_pos.left	= x;
		_pos.top	= y;
		_pos.right	= w;
		_pos.bottom = h;
	}
};


struct PluginDlgDockingInfo final
{
	String _name;
	int _internalID = -1;

	int _currContainer = -1;
	int _prevContainer = -1;
	bool _isVisible = false;

	PluginDlgDockingInfo(const char* pluginName, int id, int curr, int prev, bool isVis)
		: _internalID(id), _currContainer(curr), _prevContainer(prev), _isVisible(isVis), _name(pluginName)
	{}

	bool operator == (const PluginDlgDockingInfo& rhs) const
	{
		return _internalID == rhs._internalID and _name == rhs._name;
	}
};


struct ContainerTabInfo final
{
	int _cont = 0;
	int _activeTab = 0;

	ContainerTabInfo(int cont, int activeTab) : _cont(cont), _activeTab(activeTab) {};
};


struct DockingManagerData final
{
	int _leftWidth = 200;
	int _rightWidth = 200;
	int _topHeight = 200;
	int _bottomHight = 200;

	std::vector<FloatingWindowInfo> _flaotingWindowInfo;
	std::vector<PluginDlgDockingInfo> _pluginDockInfo;
	std::vector<ContainerTabInfo> _containerTabInfo;

	bool getFloatingRCFrom(int floatCont, Rect& rc) const
	{
		for (size_t i = 0, fwiLen = _flaotingWindowInfo.size(); i < fwiLen; ++i)
		{
			if (_flaotingWindowInfo[i]._cont == floatCont)
			{
				rc.left   = _flaotingWindowInfo[i]._pos.left;
				rc.top	= _flaotingWindowInfo[i]._pos.top;
				rc.right  = _flaotingWindowInfo[i]._pos.right;
				rc.bottom = _flaotingWindowInfo[i]._pos.bottom;
				return true;
			}
		}
		return false;
	}
};



const int FONTSTYLE_NONE = 0;
const int FONTSTYLE_BOLD = 1;
const int FONTSTYLE_ITALIC = 2;
const int FONTSTYLE_UNDERLINE = 4;

const int STYLE_NOT_USED = -1;

const int COLORSTYLE_FOREGROUND = 0x01;
const int COLORSTYLE_BACKGROUND = 0x02;
const int COLORSTYLE_ALL = COLORSTYLE_FOREGROUND|COLORSTYLE_BACKGROUND;



struct Style final
{
	int _styleID = STYLE_NOT_USED;
	String _styleDesc;

	Color& _fgColor;// = Color&(STYLE_NOT_USED);
	Color& _bgColor;// = Color&(STYLE_NOT_USED);
	int _colorStyle = COLORSTYLE_ALL;

	bool _isFontEnabled = false;
	String _fontName;
	int _fontStyle = FONTSTYLE_NONE;
	int _fontSize = STYLE_NOT_USED;

	int _nesting = FONTSTYLE_NONE;

	int _keywordClass = STYLE_NOT_USED;
	String _keywords;
};


struct GlobalOverride final
{
	bool isEnable() const {return (enableFg || enableBg || enableFont || enableFontSize || enableBold || enableItalic || enableUnderLine);}
	bool enableFg = false;
	bool enableBg = false;
	bool enableFont = false;
	bool enableFontSize = false;
	bool enableBold = false;
	bool enableItalic = false;
	bool enableUnderLine = false;
};

struct StyleArray
{
	auto begin() { return _styleVect.begin(); };
	auto end() { return _styleVect.end(); };
	void clear() { _styleVect.clear(); };

	Style& getStyler(size_t index) {
		assert(index < _styleVect.size());
		return _styleVect[index];
	};

	void addStyler(int styleID, TiXmlNode *styleNode);

	void addStyler(int styleID, const char* styleName) {
		_styleVect.emplace_back();
		Style& s = _styleVect.back();
		s._styleID = styleID;
		s._styleDesc = styleName;
		s._fgColor = Black;
		s._bgColor = White;
	};

	Style* findByID(int id) {
		for (size_t i = 0; i < _styleVect.size(); ++i)
		{
			if (_styleVect[i]._styleID == id)
				return &(_styleVect[i]);
		}
		return nullptr;
	};

	Style* findByName(const char* name) {
		for (size_t i = 0; i < _styleVect.size(); ++i)
		{
			if (_styleVect[i]._styleDesc == name)
				return &(_styleVect[i]);
		}
		return nullptr;
	};

protected:
	std::vector<Style> _styleVect;
};



struct LexerStyler : public StyleArray
{
public:
	LexerStyler & operator=(const LexerStyler & ls)
	{
		if (this != &ls)
		{
			*(static_cast<StyleArray *>(this)) = ls;
			this->_lexerName = ls._lexerName;
			this->_lexerDesc = ls._lexerDesc;
			this->_lexerUserExt = ls._lexerUserExt;
		}
		return *this;
	}

	void setLexerName(const char *lexerName)
	{
		_lexerName = lexerName;
	}

	void setLexerDesc(const char *lexerDesc)
	{
		_lexerDesc = lexerDesc;
	}

	void setLexerUserExt(const char *lexerUserExt) {
		_lexerUserExt = lexerUserExt;
	};

	const char * getLexerName() const {return _lexerName.Begin();};
	const char * getLexerDesc() const {return _lexerDesc.Begin();};
	const char * getLexerUserExt() const {return _lexerUserExt.Begin();};

private :
	String _lexerName;
	String _lexerDesc;
	String _lexerUserExt;
};

struct SortLexersInAlphabeticalOrder {
	bool operator() (LexerStyler& l, LexerStyler& r) {
		if (!lstrcmp(l.getLexerDesc(), TEXT("Search result")))
			return false;
		if (!lstrcmp(r.getLexerDesc(), TEXT("Search result")))
			return true;
		return lstrcmp(l.getLexerDesc(), r.getLexerDesc()) < 0;
	}
};

struct LexerStylerArray
{
	size_t getNbLexer() const { return _lexerStylerVect.size(); }
	void clear() { _lexerStylerVect.clear(); }

	LexerStyler & getLexerFromIndex(size_t index)
	{
		assert(index < _lexerStylerVect.size());
		return _lexerStylerVect[index];
	};

	const char * getLexerNameFromIndex(size_t index) const { return _lexerStylerVect[index].getLexerName(); }
	const char * getLexerDescFromIndex(size_t index) const { return _lexerStylerVect[index].getLexerDesc(); }

	LexerStyler * getLexerStylerByName(const char *lexerName) {
		if (!lexerName) return nullptr;
		for (size_t i = 0 ; i < _lexerStylerVect.size() ; ++i)
		{
			if (!lstrcmp(_lexerStylerVect[i].getLexerName(), lexerName))
				return &(_lexerStylerVect[i]);
		}
		return nullptr;
	};

	void addLexerStyler(const char *lexerName, const char *lexerDesc, const char *lexerUserExt, TiXmlNode *lexerNode);

	void sort() {
		std::sort(_lexerStylerVect.begin(), _lexerStylerVect.end(), SortLexersInAlphabeticalOrder());
	};

private :
	std::vector<LexerStyler> _lexerStylerVect;
};


struct NewDocDefaultSettings final
{
	EolType _format = EolType::osdefault;
	UniMode _unicodeMode = uniCookie;
	bool _openAnsiAsUtf8 = true;
	LangType _lang = L_TEXT;
	int _codepage = -1; // -1 when not using
};


struct LangMenuItem final
{
	LangType _langType = L_TEXT;
	int	_cmdID = -1;
	String _langName;

	LangMenuItem(LangType lt, int cmdID = 0, const char* langName = TEXT("")):
	_langType(lt), _cmdID(cmdID), _langName(langName){};
};

struct PrintSettings final {
	bool _printLineNumber = true;
	int _printOption = SC_PRINT_COLOURONWHITE;

	String _headerLeft;
	String _headerMiddle;
	String _headerRight;
	String _headerFontName;
	int _headerFontStyle = 0;
	int _headerFontSize = 0;

	String _footerLeft;
	String _footerMiddle;
	String _footerRight;
	String _footerFontName;
	int _footerFontStyle = 0;
	int _footerFontSize = 0;

	Rect _marge = {};

	PrintSettings() {
		_marge.left = 0; _marge.top = 0; _marge.right = 0; _marge.bottom = 0;
	};

	bool isHeaderPresent() const {
		return ((_headerLeft != TEXT("")) || (_headerMiddle != TEXT("")) || (_headerRight != TEXT("")));
	};

	bool isFooterPresent() const {
		return ((_footerLeft != TEXT("")) || (_footerMiddle != TEXT("")) || (_footerRight != TEXT("")));
	};

	bool isUserMargePresent() const {
		return ((_marge.left != 0) || (_marge.top != 0) || (_marge.right != 0) || (_marge.bottom != 0));
	};
};


class MatchedPairConf final
{
public:
	bool hasUserDefinedPairs() const { return _matchedPairs.size() != 0; }
	bool hasDefaultPairs() const { return _doParentheses||_doBrackets||_doCurlyBrackets||_doQuotes||_doDoubleQuotes||_doHtmlXmlTag; }
	bool hasAnyPairsPair() const { return hasUserDefinedPairs() || hasDefaultPairs(); }

public:
	std::vector<std::pair<char, char>> _matchedPairs;
	std::vector<std::pair<char, char>> _matchedPairsInit; // used only on init
	bool _doHtmlXmlTag = false;
	bool _doParentheses = false;
	bool _doBrackets = false;
	bool _doCurlyBrackets = false;
	bool _doQuotes = false;
	bool _doDoubleQuotes = false;
};
/*
struct DarkModeConf final
{
	bool _isEnabled = false;
	bool _isEnabledPlugin = true;
	NppDarkMode::ColorTone _colorTone = NppDarkMode::blackTone;
	NppDarkMode::Colors _customColors = NppDarkMode::getDarkModeDefaultColors();
};

struct NppGUI final
{
	NppGUI()
	{
		_appPos.left = 0;
		_appPos.top = 0;
		_appPos.right = 1100;
		_appPos.bottom = 700;

		_findWindowPos.left = 0;
		_findWindowPos.top = 0;
		_findWindowPos.right = 0;
		_findWindowPos.bottom = 0;

		_defaultDir[0] = 0;
		_defaultDirExp[0] = 0;
	}

	toolBarStatusType _toolBarStatus = TB_STANDARD;
	bool _toolbarShow = true;
	bool _statusBarShow = true;
	bool _menuBarShow = true;

	// 1st bit : draw top bar;
	// 2nd bit : draw inactive tabs
	// 3rd bit : enable drag & drop
	// 4th bit : reduce the height
	// 5th bit : enable vertical
	// 6th bit : enable multiline

	// 0:don't draw; 1:draw top bar 2:draw inactive tabs 3:draw both 7:draw both+drag&drop
	int _tabStatus = (TAB_DRAWTOPBAR | TAB_DRAWINACTIVETAB | TAB_DRAGNDROP | TAB_REDUCE | TAB_CLOSEBUTTON);

	bool _splitterPos = POS_VERTICAL;
	int _userDefineDlgStatus = UDD_DOCKED;

	int _tabSize = 4;
	bool _tabReplacedBySpace = false;

	bool _finderLinesAreCurrentlyWrapped = false;
	bool _finderPurgeBeforeEverySearch = false;
	bool _finderShowOnlyOneEntryPerFoundLine = true;

	int _fileAutoDetection = cdEnabledNew;

	bool _checkHistoryFiles = false;

	Rect _appPos = {};

	Rect _findWindowPos = {};

	bool _isMaximized = false;
	bool _isMinimizedToTray = false;
	bool _rememberLastSession = true; // remember next session boolean will be written in the settings
	bool _isCmdlineNosessionActivated = false; // used for if -nosession is indicated on the launch time
	bool _detectEncoding = true;
	bool _saveAllConfirm = true;
	bool _setSaveDlgExtFiltToAllTypes = false;
	bool _doTaskList = true;
	bool _maitainIndent = true;
	bool _enableSmartHilite = true;

	bool _smartHiliteCaseSensitive = false;
	bool _smartHiliteWordOnly = true;
	bool _smartHiliteUseFindSettings = false;
	bool _smartHiliteOnAnotherView = false;

	bool _markAllCaseSensitive = false;
	bool _markAllWordOnly = true;

	bool _disableSmartHiliteTmp = false;
	bool _enableTagsMatchHilite = true;
	bool _enableTagAttrsHilite = true;
	bool _enableHiliteNonHTMLZone = false;
	bool _styleMRU = true;
	char _leftmostDelimiter = '(';
	char _rightmostDelimiter = ')';
	bool _delimiterSelectionOnEntireDocument = false;
	bool _backSlashIsEscapeCharacterForSql = true;
	bool _stopFillingFindField = false;
	bool _monospacedFontFindDlg = false;
	bool _findDlgAlwaysVisible = false;
	bool _confirmReplaceInAllOpenDocs = true;
	bool _replaceStopsWithoutFindingNext = false;
	bool _muteSounds = false;
	bool _enableFoldCmdToggable = false;
	writeTechnologyEngine _writeTechnologyEngine = defaultTechnology;
	bool _isWordCharDefault = true;
	String _customWordChars;
	urlMode _styleURL = urlUnderLineFg;
	String _uriSchemes = TEXT("svn:// cvs:// git:// imap:// irc:// irc6:// ircs:// ldap:// ldaps:// news: telnet:// gopher:// ssh:// sftp:// smb:// skype: snmp:// spotify: steam:// sms: slack:// chrome:// bitcoin:");
	NewDocDefaultSettings _newDocDefaultSettings;

	String _dateTimeFormat = TEXT("yyyy-MM-dd HH:mm:ss");
	bool _dateTimeReverseDefaultOrder = false;

	void setTabReplacedBySpace(bool b) {_tabReplacedBySpace = b;};
	const NewDocDefaultSettings & getNewDocDefaultSettings() const {return _newDocDefaultSettings;};
	std::vector<LangMenuItem> _excludedLangList;
	bool _isLangMenuCompact = true;

	PrintSettings _printSettings;
	BackupFeature _backup = bak_none;
	bool _useDir = false;
	String _backupDir;
	DockingManagerData _dockingData;
	GlobalOverride _globalOverride;
	enum AutocStatus{autoc_none, autoc_func, autoc_word, autoc_both};
	AutocStatus _autocStatus = autoc_both;
	size_t  _autocFromLen = 1;
	bool _autocIgnoreNumbers = true;
	bool _autocInsertSelectedUseENTER = true;
	bool _autocInsertSelectedUseTAB = true;
	bool _funcParams = true;
	MatchedPairConf _matchedPairConf;

	String _definedSessionExt;
	String _definedWorkspaceExt;

	String _commandLineInterpreter = CMD_INTERPRETER;

	struct AutoUpdateOptions
	{
		bool _doAutoUpdate = true;
		int _intervalDays = 15;
		Date _nextUpdateDate;
		AutoUpdateOptions(): _nextUpdateDate(Date()) {};
	}
	_autoUpdateOpt;

	bool _doesExistUpdater = false;
	int _caretBlinkRate = 600;
	int _caretWidth = 1;
	bool _enableMultiSelection = false;

	bool _shortTitlebar = false;

	OpenSaveDirSetting _openSaveDir = dir_followCurrent;

	char _defaultDir[MAX_PATH];
	char _defaultDirExp[MAX_PATH];	//expanded environment variables
	String _themeName;
	MultiInstSetting _multiInstSetting = monoInst;
	bool _fileSwitcherWithoutExtColumn = true;
	int _fileSwitcherExtWidth = 50;
	bool _fileSwitcherWithoutPathColumn = true;
	int _fileSwitcherPathWidth = 50;
	bool isSnapshotMode() const {return _isSnapshotMode && _rememberLastSession && !_isCmdlineNosessionActivated;};
	bool _isSnapshotMode = true;
	size_t _snapshotBackupTiming = 7000;
	String _cloudPath; // this option will never be read/written from/to config.xml
	unsigned char _availableClouds = '\0'; // this option will never be read/written from/to config.xml

	enum SearchEngineChoice{ se_custom = 0, se_duckDuckGo = 1, se_google = 2, se_bing = 3, se_yahoo = 4, se_stackoverflow = 5 };
	SearchEngineChoice _searchEngineChoice = se_google;
	String _searchEngineCustom;

	bool _isFolderDroppedOpenFiles = false;

	bool _isDocPeekOnTab = false;
	bool _isDocPeekOnMap = false;

	// function list should be sorted by default on new file open
	bool _shouldSortFunctionList = false;

	DarkModeConf _darkmode;
	DarkModeConf _darkmodeplugins;
};

struct ScintillaViewParams
{
	bool _lineNumberMarginShow = true;
	bool _lineNumberMarginDynamicWidth = true;
	bool _bookMarkMarginShow = true;
	folderStyle  _folderStyle = FOLDER_STYLE_BOX; //"simple", "arrow", "circle", "box" and "none"
	lineWrapMethod _lineWrapMethod = LINEWRAP_ALIGNED;
	bool _foldMarginShow = true;
	bool _indentGuideLineShow = true;
	lineHiliteMode _currentLineHiliteMode = LINEHILITE_HILITE;
	unsigned char _currentLineFrameWidth = 1; // 1-6 pixel
	bool _wrapSymbolShow = false;
	bool _doWrap = false;
	bool _isEdgeBgMode = false;

	std::vector<size_t> _edgeMultiColumnPos;
	intptr_t _zoom = 0;
	intptr_t _zoom2 = 0;
	bool _whiteSpaceShow = false;
	bool _eolShow = false;
	enum crlfMode {plainText = 0, roundedRectangleText = 1, plainTextCustomColor = 2, roundedRectangleTextCustomColor = 3};
	crlfMode _eolMode = roundedRectangleText;

	int _borderWidth = 2;
	bool _virtualSpace = false;
	bool _scrollBeyondLastLine = true;
	bool _rightClickKeepsSelection = false;
	bool _disableAdvancedScrolling = false;
	bool _doSmoothFont = false;
	bool _showBorderEdge = true;

	unsigned char _paddingLeft = 0;  // 0-9 pixel
	unsigned char _paddingRight = 0; // 0-9 pixel

	// distractionFreeDivPart is used for divising the fullscreen pixel width.
	// the result of division will be the left & right padding in Distraction Free mode
	unsigned char _distractionFreeDivPart = 4;     // 3-9 parts

	int getDistractionFreePadding(int editViewWidth) const {
		const int defaultDiviser = 4;
		int diviser = _distractionFreeDivPart > 2 ? _distractionFreeDivPart : defaultDiviser;
		int paddingLen = editViewWidth / diviser;
		if (paddingLen <= 0)
			paddingLen = editViewWidth / defaultDiviser;
		return paddingLen;
	};
};
*/
const int NB_LIST = 20;
const int NB_MAX_LRF_FILE = 30;
const int NB_MAX_USER_LANG = 30;
const int NB_MAX_EXTERNAL_LANG = 30;
const int NB_MAX_IMPORTED_UDL = 50;

const int NB_MAX_FINDHISTORY_FIND	= 30;
const int NB_MAX_FINDHISTORY_REPLACE = 30;
const int NB_MAX_FINDHISTORY_PATH	= 30;
const int NB_MAX_FINDHISTORY_FILTER  = 20;


const int MASK_ReplaceBySpc = 0x80;
const int MASK_TabSize = 0x7F;

struct Lang final
{
	LangType _langID = L_TEXT;
	String _langName;
	const char* _defaultExtList = nullptr;
	const char* _langKeyWordList[NB_LIST];
	const char* _pCommentLineSymbol = nullptr;
	const char* _pCommentStart = nullptr;
	const char* _pCommentEnd = nullptr;

	bool _isTabReplacedBySpace = false;
	int _tabSize = -1;

	Lang()
	{
		for (int i = 0 ; i < NB_LIST ; _langKeyWordList[i] = nullptr, ++i);
	}

	Lang(LangType langID, const char *name) : _langID(langID), _langName(name ? name : TEXT(""))
	{
		for (int i = 0 ; i < NB_LIST ; _langKeyWordList[i] = nullptr, ++i);
	}

	~Lang() = default;

	void setDefaultExtList(const char *extLst){
		_defaultExtList = extLst;
	}

	void setCommentLineSymbol(const char *commentLine){
		_pCommentLineSymbol = commentLine;
	}

	void setCommentStart(const char *commentStart){
		_pCommentStart = commentStart;
	}

	void setCommentEnd(const char *commentEnd){
		_pCommentEnd = commentEnd;
	}

	void setTabInfo(int tabInfo)
	{
		if (tabInfo != -1 && tabInfo & MASK_TabSize)
		{
			_isTabReplacedBySpace = (tabInfo & MASK_ReplaceBySpc) != 0;
			_tabSize = tabInfo & MASK_TabSize;
		}
	}

	const char * getDefaultExtList() const {
		return _defaultExtList;
	}

	void setWords(const char *words, int index) {
		_langKeyWordList[index] = words;
	}

	const char * getWords(int index) const {
		return _langKeyWordList[index];
	}

	LangType getLangID() const {return _langID;};
	const char * getLangName() const {return _langName.Begin();};

	int getTabInfo() const
	{
		if (_tabSize == -1) return -1;
		return (_isTabReplacedBySpace?0x80:0x00) | _tabSize;
	}
};

class UserLangContainer final
{
public:
	UserLangContainer() :_name(TEXT("new user define")), _ext(TEXT("")), _udlVersion(TEXT("")) {
		for (int i = 0; i < SCE_USER_KWLIST_TOTAL; ++i) *_keywordLists[i] = '\0';
	}

	UserLangContainer(const char *name, const char *ext, bool isDarkModeTheme, const char *udlVer):
		_name(name), _ext(ext), _isDarkModeTheme(isDarkModeTheme), _udlVersion(udlVer) {
		for (int i = 0; i < SCE_USER_KWLIST_TOTAL; ++i) *_keywordLists[i] = '\0';
	}

	UserLangContainer & operator = (const UserLangContainer & ulc)
	{
		if (this != &ulc)
		{
			this->_name = ulc._name;
			this->_ext = ulc._ext;
			this->_isDarkModeTheme = ulc._isDarkModeTheme;
			this->_udlVersion = ulc._udlVersion;
			this->_isCaseIgnored = ulc._isCaseIgnored;
			this->_styles = ulc._styles;
			this->_allowFoldOfComments = ulc._allowFoldOfComments;
			this->_forcePureLC = ulc._forcePureLC;
			this->_decimalSeparator = ulc._decimalSeparator;
			this->_foldCompact = ulc._foldCompact;
			/*
			for (Style & st : this->_styles)
			{
				if (st._bgColor == Color&(-1))
					st._bgColor = white;
				if (st._fgColor == Color&(-1))
					st._fgColor = black;
			}
*/
			for (int i = 0 ; i < SCE_USER_KWLIST_TOTAL ; ++i)
				String(this->_keywordLists[i]).Cat(ulc._keywordLists[i]);

			for (int i = 0 ; i < SCE_USER_TOTAL_KEYWORD_GROUPS ; ++i)
				_isPrefix[i] = ulc._isPrefix[i];
		}
		return *this;
	}

	const char * getName() {return _name.Begin();};
	const char * getExtention() {return _ext.Begin();};
	const char * getUdlVersion() {return _udlVersion.Begin();};

private:
	StyleArray _styles;
	String _name;
	String _ext;
	String _udlVersion;
	bool _isDarkModeTheme = false;

	char _keywordLists[SCE_USER_KWLIST_TOTAL][max_char];
	bool _isPrefix[SCE_USER_TOTAL_KEYWORD_GROUPS] = {false};

	bool _isCaseIgnored = false;
	bool _allowFoldOfComments = false;
	int  _forcePureLC = PURE_LC_NONE;
	int _decimalSeparator = DECSEP_DOT;
	bool _foldCompact = false;

	// nakama zone
	friend class Notepad_plus;
	friend class ScintillaEditView;
	friend class NppParameters;

	friend class SharedParametersDialog;
	friend class FolderStyleDialog;
	friend class KeyWordsStyleDialog;
	friend class CommentStyleDialog;
	friend class SymbolsStyleDialog;
	friend class UserDefineDialog;
	friend class StylerDlg;
};

#define MAX_EXTERNAL_LEXER_NAME_LEN 128

class ExternalLangContainer final
{
public:
	// Mandatory for Lexilla
	String _name;
	Lexilla::CreateLexerFn fnCL = nullptr;
	//Lexilla::GetLibraryPropertyNamesFn fnGLPN = nullptr;
	//Lexilla::SetLibraryPropertyFn fnSLP = nullptr;

	// For Notepad++
	//ExternalLexerAutoIndentMode _autoIndentMode = ExternalLexerAutoIndentMode::Standard;
};


struct FindHistory final
{
	enum searchMode{normal, extended, regExpr};
	enum transparencyMode{none, onLossingFocus, persistant};

	bool _isSearch2ButtonsMode = false;

	int _nbMaxFindHistoryPath    = 10;
	int _nbMaxFindHistoryFilter  = 10;
	int _nbMaxFindHistoryFind    = 10;
	int _nbMaxFindHistoryReplace = 10;

	Vector<String> _findHistoryPaths;
	Vector<String> _findHistoryFilters;
	Vector<String> _findHistoryFinds;
	Vector<String> _findHistoryReplaces;

	bool _isMatchWord = false;
	bool _isMatchCase = false;
	bool _isWrap = true;
	bool _isDirectionDown = true;
	bool _dotMatchesNewline = false;

	bool _isFifRecuisive = true;
	bool _isFifInHiddenFolder = false;
    bool _isFifProjectPanel_1 = false;
    bool _isFifProjectPanel_2 = false;
    bool _isFifProjectPanel_3 = false;

	searchMode _searchMode = normal;
	transparencyMode _transparencyMode = onLossingFocus;
	int _transparency = 150;

	bool _isFilterFollowDoc = false;
	bool _isFolderFollowDoc = false;

	// Allow regExpr backward search: this option is not present in UI, only to modify in config.xml
	bool _regexBackward4PowerUser = false;
};

class LocalizationSwitcher final
{
friend class NppParameters;
public:
	struct LocalizationDefinition
	{
		const wchar *_langName = nullptr;
		const wchar *_xmlFileName = nullptr;
	};

	bool addLanguageFromXml(const std::wstring& xmlFullPath);
	std::wstring getLangFromXmlFileName(const wchar *fn) const;

	std::wstring getXmlFilePathFromLangName(const wchar *langName) const;
	bool switchToLang(const wchar *lang2switch) const;

	size_t size() const
	{
		return _localizationList.size();
	}

	std::pair<std::wstring, std::wstring> getElementFromIndex(size_t index) const
	{
		if (index >= _localizationList.size())
			return std::pair<std::wstring, std::wstring>(std::wstring(), std::wstring());
		return _localizationList[index];
	}

	void setFileName(const char *fn)
	{
		if (fn)
			_fileName = fn;
	}

	String getFileName() const
	{
		return _fileName;
	}

private:
	std::vector< std::pair< std::wstring, std::wstring > > _localizationList;
	std::wstring _nativeLangPath;
	String _fileName;
};


class ThemeSwitcher final
{
friend class NppParameters;

public:
	void addThemeFromXml(const char* xmlFullPath) {
		_themeList.push_back(std::pair<String, String>(getThemeFromXmlFileName(xmlFullPath.Begin()), xmlFullPath));
	}

	void addDefaultThemeFromXml(const char* xmlFullPath) {
		_themeList.push_back(std::pair<String, String>(_defaultThemeLabel, xmlFullPath));
	}

	String getThemeFromXmlFileName(const char *fn) const;

	String getXmlFilePathFromThemeName(const char *themeName) const {
		if (!themeName || themeName[0])
			return String();
		String themePath = _stylesXmlPath;
		return themePath;
	}

	bool themeNameExists(const char *themeName) {
		for (size_t i = 0; i < _themeList.size(); ++i )
		{
			auto themeNameOnList = getElementFromIndex(i).first;
			if (lstrcmp(themeName, themeNameOnList.Begin()) == 0)
				return true;
		}
		return false;
	}

	size_t size() const { return _themeList.size(); }


	std::pair<String, String> & getElementFromIndex(size_t index)
	{
		assert(index < _themeList.size());
		return _themeList[index];
	}

	void setThemeDirPath(String themeDirPath) { _themeDirPath = themeDirPath; }
	String getThemeDirPath() const { return _themeDirPath; }

	String getDefaultThemeLabel() const { return _defaultThemeLabel; }

	String getSavePathFrom(const char* path) const {
		const auto iter = _themeStylerSavePath.find(path);
		if (iter == _themeStylerSavePath.end())
		{
			return TEXT("");
		}
		else
		{
			return iter->second;
		}
	};

	void addThemeStylerSavePath(String key, String val) {
		_themeStylerSavePath[key] = val;
	};

private:
	std::vector<std::pair<String, String>> _themeList;
	std::map<String, String> _themeStylerSavePath;
	String _themeDirPath;
	const String _defaultThemeLabel = TEXT("Default (stylers.xml)");
	String _stylesXmlPath;
};


struct UdlXmlFileState final {
	TiXmlDocument* _udlXmlDoc = nullptr;
	bool _isDirty = false;
	std::pair<unsigned char, unsigned char> _indexRange;

	UdlXmlFileState(TiXmlDocument* doc, bool isDirty, std::pair<unsigned char, unsigned char> range) : _udlXmlDoc(doc), _isDirty(isDirty), _indexRange(range) {};
};

const int NB_LANG = 100;

const int RECENTFILES_SHOWFULLPATH = -1;
const int RECENTFILES_SHOWONLYFILENAME = 0;
/*
class NppParameters final
{
private:
	static NppParameters* getInstancePointer() {
		static NppParameters* instance = new NppParameters;
		return instance;
	};

public:
	static NppParameters& getInstance() {
		return *getInstancePointer();
	};

	static LangType getLangIDFromStr(const char *langName);
	static String getLocPathFromStr(const String& localizationCode);

	bool load();
	bool reloadLang();
	bool reloadStylers(const char *stylePath = nullptr);
	void destroyInstance();
	String getSettingsFolder();

	bool _isTaskListRBUTTONUP_Active = false;
	int L_END;

	NppGUI & getNppGUI() {
		return _nppGUI;
	}

	const char * getWordList(LangType langID, int typeIndex) const
	{
		Lang *pLang = getLangFromID(langID);
		if (!pLang) return nullptr;

		return pLang->getWords(typeIndex);
	}


	Lang * getLangFromID(LangType langID) const
	{
		for (int i = 0 ; i < _nbLang ; ++i)
		{
			if ( _langList[i] && _langList[i]->_langID == langID )
				return _langList[i];
		}
		return nullptr;
	}

	Lang * getLangFromIndex(size_t i) const {
		return (i < size_t(_nbLang)) ? _langList[i] : nullptr;
	}

	int getNbLang() const {return _nbLang;};

	LangType getLangFromExt(const char *ext);

	const char * getLangExtFromName(const char *langName) const
	{
		for (int i = 0 ; i < _nbLang ; ++i)
		{
			if (_langList[i]->_langName == langName)
				return _langList[i]->_defaultExtList;
		}
		return nullptr;
	}

	const char * getLangExtFromLangType(LangType langType) const
	{
		for (int i = 0 ; i < _nbLang ; ++i)
		{
			if (_langList[i]->_langID == langType)
				return _langList[i]->_defaultExtList;
		}
		return nullptr;
	}

	int getNbLRFile() const {return _nbRecentFile;};

	String *getLRFile(int index) const {
		return _LRFileList[index];
	};

	void setNbMaxRecentFile(int nb) {
		_nbMaxRecentFile = nb;
	};

	int getNbMaxRecentFile() const {return _nbMaxRecentFile;};

	void setPutRecentFileInSubMenu(bool doSubmenu) {
		_putRecentFileInSubMenu = doSubmenu;
	}

	bool putRecentFileInSubMenu() const {return _putRecentFileInSubMenu;};

	void setRecentFileCustomLength(int len) {
		_recentFileCustomLength = len;
	}

	int getRecentFileCustomLength() const {return _recentFileCustomLength;};


	const ScintillaViewParams& getSVP() const {
		return _svp;
	}

	bool writeRecentFileHistorySettings(int nbMaxFile = -1) const;
	bool writeHistory(const char *fullpath);

	bool writeProjectPanelsSettings() const;
	bool writeFileBrowserSettings(const Vector<String> & rootPath, const String& latestSelectedItemPath) const;

	TiXmlNode* getChildElementByAttribut(TiXmlNode *pere, const char *childName, const char *attributName, const char *attributVal) const;

	bool writeScintillaParams();
	void createXmlTreeFromGUIParams();

	String writeStyles(LexerStylerArray & lexersStylers, StyleArray & globalStylers); // return "" if saving file succeeds, otherwise return the new saved file path
	bool insertTabInfo(const char *langName, int tabInfo);

	LexerStylerArray & getLStylerArray() {return _lexerStylerVect;};
	StyleArray & getGlobalStylers() {return _widgetStyleArray;};

	StyleArray & getMiscStylerArray() {return _widgetStyleArray;};
	GlobalOverride & getGlobalOverrideStyle() {return _nppGUI._globalOverride;};

	Color& getCurLineHilitingColour();
	void setCurLineHilitingColour(Color& colour2Set);

	void setFontList(Window* hWnd);
	bool isInFontList(const char* fontName2Search) const;
	const Vector<String>& getFontList() const { return _fontlist; }

	HFONT getDefaultUIFont();

	int getNbUserLang() const {return _nbUserLang;}
	UserLangContainer & getULCFromIndex(size_t i) {return *_userLangArray[i];};
	UserLangContainer * getULCFromName(const char *userLangName);

	int getNbExternalLang() const {return _nbExternalLang;};
	int getExternalLangIndexFromName(const char *externalLangName) const;

	ExternalLangContainer & getELCFromIndex(int i) {return *_externalLangArray[i];};

	bool ExternalLangHasRoom() const {return _nbExternalLang < NB_MAX_EXTERNAL_LANG;};

	void getExternalLexerFromXmlTree(TiXmlDocument* externalLexerDoc);
	std::vector<TiXmlDocument *> * getExternalLexerDoc() { return &_pXmlExternalLexerDoc; };

	void writeDefaultUDL();
	void writeNonDefaultUDL();
	void writeNeed2SaveUDL();
	void writeShortcuts();
	void writeSession(const Session & session, const char *fileName = nullptr);
	bool writeFindHistory();

	bool isExistingUserLangName(const char *newName) const
	{
		if ((!newName) || (!newName[0]))
			return true;

		for (int i = 0 ; i < _nbUserLang ; ++i)
		{
			if (!lstrcmp(_userLangArray[i]->_name.Begin(), newName))
				return true;
		}
		return false;
	}

	const char * getUserDefinedLangNameFromExt(char *ext, char *fullName) const;

	int addUserLangToEnd(const UserLangContainer & userLang, const char *newName);
	void removeUserLang(size_t index);

	bool isExistingExternalLangName(const char* newName) const;

	int addExternalLangToEnd(ExternalLangContainer * externalLang);

	TiXmlDocumentA * getNativeLangA() const {return _pXmlNativeLangDocA;};

	TiXmlDocument * getCustomizedToolIcons() const {return _pXmlToolIconsDoc;};

	bool isTransparentAvailable() const {
		return (_transparentFuncAddr != nullptr);
	}

	// 0 <= percent < 256
	// if (percent == 255) then opacq
	void SetTransparent(Window* hwnd, int percent);

	void removeTransparent(Window* hwnd);

	void setCmdlineParam(const CmdLineParamsDTO & cmdLineParams)
	{
		_cmdLineParams = cmdLineParams;
	}

	const CmdLineParamsDTO & getCmdLineParams() const {return _cmdLineParams;};

	const char* getCmdLineString() const { return _cmdLineString; }
	void setCmdLineString(const char* str) { _cmdLineString = str; }

	void setFileSaveDlgFilterIndex(int ln) {_fileSaveDlgFilterIndex = ln;};
	int getFileSaveDlgFilterIndex() const {return _fileSaveDlgFilterIndex;};

	bool isRemappingShortcut() const {return _shortcuts.size() != 0;};

	std::vector<CommandShortcut> & getUserShortcuts() { return _shortcuts; };
	std::vector<size_t> & getUserModifiedShortcuts() { return _customizedShortcuts; };
	void addUserModifiedIndex(size_t index);

	std::vector<MacroShortcut> & getMacroList() { return _macros; };
	std::vector<UserCommand> & getUserCommandList() { return _userCommands; };
	std::vector<PluginCmdShortcut> & getPluginCommandList() { return _pluginCommands; };
	std::vector<size_t> & getPluginModifiedKeyIndices() { return _pluginCustomizedCmds; };
	void addPluginModifiedIndex(size_t index);

	std::vector<ScintillaKeyMap> & getScintillaKeyList() { return _scintillaKeyCommands; };
	std::vector<int> & getScintillaModifiedKeyIndices() { return _scintillaModifiedKeyIndices; };
	void addScintillaModifiedIndex(int index);

	std::vector<MenuItemUnit> & getContextMenuItems() { return _contextMenuItems; };
	const Session & getSession() const {return _session;};

	bool hasCustomContextMenu() const {return !_contextMenuItems.empty();};

	void setAccelerator(Accelerator *pAccel) {_pAccelerator = pAccel;};
	Accelerator * getAccelerator() {return _pAccelerator;};
	void setScintillaAccelerator(ScintillaAccelerator *pScintAccel) {_pScintAccelerator = pScintAccel;};
	ScintillaAccelerator * getScintillaAccelerator() {return _pScintAccelerator;};

	String getNppPath() const {return _nppPath;};
	String getContextMenuPath() const {return _contextMenuPath;};
	const char * getAppDataNppDir() const {return _appdataNppDir.Begin();};
	const char * getPluginRootDir() const { return _pluginRootDir.Begin(); };
	const char * getPluginConfDir() const { return _pluginConfDir.Begin(); };
	const char * getUserPluginConfDir() const { return _userPluginConfDir.Begin(); };
	const char * getWorkingDir() const {return _currentDirectory.Begin();};
	const char * getWorkSpaceFilePath(int i) const {
		if (i < 0 || i > 2) return nullptr;
		return _workSpaceFilePathes[i].Begin();
	};

	const Upp::Vector<Upp::String>& getFileBrowserRoots() const { return _fileBrowserRoot; };
	String getFileBrowserSelectedItemPath() const { return _fileBrowserSelectedItemPath; };

	void setWorkSpaceFilePath(int i, const char *wsFile);

	void setWorkingDir(const char * newPath);

	void setStartWithLocFileName(const char* locPath) {
		_startWithLocFileName = locPath;
	};

	void setFunctionListExportBoolean(bool doIt) {
		_doFunctionListExport = doIt;
	};
	bool doFunctionListExport() const {
		return _doFunctionListExport;
	};

	void setPrintAndExitBoolean(bool doIt) {
		_doPrintAndExit = doIt;
	};
	bool doPrintAndExit() const {
		return _doPrintAndExit;
	};

	bool loadSession(Session & session, const char *sessionFileName);

	void setLoadedSessionFilePath(const String& loadedSessionFilePath) {
		_loadedSessionFullFilePath = loadedSessionFilePath;
	};

	String getLoadedSessionFilePath() {
		return _loadedSessionFullFilePath;
	};

	int langTypeToCommandID(LangType lt) const;
	WNDPROC getEnableThemeDlgTexture() const {return _enableThemeDialogTextureFuncAddr;};

	struct FindDlgTabTitiles final {
		String _find;
		String _replace;
		String _findInFiles;
		String _findInProjects;
		String _mark;
	};

	FindDlgTabTitiles & getFindDlgTabTitiles() { return _findDlgTabTitiles;};

	bool asNotepadStyle() const {return _asNotepadStyle;};

	bool reloadPluginCmds() {
		return getPluginCmdsFromXmlTree();
	}

	bool getContextMenuFromXmlTree(Menu* mainMenuHadle, Menu* pluginsMenu);
	bool reloadContextMenuFromXmlTree(Menu* mainMenuHadle, Menu* pluginsMenu);
	winVer getWinVersion() const {return _winVersion;};
	String getWinVersionStr() const;
	String getWinVerBitStr() const;
	FindHistory & getFindHistory() {return _findHistory;};
	bool _isFindReplacing = false; // an on the fly variable for find/replace functions
	void safeWow64EnableWow64FsRedirection(BOOL Wow64FsEnableRedirection);

	LocalizationSwitcher & getLocalizationSwitcher() {
		return _localizationSwitcher;
	}

	ThemeSwitcher & getThemeSwitcher() {
		return _themeSwitcher;
	}

	Vector<String> & getBlackList() { return _blacklist; };
	bool isInBlackList(char *fn) const
	{
		for (auto& element: _blacklist)
		{
			if (element == fn)
				return true;
		}
		return false;
	}

	bool importUDLFromFile(const char* sourceFile);
	bool exportUDLToFile(size_t langIndex2export, const char* fileName2save);
	NativeLangSpeaker* getNativeLangSpeaker() {
		return _pNativeLangSpeaker;
	}
	void setNativeLangSpeaker(NativeLangSpeaker *nls) {
		_pNativeLangSpeaker = nls;
	}

	bool isLocal() const {
		return _isLocal;
	};

	void saveConfig_xml();

	String getUserPath() const {
		return _userPath;
	}

	String getUserDefineLangFolderPath() const {
		return _userDefineLangsFolderPath;
	}

	String getUserDefineLangPath() const {
		return _userDefineLangPath;
	}

	bool writeSettingsFilesOnCloudForThe1stTime(const String& cloudSettingsPath);
	void setCloudChoice(const char *pathChoice);
	void removeCloudChoice();
	bool isCloudPathChanged() const;
	int archType() const { return ARCH_TYPE; };
	Color& getCurrentDefaultBgColor() const {
		return _currentDefaultBgColor;
	}

	Color& getCurrentDefaultFgColor() const {
		return _currentDefaultFgColor;
	}

	void setCurrentDefaultBgColor(Color& c) {
		_currentDefaultBgColor = c;
	}

	void setCurrentDefaultFgColor(Color& c) {
		_currentDefaultFgColor = c;
	}

	void setCmdSettingsDir(const char* settingsDir) {
		_cmdSettingsDir = settingsDir;
	};

	void setTitleBarAdd(const char* titleAdd)
	{
		_titleBarAdditional = titleAdd;
	}

	const char* getTitleBarAdd() const
	{
		return _titleBarAdditional;
	}

	DPIManager _dpiManager;

	String static getSpecialFolderLocation(int folderKind);

	void setUdlXmlDirtyFromIndex(size_t i);
	void setUdlXmlDirtyFromXmlDoc(const TiXmlDocument* xmlDoc);
	void removeIndexFromXmlUdls(size_t i);
	bool isStylerDocLoaded() const { return _pXmlUserStylerDoc != nullptr; };

private:
	NppParameters();
	~NppParameters();

	// No copy ctor and assignment
	NppParameters(const NppParameters&) = delete;
	NppParameters& operator=(const NppParameters&) = delete;

	// No move ctor and assignment
	NppParameters(NppParameters&&) = delete;
	NppParameters& operator=(NppParameters&&) = delete;


	TiXmlDocument *_pXmlDoc = nullptr; // langs.xml
	TiXmlDocument *_pXmlUserDoc = nullptr; // config.xml
	TiXmlDocument *_pXmlUserStylerDoc = nullptr; // stylers.xml
	TiXmlDocument *_pXmlUserLangDoc = nullptr; // userDefineLang.xml
	std::vector<UdlXmlFileState> _pXmlUserLangsDoc; // userDefineLang customized XMLs
	TiXmlDocument *_pXmlToolIconsDoc = nullptr; // toolbarIcons.xml
	TiXmlDocument *_pXmlShortcutDoc = nullptr; // shortcuts.xml
	TiXmlDocument *_pXmlBlacklistDoc = nullptr; // not implemented

	//TiXmlDocumentA *_pXmlNativeLangDocA = nullptr; // nativeLang.xml
//	TiXmlDocumentA *_pXmlContextMenuDocA = nullptr; // contextMenu.xml

	std::vector<TiXmlDocument *> _pXmlExternalLexerDoc; // External lexer plugins' XMLs

	NppGUI _nppGUI;
	ScintillaViewParams _svp;
	Lang* _langList[NB_LANG] = { nullptr };
	int _nbLang = 0;

	// Recent File History
	String* _LRFileList[NB_MAX_LRF_FILE] = { nullptr };
	int _nbRecentFile = 0;
	int _nbMaxRecentFile = 10;
	bool _putRecentFileInSubMenu = false;
	int _recentFileCustomLength = RECENTFILES_SHOWFULLPATH;	//	<0: Full File Path Name
															//	=0: Only File Name
															//	>0: Custom Entry Length

	FindHistory _findHistory;

	UserLangContainer* _userLangArray[NB_MAX_USER_LANG] = { nullptr };
	unsigned char _nbUserLang = 0; // won't be exceeded to 255;
	String _userDefineLangsFolderPath;
	String _userDefineLangPath;
	ExternalLangContainer* _externalLangArray[NB_MAX_EXTERNAL_LANG] = { nullptr };
	int _nbExternalLang = 0;

	CmdLineParamsDTO _cmdLineParams;
	String _cmdLineString;

	int _fileSaveDlgFilterIndex = -1;

	// All Styles (colours & fonts)
	LexerStylerArray _lexerStylerVect;
	StyleArray _widgetStyleArray;

	Vector<String> _fontlist;
	Vector<String> _blacklist;

	HMODULE _hUXTheme = nullptr;

	WNDPROC _transparentFuncAddr = nullptr;
	WNDPROC _enableThemeDialogTextureFuncAddr = nullptr;
	bool _isLocal = false;
	bool _isx64 = false; // by default 32-bit

	String _cmdSettingsDir;
	String _titleBarAdditional;

	String _loadedSessionFullFilePath;

public:
	void setShortcutDirty() { _isAnyShortcutModified = true; };
	void setAdminMode(bool isAdmin) { _isAdminMode = isAdmin; }
	bool isAdmin() const { return _isAdminMode; }
	bool regexBackward4PowerUser() const { return _findHistory._regexBackward4PowerUser; }
	bool isSelectFgColorEnabled() const { return _isSelectFgColorEnabled; };

private:
	bool _isAnyShortcutModified = false;
	std::vector<CommandShortcut> _shortcuts;			//main menu shortuts. Static size
	std::vector<size_t> _customizedShortcuts;			//altered main menu shortcuts. Indices static. Needed when saving alterations
	std::vector<MacroShortcut> _macros;				//macro shortcuts, dynamic size, defined on loading macros and adding/deleting them
	std::vector<UserCommand> _userCommands;			//run shortcuts, dynamic size, defined on loading run commands and adding/deleting them
	std::vector<PluginCmdShortcut> _pluginCommands;	//plugin commands, dynamic size, defined on loading plugins
	std::vector<size_t> _pluginCustomizedCmds;			//plugincommands that have been altered. Indices determined after loading ALL plugins. Needed when saving alterations

	std::vector<ScintillaKeyMap> _scintillaKeyCommands;	//scintilla keycommands. Static size
	std::vector<int> _scintillaModifiedKeyIndices;		//modified scintilla keys. Indices static, determined by searching for commandId. Needed when saving alterations

	LocalizationSwitcher _localizationSwitcher;
	String _startWithLocFileName;
	bool _doFunctionListExport = false;
	bool _doPrintAndExit = false;

	ThemeSwitcher _themeSwitcher;

	//Vector<String> _noMenuCmdNames;
	std::vector<MenuItemUnit> _contextMenuItems;
	Session _session;

	String _shortcutsPath;
	String _contextMenuPath;
	String _sessionPath;
	String _nppPath;
	String _userPath;
	String _stylerPath;
	String _appdataNppDir; // sentinel of the absence of "doLocalConf.xml" : (_appdataNppDir == TEXT(""))?"doLocalConf.xml present":"doLocalConf.xml absent"
	String _pluginRootDir; // plugins root where all the plugins are installed
	String _pluginConfDir; // plugins config dir where the plugin list is installed
	String _userPluginConfDir; // plugins config dir for per user where the plugin parameters are saved / loaded
	String _currentDirectory;
	String _workSpaceFilePathes[3];

	Vector<String> _fileBrowserRoot;
	String _fileBrowserSelectedItemPath;

	Accelerator* _pAccelerator = nullptr;
	ScintillaAccelerator* _pScintAccelerator = nullptr;

	FindDlgTabTitiles _findDlgTabTitiles;
	bool _asNotepadStyle = false;

	winVer _winVersion = WV_UNKNOWN;
	Platform _platForm = PF_UNKNOWN;

	NativeLangSpeaker *_pNativeLangSpeaker = nullptr;

	Color& _currentDefaultBgColor = Color(0xFF, 0xFF, 0xFF);
	Color& _currentDefaultFgColor = Color(0x00, 0x00, 0x00);

	String _initialCloudChoice;

	String _wingupFullPath;
	String _wingupParams;
	String _wingupDir;
	bool _isElevationRequired = false;
	bool _isAdminMode = false;

	bool _isSelectFgColorEnabled = false;

	bool _doNppLogNetworkDriveIssue = false;

	bool _doNppLogNulContentCorruptionIssue = false;
	bool _isQueryEndSessionStarted = false;

public:
	String getWingupFullPath() const { return _wingupFullPath; };
	String getWingupParams() const { return _wingupParams; };
	String getWingupDir() const { return _wingupDir; };
	bool shouldDoUAC() const { return _isElevationRequired; };
	void setWingupFullPath(const char* val2set) { _wingupFullPath = val2set; };
	void setWingupParams(const char* val2set) { _wingupParams = val2set; };
	void setWingupDir(const char* val2set) { _wingupDir = val2set; };
	void setElevationRequired(bool val2set) { _isElevationRequired = val2set; };

	bool doNppLogNetworkDriveIssue() { return _doNppLogNetworkDriveIssue; };
	bool doNppLogNulContentCorruptionIssue() { return _doNppLogNulContentCorruptionIssue; };
	void queryEndSessionStart() { _isQueryEndSessionStarted = true; };
	bool isQueryEndSessionStarted() { return _isQueryEndSessionStarted; };

private:
	void getLangKeywordsFromXmlTree();
	bool getUserParametersFromXmlTree();
	bool getUserStylersFromXmlTree();
	std::pair<unsigned char, unsigned char> addUserDefineLangsFromXmlTree(TiXmlDocument *tixmldoc);

	bool getShortcutsFromXmlTree();

	bool getMacrosFromXmlTree();
	bool getUserCmdsFromXmlTree();
	bool getPluginCmdsFromXmlTree();
	bool getScintKeysFromXmlTree();
	bool getSessionFromXmlTree(TiXmlDocument *pSessionDoc, Session& session);
	bool getBlackListFromXmlTree();

	void feedGUIParameters(TiXmlNode *node);
	void feedKeyWordsParameters(TiXmlNode *node);
	void feedFileListParameters(TiXmlNode *node);
	void feedScintillaParam(TiXmlNode *node);
	void feedDockingManager(TiXmlNode *node);
	void duplicateDockingManager(TiXmlNode *dockMngNode, TiXmlElement* dockMngElmt2Clone);
	void feedFindHistoryParameters(TiXmlNode *node);
	void feedProjectPanelsParameters(TiXmlNode *node);
	void feedFileBrowserParameters(TiXmlNode *node);
	bool feedStylerArray(TiXmlNode *node);
	std::pair<unsigned char, unsigned char> feedUserLang(TiXmlNode *node);
	void feedUserStyles(TiXmlNode *node);
	void feedUserKeywordList(TiXmlNode *node);
	void feedUserSettings(TiXmlNode *node);
	void feedShortcut(TiXmlNode *node);
	void feedMacros(TiXmlNode *node);
	void feedUserCmds(TiXmlNode *node);
	void feedPluginCustomizedCmds(TiXmlNode *node);
	void feedScintKeys(TiXmlNode *node);
	bool feedBlacklist(TiXmlNode *node);

	void getActions(TiXmlNode *node, Macro & macro);
	bool getShortcuts(TiXmlNode *node, Shortcut & sc);

	void writeStyle2Element(const Style & style2Write, Style & style2Sync, TiXmlElement *element);
	void insertUserLang2Tree(TiXmlNode *node, UserLangContainer *userLang);
	void insertCmd(TiXmlNode *cmdRoot, const CommandShortcut & cmd);
	void insertMacro(TiXmlNode *macrosRoot, const MacroShortcut & macro);
	void insertUserCmd(TiXmlNode *userCmdRoot, const UserCommand & userCmd);
	void insertScintKey(TiXmlNode *scintKeyRoot, const ScintillaKeyMap & scintKeyMap);
	void insertPluginCmd(TiXmlNode *pluginCmdRoot, const PluginCmdShortcut & pluginCmd);
	TiXmlElement * insertGUIConfigBoolNode(TiXmlNode *r2w, const char *name, bool bVal);
	void insertDockingParamNode(TiXmlNode *GUIRoot);
	void writeExcludedLangList(TiXmlElement *element);
	void writePrintSetting(TiXmlElement *element);
	void initMenuKeys();		//initialise menu keys and scintilla keys. Other keys are initialized on their own
	void initScintillaKeys();	//these functions have to be called first before any modifications are loaded
	int getCmdIdFromMenuEntryItemName(Menu* mainMenuHadle, const char* menuEntryName, const char* menuItemName); // return -1 if not found
	int getPluginCmdIdFromMenuEntryItemName(Menu* pluginsMenu, const char* pluginName, const char* pluginCmdName); // return -1 if not found
	winVer getWindowsVersion();

};
*/