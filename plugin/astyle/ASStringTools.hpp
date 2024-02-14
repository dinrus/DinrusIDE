#ifndef __ASSTRINGTOOLS_HPP
#define __ASSTRINGTOOLS_HPP

#include <Core/Core.h>

using namespace Upp;

///////////////////////////////////////////////////////////////////////////////////////////
// Заменяет кусок к строке новой строкой.
void ASString_Replace(WString &s, int Pos, int Len, WString const &newString);

///////////////////////////////////////////////////////////////////////////////////////////
//Находит первый символ в строке, которого нет в другой строке.
int ASString_Find_First_Not_Of(WString const &s, WString const &Pattern, int from = 0);

///////////////////////////////////////////////////////////////////////////////////////////
// Находит последний символ в строке, которого нет в другой строке.
int ASString_Find_Last_Not_Of(WString const &s, WString const &Pattern, int from = -1);

///////////////////////////////////////////////////////////////////////////////////////////
// Находит подстроку, начиная с конца заданной строки.
int ASString_ReverseFind(WString const &s, WString const &Pattern);

#endif
