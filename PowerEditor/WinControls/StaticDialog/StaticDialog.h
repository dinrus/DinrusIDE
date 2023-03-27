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
//#include <PowerEditor/MISC/PluginsManager/Notepad_plus_msgs.h>
#include <PowerEditor/WinControls/Window.h>

typedef HRESULT (WINAPI * ETDTProc) (Upp::Ctrl*, dword);

enum class PosAlign { left, right, top, bottom };

struct DLGTEMPLATEEX
{
      WORD   dlgVer = 0;
      WORD   signature = 0;
      dword  helpID = 0;
      dword  exStyle = 0;
      dword  style = 0;
      WORD   cDlgItems = 0;
      short  x = 0;
      short  y = 0;
      short  cx = 0;
      short  cy = 0;
      // The structure has more fields but are variable length
};

class StaticDialog : public Window
{
public :
	virtual ~StaticDialog();

	virtual void create(int dialogID, bool isRTL = false, bool msgDestParent = true);

    virtual bool isCreated() const {
		return (_hSelf != Null);
	}

	void goToCenter();

	void display(bool toShow = true, bool enhancedPositioningCheckWhenShowing = false) const;

	Rect getViewablePositionRect(Rect testRc) const;

	POINT getTopPoint(Upp::Ctrl* hwnd, bool isLeft = true) const;

	bool isCheckedOrNot(int checkControlID) const
	{
		return (BST_CHECKED == ::SendMessage(::GetDlgItem( (HWND)_hSelf, checkControlID), BM_GETCHECK, 0, 0));
	}

	void setChecked(int checkControlID, bool checkOrNot = true) const
	{
		::SendDlgItemMessage((HWND)_hSelf, checkControlID, BM_SETCHECK, checkOrNot ? BST_CHECKED : BST_UNCHECKED, 0);
	}

    virtual void destroy() override;

protected:
	Rect _rc = {};
	static intptr_t CALLBACK dlgProc(Upp::Ctrl* hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual intptr_t CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam) = 0;

	HGLOBAL makeRTLResource(int dialogID, DLGTEMPLATE **ppMyDlgTemplate);
};
