#pragma once
#include "ColourPopupResource.h"
#include <PowerEditor/resource.h>
#include <PowerEditor/WinControls/Window.h>

#define WM_PICKUP_COLOR  (COLOURPOPUP_USER + 1)
#define WM_PICKUP_CANCEL (COLOURPOPUP_USER + 2)

namespace Upp{
	
	class ColourPopup : public Window
	{
	public :
		ColourPopup() = default;
		explicit ColourPopup(Color& defaultColor) : _colour(defaultColor) {}
		virtual ~ColourPopup() {}
	
		bool isCreated() const
		{
			return (_hSelf != nullptr);
		}
	
		void create(int dialogID);
	
		void doDialog(POINT p)
		{
			if (!isCreated())
				create(IDD_COLOUR_POPUP);
			::SetWindowPos(_hSelf, HWND_TOP, p.x, p.y, _rc.right - _rc.left, _rc.bottom - _rc.top, SWP_SHOWWINDOW);
		}
	
		virtual void destroy()
		{
			::DestroyWindow(_hSelf);
		}
	
		void setColour(Color& c)
		{
			_colour = c;
		}
	
		Color& getSelColour(){return _colour;};
	
	private :
		Rect _rc = {};
		Color& _colour = Color(0xFF, 0xFF, 0xFF);
	
		static intptr_t CALLBACK dlgProc(Window* hwnd, UINT message, WPARAM wParam, LPARAM lParam);
		intptr_t CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
	};
}