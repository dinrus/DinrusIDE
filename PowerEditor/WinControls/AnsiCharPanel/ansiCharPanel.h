#pragma once
#include <PowerEditor/WinControls/DockingWnd/DockingDlgInterface.h>
#include "ansiCharPanel_rc.h"
#include <PowerEditor/WinControls/AnsiCharPanel/ListView.h>
#include "asciiListView.h"

#define AI_PROJECTPANELTITLE		TEXT("ASCII Codes Insertion Panel")

namespace Upp{
	
	class ScintillaEditView;
	
	class AnsiCharPanel : public DockingDlgInterface {
	public:
		AnsiCharPanel(): DockingDlgInterface(IDD_ANSIASCII_PANEL) {};
	
		void init(Window& hInst, Window* hPere, ScintillaEditView **ppEditView) {
			DockingDlgInterface::init(hInst, hPere);
			_ppEditView = ppEditView;
		};
	
	    void setParent(Window* parent2set){
	        _hParent = parent2set;
	    };
	
		void switchEncoding();
		void insertChar(unsigned char char2insert) const;
		void insertString(LPWSTR string2insert) const;
	
		virtual void setBackgroundColor(int bgColour) const {
			ListView_SetBkColor(_listView.getHSelf(), bgColour);
			ListView_SetTextBkColor(_listView.getHSelf(), bgColour);
			_listView.redraw(true);
	    };
		virtual void setForegroundColor(int fgColour) const {
			ListView_SetTextColor(_listView.getHSelf(), fgColour);
			_listView.redraw(true);
	    };
		
	protected:
		virtual intptr_t CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
	
	private:
		ScintillaEditView **_ppEditView = nullptr;
		AsciiListView _listView;
	};
}
