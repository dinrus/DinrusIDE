#include "MacroManager.h"

#define METHOD_NAME "UscFileParser::" << UPP_FUNCTION_NAME << "(): "

namespace РНЦП {

UscFileParser::UscFileParser(const Ткст& filePath)
	: filePath(filePath)
{}

UscFileParser::MacroList UscFileParser::Parse()
{
	MacroList ret;
	
	Ткст fileContent = загрузиФайл(filePath);
	if(fileContent.пустой()) {
		Logw() << METHOD_NAME << "Следующий файл \"" << filePath << "\" не существует или пустой.";
		return ret;
	}

	try {
		СиПарсер parser(fileContent, filePath);
		
		while (!parser.кф_ли()) {
			Ткст comment = обрежьЛево(Ткст(parser.дайПбелУк(), parser.дайУк()));

			if (!parser.ид_ли())
				return ret;
			
			const char* prototypeBegin = parser.дайУк();
			Ткст id = parser.читайИд();
			
			if(id.равен("фн"))
				ReadFunction(parser, comment, prototypeBegin, ret);
			else
			if(id.равен("macro"))
				ReadMacro(parser, comment, prototypeBegin, ret);
			else
				return ret;
		}
	}
	catch (const СиПарсер::Ошибка& Ошибка) {
		Logw() << METHOD_NAME << "Разбор файла \"" << filePath << "\" завершился ошибкой: " << Ошибка << ".";
	}
	
	return ret;
}

bool UscFileParser::пригоден()
{
	return !Parse().пустой();
}

void UscFileParser::ReadFunction(СиПарсер& parser, const Ткст& comment, const char* prototypeBegin, MacroList& list)
{
	Ткст fileName = parser.дайИмяф();
	MacroElement фн(MacroElement::Type::FUNCTION, fileName, parser.дайСтроку(), comment);
	
	if(!parser.ид_ли()) {
		FindNextElement(parser);
		return;
	}
	фн.имя = parser.читайИд();
	if(!parser.сим('(')) {
		FindNextElement(parser);
		return;
	}
	фн.args = ReadArgs(parser);
	
	FinishRead(parser, prototypeBegin, фн, list);
}

void UscFileParser::ReadMacro(СиПарсер& parser, const Ткст& comment, const char* prototypeBegin, MacroList& list)
{
	Ткст fileName = parser.дайИмяф();
	MacroElement macro(MacroElement::Type::MACRO, fileName, parser.дайСтроку(), comment);
	
	if(!parser.ткст_ли()) {
		FindNextElement(parser);
		return;
	}
	macro.имя = Ткст() << (parser.ткст_ли() ? parser.читайТкст() : "");
	if(parser.сим(':')) {
		if(!parser.ткст_ли()) {
			FindNextElement(parser);
			return;
		}

		macro.имя << " : " << (parser.ткст_ли() ? parser.читайТкст() : "");
	}
	if (!parser.сим_ли('{'))
		ReadKeyDesc(parser);

	FinishRead(parser, prototypeBegin, macro, list);
}

void UscFileParser::FinishRead(СиПарсер& parser, const char* prototypeBegin, MacroElement& element, MacroList& list)
{
	const char* bodyBegin = parser.дайУк();

	element.prototype = Ткст(prototypeBegin, bodyBegin);
	
	if (!parser.сим('{')) {
		FindNextElement(parser);
		return;
	}

	РНЦП::SkipBlock(parser);

	if(parser.дайПбелУк() > bodyBegin)
		element.code = Ткст(bodyBegin, parser.дайПбелУк());

	list.добавь(element);
}

void UscFileParser::FindNextElement(СиПарсер& parser)
{
	while(!parser.кф_ли() && !parser.ид_ли("фн") && !parser.ид_ли("macro"))
		parser.пропустиТерм();
}

Ткст UscFileParser::ReadArgs(СиПарсер& parser)
{
	int level = 1;
	parser.сим('(');
	
	Ткст ret = "(";
	while(level > 0 && !parser.кф_ли()) {
		if (parser.сим('('))
			level++;
		else
		if (parser.сим(')'))
			level--;
		else
			ret << parser.дайСим();
	}
	ret << ")";

	return ret;
}

Ткст UscFileParser::ReadKeyDesc(СиПарсер& parser)
{
	if(!parser.ид_ли())
		return Ткст();

	Ткст ret = parser.читайИд();

	while(!parser.кф_ли() && parser.сим('+')) {
		if(parser.ид_ли())
			ret << "+" << parser.читайИд();

		else
		if(parser.число_ли())
			ret << "+" << parser.читайЦел();
		else
			break;
	}

	return ret;
}

}
