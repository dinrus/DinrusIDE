/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   ASResource.cpp
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
#include "astyle.h"


namespace astyle
{
const ШТкст ASResource::AS_IF = ШТкст("if");
const ШТкст ASResource::AS_ELSE = ШТкст("else");
const ШТкст ASResource::AS_FOR = ШТкст("for");
const ШТкст ASResource::AS_DO = ШТкст("do");
const ШТкст ASResource::AS_WHILE = ШТкст("while");
const ШТкст ASResource::AS_SWITCH = ШТкст("switch");
const ШТкст ASResource::AS_CASE = ШТкст("case");
const ШТкст ASResource::AS_DEFAULT = ШТкст("default");
const ШТкст ASResource::AS_CLASS = ШТкст("class");
const ШТкст ASResource::AS_STRUCT = ШТкст("struct");
const ШТкст ASResource::AS_UNION = ШТкст("union");
const ШТкст ASResource::AS_INTERFACE = ШТкст("interface");
const ШТкст ASResource::AS_NAMESPACE = ШТкст("namespace");
const ШТкст ASResource::AS_EXTERN = ШТкст("extern");
const ШТкст ASResource::AS_PUBLIC = ШТкст("public");
const ШТкст ASResource::AS_PROTECTED = ШТкст("protected");
const ШТкст ASResource::AS_PRIVATE = ШТкст("private");
const ШТкст ASResource::AS_STATIC = ШТкст("static");
const ШТкст ASResource::AS_SYNCHRONIZED = ШТкст("synchronized");
const ШТкст ASResource::AS_OPERATOR = ШТкст("operator");
const ШТкст ASResource::AS_TEMPLATE = ШТкст("template");
const ШТкст ASResource::AS_TRY = ШТкст("try");
const ШТкст ASResource::AS_CATCH = ШТкст("catch");
const ШТкст ASResource::AS_FINALLY = ШТкст("finally");
const ШТкст ASResource::AS_THROWS = ШТкст("throws");
const ШТкст ASResource::AS_CONST = ШТкст("const");

const ШТкст ASResource::AS_ASM = ШТкст("asm");

const ШТкст ASResource::AS_BAR_DEFINE = ШТкст("#define");
const ШТкст ASResource::AS_BAR_INCLUDE = ШТкст("#include");
const ШТкст ASResource::AS_BAR_IF = ШТкст("#if");
const ШТкст ASResource::AS_BAR_EL = ШТкст("#el");
const ШТкст ASResource::AS_BAR_ENDIF = ШТкст("#endif");

const ШТкст ASResource::AS_OPEN_BRACKET = ШТкст("{");
const ШТкст ASResource::AS_CLOSE_BRACKET = ШТкст("}");
const ШТкст ASResource::AS_OPEN_LINE_COMMENT = ШТкст("//");
const ШТкст ASResource::AS_OPEN_COMMENT = ШТкст("/*");
const ШТкст ASResource::AS_CLOSE_COMMENT = ШТкст("*/");

const ШТкст ASResource::AS_ASSIGN = ШТкст("=");
const ШТкст ASResource::AS_PLUS_ASSIGN = ШТкст("+=");
const ШТкст ASResource::AS_MINUS_ASSIGN = ШТкст("-=");
const ШТкст ASResource::AS_MULT_ASSIGN = ШТкст("*=");
const ШТкст ASResource::AS_DIV_ASSIGN = ШТкст("/=");
const ШТкст ASResource::AS_MOD_ASSIGN = ШТкст("%=");
const ШТкст ASResource::AS_OR_ASSIGN = ШТкст("|=");
const ШТкст ASResource::AS_AND_ASSIGN = ШТкст("&=");
const ШТкст ASResource::AS_XOR_ASSIGN = ШТкст("^=");
const ШТкст ASResource::AS_GR_GR_ASSIGN = ШТкст(">>=");
const ШТкст ASResource::AS_LS_LS_ASSIGN = ШТкст("<<=");
const ШТкст ASResource::AS_GR_GR_GR_ASSIGN = ШТкст(">>>=");
const ШТкст ASResource::AS_LS_LS_LS_ASSIGN = ШТкст("<<<=");
const ШТкст ASResource::AS_RETURN = ШТкст("return");

const ШТкст ASResource::AS_EQUAL = ШТкст("==");
const ШТкст ASResource::AS_PLUS_PLUS = ШТкст("++");
const ШТкст ASResource::AS_MINUS_MINUS = ШТкст("--");
const ШТкст ASResource::AS_NOT_EQUAL = ШТкст("!=");
const ШТкст ASResource::AS_GR_EQUAL = ШТкст(">=");
const ШТкст ASResource::AS_GR_GR = ШТкст(">>");
const ШТкст ASResource::AS_GR_GR_GR = ШТкст(">>>");
const ШТкст ASResource::AS_LS_EQUAL = ШТкст("<=");
const ШТкст ASResource::AS_LS_LS = ШТкст("<<");
const ШТкст ASResource::AS_LS_LS_LS = ШТкст("<<<");
const ШТкст ASResource::AS_ARROW = ШТкст("->");
const ШТкст ASResource::AS_AND = ШТкст("&&");
const ШТкст ASResource::AS_OR = ШТкст("||");
const ШТкст ASResource::AS_COLON_COLON = ШТкст("::");
const ШТкст ASResource::AS_PAREN_PAREN = ШТкст("()");
const ШТкст ASResource::AS_BLPAREN_BLPAREN = ШТкст("[]");

const ШТкст ASResource::AS_PLUS = ШТкст("+");
const ШТкст ASResource::AS_MINUS = ШТкст("-");
const ШТкст ASResource::AS_MULT = ШТкст("*");
const ШТкст ASResource::AS_DIV = ШТкст("/");
const ШТкст ASResource::AS_MOD = ШТкст("%");
const ШТкст ASResource::AS_GR = ШТкст(">");
const ШТкст ASResource::AS_LS = ШТкст("<");
const ШТкст ASResource::AS_NOT = ШТкст("!");
const ШТкст ASResource::AS_BIT_OR = ШТкст("|");
const ШТкст ASResource::AS_BIT_AND = ШТкст("&");
const ШТкст ASResource::AS_BIT_NOT = ШТкст("~");
const ШТкст ASResource::AS_BIT_XOR = ШТкст("^");
const ШТкст ASResource::AS_QUESTION = ШТкст("?");
const ШТкст ASResource::AS_COLON = ШТкст(":");
const ШТкст ASResource::AS_COMMA = ШТкст(",");
const ШТкст ASResource::AS_SEMICOLON = ШТкст(";");

const ШТкст ASResource::AS_FOREACH = ШТкст("foreach");
const ШТкст ASResource::AS_LOCK = ШТкст("lock");
const ШТкст ASResource::AS_UNSAFE = ШТкст("unsafe");
const ШТкст ASResource::AS_FIXED = ШТкст("fixed");
const ШТкст ASResource::AS_GET = ШТкст("get");
const ШТкст ASResource::AS_SET = ШТкст("set");
const ШТкст ASResource::AS_ADD = ШТкст("add");
const ШТкст ASResource::AS_REMOVE = ШТкст("remove");

const ШТкст ASResource::AS_CONST_CAST = ШТкст("const_cast");
const ШТкст ASResource::AS_DYNAMIC_CAST = ШТкст("dynamic_cast");
const ШТкст ASResource::AS_REINTERPRET_CAST = ШТкст("reinterpret_cast");
const ШТкст ASResource::AS_STATIC_CAST = ШТкст("static_cast");


/**
 * Build the vector of assignment operators.
 * Used by BOTH ASFormatter.cpp and ASBeautifier.cpp
 *
 * @param assignmentOperators   a reference to the vector to be built.
 */
void ASResource::buildAssignmentOperators(Вектор<const ШТкст*> &assignmentOperators)
{
	assignmentOperators.push_back(&AS_ASSIGN);
	assignmentOperators.push_back(&AS_PLUS_ASSIGN);
	assignmentOperators.push_back(&AS_MINUS_ASSIGN);
	assignmentOperators.push_back(&AS_MULT_ASSIGN);
	assignmentOperators.push_back(&AS_DIV_ASSIGN);
	assignmentOperators.push_back(&AS_MOD_ASSIGN);
	assignmentOperators.push_back(&AS_OR_ASSIGN);
	assignmentOperators.push_back(&AS_AND_ASSIGN);
	assignmentOperators.push_back(&AS_XOR_ASSIGN);

	// Java
	assignmentOperators.push_back(&AS_GR_GR_GR_ASSIGN);
	assignmentOperators.push_back(&AS_GR_GR_ASSIGN);
	assignmentOperators.push_back(&AS_LS_LS_ASSIGN);

	// Unknown
	assignmentOperators.push_back(&AS_LS_LS_LS_ASSIGN);

	assignmentOperators.push_back(&AS_RETURN);
}

/**
 * Build the vector of C++ cast operators.
 * Used by ONLY ASFormatter.cpp
 *
 * @param castOperators     a reference to the vector to be built.
 */
void ASResource::buildCastOperators(Вектор<const ШТкст*> &castOperators)
{
	castOperators.push_back(&AS_CONST_CAST);
	castOperators.push_back(&AS_DYNAMIC_CAST);
	castOperators.push_back(&AS_REINTERPRET_CAST);
	castOperators.push_back(&AS_STATIC_CAST);
}

/**
 * Build the vector of header words.
 * Used by BOTH ASFormatter.cpp and ASBeautifier.cpp
 *
 * @param headers       a reference to the vector to be built.
 */
void ASResource::buildHeaders(Вектор<const ШТкст*> &headers, int fileType, bool beautifier)
{
	headers.push_back(&AS_IF);
	headers.push_back(&AS_ELSE);
	headers.push_back(&AS_FOR);
	headers.push_back(&AS_WHILE);
	headers.push_back(&AS_DO);
	headers.push_back(&AS_SWITCH);
	headers.push_back(&AS_TRY);
	headers.push_back(&AS_CATCH);

	if (beautifier)
	{
		headers.push_back(&AS_CASE);
		headers.push_back(&AS_DEFAULT);
		headers.push_back(&AS_CONST);
		headers.push_back(&AS_STATIC);
		headers.push_back(&AS_EXTERN);
		headers.push_back(&AS_TEMPLATE);
	}

	if (fileType == JAVA_TYPE)
	{
		headers.push_back(&AS_FINALLY);
		headers.push_back(&AS_SYNCHRONIZED);
	}

	if (fileType == SHARP_TYPE)
	{
		headers.push_back(&AS_FINALLY);
		headers.push_back(&AS_FOREACH);
		headers.push_back(&AS_LOCK);
		headers.push_back(&AS_UNSAFE);
		headers.push_back(&AS_FIXED);
		headers.push_back(&AS_GET);
		headers.push_back(&AS_SET);
		headers.push_back(&AS_ADD);
		headers.push_back(&AS_REMOVE);
	}
}

/**
 * Build the vector of non-assignment operators.
 * Used by ONLY ASBeautifier.cpp
 *
 * @param nonParenHeaders       a reference to the vector to be built.
 */
void ASResource::buildNonAssignmentOperators(Вектор<const ШТкст*> &nonAssignmentOperators)
{
	nonAssignmentOperators.push_back(&AS_EQUAL);
	nonAssignmentOperators.push_back(&AS_PLUS_PLUS);
	nonAssignmentOperators.push_back(&AS_MINUS_MINUS);
	nonAssignmentOperators.push_back(&AS_NOT_EQUAL);
	nonAssignmentOperators.push_back(&AS_GR_EQUAL);
	nonAssignmentOperators.push_back(&AS_GR_GR_GR);
	nonAssignmentOperators.push_back(&AS_GR_GR);
	nonAssignmentOperators.push_back(&AS_LS_EQUAL);
	nonAssignmentOperators.push_back(&AS_LS_LS_LS);
	nonAssignmentOperators.push_back(&AS_LS_LS);
	nonAssignmentOperators.push_back(&AS_ARROW);
	nonAssignmentOperators.push_back(&AS_AND);
	nonAssignmentOperators.push_back(&AS_OR);
}

/**
 * Build the vector of header non-paren headers.
 * Used by BOTH ASFormatter.cpp and ASBeautifier.cpp
 *
 * @param nonParenHeaders       a reference to the vector to be built.
 */
void ASResource::buildNonParenHeaders(Вектор<const ШТкст*> &nonParenHeaders, int fileType, bool beautifier)
{
	nonParenHeaders.push_back(&AS_ELSE);
	nonParenHeaders.push_back(&AS_DO);
	nonParenHeaders.push_back(&AS_TRY);

	if (beautifier)
	{
		nonParenHeaders.push_back(&AS_CASE);
		nonParenHeaders.push_back(&AS_DEFAULT);
		nonParenHeaders.push_back(&AS_CONST);
		nonParenHeaders.push_back(&AS_STATIC);
		nonParenHeaders.push_back(&AS_EXTERN);
		nonParenHeaders.push_back(&AS_TEMPLATE);
	}

	if (fileType == JAVA_TYPE)
	{
		nonParenHeaders.push_back(&AS_FINALLY);
	}

	if (fileType == SHARP_TYPE)
	{
		nonParenHeaders.push_back(&AS_FINALLY);
		nonParenHeaders.push_back(&AS_UNSAFE);
		nonParenHeaders.push_back(&AS_GET);
		nonParenHeaders.push_back(&AS_SET);
		nonParenHeaders.push_back(&AS_ADD);
		nonParenHeaders.push_back(&AS_REMOVE);
	}
}

/**
 * Build the vector of operators.
 * Used by ONLY ASFormatter.cpp
 *
 * @param operators             a reference to the vector to be built.
 */
void ASResource::buildOperators(Вектор<const ШТкст*> &operators)
{
	operators.push_back(&AS_PLUS_ASSIGN);
	operators.push_back(&AS_MINUS_ASSIGN);
	operators.push_back(&AS_MULT_ASSIGN);
	operators.push_back(&AS_DIV_ASSIGN);
	operators.push_back(&AS_MOD_ASSIGN);
	operators.push_back(&AS_OR_ASSIGN);
	operators.push_back(&AS_AND_ASSIGN);
	operators.push_back(&AS_XOR_ASSIGN);
	operators.push_back(&AS_EQUAL);
	operators.push_back(&AS_PLUS_PLUS);
	operators.push_back(&AS_MINUS_MINUS);
	operators.push_back(&AS_NOT_EQUAL);
	operators.push_back(&AS_GR_EQUAL);
	operators.push_back(&AS_GR_GR_GR_ASSIGN);
	operators.push_back(&AS_GR_GR_ASSIGN);
	operators.push_back(&AS_GR_GR_GR);
	operators.push_back(&AS_GR_GR);
	operators.push_back(&AS_LS_EQUAL);
	operators.push_back(&AS_LS_LS_LS_ASSIGN);
	operators.push_back(&AS_LS_LS_ASSIGN);
	operators.push_back(&AS_LS_LS_LS);
	operators.push_back(&AS_LS_LS);
	operators.push_back(&AS_ARROW);
	operators.push_back(&AS_AND);
	operators.push_back(&AS_OR);
	operators.push_back(&AS_COLON_COLON);
	operators.push_back(&AS_PLUS);
	operators.push_back(&AS_MINUS);
	operators.push_back(&AS_MULT);
	operators.push_back(&AS_DIV);
	operators.push_back(&AS_MOD);
	operators.push_back(&AS_QUESTION);
	operators.push_back(&AS_COLON);
	operators.push_back(&AS_ASSIGN);
	operators.push_back(&AS_LS);
	operators.push_back(&AS_GR);
	operators.push_back(&AS_NOT);
	operators.push_back(&AS_BIT_OR);
	operators.push_back(&AS_BIT_AND);
	operators.push_back(&AS_BIT_NOT);
	operators.push_back(&AS_BIT_XOR);
	operators.push_back(&AS_OPERATOR);
	operators.push_back(&AS_COMMA);
	operators.push_back(&AS_RETURN);
}

/**
 * Build the vector of pre-block statements.
 * Used by ONLY ASBeautifier.cpp
 *
 * @param preBlockStatements        a reference to the vector to be built.
 */
void ASResource::buildPreBlockStatements(Вектор<const ШТкст*> &preBlockStatements)
{
	preBlockStatements.push_back(&AS_CLASS);
	preBlockStatements.push_back(&AS_STRUCT);
	preBlockStatements.push_back(&AS_UNION);
	preBlockStatements.push_back(&AS_INTERFACE);
	preBlockStatements.push_back(&AS_NAMESPACE);
	preBlockStatements.push_back(&AS_THROWS);
	preBlockStatements.push_back(&AS_EXTERN);
}

/**
 * Build the vector of pre-command headers.
 * Used by ONLY ASFormatter.cpp
 *
 * @param preCommandHeaders     a reference to the vector to be built.
 */
void ASResource::buildPreCommandHeaders(Вектор<const ШТкст*> &preCommandHeaders)
{
	preCommandHeaders.push_back(&AS_EXTERN);
	preCommandHeaders.push_back(&AS_THROWS);
	preCommandHeaders.push_back(&AS_CONST);
}

/**
 * Build the vector of pre-definition headers.
 * Used by ONLY ASFormatter.cpp
 *
 * @param preDefinitionHeaders      a reference to the vector to be built.
 */
void ASResource::buildPreDefinitionHeaders(Вектор<const ШТкст*> &preDefinitionHeaders)
{
	preDefinitionHeaders.push_back(&AS_CLASS);
	preDefinitionHeaders.push_back(&AS_INTERFACE);
	preDefinitionHeaders.push_back(&AS_NAMESPACE);
	preDefinitionHeaders.push_back(&AS_STRUCT);
}


}   // end namespace astyle
