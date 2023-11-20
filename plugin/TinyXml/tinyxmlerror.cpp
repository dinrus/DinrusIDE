#include "tinyxml.h"

// The goal of the separate error file is to make the first
// step towards localization. tinyxml (currently) only supports
// latin-1, but at least the error messages could now be translated.
//
// It also cleans up the code a bit.
//
namespace UPP{
const char* TiXmlBase::errorString[ TIXML_ERROR_STRING_COUNT ] =
{
	String("Ошибок нет"),
	String("Ошибка"),
	String("Не удалось открыть файл"),
	String("Неудачное размещение в память."),
	String("Ошибка при разборе элемента."),
	String("Не удалось прочсть имя элемента"),
	String("Ошибка при чтении значения элемента."),
	String("Ошибка при чтении атрибутов."),
	String("Ошибка: пустой тэг."),
	String("Ошибка чтения заканчивающего тэга."),
	String("Ошибка разбора Unknown."),
	String("Ошибка разбора Comment."),
	String("Ошибка разбора Declaration."),
	String("Ошибка - документ пуст.")
};
}