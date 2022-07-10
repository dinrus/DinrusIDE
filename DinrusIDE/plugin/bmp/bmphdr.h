#ifndef _nImage__bmp_h_
#define _nImage__bmp_h_

#if defined(COMPILER_MSC) || defined(COMPILER_GCC)
#pragma pack(push, 1)
#endif

struct BMP_FILEHEADER {
	word    bfType;
	dword   bfSize;
	word    bfReserved1;
	word    bfReserved2;
	dword   bfOffBits;

	void    SwapEndian()
	{
#ifdef CPU_BIG_ENDIAN
		bfType = РНЦП::SwapEndian(bfType);
		bfSize = РНЦП::SwapEndian(bfSize);
		bfOffBits = РНЦП::SwapEndian(bfOffBits);
#endif
	}
}
#ifdef COMPILER_GCC
__attribute__((packed))
#endif
;

struct BMP_INFOHEADER
{
	dword      biSize;
	int32      biWidth;
	int32      biHeight;
	word       biPlanes;
	word       biBitCount;
	dword      biCompression;
	dword      biSizeImage;
	int32      biXPelsPerMeter;
	int32      biYPelsPerMeter;
	dword      biClrUsed;
	dword      biClrImportant;

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

struct BMP_RGB
{
    byte    rgbBlue;
    byte    rgbGreen;
    byte    rgbRed;
    byte    rgbReserved;
};

struct BMP_ICONDIR {
	word           idReserved;   // Reserved (must be 0)
	word           idType;       // Resource Type (1 for icons)
	word           idCount;      // How many images?
}
#ifdef COMPILER_GCC
__attribute__((packed))
#endif
;

struct BMP_ICONDIRENTRY {
	byte        bWidth;          // устШирину, in pixels, of the image
	byte        bHeight;         // устВысоту, in pixels, of the image
	byte        bColorCount;
	byte        bReserved;
	short       wHotSpotX;
	short       wHotSpotY;
	dword       dwBytesInRes;    // How many bytes in this resource?
	dword       dwImageOffset;   // Where in the file is this image?
}
#ifdef COMPILER_GCC
__attribute__((packed))
#endif
;

#if defined(COMPILER_MSC) || defined(COMPILER_GCC)
#pragma pack(pop)
#endif

struct BMPHeader : public BMP_INFOHEADER
{
	BMP_RGB palette[256];
};

#endif
