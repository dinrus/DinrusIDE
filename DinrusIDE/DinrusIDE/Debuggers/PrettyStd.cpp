#include "Debuggers.h"

#ifdef PLATFORM_WIN32

void Pdb::PrettyStdString(Pdb::Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	adr_t a;
	int size;
	bool w = tparam[0] == "wchar_t";
	if(HasAttr(val, "__r_")) { // CLANG variant
		Val r = дайАтр(дайАтр(val, "__r_"), "__value_");
		Val s = дайАтр(r, "__s");
		size = GetByteAttr(s, "__size_");
		if(size & 1) {
			Val l = дайАтр(r, "__l");
			size = GetIntAttr(l, "__size_");
			a = DeRef(дайАтр(l, "__data_")).address;
		}
		else {
			size = size >> 1;
			a = s.address + 1 + w;
		}
	}
	else {
		Val q = дайАтр(дайАтр(val, "_Mypair"), "_Myval2");
		size = GetIntAttr(q, "_Mysize");
		int res = GetIntAttr(q, "_Myres");
		a = дайАтр(дайАтр(q, "_Bx"), "_Buf").address;
		if(res >= (w ? 8 : 16))
			a = подбериУк(a);
	}
	p.data_count = size;
	p.data_type << (w ? "short int" : "char");
	int sz = w + 1;
	for(int i = 0; i < count; i++)
		p.data_ptr.добавь(a + sz * (from + i));
	p.kind = TEXT;
}

void Pdb::PrettyStdVector(Pdb::Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	Val begin, end;
	if(HasAttr(val, "__begin_")) { // CLANG's std::vector
		begin = DeRef(дайАтр(val, "__begin_"));
		end = DeRef(дайАтр(val, "__end_"));
	}
	else {
		Val q = дайАтр(дайАтр(val, "_Mypair"), "_Myval2");
		begin = DeRef(дайАтр(q, "_Myfirst"));
		end = DeRef(дайАтр(q, "_Mylast"));
	}
	int sz = размТипа(tparam[0]);
	p.data_count = (end.address - begin.address) / sz;
	for(int i = 0; i < count; i++)
		p.data_ptr.добавь(begin.address + (i + from) * sz);
}

void Pdb::TraverseTree(bool set, Pdb::Val head, Val node, int64& from, int& count, Pdb::Pretty& p, int depth)
{
	if(depth > 40) // avoid problems if tree is damaged
		return;
	if(depth && node.address == head.address || count <= 0) // we are at the end or have read enough items
		return;
	TraverseTree(set, head, DeRef(дайАтр(node, "_Left")), from, count, p, depth + 1);
	if(node.address != head.address) {
		if(from == 0) {
			Val v = дайАтр(node, "_Myval");
			if(set)
				p.data_ptr.добавь(v.address);
			else {
				p.data_ptr.добавь(дайАтр(v, "first").address);
				p.data_ptr.добавь(дайАтр(v, "second").address);
			}
			count--;
		}
		else
			from--;
	}
	TraverseTree(set, head, DeRef(дайАтр(node, "_Right")), from, count, p, depth + 1);
}

void Pdb::TraverseTreeClang(bool set, int nodet, Val node, int64& from, int& count, Pdb::Pretty& p, int depth)
{
	if(depth > 40 || count <= 0) // avoid problems if tree is damaged
		return;

	Val left = DeRef(дайАтр(node, "__left_"));
	if(left.address)
		TraverseTreeClang(set, nodet, left, from, count, p, depth + 1);

	node.тип = nodet;
	Val data = дайАтр(node, "__value_");
	if(from == 0) {
		if(set)
			p.data_ptr.добавь(data.address);
		else {
			Val cc = дайАтр(data, "__cc");
			p.data_ptr.добавь(дайАтр(cc, "first").address);
			p.data_ptr.добавь(дайАтр(cc, "second").address);
		}
		count--;
	}
	else
		from--;

	Val right = DeRef(дайАтр(node, "__right_"));
	if(right.address)
		TraverseTreeClang(set, nodet, right, from, count, p, depth + 1);
}

void Pdb::PrettyStdTree(Pdb::Val val, bool set, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	if(HasAttr(val, "__tree_")) {
		Ткст nodet;
		if(set)
			nodet << "std::__1::__tree_node<" << tparam[0] << ",void *>";
		else {
			nodet = "std::__1::__tree_node<std::__1::__value_type<" << tparam[0] << "," << tparam[1];
			if(*nodet.последний() == '>')
				nodet << ' ';
			nodet << ">,void *>";
		}
		Val tree = дайАтр(val, "__tree_");
		Val значение = дайАтр(дайАтр(tree, "__pair1_"), "__value_");
		p.data_count = GetIntAttr(дайАтр(tree, "__pair3_"), "__value_");
		Val node = DeRef(дайАтр(значение, "__left_"));
		TraverseTreeClang(set, GetTypeInfo(nodet).тип, node, from, count, p, 0);
	}
	else {
		val = дайАтр(дайАтр(дайАтр(val, "_Mypair"), "_Myval2"), "_Myval2");
		p.data_count = GetIntAttr(val, "_Mysize");
		Val head = DeRef(дайАтр(val, "_Myhead")); // points to leftmost element (!)
		Val top = DeRef(дайАтр(head, "_Left"));
		for(int i = 0; i < 40; i++) { // find topmost node, i is depth limit
			Val v = DeRef(дайАтр(top, "_Parent"));
			if(v.address == head.address)
				break;
			top = v;
		}
		TraverseTree(set, head, top, from, count, p, 0);
	}
}

void Pdb::PrettyStdListM(Pdb::Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p, bool map)
{
	if(HasAttr(val, "__end_")) {
		p.data_count = GetIntAttr(дайАтр(val, "__size_alloc_"), "__value_");
		int node_type = GetTypeInfo("std::__1::__list_node<" + tparam[0] + ",void *>").тип;
		Val next = DeRef(дайАтр(дайАтр(val, "__end_"), "__next_"));
		while(count > 0) {
			if(from == 0) {
				Val h = next;
				h.тип = node_type;
				p.data_ptr.добавь(дайАтр(h, "__value_").address);
				count--;
			}
			else
				from--;
			next = DeRef(дайАтр(next, "__next_"));
		}
	}
	else {
		val = дайАтр(дайАтр(val, "_Mypair"), "_Myval2");
		p.data_count = GetIntAttr(val, "_Mysize");
		Val next = DeRef(дайАтр(val, "_Myhead"));
		while(count > 0) {
			next = DeRef(дайАтр(next, "_Next"));
			Val v = дайАтр(next, "_Myval");
			if(from == 0) {
				if(map) { // support for MS std::unordered_map
					p.data_ptr.добавь(дайАтр(v, "first").address);
					p.data_ptr.добавь(дайАтр(v, "second").address);
				}
				else
					p.data_ptr.добавь(дайАтр(next, "_Myval").address);
				count--;
			}
			else
				from--;
		}
	}
}

void Pdb::PrettyStdList(Pdb::Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	PrettyStdListM(val, tparam, from, count, p);
}

void Pdb::PrettyStdForwardList(Pdb::Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	Ткст значение, next;
	p.data_count = 0;
	if(HasAttr(val, "__before_begin_")) {
		val = DeRef(дайАтр(дайАтр(дайАтр(val, "__before_begin_"), "__value_"), "__next_"));
		значение = "__value_";
		next = "__next_";
	}
	else {
		val = DeRef(дайАтр(дайАтр(дайАтр(val, "_Mypair"), "_Myval2"), "_Myhead"));
		значение = "_Myval";
		next = "_Next";
	}
	while(val.address) {
		if(from == 0) {
			if(count) {
				p.data_ptr.добавь(дайАтр(val, значение).address);
				count--;
			}
		}
		else
			from--;
		val = DeRef(дайАтр(val, next));
		p.data_count++;
		if(count == 0 && p.data_count > 10000) {
			p.data_count = INT64_MAX;
			break;
		}
	}
}

void Pdb::PrettyStdDeque(Pdb::Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	int sz = размТипа(tparam[0]);
	int block_size, start;
	adr_t map;
	if(HasAttr(val, "__size_")) {
		p.data_count = GetIntAttr(дайАтр(val, "__size_"), "__value_");
		block_size = sz < 256 ? 4096 / sz : 16;
		start = GetIntAttr(val, "__start_");
		map = DeRef(дайАтр(дайАтр(val, "__map_"), "__begin_")).address;
	}
	else {
		val = дайАтр(дайАтр(val, "_Mypair"), "_Myval2");
		p.data_count = GetIntAttr(val, "_Mysize");
		block_size = sz <= 1 ? 16 : sz <= 2 ? 8 : sz <= 4 ? 4 : sz <= 8 ? 2 : 1;
		start = GetIntAttr(val, "_Myoff");
		map = DeRef(дайАтр(val, "_Map")).address;
	}
	
	for(int i = 0; i < count; i++) {
		int64 q = i + from + start;
		p.data_ptr.добавь(подбериУк(map + (q / block_size) * (win64 ? 8 : 4)) + q % block_size * sz);
	}
}

void Pdb::PrettyStdUnordered(Pdb::Val val, bool set, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	if(HasAttr(val, "__table_")) {
		val = дайАтр(val, "__table_");
		p.data_count = GetIntAttr(дайАтр(val, "__p2_"), "__value_");
		Ткст nodet = set ? "std::__1::__hash_node<" << tparam[0] << ",void *>"
		                   : "std::__1::__hash_node<std::__1::__hash_value_type<"
		                      << tparam[0] << "," << tparam[1] << " >,void *>";
		int ntype = GetTypeInfo(nodet).тип;
		adr_t next = DeRef(дайАтр(дайАтр(дайАтр(val, "__p1_"), "__value_"), "__next_")).address;
		while(next && count > 0) {
			Val v = val;
			v.тип = ntype;
			v.address = next;
			if(from == 0) {
				Val vl = дайАтр(v, "__value_");
				if(set)
					p.data_ptr.добавь(vl.address);
				else {
					vl = дайАтр(vl, "__cc");
					p.data_ptr.добавь(дайАтр(vl, "first").address);
					p.data_ptr.добавь(дайАтр(vl, "second").address);
				}
			}
			else
				from--;
			next = DeRef(дайАтр(v, "__next_")).address;
		}
	}
	else
		PrettyStdListM(дайАтр(val, "_List"), tparam, from, count, p, !set);
}

#endif
