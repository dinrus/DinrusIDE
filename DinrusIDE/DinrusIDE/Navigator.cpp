#include "DinrusIDE.h"

#define LTIMING(x) // RTIMING(x)

Ткст FormatNest(Ткст nest)
{
	int q = nest.найди("@");
	if(q >= 0) {
		nest.обрежь(q);
		nest << "[анонимное]";
	}
	return nest;
}

int CharFilterNavigator(int c)
{
	return c == ':' ? '.' : IsAlNum(c) || c == '_' || c == '.' ? c : 0;
}

void PaintTeXt(Draw& w, int& x, int y, const Ткст& text, Шрифт font, Цвет ink)
{
	static int maxascent = MaxAscent(StdFont());
	Размер sz = дайРазмТекста(text, font);
	w.DrawText(x, y + maxascent - font.GetAscent(), text, font, ink);
	x += sz.cx;
}

int DrawFileName0(Draw& w, const Прям& r, const Ткст& h, Цвет ink, int x)
{
	if(h.дайСчёт() == 0)
		return 0;
	int q = h.найди("\xff");
	Ткст ns;
	Ткст фн = h;
	if(q >= 0) {
		ns = h.середина(0, q) + ' ';
		фн = h.середина(q + 1);
	}
	Ткст s = дайИмяф(дайПапкуФайла(h)) + "/";
	x += r.left;
	if(ns.дайСчёт()) {
		PaintTeXt(w, x, r.top, ns, StdFont().Bold(), ink);
		PaintTeXt(w, x, r.top, "(", StdFont(), ink);
	}
	PaintTeXt(w, x, r.top, s, StdFont(), ink);
	s = дайИмяф(h);
	PaintTeXt(w, x, r.top, s, StdFont().Bold(), ink);
	if(ns.дайСчёт())
		PaintTeXt(w, x, r.top, ")", StdFont(), ink);
	return x - r.left;
}

Размер GetDrawFileNameSize(const Ткст& h)
{
	NilDraw w;
	return Размер(DrawFileName0(w, Размер(999999, 999999), h, Null, 0), StdFont().Bold().GetCy());
}

void DrawFileName(Draw& w, const Прям& r, const Ткст& h, Цвет ink)
{
	DrawFileName0(w, r, h, ink, min(r.дайШирину() - GetDrawFileNameSize(h).cx, 0));
}

int PaintFileName(Draw& w, const Прям& r, Ткст h, Цвет ink)
{
	if(*h == '\xff')
		h.удали(0, 1);
	return DrawFileName0(w, r, h, ink, 0);
}

Navigator::Navigator()
:	navidisplay(litem)
{
	scope_display.navigator = this;
	scope.NoHeader();
	scope.добавьКолонку().устДисплей(scope_display);
	scope.NoWantFocus();
	scope.WhenSel = THISBACK(Scope);
	scope.WhenLeftDouble = THISBACK(ScopeDblClk);

	list.NoHeader();
	list.AddRowNumColumn().устДисплей(navidisplay);
	list.SetLineCy(max(16, GetStdFontCy()));
	list.NoWantFocus();
	list.ПриЛевКлике = THISBACK(NavigatorClick);
	list.WhenSel = THISBACK(SyncNavLines);
	list.WhenLineEnabled = THISBACK(ListLineEnabled);
	
	navlines.NoHeader().NoWantFocus();
	navlines.ПриЛевКлике = THISBACK(GoToNavLine);
	navlines.добавьКолонку().устДисплей(Single<LineDisplay>());

	search <<= THISBACK(TriggerSearch);
	search.устФильтр(CharFilterNavigator);
	search.WhenEnter = THISBACK(NavigatorEnter);
	
	sortitems.Рисунок(BrowserImg::Sort());
	sortitems <<= THISBACK(NaviSort);
	sorting = false;
	
	dlgmode = false;
}

void Navigator::SyncCursor()
{
	Ткст k = "(" + GetKeyDesc(IdeKeys::AK_GOTO().ключ[0]) + ") ";
	search.NullText("Символ/номстр " + k);
	search.Подсказка(пусто_ли(search) ? Ткст() : "Стереть " + k);

	if(!navigating && theide->editfile.дайСчёт()) {
		navlines.анулируйКурсор();
		int q = linefo.найди(GetSourceFileIndex(theide->editfile));
		if(q < 0)
			return;
		navigating = true;
		SortedVectorMap<int, int>& m = linefo[q];
		q = m.найдиВерхнГран(GetCurrentLine() + 1) - 1;
		if(q >= 0 && q < m.дайСчёт())
			list.устКурсор(m[q]);
		navigating = false;
	}
	SyncLines();
	if(scope.курсор_ли())
		scope.RefreshRow(scope.дайКурсор());
}

void Navigator::SyncLines()
{
	if(пусто_ли(theide->editfile) || navigating)
		return;
	int ln = GetCurrentLine() + 1;
	int fi = GetSourceFileIndex(theide->editfile);
	int q = -1;
	for(int i = 0; i < navlines.дайСчёт(); i++) {
		const NavLine& l = navlines.дай(i, 0).To<NavLine>();
		if(l.file == fi && l.line <= ln && i < navlines.дайСчёт())
			q = i;
	}
	if(dlgmode)
		navlines.идиВНач();
	else
	if(q >= 0)
		navlines.устКурсор(q);
}

void Navigator::SyncNavLines()
{
	int sc = navlines.дайПромотку();
	navlines.очисть();
	int ii = list.дайКурсор();
	if(ii >= 0 && ii < litem.дайСчёт()) {
		Вектор<NavLine> l = GetNavLines(*litem[ii]);
		for(int i = 0; i < l.дайСчёт(); i++) {
			Ткст p = GetSourceFilePath(l[i].file);
			navlines.добавь(RawToValue(l[i]));
		}
		navlines.ScrollTo(sc);
		SyncLines();
	}
}

int Navigator::LineDisplay::DoPaint(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style, int x) const
{
	w.DrawRect(r, paper);
	const NavLine& l = q.To<NavLine>();
	x += r.left;
	Ткст p = GetSourceFilePath(l.file);
	int y = r.top + (r.дайВысоту() - StdFont().GetCy()) / 2;
	PaintTeXt(w, x, y, дайИмяф(дайПапкуФайла(p)) + "/", StdFont(), ink);
	PaintTeXt(w, x, y, дайИмяф(p), StdFont().Bold(), ink);
	PaintTeXt(w, x, y, " (", StdFont(), ink);
	PaintTeXt(w, x, y, какТкст(l.line), StdFont().Bold(), ink);
	PaintTeXt(w, x, y, ")", StdFont(), ink);
	return x - r.left;
}

void Navigator::LineDisplay::рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const
{
	DoPaint(w, r, q, ink, paper, style, min(r.дайШирину() - дайСтдРазм(q).cx, 0));
}

Размер Navigator::LineDisplay::дайСтдРазм(const Значение& q) const
{
	NilDraw w;
	return Размер(DoPaint(w, Размер(999999, 999999), q, белый(), белый(), 0, 0), StdFont().Bold().GetCy());
}

void Navigator::GoToNavLine()
{
	if(dlgmode)
		return;
	int ii = navlines.GetClickPos().y;
	if(ii >= 0 && ii < navlines.дайСчёт() && theide) {
		const NavLine& l = navlines.дай(ii, 0).To<NavLine>();
		theide->GotoPos(GetSourceFilePath(l.file), l.line);
	}
}

bool Navigator::NavLine::operator<(const NavLine& b) const
{
	Ткст p1 = GetSourceFilePath(file);
	Ткст p2 = GetSourceFilePath(b.file);
	return CombineCompare/*(!impl, !b.impl)*/
	                     (дайРасшф(p2), дайРасшф(p1)) // .h > .c
	                     (дайИмяф(p1), дайИмяф(p2))
	                     (p1, p2)
	                     (line, b.line) < 0;
}

Вектор<Navigator::NavLine> Navigator::GetNavLines(const NavItem& m)
{
	CodeBaseLock __;
	Вектор<NavLine> l;
	int q = CodeBase().найди(m.nest);
	if(q < 0 || пусто_ли(m.qitem))
		return l;
	const Массив<CppItem>& a = CodeBase()[q];
	for(int i = 0; i < a.дайСчёт(); i++) {
		const CppItem& mm = a[i];
		if(mm.qitem == m.qitem) {
			NavLine& nl = l.добавь();
			nl.impl = mm.impl;
			nl.file = mm.file;
			nl.line = mm.line;
		}
	}
	сортируй(l);
	return l;
}

void Navigator::Navigate()
{
	if(navigating)
		return;
	navigating = true;
	int ii = list.дайКурсор();
	if(theide && ii >= 0 && ii < litem.дайСчёт()) {
		int ln = GetCurrentLine() + 1;
		const NavItem& m = *litem[ii];
		if(m.kind == KIND_LINE || пусто_ли(search)) {
			theide->GotoPos(Null, m.line);
			if(m.kind == KIND_LINE) { // иди to line - restore file view
				search.очисть();
				ищи();
				navigating = false;
			}
			SyncCursor();
		}
		else
		if(m.kind == KIND_SRCFILE) {
			theide->AddHistory();
			theide->EditFile(m.ptype);
			theide->AddHistory();
		}
		else {
			Вектор<NavLine> l = GetNavLines(m);
			int q = l.дайСчёт() - 1;
			for(int i = 0; i < l.дайСчёт(); i++)
				if(GetSourceFilePath(l[i].file) == NormalizeSourcePath(theide->editfile) && l[i].line == ln) {
					q = (i + l.дайСчёт() + 1) % l.дайСчёт();
					break;
				}
			if(q >= 0 && q < l.дайСчёт()) {
				Ткст path = GetSourceFilePath(l[q].file);
				if(!theide->GotoDesignerFile(path, m.nest, m.имя, l[q].line))
					theide->GotoPos(path, l[q].line);
			}
		}
	}
	navigating = false;
}

void Navigator::ScopeDblClk()
{
	if(!scope.курсор_ли())
		return;
	Ткст h = scope.дайКлюч();
	if((byte)*h == 0xff)
		theide->GotoPos(h.середина(1), 1);
	else {
		list.идиВНач();
		Navigate();
	}
}

void Navigator::NavigatorClick()
{
	if(dlgmode)
		return;
	int q = list.GetClickPos().y;
	if(q >= 0 && q < list.дайСчёт())
		Navigate();
}

void Navigator::NavigatorEnter()
{
	if(list.дайСчёт()) {
		list.идиВНач();
		Navigate();
	}
}

void Иср::ToggleNavigator()
{
	editor.Navigator(!editor.navigator);
}

void Иср::SearchCode()
{
	if(!editor.navigator)
		editor.Navigator(true);
	if(!пусто_ли(~editor.search)) {
		editor.search.очисть();
		editor.ищи();
		editor.устФокус();
	}
	else {
		Ткст h = editor.GetWord();
		if(h.дайСчёт()) {
			editor.search <<= h;
			editor.search.выбериВсе();
			editor.ищи();
		}
		editor.search.устФокус();
	}
}

void Иср::SwitchHeader() {
	int c = filelist.дайКурсор();
	if(c < 0) return;
	Ткст currfile = filelist[c];
	const char *ext = дайПозРасшф(currfile);
	if(!stricmp(ext, ".h") || !stricmp(ext, ".hpp")
	|| !stricmp(ext, ".lay") || !stricmp(ext, ".iml")) {
		int f = filelist.найди(форсируйРасш(currfile, ".cpp"));
		if(f < 0) f = filelist.найди(форсируйРасш(currfile, ".c"));
		if(f < 0) f = filelist.найди(форсируйРасш(currfile, ".cc"));
		if(f >= 0) filelist.устКурсор(f);
	}
}

void Navigator::NavItem::уст(const CppItem& m)
{
	qitem = m.qitem;
	имя = m.имя;
	uname = m.uname;
	natural = m.natural;
	тип = m.тип;
	pname = m.pname;
	ptype = m.ptype;
	tname = m.tname;
	ctname = m.ctname;
	access = m.access;
	kind = m.kind;
	at = m.at;
	line = m.line;
	file = m.file;
	impl = m.impl;
	pass = false;
}

void Navigator::NavigatorDisplay::PaintBackground(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const
{
	int ii = q;
	if(ii < 0 || ii >= элт.дайСчёт())
		return;
	const NavItem& m = *элт[ii];
	bool focuscursor = (style & (FOCUS|CURSOR)) == (FOCUS|CURSOR) || (style & SELECT);
	if(findarg(m.kind, KIND_FILE, KIND_NEST) >= 0)
		w.DrawRect(r, focuscursor ? paper : m.kind == KIND_NEST ? смешай(SColorMark, SColorPaper, 220)
		                                    : SColorFace);
	else
		w.DrawRect(r, paper);
}


int Navigator::NavigatorDisplay::DoPaint(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const
{
	int ii = q;
	if(ii < 0 || ii >= элт.дайСчёт())
		return 0;
	const NavItem& m = *элт[ii];
	bool focuscursor = (style & (FOCUS|CURSOR)) == (FOCUS|CURSOR) || (style & SELECT);

	int x = r.left;
	int ry = r.top + r.дайВысоту() / 2;
	int y = ry - Draw::GetStdFontCy() / 2;

	if(findarg(m.kind, KIND_FILE, KIND_NEST) >= 0) {
		w.DrawRect(r, focuscursor ? paper : m.kind == KIND_NEST ? смешай(SColorMark, SColorPaper, 220)
		                                    : SColorFace);
		if(findarg(m.kind, KIND_FILE) >= 0)
			return PaintFileName(w, r, m.тип, ink);
		Ткст h = FormatNest(m.тип);
		w.DrawText(x, y, h, StdFont().Bold(), ink);
		return дайРазмТекста(h, StdFont().Bold()).cx;
	}
	
	w.DrawRect(r, paper);

	if(m.kind == KIND_SRCFILE)
		return PaintFileName(w, r, m.тип, ink);

	if(m.kind == KIND_LINE) {
		w.DrawText(x, y, m.тип, StdFont().Bold(), ink);
		return дайРазмТекста(m.тип, StdFont().Bold()).cx;
	}

	PaintCppItemImage(w, x, ry, m.access, m.kind, focuscursor);

	x += Zx(15);
	Вектор<ItemTextPart> n = ParseItemNatural(m.имя, m.natural, m.ptype, m.pname, m.тип,
	                                          m.tname, m.ctname, ~m.natural + m.at);
	int starti = 0;
	for(int i = 0; i < n.дайСчёт(); i++)
		if(n[i].тип == ITEM_NAME) {
			starti = i;
			break;
		}
	PaintText(w, x, y, m.natural, n, starti, n.дайСчёт(), focuscursor, ink, false);
	if(starti) {
		const char *h = " : ";
		w.DrawText(x, y, h, BrowserFont(), SColorText);
		x += дайРазмТекста(h, BrowserFont()).cx;
	}
	PaintText(w, x, y, m.natural, n, 0, starti, focuscursor, ink, false);
	return x;
}

void Navigator::NavigatorDisplay::рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const
{
	DoPaint(w, r, q, ink, paper, style);
}

Размер Navigator::NavigatorDisplay::дайСтдРазм(const Значение& q) const
{
	NilDraw w;
	return Размер(DoPaint(w, Размер(999999, 999999), q, белый(), белый(), 0), Draw::GetStdFontCy());
}

void Navigator::TriggerSearch()
{
	search_trigger.глушиУст(100, THISBACK(ищи));
}

void Navigator::NavGroup(bool local)
{
	CodeBaseLock __;
	for(int i = 0; i < nitem.дайСчёт(); i++) {
		NavItem& m = nitem[i];
		Ткст g = m.nest;
		if(m.kind == TYPEDEF)
			g.обрежь(max(g.найдирек("::"), 0));
		if(пусто_ли(g) || CodeBase().namespaces.найди(m.nest) >= 0) {
			if(g.дайСчёт()) // We want to show the namespace
				g << '\xff';
			else
				g.очисть();
			g << GetSourceFilePath(m.decl_file);
			g = '\xff' + g;
		}
		if(!local)
			g = (char)(m.pass + 10) + g;
		if(local)
			if(gitem.дайСчёт() && gitem.TopKey() == g)
				gitem.верх().добавь(&m);
			else
				gitem.добавь(g).добавь(&m);
		else
			gitem.дайДобавь(g).добавь(&m);
	}
}

force_inline
bool Navigator::SortByLines(const NavItem *a, const NavItem *b)
{
	return CombineCompare(a->decl_file, b->decl_file)(a->decl_line, b->decl_line) < 0;
}

force_inline
bool Navigator::SortByNames(const NavItem *a, const NavItem *b)
{
	return CombineCompare(a->имя, b->имя)(a->qitem, b->qitem) < 0;
}

void Navigator::ищи()
{
	CodeBaseLock __;
	sortitems.Check(sorting);
	int sc = scope.дайПромотку();
	Ткст ключ = scope.дайКлюч();
	Ткст s = обрежьОба(~search);
	Ткст search_name, search_nest;
	bool wholeclass = false;
	bool both = false;
	navigator_global = false;
	if(s.найди('.') >= 0) {
		Вектор<Ткст> h = разбей((Ткст)~search, '.');
		if(*s.последний() == '.')
			search_nest = Join(h, "::");
		else {
			search_name = h.вынь();
			if(h.дайСчёт())
				search_nest = Join(h, "::");
		}
		wholeclass = *s == '.' && search_nest.дайСчёт();
	}
	else {
		search_name = search_nest = ~search;
		both = true;
	}
	s = Join(разбей(s, '.'), "::") + (s.заканчиваетсяНа(".") ? "::" : "");
	int lineno = тктЦел(s);
	gitem.очисть();
	nitem.очисть();
	if(пусто_ли(theide->editfile))
		return;
	int fileii = GetSourceFileIndex(theide->editfile);
	if(!пусто_ли(lineno)) {
		NavItem& m = nitem.добавь();
		m.тип = "Перейти к строке " + какТкст(lineno);
		m.kind = KIND_LINE;
		m.line = lineno;
		gitem.добавь(Null).добавь(&m);
	}
	else
	if(пусто_ли(s) && !sorting) {
		const CppBase& b = CodeBase();
		bool sch = дайРасшф(theide->editfile) == ".sch";
		for(int i = 0; i < b.дайСчёт(); i++) {
			Ткст nest = b.дайКлюч(i);
			const Массив<CppItem>& ci = b[i];
			for(int j = 0; j < ci.дайСчёт(); j++) {
				const CppItem& m = ci[j];
				if(m.file == fileii && (!sch || !m.IsData() || m.тип != "SqlId")) {
					NavItem& n = nitem.добавь();
					n.уст(m);
					n.nest = nest;
					n.decl_line = m.line;
					n.decl_file = m.file;
					n.decl = !m.impl;
					NavLine& l = n.linefo.добавь();
					l.impl = m.impl;
					l.file = m.file;
					l.line = m.line;
				}
			}
		}
		сортируй(nitem, FieldRelation(&NavItem::line, StdLess<int>()));
		NavGroup(true);
	}
	else {
		navigator_global = true;
		const CppBase& b = CodeBase();
		Ткст usearch_nest = взаг(search_nest);
		Ткст usearch_name = взаг(search_name);
		МассивМап<Ткст, NavItem> imap;
		bool local = sorting && пусто_ли(s);
		ВекторМап<Ткст, int> nest_pass;
		for(int pass = -1; pass < 2; pass++) {
			for(int i = 0; i < b.дайСчёт(); i++) {
				Ткст nest = b.дайКлюч(i);
				bool foundnest = (wholeclass ? pass < 0 ? false :
				                               pass ? взаг(nest) == usearch_nest
				                                    : nest == search_nest
				                             : pass < 0 ? nest == search_nest :
				                               (pass ? взаг(nest).найди(usearch_nest) >= 0
				                                     : nest.начинаетсяС(search_nest)))
				                 && nest.найди('@') < 0;
				if(local || foundnest || both) {
					const Массив<CppItem>& ci = b[i];
					for(int j = 0; j < ci.дайСчёт(); j++) {
						const CppItem& m = ci[j];
						if(local ? m.file == fileii
						         : m.uname.найди('@') < 0 && (pass < 0 ? m.имя == search_name :
						                               pass ? m.uname.найди(usearch_name) >= 0
						                                    : m.имя.начинаетсяС(search_name))
						           || both && foundnest) {
							Ткст ключ = nest + '\1' + m.qitem;
							int q = nest_pass.найди(nest);
							int p = pass;
							if(q < 0) // We do not want classes to be split based on pass
								nest_pass.добавь(nest, pass);
							else
								p = nest_pass[q];
							q = imap.найди(ключ);
							if(q < 0) {
								NavItem& ni = imap.добавь(ключ);
								ni.уст(m);
								ni.nest = nest;
								ni.decl_line = ni.line;
								ni.decl_file = ni.file;
								ni.decl = !ni.impl;
								ni.pass = p;
								NavLine& l = ni.linefo.добавь();
								l.impl = m.impl;
								l.file = m.file;
								l.line = m.line;
							}
							else {
								NavItem& mm = imap[q];
								if(!m.impl &&
								  (!mm.decl
								    || CombineCompare(mm.decl_file, m.file)(mm.decl_line, m.line) < 0)) {
										mm.decl = true;
										mm.decl_line = m.line;
										mm.decl_file = m.file;
										mm.natural = m.natural;
								}
								NavLine& l = mm.linefo.добавь();
								l.impl = m.impl;
								l.file = m.file;
								l.line = m.line;
							}
						}
					}
				}
			}
		}
		nitem = imap.подбериЗначения();
		NavGroup(false);
		SortByKey(gitem);
		Вектор<Ткст> keys = gitem.подбериКлючи();
		Вектор<Вектор<NavItem *> > values = gitem.подбериЗначения();
		IndexSort(keys, values);
		for(int i = 0; i < keys.дайСчёт(); i++)
			keys[i].удали(0);
		ВекторМап<Ткст, Вектор<NavItem *> > h(pick(keys), pick(values));
		gitem = pick(h);
		for(int i = 0; i < gitem.дайСчёт(); i++)
			сортируй(gitem[i], sorting ? SortByNames : SortByLines);
		const РОбласть& wspc = GetIdeWorkspace();
		Ткст s = взаг(обрежьОба(~search));
		for(int i = 0; i < wspc.дайСчёт(); i++) {
			const Пакет& p = wspc.дайПакет(i);
			for(int j = 0; j < p.дайСчёт(); j++) {
				if(!p[j].separator && взаг(p[j]).найди(s) >= 0) {
					NavItem& m = nitem.добавь();
					m.kind = KIND_SRCFILE;
					m.тип = wspc[i] + "/" + p[j];
					m.ptype = SourcePath(wspc[i], p[j]);
					m.line = 0;
					gitem.дайДобавь("<files>").добавь(&m);
				}
			}
		}
	}
	scope.очисть();
	scope.добавь(Null);
	Индекс<Ткст> done;
	for(int i = 0; i < gitem.дайСчёт(); i++) {
		Ткст s = gitem.дайКлюч(i);
		if(done.найди(s) < 0) {
			done.добавь(s);
			scope.добавь(s);
		}
	}
	scope.ScrollTo(sc);
	if(!navigator_global || !scope.FindSetCursor(ключ))
		scope.идиВНач();
}

int Navigator::ScopeDisplay::DoPaint(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const
{
	w.DrawRect(r, paper);
	if(пусто_ли(q)) {
		const char *txt = "* ";
		int x = 0;
		w.DrawText(r.left, r.top, txt, StdFont().Bold().Italic(),
		           style & CURSOR ? ink : HighlightSetup::GetHlStyle(HighlightSetup::INK_KEYWORD).color);
		x += дайРазмТекста(txt, StdFont().Bold().Italic()).cx;
		int ii = navigator->list.дайКурсор();
		if(ii >= 0 && ii < navigator->litem.дайСчёт()) {
			const NavItem& m = *navigator->litem[ii];
			Ткст txt = m.nest;
			if(IsCppCode(m.kind))
				txt << "::" << m.имя;
			w.DrawText(r.left + x, r.top, txt, StdFont().Bold(), ink);
			x += дайРазмТекста(txt, StdFont().Bold()).cx;
		}
		return x;
	}
	Ткст h = q;
	if(*h == '\xff')
		return PaintFileName(w, r, h, ink);
	else
		h = FormatNest(h);
	w.DrawText(r.left, r.top, h, StdFont(), ink);
	return дайРазмТекста(h, StdFont()).cx;
}

void Navigator::ScopeDisplay::рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const
{
	DoPaint(w, r, q, ink, paper, style);
}

Размер Navigator::ScopeDisplay::дайСтдРазм(const Значение& q) const
{
	NilDraw w;
	return Размер(DoPaint(w, Размер(999999, 999999), q, белый(), белый(), 0), StdFont().Bold().GetCy());
}

void Navigator::Scope()
{
	LTIMING("FINALIZE");
	litem.очисть();
	nest_item.очисть();
	linefo.очисть();
	bool all = scope.дайКурсор() <= 0;
	Ткст sc = scope.дайКлюч();
	for(int i = 0; i < gitem.дайСчёт(); i++) {
		Ткст grp = gitem.дайКлюч(i);
		int    kind = KIND_NEST;
		if(*grp == '\xff')
			kind = KIND_FILE;
		if(all) {
			NavItem& m = nest_item.добавь();
			m.kind = kind;
			m.тип = FormatNest(grp);
			litem.добавь(&m);
		}
		else
		if(grp != sc)
			continue;
		const Вектор<NavItem *>& ia = gitem[i];
		for(int i = 0; i < ia.дайСчёт(); i++) {
			NavItem *m = ia[i];
			for(int j = 0; j < m->linefo.дайСчёт(); j++)
				linefo.дайДобавь(m->linefo[j].file).добавь(m->linefo[j].line, litem.дайСчёт());
			litem.добавь(m);
		}
	}
	int lsc = list.дайПромотку();
	list.очисть();
	list.SetVirtualCount(litem.дайСчёт());
	list.ScrollTo(lsc);
}

void Navigator::ListLineEnabled(int i, bool& b)
{
	if(i >= 0 && i < litem.дайСчёт()) {
		int kind = litem[i]->kind;
		if(findarg(kind, KIND_FILE, KIND_NEST) >= 0)
			b = false;
	}
}

void Navigator::NaviSort()
{
	sorting = !sorting;
	ищи();
}
