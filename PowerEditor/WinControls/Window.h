
#pragma once
#include <CtrlCore/CtrlCore.h>
#include <PowerEditor/TinyXml/tinyxml.h>

class Window : Ctrl
{
public:
    //! \name Constructors & Destructor
    //@{
    Window() = default;
    Window(const Window&) = delete;
    virtual ~Window() = default;
    //@}


    virtual void init(Ctrl& hInst, Ctrl& parent)
    {
        _hInst = &hInst;
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

    Upp::Ctrl* getHSelf() const
    {
        return _hSelf;
    }

    Upp::Ctrl* getHParent() const {
        return _hParent;
    }

    void getFocus() const {
        ::SetFocus((HWND)_hSelf);
    }

    HINSTANCE getHinst() const
    {
        //assert(_hInst != 0);
        return (HINSTANCE)_hInst;
    }


    Window& operator = (const Window&) = delete;


protected:
    Upp::Ctrl*_hInst = nullptr;
    Upp::Ctrl*_hParent = nullptr;
    Upp::Ctrl*_hSelf = nullptr;
};