#include <CtrlLib/CtrlLib.h>
#include <RasterPlayer/RasterPlayer.h>

namespace РНЦП {

Рисунок GetRect_(const Рисунок& orig, const Прям &r) {
	if(r.пустой())
		return Рисунок();
	ImageBuffer ib(r.дайРазм());
	for(int y = r.top; y < r.bottom; y++) {
		const КЗСА *s = orig[y] + r.left;
		const КЗСА *e = orig[y] + r.right;
		КЗСА *t = ib[y - r.top];
		while(s < e) {
			*t = *s;
			t++;
			s++;
		}
	}
	return ib;
}

Прям Fit(const Размер &frame, const Размер &object) {
	double objectAspect = object.cx/(double)object.cy;		
	if (frame.cx/(double)frame.cy > objectAspect) {
		double x = (frame.cx - objectAspect*frame.cy)/2.;
		return Прям(int(x), 0, int(x + objectAspect*frame.cy), frame.cy);
	} else {
		double y = (frame.cy - frame.cx/objectAspect)/2.;
		return Прям(0, int(y), frame.cx, int(y + frame.cx/objectAspect));
	}
}

RasterPlayer::RasterPlayer() {
	Transparent();
	NoWantFocus();
	background = Null;
	running = 0;
	kill = 1;
	speed = 1;
	mt = false;
}

void RasterPlayer::рисуй(Draw& w) {
	Размер sz = дайРазм();
	if (!пусто_ли(background))
		w.DrawRect(sz, background);
	if (images.пустой())
		return;
	if (!images[ind]) 
		return;
	w.DrawImage(Fit(sz, images[ind].дайРазм()), images[ind]);
}

static СтатическаяКритСекция mutex;

bool RasterPlayer::LoadBuffer(const Ткст &буфер) {
	if (буфер.пустой())
		return false;
	ТкстПоток str(буфер);
	Один<StreamRaster> raster = StreamRaster::OpenAny(str);
	if(!raster) 
		return false;
	
	стоп();	
	images.очисть();
	delays.очисть();	
	
	Размер sz = raster->дайРазм();
	ImageDraw iw(sz); 
	Рисунок previous;
	Прям r;
	for (int i = 0; i < raster->GetPageCount(); ++i) {
		if (previous) {
			iw.Alpha().DrawImage(r, previous, серыйЦвет(255));
			iw.DrawImage(r, previous);
			previous = Null;
		}
		raster->SeekPage(i);
		r = raster->GetPageRect(i);
		switch (raster->GetPageDisposal(i)) {
		case 0:	
		case 1:	break;
		case 2: iw.DrawRect(sz, SColorFace());	  
				break;
		case 3:	if (i > 0) 
					previous = GetRect_(images[i-1], r);
				iw.DrawRect(sz, SColorFace()); 
				break;
		}
		iw.Alpha().DrawImage(r.left, r.top, raster->GetImage(0, 0, r.right-r.left, r.bottom-r.top), серыйЦвет(255));
		iw.DrawImage(r.left, r.top, raster->GetImage(0, 0, r.right-r.left, r.bottom-r.top));
		images.добавь(iw);
		delays.добавь(max(50, 10*raster->GetPageDelay(i)));
	}
	ind = 0;
	return true;	
}

bool RasterPlayer::грузи(const Ткст &fileName) {
	ФайлВвод in(fileName);
	return LoadBuffer(загрузиПоток(in));
}

bool RasterPlayer::IsKilled() {
	bool ret;
	INTERLOCKED_(mutex) {ret = kill;}
	return ret;	
}

#ifdef _MULTITHREADED
void RasterPlayerThread(RasterPlayer *animatedClip) {
	ТаймСтоп t;
	double tFrame_ms = 0;
	while (!animatedClip->IsKilled()) {
		INTERLOCKED_(mutex) {
			int ind = animatedClip->ind + 1;
			if (ind > animatedClip->GetPageCount() - 1)
				ind = 0;
			tFrame_ms = animatedClip->delays[ind]/animatedClip->speed;
		}
		while (t.прошло()/1000. < tFrame_ms && !animatedClip->IsKilled())
			спи(10);
		t.переустанов();
		постОбрвыз(callback(animatedClip, &RasterPlayer::NextFrame));
	}
INTERLOCKED_(mutex) {
	animatedClip->running = false;
}
}
#endif

void RasterPlayer::TimerFun() {
INTERLOCKED_(mutex) {
	if (kill || !running)
		return;
}
	if (tTime.прошло()/1000. < tFrame_ms)
		return;	 
	tTime.переустанов();
	int iFrame = ind + 1;
	if (iFrame > GetPageCount() - 1)
		iFrame = 0;
	tFrame_ms = delays[iFrame]/speed;
	NextFrame();
}

void RasterPlayer::Play() {
	if (images.дайСчёт() <= 1)
		return;
INTERLOCKED_(mutex) {
	running  = true;
	kill = false;
}
#ifdef _MULTITHREADED
	if (mt)
		Нить().пуск(callback1(RasterPlayerThread, this));
	else
#endif
	{
		tFrame_ms = 0;
		tTime.переустанов();
		устОбрвызВремени(-50, callback(this, &RasterPlayer::TimerFun), 1);
	}
}

void RasterPlayer::стоп() {
INTERLOCKED_(mutex) {kill = true;}
#ifdef _MULTITHREADED
	if (mt) {
		while (running)
			спи(10);	
	} else 
#endif
	{
		глушиОбрвызВремени(1);
		INTERLOCKED_(mutex) {running = false;}
	}
}

RasterPlayer& RasterPlayer::SetMT(bool _mt)	{
#ifndef _MULTITHREADED
	return *this;
#endif
	bool wasrunning;
	INTERLOCKED_(mutex) {
		wasrunning = running;
	}
	стоп(); 
	mt = _mt; 
	if (wasrunning)
		Play();
	return *this;
}

RasterPlayer::~RasterPlayer() noexcept {
	стоп();	
}

void RasterPlayer::NextFrame() {
	if (images.дайСчёт() <= 1)
		return;
	ind++;
	if (ind >= images.дайСчёт())
		ind = 0;
	if (WhenShown)
		WhenShown();
	освежи();
}

}