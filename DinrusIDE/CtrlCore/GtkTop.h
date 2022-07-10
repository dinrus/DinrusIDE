//$ class РНЦП::ТопОкно : Ктрл {
	ImageGdk gdk_icon;
	ImageGdk gdk_largeicon;
	bool     topmost;
	
	enum { FULLSCREEN = 99 };

	void     центрПрям(Ктрл *owner);
	void     устРежим(int mode);
	void     SyncTopMost();

	static gboolean StateСобытие(GtkWidget *widget, GdkСобытиеWindowState *event, gpointer user_data);

	friend class Ктрл;
//$ };
