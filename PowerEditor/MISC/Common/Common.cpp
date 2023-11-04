#include <algorithm>
#include <stdexcept>
//#include <shlwapi.h>
//#include <uxtheme.h>
#include <cassert>
#include <codecvt>
#include <locale>
#include <PowerEditor/WinControls/StaticDialog/StaticDialog.h>
#include <PowerEditor/WinControls/OpenSaveFileDialog/CustomFileDialog.h>

#include <PowerEditor/MISC/Common/FileInterface.h>
#include <PowerEditor/MISC/Common/Common.h>
#include <PowerEditor/Utf8.h>
#include <Parameters.h>
#include <PowerEditor/ScintillaComponent/ScintillaComponent.h>

void printInt(int int2print)
{
	char str[32];
	wsprintf(str, TEXT("%d"), int2print);
	::MessageBox(nullptr, str, TEXT(""), MB_OK);
}


void printStr(const char *str2print)
{
	::MessageBox(nullptr, str2print, TEXT(""), MB_OK);
}

String commafyInt(size_t n)
{
	StringStream ss;
	ss.imbue(std::locale(""));
	ss << n;
	return ss.str();
}

String getFileContent(const char *file2read)
{
	if (!::PathFileExists(file2read))
		return "";

	const size_t blockSize = 1024;
	char data[blockSize];
	String wholeFileContent = "";
	FILE *fp = generic_fopen(file2read, TEXT("rb"));

	size_t lenFile = 0;
	do
	{
		lenFile = fread(data, 1, blockSize, fp);
		if (lenFile <= 0) break;
		wholeFileContent.append(data, lenFile);
	}
	while (lenFile > 0);

	fclose(fp);
	return wholeFileContent;
}

char getDriveLetter()
{
	char drive = '\0';
	char current[MAX_PATH];

	::GetCurrentDirectory(MAX_PATH, current);
	int driveNbr = ::PathGetDriveNumber(current);
	if (driveNbr != -1)
		drive = 'A' + char(driveNbr);

	return drive;
}


String relativeFilePathToFullFilePath(const char *relativeFilePath)
{
	String fullFilePathName;
	char fullFileName[MAX_PATH];
	BOOL isRelative = ::PathIsRelative(relativeFilePath);

	if (isRelative)
	{
		::GetFullPathName(relativeFilePath, MAX_PATH, fullFileName, nullptr);
		fullFilePathName += fullFileName;
	}
	else
	{
		if ((relativeFilePath[0] == '\\' && relativeFilePath[1] != '\\') || relativeFilePath[0] == '/')
		{
			fullFilePathName += getDriveLetter();
			fullFilePathName += ':';
		}

		fullFilePathName += relativeFilePath;
	}

	return fullFilePathName;
}


void writeFileContent(const char *file2write, const char *content2write)
{
	Win32_IO_File file(file2write);

	if (file.isOpened())
		file.writeStr(content2write);
}


void writeLog(const char *logFileName, const char *log2write)
{
	const dword accessParam{ GENERIC_READ | GENERIC_WRITE };
	const dword shareParam{ FILE_SHARE_READ | FILE_SHARE_WRITE };
	const dword dispParam{ OPEN_ALWAYS }; // Open existing file for writing without destroying it or create new
	const dword attribParam{ FILE_ATTRIBUTE_NORMAL };
	void* hFile = ::CreateFileW(logFileName, accessParam, shareParam, nullptr, dispParam, attribParam, nullptr);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		LARGE_INTEGER offset;
		offset.QuadPart = 0;
		::SetFilePointerEx(hFile, offset, nullptr, FILE_END);

		SYSTEMTIME currentTime = {};
		::GetLocalTime(&currentTime);
		String dateTimeStrW = getDateTimeStrFrom(TEXT("yyyy-MM-dd HH:mm:ss"), currentTime);
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		String log2writeStr = converter.to_bytes(dateTimeStrW);
		log2writeStr += "  ";
		log2writeStr += log2write;
		log2writeStr += "\n";

		dword bytes_written = 0;
		::WriteFile(hFile, log2writeStr.Begin(), static_cast<dword>(log2writeStr.GetLength()), &bytes_written, nullptr);

		::FlushFileBuffers(hFile);
		::CloseHandle(hFile);
	}
}


String folderBrowser(Window* parent, const String& title, int outputCtrlID, const char *defaultStr)
{
	String folderName;
	CustomFileDialog dlg(parent);
	dlg.setTitle(title.Begin());

	// Get an initial directory from the edit control or from argument provided
	char directory[MAX_PATH] = {};
	if (outputCtrlID != 0)
		::GetDlgItemText(parent, outputCtrlID, directory, _countof(directory));
	directory[_countof(directory) - 1] = '\0';
	if (!directory[0] && defaultStr)
		dlg.setFolder(defaultStr);
	else if (directory[0])
		dlg.setFolder(directory);

	folderName = dlg.pickFolder();
	if (!folderName.IsEmpty())
	{
		// Send the result back to the edit control
		if (outputCtrlID != 0)
			::SetDlgItemText(parent, outputCtrlID, folderName.Begin());
	}
	return folderName;
}


String getFolderName(Window* parent, const char *defaultDir)
{
	return folderBrowser(parent, TEXT("Select a folder"), 0, defaultDir);
}


void ClientRectToScreenRect(Window* hWnd, Rect* rect)
{
	POINT		pt;

	pt.x		 = rect->left;
	pt.y		 = rect->top;
	::ClientToScreen( hWnd, &pt );
	rect->left   = pt.x;
	rect->top    = pt.y;

	pt.x		 = rect->right;
	pt.y		 = rect->bottom;
	::ClientToScreen( hWnd, &pt );
	rect->right  = pt.x;
	rect->bottom = pt.y;
}


Vector<String> tokenizeString(const String& tokenString, const char delim)
{
	//Vector is created on stack and copied on return
	Vector<String> tokens;

    // Skip delimiters at beginning.
	String::size_type lastPos = tokenString.find_first_not_of(delim, 0);
    // Find first "non-delimiter".
    String::size_type pos     = tokenString.find_first_of(delim, lastPos);

    while (pos != String::npos || lastPos != String::npos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(tokenString.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = tokenString.find_first_not_of(delim, pos);
        // Find next "non-delimiter"
        pos = tokenString.find_first_of(delim, lastPos);
    }
	return tokens;
}


void ScreenRectToClientRect(Window* hWnd, Rect* rect)
{
	POINT		pt;

	pt.x		 = rect->left;
	pt.y		 = rect->top;
	::ScreenToClient( hWnd, &pt );
	rect->left   = pt.x;
	rect->top    = pt.y;

	pt.x		 = rect->right;
	pt.y		 = rect->bottom;
	::ScreenToClient( hWnd, &pt );
	rect->right  = pt.x;
	rect->bottom = pt.y;
}


int filter(unsigned int code, struct _EXCEPTION_POINTERS *)
{
    if (code == EXCEPTION_ACCESS_VIOLATION)
        return EXCEPTION_EXECUTE_HANDLER;
    return EXCEPTION_CONTINUE_SEARCH;
}


bool isInList(const char *token, const char *list)
{
	if ((!token) || (!list))
		return false;

	const size_t wordLen = 64;
	size_t listLen = lstrlen(list);

	char word[wordLen];
	size_t i = 0;
	size_t j = 0;

	for (; i <= listLen; ++i)
	{
		if ((list[i] == ' ')||(list[i] == '\0'))
		{
			if (j != 0)
			{
				word[j] = '\0';
				j = 0;

				if (!generic_stricmp(token, word))
					return true;
			}
		}
		else
		{
			word[j] = list[i];
			++j;

			if (j >= wordLen)
				return false;
		}
	}
	return false;
}


String purgeMenuItemString(const char * menuItemStr, bool keepAmpersand)
{
	const size_t cleanedNameLen = 64;
	char cleanedName[cleanedNameLen] = TEXT("");
	size_t j = 0;
	size_t menuNameLen = lstrlen(menuItemStr);
	if (menuNameLen >= cleanedNameLen)
		menuNameLen = cleanedNameLen - 1;

	for (size_t k = 0 ; k < menuNameLen ; ++k)
	{
		if (menuItemStr[k] == '\t')
		{
			cleanedName[k] = 0;
			break;
		}
		else
		{
			if (menuItemStr[k] == '&')
			{
				if (keepAmpersand)
					cleanedName[j++] = menuItemStr[k];
				//else skip
			}
			else
				cleanedName[j++] = menuItemStr[k];
		}
	}

	cleanedName[j] = 0;
	return cleanedName;
}


const wchar_t * WcharMbcsConvertor::char2wchar(const char * mbcs2Convert, size_t codepage, int lenMbcs, int* pLenWc, int* pBytesNotProcessed)
{
	// Do not process nullptr pointer
	if (!mbcs2Convert)
		return nullptr;

	// Do not process empty strings
	if (lenMbcs == 0 || lenMbcs == -1 && mbcs2Convert[0] == 0)
	{
		_wideCharStr.IsEmpty();
		return _wideCharStr;
	}

	UINT cp = static_cast<UINT>(codepage);
	int bytesNotProcessed = 0;
	int lenWc = 0;

	// If length not specified, simply convert without checking
	if (lenMbcs == -1)
	{
		lenWc = MultiByteToWideChar(cp, 0, mbcs2Convert, lenMbcs, nullptr, 0);
	}
	// Otherwise, test if we are cutting a multi-byte character at end of buffer
	else if (lenMbcs != -1 && cp == CP_UTF8) // For UTF-8, we know how to test it
	{
		int indexOfLastChar = Utf8::characterStart(mbcs2Convert, lenMbcs-1); // get index of last character
		if (indexOfLastChar != 0 && !Utf8::isValid(mbcs2Convert+indexOfLastChar, lenMbcs-indexOfLastChar)) // if it is not valid we do not process it right now (unless its the only character in String, to ensure that we always progress, e.g. that bytesNotProcessed < lenMbcs)
		{
			bytesNotProcessed = lenMbcs-indexOfLastChar;
		}
		lenWc = MultiByteToWideChar(cp, 0, mbcs2Convert, lenMbcs-bytesNotProcessed, nullptr, 0);
	}
	else // For other encodings, ask system if there are any invalid characters; note that it will not correctly know if last character is cut when there are invalid characters inside the text
	{
		lenWc = MultiByteToWideChar(cp, (lenMbcs == -1) ? 0 : MB_ERR_INVALID_CHARS, mbcs2Convert, lenMbcs, nullptr, 0);
		if (lenWc == 0 && GetLastError() == ERROR_NO_UNICODE_TRANSLATION)
		{
			// Test without last byte
			if (lenMbcs > 1) lenWc = MultiByteToWideChar(cp, MB_ERR_INVALID_CHARS, mbcs2Convert, lenMbcs-1, nullptr, 0);
			if (lenWc == 0) // don't have to check that the error is still ERROR_NO_UNICODE_TRANSLATION, since only the length parameter changed
			{
				// TODO: should warn user about incorrect loading due to invalid characters
				// We still load the file, but the system will either strip or replace invalid characters (including the last character, if cut in half)
				lenWc = MultiByteToWideChar(cp, 0, mbcs2Convert, lenMbcs, nullptr, 0);
			}
			else
			{
				// We found a valid text by removing one byte.
				bytesNotProcessed = 1;
			}
		}
	}

	if (lenWc > 0)
	{
		_wideCharStr.sizeTo(lenWc);
		MultiByteToWideChar(cp, 0, mbcs2Convert, lenMbcs-bytesNotProcessed, _wideCharStr, lenWc);
	}
	else
		_wideCharStr.IsEmpty();

	if (pLenWc)
		*pLenWc = lenWc;
	if (pBytesNotProcessed)
		*pBytesNotProcessed = bytesNotProcessed;

	return _wideCharStr;
}


// "mstart" and "mend" are pointers to indexes in mbcs2Convert,
// which are converted to the corresponding indexes in the returned wchar_t String.
const wchar_t * WcharMbcsConvertor::char2wchar(const char * mbcs2Convert, size_t codepage, intptr_t* mstart, intptr_t* mend)
{
	// Do not process nullptr pointer
	if (!mbcs2Convert) return nullptr;
	UINT cp = static_cast<UINT>(codepage);
	int len = MultiByteToWideChar(cp, 0, mbcs2Convert, -1, nullptr, 0);
	if (len > 0)
	{
		_wideCharStr.sizeTo(len);
		len = MultiByteToWideChar(cp, 0, mbcs2Convert, -1, _wideCharStr, len);

		if ((size_t)*mstart < strlen(mbcs2Convert) && (size_t)*mend <= strlen(mbcs2Convert))
		{
			*mstart = MultiByteToWideChar(cp, 0, mbcs2Convert, static_cast<int>(*mstart), _wideCharStr, 0);
			*mend   = MultiByteToWideChar(cp, 0, mbcs2Convert, static_cast<int>(*mend), _wideCharStr, 0);
			if (*mstart >= len || *mend >= len)
			{
				*mstart = 0;
				*mend = 0;
			}
		}
	}
	else
	{
		_wideCharStr.IsEmpty();
		*mstart = 0;
		*mend = 0;
	}
	return _wideCharStr;
}


const char* WcharMbcsConvertor::wchar2char(const wchar_t * wcharStr2Convert, size_t codepage, int lenWc, int* pLenMbcs)
{
	if (nullptr == wcharStr2Convert)
		return nullptr;
	UINT cp = static_cast<UINT>(codepage);
	int lenMbcs = WideCharToMultiByte(cp, 0, wcharStr2Convert, lenWc, nullptr, 0, nullptr, nullptr);
	if (lenMbcs > 0)
	{
		_multiByteStr.sizeTo(lenMbcs);
		WideCharToMultiByte(cp, 0, wcharStr2Convert, lenWc, _multiByteStr, lenMbcs, nullptr, nullptr);
	}
	else
		_multiByteStr.IsEmpty();

	if (pLenMbcs)
		*pLenMbcs = lenMbcs;
	return _multiByteStr;
}


const char * WcharMbcsConvertor::wchar2char(const wchar_t * wcharStr2Convert, size_t codepage, intptr_t* mstart, intptr_t* mend)
{
	if (nullptr == wcharStr2Convert)
		return nullptr;
	UINT cp = static_cast<UINT>(codepage);
	int len = WideCharToMultiByte(cp, 0, wcharStr2Convert, -1, nullptr, 0, nullptr, nullptr);
	if (len > 0)
	{
		_multiByteStr.sizeTo(len);
		len = WideCharToMultiByte(cp, 0, wcharStr2Convert, -1, _multiByteStr, len, nullptr, nullptr); // not needed?

        if (*mstart < lstrlenW(wcharStr2Convert) && *mend < lstrlenW(wcharStr2Convert))
        {
			*mstart = WideCharToMultiByte(cp, 0, wcharStr2Convert, (int)*mstart, nullptr, 0, nullptr, nullptr);
			*mend = WideCharToMultiByte(cp, 0, wcharStr2Convert, (int)*mend, nullptr, 0, nullptr, nullptr);
			if (*mstart >= len || *mend >= len)
			{
				*mstart = 0;
				*mend = 0;
			}
		}
	}
	else
		_multiByteStr.IsEmpty();

	return _multiByteStr;
}


std::wstring string2wstring(const String& rString, UINT codepage)
{
	int len = MultiByteToWideChar(codepage, 0, rString.Begin(), -1, nullptr, 0);
	if (len > 0)
	{
		std::vector<wchar_t> vw(len);
		MultiByteToWideChar(codepage, 0, rString.Begin(), -1, &vw[0], len);
		return &vw[0];
	}
	return std::wstring();
}


String wstring2string(const WString& rwString, UINT codepage)
{
	int len = WideCharToMultiByte(codepage, 0, rwString.Begin(), -1, nullptr, 0, nullptr, nullptr);
	if (len > 0)
	{
		std::vector<char> vw(len);
		WideCharToMultiByte(codepage, 0, rwString.Begin(), -1, &vw[0], len, nullptr, nullptr);
		return &vw[0];
	}
	return String();
}


// Escapes ampersands in file name to use it in menu
template <typename T>
String convertFileName(T beg, T end)
{
	String strTmp;

	for (T it = beg; it != end; ++it)
	{
		if (*it == '&') strTmp.push_back('&');
		strTmp.push_back(*it);
	}

	return strTmp;
}


String intToString(int val)
{
	std::vector<char> vt;
	bool isNegative = val < 0;
	// can't use abs here because std::numeric_limits<int>::min() has no positive representation
	//val = std::abs(val);

	vt.push_back('0' + static_cast<char>(std::abs(val % 10)));
	val /= 10;
	while (val != 0)
	{
		vt.push_back('0' + static_cast<char>(std::abs(val % 10)));
		val /= 10;
	}

	if (isNegative)
		vt.push_back('-');

	return String(vt.rbegin(), vt.rend());
}

String uintToString(unsigned int val)
{
	std::vector<char> vt;

	vt.push_back('0' + static_cast<char>(val % 10));
	val /= 10;
	while (val != 0)
	{
		vt.push_back('0' + static_cast<char>(val % 10));
		val /= 10;
	}

	return String(vt.rbegin(), vt.rend());
}

// Build Recent File menu entries from given
String BuildMenuFileName(int filenameLen, unsigned int pos, const String&filename, bool ordinalNumber)
{
	String strTemp;

	if (ordinalNumber)
	{
		if (pos < 9)
		{
			strTemp.push_back('&');
			strTemp.push_back('1' + static_cast<char>(pos));
		}
		else if (pos == 9)
		{
			strTemp.append(TEXT("1&0"));
		}
		else
		{
			div_t splitDigits = div(pos + 1, 10);
			strTemp.append(uintToString(splitDigits.quot));
			strTemp.push_back('&');
			strTemp.append(uintToString(splitDigits.rem));
		}
		strTemp.append(TEXT(": "));
	}
	else
	{
		strTemp.push_back('&');
	}

	if (filenameLen > 0)
	{
		std::vector<char> vt(filenameLen + 1);
		// W removed from PathCompactPathExW due to compiler errors for ANSI version.
		PathCompactPathEx(&vt[0], filename.Begin(), filenameLen + 1, 0);
		strTemp.append(convertFileName(vt.begin(), vt.begin() + lstrlen(&vt[0])));
	}
	else
	{
		// (filenameLen < 0)
		String::const_iterator it = filename.begin();

		if (filenameLen == 0)
			it += PathFindFileName(filename.Begin()) - filename.Begin();

		// MAX_PATH is still here to keep old trimming behaviour.
		if (filename.end() - it < MAX_PATH)
		{
			strTemp.append(convertFileName(it, filename.end()));
		}
		else
		{
			strTemp.append(convertFileName(it, it + MAX_PATH / 2 - 3));
			strTemp.append(TEXT("..."));
			strTemp.append(convertFileName(filename.end() - MAX_PATH / 2, filename.end()));
		}
	}

	return strTemp;
}


String PathRemoveFileSpec(String& path)
{
    String::size_type lastBackslash = path.find_last_of(TEXT('\\'));
    if (lastBackslash == String::npos)
    {
        if (path.size() >= 2 && path[1] == TEXT(':'))  // "C:foo.bar" becomes "C:"
            path.erase(2);
        else
            path.erase();
    }
    else
    {
        if (lastBackslash == 2 && path[1] == TEXT(':') && path.size() >= 3)  // "C:\foo.exe" becomes "C:\"
            path.erase(3);
        else if (lastBackslash == 0 && path.size() > 1) // "\foo.exe" becomes "\"
            path.erase(1);
        else
            path.erase(lastBackslash);
    }
	return path;
}


String pathAppend(String& strDest, const char* str2append)
{
	if (strDest.IsEmpty() && str2append.IsEmpty()) // "" + ""
	{
		strDest = TEXT("\\");
		return strDest;
	}

	if (strDest.IsEmpty() && !str2append.IsEmpty()) // "" + titi
	{
		strDest = str2append;
		return strDest;
	}

	if (strDest[strDest.GetLength() - 1] == '\\' && (!str2append.IsEmpty() && str2append[0] == '\\')) // toto\ + \titi
	{
		strDest.erase(strDest.GetLength() - 1, 1);
		strDest += str2append;
		return strDest;
	}

	if ((strDest[strDest.GetLength() - 1] == '\\' && (!str2append.IsEmpty() && str2append[0] != '\\')) // toto\ + titi
		|| (strDest[strDest.GetLength() - 1] != '\\' && (!str2append.IsEmpty() && str2append[0] == '\\'))) // toto + \titi
	{
		strDest += str2append;
		return strDest;
	}

	// toto + titi
	strDest += TEXT("\\");
	strDest += str2append;

	return strDest;
}


Color& getCtrlBgColor(Window* hWnd)
{
	Color& crRet = CLR_INVALID;
	if (hWnd && IsWindow(hWnd))
	{
		Rect rc;
		if (GetClientRect(hWnd, &rc))
		{
			HDC hDC = GetDC(hWnd);
			if (hDC)
			{
				HDC hdcMem = CreateCompatibleDC(hDC);
				if (hdcMem)
				{
					HBITMAP hBmp = CreateCompatibleBitmap(hDC,
					rc.right, rc.bottom);
					if (hBmp)
					{
						HGDIOBJ hOld = SelectObject(hdcMem, hBmp);
						if (hOld)
						{
							if (SendMessage(hWnd, WM_ERASEBKGND, reinterpret_cast<WPARAM>(hdcMem), 0))
							{
								crRet = GetPixel(hdcMem, 2, 2); // 0, 0 is usually on the border
							}
							SelectObject(hdcMem, hOld);
						}
						DeleteObject(hBmp);
					}
					DeleteDC(hdcMem);
				}
				ReleaseDC(hWnd, hDC);
			}
		}
	}
	return crRet;
}


String stringToUpper(String strToConvert)
{
    std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(),
        [](char ch){ return static_cast<char>(_totupper(ch)); }
    );
    return strToConvert;
}

String stringToLower(String strToConvert)
{
    std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), ::towlower);
    return strToConvert;
}


String stringReplace(String subject, const char* search, const char* replace)
{
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != String::npos)
	{
		subject.replace(pos, search.GetLength(), replace);
		pos += replace.GetLength();
	}
	return subject;
}


Vector<String> stringSplit(const char* input, const char* delimiter)
{
	size_t start = 0U;
	size_t end = input.find(delimiter);
	Vector<String> output;
	const size_t delimiterLength = delimiter.GetLength();
	while (end != String::npos)
	{
		output.push_back(input.substr(start, end - start));
		start = end + delimiterLength;
		end = input.find(delimiter, start);
	}
	output.push_back(input.substr(start, end));
	return output;
}


bool str2numberVector(String str2convert, std::vector<size_t>& numVect)
{
	numVect.clear();

	for (auto i : str2convert)
	{
		switch (i)
		{
		case ' ':
		case '0': case '1':	case '2': case '3':	case '4':
		case '5': case '6':	case '7': case '8':	case '9':
		{
			// correct. do nothing
		}
		break;

		default:
			return false;
		}
	}

	Vector<String> v = stringSplit(str2convert, TEXT(" "));
	for (auto i : v)
	{
		// Don't treat empty String and the number greater than 9999
		if (!i.IsEmpty() && i.GetLength() < 5)
		{
			numVect.push_back(std::stoi(i));
		}
	}
	return true;
}

String stringJoin(const Vector<String>& strings, const char* separator)
{
	String joined;
	size_t length = strings.size();
	for (size_t i = 0; i < length; ++i)
	{
		joined += strings.at(i);
		if (i != length - 1)
		{
			joined += separator;
		}
	}
	return joined;
}


String stringTakeWhileAdmissable(const char* input, const char* admissable)
{
	// Find first non-admissable character in "input", and remove everything after it.
	size_t idx = input.find_first_not_of(admissable);
	if (idx == String::npos)
	{
		return input;
	}
	else
	{
		return input.substr(0, idx);
	}
}


double stodLocale(const char* str, _locale_t loc, size_t* idx)
{
	// Copied from the std::stod implementation but uses _wcstod_l instead of wcstod.
	const wchar_t* ptr = str.Begin();
	errno = 0;
	wchar_t* eptr;
#ifdef __MINGW32__
	double ans = ::wcstod(ptr, &eptr);
#else
	double ans = ::_wcstod_l(ptr, &eptr, loc);
#endif
	if (ptr == eptr)
		throw std::invalid_argument("invalid stod argument");
	if (errno == ERANGE)
		throw std::out_of_range("stod argument out of range");
	if (idx != nullptr)
		*idx = (size_t)(eptr - ptr);
	return ans;
}

// Source: https://blogs.msdn.microsoft.com/greggm/2005/09/21/comparing-file-names-in-native-code/
// Modified to use char's instead of assuming Unicode and reformatted to conform with Notepad++ code style
static char ToUpperInvariant(char input)
{
	char result;
	LONG lres = LCMapString(LOCALE_INVARIANT, LCMAP_UPPERCASE, &input, 1, &result, 1);
	if (lres == 0)
	{
		assert(false and "LCMapString failed to convert a character to upper case");
		result = input;
	}
	return result;
}

// Source: https://blogs.msdn.microsoft.com/greggm/2005/09/21/comparing-file-names-in-native-code/
// Modified to use char's instead of assuming Unicode and reformatted to conform with Notepad++ code style
int OrdinalIgnoreCaseCompareStrings(LPCTSTR sz1, LPCTSTR sz2)
{
	if (sz1 == sz2)
	{
		return 0;
	}

	if (sz1 == nullptr) sz1 = _T("");
	if (sz2 == nullptr) sz2 = _T("");

	for (;; sz1++, sz2++)
	{
		const char c1 = *sz1;
		const char c2 = *sz2;

		// check for binary equality first
		if (c1 == c2)
		{
			if (c1 == 0)
			{
				return 0; // We have reached the end of both strings. No difference found.
			}
		}
		else
		{
			if (c1 == 0 || c2 == 0)
			{
				return (c1-c2); // We have reached the end of one String
			}

			// IMPORTANT: this needs to be upper case to match the behavior of the operating system.
			// See http://msdn.microsoft.com/library/default.asp?url=/library/en-us/dndotnet/html/StringsinNET20.asp
			const char u1 = ToUpperInvariant(c1);
			const char u2 = ToUpperInvariant(c2);
			if (u1 != u2)
			{
				return (u1-u2); // strings are different
			}
		}
	}
}

bool str2Clipboard(const String&str2cpy, Window* hwnd)
{
	size_t len2Allocate = (str2cpy.size() + 1) * sizeof(char);
	HGLOBAL hglbCopy = ::GlobalAlloc(GMEM_MOVEABLE, len2Allocate);
	if (hglbCopy == nullptr)
	{
		return false;
	}
	if (!::OpenClipboard(hwnd))
	{
		::GlobalFree(hglbCopy);
		::CloseClipboard();
		return false;
	}
	if (!::EmptyClipboard())
	{
		::GlobalFree(hglbCopy);
		::CloseClipboard();
		return false;
	}
	// Lock the handle and copy the text to the buffer.
	char *pStr = (char *)::GlobalLock(hglbCopy);
	if (pStr == nullptr)
	{
		::GlobalUnlock(hglbCopy);
		::GlobalFree(hglbCopy);
		::CloseClipboard();
		return false;
	}
	_tcscpy_s(pStr, len2Allocate / sizeof(char), str2cpy.Begin());
	::GlobalUnlock(hglbCopy);
	// Place the handle on the clipboard.
	unsigned int clipBoardFormat = CF_UNICODETEXT;
	if (::SetClipboardData(clipBoardFormat, hglbCopy) == nullptr)
	{
		::GlobalFree(hglbCopy);
		::CloseClipboard();
		return false;
	}
	if (!::CloseClipboard())
	{
		return false;
	}
	return true;
}

bool buf2Clipborad(const std::vector<SciBuffer*>& buffers, bool isFullPath, Window* hwnd)
{
	const String crlf = _T("\r\n");
	String selection;
	for (auto&& buf : buffers)
	{
		if (buf)
		{
			const char* fileName = isFullPath ? buf->getFullPathName() : buf->getFileName();
			if (fileName)
				selection += fileName;
		}
		if (!selection.IsEmpty() && !endsWith(selection, crlf))
			selection += crlf;
	}
	if (!selection.IsEmpty())
		return str2Clipboard(selection, hwnd);
	return false;
}

bool matchInList(const char *fileName, const Vector<String> & patterns)
{
	bool is_matched = false;
	for (size_t i = 0, len = patterns.size(); i < len; ++i)
	{
		if (patterns[i].GetLength() > 1 && patterns[i][0] == '!')
		{
			if (PathMatchSpec(fileName, patterns[i].Begin() + 1))
				return false;

			continue;
		}

		if (PathMatchSpec(fileName, patterns[i].Begin()))
			is_matched = true;
	}
	return is_matched;
}

bool matchInExcludeDirList(const char* dirName, const Vector<String>& patterns, size_t level)
{
	for (size_t i = 0, len = patterns.size(); i < len; ++i)
	{
		size_t patterLen = patterns[i].GetLength();

		if (patterLen > 3 && patterns[i][0] == '!' && patterns[i][1] == '+' && patterns[i][2] == '\\') // check for !+\folderPattern: for all levels - search this pattern recursively
		{
			if (PathMatchSpec(dirName, patterns[i].Begin() + 3))
				return true;
		}
		else if (patterLen > 2 && patterns[i][0] == '!' && patterns[i][1] == '\\') // check for !\folderPattern: exclusive pattern for only the 1st level
		{
			if (level == 1)
				if (PathMatchSpec(dirName, patterns[i].Begin() + 2))
					return true;
		}
	}
	return false;
}

bool allPatternsAreExclusion(const Vector<String> patterns)
{
	bool oneInclusionPatternFound = false;
	for (size_t i = 0, len = patterns.size(); i < len; ++i)
	{
		if (patterns[i][0] != '!')
		{
			oneInclusionPatternFound = true;
			break;
		}
	}
	return !oneInclusionPatternFound;
}

String GetLastErrorAsString(dword errorCode)
{
	String errorMsg(_T(""));
	// Get the error message, if any.
	// If both error codes (passed error n GetLastError) are 0, then return empty
	if (errorCode == 0)
		errorCode = GetLastError();
	if (errorCode == 0)
		return errorMsg; //No error message has been recorded

	LPWSTR messageBuffer = nullptr;
	FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, nullptr);

	errorMsg += messageBuffer;

	//Free the buffer.
	LocalFree(messageBuffer);

	return errorMsg;
}

Window* CreateToolTip(int toolID, Window* hDlg, Window& hInst, const PTSTR pszText, bool isRTL)
{
	if (!toolID || !hDlg || !pszText)
	{
		return nullptr;
	}

	// Get the window of the tool.
	Window* hwndTool = GetDlgItem(hDlg, toolID);
	if (!hwndTool)
	{
		return nullptr;
	}

	// Create the tooltip. g_hInst is the global instance handle.
	Window* hwndTip = CreateWindowEx(isRTL ? WS_EX_LAYOUTRTL : 0, TOOLTIPS_CLASS, nullptr,
		WS_POPUP | TTS_ALWAYSTIP | TTS_BALLOON,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		hDlg, nullptr,
		hInst, nullptr);

	if (!hwndTip)
	{
		return nullptr;
	}

	NppDarkMode::setDarkTooltips(hwndTip, NppDarkMode::ToolTipsType::tooltip);

	// Associate the tooltip with the tool.
	TOOLINFO toolInfo = {};
	toolInfo.cbSize = sizeof(toolInfo);
	toolInfo.hwnd = hDlg;
	toolInfo.uFlags = TTF_IDISWindow& | TTF_SUBCLASS;
	toolInfo.uId = (UINT_PTR)hwndTool;
	toolInfo.lpszText = pszText;
	if (!SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo))
	{
		DestroyWindow(hwndTip);
		return nullptr;
	}

	SendMessage(hwndTip, TTM_ACTIVATE, TRUE, 0);
	SendMessage(hwndTip, TTM_SETMAXTIPWIDTH, 0, 200);
	// Make tip stay 15 seconds
	SendMessage(hwndTip, TTM_SETDELAYTIME, TTDT_AUTOPOP, MAKELPARAM((15000), (0)));

	return hwndTip;
}

Window* CreateToolTipRect(int toolID, Window* hWnd, Window& hInst, const PTSTR pszText, const Rect rc)
{
	if (!toolID || !hWnd || !pszText)
	{
		return nullptr;
	}

	// Create the tooltip. g_hInst is the global instance handle.
	Window* hwndTip = CreateWindowEx(0, TOOLTIPS_CLASS, nullptr,
		WS_POPUP | TTS_ALWAYSTIP | TTS_BALLOON,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		hWnd, nullptr,
		hInst, nullptr);

	if (!hwndTip)
	{
		return nullptr;
	}

	// Associate the tooltip with the tool.
	TOOLINFO toolInfo = {};
	toolInfo.cbSize = sizeof(toolInfo);
	toolInfo.hwnd = hWnd;
	toolInfo.uFlags = TTF_SUBCLASS;
	toolInfo.uId = toolID;
	toolInfo.lpszText = pszText;
	toolInfo.rect = rc;
	if (!SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo))
	{
		DestroyWindow(hwndTip);
		return nullptr;
	}

	SendMessage(hwndTip, TTM_ACTIVATE, TRUE, 0);
	SendMessage(hwndTip, TTM_SETMAXTIPWIDTH, 0, 200);
	// Make tip stay 15 seconds
	SendMessage(hwndTip, TTM_SETDELAYTIME, TTDT_AUTOPOP, MAKELPARAM((15000), (0)));

	return hwndTip;
}

bool isCertificateValidated(const String& fullFilePath, const String& subjectName2check)
{
	bool isOK = false;
	HCERTSTORE hStore = nullptr;
	HCRYPTMSG hMsg = nullptr;
	PCCERT_CONTEXT pCertContext = nullptr;
	BOOL result = FALSE;
	dword dwEncoding = 0;
	dword dwContentType = 0;
	dword dwFormatType = 0;
	PCMSG_SIGNER_INFO pSignerInfo = nullptr;
	dword dwSignerInfo = 0;
	CERT_INFO CertInfo;
	LPTSTR szName = nullptr;

	String subjectName;

	try {
		// Get message handle and store handle from the signed file.
		result = CryptQueryObject(CERT_QUERY_OBJECT_FILE,
			fullFilePath.Begin(),
			CERT_QUERY_CONTENT_FLAG_PKCS7_SIGNED_EMBED,
			CERT_QUERY_FORMAT_FLAG_BINARY,
			0,
			&dwEncoding,
			&dwContentType,
			&dwFormatType,
			&hStore,
			&hMsg,
			nullptr);

		if (!result)
		{
			String errorMessage = TEXT("Check certificate of ") + fullFilePath + TEXT(" : ");
			errorMessage += GetLastErrorAsString(GetLastError());
			throw errorMessage;
		}

		// Get signer information size.
		result = CryptMsgGetParam(hMsg, CMSG_SIGNER_INFO_PARAM, 0, nullptr, &dwSignerInfo);
		if (!result)
		{
			String errorMessage = TEXT("CryptMsgGetParam first call: ");
			errorMessage += GetLastErrorAsString(GetLastError());
			throw errorMessage;
		}

		// Allocate memory for signer information.
		pSignerInfo = (PCMSG_SIGNER_INFO)LocalAlloc(LPTR, dwSignerInfo);
		if (!pSignerInfo)
		{
			String errorMessage = TEXT("CryptMsgGetParam memory allocation problem: ");
			errorMessage += GetLastErrorAsString(GetLastError());
			throw errorMessage;
		}

		// Get Signer Information.
		result = CryptMsgGetParam(hMsg, CMSG_SIGNER_INFO_PARAM, 0, (PVOID)pSignerInfo, &dwSignerInfo);
		if (!result)
		{
			String errorMessage = TEXT("CryptMsgGetParam: ");
			errorMessage += GetLastErrorAsString(GetLastError());
			throw errorMessage;
		}

		// Search for the signer certificate in the temporary
		// certificate store.
		CertInfo.Issuer = pSignerInfo->Issuer;
		CertInfo.SerialNumber = pSignerInfo->SerialNumber;

		pCertContext = CertFindCertificateInStore(hStore, X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, 0, CERT_FIND_SUBJECT_CERT, (PVOID)&CertInfo, nullptr);
		if (!pCertContext)
		{
			String errorMessage = TEXT("Certificate context: ");
			errorMessage += GetLastErrorAsString(GetLastError());
			throw errorMessage;
		}

		dword dwData;

		// Get Subject name size.
		dwData = CertGetNameString(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, nullptr, nullptr, 0);
		if (dwData <= 1)
		{
			throw String(TEXT("Certificate checking error: getting data size problem."));
		}

		// Allocate memory for subject name.
		szName = (LPTSTR)LocalAlloc(LPTR, dwData * sizeof(char));
		if (!szName)
		{
			throw String(TEXT("Certificate checking error: memory allocation problem."));
		}

		// Get subject name.
		if (CertGetNameString(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, nullptr, szName, dwData) <= 1)
		{
			throw String(TEXT("Cannot get certificate info."));
		}

		// check Subject name.
		subjectName = szName;
		if (subjectName != subjectName2check)
		{
			throw String(TEXT("Certificate checking error: the certificate is not matched."));
		}

		isOK = true;
	}
	catch (const char* s)
	{
		// display error message
		MessageBox(nullptr, s.Begin(), TEXT("Certificate checking"), MB_OK);
	}
	catch (...)
	{
		// Unknown error
		String errorMessage = TEXT("Unknown exception occured. ");
		errorMessage += GetLastErrorAsString(GetLastError());
		MessageBox(nullptr, errorMessage.Begin(), TEXT("Certificate checking"), MB_OK);
	}

	// Clean up.
	if (pSignerInfo != nullptr) LocalFree(pSignerInfo);
	if (pCertContext != nullptr) CertFreeCertificateContext(pCertContext);
	if (hStore != nullptr) CertCloseStore(hStore, 0);
	if (hMsg != nullptr) CryptMsgClose(hMsg);
	if (szName != nullptr) LocalFree(szName);

	return isOK;
}

bool isAssoCommandExisting(LPCTSTR FullPathName)
{
	bool isAssoCommandExisting = false;

	bool isFileExisting = PathFileExists(FullPathName) != FALSE;

	if (isFileExisting)
	{
		PTSTR ext = PathFindExtension(FullPathName);

		HRESULT hres;
		wchar_t buffer[MAX_PATH] = TEXT("");
		dword bufferLen = MAX_PATH;

		// check if association exist
		hres = AssocQueryString(ASSOCF_VERIFY|ASSOCF_INIT_IGNOREUNKNOWN, ASSOCSTR_COMMAND, ext, nullptr, buffer, &bufferLen);

        isAssoCommandExisting = (hres == S_OK)                  // check if association exist and no error
			&& (buffer != nullptr)                                 // check if buffer is not nullptr
			&& (wcsstr(buffer, TEXT("notepad++.exe")) == nullptr); // check association with notepad++

	}
	return isAssoCommandExisting;
}

std::wstring s2ws(const String& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX("Error in N++ String conversion s2ws!", L"Error in N++ String conversion s2ws!");

	return converterX.from_bytes(str);
}

String ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX("Error in N++ String conversion ws2s!", L"Error in N++ String conversion ws2s!");

	return converterX.to_bytes(wstr);
}

bool deleteFileOrFolder(const char* f2delete)
{
	auto len = f2delete.GetLength();
	char* actionFolder = new char[len + 2];
	wcscpy_s(actionFolder, len + 2, f2delete.Begin());
	actionFolder[len] = 0;
	actionFolder[len + 1] = 0;

	SHFILEOPSTRUCT fileOpStruct = {};
	fileOpStruct.hwnd = nullptr;
	fileOpStruct.pFrom = actionFolder;
	fileOpStruct.pTo = nullptr;
	fileOpStruct.wFunc = FO_DELETE;
	fileOpStruct.fFlags = FOF_NOCONFIRMATION | FOF_SILENT | FOF_ALLOWUNDO;
	fileOpStruct.fAnyOperationsAborted = false;
	fileOpStruct.hNameMappings = nullptr;
	fileOpStruct.lpszProgressTitle = nullptr;

	int res = SHFileOperation(&fileOpStruct);

	delete[] actionFolder;
	return (res == 0);
}

// Get a vector of full file paths in a given folder. File extension type filter should be *.*, *.xml, *.dll... according the type of file you want to get.
void getFilesInFolder(Vector<String>& files, const char* extTypeFilter, const char* inFolder)
{
	String filter = inFolder;
	pathAppend(filter, extTypeFilter);

	WIN32_FIND_DATA foundData;
	void* hFindFile = ::FindFirstFile(filter.Begin(), &foundData);

	if (hFindFile != INVALID_HANDLE_VALUE && !(foundData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		String foundFullPath = inFolder;
		pathAppend(foundFullPath, foundData.cFileName);
		files.push_back(foundFullPath);

		while (::FindNextFile(hFindFile, &foundData))
		{
			String foundFullPath2 = inFolder;
			pathAppend(foundFullPath2, foundData.cFileName);
			files.push_back(foundFullPath2);
		}
	}
	::FindClose(hFindFile);
}

void trim(String& str)
{
	// remove any leading or trailing spaces from str

	String::size_type pos = str.find_last_not_of(' ');

	if (pos != String::npos)
	{
		str.erase(pos + 1);
		pos = str.find_first_not_of(' ');
		if (pos != String::npos) str.erase(0, pos);
	}
	else str.erase(str.begin(), str.end());
}

bool endsWith(const char* s, const char* suffix)
{
#if defined(_MSVC_LANG) && (_MSVC_LANG > 201402L)
#error Replace this function with basic_string::ends_with
#endif
	size_t pos = s.find(suffix);
	return pos != s.npos && ((s.GetLength() - pos) == suffix.GetLength());
}

int nbDigitsFromNbLines(size_t nbLines)
{
	int nbDigits = 0; // minimum number of digit should be 4
	if (nbLines < 10) nbDigits = 1;
	else if (nbLines < 100) nbDigits = 2;
	else if (nbLines < 1000) nbDigits = 3;
	else if (nbLines < 10000) nbDigits = 4;
	else if (nbLines < 100000) nbDigits = 5;
	else if (nbLines < 1000000) nbDigits = 6;
	else // rare case
	{
		nbDigits = 7;
		nbLines /= 1000000;

		while (nbLines)
		{
			nbLines /= 10;
			++nbDigits;
		}
	}
	return nbDigits;
}

namespace
{
	constexpr char timeFmtEscapeChar = 0x1;
	constexpr char middayFormat[] = _T("tt");

	// Returns AM/PM String defined by the system locale for the specified time.
	// This String may be empty or customized.
	String getMiddayString(const char* localeName, const SYSTEMTIME& st)
	{
		String midday;
		midday.resize(MAX_PATH);
		int ret = GetTimeFormatEx(localeName, 0, &st, middayFormat, &midday[0], static_cast<int>(midday.size()));
		if (ret > 0)
			midday.resize(ret - 1); // Remove the null-terminator.
		else
			midday.clear();
		return midday;
	}

	// Replaces conflicting time format specifiers by a special character.
	bool escapeTimeFormat(String& format)
	{
		bool modified = false;
		for (auto& ch : format)
		{
			if (ch == middayFormat[0])
			{
				ch = timeFmtEscapeChar;
				modified = true;
			}
		}
		return modified;
	}

	// Replaces special time format characters by actual AM/PM String.
	void unescapeTimeFormat(String& format, const char* midday)
	{
		if (midday.IsEmpty())
		{
			auto it = std::remove(format.begin(), format.end(), timeFmtEscapeChar);
			if (it != format.end())
				format.erase(it, format.end());
		}
		else
		{
			size_t i = 0;
			while ((i = format.find(timeFmtEscapeChar, i)) != String::npos)
			{
				if (i + 1 < format.size() && format[i + 1] == timeFmtEscapeChar)
				{
					// 'tt' => AM/PM
					format.erase(i, std::size(middayFormat) - 1);
					format.insert(i, midday);
				}
				else
				{
					// 't' => A/P
					format[i] = midday[0];
				}
			}
		}
	}
}

String getDateTimeStrFrom(const char* dateTimeFormat, const SYSTEMTIME& st)
{
	const char* localeName = LOCALE_NAME_USER_DEFAULT;
	const dword flags = 0;

	constexpr int bufferSize = MAX_PATH;
	char buffer[bufferSize] = {};
	int ret = 0;


	// 1. Escape 'tt' that means AM/PM or 't' that means A/P.
	// This is needed to avoid conflict with 'M' date format that stands for month.
	String newFormat = dateTimeFormat;
	const bool hasMiddayFormat = escapeTimeFormat(newFormat);

	// 2. Format the time (h/m/s/t/H).
	ret = GetTimeFormatEx(localeName, flags, &st, newFormat.Begin(), buffer, bufferSize);
	if (ret != 0)
	{
		// 3. Format the date (d/y/g/M).
		// Now use the buffer as a format String to process the format specifiers not recognized by GetTimeFormatEx().
		ret = GetDateFormatEx(localeName, flags, &st, buffer, buffer, bufferSize, nullptr);
	}

	if (ret != 0)
	{
		if (hasMiddayFormat)
		{
			// 4. Now format only the AM/PM String.
			const String midday = getMiddayString(localeName, st);
			String result = buffer;
			unescapeTimeFormat(result, midday);
			return result;
		}
		return buffer;
	}

	return {};
}

// Don't forget to use DeleteObject(createdFont) before leaving the program
HFONT createFont(const char* fontName, int fontSize, bool isBold, Window* hDestParent)
{
	HDC hdc = GetDC(hDestParent);

	LOGFONT logFont = {};
	logFont.lfHeight = -MulDiv(fontSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	if (isBold)
		logFont.lfWeight = FW_BOLD;

	_tcscpy_s(logFont.lfFaceName, fontName);

	HFONT newFont = CreateFontIndirect(&logFont);

	ReleaseDC(hDestParent, hdc);

	return newFont;
}

Version::Version(const char* versionStr)
{
	try {
		auto ss = tokenizeString(versionStr, '.');

		if (ss.size() > 4)
		{
			std::wstring msg(L"\"");
			msg += versionStr;
			msg += L"\"";
			msg += TEXT(": Version parts are more than 4. The String to parse is not a valid version format. Let's make it default value in catch block.");
			throw msg;
		}

		int i = 0;
		std::vector<unsigned long*> v = { &_major, &_minor, &_patch, &_build };
		for (const auto& s : ss)
		{
			if (!isNumber(s))
			{
				std::wstring msg(L"\"");
				msg += versionStr;
				msg += L"\"";
				msg += TEXT(": One of version character is not number. The String to parse is not a valid version format. Let's make it default value in catch block.");
				throw msg;
			}
			*(v[i]) = std::stoi(s);

			++i;
		}
	}
#ifdef DEBUG
	catch (const std::wstring& s)
	{
		_major = 0;
		_minor = 0;
		_patch = 0;
		_build = 0;

		throw s;
	}
#endif
	catch (...)
	{
		_major = 0;
		_minor = 0;
		_patch = 0;
		_build = 0;
#ifdef DEBUG
		throw std::wstring(TEXT("Unknown exception from \"Version::Version(const char* versionStr)\""));
#endif
	}
}


void Version::setVersionFrom(const char* filePath)
{
	if (!filePath.IsEmpty() && ::PathFileExists(filePath.Begin()))
	{
		dword uselessArg = 0; // this variable is for passing the ignored argument to the functions
		dword bufferSize = ::GetFileVersionInfoSize(filePath.Begin(), &uselessArg);

		if (bufferSize <= 0)
			return;

		unsigned char* buffer = new unsigned char[bufferSize];
		::GetFileVersionInfo(filePath.Begin(), uselessArg, bufferSize, buffer);

		VS_FIXEDFILEINFO* lpFileInfo = nullptr;
		UINT cbFileInfo = 0;
		VerQueryValue(buffer, TEXT("\\"), reinterpret_cast<LPVOID*>(&lpFileInfo), &cbFileInfo);
		if (cbFileInfo)
		{
			_major = (lpFileInfo->dwFileVersionMS & 0xFFFF0000) >> 16;
			_minor = lpFileInfo->dwFileVersionMS & 0x0000FFFF;
			_patch = (lpFileInfo->dwFileVersionLS & 0xFFFF0000) >> 16;
			_build = lpFileInfo->dwFileVersionLS & 0x0000FFFF;
		}
		delete[] buffer;
	}
}

String Version::toString()
{
	if (_build == 0 && _patch == 0 && _minor == 0 && _major == 0) // ""
	{
		return TEXT("");
	}
	else if (_build == 0 && _patch == 0 && _minor == 0) // "major"
	{
		return std::to_wstring(_major);
	}
	else if (_build == 0 && _patch == 0) // "major.minor"
	{
		std::wstring v = std::to_wstring(_major);
		v += TEXT(".");
		v += std::to_wstring(_minor);
		return v;
	}
	else if (_build == 0) // "major.minor.patch"
	{
		std::wstring v = std::to_wstring(_major);
		v += TEXT(".");
		v += std::to_wstring(_minor);
		v += TEXT(".");
		v += std::to_wstring(_patch);
		return v;
	}

	// "major.minor.patch.build"
	std::wstring ver = std::to_wstring(_major);
	ver += TEXT(".");
	ver += std::to_wstring(_minor);
	ver += TEXT(".");
	ver += std::to_wstring(_patch);
	ver += TEXT(".");
	ver += std::to_wstring(_build);

	return ver;
}

int Version::compareTo(const Version& v2c) const
{
	if (_major > v2c._major)
		return 1;
	else if (_major < v2c._major)
		return -1;
	else // (_major == v2c._major)
	{
		if (_minor > v2c._minor)
			return 1;
		else if (_minor < v2c._minor)
			return -1;
		else // (_minor == v2c._minor)
		{
			if (_patch > v2c._patch)
				return 1;
			else if (_patch < v2c._patch)
				return -1;
			else // (_patch == v2c._patch)
			{
				if (_build > v2c._build)
					return 1;
				else if (_build < v2c._build)
					return -1;
				else // (_build == v2c._build)
				{
					return 0;
				}
			}
		}
	}
}

bool Version::isCompatibleTo(const Version& from, const Version& to) const
{
	// This method determinates if Version object is in between "from" version and "to" version, it's useful for testing compatibility of application.
	// test in versions <from, to> example:
	// 1. <0.0.0.0, 0.0.0.0>: both from to versions are empty, so it's
	// 2. <6.9, 6.9>: plugin is compatible to only v6.9
	// 3. <4.2, 6.6.6>: from v4.2 (included) to v6.6.6 (included)
	// 4. <0.0.0.0, 8.2.1>: all version until v8.2.1 (included)
	// 5. <8.3, 0.0.0.0>: from v8.3 (included) to the latest verrsion

	if (empty()) // if this version is empty, then no compatible to all version
		return false;

	if (from.IsEmpty() && to.IsEmpty()) // both versions "from" and "to" are empty: it's considered compatible, whatever this version is (match to 1)
	{
		return true;
	}

	if (from <= *this && to >= *this) // from_ver <= this_ver <= to_ver (match to 2, 3 and 4)
	{
		return true;
	}

	if (from <= *this && to.IsEmpty()) // from_ver <= this_ver (match to 5)
	{
		return true;
	}

	return false;
}
