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
	TEXT("Ошибок нет"),
	TEXT("Ошибка"),
	TEXT("Не удалось открыть файл"),
	TEXT("Неудачное размещение в память."),
	TEXT("Ошибка при разборе элемента."),
	TEXT("Не удалось прочсть имя элемента"),
	TEXT("Ошибка при чтении значения элемента."),
	TEXT("Ошибка при чтении атрибутов."),
	TEXT("Ошибка: пустой тэг."),
	TEXT("Ошибка чтения заканчивающего тэга."),
	TEXT("Ошибка разбора Unknown."),
	TEXT("Ошибка разбора Comment."),
	TEXT("Ошибка разбора Declaration."),
	TEXT("Ошибка - документ пуст.")
};
}