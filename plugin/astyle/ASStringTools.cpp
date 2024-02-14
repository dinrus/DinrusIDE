#include "ASStringTools.hpp"

///////////////////////////////////////////////////////////////////////////////////////////
// Replaces a chunk in a string with a new string
void ASString_Replace(WString &s, int Pos, int Len, WString const &newString)
{
    if(Pos < 0 || Pos >= s.GetCount())
        return;
    s.Remove(Pos, Len);
    s.Insert(Pos, newString);

} // END ASString_Replace()

///////////////////////////////////////////////////////////////////////////////////////////
// Find first character in a string *not* contained in another string
int ASString_Find_First_Not_Of(WString const &s, WString const &Pattern, int pos)
{
    if(pos < 0 || pos >= s.GetCount())
        return -1;
    int len = s.GetCount();
    while(pos < len && Pattern.Find(s[pos]) != -1)
        pos++;
    if(pos < len)
        return pos;
    else
        return -1;

} // END ASString_Find_First_Not_Of()

///////////////////////////////////////////////////////////////////////////////////////////
// Find last character in a string *not* contained in another string
int ASString_Find_Last_Not_Of(WString const &s, WString const &Pattern, int pos)
{
    if(pos < 0 || pos >= s.GetCount())
        pos = s.GetCount() -1;
    while(pos > 0 && Pattern.Find(s[pos]) != -1)
        pos--;
    return pos;

} // END ASString_Find_Last_Not_Of()

///////////////////////////////////////////////////////////////////////////////////////////
// Finds a substring starting at the end of a given string
int ASString_ReverseFind(WString const &s, WString const &Pattern)
{
    int pos = -1;
    int k = 0;
    while( (k = s.Find(Pattern, k)) >= 0)
        pos = k++;
    return pos;

} // END ASString_ReverseFind()
