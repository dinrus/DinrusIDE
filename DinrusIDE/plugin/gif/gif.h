#ifndef _plugin_gif_gif_h_
#define _plugin_gif_gif_h_

#include <Draw/Draw.h>


namespace РНЦП {
	
ИНИЦИАЛИЗУЙ(GifRaster)

class GIFRaster : public StreamRaster {
public:
	class Данные;
	Один<Данные> data;

public:
	GIFRaster();
	~GIFRaster();

	virtual bool               создай();
	virtual Размер               дайРазм();
	virtual Info               GetInfo();
	virtual Строка               дайСтроку(int line);
	virtual const RasterFormat *дайФормат();
	virtual int                GetPaletteCount();
	virtual КЗСА               *GetPalette();
	
	virtual int                GetPageCount();
	virtual int                GetActivePage() const;
	virtual void               SeekPage(int n);
	virtual int                GetPageAspect(int n);
	virtual int                GetPageDelay(int n);
	virtual Прям               GetPageRect(int n);
	virtual int				   GetPageDisposal(int n);

private: 
	bool                       иниц();
};

class GIFEncoder : public StreamRasterEncoder {
public:
	class Данные;
	Один<Данные> data;

public:
	GIFEncoder(bool ignore_alpha = false, Ткст comment = Null);
	~GIFEncoder();

	GIFEncoder&  IgnoreAlpha(bool ia = true) { ignore_alpha = ia; return *this; }
	GIFEncoder&  Comment(Ткст c)           { comment = c; return *this; }

	virtual int  GetPaletteCount();
	virtual void старт(Размер sz);
	virtual void WriteLineRaw(const byte *s);

private:
	bool         ignore_alpha;
	Ткст       comment;
};

}

#endif
