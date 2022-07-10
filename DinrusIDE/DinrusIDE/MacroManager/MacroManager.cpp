#include "MacroManager.h"

#include <DinrusIDE/Common/Common.h>

#define IMAGECLASS MacroManagerImg
#define IMAGEFILE  <DinrusIDE/MacroManager/MacroManager.iml>
#include <Draw/iml_source.h>

#define METHOD_NAME "MacroManagerWindow::" << UPP_FUNCTION_NAME << "(): "

namespace РНЦП {

MacroManagerWindow::MacroManagerWindow(const РОбласть& wspc, const Ткст& hlStyles)
	: wspc(wspc)
	, globalMacrosChanged(false)
{
	CtrlLayout(*this, t_("Macro Manager"));
	Zoomable().Sizeable().MinimizeBox(false);
	globalTree.NoRoot();
	localTree.NoRoot();
	
	parent.добавь(editor.SizePos());
	parent.добавьФрейм(splitter.лево(tab, 330));
	tab.добавь(globalTree.SizePos(), t_("Global macros"));

	editor.Highlight("usc");
	editor.устТолькоЧтен();

	LoadMacros();

	editor.скрой();
	editor.LoadHlStyles(hlStyles);
	
	InitButtons();
	InitСобытиеs();
	InitToolBar();
}

void MacroManagerWindow::InitToolBar()
{
	InitToolButton(editLabel, t_("Edit"), MacroManagerImg::PluginEdit());
	InitToolButton(exportLabel, t_("Export current"), MacroManagerImg::PluginGo());
	tool.Separator();
	InitToolButton(newGlobalLabel, t_("нов global"), IdeCommonImg::PageAdd());
	InitToolButton(importGlobalsLabel, t_("Import globals"), MacroManagerImg::PluginAdd());
	InitToolButton(exportGlobalsLabel, t_("Export globals"), MacroManagerImg::ArrowRight());
	
	newGlobalLabel.вкл();
	importGlobalsLabel.вкл();
	exportGlobalsLabel.вкл(globalTree.GetChildCount(0));
}

void MacroManagerWindow::InitToolButton(
	ToolButton& toolButton, const Ткст& label, const Рисунок& image)
{
	tool.добавь(toolButton.Надпись(label));
	toolButton.Рисунок(image);
	toolButton.SizePos();
	toolButton.откл();
}

void MacroManagerWindow::InitButtons()
{
	close.закрой();
	
	close              << [=] { Break(); };
	help               << [=] { запустиВебБраузер("https://www.ultimatepp.org/app$ide$MacroManager_en-us.html"); };
	editLabel          << [=] { OnEditFile(); };
	exportLabel        << [=] { OnExport(globalTree.дайКурсор()); };
	newGlobalLabel     << [=] { OnNewMacroFile(); };
	importGlobalsLabel << [=] { OnImport(); };
	exportGlobalsLabel << [=] { OnExport(0); };
	
	editLabel.Подсказка(t_("Edit currently selected macro inside TheIde.."));
	exportLabel.Подсказка(t_("Export selected macro file.."));
	newGlobalLabel.Подсказка(t_("создай new file that stores global macros.."));
	importGlobalsLabel.Подсказка(t_("Install file/files containing macros.."));
	exportGlobalsLabel.Подсказка(t_("Export all global macros files.."));
}

void MacroManagerWindow::InitСобытиеs()
{
	globalTree.WhenSel = [=]           { OnTreeSel(); };
	localTree.WhenSel  = [=]           { OnTreeSel(); };
	
	globalTree.WhenBar = [=](Бар& bar) { OnMacroBar(bar); };
	localTree.WhenBar  = [=](Бар& bar) { OnMacroBar(bar); };
	
	tab.КогдаУст        = [=]           { OnTabSet(); };
}

void MacroManagerWindow::OnMacroBar(Бар& bar)
{
	if(IsGlobalTab()) {
		bool partOfFile = IsGlobalFile();
		
		bar.добавь(t_("нов.."),    [=] { OnNewMacroFile(); });
		bar.добавь(t_("Import.."), [=] { OnImport(); });
		bar.добавь(t_("Delete"),   [=] { OnDeleteMacroFile(); })
		    .вкл(partOfFile);
		bar.добавь(t_("Export.."), [=] { OnExport(globalTree.дайКурсор()); })
		    .вкл(partOfFile);
		bar.Separator();
	}
	bar.добавь(t_("Edit"), [=] { OnEditFile();})
	    .вкл(IsEditPossible());
}

void MacroManagerWindow::Выкладка()
{
	OnTreeSel();
}

void MacroManagerWindow::OnTreeSel()
{
	const КтрлДерево& tree = GetCurrentTree();
	bool hasCursor = tree.курсор_ли();
	exportLabel.вкл(IsGlobalTab() && IsGlobalFile());
	
	editLabel.вкл(hasCursor && IsEditPossible());

	editor.покажи(editLabel.включен_ли());
	
	if(файл_ли())
		editor.уст(загрузиФайл(static_cast<Ткст>(tree.дай())));
	else if(IsMacro())
		editor.уст(ValueTo<MacroElement>(tree.дай()).GetContent());
}

void MacroManagerWindow::OnTabSet()
{
	exportLabel.вкл(tab.дай() == 0);
	editor.скрой();
	
	OnTreeSel();
}

void MacroManagerWindow::OnImport()
{
	auto filePath = SelectFileOpen("*.usc");
	if(пусто_ли(filePath))
		return;
	
	if(!UscFileParser(filePath).пригоден()) {
		ErrorOK(DeQtf(Ткст(t_("Import failed! Following file")) << " \"" << filePath << "\" " << t_("is not a valid macro file!")));
		return;
	}
	
	auto localDir = GetLocalDir();
	if(!дирЕсть(localDir) && !реализуйДир(localDir)) {
		ErrorOK(DeQtf(Ткст(t_("Realizing directory")) << " \"" << localDir << "\" " << t_("failed.")));
		return;
	}
	
	auto newFileName = дайИмяф(filePath);
	auto newFilePath = LocalPath(newFileName);
	if(файлЕсть(newFilePath) && !PromptYesNo(DeQtf(GenFileOverrideMessage(newFileName))))
		return;
	
	копируйфл(filePath, newFilePath);
	ReloadGlobalMacros();
	OnTreeSel();
	
	OnGlobalMacrosChanged();
}

void MacroManagerWindow::ExportFiles(Индекс<Ткст>& files, const Ткст& dir)
{
	for(const auto& file : files) {
		auto fileName = дайИмяф(file);
		auto filePath = приставьИмяф(dir, дайИмяф(file));
		
		if(файлЕсть(filePath) && !PromptYesNo(DeQtf(GenFileOverrideMessage(fileName))))
			continue;
		
		копируйфл(file, filePath);
	}
}

void MacroManagerWindow::FindNodeFiles(int id, Индекс<Ткст>& list)
{
	if(файл_ли(id)) {
		list.найдиДобавь((Ткст)globalTree.дай(id));
		return;
	}
	
	for(int i = 0; i < globalTree.GetChildCount(id); i++) {
		int node = globalTree.GetChild(id, i);
		FindNodeFiles(node, list);
	}
}

void MacroManagerWindow::OnExport(int id)
{
	if(id == 0 || IsGlobalFile()) {
		auto dir = SelectDirectory();
		if(dir.пустой())
			return;
		
		Индекс<Ткст> list;
		FindNodeFiles(id, list);
		ExportFiles(list, dir);
	}
}

void MacroManagerWindow::OnEditFile()
{
	const КтрлДерево& tree = GetCurrentTree();
	if(!tree.курсор_ли())
		return;

	if(IsMacro()) {
		MacroElement element = ValueTo<MacroElement>(tree.дай());
		WhenEdit(element.fileName, element.line - 1);
		Break();
	}
	else if(файл_ли()) {
		WhenEdit(tree.дай(), 1);
		Break();
	}
}

void MacroManagerWindow::OnNewMacroFile()
{
	Ткст fileName;
	if(!EditTextNotNull(fileName, t_("нов global macro file"), t_("Macro file имя:")))
		return;
	
	if(!fileName.заканчиваетсяНа(".usc"))
		fileName << ".usc";
	
	Ткст fullPath = приставьИмяф(GetLocalDir(), fileName);
	реализуйДир(GetLocalDir());
	if(файлЕсть(fullPath)) {
		PromptOK(Ткст() << t_("file") << " \"" << fileName << "\" " << t_("already exists!"));
		return;
	}
	
	if(!сохраниФайл(fullPath, "macro \"" + дайТитулф(fileName) + "\" {}")) {
		PromptOK(Ткст() << t_("Ошибка occured while saving file") << " \"" << fileName << "\"");
		return;
	}

	int fileNode = globalTree.добавь(0, Рисунок(), fullPath, fileName);
	auto list = UscFileParser(fullPath).Parse();
	for(const auto& element : list) {
		globalTree.добавь(fileNode, element.GetImage(), RawToValue(element), element.имя);
	}
	
	globalTree.OpenDeep(fileNode);
	globalTree.FindSetCursor(fullPath);
	
	OnGlobalMacrosChanged();
}

void MacroManagerWindow::OnDeleteMacroFile()
{
	auto fileName = static_cast<Ткст>(globalTree.дайЗначение());
	if(!PromptOKCancel(Ткст(t_("Are you sure you want to remove following macro file")) << " \"" << fileName << "\"?")) {
		return;
	}
	
	удалифл(приставьИмяф(GetLocalDir(), fileName));
	globalTree.удали(globalTree.дайКурсор());
	OnTreeSel();
	
	OnGlobalMacrosChanged();
}

void MacroManagerWindow::OnGlobalMacrosChanged()
{
	exportGlobalsLabel.вкл(globalTree.GetChildCount(0));
	globalMacrosChanged = true;
}

Ткст MacroManagerWindow::GenFileOverrideMessage(const Ткст& fileName)
{
	return Ткст(t_("Target file")) << " \"" << fileName << "\" " << t_("already exists! Do you want to overwrite it?");
}

void MacroManagerWindow::LoadUscDir(const Ткст& dir)
{
	for(ФайлПоиск ff(приставьИмяф(dir, "*.usc")); ff; ff.следщ()) {
		Ткст fileTitle = ff.дайИмя();
		if(!ff.дайПуть().заканчиваетсяНа(Ткст() << "UppLocal" << DIR_SEPS << ff.дайИмя()))
			fileTitle = "../" + fileTitle;
		
		int fileNode = globalTree.добавь(0, Рисунок(), ff.дайПуть(), fileTitle);

		auto list = UscFileParser(ff.дайПуть()).Parse();
		for(const auto& element : list)
			globalTree.добавь(fileNode, element.GetImage(), RawToValue(element), element.имя);
	}
}

void MacroManagerWindow::LoadMacros()
{
	ReloadGlobalMacros();
	ReloadLocalMacros();
}

void MacroManagerWindow::ReloadGlobalMacros()
{
	globalTree.очисть();

	LoadUscDir(GetLocalDir());
	LoadUscDir(дайПапкуФайла(конфигФайл("x")));
	
	globalTree.OpenDeep(0);
}

void MacroManagerWindow::ReloadLocalMacros()
{
	for(int i = 0; i < wspc.дайСчёт(); i++) {
		const auto& package = wspc.дайПакет(i);
		int packageNode = -1;
		for (const auto& file : package.file) {
			auto filePath = SourcePath(wspc[i], file);

			if (впроп(дайРасшф(filePath)) != ".usc")
				continue;
				
			auto list = UscFileParser(filePath).Parse();
			if (list.дайСчёт() == 0)
				continue;
			
			if(tab.дайСчёт() == 1)
				tab.добавь(localTree.SizePos(), t_("Local macros (читай only)"));
			
			if(packageNode == -1)
				packageNode = localTree.добавь(0, Рисунок(), 0, wspc[i]);
					
			int fileNode = localTree.добавь(packageNode, Рисунок(), filePath, file);
			for(int j = 0; j < list.дайСчёт(); j++) {
				auto& macroElement = list[j];
				localTree.добавь(fileNode, macroElement.GetImage(), RawToValue(macroElement), macroElement.имя);
			}
		}
	}

	localTree.OpenDeep(0);
}

}
