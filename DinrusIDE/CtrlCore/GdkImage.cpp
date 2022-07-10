#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

namespace РНЦП {

void ImageGdk::освободи()
{
	if(pixbuf) {
		g_object_unref(pixbuf);
		pixbuf = NULL;
	}
}

bool ImageGdk::уст(const Рисунок& m)
{
	освободи();
	if(m.GetSerialId() != img.GetSerialId()) {
		img = Unmultiply(m);
		if(!пусто_ли(img)) {
			Размер sz = img.дайРазм();
			pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, sz.cx, sz.cy);
			byte *l = (byte *)gdk_pixbuf_get_pixels(pixbuf);
			int stride = gdk_pixbuf_get_rowstride(pixbuf);
			for(int y = 0; y < sz.cy; y++) {
				const КЗСА *s = img[y];
				const КЗСА *e = s + sz.cx;
				byte *t = l;
				while(s < e) {
					*t++ = s->r;
					*t++ = s->g;
					*t++ = s->b;
					*t++ = s->a;
					s++;
				}
				l += stride;
			}
		}
		return true;
	}
	return false;
}

ImageGdk::ImageGdk(const Рисунок& m)
{
	pixbuf = NULL;
	уст(m);
}

ImageGdk::ImageGdk()
{
	pixbuf = NULL;
}

ImageGdk::~ImageGdk()
{
	освободи();
}

}

#endif
