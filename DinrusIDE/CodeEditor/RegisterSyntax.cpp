#include "CodeEditor.h"

namespace РНЦП {

void CreateCSyntax(Один<EditorSyntax>& e, int kind)
{
	CSyntax& s = e.создай<CSyntax>();
	s.SetHighlight(kind);
}

void RegisterCSyntax(const char *id, int kind,
                     const char *exts, const char *description) {
	EditorSyntax::регистрируй(id, callback1(CreateCSyntax, kind), exts, description);
}

void CreateDiffSyntax(Один<EditorSyntax>& e)
{
	e.создай<DiffSyntax>();
}

void CreatePythonSyntax(Один<EditorSyntax>& e)
{
	e.создай<PythonSyntax>();
}

void CreateTagSyntax(Один<EditorSyntax>& e, bool html, bool witz)
{
	e.создай<TagSyntax>().Html(html).Witz(witz);
}

void CreateLogSyntax(Один<EditorSyntax>& e)
{
	e.создай<LogSyntax>();
}

void RegisterSyntaxModules()
{
	RegisterCSyntax("cpp", CSyntax::HIGHLIGHT_CPP,
	                "*.c *.cpp *.cc *.cxx *.h *.hpp *.hh *.hxx *.m *.mm *.icpp *.conf",
	                "C/C++");
	RegisterCSyntax("java", CSyntax::HIGHLIGHT_JAVA, "*.java", "Java");
	RegisterCSyntax("js", CSyntax::HIGHLIGHT_JAVASCRIPT, "*.js", "JavaScript");
	RegisterCSyntax("cs", CSyntax::HIGHLIGHT_CS, "*.cs", "C#");
	RegisterCSyntax("json", CSyntax::HIGHLIGHT_JSON, "*.json", "JSON");
	RegisterCSyntax("css", CSyntax::HIGHLIGHT_CSS, "*.css", "Cascading Стиль Sheet");
	RegisterCSyntax("sql", CSyntax::HIGHLIGHT_SQL, "*.sql *.ddl", "SQL script");
	RegisterCSyntax("lay", CSyntax::HIGHLIGHT_LAY, "*.lay", "U++ layout");
	RegisterCSyntax("sch", CSyntax::HIGHLIGHT_SCH, "*.sch", "U++ SQL schema");
	RegisterCSyntax("t", CSyntax::HIGHLIGHT_T, "*.t *.jt", "U++ translation");
	RegisterCSyntax("usc", CSyntax::HIGHLIGHT_USC, "*.usc", "U++ widget definitions");
	RegisterCSyntax("calc", CSyntax::HIGHLIGHT_CALC, "", "");
	RegisterCSyntax("php", CSyntax::HIGHLIGHT_PHP, "*.php", "PHP");
	
	EditorSyntax::регистрируй("diff", callback(CreateDiffSyntax), "*.diff *.patch", "Diff");
	
	EditorSyntax::регистрируй("python", callback(CreatePythonSyntax), "*.py, *.pyc, *.pyd, *.pyo, *.pyw, *.pyz", "Python");
	
	EditorSyntax::регистрируй("xml", callback2(CreateTagSyntax, false, false), "*.xml *.xsd", "XML");
	EditorSyntax::регистрируй("html", callback2(CreateTagSyntax, true, false), "*.html *.htm", "HTML");
	EditorSyntax::регистрируй("witz", callback2(CreateTagSyntax, true, true), "*.witz", "Skylark templates");
	
	EditorSyntax::регистрируй("log", callback(CreateLogSyntax), "*.log *.info", "Log");
}

}
