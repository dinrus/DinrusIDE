#include "DinrusIDE.h"

Ткст ViewCache()
{
	return конфигФайл("view_maps");
}

Ткст ViewFileHash(const Ткст& path)
{
	Sha1Stream s;
	ФайлПоиск ff(path);
	if(ff) {
		Sha1Stream sha;
		sha << path << ';' << Время(ff.дайВремяПоследнЗаписи()) << ';' << ff.дайДлину();
		return приставьИмяф(ViewCache(), sha.FinishString());
	}
	return Null;
}

void ViewFile(СтрокРедакт& edit, Поток& view_file, const Ткст& path, byte charset)
{
	edit.View(view_file, charset);
	Ткст f = ViewFileHash(path);
	if(f.дайСчёт())
		грузиИзФайла([&](Поток& s) { edit.SerializeViewMap(s); }, f);
}

void CacheViewFile(СтрокРедакт& edit, const Ткст& path)
{
	if(edit.IsView()) {
		ReduceCacheFolder(ViewCache(), 20000000);
		Ткст f = ViewFileHash(path);
		RealizePath(f);
		if(f.дайСчёт())
			сохраниВФайл([&](Поток& s) { edit.SerializeViewMap(s); }, f);
	}
}

void Иср::SetupEditor(int f, Ткст hl, Ткст path)
{
	if(пусто_ли(hl)) {
		hl = EditorSyntax::GetSyntaxForFilename(path);
		if(пусто_ли(hl))
			hl = EditorSyntax::GetSyntaxForFilename(впроп(path));
		if(пусто_ли(hl) && пусто_ли(дайРасшф(path))) {
			ФайлВвод in(path);
			Ткст h = in.дай(4096);
			try {
				СиПарсер p(h);
				while(!p.кф_ли()) {
					if(p.сим('#')) {
						if(p.ид("define") || p.ид("ipathdef") || p.ид("ifdef") || p.ид("include") || p.ид("pragma")) {
							hl = "cpp";
							break;
						}
					}
					p.пропустиТерм();
				}
			}
			catch(СиПарсер::Ошибка) {}
		}
	}
	switch(f) {
	case 1:  editor.устШрифт(font1); break;
	case 2:  editor.устШрифт(font2); break;
	case 3:  editor.устШрифт(consolefont); break;
	default: editor.устШрифт(editorsplit.GetZoom() < 0 && editorsplit.горизонтален() ? veditorfont :
	                        hl == "t" ? tfont : editorfont); break;
	}
	editor.Highlight(hl);

	editor.WarnWhiteSpace(warnwhitespace &&
	                      findarg(hl, "cpp", "java", "js", "cs", "json", "css", "lay", "sch", "t", "usc") >= 0);

	editor.WordwrapComments(wordwrap_comments);
	editor.WordWrap(wordwrap);
}

void Иср::SetupEditor()
{
	if(!IsActiveFile())
		return;
	Пакет::Файл& f = ActiveFile();
	Ткст p = GetActiveFileName();
	if(p != HELPNAME)
		p = GetActiveFilePath();
	SetupEditor(f.font, f.highlight, p);
	editor.SyncNavigatorPlacement();
}

void Иср::FileCursor()
{
	WorkspaceWork::FileCursor();
	if(IsActiveFile() && !filelist[filelist.дайКурсор()].isdir) {
		Пакет::Файл& f = ActiveFile();
		editor.устРедактируем(!f.readonly);
		editor.TabSize(f.tabsize > 0 ? f.tabsize : actual.tabsize > 0 ? actual.tabsize : editortabsize);
		SetupEditor();
		Ткст headername;
		if(insert_include)
			for(int i = insert_include == 1 ? 0 : filelist.дайКурсор();
			    i >= 0 && i < filelist.дайСчёт();
			    i += insert_include == 1 ? 1 : -1) {
					Ткст nm = FileName(i);
					if(!IsSeparator(i) && дайРасшф(nm) == ".h") {
						headername = nm;
						break;
					}
			}
		Ткст p = GetActiveFileName();
		if(p != HELPNAME)
			p = GetActiveFilePath();
		EditFile0(p, f.charset ? f.charset : actual.charset ? actual.charset : default_charset,
		          Nvl(f.spellcheck_comments, actual.spellcheck_comments, spellcheck_comments),
		          headername);
	}
}

bool Иср::HasFileData(const Ткст& file)
{
	return filedata.найди(нормализуйПуть(file)) >= 0;
}

Иср::FileData& Иср::Filedata(const Ткст& file) {
	return filedata.дайДобавь(нормализуйПуть(file));
}

void DlCharsetD(СписокБроса& d)
{
	d.добавь(0, "Дефолт");
	DlCharset(d);
}

void Иср::ChangeFileCharset(const Ткст& имя, Пакет& p, byte charset)
{
	if(пусто_ли(имя))
		return;
	bool sv = false;
	for(int i = 0; i < p.file.дайСчёт(); i++)
		if(SourcePath(имя, p.file[i]) == editfile && p.file[i].charset != charset) {
			p.file[i].charset = charset;
			sv = true;
		}
	if(sv) {
		Ткст pp = PackagePathA(имя);
		p.сохрани(pp);
	}
}

void Иср::FileProperties()
{
	if(!IsActiveFile()) return;
	WithFileFormatLayout<ТопОкно> d;
	CtrlLayoutOKCancel(d, "Свойства файла");
	Пакет::Файл& f = ActiveFile();
	DlCharsetD(d.charset);
	d.font.добавь(0, "Нормальный");
	d.font.добавь(1, "Маленький");
	d.font.добавь(2, "Особый");
	d.font.добавь(3, "Консоль");
	d.highlight.добавь(Null, "Дефолт");
	for(int i = 0; i < EditorSyntax::GetSyntaxCount(); i++) {
		Ткст desc = EditorSyntax::GetSyntaxDescription(i);
		if(desc.дайСчёт())
			d.highlight.добавь(EditorSyntax::дайСинтакс(i), desc);
	}
	d.highlight.добавь("none", "Никакого");
	d.tabsize <<= f.tabsize > 0 ? f.tabsize : Null;
	d.tabsize <<= d.Breaker(111);
	d.tabsize.минмакс(1, 100);
	d.charset <<= (int)f.charset;
	d.font <<= f.font;
	d.font <<= d.Breaker(111);
	d.highlight <<= f.highlight;
	d.highlight <<= d.Breaker(111);
	d.line_endings.добавь(CRLF, "CRLF");
	d.line_endings.добавь(LF, "LF");
	d.line_endings <<= findarg(Nvl(editfile_line_endings, line_endings), LF, DETECT_LF) >= 0 ? LF : CRLF;
	d.line_endings.вкл(findarg(line_endings, DETECT_CRLF, DETECT_LF) >= 0);
	d.spellcheck_comments.добавь(Null, "Дефолт");
	DlSpellerLangs(d.spellcheck_comments);
	d.spellcheck_comments <<= f.spellcheck_comments;
	for(;;) {
		switch(d.пуск()) {
		case IDCANCEL:
			FlushFile();
			FileCursor();
			return;
		case IDOK:
			int c = filelist.дайКурсор();
			FlushFile();
			f.charset = (byte)(int)~d.charset;
			f.tabsize = Nvl((int)~d.tabsize);
			f.font = Nvl((int)~d.font);
			f.highlight = ~d.highlight;
			f.spellcheck_comments = ~d.spellcheck_comments;
			SavePackage();
			PackageCursor();
			filelist.устКурсор(c);
			editor.ClearUndo();
			if(editfile_line_endings != ~d.line_endings) {
				editfile_line_endings = ~d.line_endings;
				сохраниФайл(true);
			}
			MakeTitle();
			return;
		}
		if(!пусто_ли(editfile)) {
			int ts = Nvl((int)~d.tabsize);
			editor.TabSize(ts ? ts : editortabsize);
			SetupEditor(Nvl((int)~d.font), ~d.highlight, editfile);
		}
	}
}

void Иср::ChangeCharset()
{
	if(!IsActiveFile()) return;
	Пакет::Файл& f = ActiveFile();
	сохраниФайл();
	WithCharsetLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, "Сохранить файл с кодировкой");
	DlCharsetD(dlg.charset);
	dlg.charset <<= editor.дайНабсим();
	dlg.ActiveFocus(dlg.charset);
	byte cs;
	for(;;) {
		if(dlg.пуск() != IDOK)
			return;
		cs = (byte)(int)~dlg.charset;
		int q = editor.GetInvalidCharPos(cs);
		if(q >= 0) {
			if(PromptYesNo("Файл содержит символы, которые нельзя сохронить в текущей кодировке.&"
			               "Сохранить всё равно? (Если выберить Да, они будут заменены вопросительными знаками.)"))
								break;
			editor.устКурсор(q);
			return;
		}
		else
			break;
	}
	f.charset = cs;
	SavePackage();
	editor.устНабсим(f.charset);
	сохраниФайл(true);
	FlushFile();
	FileCursor();
	editor.ClearUndo();
}

void Иср::PosSync()
{
	if(designer || editfile.пустой())
		return;
	for(int i = 0; i < 2; i++)
		if(путиРавны(posfile[i], editfile))
			editor.устУк(posline[i], posimg[i], i);
		else
			editor.устУк(-1, Рисунок(), i);
	editor.SyncTip();
}

bool Иср::IsProjectFile(const Ткст& f) const
{
	const РОбласть& wspc = роблИср();
	for(int i = 0; i < wspc.дайСчёт(); i++) {
		const Пакет& pk = wspc.дайПакет(i);
		Ткст n = wspc[i];
		for(int i = 0; i < pk.file.дайСчёт(); i++) {
			Ткст фн = pk.file[i];
			Ткст path = SourcePath(n, фн);
			if(f == path)
				return true;
		}
	}
	return false;
}

Ткст ConvertTLine(const Ткст& line, int flag)
{
	Ткст r;
	const char *s = line;
	
	while(*s) {
		if(*s == '\"') {
			СиПарсер p(s);
			p.безПропускаПробелов();
			try {
				r.конкат(какТкстСи(p.читай1Ткст(), INT_MAX, NULL, flag));
			}
			catch(СиПарсер::Ошибка) {
				return line;
			}
			s = p.дайУк();
		}
		else
			r.конкат(*s++);
	}
	return r;
}

void Иср::сохраниФайл(bool always)
{
	issaving++;
	SaveFile0(always);
	issaving--;
}

int64 Иср::EditorHash()
{
	ххХэш64Поток ss;
	for(int i = 0; i < editor.дайСчётСтрок(); i++) {
		ss.помести(editor.дайУтф8Строку(i));
		ss.помести("\n");
	}
	return ss.финиш();
}

void Иср::SaveEditorFile(Поток& out)
{
	if(дайРасшф(editfile) == ".t") {
		for(int i = 0; i < editor.дайСчётСтрок(); i++) {
			if(i) out.PutCrLf();
			out.помести(ConvertTLine(editor.дайУтф8Строку(i), ASCSTRING_OCTALHI));
		}
	}
	else {
		int le = line_endings;
		if(le == DETECT_CRLF)
			le = Nvl(editfile_line_endings, CRLF);
		if(le == DETECT_LF)
			le = Nvl(editfile_line_endings, LF);
		editor.сохрани(out, editor.дайНабсим(), le == LF ? ТекстКтрл::LE_LF : ТекстКтрл::LE_CRLF);
	}
}

void Иср::SaveFile0(bool always)
{
	if(designer) {
		Ткст фн = designer->дайИмяф();
		Время tm = дайВремяф(фн);
		designer->сохрани();
		if(tm != дайВремяф(фн))
			TouchFile(фн);
		if(IsProjectFile(фн) && взаг(дайРасшф(фн)) == ".LAY")
			CodeBaseScanFile(фн, auto_check);
		return;
	}

	if(editfile.пустой())
		return;

	FileData& fd = Filedata(editfile);
	fd.lineinfo = editor.GetLineInfo();
	fd.lineinforem = editor.GetLineInfoRem();
	fd.editpos = editor.GetEditPos();
	fd.columnline = editor.GetColumnLine(fd.editpos.cursor);
	fd.filetime = edittime;
	if(editor.IsView())
		return;
	if(!editor.IsDirty() && !always)
		return;
	TouchFile(editfile);
	if(!файлЕсть(editfile))
		InvalidateIncludes();
	for(;;) {
		ФайлВывод out(editfile);
		SaveEditorFile(out);
		if(!out.ошибка_ли())
			break;
		int art = Prompt(Ктрл::дайИмяПрил(), CtrlImg::exclamation(),
			"Не удаётся сохранить текущий файл.&"
			"Повторить сохранение, проигнорировать или сохранить в другом месте?",
			"Сохранить как...", "Повторить", "Игнорировать");
		if(art < 0)
			break;
		if(IsDeactivationSave())
			return;
		if(art && AnySourceFs().ExecuteSaveAs()) {
			editfile = AnySourceFs();
			MakeTitle();
			PosSync();
			break;
		}
	}

	ФайлПоиск ff(editfile);
	fd.filetime = edittime = ff.дайВремяПоследнЗаписи();

	if(editor.IsDirty()) {
		text_updated.глуши();
		if(IsCppBaseFile())
			CodeBaseScanFile(editfile, auto_check);
	}

	editor.ClearDirty();

	if(впроп(дайРасшф(editfile)) == ".usc")
		SyncUsc();

	MakeTitle();
}

void Иср::FlushFile() {
	editor.CloseAssist();
	сохраниФайл();
	CacheViewFile(editor, editfile);
	editor.assist_active = false;
	if(designer) {
		designer->SaveEditPos();
		designer->DesignerCtrl().устФрейм(фреймПусто());
		if(dynamic_cast<TopicEditor *>(&designer->DesignerCtrl()))
			RefreshBrowser();
	}
	else
	if(!editfile.пустой() && !editor.IsView()) {
		FileData& fd = Filedata(editfile);
		fd.undodata = editor.PickUndoData();
		fd.filehash = EditorHash();
	}
	editfile.очисть();
	editfile_repo = NOT_REPO_DIR;
	editfile_isfolder = false;
	repo_dirs = RepoDirs(true).дайСчёт(); // Perhaps not the best place, but should be ok
	editor.очисть();
	editor.откл();
	editor.устРедактируем();
	editorsplit.Ктрл::удали();
	designer.очисть();
	view_file.закрой();
	SetBar();
}

int CharFilterMacro(int c)
{
	return iscid(c) ? c : '_';
}

void Иср::переименуйФайл(const Ткст& nm)
{
	tabs.RenameFile(editfile, nm);
	InvalidateIncludes();
}

bool Иср::удалиФайл()
{
	InvalidateIncludes();
	int q = найдиИндекс(tablru, editfile);
	if(q >= 0)
		tablru.удали(q);
	q = tabs.дайКурсор();
	if(q >= 0) {
		tabs.закройСилой(q, false);
		if(filelist.дайСчёт())
			return true;
		if(tabs.дайСчёт())
			TabFile();
		else {
			tabs.освежи();
			FlushFile();
		}
		return false;
	}
	return true;
}

void Иср::EditFile0(const Ткст& path, byte charset, int spellcheck_comments, const Ткст& headername)
{
	text_updated.глуши();

	AKEditor();
	editor.CheckEdited(false);
	editor.CloseAssist();
	if(path.пустой()) return;
	FlushFile();

	editfile = path;
	editor.устНабсим(charset);
	editor.SpellcheckComments(spellcheck_comments);
	AddLru();

	editfile_isfolder = папка_ли(editfile) || IsHelpName(editfile);
	repo_dirs = RepoDirs(true).дайСчёт(); // Perhaps not the best place, but should be ok
	
	bool candesigner = !(debugger && !editfile_isfolder && (путиРавны(path, posfile[0]) || путиРавны(path, posfile[0])))
	   && editastext.найди(path) < 0 && editashex.найди(path) < 0 && !IsNestReadOnly(editfile);
	
	if(candesigner) {
		for(int i = 0; i < дайСчётМодульИСР() && !designer; i++)
			designer = дайМодульИСР(i).CreateDesigner(this, path, charset);
	}

	if(!designer && editastext.найди(path) < 0 &&
	   (findarg(дайРасшф(path), ".log") < 0 &&
	    findarg(charset, CHARSET_UTF8_BOM, CHARSET_UTF16_LE, CHARSET_UTF16_BE,
	            CHARSET_UTF16_LE_BOM, CHARSET_UTF16_BE_BOM) < 0 &&
	    FileIsBinary(path) || editashex.найди(path) >= 0))
		designer.создай<FileHexView>().открой(path);
	
	ManageDisplayVisibility();
	
	if(designer) {
		editpane.добавь(designer->DesignerCtrl().SizePos());
		designer->DesignerCtrl().устФокус();
		designer->RestoreEditPos();
		if(filetabs)
			tabs.SetAddFile(editfile);
		MakeTitle();
		SetBar();
		editor.SyncNavigatorShow();
		return;
	}

	tabs.SetAddFile(editfile);
	tabs.SetSplitColor(editfile2, жёлтый);
	editor.вкл();
	editpane.добавь(editorsplit);
	editor.HiliteScope(hilite_scope);
	editor.OverWriteMode(false);
	ActiveFocus(editor);
	FileData& fd = Filedata(editfile);
	ФайлПоиск ff(editfile);
	bool tfile = дайРасшф(editfile) == ".t";
	if(ff) {
		edittime = ff.дайВремяПоследнЗаписи();
		if(edittime != fd.filetime || пусто_ли(fd.filetime))
			fd.undodata.очисть();
		view_file.устРазмБуф(256*1024);
		view_file.открой(editfile);
		if(view_file) {
			if(tfile && editastext.найди(editfile) < 0) {
				Ткст f;
				Ткст ln;
				for(;;) {
					int c = view_file.дай();
					if(c < 0) {
						f.конкат(ConvertTLine(ln, 0));
						break;
					}
					if(c != '\r') {
						ln.конкат(c);
						if(c == '\n') {
							f.конкат(ConvertTLine(ln, 0));
							ln.очисть();
						}
					}
				}
				editor.уст(f);
				editor.устНабсим(CHARSET_UTF8);
			}
			else {
				Ткст s = view_file.дай(3);
				if(s.дайСчёт() >= 2) {
					if((byte)s[0] == 0xff && (byte)s[1] == 0xfe)
						charset = CHARSET_UTF16_LE_BOM;
					if((byte)s[0] == 0xfe && (byte)s[1] == 0xff)
						charset = CHARSET_UTF16_BE_BOM;
				}
				if(s.дайСчёт() >= 3 && (byte)s[0] == 0xef && (byte)s[1] == 0xbb && (byte)s[2] == 0xbf)
					charset = CHARSET_UTF8_BOM;
				view_file.перейди(0);
				int le = Null;
			#ifdef CPU_64
				const int64 max_size = (int64)4096*1024*1024;
			#else
				const int64 max_size = 768*1024*1024;
			#endif
				const int view_limit = 256*1024*1024;
				if(view_file.дайРазм() < view_limit || editastext.найди(editfile) >= 0 && view_file.дайРазм() < max_size) {
					le = editor.грузи(view_file, charset);
					view_file.закрой();
				}
				else
					ViewFile(editor, view_file, editfile, charset);
				
				editfile_line_endings = le == ТекстКтрл::LE_CRLF ? CRLF : le == ТекстКтрл::LE_LF ? LF : (int)Null;
			}
		}
		else
			Exclamation("Не удалось прочесть файл.");
		editor.SetEditPos(fd.editpos);
		if(!пусто_ли(fd.columnline) && fd.columnline.y >= 0 && fd.columnline.y < editor.дайСчётСтрок())
			editor.устКурсор(editor.GetColumnLinePos(fd.columnline));
		editor.SetEditPosSbOnly(fd.editpos);
		if(!editor.IsView()) {
			if(EditorHash() != fd.filehash)
				fd.undodata.очисть();
			editor.SetPickUndoData(pick(fd.undodata));
			editor.SetLineInfo(fd.lineinfo);
			editor.SetLineInfoRem(pick(fd.lineinforem));
		}
		if(ff.толькочтен_ли() || IsNestReadOnly(editfile) || editor.IsTruncated() || editor.IsView()) {
			editor.устТолькоЧтен();
			editor.NoShowReadOnly();
		}
		fd.ClearP();
		PosSync();
		editor.ClearDirty();
	}
	else {
		RealizePath(editfile);
		if(дайРасшф(editfile) == ".h") {
			Ткст n = '_' + фильтруй(Ткст().конкат() << GetActivePackage() << '_' << дайТитулф(editfile) << "_h_",
			                        CharFilterMacro);
			Ткст s;
			s << "#ifndef " << n << "\r\n";
			s << "#define " << n << "\r\n";
			s << "\r\n";
			s << "#endif\r\n";
			editor <<= s;
			editor.устКурсор(editor.дайПоз64(2));
		}
		if(IsCSourceFile(editfile) && !пусто_ли(headername)) {
			editor <<= "#include \"" + headername + "\"\r\n";
			editor.устКурсор(editor.дайПоз64(1));
		}
		editor.устНабсим(tfile ? CHARSET_UTF8 : charset);
	}
	editor.устФокус();
	MakeTitle();
	SetBar();
	editor.SyncNavigatorShow();
	editor.assist_active = IsProjectFile(editfile) && IsCppBaseFile();
	editor.CheckEdited(true);
	editor.Annotate(editfile);
	editor.SyncNavigator();
	editfile_repo = GetRepoKind(editfile);
	editfile_includes = IncludesMD5();
}

Ткст Иср::IncludesMD5()
{ // keep track of includes for Assist++, so that we know when to save file and sync base
	int n = min(editor.дайСчётСтрок(), 1000); // ignore big files
	Md5Stream md5;
	for(int i = 0; i < n; i++) {
		Ткст l = editor.дайУтф8Строку(i);
		try {
			СиПарсер p(l);
			if(p.сим('#') && p.ид("include"))
				md5.помести(l);
		}
		catch(СиПарсер::Ошибка) {}
	}
	return md5.FinishString();
}

void Иср::ScanFile(bool check_includes)
{
	if(IsCppBaseFile()) {
		if(check_includes) {
			Ткст imd5 = IncludesMD5();
			if(editfile_includes != imd5) {
				editfile_includes = imd5;
				сохраниФайл(true);
				return;
			}
		}
		Ткст s = ~editor;
		ТкстПоток ss(s);
		CodeBaseScanFile(ss, editfile);
	}
}

bool Иср::EditFileAssistSync2()
{
	if(TryLockCodeBase()) {
		editor.Annotate(editfile);
		editor.SyncNavigator();
		UnlockCodeBase();
		return true;
	}
	return false;
}

void Иср::EditFileAssistSync()
{
	ScanFile(false);
	EditFileAssistSync2();
}

void Иср::TriggerAssistSync()
{
	if(auto_rescan && editor.дайДлину64() < 500000 && !file_scan) {
		text_updated.глушиУст(1000, [=] {
			if(!file_scan && IsCppBaseFile()) {
				Ткст s = ~editor;
				Ткст фн = editfile;
				file_scan++;
				if(Ктрл::GetСобытиеLevel() == 0 && !СоРабота::пробуйПлан([=] {
					ТкстПоток ss(s);
					file_scanned = TryCodeBaseScanFile(ss, editfile);
					file_scan--;
					if(!file_scanned)
						trigger_assist.глушиУст(100, [=] { TriggerAssistSync(); });
				})) {
					file_scan--;
					trigger_assist.глушиУст(100, [=] { TriggerAssistSync(); });
				}
			}
		});
	}
}

void Иср::EditAsHex()
{
	Ткст path = editfile;
	if(editashex.найди(path) >= 0)
		return;
	editastext.удалиКлюч(editfile);
	editashex.найдиПомести(editfile);
	byte cs = editor.дайНабсим();
	int sc = editor.GetSpellcheckComments();
	FlushFile();
	EditFile0(path, cs, sc);
}

bool Иср::IsDesignerFile(const Ткст& path)
{
	for(int i = 0; i < дайСчётМодульИСР(); i++)
		if(дайМодульИСР(i).AcceptsFile(path))
			return true;
	return false;
}

void Иср::DoEditAsText(const Ткст& path)
{
	editastext.найдиДобавь(path);
	editashex.удалиКлюч(editfile);
}

void Иср::EditAsText()
{
	Ткст path = editfile;
	if(editastext.найди(path) >= 0)
		return;
//	if(!файлЕсть(path))
//		return;
	Ткст layout;
	if(auto *l = dynamic_cast<LayDesigner *>(~designer))
		layout = l->GetCurrentLayout();
	DoEditAsText(path);
	byte cs = editor.дайНабсим();
	int sc = editor.GetSpellcheckComments();
	FlushFile();
	EditFile0(path, cs, sc);
	if(layout.дайСчёт()) {
		layout = "LAYOUT(" + layout + ",";
		for(int i = 0; i < editor.дайСчётСтрок(); i++)
			if(editor.дайУтф8Строку(i).начинаетсяС(layout)) {
				editor.GotoLine(i);
				break;
			}
	}
}

void Иср::EditUsingDesigner()
{
	Ткст path = editfile;
	if (editastext.найди(editfile) < 0 && editashex.найди(editfile) < 0)
		return;
	editashex.удалиКлюч(editfile);
	editastext.удалиКлюч(editfile);
	byte cs = editor.дайНабсим();
	int sc = editor.GetSpellcheckComments();
	FlushFile();
	EditFile0(path, cs, sc);
}

void Иср::AddEditFile(const Ткст& path)
{
	actual.file.добавьПодбор(Пакет::Файл(path));
	if(IsAux())
		SaveLoadPackageNS(false);
	else
		SaveLoadPackage(false);
	ShowFile(package.дайСчёт() - 1);
	filelist.устКурсор(filelist.дайСчёт() - 1);
}

void Иср::EditFile(const Ткст& p)
{
	if(p.пустой()) {
		FlushFile();
		return;
	}
	if(p == HELPNAME) {
		if(designer && designer->дайИмяф() == p)
			return;
		package.FindSetCursor(METAPACKAGE);
		filelist.FindSetCursor(HELPNAME);
		return;
	}

	Ткст path = нормализуйПуть(p);
	if(designer ? path == designer->дайИмяф() : path == editfile)
		return;

	FlushFile();
	if(path.пустой())
		return;
	
	for(int i = 0; i < package.дайСчёт(); i++) {
		Ткст pkg = package[i].имя;
		Пакет p;
		Fetch(p, pkg);
		for(int j = 0; j < p.file.дайСчёт(); j++)
			if(путиРавны(SourcePath(pkg, p.file[j]), path)) {
				package.FindSetCursor(pkg);
				ShowFile(j);
				filelist.FindSetCursor(p.file[j]);
				return;
			}
		if(дайРасшф(path) == ".tpp" && путиРавны(SourcePath(pkg, дайИмяф(path)), path)) {
			filelist.анулируйКурсор();
			package.анулируйКурсор();
			package.устКурсор(i);
			AddEditFile(дайИмяф(path));
			return;
		}
	}
	filelist.анулируйКурсор();
	if(!IsAux() || IsMeta()) {
		package.анулируйКурсор();
		package.устКурсор(package.дайСчёт() - 2); // use temp-aux
	}
	AddEditFile(path);
}

bool Иср::IsCppBaseFile()
{
	return IsProjectFile(editfile) && (IsCSourceFile(editfile) || IsCHeaderFile(editfile) ||
	                                   взаг(дайРасшф(editfile)) == ".SCH");
}

void Иср::CheckFileUpdate()
{
	if(editfile.пустой() || !пп_ли() || designer) return;
	ФайлПоиск ff(editfile);
	if(!ff) return;
	ФВремя tm = ff.дайВремяПоследнЗаписи();
	if(tm == edittime) return;
	edittime = tm;
	if(editor.IsDirty() && !Prompt(Ктрл::дайИмяПрил(), CtrlImg::exclamation(),
		"Текущий файл изменён вне ИСР, но редактирован и в ней.&"
		"Перезагрузить файл или оставить изменения, сделанные в ИСР ?",
		"Перезагрузить", "Оставить")) return;

	if(IsCppBaseFile())
		CodeBaseScanFile(editfile, auto_check);
	ReloadFile();
}

typedef Индекс<dword> HashBase;

static void GetLineIndex(Ткст file, HashBase& hash, Вектор<Ткст>& lines)
{
	const char *p = file;
	while(*p)
	{
		while(*p && *p != '\n' && (byte)*p <= ' ')
			p++;
		const char *b = p;
		while(*p && *p++ != '\n')
			;
		const char *e = p;
		while(e > b && (byte)e[-1] <= ' ')
			e--;
		Ткст s(b, e);
		hash.добавь(FoldHash(дайХэшЗнач(s)));
		lines.добавь(s);
	}
}

int LocateLine(Ткст old_file, int old_line, Ткст new_file)
{
	HashBase old_hash, new_hash;
	Вектор<Ткст> old_lines, new_lines;
	GetLineIndex(old_file, old_hash, old_lines);
	GetLineIndex(new_file, new_hash, new_lines);
	if(old_line <= 0)
		return 0;
	if(old_line >= old_lines.дайСчёт())
		return new_lines.дайСчёт();
	Ткст line = old_lines[old_line];
	//int hash = old_hash[old_line]; Mirek: unused
	//int fore_count = old_lines.дайСчёт() - old_line - 1;
	int best_match = 0, best_value = 0;
	for(int r = 0; r < 10 && !best_value; r++)
	{
		int src = (r & 1 ? old_line + (r >> 1) + 1 : old_line - (r >> 1));
		if(src < 0 || src >= old_lines.дайСчёт())
			continue;
		dword hash = old_hash[src];
		for(int i = new_hash.найди(hash); i >= 0; i = new_hash.найдиСледщ(i))
			if(new_lines[i] == old_lines[src])
			{
				int max_back = min(i, src);
				int max_fore = min(new_lines.дайСчёт() - i, old_lines.дайСчёт() - src) - 1;
				if(max_back + max_fore <= best_value)
					continue;
				int back = 1;
				while(back <= max_back && new_hash[i - back] == old_hash[src - back]
					&& new_lines[i - back] == old_lines[src - back])
					back++;
				int fore = 1;
				while(fore < max_fore && new_hash[i + fore] == old_hash[src + fore]
					&& new_lines[i + fore] == old_lines[src + fore])
					fore++;
				if(back + fore > best_value)
				{
					best_value = back + fore;
					best_match = minmax(i, 0, new_lines.дайСчёт());
				}
			}
	}
	return best_match;
}

void Иср::ReloadFile()
{
	if(editfile.пустой())
		return;
	Ткст фн = editfile;
	Ткст data = ~editor;
	int ln = editor.GetCursorLine();
	editfile.очисть();
	int sc = filelist.GetSbPos();
	EditFile0(фн, editor.дайНабсим(), editor.GetSpellcheckComments());
	filelist.SetSbPos(sc);
	int l = LocateLine(data, ln, ~editor);
	editor.устКурсор(editor.дайПоз64(l));
}

void Иср::EditAnyFile() {
	FileSel& fs = AnySourceFs();
#if 0
	fs.Multi(false);
	if(!fs.ExecuteOpen()) return;
	EditFile(fs);
	FileSelected();
#endif
	if(fs.ExecuteOpen())
		for(int i = 0; i < fs.дайСчёт(); i++) {
			EditFile(fs[i]);
			FileSelected();
		}
}

void Иср::тягИБрос(Точка, PasteClip& d)
{
	if(AcceptFiles(d)) {
		Вектор<Ткст> f = GetFiles(d);
		for(int i = 0; i < f.дайСчёт(); i++)
			if(файлЕсть(f[i])) {
				EditFile(f[i]);
				FileSelected();
				editor.устФокус();
			}
	}
}

void Иср::AddLru()
{
	if(editfile.пустой() || tabi) return;
	LruAdd(tablru, editfile, 200);
}

static Ткст sExFiles(const char *фн, const char **ext, int cnt)
{
	for(int i = 0; i < cnt; i++) {
		Ткст f = форсируйРасш(фн, ext[i]);
		if(файлЕсть(f))
			return f;
	}
	return Null;
}

Ткст Иср::GetOpposite()
{
	static const char *cpp[] = { ".c", ".cpp", ".cc", ".cxx" };
	static const char *hdr[] = { ".h", ".hpp", ".hh", ".hxx" };
	if(пусто_ли(editfile) || designer)
		return Null;
	Ткст ext = дайРасшф(editfile);
	for(int i = 0; i < __countof(cpp); i++)
		if(ext == cpp[i])
			return sExFiles(editfile, hdr, __countof(hdr));
	for(int i = 0; i < __countof(hdr); i++)
		if(ext == hdr[i])
			return sExFiles(editfile, cpp, __countof(cpp));
	return Null;
}

void Иср::GoOpposite()
{
	Ткст фн = GetOpposite();
	if(!пусто_ли(фн))
		EditFile(фн);
}

void Иср::PassEditor()
{
	editorsplit.NoZoom();
	SyncEditorSplit();
	SetupEditor();
	editfile2 = editfile;
	editor2.устШрифт(editor.дайШрифт());
	editor2.Highlight(editor.дайПодсвет());
	editor2.LoadHlStyles(editor.StoreHlStyles());
	editor2.NoShowReadOnly();
	byte charset = editor.дайНабсим();
	editor2.CheckEdited(false);
	view_file2.закрой();
	if(editor.IsView()) {
		view_file2.открой(editfile2);
		ViewFile(editor2, view_file2, editfile2, charset);
	}
	else
		editor2.уст(editor.дай(charset), charset);
	editor2.SetEditPosSb(editor.GetEditPos());
	editor2.CheckEdited();
	editor.устФокус();
	editor.ScrollIntoCursor();
	editor2.Annotate(editfile2);
	editor2.SpellcheckComments(editor.GetSpellcheckComments());
}

void Иср::ClearEditedFile()
{
	editor.ClearEdited();
}

void Иср::ClearEditedAll()
{
	ClearEditedFile();
	for(int i = 0; i < filedata.дайСчёт(); i++) {
		LineInfo li = editor.GetLineInfo();
		LineInfoRem lir = editor.GetLineInfoRem();
		FileData& fd = Filedata(filedata.дайКлюч(i));
		editor.SetLineInfo(fd.lineinfo);
		editor.SetLineInfoRem(pick(fd.lineinforem));
		ClearEditedFile();
		fd.lineinfo = editor.GetLineInfo();
		fd.lineinforem = editor.GetLineInfoRem();
		editor.SetLineInfo(li);
	}
}

void Иср::SplitEditor(bool horz)
{
	if(editorsplit.GetZoom() < 0)
		CloseSplit();

	if(horz)
		editorsplit.гориз(editor2, editor);
	else
		editorsplit.верт(editor2, editor);
	
	tabs.SetSplitColor(editfile, жёлтый);
	PassEditor();
}

void Иср::SwapEditors()
{
	Ткст f = editfile2;
	РедакторКода::ПозРедакт p = editor2.GetEditPos();
	if(editorsplit.дайПервОтпрыск() == &editor)
		if(editorsplit.вертикален())
			editorsplit.верт(editor2, editor);
		else
			editorsplit.гориз(editor2, editor);
	else
		if(editorsplit.вертикален())
			editorsplit.верт(editor, editor2);
		else
			editorsplit.гориз(editor, editor2);
	PassEditor();
	EditFile(f);
	editor.SetEditPos(p);
}

void Иср::CloseSplit()
{
	editorsplit.верт(editor, editor2);
	editorsplit.Zoom(0);
	view_file2.закрой();
	editfile2.очисть();
	tabs.ClearSplitColor();
	SyncEditorSplit();
	editor.устФокус();
	SetupEditor();
}

void Иср::KeySplit(bool horz)
{
	if(editorsplit.GetZoom() >= 0)
		SplitEditor(horz);
	else
		CloseSplit();
}

void Иср::SyncEditorSplit()
{
	editor.topsbbutton.ScrollStyle().SetMonoImage(IdeImg::split()).Подсказка("Разделить (Ктрл+[-])");
	editor.topsbbutton <<= THISBACK1(SplitEditor, false);
	editor.topsbbutton1.ScrollStyle().SetMonoImage(IdeImg::vsplit()).Подсказка("Разделить (Ктрл+[\\])");
	editor.topsbbutton1 <<= THISBACK1(SplitEditor, true);
	editor2.topsbbutton.ScrollStyle().SetMonoImage(IdeImg::split()).Подсказка("Разделить (Ктрл+[-])");
	editor2.topsbbutton <<= THISBACK1(SplitEditor, false);
	editor2.topsbbutton1.ScrollStyle().SetMonoImage(IdeImg::vsplit()).Подсказка("Разделить (Ктрл+[\\])");
	editor2.topsbbutton1 <<= THISBACK1(SplitEditor, true);
	if(editorsplit.GetZoom() >= 0)
		return;
	if(editorsplit.вертикален()) {
		editor.topsbbutton.ScrollStyle().SetMonoImage(IdeImg::closesplit()).Подсказка("Закрыть (Ктрл+[-])");
		editor.topsbbutton <<= THISBACK(CloseSplit);
		editor2.topsbbutton.ScrollStyle().SetMonoImage(IdeImg::closesplit()).Подсказка("Закрыть (Ктрл+[-])");
		editor2.topsbbutton <<= THISBACK(CloseSplit);
	}
	else {
		editor.topsbbutton1.ScrollStyle().SetMonoImage(IdeImg::closesplit()).Подсказка("Закрыть (Ктрл+[\\])");
		editor.topsbbutton1 <<= THISBACK(CloseSplit);
		editor2.topsbbutton1.ScrollStyle().SetMonoImage(IdeImg::closesplit()).Подсказка("Закрыть (Ктрл+[\\])");
		editor2.topsbbutton1 <<= THISBACK(CloseSplit);
	}
}

bool Иср::горячаяКлав(dword ключ)
{
	if(designer && designer->DesignerCtrl().горячаяКлав(ключ))
		return true;
	if(designer && dynamic_cast<FileHexView *>(~designer) && сверь(IdeKeys::AK_EDITASHEX, ключ)) {
		EditUsingDesigner();
		return true;
	}
	return ТопОкно::горячаяКлав(ключ);
}

Ткст Иср::исрДайИмяф()
{
	return editfile;
}

Ткст Иср::IdeGetNestFolder()
{
	Вектор<Ткст> w = GetUppDirs();
	for(int i = 0; i < w.дайСчёт(); i++)
		if(editfile.начинаетсяС(w[i]))
			return w[i];
	return Null;
}

void Иср::Duplicate()
{
	int l, h;
	if(editor.дайВыделение(l, h)) {
		editor.NextUndo();
		editor.устВыделение(h, editor.вставь(h, editor.дайВыделение()) + h);
	}
	else
		editor.дублируйСтроку();
}
