#include "CodeEditor.h"

namespace РНЦП {

МассивМап<Ткст, EditorSyntax::SyntaxDef>& EditorSyntax::defs()
{
	static МассивМап<Ткст, SyntaxDef> d;
	return d;
}

void EditorSyntax::регистрируй(const char *id, Событие<Один<EditorSyntax>&> factory,
                            const char *exts, const char *description)
{
	SyntaxDef& f = defs().дайДобавь(id);
	f.factory = factory;
	f.patterns = exts;
	f.description = description;
}

Один<EditorSyntax> EditorSyntax::создай(const char *id)
{
	Один<EditorSyntax> s;
	SyntaxDef *f = defs().найдиУк(id);
	if(f)
		f->factory(s);
	if(!s)
		s.создай();
	return s;
}

Ткст EditorSyntax::GetSyntaxForFilename(const char *фн)
{
	МассивМап<Ткст, SyntaxDef>& d = defs();
	for(int i = 0; i < d.дайСчёт(); i++)
		if(PatternMatchMulti(d[i].patterns, фн))
			return d.дайКлюч(i);
	return Null;
}

Ткст EditorSyntax::GetSyntaxDescription(int i)
{
	Ткст d = defs()[i].description;
	Ткст x = defs()[i].patterns;
	if(x.дайСчёт()) {
		x.замени("*", "");
		d << " (" << обрежьОба(x) << ")";
	}
	return d;
}

}
