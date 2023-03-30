#pragma once
//#include <PowerEditor/MISC/PluginsManager/Notepad_plus_msgs.h>
#include <PowerEditor/WinControls/WinControls.h>

typedef HRESULT (WINAPI *ETDTProc) (Window*, dword);

enum class PosAlign { left, right, top, bottom };

struct DLGTEMPLATEEX
{
      WORD   dlgVer = 0;
      WORD   signature = 0;
      dword  helpID = 0;
      dword  exStyle = 0;
      dword  style = 0;
      WORD   cDlgItems = 0;
      short  x = 0;
      short  y = 0;
      short  cx = 0;
      short  cy = 0;
      // The structure has more fields but are variable length
};
