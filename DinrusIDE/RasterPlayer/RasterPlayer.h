#ifndef _RasterPlayer_RasterPlayer_h_
#define _RasterPlayer_RasterPlayer_h_

#include <CtrlLib/CtrlLib.h>

namespace РНЦП {

class RasterPlayer : public Ктрл {
private:
	virtual void рисуй(Draw& w);
	bool IsKilled();
	
	РНЦП::Массив<Рисунок> images;
	РНЦП::Массив<int> delays;
	int ind;
	Цвет background;
	double speed;
	bool mt;
		
	ТаймСтоп tTime;
	double tFrame_ms;

public:
	RasterPlayer();
	virtual ~RasterPlayer() noexcept;

	bool грузи(const Ткст &fileName);
	bool LoadBuffer(const Ткст &буфер);

	void Play();
	void стоп();
	bool пущен()	{return running;}
	void NextFrame();
	inline void следщСтраница() {NextFrame();};
	RasterPlayer& SetBackground(Цвет c)	{background = c; освежи(); return *this;}
	RasterPlayer& SetSpeed(double s = 1)	{speed = s; освежи(); return *this;}
	RasterPlayer& SetMT(bool _mt = false);
	
	Событие<> WhenShown;
	
	int GetPageCount() 	{return images.дайСчёт();};
	int дайСчётФреймов() {return images.дайСчёт();};	
	int дайСтраницу() 		{return ind;};
	void устСтраницу(int i) {ind = minmax(i, 0, images.дайСчёт());};
	
#ifdef _MULTITHREADED	
	friend void RasterPlayerThread(RasterPlayer *animatedClip);
#endif	
	void TimerFun();
	
protected:
	volatile Атомар running, kill;
};

}

#endif
