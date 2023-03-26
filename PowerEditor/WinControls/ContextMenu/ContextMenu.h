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
#include <PowerEditor/MISC/Common/Common.h>



struct MenuItemUnit final
{
	unsigned long _cmdID = 0;
	String _itemName;
	String _parentFolderName;

	MenuItemUnit() = default;
	MenuItemUnit(unsigned long cmdID, const char* itemName, const char* parentFolderName = String())
		: _cmdID(cmdID), _itemName(itemName), _parentFolderName(parentFolderName){};
	MenuItemUnit(unsigned long cmdID, const char *itemName, const char *parentFolderName = nullptr);
};


class ContextMenu final
{
public:
	~ContextMenu();

	void create(Upp::Ctrl* hParent, const std::vector<MenuItemUnit> & menuItemArray, const Menu* mainMenuHandle = Null, bool copyLink = false);
	bool isCreated() const {return _hMenu != Null;}
	
	void display(const POINT & p) const {
		::TrackPopupMenu(_hMenu, TPM_LEFTALIGN, p.x, p.y, 0, _hParent, Null);
	}

	void enableItem(int cmdID, bool doEnable) const
	{
		int flag = doEnable ? (MF_ENABLED | MF_BYCOMMAND) : (MF_DISABLED | MF_GRAYED | MF_BYCOMMAND);
		::EnableMenuItem(_hMenu, cmdID, flag);
	}

	void checkItem(int cmdID, bool doCheck) const
	{
		::CheckMenuItem(_hMenu, cmdID, MF_BYCOMMAND | (doCheck ? MF_CHECKED : MF_UNCHECKED));
	}

	Menu* getMenuHandle() const
	{
		return _hMenu;
	}

private:
	Upp::Ctrl* _hParent = Null;
	Menu* _hMenu = Null;
	std::vector<Menu*> _subMenus;

};
