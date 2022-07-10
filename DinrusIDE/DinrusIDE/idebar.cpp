#include "DinrusIDE.h"

#define KEYGROUPNAME "Иср"
#define KEYNAMESPACE IdeKeys
#define KEYFILE      <DinrusIDE/DinrusIDE.key>
#include             <CtrlLib/key_source.h>

using namespace IdeKeys;

void Иср::DoEditKeys()
{
	редактируйКлючи();
	AKEditor();
}

void Иср::AKEditor()
{
	РедакторКода::find_next_key = AK_FINDNEXT().ключ[0];
	РедакторКода::find_prev_key = AK_FINDPREV().ключ[0];
	РедакторКода::replace_key = AK_DOREPLACE().ключ[0];
}

void Иср::PackageMenu(Бар& menu)
{
	Project(menu);
}

void Иср::FileBookmark(Бар& menu)
{
	int i;
	for(i = 0; i < 10; i++) {
		const Bookmark& b = bookmark[i];
		Ткст txt = фмт("Перейти к закладке &%d", i);
		if(!b.file.пустой())
			txt << " (" << bookmark[i].file << ')';
		menu.добавь(!b.file.пустой(), txt, THISBACK1(BookKey, K_CTRL_0 + i))
			.Ключ(K_CTRL_0 + i);
	}
	menu.MenuBreak();
	for(i = 0; i < 10; i++)
		menu.добавь("Набор", THISBACK1(BookKey, K_SHIFT_CTRL_0 + i))
		    .Ключ(K_SHIFT_CTRL_0 + i);
}

void Иср::Файл(Бар& menu)
{
	if (!IsEditorMode())
	{
		menu.добавь(AK_SETMAIN, IdeImg::MainPackage(), THISBACK(NewMainPackage))
			.вкл(!исрОтладБлокировка_ли())
			.Help("Выберите глобальную конфигурацию (var), выберите / добавьте пакет главного проекта");
	}
	
	menu.добавьМеню(AK_EDITFILE, CtrlImg::open(), THISBACK(EditAnyFile))
		.Help("Выберите любой файл в файлвыборке и откройте в редакторе");
	menu.добавьМеню(!пусто_ли(GetOpposite()), AK_OPPOSITE, IdeImg::opposite(), THISBACK(GoOpposite))
		.Help("Переключение между исходниками и заголовочниками");
	menu.добавьМеню(AK_SAVEFILE, CtrlImg::save(), THISBACK(DoSaveFile))
		.Help("Сохранить текущий файл");
	if(!designer)
		menu.добавьМеню(CanToggleReadOnly(), AK_READONLY, IdeImg::read_only(), THISBACK(ToggleReadOnly))
			.Check(editor.толькочтен_ли())
			.Help("Установить / сбрость флаг только чтение для текущего файла");

	menu.добавьМеню(!designer, AK_PRINT, CtrlImg::print(), THISBACK(Print));

//	menu.добавь("Export project", THISBACK(ExportProject))
//		.Help("копируй all project files into given directory");

	if(menu.барМеню_ли())
	{
		menu.Separator();
		menu.добавь(AK_CLOSETAB, THISBACK(ClearTab))
		    .Help("Закрыть вкладку текущего файла");
		menu.добавь(AK_CLOSETABS, THISBACK(ClearTabs))
		    .Help("Закрыть все вкладки");
		if(!designer) {
			menu.добавь("Закладки", THISBACK(FileBookmark))
				.Help("Установите оодну из закладок (1..9, 0) на текущий файл");
			menu.MenuSeparator();
		}
		menu.добавь("Показать/скрыть нижнюю панель", THISBACK(SwapBottom))
			.Check(IsBottomShown())
			.Ключ(K_ESCAPE)
			.Help("Показать / скрыть нижныы панель (с вкладками консоли, калькулятора и браузера)");
	}

	menu.добавь(AK_PACKAGESFILES, THISBACK(SwapPackagesFiles))
	    .Check(weframe.показан_ли());

	menu.MenuSeparator();

	bool split = editorsplit.GetZoom() < 0;
	menu.добавь(AK_SPLIT, THISBACK1(KeySplit, false))
	    .Check(split && editorsplit.вертикален());
	menu.добавь(AK_VSPLIT, THISBACK1(KeySplit, true))
	    .Check(split && editorsplit.горизонтален());
	menu.добавь(split, AK_SWAP, THISBACK(SwapEditors));

	menu.MenuSeparator();
	
	menu.добавь(AK_OPENFILEDIR, THISBACK(OpenFileFolder));
	menu.добавь("Копировать путь к файлу", [=] { WriteClipboardText(GetActiveFilePath()); });
	menu.Sub("Свойства", [=](Бар& bar) { FilePropertiesMenu(bar); });
	menu.MenuSeparator();

	menu.добавь(AK_STATISTICS, THISBACK(Statistics))
		.Help("Показать разнообразную статистику");

	menu.добавь("Лицензии проекта..", THISBACK(Licenses));
	menu.добавь("Прошедшие сроки...", THISBACK(Times));

	menu.добавь(AK_EXIT, THISBACK(выход));
}

void Иср::AssistEdit(Бар& menu)
{
	bool b = !editor.толькочтен_ли() && !designer;
	menu.добавь(b, "Вставить", THISBACK(InsertMenu));
	menu.добавь(b, "Вставить #include", THISBACK(InsertInclude));
}

void Иср::InsertAdvanced(Бар& bar)
{
	bool b = !editor.толькочтен_ли();
	AssistEdit(bar);
	bar.добавь(b, "Продвинутое", THISBACK(EditSpecial));
}

void Иср::EditSpecial(Бар& menu)
{
	bool b = !editor.толькочтен_ли();
	menu.добавь(AK_WORDWRAP, THISBACK(ToggleWordwrap))
	    .Check(wordwrap);
	    
	menu.Separator();
	
	menu.добавь(b, AK_SPACESTOTABS, THISBACK(EditMakeTabs))
		.Help("Преобразовать вводные пробелы на каждой строке в tabs");
	menu.добавь(b, AK_TABSTOSPACES, THISBACK(EditMakeSpaces))
		.Help("Преобразовать все tabs в пробелы");
	menu.добавь(b, AK_LINEENDINGS, THISBACK(EditMakeLineEnds))
		.Help("Удалить tabs и пробелы в конце строк");
	menu.добавь(b, AK_TRANSLATESTRING, THISBACK(TranslateString))
		.Help("Пометить текущее выделение как переведённую строку");
	menu.добавь(b, AK_SWAPCHARS, THISBACK(поменяйСимы))
	    .Help("Транспонировать символы");
	menu.добавь(AK_COPYWORD, THISBACK(копируйСлово))
	    .Help("Копировать текущий идентификатор в буфер обмена");
	menu.добавь(b, AK_DUPLICATEIT, THISBACK(Duplicate))
	    .Help("Дублировать текущую строку");
	menu.добавь(b, AK_FORMATCODE, THISBACK(FormatCode))
	    .Help("Реформатировать код в редакторе");
	menu.добавь(b, AK_FORMATJSON, THISBACK(FormatJSON))
	    .Help("Реформатировать JSON");
	menu.добавь(b, AK_FORMATXML, THISBACK(FormatXML))
	    .Help("Реформатировать XML");
	menu.добавь(b && editor.выделение_ли(), AK_TOUPPER, THISBACK(TextToUpper))
	    .Help("Преобразовать буквы выделения в верхний регистр");
	menu.добавь(b && editor.выделение_ли(), AK_TOLOWER, THISBACK(TextToLower))
	    .Help("Преобразовать буквы выделения в нижний регистр");
	menu.добавь(b && editor.выделение_ли(), AK_TOASCII, THISBACK(TextToAscii))
		.Help("Преобразовать текст в 7-битный ASCII, удалив все акценты и спецсимволы");
	menu.добавь(b && editor.выделение_ли(), AK_INITCAPS, THISBACK(TextInitCaps))
	    .Help("Озаглавить первый символ слов в выделении");
	menu.добавь(b && editor.выделение_ли(), AK_SWAPCASE, THISBACK(SwapCase))
	    .Help("Поменять регист букв в выделении");
	menu.добавь(b && editor.выделение_ли(), AK_TOCSTRING, THISBACK(ToCString))
	    .Help("Преобразовать выделение в CString");
	menu.добавь(b && editor.выделение_ли(), AK_TOCOMMENT, THISBACK(ToComment))
		.Help("Закомментировать код");
	menu.добавь(b && editor.выделение_ли(), AK_COMMENTLINES, THISBACK(CommentLines))
		.Help("Закомментировать строки кода");
	menu.добавь(b && editor.выделение_ли(), AK_UNCOMMENT, THISBACK(UnComment))
		.Help("Откомментировать код");
	menu.добавь(b, AK_REFORMAT_COMMENT, THISBACK(перефмтКоммент))
	    .Help("Реформатировать многострочный коммент в параграф");
	menu.добавь(b, "Удалить журналы отладки (DDUMP...)", [=] { RemoveDs(); });
	menu.MenuSeparator();
	menu.добавь(AK_COPY_POSITION, [=] { CopyPosition(); });
	menu.добавь(AK_GOTO_POSITION, [=] { GotoPosition(); });
}

void Иср::SearchMenu(Бар& menu)
{
	if(!designer) {
		menu.добавь(AK_FIND, THISBACK(EditFind))
			.Help("Найти текст или образец текста");
		menu.добавь(!editor.толькочтен_ли(), AK_REPLACE, THISBACK(EditReplace))
			.Help("Найти текст или образец текста, с опцией замены");
//		menu.добавь(AK_FINDSEL, THISBACK(EditFindReplacePickText))
//			.Help("покажи find / replace dialog & set active text as the 'find' text");

		menu.добавь(AK_FINDNEXT, THISBACK(EditFindNext))
			.Help("Найти следующий случай");
		menu.добавь(AK_FINDPREV, THISBACK(EditFindPrevious))
			.Help("Найти предыдущий случай");

		menu.MenuSeparator();

		menu.добавь(AK_FINDSTRING, THISBACK1(найдиТкст, false))
			.Help("найди any ordinary string constant (\"\" - delimited)");
		menu.добавь(AK_FINDSTRINGBACK, THISBACK1(найдиТкст, true))
			.Help("найди any ordinary string constant (\"\" - delimited) backwards");
		menu.MenuSeparator();
	}
	menu.добавь(AK_FINDINFILES, THISBACK1(FindInFiles, false))
		.Help("Найти текст или образец текста в поддереве заданного пути");
	menu.добавь(AK_REPLACEINFILES, THISBACK1(FindInFiles, true))
		.Help("Найти текст или образец текста в поддереве заданного пути, с опцией замен(ы)");
	menu.добавь(AK_FINDFILE, THISBACK(FindFileName))
		.Help("Найти файл по имени (используйте *, ? когда не уверены)");
}

void Иср::Edit(Бар& menu)
{
	bool b = !editor.толькочтен_ли();
	if(editfile.дайСчёт() && editashex.найди(editfile) < 0) {
		menu.добавь(AK_EDITASHEX, THISBACK(EditAsHex));
		if(!designer)
			menu.MenuSeparator();
	}
	if(designer) {
		if(файлЕсть(designer->дайИмяф())) {
			menu.добавь(AK_EDITASTEXT, THISBACK(EditAsText))
			    .Help("Редактировать как текстовый файл (не используя дизайнер)");
			menu.MenuSeparator();
		}
		if(menu.барМеню_ли())
			designer->EditMenu(menu);
	}
	else {
		bool selection = editor.IsAnySelection();

		if(editor.IsView()) {
			menu.добавь(AK_EDITASTEXT, THISBACK(EditAsText))
			    .Help("Редактировать файл");
			menu.MenuSeparator();
		}
		if(дайРасшф(editfile) == ".t") {
			if(editastext.найди(editfile) >= 0)
				menu.добавь(AK_DESIGNER, THISBACK(EditUsingDesigner))
				    .Help("Редактировать преобразованные строки");
			else
				menu.добавь(AK_EDITASTEXT, THISBACK(EditAsText))
				    .Help("Редактировать сырые строки");
			menu.MenuSeparator();
		}
		else
		if(editastext.найди(editfile) >= 0/* && IsDesignerFile(editfile)*/) {
			menu.добавь(AK_DESIGNER, THISBACK(EditUsingDesigner))
			    .Help(editor.дайДлину() > 256*1024*1024 ? "Просмотреть файл" : "Редактировать в дизайнере (не как текст)");
			menu.MenuSeparator();
		}
		menu.добавь(b, "Отменить", CtrlImg::undo(), callback(&editor, &СтрокРедакт::Undo))
			.Ключ(K_CTRL_Z)
			.вкл(editor.IsUndo())
			.Help("Отменить изменения текста");
		menu.добавь(b, "Восстановить", CtrlImg::redo(), callback(&editor, &СтрокРедакт::Redo))
			.Ключ(K_SHIFT|K_CTRL_Z)
			.вкл(editor.IsRedo())
			.Help("Восстановить отменённые изменения");

		menu.Separator();

		menu.добавь(b, "Вырезать", CtrlImg::cut(), callback(&editor, &СтрокРедакт::вырежь))
			.Ключ(K_CTRL_X)
			.вкл(selection)
			.Help("Вырезать выделение и поместить в системный буфер обмена");
		menu.добавь("Копировать", CtrlImg::copy(), callback(&editor, &СтрокРедакт::копируй))
			.Ключ(K_CTRL_C)
			.вкл(selection)
			.Help("Скопировать текущее выделение в системный буфер обмена");
		menu.добавь(b, "Вставить", CtrlImg::paste(), THISBACK(EditPaste))
			.Ключ(K_CTRL_V)
			.Help("Вставить текст из буфера обмена в позицию курсора");

		menu.Separator();
		
		menu.добавь("Выбрать все", CtrlImg::select_all(), callback(&editor, &СтрокРедакт::выбериВсе))
			.Ключ(K_CTRL_A);
	}

	menu.MenuSeparator();
	
	menu.добавь("Найти и Заменить", THISBACK(SearchMenu));

	if(!designer && menu.барМеню_ли())
		InsertAdvanced(menu);
}

bool Иср::HasMacros()
{
	const Массив<IdeMacro>& mlist = UscMacros();
	if(!mlist.пустой())
		for(int i = 0; i < mlist.дайСчёт(); i++) {
			const IdeMacro& m = mlist[i];
			if(!пусто_ли(m.menu))
				return true;
		}
	return false;
}

void Иср::MacroMenu(Бар& menu)
{
	const Массив<IdeMacro>& mlist = UscMacros();
	if(!mlist.пустой() && menu.барМеню_ли()) {
		ВекторМап< Ткст, Вектор<int> > submenu_map;
		for(int i = 0; i < mlist.дайСчёт(); i++) {
			const IdeMacro& m = mlist[i];
			if(!пусто_ли(m.menu)) {
				if(пусто_ли(m.submenu))
					submenu_map.дайДобавь(Null).добавь(i);
				else
					submenu_map.дайДобавь(m.menu).добавь(i);
			}
		}
		if(!submenu_map.пустой()) {
			Вектор<int> order = GetSortOrder(submenu_map.дайКлючи());
			for(int o = 0; o < order.дайСчёт(); o++) {
				Ткст m = submenu_map.дайКлюч(order[o]);
				Вектор<int>& mx = submenu_map[order[o]];
				МассивЗнач va;
				for(int i = 0; i < mx.дайСчёт(); i++)
					va.добавь(mx[i]);
				if(!пусто_ли(m))
					menu.добавь(m, THISBACK1(EditMacroMenu, va));
				else
					EditMacroMenu(menu, va);
			}
		}
	}
}

void Иср::EditMacroMenu(Бар& menu, МассивЗнач mx)
{
	const Массив<IdeMacro>& mlist = UscMacros();
	Вектор<Ткст> names;
	Вектор<int> Индекс;
	names.резервируй(mx.дайСчёт());
	for(int i = 0; i < mx.дайСчёт(); i++) {
		int ii = mx[i];
		if(ii >= 0 && ii < mlist.дайСчёт()) {
			const IdeMacro& m = mlist[ii];
			names.добавь(Nvl(m.submenu, m.menu));
			Индекс.добавь(ii);
		}
	}
	IndexSort(names, Индекс);
	for(int i = 0; i < Индекс.дайСчёт(); i++)
		menu.добавь(names[i], THISBACK1(EditMacro, Индекс[i]))
			.Ключ(mlist[Индекс[i]].hotkey);
}

void Иср::EditMacro(int i)
{
	const Массив<IdeMacro>& mlist = UscMacros();
	if(i >= 0 && i < mlist.дайСчёт()) {
		const IdeMacro& m = mlist[i];
		try {
			Вектор<EscValue> arg;
			EscValue api = macro_api, code = m.code;
			::выполни(UscGlobal(), &api, code, arg, 1000000);
		}
		catch(Искл e) {
			вКонсоль(e);
		}
	}
}

void Иср::настрой(Бар& menu)
{
	menu.добавь("Подробнее", THISBACK(ToggleVerboseBuild))
		.Check(console.verbosebuild)
		.Help("Log detailed description of build and debug");
	menu.MenuSeparator();
	menu.добавь("Настройки..", IdeImg::Settings(), THISBACK(SetupFormat))
		.Help("Fonts, tabs, indentation, status bar");
	menu.добавь("Сокращения..", THISBACK(Abbreviations))
		.Help("Edit abbreviation keywords and code");
	menu.добавь("Клавиатурные комбинации..", THISBACK(DoEditKeys))
		.Help("Редактировать привязки клавиш");
	menu.добавь("Менеджер Макросов..", THISBACK(DoMacroManager))
		.Help("Управление коллекцией макросов..");
	menu.добавь("Методы Построения..", THISBACK(SetupBuildMethods))
	    .Help("Установить методы построения");
#ifdef PLATFORM_WIN32
	menu.добавь("Автоматическая настройка методов построения..", callback(InstantSetup))
	    .Help("Setups/fixes build methods and basic assemblies..");
#endif
	menu.MenuSeparator();
	menu.добавь(HasGit(), "UppHub..", IdeImg::UppHub(), [] { UppHub(); });
	menu.добавь("Клонировать исходники U++ с GitHub..", [=] {
		if(SetupGITMaster()) {
			IdeAgain = true;
			Break();
		}
	});

#ifndef PLATFORM_COCOA
	const РОбласть& wspc = роблИср();
	if(wspc[0] == "ide")
		for(int i = 0; i < wspc.дайСчёт(); i++)
			if(wspc[i] == "ide/Core")
				menu.добавь("Обновить ИСР РНЦП Динрус..", [=] { UpgradeTheIDE(); });
#ifdef PLATFORM_POSIX
	menu.добавь("Установить theide.desktop", [=] { InstallDesktop(); });
#endif
#endif

	if(menu.барМеню_ли())
		SetupMobilePlatforms(menu);
}

void Иср::SetupMobilePlatforms(Бар& menu)
{
	AndroidSDK androidSDK(GetAndroidSdkPath());
	
	if(androidSDK.Validate()) {
		menu.Separator();
		menu.добавь("Андроид", THISBACK1(SetupAndroidMobilePlatform, androidSDK));
	}
	
}

void Иср::SetupAndroidMobilePlatform(Бар& menu, const AndroidSDK& androidSDK)
{
	menu.добавь("Менеджер SDK", THISBACK1(LaunchAndroidSDKManager, androidSDK));
	menu.добавь("Менеджер AVD", THISBACK1(LaunchAndroidAVDManager, androidSDK));
	menu.добавь("Монитор устройств", THISBACK1(LauchAndroidDeviceMonitor, androidSDK));
}

void Иср::ProjectRepo(Бар& menu)
{
	Вектор<Ткст> w = RepoDirs(true);
	for(int i = 0; i < w.дайСчёт(); i++)
		menu.добавь("Синхронизовать " + w[i], IdeImg::svn_dir(), THISBACK1(SyncRepoDir, w[i]));
	menu.добавь("Синхронизовать всё..", IdeImg::svn(), THISBACK(SyncRepo));
}

void Иср::Project(Бар& menu)
{
	if(menu.барИнстр_ли() && !debugger && !IsEditorMode())
	{
		mainconfiglist.вкл(idestate == EDITING);
		buildmode.вкл(idestate == EDITING);
		menu.добавь(mainconfiglist, HorzLayoutZoom(180));
		menu.Gap(4);
		menu.добавь(buildmode, HorzLayoutZoom(180));
		menu.Separator();
	}
	if(!IsEditorMode()) {
		WorkspaceWork::PackageMenu(menu);
		menu.MenuSeparator();
		menu.добавь(AK_ORGANIZER, IdeImg::package_organizer(), THISBACK(EditWorkspace))
			.Help("Пакет dependencies, compiler & linker options, output path override");
		menu.добавь(AK_CUSTOM, THISBACK(CustomSteps))
			.Help("Building intermediate files using custom commands / applications");
		if(menu.барМеню_ли())
			menu.добавь(AK_MAINCONFIG, IdeImg::main_package(), THISBACK(MainConfig))
				.Help("Configuring compiler, operating system, output application parameters, custom flags");
		menu.Separator();
		menu.добавь(AK_SYNCT, IdeImg::Language(), THISBACK1(SyncT, 0))
		    .Help("Synchronize all language translation files of current workspace");
		menu.добавьМеню(AK_TRIMPORT, IdeImg::Language(), THISBACK1(SyncT, 1))
		    .Help("Import runtime translation file");
		menu.добавьМеню(AK_TREXPORT, IdeImg::Language(), THISBACK1(SyncT, 2))
		    .Help("Export runtime translation file");
		if(OldLang())
			menu.добавь("Преобразовать s_ -> t_", THISBACK(ConvertST));
	}
	menu.MenuSeparator();
	FilePropertiesMenu0(menu);
	if(!IsEditorMode()) {
		menu.MenuSeparator();
		if(repo_dirs) {
			Ткст pp = GetActivePackagePath();
			menu.добавьМеню(файлЕсть(pp) && editfile_repo,
			             (editfile_repo == SVN_DIR ? "Показать svn-историю " : "Показать git-историю ") + дайИмяф(pp),
			             IdeImg::SvnDiff(), [=] {
				if(файлЕсть(pp))
					RunRepoDiff(pp);
			});
			if(menu.барМеню_ли())
				menu.добавь("Репозиторий", THISBACK(ProjectRepo));
			else
				menu.добавь("Синхронизовать все репозитории..", IdeImg::svn(), THISBACK(SyncRepo));
		}
	}
}

void Иср::FilePropertiesMenu0(Бар& menu)
{
	menu.добавь(IsActiveFile(), AK_FILEPROPERTIES, THISBACK(FileProperties))
		.Help("Файл properties stored in package");
}

void Иср::FilePropertiesMenu(Бар& menu)
{
	FilePropertiesMenu0(menu);
	menu.добавь(IsActiveFile() && !designer, AK_SAVEENCODING, THISBACK(ChangeCharset))
	    .Help("Преобр actual file to different encoding");
	bool candiff = IsActiveFile() && !editfile_isfolder && !designer;
	Ткст path;
	int i = filelist.дайКурсор() + 1;
	if(i >= 0 && i < fileindex.дайСчёт() && fileindex[i] < actual.file.дайСчёт())
		path = SourcePath(actualpackage, actual.file[fileindex[i]]);
	menu.Sub(candiff, "Сравнить с", [=](Бар& bar) {
		bar.добавьМеню(candiff, AK_DIFF, IdeImg::Diff(), THISBACK(Diff))
		    .Help("покажи differences between the current and selected file");
		bar.добавьМеню(candiff && файлЕсть(GetTargetLogPath()),
		            AK_DIFFLOG, IdeImg::DiffLog(), [=] { DiffWith(GetTargetLogPath()); })
		    .Help("покажи differences between the current file and the log");
		if(файлЕсть(path))
			bar.добавьМеню(candiff && файлЕсть(path), path,
			            IdeImg::DiffNext(), [=] { DiffWith(path); })
			    .Help("покажи differences between the current and the next file");
		for(Ткст p : difflru)
			if(p != path)
				bar.добавьМеню(candiff && файлЕсть(p), p,
				            IdeImg::DiffNext(), [=] { DiffWith(p); })
				    .Help("покажи differences between the current and that file");
	});
	if(editfile_repo) {
		Ткст txt = Ткст("Показать ") + (editfile_repo == SVN_DIR ? "svn-" : "git-") + "историю ";
		menu.добавьМеню(candiff, AK_SVNDIFF, IdeImg::SvnDiff(), [=] {
			if(!пусто_ли(editfile))
				RunRepoDiff(editfile);
		}).устТекст(txt + "файла..");
		if(editfile.дайСчёт()) {
			Ткст mine;
			Ткст theirs;
			Ткст original;
			Вектор<Ткст> r;
			Вектор<int> rn;
			if(editfile_repo == SVN_DIR) {
				for(ФайлПоиск ff(editfile + ".*"); ff; ff.следщ()) {
					if(ff.файл_ли()) {
						Ткст p = ff.дайПуть();
						if(p.найди(".merge-left.r") >= 0)
							original = p;
						if(p.найди(".merge-right.r") >= 0)
							theirs = p;
						if(p.найди(".working") >= 0 || p.найди(".mine") >= 0)
							mine = p;
						try {
							СиПарсер q(дайРасшф(~p));
							q.передайСим('.');
							q.передайСим('r');
							int n = q.читайЦел();
							if(q.кф_ли()) {
								r.добавь(p);
								rn.добавь(n);
							}
						}
						catch(СиПарсер::Ошибка) {}
					}
				}
				if(пусто_ли(original) && пусто_ли(theirs) && r.дайСчёт() == 2) {
					original = r[0];
					theirs = r[1];
					if(rn[1] > rn[0])
						::разверни(original, theirs);
				}
			}
			else {
				bool a = false, b = false, c = false;
				int n = min(editor.дайСчётСтрок(), 10000);
				for(int i = 0; i < n; i++) { // check that we are in git conflict
					const Ткст& s = editor.дайУтф8Строку(i);
					int ch = *s;
					a = a || ch == '<' && s.начинаетсяС("<<<<<<<");
					b = b || ch == '=' && s.начинаетсяС("=======");
					c = c || ch == '>' && s.начинаетсяС(">>>>>>>");
				}
				if(a && b && c) {
					original = "1";
					mine = "2";
					theirs = "3";
				}
			}
				
			if(mine.дайСчёт() || theirs.дайСчёт() || original.дайСчёт()) {
				menu.Sub("Конфликт SVN", [=] (Бар& bar) {
					if(mine.дайСчёт() && theirs.дайСчёт())
						bar.добавь("Сравнить мой <-> их", [=] { DiffFiles("мой", mine, "их", theirs); });
					if(mine.дайСчёт() && original.дайСчёт())
						bar.добавь("Сравнить мой <-> оригинал", [=] { DiffFiles("мой", mine, "оригинал", original); });
					if(theirs.дайСчёт() && original.дайСчёт())
						bar.добавь("Сравнить их <-> оригинал", [=] { DiffFiles("их", theirs, "оригинал", original); });
					if(mine.дайСчёт())
						bar.добавь("Сравнить текущий <-> мой", [=] { DiffFiles("текущий", editfile, "мой", mine); });
					if(theirs.дайСчёт())
						bar.добавь("Сравнить текущий <-> их", [=] { DiffFiles("текущий", editfile, "их", theirs); });
					if(original.дайСчёт())
						bar.добавь("Сравнить текущий <-> оригинал", [=] { DiffFiles("текущий", editfile, "оригинал", original); });
					bar.Separator();
					bar.добавь("Использовать мой", [=] {
						if(PromptYesNo("Вы хотите переписать текущий в [* мой]?")) {
							сохраниФайл();
							РНЦП::сохраниФайл(editfile, LoadConflictFile(mine));
						}
					});
					bar.добавь("Использовать их", [=] {
						if(PromptYesNo("Вы хотите переписать текущий в [* их]?")) {
							сохраниФайл();
							РНЦП::сохраниФайл(editfile, LoadConflictFile(theirs));
						}
					});
				});
			}
		}
		if(editfile.дайСчёт() && editfile_repo == GIT_DIR) {
		}
	}
}

void Иср::BuildFileMenu(Бар& menu)
{
	bool b = idestate == EDITING && !исрОтладБлокировка_ли();
	menu.добавь(b, "Компилировать " + дайИмяф(editfile), IdeImg::Source(), THISBACK(FileCompile))
		.Ключ(AK_COMPILEFILE)
		.Help("Скомпилировать текущий файл");
	menu.добавь(b, "Препроцессинг " + дайИмяф(editfile), IdeImg::Header(), THISBACK1(Preprocess, false))
		.Ключ(AK_PREPROCESSFILE)
		.Help("Предобработать текущий файл во временный & открыть в редакторе");
	if(findarg(current_builder, "GCC", "CLANG") >= 0)
		menu.добавь(b, "Показать код ассемблера для " + дайИмяф(editfile), THISBACK1(Preprocess, true))
			.Ключ(AK_ASSEMBLERCODE)
			.Help("Скомпилировать файл в ассемблерный код");
	if(console.verbosebuild)
		menu.добавь(b, "Внутренний Препроцессинг " + дайИмяф(editfile), IdeImg::HeaderInternal(), THISBACK(PreprocessInternal));
}

void Иср::BuildPackageMenu(Бар& menu)
{
	int pi = GetPackageIndex();
	bool b = !исрОтладБлокировка_ли() && idestate == EDITING && pi >= 0 && pi < роблИср().дайСчёт();
	Ткст имя;
	if(b)
		имя = '\'' + роблИср()[pi] + '\'';
	menu.добавь(b, "Построить пакет " + имя, THISBACK(PackageBuild))
		.Help("Построить текущий пакет");
	menu.добавь(b, "Очистить пакет " + имя, THISBACK(PackageClean))
		.Help("Удалить все промежуточные файлы из текущего пакета");
	menu.MenuSeparator();
}

void Иср::BuildMenu(Бар& menu)
{
	bool b = !исрОтладБлокировка_ли();
	menu.добавь(AK_OUTPUTMODE, THISBACK(SetupOutputMode))
	    .Help("Настроить построение цели");
	if(idestate == BUILDING)
		menu.добавь(b, "Остановить построение", IdeImg::build_stop(), THISBACK(StopBuild))
			.Ключ(AK_BUILD)
			.Help("Остановить построение");
	else
		menu.добавь(b, "Построить", IdeImg::build_make(), THISBACK(DoBuild))
			.Ключ(AK_BUILD)
			.Help("Выполнить минимальную перестройку приложения");
	b = b && idestate == EDITING;
	menu.добавь(b, AK_CLEAN, THISBACK(Clean))
		.Help("Удалить все промежуточные файлы");
//	menu.добавь("переустанов BLITZ", [=] { ResetBlitz(); })
//	    .Help("All files will be considered for BLITZ, regardless of time");
	menu.добавь(b, AK_REBUILDALL, IdeImg::build_rebuild_all(), THISBACK(RebuildAll))
		.Help("Удалить все промежуточные файлы & построить");
	menu.добавь(b, AK_CLEANUPPOUT, THISBACK(CleanUppOut))
		.Help("Удалить все файлы и подпапки в выводной & промежуточной папке (см. Основа setup)");

//	menu.MenuSeparator();

//	menu.добавь(b, AK_CREATEMAKEFILE, THISBACK(CreateMakefile))
//		.Help("создай makefile enabling IDE-independent project building");

	menu.MenuSeparator();


	if(menu.барМеню_ли() && !menu.IsScanKeys())
		BuildPackageMenu(menu);

	BuildFileMenu(menu);
	
	menu.MenuSeparator();

	menu.добавь("Остановить при ошибках", THISBACK(ToggleStopOnErrors))
		.Check(stoponerrors)
		.Help("Остановить постройку после пакета с ошибками");

	menu.MenuSeparator();

	bool ff = BottomIsFindInFiles();
	Ткст hh = ff ? "позицию" : "строку ошибки";
	bool ffb = ff ? FFound().дайСчёт() : Ошибка.дайСчёт();
	menu.добавь(ffb, AK_FINDNEXTERROR, THISBACK(FindNextError))
		.Help("Найти следующую " + hh + "согласно панели консоли");
	menu.добавь(ffb, AK_FINDPREVERROR, THISBACK(FindPrevError))
		.Help("Найти предыдущую " + hh + "согласно панели консоли");
	menu.MenuSeparator();
	menu.добавь(!пусто_ли(target), AK_OPENOUTDIR, [=] { ShellOpenFolder(дайПапкуФайла(target)); });
	menu.добавь(!пусто_ли(target), AK_COPYOUTDIR, [=] { WriteClipboardText(дайПапкуФайла(target)); });
	menu.добавь(!пусто_ли(target), AK_COPYTARGET, [=] { WriteClipboardText(target); });
	menu.добавь(!пусто_ли(target), AK_OUTDIRTERMINAL, [=] { LaunchTerminal(дайПапкуФайла(target)); });
}

void Иср::DebugMenu(Бар& menu)
{
	bool b = idestate == EDITING && !исрОтладБлокировка_ли();
	if(debugger) {
		debugger->DebugBar(menu);
		menu.MenuSeparator();
	}
	else {
		if(console.пущен())
			menu.добавь("Стоп!", THISBACK(StopDebug))
			    .Help("Остановить подконтрольный процесс");
		if(menu.барМеню_ли())
			menu.добавь(AK_RUNOPTIONS, THISBACK(RunArgs))
				.Help("Текущая папка, командная строка, перенаправ на стдвыв");
		menu.добавь(b, AK_EXECUTE, IdeImg::execute(), THISBACK(BuildAndExecute))
			.Help("Построить и выполнить приложение");
		menu.добавь(b, AK_DEBUG, IdeImg::debug_run(), THISBACK1(BuildAndDebug, false))
			.Help("Построить приложение & запустить отладчик");
		if(menu.барМеню_ли()) {
			menu.добавь(b, AK_DEBUGTO, THISBACK1(BuildAndDebug, true))
				.Help("Построить приложение & Выполнить до курсора в отладчике");
			menu.добавь(b, AK_DEBUGEXT, THISBACK(BuildAndExtDebug))
				.Help("Построить приложение & запустить внешний отладчик (см. Основа setup, default \"msdev.exe\")");
			menu.добавь(b, AK_DEBUGFILEEXT, THISBACK(BuildAndExtDebugFile))
				.Help("Построить приложение & запустить внешний отладчик, пытаться начать с текущей строки");
		#ifdef PLATFORM_POSIX
			if(IsValgrind())
				menu.добавь(b, AK_VALGRIND, THISBACK(Valgrind))
					.Help("Построить приложение & запустить в valgring");
		#endif

			menu.Separator();
		}
	}
	if(menu.барМеню_ли()) {
		menu.добавь(!editfile.пустой() /*&& !debuglock*/, AK_BREAKPOINT, THISBACK(DebugToggleBreak))
			.Help("Установить / очистить остановы на текущей строке");
		menu.добавь(!editfile.пустой(), AK_CONDBREAKPOINT, THISBACK(ConditionalBreak))
			.Help("Редактировать условные остановы");
		menu.добавь(!editfile.пустой() /*&& !debuglock*/, AK_CLEARBREAKPOINTS, THISBACK(DebugClearBreakpoints))
			.Help("Очистить все точки останова");
		menu.Separator();
		
		Ткст targetLogPath = GetTargetLogPath();
		menu.добавь(target.дайСчёт() && файлЕсть(targetLogPath), AK_OPENLOG, THISBACK1(OpenLog, targetLogPath));
	}
}

void Иср::BrowseMenu(Бар& menu)
{
	if(!IsEditorMode()) {
		if(menu.барМеню_ли()) {
			menu.добавьМеню(AK_NAVIGATOR, IdeImg::Navigator(), THISBACK(ToggleNavigator))
				.Check(editor.IsNavigator())
				.вкл(!designer);
			menu.добавь(AK_GOTO, THISBACK(SearchCode))
				.вкл(!designer)
				.Help("Перейти к заданной строке");
			menu.добавь(AK_GOTOGLOBAL, THISBACK(NavigatorDlg));
			menu.добавь(!designer, AK_JUMPS, THISBACK(ContextGoto));
			menu.добавь(!designer, AK_SWAPS, THISBACK(SwapS));
			menu.добавь(!designer, AK_ASSIST, callback(&editor, &AssistEditor::Assist));
			menu.добавь(!designer, AK_DCOPY, callback(&editor, &AssistEditor::DCopy));
			menu.добавь(!designer, AK_VIRTUALS, callback(&editor, &AssistEditor::Virtuals));
			menu.добавь(!designer, AK_THISBACKS, callback(&editor, &AssistEditor::Thisbacks));
			menu.добавь(!designer, AK_COMPLETE, callback(&editor, &AssistEditor::Complete));
			menu.добавь(!designer, AK_ABBR, callback(&editor, &AssistEditor::Abbr));
			menu.добавь(!designer, AK_GO_TO_LINE, THISBACK(GoToLine));
			AssistEdit(menu);
			menu.MenuSeparator();
		}
		
		menu.добавь("Назад", IdeImg::AssistGoBack(), THISBACK1(History, -1))
			.Ключ(K_ALT_LEFT)
			.вкл(GetHistory(-1) >= 0);
		menu.добавь("Вперёд", IdeImg::AssistGoForward(), THISBACK1(History, 1))
			.Ключ(K_ALT_RIGHT)
			.вкл(GetHistory(1) >= 0);
		
		if(menu.барМеню_ли()) {
			menu.MenuSeparator();
			menu.добавь("Преверка на изменения в исходниках", THISBACK(CheckCodeBase));
			menu.добавь("Сканировать все исходники повторно", THISBACK(RescanCode));
			if(!auto_rescan)
				menu.добавь(AK_RESCANCURRENTFILE, THISBACK(EditFileAssistSync));
			menu.MenuSeparator();
		}
	}
	else {
		menu.добавь(!designer, AK_GO_TO_LINE, THISBACK(GoToLine));
		menu.MenuSeparator();
	}
	
	if(menu.барМеню_ли()) {
		menu.добавьМеню(AK_CALC, IdeImg::calc(), THISBACK1(ToggleBottom, BCALC))
	     .Check(IsBottomShown() && btabs.дайКурсор() == BCALC);
		menu.добавьМеню(AK_QTF, IdeCommonImg::Qtf(), THISBACK(Qtf));
		menu.добавьМеню(!designer, AK_XML, IdeCommonImg::xml(), THISBACK(Xml));
		menu.добавьМеню(!designer, AK_JSON, IdeCommonImg::json(), THISBACK(Json));
		menu.добавь(AK_REFORMAT_JSON, [=] { FormatJSON_XML_File(false); });
		menu.добавь(AK_REFORMAT_XML, [=] { FormatJSON_XML_File(true); });
		menu.добавьМеню(!designer, AK_ASERRORS, IdeImg::errors(), THISBACK(AsErrors));
		menu.добавьМеню(AK_DIRDIFF, DiffImg::DirDiff(), THISBACK(DoDirDiff));
		menu.добавьМеню(AK_PATCH, DiffImg::PatchDiff(), THISBACK(DoPatchDiff));
	}
}

void Иср::HelpMenu(Бар& menu)
{
	if(!IsEditorMode()) {
		menu.добавь(AK_BROWSETOPICS, IdeImg::help(), THISBACK(ShowTopics));
		menu.добавь(editor.GetWord().дайСчёт(), AK_SEARCHTOPICS, THISBACK(SearchTopics));
	}
	menu.добавь(AK_BROWSETOPICS_WIN, IdeImg::help_win(), THISBACK(ShowTopicsWin));
	menu.MenuSeparator();
	menu.добавьМеню("Получить помощь / отчитаться о багах..", IdeImg::Go_forward(), callback1(запустиВебБраузер, "http://www.ultimatepp.org/forums"));
	menu.добавьМеню("Онлайн-документация..", IdeImg::Go_forward(), callback1(запустиВебБраузер, "http://www.ultimatepp.org/www$uppweb$documentation$en-us.html"));
	menu.добавьМеню("Общая информация..", IdeImg::Go_forward(), callback1(запустиВебБраузер, "http://www.ultimatepp.org/www$uppweb$community$en-us.html"));
	menu.Separator();
	OnlineSearchMenu(menu);
	if(menu.барМеню_ли()) {
		menu.Separator();
		menu.добавь(файлЕсть(GetIdeLogPath()), "Просмотреть лог приложения", THISBACK(ViewIdeLogFile));
		menu.Separator();
		menu.добавь("О программе..", IdeImg::info(), THISBACK(About));
	}
}

void Иср::MainMenu(Бар& menu)
{
	menu.добавь("Файл", THISBACK(Файл))
		.Help("Пакетные & файловые функци, экспорты, закладки");
	menu.добавь("Правка", THISBACK(Edit))
		.Help("Буфер обмена, поиск & замена, преобразования пробелов / табуляций, подсветка масштаба");
	if(HasMacros())
		menu.добавь("Макросы", THISBACK(MacroMenu))
			.Help("Редактор & макросы ИСР");
	menu.добавь("Проект", THISBACK(Project))
		.Help("Организатор пакета, кастомные шани, менеджер конфигурации");
	if(!IsEditorMode()) {
		menu.добавь("Постройка", THISBACK(BuildMenu))
			.Help("Постройка & отладка, незначительные опции построения, показ ошибок");
		menu.добавь("Отладка", THISBACK(DebugMenu))
			.Help("Отладочные команды (поддерживается пока только подключение gdb)");
	}
	menu.добавь("Ассист", THISBACK(BrowseMenu))
		.Help("Информация, просмотр кода и поддержка");
	menu.добавь("Настройка", THISBACK(настрой))
		.Help("Пути, настройки редактора, подключение к удалённому хосту");
	menu.добавь("Помощь", THISBACK(HelpMenu))
		.Help("Помощь, доверие и лицензия");
}

void Иср::MainTool(Бар& bar)
{
	if(!IsEditorMode()) {
		BrowseMenu(bar);
		bar.Separator();
	}
	bar.добавь("Редактировать как текст", IdeImg::EditText(), THISBACK(EditAsText))
	   .Check(!designer)
	   .вкл(!editfile_isfolder)
	   .Ключ(!designer ? 0 : K_CTRL_T);
	bool b = designer && !designer.является<FileHexView>();
	bool d = IsDesignerFile(editfile);
	bar.добавь("Редактировать с помощью дизайнера", IdeImg::EditDesigner(), THISBACK(EditUsingDesigner))
	   .Check(b || editfile_isfolder)
	   .вкл(d || editfile_isfolder)
	   .Ключ(b || !d ? 0 : K_CTRL_T);
	bar.добавь("Просмотреть как гекс", IdeImg::EditHex(), THISBACK(EditAsHex))
	   .Check(designer.является<FileHexView>())
	   .вкл(!editfile_isfolder)
	   .Ключ(K_CTRL_B);
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
	bar.добавь(дисплей, HorzLayoutZoom(170));
#endif
}

void Иср::ConsoleMenu(Бар& menu)
{
	bool selection = console.IsAnySelection();
	menu.добавь("Копировать", CtrlImg::copy(), THISBACK(ConsoleCopy))
		.Ключ(K_CTRL_C)
		.вкл(selection)
		.Help("Копировать выделение в системный буфер обмена");
	menu.добавь("Вставить", CtrlImg::paste(), THISBACK(ConsolePaste))
		.Ключ(K_CTRL_V)
		.Help("Добавить выделение в системную консоль");
	menu.Separator();
	menu.добавь(AK_FIND, [=] {
		console.FindReplace(false, true, false);
	});
	menu.Separator();
	menu.добавь("Очистить", THISBACK(ConsoleClear))
		.Help("Очистить системную консоль");
}

void Иср::SetBar()
{
	SetMenuBar();
	SetToolBar();
}

void Иср::SetMenuBar()
{
#ifdef PLATFORM_COCOA
	SetMainMenu(THISBACK(MainMenu));
	menubar.скрой();
#else
	menubar.уст(THISBACK(MainMenu));
#endif
}

void Иср::SetToolBar()
{
	toolbar.уст(THISBACK(MainTool));
}
