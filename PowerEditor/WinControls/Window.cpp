#include "WinControls.h"

 void Upp::Window::init(const Upp::Window* hInst, const Upp::Window* parent)
	    {
	        _hInst = hInst;
	;
	        _hParent = parent;
	    }
	
void Upp::Window::display(bool toShow = true) const
	    {
	        _hInst->Show(toShow);
	    }
	
void Upp::Window::reSizeTo(Upp::Rect & rc) // should NEVER be const !!!
	    {
	        ::MoveWindow( (Window&)_hSelf, rc.left, rc.top, rc.right, rc.bottom, TRUE);
	        redraw();
	    }
	
	
void Upp::Window::reSizeToWH(Upp::Rect& rc) // should NEVER be const !!!
	    {
	        ::MoveWindow((Window&)_hSelf, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
	       redraw();
	    }
	
	
 void Upp::Window::redraw(bool forceUpdate = false) const
	    {
	        ::InvalidateRect((Window&)_hSelf, nullptr, TRUE);
	        if (forceUpdate)
	            ::UpdateWindow((Window&)_hSelf);
	    }
	
	
void Upp::Window::getClientRect(Upp::Rect& rc) const
	    {
	        ::GetClientRect((Window&)_hSelf, (LPRECT) &rc);
	    }
	
void Upp::Window::getWindowRect(Upp::Rect& rc) const
	    {
	        ::GetWindowRect((Window&)_hSelf, (LPRECT) &rc);
	    }
	
int Upp::Window::getWidth() const
	    {
	        Rect rc;
	        ::GetClientRect((Window&)_hSelf, (LPRECT) &rc);
	        return (rc.right - rc.left);
	      return 0;
	    }
	
int Upp::Window::getHeight() const
	    {
	        Rect rc;
	        ::GetClientRect((Window&)_hSelf, (LPRECT) &rc);
	        if (::IsWindowVisible((Window&)_hSelf) == TRUE)
	            return (rc.bottom - rc.top);
	       return 0;
	    }
	
bool Upp::Window::isVisible() const
	    {
	        return (::IsWindowVisible((Window&)_hSelf)?true:false);
	    }
	
Upp::Ctrl* Upp::Window::getHSelf() const
	    {
	        return _hSelf;
	    }
	
Upp::Ctrl* Upp::Window::getHParent() const {
	        return _hParent;
	    }
	
void Upp::Window::getFocus() const {
	        ::SetFocus((Window&)_hSelf);
	    }
	
Upp::Ctrl* Upp::Window::getHinst() const
	    {
	       ASSERT(_hInst != nullptr);
	        return _hInst;
	    }
