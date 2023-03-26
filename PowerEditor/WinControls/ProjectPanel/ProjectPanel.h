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

#include <PowerEditor/WinControls/DockingWnd/DockingDlgInterface.h>
#include <PowerEditor/WinControls/TreeView/TreeView.h>
#include "ProjectPanel_rc.h"

#define PM_PROJECTPANELTITLE     TEXT("Project Panel")
#define PM_WORKSPACEROOTNAME     TEXT("Workspace")
#define PM_NEWFOLDERNAME         TEXT("Folder Name")
#define PM_NEWPROJECTNAME        TEXT("Project Name")

#define PM_NEWWORKSPACE            TEXT("New Workspace")
#define PM_OPENWORKSPACE           TEXT("Open Workspace")
#define PM_RELOADWORKSPACE         TEXT("Reload Workspace")
#define PM_SAVEWORKSPACE           TEXT("Save")
#define PM_SAVEASWORKSPACE         TEXT("Save As...")
#define PM_SAVEACOPYASWORKSPACE    TEXT("Save a Copy As...")
#define PM_NEWPROJECTWORKSPACE     TEXT("Add New Project")
#define PM_FINDINFILESWORKSPACE    TEXT("Find in Projects...")

#define PM_EDITRENAME              TEXT("Rename")
#define PM_EDITNEWFOLDER           TEXT("Add Folder")
#define PM_EDITADDFILES            TEXT("Add Files...")
#define PM_EDITADDFILESRECUSIVELY  TEXT("Add Files from Directory...")
#define PM_EDITREMOVE              TEXT("Remove\tDEL")
#define PM_EDITMODIFYFILE          TEXT("Modify File Path")

#define PM_WORKSPACEMENUENTRY      TEXT("Workspace")
#define PM_EDITMENUENTRY           TEXT("Edit")

#define PM_MOVEUPENTRY             TEXT("Move Up\tCtrl+Up")
#define PM_MOVEDOWNENTRY           TEXT("Move Down\tCtrl+Down")

enum NodeType {
	nodeType_root = 0, nodeType_project = 1, nodeType_folder = 2, nodeType_file = 3
};

class TiXmlNode;
class CustomFileDialog;

class ProjectPanel : public DockingDlgInterface {
public:
	ProjectPanel(): DockingDlgInterface(IDD_PROJECTPANEL) {};
	~ProjectPanel();

	void init(HINSTANCE hInst, Upp::Ctrl* hPere, int panelID) {
		DockingDlgInterface::init(hInst, hPere);
		_panelID = panelID;
	}

	virtual void display(bool toShow = true) const {
		DockingDlgInterface::display(toShow);
	};

	void setParent(Upp::Ctrl* parent2set){
		_hParent = parent2set;
	};

	void setPanelTitle(String title) {
		_panelTitle = title;
	};
	const char * getPanelTitle() const {
		return _panelTitle.Begin();
	};

	void newWorkSpace();
	bool saveWorkspaceRequest();
	bool openWorkSpace(const char *projectFileName, bool force = false);
	bool saveWorkSpace();
	bool saveWorkSpaceAs(bool saveCopyAs);
	void setWorkSpaceFilePath(const char *projectFileName){
		_workSpaceFilePath = projectFileName;
	};
	const char * getWorkSpaceFilePath() const {
		return _workSpaceFilePath.Begin();
	};
	bool isDirty() const {
		return _isDirty;
	};
	bool checkIfNeedSave();

	virtual void setBackgroundColor(COLORREF bgColour) {
		TreeView_SetBkColor(_treeView.getHSelf(), bgColour);
	};
	virtual void setForegroundColor(COLORREF fgColour) {
		TreeView_SetTextColor(_treeView.getHSelf(), fgColour);
	};
	bool enumWorkSpaceFiles(HTREEITEM tvFrom, const Vector<String> & patterns, Vector<String> & fileNames);

protected:
	TreeView _treeView;
	HIMAGELIST _hImaLst = nullptr;
	Upp::Ctrl* _hToolbarMenu = nullptr;
	Menu* _hWorkSpaceMenu = nullptr;
	Menu* _hProjectMenu = nullptr;
	Menu* _hFolderMenu = nullptr;
	Menu* _hFileMenu = nullptr;
	String _panelTitle;
	String _workSpaceFilePath;
	String _selDirOfFilesFromDirDlg;
	bool _isDirty = false;
	int _panelID = 0;

	void initMenus();
	void destroyMenus();
	void addFiles(HTREEITEM hTreeItem);
	void addFilesFromDirectory(HTREEITEM hTreeItem);
	void recursiveAddFilesFrom(const char *folderPath, HTREEITEM hTreeItem);
	HTREEITEM addFolder(HTREEITEM hTreeItem, const char *folderName);

	bool writeWorkSpace(const char *projectFileName = Null);
	String getRelativePath(const String & fn, const char *workSpaceFileName);
	void buildProjectXml(TiXmlNode *root, HTREEITEM hItem, const char* fn2write);
	NodeType getNodeType(HTREEITEM hItem);
	void setWorkSpaceDirty(bool isDirty);
	void popupMenuCmd(int cmdID);
	POINT getMenuDisplayPoint(int iButton);
	virtual intptr_t CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
	bool buildTreeFrom(TiXmlNode *projectRoot, HTREEITEM hParentItem);
	void notified(LPNMHDR notification);
	void showContextMenu(int x, int y);
	void showContextMenuFromMenuKey(HTREEITEM selectedItem, int x, int y);
	Menu* getMenuHandler(HTREEITEM selectedItem);
	String getAbsoluteFilePath(const char * relativePath);
	void openSelectFile();
	void setFileExtFilter(CustomFileDialog & fDlg);
	std::vector<String*> fullPathStrs;
};

class FileRelocalizerDlg : public StaticDialog
{
public :
	FileRelocalizerDlg() = default;
	void init(HINSTANCE hInst, Upp::Ctrl* parent) {
		Window::init(hInst, parent);
	};

	int doDialog(const char *fn, bool isRTL = false);

	virtual void destroy() {
	};

	String getFullFilePath() {
		return _fullFilePath;
	};

protected :
	virtual intptr_t CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);

private :
	String _fullFilePath;

};
