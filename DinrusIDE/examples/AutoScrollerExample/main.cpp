#include <CtrlLib/CtrlLib.h>
#include <AutoScroller/AutoScroller.h>

#define LAYOUTFILE <examples/AutoScrollerExample/Layouts.lay>
#include <CtrlCore/lay.h>

class MyAppWindow : public WithAppLayout<Upp::TopWindow> {
public:
	MyAppWindow();
	void Toggle();

public:
	Upp::AutoScroller                       scroller;
	WithScrollerViewLayout<Upp::ParentCtrl> scroller_view;
};

void MyAppWindow::Toggle()
{
	scroller.EnableScroll(!scroller.IsScrollEnabled());
}

MyAppWindow::MyAppWindow()
{
	CtrlLayout(*this, "AutoScroller Example");
	Sizeable().Zoomable();
	
	scroller.EnableScroll();
	Add(scroller.SizePos());

	CtrlLayout(scroller_view);       // will determine desired size of pane
	scroller.AddPane(scroller_view); // size remains as specified, if view changes size, scroller.Layout() needs to be called.
	                                 // if scroller changes its size, it recalculates the scroll bars automatically

	// scroller.WhenScrolled = [=] {}; //after scrolling events

	scroller_view.toggle_button << [=] { Toggle(); };
	
	scroller_view.doc0 <<= "doc0";
	scroller_view.doc1 <<= "doc1";
	scroller_view.doc2 <<= "doc2";
	scroller_view.doc3 <<= "doc3";
}

GUI_APP_MAIN
{
	MyAppWindow().Run();
}
