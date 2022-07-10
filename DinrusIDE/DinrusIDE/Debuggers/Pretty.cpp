#include "Debuggers.h"

#ifdef PLATFORM_WIN32

Pdb::Val Pdb::делайЗнач(const Ткст& тип, adr_t address)
{
	Val элт;
	(TypeInfo &)элт = GetTypeInfo(тип);
	элт.address = address;
	return элт;
}

bool Pdb::PrettyVal(Pdb::Val val, int64 from, int count, Pretty& p)
{
	ПРОВЕРЬ(count < 100000);
	
	if(val.тип < 0)
		return false;

	const Type& t = дайТип(val.тип);
	
	
	current_modbase = t.modbase; // so that we do not need to pass it as parameter in Pretty routines

	Ткст тип = t.имя;
	if(тип.обрежьСтарт("РНЦП::WithDeepCopy<")) {
		тип.обрежьКонец(">");
		тип = обрежьПраво(тип);
	}
	int q = тип.найди('<');
	Вектор<Ткст> type_param; // need to be in 'orignal' form, otherwise GetTypeInfo fails
	if(q >= 0) {
		int e = тип.найдирек('>');
		if(e >= 0) {
			int lvl = 0;
			int i = q + 1;
			int b = i;
			while(i < тип.дайСчёт()) {
				if(findarg(тип[i], ',', '>') >= 0 && lvl == 0) {
					if(i > b)
						type_param.добавь(обрежьОба(тип.середина(b, i - b)));
					b = i + 1;
				}
				if(тип[i] == '<')
					lvl++;
				else
				if(тип[i] == '>') {
					if(lvl == 0)
						break;
					lvl--;
				}
				i++;
			}
			
			тип = обрежьОба(тип.середина(0, q));
		}
	}

	if(pretty.дайСчёт() == 0) {
		pretty.добавь("РНЦП::Дата", { 0, THISFN(PrettyDate) });
		pretty.добавь("РНЦП::Время", { 0, THISFN(PrettyTime) });
		pretty.добавь("РНЦП::Цвет", { 0, THISFN(PrettyColor) });
		pretty.добавь("РНЦП::КЗСА", { 0, THISFN(PrettyRGBA) });
		pretty.добавь("РНЦП::ImageBuffer", { 0, THISFN(PrettyImageBuffer) });
		pretty.добавь("РНЦП::Рисунок", { 0, THISFN(PrettyImg) });
		pretty.добавь("РНЦП::Шрифт", { 0, THISFN(PrettyFont) });
		pretty.добавь("РНЦП::МассивЗнач", { 0, THISFN(PrettyValueArray) });
		pretty.добавь("РНЦП::МапЗнач", { 0, THISFN(PrettyValueMap) });
		pretty.добавь("РНЦП::Значение", { 0, THISFN(PrettyValue) });
		pretty.добавь("РНЦП::Ткст", { 0, THISFN(PrettyString) });
		pretty.добавь("РНЦП::ШТкст", { 0, THISFN(PrettyWString) });
		pretty.добавь("РНЦП::Вектор", { 1, THISFN(PrettyVector) });
		pretty.добавь("РНЦП::БиВектор", { 1, THISFN(PrettyBiVector) });
		pretty.добавь("РНЦП::Массив", { 1, THISFN(PrettyArray) });
		pretty.добавь("РНЦП::БиМассив", { 1, THISFN(PrettyBiArray) });
		pretty.добавь("РНЦП::Индекс", { 1, THISFN(PrettyIndex) });
		pretty.добавь("РНЦП::ВекторМап", { 2, THISFN(PrettyVectorMap) });
		pretty.добавь("РНЦП::МассивМап", { 2, THISFN(PrettyArrayMap) });

		pretty.добавь("std::basic_string", { 1, THISFN(PrettyStdString) });
		pretty.добавь("std::vector", { 1, THISFN(PrettyStdVector) });
		pretty.добавь("std::list", { 1, THISFN(PrettyStdList) });
		pretty.добавь("std::forward_list", { 1, THISFN(PrettyStdForwardList) });
		pretty.добавь("std::deque", { 1, THISFN(PrettyStdDeque) });
		pretty.добавь("std::set", { 1, [=](Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p) { PrettyStdTree(val, true, tparam, from, count, p); }});
		pretty.добавь("std::multiset", { 1, [=](Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p) { PrettyStdTree(val, true, tparam, from, count, p); }});
		pretty.добавь("std::map", { 2, [=](Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p) { PrettyStdTree(val, false, tparam, from, count, p); }});
		pretty.добавь("std::multimap", { 2, [=](Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p) { PrettyStdTree(val, false, tparam, from, count, p); }});
		pretty.добавь("std::unordered_set", { 1, [=](Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p) { PrettyStdUnordered(val, true, tparam, from, count, p); }});
		pretty.добавь("std::unordered_multiset", { 1, [=](Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p) { PrettyStdUnordered(val, true, tparam, from, count, p); }});
		pretty.добавь("std::unordered_map", { 2, [=](Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p) { PrettyStdUnordered(val, false, tparam, from, count, p); }});
		pretty.добавь("std::unordered_multimap", { 2, [=](Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p) { PrettyStdUnordered(val, false, tparam, from, count, p); }});
	}
	
	тип = фильтруй(тип, [](int c) { return c != ' ' ? c : 0; });
	тип.замени("::__1", ""); // CLANG has some weird stuff in names...

	int ii = pretty.найди(тип);
	if(ii >= 0) {
		const auto& pr = pretty[ii];
		if(type_param.дайСчёт() >= pr.a) {
			p.kind = CONTAINER;
			p.data_type.приставь(type_param, 0, pr.a);
			pr.b(val, type_param, from, count, p);
			return true;
		}
	}
	return false;
}

bool Pdb::VisualisePretty(Visual& result, Pdb::Val val, dword flags)
{
	auto ResultCount = [&](int64 count) {
		result.конкат("[", SLtBlue);
		result.конкат(count == INT64_MAX ? ">10000" : какТкст(count), SRed);
		result.конкат("] ", SLtBlue);
	};
	
	Pretty p;
	if(PrettyVal(val, 0, 0, p)) {
		if(p.kind == TEXT) {
			int count = (int)min(p.data_count, (int64)200);
			Pretty p;
			PrettyVal(val, 0, count, p);
			if(p.data_type.дайСчёт()) {
				Ткст s;
				ШТкст ws;
				int sz = размТипа(p.data_type[0]);
				for(adr_t a : p.data_ptr) {
					if(sz == 1)
						s.конкат(PeekByte(a));
					else
					if(sz == 2)
						ws.конкат(PeekWord(a));
					else
						ws.конкат(PeekDword(a));
				}
				if(!(flags & MEMBER))
					ResultCount(p.data_count);
				result.конкат(FormatString(sz == 1 ? s : ws.вТкст()), SRed);
				if(p.data_count > p.data_ptr.дайСчёт())
					result.конкат("..", SGray);
			}
		}
		else
		if(p.kind == SINGLE_VALUE) {
			Pretty p;
			PrettyVal(val, 0, 1, p);
			for(const VisualPart& vp : p.text.part)
				result.конкат(vp.text, vp.ink);
			if(p.has_data) {
				if(p.data_type.дайСчёт() && p.data_ptr.дайСчёт())
					Visualise(result, делайЗнач(p.data_type[0], p.data_ptr[0]), flags);
				else
					Visualise(result, val, flags | RAW);
			}
		}
		else { // CONTAINER
			int count = (int)min(p.data_count, (int64)40);
			Pretty p;
			PrettyVal(val, 0, count, p);

			ResultCount(p.data_count);
			if(p.data_type.дайСчёт()) {
				Буфер<Val> элт(p.data_type.дайСчёт());
				for(int i = 0; i < p.data_type.дайСчёт(); i++) {
					(TypeInfo &)элт[i] = GetTypeInfo(p.data_type[i]);
					элт[i].context = val.context;
				}
				int ii = 0;
				int n = p.data_ptr.дайСчёт() / p.data_type.дайСчёт();
				Цвет bc = decode(bc_lvl++ & 3, 0, SGray(), 1, SCyan(), 2, SBrown(), SGreen());
				result.конкат("{", bc);
				for(int i = 0; i < n; i++) {
					if(i)
						result.конкат(", ", SGray);
					for(int j = 0; j < p.data_type.дайСчёт(); j++) {
						if(j)
							result.конкат(": ", SBlue);
						элт[j].address = p.data_ptr[ii++];
						if(элт[j].тип != UNKNOWN)
							Visualise(result, элт[j], flags | MEMBER);
					}
				}
				result.конкат("}", bc);
				bc_lvl--;
			}
	
			if(p.data_count > count)
				result.конкат("..", SGray);
		}
		return true;
	}
	return false;
}

#endif
