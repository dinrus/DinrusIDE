#include "MacroManager.h"

namespace РНЦП {

MacroElement::MacroElement(Type тип, const Ткст& fileName, int line, const Ткст& comment)
	: тип(тип)
	, comment(comment)
	, fileName(fileName)
	, line(line)
{}

Рисунок MacroElement::GetImage() const
{
	switch(тип)
	{
		case(Type::MACRO):
			return MacroManagerImg::Macro();
		case(Type::FUNCTION):
			return MacroManagerImg::Fn();
		case(Type::UNKNOWN): // silence CLANG warning
			break;
	}
	return Рисунок();
}

Ткст MacroElement::GetContent() const
{
	Ткст content = обрежьОба(comment);
	if(content.дайСчёт())
		content << "\n";

	content << prototype << code;
	
	return content;
}

}
