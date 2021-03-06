#ifndef _plugin_pcx_pcxhdr_h_
#define _plugin_pcx_pcxhdr_h_

#ifdef COMPILER_MSC
#pragma pack(push, 1)
#endif

struct PCXRGB {
	byte r, g, b; 
}
#ifdef COMPILER_GCC
__attribute__((packed))
#endif
;

struct PCXHeader {
	byte    manufacturer;  // 10 == ZSoft
	byte    version;       // 0 = Paintbrush 2.5
	                       // 2 = 2.8 with palette
	                       // 3 = 2.8 without palette
	                       // 4 = PC Paintbrush for Windows
	                       // 5 = Version 3.0+ and Publishers Paintbrush */
	byte    encode;        // 1 = PCX RLE
	byte    bitsPerPixel;  // 1, 2, 4, or 8
	short   minX;
	short   minY;
	short   maxX;
	short   maxY;
	short   horzDpi;
	short   vertDpi;
	PCXRGB  palette[16];
	byte    junk;
	byte    planes;
	short   bytesPerLine;
	short   paltype;
	short   hScreenSize;
	short   vScreenSize;
	byte	filler[54];

	void    SwapEndian()
	{
#ifdef CPU_BIG_ENDIAN
		biSize = РНЦП::SwapEndian(biSize);
		biWidth = РНЦП::SwapEndian(biWidth);
		biHeight = РНЦП::SwapEndian(biHeight);
		biPlanes = РНЦП::SwapEndian(biPlanes);
		biBitCount = РНЦП::SwapEndian(biBitCount);
		biCompression = РНЦП::SwapEndian(biCompression);
		biSizeImage = РНЦП::SwapEndian(biSizeImage);
		biXPelsPerMeter = РНЦП::SwapEndian(biXPelsPerMeter);
		biYPelsPerMeter = РНЦП::SwapEndian(biYPelsPerMeter);
		biClrUsed = РНЦП::SwapEndian(biClrUsed);
		biClrImportant = РНЦП::SwapEndian(biClrImportant);
#endif
	}

}
#ifdef COMPILER_GCC
__attribute__((packed))
#endif
;

#ifdef COMPILER_MSC
#pragma pack(pop)
#endif

#endif
