#include "Docking.h"

namespace РНЦП {

/*
int DockPane::ClientToPos(const Размер& p)
{
	int w = (width>>1)*pos.дайСчёт()-1;
	return minmax(vert ? 10000 * p.cy / (дайРазм().cy - w) : 10000 * p.cx / (дайРазм().cx - w), 0, 9999);	
}
*/
void DockPane::CumulativePos(Вектор<int>& p) const
{
	for (int i = 1; i < p.дайСчёт()-1; i++)
		p[i] += p[i-1];		
	p.верх() = 10000;
}

void DockPane::NonCumulativePos(Вектор<int>& p) const
{
	for (int i = p.дайСчёт()-1; i > 0; i--)
		p[i] -= p[i-1];		
}

void DockPane::StartAnimate(int ix, Размер sz, bool restore)
{
	if (restore) {
		ПРОВЕРЬ(savedpos.дайСчёт() == pos.дайСчёт());
		animpos <<= savedpos;
	}
	else {
		int tsz = ClientToPos(sz);
		animpos <<= pos;
		if (tsz) {
			int msz = GetMinPos(ix);
			if (msz < 10000 && msz+tsz > 10000) {
				Ктрл *c = GetIndexChild(ix);
				int min = ClientToPos(c->дайМинРазм());
				if (min < tsz) {
					int std = ClientToPos(c->дайСтдРазм());
					tsz = (std < tsz) ? std : min;
				}
			}
			SmartReposUp(animpos, ix, tsz);
		}
		else
			SmartReposDown(animpos, ix);
	}
	animtick = 0;	
	устОбрвызВремени(-animinterval, THISBACK(AnimateTick), TIMEID_ANIMATE);
	AnimateTick();
}

void DockPane::AnimateTick()
{
	for (int i = 0; i < animpos.дайСчёт(); i++)
		pos[i] += ((animpos[i] - pos[i]) * animtick) / animmaxticks;
	animtick++;
	if (animtick == animmaxticks)
		EndAnimate();
	else
		Выкладка();
}

void DockPane::EndAnimate()
{
	animtick = 0;
	pos <<= animpos;
	animpos.очисть();
	глушиОбрвызВремени(TIMEID_ANIMATE);
	удали(dummy);
/*	if (dummy.дайРодителя()) {
		pos.удали(найдиИндекс(dummy));	
		dummy.удали();
	}*/
	//FixChildSizes(); 
	Выкладка();
}

void DockPane::SmartReposDown(Вектор<int>& p, int ix) 
{
	int n = 0;
	int dif = 0;
	int maxsize = 0;
	int totalsize = 0;
	int sparesize = 0;
	int cnt = p.дайСчёт();
	Вектор<int> maxpos;
	maxpos.устСчёт(cnt);

	if (p.дайСчёт() == 1) {
		p[0] = 0;
		return;
	}

	// Преобр pos to non-cumulative sizes
	NonCumulativePos(p);

	// найди max, and total sizes (in pos units)
	for (Ктрл *c = дайПервОтпрыск(); c; c = c->дайСледщ()) {
		if (n != ix) {
			maxpos[n] = max(ClientToPos(c->дайМаксРазм()), p[n]);
			maxsize += maxpos[n];
			totalsize += p[n];
		}
		n++;
	}
	
	totalsize = max(totalsize, 1);
	
	sparesize = p[ix];
	p[ix] = 0;		
	for (int i = 0; i < cnt; i++)
		p[i] += iscale(sparesize, p[i], totalsize);
		
	// Restrict to max size
	if (maxsize > 10000) {
		totalsize = 0;
		dif = 0;
		for (int i = 0; i < cnt; i++)
			if (i != ix) {
				if (p[i] > maxpos[i]) {
					dif += p[i] - maxpos[i];
					p[i] = maxpos[i];
				}
				else	
					totalsize += p[i];
			}
		// Share out extra spare space to ctrls that are less than maxsize
		while (dif > 1) {
			int sum = 0;
			for (int i = 0; i < cnt; i++) {
				if (i != ix && p[i] < maxpos[i]) { 
					int t = max(1, min((p[i]*dif) / totalsize, maxpos[i] - p[i]));
					p[i] += t;
					sum += t;
				}
			}
			dif -= sum;
		}
	}

	// Do remainder
	dif = сумма(p, 0) - 10000;
	if (dif) 
		p[ix] += dif;
	// Return to cumulative sizes	
	CumulativePos(p);	
}

void DockPane::SmartReposUp(Вектор<int>& p, int ix, int sz) 
{
	int n = 0;
	int dif = 0;
	int minsize = 0;
	int maxsize = 0;
	int totalsize = 0;
	int cnt = p.дайСчёт();
	int resizemin = max(10000 / (cnt*4), PosToClient(30));
	Вектор<int> minpos;
	Вектор<int> maxpos;
	maxpos.устСчёт(cnt);
	minpos.устСчёт(cnt);
		
	// Преобр pos to non-cumulative sizes
	NonCumulativePos(p);
	
	// найди min, max, and total sizes (in pos units)
	for (Ктрл *c = дайПервОтпрыск(); c; c = c->дайСледщ()) {
		minpos[n] = min(ClientToPos(c->дайМинРазм()), p[n]);
		maxpos[n] = max(ClientToPos(c->дайМаксРазм()), p[n]);
		maxsize += maxpos[n];		
		if (n != ix)
			minsize += minpos[n];
		n++;
	}
	totalsize = 10000 - sz - minsize;
	
	for (int i = 0; i < cnt; i++)
		if (i != ix)
			p[i] = minpos[i] + iscale(p[i], totalsize, 10000);
	p[ix] = sz;
		
	// Enforce an absolute minimum size
	dif = 0;
	minsize += minpos[ix];
	for (int i = 0; i < cnt; i++) {
		if (p[i] < resizemin && minpos[i] > p[i]) {
			dif += resizemin - p[i];
			p[i] = resizemin;	
		}
	}
	if (dif) {
		totalsize = 10000 - cnt*resizemin;
		for (int i = 0; i < cnt; i++) {
			int t = p[i] - resizemin;
			if (t > 0)
				p[i] -= (t*dif) / totalsize;
		}
	}

	// Restrict to max size
	if (maxsize > 10000) {
		totalsize = 0;
		dif = 0;
		for (int i = 0; i < cnt; i++)
			if (i != ix) {
				if (p[i] > maxpos[i]) {
					dif += p[i] - maxpos[i];
					p[i] = maxpos[i];
				}
				else	
					totalsize += p[i];
			}
			else if (p[i] < maxpos[i])
				totalsize += p[i];
		// Share out extra spare space to ctrls that are less than maxsize
		while (dif > 1) {
			int sum = 0;
			for (int i = 0; i < cnt; i++) {
				if (p[i] < maxpos[i]) { 
					int t = max(1, min((p[i]*dif) / totalsize, maxpos[i] - p[i]));
					p[i] += t;
					sum += t;
				}
			}
			dif -= sum;
		}
	}	
	
	// Do remainder
	dif = сумма(p, 0) - 10000;
	if (dif)
		p[ix] += dif;	
	// Return to cumulative sizes
	CumulativePos(p);
}

void DockPane::SimpleRepos(Вектор<int>& p, int ix, int inc)
{
	int cnt = p.дайСчёт();
	if (cnt == 1) {
		p[0] = 10000;
		return;
	}

	// Преобр pos to non-cumulative sizes
	NonCumulativePos(p);
	
	int n = 0;
	int tsz = 0;
	
	for (Ктрл *c = дайПервОтпрыск(); c; c = c->дайСледщ()) {
		if (n != ix)
			tsz += p[n];
		n++;
	}
	int dif = tsz - inc;
	int sum = 0;
	p[ix] += inc;
	for (int i = 0; i < cnt; i++) {
		if (i != ix)
			p[i] = (p[i]*dif) / tsz ;
		sum += p[i];
	}	
	dif = sum - 10000;
	if (dif)
		p[ix] += dif;
	// Return to cumulative sizes
	CumulativePos(p);
}

int DockPane::GetMinPos(int notix)
{
	int n = 0;
	int msz = 0;
	for (Ктрл *c = дайПервОтпрыск(); c; c = c->дайСледщ()) {
		if (n != notix) msz += ClientToPos(c->дайМинРазм());
		n++;
	}
	return msz;	
}

void DockPane::FixChildSizes()
{
	int cnt = pos.дайСчёт();
	if (!cnt) return;
	if (cnt == 1) {
		pos[0] = 10000;
		return;
	}
	int sum = pos[0];
	for (int i = 1; i < cnt; i++)
		sum += pos[i] - pos[i-1];	
	sum -= 10000;
	int rem = sum % cnt;
	sum /= cnt;
	if (sum)
		for (int i = 0; i < cnt; i++)
			pos[i] -= sum*(i+1);
	pos[cnt-1] -= rem;
	ПРОВЕРЬ(pos[cnt-1] == 10000);
}

/*
int DockPane::найдиИндекс(Ктрл& child)
{
	int ix = 0;
	for (Ктрл *c = дайПервОтпрыск(); c; c = c->дайСледщ()) {
		if (c == &child) return ix;
		ix++;
	}
	return -1;
}

Ктрл * DockPane::FindCtrl(int ix)
{
	Ктрл *c = дайПервОтпрыск();
	for (int i = 0; i < ix && c; i++)
		c = c->дайСледщ();
	return c;
}

void DockPane::разверни(Ктрл& child, Ктрл& newctrl)
{
	newctrl.устПрям(child.дайПрям());
	Ктрл::добавьОтпрыскПеред(&newctrl, &child);
	Ктрл::удалиОтпрыск(&child);
}
*/
void DockPane::Dock(Ктрл& newctrl, Размер sz, int ps, bool animate, bool save)
{
	if (IsAnimating())
		EndAnimate();
		
	int cnt = pos.дайСчёт();
	if (!cnt) animate = false;
	int tsz = cnt ? ClientToPos(sz) : 10000;
	ps = min(ps, cnt);
	if (ps >= 0) {
		if (ps && ps == cnt)
			Ктрл::добавьОтпрыск(&newctrl, GetLastChild());
		else {			
			Ктрл *c = дайПервОтпрыск();
			for (int i = 0; i < ps; i++)
				c = c->дайСледщ();
			Ктрл::добавьОтпрыскПеред(&newctrl, c);
		}
		pos.вставь(ps);
		pos[ps] = (ps > 0) ? pos[ps-1] : 0;
	}
	else {
		Сплиттер::добавь(newctrl);
		Сплиттер::pos.добавь(0);
		ps = cnt;
		if (ps > 0) pos[ps] = pos[ps-1];
	}
	if (save) 
		SavePos();
	if (animate)
		StartAnimate(ps, sz, false);
	else {
		if (cnt)
			SmartReposUp(pos, ps, tsz);
		else
			pos[ps] = 10000;
		Выкладка();
	}
}

void DockPane::Undock(Ктрл& child, bool animate, bool restore)
{
	if (IsAnimating())
		EndAnimate();	
	
	int ix = GetChildIndex(&child);
	if (animate && дайПервОтпрыск() != GetLastChild()) {
		dummy.удали();
		разверни(child, dummy);
		StartAnimate(ix, Размер(0, 0), restore);
	}
	else {
		if (restore) 
			RestorePos();
		SmartReposDown(pos, ix);
		child.удали();
		pos.удали(ix);	
		Выкладка();	
	}
}

Прям DockPane::GetFinalAnimRect(Ктрл& ctrl)
{
	ПРОВЕРЬ(ctrl.дайРодителя() == this);
	if (!IsAnimating())
		return ctrl.дайПрям();
	int ix = GetChildIndex(&ctrl);
	ПРОВЕРЬ(ix >= 0 && ix < animpos.дайСчёт());
	
	Прям r = дайПрям();
	int prev = ix ? animpos[ix-1] : 0;
	int width = GetSplitWidth();
	if (горизонтален()) {
		r.left += PosToClient(prev) + width*(ix);
		r.right = r.left + PosToClient(animpos[ix] - prev);				
	}
	else {
		r.top += PosToClient(prev) + width*(ix);
		r.bottom = r.top + PosToClient(animpos[ix] - prev);
	}
	return r;	
}

DockPane::DockPane()
{
	animtick = 0;
	animinterval = 20; // milliseconds
	animmaxticks = 10;	
	pos.очисть();
}

}
