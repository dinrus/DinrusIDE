#include "RichText.h"
#include <Painter/Painter.h>

namespace РНЦП {

#ifdef NEWIMAGE

struct RichImage : public RichObjectType {
	virtual Ткст GetTypeName(const Значение& v) const;
	virtual Размер   GetPhysicalSize(const Значение& data) const;
	virtual Размер   GetPixelSize(const Значение& data) const;
	virtual void   рисуй(const Значение& data, Draw& w, Размер sz, void *) const;
	virtual Рисунок  ToImage(int64, const Значение& data, Размер sz, void *) const;

	virtual bool   прими(PasteClip& clip);
	virtual Значение  читай(PasteClip& clip);
	virtual Ткст GetClipFmts() const;
	virtual Ткст GetClip(const Значение& data, const Ткст& fmt) const;

	typedef RichImage ИМЯ_КЛАССА;
};

Ткст RichImage::GetTypeName(const Значение& v) const
{
	return "image";
}

// following function pointers are set in CtrlCore (or similar host platform interface package)
static Ткст      (*sGetImageClip)(const Рисунок& img, const Ткст& fmt);
static bool        (*sAcceptImage)(PasteClip& clip);
static Рисунок       (*sGetImage)(PasteClip& clip);
static const char *(*sClipFmtsImage)();

void InitRichImage(Ткст      (*fGetImageClip)(const Рисунок& img, const Ткст& fmt),
                   bool        (*fAcceptImage)(PasteClip& clip),
                   Рисунок       (*fGetImage)(PasteClip& clip),
                   const char *(*fClipFmtsImage)())
{
	sGetImageClip = fGetImageClip;
	sAcceptImage = fAcceptImage;
	sGetImage = fGetImage;
	sClipFmtsImage = fClipFmtsImage;
}

bool RichImage::прими(PasteClip& clip)
{
	return sAcceptImage ? sAcceptImage(clip) : false;
}

Значение RichImage::читай(PasteClip& clip)
{
	if(sGetImage)
		return StoreImageAsString(sGetImage(clip));
	return Null;
}

Ткст RichImage::GetClipFmts() const
{
	if(sClipFmtsImage)
		return sClipFmtsImage();
	return Null;
}

Ткст RichImage::GetClip(const Значение& data, const Ткст& fmt) const
{
	if(sGetImageClip)
		return sGetImageClip(LoadImageFromString(data), fmt);
	return Null;
}

Размер   RichImage::GetPixelSize(const Значение& data) const
{
	return GetImageStringSize(data);
}

Размер   RichImage::GetPhysicalSize(const Значение& data) const
{
	Размер sz = GetImageStringDots(data);
	if(sz.cx == 0 || sz.cy == 0)
		sz = 600 * GetPixelSize(data) / 96;
	return sz;
}

void   RichImage::рисуй(const Значение& data, Draw& w, Размер sz, void *) const
{
	Рисунок x = LoadImageFromString(data);
//	Размер outsz(min(sz.cx, 4 * x.дайШирину()), min(sz.cy, 4 * x.дайВысоту()));
	w.DrawImage(0, 0, sz.cx, sz.cy, x);
}

Рисунок  RichImage::ToImage(int64, const Значение& data, Размер sz, void *) const
{
	return Rescale(LoadImageFromString(data), sz);
}

ИНИЦБЛОК {
	RichObject::регистрируй("image", &Single<RichImage>());
};

RichObject CreateImageObject(const Рисунок& img, int cx, int cy)
{
	RichObject o = RichObject("image", StoreImageAsString(img));
	if(cx || cy)
		o.устРазм(дайСоотношение(o.GetPixelSize(), cx, cy));
	return o;
}

struct RichPNG : public RichObjectType {
	virtual Ткст GetTypeName(const Значение& v) const;
	virtual Значение  читай(const Ткст& s) const;
	virtual Ткст пиши(const Значение& v) const;
	virtual Размер   GetPhysicalSize(const Значение& data) const;
	virtual Размер   GetPixelSize(const Значение& data) const;
	virtual void   рисуй(const Значение& data, Draw& w, Размер sz) const;
	virtual Рисунок  ToImage(int64, const Значение& data, Размер sz, void *) const;
};

Ткст RichPNG::GetTypeName(const Значение& v) const
{
	return ткст_ли(v) ? "PNG" : "image";
}

Значение RichPNG::читай(const Ткст& s) const
{
	Рисунок img = StreamRaster::LoadStringAny(s);
	if(img)
		return img;
	return s;
}

Ткст RichPNG::пиши(const Значение& v) const
{
	if(ткст_ли(v))
		return v;
	return StoreImageAsString(v);
}

Размер RichPNG::GetPhysicalSize(const Значение& data) const
{
	if(ткст_ли(data))
		return Размер(0, 0);
	return Рисунок(data).GetDots();
}

Размер RichPNG::GetPixelSize(const Значение& data) const
{
	if(ткст_ли(data))
		return Размер(0, 0);
	return Рисунок(data).GetDots();
}

void RichPNG::рисуй(const Значение& data, Draw& w, Размер sz) const
{
	if(ткст_ли(data)) {
		w.DrawRect(sz, SColorFace());
		DrawFrame(w, sz, SColorText());
		w.DrawText(2, 2, "plugin/png missing!");
		return;
	}
	Рисунок x = Рисунок(data);
	Размер outsz(min(sz.cx, 4 * x.дайШирину()), min(sz.cy, 4 * x.дайВысоту()));
	w.DrawImage(0, 0, outsz.cx, outsz.cy, x);
}

Рисунок RichPNG::ToImage(int64, const Значение& data, Размер sz, void *) const
{
	if(ткст_ли(data)) {
		ImageAnyDraw iw(sz);
		рисуй(data, iw, sz);
		return iw;
	}
	Рисунок x = Рисунок(data);
	Размер outsz(min(sz.cx, 4 * x.дайШирину()), min(sz.cy, 4 * x.дайВысоту()));
	return Rescale(x, outsz);
}

ИНИЦБЛОК {
	RichObject::регистрируй("PNG", &Single<RichPNG>());
};

struct RichRawImage : public RichObjectType {
	virtual Ткст GetTypeName(const Значение& v) const;
	virtual Значение  читай(const Ткст& s) const;
	virtual Ткст пиши(const Значение& v) const;
	virtual Размер   GetPhysicalSize(const Значение& data) const;
	virtual Размер   GetPixelSize(const Значение& data) const;
	virtual void   рисуй(const Значение& data, Draw& w, Размер sz, void *) const;
	virtual Рисунок  ToImage(int64, const Значение& data, Размер sz, void *) const;
};

Ткст RichRawImage::GetTypeName(const Значение& v) const
{
	return "rawimage";
}

Значение RichRawImage::читай(const Ткст& s) const
{
	return s;
}

Ткст RichRawImage::пиши(const Значение& v) const
{
	return v;
}

Размер RichRawImage::GetPhysicalSize(const Значение& data) const
{
	Ткст s = data;
	ТкстПоток ss(s);
	Один<StreamRaster> r = StreamRaster::OpenAny(ss);
	if(r)
		return r->GetInfo().dots;
	else
	if(ткст_ли(data) && IsSVG(~data)) {
		ПрямПЗ f = GetSVGBoundingBox(~data);
		Zoom z = GetRichTextStdScreenZoom();
		return z.d * (Размер)f.дайРазм() / z.m;
	}
	return Размер(0, 0);
}

Размер RichRawImage::GetPixelSize(const Значение& data) const
{
	Ткст s = data;
	ТкстПоток ss(s);
	Один<StreamRaster> r = StreamRaster::OpenAny(ss);
	if(r)
		return r->дайРазм();
	else
	if(ткст_ли(data) && IsSVG(~data)) {
		ПрямПЗ f = GetSVGBoundingBox(~data);
		return (Размер)f.дайРазм();
	}
	return Размер(0, 0);
}

void RichRawImage::рисуй(const Значение& data, Draw& w, Размер sz, void *) const
{
	Ткст s = data;
	ТкстПоток ss(s);
	Один<StreamRaster> r = StreamRaster::OpenAny(ss);
	if(r) {
		Размер isz = r->дайРазм();
		if(GetIsJPGFn() && GetIsJPGFn()(~r) && GetPdfDrawJPEGFn())
			GetPdfDrawJPEGFn()(w, 0, 0, sz.cx, sz.cy, data);
		else
		if(isz.cx * isz.cy > sz.cx * sz.cy) { // conserve memory by scaling down from source
			ImageEncoder m;
			Rescale(m, sz, *r, isz);
			w.DrawImage(0, 0, sz.cx, sz.cy, m);
		}
		else
			w.DrawImage(0, 0, sz.cx, sz.cy, r->GetImage()); // scale up by Draw to give e.g. PDF chance to store unscaled
	}
	else
	if(IsSVG(s))
		w.DrawImage(0, 0, RenderSVGImage(sz, s));
}

Рисунок RichRawImage::ToImage(int64 serial_id, const Значение& data, Размер sz, void *) const
{
	Ткст s = data;
	ТкстПоток ss(s);
	Один<StreamRaster> r = StreamRaster::OpenAny(ss);
	if(r) {
		struct ImgFormatLoader : ImageMaker {
			int64  serial_id;
			Ткст s;

			virtual Ткст Ключ() const  { Ткст x; RawCat(x, serial_id); return x; }
			virtual Рисунок  сделай() const {
				ТкстПоток ss(s);
				Один<StreamRaster> r = StreamRaster::OpenAny(ss);
				return r ? r->GetImage() : Рисунок();
			}
		} loader;
		loader.serial_id = serial_id;
		loader.s = s;
		return Rescale(MakeImage(loader), sz);
	}
	else
	if(ткст_ли(data) && IsSVG(~data))
		return RenderSVGImage(sz, ~data);
	return Null;
}

ИНИЦБЛОК {
	RichObject::регистрируй("rawimage", &Single<RichRawImage>());
};

RichObject CreateRawImageObject(const Ткст& s, int cx, int cy)
{
	RichObject o = RichObject("rawimage", s);
	o.InitSize(cx, cy);
	return o;
}

struct RichImlImage : public RichObjectType {
	virtual Ткст GetTypeName(const Значение& v) const;
	virtual Размер   GetPhysicalSize(const Значение& data) const;
	virtual Размер   GetPixelSize(const Значение& data) const;
	virtual void   рисуй(const Значение& data, Draw& w, Размер sz) const;
	virtual Рисунок  ToImage(int64, const Значение& data, Размер sz, void *) const;
	virtual bool   текст_ли() const;
	
	Рисунок дай(const Значение& v) const;
};

Рисунок RichImlImage::дай(const Значение& v) const
{
	return GetImlImage((Ткст)v);
}

Ткст RichImlImage::GetTypeName(const Значение& v) const
{
	return "iml";
}

bool RichImlImage::текст_ли() const
{
	return true;
}

Размер RichImlImage::GetPhysicalSize(const Значение& data) const
{
	return дай(data).дайРазм();
}

Размер RichImlImage::GetPixelSize(const Значение& data) const
{
	return дай(data).дайРазм();
}

void RichImlImage::рисуй(const Значение& data, Draw& w, Размер sz) const
{
	w.DrawImage(0, 0, sz.cx, sz.cy, дай(data));
}

Рисунок RichImlImage::ToImage(int64, const Значение& data, Размер sz, void *) const
{
	return Rescale(дай(data), sz);
}

ИНИЦБЛОК {
	RichObject::регистрируй("iml", &Single<RichImlImage>());
};

#endif

ИНИЦИАЛИЗАТОР(RichImage) {}

}
