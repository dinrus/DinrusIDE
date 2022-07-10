#include "DinrusIDE.h"

bool Иср::FindLineError(const Ткст& ln, FindLineErrorCache& cache, ErrorInfo& f)
{
	if(!cache.init) {
		ВекторМап<Ткст, Ткст> bm = GetMethodVars(method);
		cache.is_java = (bm.дай("BUILDER", Null) == "JDK");
		cache.upp = GetUppDir();
		cache.init = true;
	}
	const char *s0 = ln;
	while(*s0 == ' ' || *s0 == '\t')
		s0++;
	for(;;) {
		const char *s = s0;
		f.file.очисть();
		for(; s < ln.стоп(); s++) {
			if(*s != '\"' && (byte)*s >= 32 && *s != '(' && (f.file.дайДлину() < 3 || *s != ':'))
				f.file.конкат(*s);
			else {
				if(*s == '\"') {
					f.file = Null;
					s++;
					while(*s && *s != '\"')
						f.file.конкат(*s++);
					if(*s)
						s++;
				}
				Ткст file = f.file;
				int e = file.дайДлину();
				while(e > 0 && file[e - 1] == ' ')
					e--;
				file.обрежь(e);
				file = обрежьЛево(file);
	
				int q = cache.file.найди(file);
				if(q < 0) {
					Ткст file0 = file;
				#ifdef PLATFORM_WIN32
					if(file[0] == '\\' || file[0] == '/')
						file = Ткст(cache.upp[0], 1) + ':' + file;
				#endif
					bool exists = false;
					if(!полнпуть_ли(file) && *file != '\\' && *file != '/') {
						if(cache.wspc_paths.пустой()) {
							::РОбласть  wspc;
							wspc.скан(main);
							for(int i = 0; i < wspc.дайСчёт(); i++)
								cache.wspc_paths.добавь(дайДиректориюФайла(PackagePath(wspc[i])));
						}
						for(int i = 0; i < cache.wspc_paths.дайСчёт(); i++) {
							Ткст path = приставьИмяф(cache.wspc_paths[i], file);
							Ткст ext = впроп(дайРасшф(path));
							if(findarg(ext, ".obj", ".lib", ".o", ".so", ".a", ".", "") < 0) {
								ФайлПоиск ff;
								if(ff.ищи(path) && ff.файл_ли()) {
									file = path;
									exists = true;
									break;
								}
							}
						}
					}
					file = FollowCygwinSymlink(file);
					if(!полнпуть_ли(file) || !exists && !файлЕсть(file) || !текстфл_ли(file))
						file = Null;
					cache.file.добавь(file0, file);
				}
				else
					file = cache.file[q];
				if(file.дайСчёт()) {
					f.file = file;
					while(*s && !цифра_ли(*s)) {
						if(*s == '/' || IsAlpha(*s))
							return false;
						s++;
					}
					f.lineno = f.linepos = 0;
					СиПарсер p(s);
					try {
						if(p.цел_ли())
							f.lineno = p.читайЦел();
						if(p.сим(':') && p.цел_ли())
							f.linepos = p.читайЦел();
					}
					catch(СиПарсер::Ошибка) {}
					const char *ms = p.дайУк();
					if(ln.найди(": warning") >= 0)
						f.kind = 2;
					else if(ln.найди(": Ошибка") >= 0 || ln.найди(": fatal Ошибка") >= 0)
						f.kind = 1;
					else
						f.kind = 3;
					const char *hs = ms;
					while(!буква_ли(*hs) && *hs)
						hs++;
					f.message = *hs ? hs : ms;
					f.message = обрежьЛево(f.message);
					if(пусто_ли(f.message))
						f.message = ln;
					Вектор<Ткст> conf = SplitFlags(mainconfigparam, true);
					Ткст uppout = GetVar("OUTPUT");
					int upplen = uppout.дайДлину();
					if(cache.is_java && f.file.дайДлину() > upplen
					&& !памсравнИ(f.file, uppout, upplen) && f.file[upplen] == DIR_SEP) { // check for preprocessed file
						ФайлВвод fi(f.file);
						if(fi.открыт()) {
							Ткст fake_file = f.file;
							int fake_line = 1;
							int file_line = 1;
							while(!fi.кф_ли())
							{
								Ткст line = fi.дайСтроку();
								const char *p = line;
								if(p[0] == '/' && p[1] == '/' && p[2] == '#')
								{
									p += 3;
									if(p[0] == 'l' && p[1] == 'i' && p[2] == 'n' && p[3] == 'e')
										p += 4;
									while(*p == ' ' || *p == '\t')
										p++;
									if(цифра_ли(*p))
									{
										fake_line = stou(p, &p);
										while(*p == ' ' || *p == '\t')
											p++;
										if(*p == '\"')
											p++;
										fake_file.очисть();
										while(*p && *p != '\"')
											if(*p == '/')
											{
												fake_file.конкат('/');
												if(p[1] == '/')
													p++;
												p++;
											}
											else
												fake_file.конкат(*p++);
									}
									file_line++;
									continue;
								}
								if(f.lineno <= file_line) {
									f.file = fake_file;
									f.lineno = fake_line;
									f.linepos = 0;
									break;
								}
								file_line++;
								fake_line++;
							}
						}
					}
					return f.lineno > 0;
				}
				else
				if(*s == ':' || !strchr(s, '/') && !strchr(s, '\\')) // safe to say this is final
					break;
				else
					f.file.конкат(*s); // Файл is not complete, e.g.: C:\Program Files (x86)\Microsoft Visual Studio 10.0\Vc\Include\string.h(186)
			}
		}
		while(*s0 != ' ' && *s0 != '\t') { // skip to next whitespace to try again
			if(s0 >= ln.стоп())
				return false;
			s0++;
		}
		while(*s0 == ' ' || *s0 == '\t')
			s0++;
	}
}

void Иср::FindError()
{
	FindLineError(console.дайСтроку(console.дайКурсор()));
}

bool Иср::следщ(КтрлМассив& a, int d)
{
	if(a.видим_ли()) {
		int c = a.дайКурсор();
		for(;;) {
			c += d;
			if(c >= 0 && c < a.дайСчёт()) {
				Значение v = a.дай(c, "INFO");
				if(v.является<ErrorInfo>() && !пусто_ли(v.To<ErrorInfo>().file)) {
					a.устКурсор(c);
					return true;
				}
			}
			else {
				if(d > 0)
					a.идиВНач();
				else
					a.идиВКон();
				return true;
			}
		}
	}
	return false;
}

void Иср::FindNextError()
{
	if(следщ(Ошибка, 1) || следщ(FFound(), 1))
		return;
	int ln = console.дайСтроку(console.дайКурсор());
	int l = ln;
	for(l = ln; l < console.дайСчётСтрок(); l++)
		if(FindLineError(l)) return;
	for(l = 0; l < ln; l++)
		if(FindLineError(l)) return;
}

void Иср::FindPrevError() {
	if(следщ(Ошибка, -1) || следщ(FFound(), -1))
		return;
	int ln = console.дайСтроку(console.дайКурсор());
	int l = ln;
	Хост h;
	CreateHost(h, false, disable_uhd);
	for(l = ln - 2; l >= 0; l--)
		if(FindLineError(l)) return;
	for(l = console.дайСчётСтрок() - 1; l > ln; l--)
		if(FindLineError(l)) return;
}

void Иср::ClearErrorEditor()
{
	if(!mark_lines)
		return;

	for(int i = 0; i < filedata.дайСчёт(); i++) {
		ClearErrorEditor(filedata.дайКлюч(i));
	}
	
	SetErrorFiles(Вектор<Ткст>());
}

void Иср::ClearErrorEditor(Ткст file)
{
	linking = false;

	if(!mark_lines)
		return;
	if(file == editfile)
		editor.очистьОшибки();
	else {
		FileData& fd = Filedata(file);
		очистьОшибки(fd.lineinfo);
	}
}

void Иср::SetErrorEditor()
{
	if(Ошибка.дайСчёт()) {
		устНиз(BERRORS);
//		if(!Ошибка.курсор_ли())
//			Ошибка.идиВНач();
	}

	if(!mark_lines)
		return;

	bool refresh = false;
	Ткст    hfile;
	EditorBar hbar;
	Вектор<Ткст> errorfiles;
	FindLineErrorCache cache;
	for(int i = 0; i < console.дайСчётСтрок(); i++) {
		ErrorInfo f;
		if(FindLineError(console.дайУтф8Строку(i), cache, f)) {
			Ткст file = нормализуйПуть(f.file);
		#ifdef PLATFORM_WIN32
			errorfiles.добавь(впроп(file));
		#else
			errorfiles.добавь(file);
		#endif
			if(editfile == file) {
				editor.устОш(f.lineno - 1, f.kind);
				refresh = true;
			}
			else {
				if(hfile != file) {
					if(hfile.дайСчёт())
						Filedata(hfile).lineinfo = hbar.GetLineInfo();
					hbar.SetLineInfo(Filedata(file).lineinfo, -1);
					hfile = file;
				}
				hbar.устОш(f.lineno - 1, f.kind);
			}
		}
	}
	if(hfile.дайСчёт())
		Filedata(hfile).lineinfo = hbar.GetLineInfo();
	if(refresh)
		editor.освежиФрейм();
	SetErrorFiles(errorfiles);
}

void Иср::GoToError(const ErrorInfo& f)
{
	if(пусто_ли(f.file))
		return;
	Ткст file = нормализуйПуть(f.file);
	DoEditAsText(file);
	EditFile(file);
	int lp = max(f.linepos - 1, 0);
	int pos = editor.дайПоз(editor.дайНомСтр(f.lineno - 1), lp);
	editor.устКурсор(pos);
	if(*f.message == '\1') {
		Вектор<Ткст> h = разбей(~f.message + 1, '\1', false);
		if(h.дайСчёт() >= 4)
			editor.Illuminate(h[3].середина(atoi(h[1]), atoi(h[2])).вШТкст());
	}
	editor.курсорПоЦентру();
	editor.устФокус();
	синх();
}

void Иср::GoToError(КтрлМассив& a)
{
	Значение v = a.дай("ИНФО");
	if(v.является<ErrorInfo>())
		GoToError(ValueTo<ErrorInfo>(v));
}

bool Иср::FindLineError(int l) {
	ErrorInfo f;
	FindLineErrorCache cache;
	if(FindLineError(console.дайУтф8Строку(l), cache, f)) {
		GoToError(f);
		console.устВыделение(console.дайПоз64(l), console.дайПоз64(l + 1));
		if(btabs.дайКурсор() != BCONSOLE && !BottomIsFindInFiles())
			ShowConsole();
		return true;
	}
	return false;
}

void Иср::ClearErrorsPane()
{
	linking = false;
	linking_line.очисть();
	error_cache.очисть();
	Ошибка.очисть();
	SyncErrorsMessage();
	error_count = 0;
	warning_count = 0;
}

void Иср::PutLinking()
{
	linking = true;
	linking_line.очисть();
}

void Иср::PutLinkingEnd(bool ok)
{
	if(!ok && linking) {
		addnotes = true;
		error_count++;
		Ошибка.добавь(Null, Null, AttrText("Компоновка не удалась").Bold()
			                  .NormalPaper(HighlightSetup::GetHlStyle(HighlightSetup::PAPER_ERROR).color));
		for(int i = 0; i < linking_line.дайСчёт(); i++) {
			ErrorInfo f;
			if(!FindLineError(linking_line[i], error_cache, f)) {
				f.file = Null;
				f.lineno = Null;
				f.message = обрежьЛево(linking_line[i]);
			}
			int linecy;
			Ошибка.добавь(f.file, f.lineno,
			          AttrText(FormatErrorLine(f.message, linecy))
			          .NormalPaper(HighlightSetup::GetHlStyle(HighlightSetup::PAPER_ERROR).color),
			          RawToValue(f));
			Ошибка.SetLineCy(Ошибка.дайСчёт() - 1, linecy);
		}
	}
	SyncErrorsMessage();
	linking = false;
}

bool IsDarkMismatch()
{
	return тёмен(SColorPaper()) != тёмен(HighlightSetup::GetHlStyle(HighlightSetup::PAPER_NORMAL).color);
}

void Иср::TopAlignedDisplay::рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const
{
	w.DrawRect(r, paper);
	w.DrawText(r.left, r.top, какТкст(q));
}

ШТкст Иср::FormatErrorLine(const Ткст& text, int& linecy)
{
	ШТкст txt;
	int cx = max(GetStdFontCy() * 30, Ошибка.HeaderObject().GetTabWidth(2) - Ошибка.HeaderTab(2).GetMargin() * 2);
	int x = 0;
	Шрифт fnt = StdFont();
	ШТкст h = text.вШТкст();
	linecy = fnt.GetCy();
	const wchar *s = h;
	while(findarg(*s, ' ', '\t') >= 0)
		s++;
	while(*s) {
		int chcx = fnt[*s];
		if(x + chcx > cx) {
			txt.конкат('\n');
			x = 0;
			linecy += fnt.GetCy();
		}
		txt.конкат(*s);
		x += chcx;
		s++;
	}
	return txt;
}

ШТкст Иср::FormatErrorLineEP(const Ткст& text, const char *ep, int& linecy)
{
	ШТкст txt;
	int cx = Ошибка.HeaderObject().GetTabWidth(2) - Ошибка.HeaderTab(2).GetMargin() * 2;
	int x = 0;
	Шрифт fnt = StdFont();
	ШТкст h = text.вШТкст();
	linecy = fnt.GetCy();
	const wchar *s = h;
	while(findarg(*s, ' ', '\t') >= 0) {
		s++;
		if(*ep)
			ep++;
	}
	int lep = ' ';
	while(*s) {
		int chcx = fnt[*s];
		if(x + chcx > cx) {
			txt.конкат('\n');
			x = 0;
			linecy += fnt.GetCy();
		}
		if(lep != *ep) {
			txt.конкат(decode(*ep, '~', 2, '^', 3, 1));
			lep = *ep;
		}
		txt.конкат(*s);
		x += chcx;
		s++;
		if(*ep)
			ep++;
	}
	return txt;
}

struct ElepDisplay : public Дисплей {
	Размер    DoPaint(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const;

	virtual Размер дайСтдРазм(const Значение& q) const;
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const;
};

Размер ElepDisplay::DoPaint(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const
{
	w.DrawRect(r, paper);
	ШТкст txt = q;
	int st = 1;
	const wchar *s = txt;
	const wchar *b = s;
	int x = 0;
	int y = 0;
	int cx = 0;
	int linecy = StdFont().GetLineHeight();
	for(;;) {
		if((byte)*s < ' ') {
			int tcx = дайРазмТекста(b, StdFont(), int(s - b)).cx;
			if(st != 1 && (style & CURSOR) == 0)
				w.DrawRect(x + r.left, y + r.top, tcx, linecy,
				           HighlightSetup::GetHlStyle(st == 2 ? HighlightSetup::PAPER_WARNING
				                                              : HighlightSetup::PAPER_ERROR).color);
			w.DrawText(x + r.left, y + r.top, b, StdFont(), ink, int(s - b));
			x += tcx;
			cx = max(cx, tcx + x);
			if(*s == '\0')
				break;
			if(*s == '\n') {
				x = 0;
				y += linecy;
			}
			else
				st = *s;
			b = ++s;
		}
		else
			s++;
	}
	return Размер(cx, y + linecy);
}

Размер ElepDisplay::дайСтдРазм(const Значение& q) const
{
	NilDraw w;
	return DoPaint(w, Прям(0, 0, INT_MAX, INT_MAX), q, Null, Null, 0);
}

void ElepDisplay::рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const
{
	DoPaint(w, r, q, ink, paper, style);
}

void Иср::ConsoleLine(const Ткст& line, bool assist)
{
	if(linking) {
		linking_line.добавь(line);
		return;
	}
	ErrorInfo f;
	if(FindLineError(line, error_cache, f)) {
		if(assist)
			f.kind = 1;
		if(findarg(f.kind, 1, 2) >= 0 || Ошибка.дайСчёт() == 0) {
			Цвет paper = HighlightSetup::GetHlStyle(f.kind == 1 ? HighlightSetup::PAPER_ERROR
			                                                     : HighlightSetup::PAPER_WARNING).color;
			if(f.kind == 1)
				error_count++;
			else
				warning_count++;
			if(IsDarkMismatch())
				paper = SColorPaper();
			int linecy;
			Ошибка.добавь(f.file, f.lineno,
			          AttrText(FormatErrorLine(f.message, linecy)).NormalPaper(paper),
			          RawToValue(f));
			if(prenotes.дайСчёт()) {
				Ошибка.уст(Ошибка.дайСчёт() - 1, "ЗАМЕТКИ", prenotes);
				prenotes.очисть();
			}
			Ошибка.SetLineCy(Ошибка.дайСчёт() - 1, linecy);
			SyncErrorsMessage();
			addnotes = true;
			return;
		}
	}
	else {
		int q = line.найдиПосле(" from "); // GCC style "included from"
		ErrorInfo fi;
		if(q >= 0 && FindLineError(line.середина(q), error_cache, fi)) {
			fi.message = line;
			prenotes.добавь(RawToValue(fi));
			return;
		}
		f.lineno = Null;
		f.file = Null;
		f.message = line;
	}
	if(addnotes) {
		int cnt = Ошибка.дайСчёт();
		if(cnt == 0)
			return;
		МассивЗнач n = Ошибка.дай(cnt - 1, "ЗАМЕТКИ");
		bool iserrorpos = true;
		for(const char *s = f.message; *s; s++)
			if(*s != ' ' && *s != '~' && *s != '^' && *s != '|')
				iserrorpos = false;
		int i = n.дайСчёт() - 1;
		if(iserrorpos && i >= 0) {
			ErrorInfo f0 = ValueTo<ErrorInfo>(n[i]);
			f0.error_pos = f.message;
			n.уст(i, RawToValue(f0));
		}
		else
			n.добавь(RawToValue(f));
		Ошибка.уст(cnt - 1, "ЗАМЕТКИ", n);
	}
}

void Иср::SyncErrorsMessage()
{
	Ткст h;
	Ткст cnt;
	if(IsDarkMismatch()) {
		h = "Сообщение";
		if(error_count)
			cnt << error_count << " ошибка(и)";
		if(warning_count) {
			if(error_count)
				cnt << ", ";
			cnt << warning_count << " предупреждение(я)";
		}
	}
	else  {
		h = "\1[g Сообщение";
		if(error_count)
			cnt << "[*@r " << error_count << " ошибка" << (error_count > 1 ? "s]" : "]");
		if(warning_count) {
			if(error_count)
				cnt << ", ";
			cnt << "[@o " << warning_count << " предупреждение" << (warning_count > 1 ? "s]" : "]");
		}
	}
	if(cnt.дайСчёт())
		h << " (" << cnt << ")";
	Ошибка.HeaderTab(2).устТекст(h);
}

void Иср::ConsoleRunEnd()
{
	addnotes = false;
	prenotes.очисть();
}

void Иср::ShowFound()
{
	if(FFound().курсор_ли())
		GoToError(FFound());
}

Ткст Иср::GetErrorsText(bool all, bool src)
{
	int c = Ошибка.дайКурсор();
	if(!all && c < 0)
		return Null;
	Ткст txt;
	int h = all ? Ошибка.дайСчёт() : Ошибка.дайКурсор() + 1;
	for(int i = all ? 0 : c; i < h; i++) {
		if(src)
			txt << Ошибка.дай(i, 0) << " (" << Ошибка.дай(i, 1) << "): ";
		txt << Ошибка.дай(i, 2) << "\r\n";
	}
	return txt;
}

void Иср::CopyError(bool all)
{
	Ткст s = GetErrorsText(all, true);
	if(s.дайСчёт())
		WriteClipboardText(s);
}

void Иср::ErrorMenu(Бар& bar)
{
	bar.добавь(Ошибка.курсор_ли(), "Копировать", THISBACK1(CopyError, false));
	bar.добавь("Копировать все", THISBACK1(CopyError, true));
	bar.Separator();
	bar.добавь(Ошибка.курсор_ли(), "Искать в сети..", IdeImg::Google(), [=] {
		запустиВебБраузер("https://www.google.com/search?q=" + GetErrorsText(false, false));
	});
}

void Иср::SelError()
{
	if(removing_notes)
		return;
	if(Ошибка.курсор_ли()) {
		Значение v = Ошибка.дай("ЗАМЕТКИ");
		if(v != "0") {
			int sc = Ошибка.дайПромотку();
			removing_notes = true;
			for(int i = Ошибка.дайСчёт() - 1; i >= 0; i--)
				if(Ошибка.дай(i, "ЗАМЕТКИ") == "0")
					Ошибка.удали(i);
			removing_notes = false;
			Ошибка.ScrollTo(sc);
			Ошибка.ScrollIntoCursor();
			МассивЗнач n = v;
			int ii = Ошибка.дайКурсор();
			for(int i = 0; i < n.дайСчёт(); i++) {
				const ErrorInfo& f = ValueTo<ErrorInfo>(n[i]);
				Ошибка.вставь(++ii);
				Ошибка.уст(ii, 0, f.file);
				Ошибка.уст(ii, 1, f.lineno);
				int linecy;
				if(f.error_pos.дайСчёт()) {
					Ошибка.уст(ii, 2, FormatErrorLineEP(f.message, f.error_pos, linecy));
					Ошибка.устДисплей(ii, 2, Single<ElepDisplay>());
				}
				else
					Ошибка.уст(ii, 2, FormatErrorLine(f.message, linecy));
				Ошибка.уст(ii, "ИНФО", n[i]);
				Ошибка.уст(ii, "ЗАМЕТКИ", "0");
				Ошибка.SetLineCy(ii, linecy);
			}
		}
		GoToError(Ошибка);
	}
}

void Иср::ShowError()
{
	if(Ошибка.курсор_ли())
		GoToError(Ошибка);
}

void Иср::FoundDisplay::рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const
{
	Ткст s = q;
	if(*s == '\1') {
		Вектор<Ткст> h = разбей(~s + 1, '\1', false);
		if(h.дайСчёт() < 4)
			return;
		Один<EditorSyntax> es = EditorSyntax::создай(h[0]);
		es->IgnoreErrors();
		ШТкст ln = h[3].вШТкст();
		Вектор<СтрокРедакт::Highlight> hln;
		hln.устСчёт(ln.дайСчёт() + 1);
		for(int i = 0; i < ln.дайСчёт(); i++) {
			СтрокРедакт::Highlight& h = hln[i];
			h.paper = paper;
			h.ink = SColorText();
			h.chr = ln[i];
			h.font = StdFont();
		}
		HighlightOutput hl(hln);
		es->Highlight(ln.старт(), ln.стоп(), hl, NULL, 0, 0);
		int fcy = GetStdFontCy();
		int y = r.top + (r.дайВысоту() - fcy) / 2;
		w.DrawRect(r, paper);
		int sl = длинаУтф32(~h[3], atoi(h[1]));
		int sh = длинаУтф32(~h[3] + sl, atoi(h[2])) + sl;
		for(int text = 0; text < 2; text++) {
			int x = r.left;
			for(int i = 0; i < hln.дайСчёт(); i++) {
				Шрифт fnt = StdFont();
				int a = fnt.GetAscent();
				СтрокРедакт::Highlight& h = hln[i];
				fnt.Bold(h.font.IsBold());
				fnt.Italic(h.font.IsItalic());
				fnt.Underline(h.font.IsUnderline());
				a -= fnt.GetAscent();
				int cw = fnt[h.chr];
				if(h.chr == '\t')
					cw = 4 * fnt[' '];
				Цвет hpaper = HighlightSetup::GetHlStyle(HighlightSetup::PAPER_SELWORD).color;
				Цвет hink = h.ink;
				if(IsDarkMismatch()) {
					hpaper = paper;
					hink = ink;
				}
				if(i >= sl && i < sh && !(style & (CURSOR|SELECT|READONLY)) && !text)
					w.DrawRect(x, y, cw, fcy, hpaper);
				if(h.chr != '\t' && text)
					w.DrawText(x, y + a, &h.chr, fnt, hink, 1);
				x += cw;
			}
		}
	}
	else
		StdDisplay().рисуй(w, r, q, ink, paper, style);
}

void Иср::FoundFileDisplay::рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const
{
	w.DrawRect(r, paper);
	DrawFileName(w, r, q, ink);
}

Размер Иср::FoundFileDisplay::дайСтдРазм(const Значение& q) const
{
	return GetDrawFileNameSize(q);
}
