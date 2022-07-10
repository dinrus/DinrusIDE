#include "CodeEditor.h"

namespace РНЦП {

void РедакторКода::иницНайтиЗаменить()
{
	findreplace.find.AddButton().SetMonoImage(CtrlImg::smallright()).Подсказка("Wildcard")
		<<= THISBACK(FindWildcard);
	findreplace.replace.AddButton().SetMonoImage(CtrlImg::smallright()).Подсказка("Wildcard")
		<<= THISBACK(ReplaceWildcard);
	PutI(findreplace.find);
	PutI(findreplace.replace);
	findreplace.amend <<= THISBACK(замени);
	findreplace.amend_all <<= THISBACK1(ReplaceAll, false);
	findreplace.amend_rest <<= THISBACK1(ReplaceAll, true);
	findreplace.prev <<= THISBACK(DoFindBack);
	findreplace.replacing = false;
	found = notfoundfw = notfoundbk = foundsel = false;
	persistent_find_replace = false;
	findreplace.find <<= findreplace.wholeword <<= findreplace.wildcards
	                 <<= findreplace.incremental <<= findreplace.regexp
	                 <<= findreplace.ignorecase <<= THISBACK(IncrementalFind);
	ff_start_pos = -1;
	findreplace.find_all << [=] { найдиВсе(); };
}

ДиалогНайтиЗаменить::ДиалогНайтиЗаменить()
{
	find.NoUpDownKeys();
	samecase <<= true;
	close.Cancel();
	prev.устРисунок(CtrlImg::SmallUp());
	amend.устРисунок(CodeEditorImg::Replace());
	amend_all.устРисунок(CodeEditorImg::ReplaceAll());
	amend_rest.устРисунок(CodeEditorImg::ReplaceRest());
	find_all.устРисунок(CodeEditorImg::FindAll());
	incremental <<= true;
	mode <<= THISBACK(синх);
	mode.скрой();
	синх();
	
	find.NullText("Найти");
	replace.NullText("Заменить");
}

void ДиалогНайтиЗаменить::синх()
{
	samecase.вкл(ignorecase);
	bool b = !regexp;
	wildcards.вкл(b);
	prev.вкл(b);
	ignorecase.вкл(b);
	wholeword.вкл(b);
	incremental_from_cursor.вкл(инкрементален_ли());
	b = !mode.видим_ли() || ~mode == 0;
	replace.вкл(b);
}

dword РедакторКода::find_next_key = K_F3;
dword РедакторКода::find_prev_key = K_SHIFT_F3;
dword РедакторКода::replace_key = K_CTRL_R;

bool ДиалогНайтиЗаменить::Ключ(dword ключ, int cnt) {
	if(ключ == K_CTRL_I) {
		if(find.естьФокус()) {
			find <<= itext;
			return true;
		}
		if(replace.естьФокус()) {
			replace <<= itext;
			return true;
		}
	}
	if(ключ == K_ENTER) {
		next.WhenAction();
		return true;
	}
	if(findarg(ключ, K_TAB, K_SHIFT_TAB) >= 0 && replace.показан_ли()) {
		if(find.естьФокус())
			replace.устФокус();
		else
			find.устФокус();
		return true;
	}
	if(ключ == K_ESCAPE) {
		close.WhenAction();
		return true;
	}
	return ТопОкно::Ключ(ключ, cnt);
}

void ДиалогНайтиЗаменить::настрой(bool doreplace)
{
	CtrlLayout(*this);
	close.устРисунок(CodeEditorImg::Cancel());
	close.Подсказка("Закрыть (Esc)");
	close.Normal();
	close.устНадпись("");
	next.устРисунок(CtrlImg::SmallDown());
	next.Подсказка("Найти далее (" + GetKeyDesc(РедакторКода::find_next_key) + ")");
	next.Normal();
	next.устНадпись("");
	prev.Подсказка("Найти ранее (" + GetKeyDesc(РедакторКода::find_prev_key) + ")");
	amend.Подсказка("Заменить (" + GetKeyDesc(РедакторКода::replace_key) + ")");
	find_all.покажи();
	find_all.Подсказка("Найти все");
	amend.откл();
	replacing = doreplace;
	replace.покажи(replacing);
	amend.покажи(replacing);
	amend_all.покажи(replacing);
	amend_rest.покажи(replacing);
	устВысоту(doreplace ? GetLayoutSize().cy : replace.дайПрям().top);
	устФрейм(фреймВерхнСепаратора());
	синх();
}

void РедакторКода::устНайден(int fi, int тип, const ШТкст& text)
{
	Found& f = foundwild.по(fi);
	f.тип = тип;
	f.text = text;
}

int РедакторКода::сверь(const wchar *f, const wchar *s, int line, bool we, bool ignorecase, int fi)
{
	const wchar *b = s;
	int n = 0;
	ШТкст ln;
	while(*f) {
		if(*f == WILDANY) {
			f++;
			ШТкст wild;
			for(;;) {
				int nn = сверь(f, s, line, we, ignorecase, fi + 1);
				if(nn >= 0) {
					устНайден(fi, WILDANY, wild);
					return int(s - b) + n + nn;
				}
				wild.конкат(*s ? *s : '\n');
				if(!*s++) return -1;
			}
			return -1;
		}
		else
		if(*f == WILDONE) {
			if(!*s) return -1;
			устНайден(fi++, WILDONE, ШТкст(*s, 1));
			s++;
		}
		else
		if(*f == WILDSPACE) {
			const wchar *wb = s;
			if(*s != ' ' && *s != '\t') return -1;
			s++;
			while(*s == ' ' || *s == '\t')
				s++;
			устНайден(fi++, WILDSPACE, ШТкст(wb, s));
		}
		else
		if(*f == WILDNUMBER) {
			const wchar *wb = s;
			if(*s < '0' || *s > '9') return -1;
			s++;
			while(*s >= '0' && *s <= '9')
				s++;
			устНайден(fi++, WILDNUMBER, ШТкст(wb, s));
		}
		else
		if(*f == WILDID) {
			const wchar *wb = s;
			if(!iscib(*s)) return -1;
			s++;
			while(iscidl(*s)) s++;
			устНайден(fi++, WILDID, ШТкст(wb, s));
		}
		else
		if(*f == '\n') {
			if(*s != '\0' || ++line >= дайСчётСтрок()) return -1;
			n += int(s - b) + 1;
			ln = дайШСтроку(line);
			s = b = ln;
		}
		else {
			if(ignorecase ? взаг(*s) != взаг(*f) : *s != *f) return -1;
			s++;
		}
		f++;
	}
	return we && iscidl(*s) ? -1 : int(s - b) + n;
}

bool РедакторКода::найди(bool back, bool block)
{
	foundsel = true;
	if(notfoundfw) двигайВНачТекста();
	if(notfoundbk) двигайВКонТекста();
	foundsel = false;
	int64 cursor, pos;
	if(found)
		дайВыделение(pos, cursor);
	else
		дайВыделение(cursor, pos);
	pos = cursor;
	bool b = найдиОт(pos, back, block);
	findreplace.amend.вкл(b);
	return b;
}

bool РедакторКода::найдиРегВыр(int64 pos, bool block)
{
	RegExp regex((Ткст)~findreplace.find);
	
	int line = дайПозСтроки64(pos);
	Ткст ln = вУтф8(дайШСтроку(line).середина(ограничьРазм(pos)));
	for(;;) {
		if(regex.сверь(ln)) {
			for(int i = 0; i < regex.дайСчёт(); i++)
				устНайден(i, WILDANY, regex.дайТкст(i).вШТкст());
			int off = regex.дайСмещ();
			int len = длинаУтф32(~ln + off, regex.дайДлину());
			pos = дайПоз64(line, длинаУтф32(~ln, off) + (int)pos);
			foundtext = дайШ(pos, len);
			if(!block) {
				foundsel = true;
				устВыделение(pos, pos + len);
				foundsel = false;
				курсорПоЦентру();
			}
			foundpos = pos;
			foundsize = len;
			found = true;
			return true;
		}
		if(++line >= дайСчётСтрок()) {
			ждиВид(line);
			if(line >= дайСчётСтрок())
				return false;
		}
		ln = дайУтф8Строку(line);
		pos = 0;
		if(!SearchProgress(line))
			return false;
	}
}

void РедакторКода::найдиВсе()
{
	Вектор<Кортеж<int64, int>> found;
	foundpos = 0;
	while(найдиОт(foundpos, false, true)) {
		found.добавь(сделайКортеж(foundpos, foundsize));
		foundpos += foundsize;
		if(found.дайСчёт() >= 10000) {
			Exclamation("Слишком много совпадений, будут показаны только первые 10000.");
			break;
		}
	}
	WhenFindAll(found);
}

bool РедакторКода::найдиОт(int64 pos, bool back, bool block)
{
	notfoundbk = notfoundfw = false;
	if(findreplace.regexp) {
		if(найдиРегВыр(pos, block))
			return true;
		if(back)
			notfoundbk = true;
		else
			notfoundfw = true;
		return false;
	}
	ШТкст text = ~findreplace.find;
	ШТкст ft;
	const wchar *s = text;
	while(*s) {
		int c = *s++;
		if(c == '\\') {
			c = *s++;
			if(c == '\0') break;
			if(c == 'n')
				ft.конкат('\n');
			else
			if(c == 't')
				ft.конкат('\t');
			else
			if(c >= ' ')
				ft.конкат(c);
		}
		else
		if(c >= ' ') {
			if(findreplace.wildcards)
				ft.конкат(c == '*' ? WILDANY :
					   c == '?' ? WILDONE :
					   c == '%' ? WILDSPACE :
					   c == '#' ? WILDNUMBER :
					   c == '$' ? WILDID :
								  c
				);
			else
				ft.конкат(c);
		}
	}
	bool wb = findreplace.wholeword ? iscidl(*ft) : false;
	bool we = findreplace.wholeword ? iscidl(*ft.последний()) : false;
	if(ft.пустой()) return false;
	foundwild.очисть();
	int line = дайПозСтроки64(pos);
	ШТкст ln = дайШСтроку(line);
	const wchar *l = ln;
	s = l + pos;
	bool ignorecase = findreplace.ignorecase;
	for(;;) {
		for(;;) {
			if(!wb || (s == l || !iscidl(s[-1]))) {
				int n = сверь(ft, s, line, we, ignorecase);
				if(n >= 0) {
					int64 pos = дайПоз64(line, int(s - l));
					foundtext = дайШ(pos, n);
					if(!back || pos + n < cursor) {
						if(!block) {
							foundsel = true;
							устВыделение(pos, pos + n);
							foundsel = false;
							курсорПоЦентру();
						}
						foundpos = pos;
						foundsize = n;
						found = true;
						return true;
					}
				}
			}
			if(back) {
				if(s-- == l) break;
			}
			else
				if(!*s++) break;
		}
		if(back) {
			if(--line < 0) break;
			ln = дайШСтроку(line);
			l = ln;
			s = ln.стоп();
		}
		else {
			if(++line >= дайСчётСтрок()) {
				ждиВид(line);
				if(line >= дайСчётСтрок())
					break;
			}
			ln = дайШСтроку(line);
			l = s = ln;
		}
		if(!SearchProgress(back ? дайСчётСтрок() - line : line))
			break;
	}
	if(back)
		notfoundbk = true;
	else
		notfoundfw = true;
	return false;
}

void РедакторКода::FindReplaceAddHistory()
{
	if(!пусто_ли(findreplace.find))
		findreplace.find.AddHistory();
	if(!пусто_ли(findreplace.replace))
		findreplace.replace.AddHistory();
}

void РедакторКода::NoFindError()
{
	findreplace.find.Ошибка(false);
}

void РедакторКода::NotFound()
{
	findreplace.find.Ошибка();
	if(!findreplace.инкрементален_ли())
		устФокус();
	findreplace.amend.откл();
}

bool РедакторКода::найди(bool back, bool blockreplace, bool replace)
{
	NoFindError();
	if(найди(back, blockreplace)) {
		if(!blockreplace) {
			if(!findreplace.открыт())
				OpenNormalFindReplace(replace);
			if(!findreplace.инкрементален_ли())
				устФокус();
		}
		return true;
	}
	else {
		NotFound();
		return false;
	}
}

ШТкст РедакторКода::GetWild(int тип, int& i)
{
	for(;;) {
		if(i >= foundwild.дайСчёт()) break;
		Found& f = foundwild[i++];
		if(f.тип == тип) return f.text;
	}
	for(int j = 0; j < foundwild.дайСчёт(); j++) {
		Found& f = foundwild[j++];
		if(f.тип == тип) return f.text;
	}
	return ШТкст();
}

ШТкст РедакторКода::GetReplaceText()
{
	ШТкст rs = ~findreplace.replace;
	bool wildcards = findreplace.wildcards;
	bool samecase = findreplace.ignorecase && findreplace.samecase;

	int anyi = 0, onei = 0, spacei = 0, numberi = 0, idi = 0;
	ШТкст rt;
	const wchar *s = rs;
	while(*s) {
		int c = *s++;
		if(c == '\\') {
			c = *s++;
			if(c == '\0') break;
			if(c == 'n')
				rt.конкат('\n');
			else
			if(c == 't')
				rt.конкат('\t');
			else
			if(c >= ' ')
				rt.конкат(c);
		}
		else
		if(c >= ' ') {
			if(wildcards) {
				ШТкст w;
				if(c == '*')
					w = GetWild(WILDANY, anyi);
				else
				if(c == '?')
					w = GetWild(WILDONE, onei);
				else
				if(c == '%')
					w = GetWild(WILDSPACE, spacei);
				else
				if(c == '#')
					w = GetWild(WILDNUMBER, numberi);
				else
				if(c == '$')
					w = GetWild(WILDID, idi);
				else
				if(c == '@') {
					c = *s++;
					if(c == '\0') break;
					if(c == '@') {
						rt << какТкст(replacei).вШТкст();
						continue;
					}
					if(c == '#') {
						rt << какТкст(replacei + 1).вШТкст();
						continue;
					}
					if(c >= '1' && c <= '9') {
						c -= '1';
						w = c < foundwild.дайСчёт() ? foundwild[c].text : ШТкст();
					}
					else {
						rt.конкат('@');
						if(c >= ' ' && c < 255) rt.конкат(c);
						continue;
					}
				}
				else {
					rt.конкат(c);
					continue;
				}
				if(*s == '+') {
					w = взаг(w);
					s++;
				}
				else
				if(*s == '-') {
					w = впроп(w);
					s++;
				}
				else
				if(*s == '!') {
					w = иницШапки(w);
					s++;
				}
				rt.конкат(w);
			}
			else
				rt.конкат(c);
		}
	}
	if(samecase) {
		if(foundtext.дайСчёт() && rt.дайСчёт()) {
			if(проп_ли(foundtext[0]))
				rt.уст(0, взаг(rt[0]));
			if(заг_ли(foundtext[0]))
				rt.уст(0, впроп(rt[0]));
		}
		if(foundtext.дайСчёт() > 1) {
			if(проп_ли(foundtext[1]))
				for(int i = 1; i < rt.дайСчёт(); i++)
					rt.уст(i, взаг(rt[i]));
			if(заг_ли(foundtext[1]))
				for(int i = 1; i < rt.дайСчёт(); i++)
					rt.уст(i, впроп(rt[i]));
		}
	}
	replacei++;
	return rt;
}

void РедакторКода::замени()
{
	if(!findreplace.replacing)
		return;
	NextUndo();
	FindReplaceAddHistory();
	if(!found) return;
	bool h = persistent_find_replace;
	persistent_find_replace = true; // avoid closing of findreplace by selection change
	if(удалиВыделение()) {
		Paste(GetReplaceText());
		найди(false, false, true);
	}
	persistent_find_replace = h;
}

int РедакторКода::BlockReplace()
{
	NextUndo();
	освежи(); // Setting full-refresh here avoids Pre/пост удали/вставь costs
	int l, h;
	if(!дайВыделение32(l, h)) return 0;
	поместиКаретку(l);
	int count = 0;
	foundpos = l;
	Индекс<int> ln;
	StartSearchProgress(l, h);
	while(foundpos < дайДлину() && найдиОт(foundpos, false, true) && foundpos + foundsize <= h) {
		CachePos(foundpos);
		if(~findreplace.mode == 0) {
			удали((int)foundpos, foundsize);
			ШТкст rt = GetReplaceText();
			вставь((int)foundpos, rt);
			foundpos += rt.дайСчёт();
			if(foundsize + rt.дайСчёт() == 0 && foundpos < дайДлину())
				foundpos++;
			h = h - foundsize + rt.дайСчёт();
			count++;
		}
		else {
			ln.найдиДобавь(дайСтроку(foundpos));
			foundpos += foundsize;
		}
	}
	if(SearchCanceled())
		return count;
	EndSearchProgress();
	Progress pi("Удаление строк");
	if(~findreplace.mode != 0) {
		очистьВыделение();
		int ll = дайСтроку(l);
		int lh = дайСтроку(h);
		if(дайПоз64(lh) == h)
			lh--;
		bool mm = ~findreplace.mode == 1;
		Ткст replace;
		pi.устВсего(lh - ll + 1);
		for(int i = ll; i <= lh; i++) {
			pi.уст(i, lh - ll + 1);
			if(pi.Canceled())
				return 0;
			if((ln.найди(i) >= 0) == mm) {
				replace << дайУтф8Строку(i) << "\n";
				count++;
			}
		}
		int pos = дайПоз32(ll);
		удали(pos, дайПоз32(дайСтроку(h)) - pos);
		устВыделение(pos, pos + вставь(pos, replace));
	}
	else
		устВыделение(l, h);
	return count;
}

void РедакторКода::OpenNormalFindReplace0(bool replace)
{
	findreplace.incremental.вкл(дайДлину64() < 2000000);
	findreplace.настрой(replace);
	findreplace.find_all.покажи(!replace && WhenFindAll);
	findreplace.itext = GetI();
	findreplace.prev.покажи();
	findreplace.next <<= THISBACK(DoFind);
	findreplace.close <<= THISBACK(EscapeFindReplace);
	if(!findreplace.открыт())
		вставьФрейм(найдиФрейм(sb), findreplace);
	WhenOpenFindReplace();
	findreplace.find.устФокус();
}

void РедакторКода::OpenNormalFindReplace(bool replace)
{
	OpenNormalFindReplace0(replace);
//	if(!findreplace.incremental_from_cursor)
//		IncrementalFind();
}

void РедакторКода::FindReplace(bool pick_selection, bool pick_text, bool replace)
{
	if(толькочтен_ли())
		replace = false;
	
	if(findreplace.открыт())
		закройНайдиЗам();

	ff_start_pos = дайКурсор32();

	replacei = 0;
	ШТкст find_text;
	int find_pos = -1;
	
	findreplace.настрой(replace);
	
	if(pick_text || pick_selection)
	{
		if(выделение_ли()) {
			int l, h;
			дайВыделение32(l, h);
			if(h - l < 100) {
				find_text = GetSelectionW();
				if(find_text.найди('\n') >= 0)
					find_text.очисть();
			}
		}
		else
		if(pick_text) {
			int l, h;
			l = h = дайКурсор32();
			while(l > 0 && CharFilterCIdent(дайСим(l - 1)))
				l--;
			while(h < дайДлину64() && CharFilterCIdent(дайСим(h)))
				h++;
			find_text = дай(l, h - l).вШТкст();
			find_pos = h;
		}
		if(find_text.дайСчёт())
			findreplace.find <<= find_text;
	}
	if(выделение_ли() && replace) {
		findreplace.itext = GetI();
		SetLayout_BlockReplaceLayout(findreplace);
		findreplace.устПрям(WithBlockReplaceLayout<EmptyClass>::GetLayoutSize());
		findreplace.Титул(t_("Замена в выделении"));
		findreplace.find_all.скрой();
		findreplace.amend.скрой();
		findreplace.amend_all.скрой();
		findreplace.amend_rest.скрой();
		findreplace.prev.скрой();
		findreplace.next.Ok() <<= findreplace.Breaker(IDOK);
		findreplace.close.Cancel() <<= findreplace.Breaker(IDCANCEL);
		findreplace.close.устРисунок(Null);
		findreplace.close.Подсказка("");
		findreplace.next.устРисунок(Null);
		findreplace.next.Подсказка("");
		findreplace.mode.покажи();
		findreplace.mode <<= 0;
		findreplace.синх();
		if(findreplace.выполни() == IDOK)
			BlockReplace();
		findreplace.mode.скрой();
	}
	else {
		if(find_pos >= 0)
			устКурсор(find_pos);
		OpenNormalFindReplace(replace);
		findreplace.find.устФокус();
	}
}

void РедакторКода::ReplaceAll(bool rest)
{
	int l, h;
	дайВыделение32(l, h);
	int c = min(l, h);
	findreplace.mode <<= 0;
	устВыделение(rest * c, дайДлину64());
	BlockReplace();
	устКурсор(c);
}

void РедакторКода::InsertWildcard(const char *s)
{
	iwc = s;
}

void FindWildcardMenu(Обрвыз1<const char *> cb, Точка p, bool tablf, Ктрл *owner, bool regexp)
{
	БарМеню menu;
	if(regexp) {
		menu.добавь(t_("Один или более пробелов"), callback1(cb, " +"));
		menu.добавь(t_("Один или более любых символов"), callback1(cb, ".+"));
		menu.добавь(t_("Слово"), callback1(cb, "\\w+"));
		menu.добавь(t_("Число"), callback1(cb, "\\d+"));
		menu.добавь(t_("Любой символ"), callback1(cb, "."));
		if(tablf) {
			menu.Separator();
			menu.добавь(t_("Таб"), callback1(cb, "\\t"));
		}
	}
	else {
		menu.добавь(t_("Один или более пробелов"), callback1(cb, "%"));
		menu.добавь(t_("Один или более любых символо"), callback1(cb, "*"));
		menu.добавь(t_("Идентификатор C++"), callback1(cb, "$"));
		menu.добавь(t_("Число"), callback1(cb, "#"));
		menu.добавь(t_("Любой символ"), callback1(cb, "?"));
		if(tablf) {
			menu.Separator();
			menu.добавь(t_("Таб"), callback1(cb, "\\t"));
			menu.добавь(t_("Строка feed"), callback1(cb, "\\n"));
		}
	}
	menu.выполни(owner, p);
}

void РедакторКода::FindWildcard()
{
	int l, h;
	findreplace.find.дайВыделение(l, h);
	iwc.очисть();
	FindWildcardMenu(THISBACK(InsertWildcard), findreplace.find.GetPushScreenRect().верхПраво(), true,
	                 &findreplace, findreplace.regexp);
	if(iwc.дайСчёт()) {
		if(!findreplace.regexp)
			findreplace.wildcards = true;
		findreplace.find.устФокус();
		findreplace.find.устВыделение(l, h);
		findreplace.find.удалиВыделение();
		findreplace.find.вставь(iwc);
	}
}

void РедакторКода::ReplaceWildcard()
{
	БарМеню menu;
	Ткст ptxt;
	if(findreplace.regexp)
		ptxt = t_("Сверен подобразец %d");
	else {
		menu.добавь(t_("Matched пробелы"), THISBACK1(InsertWildcard, "%"));
		menu.добавь(t_("Matched one or more any characters"), THISBACK1(InsertWildcard, "*"));
		menu.добавь(t_("Matched C++ identifier"), THISBACK1(InsertWildcard, "$"));
		menu.добавь(t_("Matched number"), THISBACK1(InsertWildcard, "#"));
		menu.добавь(t_("Matched any character"), THISBACK1(InsertWildcard, "?"));
		ptxt = t_("Matched wildcard %d");
	}
	menu.добавь(t_("0-based replace Индекс"), THISBACK1(InsertWildcard, "@@"));
	menu.добавь(t_("1-based replace Индекс"), THISBACK1(InsertWildcard, "@#"));
	menu.Separator();
	for(int i = 1; i <= 9; i++)
		menu.добавь(фмт(ptxt, i), THISBACK1(InsertWildcard, "@"+какТкст(i)));
	menu.Separator();
	menu.добавь(t_("To upper"), THISBACK1(InsertWildcard, "+"));
	menu.добавь(t_("To lower"), THISBACK1(InsertWildcard, "-"));
	menu.добавь(t_("иницШапки"), THISBACK1(InsertWildcard, "!"));
	menu.Separator();
	menu.добавь(t_("Вкладка"), THISBACK1(InsertWildcard, "\\t"));
	menu.добавь(t_("Строка feed"), THISBACK1(InsertWildcard, "\\n"));
	int l, h;
	findreplace.replace.дайВыделение(l, h);
	iwc.очисть();
	menu.выполни(&findreplace, findreplace.replace.GetPushScreenRect().верхПраво());
	if(iwc.дайСчёт()) {
		if(!findreplace.regexp)
			findreplace.wildcards = true;
		findreplace.replace.устФокус();
		findreplace.replace.устВыделение(l, h);
		findreplace.replace.удалиВыделение();
		findreplace.replace.вставь(iwc);
	}
}

void РедакторКода::закройНайдиЗам()
{
	if(findreplace.открыт()) {
		FindReplaceAddHistory();
		удалиФрейм(findreplace);
	}
}

void РедакторКода::EscapeFindReplace()
{
	закройНайдиЗам();
	if(ff_start_pos >= 0 && ff_start_pos < дайДлину64() && findreplace.инкрементален_ли() && do_ff_restore_pos) {
		устКурсор(ff_start_pos);
		ff_start_pos = -1;
	}
}

void РедакторКода::IncrementalFind()
{
	NoFindError();
	findreplace.синх();
	if(!findreplace.инкрементален_ли() || findreplace.дайТопКтрл() == &findreplace) // || we are block replace
		return;
	bool b = найдиОт(ff_start_pos >= 0 && ff_start_pos < дайДлину64()
	                  && findreplace.incremental_from_cursor ? ff_start_pos : 0, false, false);
	findreplace.amend.вкл(b);
	if(!b)
		NotFound();
}

void РедакторКода::DoFind()
{
	найди(false, false);
}

void РедакторКода::DoFindBack()
{
	найди(true, false);
}

void РедакторКода::SerializeFind(Поток& s)
{
	int version = 2;
	s / version;
	s % findreplace.find;
	findreplace.find.SerializeList(s);
	s % findreplace.wholeword % findreplace.ignorecase % findreplace.wildcards;
	if(version >= 0)
		s % findreplace.samecase;
	s % findreplace.replace;
	if(version >= 1)
		s % findreplace.incremental;
	if(version >= 2)
		s % findreplace.regexp;
	findreplace.replace.SerializeList(s);
}

Ткст ReadList(WithDropChoice<EditString>& e)
{
	ТкстПоток ss;
	e.SerializeList(ss);
	return ss;
}

void WriteList(WithDropChoice<EditString>& e, const Ткст& data)
{
	ТкстПоток ss(data);
	e.SerializeList(ss);
}

РедакторКода::FindReplaceData РедакторКода::GetFindReplaceData()
{
	FindReplaceData r;
	r.find = ~findreplace.find;
	r.replace = ~findreplace.replace;
	r.wholeword = ~findreplace.wholeword;
	r.ignorecase = ~findreplace.ignorecase;
	r.wildcards = ~findreplace.wildcards;
	r.samecase = ~findreplace.samecase;
	r.regexp = ~findreplace.regexp;
	r.find_list = ReadList(findreplace.find);
	r.replace_list = ReadList(findreplace.replace);
	return r;
}

void РедакторКода::SetFindReplaceData(const FindReplaceData& r)
{
	findreplace.find <<= r.find;
	findreplace.replace <<= r.replace;
	findreplace.wholeword <<= r.wholeword;
	findreplace.ignorecase <<= r.ignorecase;
	findreplace.wildcards <<= r.wildcards;
	findreplace.samecase <<= r.samecase;
	findreplace.regexp <<= r.regexp;
	findreplace.mode <<= 0;
	WriteList(findreplace.find, r.find_list);
	WriteList(findreplace.replace, r.replace_list);
}

void РедакторКода::FindPrevNext(bool prev)
{
	StartSearchProgress(-1, -1);
	if(!findreplace.открыт()) {
		ШТкст find_text;
		if(выделение_ли()) {
			int l, h;
			дайВыделение32(l, h);
			if(h - l < 100) {
				find_text = GetSelectionW();
				if(find_text.найди('\n') >= 0)
					find_text.очисть();
			}
		}
		if(find_text.дайСчёт())
			findreplace.find <<= find_text;
		OpenNormalFindReplace0(false);
	}
	if(найди(prev, false))
		NoFindError();
	else
		NotFound();
	EndSearchProgress();
}

void РедакторКода::найдиСледщ()
{
	FindPrevNext(false);
}

void РедакторКода::найдиПредш()
{
	FindPrevNext(true);
}

void РедакторКода::StartSearchProgress(int64, int64)
{
	search_canceled = false;
	search_progress.создай();
	search_progress->устТекст("Сканируется этот файл");
	search_time0 = msecs();
}

bool РедакторКода::SearchProgress(int line)
{
	if(search_progress && !search_canceled && msecs(search_time0) > 20) {
		search_time0 = msecs();
		search_progress->создай();
		search_canceled = IsView() ? search_progress->SetCanceled(int(дайПоз64(line) >> 8), int(дайРазмОбзора() >> 8))
		                           : search_progress->SetCanceled(line, дайСчётСтрок());
	}
	return !search_canceled;
}

bool РедакторКода::SearchCanceled()
{
	return search_canceled;
}

void РедакторКода::EndSearchProgress()
{
	search_progress.очисть();
	search_canceled = false;
}

}
