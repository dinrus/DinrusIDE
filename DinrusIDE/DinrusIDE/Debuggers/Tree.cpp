#include "Debuggers.h"

#ifdef PLATFORM_WIN32

#define LLOG(x) // LOG(x)

void Pdb::PrettyTreeNode(int parent, Pdb::Val val, int64 from)
{
	try {
		Pretty pp;
		if(val.ref || !PrettyVal(val, 0, 0, pp))
			return;
		if(pp.kind == SINGLE_VALUE) {
			Pretty p;
			PrettyVal(val, 0, 1, p);
			if(p.text.part.дайСчёт()) {
				NamedVal nv;
				Visual result;
				nv.имя = "$$";
				for(const VisualPart& vp : p.text.part)
					result.конкат(vp.text, vp.ink);
				tree.добавь(parent, Null, RawToValue(nv), RawPickToValue(pick(result)), false);
			}
			else
			if(p.data_type.дайСчёт() && p.data_ptr.дайСчёт())
				PrettyTreeNode(parent, делайЗнач(p.data_type[0], p.data_ptr[0]), from);
		}
		if(pp.kind == CONTAINER) {
			int n = int(min(pp.data_count, from + 10000) - from);
			Pretty p;
			PrettyVal(val, from, n, p);

			if(p.data_type.дайСчёт()) {
				Буфер<Val> элт(p.data_type.дайСчёт());
				for(int i = 0; i < p.data_type.дайСчёт(); i++) {
					(TypeInfo &)элт[i] = GetTypeInfo(p.data_type[i]);
					элт[i].context = val.context;
				}
				int ii = 0;
				int n = p.data_ptr.дайСчёт() / p.data_type.дайСчёт();
				for(int i = 0; i < n; i++) {
					NamedVal nv;
					Visual result;
					nv.имя << '[' << i + from << ']';
					nv.val = val;
					nv.exp = true;
					result.конкат(nv.имя + ' ', SGray);
					try {
						for(int j = 0; j < p.data_type.дайСчёт(); j++) {
							if(j)
								result.конкат(": ");
							элт[j].address = p.data_ptr[ii++];
							nv.val = элт[j];
							if(p.data_type.дайСчёт() > 1 && j == 0)
								nv.ключ = элт[0];
							Visualise(result, элт[j], MEMBER);
						}
					}
					catch(LengthLimit) {}
					catch(СиПарсер::Ошибка e) {
						result.конкат("??");
					}
					tree.добавь(parent, Null, RawToValue(nv), RawPickToValue(pick(result)),
					         nv.ключ.тип != UNKNOWN || nv.val.тип > 0);
				}
				if(pp.data_count > n && from == 0 && pp.data_count != INT64_MAX) {
					NamedVal nv;
					nv.имя << "..";
					nv.val = val;
					int64 ii = n;
					while(ii < pp.data_count) {
						nv.from = ii;
						Visual v;
						v.конкат(Ткст() << "[" << ii << ".." << min(pp.data_count - 1, ii + 9999) << "]", SGray);
						tree.добавь(parent, Null, RawToValue(nv), RawPickToValue(pick(v)), true);
						ii += 10000;
					}
				}
			}
		}
	}
	catch(LengthLimit) {}
	catch(СиПарсер::Ошибка e) {}
}

bool Pdb::TreeNode(int parent, const Ткст& имя, Pdb::Val val, int64 from, Цвет ink, bool exp)
{
	PrettyTreeNode(parent, val);
	NamedVal nv;
	nv.имя = имя;
	nv.val = val;
	nv.from = from;
	nv.exp = exp;
	Visual v;
	bool r = true;
	try {
		v.конкат(имя, ink);
		if(!from) {
			v.конкат("=", SGray);
			Visualise(v, val, 0);
		}
	}
	catch(LengthLimit) {}
	catch(СиПарсер::Ошибка e) {
		v.конкат(e, SColorDisabled);
		r = false;
	}
	tree.добавь(parent, Null, RawToValue(nv), RawPickToValue(pick(v)), val.тип >= 0 || val.ref > 0);
	return r;
}

bool Pdb::TreeNodeExp(int parent, const Ткст& имя, Val val, int64 from, Цвет ink)
{
	return TreeNode(parent, имя, val, from, ink, true);
}

void Pdb::TreeExpand(int node)
{
	if(tree.GetChildCount(node))
		return;
	try {
		Значение v = tree.дай(node);
		if(!v.является<NamedVal>())
			return;
		const NamedVal& nv = ValueTo<NamedVal>(tree.дай(node));
		Val val = nv.val;
		if(nv.ключ.тип != UNKNOWN) {
			TreeNode(node, "ключ", nv.ключ);
			TreeNode(node, "значение", val);
			SaveTree();
			return;
		}
		if(nv.val.ref > 0) {
			Val val0 = val;
			val = DeRef(val);
			if(val.тип < 0 || val.ref > 0) {
				int sz = размТипа(val.тип);
				val.address += sz * nv.from;
				for(int i = 0; i < (nv.from ? 10000 : 40); i++) {
					if(!TreeNodeExp(node, Ткст() << "[" << i + nv.from << "]" , val, 0, SLtMagenta())) {
						SaveTree();
						return;
					}
					val.address += sz;
				}
				TreeNode(node, "[more]", val0, nv.from ? nv.from + 10000 : 40);
				SaveTree();
				return;
			}
		}
		else
		if(nv.from > 0) {
			PrettyTreeNode(node, val, nv.from);
			SaveTree();
			return;
		}
		if(val.тип < 0) {
			SaveTree();
			return;
		}
		PrettyTreeNode(node, val, 0);
		const Type& t = дайТип(val.тип);
		if(t.vtbl_typeindex == -2) {
			int count = GetSymInfo(t.modbase, тип.дайКлюч(val.тип), TI_GET_COUNT);
			Val prtti;
			prtti.ref = 1;
			prtti.тип = UINT4;
			prtti.address = val.address - 4;
			Val rtti = GetRVal(prtti);
			FnInfo rtf = GetFnInfo(rtti.address);
			TreeNode(node, rtf.имя, prtti);
			for(int i = 0; i < count; i++) {
				Val ventry;
				ventry.тип = PFUNC;
				ventry.address = val.address + 4 * i;
				TreeNode(node, фмт("[%d]", i), ventry);
			}
			SaveTree();
			return;
		}
		if(t.vtbl_typeindex >= 0) {
			Val vtbl;
			vtbl.ref = 1;
			vtbl.address = val.address + t.vtbl_offset;
			vtbl.тип = t.vtbl_typeindex;
			TreeNode(node, "virtual", vtbl);
		}
		for(int i = 0; i < t.base.дайСчёт(); i++) {
			Val r = t.base[i];
			r.address += val.address;
			if(r.тип >= 0) {
				const Type& bt = дайТип(r.тип);
				TreeNode(node, bt.имя, r);
			}
		}
		for(int i = 0; i < t.member.дайСчёт(); i++) {
			Val r = t.member[i];
			r.address += val.address;
			TreeNodeExp(node, t.member.дайКлюч(i), r);
		}
		for(int i = 0; i < t.static_member.дайСчёт(); i++) {
			Val r = t.static_member[i];
			TreeNodeExp(node, t.static_member.дайКлюч(i), r);
		}
		SaveTree();
	}
	catch(LengthLimit) {}
	catch(СиПарсер::Ошибка e) {}
}

void Pdb::StoreTree(ТкстБуф& result, int parent)
{
	int n = tree.GetChildCount(parent);
	for(int i = 0; i < n; i++) {
		int child = tree.GetChild(parent, i);
		bool first = true;
		if(tree.открыт(child)) {
			const NamedVal& nv = ValueTo<NamedVal>(tree.дай(child));
			if(!first)
				result << ";";
			first = false;
			result << " " << какТкстСи(nv.имя) << " {";
			StoreTree(result, child);
			result << "}";
		}
	}
}

void Pdb::SaveTree()
{
	if(restoring_tree)
		return;
	Значение v = tree.дай(0);
	if(!IsType<NamedVal>(v))
		return;
	const NamedVal& nv = ValueTo<NamedVal>(v);
	if(nv.val.тип < 0)
		return;
	ТкстБуф w;
	Точка p = tree.дайПромотку();
	w << p.x << ' ' << p.y << ' ';
	int id = tree.дайКурсор();
	Вектор<Ткст> cursor;
	while(id >= 0) {
		const NamedVal& nv = ValueTo<NamedVal>(tree.дай(id));
		cursor.добавь(какТкстСи(nv.имя));
		id = tree.дайРодителя(id);
	}
	реверс(cursor);
	w << Join(cursor, ";") << ": ";
	StoreTree(w, 0);
	Ткст r = w;
	treetype.дайДобавь(дайТип(nv.val.тип).имя) = r;
	LOG("SaveTree " << дайТип(nv.val.тип).имя << ' ' << r);
}

int Pdb::FindChild(int parent, Ткст id)
{
	int n = tree.GetChildCount(parent);
	for(int i = 0; i < n; i++) {
		int child = tree.GetChild(parent, i);
		const NamedVal& nv = ValueTo<NamedVal>(tree.дай(child));
		if(nv.имя == id)
			return child;
	}
	return -1;
}

void Pdb::ExpandTreeType(int parent, СиПарсер& p)
{
	while(p.ткст_ли()) {
		int child = FindChild(parent, p.читайТкст());
		if(child >= 0) {
			tree.открой(child);
			p.сим('{');
			ExpandTreeType(child, p);
			p.сим('}');
		}
	}
}

void Pdb::SetTree(const Ткст& exp)
{
	tree_exp = exp;
	SaveTree();
	tree.очисть();
	NamedVal nv;
	try {
		СиПарсер p(exp);
		nv.val = Exp(p);
	}
	catch(СиПарсер::Ошибка) {
		return;
	}
	if(nv.val.reference) {
		nv.val = DeRef(nv.val);
		nv.val.reference = false;
	}
	nv.имя = exp;
	Ткст n = exp;
	if(nv.val.тип >= 0)
		n = дайТип(nv.val.тип).имя;

	Visual v;
	try {
		v.конкат(n, SBlack);
		v.конкат("=", SGray);
		Visualise(v, nv.val, 0);
	}
	catch(LengthLimit) {}
	catch(СиПарсер::Ошибка e) {
		v.конкат(e, SColorDisabled);
	}
	tree.устДисплей(visual_display);

	tree.SetRoot(Null, RawToValue(nv), RawPickToValue(pick(v)));
	if(nv.val.тип >= 0) {
		Ткст w = treetype.дай(n, Null);
		LOG("SetTree " << n << ' ' << w);
		tree.открой(0);
		СиПарсер p(w);
		try {
			Точка sc;
			sc.x = p.читайЦел();
			sc.y = p.читайЦел();
			Вектор<Ткст> cursor;
			while(!p.сим(':')) {
				cursor.добавь(p.читайТкст());
				p.сим(';');
			}
			restoring_tree++;
			ExpandTreeType(0, p);
			restoring_tree--;
			tree.ScrollTo(sc);
			if(cursor.дайСчёт()) {
				int ii = 0;
				for(int i = 1; i < cursor.дайСчёт(); i++) {
					ii = FindChild(ii, cursor[i]);
					if(ii < 0)
						break;
				}
				if(ii >= 0)
					tree.устКурсор(ii);
			}
		}
		catch(СиПарсер::Ошибка) {}
	}
	SyncTreeDisas();
}

void Pdb::SetTreeA(КтрлМассив *array)
{
	SetTree(array->дай(0));
}

Ткст Pdb::GetTreeText(int id)
{
	return tree.дайЗначение(id).To<Visual>().дайТкст();
}

void Pdb::GetTreeText(Ткст& r, int id, int depth) {
	int n = tree.GetChildCount(id);
	r << Ткст('\t', depth) << GetTreeText(id) << "\n";
	for(int i = 0; i < n; i++)
		GetTreeText(r, tree.GetChild(id, i), depth + 1);
}

void Pdb::TreeMenu(Бар& bar)
{
	bar.добавь(tree.курсор_ли(), "Watch", [=] { TreeWatch(); });
	bar.добавь(tree.курсор_ли(), "копируй", [=] {
		ClearClipboard();
		AppendClipboardText(GetTreeText(tree.дайКурсор()));
	});
	bar.добавь(tree.дайСчётСтрок(), "копируй all", [=] {
		ClearClipboard();
		Ткст r;
		GetTreeText(r, 0, 0);
		AppendClipboardText(r);
	});
}

void Pdb::TreeWatch()
{
	int id = tree.дайКурсор();
	Ткст exp;
	while(id >= 0) {
		Значение v = tree.дай(id);
		if(!v.является<NamedVal>())
			break;
		const NamedVal& nv = v.To<NamedVal>();
		if(nv.exp) {
			if(IsAlpha(*exp))
				exp = '.' + exp;
			exp = nv.имя + exp;
		}
		id = tree.дайРодителя(id);
	}
	AddWatch(tree_exp + "." + exp);
}

#endif
