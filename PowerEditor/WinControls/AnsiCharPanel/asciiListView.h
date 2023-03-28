#pragma once

#include <PowerEditor/WinControls/AnsiCharPanel/ListView.h>

namespace Upp{
	class AsciiListView : public ListView
	{
	public:
		void setValues(int codepage = 0);
		void resetValues(int codepage);
	
		String getAscii(unsigned char value);
		String getHtmlName(unsigned char value);
		int getHtmlNumber(unsigned char value);
	private:
		int _codepage = -1;
	};

}