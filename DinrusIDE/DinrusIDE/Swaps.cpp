#include "DinrusIDE.h"

#define LLOG(x)  // DLOG(x)

void AssistEditor::SwapSContext(ParserContext& p)
{
	int i = дайКурсор32();
	if(Ch(i - 1) == ';')
		i--;
	else
		for(;;) {
			int c = Ch(i);
			if(c == '{') {
				i++;
				break;
			}
			if(c == 0 || c == ';' || c == '}')
				break;
			i++;
		}
	Контекст(p, i);
}

bool Иср::SwapSIf(const char *cref)
{
	if(designer || !editor.assist_active)
		return false;
	ParserContext p;
	editor.SwapSContext(p);
	CodeBaseLock __;
	int q = CodeBase().найди(p.current_scope);
	LLOG("SwapS scope: " << p.current_scope << ", имя " << p.current_name << ", ключ " << p.current_key);
	if(q < 0)
		return false;
	const Массив<CppItem>& nn = CodeBase()[q];
	if(cref && MakeCodeRef(p.current_scope, p.current_key) != cref || пусто_ли(p.current_name))
		return false;
	Вектор<const CppItem *> n;
	bool destructor = p.current_key.найди('~') >= 0;
	for(int i = 0; i < nn.дайСчёт(); i++) {
		const CppItem& m = nn[i];
		if(m.имя == p.current_name && destructor == (m.kind == DESTRUCTOR) && !m.IsType())
			n.добавь(&m);
	}
	if(!cref && n.дайСчёт() < 2)
		for(int i = 0; i < nn.дайСчёт(); i++)
			if(nn[i].IsType()) {
				UnlockCodeBaseAll();
				GotoCpp(nn[i]);
				return false;
			}
	if(n.дайСчёт() == 0 || пусто_ли(editfile))
		return false;
	int file = GetSourceFileIndex(editfile);
	int line = p.current.line;
	LLOG("SwapS line: " << line);
	int i;
	for(i = 0; i < n.дайСчёт(); i++) {
		LLOG("file: " << GetSourceFilePath(n[i]->file) << ", line: " << n[i]->line);
		if(n[i]->file == file && n[i]->line == line) {
			i++;
			break;
		}
	}
	CppItem m = *n[i % n.дайСчёт()];
	UnlockCodeBaseAll();
	GotoCpp(m);
	return true;
}

void Иср::SwapS()
{
	SwapSIf(NULL);
}
