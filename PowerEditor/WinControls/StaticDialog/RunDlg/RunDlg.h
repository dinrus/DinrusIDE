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

//#include <oleacc.h>
#include <PowerEditor/MISC/Common/Common.h>
#include "RunDlg_rc.h"

#define CURRENTWORD_MAXLENGTH 2048

const char fullCurrentPath[] = TEXT("FULL_CURRENT_PATH");
const char currentDirectory[] = TEXT("CURRENT_DIRECTORY");
const char onlyFileName[] = TEXT("FILE_NAME");
const char fileNamePart[] = TEXT("NAME_PART");
const char fileExtPart[] = TEXT("EXT_PART");
const char currentWord[] = TEXT("CURRENT_WORD");
const char nppDir[] = TEXT("NPP_DIRECTORY");
const char nppFullFilePath[] = TEXT("NPP_FULL_FILE_PATH");
const char currentLine[] = TEXT("CURRENT_LINE");
const char currentColumn[] = TEXT("CURRENT_COLUMN");
const char currentLineStr[] = TEXT("CURRENT_LINESTR");

int whichVar(char *str);
void expandNppEnvironmentStrs(const char *strSrc, char *stringDest, size_t strDestLen, Window* hWnd);

class Command {
public :
	Command() = default;
	explicit Command(const char *cmd) : _cmdLine(cmd){};
	explicit Command(const char* cmd) : _cmdLine(cmd){};
	Window& run(Window* hWnd);
	Window& run(Window* hWnd, const char* cwd);

protected :
	String _cmdLine;
private :
	void extractArgs(char *cmd2Exec, size_t cmd2ExecLen, char *args, size_t argsLen, const char *cmdEntier);
};

class RunDlg : public Command, public StaticDialog
{
public :
	RunDlg() = default;

	void doDialog(bool isRTL = false);
    virtual void destroy() {};

protected :
	virtual intptr_t CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);

private :
	void addTextToCombo(const char *txt2Add) const;
	void removeTextFromCombo(const char *txt2Remove) const;
};

