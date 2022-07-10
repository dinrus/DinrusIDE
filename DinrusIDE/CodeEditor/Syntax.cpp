#include "CodeEditor.h"

namespace РНЦП {

void EditorSyntax::очисть()
{
}

void EditorSyntax::ScanSyntax(const wchar *ln, const wchar *e, int line, int tab_size)
{
}

void EditorSyntax::сериализуй(Поток& s)
{
}

void EditorSyntax::IndentInsert(РедакторКода& editor, int chr, int count)
{
	editor.InsertChar(chr, count);
}

bool EditorSyntax::проверьФигСкобы(РедакторКода& e, int64& bpos0, int64& bpos)
{
	return false;
}

bool EditorSyntax::CanAssist() const
{
	return false;
}

void EditorSyntax::перефмтКоммент(РедакторКода& e)
{
}

void EditorSyntax::Highlight(const wchar *s, const wchar *end, HighlightOutput& hls, РедакторКода *editor, int line, int64 pos)
{
}

Вектор<IfState> EditorSyntax::PickIfStack()
{
	return Вектор<IfState>();
}

Цвет EditorSyntax::IfColor(char c)
{
	switch(c){
	case IfState::IF:          return светлоСиний();
	case IfState::ELIF:        return серый();
	case IfState::ELSE:        return зелёный();
	case IfState::ELSE_ERROR:  return светлоКрасный();
	case IfState::ENDIF_ERROR: return светлоМагента();
	default:                   return Null;
	}
}

void EditorSyntax::проверьОсвежиСинтакс(РедакторКода& e, int64 pos, const ШТкст& text)
{
}

EditorSyntax::~EditorSyntax() {}

}
