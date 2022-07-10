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

#ifndef ASTYLE_H
#define ASTYLE_H

#include <Core/Core.h>
#include "ASStringTools.hpp"

// 4996 - secure version deprecation warnings for .NET 2005
// 4267 - 64 bit signed/unsigned loss of data
#ifdef _MSC_VER
#pragma warning(disable: 4996)
#pragma warning(disable: 4267)
#endif

namespace astyle
{

enum FileType      { C_TYPE=0, JAVA_TYPE=1, SHARP_TYPE=2 };

/* The enums below are not recognized by 'vectors' in Microsoft Visual C++
   V5 when they are part of a namespace!!!  Use Visual C++ V6 or higher.
*/
enum BracketMode   { NONE_MODE, ATTACH_MODE, BREAK_MODE, BDAC_MODE };

// added by Massimo Del Fedele - just an helper for dialog setup
enum ParenthesisPad { PAD_NONE, PAD_INSIDE, PAD_OUTSIDE, PAD_BOTH };

enum BracketType   { NULL_TYPE = 0,
                     NAMESPACE_TYPE = 1,		// also a DEFINITION_TYPE
                     CLASS_TYPE = 2,			// also a DEFINITION_TYPE
                     DEFINITION_TYPE = 4,
                     COMMAND_TYPE = 8,
                     ARRAY_TYPE  = 16,          // arrays and enums
                     SINGLE_LINE_TYPE = 32
                   };
class ASSourceIterator
{
	public:
		int eolWindows;
		int eolLinux;
		int eolMacOld;
		char outputEOL[4];    // output end of line char
		ASSourceIterator() { eolWindows = eolLinux = eolMacOld = 0; }
		virtual ~ASSourceIterator() {}
		virtual bool hasMoreLines() const = 0;
		virtual ШТкст nextLine() = 0;
};

class ASResource
{
	public:
		void buildAssignmentOperators(Вектор<const ШТкст*> &assignmentOperators);
		void buildCastOperators(Вектор<const ШТкст*> &castOperators);
		void buildHeaders(Вектор<const ШТкст*> &headers, int fileType, bool beautifier=false);
		void buildNonAssignmentOperators(Вектор<const ШТкст*> &nonAssignmentOperators);
		void buildNonParenHeaders(Вектор<const ШТкст*> &nonParenHeaders, int fileType, bool beautifier=false);
		void buildOperators(Вектор<const ШТкст*> &operators);
		void buildPreBlockStatements(Вектор<const ШТкст*> &preBlockStatements);
		void buildPreCommandHeaders(Вектор<const ШТкст*> &preCommandHeaders);
		void buildPreDefinitionHeaders(Вектор<const ШТкст*> &preDefinitionHeaders);

	public:
		static const ШТкст AS_IF, AS_ELSE;
		static const ШТкст AS_DO, AS_WHILE;
		static const ШТкст AS_FOR;
		static const ШТкст AS_SWITCH, AS_CASE, AS_DEFAULT;
		static const ШТкст AS_TRY, AS_CATCH, AS_THROWS, AS_FINALLY;
		static const ШТкст AS_PUBLIC, AS_PROTECTED, AS_PRIVATE;
		static const ШТкст AS_CLASS, AS_STRUCT, AS_UNION, AS_INTERFACE, AS_NAMESPACE, AS_EXTERN;
		static const ШТкст AS_STATIC;
		static const ШТкст AS_CONST;
		static const ШТкст AS_SYNCHRONIZED;
		static const ШТкст AS_OPERATOR, AS_TEMPLATE;
		static const ШТкст AS_OPEN_BRACKET, AS_CLOSE_BRACKET;
		static const ШТкст AS_OPEN_LINE_COMMENT, AS_OPEN_COMMENT, AS_CLOSE_COMMENT;
		static const ШТкст AS_BAR_DEFINE, AS_BAR_INCLUDE, AS_BAR_IF, AS_BAR_EL, AS_BAR_ENDIF;
		static const ШТкст AS_RETURN;
		static const ШТкст AS_ASSIGN, AS_PLUS_ASSIGN, AS_MINUS_ASSIGN, AS_MULT_ASSIGN;
		static const ШТкст AS_DIV_ASSIGN, AS_MOD_ASSIGN, AS_XOR_ASSIGN, AS_OR_ASSIGN, AS_AND_ASSIGN;
		static const ШТкст AS_GR_GR_ASSIGN, AS_LS_LS_ASSIGN, AS_GR_GR_GR_ASSIGN, AS_LS_LS_LS_ASSIGN;
		static const ШТкст AS_EQUAL, AS_PLUS_PLUS, AS_MINUS_MINUS, AS_NOT_EQUAL, AS_GR_EQUAL, AS_GR_GR_GR, AS_GR_GR;
		static const ШТкст AS_LS_EQUAL, AS_LS_LS_LS, AS_LS_LS, AS_ARROW, AS_AND, AS_OR;
		static const ШТкст AS_COLON_COLON, AS_PAREN_PAREN, AS_BLPAREN_BLPAREN;
		static const ШТкст AS_PLUS, AS_MINUS, AS_MULT, AS_DIV, AS_MOD, AS_GR, AS_LS;
		static const ШТкст AS_NOT, AS_BIT_XOR, AS_BIT_OR, AS_BIT_AND, AS_BIT_NOT;
		static const ШТкст AS_QUESTION, AS_COLON, AS_SEMICOLON, AS_COMMA;
		static const ШТкст AS_ASM;
		static const ШТкст AS_FOREACH, AS_LOCK, AS_UNSAFE, AS_FIXED;
		static const ШТкст AS_GET, AS_SET, AS_ADD, AS_REMOVE;
		static const ШТкст AS_CONST_CAST, AS_DYNAMIC_CAST, AS_REINTERPRET_CAST, AS_STATIC_CAST;
};

class ASBeautifier : protected ASResource
{
	public:
		ASBeautifier();
		virtual ~ASBeautifier();
		virtual void init(ASSourceIterator* iter); // pointer to dynamically created iterator.
		void init();
		virtual bool hasMoreLines() const;
		virtual ШТкст nextLine();
		virtual ШТкст beautify(const ШТкст &line);
		void setTabIndentation(int length = 4, bool forceTabs = false);
		void setSpaceIndentation(int length = 4);
		void setMaxInStatementIndentLength(int max);
		void setMinConditionalIndentLength(int min);
		void setClassIndent(bool state);
		void setSwitchIndent(bool state);
		void setCaseIndent(bool state);
		void setBracketIndent(bool state);
		void setBlockIndent(bool state);
		void setNamespaceIndent(bool state);
		void setLabelIndent(bool state);
		void setCStyle();
		void setJavaStyle();
		void setSharpStyle();
		void setEmptyLineFill(bool state);
		void setPreprocessorIndent(bool state);
		int  getIndentLength(void);
		ШТкст getIndentString(void);
		bool getCaseIndent(void);
		bool getCStyle(void);
		bool getJavaStyle(void);
		bool getSharpStyle(void);
		bool getEmptyLineFill(void);

	protected:
		int getNextProgramCharDistance(const ШТкст &line, int i);
//		bool isLegalNameChar(char ch) const;
		const ШТкст *findHeader(const ШТкст &line, int i,
		                         const Вектор<const ШТкст*> &possibleHeaders,
		                         bool checkBoundry = true);
		ШТкст trim(const ШТкст &str);
		int  indexOf(Вектор<const ШТкст*> &container, const ШТкст *element);
		int  fileType;
		bool isCStyle;
		bool isJavaStyle;
		bool isSharpStyle;

		// variables set by ASFormatter - must be updated in preprocessor
		int  inLineNumber;              // for debugging
		int  outLineNumber;				// for debugging
		bool lineCommentNoBeautify;
		bool isNonInStatementArray;

	private:
		ASBeautifier(const ASBeautifier &copy);
		void operator=(ASBeautifier&); // not to be implemented

		void initStatic();
		void registerInStatementIndent(const ШТкст &line, int i, int spaceTabCount,
		                               int minIndent, bool updateParenStack);
		ШТкст preLineWS(int spaceTabCount, int tabCount);

		static Вектор<const ШТкст*> headers;
		static Вектор<const ШТкст*> nonParenHeaders;
		static Вектор<const ШТкст*> preBlockStatements;
		static Вектор<const ШТкст*> assignmentOperators;
		static Вектор<const ШТкст*> nonAssignmentOperators;

		ASSourceIterator *sourceIterator;
		Вектор<ASBeautifier*> *waitingBeautifierStack;
		Вектор<ASBeautifier*> *activeBeautifierStack;
		Вектор<int> *waitingBeautifierStackLengthStack;
		Вектор<int> *activeBeautifierStackLengthStack;
		WithDeepCopy<Вектор<const ШТкст*> > *headerStack;
		WithDeepCopy<Вектор< Вектор<const ШТкст*>* > >*tempStacks;
		WithDeepCopy<Вектор<int> > *blockParenDepthStack;
		WithDeepCopy<Вектор<bool> > *blockStatementStack;
		WithDeepCopy<Вектор<bool> >*parenStatementStack;
		WithDeepCopy<Вектор<int> > *inStatementIndentStack;
		WithDeepCopy<Вектор<int> > *inStatementIndentStackSizeStack;
		WithDeepCopy<Вектор<int> > *parenIndentStack;
		WithDeepCopy<Вектор<bool> >*bracketBlockStateStack;
		ШТкст indentString;
		const ШТкст *currentHeader;
		const ШТкст *previousLastLineHeader;
		const ШТкст *immediatelyPreviousAssignmentOp;
		const ШТкст *probationHeader;
		bool isInQuote;
		bool isInComment;
		bool isInCase;
		bool isInQuestion;
		bool isInStatement;
		bool isInHeader;
		bool isInOperator;
		bool isInTemplate;
		bool isInDefine;
		bool isInDefineDefinition;
		bool classIndent;
		bool isInClassHeader;
		bool isInClassHeaderTab;
		bool switchIndent;
		bool caseIndent;
		bool namespaceIndent;
		bool bracketIndent;
		bool blockIndent;
		bool labelIndent;
		bool preprocessorIndent;
		bool isInConditional;
		bool isMinimalConditinalIndentSet;
		bool shouldForceTabIndentation;
		bool emptyLineFill;
		bool backslashEndsPrevLine;
		bool blockCommentNoIndent;
		bool blockCommentNoBeautify;
		bool previousLineProbationTab;
		int  minConditionalIndent;
		int  parenDepth;
		int  indentLength;
		int  blockTabCount;
		int  leadingWhiteSpaces;
		int  maxInStatementIndent;
		int  templateDepth;
		int  prevFinalLineSpaceTabCount;
		int  prevFinalLineTabCount;
		int  defineTabCount;
		wchar quoteChar;
		wchar prevNonSpaceCh;
		wchar currentNonSpaceCh;
		wchar currentNonLegalCh;
		wchar prevNonLegalCh;
		wchar peekNextChar(ШТкст &line, int i);

	protected:    // inline functions
		// check if a specific character can be used in a legal variable/method/class имя
		inline bool isLegalNameChar(wchar ch) const {
			return (isalnum(ch) || ch == '.' || ch == '_' || (isJavaStyle && ch == '$') || (isCStyle && ch == '~'));
		}

		// check if a specific character is a whitespace character
		inline bool isWhiteSpace(wchar ch) const {
			return (ch == ' ' || ch == '\t');
		}
};


class ASEnhancer
{
	public:
		// functions
		ASEnhancer();
		~ASEnhancer();
		void init(int, ШТкст, bool, bool, bool, bool, bool);
		void enhance(ШТкст &line);

	private:
		// set by init function
		int    indentLength;
		bool   useTabs;
		bool   isCStyle;
		bool   isJavaStyle;
		bool   isSharpStyle;
		bool   caseIndent;
		bool   emptyLineFill;

		// parsing variables
		int  lineNumber;
		bool isInQuote;
		bool isInComment;
		wchar quoteChar;

		// unindent variables
		int  bracketCount;
		int  switchDepth;
		bool lookingForCaseBracket;
		bool unindentNextLine;

		// ТкстПоток for trace
		ТкстПоток *traceOut;

	private:    // private functions
		bool findKeyword(const ШТкст &line, int i, const char *header) const;
		int  indentLine(ШТкст  &line, const int indent) const;
		int  unindentLine(ШТкст  &line, const int unindent) const;

	private:
		// struct used by ParseFormattedLine function
		// contains variables used to unindent the case blocks
		struct switchVariables {
			int  switchBracketCount;
			int  unindentDepth;
			bool unindentCase;

			switchVariables() {                 // constructor
				switchBracketCount = 0;
				unindentDepth = 0;
				unindentCase = false;
			}
		};

	private:    // inline functions
		// check if a specific character can be used in a legal variable/method/class имя
		inline bool isLegalNameCharX(wchar ch) const {
			return (isalnum(ch) || ch == '.' || ch == '_' || (isJavaStyle && ch == '$') || (isCStyle && ch == '~'));
		}

		// check if a specific character is a whitespace character
		inline bool isWhiteSpaceX(wchar ch) const {
			return (ch == ' ' || ch == '\t');
		}
};


class ASFormatter : public ASBeautifier, private ASEnhancer
{
	public:
		ASFormatter();
		virtual ~ASFormatter();
		virtual void init(ASSourceIterator* iter);
		virtual bool hasMoreLines() const;
		virtual ШТкст nextLine();
		void setBracketFormatMode(BracketMode mode);
		void setBreakClosingHeaderBracketsMode(bool state);
		void setOperatorPaddingMode(bool mode);
		void setParensOutsidePaddingMode(bool mode);
		void setParensInsidePaddingMode(bool mode);
		void setParensUnPaddingMode(bool state);
		void setBreakOneLineBlocksMode(bool state);
		void setSingleStatementsMode(bool state);
		void setTabSpaceConversionMode(bool state);
		void setBreakBlocksMode(bool state);
		void setBreakClosingHeaderBlocksMode(bool state);
		void setBreakElseIfsMode(bool state);
		ШТкст fileName;

	private:
		void ASformatter(ASFormatter &copy);            // not to be imlpemented
		void operator=(ASFormatter&);                  // not to be implemented
		void staticInit();
		void goForward(int i);
		void trimNewLine();
		wchar peekNextChar() const;
		BracketType getBracketType() const;
		bool getNextChar();
		bool isBeforeComment() const;
		bool isBeforeLineEndComment(int startPos) const;
		bool isPointerOrReference() const;
		bool isUnaryMinus() const;
		bool isInExponent() const;
		bool isOneLineBlockReached() const;
//		bool isNextCharWhiteSpace() const;
		bool lineBeginsWith(wchar charToCheck) const;
		void appendChar(wchar ch, bool canBreakLine = true);
		void appendCharInsideComments();
		void appendSequence(const ШТкст &sequence, bool canBreakLine = true);
		void appendSpacePad();
		void appendSpaceAfter();
		void breakLine();
		void padOperators(const ШТкст *newOperator);
		void padParens();
		void formatBrackets(BracketType bracketType);
		void formatArrayBrackets(BracketType bracketType, bool isOpeningArrayBracket);
		void adjustComments();
		const ШТкст *findHeader(const Вектор<const ШТкст*> &headers, bool checkBoundry = true);

		static Вектор<const ШТкст*> headers;
		static Вектор<const ШТкст*> nonParenHeaders;
		static Вектор<const ШТкст*> preDefinitionHeaders;
		static Вектор<const ШТкст*> preCommandHeaders;
		static Вектор<const ШТкст*> operators;
		static Вектор<const ШТкст*> assignmentOperators;
		static Вектор<const ШТкст*> castOperators;

		ASSourceIterator *sourceIterator;
		Вектор<const ШТкст*> *preBracketHeaderStack;
		Вектор<BracketType> *bracketTypeStack;
		Вектор<int> *parenStack;
		ШТкст readyFormattedLine;
		ШТкст currentLine;
		ШТкст formattedLine;
		const ШТкст *currentHeader;
		const ШТкст *previousOperator;    // used ONLY by pad=oper
		wchar currentChar;
		wchar previousChar;
		wchar previousNonWSChar;
		wchar previousCommandChar;
		wchar quoteChar;
		int  charNum;
		int  spacePadNum;
		int  templateDepth;
		int  traceFileNumber;
		int  formattedLineCommentNum;		// comment location on formattedLine
		int  previousReadyFormattedLineLength;
		BracketMode bracketFormatMode;
		BracketType previousBracketType;
		bool isVirgin;
		bool shouldPadOperators;
		bool shouldPadParensOutside;
		bool shouldPadParensInside;
		bool shouldUnPadParens;
		bool shouldConvertTabs;
		bool isInLineComment;
		bool isInComment;
		bool isInPreprocessor;
		bool isInTemplate;   // true both in template definitions (e.g. template<class A>) and template usage (e.g. F<int>).
		bool doesLineStartComment;
		bool isInQuote;
		bool isInBlParen;
		bool isSpecialChar;
		bool isNonParenHeader;
		bool foundQuestionMark;
		bool foundPreDefinitionHeader;
		bool foundNamespaceHeader;
		bool foundClassHeader;
		bool foundPreCommandHeader;
		bool foundCastOperator;
		bool isInLineBreak;
//		bool isInClosingBracketLineBreak;
		bool endOfCodeReached;
		bool lineCommentNoIndent;
		bool isLineReady;
		bool isPreviousBracketBlockRelated;
		bool isInPotentialCalculation;
		bool isCharImmediatelyPostComment;
		bool isPreviousCharPostComment;
		bool isCharImmediatelyPostLineComment;
		bool isCharImmediatelyPostOpenBlock;
		bool isCharImmediatelyPostCloseBlock;
		bool isCharImmediatelyPostTemplate;
		bool shouldBreakOneLineBlocks;
		bool shouldReparseCurrentChar;
		bool shouldBreakOneLineStatements;
		bool shouldBreakLineAfterComments;
		bool shouldBreakClosingHeaderBrackets;
		bool shouldBreakElseIfs;
		bool passedSemicolon;
		bool passedColon;
		bool isImmediatelyPostComment;
		bool isImmediatelyPostLineComment;
		bool isImmediatelyPostEmptyBlock;
		bool isImmediatelyPostPreprocessor;

		bool shouldBreakBlocks;
		bool shouldBreakClosingHeaderBlocks;
		bool isPrependPostBlockEmptyLineRequested;
		bool isAppendPostBlockEmptyLineRequested;

		bool prependEmptyLine;
		bool appendOpeningBracket;
		bool foundClosingHeader;

		bool isInHeader;
		bool isImmediatelyPostHeader;

	private:    // inline functions
		// append the CURRENT character (curentChar)to the current formatted line.
		inline void appendCurrentChar(bool canBreakLine = true) {
			appendChar(currentChar, canBreakLine);
		}

		// check if a specific sequence exists in the current placement of the current line
		inline bool isSequenceReached(const char *sequence) const {
//			return currentLine.compare(charNum, strlen(sequence), sequence) == 0;
			return currentLine.середина(charNum, strlen(sequence)) == ШТкст(sequence);
		}
};

}   // end of namespace astyle

// NEEDED FOR РНЦП::Вектор<>
// marks enum BracketType as moveable so it can be used by РНЦП containers
namespace РНЦП
{
	NTL_MOVEABLE(astyle::BracketType);

} // end namespace РНЦП

#endif // closes ASTYLE_H

