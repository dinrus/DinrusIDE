#include <CtrlLib/CtrlLib.h>

#ifdef VIRTUALGUI

namespace РНЦП {

class TopWindowFrame : public Ктрл {
public:
	virtual void  Выкладка();
	virtual void  рисуй(Draw& w);
	virtual Рисунок рисКурсора(Точка p, dword keyflags);
	virtual void  леваяВнизу(Точка p, dword keyflags);
	virtual void  LeftHold(Точка p, dword keyflags);
	virtual void  леваяДКлик(Точка p, dword keyflags);
	virtual void  двигМыши(Точка p, dword keyflags);
	virtual void  режимОтмены();
	virtual void  леваяВверху(Точка p, dword keyflags);

private:
	Точка  dir;
	Точка  startpos;
	Прям   startrect;

	bool   maximized;
	Прям   overlapped;
	
	bool   holding;
	ОбрвызВремени hold;
	
	Точка GetDragMode(Точка p);
	Рисунок GetDragImage(Точка dragmode);
	void  StartDrag();
	Прям  Margins() const;
	Прям  ComputeClient(Прям r);
	void  Hold();

	typedef TopWindowFrame ИМЯ_КЛАССА;

public:
	Ткст title;
	Кнопка close, maximize;
	Рисунок  icon;
	Размер   minsize;
	bool   sizeable;
	ТопОкно *window;

	void SetTitle(const Ткст& s)           { title = s; освежи(); }
	Прям GetClient() const;
	void SetClient(Прям r);
	void GripResize();

	void  разверни();
	void  нахлёст();
	void  ToggleMaximize();
	bool  развёрнуто() const                 { return maximized; }
	void  SyncRect();
	
	TopWindowFrame();
};

extern VirtualGui *VirtualGuiPtr;

}

#endif
