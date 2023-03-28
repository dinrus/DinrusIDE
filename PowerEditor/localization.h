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
#include <PowerEditor/TinyXml/tinyXmlA/tinyxmlA.h>


class FindReplaceDlg;
class PreferenceDlg;
class ShortcutMapper;
class UserDefineDialog;
class PluginsAdminDlg;

class MenuPosition {
public:
	int _x = -1; // menu
	int _y = -1; // sub-menu
	int _z = -1; // sub-sub-menu
	char _id[64] = { '\0' }; // a unique String defined in localization XML file
};


class NativeLangSpeaker {
public:
    NativeLangSpeaker():_nativeLangA(nullptr), _nativeLangEncoding(CP_ACP), _isRTL(false), _fileName(nullptr){};
    void init(TiXmlDocumentA *nativeLangDocRootA, bool loadIfEnglish = false);
	void changeConfigLang(Window* hDlg);
	void changeLangTabContextMenu(Menu* hCM);
	TiXmlNodeA * searchDlgNode(TiXmlNodeA *node, const char *dlgTagName);
	bool changeDlgLang(Window* hDlg, const char *dlgTagName, char *title = nullptr, size_t titleMaxSize = 0);
	void changeLangTabDrapContextMenu(Menu* hCM);
	String getSpecialMenuEntryName(const char *entryName) const;
	String getNativeLangMenuString(int itemID) const;
	String getShortcutNameString(int itemID) const;

	void changeMenuLang(Menu* menuHandle);
	void changeShortcutLang();
	void changeStyleCtrlsLang(Window* hDlg, int *idArray, const char **translatedText);
    void changeUserDefineLang(UserDefineDialog *userDefineDlg);
	void changeUserDefineLangPopupDlg(Window* hDlg);
    void changeFindReplaceDlgLang(FindReplaceDlg & findReplaceDlg);
    void changePrefereceDlgLang(PreferenceDlg & preference);
	void changePluginsAdminDlgLang(PluginsAdminDlg & pluginsAdminDlg);

	bool getDoSaveOrNotStrings(String& title, String& msg);

    bool isRTL() const {
        return _isRTL;
    };

    const char * getFileName() const {
        return _fileName;
    };

    const TiXmlNodeA * getNativeLangA() {
        return _nativeLangA;
    };

    int getLangEncoding() const {
        return _nativeLangEncoding;
    };
	bool getMsgBoxLang(const char *msgBoxTagName, String& title, String& message);
	String getShortcutMapperLangStr(const char *nodeName, const char *defaultStr) const;
	String getProjectPanelLangMenuStr(const char * nodeName, int cmdID, const char *defaultStr) const;
	String getFileBrowserLangMenuStr(int cmdID, const char *defaultStr) const;
	String getAttrNameStr(const char *defaultStr, const char *nodeL1Name, const char *nodeL2Name, const char *nodeL3Name = "name") const;
	String getLocalizedStrFromID(const char *strID, const char* defaultString) const;

	int messageBox(const char *msgBoxTagName, Window* hWnd, const char *message, const char *title, int msgBoxType, int intInfo = 0, const char *strInfo = nullptr);
private:
	TiXmlNodeA *_nativeLangA;
	int _nativeLangEncoding;
    bool _isRTL;
    const char *_fileName;
};


MenuPosition& getMenuPosition(const char *id);

