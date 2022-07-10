#include "GeomCtrl.h"

namespace РНЦП {

EditDegree::EditDegree()
{
	добавьФрейм(spin);
	spin.inc.WhenRepeat = spin.inc.WhenPush = THISBACK(OnInc);
	spin.dec.WhenRepeat = spin.dec.WhenPush = THISBACK(OnDec);
}

void EditDegree::OnInc()
{
	double a = дайДанные();
	a = floor(Nvl(a) + 1.5);
	if(a >= Nvl(дайМакс(), +360.0))
		a = Nvl(дайМин(), -360.0);
	устДанные(a);
	Action();
}

void EditDegree::OnDec()
{
	double a = дайДанные();
	a = ceil(Nvl(a) - 1.5);
	if(a <= Nvl(дайМин(), -360.0))
		a = Nvl(дайМакс(), +360.0);
	устДанные(a);
	Action();
}

bool EditDegree::Ключ(dword ключ, int repcnt)
{
	if(ключ == K_ALT_UP)
	{
		OnInc();
		return true;
	}
	if(ключ == K_ALT_DOWN)
	{
		OnDec();
		return true;
	}
	return EditValue<double, ConvertDegree>::Ключ(ключ, repcnt);
}

}
