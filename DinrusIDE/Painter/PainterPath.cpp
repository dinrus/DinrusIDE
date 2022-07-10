#include "Painter.h"

namespace РНЦП {

Рисовало& Рисовало::Path(СиПарсер& p)
{
	ТочкаПЗ current(0, 0);
	bool done = false;
	while(!p.кф_ли()) {
		int c = p.дайСим();
		p.пробелы();
		bool rel = заг_ли(c);
		ТочкаПЗ t, t1, t2;
		auto читайДво = [&] {
			while(p.сим(','));
			return p.дво2_ли() ? p.читайДво() : 0;
		};
		auto ReadPointP = [&](ТочкаПЗ current, bool rel) {
			ТочкаПЗ t;
			t.x = читайДво();
			t.y = читайДво();
			if(rel)
				t += current;
			return t;
		};
		auto ReadPoint = [&]() { return ReadPointP(current, rel); };
		switch(взаг(c)) {
		case 'M':
			current = ReadPoint();
			Move(current);
		case 'L':
			while(p.дво2_ли()) {
				current = ReadPoint();
				Строка(current);
			}
			done = true;
			break;
		case 'Z':
			закрой();
			done = true;
			break;
		case 'H':
			while(p.дво2_ли()) {
				current.x = p.читайДво() + rel * current.x;
				Строка(current);
				done = true;
			}
			break;
		case 'V':
			while(p.дво2_ли()) {
				current.y = p.читайДво() + rel * current.y;
				Строка(current);
				done = true;
			}
			break;
		case 'C':
			while(p.дво2_ли()) {
				t1 = ReadPoint();
				t2 = ReadPoint();
				current = ReadPoint();
				Cubic(t1, t2, current);
				done = true;
			}
			break;
		case 'S':
			while(p.дво2_ли()) {
				t2 = ReadPoint();
				current = ReadPoint();
				Cubic(t2, current);
				done = true;
			}
			break;
		case 'Q':
			while(p.дво2_ли()) {
				t1 = ReadPoint();
				current = ReadPoint();
				Quadratic(t1, current);
				done = true;
			}
			break;
		case 'T':
			while(p.дво2_ли()) {
				current = ReadPoint();
				Quadratic(current);
				done = true;
			}
			break;
		case 'A':
			while(p.дво2_ли()) {
				t1 = ReadPointP(ТочкаПЗ(0, 0), false);
				double xangle = читайДво();
				auto ReadBool = [&] {
					while(p.сим(','));
					if(p.сим('1')) return true;
					p.сим('0');
					return false;
				};
				bool large = ReadBool();
				bool sweep = ReadBool();
				current = ReadPoint();
				SvgArc(t1, xangle * M_PI / 180.0, large, sweep, current);
				done = true;
			}
			break;
		default:
			if(!done)
				Move(0, 0); // to clear previous path
			return *this;
		}
	}
	if(!done)
		Move(0, 0); // to clear previous path
	return *this;
}

Рисовало& Рисовало::Path(const char *path)
{
	try {
		СиПарсер p(path);
		Path(p);
	}
	catch(СиПарсер::Ошибка) {}
	return *this;
}

}
