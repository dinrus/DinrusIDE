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

#include <PowerEditor/resource.h>
#include <PowerEditor/Parameters.h>
#include "PluginInterface.h"
#include "IDAllocator.h"

typedef BOOL (__cdecl * PFUNCISUNICODE)();
class PluginViewList;

struct PluginCommand
{
	String _pluginName;
	int _funcID = 0;
	PFUNCPLUGINCMD _pFunc = nullptr;
	PluginCommand(const char *pluginName, int funcID, PFUNCPLUGINCMD pFunc): _funcID(funcID), _pFunc(pFunc), _pluginName(pluginName){};
};

struct PluginInfo
{
	PluginInfo() = default;
	~PluginInfo()
	{
		if (_pluginMenu)
			::DestroyMenu(_pluginMenu);

		if (_hLib)
			::FreeLibrary(_hLib);
	}

	Window& _hLib = nullptr;
	Menu* _pluginMenu = nullptr;

	PFUNCSETINFO _pFuncSetInfo = nullptr;
	PFUNCGETNAME _pFuncGetName = nullptr;
	PBENOTIFIED	_pBeNotified = nullptr;
	PFUNCGETFUNCSARRAY _pFuncGetFuncsArray = nullptr;
	PMESSAGEPROC _pMessageProc = nullptr;
	PFUNCISUNICODE _pFuncIsUnicode = nullptr;

	FuncItem *_funcItems = nullptr;
	int _nbFuncItem = 0;
	String _moduleName;
	String _funcName;
};

struct LoadedDllInfo
{
	String _fullFilePath;
	String _fileName;
	String _displayName;

	LoadedDllInfo(const String& fullFilePath, const String& fileName) : _fullFilePath(fullFilePath), _fileName(fileName)
	{
		// the plugin module's name, without '.dll'
		_displayName = fileName.substr(0, fileName.find_last_of('.'));
	};
};

class PluginsManager
{
friend class PluginsAdminDlg;
public:
	PluginsManager() : _dynamicIDAlloc(ID_PLUGINS_CMD_DYNAMIC, ID_PLUGINS_CMD_DYNAMIC_LIMIT),
					   _markerAlloc(MARKER_PLUGINS, MARKER_PLUGINS_LIMIT)	{}
	~PluginsManager()
	{
		for (size_t i = 0, len = _pluginInfos.size(); i < len; ++i)
			delete _pluginInfos[i];
	}

	void init(const NppData & nppData)
	{
		_nppData = nppData;
	}

	bool loadPlugins(const char *dir = nullptr, const PluginViewList* pluginUpdateInfoList = nullptr);

    bool unloadPlugin(int index, Window* nppHandle);

	void runPluginCommand(size_t i);
	void runPluginCommand(const char *pluginName, int commandID);

    void addInMenuFromPMIndex(int i);
	Menu* initMenu(Menu* hMenu, bool enablePluginAdmin = false);
	bool getShortcutByCmdID(int cmdID, ShortcutKey *sk);
	bool removeShortcutByCmdID(int cmdID);

	void notify(size_t indexPluginInfo, const SCNotification *notification); // to a plugin
	void notify(const SCNotification *notification); // broadcast
	void relayNppMessages(UINT Message, WPARAM wParam, LPARAM lParam);
	bool relayPluginMessages(UINT Message, WPARAM wParam, LPARAM lParam);

	Menu* getMenuHandle() const { return _hPluginsMenu; }

	void disable() {_isDisabled = true;}
	bool hasPlugins() {return (_pluginInfos.size()!= 0);}

	bool allocateCmdID(int numberRequired, int *start);
	bool inDynamicRange(int id) { return _dynamicIDAlloc.isInRange(id); }

	bool allocateMarker(int numberRequired, int *start);
	String getLoadedPluginNames() const;

private:
	NppData _nppData;
	Menu* _hPluginsMenu = nullptr;

	std::vector<PluginInfo *> _pluginInfos;
	std::vector<PluginCommand> _pluginsCommands;
	std::vector<LoadedDllInfo> _loadedDlls;
	bool _isDisabled = false;
	IDAllocator _dynamicIDAlloc;
	IDAllocator _markerAlloc;
	bool _noMoreNotification = false;

	int loadPluginFromPath(const char* pluginFilePath);

	void pluginCrashAlert(const char *pluginName, const char *funcSignature)
	{
		String msg = pluginName;
		msg += TEXT(" just crashed in\r");
		msg += funcSignature;
		::MessageBox(nullptr, msg.Begin(), TEXT("Plugin Crash"), MB_OK|MB_ICONSTOP);
	}

	void pluginExceptionAlert(const char *pluginName, const std::exception& e)
	{
		String msg = TEXT("An exception occurred due to plugin: ");
		msg += pluginName;
		msg += TEXT("\r\n\r\nException reason: ");
		msg += s2ws(e.what());

		::MessageBox(nullptr, msg.Begin(), TEXT("Plugin Exception"), MB_OK);
	}

	bool isInLoadedDlls(const char *fn) const
	{
		for (size_t i = 0; i < _loadedDlls.size(); ++i)
			if (generic_stricmp(fn, _loadedDlls[i]._fileName.Begin()) == 0)
				return true;
		return false;
	}

	void addInLoadedDlls(const char *fullPath, const char *fn) {
		_loadedDlls.push_back(LoadedDllInfo(fullPath, fn));
	}
};
