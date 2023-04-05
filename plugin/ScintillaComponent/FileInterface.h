#pragma once

#include <Core/Core.h>

//#include <windows.h>
//#include <string>
//#include <tchar.h>
//#include <cstdint>


class SciFile final
{
public:
	SciFile(const char *fname);
	SciFile(const wchar *fname);

	SciFile() = delete;
	SciFile(const SciFile&) = delete;
	SciFile& operator=(const SciFile&) = delete;

	~SciFile() {
		close();
	};

	bool isOpened() {
		return (_hFile != INVALID_HANDLE_VALUE);
	};

	void close();
	//int_fast64_t getSize();
	//unsigned long read(void *rbuf, unsigned long buf_size);

	bool write(const void *wbuf, unsigned long buf_size);

	bool writeStr(const String& str) {
		return write(str.c_str(), static_cast<unsigned long>(str.GetLength()));
	};

private:
	void*	_hFile		{INVALID_HANDLE_VALUE};
	bool	_written	{false};
	String _path;
	
#ifdef PLATFORM_WIN32
	const Upp::dword _accessParam  { GENERIC_READ | GENERIC_WRITE };
	const Upp::dword _shareParam   { FILE_SHARE_READ | FILE_SHARE_WRITE };
	const Upp::dword _dispParam    { CREATE_ALWAYS };
	const Upp::dword _attribParam  { FILE_ATTRIBUTE_NORMAL };
#endif
};
