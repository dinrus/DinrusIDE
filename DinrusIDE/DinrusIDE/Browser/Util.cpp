#include "Browser.h"

void SplitCodeRef(const Ткст& s, Ткст& scope, Ткст& элт)
{
	int q = s.найдиПервыйИз("( ");
	q = q >= 0 ? s.найдирек(':', q) : s.найдирек(':');
	if(q < 0) {
		scope.очисть();
		элт = s;
	}
	else {
		scope = s.середина(0, max(q - 1, 0));
		элт = s.середина(q + 1);
	}
}

Ткст MakeCodeRef(const Ткст& nest, const Ткст& элт)
{
	if(nest.дайСчёт())
		return nest + "::" + элт;
	return элт;
}

int GetMatchLen(const char *s, const char *t)
{
	int i = 0;
	while(s[i] == t[i] && s[i])
		i++;
	return i;
}

const CppItem *GetCodeRefItem(const Ткст& ref, const Ткст& rfile)
{
	CodeBaseLock __;
	Ткст scope;
	Ткст элт;
	SplitCodeRef(ref, scope, элт);
	int q = CodeBase().найди(scope);
	if(q < 0)
		return NULL;
	const Массив<CppItem>& n = CodeBase()[q];
	q = FindItem(n, элт);
	if(q < 0)
		return NULL;
	if(!пусто_ли(rfile)) {
		int i = q;
		int qml = 0;
		while(i < n.дайСчёт() && n[i].qitem == элт) {
			int ml = GetMatchLen(GetSourceFilePath(n[i].file), rfile);
			if(ml > qml) {
				q = i;
				qml = ml;
			}
			i++;
		}
	}
	return &n[q];
}

const CppItem *GetCodeRefItem(const Ткст& ref)
{
	return GetCodeRefItem(ref, Null);
}
