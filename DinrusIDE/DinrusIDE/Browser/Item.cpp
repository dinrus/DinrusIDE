#include "Browser.h"

#define IMAGECLASS BrowserImg
#define IMAGEFILE <DinrusIDE/Browser/Browser.iml>
#include <Draw/iml_source.h>

#define LLOG(x) // DLOG(x)

bool IsCppKeyword(const Ткст& id)
{
	static Индекс<Ткст> kw;
	ONCELOCK {
		const char **cppk = CppKeyword();
		for(int i = 0; cppk[i]; i++)
			kw.добавь(cppk[i]);
	}
	return kw.найди(id) >= 0;
}

bool IsCppType(const Ткст& id)
{
	static const char *t[] = {
		"int", "long", "short", "void", "float", "double", "char", "signed", "unsigned", "bool",
	    "const", "mutable", "struct", "class", "union"
	};
	static Индекс<Ткст> kt;
	ONCELOCK {
		for(int i = 0; i < __countof(t); i++)
			kt.добавь(t[i]);
	}
	return kt.найди(id) >= 0;
}

int InScListIndext(const char *s, const char *list)
{
	int ii = 0;
	for(;;) {
		const char *q = s;
		while(*list == ' ') list++;
		for(;;) {
			if(*q == '\0' && *list == '\0') return ii;
			if(*q != *list) {
				if(*q == '\0' && (*list == '<' || *list == ';' || *list == ',' || *list == '>'))
					return ii;
				if(*list == '\0') return -1;
				break;
			}
			q++;
			list++;
		}
		while(*list && *list != ';' && *list != '<' && *list != '>' && *list != ',') list++;
		if(*list == '\0') return -1;
		list++;
		ii++;
	}
}

static Ткст s_pick_("pick_");

static bool sOperatorTab[256];

ИНИЦБЛОК {
	for(const char *s = "!+-*^/%~&|=[]:?."; *s; s++)
		sOperatorTab[(int)*s] = true;
}

inline bool sOperator(byte c)
{
	return sOperatorTab[c];
}

Вектор<ItemTextPart> ParseItemNatural(const Ткст& имя,
                                      const Ткст& natural,
                                      const Ткст& ptype, const Ткст& pname,
                                      const Ткст& тип, const Ткст& tname,
                                      const Ткст& ctname,
                                      const char *s)
{
	Вектор<ItemTextPart> part;
	int len = имя.дайДлину();
	if(len == 0) {
		ItemTextPart& p = part.добавь();
		p.pos = 0;
		p.len = natural.дайДлину();
		p.тип = ITEM_TEXT;
		p.pari = -1;
		return part;
	}
	bool param = false;
	int pari = -1;
	int par = 0;
	while(*s) {
		ItemTextPart& p = part.добавь();
		p.pos = (int)(s - ~natural);
		p.тип = ITEM_TEXT;
		p.pari = pari;
		int n = 1;
		if(*s >= '0' && *s <= '9') {
			while(s[n] >= '0' && s[n] <= '9' || (s[n] == 'x' || s[n] == 'X'))
				n++;
			p.тип = ITEM_NUMBER;
		}
		else
		if(iscid(*s) || *s == ':') {
			if(strncmp(s, имя, len) == 0 && !iscid(s[len])) {
				p.тип = ITEM_NAME;
				n = len;
				param = true;
			}
			else {
				Ткст id;
				n = 0;
				while(iscid(s[n]) || s[n] == ':')
					id.конкат(s[n++]);
				if(IsCppType(id))
					p.тип = ITEM_CPP_TYPE;
				else
				if(IsCppKeyword(id))
					p.тип = ITEM_CPP;
				else
				if(InScList(id, pname))
					p.тип = ITEM_PNAME;
				else
				if(id == s_pick_) {
					p.тип = ITEM_UPP;
				}
				else
				if(InScList(id, tname) || InScList(id, ctname))
					p.тип = ITEM_TNAME;
				else
				if(param) {
					int ii = InScListIndext(id, ptype);
					if(ii >= 0)
						p.тип = ITEM_PTYPE + ii;
				}
				else {
					int ii = InScListIndext(id, тип);
					if(ii >= 0)
						p.тип = ITEM_TYPE + ii;
				}
				LLOG("id: " << id << ", тип: " << p.тип);
			}
		}
		else
		if(sOperator(*s)) {
			while(sOperator(s[n]))
				n++;
			p.тип = ITEM_CPP;
		}
		else {
			p.тип = ITEM_SIGN;
			if(pari >= 0) {
				if(*s == '(' || *s == '<')
					par++;
				if(*s == ')' || *s == '>') {
					par--;
					if(par < 0) {
						p.pari = -1;
						pari = -1;
						param = false;
					}
				}
				if(*s == ',' && par == 0) {
					p.pari = -1;
					pari++;
				}
			}
			else
			if(*s == '(' && param) {
				pari = 0;
				par = 0;
			}
			while(s[n] && !iscid(s[n])) {  // Anonymous structure имя
				if(s[n] == '@') {
					p.len = n;
					return part;
				}
				n++;
			}
		}
		p.len = n;
		s += n;
	}
	return part;
}

Вектор<ItemTextPart> ParseItemNatural(const Ткст& имя, const CppItem& m, const char *s)
{
	return ParseItemNatural(имя, m.natural, m.ptype, m.pname, m.тип, m.tname, m.ctname, s);
}

Вектор<ItemTextPart> ParseItemNatural(const CppItemInfo& m, const char *s)
{
	return ParseItemNatural(m.имя, m, s);
}

Вектор<ItemTextPart> ParseItemNatural(const CppItemInfo& m)
{
	return ParseItemNatural(m, ~m.natural + m.at);
}
