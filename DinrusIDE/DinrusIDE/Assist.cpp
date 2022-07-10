#include "DinrusIDE.h"

#if 0
#define LDUMP(x)     DDUMP(x)
#define LDUMPC(x)    DDUMPC(x)
#define LLOG(x)      DLOG(x)
#else
#define LDUMP(x)
#define LDUMPC(x)
#define LLOG(x)
#endif

#define LTIMING(x) // DTIMING(x)

class IndexSeparatorFrameCls : public КтрлФрейм {
	virtual void выложиФрейм(Прям& r)                   { r.right -= 1; }
	virtual void рисуйФрейм(Draw& w, const Прям& r) {
		w.DrawRect(r.right - 1, r.top, 1, r.устВысоту(), SColorShadow);
	}
	virtual void добавьРазмФрейма(Размер& sz) { sz.cx += 2; }
};

Значение AssistEditor::AssistItemConvert::фмт(const Значение& q) const
{
	int ii = q;
	if(ii >= 0 && ii < editor->assist_item_ndx.дайСчёт()) {
		ii = editor->assist_item_ndx[ii];
		if(ii < editor->assist_item.дайСчёт())
			return RawToValue(editor->assist_item[ii]);
	}
	CppItemInfo empty;
	return RawToValue(empty);
}

void AssistEditor::SyncNavigatorPlacement()
{
	int sz = navigatorframe.дайРазм();
	if(navigator_right)
		navigatorframe.право(navigatorpane, sz);
	else
		navigatorframe.лево(navigatorpane, sz);
}

AssistEditor::AssistEditor()
{
	assist_convert.editor = this;
	assist.NoHeader();
	assist.NoGrid();
	assist.AddRowNumColumn().Margin(0).SetConvert(assist_convert).устДисплей(Single<CppItemInfoDisplay>());
	assist.NoWantFocus();
	assist.ПриЛевКлике = THISBACK(AssistInsert);
	тип.NoHeader();
	тип.NoGrid();
	тип.добавьКолонку();
	тип.WhenCursor = THISBACK(SyncAssist);
	тип.NoWantFocus();
	popup.гориз(тип, assist);
	popup.устПоз(2000);
	auto_assist = auto_check = true;
	commentdp = false;

	SyncNavigatorPlacement();
	navigatorframe.лево(navigatorpane, HorzLayoutZoom(140));
	navigating = false;

	int cy = search.дайМинРазм().cy;
	navigatorpane.добавь(search.TopPos(0, cy).HSizePos(0, cy + 4));
	navigatorpane.добавь(sortitems.TopPos(0, cy).RightPos(0, cy));
	navigatorpane.добавь(navigator_splitter.VSizePos(cy, 0).HSizePos());
	navigator_splitter.верт() << scope << list << navlines;
	navigator_splitter.устПоз(1500, 0);
	navigator_splitter.устПоз(9500, 1);

	navigator = true;

	WhenAnnotationMove = THISBACK(SyncAnnotationPopup);
	WhenAnnotationClick = THISBACK1(EditAnnotation, true);
	WhenAnnotationRightClick = THISBACK1(EditAnnotation, false);
	Annotations(Zx(12));
	annotation_popup.фон(SColorPaper());
	annotation_popup.устФрейм(фреймЧёрный());
	annotation_popup.Margins(6);
	annotation_popup.NoSb();
	
	thisback = false;
	
	cachedpos = INT_MAX;
	cachedln = -1;
	
	parami = 0;

	param_info.Margins(2);
	param_info.фон(SColorPaper());
	param_info.устФрейм(фреймЧёрный());
	param_info.BackPaint();
	param_info.NoSb();
	
	include_assist = false;
	
	NoFindReplace();
}

int CppItemInfoOrder(const Значение& va, const Значение& vb) {
	const CppItemInfo& a = ValueTo<CppItemInfo>(va);
	const CppItemInfo& b = ValueTo<CppItemInfo>(vb);
	return CombineCompare(a.имя, b.имя)(a.natural, b.natural);
}

void AssistEditor::CloseAssist()
{
	if(popup.открыт())
		popup.закрой();
	if(annotation_popup.открыт())
		annotation_popup.закрой();
	assist_item.очисть();
	CloseTip();
}

bool isincludefnchar(int c)
{
	return c && c != '<' && c != '>' && c != '?' &&
	       c != ' ' && c != '\"' && c != '/' && c != '\\' && c >= 32 && c < 65536;
}

Ткст AssistEditor::ReadIdBackPos(int& pos, bool include)
{
	Ткст id;
	bool (*test)(int c) = include ? isincludefnchar : iscid;
	while(pos > 0 && (*test)(дайСим(pos - 1)))
		pos--;
	int q = pos;
	while(q < дайДлину64() && (*test)(дайСим(q)))
		id << (char)дайСим(q++);
	return id;
}

Ткст AssistEditor::ReadIdBack(int q, bool include, bool *destructor)
{
	Ткст id = ReadIdBackPos(q, include);
	if(destructor) {
		int n = 0;
		while(q > 0 && isspace(дайСим(q - 1)) && n < 100) {
			q--;
			n++;
		}
		*destructor = q > 0 && дайСим(q - 1) == '~';
	}
	return id;
}

void AssistEditor::DirtyFrom(int line)
{
	if(line >= cachedln) {
		cachedpos = INT_MAX;
		cachedline.очисть();
		cachedln = -1;
	}
	РедакторКода::DirtyFrom(line);
}

int AssistEditor::Ch(int i)
{
	if(i >= 0 && i < дайДлину64()) {
		if(i < cachedpos || i - cachedpos > cachedline.дайСчёт()) {
			cachedln = дайСтроку(i);
			cachedline = дайШСтроку(cachedln);
			cachedpos = дайПоз32(cachedln);
		}
		i -= cachedpos;
		return i < cachedline.дайСчёт() ? cachedline[i] : '\n';
	}
	return 0;
}

int AssistEditor::ParsBack(int q)
{
	int level = 1;
	--q;
	while(q > 0) {
		if(isrbrkt(Ch(q)))
			level++;
		if(islbrkt(Ch(q)))
			if(--level <= 0)
				break;
		--q;
	}
	return max(0, q);
}

bool IsSpc(int c)
{
	return c > 0 && c <= 32;
}

void AssistEditor::SkipSpcBack(int& q)
{
	while(q > 0 && IsSpc(Ch(q - 1)))
		q--;
}

Ткст AssistEditor::IdBack(int& qq)
{
	Ткст r;
	if(iscid(Ch(qq - 1))) {
		int q = qq;
		while(iscid(Ch(q - 1)))
			q--;
		if(iscib(Ch(q))) {
			qq = q;
			while(q < дайДлину64() && iscid(Ch(q)))
				r.конкат(Ch(q++));
		}
	}
	return r;
}

Ткст AssistEditor::CompleteIdBack(int& q, const Индекс<Ткст>& locals)
{
	Ткст id;
	for(;;) {
		SkipSpcBack(q);
		if(Ch(q - 1) == ',') {
			q--;
			id = ',' + id;
		}
		else
		if(Ch(q - 1) == '>') {
			q--;
			id = '>' + id;
		}
		else
		if(Ch(q - 1) == '<') {
			q--;
			id = '<' + id;
		}
		else
		if(Ch(q - 1) == ':' && Ch(q - 2) == ':') {
			q -= 2;
			id = "::" + id;
		}
		else {
			if(iscib(*id))
				break;
			Ткст nid = IdBack(q);
			if(пусто_ли(nid))
				break;
			if(locals.найди(nid) >= 0 && findarg(*id, '<', '>') >= 0)
				return id.середина(1);
			id = nid + id;
		}
	}
	return id;
}

Вектор<Ткст> AssistEditor::ReadBack(int q, const Индекс<Ткст>& locals)
{
	Вектор<Ткст> r;
	тип.очисть();
	bool wasid = true;
	for(;;) {
		if(r.дайСчёт() > 200) {
			r.очисть();
			тип.очисть();
			break;
		}
		SkipSpcBack(q);
		int c = Ch(q - 1);
		if(c == '>' && !wasid) {
			q--;
			r.добавь() = CompleteIdBack(q, locals) + ">";
			wasid = true;
			continue;
		}
		if(iscid(c)) {
			if(wasid)
				break;
			Ткст id;
			for(;;) {
				id = IdBack(q) + id;
				SkipSpcBack(q);
				if(!(Ch(q - 1) == ':' && Ch(q - 2) == ':'))
					break;
				q -= 2;
				id = "::" + id;
				SkipSpcBack(q);
			}
			r.добавь() = id;
			wasid = true;
			continue;
		}
		else {
//			if(findarg(c, '(', '[', '{') >= 0)
//				break;
			if(c == ']') {
				if(wasid)
					break;
				r.добавь("[]");
				q = ParsBack(q - 1);
				wasid = false;
				continue;
			}
			else
			if(c == ')') {
				if(wasid)
					break;
				r.добавь("()");
				q = ParsBack(q - 1);
				wasid = false;
				continue;
			}
			wasid = false;
			c = Ch(q - 1);
			if(c == '>' && Ch(q - 2) == '-') {
				r.добавь("->");
				q -= 2;
				continue;
			}
			if(c == '.') {
				r.добавь(".");
				q--;
				continue;
			}
		}
		break;
	}
	реверс(r);
	return r;
}

void AssistEditor::SyncAssist()
{
	LTIMING("SyncAssist");
	bool destructor;
	Ткст имя = ReadIdBack(дайКурсор32(), include_assist, &destructor);
	Ткст uname = взаг(имя);
	assist_item_ndx.очисть();
	int typei = тип.дайКурсор() - 1;
	Буфер<bool> found(assist_item.дайСчёт(), false);
	for(int pass = 0; pass < 2; pass++) {
		ВекторМап<Ткст, int> over;
		for(int i = 0; i < assist_item.дайСчёт(); i++) {
			const CppItemInfo& m = assist_item[i];
			if(!found[i] &&
			   (typei < 0 || m.typei == typei) &&
			   (pass ? m.uname.начинаетсяС(uname) : m.имя.начинаетсяС(имя)) &&
			   (!destructor || m.kind == DESTRUCTOR && m.scope == current_type + "::")) {
					int q = include_assist ? -1 : over.найди(m.qitem);
					if(q < 0 || over[q] == m.typei && m.scope.дайСчёт()) {
						found[i] = true;
						assist_item_ndx.добавь(i);
						if(q < 0)
							over.добавь(m.qitem, m.typei);
					}
			}
		}
	}
	assist.очисть();
	assist.SetVirtualCount(assist_item_ndx.дайСчёт());
}

bool AssistEditor::IncludeAssist()
{
	Вектор<Ткст> include;
	Ткст ln = дайУтф8Строку(GetCursorLine());
	СиПарсер p(ln);
	try {
		if(!p.сим('#') || !p.ид("include"))
			return false;
		if(p.сим('\"')) {
			include.добавь(дайПапкуФайла(theide->editfile));
			include_local = true;
		}
		else {
			p.сим('<');
			include = SplitDirs(theide->GetIncludePath());
			include_local = false;
		}
		include_path.очисть();
		include_back = 0;
		if(include_local)
			while(p.сим3('.', '.', '/') || p.сим3('.', '.', '\\')) {
				include.верх() = дайПапкуФайла(include.верх());
				include_back++;
			}
		for(;;) {
			Ткст dir;
			while(isincludefnchar(p.подбериСим()))
				dir.конкат(p.дайСим());
			if(dir.дайСчёт() && (p.сим('/') || p.сим('\\'))) {
				if(include_path.дайСчёт())
					include_path << '/';
				include_path << dir;
			}
			else
				break;
		}
	}
	catch(СиПарсер::Ошибка) {
		return false;
	}
	Вектор<Ткст> folder, upper_folder, file, upper_file;
	for(int i = 0; i < include.дайСчёт(); i++) {
		ФайлПоиск ff(приставьИмяф(приставьИмяф(include[i], include_path), "*.*"));
		while(ff) {
			Ткст фн = ff.дайИмя();
			if(!ff.скрыт_ли()) {
				if(ff.папка_ли()) {
					folder.добавь(фн);
					upper_folder.добавь(взаг(фн));
				}
				else {
					static Индекс<Ткст> ext(разбей(".h;.hpp;.hh;.hxx;.i;.lay;.iml;.t;.dli", ';'));
					Ткст fext = дайРасшф(фн);
					if(fext.дайСчёт() == 0 || ext.найди(впроп(fext)) >= 0) {
						file.добавь(фн);
						upper_file.добавь(взаг(фн));
					}
				}
			}
			ff.следщ();
		}
	}
	IndexSort(upper_folder, folder);
	Индекс<Ткст> fnset;
	for(int i = 0; i < folder.дайСчёт(); i++) {
		Ткст фн = folder[i];
		if(fnset.найди(фн) < 0) {
			fnset.добавь(фн);
			CppItemInfo& f = assist_item.добавь();
			f.имя = f.natural = фн;
			f.access = 0;
			f.kind = KIND_INCLUDEFOLDER;
		}
	}
	IndexSort(upper_file, file);
	for(int i = 0; i < file.дайСчёт(); i++) {
		Ткст фн = file[i];
		CppItemInfo& f = assist_item.добавь();
		f.имя = f.natural = фн;
		f.access = 0;
		static Индекс<Ткст> hdr(разбей(".h;.hpp;.hh;.hxx", ';'));
		Ткст fext = дайРасшф(фн);
		f.kind = hdr.найди(впроп(дайРасшф(фн))) >= 0 || fext.дайСчёт() == 0 ? KIND_INCLUDEFILE
		                                                                        : KIND_INCLUDEFILE_ANY;
	}
	include_assist = true;
	if(include_path.дайСчёт())
		include_path << "/";
	PopUpAssist();
	return true;
}

void AssistEditor::Assist()
{
	LTIMING("Assist");
	if(!assist_active)
		return;
	CloseAssist();
	int q = дайКурсор32();
	assist_cursor = q;
	assist_type.очисть();
	assist_item.очисть();
	include_assist = false;
	if(IncludeAssist())
		return;
	ParserContext parser;
	Контекст(parser, дайКурсор32());
	Индекс<Ткст> in_types;
	while(iscid(Ch(q - 1)) || Ch(q - 1) == '~')
		q--;
	SkipSpcBack(q);
	thisback = false;
	current_type.очисть();
	LTIMING("Assist2");
	if(Ch(q - 1) == '(') {
		int qq = q - 1;
		Ткст id = IdBack(qq);
		int tn = findarg(id, "THISBACK", "THISBACK1", "THISBACK2", "THISBACK3", "THISBACK4");
		if(tn >= 0) {
			thisback = true;
			thisbackn = tn > 0;
			GatherItems(parser.current_scope, false, in_types, false);
			RemoveDuplicates();
			PopUpAssist();
			return;
		}
	}
	if(Ch(q - 1) == ':' && Ch(q - 2) == ':') {
		q -= 2;
		Вектор<Ткст> tparam;
		Ткст scope = ParseTemplatedType(Qualify(parser.current_scope,
		                                          CompleteIdBack(q, parser.local.дайИндекс()),
		                                          parser.context.namespace_using),
		                                  tparam);
		GatherItems(scope, false, in_types, true);
		current_type = scope;
	}
	else {
		Ткст tp;
		Вектор<Ткст> xp = ReadBack(q, parser.local.дайИндекс());
		bool isok = false;
		if(xp.дайСчёт() && xp[0].начинаетсяС("::")) // ::Foo().
			xp[0] = xp[0].середина(2);
		for(int i = 0; i < xp.дайСчёт(); i++)
			if(iscib(*xp[i])) {
				isok = true;
				break;
			}
		if(xp.дайСчёт()) {
			if(isok) { // Do nothing on pressing '.' when there is no identifier before
				Индекс<Ткст> typeset = EvaluateExpressionType(parser, xp);
				for(int i = 0; i < typeset.дайСчёт(); i++)
					if(typeset[i].дайСчёт())
						GatherItems(typeset[i], xp[0] != "this", in_types, false);
			}
		}
		else {
			GatherItems(parser.current_scope, false, in_types, true);
			Вектор<Ткст> ns = parser.GetNamespaces();
			for(int i = 0; i < ns.дайСчёт(); i++)
				if(parser.current_scope != ns[i]) // Do not scan namespace already scanned
					GatherItems(ns[i], false, in_types, true);
		}
	}
	LTIMING("Assist3");
	RemoveDuplicates();
	PopUpAssist();
}

Ук<Ктрл> AssistEditor::assist_ptr;

bool AssistEditor::WheelHook(Ктрл *, bool inframe, int event, Точка p, int zdelta, dword keyflags)
{
	if(!inframe && event == MOUSEWHEEL && assist_ptr && assist_ptr->открыт()) {
		assist_ptr->колесоМыши(p, zdelta, keyflags);
		return true;
	}
	return false;
}

void AssistEditor::PopUpAssist(bool auto_insert)
{
	LTIMING("PopUpAssist");
	if(assist_item.дайСчёт() == 0)
		return;
	int lcy = max(16, BrowserFont().Info().дайВысоту());
	тип.очисть();
	тип.добавь(AttrText("<all>").Ink(SColorHighlight()));
	if(assist_type.дайСчёт() == 0)
		popup.Zoom(1);
	else {
		for(int i = 0; i < assist_type.дайСчёт(); i++) {
			Ткст s = assist_type[i];
			if(s[0] == ':' && s[1] == ':')
				s = s.середина(2);
			s = Nvl(s, "<globals>");
			if(s[0] == '<')
				тип.добавь(AttrText(s).Ink(SColorMark()));
			else
				тип.добавь(Nvl(s, "<globals>"));
		}
		popup.NoZoom();
	}
	тип.устКурсор(0);
	if(!assist.дайСчёт())
		return;
	LTIMING("PopUpAssist2");
	int cy = VertLayoutZoom(304);
	cy += HeaderCtrl::GetStdHeight();
	assist.SetLineCy(lcy);
	Точка p = GetCaretPoint() + GetScreenView().верхЛево();
	Прям wa = GetWorkArea();
	int cx = min(wa.устШирину() - 100, Zx(800));
	if(p.x + cx > wa.right)
		p.x = wa.right - cx;
	popup.устПрям(RectC(p.x, p.y + cy + GetFontSize().cy < wa.bottom ? p.y + GetFontSize().cy : p.y - cy, cx, cy) & wa);
	popup.BackPaint();
	if(auto_insert && assist.дайСчёт() == 1) {
		assist.идиВНач();
		AssistInsert();
	}
	else {
		popup.Ктрл::PopUp(this, false, false, true);
		assist_ptr = &assist;
		ONCELOCK {
			устХукМыши(AssistEditor::WheelHook);
		}
	}
}

bool sILess(const Ткст& a, const Ткст& b)
{
	return взаг(a) < взаг(b);
}

void AssistEditor::Complete()
{
	CloseAssist();

	int c = дайКурсор32();
	Ткст q = IdBack(c);

	Индекс<Ткст> ids;
	int64 len = 0;
	for(int i = 0; i < дайСчётСтрок() && len < 1000000; i++) {
		Ткст x = дайУтф8Строку(i);
		len += x.дайДлину();
		СиПарсер p(x);
		try {
			while(!p.кф_ли())
				if(p.ид_ли()) {
					Ткст h = p.читайИд();
					if(h != q)
						ids.найдиДобавь(h);
				}
				else
					p.пропустиТерм();
		}
		catch(СиПарсер::Ошибка) {
			return;
		}
	}
	
	Вектор<Ткст> id = ids.подбериКлючи();
	РНЦП::сортируй(id, sILess);

	if(q.дайСчёт()) {
		Ткст h;
		for(int i = 0; i < id.дайСчёт(); i++) {
			Ткст s = id[i];
			if(s.начинаетсяС(q)) {
				if(пусто_ли(h))
					h = s;
				else {
					s.обрежь(min(s.дайСчёт(), h.дайСчёт()));
					for(int j = 0; j < s.дайСчёт(); j++)
						if(s[j] != h[j]) {
							h.обрежь(j);
							break;
						}
				}
			}
		}
		if(h.дайСчёт() > q.дайСчёт())
			Paste(h.середина(q.дайСчёт()).вШТкст());
	}
	for(int i = 0; i < id.дайСчёт(); i++) {
		CppItemInfo& f = assist_item.добавь();
		f.имя = f.natural = f.qitem = id[i];
		f.access = 0;
		f.kind = 100;
	}
	assist_type.очисть();
	PopUpAssist(true);
}

void AssistEditor::Abbr()
{
	CloseAssist();
	int c = дайКурсор32();
	int ch;
	Ткст s;
	while(IsAlpha(ch = Ch(c - 1))) {
		s.вставь(0, ch);
		--c;
	}
	int len = s.дайСчёт();
	s = theide->abbr.дай(s, Ткст());
	if(пусто_ли(s))
		return;
	NextUndo();
	устКурсор(c);
	удали(c, len);
	int linepos = c;
	int line = дайПозСтроки32(linepos);
	ШТкст h = дайШСтроку(line).середина(0, linepos);
	for(int i = 0; i < s.дайСчёт(); i++) {
		ch = s[i];
		switch(ch) {
		case '@':
			c = дайКурсор32();
			break;
		case '\n':
			InsertChar('\n');
			for(int j = 0; j < h.дайСчёт(); j++)
				InsertChar(h[j]);
			break;
		default:
			if((byte)s[i] >= ' ' || s[i] == '\t')
				InsertChar(s[i]);
			break;
		}
	}
	устКурсор(c);
}

void AssistEditor::AssistInsert()
{
	if(assist.курсор_ли()) {
		int ii = assist.дайКурсор();
		if(ii < 0 || ii >= assist_item_ndx.дайСчёт())
			return;
		ii = assist_item_ndx[ii];
		if(ii >= assist_item.дайСчёт()) {
			CloseAssist();
			IgnoreMouseUp();
			return;
		}
		const CppItemInfo& f = assist_item[ii];
		if(include_assist) {
			int ln = дайСтроку(дайКурсор32());
			int pos = дайПоз32(ln);
			удали(pos, дайДлинуСтроки(ln));
			устКурсор(pos);
			Ткст h;
			for(int i = 0; i < include_back; i++)
				h << "../";
			Paste(вЮникод(Ткст().конкат() << "#include "
			                << (include_local ? "\"" : "<")
			                << h << include_path
			                << f.имя
			                << (f.kind == KIND_INCLUDEFOLDER ? "/" : include_local ? "\"" : ">")
			                , CHARSET_WIN1250));
			if(f.kind == KIND_INCLUDEFOLDER) {
				Assist();
				IgnoreMouseUp();
				return;
			}
			else {
				Ткст pkg = include_path.лево(include_path.дайСчёт() - 1);
				Вектор<Ткст> nests = GetUppDirs();
				for(int i = 0; i < nests.дайСчёт(); i++){
					if(файлЕсть(nests[i] + "/" + include_path + дайИмяф(pkg) + ".upp")) {
						Иср *ide = dynamic_cast<Иср *>(TheIde());
						if(ide)
							ide->AddPackage(pkg);
						break;
					}
				}
			}
		}
		else {
			Ткст txt = f.имя;
			int l = txt.дайСчёт();
			int pl = txt.дайСчёт();
			if(!thisback && f.kind >= FUNCTION && f.kind <= INLINEFRIEND)
				txt << "()";
			int cl = дайКурсор32();
			int ch = cl;
			while(iscid(Ch(cl - 1)))
				cl--;
			while(iscid(Ch(ch)))
				ch++;
			удали(cl, ch - cl);
			устКурсор(cl);
			if(thisback)
				for(;;) {
					int c = Ch(cl++);
					if(!c || Ch(cl) == ',' || Ch(cl) == ')')
						break;
					if(c != ' ') {
						if(thisbackn)
							txt << ", ";
						txt << ')';
						break;
					}
				}
			if(findarg(f.kind, CONSTRUCTOR, DESTRUCTOR) >= 0)
				txt << "()";
			int n = Paste(вЮникод(txt, CHARSET_WIN1250));
			if(!thisback && f.kind >= FUNCTION && f.kind <= INLINEFRIEND) {
				устКурсор(дайКурсор32() - 1);
				StartParamInfo(f, cl);
				if(f.natural.заканчиваетсяНа("()"))
					устКурсор(дайКурсор32() + 1);
			}
			else
			if(thisback) {
				if(thisbackn)
					устКурсор(дайКурсор32() - 1);
			}
			else
			if(!inbody)
				устКурсор(cl + n - thisbackn);
			else
			if(pl > l)
				устВыделение(cl + l, cl + pl);
			else
				устКурсор(cl + l);
		}
	}
	CloseAssist();
	IgnoreMouseUp();
}

bool AssistEditor::InCode()
{
	int pos = дайКурсор32();
	int line = дайПозСтроки32(pos);
	Один<EditorSyntax> st = дайСинтакс(line);
	ШТкст l = дайШСтроку(line);
	st->ScanSyntax(l, ~l + pos, line, GetTabSize());
	return st->CanAssist();
}

bool isaid(int c)
{
	return c == '~' || iscid(c);
}

bool AssistEditor::Ключ(dword ключ, int count)
{
	if(popup.открыт()) {
		int k = ключ & ~K_CTRL;
		КтрлМассив& kt = ключ & K_CTRL ? тип : assist;
		if(k == K_UP || k == K_PAGEUP || k == K_CTRL_PAGEUP || k == K_CTRL_END) {
			if(kt.курсор_ли())
				return kt.Ключ(k, count);
			else {
				kt.устКурсор(kt.дайСчёт() - 1);
				return true;
			}
		}
		if(k == K_DOWN || k == K_PAGEDOWN || k == K_CTRL_PAGEDOWN || k == K_CTRL_HOME) {
			if(kt.курсор_ли())
				return kt.Ключ(k, count);
			else {
				kt.устКурсор(0);
				return true;
			}
		}
		if(ключ == K_ENTER && assist.курсор_ли()) {
			AssistInsert();
			return true;
		}
		if(ключ == K_TAB && !assist.курсор_ли() && assist.дайСчёт()) {
			assist.идиВНач();
			AssistInsert();
			return true;
		}
	}
	int c = дайКурсор32();
	int cc = дайСим(c);
	int bcc = c > 0 ? дайСим(c - 1) : 0;
	bool b = РедакторКода::Ключ(ключ, count);
	if(b && search.естьФокус())
		устФокус();
	if(толькочтен_ли())
		return b;
	if(assist.открыт()) {
		bool (*test)(int c) = include_assist ? isincludefnchar : isaid;
		if(!(*test)(ключ) &&
		   !((*test)(cc) && (ключ == K_DELETE || ключ == K_RIGHT)) &&
		   !((*test)(bcc) && (ключ == K_LEFT || ключ == K_BACKSPACE))) {
			if(b) {
				CloseAssist();
				if(include_assist ? (ключ == '/' || ключ == '\\') : ключ == '.')
					Assist();
			}
		}
		else
			SyncAssist();
	}
	else
	if(auto_assist) {
		if(InCode()) {
			if(ключ == '.' || ключ == '>' && Ch(дайКурсор32() - 2) == '-' ||
			   ключ == ':' && Ch(дайКурсор32() - 2) == ':')
				Assist();
			else
			if(ключ == '(') {
				int q = дайКурсор32() - 1;
				Ткст id = IdBack(q);
				if(id == "THISBACK" || id == "THISBACK1" || id == "THISBACK2" || id == "THISBACK3" || id == "THISBACK4")
					Assist();
			}
		}
		if((ключ == '\"' || ключ == '<' || ключ == '/' || ключ == '\\') && дайУтф8Строку(GetCursorLine()).начинаетсяС("#include"))
			Assist();
	}
	return b;
}

void AssistEditor::колесоМыши(Точка p, int zdelta, dword keyflags)
{
	if(keyflags & K_CTRL)
		WhenFontScroll(зн(zdelta));
	else
		if(assist.открыт())
			assist.колесоМыши(p, zdelta, keyflags);
		else
			РедакторКода::колесоМыши(p, zdelta, keyflags);
}

void AssistEditor::леваяВнизу(Точка p, dword keyflags)
{
	CloseAssist();
	РедакторКода::леваяВнизу(p, keyflags);
}

void AssistEditor::расфокусирован()
{
	CloseAssist();
}

Ткст AssistEditor::RemoveDefPar(const char *s)
{
	Ткст r;
	int lvl = 0;
	bool dp = true;
	while(*s) {
		byte c = *s++;
		if(c == '(')
			lvl++;
		if(lvl == 0) {
			if(c == '=') {
				dp = false;
				if(commentdp)
					r.конкат("/* ");
				else
					while(r.дайСчёт() && *r.последний() == ' ')
						r.обрежь(r.дайСчёт() - 1);
			}
			if(c == ')') {
				if(!dp && commentdp)
					r.конкат("*/");
				r.конкат(')');
				try {
					if(СиПарсер(s).сим('='))
						break;
				}
				catch(СиПарсер::Ошибка) {}
				r.конкат(s);
				break;
			}
			if(c == ',') {
				if(!dp && commentdp)
					r.конкат("*/");
				dp = true;
			}
		}
		else
		if(c == ')')
			lvl--;
		if(dp || commentdp)
			r.конкат(c);
	}
	return r;
}

Ткст AssistEditor::MakeDefinition(const Ткст& cls, const Ткст& _n)
{
	Ткст n = обрежьЛево(_n);
	auto RemoveId = [&](const char *s) {
		int len = strlen(s);
		int q = n.найди(s);
		if(q >= 0 && (q == 0 || !iscid(n[q - 1])) && (q + len >= n.дайСчёт() || !iscid(n[q + len])))
			n.удали(q, len);
	};
	RemoveId("override");
	RemoveId("final");
	СиПарсер p(n);
	try {
		bool dest = false;
		const char *beg = n;
		while(!p.кф_ли()) {
			const char *b = p.дайУк();
			if(p.ид("operator"))
				return cls.дайСчёт() ? нормализуйПробелы(Ткст(beg, b) + ' ' + cls + "::" + b)
				                      : нормализуйПробелы(Ткст(beg, b) + ' ' + b);
			if(p.сим('~')) {
				beg = p.дайУк();
				dest = true;
			}
			else
			if(p.ид_ли()) {
				Ткст id = p.читайИд();
				if(p.сим('(')) {
					Ткст rp = RemoveDefPar(p.дайУк());
					auto merge = [](Ткст a, Ткст b) {
						return IsAlNum(*a.последний()) && IsAlNum(*b) ? a + ' ' + b : a + b;
					};
					if(cls.дайСчёт() == 0)
						return нормализуйПробелы(merge(Ткст(beg, b), id) + '(' + rp);
					if(dest)
						return нормализуйПробелы(Ткст(beg, b) + cls + "::~" + id + '(' + rp);
					else
						return нормализуйПробелы(merge(Ткст(beg, b), cls) + "::" + id + '(' + rp);
				}
			}
			else
				p.пропустиТерм();
		}
	}
	catch(СиПарсер::Ошибка) {}
	return n;
}

void Иср::IdeGotoCodeRef(Ткст coderef)
{
	LLOG("IdeGotoLink " << coderef);
	CodeBaseLock __;
	if(пусто_ли(coderef)) return;
	Ткст scope, элт;
	SplitCodeRef(coderef, scope, элт);
	int q = CodeBase().найди(scope);
	if(q < 0)
		return;
	const Массив<CppItem>& n = CodeBase()[q];
	q = FindItem(n, элт);
	if(q >= 0)
		JumpToDefinition(n, q, scope);
}

bool AssistEditor::Esc()
{
	bool r = false;
	if(assist.открыт()) {
		CloseAssist();
		r = true;
	}
	if(param_info.открыт()) {
		for(int i = 0; i < PARAMN; i++)
			param[i].line = -1;
		param_info.закрой();
		r = true;
	}
	return r;
}

void AssistEditor::SyncNavigatorShow()
{
	navigatorframe.покажи(navigator && theide && !theide->designer && !theide->IsEditorMode());
}

void AssistEditor::Navigator(bool nav)
{
	navigator = nav;
	SyncNavigatorShow();
	if(IsNavigator())
		устФокус();
	SyncNavigator();
	SyncCursor();
}

void AssistEditor::SyncNavigator()
{
	if(navigating)
		return;
	if(IsNavigator()) {
		ищи();
		SyncCursor();
	}
	SyncNavigatorShow();
}

void AssistEditor::SelectionChanged()
{
	РедакторКода::SelectionChanged();
	SyncCursor();
	SyncParamInfo();
	WhenSelectionChanged();
}

void AssistEditor::SerializeNavigator(Поток& s)
{
	int version = 5;
	s / version;
	s % navigatorframe;
	s % navigator;
	if(version == 1 || version == 3) {
		Сплиттер dummy;
		s % dummy;
	}
	if(version >= 4)
		s % navigator_splitter;
	if(version >= 5)
		s % navigator_right;
	Navigator(navigator);
	
	if(s.грузится())
		SyncNavigatorPlacement();
}
