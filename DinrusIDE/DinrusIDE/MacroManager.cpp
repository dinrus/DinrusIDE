#include "DinrusIDE.h"

#include <DinrusIDE/MacroManager/MacroManager.h>

void Иср::DoMacroManager()
{
	MacroManagerWindow manager(роблИср(), editor.StoreHlStyles());
	manager.WhenEdit = [=](Ткст fileName, int line) {
		EditFile(fileName);
		editor.устКурсор(editor.дайПоз64(line));
		editor.курсорПоЦентру();
	};
	manager.выполни();
	
	if (manager.IsGlobalMacrosChanged()) {
		SyncUsc();
		SetMenuBar();
	}
}
