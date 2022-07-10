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

bool GetIdScope(Ткст& os, const Ткст& scope, const Ткст& id, Индекс<Ткст>& done)
{
	CodeBaseLock __;
	if(done.найди(scope) >= 0)
		return Null;
	done.добавь(scope);
	Вектор<Ткст> tparam;
	Ткст n = ParseTemplatedType(scope, tparam);
	Ткст nn = n + "::" + id;
	if(CodeBase().найди(nn) >= 0) { // Консоль -> СтрокРедакт::ПозРедакт
		os = nn;
		return true;
	}
	int q = CodeBase().найди(n);
	if(q < 0)
		return Null;
	const Массив<CppItem>& m = CodeBase()[q];
	Вектор<Ткст> r;
	if(FindName(m, id) >= 0) {
		os = n;
		return true;
	}
	for(int i = 0; i < m.дайСчёт(); i++) {
		const CppItem& im = m[i];
		if(im.IsType()) {
			Вектор<Ткст> b = разбей(im.qptype, ';');
			ResolveTParam(b, tparam);
			for(int i = 0; i < b.дайСчёт(); i++) {
				if(GetIdScope(os, b[i], id, done))
					return true;
			}
		}
	}
	return false;
}

bool GetIdScope(Ткст& os, const Ткст& scope, const Ткст& id)
{
	Индекс<Ткст> done;
	return GetIdScope(os, scope, id, done);
}

bool IsPif(const Ткст& l)
{
	return l.найди("#if") >= 0;
}

bool IsPelse(const Ткст& l)
{
	return l.найди("#el") >= 0;
}

bool IsPendif(const Ткст& l)
{
	return l.найди("#endif") >= 0;
}

void Иср::найдиИд(const Ткст& id)
{
	int pos = editor.дайКурсор();
	int h = min(editor.дайДлину(), pos + 4000);
	for(;;) {
		if(pos >= h || editor[pos] == ';')
			break;
		if(iscib(editor[pos])) {
			int p0 = pos;
			Ткст tid;
			while(pos < h && iscid(editor[pos])) {
				tid.конкат(editor[pos]);
				pos++;
			}
			if(tid == id) {
				editor.устКурсор(p0);
				return;
			}
		}
		else
			pos++;
	}
}

Ткст RemoveTemplateParams(const Ткст& s)
{
	Вектор<Ткст> dummy;
	return ParseTemplatedType(s, dummy);
}

bool Иср::OpenLink(const Ткст& s, int pos)
{ // try to find link at cursor, either http, https or file
	auto IsLinkChar = [](int c) { return findarg(c, '\'', '\"', '\t', ' ', '\0') < 0; };
	int b = pos;
	while(b > 0 && IsLinkChar(s[b - 1]))
		b--;
	int e = pos;
	while(IsLinkChar(s[e]))
		e++;
	Ткст link = s.середина(b, e - b);
	if(link.начинаетсяС("http://") || link.начинаетсяС("https://"))
		запустиВебБраузер(link);
	else
	if(файлЕсть(link))
		EditFile(link);
	else
		return false;
	return true;
}

void Иср::ContextGoto0(int pos)
{
	if(designer)
		return;
	int lp = pos;
	int li = editor.GetLinePos(lp);
	Ткст l = editor.дайУтф8Строку(li);
	if(OpenLink(l, lp))
		return;
	if(IsPif(l) || IsPelse(l)) {
		int lvl = 0;
		while(li + 1 < editor.дайСчётСтрок()) {
			l = editor.дайУтф8Строку(++li);
			if(IsPif(l))
				lvl++;
			if(IsPelse(l) && lvl == 0)
				break;
			if(IsPendif(l)) {
				if(lvl == 0) break;
				lvl--;
			}
		}
		AddHistory();
		editor.устКурсор(editor.дайПоз64(li));
		return;
	}
	if(IsPendif(l)) {
		int lvl = 0;
		while(li - 1 >= 0) {
			l = editor.дайУтф8Строку(--li);
			if(IsPif(l)) {
				if(lvl == 0) break;
				lvl--;
			}
			if(IsPendif(l))
				lvl++;
		}
		AddHistory();
		editor.устКурсор(editor.дайПоз64(li));
		return;
	}
	int cr = editor.Ch(pos);
	int cl = editor.Ch(pos - 1);
	if(!IsAlNum(cr)) {
		if(islbrkt(cr)) {
			AddHistory();
			editor.MoveNextBrk(false);
			return;
		}
		if(isrbrkt(cr)) {
			AddHistory();
			editor.MovePrevBrk(false);
			return;
		}
		if(islbrkt(cl)) {
			AddHistory();
			editor.MoveNextBrk(false);
			return;
		}
		if(isrbrkt(cl)) {
			AddHistory();
			editor.MovePrevBrk(false);
			return;
		}
	}
	try {
		СиПарсер p(l);
		if(p.сим('#') && p.ид("include")) {
			Ткст path = FindIncludeFile(p.дайУк(), дайПапкуФайла(editfile), SplitDirs(GetIncludePath()));
			if(!пусто_ли(path)) {
				AddHistory();
				EditFile(path);
				editor.устКурсор(0);
				AddHistory();
			}
			return;
		}
	}
	catch(СиПарсер::Ошибка) {}
	int q = pos;
	while(iscid(editor.Ch(q - 1)))
		q--;
	Ткст tp;
	Вектор<Ткст> xp = editor.ReadBack(q, Индекс<Ткст>()); // try to load expression like "x[i]." or "ptr->"
	Индекс<Ткст> тип;
	ParserContext parser;
	int ci = pos;
	for(;;) {
		int c = editor.Ch(ci);
		if(c == '{' && editor.Ch(ci + 1)) {
			ci++;
			break;
		}
		if(c == '}' || c == 0 || c == ';')
			break;
		ci++;
	}
	editor.Контекст(parser, ci);

	CodeBaseLock __;

	if(xp.дайСчёт()) {
		тип = editor.EvaluateExpressionType(parser, xp);
		if(тип.дайСчёт() == 0)
			return;
	}
	
	Ткст id = editor.GetWord(pos);
	if(id.дайСчёт() == 0)
		return;
	
	Ткст qual; // Try to load тип qualification like Foo::Бар, Вектор<Ткст>::Обходчик
	while(editor.Ch(q - 1) == ' ')
		q--;
	if(editor.Ch(q - 1) == ':' && editor.Ch(q - 2) == ':') {
		q -= 3;
		while(q >= 0) {
			int c = editor.Ch(q);
			if(iscid(c) || findarg(c, '<', '>', ':', ',', ' ') >= 0) {
				if(c != ' ')
					qual = (char)c + qual;
				q--;
			}
			else
				break;
		}
		if(qual.дайСчёт() == 0)
			qual = ":";
	}

	Вектор<Ткст> scope;
	Вектор<bool> istype; // prefer тип (e.g. struct Foo) over constructor (Foo::Foo())
	for(int i = 0; i < тип.дайСчёт(); i++) { // 'x.attr'
		Индекс<Ткст> done;
		Ткст r;
		if(GetIdScope(r, тип[i], id, done)) {
			Вектор<Ткст> todo;
			todo.добавь(r);
			while(scope.дайСчёт() < 100 && todo.дайСчёт()) { // добавь base classes
				Ткст t = todo[0];
				todo.удали(0);
				if(t.заканчиваетсяНа("::"))
					t.обрежь(t.дайСчёт() - 2);
				if(t.дайСчёт()) {
					scope.добавь(t);
					istype.добавь(false);
					ScopeInfo f(CodeBase(), t); // Try base classes too!
					todo.приставь(f.GetBases());
				}
			}
		}
	}
	
	Вектор<Ткст> ns = parser.GetNamespaces();

	if(qual.дайСчёт() > 2 && qual.начинаетсяС("::"))
		qual = qual.середина(2);
	if(qual.дайСчёт()) { // Ктрл::MOUSELEFT, Вектор<Ткст>::Обходчик
		Вектор<Ткст> todo;
		Ткст qa = Qualify(CodeBase(), parser.current_scope, *qual == ':' ? id : qual + "::" + id,
		                    parser.context.namespace_using);
		qa = RemoveTemplateParams(qa);
		if(CodeBase().найди(qa) < 0) { // РНЦП::FileTabs::RenameFile
			int q = qa.найдирек("::");
			if(q > 0) {
				Ткст h = qa.середина(0, q);
				if(CodeBase().найди(h) >= 0) {
					scope.добавь(h);
					istype.добавь(false);
				}
			}
			else {
				scope.добавь(Null); // добавь global namespace
				istype.добавь(false);
			}
		}
		todo.добавь(qa);
		while(scope.дайСчёт() < 100 && todo.дайСчёт()) {
			Ткст t = todo[0];
			if(t.заканчиваетсяНа("::"))
				t.обрежь(t.дайСчёт() - 2);
			todo.удали(0);
			if(CodeBase().найди(t) >= 0) { // Ктрл::MOUSELEFT
				scope.добавь(t);
				istype.добавь(true);
			}
			Ткст tt = t;
			tt << "::" << id;
			if(CodeBase().найди(tt) >= 0) { // Вектор<Ткст>::Обходчик
				scope.добавь(tt);
				istype.добавь(true);
			}
			ScopeInfo f(CodeBase(), t); // Try base classes too!
			todo.приставь(f.GetBases());
		}
	}
	else {
		Вектор<Ткст> todo;
		todo.добавь(parser.current_scope);
		while(scope.дайСчёт() < 100 && todo.дайСчёт()) { // добавь base classes
			Ткст t = todo[0];
			todo.удали(0);
			t.обрежьКонец("::");
			if(t.дайСчёт()) {
				scope.добавь(t);
				istype.добавь(false);
				ScopeInfo f(CodeBase(), t); // Try base classes too!
				todo.приставь(f.GetBases());
			}
		}
		if(xp.дайСчёт() == 0) {
			q = parser.local.найди(id);
			if(q >= 0) { // Try locals
				AddHistory();
				editor.устКурсор(editor.дайПоз64(parser.local[q].line - 1));
				найдиИд(id);
				return;
			}
		}
		// Can be unqualified тип имя like 'Ткст'
		Ткст t = RemoveTemplateParams(Qualify(CodeBase(), parser.current_scope, id, parser.context.namespace_using));
		for(int i = 0; i < ns.дайСчёт(); i++) {
			Ткст tt = Merge("::", ns[i], t);
			if(CodeBase().найди(tt) >= 0) {
				scope.добавь(tt);
				istype.добавь(true);
			}
		}
	}
	
	Вектор<Ткст> usings = разбей(parser.context.namespace_using, ';');
	usings.добавь(""); // добавь global namespace too
	
	Индекс<Ткст> done;
	for(int i = 0; i < ns.дайСчёт(); i++) {
		Ткст r;
		if(GetIdScope(r, ns[i], id, done)) {
			scope.добавь(r);
			istype.добавь(false);
		}
	}

	for(int j = 0; j < scope.дайСчёт(); j++) {
		q = CodeBase().найди(scope[j]);
		if(q >= 0) {
			int ii = -1;
			const Массив<CppItem>& n = CodeBase()[q];
			for(int i = 0; i < n.дайСчёт(); i++) {
				const CppItem& m = n[i];
				if(m.имя == id) {
					if(ii < 0)
						ii = i;
					else {
						const CppItem& mm = n[ii];
						if(CombineCompare(findarg(mm.kind, CONSTRUCTOR, DESTRUCTOR) < 0,
						                  findarg(m.kind, CONSTRUCTOR, DESTRUCTOR) < 0)
						                 (!istype[j] || mm.IsType(), !istype[j] || m.IsType())
							             (mm.impl, m.impl)
							             (findarg(mm.filetype, FILE_CPP, FILE_C) >= 0,
							              findarg(m.filetype, FILE_CPP, FILE_C) >= 0)
							             (mm.line, m.line) < 0)
							ii = i;
					}
				}
			}
			if(ii >= 0) {
				JumpToDefinition(n, ii, scope[j]);
				найдиИд(id);
				return;
			}
		}
	}

	if(id.начинаетсяС("AK_")) {
		Ткст ak_id = id.середина(3);
		const РОбласть& wspc = GetIdeWorkspace();
		for(int i = 0; i < wspc.дайСчёт(); i++) {
			Ткст pn = wspc[i];
			const Пакет& p = wspc.дайПакет(i);
			Ткст pp = PackageDirectory(pn);
			for(int j = 0; j < p.дайСчёт(); j++)
				if(!p[j].separator) {
					Ткст фн = приставьИмяф(pp, p[j]);
					if(дайРасшф(фн) == ".ключ") {
						ФайлВвод in(фн);
						int line = 0;
						while(!in.кф_ли()) {
							Ткст s = in.дайСтроку();
							try {
								СиПарсер p(s);
								if(p.ид("KEY") && p.сим('(') && p.ид(ak_id)) {
									UnlockCodeBaseAll(); // so that scan in сохраниФайл does not fail with deadlock
									GotoPos(фн, line + 1);
									return;
								}
							}
							catch(СиПарсер::Ошибка) {}
							line++;
						}
					}
				}
		}
	}
}

void Иср::ContextGoto()
{
	ContextGoto0(editor.дайКурсор());
}

void Иср::CtrlClick(int64 pos)
{
	if(pos < INT_MAX)
		ContextGoto0((int)pos);
}

bool Иср::GotoDesignerFile(const Ткст& path, const Ткст& scope, const Ткст& имя, int line)
{
	if(впроп(дайРасшф(path)) == ".lay") {
		AddHistory();
		EditFile(path);
		LayDesigner *l = dynamic_cast<LayDesigner *>(~designer);
		if(l) {
			if(scope.начинаетсяС("With"))
				l->FindLayout(scope.середина(4), имя);
			else
			if(имя.начинаетсяС("SetLayout_"))
				l->FindLayout(имя.середина(10), Null);
		}
		else {
			editor.устКурсор(editor.дайПоз64(line - 1));
			editor.TopCursor(4);
			editor.устФокус();
		}
		AddHistory();
		return true;
	}
	else
	if(впроп(дайРасшф(path)) == ".iml") {
		AddHistory();
		EditFile(path);
		IdeIconDes *l = dynamic_cast<IdeIconDes *>(~designer);
		if(l)
			l->найдиИд(имя);
		else
			editor.устФокус();
		AddHistory();
		return true;
	}
	return false;
}

void Иср::JumpToDefinition(const Массив<CppItem>& n, int q, const Ткст& scope)
{
	Ткст qitem = n[q].qitem;
	int i = q;
	int qml = 0;
	int qcpp = -1;
	int qcppml = 0;
	int qimpl = -1;
	int qimplml = 0;
	Ткст currentfile = editfile;
	while(i < n.дайСчёт() && n[i].qitem == qitem) {
		const CppItem& m = n[i];
		int ml = GetMatchLen(editfile, GetSourceFilePath(m.file));
		if(m.impl && ml > qimplml) {
			qimplml = ml;
			qimpl = i;
		}
		if((m.filetype == FILE_CPP || m.filetype == FILE_C) && ml > qcppml) {
			qcpp = i;
			qcppml = ml;
		}
		if(ml > qml) {
			q = i;
			qml = ml;
		}
		i++;
	}
	CppItem pos = n[qimpl >= 0 ? qimpl : qcpp >= 0 ? qcpp : q];
	Ткст path = GetSourceFilePath(pos.file);
	editastext.удалиКлюч(path);
	editashex.удалиКлюч(path);
	UnlockCodeBaseAll();
	if(!GotoDesignerFile(path, scope, pos.имя, pos.line))
		GotoCpp(pos);
}

void Иср::GotoFileAndId(const Ткст& path, const Ткст& id)
{
	AddHistory();
	EditFile(path);
	ШТкст wid = id.вШТкст();
	if(editor.дайДлину64() < 100000) {
		for(int i = 0; i < editor.дайСчётСтрок(); i++) {
			ШТкст ln = editor.дайШСтроку(i);
			int q = ln.найди(wid);
			while(q >= 0) {
				if(q == 0 || !iscid(ln[q - 1]) && !iscid(ln[q + wid.дайСчёт()])) {
					editor.устКурсор(editor.дайПоз64(i, q));
					editor.курсорПоЦентру();
					return;
				}
				if(q + 1 >= ln.дайСчёт())
					break;
				q = ln.найди(wid, q + 1);
			}
		}
	}
	AddHistory();
}
