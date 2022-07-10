#include "IconDes.h"

namespace РНЦП {

Ткст IconDes::FormatImageName(const Слот& c)
{
	Размер sz = c.image.дайРазм();
	Ткст r;
	r << c.имя << " " << sz.cx << " x " << sz.cy;
	if(c.flags & IML_IMAGE_FLAG_FIXED)
		r << " Fxd";
	else {
		if(c.flags & IML_IMAGE_FLAG_FIXED_COLORS)
			r << " Clr";
		if(c.flags & IML_IMAGE_FLAG_FIXED_SIZE)
			r << " Sz";
	}
	if(c.flags & IML_IMAGE_FLAG_UHD)
		r << " HD";
	if(c.flags & IML_IMAGE_FLAG_DARK)
		r << " Dk";
	if(c.exp)
		r << " X";
	return r;
}

void IconDes::SyncList()
{
	if(syncinglist)
		return;
	syncinglist++;
	int sc = ilist.дайПромотку();
	int q = ilist.дайКлюч();
	ilist.очисть();
	Ткст s = взаг((Ткст)~search);
	for(int i = 0; i < slot.дайСчёт(); i++) {
		Слот& c = slot[i];
		if(взаг(c.имя).найди(s) >= 0)
			ilist.добавь(i, FormatImageName(c), c.image);
	}
	ilist.ScrollTo(sc);
	ilist.FindSetCursor(q);
	syncinglist--;
}

void IconDes::ищи()
{
	SyncList();
}

void IconDes::GoTo(int q)
{
	ilist.FindSetCursor(q);
	if(ilist.курсор_ли())
		return;
	search <<= Null;
	SyncList();
	ilist.FindSetCursor(q);
}

static int sCharFilterCid(int c)
{
	return IsAlNum(c) || c == '_' ? c : 0;
}

void IconDes::PlaceDlg(ТопОкно& dlg)
{
	Прям r = ilist.дайПрямЭкрана();
	Размер sz = dlg.дайРазм();
	dlg.NoCenter().устПрям(max(0, r.left + (r.устШирину() - sz.cx) / 2), r.bottom + 32, sz.cx, sz.cy);
}

void IconDes::PrepareImageDlg(WithImageLayout<ТопОкно>& dlg)
{
	CtrlLayoutOKCancel(dlg, "нов image");
	dlg.cx <<= 16;
	dlg.cy <<= 16;
	if(IsCurrent()) {
		Размер sz = GetImageSize();
		dlg.cx <<= sz.cx;
		dlg.cy <<= sz.cy;

		dword flags = IsCurrent() ? Current().flags : 0;
		dlg.fixed <<= !!(flags & IML_IMAGE_FLAG_FIXED);
		dlg.fixed_colors <<= !!(flags & IML_IMAGE_FLAG_FIXED_COLORS);
		dlg.fixed_size <<= !!(flags & IML_IMAGE_FLAG_FIXED_SIZE);
		
		dlg.uhd <<= !!(flags & IML_IMAGE_FLAG_UHD);
		dlg.dark <<= !!(flags & IML_IMAGE_FLAG_DARK);
		
		dlg.uhd ^= dlg.dark ^= dlg.exp ^= dlg.fixed_colors ^= dlg.fixed_size ^= dlg.fixed ^= dlg.имя ^=
			[&] { dlg.Break(-1000); };
	}
	dlg.имя.устФильтр(sCharFilterCid);
}

void IconDes::SyncDlg(WithImageLayout<ТопОкно>& dlg)
{
	bool b = !dlg.fixed;
	dlg.fixed_colors.вкл(b);
	dlg.fixed_size.вкл(b);
}

dword IconDes::GetFlags(WithImageLayout<ТопОкно>& dlg)
{
	dword flags = 0;
	if(dlg.fixed)
		flags |= IML_IMAGE_FLAG_FIXED;
	if(dlg.fixed_colors)
		flags |= IML_IMAGE_FLAG_FIXED_COLORS;
	if(dlg.fixed_size)
		flags |= IML_IMAGE_FLAG_FIXED_SIZE;
	if(dlg.uhd)
		flags |= IML_IMAGE_FLAG_UHD;
	if(dlg.dark)
		flags |= IML_IMAGE_FLAG_DARK;
	return flags;
}

void IconDes::PrepareImageSizeDlg(WithImageSizeLayout<ТопОкно>& dlg)
{
	CtrlLayoutOKCancel(dlg, "нов image");
	dlg.cx <<= 16;
	dlg.cy <<= 16;
	if(IsCurrent()) {
		Размер sz = GetImageSize();
		dlg.cx <<= sz.cx;
		dlg.cy <<= sz.cy;
	}
}

bool CheckName(WithImageLayout<ТопОкно>& dlg)
{
	Ткст n = ~dlg.имя;
	СиПарсер p(n);
	if(p.ид_ли()) return true;
	Exclamation("Invalid имя!");
	return false;
}

void IconDes::InsertRemoved(int q)
{
	if(q >= 0 && q < removed.дайСчёт()) {
		int ii = ilist.курсор_ли() ? (int)ilist.дайКлюч() : 0;
		slot.вставь(ii) = removed[q];
		removed.удали(q);
		SyncList();
		GoTo(ii);
	}
}

void SetRes(Рисунок& m, int resolution)
{
	ImageBuffer ib(m);
	ib.SetResolution(findarg(resolution, IMAGE_RESOLUTION_STANDARD, IMAGE_RESOLUTION_UHD,
	                         IMAGE_RESOLUTION_NONE)
	                 >= 0 ? resolution : IMAGE_RESOLUTION_STANDARD);
	m = ib;
}

IconDes::Слот& IconDes::ImageInsert(int ii, const Ткст& имя, const Рисунок& m, bool exp)
{
	Слот& c = slot.вставь(ii);
	c.имя = имя;
	c.image = m;
	c.exp = exp;
	SyncList();
	GoTo(ii);
	return c;
}

IconDes::Слот& IconDes::ImageInsert(const Ткст& имя, const Рисунок& m, bool exp)
{
	int ii = ilist.курсор_ли() ? (int)ilist.дайКлюч() : 0;
	if(ii == slot.дайСчёт() - 1)
		ii = slot.дайСчёт();
	return ImageInsert(ii, имя, m, exp);
}

void IconDes::InsertImage()
{
	WithImageLayout<ТопОкно> dlg;
	PrepareImageDlg(dlg);
	for(;;) {
		SyncDlg(dlg);
		int c = dlg.пуск();
		if(c == IDCANCEL)
			return;
		if(c == IDOK && CheckName(dlg))
			break;
	}
	Рисунок m = CreateImage(Размер(~dlg.cx, ~dlg.cy), Null);
	ImageInsert(~dlg.имя, m, dlg.exp).flags = GetFlags(dlg);
}

void IconDes::Slice()
{
	if(!IsCurrent())
		return;
	Рисунок src = Current().image;
	Размер isz = src.дайРазм();
	int cc = min(isz.cx, isz.cy);
	if(!cc)
		return;
	WithImageLayout<ТопОкно> dlg;
	PrepareImageDlg(dlg);
	dlg.имя <<= Current().имя;
	dlg.cx <<= cc;
	dlg.cy <<= cc;
	dlg.Титул("Slice image");
	for(;;) {
		SyncDlg(dlg);
		int c = dlg.пуск();
		if(c == IDCANCEL)
			return;
		if(c == IDOK && CheckName(dlg))
			break;
	}
	Ткст s = ~dlg.имя;
	int n = 0;
	int ii = ilist.дайКлюч();
	for(int y = 0; y < isz.cy; y += (int)~dlg.cy)
		for(int x = 0; x < isz.cx; x += (int)~dlg.cx) {
			Рисунок m = Crop(src, x, y, ~dlg.cx, ~dlg.cy);
			ImageInsert(++ii, s + какТкст(n++), m, ~dlg.exp).flags = GetFlags(dlg);
		}
}

void IconDes::Duplicate()
{
	if(!IsCurrent())
		return;
	Слот& c = Current();
	ImageInsert(c.имя, c.image);
	EditImage();
}

void IconDes::InsertPaste()
{
	Рисунок m = ReadClipboardImage();
	if(пусто_ли(m)) {
		Exclamation("Clipboard does not contain an image.");
		return;
	}
	SetRes(m, IMAGE_RESOLUTION_STANDARD);
	ImageInsert("", m);
	EditImage();
}

struct FileImage : ImageMaker {
	Ткст filename;
	Размер   size;
	
	virtual Ткст Ключ() const { return filename + '/' + какТкст(size); }
	virtual Рисунок сделай() const {
		if(дайДлинуф(filename) > 1024 * 1024 * 20)
			return Null;
		Рисунок m = StreamRaster::LoadFileAny(filename);
		Размер sz = m.дайРазм();
		if(sz.cx > size.cx || sz.cy > size.cy) {
			if(sz.cx * size.cy > sz.cy * size.cx)
				sz = дайСоотношение(sz, size.cx, 0);
			else
				sz = дайСоотношение(sz, 0, size.cy);
			return Rescale(m, sz);
		}
		return m;
	}
};

struct ImgPreview : Дисплей {
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const {
		if(!пусто_ли(q)) {
			FileImage im;
			im.size = r.дайРазм();
			im.filename = q;
			w.DrawRect(r, SColorPaper);
			Рисунок m = MakeImage(im);
			Точка p = r.позЦентра(m.дайРазм());
			w.DrawImage(p.x, p.y, m);
		}
	}
};

static void sLoadImage(const Ткст& path, Рисунок& result)
{
	if(findarg(впроп(дайРасшф(path)), ".png", ".gif", ".jpeg", ".jpg") < 0)
		return;
	ФайлВвод in(path);
	if(!in)
		return;
	Один<StreamRaster> r = StreamRaster::OpenAny(in);
	if(!r)
		return;
	Размер sz = r->дайРазм();
	if(sz.cx > 80 || sz.cy > 80)
		return;
	result = r->GetImage();
}

FileSel& IconDes::ImgFile()
{
	static FileSel sel;
	ONCELOCK {
		sel.Type("Рисунок files", "*.png *.bmp *.jpg *.jpeg *.gif *.ico");
		sel.AllFilesType();
		sel.Multi();
		sel.WhenIconLazy = sLoadImage;
		sel.Preview(Single<ImgPreview>());
	}
	return sel;
}

int CharFilterImageId(int c)
{
	return IsAlNum(c) ? c : '_';
}

void IconDes::InsertFile()
{
	if(!ImgFile().ExecuteOpen()) return;
	for(int i = 0; i < ImgFile().дайСчёт(); i++) {
		Ткст фн = ImgFile()[i];
		Индекс<Рисунок> ml;
		if(впроп(дайРасшф(фн)) == ".ico")
			for(Рисунок m : ReadIcon(загрузиФайл(фн), true))
				ml.найдиДобавь(m);
		else {
			Рисунок mm = StreamRaster::LoadFileAny(фн);
			if(пусто_ли(mm))
				Exclamation(DeQtf(фн) + " not an image.");
			else
				ml.добавь(mm);
		}
		int ii = 0;
		for(Рисунок m : ml) {
			Ткст id = фильтруй(дайТитулф(фн), CharFilterImageId);
			if(!IsAlpha(*id) && *id != '_')
				id = '_' + id;
			if(ii)
				id << "_" << ii;
			SetRes(m, IMAGE_RESOLUTION_STANDARD);
			ImageInsert(id, m);
			ii++;
		}
	}
}

void IconDes::ExportPngs()
{
	Ткст dir = SelectDirectory();
	if(!dir.пустой())
		for(int i = 0; i < дайСчёт(); i++) {
			dword f = GetFlags(i);
			Ткст n = дайИмя(i);
			if(f & IML_IMAGE_FLAG_UHD)
				n << ".uhd";
			if(f & IML_IMAGE_FLAG_DARK)
				n << ".dark";
			PNGEncoder().сохраниФайл(приставьИмяф(dir, n + ".png"), GetImage(i));
		}
}

void IconDes::InsertIml()
{
	Массив<ImlImage> iml;
	int f;
	if(LoadIml(SelectLoadFile("Iml files\t*.iml"), iml, f))
		for(const ImlImage& m : iml) {
			ImageInsert(m.имя, m.image, m.exp).flags = m.flags;
			GoTo((int)ilist.дайКлюч() + 1);
		}
}

void IconDes::ListCursor()
{
	SyncImage();
}

void IconDes::EditImageSize()
{
	Слот& c = Current();
	WithImageSizeLayout<ТопОкно> dlg;
	PrepareImageSizeDlg(dlg);
	dlg.Breaker(dlg.cx);
	dlg.Breaker(dlg.cy);
	Рисунок img = c.image;
	dlg.cx <<= img.дайШирину();
	dlg.cy <<= img.дайВысоту();
	for(;;) {
		switch(dlg.пуск()) {
		case IDCANCEL:
			c.image = img;
			переустанов();
			return;
		case IDOK:
			переустанов();
			SyncList();
			return;
		}
		c.image = CreateImage(Размер(minmax((int)~dlg.cx, 1, 8192), minmax((int)~dlg.cy, 1, 8192)), Null);
		РНЦП::копируй(c.image, Точка(0, 0), img, img.дайРазм());
		переустанов();
	}
}

void IconDes::EditImage()
{
	if(!IsCurrent())
		return;
	if(single_mode) {
		EditImageSize();
		return;
	}
	Слот& c = Current();
	WithImageLayout<ТопОкно> dlg;
	PrepareImageDlg(dlg);
	dlg.Титул("Рисунок");
	dlg.Breaker(dlg.cx);
	dlg.Breaker(dlg.cy);
	Рисунок img = c.image;
	dword flags = c.flags;
	bool exp = c.exp;
	Ткст имя = c.имя;
	
	dlg.cx <<= img.дайШирину();
	dlg.cy <<= img.дайВысоту();
	dlg.имя <<= c.имя;
	dlg.exp <<= c.exp;
	for(;;) {
		SyncDlg(dlg);
		switch(dlg.пуск()) {
		case IDCANCEL:
			c.image = img;
			c.flags = flags;
			c.exp = exp;
			c.имя = имя;
			переустанов();
			return;
		case IDOK:
			if(CheckName(dlg)) {
				c.имя = ~dlg.имя;
				c.exp = ~dlg.exp;
				c.flags = GetFlags(dlg);
				ilist.уст(1, FormatImageName(c));
				int q = ilist.дайКлюч();
				переустанов();
				SyncList();
				GoTo(q);
				return;
			}
		}
		c.имя = ~dlg.имя;
		c.flags = GetFlags(dlg);
		c.image = CreateImage(Размер(minmax((int)~dlg.cx, 1, 8192), minmax((int)~dlg.cy, 1, 8192)), Null);
		c.exp = ~dlg.exp;
		РНЦП::копируй(c.image, Точка(0, 0), img, img.дайРазм());
		SetHotSpots(c.image, img.GetHotSpot(), img.Get2ndSpot());
		переустанов();
	}
}

void IconDes::RemoveImage()
{
	if(!IsCurrent() || !PromptYesNo("удали current image?"))
		return;
	int ii = ilist.дайКлюч();
	while(removed.дайСчёт() > 12)
		removed.удали(0);
	Слот& r = removed.добавь();
	r = slot[ii];
	if(r.image.дайШирину() <= 128 && r.image.дайВысоту() <= 128)
		r.base_image = Rescale(r.image, Размер(16, 16));
	else
		r.base_image = IconDesImg::LargeImage();
	slot.удали(ii);
	ilist.анулируйКурсор();
	SyncList();
	if(ii < slot.дайСчёт())
		GoTo(ii);
	else
		ilist.идиВКон();
}

void IconDes::ChangeSlot(int d)
{
	if(!IsCurrent())
		return;
	int c = ilist.дайКурсор();
	d = c + d;
	if(d >= 0 && d < ilist.дайСчёт())
		ilist.устКурсор(d);
}

void IconDes::ListMenu(Бар& bar)
{
	using namespace IconDesKeys;
	if(single_mode)
		bar.добавь(IsCurrent(), AK_RESIZE_SINGLE, IconDesImg::Edit(), THISBACK(EditImage));
	else {
		bar.добавь(AK_INSERT_IMAGE, IconDesImg::Insert(), THISBACK(InsertImage));
		bar.добавь(IsCurrent(), AK_IMAGE, IconDesImg::Edit(), THISBACK(EditImage));
		bar.добавь(IsCurrent(), AK_REMOVE_IMAGE, IconDesImg::Remove(), THISBACK(RemoveImage));
		bar.добавь(IsCurrent(), AK_DUPLICATE, IconDesImg::Duplicate(), THISBACK(Duplicate));
		bar.добавь(AK_INSERT_CLIP, IconDesImg::InsertPaste(), THISBACK(InsertPaste));
		bar.добавь(AK_INSERT_FILE, IconDesImg::InsertFile(), THISBACK(InsertFile));
		bar.добавь(AK_INSERT_IML, IconDesImg::InsertIml(), THISBACK(InsertIml));
		bar.добавь(AK_EXPORT_PNGS, IconDesImg::ExportPngs(), THISBACK(ExportPngs));
		bar.Separator();
		int q = ilist.дайКлюч();
		bar.добавь(IsCurrent() && q > 0, AK_MOVE_UP, IconDesImg::MoveUp(),
		        THISBACK1(MoveSlot, -1));
		bar.добавь(IsCurrent() && q < slot.дайСчёт() - 1, AK_MOVE_DOWN, IconDesImg::MoveDown(),
		        THISBACK1(MoveSlot, 1));
		if(removed.дайСчёт()) {
			bar.Separator();
			for(int i = removed.дайСчёт() - 1; i >= 0; i--) {
				Слот& r = removed[i];
				bar.добавь("Вставить " + FormatImageName(r), r.base_image, THISBACK1(InsertRemoved, i));
			}
		}
	}
	bar.Separator();
	EditBar(bar);
	ListMenuEx(bar);
}

void IconDes::ListMenuEx(Бар& bar) {}


void IconDes::очисть()
{
	ilist.очисть();
	slot.очисть();
	переустанов();
}

IconDes::Слот& IconDes::добавьРисунок(const Ткст& имя, const Рисунок& image, bool exp)
{
	int q = slot.дайСчёт();
	Слот& c = slot.добавь();
	c.имя = имя;
	c.image = image;
	c.exp = exp;
	ilist.добавь(q, FormatImageName(c), c.image);
	ilist.идиВНач();
	return c;
}

int IconDes::дайСчёт() const
{
	return slot.дайСчёт();
}

Рисунок IconDes::GetImage(int ii) const
{
	return slot[ii].image;
}

Ткст IconDes::дайИмя(int ii) const
{
	return slot[ii].имя;
}

dword IconDes::GetFlags(int ii) const
{
	return slot[ii].flags;
}

bool IconDes::FindName(const Ткст& имя)
{
	for(int i = 0; i < slot.дайСчёт(); i++)
		if(slot[i].имя == имя) {
			GoTo(i);
			return true;
		}
	return false;
}

Ткст IconDes::GetCurrentName() const
{
	if(ilist.курсор_ли())
		return дайИмя(ilist.дайКлюч());
	return Ткст();
}

bool IconDes::GetExport(int ii) const
{
	return slot[ii].exp;
}

void IconDes::MoveSlot(int d)
{
	if(!IsCurrent())
		return;
	int c = ilist.дайКлюч();
	d = c + d;
	if(d >= 0 && d < slot.дайСчёт()) {
		slot.разверни(c, d);
		search <<= Null;
		SyncList();
		GoTo(d);
	}
}

void IconDes::вставьТиБ(int line, PasteClip& d)
{
	if(GetInternalPtr<КтрлМассив>(d, "icondes-icon") == &ilist && IsCurrent() &&
	   line >= 0 && line <= slot.дайСчёт() && d.прими()) {
		int c = ilist.дайКлюч();
		slot.перемести(c, line);
		if(c <= line)
			line--;
		search <<= Null;
		SyncList();
		GoTo(line);
	}
}

void IconDes::тяни()
{
	ilist.DoDragAndDrop(InternalClip(ilist, "icondes-icon"), ilist.дайСэиплТяга(), DND_MOVE);
}

}
