#include "DinrusIDE.h"

Ткст Иср::GetAndroidSdkPath()
{
	ВекторМап<Ткст, Ткст> bm = GetMethodVars(method);
	if(bm.дай("BUILDER", "") == "ANDROID")
		return bm.дай("SDK_PATH", Ткст());
	return Ткст();
}
