// This file is part of Notepad++ project
// Copyright (C)2021 Pavel Nedev (pg.nedev@gmail.com)

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

#include <locale>
#include <codecvt>
#include <PowerEditor/MISC/Common/FileInterface.h>
#include <PowerEditor/Parameters.h>

Win32_IO_File::Win32_IO_File(const char *fname)
{
	if (fname)
	{
		_path = fname;
		_hFile = ::CreateFileA(fname, _accessParam, _shareParam, nullptr, _dispParam, _attribParam, nullptr);
	}
}


Win32_IO_File::Win32_IO_File(const wchar_t *fname)
{
	if (fname)
	{
		std::wstring fn = fname;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		_path = converter.to_bytes(fn);
		_hFile = ::CreateFileW(fname, _accessParam, _shareParam, nullptr, _dispParam, _attribParam, nullptr);

		NppParameters& nppParam = NppParameters::getInstance();
		if (nppParam.isQueryEndSessionStarted() && nppParam.doNppLogNulContentCorruptionIssue())
		{
			String issueFn = nppLogNulContentCorruptionIssue;
			issueFn += TEXT(".log");
			String nppIssueLog = nppParam.getUserPath();
			pathAppend(nppIssueLog, issueFn);

			String msg = _path;
			msg += " is opened.";
			writeLog(nppIssueLog.Begin(), msg.Begin());
		}
	}
}

void Win32_IO_File::close()
{
	if (isOpened())
	{
		dword flushError = NOERROR;
		if (_written)
		{
			if (!::FlushFileBuffers(_hFile))
				flushError = ::GetLastError();
		}
		::CloseHandle(_hFile);

		_hFile = INVALID_HANDLE_VALUE;


		NppParameters& nppParam = NppParameters::getInstance();
		if (nppParam.isQueryEndSessionStarted() && nppParam.doNppLogNulContentCorruptionIssue())
		{
			String issueFn = nppLogNulContentCorruptionIssue;
			issueFn += TEXT(".log");
			String nppIssueLog = nppParam.getUserPath();
			pathAppend(nppIssueLog, issueFn);


			String msg;
			if (flushError != NOERROR)
			{
				LPSTR messageBuffer = nullptr;
				FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					nullptr, flushError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, nullptr);
				msg += messageBuffer;

				//Free the buffer.
				LocalFree(messageBuffer);
				msg += "\n";
			}
			msg += _path;
			msg += " is closed.";
			writeLog(nppIssueLog.Begin(), msg.Begin());
		}
	}
}

/*
int_fast64_t Win32_IO_File::getSize()
{
	LARGE_INTEGER r;
	r.QuadPart = -1;

	if (isOpened())
		::GetFileSizeEx(_hFile, &r);

	return static_cast<int_fast64_t>(r.QuadPart);
}

unsigned long Win32_IO_File::read(void *rbuf, unsigned long buf_size)
{
	if (!isOpened() || (rbuf == nullptr) || (buf_size == 0))
		return 0;

	dword bytes_read = 0;

	if (::ReadFile(_hFile, rbuf, buf_size, &bytes_read, nullptr) == FALSE)
		return 0;

	return bytes_read;
}
*/

bool Win32_IO_File::write(const void *wbuf, unsigned long buf_size)
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
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
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

