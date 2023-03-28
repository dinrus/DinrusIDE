#ifndef _PowerEditor_WinControls_WinControls_h_
#define _PowerEditor_WinControls_WinControls_h_

namespace Upp{
	
	class Window : public Ctrl
	{
		
	public:
	    Window() = default;
	    Window(const Window&) = delete;
	    virtual ~Window() = default;
	
	    virtual void init(Window& hInst, Window& parent);
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

	class URLCtrl : public Window {
	public:
	    void create(Window* itemHandle, const char * link, const Color linkColor = Color(0,0,255));
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
	    const Color _linkColor = Color(0xFF, 0xFF, 0xFF);
	    const Color _visitedColor = Color(0xFF, 0xFF, 0xFF);
	    bool  _clicking = false;
	
	    static LRESULT CALLBACK URLCtrlProc(Window* hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
	    LRESULT runProc(Window* hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
	};
////////////////////////////////////////

////////////////////////////////////////
}//end of ns
#endif
