class TopFrameDraw : public SystemDraw {
public:
	TopFrameDraw(Ктрл *ctrl, const Прям& r);
	~TopFrameDraw();

protected:
	bool   caret;
};


#define ГЛАВНАЯ_ГИП_ПРИЛ \
void главФнГип_(); \
\
int main(int argc, const char **argv, const char **envptr) { \
	РНЦП::иницПрил__(argc, argv, envptr); \
	GUI_APP_MAIN_HOOK \
	РНЦП::Ктрл::InitX11(NULL); \
	РНЦП::выполниПрил__(главФнГип_); \
	РНЦП::Ктрл::ExitX11(); \
	РНЦП::выходПрил__(); \
	return РНЦП::дайКодВыхода(); \
} \
 \
void главФнГип_()

class DHCtrl : public Ктрл {
	int isError;
	bool isMapped;
	Размер CurrentSize;
	XVisualInfo* UserVisualInfo;
	Ткст ErrorMessage;

	void MapWindow(bool map);
	bool иниц(void);
	void Terminate(void);

protected:
	Visual     *GetVisual(void);
	XVisualInfo GetVisualInfo(void);

	virtual XVisualInfo *CreateVisual(void) {return 0;}
	virtual void SetAttributes(unsigned long &ValueMask, XSetWindowAttributes &attr) {}
	virtual void рисуй(Draw &draw) {}
	virtual void BeforeInit(void) {}
	virtual void AfterInit(bool Ошибка) {}
	virtual void BeforeTerminate(void) {}
	virtual void AfterTerminate(void) {}
	virtual void Resize(int w, int h) {}

	void устОш(bool err) { isError = err; }
	void SetErrorMessage(Ткст const &msg) { ErrorMessage = msg; }

	virtual void State(int reason);
	
	Window   hwnd;	
	bool isInitialized;
	
public:
	typedef DHCtrl ИМЯ_КЛАССА;

	bool   IsInitialized(void) { return isInitialized; }

	bool   дайОш(void) { return isError; }
	Ткст дайОшСооб(void) { return ErrorMessage; }

	DHCtrl();
	~DHCtrl();
};
