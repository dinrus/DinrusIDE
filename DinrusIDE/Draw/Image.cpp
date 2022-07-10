#include "Draw.h"

namespace РНЦП {

#define LTIMING(x) // RTIMING(x)

int ImageBuffer::ScanKind() const
{
	const КЗСА *s = pixels;
	const КЗСА *ee = pixels + (дайДлину() & ~3);
	while(s < ee) {
		if((s[0].a & s[1].a & s[2].a & s[3].a) != 255)
			return IMAGE_ALPHA;
		s += 4;
	}
	const КЗСА *e = pixels + дайДлину();
	while(s < e) {
		if(s->a != 255)
			return IMAGE_ALPHA;
		s++;
	}
	return IMAGE_OPAQUE;
}

void ImageBuffer::SetHotSpots(const Рисунок& src)
{
	SetHotSpot(src.GetHotSpot());
	Set2ndSpot(src.Get2ndSpot());
}

void ImageBuffer::создай(int cx, int cy)
{
	ПРОВЕРЬ(cx >= 0 && cy >= 0);
	size.cx = cx;
	size.cy = cy;
	pixels.размести(дайДлину());
#ifdef _ОТЛАДКА
	КЗСА *s = pixels;
	КЗСА *e = pixels + дайДлину();
	byte  a = 0;
	while(s < e) {
		s->a = a;
		a = ~a;
		s->r = a ? 255 : 0;
		s->g = s->b = 0;
		s++;
	}
#endif
	kind = IMAGE_UNKNOWN;
	InitAttrs();
}

void ImageBuffer::InitAttrs()
{
	spot2 = hotspot = Точка(0, 0);
	dots = Размер(0, 0);
	resolution = IMAGE_RESOLUTION_NONE;
	paintonce = false;
}

void ImageBuffer::CopyAttrs(const ImageBuffer& img)
{
	SetHotSpot(img.GetHotSpot());
	Set2ndSpot(img.Get2ndSpot());
	SetDots(img.GetDots());
	SetResolution(img.GetResolution());
	PaintOnceHint(img.IsPaintOnceHint());
}

void ImageBuffer::CopyAttrs(const Рисунок& img)
{
	if(img.data)
		CopyAttrs(img.data->буфер);
	else
		InitAttrs();
}

void ImageBuffer::DeepCopy(const ImageBuffer& img)
{
	создай(img.дайРазм());
	CopyAttrs(img);
	memcpy_t(~pixels, ~img.pixels, дайДлину());
}

void ImageBuffer::уст(Рисунок& img)
{
	if(img.data)
		if(img.data->refcount == 1) {
			size = img.дайРазм();
			kind = IMAGE_UNKNOWN;
			CopyAttrs(img);
			pixels = pick(img.data->буфер.pixels);
			img.очисть();
		}
		else {
			DeepCopy(img.data->буфер);
			kind = IMAGE_UNKNOWN;
			img.очисть();
		}
	else
		создай(0, 0);
}


void ImageBuffer::operator=(Рисунок& img)
{
	очисть();
	уст(img);
}

void ImageBuffer::operator=(ImageBuffer& img)
{
	очисть();
	Рисунок m = img;
	уст(m);
}

ImageBuffer::ImageBuffer(Рисунок& img)
{
	уст(img);
}

ImageBuffer::ImageBuffer(ImageBuffer& b)
{
	kind = b.kind;
	size = b.size;
	pixels = pick(b.pixels);
	CopyAttrs(b);
}

void ImageBuffer::SetDPI(Размер dpi) 
{
	dots.cx = int(600.*size.cx/dpi.cx);
	dots.cy = int(600.*size.cy/dpi.cy);
}

Размер ImageBuffer::GetDPI() 
{
	return Размер(dots.cx ? int(600.*size.cx/dots.cx) : 0, dots.cy ? int(600.*size.cy/dots.cy) : 0);
}

void Рисунок::уст(ImageBuffer& b)
{
	if(b.дайШирину() == 0 || b.дайВысоту() == 0)
		data = NULL;
	else
		data = new Данные(b);
}

void Рисунок::очисть()
{
	if(data)
		data->отпусти();
	data = NULL;
}

Рисунок& Рисунок::operator=(ImageBuffer& img)
{
	if(data)
		data->отпусти();
	уст(img);
	return *this;
}

Рисунок& Рисунок::operator=(const Рисунок& img)
{
	Данные *d = data;
	data = img.data;
	if(data)
		data->Retain();
	if(d)
		d->отпусти();
	return *this;
}

Точка Рисунок::GetHotSpot() const
{
	return data ? data->буфер.GetHotSpot() : Точка(0, 0);
}

Точка Рисунок::Get2ndSpot() const
{
	return data ? data->буфер.Get2ndSpot() : Точка(0, 0);
}

Размер Рисунок::GetDots() const
{
	return data ? data->буфер.GetDots() : Размер(0, 0);
}

Размер Рисунок::GetDPI() const
{
	Размер size = дайРазм();
	Размер dots = GetDots();
	return data ?  Размер(int(600.*size.cx/dots.cx), int(600.*size.cy/dots.cy)): Размер(0, 0);
}

int Рисунок::GetResolution() const
{
	return data ? data->буфер.GetResolution() : IMAGE_RESOLUTION_NONE;
}

int Рисунок::GetKindNoScan() const
{
	return data ? data->буфер.GetKind() : IMAGE_ALPHA;
}

int Рисунок::Данные::GetKind()
{
	int k = буфер.GetKind();
	if(k != IMAGE_UNKNOWN)
		return k;
	k = буфер.ScanKind();
	буфер.SetKind(k);
	return k;
}

int Рисунок::GetKind() const
{
	return data ? data->GetKind() : IMAGE_EMPTY;
}

void Рисунок::сериализуй(Поток& s)
{
	Точка spot2 = Get2ndSpot();
	int version = spot2.x || spot2.y; // version 1 only if we need 2nd spot, to improve BW compatibility
	s / version;
	Размер sz = дайРазм();
	Точка p = GetHotSpot();
	Размер dots = GetDots();
	s % sz % p % dots;
	Точка p2(0, 0);
	if(version >= 1) {
		p2 = spot2;
		s % p2;
	}
	int64 len = (int64)sz.cx * (int64)sz.cy * (int64)sizeof(КЗСА);
	if(s.грузится()) {
		if(len) {
			ImageBuffer b;
			if(len < 6 * 1024 * 1024) {
				b.создай(sz);
				if(!s.дайВсе(~b, (int)len)) {
					очисть();
					s.загрузиОш();
					return;
				}
			}
			else {
				Huge h;
				if(!s.дайВсе(h, (size_t)len)) {
					очисть();
					s.загрузиОш();
					return;
				}
				b.создай(sz);
				h.дай(~b);
			}

			b.SetDots(dots);
			b.SetHotSpot(p);
			b.Set2ndSpot(p2);
			*this = b;
		}
		else
			очисть();
	}
	else
		s.помести64(~*this, len);
}
ИНИЦБЛОК {
	Значение::регистрируй<Рисунок>("Рисунок");
}

bool Рисунок::operator==(const Рисунок& img) const
{
	static_assert(sizeof(КЗСА) == 4, "sizeof(КЗСА)");
	return одинаково(img) ||
	   дайРазм() == img.дайРазм() &&
	   GetHotSpot() == img.GetHotSpot() &&
	   Get2ndSpot() == img.Get2ndSpot() &&
	   GetDots() == img.GetDots() &&
	   GetResolution() == img.GetResolution() &&
	   memeq_t(~*this, ~img, дайДлину());
}

bool Рисунок::operator!=(const Рисунок& img) const
{
	return !operator==(img);
}

hash_t Рисунок::дайХэшЗнач() const
{
	return memhash(~*this, дайДлину() * sizeof(КЗСА));
}

Рисунок::Рисунок(const Рисунок& img)
{
	data = img.data;
	if(data)
		data->Retain();
}

Рисунок::Рисунок(Рисунок (*фн)())
{
	data = NULL;
	*this = (*фн)();
}

Рисунок::Рисунок(const Значение& src)
{
	data = NULL;
	*this = src.дай<Рисунок>();
}

Рисунок::Рисунок(ImageBuffer& b)
{
	уст(b);
}

Рисунок::~Рисунок()
{
	if(data)
		data->отпусти();
}

Ткст Рисунок::вТкст() const
{
	return Ткст("Рисунок ").конкат() << дайРазм();
}

Рисунок::Данные::Данные(ImageBuffer& b)
:	буфер(b)
{
	paintcount = 0;
	paintonly = false;
	refcount = 1;
	aux_data = 0;
	INTERLOCKED {
		static int64 gserial;
		serial = ++gserial;
	}
}

void Рисунок::SetAuxData(uint64 adata)
{
	if(data)
		data->aux_data = adata;
}

uint64 Рисунок::GetAuxData() const
{
	return data ? data->aux_data : 0;
}

static void sMultiply(ImageBuffer& b, int (*op)(КЗСА *t, const КЗСА *s, int len))
{
	if(b.GetKind() != IMAGE_OPAQUE && b.GetKind() != IMAGE_EMPTY)
		(*op)(~b, ~b, b.дайДлину());
}

void Premultiply(ImageBuffer& b)
{
	sMultiply(b, Premultiply);
}

void Unmultiply(ImageBuffer& b)
{
	sMultiply(b, Unmultiply);
}

static Рисунок sMultiply(const Рисунок& img, int (*op)(КЗСА *t, const КЗСА *s, int len))
{
	int k = img.GetKind();
	if(k == IMAGE_OPAQUE || k == IMAGE_EMPTY)
		return img;
	ImageBuffer ib(img.дайРазм());
	ib.CopyAttrs(img);
	ib.SetKind((*op)(~ib, ~img, ib.дайДлину()));
	return ib;
}

Рисунок Premultiply(const Рисунок& img)
{
	return sMultiply(img, Premultiply);
}

Рисунок Unmultiply(const Рисунок& img)
{
	return sMultiply(img, Unmultiply);
}

Ткст StoreImageAsString(const Рисунок& img)
{
	if(img.GetKind() == IMAGE_EMPTY)
		return Null;
	int тип = img.GetKind() == IMAGE_OPAQUE ? 3 : 4;
	тип |= decode(img.GetResolution(), IMAGE_RESOLUTION_STANDARD, 0x40, IMAGE_RESOLUTION_UHD, 0x80, 0);
	ТкстПоток ss;
	ss.помести(тип);
	Размер sz = img.дайРазм();
	ss.Put16le(sz.cx);
	ss.Put16le(sz.cy);
	Точка p = img.GetHotSpot();
	ss.Put16le(p.x);
	ss.Put16le(p.y);
	Размер dots = img.GetDots();
	ss.Put16le(dots.cx);
	ss.Put16le(dots.cy);
	const КЗСА *s = img;
	const КЗСА *e = s + img.дайДлину();
	Буфер<byte> b(тип * img.дайДлину());
	byte *t = b;
	if(тип == 3)
		while(s < e) {
			*t++ = s->r;
			*t++ = s->g;
			*t++ = s->b;
			s++;
		}
	else
		while(s < e) {
			*t++ = s->r;
			*t++ = s->g;
			*t++ = s->b;
			*t++ = s->a;
			s++;
		}
	ПамЧтенПоток m(b, тип * img.дайДлину());
	ZCompress(ss, m);
	return ss;
}

Рисунок  LoadImageFromString(const Ткст& src)
{
	if(src.дайДлину() < 13)
		return Null;
	ТкстПоток ss(src);
	int тип = ss.дай();
	int resolution = decode(тип & 0xc0, 0x40, IMAGE_RESOLUTION_STANDARD, 0x80, IMAGE_RESOLUTION_UHD, 0);
	тип &= 0x3f;
	Размер sz;
	sz.cx = ss.Get16le();
	sz.cy = ss.Get16le();
	if(sz.cx < 0 || sz.cy < 0)
		return Null;
	Точка p;
	p.x = ss.Get16le();
	p.y = ss.Get16le();
	if(p.x < 0 || p.y < 0)
		return Null;
	Размер dots;
	dots.cx = ss.Get16le();
	dots.cy = ss.Get16le();
	if(dots.cx < 0 || dots.cy < 0)
		return Null;
	ТкстПоток out;
	ZDecompress(out, ss);
	Ткст data = out;
	if(data.дайДлину() != тип * sz.cx * sz.cy)
		return Рисунок();
	ImageBuffer ib(sz);
	ib.SetHotSpot(p);
	ib.SetDots(dots);
	ib.SetResolution(resolution);
	КЗСА *t = ib;
	const КЗСА *e = t + ib.дайДлину();
	const byte *s = data;
	if(тип == 3)
		while(t < e) {
			t->r = *s++;
			t->g = *s++;
			t->b = *s++;
			t->a = 255;
			t++;
		}
	else
	if(тип == 4)
		while(t < e) {
			t->r = *s++;
			t->g = *s++;
			t->b = *s++;
			t->a = *s++;
			t++;
		}
	else
		return Рисунок();
	return ib;
}

Размер GetImageStringSize(const Ткст& src)
{
	if(src.дайДлину() < 13)
		return Размер(0, 0);
	ТкстПоток ss(src);
	ss.дай();
	Размер sz;
	sz.cx = ss.Get16le();
	sz.cy = ss.Get16le();
	return sz;
}

Размер GetImageStringDots(const Ткст& src)
{
	if(src.дайДлину() < 13)
		return Размер(0, 0);
	ТкстПоток ss(src);
	ss.SeekCur(9);
	Размер sz;
	sz.cx = ss.Get16le();
	sz.cy = ss.Get16le();
	return sz;
}

}
