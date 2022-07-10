#include <CtrlLib/CtrlLib.h>

namespace РНЦП {

const Точка Calendar::nullday = Точка(-1, -1);

void PopUpCtrl::PopUp(Ктрл *owner, const Прям& rt)
{
	закрой();
	переустанов();
	устПрям(rt);
	Ктрл::PopUp(owner, true, true, GUI_DropShadows());
}

void PopUpCtrl::откл()
{
	if(WhenDeactivate)
	{
		WhenDeactivate();
	}
	else if(открыт() && IsPopUp())
	{
		WhenPopDown();
		IgnoreMouseClick();
		закрой();
	}
}

bool PopUpCtrl::IsPopUp() const
{
	return popup || Ктрл::IsPopUp();
}

void PopUpCtrl::SetPopUp(bool b /* = true*/)
{
	popup = b;
}

Calendar::Calendar()
{
	устФрейм(фреймЧёрный());

	добавь(spin_year);
	добавь(spin_month);
	добавь(spin_all);

	spin_month.SetCallbacks(THISBACK(OnMonthLeft), THISBACK(OnMonthRight));
	spin_month.SetTips(t_("Previous month"), t_("следщ month"));
	spin_year.SetCallbacks(THISBACK(OnYearLeft), THISBACK(OnYearRight));
	spin_year.SetTips(t_("Previous year"), t_("следщ year"));
	spin_all.SetCallbacks(THISBACK(OnMonthLeft), THISBACK(OnMonthRight));
	spin_all.SetTips(t_("Previous month"), t_("следщ month"));

	time_mode = false;
	swap_month_year = false;
	one_button = false;
	first_day = MONDAY;
	selall = true;
	bs = 5;
	устСтиль(дефСтиль());
	переустанов();
	
	вычислиРазм();
}

void Calendar::переустанов()
{
	today = дайСисВремя();
	view = today;
	view.day = 0;
	sel = today;

	istoday = false;
	wastoday = false;

	newday = oldday = nullday;
	newweek = oldweek = -1;
	stoday = фмт("%s: %s", t_("Today"), time_mode ? фмт(today) : фмт(Дата(today)));

	UpdateFields();
}

Calendar& Calendar::устСтиль(const Стиль& s)
{
	style = &s;
	nbg = CtrlImg::Bg();
	освежи();
	return *this;
}

void Calendar::OnMonthLeft()
{
	if(--view.month < 1)
	{
		view.month = 12;
		if(view.year > 0)
			view.year--;
	}
	UpdateFields();
}

void Calendar::OnMonthRight()
{
	if(++view.month > 12)
	{
		view.month = 1;
		view.year++;
	}
	UpdateFields();
}

void Calendar::OnYearLeft()
{
	if(view.year > 0) view.year--;
	UpdateFields();
}

void Calendar::OnYearRight()
{
	view.year++;
	UpdateFields();
}

void Calendar::UpdateFields()
{
	Ткст month = имяМесяца(view.month - 1);
	Ткст year = какТкст(view.year);
	spin_month.устТекст(month);
	spin_year.устТекст(year);
	if(swap_month_year)
		spin_all.устТекст(month + " " + year);
	else
		spin_all.устТекст(year + " " + month);
	освежи();
}

int	Calendar::деньНедели(int day, int month, int year, int zelleroffset)
{
	if(month < 3)
	{
		month += 12 ;
		year -= 1 ;
    }

	int n1 = (26 * (month + 1)) / 10;
	int n2 = (125 * year) / 100;

	return ((day + (8 - first_day) + n1 + n2 - (year / 100) + (year / 400) - zelleroffset) % 7);
}

int Calendar::WeekOfYear(int day, int month, int year) /* ISO-8601 */
{
	const static int mnth[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
	int weeknum;

	bool cleapyear = високосенГод(year);
	bool bleapyear = високосенГод(year - 1);

	int daynum = day + mnth[month - 1];
	if(cleapyear && month > 2)
		daynum += 1;

	int yy = (year - 1) % 100;
	int c = (year - 1) - yy;
	int g = yy + yy / 4;
	int jan1weekday = 1 + (((((c / 100) % 4) * 5) + g) % 7);
	int h = daynum + (jan1weekday - 1);
	int weekday = 1 + ((h -1) % 7);

	int yearnum;

	if(daynum <= (8 - jan1weekday) && jan1weekday > 4)
	{
		yearnum = year - 1;
		if(jan1weekday == 5 || (jan1weekday == 6 && bleapyear))
			weeknum = 53;
		else
			weeknum = 52;
	}
	else
		yearnum = year;

	if(yearnum == year)
	{
		int yeardays = cleapyear ? 366 : 365;
		if((yeardays - daynum) < (4 - weekday))
		{
			yearnum = year + 1;
			weeknum = 1;
		}
	}
	if(yearnum == year)
	{
		int fday = daynum + (7 - weekday) + (jan1weekday -1);
		weeknum = fday / 7;
		if(jan1weekday > 4)
			weeknum -= 1;
	}

	return weeknum;
}

void Calendar::леваяВнизу(Точка p, dword keyflags)
{
	if(WhenWeek && newweek >= 0) {
		if(PopUpCtrl::IsPopUp())
			откл();
		Дата d = view;
		int wday = days[newweek][0];
		d.day = wday;
		if(wday < 0) {
			d.day = -wday;
			if(newweek == 0)
				d.month--;
			else
				d.month++;
			if(d.month < 1) {
				d.month = 12;
				d.year--;
			}
			if(d.month > 12) {
				d.month = 1;
				d.year++;
			}
		}
		WhenWeek(d);
		return;
	}

	bool isnewday = newday != nullday;
	int day = 1;
	bool refall = false;
	if(isnewday)
	{
		day = Day(newday);
		if(!selall && day < 0) return;

		if(day < 0)
		{
			view.day = -day;
			if(newday.y == 0)
				view.month--;
			else
				view.month++;

			if(view.month < 1)
			{
				view.month = 12;
				view.year--;
			}
			if(view.month > 12)
			{
				view.month = 1;
				view.year++;
			}
			refall = true;
		}
	}
	else if(!istoday)
		return;

	Время tm = istoday ? today : view;

	if(time_mode)
		WhenTime(tm);
	else
	{
		tm.hour = 0;
		tm.minute = 0;
		tm.second = 0;
	}

	if(PopUpCtrl::IsPopUp())
	{
		sel = tm;
		откл();
		WhenAction();
	}
	else
	{
		sel = tm;
		WhenAction();

		if(refall)
		{
			view.day = 0;
			освежи();
			синх();
			oldday = nullday;
			двигМыши(p, 0);
			return;
		}

		освежи();
		if(istoday)
			SetDate(tm);
		else
			освежи();
	}

	WhenSelect();
}

void Calendar::двигМыши(Точка p, dword keyflags)
{
	newday = GetDay(p);
	if(newday != oldday)
	{
		if(oldday != nullday)
		{
			bool b = (selall == false ? Day(oldday) > 0 : true);
			if(b)
			{
				view.day = 0;
				освежи();
			}
		}
		if(newday != nullday)
		{
			bool b = (selall == false ? Day(newday) > 0 : true);
			if(b)
			{
				view.day = Day(newday);
				освежи();
			}
		}
		oldday = newday;
	}
	newweek = GetWeek(p);
	if(newweek != oldweek && WhenWeek) {
		освежи();
		oldweek = newweek;
	}
	istoday = MouseOnToday(p);
	if(istoday != wastoday)
	{
		wastoday = istoday;
		освежи();
	}
}

Рисунок Calendar::рисКурсора(Точка p, dword keyflags)
{
	bool b = (selall == false ? Day(newday) > 0 : true);
	if((newday != nullday && b) || istoday)
		return Рисунок::Hand();
	else
		return Рисунок::Arrow();
}

bool Calendar::MouseOnToday(Точка p)
{
	Размер sz = дайРазм();
	int x0 = (sz.cx - sztoday.cx) / 2;
	int x1 = x0 + sztoday.cx;
	int y0 = sz.cy - (rh + sztoday.cy) / 2;
	int y1 = y0 + sztoday.cy;
	return (p.x >= x0 && p.x <= x1 && p.y >= y0 && p.y <= y1);
}

Точка Calendar::GetDay(Точка p)
{
	for(int i = 0; i < rows; i++)
	{
		int y0 = 2 + (int)((i + 1) * rowh + hs);
		int y1 = y0 + rh;

		if(p.y >= y0 && p.y < y1) {
			for(int j = 0; j < cols; j++)
			{
				int x0 = bs + 2 + (int)((j + 1) * colw);
				int x1 = x0 + cw;
				if(p.x >= x0 && p.x < x1)
					return Точка(j, i);
			}
		}
	}
	return Точка(-1, -1);
}

int Calendar::GetWeek(Точка p)
{
	for(int i = 0; i < rows; i++)
	{
		int y0 = 2 + (int)((i + 1) * rowh + hs);
		int y1 = y0 + rh;
		if(p.y >= y0 && p.y < y1 && p.x < bs + 2 + colw)
			return i;
	}
	return -1;
}

Размер Calendar::вычислиРазм()
{
	const Стиль &st = *style;
	Шрифт fnt = st.font;
	spin_all.устШрифт(fnt);
	spin_month.устШрифт(fnt);
	spin_year.устШрифт(fnt);

	spin_all.SetLeftImage(st.spinleftimg);
	spin_all.SetRightImage(st.spinrightimg);
	spin_month.SetLeftImage(st.spinleftimg);
	spin_month.SetRightImage(st.spinrightimg);
	spin_year.SetLeftImage(st.spinleftimg);
	spin_year.SetRightImage(st.spinrightimg);

	spin_all.SetHighlight(st.spinhighlight);
	spin_month.SetHighlight(st.spinhighlight);
	spin_year.SetHighlight(st.spinhighlight);

	Размер sz = IsPopUp() ? Размер(-1, -1) : дайРазм();
	Размер tsz = дайРазмТекста("WW", fnt.NoBold().NoUnderline());
	Размер rsz;

	fh = tsz.cy;

	colw = (float)(tsz.cx + 6);
	rowh = (float)(tsz.cy + 4);
	hs = /*spin_all.дайВысоту()*/tsz.cy + 4;

	rsz.cx = bs * 2 + 2 + (int)(colw * (cols + 1));
	rsz.cy = (int)(rowh * (rows + 2) + hs);

	if(sz.cx > rsz.cx)
	{
		colw = (sz.cx - bs * 2 + 2) / (float) (cols + 1);
		rsz.cx = sz.cx;
	}
	if(sz.cy > rsz.cy)
	{
		rowh = (sz.cy - hs) / (float) (rows + 2);
		rsz.cy = sz.cy;
	}

	cw = (int)(colw);
	rh = (int)(rowh);

	ts = rh;

	return rsz;
}

void Calendar::рисуй(Draw &w)
{
	const Стиль &st = *style;
	Шрифт fnt = st.font;

	Размер sz = дайРазм();

	Размер tsz;
	Ткст str;
	int d = 1;
	int m = view.month;
	int y = view.year;

	if(w.IsPainting(0, 0, sz.cx, hs))
	{
		w.DrawImage(0, 0, sz.cx, hs, nbg);
		curdate = фмт("%s %d", имяМесяца(view.month - 1), view.year);
	}
	w.DrawRect(0, hs, sz.cx, sz.cy - ts - hs, st.bgmain);

	if(w.IsPainting(0, hs, sz.cx, rh))
	{
		int y = (int) (hs + (rowh - fh) / 2.0);
		fnt.NoBold().NoUnderline();
		tsz = дайРазмТекста(t_("Wk"), fnt);
		w.DrawText(bs + (cw - tsz.cx) / 2, y, t_("Wk"), fnt, st.week);

		for(int i = 0; i < cols; i++)
		{
			const Ткст &s = DyName((i + first_day) % 7);
			tsz = дайРазмТекста(s, fnt);
			w.DrawText(bs  + 2 + (int)((i + 1) * colw) + (cw - tsz.cx) / 2, y, s, fnt, st.dayname);
		}
	}

	if(w.IsPainting(0, sz.cy - ts, sz.cx, ts))
	{
		sztoday = дайРазмТекста(stoday, fnt.Bold().Underline(istoday));
		w.DrawRect(0, sz.cy - ts, sz.cx, ts, st.bgtoday);
		w.DrawText((sz.cx - sztoday.cx) / 2, sz.cy - (ts + sztoday.cy) / 2, stoday, fnt, istoday ? st.selecttoday : st.today);
	}

	int fd = деньНедели(1, view.month, view.year);

	if(fd > 0)
	{
		if(--m < 1)
		{
			m = 12;
			y -= 1;
		}
		d = дайДниМесяца(m, y) - fd + 1;
	}
	
	for(int i = 0; i < rows; i++)
	{
		int yp = 2 + (int) ((i + 1) * rowh + hs);
		int yc = (rh - fh) / 2;

		str = какТкст(WeekOfYear(d, m, y));
		
		Шрифт wfnt = fnt;
		wfnt.NoBold().NoUnderline();
		Цвет wcolor = st.week;
		if(newweek >= 0 && WhenWeek && newweek == i) {
			wfnt.Bold().Underline();
			wcolor = st.selectday;
		}
		
		w.DrawText(bs + (cw - дайРазмТекста(str, wfnt).cx) / 2, yp + yc, str, wfnt, wcolor);

		for(int j = 0; j < cols; j++)
		{
			int xp = bs + 2 + (int)((j + 1) * colw);

			if(w.IsPainting(xp, yp, cw, rh))
			{
				Цвет fg = st.fgmain;
				Цвет bg = st.bgmain;

				fnt.NoBold().NoUnderline();
				bool special = false;
				int sd = d;

				if(m == view.month)
				{
					Day(j, i) = d;

					fg = SBlack;

					if(j == 6)
					{
						fg = SRed;
					}
					if(d == 1)
					{
						firstday.x = j;
						firstday.y = i;
					}
					if(d == today.day && m == today.month && y == today.year)
					{
						fg = st.fgtoday;
						bg = st.bgtoday;
						fnt.Bold();
						special = true;

						curday.x = j;
						curday.y = i;
					}
					if(d == sel.day && m == sel.month && y == sel.year)
					{
						fg = st.fgselect;
						bg = st.bgselect;
						fnt.Bold();
						special = true;

						prevday.x = j;
						prevday.y = i;
					}
				}
				else
				{
					fg = st.outofmonth;
					sd = d > 0 ? -d : d;
					Day(j, i) = sd;
				}
				if(sd == char(view.day))
				{
					if(sd < 0 && selall)
					{
						fg = st.outofmonth;
						fnt.Bold().Underline(!special);
					}
					if(sd > 0 && view.month == m)
					{
						fg = st.selectday;
						fnt.Bold().Underline(!special);
					}
				}

				w.DrawRect(xp, yp, cw, rh, bg);
				if(special)
				{
					DrawFrame(w, xp + 1, yp + 1, cw - 2, rh - 2, SBlack);
					DrawFrame(w, xp, yp, cw, rh, st.bgmain);
				}

				str = какТкст(абс(d));
				w.DrawText(xp + (cw - дайРазмТекста(str, fnt).cx) / 2, yp + yc , str, fnt, fg);
			}

			if(++d > дайДниМесяца(m, y))
			{
				d = 1;
				if(++m > 12)
				{
					m = 1;
					y += 1;
				}
			}
		}
	}

	w.DrawRect(bs, (int) (hs + rowh) + 1, sz.cx - bs * 2, 1, st.line);
	w.DrawRect(bs + cw + 1, hs + bs, 1, sz.cy - hs - ts - bs * 1, st.line);

	lastrow = row;
}

void Calendar::State(int reason)
{
	if(reason == OPEN)
	{
		вычислиРазм();

		int yw = 0;
		int mw = 0;
		for(int i = 0; i < 12; i++)
			mw = max(mw, spin_month.дайШирину(имяМесяца(i)));

		if(one_button)
		{
			yw = spin_year.дайШирину(" 0000", false);
			spin_all.HCenterPos(mw + yw, 0).TopPos(0, hs);
		}
		else
		{
			yw = spin_year.дайШирину("0000");

			spin_month.TopPos(0, hs);
			spin_year.TopPos(0, hs);
			if(swap_month_year)
			{
				spin_month.RightPos(0, mw);
				spin_year.LeftPos(0, yw);
			}
			else
			{
				spin_month.LeftPos(0, mw);
				spin_year.RightPos(0, yw);
			}
		}
	}
}

bool Calendar::Ключ(dword ключ, int count)
{
	if(ключ == K_ESCAPE)
	{
		откл();
		return true;
	}

	return Ктрл::Ключ(ключ, count);
}

void Calendar::выходМыши()
{
	if(istoday)
	{
		istoday = false;
		wastoday = false;
		освежи();
	}
	else
		двигМыши(Точка(-1, -1), 0);
}

Дата Calendar::GetDate() const
{
	return sel;
}

void Calendar::SetDate(int y, int m, int d)
{
	SetDate(Дата(y, m, d));
}

void Calendar::SetDate(const Дата &dt)
{
	sel = !пусто_ли(dt) && dt.пригоден() ? Время(dt.year, dt.month, dt.day, 0, 0, 0) : today;
	view = sel;
	view.day = 0;
	UpdateFields();
}

Дата Calendar::дайВремя() const
{
	return sel;
}

void Calendar::SetTime(int y, int m, int d, int h, int n, int s)
{
	SetTime(Время(y, m, d, h, n, s));
}

void Calendar::SetTime(const Время &tm)
{
	sel = !пусто_ли(tm) && tm.пригоден() ? tm : today;
	view = sel;
	view.day = 0;
	UpdateFields();
}

void Calendar::PopUp(Ктрл *owner, Прям &rt)
{
	закрой();
	переустанов();
	SetPopUp(true);
	устПрям(rt);
	Ктрл::PopUp(owner, true, true, GUI_DropShadows());
	устФокус();
}

void Calendar::SetView(const Время &v)
{
	view = v;
	view.day = 0;
	UpdateFields();
}

// Clock

void LineCtrl::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	w.DrawRect(sz, Цвет(240, 240, 240));

	if(real_pos < 0)
		return;

	if(sz.cx > sz.cy)
		w.DrawRect(Прям(0, 0, min(sz.cx, real_pos), sz.cy), смешай(SColorHighlight(), белый, 200));
	else
		w.DrawRect(Прям(0, max(0, real_pos), sz.cx, sz.cy), смешай(SColorHighlight(), белый, 200));
}

void LineCtrl::устПоз(Точка p)
{
	Размер sz = дайРазм();

	bool vh = sz.cy > sz.cx;
	int mp = max(sz.cx, sz.cy);
	real_pos = vh ? p.y : p.x;

	if(real_pos < 0)
		real_pos = 0;
	else if(real_pos > mp)
		real_pos = mp;

	pos = (vh ? mp - real_pos : real_pos) * 100 / mp;
	освежи();
	WhenAction();
}

int LineCtrl::дайПоз()
{
	return pos;
}

void LineCtrl::устПоз(int p)
{
	Размер sz = дайРазм();
	pos = p;
	real_pos = p * max(sz.cx, sz.cy) / 100;
	if(sz.cy > sz.cx)
		real_pos = sz.cy - real_pos;
	освежи();
}

void LineCtrl::леваяВнизу(Точка p, dword keyflags)
{
	SetCapture();
	устПоз(p);
}

void LineCtrl::двигМыши(Точка p, dword keyflags)
{
	if(HasCapture())
		устПоз(p);
}

void LineCtrl::леваяВверху(Точка p, dword keyflags)
{
	ReleaseCapture();
}

LineCtrl::LineCtrl()
{
	pos = 0;
	real_pos = -1;
	BackPaint();
}

//Clock

Clock& Clock::устСтиль(const Стиль& s)
{
	style = &s;
	nbg = CtrlImg::Bg();
	освежи();
	return *this;
}

void Clock::PaintPtr(int n, Draw& w, ТочкаПЗ p, double pos, double m, double rd, int d, Цвет color, Точка cf)
{
	double dx = m * sin(pos * 2 * M_PI);
	double dy = m * cos(pos * 2 * M_PI);

	lines[n].s.x = p.x - dx * 35 / 2.0;
	lines[n].s.y = p.y + dy * 35 / 2.0;
	lines[n].e.x = p.x + dx * (cf.x - rd);
	lines[n].e.y = p.y - dy * (cf.y - rd);

	w.DrawLine(lines[n].s, lines[n].e, d, color);
}

void Clock::PaintCenteredText(Draw& w, int x, int y, const char *text, const Шрифт& fnt, Цвет c)
{
	Размер tsz = дайРазмТекста(text, fnt);
	w.DrawText(x - tsz.cx / 2, y - tsz.cy / 2, text, fnt, c);
}

void Clock::PaintCenteredImage(Draw &w, int x, int y, const Рисунок& img)
{
	Размер isz = img.дайРазм();
	w.DrawImage(x - isz.cx / 2, y - isz.cy / 2, img);
}

bool Clock::Ключ(dword ключ, int count)
{
	if(ключ == K_TAB || ключ == K_SHIFT_TAB)
	{
		if(ed_minute.естьФокус())
			ed_hour.устФокус();
		else if(ed_hour.естьФокус())
			ed_minute.устФокус();
		else
			ed_hour.устФокус();
		return true;
	}
	return Ктрл::Ключ(ключ, count);
}

void Clock::CalcSizes()
{
	int margin = 3;
	sz = дайРазм();
	int cx = sz.cx - 7 * 2;
	int cy = sz.cy - 2 * margin - hs;
	cm.x = cx / 2 + 7;
	cm.y = cy / 2 + margin + hs;
	cf.x = min(cy / 2, cx / 2);
	cf.y = cf.x;
}

void Clock::рисуй(Draw& w)
{
	const Стиль &st = *style;

	CalcSizes();

	w.DrawRect(sz, st.bgmain);
	w.DrawImage(0, 0, sz.cx, hs, nbg);
	
	w.Clip(0, hs, sz.cx, sz.cy - hs);

	if(colon)
		PaintCenteredText(w, sz.cx / 2, hs / 2 - 1, " : ", StdFont().Bold(), SColorHighlightText());

	//w.DrawEllipse(cm.x - r / 2, cm.y - r / 2, cf.x, cf.x, смешай(st.header, белый, 250), PEN_NULL, чёрный);

	Шрифт fnt = st.font;
	fnt.устВысоту(max(6, min(16, fnt.дайВысоту() * int(cf.x) / Ктрл::VertLayoutZoom(65))));

	for(int i = 1; i <= 12; i++) {
		PaintCenteredText(w,
		                  int(cm.x + 0.8 * sin(i * M_PI / 6.0) * cf.x),
		                  int(cm.y - 0.8 * cos(i * M_PI / 6.0) * cf.y),
		                  какТкст(i), fnt.Bold(i % 3 == 0), SColorText());
	}

	for(int i = 1; i <= 60; i++) {
		int x = int(cm.x + 0.95 * sin(i * M_PI / 30.0) * cf.x);
		int y = int(cm.y - 0.95 * cos(i * M_PI / 30.0) * cf.y);
		PaintCenteredImage(w, x, y,
		                   cur_point == i ? CtrlImg::BigDotH()
		                                  : i % 5 == 0 ? CtrlImg::BigDot() : CtrlImg::SmallDot());
	}
	
	PaintPtr(0, w, cm, cur_time / 3600.0 / 12.0, 0.5, 3.0, 5, cur_line == 0 ? st.arrowhl : st.arrowhour, cf);
	PaintPtr(1, w, cm, cur_time / 3600.0, 0.6, 3.0, 3, cur_line == 1 ? st.arrowhl : st.arrowminute, cf);
	if(seconds)
		PaintPtr(2, w, cm, cur_time / 60.0, 0.75, 5.0, 2, cur_line == 2 ? st.arrowhl : st.arrowsecond, cf);
	
	w.стоп();
}

int Clock::GetDir(int pp, int cp)
{
	dir = 0;
	if(cp >= 1 && cp <= 30 && pp >= 1 && pp <= 30 ||
	   cp > 30 && cp <= 60 && pp > 30 && pp <= 60)
	{
		dir = pp < cp ? 1 : -1;
	}
	else //there is a small problem on 30 when direction is opposite to "correct dir". Fortunatelly it won't hurt us hour/minute change is on 60
	{
		if(pp <= 60 && pp > 30 && cp >= 1)
			dir = 1;
		else if(pp >= 1 && cp <= 60 && cp > 30)
			dir = -1;
	}

	return dir;
}

Clock::минмакс Clock::SetMinMax(int v, int min, int max)
{
	минмакс mm;
	if(v > max)
	{
		mm.diff = 1;
		mm.значение = min;
	}
	else if(v < min)
	{
		mm.diff = -1;
		mm.значение = max;
	}
	else
	{
		mm.diff = 0;
		mm.значение = v;
	}
	return mm;
}

int Clock::IncFactor(int dir, int pp, int cp)
{
	if(dir > 0 && (pp < 60 && pp > 45 && (cp == 60 || (cp >= 1 && cp < 15))))
		return 1;
	if(dir < 0 && ((pp == 60 || (pp >= 1 && pp < 15)) && cp < 60 && cp > 45))
		return -1;

	return 0;
}

void Clock::двигМыши(Точка p, dword keyflags)
{
	if(!HasCapture())
	{
		cur_line = GetPointedLine(p);
		if(cur_line != prv_line)
		{
			освежи();
			prv_line = cur_line;
		}
		if(cur_line < 0)
			accept_time = !IsCircle(p, cm, cf.x / 2);
	}
	else
	{
		cur_point = GetPoint(p);
		if(cur_point != prv_point && cur_point >= 0)
		{
			int dir = GetDir(prv_point, cur_point);
			int inc = IncFactor(dir, prv_point, cur_point);
			int cp  = cur_point == 60 ? 0 : cur_point;

			if(cur_line == 0)
			{
				int h = cp / 5;
				if(sel.hour >= 12)
					h += 12;
				if(inc > 0)
					h += 12;
				else if(inc < 0)
					h -= 12;

				sel.hour = SetMinMax(h, 0, 23).значение;
			}
			else if(cur_line == 1)
			{
				sel.hour = SetMinMax(sel.hour + inc, 0, 23).значение;
				sel.minute = cp;
			}
			else
			{
				минмакс mm = SetMinMax(sel.minute + inc, 0, 59);
				int hour = int(sel.hour) + mm.diff;
				sel.hour = SetMinMax(hour, 0, 23).значение;
				sel.minute = mm.значение;
				sel.second = cp;
			}

			UpdateTime();
			prv_point = cur_point;
		}
	}
}

void Clock::леваяВнизу(Точка p, dword keyflags)
{
	if(cur_line >= 0)
		SetCapture();
	else if(accept_time)
	{
		откл();
		WhenAction();
	}
}

void Clock::леваяВверху(Точка p, dword keyflags)
{
	ReleaseCapture();
	if(cur_point >= 0)
	{
		cur_point = -1;
		prv_point = -1;
		освежи();
	}
}

Рисунок Clock::рисКурсора(Точка p, dword keyflags)
{
	if(cur_line >= 0 || accept_time)
		return CtrlImg::HandCursor();
	else
		return Рисунок::Arrow();
}

int Clock::GetPointedLine(Точка p)
{
	for(int i = 0; i < 2 + int(seconds); i++)
		if(IsLine(lines[i].s, lines[i].e, p))
			return i;
	return -1;
}

bool Clock::IsLine(ТочкаПЗ s, ТочкаПЗ e, ТочкаПЗ p, double tolerance)
{
	double minx = min(s.x, e.x);
	double maxx = max(s.x, e.x);
	double miny = min(s.y, e.y);
	double maxy = max(s.y, e.y);

	if(p.x < minx - tolerance) return false;
	if(p.x > maxx + tolerance) return false;
	if(p.y < miny - tolerance) return false;
	if(p.y > maxy + tolerance) return false;

	double dx = e.x - s.x;
	double dy = e.y - s.y;
	double u = (p.x - s.x) * dy - (p.y - s.y) * dx;
	u *= u;
	double l = dx * dx + dy * dy;
	if(l == 0.0)
		return false;
	return u / l < tolerance * tolerance;
}

bool Clock::IsCircle(ТочкаПЗ p, ТочкаПЗ s, double r)
{
	double dx = p.x - s.x;
	double dy = p.y - s.y;
	return dx * dx + dy * dy < r * r;
}

int Clock::GetPoint(ТочкаПЗ p, double tolerance /* = 4.0*/)
{
	double tcx = 0.95 * cf.x;
	double tcy = 0.95 * cf.y;

	double dx = p.x - cm.x;
	double dy = p.y - cm.y;

	double r = sqrt(double(dx * dx + dy * dy));
	double sa = r != 0 ? dx / r : 0;
	double ca = r != 0 ? dy / r : 0;

	p.x = cm.x + tcx * sa;
	p.y = cm.y + tcy * ca;

	for(int i = 1; i <= 60; i++) {
		double x = cm.x + tcx * sin(i * M_PI / 30);
		double y = cm.y - tcy * cos(i * M_PI / 30);
		if(fabs(p.x - x) + fabs(p.y - y) < tolerance)
			return i;
	}
	return -1;
}

void Clock::Timer()
{
	освежи();
}

void Clock::UpdateFields()
{
	ed_hour <<= sel.hour;
	ed_minute <<= sel.minute;
	ln_minute.устПоз(sel.minute * 100 / 59);
	ln_hour.устПоз(sel.hour * 100 / 23);
	spin_hour.устТекст(фмт("%.2d", sel.hour));
	spin_minute.устТекст(фмт("%.2d", sel.minute));
}

void Clock::SetHourEdit()
{
	sel.hour = int(~ed_hour);
	UpdateTime();
}

void Clock::SetMinuteEdit()
{
	sel.minute = int(~ed_minute);
	UpdateTime();
}

void Clock::SetHourLine()
{
	sel.hour = ln_hour.дайПоз() * 23 / 100;
	UpdateTime();
}

void Clock::SetMinuteLine()
{
	sel.minute = ln_minute.дайПоз() * 59 / 100;
	UpdateTime();
}

void Clock::SetHourLeft()
{
	sel.hour = SetMinMax(sel.hour - 1, 0, 23).значение;
	UpdateTime();
}

void Clock::SetHourRight()
{
	sel.hour = SetMinMax(sel.hour + 1, 0, 23).значение;
	UpdateTime();
}

void Clock::SetMinuteLeft()
{
	минмакс mm = SetMinMax(sel.minute - 1, 0, 59);
	sel.minute = mm.значение;
	sel.hour = SetMinMax(sel.hour + mm.diff, 0, 23).значение;
	UpdateTime();
}

void Clock::SetMinuteRight()
{
	минмакс mm = SetMinMax(sel.minute + 1, 0, 59);
	sel.minute = mm.значение;
	sel.hour = SetMinMax(sel.hour + mm.diff, 0, 23).значение;
	UpdateTime();
}

void Clock::UpdateTime()
{
	cur_time = int(sel.hour) * 3600 + int(sel.minute) * 60 + int(sel.second);
	UpdateFields();
	освежи();
}

Размер Clock::вычислиРазм()
{
	const Стиль &st = *style;
	spin_hour.устШрифт(st.font);
	spin_minute.устШрифт(st.font);
	Шрифт fnt = st.font;
	Размер tsz = дайРазмТекста("W", st.font);
	hs = tsz.cy + 4;
	//TODO: найди better scaling method.
	double d = fnt == StdFont() ? 1 : tsz.cy / double(tsz.cx);
	return Размер(int(150.0 * d), int(157.0 * d));
}

void Clock::State(int reason)
{
	if(reason == OPEN)
	{
		вычислиРазм();
		//spin_hour.LeftPos(0, spin_hour.дайШирину("00")).TopPos(0, hs);
		//spin_minute.RightPos(0, spin_minute.дайШирину("00")).TopPos(0, hs);
		int shw = spin_hour.дайШирину("00");
		int smw = spin_minute.дайШирину("00");
		spin_hour.HCenterPos(shw, -shw / 2).TopPos(0, hs);
		spin_minute.HCenterPos(smw, smw / 2).TopPos(0, hs);
		ln_minute.RightPos(0, 5).VSizePos(hs, 0);
		ln_hour.LeftPos(0, 5).VSizePos(hs, 0);
	}
}

Значение Clock::дайДанные() const
{
	return sel;
}

void Clock::устДанные(const Значение& v)
{
	SetTime((Время) v);
}

Время Clock::дайВремя() const
{
	return sel;
}

void Clock::SetTime(const Время& tm)
{
	sel = !пусто_ли(tm) && tm.пригоден() ? tm : дайСисВремя();
	UpdateTime();
}

void Clock::SetTime(int h, int n, int s)
{
	sel.hour = h;
	sel.minute = n;
	sel.second = s;
	UpdateTime();
}

void Clock::переустанов()
{
	sel = дайСисВремя();
	UpdateTime();
}

Clock::Clock()
{
	sel = дайСисВремя();
	UpdateTime();

	добавь(ed_hour);
	добавь(ed_minute);
	добавь(ln_hour);
	добавь(ln_minute);
	добавь(spin_hour);
	добавь(spin_minute);

	ed_hour.мин(0).макс(23);
	ed_minute.мин(0).макс(59);
	ed_hour <<= THISBACK(SetHourEdit);
	ed_minute <<= THISBACK(SetMinuteEdit);
	ln_hour <<= THISBACK(SetHourLine);
	ln_minute <<= THISBACK(SetMinuteLine);
	spin_hour.SetCallbacks(THISBACK(SetHourLeft), THISBACK(SetHourRight));
	spin_hour.SetTips(t_("Previous hour"), t_("следщ hour"));
	spin_minute.SetCallbacks(THISBACK(SetMinuteLeft), THISBACK(SetMinuteRight));
	spin_minute.SetTips(t_("Previous minute"), t_("следщ minute"));

	cur_line = -1;
	prv_line = -1;
	cur_hour = -1;
	cur_minute = -1;
	cur_second = -1;

	prv_point = -1;
	cur_point = -1;

	устСтиль(дефСтиль());
	seconds = true;
	colon = false;
	accept_time = false;

	устФрейм(фреймЧёрный());
	BackPaint();
	
	вычислиРазм();
}

// CalendarClock

CalendarClock::CalendarClock(int m) : mode(m)
{
	if(mode == MODE_TIME)
	{
		добавь(clock);
		clock.устФрейм(фреймПусто());
		calendar.устФрейм(фреймПусто());
		устФрейм(фреймЧёрный());
		calendar.TimeMode();
		calendar.WhenTime = THISBACK(UpdateTime);
	}

	добавь(calendar);
	calendar.WhenDeactivate = THISBACK(откл);
	clock.WhenDeactivate = THISBACK(откл);
}

void CalendarClock::UpdateTime(Время &tm)
{
	tm.hour = clock.GetHour();
	tm.minute = clock.GetMinute();
	tm.second = clock.GetSecond();
}

void CalendarClock::PopUp(Ктрл *owner, const Прям& rt)
{
	закрой();
	calendar.переустанов();
	clock.переустанов();
	calendar.SetPopUp(true);
	clock.SetPopUp(true);
	устПрям(rt);
	Ктрл::PopUp(owner, true, true, GUI_DropShadows());
	устФокус();
}

Размер CalendarClock::вычислиРазм()
{
	calendar_size = calendar.GetPopUpSize();
	clock_size = clock.GetPopUpSize();
	return mode == MODE_DATE ? calendar_size
	                         : Размер(calendar_size.cx + clock_size.cx + 2, max(calendar_size.cy, clock_size.cy));
}

void CalendarClock::Выкладка()
{
	sz = вычислиРазм();
	calendar.LeftPos(0, calendar_size.cx).VSizePos(0, 0);
	clock.RightPos(0, clock_size.cx).VSizePos(0, 0);
}

void CalendarClock::откл()
{
	if(открыт() && IsPopUp())
	{
		WhenPopDown();
		IgnoreMouseUp();
		закрой();
		calendar.SetPopUp(false);
		clock.SetPopUp(false);
	}
}

bool CalendarClock::Ключ(dword ключ, int count)
{
	if(ключ == K_ESCAPE)
	{
		откл();
		return true;
	}

	return Ктрл::Ключ(ключ, count);
}

DropDate::DropDate() : DateTimeCtrl<EditDate>(CalendarClock::MODE_DATE)
{}

DropDate& DropDate::SetDate(int y, int m, int d)
{
	EditDate::устДанные(Дата(y, m, d));
	return *this;
}

DropTime::DropTime() : DateTimeCtrl<EditTime>(CalendarClock::MODE_TIME)
{}

DropTime& DropTime::SetTime(int y, int m, int d, int h, int n, int s)
{
	EditTime::устДанные(Время(y, m, d, h, n, s));
	return *this;
}

// FlatButton

FlatButton::FlatButton()
{
	hl = смешай(SColorHighlight, белый, 200);
	bg = смешай(SColorHighlight, белый, 50);
	fg = SColorPaper;
	left = true;
	highlight = true;
	Transparent();
}

void FlatButton::DrawFrame(Draw &w, const Прям& r, Цвет lc, Цвет tc, Цвет rc, Цвет bc)
{
	w.DrawRect(r.left, r.top, r.left + 1, r.bottom, lc);
	w.DrawRect(r.right - 1, r.top, r.right, r.bottom, rc);
	w.DrawRect(r.left, r.top, r.right, r.top + 1, tc);
	w.DrawRect(r.left, r.bottom - 1, r.right, r.bottom, bc);
}

void FlatButton::рисуй(Draw &w)
{
	Размер sz = дайРазм();
	Размер isz = img.дайРазм();

	int dx = IsPush() * (left ? -1 : 1);
	int dy = IsPush();

	Точка p((sz.cx - isz.cx) / 2 + dx, (sz.cy - isz.cy) / 2 + dy);

	if(highlight)
		w.DrawImage(p.x, p.y, img, естьМышь() ? SColorHighlightText() : чёрный);
	else
		w.DrawImage(p.x, p.y, img);
}

FlatSpin::FlatSpin()
{
	tsz.очисть();
	left.устРисунок(CtrlImg::smallleft());
	right.устРисунок(CtrlImg::smallright());
	добавь(left);
	добавь(right);
	left.устЛев();
	right.устПрав();
	Transparent();
	font = StdFont().Bold();
	selected = false;
	selectable = false;
}

FlatSpin& FlatSpin::Selectable(bool b /* = true*/)
{
	selectable = b;
	return *this;
}

void FlatSpin::устШрифт(const Шрифт& fnt)
{
	font = fnt;
	font.Bold();
}

int FlatSpin::дайШирину(const Ткст& s, bool with_buttons)
{
	Размер sl = left.GetImage().дайРазм();
	Размер sr = right.GetImage().дайРазм();
	int width = дайРазмТекста(s, font).cx;

	if(with_buttons)
		width += sl.cx + max(8, sl.cx / 2) + sr.cx + max(8, sr.cx / 2);

	return width;
}

int FlatSpin::дайВысоту()
{
	int ih = max(left.GetImage().дайРазм().cy + 8, right.GetImage().дайРазм().cy + 8);
	return max(font.дайВысоту(), ih);
}

void FlatSpin::устТекст(const Ткст& s)
{
	text = s;
	tsz = дайРазмТекста(text, font);
	освежи();
}

void FlatSpin::SetTips(const char *tipl, const char *tipr)
{
	left.Подсказка(tipl);
	right.Подсказка(tipr);
}

void FlatSpin::SetCallbacks(const Событие<>& cbl, const Событие<>& cbr)
{
	left.WhenAction = cbl;
	right.WhenAction = cbr;
}

void FlatSpin::Выкладка()
{
	left.LeftPos(0, left.GetImage().дайРазм().cx + 8).VSizePos();
	right.RightPos(0, right.GetImage().дайРазм().cx + 8).VSizePos();
}

void FlatSpin::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	w.DrawText((sz.cx - tsz.cx) / 2, (sz.cy - tsz.cy) / 2, text, font.Underline(selected), SColorHighlightText());
}

Рисунок FlatSpin::рисКурсора(Точка p, dword keyflags)
{
	if(selectable && selected)
		return CtrlImg::HandCursor();
	else
		return Рисунок::Arrow();
}

void FlatSpin::выходМыши()
{
	selected = false;
	освежи();
}

void FlatSpin::двигМыши(Точка p, dword keyflags)
{
	if(!selectable)
		return;

	Размер sz = дайРазм();
	bool s = selected;
	selected = p.x >= (sz.cx - tsz.cx) / 2 && p.x <= (sz.cx + tsz.cx) / 2;
	if(s != selected)
		освежи();
}

void FlatSpin::леваяВнизу(Точка p, dword keyflags)
{
	if(selected)
		WhenAction();
}

CH_STYLE(Clock, Стиль, дефСтиль)
{
	header        = SColorHighlight;
	bgmain        = SColorPaper;
	fgmain        = SColorText;
	arrowhl       = SGreen;
	arrowhour     = SRed;
	arrowminute   = SBlue;
	arrowsecond   = SBlack;
	font          = StdFont();
}

CH_STYLE(Calendar, Стиль, дефСтиль)
{
	header        = SColorHighlight;

	bgmain        = SColorPaper;
	bgtoday       = SColorPaper;
	bgselect      = AdjustIfDark(Цвет(255, 254, 220));

	fgmain        = SColorText;
	fgtoday       = SBlack;
	fgselect      = SBlack;

	outofmonth    = AdjustIfDark(Цвет(180, 180, 180));
	curdate       = SWhite;
	today         = SColorText;
	selecttoday   = SColorMark;
	cursorday     = SColorText;
	selectday     = SColorMark;
	line          = SGray;
	dayname       = SColorText;
	week          = SColorText;
	font          = StdFont();
	spinleftimg   = CtrlImg::smallleft();
	spinrightimg  = CtrlImg::smallright();
	spinhighlight = true;
}

}
