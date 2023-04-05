#pragma once

#include <PowerEditor/WinControls/Window.h>

#define CPN_COLOURPICKED (BN_CLICKED)

namespace Upp{
	
class ColourPopup;

	class ColourPicker : public Window
	{
	public :
		ColourPicker() = default;
	    ~ColourPicker() = default;
		virtual void init(Window& hInst, Window* parent);
		virtual void destroy();
	    void setColour(Color& c) {
	        _currentColour = c;
	    };
	
		Color& getColour() const {return _currentColour;};
		bool isEnabled() {return _isEnabled;};
		void setEnabled(bool enabled) {_isEnabled = enabled;};
	
	private :
		Color& _currentColour = Color(0xFF, 0x00, 0x00);
	    WNDPROC _buttonDefaultProc = nullptr;
		ColourPopup *_pColourPopup = nullptr;
		bool _isEnabled = true;
	
	    static LRESULT CALLBACK staticWinProc(Window* hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	        //return (((ColourPicker *)(::GetWindowLongPtr(hwnd, GWLP_USERDATA)))->runProc(Message, wParam, lParam));
	    };
		LRESULT runProc(UINT Message, WPARAM wParam, LPARAM lParam);
	    void drawForeground(HDC hDC);
		void drawBackground(HDC hDC);
	};

}