#ifndef _MacroManager_MacroManager_h_
#define _MacroManager_MacroManager_h_

#include <CtrlLib/CtrlLib.h>
#include <CodeEditor/CodeEditor.h>
#include <DinrusIDE/Core/Core.h>

namespace РНЦП {

#define LAYOUTFILE <DinrusIDE/MacroManager/MacroManager.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS MacroManagerImg
#define IMAGEFILE  <DinrusIDE/MacroManager/MacroManager.iml>
#include <Draw/iml_header.h>

class MacroElement final {
public:
	enum class Type {
		MACRO,
		FUNCTION,
		UNKNOWN
	};

public:
	MacroElement(Type тип, const Ткст& fileName, int line, const Ткст& comment);
	
	Рисунок  GetImage() const;
	Ткст GetContent() const;
	
public:
	Type   тип;
	Ткст comment;
	Ткст имя;
	Ткст prototype;
	Ткст args;
	Ткст code;
	Ткст fileName;
	int    line;
};

class UscFileParser final {
public:
	using MacroList = Массив<MacroElement>;
	
public:
	UscFileParser(const Ткст& filePath);
	
	MacroList Parse();
	bool      пригоден();
	
private:
	void ReadFunction(СиПарсер& parser, const Ткст& comment, const char* prototypeBegin, MacroList& list);
	void ReadMacro(СиПарсер& parser, const Ткст& comment, const char* prototypeBegin, MacroList& list);
	void FinishRead(СиПарсер& parser, const char* prototypeBegin, MacroElement& element, MacroList& list);

private:
	static void   FindNextElement(СиПарсер& parser);
	static Ткст ReadArgs(СиПарсер& parser);
	static Ткст ReadKeyDesc(СиПарсер& parser);
	
private:
	Ткст filePath;
};

class MacroManagerWindow final : public WithMacroManagerLayout<ТопОкно> {
public:
	using MacroStore = МассивМап<Ткст, Массив<MacroElement>>;
	
public:
	MacroManagerWindow(const РОбласть& wspc, const Ткст& hlStyles);
	
	bool IsGlobalMacrosChanged() const { return globalMacrosChanged; }
	
	void Выкладка() override;
	
public:
	Событие<Ткст, int> WhenEdit;

private:
	void InitButtons();
	void InitСобытиеs();
	void InitToolBar();
	void InitToolButton(ToolButton& toolButton, const Ткст& label, const Рисунок& image);
	
	void LoadUscDir(const Ткст& dir);
	void LoadMacros();
	void ReloadGlobalMacros();
	void ReloadLocalMacros();

	void OnMacroBar(Бар& bar);
	void OnTreeSel();
	void OnTabSet();
	void OnImport();
	void OnExport(int id);
	void OnEditFile();
	void OnNewMacroFile();
	void OnDeleteMacroFile();
	void OnGlobalMacrosChanged();

	void ExportFiles(Индекс<Ткст>& files, const Ткст& dir);
	void FindNodeFiles(int id, Индекс<Ткст>& list);

private:
	static Ткст GenFileOverrideMessage(const Ткст& fileName);

	bool IsGlobalFile() const     { return globalTree.курсор_ли() && globalTree.дай().является<Ткст>(); }
	bool файл_ли() const           { return GetCurrentTree().дай().является<Ткст>(); }
	bool файл_ли(int id) const     { return globalTree.дай(id).является<Ткст>(); }
	bool IsMacro() const          { return GetCurrentTree().дай().является<MacroElement>();}
	bool IsEditPossible() const   { return файл_ли() || IsMacro(); }
	
	bool IsGlobalTab() const      { return !tab.дай(); }
	
	const КтрлДерево& GetCurrentTree() const { return tab.дай() ? localTree : globalTree;};

private:
	const РОбласть& wspc;
	
	ToolButton       editLabel;
	ToolButton       exportLabel;
	ToolButton       newGlobalLabel;
	ToolButton       importGlobalsLabel;
	ToolButton       exportGlobalsLabel;
	
	КтрлВкладка          tab;
	КтрлДерево         globalTree;
	КтрлДерево         localTree;
	
	SplitterFrame    splitter;
	РедакторКода       editor;
	
	bool             globalMacrosChanged;
};

}


#endif
