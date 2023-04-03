#include <PowerEditor/TinyXml/tinyxml.h>

// The goal of the separate error file is to make the first
// step towards localization. tinyxml (currently) only supports
// latin-1, but at least the error messages could now be translated.
//
// It also cleans up the code a bit.
//
namespace UPP{
const char* TiXmlBase::errorString[ TIXML_ERROR_STRING_COUNT ] =
{
	TEXT("No error"),
	TEXT("Error"),
	TEXT("Failed to open file"),
	TEXT("Memory allocation failed."),
	TEXT("Error parsing Element."),
	TEXT("Failed to read Element name"),
	TEXT("Error reading Element value."),
	TEXT("Error reading Attributes."),
	TEXT("Error: empty tag."),
	TEXT("Error reading end tag."),
	TEXT("Error parsing Unknown."),
	TEXT("Error parsing Comment."),
	TEXT("Error parsing Declaration."),
	TEXT("Error document empty.")
};
}