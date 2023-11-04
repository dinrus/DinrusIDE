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

//#include <windowsx.h>

#include <PowerEditor/WinControls/ProjectPanel/ProjectPanel.h>
#include <PowerEditor/resource.h>
#include <plugin/TinyXml/tinyxml.h>
#include <PowerEditor/WinControls/OpenSaveFileDialog/CustomFileDialog.h>
#include <PowerEditor/localization.h>
#include <PowerEditor/Parameters.h>

#define INDEX_CLEAN_ROOT     0
#define INDEX_DIRTY_ROOT     1
#define INDEX_PROJECT        2
#define INDEX_OPEN_NODE	     3
#define INDEX_CLOSED_NODE    4
#define INDEX_LEAF           5
#define INDEX_LEAF_INVALID   6

ProjectPanel::~ProjectPanel()
{
	for (const auto s : fullPathStrs)
	{
		delete s;
	}
}

intptr_t CALLBACK ProjectPanel::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG :
		{
			ProjectPanel::initMenus();

			// Create toolbar menu
			int style = WS_CHILD | WS_VISIBLE | CCS_ADJUSTABLE | TBSTYLE_AUTOSIZE | TBSTYLE_FLAT | TBSTYLE_LIST;
			_hToolbarMenu = CreateWindowEx(0,TOOLBARCLASSNAME,nullptr, style,
								   0,0,0,0,_hSelf, nullptr, _hInst, nullptr);

			TBBUTTON tbButtons[2];

			NativeLangSpeaker *pNativeSpeaker = (NppParameters::getInstance()).getNativeLangSpeaker();
			String workspace_entry = pNativeSpeaker->getProjectPanelLangMenuStr("Entries", 0, PM_WORKSPACEMENUENTRY);
			String edit_entry = pNativeSpeaker->getProjectPanelLangMenuStr("Entries", 1, PM_EDITMENUENTRY);

			tbButtons[0].idCommand = IDB_PROJECT_BTN;
			tbButtons[0].iBitmap = I_IMAGENONE;
			tbButtons[0].fsState = TBSTATE_ENABLED;
			tbButtons[0].fsStyle = BTNS_BUTTON | BTNS_AUTOSIZE;
			tbButtons[0].iString = (intptr_t)workspace_entry.Begin();

			tbButtons[1].idCommand = IDB_EDIT_BTN;
			tbButtons[1].iBitmap = I_IMAGENONE;
			tbButtons[1].fsState = TBSTATE_ENABLED;
			tbButtons[1].fsStyle = BTNS_BUTTON | BTNS_AUTOSIZE;
			tbButtons[1].iString = (intptr_t)edit_entry.Begin();

			SendMessage(_hToolbarMenu, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);
			SendMessage(_hToolbarMenu, TB_ADDBUTTONS, sizeof(tbButtons) / sizeof(TBBUTTON), reinterpret_cast<LPARAM>(&tbButtons));
			SendMessage(_hToolbarMenu, TB_AUTOSIZE, 0, 0);
			ShowWindow(_hToolbarMenu, SW_SHOW);

			_treeView.init(_hInst, _hSelf, ID_PROJECTTREEVIEW);
			_treeView.setImageList(CX_BITMAP, CY_BITMAP, 7, IDI_PROJECT_WORKSPACE, IDI_PROJECT_WORKSPACEDIRTY, IDI_PROJECT_PROJECT, IDI_PROJECT_FOLDEROPEN, IDI_PROJECT_FOLDERCLOSE, IDI_PROJECT_FILE, IDI_PROJECT_FILEINVALID);

			_treeView.addCanNotDropInList(INDEX_LEAF);
			_treeView.addCanNotDropInList(INDEX_LEAF_INVALID);

			_treeView.addCanNotDragOutList(INDEX_CLEAN_ROOT);
			_treeView.addCanNotDragOutList(INDEX_DIRTY_ROOT);
			_treeView.addCanNotDragOutList(INDEX_PROJECT);

			_treeView.display();
			if (!openWorkSpace(_workSpaceFilePath.Begin(), true))
				newWorkSpace();

			NppDarkMode::autoSubclassAndThemeChildControls(_hSelf);
			NppDarkMode::autoSubclassAndThemeWindowNotify(_hSelf);

			return TRUE;
		}

		case NPPM_INTERNAL_REFRESHDARKMODE:
		{
			if (static_cast<BOOL>(lParam) != TRUE)
			{
				NppDarkMode::autoThemeChildControls(_hSelf);
			}
			NppDarkMode::setTreeViewStyle(_treeView.getHSelf());
			return TRUE;
		}

		case WM_MOUSEMOVE:
			if (_treeView.isDragging())
				_treeView.dragItem(_hSelf, LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_LBUTTONUP:
			if (_treeView.isDragging())
				if (_treeView.dropItem())
					setWorkSpaceDirty(true);
			break;

		case WM_NOTIFY:
		{
			notified((LPNMHDR)lParam);
		}
		return TRUE;

		case WM_SIZE:
		{
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);
			Rect toolbarMenuRect;
			::GetClientRect(_hToolbarMenu, &toolbarMenuRect);

			::MoveWindow(_hToolbarMenu, 0, 0, width, toolbarMenuRect.bottom, TRUE);

			Window* hwnd = _treeView.getHSelf();
			if (hwnd)
				::MoveWindow(hwnd, 0, toolbarMenuRect.bottom + 2, width, height - toolbarMenuRect.bottom - 2, TRUE);
			break;
		}

		case WM_CONTEXTMENU:
			if (!_treeView.isDragging())
			{
				int xPos = GET_X_LPARAM(lParam);
				int yPos = GET_Y_LPARAM(lParam);

				// If the context menu is generated from the keyboard, then we
				// should display it at the location of the current selection
				if (xPos == -1 && yPos == -1)
				{
					HTREEITEM selectedItem = _treeView.getSelection();

					if (selectedItem)
					{
						Rect selectedItemRect;
						if (TreeView_GetItemRect(_treeView.getHSelf(), selectedItem, &selectedItemRect, TRUE))
						{
							showContextMenuFromMenuKey(selectedItem, (selectedItemRect.left + selectedItemRect.right) / 2, (selectedItemRect.top + selectedItemRect.bottom) / 2);
						}
					}
				}
				else
				{
					showContextMenu(xPos, yPos);
				}
			}
		return TRUE;

		case WM_COMMAND:
		{
			popupMenuCmd(LOWORD(wParam));
			break;
		}

		case WM_DESTROY:
		{
			_treeView.destroy();
			destroyMenus();
			::DestroyWindow(_hToolbarMenu);
			break;
		}

		default :
			return DockingDlgInterface::run_dlgProc(message, wParam, lParam);
	}
	return DockingDlgInterface::run_dlgProc(message, wParam, lParam);
}

bool ProjectPanel::checkIfNeedSave()
{
	if (_isDirty)
	{
		const char * title = _workSpaceFilePath.GetLength() > 0 ? PathFindFileName (_workSpaceFilePath.Begin()) : _panelTitle.Begin();
		NativeLangSpeaker *pNativeSpeaker = (NppParameters::getInstance()).getNativeLangSpeaker();
		int res = pNativeSpeaker->messageBox("ProjectPanelChanged",
			_hSelf,
			TEXT("The workspace was modified. Do you want to save it?"),
			TEXT("$STR_REPLACE$"),
			MB_YESNOCANCEL | MB_ICONQUESTION,
			0,
			title);

		if (res == IDYES)
		{
			if (!saveWorkSpace())
				return false;
		}
		else if (res == IDNO)
		{
			// Don't save so do nothing here
		}
		else
		{
			// Cancelled
			return false;
		}
	}
	return true;
}

void ProjectPanel::initMenus()
{
	_hWorkSpaceMenu = ::CreatePopupMenu();

	NativeLangSpeaker *pNativeSpeaker = (NppParameters::getInstance()).getNativeLangSpeaker();

	String new_workspace = pNativeSpeaker->getProjectPanelLangMenuStr("WorkspaceMenu", IDM_PROJECT_NEWWS, PM_NEWWORKSPACE);
	String open_workspace = pNativeSpeaker->getProjectPanelLangMenuStr("WorkspaceMenu", IDM_PROJECT_OPENWS, PM_OPENWORKSPACE);
	String reload_workspace = pNativeSpeaker->getProjectPanelLangMenuStr("WorkspaceMenu", IDM_PROJECT_RELOADWS, PM_RELOADWORKSPACE);
	String save_workspace = pNativeSpeaker->getProjectPanelLangMenuStr("WorkspaceMenu", IDM_PROJECT_SAVEWS, PM_SAVEWORKSPACE);
	String saveas_workspace = pNativeSpeaker->getProjectPanelLangMenuStr("WorkspaceMenu", IDM_PROJECT_SAVEASWS, PM_SAVEASWORKSPACE);
	String saveacopyas_workspace = pNativeSpeaker->getProjectPanelLangMenuStr("WorkspaceMenu", IDM_PROJECT_SAVEACOPYASWS, PM_SAVEACOPYASWORKSPACE);
	String newproject_workspace = pNativeSpeaker->getProjectPanelLangMenuStr("WorkspaceMenu", IDM_PROJECT_NEWPROJECT, PM_NEWPROJECTWORKSPACE);
	String findinprojects_workspace = pNativeSpeaker->getProjectPanelLangMenuStr("WorkspaceMenu", IDM_PROJECT_FINDINPROJECTSWS, PM_FINDINFILESWORKSPACE);

	::InsertMenu(_hWorkSpaceMenu, 0, MF_BYCOMMAND, IDM_PROJECT_NEWWS, new_workspace.Begin());
	::InsertMenu(_hWorkSpaceMenu, 0, MF_BYCOMMAND, IDM_PROJECT_OPENWS, open_workspace.Begin());
	::InsertMenu(_hWorkSpaceMenu, 0, MF_BYCOMMAND, IDM_PROJECT_RELOADWS, reload_workspace.Begin());
	::InsertMenu(_hWorkSpaceMenu, 0, MF_BYCOMMAND, IDM_PROJECT_SAVEWS, save_workspace.Begin());
	::InsertMenu(_hWorkSpaceMenu, 0, MF_BYCOMMAND, IDM_PROJECT_SAVEASWS, saveas_workspace.Begin());
	::InsertMenu(_hWorkSpaceMenu, 0, MF_BYCOMMAND, IDM_PROJECT_SAVEACOPYASWS, saveacopyas_workspace.Begin());
	::InsertMenu(_hWorkSpaceMenu, 0, MF_BYCOMMAND, static_cast<UINT>(-1), 0);
	::InsertMenu(_hWorkSpaceMenu, 0, MF_BYCOMMAND, IDM_PROJECT_NEWPROJECT, newproject_workspace.Begin());
	::InsertMenu(_hWorkSpaceMenu, 0, MF_BYCOMMAND, static_cast<UINT>(-1), 0);
	::InsertMenu(_hWorkSpaceMenu, 0, MF_BYCOMMAND, IDM_PROJECT_FINDINPROJECTSWS, findinprojects_workspace.Begin());

	String edit_moveup = pNativeSpeaker->getProjectPanelLangMenuStr("ProjectMenu", IDM_PROJECT_MOVEUP, PM_MOVEUPENTRY);
	String edit_movedown = pNativeSpeaker->getProjectPanelLangMenuStr("ProjectMenu", IDM_PROJECT_MOVEDOWN, PM_MOVEDOWNENTRY);
	String edit_rename = pNativeSpeaker->getProjectPanelLangMenuStr("ProjectMenu", IDM_PROJECT_RENAME, PM_EDITRENAME);
	String edit_addfolder = pNativeSpeaker->getProjectPanelLangMenuStr("ProjectMenu", IDM_PROJECT_NEWFOLDER, PM_EDITNEWFOLDER);
	String edit_addfiles = pNativeSpeaker->getProjectPanelLangMenuStr("ProjectMenu", IDM_PROJECT_ADDFILES, PM_EDITADDFILES);
	String edit_addfilesRecursive = pNativeSpeaker->getProjectPanelLangMenuStr("ProjectMenu", IDM_PROJECT_ADDFILESRECUSIVELY, PM_EDITADDFILESRECUSIVELY);
	String edit_remove = pNativeSpeaker->getProjectPanelLangMenuStr("ProjectMenu", IDM_PROJECT_DELETEFOLDER, PM_EDITREMOVE);

	_hProjectMenu = ::CreatePopupMenu();
	::InsertMenu(_hProjectMenu, 0, MF_BYCOMMAND, IDM_PROJECT_MOVEUP, edit_moveup.Begin());
	::InsertMenu(_hProjectMenu, 0, MF_BYCOMMAND, IDM_PROJECT_MOVEDOWN, edit_movedown.Begin());
	::InsertMenu(_hProjectMenu, 0, MF_BYCOMMAND, UINT(-1), 0);
	::InsertMenu(_hProjectMenu, 0, MF_BYCOMMAND, IDM_PROJECT_RENAME, edit_rename.Begin());
	::InsertMenu(_hProjectMenu, 0, MF_BYCOMMAND, IDM_PROJECT_NEWFOLDER, edit_addfolder.Begin());
	::InsertMenu(_hProjectMenu, 0, MF_BYCOMMAND, IDM_PROJECT_ADDFILES, edit_addfiles.Begin());
	::InsertMenu(_hProjectMenu, 0, MF_BYCOMMAND, IDM_PROJECT_ADDFILESRECUSIVELY, edit_addfilesRecursive.Begin());
	::InsertMenu(_hProjectMenu, 0, MF_BYCOMMAND, IDM_PROJECT_DELETEFOLDER, edit_remove.Begin());

	edit_moveup = pNativeSpeaker->getProjectPanelLangMenuStr("FolderMenu", IDM_PROJECT_MOVEUP, PM_MOVEUPENTRY);
	edit_movedown = pNativeSpeaker->getProjectPanelLangMenuStr("FolderMenu", IDM_PROJECT_MOVEDOWN, PM_MOVEDOWNENTRY);
	edit_rename = pNativeSpeaker->getProjectPanelLangMenuStr("FolderMenu", IDM_PROJECT_RENAME, PM_EDITRENAME);
	edit_addfolder = pNativeSpeaker->getProjectPanelLangMenuStr("FolderMenu", IDM_PROJECT_NEWFOLDER, PM_EDITNEWFOLDER);
	edit_addfiles = pNativeSpeaker->getProjectPanelLangMenuStr("FolderMenu", IDM_PROJECT_ADDFILES, PM_EDITADDFILES);
	edit_addfilesRecursive = pNativeSpeaker->getProjectPanelLangMenuStr("FolderMenu", IDM_PROJECT_ADDFILESRECUSIVELY, PM_EDITADDFILESRECUSIVELY);
	edit_remove = pNativeSpeaker->getProjectPanelLangMenuStr("FolderMenu", IDM_PROJECT_DELETEFOLDER, PM_EDITREMOVE);

	_hFolderMenu = ::CreatePopupMenu();
	::InsertMenu(_hFolderMenu, 0, MF_BYCOMMAND, IDM_PROJECT_MOVEUP,        edit_moveup.Begin());
	::InsertMenu(_hFolderMenu, 0, MF_BYCOMMAND, IDM_PROJECT_MOVEDOWN,      edit_movedown.Begin());
	::InsertMenu(_hFolderMenu, 0, MF_BYCOMMAND, UINT(-1), 0);
	::InsertMenu(_hFolderMenu, 0, MF_BYCOMMAND, IDM_PROJECT_RENAME,        edit_rename.Begin());
	::InsertMenu(_hFolderMenu, 0, MF_BYCOMMAND, IDM_PROJECT_NEWFOLDER,     edit_addfolder.Begin());
	::InsertMenu(_hFolderMenu, 0, MF_BYCOMMAND, IDM_PROJECT_ADDFILES,      edit_addfiles.Begin());
	::InsertMenu(_hFolderMenu, 0, MF_BYCOMMAND, IDM_PROJECT_ADDFILESRECUSIVELY, edit_addfilesRecursive.Begin());
	::InsertMenu(_hFolderMenu, 0, MF_BYCOMMAND, IDM_PROJECT_DELETEFOLDER,  edit_remove.Begin());

	edit_moveup = pNativeSpeaker->getProjectPanelLangMenuStr("FileMenu", IDM_PROJECT_MOVEUP, PM_MOVEUPENTRY);
	edit_movedown = pNativeSpeaker->getProjectPanelLangMenuStr("FileMenu", IDM_PROJECT_MOVEDOWN, PM_MOVEDOWNENTRY);
	edit_rename = pNativeSpeaker->getProjectPanelLangMenuStr("FileMenu", IDM_PROJECT_RENAME, PM_EDITRENAME);
	edit_remove = pNativeSpeaker->getProjectPanelLangMenuStr("FileMenu", IDM_PROJECT_DELETEFILE, PM_EDITREMOVE);
	String edit_modifyfile = pNativeSpeaker->getProjectPanelLangMenuStr("FileMenu", IDM_PROJECT_MODIFYFILEPATH, PM_EDITMODIFYFILE);

	_hFileMenu = ::CreatePopupMenu();
	::InsertMenu(_hFileMenu, 0, MF_BYCOMMAND, IDM_PROJECT_MOVEUP, edit_moveup.Begin());
	::InsertMenu(_hFileMenu, 0, MF_BYCOMMAND, IDM_PROJECT_MOVEDOWN, edit_movedown.Begin());
	::InsertMenu(_hFileMenu, 0, MF_BYCOMMAND, UINT(-1), 0);
	::InsertMenu(_hFileMenu, 0, MF_BYCOMMAND, IDM_PROJECT_RENAME, edit_rename.Begin());
	::InsertMenu(_hFileMenu, 0, MF_BYCOMMAND, IDM_PROJECT_DELETEFILE, edit_remove.Begin());
	::InsertMenu(_hFileMenu, 0, MF_BYCOMMAND, IDM_PROJECT_MODIFYFILEPATH, edit_modifyfile.Begin());
}

void ProjectPanel::destroyMenus()
{
	::DestroyMenu(_hWorkSpaceMenu);
	::DestroyMenu(_hProjectMenu);
	::DestroyMenu(_hFolderMenu);
	::DestroyMenu(_hFileMenu);
}

bool ProjectPanel::openWorkSpace(const char *projectFileName, bool force)
{
	if ((!force) && (_workSpaceFilePath.GetLength() > 0))
	{ // Return if it is better to keep the current workspace tree
		String newWorkspace = projectFileName;
		if (newWorkspace == _workSpaceFilePath)
			return true;
		if (!saveWorkspaceRequest())
			return true;
	}

	TiXmlDocument *pXmlDocProject = new TiXmlDocument(projectFileName);
	bool loadOkay = pXmlDocProject->LoadFile();
	if (!loadOkay)
	{
		delete pXmlDocProject;
		return false;
	}

	TiXmlNode *root = pXmlDocProject->FirstChild(TEXT("NotepadPlus"));
	if (!root)
	{
		delete pXmlDocProject;
		return false;
	}


	TiXmlNode *childNode = root->FirstChildElement(TEXT("Project"));
	if (!childNode)
	{
		delete pXmlDocProject;
		return false;
	}

	if (!::PathFileExists(projectFileName))
	{
		delete pXmlDocProject;
		return false;
	}

	_treeView.removeAllItems();
	_workSpaceFilePath = projectFileName;

	char * fileName = PathFindFileName(projectFileName);
	HTREEITEM rootItem = _treeView.addItem(fileName, TVI_ROOT, INDEX_CLEAN_ROOT);

	for ( ; childNode ; childNode = childNode->NextSibling(TEXT("Project")))
	{
		HTREEITEM projectItem = _treeView.addItem((childNode->ToElement())->Attribute(TEXT("name")), rootItem, INDEX_PROJECT);
		buildTreeFrom(childNode, projectItem);
	}
	setWorkSpaceDirty(false);
	_treeView.expand(rootItem);

	delete pXmlDocProject;
	return loadOkay;
}

void ProjectPanel::newWorkSpace()
{
	NativeLangSpeaker *pNativeSpeaker = (NppParameters::getInstance()).getNativeLangSpeaker();
	String workspace = pNativeSpeaker->getAttrNameStr(PM_WORKSPACEROOTNAME, "ProjectManager", "WorkspaceRootName");
	_treeView.addItem(workspace.Begin(), TVI_ROOT, INDEX_CLEAN_ROOT);
	setWorkSpaceDirty(false);
	_workSpaceFilePath = TEXT("");
}

bool ProjectPanel::saveWorkSpace()
{
	if (_workSpaceFilePath == TEXT(""))
	{
		return saveWorkSpaceAs(false);
	}
	else
	{
		if (!writeWorkSpace())
			return false;

		setWorkSpaceDirty(false);
		return true;
	}
}

bool ProjectPanel::writeWorkSpace(const char *projectFileName)
{
	//write <NotepadPlus>: use the default file name if new file name is not given
	const char * fn2write = projectFileName?projectFileName:_workSpaceFilePath.Begin();
	TiXmlDocument projDoc(fn2write);
	TiXmlNode *root = projDoc.InsertEndChild(TiXmlElement(TEXT("NotepadPlus")));

	char textBuffer[MAX_PATH];
	TVITEM tvItem;
	tvItem.mask = TVIF_TEXT;
	tvItem.pszText = textBuffer;
	tvItem.cchTextMax = MAX_PATH;

	//for each project, write <Project>
	HTREEITEM tvRoot = _treeView.getRoot();
	if (!tvRoot)
		return false;

	for (HTREEITEM tvProj = _treeView.getChildFrom(tvRoot);
		tvProj != nullptr;
		tvProj = _treeView.getNextSibling(tvProj))
	{
		tvItem.hItem = tvProj;
		SendMessage(_treeView.getHSelf(), TVM_GETITEM, 0, reinterpret_cast<LPARAM>(&tvItem));
		//printStr(tvItem.pszText);

		TiXmlNode *projRoot = root->InsertEndChild(TiXmlElement(TEXT("Project")));
		projRoot->ToElement()->SetAttribute(TEXT("name"), tvItem.pszText);
		buildProjectXml(projRoot, tvProj, fn2write);
	}

	if (!projDoc.SaveFile())
	{
		const char * title = _workSpaceFilePath.GetLength() > 0 ? PathFindFileName (_workSpaceFilePath.Begin()) : _panelTitle.Begin();
		NativeLangSpeaker *pNativeSpeaker = (NppParameters::getInstance()).getNativeLangSpeaker();
		pNativeSpeaker->messageBox("ProjectPanelSaveError",
		_hSelf,
		TEXT("An error occurred while writing your workspace file.\nYour workspace has not been saved."),
		TEXT("$STR_REPLACE$"),
		MB_OK | MB_ICONERROR,
		0,
		title);
		return false;
	}
	char * fileName = PathFindFileName(fn2write);
	_treeView.renameItem(tvRoot, fileName);
	return true;
}

void ProjectPanel::buildProjectXml(TiXmlNode *node, HTREEITEM hItem, const char* fn2write)
{
	char textBuffer[MAX_PATH];
	TVITEM tvItem;
	tvItem.mask = TVIF_TEXT | TVIF_PARAM;
	tvItem.pszText = textBuffer;
	tvItem.cchTextMax = MAX_PATH;

	for (HTREEITEM hItemNode = _treeView.getChildFrom(hItem);
		hItemNode != nullptr;
		hItemNode = _treeView.getNextSibling(hItemNode))
	{
		tvItem.hItem = hItemNode;
		SendMessage(_treeView.getHSelf(), TVM_GETITEM, 0, reinterpret_cast<LPARAM>(&tvItem));
		if (tvItem.lParam)
		{
			String *fn = (String *)tvItem.lParam;
			String newFn = getRelativePath(*fn, fn2write);
			TiXmlNode *fileLeaf = node->InsertEndChild(TiXmlElement(TEXT("File")));
			fileLeaf->ToElement()->SetAttribute(TEXT("name"), newFn.Begin());
		}
		else
		{
			TiXmlNode *folderNode = node->InsertEndChild(TiXmlElement(TEXT("Folder")));
			folderNode->ToElement()->SetAttribute(TEXT("name"), tvItem.pszText);
			buildProjectXml(folderNode, hItemNode, fn2write);
		}
	}
}

bool ProjectPanel::enumWorkSpaceFiles(HTREEITEM tvFrom, const Vector<String> & patterns, Vector<String> & fileNames)
{
	char textBuffer[MAX_PATH];
	TVITEM tvItem;
	tvItem.mask = TVIF_TEXT | TVIF_PARAM;
	tvItem.pszText = textBuffer;
	tvItem.cchTextMax = MAX_PATH;

	HTREEITEM tvRoot = tvFrom ? tvFrom : _treeView.getRoot();
	if (!tvRoot) return false;

	for (HTREEITEM tvProj = _treeView.getChildFrom(tvRoot);
		tvProj != nullptr;
		tvProj = _treeView.getNextSibling(tvProj))
	{
		tvItem.hItem = tvProj;
		SendMessage(_treeView.getHSelf(), TVM_GETITEM, 0, reinterpret_cast<LPARAM>(&tvItem));
		if (tvItem.lParam)
		{
			if (matchInList(tvItem.pszText, patterns))
			{
				String *fn = (String *)tvItem.lParam;
				fileNames.push_back (*fn);
			}
		}
		else
		{
			if (!enumWorkSpaceFiles (tvProj, patterns, fileNames)) return false;
		}
	}
	return true;
}

String ProjectPanel::getRelativePath(const String& filePath, const char *workSpaceFileName)
{
	char wsfn[MAX_PATH];
	wcscpy_s(wsfn, workSpaceFileName);
	::PathRemoveFileSpec(wsfn);

	size_t pos_found = filePath.find(wsfn);
	if (pos_found == String::npos)
		return filePath;
	const char *relativeFile = filePath.Begin() + lstrlen(wsfn);
	if (relativeFile[0] == '\\')
		++relativeFile;
	return relativeFile;
}

bool ProjectPanel::buildTreeFrom(TiXmlNode *projectRoot, HTREEITEM hParentItem)
{
	for (TiXmlNode *childNode = projectRoot->FirstChildElement();
		childNode ;
		childNode = childNode->NextSibling())
	{
		const char *v = childNode->Value();
		if (lstrcmp(TEXT("Folder"), v) == 0)
		{
			HTREEITEM addedItem = _treeView.addItem((childNode->ToElement())->Attribute(TEXT("name")), hParentItem, INDEX_CLOSED_NODE);
			if (!childNode->NoChildren())
			{
				bool isOK = buildTreeFrom(childNode, addedItem);
				if (!isOK)
					return false;
			}
		}
		else if (lstrcmp(TEXT("File"), v) == 0)
		{
			const char *strValue = (childNode->ToElement())->Attribute(TEXT("name"));
			String fullPath = getAbsoluteFilePath(strValue);
			char *strValueLabel = ::PathFindFileName(strValue);
			int iImage = ::PathFileExists(fullPath.Begin())?INDEX_LEAF:INDEX_LEAF_INVALID;

			String* fullPathStr = new String(fullPath);
			fullPathStrs.push_back(fullPathStr);
			LPARAM lParamFullPathStr = reinterpret_cast<LPARAM>(fullPathStr);

			_treeView.addItem(strValueLabel, hParentItem, iImage, lParamFullPathStr);
		}
	}
	return true;
}

String ProjectPanel::getAbsoluteFilePath(const char * relativePath)
{
	if (!::PathIsRelative(relativePath))
		return relativePath;

	char absolutePath[MAX_PATH];
	wcscpy_s(absolutePath, _workSpaceFilePath.Begin());
	::PathRemoveFileSpec(absolutePath);
	::PathAppend(absolutePath, relativePath);
	return absolutePath;
}

void ProjectPanel::openSelectFile()
{
	TVITEM tvItem;
	tvItem.mask = TVIF_PARAM;
	tvItem.hItem = _treeView.getSelection();
	::SendMessage(_treeView.getHSelf(), TVM_GETITEM, 0, reinterpret_cast<LPARAM>(&tvItem));

	NodeType nType = getNodeType(tvItem.hItem);
	String *fn = (String *)tvItem.lParam;
	if (nType == nodeType_file && fn)
	{
		tvItem.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		if (::PathFileExists(fn->Begin()))
		{
			::PostMessage(_hParent, NPPM_DOOPEN, 0, reinterpret_cast<LPARAM>(fn->Begin()));
			tvItem.iImage = INDEX_LEAF;
			tvItem.iSelectedImage = INDEX_LEAF;
		}
		else
		{
			tvItem.iImage = INDEX_LEAF_INVALID;
			tvItem.iSelectedImage = INDEX_LEAF_INVALID;
		}
		TreeView_SetItem(_treeView.getHSelf(), &tvItem);
	}
}


void ProjectPanel::notified(LPNMHDR notification)
{
	if (notification->code == DMN_CLOSE)
	{
		::SendMessage(_hParent, WM_COMMAND, IDM_VIEW_PROJECT_PANEL_1 + _panelID, 0);
		SetWindowLongPtr (getHSelf(), DWLP_MSGRESULT, _isClosed ? 0 : 1);
	}
	else if ((notification->hwndFrom == _treeView.getHSelf()))
	{
		char textBuffer[MAX_PATH] = { '\0' };
		TVITEM tvItem;
		tvItem.mask = TVIF_TEXT | TVIF_PARAM;
		tvItem.pszText = textBuffer;
		tvItem.cchTextMax = MAX_PATH;

		switch (notification->code)
		{
			case NM_DBLCLK:
			{
				openSelectFile();
			}
			break;

			case NM_RETURN:
				SetWindowLongPtr(_hSelf, DWLP_MSGRESULT, 1);
			break;

			case TVN_ENDLABELEDIT:
			{
				LPNMTVDISPINFO tvnotif = (LPNMTVDISPINFO)notification;
				if (!tvnotif->item.pszText)
					return;
				if (getNodeType(tvnotif->item.hItem) == nodeType_root)
					return;

				// Processing for only File case
				if (tvnotif->item.lParam)
				{
					// Get the old label
					tvItem.hItem = _treeView.getSelection();
					::SendMessage(_treeView.getHSelf(), TVM_GETITEM, 0, reinterpret_cast<LPARAM>(&tvItem));
					size_t len = lstrlen(tvItem.pszText);

					// Find the position of old label in File path
					String *filePath = (String *)tvnotif->item.lParam;
					size_t found = filePath->rfind(tvItem.pszText);

					// If found the old label, replace it with the modified one
					if (found != String::npos)
						filePath->replace(found, len, tvnotif->item.pszText);

					// Check the validity of modified file path
					tvItem.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
					if (::PathFileExists(filePath->Begin()))
					{
						tvItem.iImage = INDEX_LEAF;
						tvItem.iSelectedImage = INDEX_LEAF;
					}
					else
					{
						tvItem.iImage = INDEX_LEAF_INVALID;
						tvItem.iSelectedImage = INDEX_LEAF_INVALID;
					}
					TreeView_SetItem(_treeView.getHSelf(), &tvItem);
				}

				// For File, Folder and Project
				::SendMessage(_treeView.getHSelf(), TVM_SETITEM, 0, reinterpret_cast<LPARAM>(&(tvnotif->item)));
				setWorkSpaceDirty(true);
			}
			break;

			case TVN_GETINFOTIP:
			{
				LPNMTVGETINFOTIP lpGetInfoTip = (LPNMTVGETINFOTIP)notification;
				String *str = nullptr ;

				if (_treeView.getRoot() == lpGetInfoTip->hItem)
				{
					str = &_workSpaceFilePath;
				}
				else
				{
					str = (String *)lpGetInfoTip->lParam;
					if (!str)
						return;
				}
				lpGetInfoTip->pszText = (LPTSTR)str->Begin();
				lpGetInfoTip->cchTextMax = static_cast<int32_t>(str->size());
			}
			break;

			case TVN_KEYDOWN:
			{
				LPNMTVKEYDOWN ptvkd = (LPNMTVKEYDOWN)notification;

				if (ptvkd->wVKey == VK_DELETE)
				{
					HTREEITEM hItem = _treeView.getSelection();
					NodeType nType = getNodeType(hItem);
					if (nType == nodeType_project || nType == nodeType_folder)
						popupMenuCmd(IDM_PROJECT_DELETEFOLDER);
					else if (nType == nodeType_file)
						popupMenuCmd(IDM_PROJECT_DELETEFILE);
				}
				else if (ptvkd->wVKey == VK_RETURN)
				{
					HTREEITEM hItem = _treeView.getSelection();
					NodeType nType = getNodeType(hItem);
					if (nType == nodeType_file)
						openSelectFile();
					else
						_treeView.toggleExpandCollapse(hItem);
				}
				else if (ptvkd->wVKey == VK_UP)
				{
					if (0x80 & GetKeyState(VK_CONTROL))
					{
						popupMenuCmd(IDM_PROJECT_MOVEUP);
					}
				}
				else if (ptvkd->wVKey == VK_DOWN)
				{
					if (0x80 & GetKeyState(VK_CONTROL))
					{
						popupMenuCmd(IDM_PROJECT_MOVEDOWN);
					}
				}
				else if (ptvkd->wVKey == VK_F2)
					popupMenuCmd(IDM_PROJECT_RENAME);

			}
			break;

			case TVN_ITEMEXPANDED:
			{
				LPNMTREEVIEW nmtv = (LPNMTREEVIEW)notification;
				tvItem.hItem = nmtv->itemNew.hItem;
				tvItem.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;

				if (getNodeType(nmtv->itemNew.hItem) == nodeType_folder)
				{
					if (nmtv->action == TVE_COLLAPSE)
					{
						_treeView.setItemImage(nmtv->itemNew.hItem, INDEX_CLOSED_NODE, INDEX_CLOSED_NODE);
					}
					else if (nmtv->action == TVE_EXPAND)
					{
						_treeView.setItemImage(nmtv->itemNew.hItem, INDEX_OPEN_NODE, INDEX_OPEN_NODE);
					}
				}
			}
			break;

			case TVN_BEGINDRAG:
			{
				_treeView.beginDrag((LPNMTREEVIEW)notification);
			}
			break;
		}
	}
}

void ProjectPanel::setWorkSpaceDirty(bool isDirty)
{
	_isDirty = isDirty;
	int iImg = _isDirty?INDEX_DIRTY_ROOT:INDEX_CLEAN_ROOT;
	_treeView.setItemImage(_treeView.getRoot(), iImg, iImg);
}

NodeType ProjectPanel::getNodeType(HTREEITEM hItem)
{
	TVITEM tvItem;
	tvItem.hItem = hItem;
	tvItem.mask = TVIF_IMAGE | TVIF_PARAM;
	SendMessage(_treeView.getHSelf(), TVM_GETITEM, 0, reinterpret_cast<LPARAM>(&tvItem));

	// Root
	if (tvItem.iImage == INDEX_CLEAN_ROOT || tvItem.iImage == INDEX_DIRTY_ROOT)
	{
		return nodeType_root;
	}
	// Project
	else if (tvItem.iImage == INDEX_PROJECT)
	{
		return nodeType_project;
	}
	// Folder
	else if (!tvItem.lParam)
	{
		return nodeType_folder;
	}
	// File
	else
	{
		return nodeType_file;
	}
}

void ProjectPanel::showContextMenu(int x, int y)
{
	TVHITTESTINFO tvHitInfo;
	HTREEITEM hTreeItem;

	// Detect if the given position is on the element TVITEM
	tvHitInfo.pt.x = x;
	tvHitInfo.pt.y = y;
	tvHitInfo.flags = 0;
	ScreenToClient(_treeView.getHSelf(), &(tvHitInfo.pt));
	hTreeItem = TreeView_HitTest(_treeView.getHSelf(), &tvHitInfo);

	if (tvHitInfo.hItem != nullptr)
	{
		// Make item selected
		_treeView.selectItem(tvHitInfo.hItem);
		Menu* hMenu = getMenuHandler(tvHitInfo.hItem);
		TrackPopupMenu(hMenu,
			NppParameters::getInstance().getNativeLangSpeaker()->isRTL() ? TPM_RIGHTALIGN | TPM_LAYOUTRTL : TPM_LEFTALIGN,
			x, y, 0, _hSelf, nullptr);
	}
}

void ProjectPanel::showContextMenuFromMenuKey(HTREEITEM selectedItem, int x, int y)
{
	POINT p;
	p.x = x;
	p.y = y;

	ClientToScreen(_treeView.getHSelf(), &p);

	if (selectedItem != nullptr)
	{
		Menu* hMenu = getMenuHandler(selectedItem);
		TrackPopupMenu(hMenu,
			NppParameters::getInstance().getNativeLangSpeaker()->isRTL() ? TPM_RIGHTALIGN | TPM_LAYOUTRTL : TPM_LEFTALIGN,
			x, y, 0, _hSelf, nullptr);
	}
}

Menu* ProjectPanel::getMenuHandler(HTREEITEM selectedItem)
{
	// get clicked item type
	NodeType nodeType = getNodeType(selectedItem);
	Menu* hMenu = nullptr;

	if (nodeType == nodeType_root)
		hMenu = _hWorkSpaceMenu;
	else if (nodeType == nodeType_project)
		hMenu = _hProjectMenu;
	else if (nodeType == nodeType_folder)
		hMenu = _hFolderMenu;
	else //nodeType_file
		hMenu = _hFileMenu;

	return hMenu;
}

POINT ProjectPanel::getMenuDisplayPoint(int iButton)
{
	POINT p;
	Rect btnRect;
	SendMessage(_hToolbarMenu, TB_GETITEMRECT, iButton, reinterpret_cast<LPARAM>(&btnRect));

	p.x = btnRect.left;
	p.y = btnRect.top + btnRect.bottom;
	ClientToScreen(_hToolbarMenu, &p);
	return p;
}

HTREEITEM ProjectPanel::addFolder(HTREEITEM hTreeItem, const char *folderName)
{
	HTREEITEM addedItem = _treeView.addItem(folderName, hTreeItem, INDEX_CLOSED_NODE);

	TreeView_Expand(_treeView.getHSelf(), hTreeItem, TVE_EXPAND);
	TreeView_EditLabel(_treeView.getHSelf(), addedItem);
	if (getNodeType(hTreeItem) == nodeType_folder)
		_treeView.setItemImage(hTreeItem, INDEX_OPEN_NODE, INDEX_OPEN_NODE);

	return addedItem;
}

bool ProjectPanel::saveWorkspaceRequest()
{ // returns true for continue and false for break
	if (_isDirty)
	{
		NativeLangSpeaker *pNativeSpeaker = (NppParameters::getInstance()).getNativeLangSpeaker();
		int res = pNativeSpeaker->messageBox("ProjectPanelOpenDoSaveDirtyWsOrNot",
					_hSelf,
					TEXT("The current workspace was modified. Do you want to save the current project?"),
					TEXT("Open Workspace"),
					MB_YESNOCANCEL | MB_ICONQUESTION | MB_APPLMODAL);

		if (res == IDYES)
		{
			if (!saveWorkSpace())
				return false;
		}
		else if (res == IDNO)
		{
			// Don't save so do nothing here
		}
		else if (res == IDCANCEL)
		{
			// User cancels action "New Workspace" so we interrupt here
			return false;
		}
	}
	return true;
}

void ProjectPanel::popupMenuCmd(int cmdID)
{
	// get selected item handle
	HTREEITEM hTreeItem = _treeView.getSelection();
	if (!hTreeItem)
		return;

	switch (cmdID)
	{
		//
		// Toolbar menu buttons
		//
		case IDB_PROJECT_BTN:
		{
		  POINT p = getMenuDisplayPoint(0);
		  TrackPopupMenu(_hWorkSpaceMenu,
			  NppParameters::getInstance().getNativeLangSpeaker()->isRTL() ? TPM_RIGHTALIGN | TPM_LAYOUTRTL : TPM_LEFTALIGN,
			  p.x, p.y, 0, _hSelf, nullptr);
		}
		break;

		case IDB_EDIT_BTN:
		{
			POINT p = getMenuDisplayPoint(1);
			Menu* hMenu = nullptr;
			NodeType nodeType = getNodeType(hTreeItem);
			if (nodeType == nodeType_project)
				hMenu = _hProjectMenu;
			else if (nodeType == nodeType_folder)
				hMenu = _hFolderMenu;
			else if (nodeType == nodeType_file)
				hMenu = _hFileMenu;
			if (hMenu)
				TrackPopupMenu(hMenu,
					NppParameters::getInstance().getNativeLangSpeaker()->isRTL() ? TPM_RIGHTALIGN | TPM_LAYOUTRTL : TPM_LEFTALIGN,
					p.x, p.y, 0, _hSelf, nullptr);
		}
		break;

		//
		// Toolbar menu commands
		//
		case IDM_PROJECT_NEWPROJECT :
		{
			HTREEITEM root = _treeView.getRoot();

			NativeLangSpeaker *pNativeSpeaker = (NppParameters::getInstance()).getNativeLangSpeaker();
			String newProjectLabel = pNativeSpeaker->getAttrNameStr(PM_NEWPROJECTNAME, "ProjectManager", "NewProjectName");
			HTREEITEM addedItem = _treeView.addItem(newProjectLabel.Begin(),  root, INDEX_PROJECT);
			setWorkSpaceDirty(true);
			_treeView.expand(hTreeItem);
			TreeView_EditLabel(_treeView.getHSelf(), addedItem);
		}
		break;

		case IDM_PROJECT_NEWWS :
		{
			if (_isDirty)
			{
				NativeLangSpeaker *pNativeSpeaker = (NppParameters::getInstance()).getNativeLangSpeaker();
				int res = pNativeSpeaker->messageBox("ProjectPanelNewDoSaveDirtyWsOrNot",
					_hSelf,
					TEXT("The current workspace was modified. Do you want to save the current project?"),
					TEXT("New Workspace"),
					MB_YESNOCANCEL | MB_ICONQUESTION | MB_APPLMODAL);
				if (res == IDYES)
				{
					if (!saveWorkSpace())
						return;
				}
				else if (res == IDNO)
				{
					// Don't save so do nothing here
				}
				else if (res == IDCANCEL)
				{
					// User cancels action "New Workspace" so we interrupt here
					return;
				}
			}
			_treeView.removeAllItems();
			newWorkSpace();
		}
		break;

		case IDM_PROJECT_RENAME :
			TreeView_EditLabel(_treeView.getHSelf(), hTreeItem);
		break;

		case IDM_PROJECT_NEWFOLDER :
		{
			NativeLangSpeaker *pNativeSpeaker = (NppParameters::getInstance()).getNativeLangSpeaker();
			String newFolderLabel = pNativeSpeaker->getAttrNameStr(PM_NEWFOLDERNAME, "ProjectManager", "NewFolderName");
			addFolder(hTreeItem, newFolderLabel.Begin());
			setWorkSpaceDirty(true);
		}
		break;

		case IDM_PROJECT_MOVEDOWN :
		{
			if (_treeView.moveDown(hTreeItem))
				setWorkSpaceDirty(true);
		}
		break;

		case IDM_PROJECT_MOVEUP :
		{
			if (_treeView.moveUp(hTreeItem))
				setWorkSpaceDirty(true);
		}
		break;

		case IDM_PROJECT_ADDFILES :
		{
			addFiles(hTreeItem);
			if (getNodeType(hTreeItem) == nodeType_folder)
				_treeView.setItemImage(hTreeItem, INDEX_OPEN_NODE, INDEX_OPEN_NODE);
		}
		break;

		case IDM_PROJECT_ADDFILESRECUSIVELY :
		{
			addFilesFromDirectory(hTreeItem);
			if (getNodeType(hTreeItem) == nodeType_folder)
				_treeView.setItemImage(hTreeItem, INDEX_OPEN_NODE, INDEX_OPEN_NODE);
		}
		break;

		case IDM_PROJECT_OPENWS:
		{
			if (!saveWorkspaceRequest())
				break;

			CustomFileDialog fDlg(_hSelf);
			setFileExtFilter(fDlg);
			const String fn = fDlg.doOpenSingleFileDlg();
			if (!fn.IsEmpty())
			{
				if (!openWorkSpace(fn.Begin(), true))
				{
					NativeLangSpeaker *pNativeSpeaker = (NppParameters::getInstance()).getNativeLangSpeaker();
					pNativeSpeaker->messageBox("ProjectPanelOpenFailed",
						_hSelf,
						TEXT("The workspace could not be opened.\rIt seems the file to open is not a valid project file."),
						TEXT("Open Workspace"),
						MB_OK);
					return;
				}
			}
		}
		break;

		case IDM_PROJECT_RELOADWS:
		{
			NativeLangSpeaker *pNativeSpeaker = (NppParameters::getInstance()).getNativeLangSpeaker();
			bool forceOpen = false;
			if (_isDirty)
			{
				int res = pNativeSpeaker->messageBox("ProjectPanelReloadDirty",
					_hSelf,
					TEXT("The current workspace was modified. Reloading will discard all modifications.\rDo you want to continue?"),
					TEXT("Reload Workspace"),
					MB_YESNO | MB_ICONQUESTION | MB_APPLMODAL);

				if (res == IDYES)
				{
					forceOpen = true;
				}
				else if (res == IDNO)
				{
					return;
				}
			}

			if (::PathFileExists(_workSpaceFilePath.Begin()))
			{
				openWorkSpace(_workSpaceFilePath.Begin(), forceOpen);
			}
			else
			{
				pNativeSpeaker->messageBox("ProjectPanelReloadError",
					_hSelf,
					TEXT("Cannot find the file to reload."),
					TEXT("Reload Workspace"),
					MB_OK);
			}
		}
		break;

		case IDM_PROJECT_SAVEWS:
			saveWorkSpace();
		break;

		case IDM_PROJECT_SAVEACOPYASWS:
		case IDM_PROJECT_SAVEASWS:
		{
			saveWorkSpaceAs(cmdID == IDM_PROJECT_SAVEACOPYASWS);
		}
		break;

		case IDM_PROJECT_FINDINPROJECTSWS:
		{
			::SendMessage(_hParent, NPPM_INTERNAL_FINDINPROJECTS, (WPARAM) 1 << _panelID, 0);
		}
		break;

		case IDM_PROJECT_DELETEFOLDER :
		{
			HTREEITEM parent = _treeView.getParent(hTreeItem);

			if (_treeView.getChildFrom(hTreeItem) != nullptr)
			{
				NativeLangSpeaker *pNativeSpeaker = (NppParameters::getInstance()).getNativeLangSpeaker();
				int res = pNativeSpeaker->messageBox("ProjectPanelRemoveFolderFromProject",
					_hSelf,
					TEXT("All the sub-items will be removed.\rAre you sure you want to remove this folder from the project?"),
					TEXT("Remove folder from project"),
					MB_YESNO);
				if (res == IDYES)
				{
					_treeView.removeItem(hTreeItem);
					setWorkSpaceDirty(true);
				}
			}
			else
			{
				_treeView.removeItem(hTreeItem);
				setWorkSpaceDirty(true);
			}
			if (getNodeType(parent) == nodeType_folder)
				_treeView.setItemImage(parent, INDEX_CLOSED_NODE, INDEX_CLOSED_NODE);
		}
		break;

		case IDM_PROJECT_DELETEFILE :
		{
			HTREEITEM parent = _treeView.getParent(hTreeItem);

			NativeLangSpeaker *pNativeSpeaker = (NppParameters::getInstance()).getNativeLangSpeaker();
			int res = pNativeSpeaker->messageBox("ProjectPanelRemoveFileFromProject",
				_hSelf,
				TEXT("Are you sure you want to remove this file from the project?"),
				TEXT("Remove file from project"),
				MB_YESNO);
			if (res == IDYES)
			{
				_treeView.removeItem(hTreeItem);
				setWorkSpaceDirty(true);
				if (getNodeType(parent) == nodeType_folder)
					_treeView.setItemImage(parent, INDEX_CLOSED_NODE, INDEX_CLOSED_NODE);
			}
		}
		break;

		case IDM_PROJECT_MODIFYFILEPATH :
		{
			FileRelocalizerDlg fileRelocalizerDlg;
			fileRelocalizerDlg.init(_hInst, _hParent);

			char textBuffer[MAX_PATH];
			TVITEM tvItem;
			tvItem.hItem = hTreeItem;
			tvItem.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			tvItem.pszText = textBuffer;
			tvItem.cchTextMax = MAX_PATH;

			SendMessage(_treeView.getHSelf(), TVM_GETITEM, 0, reinterpret_cast<LPARAM>(&tvItem));
			if (!tvItem.lParam)
				return;
			String * fn = (String *)tvItem.lParam;

			if (fileRelocalizerDlg.doDialog(fn->Begin()) == 0)
			{
				String newValue = fileRelocalizerDlg.getFullFilePath();
				if (*fn == newValue)
					return;

				*fn = newValue;
				char *strValueLabel = ::PathFindFileName(fn->Begin());
				wcscpy_s(textBuffer, strValueLabel);
				int iImage = ::PathFileExists(fn->Begin())?INDEX_LEAF:INDEX_LEAF_INVALID;
				tvItem.iImage = tvItem.iSelectedImage = iImage;
				SendMessage(_treeView.getHSelf(), TVM_SETITEM, 0, reinterpret_cast<LPARAM>(&tvItem));
				setWorkSpaceDirty(true);
			}
		}
		break;
	}
}

bool ProjectPanel::saveWorkSpaceAs(bool saveCopyAs)
{
	CustomFileDialog fDlg(_hSelf);
	setFileExtFilter(fDlg);
	fDlg.setExtIndex(0);		// 0 index for "custom extention" type if any else for "All types *.*"

	const String fn = fDlg.doSaveDlg();
	if (fn.IsEmpty())
		return false;

	if (!writeWorkSpace(fn.Begin()))
		return false;

	if (!saveCopyAs)
	{
		_workSpaceFilePath = fn;
		setWorkSpaceDirty(false);
	}
	return true;
}

void ProjectPanel::setFileExtFilter(CustomFileDialog & fDlg)
{
	const char *ext = NppParameters::getInstance().getNppGUI()._definedWorkspaceExt.Begin();
	String workspaceExt = TEXT("");
	if (*ext != '\0')
	{
		if (*ext != '.')
			workspaceExt += TEXT(".");
		workspaceExt += ext;
		fDlg.setExtFilter(TEXT("Workspace file"), workspaceExt.Begin());
		fDlg.setDefExt(ext);
	}
	fDlg.setExtFilter(TEXT("All types"), TEXT(".*"));
}

void ProjectPanel::addFiles(HTREEITEM hTreeItem)
{
	CustomFileDialog fDlg(_hSelf);
	fDlg.setExtFilter(TEXT("All types"), TEXT(".*"));

	const auto& fns = fDlg.doOpenMultiFilesDlg();
	if (!fns.IsEmpty())
	{
		size_t sz = fns.size();
		for (size_t i = 0 ; i < sz ; ++i)
		{
			char *strValueLabel = ::PathFindFileName(fns.at(i).Begin());

			String* pathFileStr = new String(fns.at(i));
			fullPathStrs.push_back(pathFileStr);
			LPARAM lParamPathFileStr = reinterpret_cast<LPARAM>(pathFileStr);

			_treeView.addItem(strValueLabel, hTreeItem, INDEX_LEAF, lParamPathFileStr);
		}
		_treeView.expand(hTreeItem);
		setWorkSpaceDirty(true);
	}
}

void ProjectPanel::recursiveAddFilesFrom(const char *folderPath, HTREEITEM hTreeItem)
{
	bool isRecursive = true;
	bool isInHiddenDir = false;
	String dirFilter(folderPath);
	if (folderPath[lstrlen(folderPath)-1] != '\\')
		dirFilter += TEXT("\\");

	dirFilter += TEXT("*.*");
	WIN32_FIND_DATA foundData;
	Vector<String> files;

	void* hFile = ::FindFirstFile(dirFilter.Begin(), &foundData);

	do {
		if (hFile == INVALID_HANDLE_VALUE)
			break;

		if (foundData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (!isInHiddenDir && (foundData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
			{
				// do nothing
			}
			else if (isRecursive)
			{
				if ((OrdinalIgnoreCaseCompareStrings(foundData.cFileName, TEXT(".")) != 0) && (OrdinalIgnoreCaseCompareStrings(foundData.cFileName, TEXT("..")) != 0))
				{
					String pathDir(folderPath);
					if (folderPath[lstrlen(folderPath)-1] != '\\')
						pathDir += TEXT("\\");
					pathDir += foundData.cFileName;
					pathDir += TEXT("\\");
					HTREEITEM addedItem = addFolder(hTreeItem, foundData.cFileName);
					recursiveAddFilesFrom(pathDir.Begin(), addedItem);
				}
			}
		}
		else
		{
			files.push_back(foundData.cFileName);
		}
	} while (::FindNextFile(hFile, &foundData));

	for (size_t i = 0, len = files.size() ; i < len ; ++i)
	{
		String pathFile(folderPath);
		if (folderPath[lstrlen(folderPath)-1] != '\\')
			pathFile += TEXT("\\");
		pathFile += files[i];

		String* pathFileStr = new String(pathFile);
		fullPathStrs.push_back(pathFileStr);
		LPARAM lParamPathFileStr = reinterpret_cast<LPARAM>(pathFileStr);
		_treeView.addItem(files[i].Begin(), hTreeItem, INDEX_LEAF, lParamPathFileStr);
	}

	::FindClose(hFile);
}

void ProjectPanel::addFilesFromDirectory(HTREEITEM hTreeItem)
{
	if (_selDirOfFilesFromDirDlg == TEXT("") && _workSpaceFilePath != TEXT(""))
	{
		char dir[MAX_PATH];
		wcscpy_s(dir, _workSpaceFilePath.Begin());
		::PathRemoveFileSpec(dir);
		_selDirOfFilesFromDirDlg = dir;
	}
	String dirPath;
	if (_selDirOfFilesFromDirDlg != TEXT(""))
		dirPath = getFolderName(_hSelf, _selDirOfFilesFromDirDlg.Begin());
	else
		dirPath = getFolderName(_hSelf);

	if (dirPath != TEXT(""))
	{
		recursiveAddFilesFrom(dirPath.Begin(), hTreeItem);
		_treeView.expand(hTreeItem);
		setWorkSpaceDirty(true);
		_selDirOfFilesFromDirDlg = dirPath;
	}
}

intptr_t CALLBACK FileRelocalizerDlg::run_dlgProc(UINT Message, WPARAM wParam, LPARAM)
{
	switch (Message)
	{
		case WM_INITDIALOG :
		{
			goToCenter();
			::SetDlgItemText(_hSelf, IDC_EDIT_FILEFULLPATHNAME, _fullFilePath.Begin());
			return TRUE;
		}
		case WM_COMMAND :
		{
			switch (wParam)
			{
				case IDOK :
				{
					char textBuf[MAX_PATH];
					::GetDlgItemText(_hSelf, IDC_EDIT_FILEFULLPATHNAME, textBuf, MAX_PATH);
					_fullFilePath = textBuf;
					::EndDialog(_hSelf, 0);
				}
				return TRUE;

				case IDCANCEL :
					::EndDialog(_hSelf, -1);
				return TRUE;

				default:
					return FALSE;
			}
		}
		default :
			return FALSE;
	}
}

int FileRelocalizerDlg::doDialog(const char *fn, bool isRTL)
{
	_fullFilePath = fn;

	if (isRTL)
	{
		DLGTEMPLATE *pMyDlgTemplate = nullptr;
		HGLOBAL hMyDlgTemplate = makeRTLResource(IDD_FILERELOCALIZER_DIALOG, &pMyDlgTemplate);
		int result = static_cast<int32_t>(::DialogBoxIndirectParam(_hInst, pMyDlgTemplate, _hParent, dlgProc, reinterpret_cast<LPARAM>(this)));
		::GlobalFree(hMyDlgTemplate);
		return result;
	}
	return static_cast<int32_t>(::DialogBoxParam(_hInst, MAKEINTRESOURCE(IDD_FILERELOCALIZER_DIALOG), _hParent, dlgProc, reinterpret_cast<LPARAM>(this)));
}

