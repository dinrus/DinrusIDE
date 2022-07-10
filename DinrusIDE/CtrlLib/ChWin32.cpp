#include "CtrlLib.h"

#ifdef GUI_WIN

#define LLOG(x)   // RLOG(x)
#define LTIMING(x) // RTIMING(x)

#include <uxtheme.h>
#if defined(_MSC_VER) && _MSC_VER > 1400	// Visual C > 2005
#include <vssym32.h>
#else
#include <tmschema.h>
#endif

namespace РНЦП {

#define DLLFILENAME "uxtheme.dll"
#define МОДУЛЬДЛИ   XpTheme
#define DLIHEADER   <CtrlLib/XPTheme.dli>
#define DLLCALL     STDAPICALLTYPE
#define WIDGET(x)
#include <Core/dli.h>
#undef  WIDGET

#define FN(a, b, c)
#define WIDGET(x)    XP_##x,
enum {
#include "XPTheme.dli"
	XP_COUNT
};

#undef  WIDGET
#define WIDGET(x)   #x,

static const char *xp_widget_name[] = {
#include "XPTheme.dli"
	NULL
};

#undef  WIDGET
#undef  FN

struct XpElement : Движимое<XpElement> {
	int16 widget;
	int8  part;
	int8  state;

	bool  whista;
	bool  contentm;

	bool  operator==(const XpElement& e) const {
		return e.widget == widget && e.part == part && e.state == state;
	}

	hash_t дайХэшЗнач() const { return widget ^ part ^ state; }

	XpElement() { contentm = whista = false; }
};

static HANDLE xp_widget_handle[XP_COUNT];
static ВекторМап<XpElement, int> xp_margin;
static ВекторМап<XpElement, int> xp_opaque;

void XpClear()
{
	memset(xp_widget_handle, 0, sizeof(xp_widget_handle));
	xp_margin.очисть();
	xp_opaque.очисть();
}

HANDLE XpWidget(int widget)
{
	if(xp_widget_handle[widget] == NULL && XpTheme())
		xp_widget_handle[widget] = XpTheme().OpenThemeData(NULL,
		                            вСисНабсимШ(xp_widget_name[widget]));
	return xp_widget_handle[widget];
}

Рисунок XpImage0(int widget, int part, int state, Цвет color, Размер sz)
{
	HANDLE theme = XpWidget(widget);
	if(!theme)
		return Null;
	if(пусто_ли(sz))
		XpTheme().GetThemePartSize(theme, ScreenHDC(), part, state, NULL,
		                           1 /*TS_TRUE*/, sz);
	Цвет c = белый;
	Рисунок m[2];
	for(int q = 0; q < 2; q++) {
		ImageDraw iw(sz);
		iw.DrawRect(sz, Nvl(color, c));
		HDC hdc = iw.BeginGdi();
		Прям r(sz);
		XpTheme().DrawThemeBackground(theme, hdc, part, state, r, NULL);
		iw.EndGdi();
		m[q] = iw;
		if(!пусто_ли(color))
			return m[q];
		c = чёрный;
	}
	return RecreateAlpha(m[0], m[1]);
}

bool sEmulateDarkTheme;

Цвет sAdjust(Цвет c)
{
	return sEmulateDarkTheme ? тёмнаяТема(c) : c;
}

Рисунок XpImage(int widget, int part, int state, Цвет color = Null, Размер sz = Null)
{
	Рисунок m = XpImage0(widget, part, state, color, sz);
	return sEmulateDarkTheme ? тёмнаяТема(m) : m;
}

bool XpIsAvailable(int widget, int part, int state)
{
	HANDLE theme = XpWidget(widget);
	if(!theme)
		return false;
	return XpTheme().IsThemePartDefined(theme, part, state);
}

int XpMargin(const XpElement& e)
{
	int q = xp_margin.найди(e);
	if(q >= 0)
		return xp_margin[q];

	q = ImageMargin(XpImage(e.widget, e.part, e.state, белый, Размер(30, 30)), 4, 10);
	xp_margin.добавь(e, q);
	return q;
}

Цвет XpColor(int widget, int part, int state, int тип)
{
	COLORREF cref = 0;
	HANDLE theme = XpWidget(widget);
	if(!theme)
		return Null;
	int r = XpTheme().GetThemeColor(theme, part, state, тип, &cref);
	return r == S_OK ? sAdjust(Цвет::FromCR(cref)) : Null;
}

int XpInt(int widget, int part, int state, int тип)
{
	HANDLE theme = XpWidget(widget);
	if(!theme)
		return Null;
	int n = Null;
	int r = XpTheme().GetThemeInt(theme, part, state, тип, &n);
	return r == S_OK ? n : Null;
}

int XpBool(int widget, int part, int state, int тип)
{
	HANDLE theme = XpWidget(widget);
	if(!theme)
		return Null;
	BOOL flag = false;
	int r = XpTheme().GetThemeBool(theme, part, state, тип, &flag);
	return r == S_OK ? flag : Null;
}

void SetXpImages(int uii, int n, int widget, int part, int state)
{
	for(int i = 0; i < n; i++) {
		Рисунок m = XpImage(widget, part, state++);
		if(!пусто_ли(m))
			CtrlsImg::уст(uii++, m);
	}
}

void SetXpColors(Цвет *color, int n, int widget, int part, int state, int тип)
{
	int ii = 0;
	for(int i = 0; i < n; i++) {
		Цвет c = XpColor(widget, part, state++, тип);
		if(!пусто_ли(c))
			color[ii++] = c;
	}
}

struct Win32ImageMaker : ImageMaker {
	int        widget;
	int        part;
	int        state;
	Размер       sz;

	virtual Ткст Ключ() const {
		ТкстБуф ключ;
		RawCat(ключ, widget);
		RawCat(ключ, part);
		RawCat(ключ, state);
		RawCat(ключ, sz);
		return Ткст(ключ);
	}
	virtual Рисунок сделай() const {
		Прям rr(sz);
		Рисунок m[2];
		for(int i = 0; i < 2; i++) {
			ImageDraw iw(sz);
			iw.DrawRect(sz, i ? чёрный() : белый());
			XpTheme().DrawThemeBackground(XpWidget(widget), iw.BeginGdi(), part, state, rr, NULL);
			iw.EndGdi();
			m[i] = iw;
		}
		return тёмнаяТема(RecreateAlpha(m[0], m[1]));
	}
};

Значение XpLookFn(Draw& w, const Прям& rect, const Значение& v, int op, Цвет)
{
	if(IsTypeRaw<XpElement>(v)) {
		const XpElement& e = ValueTo<XpElement>(v);
		HANDLE htheme = XpWidget(e.widget);
		int q;
		switch(op) {
		case LOOK_MARGINS:
			if(e.contentm && htheme) {
				Прям r(0, 0, 100, 100);
				Прям cr;
				XpTheme().GetThemeBackgroundContentRect(htheme, NULL, e.part, e.state, r, cr);
				cr.bottom = 100 - cr.bottom;
				cr.right = 100 - cr.right;
				return cr;
			}
			q = XpMargin(e);
			return Прям(q, q, q, q);
		}
		if(op == LOOK_PAINT || op == LOOK_PAINTEDGE) {
			LTIMING("XpPaint");
			Прям r = rect;
			if(op == LOOK_PAINTEDGE) {
				q = XpMargin(e);
				w.старт();
				w.ExcludeClip(r.left + q, r.top + q,
				              r.устШирину() - 2 * q, r.устВысоту() - 2 * q);
			}
			if(e.whista) {
				w.Clip(r);
				r.right++;
				r.top--;
				r.bottom++;
			}
			if(htheme) {
				if(sEmulateDarkTheme) {
					Win32ImageMaker m;
					m.widget = e.widget;
					m.part = e.part;
					m.state = e.state;
					m.sz = r.дайРазм();
					w.DrawImage(r.left, r.top, MakeImage(m));
				}
				else {
					SystemDraw *sw = dynamic_cast<SystemDraw *>(&w);
					if(sw) {
						HDC hdc = sw->BeginGdi();
						XpTheme().DrawThemeBackground(htheme, hdc, e.part, e.state, r, NULL);
						sw->EndGdi();
					}
				}
			}
			if(e.whista)
				w.стоп();
			if(op == LOOK_PAINTEDGE)
				w.стоп();
			return 1;
		}
	}
	return Null;
}

struct chCtrlsImg {
	int id;
	int count;
	int widget;
	int part;
	int state;
};

static chCtrlsImg sImgs[] = {
	{ CtrlsImg::I_SizeGrip, 1, XP_STATUS, SP_GRIPPER, 0 },
	{ CtrlsImg::I_S0, 4, XP_BUTTON, BP_RADIOBUTTON, RBS_UNCHECKEDNORMAL },
	{ CtrlsImg::I_S1, 4, XP_BUTTON, BP_RADIOBUTTON, RBS_CHECKEDNORMAL },
	{ CtrlsImg::I_O0, 4, XP_BUTTON, BP_CHECKBOX, CBS_UNCHECKEDNORMAL },
	{ CtrlsImg::I_O1, 4, XP_BUTTON, BP_CHECKBOX, CBS_CHECKEDNORMAL },
	{ CtrlsImg::I_O2, 4, XP_BUTTON, BP_CHECKBOX, CBS_MIXEDNORMAL },
};

void Win32Look(Значение *ch, int count, int widget, int part, int state = 1, bool contentm = false)
{
	for(int i = 0; i < count; i++) {
		XpElement e;
		e.widget = widget;
		e.part = part;
		e.state = state + i;
		e.contentm = contentm;
		ch[i] = RawToValue(e);
	}
}

void Win32Look(Значение& ch, int widget, int part, int state = 1, bool contentm = false)
{
	Win32Look(&ch, 1, widget, part, state, contentm);
}

Ткст XpThemeInfo(LPCWSTR pszPropertyName)
{
	WCHAR theme[512], colors[512], size[512];
	XpTheme().GetCurrentThemeName(theme, 512, colors, 512, size, 512);
	WCHAR h[1024];
	XpTheme().GetThemeDocumentationProperty(theme, pszPropertyName, h, 1000);
	return вУтф8(h);
}

struct sysColor {
	void (*set)(Цвет c);
	int syscolor;
};

static sysColor sSysColor[] = {
	{ SColorPaper_Write, COLOR_WINDOW },
	{ SColorFace_Write, COLOR_3DFACE },
	{ SColorText_Write, COLOR_WINDOWTEXT },
	{ SColorHighlight_Write, COLOR_HIGHLIGHT },
	{ SColorHighlightText_Write, COLOR_HIGHLIGHTTEXT },
	{ SColorMenu_Write, COLOR_3DFACE },
/*
	{ SColorMenu_Write, COLOR_MENU },
	{ SColorMenuText_Write, COLOR_MENUTEXT },
	{ SColorInfo_Write, COLOR_INFOBK },
	{ SColorInfoText_Write, COLOR_INFOTEXT },
	{ SColorLight_Write, COLOR_3DHILIGHT },
	{ SColorShadow_Write, COLOR_3DSHADOW },
*/
};

bool IsSysFlag(dword flag)
{
	BOOL b;
	return SystemParametersInfo(flag, 0, &b, 0) && b;
}

void ToImageIfDark(Значение& v)
{ // Optimize performance - ChPaint will pick single-color body and use DrawRect
	if(sEmulateDarkTheme) {
		ImageDraw iw(64, 64);
		ChPaint(iw, Размер(64, 64), v);
		Рисунок m = iw;
		v = WithHotSpot(m, 8, 8);
	}
}

bool IsSystemThemeDark()
{
	Ткст s = дайТкстВинРега("AppsUseLightTheme", "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", HKEY_CURRENT_USER);
	bool b = s.дайСчёт() && *s == 0;
	return дайСреду("UPP_DARKMODE__") == "1" ? !b : b;
}

void ChHostSkin()
{
	if(Ктрл::IsUHDEnabled()) {
		HRESULT (STDAPICALLTYPE *SetProcessDpiAwareness)(int);
		фнДлл(SetProcessDpiAwareness, "Shcore.dll", "SetProcessDpiAwareness");
		if(SetProcessDpiAwareness)
			SetProcessDpiAwareness(1);
		else {
			BOOL (STDAPICALLTYPE * SetProcessDPIAware)(void);
			фнДлл(SetProcessDPIAware, "User32.dll", "SetProcessDPIAware");
			if(SetProcessDPIAware)
				(*SetProcessDPIAware)();
		}
	}

	sEmulateDarkTheme = Ктрл::IsDarkThemeEnabled() && IsSystemThemeDark() && !тёмен(Цвет::FromCR(GetSysColor(COLOR_WINDOW)));

	NONCLIENTMETRICSW ncm;
#if (WINVER >= 0x0600 && !defined(__MINGW32_VERSION))
	ncm.cbSize = sizeof(ncm) - sizeof(ncm.iPaddedBorderWidth); // WinXP does not like it...
#else
	ncm.cbSize = sizeof(ncm);
#endif
	::SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);
	Ткст имя = изСисНабсимаШ(ncm.lfMenuFont.lfFaceName);
	int height = абс((int)ncm.lfMenuFont.lfHeight);
	int q = Шрифт::FindFaceNameIndex(имя);
	if(height > 0 && height < 200) // sanity..
		Шрифт::SetDefaultFont(Шрифт(q >= 0 ? q : Шрифт::SANSSERIF, height));

	XpClear();
	 
	if(sEmulateDarkTheme){
		SColorPaper_Write(Цвет(0x19, 0x19, 0x19));
		SColorFace_Write(Цвет(0x20, 0x20, 0x20));
		SColorText_Write(белый());
		SColorMenu_Write(SColorFace());
		SColorLight_Write(FaceColor(0x20));
		SColorShadow_Write(FaceColor(0x20));
		SColorHighlight_Write(Цвет::FromCR(GetSysColor(COLOR_HIGHLIGHT)));
		SColorHighlightText_Write(Цвет::FromCR(GetSysColor(COLOR_HIGHLIGHTTEXT)));
		SColorMenuMark_Write(Цвет(0x80, 0x80, 0x80));
		DropEdge_Write(Цвет(0x80, 0x80, 0x80));
	}
	else for(sysColor *s = sSysColor; s < sSysColor + __countof(sSysColor); s++) // this also resets all imls via SColorPaper_Write!!!
		(*s->set)(sAdjust(Цвет::FromCR(GetSysColor(s->syscolor))));

	ChBaseSkin();

	GUI_GlobalStyle_Write(GUISTYLE_XP);
#ifndef PLATFORM_WINCE
	GUI_DragFullWindow_Write(IsSysFlag(SPI_GETDRAGFULLWINDOWS));
#endif
	GUI_PopUpEffect_Write(IsSysFlag(0x1002 /*SPI_GETMENUANIMATION*/) ?
	                      IsSysFlag(0x1012 /*SPI_GETMENUFADE*/) ? GUIEFFECT_FADE
	                                                            : GUIEFFECT_SLIDE
	                                                            : GUIEFFECT_NONE);
	GUI_DropShadows_Write(IsSysFlag(0x1024 /*SPI_GETDROPSHADOW*/));
	GUI_AltAccessKeys_Write(!IsSysFlag(0x100A /*SPI_GETKEYBOARDCUES*/));
	GUI_AKD_Conservative_Write(0);
	GUI_DragDistance_Write(GetSystemMetrics(SM_CXDRAG));
	GUI_DblClickTime_Write(GetDoubleClickTime());

	int slines;
	SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &slines, 0);
	GUI_WheelScrollLines_Write(slines);

	CtrlImg::уст(CtrlImg::I_information, Win32Icon(IDI_INFORMATION));
	CtrlImg::уст(CtrlImg::I_question, Win32Icon(IDI_QUESTION));
	CtrlImg::уст(CtrlImg::I_exclamation, Win32Icon(IDI_EXCLAMATION));
	CtrlImg::уст(CtrlImg::I_error, Win32Icon(IDI_ERROR));

	FrameButtonWidth_Write(GetSystemMetrics(SM_CYHSCROLL));
	ScrollBarArrowSize_Write(GetSystemMetrics(SM_CXHSCROLL));

	if(!sEmulateDarkTheme){
		dword x = GetSysColor(COLOR_GRAYTEXT);
		SColorDisabled_Write(sAdjust(x ? Цвет::FromCR(x) : Цвет::FromCR(GetSysColor(COLOR_3DSHADOW))));
	}
	else SColorDisabled_Write(Цвет(0x80, 0x80, 0x80));

	ChLookFn(XpLookFn);

	if(XpWidget(XP_BUTTON)) {
		GUI_GlobalStyle_Write(GUISTYLE_XP);
		EditFieldIsThin_Write(1);

		bool vista_aero = IsWinVista() && XpThemeInfo(L"ThemeName") == "Aero";

		if(vista_aero) {
			int efp = 6;
			for(int i = 0; i < 4; i++) {
				int efs = i + 1;
				int ebsx = max(2, XpInt(XP_EDIT, efp, efs, 2403/*TMT_BORDERSIZE*/));
				int ebsy = max(1, XpInt(XP_EDIT, efp, efs, 2403/*TMT_BORDERSIZE*/));
				Рисунок ee = XpImage(XP_EDIT, efp, efs, SColorFace(), Размер(10 * ebsx, 10 * ebsx));
				ImageBuffer eb(ee);
				eb.SetHotSpot(Точка(ebsx, ebsy));
				ee = eb;
				EditField::Стиль& s = EditField::дефСтиль().пиши();
				s.activeedge = true;
				if(!sEmulateDarkTheme) s.edge[i] = ee;
				else s.edge[i] = SColorShadow();
			}
		}
		else {
			int efp = EP_EDITTEXT;
			int efs = 1;
			int ebsx = max(2, XpInt(XP_EDIT, efp, efs, 2403/*TMT_BORDERSIZE*/));
			int ebsy = max(1, XpInt(XP_EDIT, efp, efs, 2403/*TMT_BORDERSIZE*/));
			Рисунок ee = XpImage(XP_EDIT, efp, efs, SColorFace(), Размер(10 * ebsx, 10 * ebsx));
			ImageBuffer eb(ee);
			eb.SetHotSpot(Точка(ebsx, ebsy));
			ee = eb;
			EditField::Стиль& s = EditField::дефСтиль().пиши();
			s.activeedge = false;
			s.edge[0] = ee;
		}

		int ebs = max(2, XpInt(XP_LISTVIEW, 0, 1, 2403/*TMT_BORDERSIZE*/));
		Рисунок ee = XpImage(XP_LISTVIEW, 0, 1, SColorFace(), Размер(10 * ebs, 10 * ebs));
		ImageBuffer eb(ee);
		eb.SetHotSpot(Точка(ebs, ebs));
		ee = eb;
		if(!sEmulateDarkTheme) ViewEdge_Write(ee);
		else ViewEdge_Write(SColorShadow());

		for(chCtrlsImg *m = sImgs; m < sImgs + __countof(sImgs); m++)
			SetXpImages(m->id, m->count, m->widget, m->part, m->state);

		{
			Кнопка::Стиль& s = Кнопка::StyleNormal().пиши();
			Win32Look(s.look, 4, XP_BUTTON, BP_PUSHBUTTON);
			SetXpColors(s.textcolor, 4, XP_BUTTON, BP_PUSHBUTTON, PBS_NORMAL, 3803/*TMT_TEXTCOLOR*/);
		}
		{
			Кнопка::Стиль& s = Кнопка::StyleOk().пиши();
			Win32Look(s.look, 4, XP_BUTTON, BP_PUSHBUTTON);
			Win32Look(s.look[0], XP_BUTTON, BP_PUSHBUTTON, PBS_DEFAULTED);
		}
		{
			ToolBar::Стиль& s = ToolBar::дефСтиль().пиши();
			Win32Look(s.buttonstyle.look, 6, XP_TOOLBAR, 1, 1);
			Win32Look(ToolButton::дефСтиль().пиши().look, 6, XP_TOOLBAR, 1, 1);
			if(!sEmulateDarkTheme){
				Win32Look(s.arealook, XP_REBAR, 0, 1);
				ToImageIfDark(s.arealook);
			}
			s.breaksep.l2 = SColorLight();
		}
		Win32Look(СтатусБар::дефСтиль().пиши().look, XP_STATUS, 0, 1);
		{
			ПромотБар::Стиль& s = ПромотБар::дефСтиль().пиши();
			Win32Look(s.hupper, 4, XP_SCROLLBAR, SBP_LOWERTRACKHORZ);
			Win32Look(s.hlower, 4, XP_SCROLLBAR, SBP_UPPERTRACKHORZ);
			Win32Look(s.vupper, 4, XP_SCROLLBAR, SBP_LOWERTRACKVERT);
			Win32Look(s.vlower, 4, XP_SCROLLBAR, SBP_UPPERTRACKVERT);
			Win32Look(s.up.look, 4, XP_SCROLLBAR, SBP_ARROWBTN, ABS_UPNORMAL);
			Win32Look(s.down.look, 4, XP_SCROLLBAR, SBP_ARROWBTN, ABS_DOWNNORMAL);
			Win32Look(s.left.look, 4, XP_SCROLLBAR, SBP_ARROWBTN, ABS_LEFTNORMAL);
			Win32Look(s.right.look, 4, XP_SCROLLBAR, SBP_ARROWBTN, ABS_RIGHTNORMAL);
		}
		{
			КтрлВкладка::Стиль& s = КтрлВкладка::дефСтиль().пиши();
			if(!sEmulateDarkTheme){
				Win32Look(s.normal, 4, XP_TAB, TABP_TABITEM);
				Win32Look(s.first, 4, XP_TAB, TABP_TABITEMLEFTEDGE);
				Win32Look(s.last, 4, XP_TAB, TABP_TABITEM);
				Win32Look(s.both, 4, XP_TAB, TABP_TABITEMBOTHEDGE);
				Win32Look(s.body, XP_TAB, TABP_PANE);
				ToImageIfDark(s.body);
			}
			else{
				s.normal[0] = s.first[0] = s.last[0] = s.both[0] = FaceColor(0x10);
				s.normal[1] = s.first[1] = s.last[1] = s.both[1] = FaceColor(0x20);
				s.normal[2] = s.first[2] = s.last[2] = s.both[2] = FaceColor(0x30);
				s.normal[3] = s.first[3] = s.last[3] = s.both[3] = FaceColor(0x20);
				s.text_color[3] = SColorDisabled();
				s.body = SColorShadow();
			}
		}
		{
			СпинКнопки::Стиль& s = СпинКнопки::дефСтиль().пиши();
			Win32Look(s.inc.look, 4, XP_SPIN, SPNP_UP);
			Win32Look(s.dec.look, 4, XP_SPIN, SPNP_DOWN);
			s.width = FrameButtonWidth();
		}
		{
			СпинКнопки::Стиль& s = СпинКнопки::StyleOnSides().пиши();
			Win32Look(s.inc.look, 4, XP_SCROLLBAR, SBP_ARROWBTN, ABS_UPNORMAL);
			Win32Look(s.dec.look, 4, XP_SCROLLBAR, SBP_ARROWBTN, ABS_DOWNNORMAL);
		}
		{
			MultiButton::Стиль& s = MultiButton::дефСтиль().пиши();
			s.usetrivial = true;
			if(vista_aero) {
				s.edge[0] = Null;
				Win32Look(s.look, 4, XP_COMBOBOX, 5);
				s.trivialborder = s.border = 0;
				s.sep1 = XpColor(XP_COMBOBOX, 5, 1, 3822/*TMT_BORDERCOLORHINT*/);
				s.sep2 = Null;
				s.sepm = 2;
				{
					MultiButton::Стиль& s = MultiButton::StyleFrame().пиши();
					for(int i = 0; i < 4; i++) {
						s.left[i] = Unglyph(XpImage(XP_COMBOBOX, 7, i + 1, Null, Размер(40, 40)));
						s.right[i] = Unglyph(XpImage(XP_COMBOBOX, 6, i + 1, Null, Размер(40, 40)));
						s.lmiddle[i] = s.rmiddle[i] = HorzBlend(s.right[i], s.left[i], 6, 14);
						Win32Look(s.look[i], XP_COMBOBOX, 4, i + 1);
					}
					s.activeedge = true;
					s.sep1 = Null;
				}
			}
			else {
				Win32Look(s.trivial, 4, XP_COMBOBOX, CP_DROPDOWNBUTTON);
				Win32Look(s.edge[0], XP_COMBOBOX, 0, 0);
				s.border = s.trivialborder = XpInt(XP_COMBOBOX, CP_DROPDOWNBUTTON, CBXS_NORMAL, 2403/*TMT_BORDERSIZE*/);
			}
		}

		{
			HeaderCtrl::Стиль& hs = HeaderCtrl::дефСтиль().пиши();
			for(int i = 0; i < 4; i++) {
				Рисунок h = CreateImage(Размер(10, 10), FaceColor(decode(i, CTRL_NORMAL, 5,
				                                                     CTRL_HOT, IsDarkTheme() ? 10 : 0,
				                                                     CTRL_PRESSED, -5,
				                                                     -8)));
				hs.look[i] = ChHot(WithBottomLine(WithRightLine(h, SColorShadow(), 1), SColorShadow()));
			}
		}
		{
			ProgressIndicator::Стиль& s = ProgressIndicator::дефСтиль().пиши();
			Win32Look(s.hlook, XP_PROGRESS, PP_BAR, 0, true);
			Win32Look(s.vlook, XP_PROGRESS, PP_BARVERT, 0, true);
			Win32Look(s.hchunk, XP_PROGRESS, PP_CHUNK);
			Win32Look(s.vchunk, XP_PROGRESS, PP_CHUNKVERT);
		}
		{
			БарМеню::Стиль& s = БарМеню::дефСтиль().пиши();
			if(vista_aero) {
				s.topitemtext[0] = s.topitemtext[1] = s.topitemtext[2] =
					s.itemtext = XpColor(XP_MENU, 8 /*MENU_POPUPITEM*/,
					2 /*HOT*/, 3803/*TMT_TEXTCOLOR*/);
				Win32Look(s.элт, XP_MENU, 14 /*MENU_POPUPITEM*/, 2 /*HOT*/);
				Win32Look(s.topitem[1], XP_MENU, 8 , 2 /*HOT*/);
				Win32Look(s.topitem[2], XP_MENU, 8 , 3 /*HOT*/);
				Win32Look(s.popupiconbar, XP_MENU, 13, 1);
				s.leftgap = DPI(32);
				s.textgap = Zx(6);
				s.lsepm = Zx(32);
				s.separator.l1 = смешай(SColorMenu(), SColorShadow());
				s.separator.l2 = SColorLight();
			}
			if(!sEmulateDarkTheme){
				Win32Look(s.arealook, XP_REBAR, 0, 1);
				ToImageIfDark(s.arealook);
			}
			else{
				s.topbar = s.arealook = SColorFace();
				s.topitem[1] = s.topitem[2] = s.popupbody = s.popupiconbar = смешай(SColorFace(), SColorShadow(),92);
				s.popupframe = чёрный();
				s.элт = SColorShadow();
			}
			CtrlImg::уст(CtrlImg::I_MenuCheck0, CtrlsImg::O0());
			CtrlImg::уст(CtrlImg::I_MenuCheck1, CtrlsImg::O1());
			CtrlImg::уст(CtrlImg::I_MenuRadio0, CtrlsImg::S0());
			CtrlImg::уст(CtrlImg::I_MenuRadio1, CtrlsImg::S1());
		}

/*		CtrlImg::уст("hthumb", XpImage(XP_TRACKBAR, TKP_THUMB, TUS_NORMAL, Null, Размер(10, 20)));
		CtrlImg::уст("hthumb1", XpImage(XP_TRACKBAR, TKP_THUMB, TUS_PRESSED, Null, Размер(10, 20)));
		CtrlImg::уст("vthumb", XpImage(XP_TRACKBAR, TKP_THUMBVERT, TUS_NORMAL, Null, Размер(20, 10)));
		CtrlImg::уст("vthumb1", XpImage(XP_TRACKBAR, TKP_THUMBVERT, TUS_PRESSED, Null, Размер(20, 10)));
*/
		XpElement e;
		for(int i = 0; i < 4; i++) {
			{
				ПромотБар::Стиль& s = ПромотБар::дефСтиль().пиши();
				e.widget = XP_SCROLLBAR;
				e.state = 1 + i;
				e.contentm = false;
				e.part = SBP_THUMBBTNHORZ;
				s.hthumb[i] = ChLookWith(RawToValue(e), XpImage(XP_SCROLLBAR, SBP_GRIPPERHORZ, 1));
				e.part = SBP_THUMBBTNVERT;
				s.vthumb[i] = ChLookWith(RawToValue(e), XpImage(XP_SCROLLBAR, SBP_GRIPPERVERT, 1));
			}
			Цвет paper = i == 3 ? SColorFace : SColorPaper;
			Рисунок m = XpImage(XP_COMBOBOX, CP_DROPDOWNBUTTON, CBXS_NORMAL + i, paper, Размер(32, 32));
			Рисунок mm = m;
			Размер isz = m.дайРазм();
			int cbs = XpInt(XP_COMBOBOX, CP_DROPDOWNBUTTON, CBXS_NORMAL + i, 2403/*TMT_BORDERSIZE*/);
			if(cbs == 0) {
				Рисунок h = HorzSymm(m);
				Кнопка::StyleLeftEdge().пиши().look[i] = Unglyph(Crop(h, 1, 1, isz.cx - 1, isz.cy - 2));
				m = Crop(h, 0, 1, isz.cx - 1, isz.cy - 2);
			}
			bool outer = false;
			if(cbs == 1 &&
			   XpBool(XP_COMBOBOX, CP_DROPDOWNBUTTON, CBXS_NORMAL + i, 2203/*TMT_BORDERONLY*/) == 0 &&
			   XpInt(XP_COMBOBOX, 0, 0, 2403/*TMT_BORDERSIZE*/) == 1 &&
			   XpBool(XP_COMBOBOX, 0, 0, 2203/*TMT_BORDERONLY*/) == 0) {
					m = Crop(m, 0, 1, isz.cx - 1, isz.cy - 2);
					e.widget = XP_COMBOBOX;
					e.part = CP_DROPDOWNBUTTON;
					e.state = CBXS_NORMAL + i;
					e.whista = true;
					outer = true;
			}
			Цвет c;
			double gf;
			m = Unglyph(m, c, gf);
			if(i == 0 && (gf > 150 || vista_aero))
				CtrlsImg::уст(CtrlsImg::I_DA, CtrlsImg::kDA());
			Кнопка::StyleEdge().пиши().look[i] = m;
			if(cbs)
				Кнопка::StyleLeftEdge().пиши().look[i] = m;

			MultiButton::дефСтиль().пиши().simple[i] = m;
			MultiButton::StyleFrame().пиши().simple[i] = m;
			
			Кнопка::StyleNormal().пиши().monocolor[i] = c;
			Кнопка::StyleOk().пиши().monocolor[i] = c;
			Кнопка::StyleEdge().пиши().monocolor[i] = c;
			Кнопка::StyleLeftEdge().пиши().monocolor[i] = c;
			Кнопка::StyleScroll().пиши().monocolor[i] = c;

			MultiButton::дефСтиль().пиши().monocolor[i] = c;
			MultiButton::StyleFrame().пиши().monocolor[i] = c;
			if(!vista_aero) {
				MultiButton::Стиль& s = MultiButton::дефСтиль().пиши();
				MultiButton::Стиль& fs = MultiButton::StyleFrame().пиши();
				Рисунок cm = Unglyph(XpImage(XP_COMBOBOX, CP_DROPDOWNBUTTON, i + 1, Null, Размер(40, 40)));
				fs.right[i] = s.right[i] = cm;
				fs.left[i] = s.left[i] = MirrorHorz(s.right[i]);
				if(s.border == 0) {
					mm = Crop(Unglyph(cm), 0, 0, 12, 20);
					ImageBuffer ib(mm);
					ib.SetHotSpot(cm.GetHotSpot());
					mm = ib;
				}
				else
					mm = m;
				fs.rmiddle[i] = s.rmiddle[i] = mm;
				fs.lmiddle[i] = s.lmiddle[i] = MirrorHorz(mm);
			}

			m = Unglyph(XpImage(XP_SCROLLBAR, SBP_ARROWBTN, i + ABS_UPNORMAL, paper, Размер(40, 40)));
			Кнопка::StyleScroll().пиши().look[i] =
			      VertBlend(m, Unglyph(XpImage(XP_SCROLLBAR, SBP_ARROWBTN, i + ABS_DOWNNORMAL,
			                                   paper, Размер(40, 40))),
			                14, 26);
		}
		
//		LabelBoxTextColor_Write(XpColor(XP_BUTTON, BP_GROUPBOX, GBS_NORMAL, 3803/*TMT_TEXTCOLOR*/));
//		LabelBoxColor_Write(XpColor(XP_BUTTON, BP_GROUPBOX, GBS_NORMAL, 3822/*TMT_BORDERCOLORHINT*/));
	}
	else
		ChClassicSkin();
}

}

#endif
