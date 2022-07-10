#include "ppm.h"

namespace РНЦП {

int PPMEncoder::GetPaletteCount()
{
	return 0;
}

void PPMEncoder::старт(Размер sz)
{
	size = sz;
	формат.Set24be(0xff0000, 0x00ff00, 0x0000ff);
	GetStream() << "P6\n" << какТкст(sz.cx) << ' ' << какТкст(sz.cy) << "\n" << 255 << "\n";
}

void PPMEncoder::WriteLineRaw(const byte *data)
{
	GetStream().помести(data, size.cx * 3);
}

}