#include "DinrusIDE.h"

#define KEYGROUPNAME "Ide"
#define KEYNAMESPACE IdeKeys
#define KEYFILE      <DinrusIDE/ide.key>
#include             <CtrlLib/key_source.h>

using namespace IdeKeys;

void Ide::DoEditKeys()
{
	EditKeys();
	AKEditor();
}

void Ide::AKEditor()
{
	CodeEditor::find_next_key = AK_FINDNEXT().key[0];
	CodeEditor::find_prev_key = AK_FINDPREV().key[0];
	CodeEditor::replace_key = AK_DOREPLACE().key[0];
}

void Ide::PackageMenu(Bar& menu)
{
	Project(menu);
}

void Ide::FileBookmark(Bar& menu)
{
	int i;
	for(i = 0; i < 10; i++) {
		const Bookmark& b = bookmark[i];
		String txt = Format("Перейти к закладке &%d", i);
		if(!b.file.IsEmpty())
			txt << " (" << bookmark[i].file << ')';
		menu.Add(!b.file.IsEmpty(), txt, THISBACK1(BookKey, K_CTRL_0 + i))
			.Key(K_CTRL_0 + i);
	}
	menu.MenuBreak();
	for(i = 0; i < 10; i++)
		menu.Add("Установить", THISBACK1(BookKey, K_SHIFT_CTRL_0 + i))
		    .Key(K_SHIFT_CTRL_0 + i);
}

void Ide::File(Bar& menu)
{
	if (!IsEditorMode())
	{
		menu.Add(AK_SETMAIN, IdeImg::MainPackage(), THISBACK(NewMainPackage))
			.Enable(!IdeIsDebugLock())
			.Help("Выберите глобальную конфигурацию (var), выберите / добавьте пакет главного проекта");
	}
	
	menu.AddMenu(AK_EDITFILE, CtrlImg::open(), THISBACK(EditAnyFile))
		.Help("Выберите любой файл в файлвыборке и откройте в редакторе");
	menu.AddMenu(!IsNull(GetOpposite()), AK_OPPOSITE, IdeImg::opposite(), THISBACK(GoOpposite))
		.Help("Переключение между исходниками и заголовочниками");
	menu.AddMenu(AK_SAVEFILE, CtrlImg::save(), THISBACK(DoSaveFile))
		.Help("Сохранить текущий файл");
	if(!designer)
		menu.AddMenu(CanToggleReadOnly(), AK_READONLY, IdeImg::read_only(), THISBACK(ToggleReadOnly))
			.Check(editor.IsReadOnly())
			.Help("Установить / сбросить флаг только чтение для текущего файла");

	menu.AddMenu(!designer, AK_PRINT, CtrlImg::print(), THISBACK(Print));

//	menu.Add("Export project", THISBACK(ExportProject))
//		.Help("Copy all project files into given directory");

	if(menu.IsMenuBar())
	{
		menu.Separator();
		menu.Add(AK_CLOSETAB, THISBACK(ClearTab))
		    .Help("Закрыть вкладку текущего файла");
		menu.Add(AK_CLOSETABS, THISBACK(ClearTabs))
		    .Help("Закрыть все вкладки");
		if(!designer) {
			menu.Add("Закладки", THISBACK(FileBookmark))
				.Help("Установите оодну из закладок (1..9, 0) на текущий файл");
			menu.MenuSeparator();
		}
		menu.Add("Показать/скрыть нижнюю панель", THISBACK(SwapBottom))
			.Check(IsBottomShown())
			.Key(K_ESCAPE)
			.Help("Показать / скрыть нижнюю панель (с вкладками консоли, калькулятора и браузера)");
	}

	menu.Add(AK_PACKAGESFILES, THISBACK(SwapPackagesFiles))
	    .Check(weframe.IsShown());

	menu.MenuSeparator();

	bool split = editorsplit.GetZoom() < 0;
	menu.Add(AK_SPLIT, THISBACK1(KeySplit, false))
	    .Check(split && editorsplit.IsVert());
	menu.Add(AK_VSPLIT, THISBACK1(KeySplit, true))
	    .Check(split && editorsplit.IsHorz());
	menu.Add(split, AK_SWAP, THISBACK(SwapEditors));

	menu.MenuSeparator();
	
	menu.Add(AK_OPENFILEDIR, THISBACK(OpenFileFolder));
	menu.Add("Копировать путь к файлу", [=] { WriteClipboardText(GetActiveFilePath()); });
	menu.Sub("Свойства", [=](Bar& bar) { FilePropertiesMenu(bar); });
	menu.MenuSeparator();

	menu.Add(AK_STATISTICS, THISBACK(Statistics))
		.Help("Показать разнообразную статистику");

	menu.Add("Лицензии проекта..", THISBACK(Licenses));
	menu.Add("Прошедшие сроки...", THISBACK(Times));

	menu.Add(AK_EXIT, THISBACK(Exit));
}

void Ide::AssistEdit(Bar& menu)
{
	bool b = !editor.IsReadOnly() && !designer;
	menu.Add(b, "Вставить", THISBACK(InsertMenu));
	menu.Add(b, "Вставить #include", THISBACK(InsertInclude));
}

void Ide::InsertAdvanced(Bar& bar)
{
	bool b = !editor.IsReadOnly();
	AssistEdit(bar);
	bar.Add(b, "Продвинутое", THISBACK(EditSpecial));
}

void Ide::EditSpecial(Bar& menu)
{
	bool b = !editor.IsReadOnly();
	menu.Add(AK_WORDWRAP, THISBACK(ToggleWordwrap))
	    .Check(wordwrap);
	    
	menu.Separator();
	
	menu.Add(b, AK_SPACESTOTABS, THISBACK(EditMakeTabs))
		.Help("Преобразовать вводные пробелы на каждой строке в табы");
	menu.Add(b, AK_TABSTOSPACES, THISBACK(EditMakeSpaces))
		.Help("Преобразовать все табы в пробелы");
	menu.Add(b, AK_LINEENDINGS, THISBACK(EditMakeLineEnds))
		.Help("Удалить табы и пробелы в конце строк");
	menu.Add(b, AK_TRANSLATESTRING, THISBACK(TranslateString))
		.Help("Пометить текущее выделение как переведённую строку");
	menu.Add(b, AK_SWAPCHARS, THISBACK(SwapChars))
	    .Help("Транспонировать символы");
	menu.Add(AK_COPYWORD, THISBACK(CopyWord))
	    .Help("Копировать текущий идентификатор в buffer обмена");
	menu.Add(b, AK_DUPLICATEIT, THISBACK(Duplicate))
	    .Help("Дублировать текущую строку");
	menu.Add(b, AK_FORMATCODE, THISBACK(FormatCode))
	    .Help("Реформатировать код в редакторе");
	menu.Add(b, AK_FORMATJSON, THISBACK(FormatJSON))
	    .Help("Реформатировать JSON");
	menu.Add(b, AK_FORMATXML, THISBACK(FormatXML))
	    .Help("Реформатировать XML");
	menu.Add(b && editor.IsSelection(), AK_TOUPPER, THISBACK(TextToUpper))
	    .Help("Преобразовать буквы выделения в верхний регистр");
	menu.Add(b && editor.IsSelection(), AK_TOLOWER, THISBACK(TextToLower))
	    .Help("Преобразовать буквы выделения в нижний регистр");
	menu.Add(b && editor.IsSelection(), AK_TOASCII, THISBACK(TextToAscii))
		.Help("Преобразовать текст в 7-битный ASCII, удалив все акценты и спецсимволы");
	menu.Add(b && editor.IsSelection(), AK_INITCAPS, THISBACK(TextInitCaps))
	    .Help("Озаглавить первый символ слов в выделении");
	menu.Add(b && editor.IsSelection(), AK_SWAPCASE, THISBACK(SwapCase))
	    .Help("Поменять регист букв в выделении");
	menu.Add(b && editor.IsSelection(), AK_TOCSTRING, THISBACK(ToCString))
	    .Help("Преобразовать выделение в CString");
	menu.Add(b && editor.IsSelection(), AK_TOCOMMENT, THISBACK(ToComment))
		.Help("Закомментировать код");
	menu.Add(b && editor.IsSelection(), AK_COMMENTLINES, THISBACK(CommentLines))
		.Help("Закомментировать строки кода");
	menu.Add(b && editor.IsSelection(), AK_UNCOMMENT, THISBACK(UnComment))
		.Help("Откомментировать код");
	menu.Add(b, AK_REFORMAT_COMMENT, THISBACK(ReformatComment))
	    .Help("Реформатировать многострочный коммент в параграф");
	menu.Add(b, "Удалить журналы отладки (DDUMP...)", [=] { RemoveDs(); });
	menu.MenuSeparator();
	menu.Add(AK_COPY_POSITION, [=] { CopyPosition(); });
	menu.Add(AK_GOTO_POSITION, [=] { GotoPosition(); });
}

void Ide::SearchMenu(Bar& menu)
{
	if(!designer) {
		menu.Add(AK_FIND, THISBACK(EditFind))
			.Help("Найти текст или образец текста");
		menu.Add(!editor.IsReadOnly(), AK_REPLACE, THISBACK(EditReplace))
			.Help("Найти текст или образец текста, с опцией замены");
//		menu.Add(AK_FINDSEL, THISBACK(EditFindReplacePickText))
//			.Help("Show find / replace dialog & set active text as the 'find' text");

		menu.Add(AK_FINDNEXT, THISBACK(EditFindNext))
			.Help("Найти следующий случай");
		menu.Add(AK_FINDPREV, THISBACK(EditFindPrevious))
			.Help("Найти предыдущий случай");

		menu.MenuSeparator();

		menu.Add(AK_FINDSTRING, THISBACK1(FindString, false))
			.Help("Найти любую обычную строковую константу (\"\" - ограничено)");
		menu.Add(AK_FINDSTRINGBACK, THISBACK1(FindString, true))
			.Help("Найти любую обычную строковую константу (\"\" - ограничено) рекурсивно");
		menu.MenuSeparator();
	}
	menu.Add(AK_FINDINFILES, THISBACK1(FindInFiles, false))
		.Help("Найти текст или образец текста в поддереве заданного пути");
	menu.Add(AK_REPLACEINFILES, THISBACK1(FindInFiles, true))
		.Help("Найти текст или образец текста в поддереве заданного пути, с опцией замен(ы)");
	menu.Add(AK_FINDFILE, THISBACK(FindFileName))
		.Help("Найти файл по имени (используйте *, ? когда не уверены)");
}

void Ide::Edit(Bar& menu)
{
	bool b = !editor.IsReadOnly();
	if(editfile.GetCount() && editashex.Find(editfile) < 0) {
		menu.Add(AK_EDITASHEX, THISBACK(EditAsHex));
		if(!designer)
			menu.MenuSeparator();
	}
	if(designer) {
		if(FileExists(designer->GetFileName())) {
			menu.Add(AK_EDITASTEXT, THISBACK(EditAsText))
			    .Help("Редактировать как текстовый файл (не используя дизайнер)");
			menu.MenuSeparator();
		}
		if(menu.IsMenuBar())
			designer->EditMenu(menu);
	}
	else {
		bool selection = editor.IsAnySelection();

		if(editor.IsView()) {
			menu.Add(AK_EDITASTEXT, THISBACK(EditAsText))
			    .Help("Редактировать файл");
			menu.MenuSeparator();
		}
		if(GetFileExt(editfile) == ".t") {
			if(editastext.Find(editfile) >= 0)
				menu.Add(AK_DESIGNER, THISBACK(EditUsingDesigner))
				    .Help("Редактировать преобразованные строки");
			else
				menu.Add(AK_EDITASTEXT, THISBACK(EditAsText))
				    .Help("Редактировать сырые строки");
			menu.MenuSeparator();
		}
		else
		if(editastext.Find(editfile) >= 0/* && IsDesignerFile(editfile)*/) {
			menu.Add(AK_DESIGNER, THISBACK(EditUsingDesigner))
			    .Help(editor.GetLength() > 256*1024*1024 ? "Просмотреть файл" : "Редактировать в дизайнере (не как текст)");
			menu.MenuSeparator();
		}
		menu.Add(b, "Отменить", CtrlImg::undo(), callback(&editor, &LineEdit::Undo))
			.Key(K_CTRL_Z)
			.Enable(editor.IsUndo())
			.Help("Отменить изменения текста");
		menu.Add(b, "Восстановить", CtrlImg::redo(), callback(&editor, &LineEdit::Redo))
			.Key(K_SHIFT|K_CTRL_Z)
			.Enable(editor.IsRedo())
			.Help("Восстановить отменённые изменения");

		menu.Separator();

		menu.Add(b, "Вырезать", CtrlImg::cut(), callback(&editor, &LineEdit::Cut))
			.Key(K_CTRL_X)
			.Enable(selection)
			.Help("Вырезать выделение и поместить в системный buffer обмена");
		menu.Add("Копировать", CtrlImg::copy(), callback(&editor, &LineEdit::Copy))
			.Key(K_CTRL_C)
			.Enable(selection)
			.Help("Скопировать текущее выделение в системный buffer обмена");
		menu.Add(b, "Вставить", CtrlImg::paste(), THISBACK(EditPaste))
			.Key(K_CTRL_V)
			.Help("Вставить текст из буфера обмена в позицию курсора");

		menu.Separator();
		
		menu.Add("Выбрать все", CtrlImg::select_all(), callback(&editor, &LineEdit::SelectAll))
			.Key(K_CTRL_A);
	}

	menu.MenuSeparator();
	
	menu.Add("Найти и Заменить", THISBACK(SearchMenu));

	if(!designer && menu.IsMenuBar())
		InsertAdvanced(menu);
}

bool Ide::HasMacros()
{
	const Array<IdeMacro>& mlist = UscMacros();
	if(!mlist.IsEmpty())
		for(int i = 0; i < mlist.GetCount(); i++) {
			const IdeMacro& m = mlist[i];
			if(!IsNull(m.menu))
				return true;
		}
	return false;
}

void Ide::MacroMenu(Bar& menu)
{
	const Array<IdeMacro>& mlist = UscMacros();
	if(!mlist.IsEmpty() && menu.IsMenuBar()) {
		VectorMap< String, Vector<int> > submenu_map;
		for(int i = 0; i < mlist.GetCount(); i++) {
			const IdeMacro& m = mlist[i];
			if(!IsNull(m.menu)) {
				if(IsNull(m.submenu))
					submenu_map.GetAdd(Null).Add(i);
				else
					submenu_map.GetAdd(m.menu).Add(i);
			}
		}
		if(!submenu_map.IsEmpty()) {
			Vector<int> order = GetSortOrder(submenu_map.GetKeys());
			for(int o = 0; o < order.GetCount(); o++) {
				String m = submenu_map.GetKey(order[o]);
				Vector<int>& mx = submenu_map[order[o]];
				ValueArray va;
				for(int i = 0; i < mx.GetCount(); i++)
					va.Add(mx[i]);
				if(!IsNull(m))
					menu.Add(m, THISBACK1(EditMacroMenu, va));
				else
					EditMacroMenu(menu, va);
			}
		}
	}
}

void Ide::EditMacroMenu(Bar& menu, ValueArray mx)
{
	const Array<IdeMacro>& mlist = UscMacros();
	Vector<String> names;
	Vector<int> index;
	names.Reserve(mx.GetCount());
	for(int i = 0; i < mx.GetCount(); i++) {
		int ii = mx[i];
		if(ii >= 0 && ii < mlist.GetCount()) {
			const IdeMacro& m = mlist[ii];
			names.Add(Nvl(m.submenu, m.menu));
			index.Add(ii);
		}
	}
	IndexSort(names, index);
	for(int i = 0; i < index.GetCount(); i++)
		menu.Add(names[i], THISBACK1(EditMacro, index[i]))
			.Key(mlist[index[i]].hotkey);
}

void Ide::EditMacro(int i)
{
	const Array<IdeMacro>& mlist = UscMacros();
	if(i >= 0 && i < mlist.GetCount()) {
		const IdeMacro& m = mlist[i];
		try {
			Vector<EscValue> arg;
			EscValue api = macro_api, code = m.code;
			::Execute(UscGlobal(), &api, code, arg, 1000000);
		}
		catch(Exc e) {
			PutConsole(e);
		}
	}
}

void Ide::Setup(Bar& menu)
{
	menu.Add("Подробнее", THISBACK(ToggleVerboseBuild))
		.Check(console.verbosebuild)
		.Help("Заносить в лог детальное описание построения и отладки");
	menu.MenuSeparator();
	menu.Add("Настройки..", IdeImg::Settings(), THISBACK(SetupFormat))
		.Help("Шрифты, табы, отступы,строка состояния");
	menu.Add("Сокращения..", THISBACK(Abbreviations))
		.Help("Редактировать ключслова-аббревиации и код");
	menu.Add("Клавиатурные комбинации..", THISBACK(DoEditKeys))
		.Help("Редактировать привязки клавиш");
	menu.Add("Менеджер Макросов..", THISBACK(DoMacroManager))
		.Help("Управление коллекцией макросов..");
	menu.Add("Методы Построения..", THISBACK(SetupBuildMethods))
	    .Help("Установить методы построения");
#ifdef PLATFORM_WIN32
	menu.Add("Автоматическая настройка методов построения..", callback(InstantSetup))
	    .Help("Устанавливает.исправляет методы построения и базовые сборки..");
#endif
	menu.MenuSeparator();
	menu.Add(HasGit(), "DinrusUppHub..", IdeImg::UppHub(), [] { UppHub(); });
	menu.Add("Клонировать исходники U++ с GitHub..", [=] {
		if(SetupGITMaster()) {
			IdeAgain = true;
			Break();
		}
	});

#ifndef PLATFORM_COCOA
	const Workspace& wspc = IdeWorkspace();
	if(wspc[0] == "ide")
		for(int i = 0; i < wspc.GetCount(); i++)
			if(wspc[i] == "DinrusIDE/Core")
				menu.Add("Обновить ИСР РНЦП Динрус..", [=] { UpgradeDinrusIde(); });
#ifdef PLATFORM_POSIX
	menu.Add("Установить dinruside.desktop", [=] { InstallDesktop(); });
#endif
#endif

	if(menu.IsMenuBar())
		SetupMobilePlatforms(menu);
}

void Ide::SetupMobilePlatforms(Bar& menu)
{
	AndroidSDK androidSDK(GetAndroidSdkPath());
	
	if(androidSDK.Validate()) {
		menu.Separator();
		menu.Add("Андроид", THISBACK1(SetupAndroidMobilePlatform, androidSDK));
	}
	
}

void Ide::SetupAndroidMobilePlatform(Bar& menu, const AndroidSDK& androidSDK)
{
	menu.Add("Менеджер SDK", THISBACK1(LaunchAndroidSDKManager, androidSDK));
	menu.Add("Менеджер AVD", THISBACK1(LaunchAndroidAVDManager, androidSDK));
	menu.Add("Монитор устройств", THISBACK1(LauchAndroidDeviceMonitor, androidSDK));
}

void Ide::ProjectRepo(Bar& menu)
{
	Vector<String> w = RepoDirs(true);
	for(int i = 0; i < w.GetCount(); i++)
		menu.Add("Синхронизовать " + w[i], IdeImg::svn_dir(), THISBACK1(SyncRepoDir, w[i]));
	menu.Add("Синхронизовать всё..", IdeImg::svn(), THISBACK(SyncRepo));
}

void Ide::Project(Bar& menu)
{
	if(menu.IsToolBar() && !debugger && !IsEditorMode())
	{
		mainconfiglist.Enable(idestate == EDITING);
		buildmode.Enable(idestate == EDITING);
		menu.Add(mainconfiglist, HorzLayoutZoom(180));
		menu.Gap(4);
		menu.Add(buildmode, HorzLayoutZoom(180));
		menu.Separator();
	}
	if(!IsEditorMode()) {
		WorkspaceWork::PackageMenu(menu);
		menu.MenuSeparator();
		menu.Add(AK_ORGANIZER, IdeImg::package_organizer(), THISBACK(EditWorkspace))
			.Help("Зависимости пакетов, опции компилятора & компоновщика, переписать путь вывода");
		menu.Add(AK_CUSTOM, THISBACK(CustomSteps))
			.Help("Построение промежуточных файлов, используя кастомные команды / приложения");
		if(menu.IsMenuBar())
			menu.Add(AK_MAINCONFIG, IdeImg::main_package(), THISBACK(MainConfig))
				.Help("Конфигурирование компилятора, операционной системы, выходных параметров приложения, кастомных флагов");
		menu.Separator();
		menu.Add(AK_SYNCT, IdeImg::Language(), THISBACK1(SyncT, 0))
		    .Help("Синхронизовать все файлы переводов текущего рабочего пространства");
		menu.AddMenu(AK_TRIMPORT, IdeImg::Language(), THISBACK1(SyncT, 1))
		    .Help("Импортировать рантаймный файл перевода");
		menu.AddMenu(AK_TREXPORT, IdeImg::Language(), THISBACK1(SyncT, 2))
		    .Help("Экспортировать рантаймный файл перевода");
		if(OldLang())
			menu.Add("Преобразовать s_ -> t_", THISBACK(ConvertST));
	}
	menu.MenuSeparator();
	FilePropertiesMenu0(menu);
	if(!IsEditorMode()) {
		menu.MenuSeparator();
		if(repo_dirs) {
			String pp = GetActivePackagePath();
			menu.AddMenu(FileExists(pp) && editfile_repo,
			             (editfile_repo == SVN_DIR ? "Показать svn-историю " : "Показать git-историю ") + GetFileName(pp),
			             IdeImg::SvnDiff(), [=] {
				if(FileExists(pp))
					RunRepoDiff(pp);
			});
			if(menu.IsMenuBar())
				menu.Add("Репозиторий", THISBACK(ProjectRepo));
			else
				menu.Add("Синхронизовать все репозитории..", IdeImg::svn(), THISBACK(SyncRepo));
		}
	}
}

void Ide::FilePropertiesMenu0(Bar& menu)
{
	menu.Add(IsActiveFile(), AK_FILEPROPERTIES, THISBACK(FileProperties))
		.Help("Свойства файла, сохранённые в пакете");
}

void Ide::FilePropertiesMenu(Bar& menu)
{
	FilePropertiesMenu0(menu);
	menu.Add(IsActiveFile() && !designer, AK_SAVEENCODING, THISBACK(ChangeCharset))
	    .Help("Преобразовать файл в другую кодировку");
	bool candiff = IsActiveFile() && !editfile_isfolder && !designer;
	String path;
	int i = filelist.GetCursor() + 1;
	if(i >= 0 && i < fileindex.GetCount() && fileindex[i] < actual.file.GetCount())
		path = SourcePath(actualpackage, actual.file[fileindex[i]]);
	menu.Sub(candiff, "Сравнить с", [=](Bar& bar) {
		bar.AddMenu(candiff, AK_DIFF, IdeImg::Diff(), THISBACK(Diff))
		    .Help("Показать разницу между текущим и выбранным файлами");
		bar.AddMenu(candiff && FileExists(GetTargetLogPath()),
		            AK_DIFFLOG, IdeImg::DiffLog(), [=] { DiffWith(GetTargetLogPath()); })
		    .Help("Показать разницу между текущим файлом и логом");
		if(FileExists(path))
			bar.AddMenu(candiff && FileExists(path), path,
			            IdeImg::DiffNext(), [=] { DiffWith(path); })
			    .Help("Показать разницу между текущим и следующим файлами");
		for(String p : difflru)
			if(p != path)
				bar.AddMenu(candiff && FileExists(p), p,
				            IdeImg::DiffNext(), [=] { DiffWith(p); })
				    .Help("Показать разницу между текущим и тем файлами");
	});
	if(editfile_repo) {
		String txt = String("Показать ") + (editfile_repo == SVN_DIR ? "svn-" : "git-") + "историю ";
		menu.AddMenu(candiff, AK_SVNDIFF, IdeImg::SvnDiff(), [=] {
			if(!IsNull(editfile))
				RunRepoDiff(editfile);
		}).Text(txt + "файла..");
		if(editfile.GetCount()) {
			String mine;
			String theirs;
			String original;
			Vector<String> r;
			Vector<int> rn;
			if(editfile_repo == SVN_DIR) {
				for(FindFile ff(editfile + ".*"); ff; ff.Next()) {
					if(ff.IsFile()) {
						String p = ff.GetPath();
						if(p.Find(".merge-left.r") >= 0)
							original = p;
						if(p.Find(".merge-right.r") >= 0)
							theirs = p;
						if(p.Find(".working") >= 0 || p.Find(".mine") >= 0)
							mine = p;
						try {
							CParser q(GetFileExt(~p));
							q.PassChar('.');
							q.PassChar('r');
							int n = q.ReadInt();
							if(q.IsEof()) {
								r.Add(p);
								rn.Add(n);
							}
						}
						catch(CParser::Error) {}
					}
				}
				if(IsNull(original) && IsNull(theirs) && r.GetCount() == 2) {
					original = r[0];
					theirs = r[1];
					if(rn[1] > rn[0])
						Swap(original, theirs);
				}
			}
			else {
				bool a = false, b = false, c = false;
				int n = min(editor.GetLineCount(), 10000);
				for(int i = 0; i < n; i++) { // check that we are in git conflict
					const String& s = editor.GetUtf8Line(i);
					int ch = *s;
					a = a || ch == '<' && s.StartsWith("<<<<<<<");
					b = b || ch == '=' && s.StartsWith("=======");
					c = c || ch == '>' && s.StartsWith(">>>>>>>");
				}
				if(a && b && c) {
					original = "1";
					mine = "2";
					theirs = "3";
				}
			}
				
			if(mine.GetCount() || theirs.GetCount() || original.GetCount()) {
				menu.Sub("Конфликт SVN", [=] (Bar& bar) {
					if(mine.GetCount() && theirs.GetCount())
						bar.Add("Сравнить мой <-> их", [=] { DiffFiles("мой", mine, "их", theirs); });
					if(mine.GetCount() && original.GetCount())
						bar.Add("Сравнить мой <-> оригинал", [=] { DiffFiles("мой", mine, "оригинал", original); });
					if(theirs.GetCount() && original.GetCount())
						bar.Add("Сравнить их <-> оригинал", [=] { DiffFiles("их", theirs, "оригинал", original); });
					if(mine.GetCount())
						bar.Add("Сравнить текущий <-> мой", [=] { DiffFiles("текущий", editfile, "мой", mine); });
					if(theirs.GetCount())
						bar.Add("Сравнить текущий <-> их", [=] { DiffFiles("текущий", editfile, "их", theirs); });
					if(original.GetCount())
						bar.Add("Сравнить текущий <-> оригинал", [=] { DiffFiles("текущий", editfile, "оригинал", original); });
					bar.Separator();
					bar.Add("Использовать мой", [=] {
						if(PromptYesNo("Вы хотите переписать текущий в [* мой]?")) {
							SaveFile();
							Upp::SaveFile(editfile, LoadConflictFile(mine));
						}
					});
					bar.Add("Использовать их", [=] {
						if(PromptYesNo("Вы хотите переписать текущий в [* их]?")) {
							SaveFile();
							Upp::SaveFile(editfile, LoadConflictFile(theirs));
						}
					});
				});
			}
		}
		if(editfile.GetCount() && editfile_repo == GIT_DIR) {
		}
	}
}

void Ide::BuildFileMenu(Bar& menu)
{
	bool b = idestate == EDITING && !IdeIsDebugLock();
	menu.Add(b, "Компилировать " + GetFileName(editfile), IdeImg::Source(), THISBACK(FileCompile))
		.Key(AK_COMPILEFILE)
		.Help("Скомпилировать текущий файл");
	menu.Add(b, "Препроцессинг " + GetFileName(editfile), IdeImg::Header(), THISBACK1(Preprocess, false))
		.Key(AK_PREPROCESSFILE)
		.Help("Предобработать текущий файл во временный & открыть в редакторе");
	if(findarg(current_builder, "GCC", "CLANG") >= 0)
		menu.Add(b, "Показать код ассемблера для " + GetFileName(editfile), THISBACK1(Preprocess, true))
			.Key(AK_ASSEMBLERCODE)
			.Help("Скомпилировать файл в ассемблерный код");
	if(console.verbosebuild)
		menu.Add(b, "Внутренний Препроцессинг " + GetFileName(editfile), IdeImg::HeaderInternal(), THISBACK(PreprocessInternal));
}

void Ide::BuildPackageMenu(Bar& menu)
{
	int pi = GetPackageIndex();
	bool b = !IdeIsDebugLock() && idestate == EDITING && pi >= 0 && pi < IdeWorkspace().GetCount();
	String name;
	if(b)
		name = '\'' + IdeWorkspace()[pi] + '\'';
	menu.Add(b, "Построить пакет " + name, THISBACK(PackageBuild))
		.Help("Построить текущий пакет");
	menu.Add(b, "Очистить пакет " + name, THISBACK(PackageClean))
		.Help("Удалить все промежуточные файлы из текущего пакета");
	menu.MenuSeparator();
}

void Ide::BuildMenu(Bar& menu)
{
	bool b = !IdeIsDebugLock();
	menu.Add(AK_OUTPUTMODE, THISBACK(SetupOutputMode))
	    .Help("Настроить построение цели");
	if(idestate == BUILDING)
		menu.Add(b, "Остановить построение", IdeImg::build_stop(), THISBACK(StopBuild))
			.Key(AK_BUILD)
			.Help("Остановить построение");
	else
		menu.Add(b, "Построить", IdeImg::build_make(), THISBACK(DoBuild))
			.Key(AK_BUILD)
			.Help("Выполнить минимальную перестройку приложения");
	b = b && idestate == EDITING;
	menu.Add(b, AK_CLEAN, THISBACK(Clean))
		.Help("Удалить все промежуточные файлы");
//	menu.Add("Reset BLITZ", [=] { ResetBlitz(); })
//	    .Help("All files will be considered for BLITZ, regardless of time");
	menu.Add(b, AK_REBUILDALL, IdeImg::build_rebuild_all(), THISBACK(RebuildAll))
		.Help("Удалить все промежуточные файлы & построить");
	menu.Add(b, AK_CLEANUPPOUT, THISBACK(CleanUppOut))
		.Help("Удалить все файлы и подпапки в выводной & промежуточной папке (см. Base setup)");

//	menu.MenuSeparator();

//	menu.Add(b, AK_CREATEMAKEFILE, THISBACK(CreateMakefile))
//		.Help("Create makefile enabling IDE-independent project building");

	menu.MenuSeparator();


	if(menu.IsMenuBar() && !menu.IsScanKeys())
		BuildPackageMenu(menu);

	BuildFileMenu(menu);
	
	menu.MenuSeparator();

	menu.Add("Остановить при ошибках", THISBACK(ToggleStopOnErrors))
		.Check(stoponerrors)
		.Help("Остановить постройку после пакета с ошибками");

	menu.MenuSeparator();

	bool ff = BottomIsFindInFiles();
	String hh = ff ? "позицию" : "строку ошибки";
	bool ffb = ff ? FFound().GetCount() : error.GetCount();
	menu.Add(ffb, AK_FINDNEXTERROR, THISBACK(FindNextError))
		.Help("Найти следующую " + hh + "согласно панели консоли");
	menu.Add(ffb, AK_FINDPREVERROR, THISBACK(FindPrevError))
		.Help("Найти предыдущую " + hh + "согласно панели консоли");
	menu.MenuSeparator();
	menu.Add(!IsNull(target), AK_OPENOUTDIR, [=] { ShellOpenFolder(GetFileFolder(target)); });
	menu.Add(!IsNull(target), AK_COPYOUTDIR, [=] { WriteClipboardText(GetFileFolder(target)); });
	menu.Add(!IsNull(target), AK_COPYTARGET, [=] { WriteClipboardText(target); });
	menu.Add(!IsNull(target), AK_OUTDIRTERMINAL, [=] { LaunchTerminal(GetFileFolder(target)); });
}

void Ide::DebugMenu(Bar& menu)
{
	bool b = idestate == EDITING && !IdeIsDebugLock();
	if(debugger) {
		debugger->DebugBar(menu);
		menu.MenuSeparator();
	}
	else {
		if(console.IsRunning())
			menu.Add("Стоп!", THISBACK(StopDebug))
			    .Help("Остановить подконтрольный процесс");
		if(menu.IsMenuBar())
			menu.Add(AK_RUNOPTIONS, THISBACK(RunArgs))
				.Help("Текущая папка, командная строка, перенаправ на стдвыв");
		menu.Add(b, AK_EXECUTE, IdeImg::execute(), THISBACK(BuildAndExecute))
			.Help("Построить и выполнить приложение");
		menu.Add(b, AK_DEBUG, IdeImg::debug_run(), THISBACK1(BuildAndDebug, false))
			.Help("Построить приложение & запустить отладчик");
		if(menu.IsMenuBar()) {
			menu.Add(b, AK_DEBUGTO, THISBACK1(BuildAndDebug, true))
				.Help("Построить приложение & Выполнить до курсора в отладчике");
			menu.Add(b, AK_DEBUGEXT, THISBACK(BuildAndExtDebug))
				.Help("Построить приложение & запустить внешний отладчик (см. Base setup, default \"msdev.exe\")");
			menu.Add(b, AK_DEBUGFILEEXT, THISBACK(BuildAndExtDebugFile))
				.Help("Построить приложение & запустить внешний отладчик, пытаться начать с текущей строки");
		#ifdef PLATFORM_POSIX
			if(IsValgrind())
				menu.Add(b, AK_VALGRIND, THISBACK(Valgrind))
					.Help("Построить приложение & запустить в valgring");
		#endif

			menu.Separator();
		}
	}
	if(menu.IsMenuBar()) {
		menu.Add(!editfile.IsEmpty() /*&& !debuglock*/, AK_BREAKPOINT, THISBACK(DebugToggleBreak))
			.Help("Установить / очистить остановы на текущей строке");
		menu.Add(!editfile.IsEmpty(), AK_CONDBREAKPOINT, THISBACK(ConditionalBreak))
			.Help("Редактировать условные остановы");
		menu.Add(!editfile.IsEmpty() /*&& !debuglock*/, AK_CLEARBREAKPOINTS, THISBACK(DebugClearBreakpoints))
			.Help("Очистить все точки останова");
		menu.Separator();
		
		String targetLogPath = GetTargetLogPath();
		menu.Add(target.GetCount() && FileExists(targetLogPath), AK_OPENLOG, THISBACK1(OpenLog, targetLogPath));
	}
}

void Ide::BrowseMenu(Bar& menu)
{
	if(!IsEditorMode()) {
		if(menu.IsMenuBar()) {
			menu.AddMenu(AK_NAVIGATOR, IdeImg::Navigator(), THISBACK(ToggleNavigator))
				.Check(editor.IsNavigator())
				.Enable(!designer);
			menu.Add(AK_GOTO, THISBACK(SearchCode))
				.Enable(!designer)
				.Help("Перейти к заданной строке");
			menu.Add(AK_GOTOGLOBAL, THISBACK(NavigatorDlg));
			menu.Add(!designer, AK_JUMPS, THISBACK(ContextGoto));
			menu.Add(!designer, AK_SWAPS, THISBACK(SwapS));
			menu.Add(!designer, AK_ASSIST, callback(&editor, &AssistEditor::Assist));
			menu.Add(!designer, AK_DCOPY, callback(&editor, &AssistEditor::DCopy));
			menu.Add(!designer, AK_VIRTUALS, callback(&editor, &AssistEditor::Virtuals));
			menu.Add(!designer, AK_THISBACKS, callback(&editor, &AssistEditor::Thisbacks));
			menu.Add(!designer, AK_COMPLETE, callback(&editor, &AssistEditor::Complete));
			menu.Add(!designer, AK_ABBR, callback(&editor, &AssistEditor::Abbr));
			menu.Add(!designer, AK_GO_TO_LINE, THISBACK(GoToLine));
			AssistEdit(menu);
			menu.MenuSeparator();
		}
		
		menu.Add("Назад", IdeImg::AssistGoBack(), THISBACK1(History, -1))
			.Key(K_ALT_LEFT)
			.Enable(GetHistory(-1) >= 0);
		menu.Add("Вперёд", IdeImg::AssistGoForward(), THISBACK1(History, 1))
			.Key(K_ALT_RIGHT)
			.Enable(GetHistory(1) >= 0);
		
		if(menu.IsMenuBar()) {
			menu.MenuSeparator();
			menu.Add("Проверка на изменения в исходниках", THISBACK(CheckCodeBase));
			menu.Add("Сканировать все исходники повторно", THISBACK(RescanCode));
			if(!auto_rescan)
				menu.Add(AK_RESCANCURRENTFILE, THISBACK(EditFileAssistSync));
			menu.MenuSeparator();
		}
	}
	else {
		menu.Add(!designer, AK_GO_TO_LINE, THISBACK(GoToLine));
		menu.MenuSeparator();
	}
	
	if(menu.IsMenuBar()) {
		menu.AddMenu(AK_CALC, IdeImg::calc(), THISBACK1(ToggleBottom, BCALC))
	     .Check(IsBottomShown() && btabs.GetCursor() == BCALC);
		menu.AddMenu(AK_QTF, IdeCommonImg::Qtf(), THISBACK(Qtf));
		menu.AddMenu(!designer, AK_XML, IdeCommonImg::xml(), THISBACK(Xml));
		menu.AddMenu(!designer, AK_JSON, IdeCommonImg::json(), THISBACK(Json));
		menu.Add(AK_REFORMAT_JSON, [=] { FormatJSON_XML_File(false); });
		menu.Add(AK_REFORMAT_XML, [=] { FormatJSON_XML_File(true); });
		menu.AddMenu(!designer, AK_ASERRORS, IdeImg::errors(), THISBACK(AsErrors));
		menu.AddMenu(AK_DIRDIFF, DiffImg::DirDiff(), THISBACK(DoDirDiff));
		menu.AddMenu(AK_PATCH, DiffImg::PatchDiff(), THISBACK(DoPatchDiff));
	}
}

void Ide::HelpMenu(Bar& menu)
{
	if(!IsEditorMode()) {
		menu.Add(AK_BROWSETOPICS, IdeImg::help(), THISBACK(ShowTopics));
		menu.Add(editor.GetWord().GetCount(), AK_SEARCHTOPICS, THISBACK(SearchTopics));
	}
	menu.Add(AK_BROWSETOPICS_WIN, IdeImg::help_win(), THISBACK(ShowTopicsWin));
	menu.MenuSeparator();
	menu.AddMenu("Получить помощь / отчитаться о багах..", IdeImg::Go_forward(), callback1(LaunchWebBrowser, "http://www.ultimatepp.org/forums"));
	menu.AddMenu("Онлайн-документация..", IdeImg::Go_forward(), callback1(LaunchWebBrowser, "http://www.ultimatepp.org/www$uppweb$documentation$en-us.html"));
	menu.AddMenu("Общая информация..", IdeImg::Go_forward(), callback1(LaunchWebBrowser, "http://www.ultimatepp.org/www$uppweb$community$en-us.html"));
	menu.Separator();
	OnlineSearchMenu(menu);
	if(menu.IsMenuBar()) {
		menu.Separator();
		menu.Add(FileExists(GetIdeLogPath()), "Просмотреть лог приложения", THISBACK(ViewIdeLogFile));
		menu.Separator();
		menu.Add("О программе..", IdeImg::info(), THISBACK(About));
	}
}

void Ide::MainMenu(Bar& menu)
{
	menu.Add("Файл", THISBACK(File))
		.Help("Пакетные & файловые функци, экспорты, закладки");
	menu.Add("Правка", THISBACK(Edit))
		.Help("Буфер обмена, поиск & замена, преобразования пробелов / табуляций, подсветка масштаба");
	if(HasMacros())
		menu.Add("Макросы", THISBACK(MacroMenu))
			.Help("Редактор & макросы ИСР");
	menu.Add("Проект", THISBACK(Project))
		.Help("Организатор пакета, кастомные шани, менеджер конфигурации");
	if(!IsEditorMode()) {
		menu.Add("Постройка", THISBACK(BuildMenu))
			.Help("Постройка & отладка, незначительные опции построения, показ ошибок");
		menu.Add("Отладка", THISBACK(DebugMenu))
			.Help("Отладочные команды (поддерживается пока только подключение gdb)");
	}
	menu.Add("Ассист", THISBACK(BrowseMenu))
		.Help("Информация, просмотр кода и поддержка");
	menu.Add("Настройка", THISBACK(Setup))
		.Help("Пути, настройки редактора, подключение к удалённому хосту");
	menu.Add("Помощь", THISBACK(HelpMenu))
		.Help("Помощь, доверие и лицензия");
}

void Ide::MainTool(Bar& bar)
{
	if(!IsEditorMode()) {
		BrowseMenu(bar);
		bar.Separator();
	}
	bar.Add("Редактировать как текст", IdeImg::EditText(), THISBACK(EditAsText))
	   .Check(!designer)
	   .Enable(!editfile_isfolder)
	   .Key(!designer ? 0 : K_CTRL_T);
	bool b = designer && !designer.Is<FileHexView>();
	bool d = IsDesignerFile(editfile);
	bar.Add("Редактировать с помощью дизайнера", IdeImg::EditDesigner(), THISBACK(EditUsingDesigner))
	   .Check(b || editfile_isfolder)
	   .Enable(d || editfile_isfolder)
	   .Key(b || !d ? 0 : K_CTRL_T);
	bar.Add("Просмотреть как гекс", IdeImg::EditHex(), THISBACK(EditAsHex))
	   .Check(designer.Is<FileHexView>())
	   .Enable(!editfile_isfolder)
	   .Key(K_CTRL_B);
	if(!designer)
		bar.Separator();
	Edit(bar);
	if(debugger) {
		DebugMenu(bar);
		bar.Separator();
	}
	Project(bar);
	if(!IsEditorMode()) {
		BuildMenu(bar);
		if(!debugger) {
			bar.Separator();
			DebugMenu(bar);
		}
		bar.Separator();
	}
	HelpMenu(bar);
#ifdef PLATFORM_COCOA
	bar.GapRight();
	bar.Add(display, HorzLayoutZoom(170));
#endif
}

void Ide::ConsoleMenu(Bar& menu)
{
	bool selection = console.IsAnySelection();
	menu.Add("Копировать", CtrlImg::copy(), THISBACK(ConsoleCopy))
		.Key(K_CTRL_C)
		.Enable(selection)
		.Help("Копировать выделение в системный buffer обмена");
	menu.Add("Вставить", CtrlImg::paste(), THISBACK(ConsolePaste))
		.Key(K_CTRL_V)
		.Help("Добавить выделение в системную консоль");
	menu.Separator();
	menu.Add(AK_FIND, [=] {
		console.FindReplace(false, true, false);
	});
	menu.Separator();
	menu.Add("Очистить", THISBACK(ConsoleClear))
		.Help("Очистить системную консоль");
}

void Ide::SetBar()
{
	SetMenuBar();
	SetToolBar();
}

void Ide::SetMenuBar()
{
#ifdef PLATFORM_COCOA
	SetMainMenu(THISBACK(MainMenu));
	menubar.Hide();
#else
	menubar.Set(THISBACK(MainMenu));
#endif
}

void Ide::SetToolBar()
{
	toolbar.Set(THISBACK(MainTool));
}
