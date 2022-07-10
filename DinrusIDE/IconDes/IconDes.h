#ifndef _IconDes_IconDes_h_
#define _IconDes_IconDes_h_

#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>
#include <RichEdit/RichEdit.h>
#include <plugin/bmp/bmp.h>

namespace РНЦП {

#define IMAGECLASS IconDesImg
#define IMAGEFILE <IconDes/IconDes.iml>
#include <Draw/iml_header.h>

#define LAYOUTFILE <IconDes/IconDes.lay>
#include <CtrlCore/lay.h>

#define KEYNAMESPACE IconDesKeys
#define KEYGROUPNAME "Иконка designer"
#define KEYFILE      <IconDes/IconDes.key>
#include             <CtrlLib/key_header.h>

class AlphaCtrl : public Ктрл {
public:
	virtual void Выкладка();
	virtual void рисуй(Draw& w);
	virtual void леваяВнизу(Точка p, dword keyflags);
	virtual void двигМыши(Точка p, dword keyflags);

private:
	Рисунок img;
	Цвет color;
	int   alpha;
	bool  mask;
	int   maskvalue;

	void MakeImage();
	void DoPoint(Точка p);

public:
	void устЦвет(Цвет c);
	void уст(int a);
	int  дай() const;
	void Mask(bool m);
	bool IsMask() const                   { return mask; }

	AlphaCtrl();
};

class RGBACtrl : public Ктрл {
public:
	virtual void рисуй(Draw& w);
	virtual void Выкладка();
	virtual void леваяВнизу(Точка p, dword keyflags);
	virtual void двигМыши(Точка p, dword keyflags);

private:
	Размер  cbox;
	Точка cs;
	enum { COLORCOUNT = 216 + 18 + 18 };

	Цвет         color;
	ColorRampCtrl ramp;
	AlphaCtrl     alpha;
	EditString    text;
	ФреймПраво<Кнопка> setcolor;
	Ук<Ктрл>     subctrl;

	Цвет дайЦвет(int i) const;
	void  Ramp();
	void  Alpha();
	void  SyncText();
	int   DoLayout(Размер sz, bool set);

public:
	typedef RGBACtrl ИМЯ_КЛАССА;

	void уст(КЗСА c);
	void MaskSet(int a);
	КЗСА дай() const;
	
	Цвет дайЦвет() const;
	int   GetAlpha() const;

	void Mask(bool b);

	void SubCtrl(Ктрл *c);
	
	int  дайВысоту(int cx);

	RGBACtrl();
};

struct IconShow : public Ктрл {
	Рисунок image;
	bool  show_small;
	bool  show_other;

	void рисуй(Draw& w);

	IconShow() { BackPaint(); }
};

void   FloodFill(ImageBuffer& img, КЗСА color, Точка pt, const Прям& rc, int tolerance);
void   InterpolateImage(Рисунок& img, const Прям& _rc);
void   MirrorHorz(Рисунок& img, const Прям& rect);
void   MirrorVert(Рисунок& img, const Прям& rect);
Ткст PackImlData(const Вектор<ImageIml>& image);
Рисунок  DownSample3x(const Рисунок& src);
Рисунок  DownSample2x(const Рисунок& src);

/*
struct IconDraw : ImagePainter {
	IconDraw(Размер sz) : ImagePainter(sz, MODE_NOAA) {}
};
*/

struct IconDraw : NilDraw, DDARasterizer {
	КЗСА        docolor;
	ImageBuffer image;
	
	virtual void PutHorz(int x, int y, int cx);
	virtual void PutVert(int x, int y, int cy);

	virtual void DrawRectOp(int x, int y, int cx, int cy, Цвет color);
	virtual void DrawLineOp(int x1, int y1, int x2, int y2, int width, Цвет color);
	virtual void DrawEllipseOp(const Прям& r, Цвет color, int pen, Цвет pencolor);
	
	operator Рисунок() { return image; }
	
	IconDraw(Размер sz) { image.создай(sz); Cy(sz.cy); }
};

class IconDes : public Ктрл {
public:
	virtual void  Выкладка();
	virtual void  рисуй(Draw& w);
	virtual void  леваяВнизу(Точка p, dword keyflags);
	virtual void  двигМыши(Точка p, dword keyflags);
	virtual void  входМыши(Точка, dword)                 { SyncStatus(); }
	virtual void  выходМыши()                             { SyncStatus(); }
	virtual void  леваяВверху(Точка p, dword keyflags);
	virtual void  колесоМыши(Точка p, int zdelta, dword keyflags);
	virtual void  праваяВнизу(Точка p, dword keyflags);
	virtual bool  Ключ(dword ключ, int count);
	virtual Рисунок рисКурсора(Точка p, dword keyflags);

private:
	struct Слот {
		Ткст          имя;
		Рисунок           image;
		Рисунок           base_image;
		Рисунок           selection;
		Точка           pastepos;
		Рисунок           paste_image;
		Ткст          undo;
		Ткст          redo;
		bool            exp;
		dword           flags = 0;

		Слот();
	};
	
	enum {
		PASTE_TRANSPARENT, PASTE_OPAQUE, PASTE_BACK
	};

	Массив<Слот>  slot;
	Слот         dummy;

	int          magnify;
	int          pen;
	Точка        startpoint;
	Прям         m1refresh;
	void        (IconDes::*tool)(Точка p, dword flags);
	bool         doselection = false;
	bool         selectrect = false;
	int          paste_mode;
	bool         show_other = false;
	bool         show_small = false;

	ПрокрутБары   sb;
	Промотчик     scroller;
	ToolBar      toolbar;

	SplitterFrame  leftpane;
	SplitterFrame  bottompane;

	КтрлРодитель     imgs;
	КтрлМассив      ilist;
	EditString     search;

	RGBACtrl       rgbactrl;
	IconShow       iconshow;
	Рисунок          cursor_image;
	Рисунок          fill_cursor, fill_cursor2, fill_cursor3, antifill_cursor;
	bool           single_mode;
	КтрлРодитель     single;
	Надпись          info;
	Кнопка         resize;

	Массив<Слот>    removed;

	ФреймПраво<Надпись> status;

	int syncinglist = 0;
	
	Прям           rect;

	struct TextDlg : WithIconDesTextLayout<ТопОкно> {
		typedef TextDlg ИМЯ_КЛАССА;
		
		Шрифт дайШрифт();
		
		TextDlg();
	};
	
	TextDlg        textdlg;

	void  PenSet(Точка p, dword flags);

	void  LineTool(Точка p, dword f);
	void  FreehandTool(Точка p, dword f);

	void  EllipseTool0(Точка p, dword flags, Цвет inner);
	void  EllipseTool(Точка p, dword f);
	void  EmptyEllipseTool(Точка p, dword f);

	void  RectTool0(Точка p, dword f, bool empty);
	void  RectTool(Точка p, dword f);
	void  EmptyRectTool(Точка p, dword f);

	void  HotSpotTool(Точка p, dword f);

	void  DoFill(int tolerance);
	void  FillTool(Точка p, dword flags);
	void  Fill2Tool(Точка p, dword flags);
	void  Fill3Tool(Точка p, dword flags);
	void  AntiFillTool(Точка p, dword flags);

	void  устТекст();
	void  PasteText();
	void  CloseText();

	bool         IsCurrent()            { return !пусто_ли(ilist.дайКлюч()); }
	Слот&        Current();

	Рисунок&       CurrentImage();
	void         SetCurrentImage(ImageBuffer& ib);
	КЗСА         CurrentColor();
	Размер         GetImageSize();
	bool         InImage(int x, int y);
	bool         InImage(Точка p)       { return InImage(p.x, p.y); }


	bool  IsHotSpot() const             { return tool == &IconDes::HotSpotTool; }

	Рисунок MakeIconDesCursor(const Рисунок& arrow, const Рисунок& cmask);

	void  SyncShow();

	void  RefreshPixel(Точка p, int cx = 1, int cy = 1);
	void  RefreshPixel(int x, int y, int cx = 1, int cy = 1);
	Точка дайПоз(Точка p);
	void  уст(Точка p, КЗСА rgba, dword flags);
	void  ApplyDraw(IconDraw& iw, dword flags);
	void  ApplyImage(Рисунок m, dword flags, bool alpha = false);

	void  SyncImage();
	void  переустанов();

	bool  IsPasting()                   { return IsCurrent() && !пусто_ли(Current().pastepos); }
	void  MakePaste();
	void  Paste(const Рисунок& img);
	void  FinishPaste();

	void  SetSelect(int a);
	Прям  SelectionRect();
	void  выдели();
	void  InvertSelect();
	void  CancelSelect();
	void  SelectRect();
	Рисунок копируй(const Прям& r);
	void  Delete();
	void  Move();
	void  MaskSelection();

	void  SetSb();
	void  промотай();

	void  SetTool(void (IconDes::*tool)(Точка p, dword flags));

	void  SetMagnify(int mag);
	void  ZoomIn();
	void  ZoomOut();

	void  SetPen(int pen);

	void  MainToolBar(Бар& bar);
	void  SetBar();

	void  SaveUndo();
	void  Undo();
	void  Redo();

	void  DoPaste();
	void  DoCopy();
	void  DoCut();

	void  ColorChanged();

	void  MaskFill(ImageBuffer& ib, КЗСА color, int mask);
	void  SetColor0(КЗСА color);
	void  устЦвет();
	void  DoDelete();
	void  Interpolate();
	bool  BeginTransform();
	void  MirrorX();
	void  MirrorY();
	void  SymmX();
	void  SymmY();
	void  FreeRotate();
	void  Rotate();
	void  SmoothRescale();
	void  KeyMove(int dx, int dy);

	void  BeginResize();
	void  ResizeUp();
	void  ResizeDown();
	void  ResizeUp2();
	void  ResizeDown2();
	void  Upscale();

	void  PlaceDlg(ТопОкно& dlg);
	Рисунок ImageStart();
	void  ImageSet(const Рисунок& m);
	void  BlurSharpen();
	void  Colorize();
	void  Chroma();
	void  Contrast();
	void  Alpha();
	void  Colors();
	void  Smoothen();

	void  ищи();
	void  GoTo(int q);
	void  SyncList();
	void  ListCursor();
	void  PrepareImageDlg(WithImageLayout<ТопОкно>& dlg);
	void  PrepareImageSizeDlg(WithImageSizeLayout<ТопОкно>& dlg);
	void  SyncDlg(WithImageLayout<ТопОкно>& dlg);
	dword GetFlags(WithImageLayout<ТопОкно>& dlg);
	Слот& ImageInsert(int ii, const Ткст& имя, const Рисунок& m, bool exp = false);
	Слот& ImageInsert(const Ткст& имя, const Рисунок& m, bool exp = false);
	void  Slice();
	void  InsertImage();
	void  InsertRemoved(int ii);
	void  EditImageSize();
	void  EditImage();
	void  RemoveImage();
	void  Duplicate();
	void  InsertPaste();
	void  InsertFile();
	void  ExportPngs();
	void  InsertIml();
	void  MoveSlot(int d);
	void  ChangeSlot(int d);
	void  вставьТиБ(int line, PasteClip& d);
	void  тяни();

	static FileSel& ImgFile();
	static Ткст FormatImageName(const Слот& c);

public:
	virtual void ToolEx(Бар& bar);
	virtual void ListMenuEx(Бар& bar);

	void  ListMenu(Бар& bar);
	void  EditBar(Бар& bar);
	void  SettingBar(Бар& bar);
	void  SelectBar(Бар& bar);
	void  ImageBar(Бар& bar);
	void  DrawBar(Бар& bar);

	struct EditPosSlot : Движимое<EditPosSlot> {
		Ткст          undo;
		Ткст          redo;
		Ткст          selection;
		bool            supersampling;
	};

	struct ПозРедакт {
		Вектор<EditPosSlot> slot;
		int                 cursor, sc;
	};

	void   очисть();
	Слот&  добавьРисунок(const Ткст& имя, const Рисунок& image, bool exp);
	int    дайСчёт() const;
	Рисунок  GetImage(int ii) const;
	Ткст дайИмя(int ii) const;
	bool   GetExport(int ii) const;
	dword  GetFlags(int ii) const;
	bool   FindName(const Ткст& имя);

	Ткст GetCurrentName() const;
	
	void    SyncStatus();

	ПозРедакт GetEditPos();
	void    SetEditPos(const ПозРедакт& o);

	void    сериализуйНастройки(Поток& s);
	
	void    SingleMode();
	bool    IsSingleMode() const                  { return single_mode; }

	typedef IconDes ИМЯ_КЛАССА;

	IconDes();
};

struct ImlImage : ImageIml {
	Ткст имя;
	bool   exp;
};

bool   LoadIml(const Ткст& data, Массив<ImlImage>& img, int& формат);
Ткст SaveIml(const Массив<ImlImage>& iml, int формат, const Ткст& eol = "\r\n");

void SetRes(Рисунок& m, int resolution);

}

#endif
