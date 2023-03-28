// This file is part of Notepad++ project
// Copyright (C)2021 Don HO <don.h@free.fr>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.


#pragma once

#include <PowerEditor/WinControls/Window.h>

//#include <PowerEditor/MISC/Common/Common.h>
namespace Upp{
	
	class URLCtrl : public Window {
	public:
	    void create(Window* itemHandle, const char * link, const Color& linkColor = Color(0,0,255));
		void create(Window* itemHandle, int cmd, Window* msgDest = nullptr);
	    void destroy();
	private:
		void action();
	protected :
	    String _URL;
	    HFONT _hfUnderlined = nullptr;
	    HCURSOR _hCursor = nullptr;
	
		Window* _msgDest = nullptr;
		unsigned long _cmdID = 0;
	
	    WNDPROC  _oldproc = nullptr;
	    const Color& _linkColor = Color(0xFF, 0xFF, 0xFF);
	    const Color& _visitedColor = Color(0xFF, 0xFF, 0xFF);
	    bool  _clicking = false;
	
	    static LRESULT CALLBACK URLCtrlProc(Window* hwnd, UINT Message, WPARAM wParam, LPARAM lParam){
	        return ((URLCtrl *)(::GetWindowLongPtr((HWND) hwnd, GWLP_USERDATA)))->runProc(hwnd, Message, wParam, lParam);
	    };
	    LRESULT runProc(Window* hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
	};
}
