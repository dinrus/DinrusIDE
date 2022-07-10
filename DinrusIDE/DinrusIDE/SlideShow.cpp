#include "DinrusIDE.h"

struct SlideShow;

struct SlideShowView : RichTextView {
	SlideShow *slideshow;
	Рисунок      cursor;
	
	virtual void леваяВнизу(Точка p, dword keyflags);
	virtual void праваяВнизу(Точка p, dword keyflags);
	virtual void колесоМыши(Точка p, int zdelta, dword keyflags);
	virtual Рисунок рисКурсора(Точка p, dword keyflags);
};

struct SlideShowSettingsDlg : WithSlideShowSettingsLayout<ТопОкно> {
	SlideShow *slideshow;

	typedef SlideShowSettingsDlg ИМЯ_КЛАССА;

	SlideShowSettingsDlg();
};

struct SlideShow : ТопОкно {
	virtual bool Ключ(dword ключ, int count);
	virtual void Выкладка();

	SlideShowSettingsDlg настройки;
	SlideShowView   text;
	RichTextView    title;
	Вектор<Ткст>  path;
	int             page;
	int             rp;

	int             zoom = 16;
	int             margins = 1;
	int             titleh = DPI(200);

	void устСтраницу();
	void Settings();
	void Export();
	
	void сериализуй(Поток& s);

	SlideShow();
	~SlideShow();
};

SlideShowSettingsDlg::SlideShowSettingsDlg()
{
	CtrlLayout(*this, "Настройки");
	
	vcenter <<= true;
	title <<= false;
	
	vcenter << [=] { slideshow->устСтраницу(); };
	title << [=] { slideshow->устСтраницу(); };
	
	zoom_plus.WhenRepeat = zoom_plus ^= [=] { slideshow->Ключ(K_CTRL|K_ADD, 1); };
	zoom_minus.WhenRepeat = zoom_minus ^= [=] { slideshow->Ключ(K_CTRL|K_SUBTRACT, 1); };
	margin_plus.WhenRepeat = margin_plus ^= [=] { slideshow->Ключ(K_ALT|K_ADD, 1); };
	margin_minus.WhenRepeat = margin_minus ^= [=] { slideshow->Ключ(K_ALT|K_SUBTRACT, 1); };
	title_plus.WhenRepeat = title_plus ^= [=] { slideshow->Ключ(K_CTRL|K_ALT|K_ADD, 1); };
	title_minus.WhenRepeat = title_minus ^= [=] { slideshow->Ключ(K_CTRL|K_ALT|K_SUBTRACT, 1); };
	pointer ^= [=] { slideshow->устСтраницу(); };
}

void SlideShow::сериализуй(Поток& s)
{
	int version = 1;
	s / version;
	s % zoom % margins % titleh;
	bool h = ~настройки.title;
	s % h;
	настройки.title <<= h;
	h = ~настройки.vcenter;
	s % h;
	настройки.vcenter <<= h;
	if(version >= 2)
		s % настройки.pointer;
}

ИНИЦБЛОК {
	региструйГлобКонфиг("SlideShow");
};

SlideShow::SlideShow()
{
	text.slideshow = настройки.slideshow = this;
	добавь(text.SizePos());
	text.NoHyperlinkDecoration();
	text.NoSb();
	text.устФрейм(фреймПусто());
	rp = -1;
	title.NoSb();
	добавь(title);
	title.устФрейм(фреймПусто());
	title.SizePos();
	настройки.report << [=] { Export(); };
	грузиИзГлоба(*this, "SlideShow");
}

SlideShow::~SlideShow()
{
	сохраниВГлоб(*this, "SlideShow");
}

void SlideShow::Export()
{
	Report r(6074, 3968);
	bool next = false;
	for(Ткст p : path) {
		if(next)
			r.NewPage();
		Topic t = ReadTopic(загрузиФайл(p));
		Ткст title = "{{1@C~ [*R+" + какТкст(3 * titleh / 2) + " \1" + t.title + "\1}}";
		r << title;
		Размер pgsz = r.GetPageSize();
		int titlecy = ParseQTF(title).дайВысоту(pgsz.cx);
		RichText txt = ParseQTF(t.text);
		txt.ApplyZoom(Zoom(zoom, 27));
		int textcy = txt.дайВысоту(pgsz.cx);
		int cy = pgsz.cy - titlecy;
		if(cy > textcy)
			r.устУ(r.дайУ() + (cy - textcy) / 2);
		r.помести(txt);
		next = true;
	}
	Perform(r);
	Break();
}

void SlideShow::Settings()
{
	if(настройки.открыт())
		настройки.закрой();
	else
		настройки.открой(this);
}

void SlideShowView::леваяВнизу(Точка p, dword keyflags)
{
	int m = DPI(8) * slideshow->margins;
	if(p.x <= m)
		slideshow->Ключ(K_PAGEUP, 1);
	else
	if(p.x >= дайРазм().cx - m)
		slideshow->Ключ(K_PAGEDOWN, 1);
	else
		slideshow->леваяВнизу(p, keyflags);
}

void SlideShowView::праваяВнизу(Точка p, dword)
{
	slideshow->Settings();
}

void SlideShowView::колесоМыши(Точка p, int zdelta, dword keyflags)
{
	if(keyflags & K_CTRL) {
		slideshow->Ключ(zdelta < 0 ? K_CTRL|K_SUBTRACT : K_CTRL|K_ADD, 1);
		return;
	}
	if(keyflags & K_ALT) {
		slideshow->Ключ(zdelta < 0 ? K_ALT|K_SUBTRACT : K_ALT|K_ADD, 1);
		return;
	}
	RichTextView::колесоМыши(p, zdelta, keyflags);
}

Рисунок SlideShowView::рисКурсора(Точка p, dword keyflags)
{
	if(пусто_ли(cursor))
		return RichTextView::рисКурсора(p, keyflags);
	return cursor;
}

bool SlideShow::Ключ(dword ключ, int count)
{
	switch(ключ) {
	case K_ENTER:
		Settings();
		return true;
	case K_ESCAPE:
		Break();
		return true;
	case K_HOME:
		page = 0;
		break;
	case K_LEFT:
	case K_PAGEUP:
	case K_MOUSE_BACKWARD:
		page--;
		break;
	case K_RIGHT:
	case K_PAGEDOWN:
	case K_SPACE:
	case K_MOUSE_FORWARD:
		page++;
		break;
	case K_CTRL|K_ALT|K_ADD:
		titleh = min(titleh + 5, DPI(500));
		break;
	case K_CTRL|K_ALT|K_SUBTRACT:
		titleh = max(titleh - 5, 10);
		break;
	case K_ALT|K_ADD:
		margins = min(margins + 1, 100);
		break;
	case K_ALT|K_SUBTRACT:
		margins = max(margins - 1, 1);
		break;
	case K_CTRL|K_ADD:
		zoom = min(zoom + 1, 100);
		break;
	case K_CTRL|K_SUBTRACT:
		zoom = max(zoom - 1, 1);
		break;
	case K_CTRL_V:
		настройки.vcenter <<= !настройки.vcenter;
		break;
	default:
		return true;
	}
	устСтраницу();
	return true;
}

void SlideShow::Выкладка()
{
	устСтраницу();
}

void SlideShow::устСтраницу()
{
	page = minmax(page, 0, path.дайСчёт() - 1);
	Topic t;
	if(page >= 0)
		t = ReadTopic(загрузиФайл(path[page]));
	if(настройки.title) {
//		title <<= "{{1@C~ [*R+" + какТкст(titleh) + " \1" + t.title + "\1}}";
		title <<= "[*R+" + какТкст(titleh) + " \1" + t.title + "\1]";
		title.фон(светлоЦыан());
		title.HMargins(DPI(8) * margins);
		title.VMargins(0);
		int cy = min(title.GetCy() + DPI(10), дайРазм().cy / 2);
		title.TopPos(0, cy);
		text.VSizePos(cy + 10, 0);
		title.покажи();
	}
	else {
		text.VSizePos(0, 0);
		title.скрой();
	}

	text.SetZoom(Zoom(DPI(zoom), 100));
	text.VCenter(настройки.vcenter);
	text.HMargins(DPI(8) * margins);
	if(page != rp) {
		rp = page;
		text <<= t.text;
	}
	
	text.cursor = decode(~настройки.pointer, 1, IdeImg::ShowPointer1(), 2, IdeImg::ShowPointer2(),
	                     Рисунок());
}

void TopicCtrl::SShow()
{
	SlideShow ss;
	ss.устПрям(дайПрямЭкрана());
	ss.полноэкранно();
	TopicLink tl = ParseTopicLink(GetCurrent());
	if(пусто_ли(tl.package))
		return;
	
	Ткст folder = приставьИмяф(PackageDirectory(tl.package), tl.группа + ".tpp");
	ФайлПоиск ff(приставьИмяф(folder, "*.tpp"));
	Массив<Ткст> l;
	while(ff) {
		if(ff.файл_ли())
			l.добавь(дайТитулф(ff.дайИмя()));
		ff.следщ();
	}
	if(l.дайСчёт() == 0)
		return;
	сортируй(l);
	ss.page = 0;
	for(int i = 0; i < l.дайСчёт(); i++) {
		if(tl.topic == l[i])
			ss.page = i;
		ss.path.добавь(приставьИмяф(folder, l[i] + ".tpp"));
	}
	ss.устСтраницу();
	ss.пуск();
	tl.topic = l[ss.page];
	GoTo(TopicLinkString(tl));
}
