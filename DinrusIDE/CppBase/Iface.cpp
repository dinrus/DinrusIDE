#include "CppBase.h"
#include "Internal.h"

namespace РНЦП {

bool HasCPPFileKeyword(const Ткст& path, const Ткст& id)
{
	const PPFile& f = GetPPFile(NormalizeSourcePath(path));
	return найдиИндекс(f.keywords, id) >= 0;
}

Ткст GetDependeciesMD5(const Ткст& path, Индекс<Ткст>& visited)
{
	Cpp pp;
	ФайлВвод in(path);
	Ткст npath = NormalizeSourcePath(path);
	pp.Preprocess(npath, in, GetMasterFile(npath), true);
	Ткст md5 = pp.GetDependeciesMd5(GetPPFile(path).keywords);
	visited = pick(pp.visited);
	return md5;
}

Ткст GetPPMD5(const Ткст& фн)
{
	return GetPPFile(фн).md5sum;
}

void Parse(CppBase& base,
           const Ткст& src, int file, int filetype, const Ткст& path,
           Событие<int, const Ткст&> Ошибка, const Вектор<Ткст>& namespace_stack,
	       const Индекс<Ткст>& namespace_using)
{
	ТкстПоток pin(src);
	Parser p;
	p.Do(pin, base, file, filetype, дайИмяф(path), Ошибка, Вектор<Ткст>(),
	     namespace_stack, namespace_using);
}

void PreprocessParse(CppBase& base, Поток& in, int file, const Ткст& path,
                     Событие<int, const Ткст&> Ошибка)
{
	Cpp cpp;
	cpp.Preprocess(path, in, GetMasterFile(path));
	int filetype = decode(впроп(дайРасшф(path)), ".h", FILE_H, ".hpp", FILE_HPP,
	                       ".cpp", FILE_CPP, ".icpp", FILE_CPP, ".c", FILE_C, FILE_OTHER);
	Parse(base, cpp.output, file, filetype, path, Ошибка, cpp.namespace_stack, cpp.namespace_using);
}

Ткст PreprocessCpp(const Ткст& src, const Ткст& path)
{
	Cpp cpp;
	ТкстПоток in(src);
	Ткст p = NormalizeSourcePath(path);
	cpp.Preprocess(p, in, GetMasterFile(p));
	return cpp.output;
}

ParserContext AssistParse(const Ткст& src, const Ткст& path_, Событие<int, const Ткст&> Ошибка,
                          Функция<Ткст(Ткст, Ткст, Ткст)> qualify)
{
	Ткст path = NormalizeSourcePath(path_);
	Cpp cpp;
	ТкстПоток ss(src);
	cpp.Preprocess(path, ss, GetMasterFile(path));
	Parser parser;
	parser.dobody = true; // will do bodies and not write anything to base
	parser.qualify = qualify;
	ТкстПоток pin(cpp.output);
	CppBase dummy;
	parser.Do(pin, dummy, Null, Null, дайТитулф(path), Ошибка,
	          Вектор<Ткст>(), cpp.namespace_stack, cpp.namespace_using); // needs CodeBase to identify тип names
	return pick(parser);
}

void SimpleParse(CppBase& cpp, const Ткст& txt, const Ткст& cls)
{
	Parser parser; // we do not need/want preprocessing here
	ТкстПоток ss(txt);
	parser.Do(ss, cpp, Null, Null, Null, CNULL, разбей(cls, ':'),
	          Вектор<Ткст>(), Индекс<Ткст>());
}

};