#include "CtrlCore.h"

#ifdef GUI_X11

namespace РНЦП {

Вектор<Прям> RectRgn(const Прям& r)
{
	Вектор<Прям> q;
	q.добавь(r);
	return q;
}

Вектор<Прям> пересек(const Вектор<Прям>& r1, const Вектор<Прям>& r2)
{
	Вектор<Прям> q;
	for(int i = 0; i < r1.дайСчёт(); i++)
		for(int j = 0; j < r2.дайСчёт(); j++) {
			Прям r = r1[i] & r2[j];
			if(!r.пустой())
				q.добавь(r);
		}
	return q;
}

Вектор<Прям> Subtract(const Вектор<Прям>& r1, const Вектор<Прям>& r2)
{
	Вектор<Прям> q;
	bool dummy;
	q <<= r1;
	for(int i = 0; i < r2.дайСчёт(); i++)
		q = Subtract(q, r2[i], dummy);
	return q;
}

Вектор<Прям> Xor(const Вектор<Прям>& r1, const Вектор<Прям>& r2)
{
	Вектор<Прям> is = пересек(r1, r2);
	Вектор<Прям> q = Subtract(r1, is);
	q.приставь(Subtract(r2, is));
	return q;
}

}

#endif
