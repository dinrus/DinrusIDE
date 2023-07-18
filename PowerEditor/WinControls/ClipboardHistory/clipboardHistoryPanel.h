#pragma once

#include <PowerEditor/WinControls/DockingWnd/DockingDlgInterface.h>
#include "clipboardHistoryPanel_rc.h"
#include <vector>

#define CH_PROJECTPANELTITLE		TEXT("Clipboard History")

typedef std::vector<unsigned char> ClipboardData;

namespace Upp{
	
	class ScintillaEditView;
	
	class ByteArray {
	public:
		ByteArray() = default;
		explicit ByteArray(ClipboardData cd);
		
		~ByteArray() {
			if (_pBytes)
				delete [] _pBytes;
		};
		const unsigned char * getPointer() const {return _pBytes;};
		size_t getLength() const {return _length;};
	protected:
		unsigned char *_pBytes = nullptr;
		size_t _length = 0;
	};
	
	class StringArray : public ByteArray {
	public:
		StringArray(ClipboardData cd, size_t maxLen);
	};
	
	class ClipboardHistoryPanel : public DockingDlgInterface {
	public:
		ClipboardHistoryPanel(): DockingDlgInterface(IDD_CLIPBOARDHISTORY_PANEL), _ppEditView(nullptr), _hwndNextCbViewer(nullptr), _lbBgColor(-1), _lbFgColor(-1) {};
	
		void init(Window& hInst, Window* hPere, ScintillaEditView **ppEditView) {
			DockingDlgInterface::init(hInst, hPere);
			_ppEditView = ppEditView;
		};
	
	    void setParent(Window* parent2set){
	        _hParent = parent2set;
	    };
	
		ClipboardData getClipboadData();
		void addToClipboadHistory(ClipboardData cbd);
		int getClipboardDataIndex(ClipboardData cbd);
	
		virtual void setBackgroundColor(Color& bgColour) {
			_lbBgColor = bgColour;
	    };
		virtual void setForegroundColor(Color& fgColour) {
			_lbFgColor = fgColour;
	    };
	
		void drawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	
	protected:
		virtual intptr_t CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
	
	private:
		ScintillaEditView **_ppEditView = nullptr;
		std::vector<ClipboardData> _clipboardDataVector;
		Window* _hwndNextCbViewer = nullptr;
		int _lbBgColor = -1;
		int _lbFgColor= -1;
	
	};

}