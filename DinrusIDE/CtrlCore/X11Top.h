//$ class РНЦП::ТопОкно {
protected:
	virtual void    СобытиеProc(XWindow& w, XСобытие *event);
private:
	XSizeHints *size_hints;
	XWMHints   *wm_hints;
	XClassHint *class_hint;
	Размер        xminsize, xmaxsize;
	bool        topmost;
	
	Буфер<unsigned long>  PreperIcon(const Рисунок& icon, int& len);
	
	void                   центрПрям(Ктрл *owner);
	void                   DefSyncTitle();
	void                   EndIgnoreTakeFocus();

	void                   SyncState();

	Рисунок                  invert;
	ШТкст                title2;

	static  Прям           windowFrameMargin;
//$ };
