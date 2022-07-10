#ifndef _ide_About_h_
#define _ide_About_h_

#include <CtrlLib/CtrlLib.h>

namespace РНЦП {

class SplashCtrl final : public Ктрл
{
public:
	static Ткст GenerateVersionInfo(char separator = '\n');
	static Ткст GenerateVersionNumber();
	static Размер   MakeLogo(Ктрл& parent, Массив<Ктрл>& ctrl);
	
public:
	SplashCtrl();
	
private:
	Массив<Ктрл> ctrl;
};

}

#endif
