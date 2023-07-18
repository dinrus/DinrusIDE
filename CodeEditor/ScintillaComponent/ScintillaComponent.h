#ifndef _PowerEditor_ScintillaComponent_ScintillaComponent_h_
#define _PowerEditor_ScintillaComponent_ScintillaComponent_h_
////////////////////////////////////
#include <plugin/TinyXml/tinyxml.h>
#include <mutex>
#include <map>
#include <Scintilla/BoostRegexSearch.h>
#include <vector>
#include <Lexilla/SciLexer.h>
#include <unordered_map>
#include "Utf8_16.h"
#include "Parameters.h"

//#include <PowerEditor/WinControls/StaticDialog/StaticDialog.h>
//#include <PowerEditor/WinControls/TabBar/TabBar.h>
//#include <PowerEditor/WinControls/DockingWnd/DockingDlgInterface.h>
//#include <PowerEditor/WinControls/StatusBar/StatusBar.h>
//#include <PowerEditor/resource.h>
//#include <PowerEditor/MISC/Common/Common.h>
//#include <PowerEditor/WinControls/TabBar/ControlsTab.h>
//#include <PowerEditor/WinControls/ColourPicker/ColourPicker.h>
//#include <PowerEditor/Parameters.h>
//#include <PowerEditor/WinControls/AboutDlg/URLCtrl.h>


namespace Upp{
	namespace ScintillaComponent{
	
class ScintillaEditView;
class UserLangContainer;
struct Style;
class SciBuffer;
class Notepad_plus;
class FunctionCallTip;

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

//////////////////
///AutoCompletion
////////////////////
const size_t tagMaxLen = 256;

struct MatchedCharInserted {
    MatchedCharInserted() = delete;
    char _c;
    size_t _pos;
    MatchedCharInserted(char c, size_t pos) : _c(c), _pos(pos) {};
};

class InsertedMatchedChars {
public:
    void init(ScintillaEditView * pEditView) { _pEditView = pEditView; };
    void removeInvalidElements(MatchedCharInserted mci);
    void add(MatchedCharInserted mci);
    bool isEmpty() const { return _insertedMatchedChars.size() == 0; };
    intptr_t search(char startChar, char endChar, size_t posToDetect);

private:
    std::vector<MatchedCharInserted> _insertedMatchedChars;
    ScintillaEditView * _pEditView = nullptr;
};

class AutoCompletion {
public:
    explicit AutoCompletion(ScintillaEditView * pEditView): _pEditView(pEditView), _funcCalltip(pEditView) {
        //Do not load any language yet
        _insertedMatchedChars.init(_pEditView);
    };

    ~AutoCompletion(){
        delete _pXmlFile;
    };

    enum class AutocompleteColorIndex {
        autocompleteText,
        autocompleteBg,
        selectedText,
        selectedBg,
        calltipBg,
        calltipText,
        calltipHighlight
    };

    bool setLanguage(LangType language);

    //AutoComplete from the list
    bool showApiComplete();
    //WordCompletion from the current file
    bool showWordComplete(bool autoInsert); //autoInsert true if completion should fill in the word on a single match
    // AutoComplete from both the list and the current file
    bool showApiAndWordComplete();
    //Parameter display from the list
    bool showFunctionComplete();
    // Autocomplete from path.
    void showPathCompletion();

    void insertMatchedChars(int character, const MatchedPairConf & matchedPairConf);
    void update(int character);
    void callTipClick(size_t direction);
    void getCloseTag(char *closeTag, size_t closeTagLen, size_t caretPos, bool isHTML);

    static void setColour(Color colour2Set, AutocompleteColorIndex i);
    static void drawAutocomplete(ScintillaEditView* pEditView);

protected:
    static Color _autocompleteBg;
    static Color _autocompleteText;
    static Color _selectedBg;
    static Color _selectedText;
    static Color _calltipBg;
    static Color _calltipText;
    static Color _calltipHighlight;

private:
    bool _funcCompletionActive = false;
    ScintillaEditView * _pEditView = nullptr;
    LangType _curLang = L_TEXT;
    TiXmlDocument *_pXmlFile = nullptr;
    TiXmlElement *_pXmlKeyword = nullptr;
    bool _isFxImageRegistered = false;

    InsertedMatchedChars _insertedMatchedChars;

    bool _ignoreCase = true;

    Vector<String> _keyWordArray;
    String _keyWords;
    size_t _keyWordMaxLen = 0;

    FunctionCallTip _funcCalltip;

    const char * getApiFileName();
    void getWordArray(Vector<String> & wordArray, char *beginChars, char *excludeChars);
};
////////////////////////////////////////////////////////////////////////////////////////
///Buffer.h
//////////////////////
typedef SciBuffer* BufferID;    //each buffer has unique ID by which it can be retrieved
#define BUFFER_INVALID  reinterpret_cast<BufferID>(0)
#define NPP_STYLING_FILESIZE_LIMIT (200 * 1024 * 1024) // 200MB+ file won't be styled

typedef sptr_t Document;

enum DocFileStatus {
    DOC_REGULAR    = 0x01, // should not be combined with anything
    DOC_UNNAMED    = 0x02, // not saved (new ##)
    DOC_DELETED    = 0x04, // doesn't exist in environment anymore, but not DOC_UNNAMED
    DOC_MODIFIED   = 0x08, // File in environment has changed
    DOC_NEEDRELOAD = 0x10  // File is modified & needed to be reload (by log monitoring)
};

enum BufferStatusInfo {
    BufferChangeNone        = 0x000,  // Nothing to change
    BufferChangeLanguage    = 0x001,  // Language was altered
    BufferChangeDirty       = 0x002,  // SciBuffer has changed dirty state
    BufferChangeFormat      = 0x004,  // EOL type was changed
    BufferChangeUnicode     = 0x008,  // Unicode type was changed
    BufferChangeReadonly    = 0x010,  // Readonly state was changed, can be both file and user
    BufferChangeStatus      = 0x020,  // Filesystem Status has changed
    BufferChangeTimestamp   = 0x040,  // Timestamp was changed
    BufferChangeFilename    = 0x080,  // Filename was changed
    BufferChangeRecentTag   = 0x100,  // Recent tag has changed
    BufferChangeLexing      = 0x200,  // Document needs lexing
    BufferChangeMask        = 0x3FF   // Mask: covers all changes
};

enum SavingStatus {
    SaveOK             = 0,
    SaveOpenFailed     = 1,
    SaveWritingFailed  = 2
};

const char UNTITLED_STR[] = TEXT("new ");

//File manager class maintains all buffers
class FileManager final {
public:
    void init(Notepad_plus* pNotepadPlus, ScintillaEditView* pscratchTilla);

    void checkFilesystemChanges(bool bCheckOnlyCurrentBuffer);

    size_t getNbBuffers() const { return _nbBufs; };
    size_t getNbDirtyBuffers() const;
    int getBufferIndexByID(BufferID id);
    SciBuffer * getBufferByIndex(size_t index);
    SciBuffer * getBufferByID(BufferID id) {return static_cast<SciBuffer*>(id);}

    void beNotifiedOfBufferChange(SciBuffer * theBuf, int mask);

    void closeBuffer(BufferID, ScintillaEditView * identifer);      //called by Notepad++

    void addBufferReference(BufferID id, ScintillaEditView * identifer);    //called by Scintilla etc indirectly

    BufferID loadFile(const char * filename, Document doc = nullptr, int encoding = -1, const char *backupFileName = nullptr, FILETIME fileNameTimestamp = {});   //ID == BUFFER_INVALID on failure. If Doc == nullptr, a new file is created, otherwise data is loaded in given document
    BufferID newEmptyDocument();
    //create SciBuffer from existing Scintilla, used from new Scintillas. If dontIncrease = true, then the new document number isnt increased afterwards.
    //usefull for temporary but neccesary docs
    //If dontRef = false, then no extra reference is added for the doc. Its the responsibility of the caller to do so
    BufferID bufferFromDocument(Document doc,  bool dontIncrease = false, bool dontRef = false);

    BufferID getBufferFromName(const char * name);
    BufferID getBufferFromDocument(Document doc);

    void setLoadedBufferEncodingAndEol(SciBuffer* buf, const Utf8_16_Read& UnicodeConvertor, int encoding, EolType bkformat);
    bool reloadBuffer(BufferID id);
    bool reloadBufferDeferred(BufferID id);
    SavingStatus saveBuffer(BufferID id, const char* filename, bool isCopy = false);
    bool backupCurrentBuffer();
    bool deleteBufferBackup(BufferID id);
    bool deleteFile(BufferID id);
    bool moveFile(BufferID id, const char * newFilename);
    bool createEmptyFile(const char * path);
    static FileManager& getInstance() {
        static FileManager instance;
        return instance;
    };
    int getFileNameFromBuffer(BufferID id, char * fn2copy);
    size_t docLength(SciBuffer * buffer) const;
    size_t nextUntitledNewNumber() const;

private:
    struct LoadedFileFormat {
        LoadedFileFormat() = default;
        LangType _language = L_TEXT;
        int _encoding = 0;
        EolType _eolFormat = EolType::osdefault;
    };

    FileManager() = default;
    ~FileManager();

    // No copy ctor and assignment
    FileManager(const FileManager&) = delete;
    FileManager& operator=(const FileManager&) = delete;

    // No move ctor and assignment
    FileManager(FileManager&&) = delete;
    FileManager& operator=(FileManager&&) = delete;

    int detectCodepage(char* buf, size_t len);
    bool loadFileData(Document doc, int64_t fileSize, const char* filename, char* buffer, Utf8_16_Read* UnicodeConvertor, LoadedFileFormat& fileFormat);
    LangType detectLanguageFromTextBegining(const unsigned char *data, size_t dataLen);


private:
    Notepad_plus* _pNotepadPlus = nullptr;
    ScintillaEditView* _pscratchTilla = nullptr;
    Document _scratchDocDefault = 0;
    std::vector<SciBuffer*> _buffers;
    BufferID _nextBufferID = 0;
    size_t _nbBufs = 0;
};

#define MainFileManager FileManager::getInstance()

class SciBuffer final {
    friend class FileManager;
public:
    //Loading a document:
    //constructor with ID.
    //Set a reference (pointer to a container mostly, like DocTabView or ScintillaEditView)
    //Set the position manually if needed
    //Load the document into Scintilla/add to TabBar
    //The entire lifetime if the buffer, the Document has reference count of _atleast_ one
    //Destructor makes sure its purged
    SciBuffer(FileManager * pManager, BufferID id, Document doc, DocFileStatus type, const char *fileName, bool isLargeFile);

    // this method 1. copies the file name
    //             2. determinates the language from the ext of file name
    //             3. gets the last modified time
    void setFileName(const char *fn, LangType defaultLang = L_TEXT);

    const char * getFullPathName() const { return _fullPathName.Begin(); }

    const char * getFileName() const { return _fileName; }

    BufferID getID() const { return _id; }

    void increaseRecentTag() {
        _recentTag = ++_recentTagCtr;
        doNotify(BufferChangeRecentTag);
    }

    long getRecentTag() const { return _recentTag; }

    bool checkFileState();

    bool isDirty() const { return _isDirty; }

    bool isReadOnly() const { return (_isUserReadOnly || _isFileReadOnly); };

    bool isUntitled() const { return (_currentStatus == DOC_UNNAMED); }

    bool getFileReadOnly() const { return _isFileReadOnly; }

    void setFileReadOnly(bool ro) {
        _isFileReadOnly = ro;
        doNotify(BufferChangeReadonly);
    }

    bool getUserReadOnly() const { return _isUserReadOnly; }

    void setUserReadOnly(bool ro) {
        _isUserReadOnly = ro;
        doNotify(BufferChangeReadonly);
    }

    EolType getEolFormat() const { return _eolFormat; }

    void setEolFormat(EolType format) {
        _eolFormat = format;
        doNotify(BufferChangeFormat);
    }

    LangType getLangType() const { return _lang; }

    void setLangType(LangType lang, const char * userLangName = TEXT(""));

    UniMode getUnicodeMode() const { return _unicodeMode; }

    void setUnicodeMode(UniMode mode);

    int getEncoding() const { return _encoding; }

    void setEncoding(int encoding);

    DocFileStatus getStatus() const { return _currentStatus; }

    Document getDocument() { return _doc; }

    void setDirty(bool dirty);

    void setPosition(const Position & pos, ScintillaEditView * identifier);
    Position & getPosition(ScintillaEditView * identifier);

    void setHeaderLineState(const std::vector<size_t> & folds, ScintillaEditView * identifier);
    const std::vector<size_t> & getHeaderLineState(const ScintillaEditView * identifier) const;

    bool isUserDefineLangExt() const { return (_userLangExt[0] != '\0'); }

    const char * getUserDefineLangName() const  { return _userLangExt.Begin(); }

    const char * getCommentLineSymbol() const {
        Lang *l = getCurrentLang();
        if (!l)
            return nullptr;
        return l->_pCommentLineSymbol;
    }

    const char * getCommentStart() const {
        Lang *l = getCurrentLang();
        if (!l)
            return nullptr;
        return l->_pCommentStart;
    }

    const char * getCommentEnd() const {
        Lang *l = getCurrentLang();
        if (!l)
            return nullptr;
        return l->_pCommentEnd;
    }

    bool getNeedsLexing() const { return _needLexer; }

    void setNeedsLexing(bool lex) {
        _needLexer = lex;
        doNotify(BufferChangeLexing);
    }

    //these two return reference count after operation
    int addReference(ScintillaEditView * identifier);       //if ID not registered, creates a new Position for that ID and new foldstate
    int removeReference(ScintillaEditView * identifier);        //reduces reference. If zero, Document is purged

    void setHideLineChanged(bool isHide, size_t location);

    void setDeferredReload();

    bool getNeedReload() const { return _needReloading; }
    void setNeedReload(bool reload) { _needReloading = reload; }

    size_t docLength() const {
        assert(_pManager != nullptr);
        return _pManager->docLength(_id);
    }

    int64_t getFileLength() const; // return file length. -1 if file is not existing.

    enum fileTimeType { ft_created, ft_modified, ft_accessed };
    String getFileTime(fileTimeType ftt) const;

    Lang * getCurrentLang() const;

    bool isModified() const { return _isModified; }
    void setModifiedStatus(bool isModified) { _isModified = isModified; }

    String getBackupFileName() const { return _backupFileName; }
    void setBackupFileName(const char* fileName) { _backupFileName = fileName; }

    FILETIME getLastModifiedTimestamp() const { return _timeStamp; }

    bool isLoadedDirty() const { return _isLoadedDirty; }
    void setLoadedDirty(bool val) { _isLoadedDirty = val; }

    bool isUnsync() const { return _isUnsync; }
    void setUnsync(bool val) { _isUnsync = val; }

    bool isSavePointDirty() const { return _isSavePointDirty; }
    void setSavePointDirty(bool val) { _isSavePointDirty = val; }

    bool isLargeFile() const { return _isLargeFile; }

    void startMonitoring() {
        _isMonitoringOn = true;
        _eventHandle = ::CreateEvent(nullptr, TRUE, FALSE, nullptr);
    };

    void* getMonitoringEvent() const { return _eventHandle; };

    void stopMonitoring() {
        _isMonitoringOn = false;
        ::SetEvent(_eventHandle);
        ::CloseHandle(_eventHandle);
    };

    bool isMonitoringOn() const { return _isMonitoringOn; };
    void updateTimeStamp();
    void reload();
    void setMapPosition(const MapPosition & mapPosition) { _mapPosition = mapPosition; };
    MapPosition getMapPosition() const { return _mapPosition; };

    void langHasBeenSetFromMenu() { _hasLangBeenSetFromMenu = true; };

private:
    int indexOfReference(const ScintillaEditView * identifier) const;

    void setStatus(DocFileStatus status) {
        _currentStatus = status;
        doNotify(BufferChangeStatus);
    }

    void doNotify(int mask);

    SciBuffer(const SciBuffer&) = delete;
    SciBuffer& operator = (const SciBuffer&) = delete;


private:
    FileManager * _pManager = nullptr;
    bool _canNotify = false; // All the notification should be disabled at the beginning
    int _references = 0; // if no references file inaccessible, can be closed
    BufferID _id = nullptr;

    //document properties
    Document _doc;  //invariable
    LangType _lang = L_TEXT;
    String _userLangExt; // it's useful if only (_lang == L_USER)
    bool _isDirty = false;
    EolType _eolFormat = EolType::osdefault;
    UniMode _unicodeMode = uniUTF8;
    int _encoding = -1;
    bool _isUserReadOnly = false;
    bool _needLexer = false; // new buffers do not need lexing, Scintilla takes care of that
    //these properties have to be duplicated because of multiple references

    //All the vectors must have the same size at all times
    std::vector<ScintillaEditView *> _referees; // Instances of ScintillaEditView which contain this buffer
    std::vector<Position> _positions;
    std::vector<std::vector<size_t>> _foldStates;

    //Environment properties
    DocFileStatus _currentStatus = DOC_REGULAR;
    FILETIME _timeStamp = {}; // 0 if it's a new doc

    bool _isFileReadOnly = false;
    String _fullPathName;
    char * _fileName = nullptr; // points to filename part in _fullPathName
    bool _needReloading = false; // True if SciBuffer needs to be reloaded on activation

    long _recentTag = -1;
    static long _recentTagCtr;

    // For backup system
    String _backupFileName;
    bool _isModified = false;
    bool _isLoadedDirty = false; // it's the indicator for finding buffer's initial state

    bool _isUnsync = false; // SciBuffer should be always dirty (with any undo/redo operation) if the editing buffer is unsyncronized with file on disk.
                            // By "unsyncronized" it means :
                            // 1. the file is deleted outside but the buffer in Notepad++ is kept.
                            // 2. the file is modified by another app but the buffer is not reloaded in Notepad++.
                            // Note that if the buffer is untitled, there's no correspondent file on the disk so the buffer is considered as independent therefore synchronized.

    bool _isLargeFile = false; // The loading of huge files will disable automatically 1. auto-completion 2. snapshot periode backup 3. backup on save 4. word wrap

    bool _isSavePointDirty = false; // After converting document to another ecoding, the document becomes dirty, and all the undo states are emptied.
                                    // This variable member keeps this situation in memory and when the undo state back to the save_point_reached, it'll still be dirty (its original state)

    // For the monitoring
    void* _eventHandle = nullptr;
    bool _isMonitoringOn = false;

    bool _hasLangBeenSetFromMenu = false;

    MapPosition _mapPosition;

    std::mutex _reloadFromDiskRequestGuard;
};
////////////////////////////////////////////////////////////////////////
//colors.h
/////////////
const Color red                      = Color(0xFF,    0,    0);
const Color darkRed                  = Color(0x80,    0,    0);
const Color offWhite                 = Color(0xFF, 0xFB, 0xF0);
const Color darkGreen                = Color(0,    0x80,    0);
const Color liteGreen                = Color(0,    0xFF,    0);
const Color blueGreen                = Color(0,    0x80, 0x80);
const Color liteRed                  = Color(0xFF, 0xAA, 0xAA);
const Color liteBlueGreen            = Color(0xAA, 0xFF, 0xC8);

const Color liteBlue                 = Color(0xA6, 0xCA, 0xF0);
const Color veryLiteBlue             = Color(0xC4, 0xF9, 0xFD);
const Color extremeLiteBlue          = Color(0xF2, 0xF4, 0xFF);

const Color darkBlue                 = Color(0,       0, 0x80);
const Color blue                     = Color(0,       0, 0xFF);
const Color black                    = Color(0,       0,    0);
const Color white                    = Color(0xFF, 0xFF, 0xFF);
const Color darkGrey                 = Color(64,     64,   64);
const Color grey                     = Color(128,   128,  128);
const Color liteGrey                 = Color(192,   192,  192);
const Color veryLiteGrey             = Color(224,   224,  224);
const Color brown                    = Color(128,    64,    0);
//const Color greenBlue              = Color(192,   128,   64);
const Color darkYellow               = Color(0xFF, 0xC0,    0);
const Color yellow                   = Color(0xFF, 0xFF,    0);
const Color lightYellow              = Color(0xFF, 0xFF, 0xD5);
const Color cyan                     = Color(0,    0xFF, 0xFF);
const Color orange                   = Color(0xFF, 0x80, 0x00);
const Color purple                   = Color(0x80, 0x00, 0xFF);
const Color deepPurple               = Color(0x87, 0x13, 0x97);

const Color extremeLitePurple        = Color(0xF8, 0xE8, 0xFF);
const Color veryLitePurple           = Color(0xE7, 0xD8, 0xE9);
const Color liteBerge                = Color(0xFE, 0xFC, 0xF5);
const Color berge                    = Color(0xFD, 0xF8, 0xE3);
////////////////////////////////////////////////////////////////////////
///columnEditor.h
//////////////////////////////////////////////
const bool activeText = true;
const bool activeNumeric = false;

class ColumnEditorDlg : public WithColumnEditorDlgLayout<TopWindow>
{
public :
    ColumnEditorDlg() = default;
    void init(Window& hInst, Window* hPere, ScintillaEditView **ppEditView);

    virtual void create(int dialogID, bool isRTL = false, bool msgDestParent = true) {
        StaticDialog::create(dialogID, isRTL, msgDestParent);
    };

    void doDialog(bool isRTL = false) {
        if (!isCreated())
            create(IDD_COLUMNEDIT, isRTL);
        bool isTextMode = (BST_CHECKED == ::SendDlgItemMessage(_hSelf, IDC_COL_TEXT_RADIO, BM_GETCHECK, 0, 0));
        display();
        ::SetFocus(::GetDlgItem(_hSelf, isTextMode?IDC_COL_TEXT_EDIT:IDC_COL_INITNUM_EDIT));
    };

    virtual void display(bool toShow = true) const;
    void switchTo(bool toText);
    UCHAR getFormat();

protected :
    virtual intptr_t CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);

private :
    ScintillaEditView **_ppEditView = nullptr;
};
////////////////////////////////////////////////////////////
//////columnEditor_rc.h
////////////////////////
IDD_COLUMNEDIT DIALOGEX 26, 41, 223, 221
STYLE DS_SETFONT | DS_FIXEDSYS | WS_POPUP | WS_CAPTION | WS_SYSMENU
EXSTYLE WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE
        CAPTION "Column / Multi-Selection Editor"
FONT 8, TEXT("MS Shell Dlg"), 0, 0, 0x0
BEGIN
    GROUPBOX        "",IDC_COL_TEXT_GRP_STATIC,12,14,124,50
    GROUPBOX        "",IDC_COL_NUM_GRP_STATIC,12,79,204,135
    CONTROL         "&Вставляемый Текст",IDC_COL_TEXT_RADIO,"Button",BS_AUTORADIOBUTTON | WS_TABSTOP | WS_GROUP,7,5,124,9
    CONTROL         "&Number to Insert",IDC_COL_NUM_RADIO,"Button",BS_AUTORADIOBUTTON | WS_TABSTOP | WS_GROUP, 7,70,204,9
    EDITTEXT        IDC_COL_TEXT_EDIT,25,32,97,14,ES_AUTOHSCROLL
    RTEXT           "&Исходное число :",IDC_COL_INITNUM_STATIC,15,91,76,8
    EDITTEXT        IDC_COL_INITNUM_EDIT,95,89,38,12,ES_NUMBER

    RTEXT           "Увеличить на :",IDC_COL_INCRNUM_STATIC,16,112,75,8
    EDITTEXT        IDC_COL_INCREASENUM_EDIT,95,110,38,12,ES_NUMBER

    RTEXT           "&Повторить :",IDC_COL_REPEATNUM_STATIC,16,133,75,8
    EDITTEXT        IDC_COL_REPEATNUM_EDIT,95,131,38,12,ES_NUMBER

    CONTROL         "Вводные нули", IDC_COL_LEADZERO_CHECK,"Button", BS_AUTOCHECKBOX | WS_TABSTOP,140,133,70,10

    CONTROL         "&Dec",IDC_COL_DEC_RADIO,"Button",BS_AUTORADIOBUTTON | WS_TABSTOP,30,169,70,10
    CONTROL         "&Hex",IDC_COL_HEX_RADIO,"Button",BS_AUTORADIOBUTTON | WS_TABSTOP,124,169,70,10
    CONTROL         "&Oct",IDC_COL_OCT_RADIO,"Button",BS_AUTORADIOBUTTON | WS_TABSTOP,30,188,70,10
    CONTROL         "&Bin",IDC_COL_BIN_RADIO,"Button",BS_AUTORADIOBUTTON | WS_TABSTOP,124,188,70,10
    GROUPBOX        "Format",IDC_COL_FORMAT_GRP_STATIC,20,153,188,54,BS_CENTER
    DEFPUSHBUTTON   "OK",IDOK,145,13,70,14,BS_NOTIFY
    PUSHBUTTON      "Отмена",IDCANCEL,145,36,70,14,BS_NOTIFY
END
////////////////////////////////////////////////////////////////////////
///DocTabView.h
//////////////////////////
const int SAVED_IMG_INDEX = 0;
const int UNSAVED_IMG_INDEX = 1;
const int REDONLY_IMG_INDEX = 2;
const int MONITORING_IMG_INDEX = 3;

class DocTabView : public TabBarPlus
{
public :
    DocTabView():TabBarPlus(), _pView(nullptr) {};
    virtual ~DocTabView(){};

    virtual void destroy() {
        TabBarPlus::destroy();
    };

    void init(Window& hInst, Window* parent, ScintillaEditView * pView, std::vector<IconList *> pIconListVector, unsigned char indexChoice) {
        TabBarPlus::init(hInst, parent);
        _pView = pView;

        if (!pIconListVector.empty())
        {
            _pIconListVector = pIconListVector;

            if (indexChoice >= pIconListVector.size())
                _iconListIndexChoice = 0;
            else
                _iconListIndexChoice = indexChoice;
        }

        if (_iconListIndexChoice != -1)
            TabBar::setImageList(_pIconListVector[_iconListIndexChoice]->getHandle());
        return;
    };

    void changeIcons(unsigned char choice) {
        if (choice >= _pIconListVector.size())
            return;
        _iconListIndexChoice = choice;
        TabBar::setImageList(_pIconListVector[_iconListIndexChoice]->getHandle());
    };

    void addBuffer(BufferID buffer);
    void closeBuffer(BufferID buffer);
    void bufferUpdated(SciBuffer * buffer, int mask);

    bool activateBuffer(BufferID buffer);

    BufferID activeBuffer();
    BufferID findBufferByName(const char * fullfilename);   //-1 if not found, something else otherwise

    int getIndexByBuffer(BufferID id);
    BufferID getBufferByIndex(size_t index);

    void setBuffer(size_t index, BufferID id);

    static bool setHideTabBarStatus(bool hideOrNot) {
        bool temp = _hideTabBarStatus;
        _hideTabBarStatus = hideOrNot;
        return temp;
    };

    static bool getHideTabBarStatus() {
        return _hideTabBarStatus;
    };

    virtual void reSizeTo(Rect & rc);

    const ScintillaEditView* getScintillaEditView() const {
        return _pView;
    };

private :
    ScintillaEditView *_pView = nullptr;
    static bool _hideTabBarStatus;

    std::vector<IconList *> _pIconListVector;
    int _iconListIndexChoice = -1;
};
//////////////////////////////////
///FindReplaceDlg.h
//////////////////////////////////////
#define FIND_RECURSIVE 1
#define FIND_INHIDDENDIR 2

#define FINDREPLACE_MAXLENGTH 2048

enum DIALOG_TYPE {FIND_DLG, REPLACE_DLG, FINDINFILES_DLG, FINDINPROJECTS_DLG, MARK_DLG};

#define DIR_DOWN true
#define DIR_UP false

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
        return  (option->_isWholeWord ? SCFIND_WHOLEWORD : 0) |
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
        _markingsStruct._markings = nullptr;
    };

    ~Finder() {
        _scintView.destroy();
    }
    void init(Window& hInst, Window* hPere, ScintillaEditView **ppEditView) {
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

    bool isLineActualSearchResult(const String& s) const;
    String& prepareStringForClipboard(String& s) const;

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
    void init(Window& hInst, Window* hPere) {
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

    void init(Window& hInst, Window* hPere, ScintillaEditView **ppEditView) {
        Window::init(hInst, hPere);
        if (!ppEditView)
            throw std::runtime_error("FindIncrementDlg::init : ppEditView is null.");
        _ppEditView = ppEditView;
    };

    virtual void create(int dialogID, bool isRTL = false, bool msgDestParent = true);

    void initOptionsFromDlg();

    void doDialog(DIALOG_TYPE whichType, bool isRTL = false, bool toShow = true);
    bool processFindNext(const char *txt2find, const FindOption *options = nullptr, FindStatus *oFindStatus = nullptr, FindNextType findNextType = FINDNEXTTYPE_FINDNEXT);
    bool processReplace(const char *txt2find, const char *txt2replace, const FindOption *options = nullptr);

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

    const String& getFilters() const {return _env->_filters;};
    const String& getDirectory() const {return _env->_directory;};
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

    Window* getHFindResults() {
        if (_pFinder)
            return _pFinder->_scintView.getHSelf();
        return nullptr;
    }

    void updateFinderScintilla() {
        if (_pFinder && _pFinder->isCreated() && _pFinder->isVisible())
        {
            _pFinder->setFinderStyle();
        }
    };

    void execSavedCommand(int cmd, uptr_t intValue, const char* stringValue);
    void clearMarks(const FindOption& opt);
    void setStatusbarMessage(const String& msg, FindStatus staus, char const *pTooltipMsg = nullptr);
    String getScopeInfoForStatusBar(FindOption const *pFindOpt) const;
    Finder * createFinder();
    bool removeFinder(Finder *finder2remove);
    DIALOG_TYPE getCurrentStatus() {return _currentStatus;};

protected :
    void resizeDialogElements(LONG newWidth);
    virtual intptr_t CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
    static LONG_PTR originalFinderProc;
    static LONG_PTR originalComboEditProc;

    static LRESULT FAR PASCAL comboEditProc(Window* hwnd, UINT message, WPARAM wParam, LPARAM lParam);

    // Window procedure for the finder
    static LRESULT FAR PASCAL finderProc(Window* hwnd, UINT message, WPARAM wParam, LPARAM lParam);

    void combo2ExtendedMode(int comboID);

private :
    Rect _initialWindowRect = {};
    LONG _deltaWidth = 0;
    LONG _initialClientWidth = 0;

    DIALOG_TYPE _currentStatus = DIALOG_TYPE::FIND_DLG;
    Rect _findClosePos, _replaceClosePos, _findInFilesClosePos, _markClosePos;
    Rect _countInSelFramePos, _replaceInSelFramePos;
    Rect _countInSelCheckPos, _replaceInSelCheckPos;

    ScintillaEditView **_ppEditView = nullptr;
    Finder  *_pFinder = nullptr;
    String _findResTitle;

    std::vector<Finder*> _findersOfFinder{};

    Window* _shiftTrickUpTip = nullptr;
    Window* _2ButtonsTip = nullptr;
    Window* _filterTip = nullptr;

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
    Window* _statusbarTooltipWnd = nullptr;
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
    void init(Window& hInst, Window* hPere, FindReplaceDlg *pFRDlg, bool isRTL = false);
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
    void markSelectedTextInc(bool enable, FindOption *opt = nullptr);
};


class Progress
{
public:
    explicit Progress(Window& hInst);
    ~Progress();

    // Disable copy construction and operator=
    Progress(const Progress&) = delete;
    const Progress& operator=(const Progress&) = delete;

    Window* open(Window* hCallerWnd, const char* header = nullptr);
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

    static dword WINAPI threadFunc(LPVOID data);
    static LRESULT APIENTRY wndProc(Window* hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

    int thread();
    int createProgressWindow();

    Window& _hInst = nullptr;
    volatile Window* _hwnd = nullptr;
    Window* _hCallerWnd = nullptr;
    char _header[128] = {'\0'};
    void* _hThread = nullptr;
    void* _hActiveState = nullptr;
    Window* _hPText = nullptr;
    Window* _hPBar = nullptr;
    Window* _hBtn = nullptr;
};
///////////////////////////////////////////////
//////Ресурсы диалога
///////////////////////////////////////
IDD_FIND_REPLACE_DLG DIALOGEX 36, 44, 382, 200
STYLE DS_SETFONT | DS_FIXEDSYS | WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME
EXSTYLE WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE
CAPTION "Заменить"
FONT 8, "MS Shell Dlg", 0, 0, 0x0
BEGIN
    RTEXT           "&Find what :",IDFINDWHAT_STATIC,1,22,73,8
    COMBOBOX        IDFINDWHAT,76,20,170,150,CBS_DROPDOWN | CBS_AUTOHSCROLL | WS_TABSTOP
    RTEXT           "Rep&lace with :",ID_STATICTEXT_REPLACE,1,40,73,8
    COMBOBOX        IDREPLACEWITH,76,38,170,50,CBS_DROPDOWN | CBS_AUTOHSCROLL | WS_TABSTOP
    PUSHBUTTON      "",IDD_FINDREPLACE_SWAP_BUTTON,248,27,16,17
    RTEXT           "Filter&s :",IDD_FINDINFILES_FILTERS_STATIC,1,58,73,8, SS_NOTIFY
    COMBOBOX        IDD_FINDINFILES_FILTERS_COMBO,76,56,170,150,CBS_DROPDOWN | CBS_AUTOHSCROLL | WS_TABSTOP
    RTEXT           "Dir&ectory :",IDD_FINDINFILES_DIR_STATIC,7,76,41,8
    COMBOBOX        IDD_FINDINFILES_DIR_COMBO,50,74,196,150,CBS_DROPDOWN | CBS_AUTOHSCROLL | WS_TABSTOP
    PUSHBUTTON      "...",IDD_FINDINFILES_BROWSE_BUTTON,248,73,16,14
    CONTROL         "Follow current doc.",IDD_FINDINFILES_FOLDERFOLLOWSDOC_CHECK,
                    "Button",BS_AUTOCHECKBOX | WS_TABSTOP,270,73,94,15
    CONTROL         "In all su&b-folders",IDD_FINDINFILES_RECURSIVE_CHECK,
                    "Button",BS_AUTOCHECKBOX | WS_TABSTOP,270,87,94,15
    CONTROL         "In &hidden folders",IDD_FINDINFILES_INHIDDENDIR_CHECK,
                    "Button",BS_AUTOCHECKBOX | WS_TABSTOP,270,101,94,15
    CONTROL         "Project Panel 1",IDD_FINDINFILES_PROJECT1_CHECK,
                    "Button",BS_AUTOCHECKBOX | WS_TABSTOP,270,73,94,15
    CONTROL         "Project Panel 2",IDD_FINDINFILES_PROJECT2_CHECK,
                    "Button",BS_AUTOCHECKBOX | WS_TABSTOP,270,87,94,15
    CONTROL         "Project Panel 3",IDD_FINDINFILES_PROJECT3_CHECK,
                    "Button",BS_AUTOCHECKBOX | WS_TABSTOP,270,101,94,15
    CONTROL         "Book&mark line",IDC_MARKLINE_CHECK,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,12,52,140,15
    CONTROL         "Purge for each search",IDC_PURGE_CHECK,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,12,64,140,15
    PUSHBUTTON      "Mark All",IDCMARKALL,268,20,91,14
    GROUPBOX        "",IDC_REPLACEINSELECTION,192,50,170,23
    CONTROL         "In select&ion",IDC_IN_SELECTION_CHECK,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,200,58,59,10
    PUSHBUTTON      "Clear all marks",IDC_CLEAR_ALL,268,38,91,14
    CONTROL         "Backward direction", IDC_BACKWARDDIRECTION, "Button", BS_AUTOCHECKBOX | WS_TABSTOP, 12, 76, 140, 15
    CONTROL         "Match &whole word only",IDWHOLEWORD,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,12,88,140,15
    CONTROL         "Match &case",IDMATCHCASE,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,12,100,140,15
    CONTROL         "Wra&p around",IDWRAP,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,12,112,140,15
    GROUPBOX        "Search Mode",IDC_MODE_STATIC,6,131,159,48
    CONTROL         "&Normal",IDNORMAL,"Button",BS_AUTORADIOBUTTON | WS_GROUP,12,143,126,10
    CONTROL         "E&xtended (\\n, \\r, \\t, \\0, \\x...)",IDEXTENDED,
                    "Button",BS_AUTORADIOBUTTON,12,155,145,10
    CONTROL         "Re&gular expression",IDREGEXP,"Button",BS_AUTORADIOBUTTON,12,167,78,10
    CONTROL         "&. matches newline",IDREDOTMATCHNL,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,93,167,68,9
    PUSHBUTTON      "", IDC_FINDPREV, 268, 20, 18, 14, WS_GROUP | BS_MULTILINE
    PUSHBUTTON      "", IDC_FINDNEXT, 289, 20, 70, 14, WS_GROUP | BS_MULTILINE
    PUSHBUTTON      "Find Next",IDOK,268,20,91,14,WS_GROUP
    CONTROL         "", IDC_2_BUTTONS_MODE, "Button", BS_AUTOCHECKBOX | WS_TABSTOP, 363, 20, 15, 15
    PUSHBUTTON      "Coun&t",IDCCOUNTALL,268,38,91,14
    PUSHBUTTON      "Find All in Current &Document",IDC_FINDALL_CURRENTFILE,268,56,91,21,BS_MULTILINE
    PUSHBUTTON      "Find All in All &Opened Documents",IDC_FINDALL_OPENEDFILES,268,80,91,21,BS_MULTILINE
    PUSHBUTTON      "&Replace",IDREPLACE,268,38,91,14
    PUSHBUTTON      "Replace &All",IDREPLACEALL,268,56,91,14
    PUSHBUTTON      "Replace All in All Opened Doc&uments",IDC_REPLACE_OPENEDFILES,268,74,91,21,BS_MULTILINE
    PUSHBUTTON      "Find All",IDD_FINDINFILES_FIND_BUTTON,268,20,91,14,WS_GROUP
    PUSHBUTTON      "Replace in Files",IDD_FINDINFILES_REPLACEINFILES,268,38,91,14
    PUSHBUTTON      "Replace in Projects",IDD_FINDINFILES_REPLACEINPROJECTS,268,38,91,14
    PUSHBUTTON      "Copy Marked Text",IDC_COPY_MARKED_TEXT,268,56,91,14
    PUSHBUTTON      "Close",IDCANCEL,268,98,91,14
    GROUPBOX        "",IDC_TRANSPARENT_GRPBOX,258,131,99,48
    CONTROL         "Transparenc&y",IDC_TRANSPARENT_CHECK,"Button",BS_AUTOCHECKBOX | NOT WS_VISIBLE | WS_TABSTOP,254,131,80,10
    CONTROL         "On losing focus",IDC_TRANSPARENT_LOSSFOCUS_RADIO,"Button",BS_AUTORADIOBUTTON | WS_GROUP | WS_TABSTOP,268,143,85,10
    CONTROL         "Always",IDC_TRANSPARENT_ALWAYS_RADIO,"Button",BS_AUTORADIOBUTTON ,268,155,85,10
    CONTROL         "",IDC_PERCENTAGE_SLIDER,"msctls_trackbar32",TBS_BOTH | TBS_NOTICKS | TBS_TRANSPARENTBKGND | NOT WS_VISIBLE | WS_TABSTOP,265,166,85,10
END

IDB_INCREMENTAL_BG          BITMAP                  "../icons/incrementalBg.bmp"

IDD_INCREMENT_FIND DIALOGEX 0, 0, 680, 20
STYLE DS_SYSMODAL | DS_CONTROL | DS_FIXEDSYS | WS_CHILD | WS_CLIPCHILDREN
FONT 8, TEXT("MS Shell Dlg")
BEGIN
    PUSHBUTTON   "X",IDCANCEL,2,3,16,14
    RTEXT           "Find:",IDC_INCSTATIC,18,6,46,12
    EDITTEXT        IDC_INCFINDTEXT,65,6,175,10,ES_AUTOHSCROLL | ES_WANTRETURN | NOT WS_BORDER | WS_TABSTOP
    PUSHBUTTON   "<",IDC_INCFINDPREVOK,243,3,16,14, WS_TABSTOP
    PUSHBUTTON   ">",IDC_INCFINDNXTOK,263,3,16,14, WS_TABSTOP
    CONTROL         "Match &case", IDC_INCFINDMATCHCASE,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,290,5,100,12
    CONTROL         "&Highlight all", IDC_INCFINDHILITEALL,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,400,5,100,12
    LTEXT           "",IDC_INCFINDSTATUS,520,6,180,12
END

IDD_FINDRESULT DIALOGEX 26, 41, 223, 67
STYLE DS_SETFONT | DS_FIXEDSYS | WS_POPUP | WS_CAPTION | WS_SYSMENU
EXSTYLE WS_EX_TOOLWINDOW | WS_EX_WINDOWEDGE
CAPTION "Search results"
FONT 8, TEXT("MS Shell Dlg"), 0, 0, 0x0
BEGIN
DEFPUSHBUTTON   ">",IDC_INCFINDNXTOK,243,0,16,14, NOT WS_VISIBLE
END

IDD_FINDINFINDER_DLG DIALOGEX 36, 44, 367, 200
STYLE DS_SETFONT | DS_MODALFRAME | DS_FIXEDSYS | WS_POPUP | WS_CAPTION | WS_SYSMENU
EXSTYLE WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE
CAPTION "Find in search results"
FONT 8, "MS Shell Dlg", 0, 0, 0x0
BEGIN
    RTEXT           "&Find what :",IDFINDWHAT_STATIC_FIFOLDER,6,22,75,8
    COMBOBOX        IDFINDWHAT_FIFOLDER,83,20,178,150,CBS_DROPDOWN | CBS_AUTOHSCROLL | WS_TABSTOP
    CONTROL         "Search only in found lines",IDC_MATCHLINENUM_CHECK_FIFOLDER,"Button",BS_AUTOCHECKBOX | BS_MULTILINE | WS_TABSTOP,12,66,99,16
    CONTROL         "Match &whole word only",IDWHOLEWORD_FIFOLDER,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,12,88,140,15
    CONTROL         "Match &case",IDMATCHCASE_FIFOLDER,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,12,100,140,15

    GROUPBOX        "Search Mode",IDC_MODE_STATIC_FIFOLDER,6,131,159,48
    CONTROL         "&Normal",IDNORMAL_FIFOLDER,"Button",BS_AUTORADIOBUTTON | WS_GROUP,12,143,126,10
    CONTROL         "E&xtended (\\n, \\r, \\t, \\0, \\x...)",IDEXTENDED_FIFOLDER, "Button",BS_AUTORADIOBUTTON,12,155,145,10
    CONTROL         "Re&gular expression",IDREGEXP_FIFOLDER,"Button",BS_AUTORADIOBUTTON,12,167,78,10
    CONTROL         "&. matches newline",IDREDOTMATCHNL_FIFOLDER,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,93,167,68,9
    PUSHBUTTON      "Find All",IDOK,268,20,90,14,WS_GROUP
    PUSHBUTTON      "Close",IDCANCEL,268,38,90,14
END
///////////////////////////////////////////////////
////FunctionCallTip.h
/////////////////////////////////////////
typedef std::vector<const char *> stringVec;

class FunctionCallTip {
	 friend class AutoCompletion;
public:
	explicit FunctionCallTip(ScintillaEditView * pEditView) : _pEditView(pEditView) {};
	~FunctionCallTip() {/* cleanup(); */};
	void setLanguageXML(TiXmlElement * pXmlKeyword);	//set calltip keyword node
	bool updateCalltip(int ch, bool needShown = false);	//Ch is character typed, or 0 if another event occured. NeedShown is true if calltip should be attempted to displayed. Return true if calltip was made visible
	void showNextOverload();							//show next overlaoded parameters
	void showPrevOverload();							//show prev overlaoded parameters
	bool isVisible() { return _pEditView?_pEditView->execute(SCI_CALLTIPACTIVE) == TRUE:false; };	//true if calltip visible
	void close();					//Close calltip if visible

private:
	ScintillaEditView * _pEditView = nullptr;	//Scintilla to display calltip in
	TiXmlElement * _pXmlKeyword = nullptr;	//current keyword node (first one)

	intptr_t _curPos = 0;					//cursor position
	intptr_t _startPos = 0;					//display start position

	TiXmlElement * _curFunction = nullptr;	//current function element
	//cache some XML values n stuff
	char * _funcName = nullptr;				//name of function
	stringVec _retVals;				//vector of overload return values/types
	std::vector<stringVec> _overloads;	//vector of overload params (=vector)
	stringVec _descriptions;		//vecotr of function descriptions
	size_t _currentNbOverloads = 0;		//current amount of overloads
	size_t _currentOverload = 0;			//current chosen overload
	size_t _currentParam = 0;				//current highlighted param

	char _start = '(';
	char _stop = ')';
	char _param = ',';
	char _terminal = ';';
    String _additionalWordChar = TEXT("");
	bool _ignoreCase = true;
	bool _selfActivated = false;

	bool getCursorFunction();		//retrieve data about function at cursor. Returns true if a function was found. Calls loaddata if needed
	bool loadFunction();			//returns true if the function can be found
	void showCalltip();				//display calltip based on current variables
	void reset();					//reset all vars in case function is invalidated
	void cleanup();					//delete any leftovers
    bool isBasicWordChar(char ch) const {
        return ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') || ch == '_');
    };
    bool isAdditionalWordChar(char ch) const {
        const char *addChars = _additionalWordChar.Begin();
        size_t len = _additionalWordChar.GetLength();
        for (size_t i = 0 ; i < len ; ++i)
            if (ch == addChars[i])
                return true;
        return false;
    };
};
//////////////////////////////
///GoToLineDlg.h
/////////////////////////////
class GoToLineDlg : public StaticDialog
{
public :
	GoToLineDlg() = default;

	void init(Window& hInst, Window* hPere, ScintillaEditView **ppEditView) {
		Window::init(hInst, hPere);
		if (!ppEditView)
			throw std::runtime_error("GoToLineDlg::init : ppEditView is null.");
		_ppEditView = ppEditView;
	};

	virtual void create(int dialogID, bool isRTL = false, bool msgDestParent = true) {
		StaticDialog::create(dialogID, isRTL, msgDestParent);
	};

	void doDialog(bool isRTL = false) {
		if (!isCreated())
			create(IDD_GOLINE, isRTL);
		display();
	};

    virtual void display(bool toShow = true) const {
        Window::display(toShow);
        if (toShow)
            ::SetFocus(::GetDlgItem(_hSelf, ID_GOLINE_EDIT));
    };

protected :
	enum mode {go2line, go2offsset};
	mode _mode = go2line;
	virtual intptr_t CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);

private :
    ScintillaEditView **_ppEditView = nullptr;

    void updateLinesNumbers() const;

    void cleanLineEdit() const {
        ::SetDlgItemText(_hSelf, ID_GOLINE_EDIT, TEXT(""));
    };

    long long getLine() const {
		const int maxLen = 256;
		char goLineEditStr[maxLen] = {'\0'};
		UINT count = ::GetDlgItemTextA(_hSelf, ID_GOLINE_EDIT, goLineEditStr, maxLen);
		if (!count)
			return -1;
		char* p_end;
		return strtoll(goLineEditStr, &p_end, 10);
    };

};
//////////////////////////////////////
////Printer.h
////////////////////////////////////
struct NPP_RangeToFormat {
	HDC hdc = nullptr;
	HDC hdcTarget = nullptr;
	Rect rc = {};
	Rect rcPage = {};
	Sci_CharacterRangeFull chrg = {};
};

class Printer
{
public :
	Printer() = default;

	void init(Window& hInst, Window* hwnd, ScintillaEditView *pSEView, bool showDialog, size_t startPos, size_t endPos, bool isRTL = false);
	size_t doPrint() {
		if (!::PrintDlg(&_pdlg))
				return 0;

		return doPrint(true);
	};
	size_t doPrint(bool justDoIt);

private :
	PRINTDLG _pdlg = {};
	ScintillaEditView *_pSEView = nullptr;
	size_t _startPos = 0;
	size_t _endPos = 0;
	size_t _nbPageTotal =0;
	bool _isRTL = false;
};
///////////////////////////
////ScintillaCtrls.h
/////////////////////
class ScintillaCtrls {
public :
	void init(Window& hInst, Window* hNpp) {
		_hInst = hInst;
		_hParent = hNpp;
	};

	Window* createSintilla(Window* hParent);
	ScintillaEditView * getScintillaEditViewFrom(Window* handle2Find);
	//bool destroyScintilla(Window* handle2Destroy);
	void destroy();
	
private:
	std::vector<ScintillaEditView *> _scintVector;
	Window& _hInst = nullptr;
	Window* _hParent = nullptr;

	int getIndexFrom(Window* handle2Find);
};
////////////////////////////////
///ScintillaRef.h
/////////////////////////////////
enum folderStyle {FOLDER_TYPE, FOLDER_STYLE_SIMPLE, FOLDER_STYLE_ARROW, FOLDER_STYLE_CIRCLE, FOLDER_STYLE_BOX, FOLDER_STYLE_NONE};
enum lineWrapMethod {LINEWRAP_DEFAULT, LINEWRAP_ALIGNED, LINEWRAP_INDENT};
enum lineHiliteMode {LINEHILITE_NONE, LINEHILITE_HILITE, LINEHILITE_FRAME};
/////////////////////////////
///SmartHighlighter.h
////////////////////
class SmartHighlighter {
public:
	explicit SmartHighlighter(FindReplaceDlg * pFRDlg);
	void highlightView(ScintillaEditView * pHighlightView, ScintillaEditView * unfocusView);
	void highlightViewWithWord(ScintillaEditView * pHighlightView, const String& word2Hilite);

private:
	FindReplaceDlg * _pFRDlg = nullptr;
};
///////////////////////////////
///UserDefineDialog.h
///////////////////////////////
#define WL_LEN_MAX 1024
#define BOLD_MASK     1
#define ITALIC_MASK   2
const bool DOCK = true;
const bool UNDOCK = false;

class GlobalMappers
{
    public:

        std::unordered_map<String, int> keywordIdMapper;
        std::unordered_map<int, String> keywordNameMapper;

        std::unordered_map<String, int> styleIdMapper;
        std::unordered_map<int, String> styleNameMapper;

        std::unordered_map<String, int> temp;
        std::unordered_map<String, int>::iterator iter;

        std::unordered_map<int, int> nestingMapper;
        std::unordered_map<int, int> dialogMapper;
        std::unordered_map<int, String> setLexerMapper;

        // only default constructor is needed
        GlobalMappers()
        {
            // pre 2.0
            temp[TEXT("Operators")]                     = SCE_USER_KWLIST_OPERATORS1;
            temp[TEXT("Folder+")]                       = SCE_USER_KWLIST_FOLDERS_IN_CODE1_OPEN;
            temp[TEXT("Folder-")]                       = SCE_USER_KWLIST_FOLDERS_IN_CODE1_CLOSE;
            temp[TEXT("Words1")]                        = SCE_USER_KWLIST_KEYWORDS1;
            temp[TEXT("Words2")]                        = SCE_USER_KWLIST_KEYWORDS2;
            temp[TEXT("Words3")]                        = SCE_USER_KWLIST_KEYWORDS3;
            temp[TEXT("Words4")]                        = SCE_USER_KWLIST_KEYWORDS4;

            // in case of duplicate entries, newer String should overwrite old one !
            for (iter = temp.begin(); iter != temp.end(); ++iter)
                keywordNameMapper[iter->second] = iter->first;
            keywordIdMapper.insert(temp.begin(), temp.end());
            temp.clear();

            // 2.0
            temp[TEXT("Comments")]                      = SCE_USER_KWLIST_COMMENTS;
            temp[TEXT("Numbers, additional")]           = SCE_USER_KWLIST_NUMBER_RANGE;
            temp[TEXT("Numbers, prefixes")]             = SCE_USER_KWLIST_NUMBER_PREFIX2;
            temp[TEXT("Numbers, extras with prefixes")] = SCE_USER_KWLIST_NUMBER_EXTRAS2;
            temp[TEXT("Numbers, suffixes")]             = SCE_USER_KWLIST_NUMBER_SUFFIX2;
            temp[TEXT("Operators1")]                    = SCE_USER_KWLIST_OPERATORS1;
            temp[TEXT("Operators2")]                    = SCE_USER_KWLIST_OPERATORS2;
            temp[TEXT("Folders in code1, open")]        = SCE_USER_KWLIST_FOLDERS_IN_CODE1_OPEN;
            temp[TEXT("Folders in code1, middle")]      = SCE_USER_KWLIST_FOLDERS_IN_CODE1_MIDDLE;
            temp[TEXT("Folders in code1, close")]       = SCE_USER_KWLIST_FOLDERS_IN_CODE1_CLOSE;
            temp[TEXT("Folders in code2, open")]        = SCE_USER_KWLIST_FOLDERS_IN_CODE2_OPEN;
            temp[TEXT("Folders in code2, middle")]      = SCE_USER_KWLIST_FOLDERS_IN_CODE2_MIDDLE;
            temp[TEXT("Folders in code2, close")]       = SCE_USER_KWLIST_FOLDERS_IN_CODE2_CLOSE;
            temp[TEXT("Folders in comment, open")]      = SCE_USER_KWLIST_FOLDERS_IN_COMMENT_OPEN;
            temp[TEXT("Folders in comment, middle")]    = SCE_USER_KWLIST_FOLDERS_IN_COMMENT_MIDDLE;
            temp[TEXT("Folders in comment, close")]     = SCE_USER_KWLIST_FOLDERS_IN_COMMENT_CLOSE;
            temp[TEXT("Keywords1")]                     = SCE_USER_KWLIST_KEYWORDS1;
            temp[TEXT("Keywords2")]                     = SCE_USER_KWLIST_KEYWORDS2;
            temp[TEXT("Keywords3")]                     = SCE_USER_KWLIST_KEYWORDS3;
            temp[TEXT("Keywords4")]                     = SCE_USER_KWLIST_KEYWORDS4;
            temp[TEXT("Keywords5")]                     = SCE_USER_KWLIST_KEYWORDS5;
            temp[TEXT("Keywords6")]                     = SCE_USER_KWLIST_KEYWORDS6;
            temp[TEXT("Keywords7")]                     = SCE_USER_KWLIST_KEYWORDS7;
            temp[TEXT("Keywords8")]                     = SCE_USER_KWLIST_KEYWORDS8;
            temp[TEXT("Delimiters")]                    = SCE_USER_KWLIST_DELIMITERS;

            // in case of duplicate entries, newer String should overwrite old one !
            for (iter = temp.begin(); iter != temp.end(); ++iter)
                keywordNameMapper[iter->second] = iter->first;
            keywordIdMapper.insert(temp.begin(), temp.end());
            temp.clear();

            // 2.1
            temp[TEXT("Numbers, prefix1")]              = SCE_USER_KWLIST_NUMBER_PREFIX1;
            temp[TEXT("Numbers, prefix2")]              = SCE_USER_KWLIST_NUMBER_PREFIX2;
            temp[TEXT("Numbers, extras1")]              = SCE_USER_KWLIST_NUMBER_EXTRAS1;
            temp[TEXT("Numbers, extras2")]              = SCE_USER_KWLIST_NUMBER_EXTRAS2;
            temp[TEXT("Numbers, suffix1")]              = SCE_USER_KWLIST_NUMBER_SUFFIX1;
            temp[TEXT("Numbers, suffix2")]              = SCE_USER_KWLIST_NUMBER_SUFFIX2;
            temp[TEXT("Numbers, range")]                = SCE_USER_KWLIST_NUMBER_RANGE;

            // in case of duplicate entries, newer String should overwrite old one !
            for (iter = temp.begin(); iter != temp.end(); ++iter)
                keywordNameMapper[iter->second] = iter->first;
            keywordIdMapper.insert(temp.begin(), temp.end());
            temp.clear();

            // pre 2.0
            temp[TEXT("FOLDEROPEN")]           = SCE_USER_STYLE_FOLDER_IN_CODE1;
            temp[TEXT("FOLDERCLOSE")]          = SCE_USER_STYLE_FOLDER_IN_CODE1;
            temp[TEXT("KEYWORD1")]             = SCE_USER_STYLE_KEYWORD1;
            temp[TEXT("KEYWORD2")]             = SCE_USER_STYLE_KEYWORD2;
            temp[TEXT("KEYWORD3")]             = SCE_USER_STYLE_KEYWORD3;
            temp[TEXT("KEYWORD4")]             = SCE_USER_STYLE_KEYWORD4;
            temp[TEXT("COMMENT")]              = SCE_USER_STYLE_COMMENT;
            temp[TEXT("COMMENT LINE")]         = SCE_USER_STYLE_COMMENTLINE;
            temp[TEXT("NUMBER")]               = SCE_USER_STYLE_NUMBER;
            temp[TEXT("OPERATOR")]             = SCE_USER_STYLE_OPERATOR;
            temp[TEXT("DELIMINER1")]           = SCE_USER_STYLE_DELIMITER1;
            temp[TEXT("DELIMINER2")]           = SCE_USER_STYLE_DELIMITER2;
            temp[TEXT("DELIMINER3")]           = SCE_USER_STYLE_DELIMITER3;

            // in case of duplicate entries, newer String should overwrite old one !
            for (iter = temp.begin(); iter != temp.end(); ++iter)
                styleNameMapper[iter->second] = iter->first;
            styleIdMapper.insert(temp.begin(), temp.end());
            temp.clear();

            // post 2.0
            temp[TEXT("DEFAULT")]              = SCE_USER_STYLE_DEFAULT;
            temp[TEXT("COMMENTS")]             = SCE_USER_STYLE_COMMENT;
            temp[TEXT("LINE COMMENTS")]        = SCE_USER_STYLE_COMMENTLINE;
            temp[TEXT("NUMBERS")]              = SCE_USER_STYLE_NUMBER;
            temp[TEXT("KEYWORDS1")]            = SCE_USER_STYLE_KEYWORD1;
            temp[TEXT("KEYWORDS2")]            = SCE_USER_STYLE_KEYWORD2;
            temp[TEXT("KEYWORDS3")]            = SCE_USER_STYLE_KEYWORD3;
            temp[TEXT("KEYWORDS4")]            = SCE_USER_STYLE_KEYWORD4;
            temp[TEXT("KEYWORDS5")]            = SCE_USER_STYLE_KEYWORD5;
            temp[TEXT("KEYWORDS6")]            = SCE_USER_STYLE_KEYWORD6;
            temp[TEXT("KEYWORDS7")]            = SCE_USER_STYLE_KEYWORD7;
            temp[TEXT("KEYWORDS8")]            = SCE_USER_STYLE_KEYWORD8;
            temp[TEXT("OPERATORS")]            = SCE_USER_STYLE_OPERATOR;
            temp[TEXT("FOLDER IN CODE1")]      = SCE_USER_STYLE_FOLDER_IN_CODE1;
            temp[TEXT("FOLDER IN CODE2")]      = SCE_USER_STYLE_FOLDER_IN_CODE2;
            temp[TEXT("FOLDER IN COMMENT")]    = SCE_USER_STYLE_FOLDER_IN_COMMENT;
            temp[TEXT("DELIMITERS1")]          = SCE_USER_STYLE_DELIMITER1;
            temp[TEXT("DELIMITERS2")]          = SCE_USER_STYLE_DELIMITER2;
            temp[TEXT("DELIMITERS3")]          = SCE_USER_STYLE_DELIMITER3;
            temp[TEXT("DELIMITERS4")]          = SCE_USER_STYLE_DELIMITER4;
            temp[TEXT("DELIMITERS5")]          = SCE_USER_STYLE_DELIMITER5;
            temp[TEXT("DELIMITERS6")]          = SCE_USER_STYLE_DELIMITER6;
            temp[TEXT("DELIMITERS7")]          = SCE_USER_STYLE_DELIMITER7;
            temp[TEXT("DELIMITERS8")]          = SCE_USER_STYLE_DELIMITER8;

            // in case of duplicate entries, newer String should overwrite old one !
            for (iter = temp.begin(); iter != temp.end(); ++iter)
                styleNameMapper[iter->second] = iter->first;
            styleIdMapper.insert(temp.begin(), temp.end());
            temp.clear();

            nestingMapper[IDC_STYLER_CHECK_NESTING_DELIMITER1]      = SCE_USER_MASK_NESTING_DELIMITER1;
            nestingMapper[IDC_STYLER_CHECK_NESTING_DELIMITER2]      = SCE_USER_MASK_NESTING_DELIMITER2;
            nestingMapper[IDC_STYLER_CHECK_NESTING_DELIMITER3]      = SCE_USER_MASK_NESTING_DELIMITER3;
            nestingMapper[IDC_STYLER_CHECK_NESTING_DELIMITER4]      = SCE_USER_MASK_NESTING_DELIMITER4;
            nestingMapper[IDC_STYLER_CHECK_NESTING_DELIMITER5]      = SCE_USER_MASK_NESTING_DELIMITER5;
            nestingMapper[IDC_STYLER_CHECK_NESTING_DELIMITER6]      = SCE_USER_MASK_NESTING_DELIMITER6;
            nestingMapper[IDC_STYLER_CHECK_NESTING_DELIMITER7]      = SCE_USER_MASK_NESTING_DELIMITER7;
            nestingMapper[IDC_STYLER_CHECK_NESTING_DELIMITER8]      = SCE_USER_MASK_NESTING_DELIMITER8;
            nestingMapper[IDC_STYLER_CHECK_NESTING_COMMENT]         = SCE_USER_MASK_NESTING_COMMENT;
            nestingMapper[IDC_STYLER_CHECK_NESTING_COMMENT_LINE]    = SCE_USER_MASK_NESTING_COMMENT_LINE;
            nestingMapper[IDC_STYLER_CHECK_NESTING_KEYWORD1]        = SCE_USER_MASK_NESTING_KEYWORD1;
            nestingMapper[IDC_STYLER_CHECK_NESTING_KEYWORD2]        = SCE_USER_MASK_NESTING_KEYWORD2;
            nestingMapper[IDC_STYLER_CHECK_NESTING_KEYWORD3]        = SCE_USER_MASK_NESTING_KEYWORD3;
            nestingMapper[IDC_STYLER_CHECK_NESTING_KEYWORD4]        = SCE_USER_MASK_NESTING_KEYWORD4;
            nestingMapper[IDC_STYLER_CHECK_NESTING_KEYWORD5]        = SCE_USER_MASK_NESTING_KEYWORD5;
            nestingMapper[IDC_STYLER_CHECK_NESTING_KEYWORD6]        = SCE_USER_MASK_NESTING_KEYWORD6;
            nestingMapper[IDC_STYLER_CHECK_NESTING_KEYWORD7]        = SCE_USER_MASK_NESTING_KEYWORD7;
            nestingMapper[IDC_STYLER_CHECK_NESTING_KEYWORD8]        = SCE_USER_MASK_NESTING_KEYWORD8;
            nestingMapper[IDC_STYLER_CHECK_NESTING_OPERATORS1]      = SCE_USER_MASK_NESTING_OPERATORS1;
            nestingMapper[IDC_STYLER_CHECK_NESTING_OPERATORS2]      = SCE_USER_MASK_NESTING_OPERATORS2;
            nestingMapper[IDC_STYLER_CHECK_NESTING_NUMBERS]         = SCE_USER_MASK_NESTING_NUMBERS;

            dialogMapper[IDC_NUMBER_PREFIX1_EDIT]           = SCE_USER_KWLIST_NUMBER_PREFIX1;
            dialogMapper[IDC_NUMBER_PREFIX2_EDIT]           = SCE_USER_KWLIST_NUMBER_PREFIX2;
            dialogMapper[IDC_NUMBER_EXTRAS1_EDIT]           = SCE_USER_KWLIST_NUMBER_EXTRAS1;
            dialogMapper[IDC_NUMBER_EXTRAS2_EDIT]           = SCE_USER_KWLIST_NUMBER_EXTRAS2;
            dialogMapper[IDC_NUMBER_SUFFIX1_EDIT]           = SCE_USER_KWLIST_NUMBER_SUFFIX1;
            dialogMapper[IDC_NUMBER_SUFFIX2_EDIT]           = SCE_USER_KWLIST_NUMBER_SUFFIX2;
            dialogMapper[IDC_NUMBER_RANGE_EDIT]             = SCE_USER_KWLIST_NUMBER_RANGE;

            dialogMapper[IDC_FOLDER_IN_CODE1_OPEN_EDIT]     = SCE_USER_KWLIST_FOLDERS_IN_CODE1_OPEN;
            dialogMapper[IDC_FOLDER_IN_CODE1_MIDDLE_EDIT]   = SCE_USER_KWLIST_FOLDERS_IN_CODE1_MIDDLE;
            dialogMapper[IDC_FOLDER_IN_CODE1_CLOSE_EDIT]    = SCE_USER_KWLIST_FOLDERS_IN_CODE1_CLOSE;
            dialogMapper[IDC_FOLDER_IN_CODE2_OPEN_EDIT]     = SCE_USER_KWLIST_FOLDERS_IN_CODE2_OPEN;
            dialogMapper[IDC_FOLDER_IN_CODE2_MIDDLE_EDIT]   = SCE_USER_KWLIST_FOLDERS_IN_CODE2_MIDDLE;
            dialogMapper[IDC_FOLDER_IN_CODE2_CLOSE_EDIT]    = SCE_USER_KWLIST_FOLDERS_IN_CODE2_CLOSE;
            dialogMapper[IDC_FOLDER_IN_COMMENT_OPEN_EDIT]   = SCE_USER_KWLIST_FOLDERS_IN_COMMENT_OPEN;
            dialogMapper[IDC_FOLDER_IN_COMMENT_MIDDLE_EDIT] = SCE_USER_KWLIST_FOLDERS_IN_COMMENT_MIDDLE;
            dialogMapper[IDC_FOLDER_IN_COMMENT_CLOSE_EDIT]  = SCE_USER_KWLIST_FOLDERS_IN_COMMENT_CLOSE;

            dialogMapper[IDC_KEYWORD1_EDIT]                 = SCE_USER_KWLIST_KEYWORDS1;
            dialogMapper[IDC_KEYWORD2_EDIT]                 = SCE_USER_KWLIST_KEYWORDS2;
            dialogMapper[IDC_KEYWORD3_EDIT]                 = SCE_USER_KWLIST_KEYWORDS3;
            dialogMapper[IDC_KEYWORD4_EDIT]                 = SCE_USER_KWLIST_KEYWORDS4;
            dialogMapper[IDC_KEYWORD5_EDIT]                 = SCE_USER_KWLIST_KEYWORDS5;
            dialogMapper[IDC_KEYWORD6_EDIT]                 = SCE_USER_KWLIST_KEYWORDS6;
            dialogMapper[IDC_KEYWORD7_EDIT]                 = SCE_USER_KWLIST_KEYWORDS7;
            dialogMapper[IDC_KEYWORD8_EDIT]                 = SCE_USER_KWLIST_KEYWORDS8;

            setLexerMapper[SCE_USER_KWLIST_COMMENTS]                = "userDefine.comments";
            setLexerMapper[SCE_USER_KWLIST_DELIMITERS]              = "userDefine.delimiters";
            setLexerMapper[SCE_USER_KWLIST_OPERATORS1]              = "userDefine.operators1";
            setLexerMapper[SCE_USER_KWLIST_NUMBER_PREFIX1]          = "userDefine.numberPrefix1";
            setLexerMapper[SCE_USER_KWLIST_NUMBER_PREFIX2]          = "userDefine.numberPrefix2";
            setLexerMapper[SCE_USER_KWLIST_NUMBER_EXTRAS1]          = "userDefine.numberExtras1";
            setLexerMapper[SCE_USER_KWLIST_NUMBER_EXTRAS2]          = "userDefine.numberExtras2";
            setLexerMapper[SCE_USER_KWLIST_NUMBER_SUFFIX1]          = "userDefine.numberSuffix1";
            setLexerMapper[SCE_USER_KWLIST_NUMBER_SUFFIX2]          = "userDefine.numberSuffix2";
            setLexerMapper[SCE_USER_KWLIST_NUMBER_RANGE]            = "userDefine.numberRange";
            setLexerMapper[SCE_USER_KWLIST_FOLDERS_IN_CODE1_OPEN]   = "userDefine.foldersInCode1Open";
            setLexerMapper[SCE_USER_KWLIST_FOLDERS_IN_CODE1_MIDDLE] = "userDefine.foldersInCode1Middle";
            setLexerMapper[SCE_USER_KWLIST_FOLDERS_IN_CODE1_CLOSE]  = "userDefine.foldersInCode1Close";
        };
};

GlobalMappers & globalMappper();

class SharedParametersDialog : public StaticDialog
{
friend class StylerDlg;
public:
    SharedParametersDialog() = default;
    virtual void updateDlg() = 0;
protected :
    //Shared data
    static UserLangContainer *_pUserLang;
    static ScintillaEditView *_pScintilla;
    intptr_t CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam);
    bool setPropertyByCheck(Window* hwnd, WPARAM id, bool & bool2set);
    virtual void setKeywords2List(int ctrlID) = 0;
};

class FolderStyleDialog : public SharedParametersDialog
{
public:
    FolderStyleDialog() = default;
    void updateDlg();
protected :
    intptr_t CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam);
    void setKeywords2List(int ctrlID);
private :
    void retrieve(char *dest, const char *toRetrieve, char *prefix) const;
    URLCtrl _pageLink;
};

class KeyWordsStyleDialog : public SharedParametersDialog
{
public:
    KeyWordsStyleDialog() = default;
    void updateDlg();
protected :
    intptr_t CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam);
    void setKeywords2List(int id);
};

class CommentStyleDialog : public SharedParametersDialog
{
public :
    CommentStyleDialog() = default;
    void updateDlg();
protected :
    intptr_t CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam);
    void setKeywords2List(int id);
private :
    void retrieve(char *dest, const char *toRetrieve, const char *prefix) const;
};

class SymbolsStyleDialog : public SharedParametersDialog
{
public :
    SymbolsStyleDialog() = default;
    void updateDlg();
protected :
    intptr_t CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam);
    void setKeywords2List(int id);
private :
    void retrieve(char *dest, const char *toRetrieve, char *prefix) const;
};

class UserDefineDialog : public SharedParametersDialog
{
friend class ScintillaEditView;
public :
    UserDefineDialog();
    ~UserDefineDialog();
    void init(Window& hInst, Window* hPere, ScintillaEditView *pSev) {
        if (!_pScintilla)
        {
            Window::init(hInst, hPere);
            _pScintilla = pSev;
        }
    };
    void setScintilla(ScintillaEditView *pScinView) {
        _pScintilla = pScinView;
    };
     virtual void create(int dialogID, bool isRTL = false, bool msgDestParent = true) {
        StaticDialog::create(dialogID, isRTL, msgDestParent);
    }
    void destroy() {
        // A Ajouter les fils...
    };
    int getWidth() const {
        return _dlgPos.right;
    };
    int getHeight() const {
        return _dlgPos.bottom;
    };
    void doDialog(bool willBeShown = true, bool isRTL = false) {
        if (!isCreated())
            create(IDD_GLOBAL_USERDEFINE_DLG, isRTL);
        display(willBeShown);
    };
    virtual void reSizeTo(Rect & rc) // should NEVER be const !!!
    {
        Window::reSizeTo(rc);
        display(false);
        display();
    };
    void reloadLangCombo();
    void changeStyle();
    bool isDocked() const {return _status == DOCK;};
    void setDockStatus(bool isDocked) {_status = isDocked;};
    Window* getFolderHandle() const {
        return _folderStyleDlg.getHSelf();
    };
    Window* getKeywordsHandle() const {
        return _keyWordsStyleDlg.getHSelf();
    };
    Window* getCommentHandle() const {
        return _commentStyleDlg.getHSelf();
    };
    Window* getSymbolHandle() const {
        return _symbolsStyleDlg.getHSelf();
    };
    void setTabName(int index, const char *name2set) {
        _ctrlTab.renameTab(index, name2set);
    };
protected :
    virtual intptr_t CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
private :
    ControlsTab _ctrlTab;
    WindowVector _wVector;
    UserLangContainer *_pCurrentUserLang = nullptr;
    FolderStyleDialog       _folderStyleDlg;
    KeyWordsStyleDialog     _keyWordsStyleDlg;
    CommentStyleDialog      _commentStyleDlg;
    SymbolsStyleDialog      _symbolsStyleDlg;
    bool _status = UNDOCK;
    Rect _dlgPos = {};
    int _currentHight = 0;
    int _yScrollPos = 0;
    int _prevHightVal = 0;
    void getActualPosSize() {
        ::GetWindowRect(_hSelf, &_dlgPos);
        _dlgPos.right -= _dlgPos.left;
        _dlgPos.bottom -= _dlgPos.top;
    };
    void restorePosSize(){reSizeTo(_dlgPos);};
    void enableLangAndControlsBy(size_t index);
protected :
    void setKeywords2List(int){};
    void updateDlg();
};

class StringDlg : public StaticDialog
{
public :
    StringDlg() = default;
    void init(Window& hInst, Window* parent, const char *title, const char *staticName, const char *text2Set, int txtLen = 0, const char* restrictedChars = nullptr, bool bGotoCenter = false) {
        Window::init(hInst, parent);
        _title = title;
        _static = staticName;
        _textValue = text2Set;
        _txtLen = txtLen;
        _shouldGotoCenter = bGotoCenter;
        if (restrictedChars && strlen__(restrictedChars))
        {
            _restrictedChars = restrictedChars;
        }
    };

    intptr_t doDialog() {
        return ::DialogBoxParam(_hInst, MAKEINTRESOURCE(IDD_STRING_DLG), _hParent,  dlgProc, reinterpret_cast<LPARAM>(this));
    };

    virtual void destroy() {};

protected :
    intptr_t CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM);

    // Custom proc to subclass edit control
    LRESULT static CALLBACK customEditProc(Window* hEdit, UINT msg, WPARAM wParam, LPARAM lParam);

    bool isAllowed(const char* txt);
    void HandlePaste(Window* hEdit);

private :
    String _title;
    String _textValue;
    String _static;
    String _restrictedChars;
    int _txtLen = 0;
    bool _shouldGotoCenter = false;
    WNDPROC _oldEditProc = nullptr;
};

class StylerDlg
{
public:
    StylerDlg( Window& hInst, Window* parent, int stylerIndex = 0, int enabledNesters = -1):
        _hInst(hInst), _parent(parent), _stylerIndex(stylerIndex), _enabledNesters(enabledNesters) {
        _pFgColour = new ColourPicker;
        _pBgColour = new ColourPicker;
        _initialStyle = SharedParametersDialog::_pUserLang->_styles.getStyler(stylerIndex);
    };

    ~StylerDlg() {
        _pFgColour->destroy();
        _pBgColour->destroy();
        delete _pFgColour;
        delete _pBgColour;
    };

    long doDialog() {
        return long(::DialogBoxParam(_hInst, MAKEINTRESOURCE(IDD_STYLER_POPUP_DLG), _parent, dlgProc, reinterpret_cast<LPARAM>(this)));
    };

    static intptr_t CALLBACK dlgProc(Window* hwnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    Window& _hInst = nullptr;
    Window* _parent = nullptr;
    int _stylerIndex = 0;
    int _enabledNesters = 0;
    ColourPicker * _pFgColour = nullptr;
    ColourPicker * _pBgColour = nullptr;
    Style _initialStyle;

    void move2CtrlRight(Window* hwndDlg, int ctrlID, Window* handle2Move, int handle2MoveWidth, int handle2MoveHeight);
};
////////////////////////////////////////
////Ресурс диалога
/////////////////////////////////////
IDD_FOLDER_STYLE_DLG DIALOGEX 36, 44, 466, 415
STYLE DS_SETFONT | DS_3DLOOK | DS_FIXEDSYS | WS_CHILD
FONT 8, "MS Shell Dlg", 0, 0, 0x0
BEGIN
    LTEXT           "https://ivan-radic.github.io/udl-documentation/",IDC_WEB_HELP_LINK,40,45,160,13
    LTEXT           "Temporary doc site:",IDC_WEB_HELP_STATIC,40,31,160,13
    GROUPBOX        "Documentation:",IDC_WEB_HELP_DESCGROUP_STATIC,12,14,210,49
    PUSHBUTTON      "Styler",IDC_DEFAULT_STYLER,87,94,54,13
    GROUPBOX        "Default style:",IDC_DEFAULT_DESCGROUP_STATIC,12,71,210,49
    CONTROL         "Fold &compact (fold empty lines too)",IDC_FOLDER_FOLD_COMPACT,
                    "Button",BS_AUTOCHECKBOX | WS_TABSTOP,12,143,152,11
    EDITTEXT        IDC_FOLDER_IN_COMMENT_OPEN_EDIT,252,62,190,22,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_FOLDER_IN_COMMENT_MIDDLE_EDIT,252,99,190,22,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_FOLDER_IN_COMMENT_CLOSE_EDIT,252,137,190,22,ES_MULTILINE | WS_VSCROLL
    PUSHBUTTON      "Styler",IDC_FOLDER_IN_COMMENT_STYLER,252,30,54,13
    LTEXT           "Open:",IDC_FOLDER_IN_COMMENT_OPEN_STATIC,252,53,113,8
    LTEXT           "Middle:",IDC_FOLDER_IN_COMMENT_MIDDLE_STATIC,252,89,113,9
    LTEXT           "Close:",IDC_FOLDER_IN_COMMENT_CLOSE_STATIC,252,126,113,9
    GROUPBOX        "Folding in comment style:",IDC_FOLDER_IN_COMMENT_DESCGROUP_STATIC,241,14,210,156,BS_CENTER
    EDITTEXT        IDC_FOLDER_IN_CODE1_OPEN_EDIT,22,236,190,22,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_FOLDER_IN_CODE1_MIDDLE_EDIT,22,273,190,22,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_FOLDER_IN_CODE1_CLOSE_EDIT,22,311,190,22,ES_MULTILINE | WS_VSCROLL
    PUSHBUTTON      "Styler",IDC_FOLDER_IN_CODE1_STYLER,22,203,54,13
    LTEXT           "Open:",IDC_FOLDER_IN_CODE1_OPEN_STATIC,23,224,113,9
    LTEXT           "Middle:",IDC_FOLDER_IN_CODE1_MIDDLE_STATIC,23,262,113,9
    LTEXT           "Close:",IDC_FOLDER_IN_CODE1_CLOSE_STATIC,23,299,113,9
    GROUPBOX        "Folding in code 1 style:",IDC_FOLDER_IN_CODE1_DESCGROUP_STATIC,12,186,210,156,BS_CENTER
    EDITTEXT        IDC_FOLDER_IN_CODE2_OPEN_EDIT,253,236,190,22,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_FOLDER_IN_CODE2_MIDDLE_EDIT,253,273,190,22,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_FOLDER_IN_CODE2_CLOSE_EDIT,253,311,190,22,ES_MULTILINE | WS_VSCROLL
    PUSHBUTTON      "Styler",IDC_FOLDER_IN_CODE2_STYLER,253,202,54,13
    LTEXT           "Open:",IDC_FOLDER_IN_CODE2_OPEN_STATIC,253,224,113,9
    LTEXT           "Middle:",IDC_FOLDER_IN_CODE2_MIDDLE_STATIC,253,261,113,9
    LTEXT           "Close:",IDC_FOLDER_IN_CODE2_CLOSE_STATIC,253,298,113,9
    GROUPBOX        "Folding in code 2 style (separators needed):",IDC_FOLDER_IN_CODE2_DESCGROUP_STATIC,242,186,210,156,BS_CENTER
END

IDD_SYMBOL_STYLE_DLG DIALOGEX 36, 44, 466, 415
STYLE DS_SETFONT | DS_3DLOOK | DS_FIXEDSYS | WS_CHILD
FONT 8, "MS Shell Dlg", 0, 0, 0x0
BEGIN
    EDITTEXT        IDC_OPERATOR1_EDIT,15,31,205,19,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_OPERATOR2_EDIT,247,31,206,19,ES_MULTILINE | WS_VSCROLL
    PUSHBUTTON      "Styler",IDC_OPERATOR_STYLER,14,12,54,13
    GROUPBOX        "Operators style",IDC_OPERATOR_DESCGROUP_STATIC,8,3,451,55,BS_CENTER
    RTEXT           "Operators 1",IDC_OPERATOR1_STATIC,142,20,75,10,0,WS_EX_RIGHT
    RTEXT           "Operators 2 (separators required)",IDC_OPERATOR2_STATIC,328,20,124,10,0,WS_EX_RIGHT
    EDITTEXT        IDC_DELIMITER1_BOUNDARYOPEN_EDIT,72,88,144,12,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_DELIMITER1_ESCAPE_EDIT,72,108,144,12,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_DELIMITER1_BOUNDARYCLOSE_EDIT,72,126,144,12,ES_MULTILINE | WS_VSCROLL
    PUSHBUTTON      "Styler",IDC_DELIMITER1_STYLER,162,71,54,13
    RTEXT           "Open: ",IDC_DELIMITER1_BOUNDARYOPEN_STATIC,12,90,56,8
    RTEXT           "Escape: ",IDC_DELIMITER1_ESCAPE_STATIC,12,109,56,8
    RTEXT           "Close: ",IDC_DELIMITER1_BOUNDARYCLOSE_STATIC,12,127,56,8
    GROUPBOX        "Delimiter 1 style",IDC_DELIMITER1_DESCGROUP_STATIC,8,63,212,82,BS_LEFT
    EDITTEXT        IDC_DELIMITER2_BOUNDARYOPEN_EDIT,309,88,144,12,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_DELIMITER2_ESCAPE_EDIT,309,107,144,12,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_DELIMITER2_BOUNDARYCLOSE_EDIT,309,125,144,12,ES_MULTILINE | WS_VSCROLL
    PUSHBUTTON      "Styler",IDC_DELIMITER2_STYLER,399,71,54,13
    RTEXT           "Open: ",IDC_DELIMITER2_BOUNDARYOPEN_STATIC,251,90,56,8
    RTEXT           "Escape: ",IDC_DELIMITER2_ESCAPE_STATIC,251,109,56,8
    RTEXT           "Close: ",IDC_DELIMITER2_BOUNDARYCLOSE_STATIC,251,127,56,8
    GROUPBOX        "Delimiter 2 style",IDC_DELIMITER2_DESCGROUP_STATIC,245,63,212,82,BS_LEFT
    EDITTEXT        IDC_DELIMITER3_BOUNDARYOPEN_EDIT,72,174,144,12,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_DELIMITER3_ESCAPE_EDIT,72,193,144,12,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_DELIMITER3_BOUNDARYCLOSE_EDIT,72,212,144,12,ES_MULTILINE | WS_VSCROLL
    PUSHBUTTON      "Styler",IDC_DELIMITER3_STYLER,162,156,54,13
    RTEXT           "Open: ",IDC_DELIMITER3_BOUNDARYOPEN_STATIC,12,176,56,8
    RTEXT           "Escape: ",IDC_DELIMITER3_ESCAPE_STATIC,12,195,56,8
    RTEXT           "Close: ",IDC_DELIMITER3_BOUNDARYCLOSE_STATIC,12,214,56,8
    GROUPBOX        "Delimiter 3 style",IDC_DELIMITER3_DESCGROUP_STATIC,8,148,212,82,BS_LEFT
    EDITTEXT        IDC_DELIMITER4_BOUNDARYOPEN_EDIT,309,174,144,12,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_DELIMITER4_ESCAPE_EDIT,309,193,144,12,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_DELIMITER4_BOUNDARYCLOSE_EDIT,309,212,144,12,ES_MULTILINE | WS_VSCROLL
    PUSHBUTTON      "Styler",IDC_DELIMITER4_STYLER,399,156,54,13
    RTEXT           "Open: ",IDC_DELIMITER4_BOUNDARYOPEN_STATIC,251,176,56,8
    RTEXT           "Escape: ",IDC_DELIMITER4_ESCAPE_STATIC,251,195,56,8
    RTEXT           "Close: ",IDC_DELIMITER4_BOUNDARYCLOSE_STATIC,251,214,56,8
    GROUPBOX        "Delimiter 4 style",IDC_DELIMITER4_DESCGROUP_STATIC,245,148,212,82,BS_LEFT
    EDITTEXT        IDC_DELIMITER5_BOUNDARYOPEN_EDIT,72,259,144,12,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_DELIMITER5_ESCAPE_EDIT,72,278,144,12,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_DELIMITER5_BOUNDARYCLOSE_EDIT,72,298,144,12,ES_MULTILINE | WS_VSCROLL
    PUSHBUTTON      "Styler",IDC_DELIMITER5_STYLER,162,242,54,13
    RTEXT           "Open: ",IDC_DELIMITER5_BOUNDARYOPEN_STATIC,12,260,56,8
    RTEXT           "Escape: ",IDC_DELIMITER5_ESCAPE_STATIC,12,279,56,8
    RTEXT           "Close: ",IDC_DELIMITER5_BOUNDARYCLOSE_STATIC,12,299,56,8
    GROUPBOX        "Delimiter 5 style",IDC_DELIMITER5_DESCGROUP_STATIC,8,234,212,82,BS_LEFT
    EDITTEXT        IDC_DELIMITER6_BOUNDARYOPEN_EDIT,309,259,144,12,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_DELIMITER6_ESCAPE_EDIT,309,278,144,12,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_DELIMITER6_BOUNDARYCLOSE_EDIT,309,297,144,12,ES_MULTILINE | WS_VSCROLL
    PUSHBUTTON      "Styler",IDC_DELIMITER6_STYLER,399,242,54,13
    RTEXT           "Open: ",IDC_DELIMITER6_BOUNDARYOPEN_STATIC,251,261,56,8
    RTEXT           "Escape: ",IDC_DELIMITER6_ESCAPE_STATIC,251,280,56,8
    RTEXT           "Close: ",IDC_DELIMITER6_BOUNDARYCLOSE_STATIC,251,299,56,8
    GROUPBOX        "Delimiter 6 style",IDC_DELIMITER6_DESCGROUP_STATIC,245,234,212,82,BS_LEFT
    EDITTEXT        IDC_DELIMITER7_BOUNDARYOPEN_EDIT,72,345,144,12,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_DELIMITER7_ESCAPE_EDIT,72,364,144,12,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_DELIMITER7_BOUNDARYCLOSE_EDIT,72,383,144,12,ES_MULTILINE | WS_VSCROLL
    PUSHBUTTON      "Styler",IDC_DELIMITER7_STYLER,162,328,54,13
    RTEXT           "Open: ",IDC_DELIMITER7_BOUNDARYOPEN_STATIC,251,347,56,8
    RTEXT           "Escape: ",IDC_DELIMITER7_ESCAPE_STATIC,251,365,56,8
    RTEXT           "Close: ",IDC_DELIMITER7_BOUNDARYCLOSE_STATIC,251,384,56,8
    GROUPBOX        "Delimiter 7 style",IDC_DELIMITER7_DESCGROUP_STATIC,8,320,212,82,BS_LEFT
    EDITTEXT        IDC_DELIMITER8_BOUNDARYOPEN_EDIT,309,345,144,12,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_DELIMITER8_ESCAPE_EDIT,309,364,144,12,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_DELIMITER8_BOUNDARYCLOSE_EDIT,309,383,144,12,ES_MULTILINE | WS_VSCROLL
    PUSHBUTTON      "Styler",IDC_DELIMITER8_STYLER,399,328,54,13
    RTEXT           "Open: ",IDC_DELIMITER8_BOUNDARYOPEN_STATIC,12,346,56,8
    RTEXT           "Escape: ",IDC_DELIMITER8_ESCAPE_STATIC,12,365,56,8
    RTEXT           "Close: ",IDC_DELIMITER8_BOUNDARYCLOSE_STATIC,12,385,56,8
    GROUPBOX        "Delimiter 8 style",IDC_DELIMITER8_DESCGROUP_STATIC,245,320,212,82,BS_LEFT
END

IDD_KEYWORD_STYLE_DLG DIALOGEX 36, 44, 466, 415
STYLE DS_SETFONT | DS_3DLOOK | DS_FIXEDSYS | WS_CHILD
FONT 8, "MS Shell Dlg", 0, 0, 0x0
BEGIN
    EDITTEXT        IDC_KEYWORD1_EDIT,19,44,180,50,ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL
    GROUPBOX        "1st group",IDC_KEYWORD1_DESCGROUP_STATIC,9,17,200,85,BS_CENTER
    PUSHBUTTON      "Styler",IDC_KEYWORD1_STYLER,19,27,53,13
    CONTROL         "Prefix mode",IDC_KEYWORD1_PREFIX_CHECK,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,89,31,68,13
    EDITTEXT        IDC_KEYWORD2_EDIT,262,44,180,50,ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL
    GROUPBOX        "2nd group",IDC_KEYWORD2_DESCGROUP_STATIC,252,17,200,85,BS_CENTER
    PUSHBUTTON      "Styler",IDC_KEYWORD2_STYLER,262,26,53,13
    CONTROL         "Prefix mode",IDC_KEYWORD2_PREFIX_CHECK,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,332,31,68,13
    EDITTEXT        IDC_KEYWORD3_EDIT,19,138,180,50,ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL
    GROUPBOX        "3rd group",IDC_KEYWORD3_DESCGROUP_STATIC,10,111,200,85,BS_CENTER
    PUSHBUTTON      "Styler",IDC_KEYWORD3_STYLER,19,120,53,13
    CONTROL         "Prefix mode",IDC_KEYWORD3_PREFIX_CHECK,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,89,125,68,13
    EDITTEXT        IDC_KEYWORD4_EDIT,262,138,180,50,ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL
    GROUPBOX        "4th group",IDC_KEYWORD4_DESCGROUP_STATIC,252,111,200,85,BS_CENTER
    PUSHBUTTON      "Styler",IDC_KEYWORD4_STYLER,262,120,53,13
    CONTROL         "Prefix mode",IDC_KEYWORD4_PREFIX_CHECK,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,332,125,68,13
    EDITTEXT        IDC_KEYWORD5_EDIT,19,233,180,50,ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL
    GROUPBOX        "5th group",IDC_KEYWORD5_DESCGROUP_STATIC,10,206,200,85,BS_CENTER
    PUSHBUTTON      "Styler",IDC_KEYWORD5_STYLER,19,215,53,13
    CONTROL         "Prefix mode",IDC_KEYWORD5_PREFIX_CHECK,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,89,220,68,13
    EDITTEXT        IDC_KEYWORD6_EDIT,262,233,180,50,ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL
    GROUPBOX        "6th group",IDC_KEYWORD6_DESCGROUP_STATIC,252,206,200,85,BS_CENTER
    PUSHBUTTON      "Styler",IDC_KEYWORD6_STYLER,262,215,53,13
    CONTROL         "Prefix mode",IDC_KEYWORD6_PREFIX_CHECK,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,332,220,68,13
    EDITTEXT        IDC_KEYWORD7_EDIT,19,329,180,50,ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL
    GROUPBOX        "7th group",IDC_KEYWORD7_DESCGROUP_STATIC,10,302,200,85,BS_CENTER
    PUSHBUTTON      "Styler",IDC_KEYWORD7_STYLER,19,311,53,13
    CONTROL         "Prefix mode",IDC_KEYWORD7_PREFIX_CHECK,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,89,315,72,13
    EDITTEXT        IDC_KEYWORD8_EDIT,262,329,180,50,ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL
    GROUPBOX        "8th group",IDC_KEYWORD8_DESCGROUP_STATIC,252,302,200,85,BS_CENTER
    PUSHBUTTON      "Styler",IDC_KEYWORD8_STYLER,262,311,53,13
    CONTROL         "Prefix mode",IDC_KEYWORD8_PREFIX_CHECK,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,332,315,72,13
END

IDD_COMMENT_STYLE_DLG DIALOGEX 36, 44, 466, 415
STYLE DS_SETFONT | DS_3DLOOK | DS_FIXEDSYS | WS_CHILD
FONT 8, "MS Shell Dlg", 0, 0, 0x0
BEGIN
    GROUPBOX        "Line comment position",IDC_COMMENTLINE_POSITION_STATIC,11,7,156,57
    CONTROL         "Allow folding of comments",IDC_FOLDING_OF_COMMENTS,
                    "Button",BS_AUTOCHECKBOX | WS_TABSTOP,188,14,131,8
    CONTROL         "Allow anywhere",IDC_ALLOW_ANYWHERE,"Button",BS_AUTORADIOBUTTON | WS_GROUP,33,22,121,8
    CONTROL         "Force at beginning of line",IDC_FORCE_AT_BOL,"Button",BS_AUTORADIOBUTTON,33,34,121,8
    CONTROL         "Allow preceding whitespace",IDC_ALLOW_WHITESPACE,
                    "Button",BS_AUTORADIOBUTTON,33,46,121,8
    EDITTEXT        IDC_COMMENTLINE_OPEN_EDIT,85,105,138,12,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_COMMENTLINE_CONTINUE_EDIT,85,124,138,12,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_COMMENTLINE_CLOSE_EDIT,85,144,138,12,ES_MULTILINE | WS_VSCROLL
    PUSHBUTTON      "Styler",IDC_COMMENTLINE_STYLER,169,85,54,13
    RTEXT           "Open:",IDC_COMMENTLINE_OPEN_STATIC,12,105,71,9
    RTEXT           "Continue character:",IDC_COMMENTLINE_CONTINUE_STATIC,12,126,71,9
    RTEXT           "Close:",IDC_COMMENTLINE_CLOSE_STATIC,12,145,71,9
    GROUPBOX        "Comment line style",IDC_COMMENTLINE_DESCGROUP_STATIC,10,71,219,94,BS_CENTER
    EDITTEXT        IDC_COMMENT_OPEN_EDIT,312,112,138,12,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_COMMENT_CLOSE_EDIT,312,140,138,12,ES_MULTILINE | WS_VSCROLL
    PUSHBUTTON      "Styler",IDC_COMMENT_STYLER,396,85,54,13
    RTEXT           "Open:",IDC_COMMENT_OPEN_STATIC,242,113,67,9
    RTEXT           "Close:",IDC_COMMENT_CLOSE_STATIC,242,141,68,9
    GROUPBOX        "Comment style",IDC_COMMENT_DESCGROUP_STATIC,238,71,219,94,BS_CENTER
    GROUPBOX        "Number style",IDC_NUMBER_DESCGROUP_STATIC,10,174,447,190,BS_CENTER
    PUSHBUTTON      "Styler",IDC_NUMBER_STYLER,376,185,54,13
    RTEXT           "Prefix 1:",IDC_NUMBER_PREFIX1_STATIC,14,204,45,13
    RTEXT           "Prefix 2:",IDC_NUMBER_PREFIX2_STATIC,245,204,45,13
    RTEXT           "Extras 1:",IDC_NUMBER_EXTRAS1_STATIC,14,235,45,13
    RTEXT           "Extras 2:",IDC_NUMBER_EXTRAS2_STATIC,245,235,45,13
    RTEXT           "Suffix 1:",IDC_NUMBER_SUFFIX1_STATIC,14,266,45,13
    RTEXT           "Suffix 2:",IDC_NUMBER_SUFFIX2_STATIC,245,266,45,13
    RTEXT           "Range: ",IDC_NUMBER_RANGE_STATIC,14,298,45,13
    EDITTEXT        IDC_NUMBER_PREFIX1_EDIT,63,204,136,21,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_NUMBER_PREFIX2_EDIT,294,204,136,21,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_NUMBER_EXTRAS1_EDIT,63,235,136,21,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_NUMBER_EXTRAS2_EDIT,295,235,136,21,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_NUMBER_SUFFIX1_EDIT,63,266,136,21,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_NUMBER_SUFFIX2_EDIT,295,266,136,21,ES_MULTILINE | WS_VSCROLL
    EDITTEXT        IDC_NUMBER_RANGE_EDIT,63,298,136,21,ES_MULTILINE | WS_VSCROLL
    GROUPBOX        "Decimal separator",IDC_DECIMAL_SEPARATOR_STATIC,295,299,136,53,BS_CENTER
    CONTROL         "Dot",IDC_DOT_RADIO,"Button",BS_AUTORADIOBUTTON | WS_GROUP,319,310,59,8
    CONTROL         "Comma",IDC_COMMA_RADIO,"Button",BS_AUTORADIOBUTTON,319,322,59,8
    CONTROL         "Both",IDC_BOTH_RADIO,"Button",BS_AUTORADIOBUTTON,319,335,59,8
END

IDD_GLOBAL_USERDEFINE_DLG DIALOGEX 36, 44, 490, 470
STYLE DS_SETFONT | DS_FIXEDSYS | WS_POPUP | WS_CAPTION | WS_VSCROLL | WS_SYSMENU
EXSTYLE WS_EX_TOOLWINDOW
CAPTION "User Defined Language v2.1"
FONT 8, "MS Shell Dlg", 0, 0, 0x0
BEGIN
    LTEXT           "User language : ",IDC_LANGNAME_STATIC,0,3,63,8,0,WS_EX_RIGHT
    COMBOBOX        IDC_LANGNAME_COMBO,64,1,90,58,CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | WS_VSCROLL | WS_TABSTOP
    CONTROL         "Transparency",IDC_UD_TRANSPARENT_CHECK,"Button",BS_AUTOCHECKBOX | NOT WS_VISIBLE | WS_TABSTOP,408,22,66,10
    CONTROL         "",IDC_UD_PERCENTAGE_SLIDER,"msctls_trackbar32",TBS_BOTH | TBS_NOTICKS | TBS_TRANSPARENTBKGND | NOT WS_VISIBLE | WS_TABSTOP,422,33,53,10
    PUSHBUTTON      "Dock",IDC_DOCK_BUTTON,425,1,50,14,BS_FLAT
    PUSHBUTTON      "Create New...",IDC_ADDNEW_BUTTON,161,1,62,14
    PUSHBUTTON      "Save As...",IDC_SAVEAS_BUTTON,226,1,62,14
    CONTROL         "Ignore case",IDC_LANGNAME_IGNORECASE_CHECK,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,186,24,76,10
    RTEXT           "Ext. :",IDC_EXT_STATIC,259,25,33,8,0,WS_EX_RIGHT
    EDITTEXT        IDC_EXT_EDIT,293,23,33,14,ES_AUTOHSCROLL
    PUSHBUTTON      "Import...",IDC_IMPORT_BUTTON,23,21,62,14
    PUSHBUTTON      "Export...",IDC_EXPORT_BUTTON,93,21,62,14
    PUSHBUTTON      "Rename",IDC_RENAME_BUTTON,290,1,62,14
    PUSHBUTTON      "Remove",IDC_REMOVELANG_BUTTON,354,1,62,14
END

IDD_STRING_DLG DIALOGEX 0, 0, 151, 52
STYLE DS_SETFONT | DS_MODALFRAME | DS_FIXEDSYS | WS_POPUP | WS_CAPTION | WS_SYSMENU
FONT 8, "MS Shell Dlg", 0, 0, 0x1
BEGIN
    LTEXT           "STATIC :",IDC_STRING_STATIC,6,4,42,8,0,WS_EX_RIGHT
    EDITTEXT        IDC_STRING_EDIT,49,2,88,14,ES_AUTOHSCROLL
    PUSHBUTTON      "OK",IDOK,20,26,50,14
    PUSHBUTTON      "Cancel",IDCANCEL,87,26,50,14
END

IDD_STYLER_POPUP_DLG DIALOGEX 100, 50, 324, 305
STYLE DS_SETFONT | DS_MODALFRAME | DS_FIXEDSYS | WS_POPUP | WS_CAPTION
CAPTION "Styler Dialog"
FONT 8, "MS Shell Dlg", 400, 0, 0x1
BEGIN
    DEFPUSHBUTTON   "OK",IDOK,100,280,50,14
    PUSHBUTTON      "Cancel",IDCANCEL,172,280,50,14
    CONTROL         "Bold",IDC_STYLER_CHECK_BOLD,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,201,23,90,13
    CONTROL         "Italic",IDC_STYLER_CHECK_ITALIC,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,201,37,90,13
    CONTROL         "Underline",IDC_STYLER_CHECK_UNDERLINE,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,201,51,90,13
    COMBOBOX        IDC_STYLER_COMBO_FONT_NAME,65,29,104,80,CBS_DROPDOWNLIST | CBS_SORT | WS_VSCROLL | WS_TABSTOP
    COMBOBOX        IDC_STYLER_COMBO_FONT_SIZE,65,49,104,77,CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP
    RTEXT           "Foreground color:",IDC_STYLER_FG_STATIC,14,75,75,11
    CONTROL         "Transparent",IDC_STYLER_CHECK_FG_TRANSPARENT,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,33,92,78,13
    RTEXT           "Background color:",IDC_STYLER_BG_STATIC,153,75,75,11
    CONTROL         "Transparent",IDC_STYLER_CHECK_BG_TRANSPARENT,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,172,92,78,13
    CONTROL         "Delimiter 1",IDC_STYLER_CHECK_NESTING_DELIMITER1,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,38,132,76,13
    CONTROL         "Delimiter 2",IDC_STYLER_CHECK_NESTING_DELIMITER2,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,38,149,76,13
    CONTROL         "Delimiter 3",IDC_STYLER_CHECK_NESTING_DELIMITER3,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,38,166,76,13
    CONTROL         "Comment",IDC_STYLER_CHECK_NESTING_COMMENT,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,209,132,76,13
    CONTROL         "Comment line",IDC_STYLER_CHECK_NESTING_COMMENT_LINE,
                    "Button",BS_AUTOCHECKBOX | WS_TABSTOP,209,149,76,13
    CONTROL         "Keyword 1",IDC_STYLER_CHECK_NESTING_KEYWORD1,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,119,132,76,13
    CONTROL         "Keyword 2",IDC_STYLER_CHECK_NESTING_KEYWORD2,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,119,149,76,13
    CONTROL         "Keyword 3",IDC_STYLER_CHECK_NESTING_KEYWORD3,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,119,166,76,13
    CONTROL         "Keyword 4",IDC_STYLER_CHECK_NESTING_KEYWORD4,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,119,183,76,13
    GROUPBOX        "Nesting:",IDC_STYLER_STATIC_NESTING_GROUP,14,116,295,157
    GROUPBOX        "Font options:",IDC_STYLER_STATIC_FONT_OPTIONS,14,15,295,95
    CONTROL         "Delimiter 4",IDC_STYLER_CHECK_NESTING_DELIMITER4,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,38,183,76,13
    CONTROL         "Delimiter 5",IDC_STYLER_CHECK_NESTING_DELIMITER5,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,38,200,76,13
    CONTROL         "Delimiter 6",IDC_STYLER_CHECK_NESTING_DELIMITER6,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,38,217,76,13
    CONTROL         "Delimiter 7",IDC_STYLER_CHECK_NESTING_DELIMITER7,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,38,234,76,13
    CONTROL         "Delimiter 8",IDC_STYLER_CHECK_NESTING_DELIMITER8,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,38,251,76,13
    CONTROL         "Keyword 5",IDC_STYLER_CHECK_NESTING_KEYWORD5,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,119,200,76,13
    CONTROL         "Keyword 6",IDC_STYLER_CHECK_NESTING_KEYWORD6,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,119,217,76,13
    CONTROL         "Keyword 7",IDC_STYLER_CHECK_NESTING_KEYWORD7,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,119,234,76,13
    CONTROL         "Keyword 8",IDC_STYLER_CHECK_NESTING_KEYWORD8,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,119,251,76,13
    RTEXT           "Name:",IDC_STYLER_NAME_STATIC,26,30,36,12
    RTEXT           "Size:",IDC_STYLER_SIZE_STATIC,25,50,36,12
    CONTROL         "Operators 1",IDC_STYLER_CHECK_NESTING_OPERATORS1,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,209,166,76,13
    CONTROL         "Operators 2",IDC_STYLER_CHECK_NESTING_OPERATORS2,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,209,183,76,13
    CONTROL         "Numbers",IDC_STYLER_CHECK_NESTING_NUMBERS,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,209,200,76,13
END

//////////////////////////////////
////////UserDefineLangReference.h
//////////////////////////////////
#include <Lexilla/SciLexer.h>

const int langNameLenMax = 64;
const int extsLenMax = 256;
const int max_char = 1024*30;
/////////////////////////////
#define    IDD_GLOBAL_USERDEFINE_DLG 20000
    #define    IDC_DOCK_BUTTON                              (IDD_GLOBAL_USERDEFINE_DLG + 1 )
    #define    IDC_RENAME_BUTTON                            (IDD_GLOBAL_USERDEFINE_DLG + 2 )
    #define    IDC_ADDNEW_BUTTON                            (IDD_GLOBAL_USERDEFINE_DLG + 3 )
    #define    IDC_REMOVELANG_BUTTON                        (IDD_GLOBAL_USERDEFINE_DLG + 4 )
    #define    IDC_SAVEAS_BUTTON                            (IDD_GLOBAL_USERDEFINE_DLG + 5 )
    #define    IDC_LANGNAME_COMBO                           (IDD_GLOBAL_USERDEFINE_DLG + 6 )
    #define    IDC_LANGNAME_STATIC                          (IDD_GLOBAL_USERDEFINE_DLG + 7 )
    #define    IDC_EXT_EDIT                                 (IDD_GLOBAL_USERDEFINE_DLG + 8 )
    #define    IDC_EXT_STATIC                               (IDD_GLOBAL_USERDEFINE_DLG + 9 )

    #define    IDC_UD_PERCENTAGE_SLIDER                     (IDD_GLOBAL_USERDEFINE_DLG + 10)
    #define    IDC_UD_TRANSPARENT_CHECK                     (IDD_GLOBAL_USERDEFINE_DLG + 11)
    #define    IDC_LANGNAME_IGNORECASE_CHECK                (IDD_GLOBAL_USERDEFINE_DLG + 12)
    #define    IDC_AUTOCOMPLET_EDIT                         (IDD_GLOBAL_USERDEFINE_DLG + 13)
    #define    IDC_AUTOCOMPLET_STATIC                       (IDD_GLOBAL_USERDEFINE_DLG + 14)
    #define    IDC_IMPORT_BUTTON                            (IDD_GLOBAL_USERDEFINE_DLG + 15)
    #define    IDC_EXPORT_BUTTON                            (IDD_GLOBAL_USERDEFINE_DLG + 16)

#define    IDD_FOLDER_STYLE_DLG   21000 // IDD_GLOBAL_USERDEFINE_DLG + 1000
     #define    IDC_DEFAULT                                 (IDD_FOLDER_STYLE_DLG + 100)
        #define    IDC_DEFAULT_DESCGROUP_STATIC             (IDC_DEFAULT + 1)
        #define    IDC_DEFAULT_STYLER                       (IDC_DEFAULT + 2)
        #define    IDC_WEB_HELP_LINK                        (IDC_DEFAULT + 3)
        #define    IDC_WEB_HELP_STATIC                      (IDC_DEFAULT + 4)
        #define    IDC_WEB_HELP_DESCGROUP_STATIC            (IDC_DEFAULT + 5)
        #define    IDC_FOLDER_FOLD_COMPACT                  (IDC_DEFAULT + 6)

    #define    IDC_FOLDER_IN_CODE1                          (IDD_FOLDER_STYLE_DLG + 200)
        #define IDC_FOLDER_IN_CODE1_DESCGROUP_STATIC            (IDC_FOLDER_IN_CODE1 + 20)
        #define IDC_FOLDER_IN_CODE1_OPEN_EDIT                   (IDC_FOLDER_IN_CODE1 + 21)
        #define IDC_FOLDER_IN_CODE1_MIDDLE_EDIT                 (IDC_FOLDER_IN_CODE1 + 22)
        #define IDC_FOLDER_IN_CODE1_CLOSE_EDIT                  (IDC_FOLDER_IN_CODE1 + 23)
        #define IDC_FOLDER_IN_CODE1_OPEN_STATIC                 (IDC_FOLDER_IN_CODE1 + 24)
        #define IDC_FOLDER_IN_CODE1_MIDDLE_STATIC               (IDC_FOLDER_IN_CODE1 + 25)
        #define IDC_FOLDER_IN_CODE1_CLOSE_STATIC                (IDC_FOLDER_IN_CODE1 + 26)
        #define IDC_FOLDER_IN_CODE1_STYLER                      (IDC_FOLDER_IN_CODE1 + 27)

    #define    IDC_FOLDER_IN_CODE2                          (IDD_FOLDER_STYLE_DLG + 300)
        #define IDC_FOLDER_IN_CODE2_DESCGROUP_STATIC            (IDC_FOLDER_IN_CODE2 + 20)
        #define IDC_FOLDER_IN_CODE2_OPEN_EDIT                   (IDC_FOLDER_IN_CODE2 + 21)
        #define IDC_FOLDER_IN_CODE2_MIDDLE_EDIT                 (IDC_FOLDER_IN_CODE2 + 22)
        #define IDC_FOLDER_IN_CODE2_CLOSE_EDIT                  (IDC_FOLDER_IN_CODE2 + 23)
        #define IDC_FOLDER_IN_CODE2_OPEN_STATIC                 (IDC_FOLDER_IN_CODE2 + 24)
        #define IDC_FOLDER_IN_CODE2_MIDDLE_STATIC               (IDC_FOLDER_IN_CODE2 + 25)
        #define IDC_FOLDER_IN_CODE2_CLOSE_STATIC                (IDC_FOLDER_IN_CODE2 + 26)
        #define IDC_FOLDER_IN_CODE2_STYLER                      (IDC_FOLDER_IN_CODE2 + 27)

    #define    IDC_FOLDER_IN_COMMENT                         (IDD_FOLDER_STYLE_DLG + 400)
        #define IDC_FOLDER_IN_COMMENT_DESCGROUP_STATIC          (IDC_FOLDER_IN_COMMENT + 20)
        #define IDC_FOLDER_IN_COMMENT_OPEN_EDIT                 (IDC_FOLDER_IN_COMMENT + 21)
        #define IDC_FOLDER_IN_COMMENT_MIDDLE_EDIT               (IDC_FOLDER_IN_COMMENT + 22)
        #define IDC_FOLDER_IN_COMMENT_CLOSE_EDIT                (IDC_FOLDER_IN_COMMENT + 23)
        #define IDC_FOLDER_IN_COMMENT_OPEN_STATIC               (IDC_FOLDER_IN_COMMENT + 24)
        #define IDC_FOLDER_IN_COMMENT_MIDDLE_STATIC             (IDC_FOLDER_IN_COMMENT + 25)
        #define IDC_FOLDER_IN_COMMENT_CLOSE_STATIC              (IDC_FOLDER_IN_COMMENT + 26)
        #define IDC_FOLDER_IN_COMMENT_STYLER                    (IDC_FOLDER_IN_COMMENT + 27)

#define    IDD_KEYWORD_STYLE_DLG   22000 //(IDD_GLOBAL_USERDEFINE_DLG + 2000)
    #define    IDC_KEYWORD1                                 (IDD_KEYWORD_STYLE_DLG + 100)
        #define    IDC_KEYWORD1_DESCGROUP_STATIC            (IDC_KEYWORD1 + 1 )
        #define    IDC_KEYWORD1_EDIT                        (IDC_KEYWORD1 + 20)
        #define    IDC_KEYWORD1_PREFIX_CHECK                (IDC_KEYWORD1 + 21)
        #define    IDC_KEYWORD1_STYLER                      (IDC_KEYWORD1 + 22)

    #define    IDC_KEYWORD2                                 (IDD_KEYWORD_STYLE_DLG + 200)
        #define    IDC_KEYWORD2_DESCGROUP_STATIC            (IDC_KEYWORD2 + 1 )
        #define    IDC_KEYWORD2_EDIT                        (IDC_KEYWORD2 + 20)
        #define    IDC_KEYWORD2_PREFIX_CHECK                (IDC_KEYWORD2 + 21)
        #define    IDC_KEYWORD2_STYLER                      (IDC_KEYWORD2 + 22)

    #define    IDC_KEYWORD3                                 (IDD_KEYWORD_STYLE_DLG + 300)
        #define    IDC_KEYWORD3_DESCGROUP_STATIC            (IDC_KEYWORD3 + 1 )
        #define    IDC_KEYWORD3_EDIT                        (IDC_KEYWORD3 + 20)
        #define    IDC_KEYWORD3_PREFIX_CHECK                (IDC_KEYWORD3 + 21)
        #define    IDC_KEYWORD3_STYLER                      (IDC_KEYWORD3 + 22)

    #define    IDC_KEYWORD4                                 (IDD_KEYWORD_STYLE_DLG + 400)
        #define    IDC_KEYWORD4_DESCGROUP_STATIC            (IDC_KEYWORD4 + 1 )
        #define    IDC_KEYWORD4_EDIT                        (IDC_KEYWORD4 + 20)
        #define    IDC_KEYWORD4_PREFIX_CHECK                (IDC_KEYWORD4 + 21)
        #define    IDC_KEYWORD4_STYLER                      (IDC_KEYWORD4 + 22)

    #define    IDC_KEYWORD5                                 (IDD_KEYWORD_STYLE_DLG + 450)
        #define    IDC_KEYWORD5_DESCGROUP_STATIC            (IDC_KEYWORD5 + 1 )
        #define    IDC_KEYWORD5_EDIT                        (IDC_KEYWORD5 + 20)
        #define    IDC_KEYWORD5_PREFIX_CHECK                (IDC_KEYWORD5 + 21)
        #define    IDC_KEYWORD5_STYLER                      (IDC_KEYWORD5 + 22)

    #define    IDC_KEYWORD6                                 (IDD_KEYWORD_STYLE_DLG + 500)
        #define    IDC_KEYWORD6_DESCGROUP_STATIC            (IDC_KEYWORD6 + 1 )
        #define    IDC_KEYWORD6_EDIT                        (IDC_KEYWORD6 + 20)
        #define    IDC_KEYWORD6_PREFIX_CHECK                (IDC_KEYWORD6 + 21)
        #define    IDC_KEYWORD6_STYLER                      (IDC_KEYWORD6 + 22)

    #define    IDC_KEYWORD7                                 (IDD_KEYWORD_STYLE_DLG + 550)
        #define    IDC_KEYWORD7_DESCGROUP_STATIC            (IDC_KEYWORD7 + 1 )
        #define    IDC_KEYWORD7_EDIT                        (IDC_KEYWORD7 + 20)
        #define    IDC_KEYWORD7_PREFIX_CHECK                (IDC_KEYWORD7 + 21)
        #define    IDC_KEYWORD7_STYLER                      (IDC_KEYWORD7 + 22)

    #define    IDC_KEYWORD8                                 (IDD_KEYWORD_STYLE_DLG + 600)
        #define    IDC_KEYWORD8_DESCGROUP_STATIC            (IDC_KEYWORD8 + 1 )
        #define    IDC_KEYWORD8_EDIT                        (IDC_KEYWORD8 + 20)
        #define    IDC_KEYWORD8_PREFIX_CHECK                (IDC_KEYWORD8 + 21)
        #define    IDC_KEYWORD8_STYLER                      (IDC_KEYWORD8 + 22)

#define    IDD_COMMENT_STYLE_DLG 23000 //(IDD_GLOBAL_USERDEFINE_DLG + 3000)
    #define IDC_FOLDING_OF_COMMENTS             (IDD_COMMENT_STYLE_DLG + 1)
    #define IDC_COMMENTLINE_POSITION_STATIC     (IDD_COMMENT_STYLE_DLG + 3)
    #define IDC_ALLOW_ANYWHERE                  (IDD_COMMENT_STYLE_DLG + 4)
    #define IDC_FORCE_AT_BOL                    (IDD_COMMENT_STYLE_DLG + 5)
    #define IDC_ALLOW_WHITESPACE                (IDD_COMMENT_STYLE_DLG + 6)

    #define    IDC_COMMENT                                  (IDD_COMMENT_STYLE_DLG + 100)
        #define    IDC_COMMENT_DESCGROUP_STATIC             (IDC_COMMENT + 1 )
        #define    IDC_COMMENT_OPEN_EDIT                    (IDC_COMMENT + 20)
        #define    IDC_COMMENT_CLOSE_EDIT                   (IDC_COMMENT + 21)
        #define    IDC_COMMENT_OPEN_STATIC                  (IDC_COMMENT + 22)
        #define    IDC_COMMENT_CLOSE_STATIC                 (IDC_COMMENT + 23)
        #define    IDC_COMMENT_STYLER                       (IDC_COMMENT + 24)

    #define    IDC_NUMBER                                   (IDD_COMMENT_STYLE_DLG + 200)
        #define    IDC_NUMBER_DESCGROUP_STATIC              (IDC_NUMBER + 1 )
        #define    IDC_NUMBER_STYLER                        (IDC_NUMBER + 20)
        #define    IDC_NUMBER_PREFIX1_STATIC                (IDC_NUMBER + 30)
        #define    IDC_NUMBER_PREFIX1_EDIT                  (IDC_NUMBER + 31)
        #define    IDC_NUMBER_PREFIX2_STATIC                (IDC_NUMBER + 32)
        #define    IDC_NUMBER_PREFIX2_EDIT                  (IDC_NUMBER + 33)
        #define    IDC_NUMBER_EXTRAS1_STATIC                (IDC_NUMBER + 34)
        #define    IDC_NUMBER_EXTRAS1_EDIT                  (IDC_NUMBER + 35)
        #define    IDC_NUMBER_EXTRAS2_STATIC                (IDC_NUMBER + 36)
        #define    IDC_NUMBER_EXTRAS2_EDIT                  (IDC_NUMBER + 37)
        #define    IDC_NUMBER_SUFFIX1_STATIC                (IDC_NUMBER + 38)
        #define    IDC_NUMBER_SUFFIX1_EDIT                  (IDC_NUMBER + 39)
        #define    IDC_NUMBER_SUFFIX2_STATIC                (IDC_NUMBER + 40)
        #define    IDC_NUMBER_SUFFIX2_EDIT                  (IDC_NUMBER + 41)
        #define    IDC_NUMBER_RANGE_STATIC                  (IDC_NUMBER + 42)
        #define    IDC_NUMBER_RANGE_EDIT                    (IDC_NUMBER + 43)
        #define    IDC_DECIMAL_SEPARATOR_STATIC             (IDC_NUMBER + 44)
        #define    IDC_DOT_RADIO                            (IDC_NUMBER + 45)
        #define    IDC_COMMA_RADIO                          (IDC_NUMBER + 46)
        #define    IDC_BOTH_RADIO                           (IDC_NUMBER + 47)

    #define    IDC_COMMENTLINE                              (IDD_COMMENT_STYLE_DLG + 300)
        #define    IDC_COMMENTLINE_DESCGROUP_STATIC         (IDC_COMMENTLINE + 1 )
        #define    IDC_COMMENTLINE_OPEN_EDIT                (IDC_COMMENTLINE + 20)
        #define    IDC_COMMENTLINE_CONTINUE_EDIT            (IDC_COMMENTLINE + 21)
        #define    IDC_COMMENTLINE_CLOSE_EDIT               (IDC_COMMENTLINE + 22)
        #define    IDC_COMMENTLINE_OPEN_STATIC              (IDC_COMMENTLINE + 23)
        #define    IDC_COMMENTLINE_CONTINUE_STATIC          (IDC_COMMENTLINE + 24)
        #define    IDC_COMMENTLINE_CLOSE_STATIC             (IDC_COMMENTLINE + 25)
        #define    IDC_COMMENTLINE_STYLER                   (IDC_COMMENTLINE + 26)

#define    IDD_SYMBOL_STYLE_DLG   24000   //IDD_GLOBAL_USERDEFINE_DLG + 4000
    #define    IDC_OPERATOR                             (IDD_SYMBOL_STYLE_DLG + 100)
        #define    IDC_OPERATOR_DESCGROUP_STATIC            (IDC_OPERATOR + 1 )
        #define    IDC_OPERATOR_STYLER                      (IDC_OPERATOR + 13)
        #define    IDC_OPERATOR1_EDIT                       (IDC_OPERATOR + 14)
        #define    IDC_OPERATOR2_EDIT                       (IDC_OPERATOR + 15)
        #define    IDC_OPERATOR1_STATIC                     (IDC_OPERATOR + 16)
        #define    IDC_OPERATOR2_STATIC                     (IDC_OPERATOR + 17)

    #define    IDC_DELIMITER1                               (IDD_SYMBOL_STYLE_DLG + 200)
        #define    IDC_DELIMITER1_DESCGROUP_STATIC          (IDC_DELIMITER1 + 1 )
        #define    IDC_DELIMITER1_BOUNDARYOPEN_EDIT         (IDC_DELIMITER1 + 17)
        #define    IDC_DELIMITER1_ESCAPE_EDIT               (IDC_DELIMITER1 + 18)
        #define    IDC_DELIMITER1_BOUNDARYCLOSE_EDIT        (IDC_DELIMITER1 + 19)
        #define    IDC_DELIMITER1_BOUNDARYOPEN_STATIC       (IDC_DELIMITER1 + 20)
        #define    IDC_DELIMITER1_ESCAPE_STATIC             (IDC_DELIMITER1 + 21)
        #define    IDC_DELIMITER1_BOUNDARYCLOSE_STATIC      (IDC_DELIMITER1 + 22)
        #define    IDC_DELIMITER1_STYLER                    (IDC_DELIMITER1 + 23)

    #define    IDC_DELIMITER2                               (IDD_SYMBOL_STYLE_DLG + 300)
        #define    IDC_DELIMITER2_DESCGROUP_STATIC          (IDC_DELIMITER2 + 1 )
        #define    IDC_DELIMITER2_BOUNDARYOPEN_EDIT         (IDC_DELIMITER2 + 17)
        #define    IDC_DELIMITER2_ESCAPE_EDIT               (IDC_DELIMITER2 + 18)
        #define    IDC_DELIMITER2_BOUNDARYCLOSE_EDIT        (IDC_DELIMITER2 + 19)
        #define    IDC_DELIMITER2_BOUNDARYOPEN_STATIC       (IDC_DELIMITER2 + 20)
        #define    IDC_DELIMITER2_ESCAPE_STATIC             (IDC_DELIMITER2 + 21)
        #define    IDC_DELIMITER2_BOUNDARYCLOSE_STATIC      (IDC_DELIMITER2 + 22)
        #define    IDC_DELIMITER2_STYLER                    (IDC_DELIMITER2 + 23)

    #define    IDC_DELIMITER3                               (IDD_SYMBOL_STYLE_DLG + 400)
        #define    IDC_DELIMITER3_DESCGROUP_STATIC          (IDC_DELIMITER3 + 1 )
        #define    IDC_DELIMITER3_BOUNDARYOPEN_EDIT         (IDC_DELIMITER3 + 17)
        #define    IDC_DELIMITER3_ESCAPE_EDIT               (IDC_DELIMITER3 + 18)
        #define    IDC_DELIMITER3_BOUNDARYCLOSE_EDIT        (IDC_DELIMITER3 + 19)
        #define    IDC_DELIMITER3_BOUNDARYOPEN_STATIC       (IDC_DELIMITER3 + 20)
        #define    IDC_DELIMITER3_ESCAPE_STATIC             (IDC_DELIMITER3 + 21)
        #define    IDC_DELIMITER3_BOUNDARYCLOSE_STATIC      (IDC_DELIMITER3 + 22)
        #define    IDC_DELIMITER3_STYLER                    (IDC_DELIMITER3 + 23)

    #define    IDC_DELIMITER4                               (IDD_SYMBOL_STYLE_DLG + 450)
        #define    IDC_DELIMITER4_DESCGROUP_STATIC          (IDC_DELIMITER4 + 1 )
        #define    IDC_DELIMITER4_BOUNDARYOPEN_EDIT         (IDC_DELIMITER4 + 17)
        #define    IDC_DELIMITER4_ESCAPE_EDIT               (IDC_DELIMITER4 + 18)
        #define    IDC_DELIMITER4_BOUNDARYCLOSE_EDIT        (IDC_DELIMITER4 + 19)
        #define    IDC_DELIMITER4_BOUNDARYOPEN_STATIC       (IDC_DELIMITER4 + 20)
        #define    IDC_DELIMITER4_ESCAPE_STATIC             (IDC_DELIMITER4 + 21)
        #define    IDC_DELIMITER4_BOUNDARYCLOSE_STATIC      (IDC_DELIMITER4 + 22)
        #define    IDC_DELIMITER4_STYLER                    (IDC_DELIMITER4 + 23)

    #define    IDC_DELIMITER5                               (IDD_SYMBOL_STYLE_DLG + 500)
        #define    IDC_DELIMITER5_DESCGROUP_STATIC          (IDC_DELIMITER5 + 1 )
        #define    IDC_DELIMITER5_BOUNDARYOPEN_EDIT         (IDC_DELIMITER5 + 17)
        #define    IDC_DELIMITER5_ESCAPE_EDIT               (IDC_DELIMITER5 + 18)
        #define    IDC_DELIMITER5_BOUNDARYCLOSE_EDIT        (IDC_DELIMITER5 + 19)
        #define    IDC_DELIMITER5_BOUNDARYOPEN_STATIC       (IDC_DELIMITER5 + 20)
        #define    IDC_DELIMITER5_ESCAPE_STATIC             (IDC_DELIMITER5 + 21)
        #define    IDC_DELIMITER5_BOUNDARYCLOSE_STATIC      (IDC_DELIMITER5 + 22)
        #define    IDC_DELIMITER5_STYLER                    (IDC_DELIMITER5 + 23)

    #define    IDC_DELIMITER6                               (IDD_SYMBOL_STYLE_DLG + 550)
        #define    IDC_DELIMITER6_DESCGROUP_STATIC          (IDC_DELIMITER6 + 1 )
        #define    IDC_DELIMITER6_BOUNDARYOPEN_EDIT         (IDC_DELIMITER6 + 17)
        #define    IDC_DELIMITER6_ESCAPE_EDIT               (IDC_DELIMITER6 + 18)
        #define    IDC_DELIMITER6_BOUNDARYCLOSE_EDIT        (IDC_DELIMITER6 + 19)
        #define    IDC_DELIMITER6_BOUNDARYOPEN_STATIC       (IDC_DELIMITER6 + 20)
        #define    IDC_DELIMITER6_ESCAPE_STATIC             (IDC_DELIMITER6 + 21)
        #define    IDC_DELIMITER6_BOUNDARYCLOSE_STATIC      (IDC_DELIMITER6 + 22)
        #define    IDC_DELIMITER6_STYLER                    (IDC_DELIMITER6 + 23)

    #define    IDC_DELIMITER7                               (IDD_SYMBOL_STYLE_DLG + 600)
        #define    IDC_DELIMITER7_DESCGROUP_STATIC          (IDC_DELIMITER7 + 1 )
        #define    IDC_DELIMITER7_BOUNDARYOPEN_EDIT         (IDC_DELIMITER7 + 17)
        #define    IDC_DELIMITER7_ESCAPE_EDIT               (IDC_DELIMITER7 + 18)
        #define    IDC_DELIMITER7_BOUNDARYCLOSE_EDIT        (IDC_DELIMITER7 + 19)
        #define    IDC_DELIMITER7_BOUNDARYOPEN_STATIC       (IDC_DELIMITER7 + 20)
        #define    IDC_DELIMITER7_ESCAPE_STATIC             (IDC_DELIMITER7 + 21)
        #define    IDC_DELIMITER7_BOUNDARYCLOSE_STATIC      (IDC_DELIMITER7 + 22)
        #define    IDC_DELIMITER7_STYLER                    (IDC_DELIMITER7 + 23)

    #define    IDC_DELIMITER8                               (IDD_SYMBOL_STYLE_DLG + 650)
        #define    IDC_DELIMITER8_DESCGROUP_STATIC          (IDC_DELIMITER8 + 1 )
        #define    IDC_DELIMITER8_BOUNDARYOPEN_EDIT         (IDC_DELIMITER8 + 17)
        #define    IDC_DELIMITER8_ESCAPE_EDIT               (IDC_DELIMITER8 + 18)
        #define    IDC_DELIMITER8_BOUNDARYCLOSE_EDIT        (IDC_DELIMITER8 + 19)
        #define    IDC_DELIMITER8_BOUNDARYOPEN_STATIC       (IDC_DELIMITER8 + 20)
        #define    IDC_DELIMITER8_ESCAPE_STATIC             (IDC_DELIMITER8 + 21)
        #define    IDC_DELIMITER8_BOUNDARYCLOSE_STATIC      (IDC_DELIMITER8 + 22)
        #define    IDC_DELIMITER8_STYLER                    (IDC_DELIMITER8 + 23)

#define    IDD_STYLER_POPUP_DLG   25000   //IDD_GLOBAL_USERDEFINE_DLG + 5000
    #define IDC_STYLER_CHECK_BOLD                           (IDD_STYLER_POPUP_DLG + 1 )
    #define IDC_STYLER_CHECK_ITALIC                         (IDD_STYLER_POPUP_DLG + 2 )
    #define IDC_STYLER_CHECK_UNDERLINE                      (IDD_STYLER_POPUP_DLG + 3 )
    #define IDC_STYLER_COMBO_FONT_NAME                      (IDD_STYLER_POPUP_DLG + 4 )
    #define IDC_STYLER_COMBO_FONT_SIZE                      (IDD_STYLER_POPUP_DLG + 5 )
    #define IDC_STYLER_FG_STATIC                            (IDD_STYLER_POPUP_DLG + 6 )
    #define IDC_STYLER_BG_STATIC                            (IDD_STYLER_POPUP_DLG + 7 )
    #define IDC_STYLER_CHECK_NESTING_DELIMITER1             (IDD_STYLER_POPUP_DLG + 8 )
    #define IDC_STYLER_CHECK_NESTING_DELIMITER2             (IDD_STYLER_POPUP_DLG + 9 )
    #define IDC_STYLER_CHECK_NESTING_DELIMITER3             (IDD_STYLER_POPUP_DLG + 10)
    #define IDC_STYLER_CHECK_NESTING_DELIMITER4             (IDD_STYLER_POPUP_DLG + 11)
    #define IDC_STYLER_CHECK_NESTING_DELIMITER5             (IDD_STYLER_POPUP_DLG + 12)
    #define IDC_STYLER_CHECK_NESTING_DELIMITER6             (IDD_STYLER_POPUP_DLG + 13)
    #define IDC_STYLER_CHECK_NESTING_DELIMITER7             (IDD_STYLER_POPUP_DLG + 14)
    #define IDC_STYLER_CHECK_NESTING_DELIMITER8             (IDD_STYLER_POPUP_DLG + 15)
    #define IDC_STYLER_CHECK_NESTING_COMMENT                (IDD_STYLER_POPUP_DLG + 16)
    #define IDC_STYLER_CHECK_NESTING_COMMENT_LINE           (IDD_STYLER_POPUP_DLG + 17)
    #define IDC_STYLER_CHECK_NESTING_KEYWORD1               (IDD_STYLER_POPUP_DLG + 18)
    #define IDC_STYLER_CHECK_NESTING_KEYWORD2               (IDD_STYLER_POPUP_DLG + 19)
    #define IDC_STYLER_CHECK_NESTING_KEYWORD3               (IDD_STYLER_POPUP_DLG + 20)
    #define IDC_STYLER_CHECK_NESTING_KEYWORD4               (IDD_STYLER_POPUP_DLG + 21)
    #define IDC_STYLER_CHECK_NESTING_KEYWORD5               (IDD_STYLER_POPUP_DLG + 22)
    #define IDC_STYLER_CHECK_NESTING_KEYWORD6               (IDD_STYLER_POPUP_DLG + 23)
    #define IDC_STYLER_CHECK_NESTING_KEYWORD7               (IDD_STYLER_POPUP_DLG + 24)
    #define IDC_STYLER_CHECK_NESTING_KEYWORD8               (IDD_STYLER_POPUP_DLG + 25)
    #define IDC_STYLER_CHECK_NESTING_OPERATORS1             (IDD_STYLER_POPUP_DLG + 26)
    #define IDC_STYLER_CHECK_NESTING_OPERATORS2             (IDD_STYLER_POPUP_DLG + 27)
    #define IDC_STYLER_CHECK_NESTING_NUMBERS                (IDD_STYLER_POPUP_DLG + 28)
    #define IDC_STYLER_STATIC_NESTING_GROUP                 (IDD_STYLER_POPUP_DLG + 29)
    #define IDC_STYLER_STATIC_FONT_OPTIONS                  (IDD_STYLER_POPUP_DLG + 30)
    #define IDC_STYLER_NAME_STATIC                          (IDD_STYLER_POPUP_DLG + 31)
    #define IDC_STYLER_SIZE_STATIC                          (IDD_STYLER_POPUP_DLG + 32)
    #define IDC_STYLER_CHECK_FG_TRANSPARENT                 (IDD_STYLER_POPUP_DLG + 33)
    #define IDC_STYLER_CHECK_BG_TRANSPARENT                 (IDD_STYLER_POPUP_DLG + 34)

#define    IDD_STRING_DLG   26000   //IDD_GLOBAL_USERDEFINE_DLG + 6000
    #define    IDC_STRING_STATIC                            (IDD_STRING_DLG + 1)
    #define    IDC_STRING_EDIT                              (IDD_STRING_DLG + 2)
/////////////////////////////////////////////
////////xmlMatchedTagsHighlighter.h
////////////////////////////////////////////
class XmlMatchedTagsHighlighter {
public:
	explicit XmlMatchedTagsHighlighter(ScintillaEditView *pEditView):_pEditView(pEditView){};
	void tagMatch(bool doHiliteAttr);
	
private:
	ScintillaEditView* _pEditView = nullptr;
	
	struct XmlMatchedTagsPos {
		intptr_t tagOpenStart = 0;
		intptr_t tagNameEnd = 0;
		intptr_t tagOpenEnd = 0;

		intptr_t tagCloseStart = 0;
		intptr_t tagCloseEnd = 0;
	};

	struct FindResult {
		intptr_t start = 0;
		intptr_t end = 0;
		bool success = false;
	};
	
	bool getXmlMatchedTagsPos(XmlMatchedTagsPos & tagsPos);

	// Allowed whitespace characters in XML
	bool isWhitespace(intptr_t ch) { return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n'; }

	FindResult findText(const char *text, intptr_t start, intptr_t end, int flags = 0);
	FindResult findOpenTag(const String& tagName, intptr_t start, intptr_t end);
	FindResult findCloseTag(const String& tagName, intptr_t start, intptr_t end);
	intptr_t findCloseAngle(intptr_t startPosition, intptr_t endPosition);
	
	std::vector< std::pair<intptr_t, intptr_t> > getAttributesPos(intptr_t start, intptr_t end);
	
};
//////////////////////////////
//ns ends
 }
}
////////////////////////////////////////////////////////////////////////
#endif
