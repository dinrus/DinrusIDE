#include "Browser.h"

#define LTIMING(x) // RTIMING(x)
#define LDUMP(x)   // DUMP(x)

void LaySkipRest(СиПарсер& p)
{
	int lvl = 1;
	while(!p.кф_ли()) {
		if(p.сим('('))
			lvl++;
		else
		if(p.сим(')')) {
			if(--lvl == 0)
				break;
		}
		else
			p.пропустиТерм();
	}
}

inline void WriteLines(Ткст& r, int count)
{
	for (int i = 0; i < count; ++i)
		r << '\n';
}

Ткст PreprocessLayFile(const char *фн)
{
	LTIMING("Lay file");
	Ткст s = загрузиФайл(фн);
	СиПарсер p(s);

	Ткст r = "using namespace РНЦП;";
	try {
		int line = p.дайСтроку();
		if(p.сим('#') && p.ид("ifdef")) {
			if(!p.ид("LAYOUTFILE"))
				p.ид("LAYOUT");
			WriteLines(r, p.дайСтроку() - line);
		}
		while(!p.кф_ли()) {
			line = p.дайСтроку();
			p.передайИд("LAYOUT");
			p.передайСим('(');
			Ткст id = p.читайИд();
			r << "void SetLayout_" + id + "(); template <class T> struct With" << id << " : public T {"
			  << "\tstatic Размер GetLayoutSize();";
			LaySkipRest(p);
			WriteLines(r, p.дайСтроку() - line);
			for(;;) {
				line = p.дайСтроку();
				if(p.ид("ЭЛТ")) {
					p.передайСим('(');
					if(p.ид_ли()) {
						Ткст тип = p.ReadIdt();
						int q = тип.найдирек(':');
						if(q >= 0)
							тип = тип.середина(q + 1);
						p.передайСим(',');
						Ткст имя = p.читайИд();
						if(!имя.начинаетсяС("dv___")) {
							r << '\t' << тип;
							r << ' ' << имя << ";";
						}
					}
				}
				else
				if(p.ид("UNTYPED"))
					p.сим('(');
				else
					break;
				LaySkipRest(p);
				WriteLines(r, p.дайСтроку() - line);
			}
			line = p.дайСтроку();
			p.передайИд("END_LAYOUT");
			if(p.сим('#'))
				p.ид("endif");
			r << "};";
			WriteLines(r, p.дайСтроку() - line);
		}
	}
	catch(СиПарсер::Ошибка) {}
	LDUMP(r);
	return r;
}
