#include "Common.h"

static Вектор<МодульИСР *>& sM()
{
	static Вектор<МодульИСР *> m;
	return m;
}

void регМодульИСР(МодульИСР& module)
{
	sM().добавь(&module);
}

int  дайСчётМодульИСР()
{
	return sM().дайСчёт();
}

МодульИСР& дайМодульИСР(int q)
{
	return *sM()[q];
}
