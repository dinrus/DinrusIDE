#include <locale>
#include <codecvt>
#include "FileInterface.h"
#include "Parameters.h"

SciFile::SciFile(const char *fname)
{
	if (fname)
	{
		_path = fname;
		
	#ifdef PLATFORM_WIN32
		_hFile = ::CreateFileA(fname, _accessParam, _shareParam, nullptr, _dispParam, _attribParam, nullptr);
	#endif
	}
}


SciFile::SciFile(const wchar *fname)
{
	if (fname)
	{
		std::wstring fn = fname;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar>> converter;
		_path = converter.to_bytes(fn);
		
		#ifdef PLATFORM_WIN32
		_hFile = ::CreateFileW(fname, _accessParam, _shareParam, nullptr, _dispParam, _attribParam, nullptr);
		#endif

		NppParameters& nppParam = NppParameters::getInstance();
		if (nppParam.isQueryEndSessionStarted() && nppParam.doNppLogNulContentCorruptionIssue())
		{
			String issueFn = nppLogNulContentCorruptionIssue;
			issueFn += ".log";
			String nppIssueLog = nppParam.getUserPath();
			pathAppend(nppIssueLog, issueFn);

			String msg = _path;
			msg += " открыт.";
			writeLog(nppIssueLog.Begin(), msg.Begin());
		}
	}
}

void SciFile::close()
{
	if (isOpened())
	{
		dword flushError = NOERROR;
		if (_written)
		{
			#ifdef PLATFORM_WIN32
			if (!::FlushFileBuffers(_hFile))
				flushError = ::GetLastError();
			#endif
		}
		
		#ifdef PLATFORM_WIN32
		::CloseHandle(_hFile);

		_hFile = INVALID_HANDLE_VALUE;
		#endif


		NppParameters& nppParam = NppParameters::getInstance();
		if (nppParam.isQueryEndSessionStarted() && nppParam.doNppLogNulContentCorruptionIssue())
		{
			String issueFn = nppLogNulContentCorruptionIssue;
			issueFn += ".log";
			String nppIssueLog = nppParam.getUserPath();
			pathAppend(nppIssueLog, issueFn);


			String msg;
			if (flushError != NOERROR)
			{
				#ifdef PLATFORM_WIN32
				LPSTR messageBuffer = nullptr;
				FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					nullptr, flushError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, nullptr);
				msg += messageBuffer;

				//Free the buffer.
				LocalFree(messageBuffer);
				msg += "\n";
				#endif
			}
			msg += _path;
			msg += " is closed.";
			writeLog(nppIssueLog.Begin(), msg.Begin());
		}
	}
}

/*
int_fast64_t SciFile::getSize()
{
	LARGE_INTEGER r;
	r.QuadPart = -1;

	if (isOpened())
		::GetFileSizeEx(_hFile, &r);

	return static_cast<int_fast64_t>(r.QuadPart);
}

unsigned long SciFile::read(void *rbuf, unsigned long buf_size)
{
	if (!isOpened() || (rbuf == nullptr) || (buf_size == 0))
		return 0;

	dword bytes_read = 0;

	if (::ReadFile(_hFile, rbuf, buf_size, &bytes_read, nullptr) == FALSE)
		return 0;

	return bytes_read;
}
*/

bool SciFile::write(const void *wbuf, unsigned long buf_size)
{
	if (!isOpened() || (wbuf == nullptr))
		return false;

	dword bytes_written = 0;

	NppParameters& nppParam = NppParameters::getInstance();
	if (::WriteFile(_hFile, wbuf, buf_size, &bytes_written, nullptr) == FALSE)
	{
		if (nppParam.isQueryEndSessionStarted() && nppParam.doNppLogNulContentCorruptionIssue())
		{
			String issueFn = nppLogNulContentCorruptionIssue;
			issueFn += TEXT(".log");
			String nppIssueLog = nppParam.getUserPath();
			pathAppend(nppIssueLog, issueFn);

			String msg = _path;
			msg += " written failed: ";
			std::wstring lastErrorMsg = GetLastErrorAsString(::GetLastError());
			std::wstring_convert<std::codecvt_utf8_utf16<wchar>> converter;
			msg += converter.to_bytes(lastErrorMsg);
			writeLog(nppIssueLog.Begin(), msg.Begin());
		}

		return false;
	}
	else
	{
		if (nppParam.isQueryEndSessionStarted() && nppParam.doNppLogNulContentCorruptionIssue())
		{
			String issueFn = nppLogNulContentCorruptionIssue;
			issueFn += TEXT(".log");
			String nppIssueLog = nppParam.getUserPath();
			pathAppend(nppIssueLog, issueFn);

			String msg = _path;
			msg += "  ";
			msg += std::to_string(bytes_written);
			msg += "/";
			msg += std::to_string(buf_size);
			msg += " bytes are written.";
			writeLog(nppIssueLog.Begin(), msg.Begin());
		}
	}

	if (!_written)
		_written = true;

	return (bytes_written == buf_size);
}

