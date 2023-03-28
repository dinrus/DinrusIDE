// This file is part of Notepad++ project
// Copyright (C) 2021 The Notepad++ Contributors.

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

//#include <PowerEditor/MISC/Common/Common.h>
#include <memory>
#include <PowerEditor/MISC/Common/Common.h>
// Customizable file dialog.
// It allows adding custom controls like checkbox to the dialog.
// This class loosely follows the interface of the FileDialog class.
// However, the implementation is different.
class CustomFileDialog
{
public:
	explicit CustomFileDialog(Window* hwnd);
	~CustomFileDialog();
	void setTitle(const char* title);
	void setExtFilter(const char* text, const char* ext);
	void setExtFilter(const char* text, std::initializer_list<const char*> exts);
	void setDefExt(const char* ext);
	void setDefFileName(const char *fn);
	void setFolder(const char* folder);
	void setCheckbox(const char* text, bool isActive = true);
	void setExtIndex(int extTypeIndex);

	void enableFileTypeCheckbox(const char* text, bool value);
	bool getFileTypeCheckboxValue() const;

	// Empty String is not a valid file name and may signal that the dialog was canceled.
	String doSaveDlg();
	String pickFolder();
	String doOpenSingleFileDlg();
	Upp::Vector<String> doOpenMultiFilesDlg();

	bool getCheckboxState() const;
	bool isReadOnly() const;

private:
	class Impl;
	std::unique_ptr<Impl> _impl;
};
