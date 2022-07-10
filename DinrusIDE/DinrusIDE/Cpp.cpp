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

#define LTIMING(x) // DTIMING(x)

Ткст ResolveTParam(const Ткст& тип, const Вектор<Ткст>& tparam)
{
	CodeBaseLock __;
	return ResolveTParam(CodeBase(), тип, tparam);
}

void ResolveTParam(Вектор<Ткст>& тип, const Вектор<Ткст>& tparam)
{
	CodeBaseLock __;
	return ResolveTParam(CodeBase(), тип, tparam);
}

Ткст Qualify(const Ткст& scope, const Ткст& тип, const Ткст& usings)
{
	CodeBaseLock __;
	return Qualify(CodeBase(), scope, тип, usings);
}

void AssistScanError(int line, const Ткст& text)
{
#ifdef _ОТЛАДКА
	PutVerbose(Ткст().конкат() << "(" << line << "): " << text);
#endif
}

void AssistEditor::Контекст(ParserContext& parser, int pos)
{
	LTIMING("Контекст");
	LLOG("---------- Контекст " << path);
	
	theide->ScanFile(true);
	
	parser = AssistParse(дай(0, pos), theide->editfile, AssistScanError,
	                     [&](Ткст scope, Ткст тип, Ткст usings) {
							CodeBaseLock __;
							Ткст t = Qualify(CodeBase(), scope, тип, usings);
							return CodeBase().найди(NoTemplatePars(t)) >= 0 ? t : Null;
	                     });
	inbody = parser.IsInBody();
#ifdef _ОТЛАДКА
	PutVerbose("body: " + какТкст(inbody));
	PutVerbose("scope: " + какТкст(parser.current_scope));
	PutVerbose("using: " + какТкст(parser.context.namespace_using));
	for(int i = 0; i < parser.local.дайСчёт(); i++)
		PutVerbose(parser.local.дайКлюч(i) + ": " + parser.local[i].тип);
#endif
}

Индекс<Ткст> AssistEditor::EvaluateExpressionType(const ParserContext& parser, const Вектор<Ткст>& xp)
{
	CodeBaseLock __;
	return GetExpressionType(CodeBase(), parser, xp);
}

void AssistEditor::AssistItemAdd(const Ткст& scope, const CppItem& m, int typei)
{
	if(!iscib(*m.имя) || m.имя.дайСчёт() == 0)
		return;
	CppItemInfo& f = assist_item.добавь();
	f.typei = typei;
	f.scope = scope;
	(CppItem&)f = m;
}

void AssistEditor::GatherItems(const Ткст& тип, bool only_public, Индекс<Ткст>& in_types, bool types)
{
	LTIMING("GatherItems");
	LLOG("---- GatherItems " << тип);
	CodeBaseLock __;
	if(in_types.найди(тип) >= 0) {
		LLOG("-> recursion, exiting");
		return;
	}
	in_types.добавь(тип);
	Вектор<Ткст> tparam;
	Ткст ntp = ParseTemplatedType(ResolveTParam(тип, tparam), tparam);
	int q = CodeBase().найди(ntp);
	if(q < 0) {
		ntp.замени("*", ""); // * can be part of тип as result of template substitution
		q = CodeBase().найди(ntp);
	}
	if(q >= 0) {
		if(types) {
			if(ntp.дайСчёт())
				ntp << "::";
			int typei = assist_type.найдиДобавь("<types>");
			for(int i = 0; i < CodeBase().дайСчёт(); i++) {
				Ткст nest = CodeBase().дайКлюч(i);
				if(nest.дайДлину() > ntp.дайДлину() &&        // Subscope of scope
				   memcmp(~ntp, ~nest, ntp.дайДлину()) == 0 && // e.g. РНЦП:: -> РНЦП::Ткст
				   nest.найди("::", ntp.дайДлину()) < 0) {      // but not РНЦП::Ткст::Буфер
					Массив<CppItem>& n = CodeBase()[i];
					for(int i = 0; i < n.дайСчёт(); i++) {
						const CppItem& m = n[i];
						if(m.IsType())
							AssistItemAdd(nest, m, typei);
					}
				}
			}
		}
		const Массив<CppItem>& n = CodeBase()[q];
		Ткст base;
		int typei = assist_type.найдиДобавь(ntp);
		bool op = only_public;
		for(int i = 0; i < n.дайСчёт(); i++)
			if(n[i].kind == FRIENDCLASS)
				op = false;
		for(int i = 0; i < n.дайСчёт(); i++) {
			const CppItem& im = n[i];
			if(im.kind == STRUCT || im.kind == STRUCTTEMPLATE)
				base << im.qptype << ';';
			if((im.IsCode() || !thisback && (im.IsData() || im.IsMacro() && пусто_ли(тип)))
			   && (!op || im.access == PUBLIC)) {
				AssistItemAdd(ntp, im, typei);
			}
		}
		if(!thisback) {
			Вектор<Ткст> b = разбей(base, ';');
			Индекс<Ткст> h;
			for(int i = 0; i < b.дайСчёт(); i++)
				h.найдиДобавь(b[i]);
			b = h.подбериКлючи();
			ResolveTParam(b, tparam);
			for(int i = 0; i < b.дайСчёт(); i++)
				if(b[i].дайСчёт())
					GatherItems(b[i], only_public, in_types, types);
		}
	}
	in_types.сбрось();
}

bool OrderAssistItems(const CppItemInfo& a, const CppItemInfo& b)
{
	return CombineCompare(a.uname, b.uname)(a.typei, b.typei)(a.qitem, b.qitem)(a.impl, b.impl)(a.filetype, b.filetype)(a.line, b.line) < 0;
}

void AssistEditor::RemoveDuplicates()
{
	LTIMING("RemoveDuplicates");
	{ LTIMING("сортируй");
	РНЦП::сортируй(assist_item, OrderAssistItems);
	}
	Вектор<int> remove;
	{
	LTIMING("найди duplicates");
	int i = 0;
	while(i < assist_item.дайСчёт()) { // удали identical items
		int ii = i;
		i++;
		while(i < assist_item.дайСчёт()
			  && assist_item[ii].typei == assist_item[i].typei
		      && assist_item[ii].qitem == assist_item[i].qitem
		      && assist_item[ii].scope == assist_item[i].scope)
			remove.добавь(i++);
	}
	}
	LTIMING("Final remove");
	assist_item.удали(remove);
}
