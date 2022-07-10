#include "Browser.h"

#define LDUMP(x) // DDUMP(x)

Ткст PreprocessImlFile(const char *фн)
{
	Ткст s = загрузиФайл(фн);
	
	СиПарсер p(s);
	Ткст iml, i_ml;
	try {
		p.передайИд("PREMULTIPLIED");
		for(;;) {
			if(p.ид("IMAGE_ID")) {
				p.сим('(');
				Ткст id = p.читайИд();
				i_ml << "\tI_" << id << ",\n";
				iml << "\tstatic Рисунок " << id << "();\n";
				p.сим(')');
			}
			else
			if(p.ид("IMAGE_META")) {
				p.сим('(');
				p.читайТкст();
				p.сим(',');
				p.читайТкст();
				p.сим(')');
			}
			else
				break;
		}
	}
	catch(СиПарсер::Ошибка) {}

	Ткст r;
#ifdef PLATFORM_WIN32
	ФайлПоиск ff(фн);
	r << "struct " << дайТитулф(ff.дайПуть()) << "Img {\n";
#else
	r << "struct " << дайТитулф(фн) << "Img {\n";
#endif
	if(iml.дайСчёт())
		r << "\tenum {" << i_ml << "\t};\n" << iml;
	r <<
		"\n\tstatic Iml&   Iml();"
		"static int    найди(const РНЦП::Ткст& s);"
		"static int    найди(const char *s);"
		"static int    дайСчёт();"
		"static Ткст дайИд(int i);"
		"static Рисунок  дай(int i);"
		"static Рисунок  дай(const char *s);"
		"static Рисунок  дай(const РНЦП::Ткст& s);"
		"static void   уст(int i, const РНЦП::Рисунок& m);"
		"static void   уст(const char *s, const РНЦП::Рисунок& m);"
		"static void   переустанов();"
	"};\n";
	LDUMP(r);
	return r;
}
