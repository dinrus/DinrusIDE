
#pragma once
#include <CtrlCore/CtrlCore.h>

using namespace Upp;

namespace Upp{
	
	class Window : public Ctrl
	{
	public:
	    //! \name Constructors & Destructor
	    //@{
	    Window() = default;
	    Window(const Window&) = delete;
	    virtual ~Window() = default;
	    //@}
	
	
	    virtual void init(Window& hInst, Window& parent)
	    {
	        _hInst = &hInst;
	;
	        _hParent = &parent;
	    }
	
	    virtual void destroy() = 0;
	
	    virtual void display(bool toShow = true) const
	    {
	        _hInst->Show(toShow);
	    }
	
	
	    virtual void reSizeTo(Rect & rc) // should NEVER be const !!!
	    {
	        ::MoveWindow( (HWND)_hSelf, rc.left, rc.top, rc.right, rc.bottom, TRUE);
	        redraw();
	    }
	
	
	    virtual void reSizeToWH(Rect& rc) // should NEVER be const !!!
	    {
	        ::MoveWindow((HWND)_hSelf, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
	       redraw();
	    }
	
	
	    virtual void redraw(bool forceUpdate = false) const
	    {
	        ::InvalidateRect((HWND)_hSelf, nullptr, TRUE);
	        if (forceUpdate)
	            ::UpdateWindow((HWND)_hSelf);
	    }
	
	
	    virtual void getClientRect(Rect& rc) const
	    {
	        ::GetClientRect((HWND)_hSelf, (LPRECT) &rc);
	    }
	
	    virtual void getWindowRect(Rect& rc) const
	    {
	        ::GetWindowRect((HWND)_hSelf, (LPRECT) &rc);
	    }
	
	    virtual int getWidth() const
	    {
	        Rect rc;
	        ::GetClientRect((HWND)_hSelf, (LPRECT) &rc);
	        return (rc.right - rc.left);
	      return 0;
	    }
	
	    virtual int getHeight() const
	    {
	        Rect rc;
	        ::GetClientRect((HWND)_hSelf, (LPRECT) &rc);
	        if (::IsWindowVisible((HWND)_hSelf) == TRUE)
	            return (rc.bottom - rc.top);
	       return 0;
	    }
	
	    virtual bool isVisible() const
	    {
	        return (::IsWindowVisible((HWND)_hSelf)?true:false);
	    }
	
	    Ctrl* getHSelf() const
	    {
	        return _hSelf;
	    }
	
	    Ctrl* getHParent() const {
	        return _hParent;
	    }
	
	    void getFocus() const {
	        ::SetFocus((HWND)_hSelf);
	    }
	
	    Ctrl* getHinst() const
	    {
	       ASSERT(_hInst != nullptr);
	        return _hInst;
	    }
	
	
	    Window& operator = (const Window&) = delete;
	
	
	protected:
	    Ctrl*_hInst = nullptr;
	    Ctrl*_hParent = nullptr;
	    Ctrl*_hSelf = nullptr;
	};

}