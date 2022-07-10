#include "CppBase.h"
#include "Internal.h"

namespace РНЦП {

Ткст CppItem::вТкст() const
{
	ТкстПоток ss;
	Dump(ss);
	return ss;
}

Ткст CppItemKindAsString(int kind)
{
	return decode(kind,
				STRUCT, "STRUCT",
				STRUCTTEMPLATE, "STRUCTTEMPLATE",
				TYPEDEF, "TYPEDEF",
				CONSTRUCTOR, "CONSTRUCTOR",
				DESTRUCTOR, "DESTRUCTOR",
				FUNCTION, "FUNCTION",
				INSTANCEFUNCTION, "INSTANCEFUNCTION",
				CLASSFUNCTION, "CLASSFUNCTION",
				FUNCTIONTEMPLATE, "FUNCTIONTEMPLATE",
				INSTANCEFUNCTIONTEMPLATE, "INSTANCEFUNCTIONTEMPLATE",
				CLASSFUNCTIONTEMPLATE, "CLASSFUNCTIONTEMPLATE",
				INLINEFRIEND, "INLINEFRIEND",
				VARIABLE, "VARIABLE",
				INSTANCEVARIABLE, "INSTANCEVARIABLE",
				CLASSVARIABLE, "CLASSVARIABLE",
				ENUM, "ENUM",
				MACRO, "MACRO",
				FRIENDCLASS, "FRIENDCLASS",
				NAMESPACE, "NAMESPACE",
				"?kind:" + какТкст(kind));
}

void CppItem::Dump(Поток& s) const
{
	s << Nvl(qitem, "?") << ' ';
	s << CppItemKindAsString(kind) << ' '
	  << decode(access,
				PUBLIC, "PUBLIC",
				PROTECTED, "PROTECTED",
				PRIVATE, "PRIVATE",
				"?access:" + какТкст(access)) << ' '
	  << decode(filetype,
				FILE_H, "FILE_H",
				FILE_HPP, "FILE_HPP",
				FILE_CPP, "FILE_CPP",
				FILE_C, "FILE_C",
				FILE_OTHER, "FILE_OTHER",
				"?filetype:" + какТкст(filetype))
	;
#define PUT(x) if(x) s << ' ' << #x;
	PUT(virt);
	PUT(decla);
	PUT(lvalue);
	PUT(isptr);
	PUT(impl);
	PUT(qualify);
#undef PUT
	s << ' ' << line << '\n';
#define PUT(x) if(x.дайСчёт()) s << "      " << #x << ": " << x << '\n';
	PUT(элт);
	PUT(имя);
	PUT(uname);
	PUT(natural);
	PUT(тип);
	PUT(qtype);
	PUT(tparam);
	PUT(param);
	PUT(pname);
	PUT(ptype);
	PUT(qptype);
	PUT(tname);
	PUT(ctname);
	PUT(using_namespaces);
#undef PUT
}

}
