#ifndef __Geom_Draw_hrr__
#define __Geom_Draw_hrr__

namespace РНЦП {

//#include "Поток.h"

class ImageWriter : public RasterEncoder {
public:
	ImageWriter() : output(NULL) {}
	ImageWriter(ImageBuffer& output, bool merge = true)                       { открой(output, merge); }
	ImageWriter(ImageBuffer& output, Точка pos, bool merge = true)            { открой(output, pos, merge); }
	ImageWriter(ImageBuffer& output, Точка pos, Прям clip, bool merge = true) { открой(output, pos, clip, merge); }

	void         открой(ImageBuffer& output, bool merge = true)                 { открой(output, Точка(0, 0), merge); }
	void         открой(ImageBuffer& output, Точка pos, bool merge = true)      { открой(output, pos, Прям(output.дайРазм()), merge); }
	void         открой(ImageBuffer& output, Точка pos, Прям clip, bool merge = true);

	virtual void старт(Размер sz);
	virtual void WriteLineRaw(const byte *data);

private:
	ImageBuffer  *output;
	Точка        pos;
	int          left, width, offset;
	Прям         clip;
	int          line;
	Размер         src_size;
	bool         merge;
};

class ImageBufferRaster : public Raster {
public:
	ImageBufferRaster(const ImageBuffer& буфер);
	ImageBufferRaster(const ImageBuffer& буфер, const Прям& crop);

	virtual Размер       дайРазм();
	virtual Info       GetInfo();
	virtual Строка       дайСтроку(int line);

private:
	const ImageBuffer& буфер;
	Прям               crop;
};

class HRRInfo
{
	friend class HRR;

public:
	HRRInfo();
	HRRInfo(const ПрямПЗ& log_rect, const ПрямПЗ& map_rect = Null,
		int levels = 5, Цвет background = белый,
		int method = METHOD_JPG, int quality = DFLT_JPG_QUALITY,
		bool mono = false, Цвет mono_black = чёрный, Цвет mono_white = белый);

	void               сериализуй(Поток& stream);

	bool               пустой() const       { return levels == 0; }

	int                GetLevels() const     { return levels; }
	int                дайМаксРазм() const    { return levels ? UNIT << (levels - 1) : 0; }
	ПрямПЗ              GetLogRect() const    { return log_rect; }
	ПрямПЗ              GetMapRect() const    { return map_rect; }
	Цвет              дайФон() const { return background; }
	bool               IsMono() const        { return mono; }
	Цвет              GetMonoBlack() const  { return mono_black; }
	Цвет              GetMonoWhite() const  { return mono_white; }
	int                GetMethod() const     { return method; }
	int                GetQuality() const    { return quality; }

	Один<StreamRaster>  GetDecoder() const;
	Один<StreamRasterEncoder> GetEncoder() const;

	static double      GetEstimatedFileSize(int _levels, int method, int quality);

	static int         Pack(int method, int quality) { return (method << 16) | (quality & 0xFFFF); }
	static int         Method(int p)         { return p >> 16; }
	static int         Quality(int p)        { return p & 0xFFFF; }

	static Вектор<int> EnumMethods();
	static Вектор<int> EnumQualities(int method);
	static Ткст      дайИмя(int method, int quality);
	static ВекторМап<int, Ткст> GetPackMap();

public:
	enum
	{
		HALF_BITS = 8,
		HALF      = 1 << HALF_BITS,
		UNIT      = HALF << 1,
		STDLEVEL  = 6,
		MAXLEVELS = 12,
		LCOUNT    = 2,
		UCOUNT    = 1 << LCOUNT,
	};

	enum
	{
		METHOD_JPG,
		METHOD_GIF,
		METHOD_RLE,
		METHOD_PNG,
		METHOD_ZIM,
		METHOD_BZM,

		DFLT_JPG_QUALITY = 50,
	};

protected:
	Цвет      background;
	Цвет      mono_black;
	Цвет      mono_white;
	ПрямПЗ      log_rect;
	ПрямПЗ      map_rect; // logical rectangle inflated to tightly bounding square
	int        levels;
	int        method;
	int        quality;
	bool       mono;
};

class HRR
{
public:
	struct Block
	{
		Block(const HRR& hrr) : hrr(hrr) {}

		void        иниц(Размер size, КЗСА color);

		ImageBuffer block;
		Размер        size;
		Прям        area;
		ПрямПЗ       log_area;
		int         level;
		const HRR&  hrr;
	};

	struct Cursor {
		Cursor(HRR& owner, const ПрямПЗ& extent, double measure, int alpha = 100,
			Цвет mono_black = Null, Цвет mono_white = Null, Цвет blend_bgnd = Null);

		bool Fetch(ПрямПЗ& part_rc);
		Рисунок дай();

		HRR& owner;
		ПрямПЗ extent;
		double measure;
		Цвет mono_black;
		Цвет mono_white;
		int alpha;
		Цвет blend_bgnd;

		Один<StreamRaster> raster;
		int level;
		int total;
		Прям rc;
		Точка block;
		int cimg;
	};

	friend struct Cursor;

	enum
	{
		DEFAULT_CACHE_SIZEOF_LIMIT = 20000000,
	};

	typedef Gate1<Block&>      Writeback;

	HRR();
	HRR(const char *path, bool read_only = true);

	bool                       открой(const char *path, bool read_only = true);
	void                       закрой();

	bool                       создай(const HRRInfo& _info, const char *path);
	void                       пиши(Writeback drawback, bool downscale = true);
	ПрямПЗ                      GetLogBlockRect(int level, const Прям& rc) const;
	Matrixf                    GetPixMapMatrix(int level, int x, int y) const;
	int64                      GetFileWriteSize() const;

	void                       SetCacheSizeLimit(int cl)       { FlushCache(cache_sizeof_limit = cl); }
	int                        GetCacheSizeLimit() const       { return cache_sizeof_limit; }
	int                        GetCacheSize() const            { return cache_sizeof + directory_sizeof; }
	void                       очистьКэш();

	Ткст                     дайМап(Ткст ключ) const        { return map.дай(ключ, Null); }
	void                       устМап(Ткст ключ, Ткст значение);
	void                       ClearMap(Ткст ключ)            { устМап(ключ, Null); }
	void                       ClearMap()                      { map.очисть(); }
	const ВекторМап<Ткст, Ткст>& дайМап() const            { return map; }
	void                       FlushMap();

	bool                       открыт() const                  { return stream.открыт(); }
	bool                       ошибка_ли() const                 { return stream.ошибка_ли(); }
	int                        SizeOfInstance() const;

	void                       рисуй(Draw& draw, Прям dest, ПрямПЗ src,
		int wash = 100, int max_pixel = 1, Цвет mono_black = Null, Цвет mono_white = Null, Цвет blend_bgnd = Null);

	void                       рисуй(Draw& draw, const Matrixf& trg_pix, GisTransform transform,
		int wash = 100, int max_pixel = 1, Цвет mono_black = Null, Цвет mono_white = Null, Цвет blend_bgnd = Null);

	static int                 GetProgressCount(int levels, bool downscale = true);

	const HRRInfo&             GetInfo() const                 { return info; }

	static Один<StreamRasterEncoder> (*CreateEncoder)(const HRRInfo& info);
	static Один<StreamRaster>        (*CreateDecoder)(const HRRInfo& info);
	static Один<StreamRasterEncoder> StdCreateEncoder(const HRRInfo& info);
	static Один<StreamRaster>        StdCreateDecoder(const HRRInfo& info);

private:
	bool                       пиши(Writeback drawback, bool downscale, int level, int px, int py,
		StreamRasterEncoder& формат, Block *put);
	void                       сериализуй();
	void                       FlushCache(int limit);

private:
	HRRInfo                    info;
	ФайлПоток                 stream;
	int                        version;
	Вектор<int64>              pixel_directory_offset;
	Вектор<int64>              mask_directory_offset;
//	Вектор< Вектор<int> >      pixel_directory;
//	Вектор< Вектор<int> >      mask_directory;
	int                        directory_sizeof;
	МассивМап<Точка, Рисунок>     image_cache;
	ВекторМап<int, Размер>       size_cache;
	int                        cache_sizeof;
	int                        cache_sizeof_limit;
	ВекторМап<Ткст, Ткст>  map;
	int                        map_offset;
};

}

#endif//__Geom_Draw_hrr__
