#pragma once

#include "dockingResource.h"
#include "Docking.h"

#include <PowerEditor/MISC/Common/Common.h>
#include <PowerEditor/WinControls/StaticDialog/StaticDialog.h>
#include <PowerEditor/NppDarkMode.h>
#include <PowerEditor/MISC/PluginsManager/Notepad_plus_msgs.h>



class DockingDlgInterface : public StaticDialog
{
public:
	DockingDlgInterface() = default;
	explicit DockingDlgInterface(int dlgID): _dlgID(dlgID) {}

	virtual void init(Window& hInst, Window* parent) {
		StaticDialog::init(hInst, parent);
		char temp[MAX_PATH];
		::GetModuleFileName(reinterpret_cast<HMODULE>(hInst), temp, MAX_PATH);
		_moduleName = ::PathFindFileName(temp);
	}

    void create(tTbData* data, bool isRTL = false) {
		assert(data != nullptr);
		StaticDialog::create(_dlgID, isRTL);
		char temp[MAX_PATH];
		::GetWindowText(_hSelf, temp, MAX_PATH);
		_pluginName = temp;

        // user information
		data->hClient = _hSelf;
		data->pszName = _pluginName.Begin();

		// supported features by plugin
		data->uMask = 0;

		// additional info
		data->pszAddInfo = nullptr;
	}

	virtual void updateDockingDlg() {
		::SendMessage(_hParent, NPPM_DMMUPDATEDISPINFO, 0, reinterpret_cast<LPARAM>(_hSelf));
	}

    virtual void destroy() {
		StaticDialog::destroy();
	}

	virtual void setBackgroundColor(Color&) {}
	virtual void setForegroundColor(Color&) {}

	virtual void display(bool toShow = true) const {
		::SendMessage(_hParent, toShow ? NPPM_DMMSHOW : NPPM_DMMHIDE, 0, reinterpret_cast<LPARAM>(_hSelf));
	}

	bool isClosed() const {
		return _isClosed;
	}

	void setClosed(bool toClose) {
		_isClosed = toClose;
	}

	const char * getPluginFileName() const {
		return _moduleName.Begin();
	}

protected :
	int	_dlgID = -1;
	bool _isFloating = true;
	int _iDockedPos = 0;
	String _moduleName;
	String _pluginName;
	bool _isClosed = false;

	virtual intptr_t CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message)
		{
			case WM_ERASEBKGND:
			{
				if (!NppDarkMode::isEnabled())
				{
					break;
				}

				Rect rc = {};
				getClientRect(rc);
				::FillRect(reinterpret_cast<HDC>(wParam), &rc, NppDarkMode::getDarkerBackgroundBrush());
				return TRUE;
			}
			case WM_NOTIFY:
			{
				LPNMHDR	pnmh = reinterpret_cast<LPNMHDR>(lParam);

				if (pnmh->hwndFrom == _hParent)
				{
					switch (LOWORD(pnmh->code))
					{
						case DMN_CLOSE:
						{
							break;
						}
						case DMN_FLOAT:
						{
							_isFloating = true;
							break;
						}
						case DMN_DOCK:
						{
							_iDockedPos = HIWORD(pnmh->code);
							_isFloating = false;
							break;
						}
						default:
							break;
					}
				}
				break;
			}
			default:
				break;
		}
		return FALSE;
	};
};
