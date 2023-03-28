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



#include <stdexcept>
#include <PowerEditor/WinControls/AnsiCharPanel/ListView.h>
#include <PowerEditor/Parameters.h>
#include <PowerEditor/localization.h>

using namespace std;

void ListView::init(Window& hInst, Window* parent)
{
	Window::init(hInst, parent);
	INITCOMMONCONTROLSEX icex;

	// Ensure that the common control DLL is loaded.
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC  = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	// Create the list-view window in report view with label editing enabled.
	int listViewStyles = LVS_REPORT | LVS_NOSORTHEADER\
						| LVS_SINGLESEL | LVS_AUTOARRANGE\
						| LVS_SHAREIMAGELISTS | LVS_SHOWSELALWAYS;

	_hSelf = ::CreateWindow(WC_LISTVIEW,
                                TEXT(""),
								WS_CHILD | WS_BORDER | listViewStyles,
                                0,
                                0,
                                0,
                                0,
                                _hParent,
                                nullptr,
                                hInst,
                                nullptr);
	if (!_hSelf)
	{
		throw std::runtime_error("ListView::init : CreateWindowEx() function return null");
	}

	::SetWindowLongPtr(_hSelf, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	_defaultProc = reinterpret_cast<WNDPROC>(::SetWindowLongPtr(_hSelf, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(staticProc)));

	dword exStyle = ListView_GetExtendedListViewStyle(_hSelf);
	exStyle |= LVS_EX_FULLROWSELECT | LVS_EX_BORDERSELECT | LVS_EX_DOUBLEBUFFER | _extraStyle;
	ListView_SetExtendedListViewStyle(_hSelf, exStyle);

	if (_columnInfos.size())
	{
		LVCOLUMN lvColumn;
		lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;

		short i = 0;
		for (auto it = _columnInfos.begin(); it != _columnInfos.end(); ++it)
		{
			lvColumn.cx = static_cast<int>(it->_width);
			lvColumn.pszText = const_cast<char *>(it->_label.Begin());
			ListView_InsertColumn(_hSelf, ++i, &lvColumn);  // index is not 0 based but 1 based
		}
	}
}

void ListView::destroy()
{
	::DestroyWindow(_hSelf);
	_hSelf = nullptr;
}

void ListView::addLine(const Vector<String> & values2Add, LPARAM lParam, int pos2insert)
{
	if (!values2Add.size())
		return;

	if (pos2insert == -1)
		pos2insert = static_cast<int>(nbItem());

	auto it = values2Add.begin();

	LVITEM item;
	item.mask = LVIF_TEXT | LVIF_PARAM;

	item.pszText = const_cast<char *>(it->Begin());
	item.iItem = pos2insert;
	item.iSubItem = 0;
	item.lParam = lParam;
	ListView_InsertItem(_hSelf, &item);
	++it;

	int j = 0;
	for (; it != values2Add.end(); ++it)
	{
		ListView_SetItemText(_hSelf, pos2insert, ++j, const_cast<char *>(it->Begin()));
	}
}

size_t ListView::findAlphabeticalOrderPos(const char* string2Cmp, SortDirection sortDir)
{
	size_t nbItem = ListView_GetItemCount(_hSelf);
	if (!nbItem)
		return 0;

	for (size_t i = 0; i < nbItem; ++i)
	{
		char str[MAX_PATH] = { '\0' };
		ListView_GetItemText(_hSelf, i, 0, str, sizeof(str));

		int res = lstrcmp(string2Cmp.Begin(), str);

		if (res < 0) // string2Cmp < str
		{
			if (sortDir == sortEncrease)
			{
				return i;
			}
		}
		else // str2Cmp >= str
		{
			if (sortDir == sortDecrease)
			{
				return i;
			}
		}
	}
	return nbItem;
}


LPARAM ListView::getLParamFromIndex(int itemIndex) const
{
	LVITEM item;
	item.mask = LVIF_PARAM;
	item.iItem = itemIndex;
	ListView_GetItem(_hSelf, &item);

	return item.lParam;
}

std::vector<size_t> ListView::getCheckedIndexes() const
{
	vector<size_t> checkedIndexes;
	size_t nbItem = ListView_GetItemCount(_hSelf);
	for (size_t i = 0; i < nbItem; ++i)
	{
		UINT st = ListView_GetItemState(_hSelf, i, LVIS_STATEIMAGEMASK);
		if (st == INDEXTOSTATEIMAGEMASK(2)) // checked
			checkedIndexes.push_back(i);
	}
	return checkedIndexes;
}

LRESULT ListView::runProc(Window* hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	return ::CallWindowProc(_defaultProc, hwnd, Message, wParam, lParam);
}

