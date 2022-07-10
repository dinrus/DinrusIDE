#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

//#include <shellapi.h>

namespace РНЦП {

#define LTIMING(x) // RTIMING(x)
#define LLOG(x)

void устПоверхность(SystemDraw& w, const Прям& dest, const КЗСА *pixels, Размер srcsz, Точка poff)
{
	w.FlushText();
	Размер dsz = dest.дайРазм();
	cairo_surface_t *surface = cairo_image_surface_create_for_data((byte *)pixels, CAIRO_FORMAT_ARGB32, dsz.cx, dsz.cy, 4 * dsz.cx);
	cairo_set_source_surface(w, surface, dest.left, dest.top);
	cairo_paint(w);
	cairo_surface_destroy(surface);
}

struct ImageSysData {
	Рисунок            img;
	cairo_surface_t *surface = NULL;
	
	void иниц(const Рисунок& m, cairo_surface_t *other);
	~ImageSysData();
};

cairo_surface_t *CreateCairoSurface(const Рисунок& img, cairo_surface_t *other)
{
	Размер isz = img.дайРазм();
	cairo_format_t fmt = CAIRO_FORMAT_ARGB32;
	cairo_surface_t *surface = other ? cairo_surface_create_similar_image(other, fmt, isz.cx, isz.cy)
	                                 : cairo_image_surface_create(fmt, isz.cx, isz.cy);
	cairo_surface_flush(surface);
	byte *a = (byte *)cairo_image_surface_get_data(surface);
	int stride = cairo_format_stride_for_width(fmt, isz.cx);
	for(int yy = 0; yy < isz.cy; yy++) {
		копируй((КЗСА *)a, img[yy], isz.cx);
		a += stride;
	}
	cairo_surface_mark_dirty(surface);
	return surface;
}

cairo_surface_t *CreateCairoSurface(const Рисунок& img)
{
	return CreateCairoSurface(img, NULL);
}

void ImageSysData::иниц(const Рисунок& m, cairo_surface_t *other)
{
	img = m;
	surface = CreateCairoSurface(m, other);
	SysImageRealized(img);
}

ImageSysData::~ImageSysData()
{
	SysImageReleased(img);
	cairo_surface_destroy(surface);
}

struct ImageSysDataMaker : LRUCache<ImageSysData, int64>::Делец {
	Рисунок img;
	cairo_surface_t *other;

	virtual int64  Ключ() const                      { return img.GetSerialId(); }
	virtual int    сделай(ImageSysData& object) const { object.иниц(img, other); return img.дайДлину(); }
};

void SystemDraw::SysDrawImageOp(int x, int y, const Рисунок& img, Цвет color)
{
	ЗамкниГип __;
	FlushText();
	if(img.дайДлину() == 0)
		return;
	if(img.IsPaintOnceHint()) {
		устПоверхность(*this, x, y, img.дайШирину(), img.дайВысоту(), ~img);
		return;
	}
	LLOG("SysDrawImageOp " << img.GetSerialId() << ' ' << x << ", " << y << ", "<< img.дайРазм());
	ImageSysDataMaker m;
	static LRUCache<ImageSysData, int64> cache;
	static int Rsz;
	Rsz += img.дайДлину();
	if(Rsz > 200 * 200) { // we do not want to do this for each small image painted...
		Rsz = 0;
		cache.удали([](const ImageSysData& object) {
			return object.img.GetRefCount() == 1;
		});
	}
	LLOG("SysImage cache pixels " << cache.дайРазм() << ", count " << cache.дайСчёт());
	m.img = img;
	m.other = cairo_get_target(cr);
	ImageSysData& sd = cache.дай(m);
	if(!пусто_ли(color)) {
		устЦвет(color);
		cairo_mask_surface(cr, sd.surface, x, y);
	}
	else {
		cairo_set_source_surface(cr, sd.surface, x, y);
		cairo_paint(cr);
	}
	static Размер ssz;
	if(ssz.cx == 0)
		ssz = Ктрл::GetVirtualScreenArea().дайРазм();
	cache.сожми(4 * ssz.cx * ssz.cy, 1000); // Кэш must be after рисуй because of PaintOnly!
}

Draw& ImageDraw::Alpha()
{
	if(!alpha_surface) {
		alpha_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, isz.cx, isz.cy);
		alpha.cr = cairo_create(alpha_surface);
	}
	return alpha;
}

void CairoGet(ImageBuffer& b, Размер isz, cairo_surface_t *surface, cairo_surface_t *alpha_surface)
{
	cairo_surface_flush(surface);
	byte *a = (byte *)cairo_image_surface_get_data(surface);
	int stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, isz.cx);
	КЗСА *t = b;
	byte *aa = NULL;
	if(alpha_surface) {
		cairo_surface_flush(alpha_surface);
		aa = (byte *)cairo_image_surface_get_data(alpha_surface);
	}
	for(int yy = 0; yy < isz.cy; yy++) {
		КЗСА *s = (КЗСА *)a;
		КЗСА *e = s + isz.cx;
		if(aa) {
			КЗСА *ss = (КЗСА *)aa;
			while(s < e) {
				*t = *s++;
				(t++)->a = (ss++)->r;
			}
			aa += stride;
			b.SetKind(IMAGE_ALPHA);
		}
		else {
			while(s < e) {
				*t = *s++;
				(t++)->a = 255;
			}
			b.SetKind(IMAGE_OPAQUE);
		}
		a += stride;
	}
}

void ImageDraw::FetchStraight(ImageBuffer& b) const
{
	ImageDraw *m = const_cast<ImageDraw *>(this);
	m->FlushText();
	if(alpha_surface)
		m->alpha.FlushText();
	CairoGet(b, isz, surface, alpha_surface);
}

ImageDraw::operator Рисунок() const
{
	ImageBuffer img(isz);
	FetchStraight(img);
	Premultiply(img);
	return img;
}

Рисунок ImageDraw::GetStraight() const
{
	ImageBuffer img(isz);
	FetchStraight(img);
	return img;
}

void ImageDraw::иниц(Размер sz)
{
	isz = sz;
	surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, isz.cx, isz.cy);
	cr = cairo_create(surface);
	alpha_surface = NULL;
	del = true;
}

ImageDraw::ImageDraw(Размер sz)
{
	иниц(sz);
}

ImageDraw::ImageDraw(int cx, int cy)
{
	иниц(Размер(cx, cy));
}

ImageDraw::ImageDraw(cairo_t *cr_, Размер sz)
{
	isz = sz;
	cr = cr_;
	surface = cairo_get_target(cr);
	alpha_surface = NULL;
	del = false;
}

ImageDraw::~ImageDraw()
{
	if(del) {
		cairo_destroy(cr);
		cairo_surface_destroy(surface);
		if(alpha_surface) {
			cairo_destroy(alpha.cr);
			cairo_surface_destroy(alpha_surface);
		}
	}
}

void BackDraw::помести(SystemDraw& w, int x, int y)
{
}

void BackDraw::создай(SystemDraw& w, int cx, int cy)
{
}

void BackDraw::разрушь()
{
}

#define FCURSOR_(x) { static Рисунок h; ONCELOCK { h = CreateImage(Размер(1, 1), чёрный); h.SetAuxData(x + 1); } return h; }

Рисунок Рисунок::Arrow() FCURSOR_(GDK_LEFT_PTR)
Рисунок Рисунок::жди() FCURSOR_(GDK_WATCH)
Рисунок Рисунок::IBeam() FCURSOR_(GDK_XTERM)
Рисунок Рисунок::No() FCURSOR_(GDK_CIRCLE)
Рисунок Рисунок::SizeAll() FCURSOR_(GDK_FLEUR)
Рисунок Рисунок::SizeHorz() FCURSOR_(GDK_SB_H_DOUBLE_ARROW)
Рисунок Рисунок::SizeVert() FCURSOR_(GDK_SB_V_DOUBLE_ARROW)
Рисунок Рисунок::SizeTopLeft() FCURSOR_(GDK_TOP_LEFT_CORNER)
Рисунок Рисунок::SizeTop() FCURSOR_(GDK_TOP_SIDE)
Рисунок Рисунок::SizeTopRight() FCURSOR_(GDK_TOP_RIGHT_CORNER)
Рисунок Рисунок::SizeLeft() FCURSOR_(GDK_LEFT_SIDE)
Рисунок Рисунок::SizeRight() FCURSOR_(GDK_RIGHT_SIDE)
Рисунок Рисунок::SizeBottomLeft() FCURSOR_(GDK_BOTTOM_LEFT_CORNER)
Рисунок Рисунок::SizeBottom() FCURSOR_(GDK_BOTTOM_SIDE)
Рисунок Рисунок::SizeBottomRight()  FCURSOR_(GDK_BOTTOM_RIGHT_CORNER)
Рисунок Рисунок::Cross() FCURSOR_(GDK_CROSSHAIR)
Рисунок Рисунок::Hand() FCURSOR_(GDK_HAND1)

}

#endif
