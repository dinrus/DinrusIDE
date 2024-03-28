#include "MacroManager.h"

#include <DinrusIDE/Common/Common.h>

#define IMAGECLASS MacroManagerImg
#define IMAGEFILE  <DinrusIDE/MacroManager/MacroManager.iml>
#include <Draw/iml_source.h>

#define METHOD_NAME "MacroManagerWindow::" << UPP_FUNCTION_NAME << "(): "

namespace Upp {

MacroManagerWindow::MacroManagerWindow(const Workspace& wspc, const String& hlStyles)
	: wspc(wspc)
	, globalMacrosChanged(false)
{
	CtrlLayout(*this, t_("Менеджер Макросов"));
	Zoomable().Sizeable().MinimizeBox(false);
	globalTree.NoRoot();
	localTree.NoRoot();
	
	parent.Add(editor.SizePos());
	parent.AddFrame(splitter.Left(tab, 330));
	tab.Add(globalTree.SizePos(), t_("Глобальные макросы"));

	editor.Highlight("usc");
	editor.SetReadOnly();

	LoadMacros();

	editor.Hide();
	editor.LoadHlStyles(hlStyles);
	
	InitButtons();
	InitEvents();
	InitToolBar();
}

void MacroManagerWindow::InitToolBar()
{
	InitToolButton(editLabel, t_("Редактировать"), MacroManagerImg::PluginEdit());
	InitToolButton(exportLabel, t_("Экспортировать текущее"), MacroManagerImg::PluginGo());
	tool.Separator();
	InitToolButton(newGlobalLabel, t_("Новый глобальный"), IdeCommonImg::PageAdd());
	InitToolButton(importGlobalsLabel, t_("Импортировать глобальные"), MacroManagerImg::PluginAdd());
	InitToolButton(exportGlobalsLabel, t_("Экспортировать глобальные"), MacroManagerImg::ArrowRight());
	
	newGlobalLabel.Enable();
	importGlobalsLabel.Enable();
	exportGlobalsLabel.Enable(globalTree.GetChildCount(0));
}

void MacroManagerWindow::InitToolButton(
	ToolButton& toolButton, const String& label, const Image& image)
{
	tool.Add(toolButton.Label(label));
	toolButton.Image(image);
	toolButton.SizePos();
	toolButton.Disable();
}

void MacroManagerWindow::InitButtons()
{
	close.Close();
	
	close              << [=, this] { Break(); };
	help               << [=] { LaunchWebBrowser("https://www.ultimatepp.org/app$ide$MacroManager_en-us.html"); };
	editLabel          << [=, this] { OnEditFile(); };
	exportLabel        << [=, this] { OnExport(globalTree.GetCursor()); };
	newGlobalLabel     << [=, this] { OnNewMacroFile(); };
	importGlobalsLabel << [=, this] { OnImport(); };
	exportGlobalsLabel << [=, this] { OnExport(0); };
	
	editLabel.Tip(t_("Редактировать текущий выбранный макрос внутри DinrusIDE.."));
	exportLabel.Tip(t_("Экспортировать выделенный макрос в файл.."));
	newGlobalLabel.Tip(t_("Создать новый файл с глобальными макросами.."));
	importGlobalsLabel.Tip(t_("Установить файл/файлы с макросами.."));
	exportGlobalsLabel.Tip(t_("Экспортировать все файлы глобальных макросов.."));
}

void MacroManagerWindow::InitEvents()
{
	globalTree.WhenSel = [=, this]           { OnTreeSel(); };
	localTree.WhenSel  = [=, this]           { OnTreeSel(); };
	
	globalTree.WhenBar = [=, this](Bar& bar) { OnMacroBar(bar); };
	localTree.WhenBar  = [=, this](Bar& bar) { OnMacroBar(bar); };
	
	tab.WhenSet        = [=, this]           { OnTabSet(); };
}

void MacroManagerWindow::OnMacroBar(Bar& bar)
{
	if(IsGlobalTab()) {
		bool partOfFile = IsGlobalFile();
		
		bar.Add(t_("Новый.."),    [=, this] { OnNewMacroFile(); });
		bar.Add(t_("Импортировать.."), [=, this] { OnImport(); });
		bar.Add(t_("Удалить"),   [=, this] { OnDeleteMacroFile(); })
		    .Enable(partOfFile);
		bar.Add(t_("Экспортировать.."), [=, this] { OnExport(globalTree.GetCursor()); })
		    .Enable(partOfFile);
		bar.Separator();
	}
	bar.Add(t_("Редактировать"), [=, this] { OnEditFile();})
	    .Enable(IsEditPossible());
}

void MacroManagerWindow::Layout()
{
	OnTreeSel();
}

void MacroManagerWindow::OnTreeSel()
{
	const TreeCtrl& tree = GetCurrentTree();
	bool hasCursor = tree.IsCursor();
	exportLabel.Enable(IsGlobalTab() && IsGlobalFile());
	
	editLabel.Enable(hasCursor && IsEditPossible());

	editor.Show(editLabel.IsEnabled());
	
	if(IsFile())
		editor.Set(LoadFile(static_cast<String>(tree.Get())));
	else if(IsMacro())
		editor.Set(ValueTo<MacroElement>(tree.Get()).GetContent());
}

void MacroManagerWindow::OnTabSet()
{
	exportLabel.Enable(tab.Get() == 0);
	editor.Hide();
	
	OnTreeSel();
}

void MacroManagerWindow::OnImport()
{
	auto filePath = SelectFileOpen("*.usc");
	if(IsNull(filePath))
		return;
	
	if(!UscFileParser(filePath).IsValid()) {
		ErrorOK(DeQtf(String(t_("Неудачный импорт! Следующий файл")) << " \"" << filePath << "\" " << t_("не является валидным файлом макросов!")));
		return;
	}
	
	auto localDir = GetLocalDir();
	if(!DirectoryExists(localDir) && !RealizeDirectory(localDir)) {
		ErrorOK(DeQtf(String(t_("Реализация директории")) << " \"" << localDir << "\" " << t_("провалилась.")));
		return;
	}
	
	auto newFileName = GetFileName(filePath);
	auto newFilePath = LocalPath(newFileName);
	if(FileExists(newFilePath) && !PromptYesNo(DeQtf(GenFileOverrideMessage(newFileName))))
		return;
	
	FileCopy(filePath, newFilePath);
	ReloadGlobalMacros();
	OnTreeSel();
	
	OnGlobalMacrosChanged();
}

void MacroManagerWindow::ExportFiles(Index<String>& files, const String& dir)
{
	for(const auto& file : files) {
		auto fileName = GetFileName(file);
		auto filePath = AppendFileName(dir, GetFileName(file));
		
		if(FileExists(filePath) && !PromptYesNo(DeQtf(GenFileOverrideMessage(fileName))))
			continue;
		
		FileCopy(file, filePath);
	}
}

void MacroManagerWindow::FindNodeFiles(int id, Index<String>& list)
{
	if(IsFile(id)) {
		list.FindAdd((String)globalTree.Get(id));
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
		if(dir.IsEmpty())
			return;
		
		Index<String> list;
		FindNodeFiles(id, list);
		ExportFiles(list, dir);
	}
}

void MacroManagerWindow::OnEditFile()
{
	const TreeCtrl& tree = GetCurrentTree();
	if(!tree.IsCursor())
		return;

	if(IsMacro()) {
		MacroElement element = ValueTo<MacroElement>(tree.Get());
		WhenEdit(element.fileName, element.line - 1);
		Break();
	}
	else if(IsFile()) {
		WhenEdit(tree.Get(), 1);
		Break();
	}
}

void MacroManagerWindow::OnNewMacroFile()
{
	String fileName;
	if(!EditTextNotNull(fileName, t_("Новый файл глобальных макросов"), t_("Имя файла макросов:")))
		return;
	
	if(!fileName.EndsWith(".usc"))
		fileName << ".usc";
	
	String fullPath = AppendFileName(GetLocalDir(), fileName);
	RealizeDirectory(GetLocalDir());
	if(FileExists(fullPath)) {
		PromptOK(String() << t_("файл") << " \"" << fileName << "\" " << t_("уже существует!"));
		return;
	}
	
	if(!SaveFile(fullPath, "macro \"" + GetFileTitle(fileName) + "\" {}")) {
		PromptOK(String() << t_("Ошибка при сохранении файла") << " \"" << fileName << "\"");
		return;
	}

	int fileNode = globalTree.Add(0, Image(), fullPath, fileName);
	auto list = UscFileParser(fullPath).Parse();
	for(const auto& element : list) {
		globalTree.Add(fileNode, element.GetImage(), RawToValue(element), element.name);
	}
	
	globalTree.OpenDeep(fileNode);
	globalTree.FindSetCursor(fullPath);
	
	OnGlobalMacrosChanged();
}

void MacroManagerWindow::OnDeleteMacroFile()
{
	auto fileName = static_cast<String>(globalTree.GetValue());
	if(!PromptOKCancel(String(t_("Вы действительно хотите удалить следующий файл макросов")) << " \"" << fileName << "\"?")) {
		return;
	}
	
	FileDelete(AppendFileName(GetLocalDir(), fileName));
	globalTree.Remove(globalTree.GetCursor());
	OnTreeSel();
	
	OnGlobalMacrosChanged();
}

void MacroManagerWindow::OnGlobalMacrosChanged()
{
	exportGlobalsLabel.Enable(globalTree.GetChildCount(0));
	globalMacrosChanged = true;
}

String MacroManagerWindow::GenFileOverrideMessage(const String& fileName)
{
	return String(t_("Целевой файл")) << " \"" << fileName << "\" " << t_("уже существует! Желаете его перезаписать?");
}

void MacroManagerWindow::LoadUscDir(const String& dir)
{
	for(FindFile ff(AppendFileName(dir, "*.usc")); ff; ff.Next()) {
		String fileTitle = ff.GetName();
		if(!ff.GetPath().EndsWith(String() << "DinrusLocal" << DIR_SEPS << ff.GetName()))
			fileTitle = "../" + fileTitle;
		
		int fileNode = globalTree.Add(0, Image(), ff.GetPath(), fileTitle);

		auto list = UscFileParser(ff.GetPath()).Parse();
		for(const auto& element : list)
			globalTree.Add(fileNode, element.GetImage(), RawToValue(element), element.name);
	}
}

void MacroManagerWindow::LoadMacros()
{
	ReloadGlobalMacros();
	ReloadLocalMacros();
}

void MacroManagerWindow::ReloadGlobalMacros()
{
	globalTree.Clear();

	LoadUscDir(GetLocalDir());
	LoadUscDir(GetFileFolder(ConfigFile("x")));
	
	globalTree.OpenDeep(0);
}

void MacroManagerWindow::ReloadLocalMacros()
{
	for(int i = 0; i < wspc.GetCount(); i++) {
		const auto& package = wspc.GetPackage(i);
		int packageNode = -1;
		for (const auto& file : package.file) {
			auto filePath = SourcePath(wspc[i], file);

			if (ToLower(GetFileExt(filePath)) != ".usc")
				continue;
				
			auto list = UscFileParser(filePath).Parse();
			if (list.GetCount() == 0)
				continue;
			
			if(tab.GetCount() == 1)
				tab.Add(localTree.SizePos(), t_("Локальные макросы (Только чтение)"));
			
			if(packageNode == -1)
				packageNode = localTree.Add(0, Image(), 0, wspc[i]);
					
			int fileNode = localTree.Add(packageNode, Image(), filePath, file);
			for(int j = 0; j < list.GetCount(); j++) {
				auto& macroElement = list[j];
				localTree.Add(fileNode, macroElement.GetImage(), RawToValue(macroElement), macroElement.name);
			}
		}
	}

	localTree.OpenDeep(0);
}

}
