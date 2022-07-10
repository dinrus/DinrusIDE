//$ namespace РНЦП {
//$ class Ктрл {
	void создай(Ктрл *owner, bool popup);

	static gboolean GtkProc(GtkWidget *widget, GdkСобытие *event, gpointer user_data);
	static void     IMCommit(GtkIMContext *context, gchar *str, gpointer user_data);
	static void     IMPreedit(GtkIMContext *context, gpointer user_data);
	static void     IMPreeditEnd(GtkIMContext *context, gpointer user_data);
	static void     IMLocation(Ктрл *w);

	static int      DoButtonСобытие(GdkСобытие *event, bool press);
	static void     AddСобытие(gpointer user_data, int тип, const Значение& значение, GdkСобытие *event);
	static void     FetchСобытиеs(bool may_block);

/*
_DBG_
	static void     AddСобытие(gpointer user_data, int тип, const Значение& значение);
	static void     DoMouseEvent(int state, Точка pos);
*/

	bool   DispatchMouseIn(int act, int zd);
	Рисунок  GtkMouseEvent(int action, int act, int zd);
	void   GtkButtonСобытие(int action);

	static bool IsWaitingСобытие0(bool fetch);
	static bool ProcessСобытие0(bool *quit, bool fetch);
	static bool ProcessСобытиеs0(bool *quit, bool fetch);

	static void CaptureSync();

	static gboolean GtkСобытие(GtkWidget *widget, GdkСобытие *ключ, gpointer user_data);
	static gboolean GtkDraw(GtkWidget *widget, cairo_t *cr, gpointer data);

	static Точка GetMouseInfo(GdkWindow *win, GdkModifierType& mod);
	static GdkDevice *GetMouseDevice();
#if GTK_CHECK_VERSION(3, 20, 0)
	static GdkSeat   *GetSeat();
#endif
	static bool MouseIsGrabbed();
	bool GrabMouse();
	static void UngrabMouse();
	
	static int scale; // in case GUI is scaling (e.g. in UHD mode)

	enum {
		EVENT_NONE = -15321,
		EVENT_TEXT,
		EVENT_FOCUS_CHANGE,
	};
	
	struct GСобытие0 {
		int        time;
		int        windowid;
		int        тип;
		Значение      значение;
		Точка      mousepos;
		guint      state;
		int        count;

		bool       pen;
		bool       pen_barrel;
		bool       pen_inverted;
		bool       pen_eraser;
		double     pen_pressure;
		double     pen_rotation;
		ТочкаПЗ     pen_tilt;
	};
	
	struct GСобытие : Движимое<GСобытие, GСобытие0> {
		GdkСобытие  *event;

		void освободи();
		void уст(const GСобытие& e);
		GСобытие(const GСобытие& e);
		void operator=(const GСобытие& e);

		GСобытие();
		~GСобытие();
	};
	
	struct Win : Движимое<Win> {
		int          id;
		GtkWidget   *gtk;
		GdkWindow   *gdk;
		Ук<Ктрл>    ctrl;
		Вектор<Прям> invalid; // areas invalidated to be processed at next opportunity
	};

	void   Proc();
	bool   SweepConfigure(bool wait);
	bool   SweepFocus(bool wait);
	void   SyncWndRect(const Прям& rect);

	static БиВектор<GСобытие>  Событиеs;
	static Вектор<Ук<Ктрл>> activePopup; // created with 'activate' flag - usually menu
	static Вектор<Ук<Ктрл>> visiblePopup; // any popup visible on screen
	static Вектор<Win>       wins;
	static int               WndCaretTime;
	static bool              WndCaretVisible;
	static Ук<Ктрл>         grabwindow;
	static Ук<Ктрл>         grabpopup;
	static Ук<Ктрл>         sel_ctrl;
	static Ук<Ктрл>         activeCtrl;
	static bool              invalids; // there are active invalid areas

	static int найдиИд(int id);
	static int FindCtrl(Ктрл *ctrl);
	static int FindGtkWindow(GtkWidget *gtk);
	static int FindGdkWindow(GdkWindow *gdk);
	
	static Ктрл *GetTopCtrlFromId(int id);
	static Ктрл *GetTopCtrlFromId(gpointer user_data) { return GetTopCtrlFromId((uint32)(uintptr_t)user_data); }

	static void SyncPopupCapture();
	void ReleasePopupCapture();
	
	static void FocusSync();
	static void анимируйКаретку();
	static gboolean TimeHandler(GtkWidget *);
	static void InvalidateMousePos();
	static void StopGrabPopup();
	static void StartGrabPopup();
	static bool ReleaseWndCapture0();
	static void DoCancelPreedit();
	
	static Прям frameMargins;
	static Прям GetFrameMargins();

	static Индекс<Ткст>   dnd_targets;
	static Ткст          dnd_text_target;
	static Ткст          dnd_image_target;
	static Ткст          dnd_files_target;
	static Ткст          dnd_data;
	static Ткст          dnd_data_fmt;
	static bool            dnd_data_wait;
	static bool            dnd_events;
	static GtkWidget      *dnd_widget;
	static GdkDragContext *dnd_context;
	static guint           dnd_time;

	static Ук<Ктрл>                          dnd_source;
	static const ВекторМап<Ткст, ClipData> *dnd_source_data;
	static Вектор<Ткст>                     dnd_fmts;
	static int                                dnd_result;
	static Рисунок                              dnd_icon;
	
	static void            GtkSelectionDataSet(GtkSelectionData *selection_data, const Ткст& fmt, const Ткст& data);
	static void            GtkGetClipData(GtkClipboard *clipboard, GtkSelectionData *selection_data, guint info, gpointer user_data);
	static void            AddFmt(GtkTargetList *list, const Ткст& fmt, int info);
	static GtkTargetList  *CreateTargetList(const ВекторМап<Ткст, ClipData>& target);
	static Ткст          GtkDataGet(GtkSelectionData *s);

	       void DndInit();
	       void DndExit();

	static void GtkDragBegin(GtkWidget *widget, GdkDragContext *context, gpointer user_data);
	static void GtkDragDelete(GtkWidget *widget, GdkDragContext *context, gpointer user_data);
	static void GtkDragGetData(GtkWidget *widget, GdkDragContext *context, GtkSelectionData *data, guint info, guint time, gpointer user_data);
	static void GtkDragDataReceived(GtkWidget *widget, GdkDragContext *context, gint x, gint y, GtkSelectionData *data, guint info, guint time, gpointer user_data);
	static gboolean GtkDragDrop(GtkWidget *widget, GdkDragContext *context, gint x, gint y, guint time, gpointer user_data);
	static void GtkDragEnd(GtkWidget *widget, GdkDragContext *context, gpointer user_data);
	static gboolean GtkDragFailed(GtkWidget *widget, GdkDragContext *context, GtkDragResult result, gpointer user_data);
	static void GtkDragLeave(GtkWidget *widget, GdkDragContext *context, guint time, gpointer user_data);
	static gboolean GtkDragMotion(GtkWidget *widget, GdkDragContext *context, gint x, gint y, guint time, gpointer user_data);

	static Ктрл  *DragWnd(gpointer user_data);
	static void   DndTargets(GdkDragContext *context);
	static bool   IsDragAvailable(const char *fmt);
	static Ткст DragGet(const char *fmt);
	static PasteClip GtkDnd(GtkWidget *widget, GdkDragContext *context, gint x, gint y,
	                        guint time, gpointer user_data, bool paste);
	static bool   ProcessInvalids();

	friend void InitGtkApp(int argc, char **argv, const char **envptr);
	friend void GuiPlatformGripResize(ТопОкно *q);

public: // really private:
	struct Gclipboard {
		ВекторМап<Ткст, ClipData> target;
		GtkClipboard *clipboard;

		Ткст дай(const Ткст& fmt);
		void   помести(const Ткст& fmt, const ClipData& data);
		bool   IsAvailable(const Ткст& fmt);
		void   очисть();

		Gclipboard(GdkAtom тип);
	};

	static Gclipboard& gclipboard();
	static Gclipboard& gselection();
	static Ткст      RenderPrimarySelection(const Значение& fmt);

	static Вектор<Событие<>> hotkey;
	static Вектор<dword>   keyhot;
	static Вектор<dword>   modhot;
	static guint           MouseState;

	static int    SCL(int x)                        { return scale * x; }
	static Прям   SCL(int x, int y, int cx, int cy) { return RectC(SCL(x), SCL(y), SCL(cx), SCL(cy)); }
	static double LSC(int x)                        { return (double)x / scale; }
            
public:
	static void      окончиСессию()              {}
	static bool      IsEndSession()            { return false; }
	static void      PanicMsgBox(const char *title, const char *text);
	
	static Точка     CurrentMousePos;
	static guint     CurrentState;
	static guint32   CurrentTime;
	static GСобытие    CurrentСобытие;

	GdkWindow *gdk() const { return top ? gtk_widget_get_window(top->window) : NULL; }
	GtkWindow *gtk() const { return top ? (GtkWindow *)top->window : NULL; }

	static GdkFilterReturn RootKeyFilter(GdkXСобытие *xevent, GdkСобытие *event, gpointer data);

//$ }};
