#include "DinrusIDE.h"

using namespace astyle;

/////////////////////////////////////////////////////////////////////////////////////////
// CLASS ASStreamIterator
class ASStreamIterator : public ASSourceIterator
{
	protected:

		enum EolMode { crlf, lf, cr, lfcr } ;
		ШТкст const &src;
		int p;
		EolMode eolMode;

	public:
		ASStreamIterator(ШТкст const &Src);
		virtual ~ASStreamIterator() {}
		virtual bool hasMoreLines() const;
		virtual ШТкст nextLine();

		ШТкст getEol();

}; // END Class ASStreamIterator

// Constructor Class ASStreamIterator
ASStreamIterator::ASStreamIterator(ШТкст const &Src) : ASSourceIterator(), src(Src)
{
	// initializes буфер pointer
	p = 0;

	// initializes Eol Режим
	eolMode = crlf;

} // END Constructor Class ASStreamIterator


ШТкст ASStreamIterator::getEol()
{
	switch(eolMode)
	{
		case cr:
			return "\r";
		case lf:
			return "\n";
		case lfcr:
			return "\r\n";
		default:
			return "\n\r";
	}

} // END ASStreamIterator::getEol()

bool ASStreamIterator::hasMoreLines() const
{
	return src[p] != 0;

} // END ASStreamIterator::hasMoreLines()

ШТкст ASStreamIterator::nextLine()
{
	int p2 = p;
	wchar c1, c2;
	while(src[p2] && src[p2] != '\n' && src[p2] != '\r')
		p2++;
	ШТкст line = src.середина(p, p2 - p);

	if( (c1 = src[p2]) != 0)
	{
		p2++;
		c2 = src[p2];
		if(c1 == '\r' && c2 == '\n')
		{
			p2++;
			eolMode = crlf;
		}
		else if(c1 == '\n' && c2 == '\r')
		{
			p2++;
			eolMode = lfcr;
		}
		else if(c1 == '\n')
			eolMode = lf;
		else
			eolMode = cr;
	}
	p = p2;

	return line;

} // END ASStreamIterator::nextLine()

/////////////////////////////////////////////////////////////////////////////////////////
// Formats a string of code with a given formatter
ШТкст Иср::FormatCodeString(ШТкст const &Src, ASFormatter &Formatter)
{
	// Creates the output string
	ШТкст Dest;

	// Creates the iterator for the formatter
	ASStreamIterator streamIterator(Src);

	// Initializes the formatter to work on selected stream
	Formatter.init(&streamIterator);

	// Processes all text in source file and put them in output file
	while (Formatter.hasMoreLines())
		Dest << Formatter.nextLine() << streamIterator.getEol();

	// returns output string
	return Dest;

} // END Иср::FormatCodeString()

/////////////////////////////////////////////////////////////////////////////////////////
// Formats editor's code with Иср формат parameters
void Иср::FormatCode()
{
	if(editor.толькочтен_ли()) return;
	// Gets editor contents from editor
	ШТкст Src;
	int l, h;
	bool sel = editor.дайВыделение(l, h);
	if(sel)
		Src = editor.GetSelectionW();
	else
		Src = editor.дайШ();

	// Instantiate the formatter object
	ASFormatter Formatter;

	// Sets up the formatter for C++ formatting
	Formatter.setCStyle();

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Sets up astyle options
	Formatter.setBracketIndent(astyle_BracketIndent);
	Formatter.setNamespaceIndent(astyle_NamespaceIndent);
	Formatter.setBlockIndent(astyle_BlockIndent);
	Formatter.setCaseIndent(astyle_CaseIndent);
	Formatter.setClassIndent(astyle_ClassIndent);
	Formatter.setLabelIndent(astyle_LabelIndent);
	Formatter.setSwitchIndent(astyle_SwitchIndent);
	Formatter.setPreprocessorIndent(astyle_PreprocessorIndent);
	Formatter.setMaxInStatementIndentLength(astyle_MaxInStatementIndentLength);
	Formatter.setMinConditionalIndentLength(astyle_MinInStatementIndentLength);
	Formatter.setBreakClosingHeaderBracketsMode(astyle_BreakClosingHeaderBracketsMode);
	Formatter.setBreakElseIfsMode(astyle_BreakElseIfsMode);
	Formatter.setBreakOneLineBlocksMode(astyle_BreakOneLineBlocksMode);
	Formatter.setSingleStatementsMode(astyle_SingleStatementsMode);
	Formatter.setBreakBlocksMode(astyle_BreakBlocksMode);
	Formatter.setBreakClosingHeaderBlocksMode(astyle_BreakClosingHeaderBlocksMode);
	Formatter.setBracketFormatMode((astyle::BracketMode)astyle_BracketFormatMode);
	switch(astyle_ParensPaddingMode)
	{
		case PAD_INSIDE :
			Formatter.setParensInsidePaddingMode(true);
			Formatter.setParensOutsidePaddingMode(false);
			break;
		case PAD_OUTSIDE :
			Formatter.setParensInsidePaddingMode(false);
			Formatter.setParensOutsidePaddingMode(true);
			break;
		case PAD_BOTH :
			Formatter.setParensInsidePaddingMode(true);
			Formatter.setParensOutsidePaddingMode(true);
			break;
		default :
			Formatter.setParensOutsidePaddingMode(false);
			Formatter.setParensInsidePaddingMode(false);
			break;
	}
	Formatter.setParensUnPaddingMode(astyle_ParensUnPaddingMode);
	Formatter.setOperatorPaddingMode(astyle_OperatorPaddingMode);
	Formatter.setEmptyLineFill(astyle_EmptyLineFill);
	Formatter.setTabSpaceConversionMode(astyle_TabSpaceConversionMode);
	if(!indent_spaces)
		Formatter.setTabIndentation(editortabsize, true);
	else
		Formatter.setSpaceIndentation(indent_amount);

	ШТкст Dest = FormatCodeString(Src, Formatter);

	editor.NextUndo();
	if(sel) {
		editor.удали(l, h - l);
		editor.устВыделение(l, l + editor.вставь(l, Dest));
	}
	else {
		editor.удали(0, editor.дайДлину());
		editor.вставь(0, Dest);
	}
}

bool ReFormatJSON_XML( Ткст& text, bool xml)
{
	if(xml) {
		try {
			УзелРяр n = разбериРЯР(text);
			text = какРЯР(n);
		}
		catch(ОшибкаРяр) {
			Exclamation("Ошибка passing the XML!");
			return false;
		}
	}
	else {
		Значение v = ParseJSON(text);
		if(v.ошибка_ли()) {
			Exclamation("Ошибка passing the JSON!");
			return false;
		}
		text = AsJSON(v, true);
	}
	return true;
}

void Иср::FormatJSON_XML(bool xml)
{
	int l, h;
	bool sel = editor.дайВыделение(l, h);
	if((sel ? h - l : editor.дайДлину()) > 75*1024*1024) {
		Exclamation("Too big to reformat");
		return;
	}
	Ткст text;
	if(sel)
		text = editor.дайВыделение();
	else {
		сохраниФайл();
		text = загрузиФайл(editfile);
	}
	if(!ReFormatJSON_XML(text, xml))
		return;
	editor.NextUndo();
	if(sel) {
		editor.удали(l, h - l);
		editor.устВыделение(l, l + editor.вставь(l, text));
	}
	else {
		editor.удали(0, editor.дайДлину());
		editor.вставь(0, text);
	}
}

void Иср::FormatJSON()
{
	FormatJSON_XML(false);
}

void Иср::FormatXML()
{
	FormatJSON_XML(true);
}

void Иср::FormatJSON_XML_File(bool xml)
{
	if(пусто_ли(editfile))
		return;
	if(дайДлинуф(editfile) >= 75*1024*1024)
		Exclamation("Too big to reformat");
	сохраниФайл();
	Ткст text = загрузиФайл(editfile);
	if(!ReFormatJSON_XML(text, xml))
		return;
	if(PromptYesNo("Overwrite \1" + editfile + "\1 with reformated " + (xml ? "XML" : "JSON") + "?")) {
		РНЦП::сохраниФайл(editfile, text);
		EditAsText();
	}
}
