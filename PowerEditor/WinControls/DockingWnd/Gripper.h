// This file is part of Notepad++ project
// Copyright (C)2006 Jens Lorenz <jens.plugin.npp@gmx.de>

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

//#include <windows.h>
//#include <commctrl.h>
#include <PowerEditor/MISC/Common/Common.h>
#include "Docking.h"
#include "dockingResource.h"

class DockingCont;
class DockingManager;

// For the following #define see the comments at drawRectangle() definition. (jg)
#define USE_LOCKWINDOWUPDATE


// Used by getRectAndStyle() to draw the drag rectangle
static const WORD DotPattern[] =
{
    0x00aa, 0x0055, 0x00aa, 0x0055, 0x00aa, 0x0055, 0x00aa, 0x0055
};


#define MDLG_CLASS_NAME TEXT("moveDlg")


class Gripper final
{
public:
    Gripper() = default;;

    void init(Window& hInst, Window* hParent) {
        _hInst   = hInst;
        _hParent = hParent;
        dword hwndExStyle = (dword)GetWindowLongPtr(_hParent, GWL_EXSTYLE);
        _isRTL = hwndExStyle & WS_EX_LAYOUTRTL;
    };

    void startGrip(DockingCont* pCont, DockingManager* pDockMgr);

    ~Gripper() {
        if (_hdc) {
            // usually this should already have been done by a call to drawRectangle(),
            // here just for cases where usual handling was interrupted (jg)
            #ifdef USE_LOCKWINDOWUPDATE
            ::LockWindowUpdate(nullptr);
            #endif
            ::ReleaseDC(0, _hdc);
        }
        if (_hbm) {
            ::DeleteObject(_hbm);
        }
        if (_hbrush) {
            ::DeleteObject(_hbrush);
        }
    };

protected :

    void create();

    static LRESULT CALLBACK staticWinProc(Window* hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
    LRESULT runProc(UINT Message, WPARAM wParam, LPARAM lParam);

    void onMove();
    void onButtonUp();

    void doTabReordering(POINT pt);
    void drawRectangle(const POINT* pPt);
    void getMousePoints(POINT* pt, POINT* ptPrev);
    void getMovingRect(POINT pt, Rect *rc);
    DockingCont * contHitTest(POINT pt);
    DockingCont * workHitTest(POINT pt, Rect *rcCont = nullptr);

    void initTabInformation();

    void CalcRectToScreen(Window* hWnd, Rect *rc) {
        ClientRectToScreenRect(hWnd, rc);
        ShrinkRcToSize(rc);
    };
    void CalcRectToClient(Window* hWnd, Rect *rc) {
        ScreenRectToClientRect(hWnd, rc);
        ShrinkRcToSize(rc);
    };
    void ShrinkRcToSize(Rect *rc) {
        _isRTL ? rc->right = rc->left - rc->right : rc->right -= rc->left;
        rc->bottom -= rc->top;
    };
    void DoCalcGripperRect(Rect* rc, Rect rcCorr, POINT pt) {
        if ((rc->left + rc->right) < pt.x)
            rc->left = pt.x - 20;
        if ((rc->top + rc->bottom) < pt.y)
            rc->top  += rcCorr.bottom - rc->bottom;
    };

private:
    // Handle
    Window& _hInst = nullptr;
    Window* _hParent = nullptr;
    Window* _hSelf = nullptr;

    // data of container
    tDockMgr _dockData;
    DockingManager *_pDockMgr = nullptr;
    DockingCont *_pCont = nullptr;

    // mouse offset in moving rectangle
    POINT _ptOffset = {};

    // remembers old mouse point
    POINT _ptOld = {};
    BOOL _bPtOldValid = FALSE;

    // remember last drawn rectangle (jg)
    Rect _rcPrev = {};

    // for sorting tabs
    Window* _hTab = nullptr;
    Window* _hTabSource = nullptr;
    BOOL _startMovingFromTab = FALSE;
    int _iItem = 0;
    Rect _rcItem = {};
    TCITEM _tcItem;

    HDC _hdc = nullptr;
    HBITMAP _hbm = nullptr;
    HBRUSH _hbrush = nullptr;

    // is class registered
    static BOOL _isRegistered;

    // get layout direction
    bool _isRTL = false;
};

