#include "zip.h"

namespace РНЦП {

bool FileUnZip::создай(const char *имя)
{
	if(!zip.открой(имя))
		return false;
	UnZip::создай(zip);
	return true;
}

void MemUnZip::создай(const void *ptr, int count)
{
	zip.создай(ptr, count);
	UnZip::создай(zip);
}

void StringUnZip::создай(const Ткст& s)
{
	zip.открой(s);
	UnZip::создай(zip);
}

}
