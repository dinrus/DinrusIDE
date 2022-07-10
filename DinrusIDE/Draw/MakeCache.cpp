#include "Draw.h"

namespace РНЦП {

#define LLOG(x)  //  DLOG(x)

struct scImageMaker : ValueMaker {
	const ImageMaker *m;
	bool  paintonly;

	virtual Ткст Ключ() const {
		ТкстБуф s;
		s.конкат(typeid(*m).name());
		RawCat(s, paintonly);
		s.конкат(m->Ключ());
		return Ткст(s);
	}
	virtual int    сделай(Значение& object) const {
		Рисунок img = m->сделай();
		LLOG("ImageMaker " << object.GetSerialId() << ", size " << object.дайРазм() << ", paintonly: " << paintonly);
		if(paintonly && !пусто_ли(img) && img.GetRefCount() == 1)
			SetPaintOnly__(img);
		object = img;
		return img.дайДлину() + 64;
	}
};

void SysImageRealized(const Рисунок& img)
{ // Pixel data copied to host platform, no need to keep pixels data in cache if it is paintonly kind
	if(!IsValueCacheActive())
		return;
	LLOG("SysImageRealized " << img.дайРазм());
	if(img.data && img.data->paintonly) {
		LLOG("Dropping PAINTONLY pixels of image #" << img.GetSerialId() << ", cache size: " << sImageCache().дайРазм() << ", img " << img.дайДлину());
		DropPixels___(img.data->буфер);
		ValueCacheAdjustSize([](const Значение& v) -> int {
			if(v.является<Рисунок>()) {
				const Рисунок& img = v.To<Рисунок>();
				return 64 + (~img ? img.дайДлину() : 0);
			}
			return -1;
		});
		LLOG("After drop, cache size: " << TheValueCache().дайРазм());
	}
}

void SysImageReleased(const Рисунок& img)
{ // CtrlCore removed handle for img, have to remove paintonly
	if(!IsValueCacheActive())
		return;
	if(!~img) { // No data -> this is paintonly image
		int64 serial_id = img.GetSerialId();
		LLOG("SysImageReleased " << img.GetSerialId() << ", cache size: " << sImageCache().дайРазм() << ", count " << sImageCache().дайСчёт());
		int n = ValueCacheRemoveOne([&](const Значение& v) -> bool {
			return v.является<Рисунок>() && v.To<Рисунок>().GetSerialId() == serial_id;
		});
		IGNORE_RESULT(n); // suppress warning about unused 'n' without LLOGs
		LLOG("SysImageReleased count: " << n);
		LLOG("SysImageReleased done cache size: " << sImageCache().дайРазм() << ", count " << sImageCache().дайСчёт());
	}
}

void SetMakeImageCacheMax(int m)
{
	SetupValueCache(m, 0, 0.125);
}

void  SetMakeImageCacheSize(int m)
{
	SetMakeImageCacheMax(m);
}

void SweepMkImageCache()
{
	AdjustValueCache();
}

Рисунок MakeImage__(const ImageMaker& m, bool paintonly)
{
	scImageMaker cm;
	cm.m = &m;
	cm.paintonly = paintonly;
	return MakeValue(cm);
}

Рисунок MakeImage(const ImageMaker& m)
{
	return MakeImage__(m, false);
}

Рисунок MakeImagePaintOnly(const ImageMaker& m)
{
	return MakeImage__(m, true);
}

class SimpleImageMaker : public ImageMaker {
	Рисунок (*make)(const Рисунок& image);
	Рисунок image;

public:
	virtual Ткст Ключ() const;
	virtual Рисунок  сделай() const;

	SimpleImageMaker(const Рисунок& image, Рисунок (*make)(const Рисунок& image))
	:	make(make),image(image) {}
};

Ткст SimpleImageMaker::Ключ() const
{
	Ткст ключ;
	RawCat(ключ, image.GetSerialId());
	RawCat(ключ, make);
	return ключ;
}

Рисунок SimpleImageMaker::сделай() const
{
	return (*make)(image);
}

Рисунок MakeImage(const Рисунок& image, Рисунок (*make)(const Рисунок& image))
{
	return MakeImage(SimpleImageMaker(image, make));
}

struct sCachedRescale : public ImageMaker
{
	Прям  src;
	Размер  sz;
	Рисунок img;
	int   filter;

	virtual Ткст Ключ() const {
		ТкстБуф h;
		RawCat(h, src.left);
		RawCat(h, src.top);
		RawCat(h, src.right);
		RawCat(h, src.bottom);
		RawCat(h, sz.cx);
		RawCat(h, sz.cy);
		RawCat(h, img.GetSerialId());
		RawCat(h, filter);
		return Ткст(h);
	}

	virtual Рисунок сделай() const {
		Рисунок im = пусто_ли(filter) ? Rescale(img, sz, src) : RescaleFilter(img, sz, src, filter);
		ImageBuffer m(im);
		m.SetHotSpot(sz * (img.GetHotSpot() - src.верхЛево()) / src.дайРазм());
		m.Set2ndSpot(sz * (img.Get2ndSpot() - src.верхЛево()) / src.дайРазм());
		return m;
	}
};

Рисунок CachedRescale(const Рисунок& m, Размер sz, const Прям& src, int filter)
{
	if(m.дайРазм() == sz && src == sz)
		return m;
	sCachedRescale cr;
	cr.sz = sz;
	cr.src = src;
	cr.img = m;
	cr.filter = filter;
	return MakeImage(cr);
}

Рисунок CachedRescale(const Рисунок& m, Размер sz, int filter)
{
	return CachedRescale(m, sz, m.дайРазм(), filter);
}

Рисунок CachedRescalePaintOnly(const Рисунок& m, Размер sz, const Прям& src, int filter)
{
	if(m.дайРазм() == sz)
		return m;
	sCachedRescale cr;
	cr.sz = sz;
	cr.src = src;
	cr.img = m;
	cr.filter = filter;
	return MakeImagePaintOnly(cr);
}

Рисунок CachedRescalePaintOnly(const Рисунок& m, Размер sz, int filter)
{
	return CachedRescalePaintOnly(m, sz, m.дайРазм(), filter);
}

struct sColorize : public ImageMaker
{
	Рисунок img;
	Цвет color;

	virtual Ткст Ключ() const {
		ТкстБуф h;
		RawCat(h, color);
		RawCat(h, img.GetSerialId());
		return Ткст(h);
	}

	virtual Рисунок сделай() const {
		return SetColorKeepAlpha(img, color);
	}
};

Рисунок CachedSetColorKeepAlpha(const Рисунок& img, Цвет color)
{
	sColorize m;
	m.img = img;
	m.color = color;
	return MakeImage(m);
}

Рисунок CachedSetColorKeepAlphaPaintOnly(const Рисунок& img, Цвет color)
{
	sColorize m;
	m.img = img;
	m.color = color;
	return MakeImagePaintOnly(m);
}

}
