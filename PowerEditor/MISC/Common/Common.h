#pragma once
#include <PowerEditor/WinControls/WinControls.h>
#include <unordered_set>


namespace Upp{

const bool dirUp = true;
const bool dirDown = false;

#define NPP_CP_WIN_1252           1252
#define NPP_CP_DOS_437            437
#define NPP_CP_BIG5               950

#define LINKTRIGGERED WM_USER+555

#define BCKGRD_COLOR (Color(255,102,102))
#define TXT_COLOR    (Color(255,255,255))

#define COPYDATA_FILENAMES COPYDATA_FILENAMESW
#define NPP_INTERNAL_FUCTION_STR TEXT("Notepad++::InternalFunction")

String folderBrowser(Window* parent, const char* title = TEXT(""), int outputCtrlID = 0, const char *defaultStr = nullptr);
String getFolderName(Window* parent, const char *defaultDir = nullptr);

void printInt(int int2print);
void printStr(const char *str2print);
String commafyInt(size_t n);

void writeLog(const char *logFileName, const char *log2write);
int filter(unsigned int code, struct _EXCEPTION_POINTERS *ep);
String purgeMenuItemString(const char * menuItemStr, bool keepAmpersand = false);
Upp::Vector<String> tokenizeString(const String& tokenString, const char delim);

void ClientRectToScreenRect(Window* hWnd, Rect* rect);
void ScreenRectToClientRect(Window* hWnd, Rect* rect);

std::wstring string2wstring(const String& rString, UINT codepage);
String wstring2string(const WString& rwString, UINT codepage);
bool isInList(const char *token, const char *list);
String BuildMenuFileName(int filenameLen, unsigned int pos, const String&filename, bool ordinalNumber = true);

String getFileContent(const char *file2read);
String relativeFilePathToFullFilePath(const char *relativeFilePath);
void writeFileContent(const char *file2write, const char *content2write);
bool matchInList(const char *fileName, const Upp::Vector<String> & patterns);
bool matchInExcludeDirList(const char* dirName, const Upp::Vector<String>& patterns, size_t level);
bool allPatternsAreExclusion(const Upp::Vector<String> patterns);

class WcharMbcsConvertor final
{
public:
	static WcharMbcsConvertor& getInstance() {
		static WcharMbcsConvertor instance;
		return instance;
	}

	const wchar_t * char2wchar(const char *mbStr, size_t codepage, int lenMbcs =-1, int* pLenOut=nullptr, int* pBytesNotProcessed=nullptr);
	const wchar_t * char2wchar(const char *mbcs2Convert, size_t codepage, intptr_t* mstart, intptr_t* mend);
	const char * wchar2char(const wchar_t *wcStr, size_t codepage, int lenIn = -1, int* pLenOut = nullptr);
	const char * wchar2char(const wchar_t *wcStr, size_t codepage, intptr_t* mstart, intptr_t* mend);

	const char * encode(UINT fromCodepage, UINT toCodepage, const char *txt2Encode, int lenIn = -1, int* pLenOut=nullptr, int* pBytesNotProcessed=nullptr)
	{
		int lenWc = 0;
        const wchar_t * strW = char2wchar(txt2Encode, fromCodepage, lenIn, &lenWc, pBytesNotProcessed);
        return wchar2char(strW, toCodepage, lenWc, pLenOut);
    }

protected:
	WcharMbcsConvertor() = default;
	~WcharMbcsConvertor() = default;

	// Since there's no public ctor, we need to void the default assignment operator and copy ctor.
	// Since these are marked as deleted does not matter under which access specifier are kept
	WcharMbcsConvertor(const WcharMbcsConvertor&) = delete;
	WcharMbcsConvertor& operator= (const WcharMbcsConvertor&) = delete;

	// No move ctor and assignment
	WcharMbcsConvertor(WcharMbcsConvertor&&) = delete;
	WcharMbcsConvertor& operator= (WcharMbcsConvertor&&) = delete;

	template <class T>
	class StringBuffer final
	{
	public:
		~StringBuffer() { if (_allocLen) delete[] _str; }

		void sizeTo(size_t size)
		{
			if (_allocLen < size)
			{
				if (_allocLen)
					delete[] _str;
				_allocLen = fmax(size, initSize);
				_str = new T[_allocLen];
			}
		}

		void empty()
		{
			static T nullStr = 0; // routines may return an empty String, with null terminator, without allocating memory; a pointer to this null character will be returned in that case
			if (_allocLen == 0)
				_str = &nullStr;
			else
				_str[0] = 0;
		}

		operator T* () { return _str; }
		operator const T* () const { return _str; }

	protected:
		static const int initSize = 1024;
		size_t _allocLen = 0;
		T* _str = nullptr;
	};

	StringBuffer<char> _multiByteStr;
	StringBuffer<wchar_t> _wideCharStr;
};


#define REBARBAND_SIZE sizeof(REBARBANDINFO)

String PathRemoveFileSpec(String& path);
String pathAppend(String&strDest, const String& str2append);
Color& getCtrlBgColor(Window* hWnd);
String stringToUpper(String strToConvert);
String stringToLower(String strToConvert);
String stringReplace(String subject, const char* search, const char* replace);
Upp::Vector<String> stringSplit(const char* input, const char* delimiter);
bool str2numberVector(String str2convert, std::vector<size_t>& numVect);
String stringJoin(const Upp::Vector<String>& strings, const char* separator);
String stringTakeWhileAdmissable(const char* input, const char* admissable);
double stodLocale(const char* str, _locale_t loc, size_t* idx = nullptr);

int OrdinalIgnoreCaseCompareStrings(LPCTSTR sz1, LPCTSTR sz2);

bool str2Clipboard(const String&str2cpy, Window* hwnd);
class SciBuffer;
bool buf2Clipborad(const std::vector<SciBuffer*>& buffers, bool isFullPath, Window* hwnd);

String GetLastErrorAsString(dword errorCode = 0);

String intToString(int val);
String uintToString(unsigned int val);

Window* CreateToolTip(int toolID, Window* hDlg, Window& hInst, const PTSTR pszText, bool isRTL);
Window* CreateToolTipRect(int toolID, Window* hWnd, Window& hInst, const PTSTR pszText, const Rect rc);

bool isCertificateValidated(const String& fullFilePath, const String& subjectName2check);
bool isAssoCommandExisting(LPCTSTR FullPathName);

std::wstring s2ws(const String& str);
String ws2s(const std::wstring& wstr);

bool deleteFileOrFolder(const char* f2delete);

void getFilesInFolder(Upp::Vector<String>& files, const char* extTypeFilter, const char* inFolder);

template<typename T> size_t vecRemoveDuplicates(std::vector<T>& vec, bool isSorted = false, bool canSort = false)
{
	if (!isSorted && canSort)
	{
		std::sort(vec.begin(), vec.end());
		isSorted = true;
	}

	if (isSorted)
	{
		typename std::vector<T>::iterator it;
		it = std::unique(vec.begin(), vec.end());
		vec.resize(distance(vec.begin(), it));  // unique() does not shrink the vector
	}
	else
	{
		std::unordered_set<T> seen;
		auto newEnd = std::remove_if(vec.begin(), vec.end(), [&seen](const T& value)
			{
				return !seen.insert(value).second;
			});
		vec.erase(newEnd, vec.end());
	}
	return vec.size();
}

void trim(String& str);
bool endsWith(const char* s, const char* suffix);

int nbDigitsFromNbLines(size_t nbLines);

String getDateTimeStrFrom(const char* dateTimeFormat, const SYSTEMTIME& st);

HFONT createFont(const char* fontName, int fontSize, bool isBold, Window* hDestParent);

class Version final
{
public:
	Version() = default;
	Version(const char* versionStr);

	void setVersionFrom(const char* filePath);
	String toString();
	bool isNumber(const char* s) const {
		return IsNumber(s);
	};

	int compareTo(const Version& v2c) const;

	bool operator < (const Version& v2c) const {
		return compareTo(v2c) == -1;
	};

	bool operator <= (const Version& v2c) const {
		int r = compareTo(v2c);
		return r == -1 || r == 0;
	};

	bool operator > (const Version& v2c) const {
		return compareTo(v2c) == 1;
	};

	bool operator >= (const Version& v2c) const {
		int r = compareTo(v2c);
		return r == 1 || r == 0;
	};

	bool operator == (const Version& v2c) const {
		return compareTo(v2c) == 0;
	};

	bool operator != (const Version& v2c) const {
		return compareTo(v2c) != 0;
	};

	bool empty() const {
		return _major == 0 && _minor == 0 && _patch == 0 && _build == 0;
	}

	bool isCompatibleTo(const Version& from, const Version& to) const;

private:
	unsigned long _major = 0;
	unsigned long _minor = 0;
	unsigned long _patch = 0;
	unsigned long _build = 0;
};

}//ns end