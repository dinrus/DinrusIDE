#include "DinrusIDE.h"

#if 0
#define LDUMP(x)     DDUMP(x)
#define LDUMPC(x)    DDUMPC(x)
#define LLOG(x)      DLOG(x)
#else
#define LDUMP(x)
#define LDUMPC(x)
#define LLOG(x)
#endif

void AssistEditor::DCopy()
{
	Ткст r;
	int l, h;
	bool decla = false;
	if(!дайВыделение32(l, h)) {
		int i = дайСтроку(дайКурсор32());
		l = дайПоз32(i);
		h = l;
		while(h < дайДлину32() && h - l < 1000) {
			int c = дайСим(h);
			if(c == ';') {
				decla = true;
				break;
			}
			if(c == '{')
				break;
			h++;
			if(c == '\"') {
				while(h < дайДлину32()) {
					int c = дайСим(h);
					if(c == '\"' || c == '\n')
						break;
					h++;
					if(c == '\\' && h < дайДлину32())
						h++;
				}
			}
		}
	}
	else
		decla = true;

	ParserContext ctx;
	Контекст(ctx, l);
	Ткст txt = дай(l, h - l);
	Ткст cls = ctx.current_scope;
	int best = 0;
	const Индекс<Ткст>& ns = CodeBase().namespaces;
	for(int i = 0; i < ns.дайСчёт(); i++) {
		Ткст h = ns[i];
		for(int pass = 0; pass < 2; pass++) {
			if(h.дайСчёт() > best && cls.начинаетсяС(h))
				best = h.дайСчёт();
			h << "::";
		}
	}
	cls.удали(0, best);

	CppBase cpp;
	SimpleParse(cpp, txt, cls);

	if(cpp.дайСчёт() == 0) { // scan for THISBACKs
		Индекс<Ткст> id;
		СиПарсер p(txt);
		try {
			while(!p.кф_ли()) {
				if(p.ид("THISBACK") && p.сим('('))
					id.найдиДобавь(p.читайИд());
				p.пропустиТерм();
			}
		}
		catch(СиПарсер::Ошибка) {
		}
		for(int i = 0; i < id.дайСчёт(); i++)
			r << "\tvoid " << id[i] << "();\n";
	}
	else
		for(int i = 0; i < cpp.дайСчёт(); i++) {
			const Массив<CppItem>& n = cpp[i];
			bool decl = decla;
			for(int j = 0; j < n.дайСчёт(); j++)
				if(n[j].impl)
					decl = false;
			for(int j = 0; j < n.дайСчёт(); j++) {
				const CppItem& m = n[j];
				if(m.IsCode()) {
					if(decl)
						r << MakeDefinition(cls, m.natural) << "\n{\n}\n\n";
					else {
						if(cpp.IsType(i))
						   r << Ткст('\t', разбей(cpp.дайКлюч(i), ':').дайСчёт());
						r << m.natural << ";\n";
					}
				}
				if(m.IsData()) {
					Ткст nat = m.natural;
					if(cls.дайСчёт()) {
						nat.замени("static", "");
						nat = обрежьЛево(nat);
						const char *s = nat;
						while(*s) {
							if(iscib(*s)) {
								const char *b = s;
								while(iscid(*s)) s++;
								Ткст id(b, s);
								if(m.имя == id) {
									if(cls.дайСчёт())
										r << cls << "::" << m.имя << s;
									else
										r << m.имя << s;
									break;
								}
								r << id;
							}
							else
								r << *s++;
						}
					}
					else {
						int q = nat.найдирек("::");
						if(q >= 0) { // Foo Class2 :: Class::variable; -> static Foo variable;
							int e = q + 2;
							for(;;) {
								while(q >= 0 && nat[q - 1] == ' ')
									q--;
								if(q == 0 || !iscid(nat[q - 1]))
									break;
								while(q >= 0 && iscid(nat[q - 1]))
									q--;
								int w = nat.найдирек("::", q);
								if(w < 0)
									break;
								q = w;
							}
							nat.удали(q, e - q);
							r << "static " << nat;
						}
						else
							r << "extern " << nat;
					}
					r << ";\n";
				}
			}
		}
	WriteClipboardText(r);
}
