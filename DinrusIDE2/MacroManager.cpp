#include "DinrusIDE2.h"

#include <DinrusIDE2/MacroManager/MacroManager.h>

void Ide::DoMacroManager()
{
	MacroManagerWindow manager(IdeWorkspace(), editor.StoreHlStyles());
	manager.WhenEdit = [=](String fileName, int line) {
		EditFile(fileName);
		editor.SetCursor(editor.GetPos64(line));
		editor.CenterCursor();
	};
	manager.Execute();
	
	if (manager.IsGlobalMacrosChanged()) {
		SyncUsc();
		SetMenuBar();
	}
}
