/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   astyle.h
 *
 *   This file is a part of "Artistic Стиль" - an indentation and
 *   reformatting tool for C, C++, C# and Java source files.
 *   http://astyle.sourceforge.net
 *
 *   The "Artistic Стиль" project, including all files needed to
 *   compile it, is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the освободи Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later
 *   version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this project; if not, write to the
 *   освободи Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *   Boston, MA  02110-1301, USA.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
 
 /*
 	2008-01-26 Patches by Massimo Del Fedele :
 		- modified sources to use Ultimate++ containers instead std:: ones
 		- fixed memory leaks based on bug report 1804791 submitted by Eran Ifrah
 		- modified to work with unicode
 */
#ifndef __ASSTRINGTOOLS_HPP
#define __ASSTRINGTOOLS_HPP

#include <Core/Core.h>

using namespace РНЦП;

///////////////////////////////////////////////////////////////////////////////////////////
// Replaces a chunk in a string with a new string
void ASString_Replace(ШТкст &s, int Поз, int Len, ШТкст const &newString);

///////////////////////////////////////////////////////////////////////////////////////////
// найди first character in a string *not* contained in another string
int ASString_Find_First_Not_Of(ШТкст const &s, ШТкст const &образец, int from = 0);

///////////////////////////////////////////////////////////////////////////////////////////
// найди last character in a string *not* contained in another string
int ASString_Find_Last_Not_Of(ШТкст const &s, ШТкст const &образец, int from = -1);

///////////////////////////////////////////////////////////////////////////////////////////
// Finds a substring starting at the end of a given string
int ASString_ReverseFind(ШТкст const &s, ШТкст const &образец);

#endif
