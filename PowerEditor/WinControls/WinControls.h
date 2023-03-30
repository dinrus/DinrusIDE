#ifndef _PowerEditor_WinControls_WinControls_h_
#define _PowerEditor_WinControls_WinControls_h_

#include <Core/Core.h>
#include <CtrlLib/CtrlLib.h>
#include <PowerEditor/MISC/Common/Common.h>

namespace Upp{
	
#define LICENCE_TXT \
TEXT("This program is free software; you can redistribute it and/or \
modify it under the terms of the GNU General Public License \
as published by the Free Software Foundation; either \
version 3 of the License, or at your option any later version.\r\n\
\r\n\
This program is distributed in the hope that it will be useful, \
but WITHOUT ANY WARRANTY; without even the implied warranty of \
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the \
GNU General Public License for more details. \r\n\
\r\n\
You should have received a copy of the GNU General Public License \
along with this program. If not, see <https://www.gnu.org/licenses/>.")
	
	class Window : public Ctrl
	{
		
	public:
	    Window() = default;
	    Window(const Window&) = delete;
	    virtual ~Window() = default;
	
	    virtual void init(const Window* hInst, const Window* parent);
	    virtual void destroy() = 0;
	    virtual void display(bool toShow = true) const;
	    virtual void reSizeTo(Rect & rc);
	    virtual void reSizeToWH(Rect& rc) ;
	    virtual void redraw(bool forceUpdate = false) const;
	    virtual void getClientRect(Rect& rc) const;
	    virtual void getWindowRect(Rect& rc) const;
	    virtual int getWidth() const;
	    virtual int getHeight() const;
	    virtual bool isVisible() const;
	    Ctrl* getHSelf() const;
	    Ctrl* getHParent() const ;
	    void getFocus() const ;
	    Ctrl* getHinst() const;
	
	    Window& operator = (const Window&) = delete;
	
	protected:
	    Ctrl*_hInst = nullptr;
	    Ctrl*_hParent = nullptr;
	    Ctrl*_hSelf = nullptr;
	};
	
///////////////////////////////////
class StaticDialog : public Window
{
public :
	virtual ~StaticDialog();

	virtual void create(int dialogID, bool isRTL = false, bool msgDestParent = true);

    virtual bool isCreated() const {
		return (_hSelf != nullptr);
	}

	void goToCenter();

	void display(bool toShow = true, bool enhancedPositioningCheckWhenShowing = false) const;

	Rect getViewablePositionRect(Rect testRc) const;

	POINT getTopPoint(Window* hwnd, bool isLeft = true) const;

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
	Upp::Rect _rc = {};
	static intptr_t CALLBACK dlgProc(Window* hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual intptr_t CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam) = 0;

	HGLOBAL makeRTLResource(int dialogID, DLGTEMPLATE **ppMyDlgTemplate);
};
/////////////////////////////////////////////////////////////////////////////

	class URLCtrl : public Window {
	public:
	    void create(Window* itemHandle, const char * link, const Color& linkColor = Color(0,0,255));
		void create(Window* itemHandle, int cmd, Window* msgDest = nullptr);
	    void destroy();
	private:
		void action();
	protected :
	    String _URL;
	    HFONT _hfUnderlined = nullptr;
	    HCURSOR _hCursor = nullptr;
	
		Window* _msgDest = nullptr;
		unsigned long _cmdID = 0;
	
	    WNDPROC  _oldproc = nullptr;
	    const Color& _linkColor = Color(0xFF, 0xFF, 0xFF);
	    const Color& _visitedColor = Color(0xFF, 0xFF, 0xFF);
	    bool  _clicking = false;
	
	    static LRESULT CALLBACK URLCtrlProc(Window* hwnd, UINT Message, WPARAM wParam, LPARAM lParam){
	        return ((URLCtrl *)(::GetWindowLongPtr((HWND) hwnd, GWLP_USERDATA)))->runProc(hwnd, Message, wParam, lParam);
	    };
	    LRESULT runProc(Window* hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
	};
////////////////////////////////////////
class AboutDlg : public StaticDialog
	{
	public :
		AboutDlg() = default;
	
		void doDialog();
	
	    virtual void destroy() {
	        _emailLink.destroy();
	        _pageLink.destroy();
	    };
	
	protected :
		virtual intptr_t CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
	
	private :
	    URLCtrl _emailLink;
	    URLCtrl _pageLink;
	};
	
/////////////////////////////////////////////////////////////
	class DebugInfoDlg : public StaticDialog
	{
	public:
		DebugInfoDlg() = default;
	
		void init(Window& hInst, Window* parent, bool isAdmin, const char* loadedPlugins) {
			_isAdmin = isAdmin;
			_loadedPlugins = loadedPlugins;
			Window::init(hInst.getHinst(), parent);
		};
	
		void doDialog();
	
		void refreshDebugInfo();
	
		virtual void destroy() {
			_copyToClipboardLink.destroy();
		};
	
	protected:
		virtual intptr_t CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
	
	private:
		typedef const CHAR * (__cdecl * PWINEGETVERSION)();
		String _debugInfoStr;
		String _debugInfoDisplay;
		const String _cmdLinePlaceHolder = "$COMMAND_LINE_PLACEHOLDER$";
		bool _isAdmin = false;
		String _loadedPlugins;
		URLCtrl _copyToClipboardLink;
	};
	
	class DoSaveOrNotBox : public StaticDialog
	{
	public:
		DoSaveOrNotBox() = default;
	
		void init(Window& hInst, Window* parent, const char* fn, bool isMulti) {
			Window::init(hInst, parent);
			if (fn)
				_fn = fn;
	
			_isMulti = isMulti;
		};
	
		void doDialog(bool isRTL = false);
	
		virtual void destroy() {};
	
		int getClickedButtonId() const {
			return clickedButtonId;
		};
	
		void changeLang();
	
	protected:
		virtual intptr_t CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
	
	private:
		int clickedButtonId = -1;
		String _fn;
		bool _isMulti = false;
	};
///////////////////////////////////
	class DoSaveAllBox : public StaticDialog
	{
	public:
		DoSaveAllBox() = default;
		void doDialog(bool isRTL = false);
		virtual void destroy() {};
	
		int getClickedButtonId() const {
			return clickedButtonId;
		};
	
		void changeLang();
	
	protected:
		virtual intptr_t CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
	
	private:
		int clickedButtonId = -1;
	};
///////////////////////////////////////

////////////////////////////////////////
}//end of ns
#endif
