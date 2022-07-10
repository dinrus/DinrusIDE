#include "Java.h"

namespace РНЦП {

Ткст Java::GetDelimiter()
{
#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64)
	return ";";
#else
	return ":";
#endif
}

}
