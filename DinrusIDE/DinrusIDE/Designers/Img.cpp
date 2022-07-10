#include "Designers.h"

void IdeImgView::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	Ткст t = (img_sz != img.дайРазм() ? "Resized from: " : "Рисунок size: ");
	t << фмт("%d x %d", img_sz.cx, img_sz.cy);
	int tcy = Draw::GetStdFontCy();
	w.DrawRect(0, 0, sz.cx, tcy, SColorFace());
	w.DrawText(5, 0, t, StdFont(), SColorText());
	int ii = 0;
	for(int x = 0; x < sz.cx; x += 16) {
		int jj = ii;
		for(int y = tcy; y < sz.cy; y += 16)
			w.DrawRect(x, y, 16, 16, jj++ & 1 ? светлоСерый() : белоСерый());
		ii++;
	}
	w.DrawImage(5, 5 + tcy, img);
}

void IdeImgView::EditMenu(Бар& menu)
{
	
}

bool IsImgFile(const Ткст& path)
{
	Ткст s = впроп(дайРасшф(path));
	return s == ".png" || s == ".jpg" || s == ".gif" || s == ".bmp";
}

struct ImageViewModule : public МодульИСР {
	virtual Ткст       GetID() { return "ImageViewModule"; }
	virtual void CleanUsc() {}
	virtual bool ParseUsc(СиПарсер& p) { return false; }
	virtual Рисунок FileIcon(const char *path) {
		return IsImgFile(path) ? IconDesImg::FileIcon() : Null;
	}
	virtual IdeDesigner *CreateDesigner(const char *path, byte) {
		if(IsImgFile(path)) {
			ФайлВвод in(path);
			Один<StreamRaster> o = StreamRaster::OpenAny(in);
			if(o) {
				Размер sz = o->дайРазм();
				if(впроп(дайРасшф(path)) == ".png" && sz.cx <= 768 && sz.cy <= 512) {
					IdePngDes *d = new IdePngDes;
					d->грузи(path);
					d->RestoreEditPos();
					return d;
				}
				else {
					IdeImgView *d = new IdeImgView;
					d->img_sz = sz;
					d->filename = path;
					if(sz.cx <= 1024 && sz.cy <= 768)
						d->img = o->GetImage();
					else {
						ImageEncoder m;
						Rescale(m, дайРазмСхождения(sz, Размер(1024, 768)), *o, sz);
						d->img = m;
					}
					return d;
				}
			}
			else
			if(впроп(дайРасшф(path)) == ".png" && дайДлинуф(path) <= 0) {
				IdePngDes *d = new IdePngDes;
				d->создай(path);
				d->RestoreEditPos();
				return d;
			}
		}
		return NULL;
	}
};

ИНИЦИАЛИЗАТОР(Img)
{
	регМодульИСР(Single<ImageViewModule>());
}
