#include "Browser.h"

bool MatchCib(const Ткст& s, const Ткст& match)
{
	if(пусто_ли(match)) return true;
	int q = взаг(s).найди(match);
	return q > 0 && !iscid(s[q - 1]) || q == 0;	
}

bool MatchPm(const Ткст& фн, const Ткст& pm)
{
	if(пусто_ли(pm))
		return true;
	return фн.начинаетсяС(pm);
}

bool MatchPm(int file, const Ткст& pm)
{
	if(пусто_ли(pm))
		return true;
	return GetSourceFilePath(file).начинаетсяС(pm);
}

bool MatchPm(const Массив<CppItem>& n, const Ткст& pm)
{
	if(пусто_ли(pm))
		return true;
	for(int i = 0; i < n.дайСчёт(); i++)
		if(MatchPm(n[i].file, pm))
			return true;
	return false;
}

bool HasItem(const Массив<CppItem>& n, const Ткст& m)
{
	if(пусто_ли(m))
		return true;
	for(int i = 0; i < n.дайСчёт(); i++)
		if(n[i].uname.начинаетсяС(m))
			return true;
	return false;
}

struct ScopeLess {
	bool operator()(const Ткст& a, const Ткст& b) const {
		if((*a == '<') != (*b == '<'))
			return *a > *b;
		return a < b;
	}
};

Ткст CodeBrowser::GetPm()
{
	Ткст pm;
	if(TheIde() && range) {
		if(range == 1)
			pm = TheIde()->IdeGetNestFolder();
		else {
			pm = TheIde()->исрДайИмяф();
			if(range == 2)
				pm = дайПапкуФайла(pm);
		}
	}
	return pm;
}

Ткст GetFileText(const Ткст& s)
{
	return '<' + дайИмяф(дайПапкуФайла(s)) + '/' + дайИмяф(s) + '>';
}

void CodeBrowser::грузи()
{
	Ткст match = взаг((Ткст)~search_scope);
	Ткст pm = GetPm();
	Вектор<Ткст> txt;
	Вектор<Значение>  ndx;
	Индекс<int>     fi;
	Индекс<Ткст>  fs;
	{
		CodeBaseLock __;
		for(int i = 0; i < CodeBase().дайСчёт(); i++) {
			Ткст s = CodeBase().дайКлюч(i);
			const Массив<CppItem>& n = CodeBase()[i];
			if(s.дайСчёт())
				if(MatchCib(s, match) && MatchPm(n, pm)) {
					txt.добавь(s);
					ndx.добавь(s);
				}
			for(int i = 0; i < n.дайСчёт(); i++) {
				int f = n[i].file;
				if(fi.найди(f) < 0) {
					Ткст s = GetFileText(GetSourceFilePath(f));
					if(s.начинаетсяС(pm) && MatchCib(s, match)) {
						txt.добавь(s);
						ndx.добавь(f);
						fs.добавь(s);
						fi.добавь(f);
					}
				}
			}
		}
		const РОбласть& wspc = GetIdeWorkspace();
		for(int i = 0; i < wspc.дайСчёт(); i++) {
			Ткст pn = wspc[i];
			const Пакет& p = wspc.дайПакет(i);
			Ткст pp = PackageDirectory(pn);
			for(int j = 0; j < p.дайСчёт(); j++)
				if(!p[j].separator) {
					Ткст фн = приставьИмяф(pp, p[j]);
					Ткст s = GetFileText(приставьИмяф(pn, p[j]));
					if(fs.найди(s) < 0 && MatchCib(s, match) && MatchPm(фн, pm)) {
						int f = GetSourceFileIndex(SourcePath(pn, p[j]));
						txt.добавь(s);
						ndx.добавь(f);
						fs.добавь(s);
					}
				}
		}
	}
	IndexSort(txt, ndx, ScopeLess());
	Значение ключ = scope.дайКлюч();
	int sc = scope.GetCursorSc();
	scope.очисть();
	for(int i = 0; i < txt.дайСчёт(); i++)
		scope.добавь(ткст_ли(ndx[i]) ? ndx[i] : Null, txt[i], ndx[i]);
	if(scope.FindSetCursor(ключ))
		scope.ScCursor(sc);
//	clear.вкл(IsSearch());
}

int ItemCompare(const Значение& v1, const Значение& v2)
{
	const CppItemInfo& a = ValueTo<CppItemInfo>(v1);
	const CppItemInfo& b = ValueTo<CppItemInfo>(v2);
	int q = a.inherited - b.inherited;
	if(q) return q;
	q = сравниЗнак(GetSourceFilePath(a.file), GetSourceFilePath(b.file));
	return q ? q : a.line - b.line;
}

int ItemCompareLexical(const Значение& v1, const Значение& v2)
{
	const CppItemInfo& a = ValueTo<CppItemInfo>(v1);
	const CppItemInfo& b = ValueTo<CppItemInfo>(v2);
	int q = (int)b.IsType() - (int)a.IsType();
	if(q) return q;
	q = сравниЗнак(a.имя, b.имя);
	if(q) return q;
	return сравниЗнак(a.qitem, b.qitem);
}

void GatherMethods(const Ткст& тип, ВекторМап<Ткст, bool>& inherited, bool g, Индекс<Ткст>& done)
{
	CodeBaseLock __;
	if(done.найди(тип) >= 0) return;
	done.добавь(тип);
	int q = CodeBase().найди(тип);
	if(q < 0) return;
	const Массив<CppItem>& n = CodeBase()[q];
	Индекс<Ткст> set;
	for(int i = 0; i < n.дайСчёт(); i++) {
		const CppItem& m = n[i];
		if(set.найди(m.qitem) < 0) {
			set.добавь(m.qitem);
			if(m.IsType()) {
				Вектор<Ткст> base = разбей(m.qptype, ';');
				for(int i = 0; i < base.дайСчёт(); i++)
					GatherMethods(base[i], inherited, true, done);
			}
			if(m.IsCode() && g) {
				bool& virt = inherited.дайДобавь(m.qitem);
				virt = virt || m.virt;
			}
		}
	}
}

void GatherMethods(const Ткст& тип, ВекторМап<Ткст, bool>& inherited, bool g)
{
	Индекс<Ткст> done;
	GatherMethods(тип, inherited, g, done);
}

void CodeBrowser::LoadScope()
{
	Значение ключ = элт.дайКлюч();
	int sc = элт.GetCursorSc();
	{
		CodeBaseLock __;
		Ткст find = взаг((Ткст)~search);
		элт.очисть();
		if(!scope.курсор_ли())
			return;
		Значение x = scope.дай(2);
		int file = число_ли(x) ? (int)x : -1;
		Ткст scope = file < 0 ? Ткст(x) : Ткст();
		auto Do = [&](int q) {
			const Массив<CppItem>& n = CodeBase()[q];
			ВекторМап<Ткст, bool> inherited;
			if(file < 0)
				GatherMethods(scope, inherited, false);
			Индекс<Ткст> set;
			for(int i = 0; i < n.дайСчёт(); i++) {
				CppItemInfo m;
				(CppItem&) m = n[i];
				if((file < 0 || m.file == file) && m.uname.начинаетсяС(find) && set.найди(m.qitem) < 0) {
					set.добавь(m.qitem);
					int q = inherited.найди(m.qitem);
					if(q >= 0) {
						m.over = true;
						m.virt = m.virt || inherited[q];
					}
					элт.добавь(m.qitem, RawToValue(m));
				}
			}
		};
		if(file >= 0)
			for(int q = 0; q < CodeBase().дайСчёт(); q++)
				Do(q);
		else {
			int q = CodeBase().найди(scope);
			if(q >= 0)
				Do(q);
		}
	}
	элт.сортируй(1, sort ? ItemCompareLexical : ItemCompare);
	if(элт.FindSetCursor(ключ))
		элт.ScCursor(sc);
//	clear.вкл(IsSearch());
}

Ткст CodeBrowser::GetCodeRef(int i) const
{
	if(scope.курсор_ли())
		return MakeCodeRef(scope.дайКлюч(), элт.дай(i, 0));
	return Null;
}

Ткст CodeBrowser::GetCodeRef() const
{
	return элт.курсор_ли() ? GetCodeRef(элт.дайКурсор()) : Ткст();
}

CppItemInfo CodeBrowser::GetItemInfo(int i) const
{
	return ValueTo<CppItemInfo>(элт.дай(i, 1));
}

CppItemInfo CodeBrowser::GetItemInfo() const
{
	return GetItemInfo(элт.дайКурсор());
}

int SearchItemFilter(int c)
{
	c = взаг(c);
	return IsAlNum(c) || c == '/' || c == '.' || c == '<' || c == '>' || c == '_' ? c : 0;
}

void CodeBrowser::идиК(const Ткст& coderef, const Ткст& rfile)
{
	if(пусто_ли(coderef)) {
		элт.анулируйКурсор();
		scope.FindSetCursor(GetFileText(rfile), 1);
		return;
	}
	else
	if(coderef != GetCodeRef()) {
		if(!пусто_ли(search_scope)) {
			грузи();
			LoadScope();
		}
		Ткст sc, im;
		SplitCodeRef(coderef, sc, im);
		if(пусто_ли(sc)) {
			const CppItem *m = GetCodeRefItem(coderef, rfile);
			if(m)
				scope.FindSetCursor(m->file, 2);
		}
		else
			scope.FindSetCursor(sc);
		элт.FindSetCursor(im);
	}
}

void CodeBrowser::ищи()
{
	LoadScope();
	if(!scope.курсор_ли())
		scope.устКурсор(0);
	if(!элт.курсор_ли())
		элт.устКурсор(0);
}

bool CodeBrowser::Ключ(dword ключ, int count)
{
//	clear.вкл(IsSearch());
	if(ключ == K_UP || ключ == K_DOWN) {
		if(search.естьФокус()) {
			int l, h;
			search.дайВыделение(l, h);
			Значение v = ~search;
			if(элт.курсор_ли())
				элт.Ключ(ключ, count);
			else
			if(ключ == K_UP)
				элт.идиВКон();
			else
				элт.идиВНач();
			WhenKeyItem();
			search <<= v;
			search.устФокус();
			search.устВыделение(l, h);
			return true;
		}
		if(search_scope.естьФокус()) {
			scope.Ключ(ключ, count);
			return true;
		}
	}
	return false;
}

bool CodeBrowser::IsSearch() const
{
	return !пусто_ли(search) || !пусто_ли(search_scope);
}

void CodeBrowser::ClearSearch()
{
	if(!IsSearch())
		return;
	search_scope <<= search <<= Null;
	грузи();
	WhenClear();
}

void CodeBrowser::устДиапазон(int r)
{
	if(range == r)
		r = 0;
	range = r;
	for(int i = 0; i < 3; i++)
		rangebutton[i] <<= range == i + 1;
	грузи();
}

CodeBrowser::CodeBrowser()
{
	scope.добавьКлюч();
	scope.добавьКолонку("Масштаб");
	scope.WhenSel = THISBACK(LoadScope);
	scope.NoHeader().NoGrid();
	search_scope <<= THISBACK(грузи);
	search_scope.устФильтр(SearchItemFilter);
	search_scope.NullText("Найти тип или заголовок ");
	элт.добавьКлюч();
	элт.добавьКолонку("Элемент").устДисплей(дисплей).Margin(2);
	элт.NoHeader();
	элт.SetLineCy(BrowserFont().Info().дайВысоту() + 3);
	search.NullText("Найти ");
	search.устФильтр(SearchItemFilter);
	search <<= THISBACK(ищи);
//	search.добавьФрейм(clear);
//	clear.устРисунок(BrowserImg::очисть());
//	clear.NoWantFocus();
//	clear <<= THISBACK(ClearSearch);
	range = 0;
	static const char *tip[] = { "Гнездо", "Пакет", "Файл" };
	for(int i = 0; i < 3; i++)
		rangebutton[i].устРисунок(BrowserImg::дай(BrowserImg::I_range_nest + i)).Подсказка(tip[i]).NoWantFocus()
		              <<= THISBACK1(устДиапазон, i + 1);
	устДиапазон(0);
	sort.Подсказка("Упорядочить по именам");
	sort.устРисунок(BrowserImg::Sort()).NoWantFocus();
	sort <<= THISBACK(LoadScope);
}
