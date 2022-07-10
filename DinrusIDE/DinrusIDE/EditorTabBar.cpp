#include "DinrusIDE.h"

EditorTabBar::EditorTabBar()
{
	сСтэкингом();
	автоСкрыватьПромот(false);
	FileIcons(false);
	устГраницу(1);
}

Ткст EditorTabBar::GetFile(int n) const
{
	return tabs[n].ключ;
}

bool EditorTabBar::FindSetFile(const Ткст& фн)
{
	int n = найдиКлюч(фн);
	if(n >= 0)
	{
		устКурсор(n);
		return true;
	}
	return false;
}

void EditorTabBar::SetAddFile(const Ткст& фн)
{
	if(пусто_ли(фн))
		return;
	if(FindSetFile(фн))
		return;
	AddFile(ШТкст(фн), IdeFileImage(фн, false, false), true);
}

void EditorTabBar::RenameFile(const Ткст& фн, const Ткст& nn)
{
	FileTabs::RenameFile(ШТкст(фн), ШТкст(nn));
}

void EditorTabBar::FixIcons()
{
	for(int i = 0; i < tabs.дайСчёт(); i++)
		tabs[i].img = IdeFileImage(GetFile(i), false, false);
	Repos();
	освежи();
}

void EditorTabBar::SetSplitColor(const Ткст& фн, const Цвет& c)
{
	int n = -1;
	
	for(int i = 0; i < tabs.дайСчёт(); i++)
	{
		tabs[i].col = Null;
		
		if(n < 0 && tabs[i].ключ == фн)
			n = i;
	}
	
	if(n >= 0)
	{
		if(stacking)
			n = найдиНачСтэка(tabs[n].stack);
		tabs[n].col = c;
	}
	
	освежи();
}

void EditorTabBar::ClearSplitColor()
{
	for(int i = 0; i < tabs.дайСчёт(); i++)
		tabs[i].col = Null;
	освежи();
}

void Иср::TabFile()
{
	int q = tabs.дайКурсор();
	if(q >= 0)
		EditFile(tabs.GetFile(q));
}

void Иср::ClearTab()
{
	int c = tabs.дайКурсор();
	if(c >= 0)
		tabs.закрой(c);
}

void Иср::ClearTabs()
{
	tabs.очисть();
	FileSelected();
}

void Иср::CloseRest(EditorTabBar *tabs)
{
	Индекс<Ткст> фн;
	const РОбласть& wspc = роблИср();
	for(int i = 0; i < wspc.дайСчёт(); i++)
		for(int j = 0; j < wspc.дайПакет(i).file.дайСчёт(); j++)
			фн.добавь(SourcePath(wspc[i], wspc.дайПакет(i).file[j]));
	Ткст cfn;
	if(tabs->дайКурсор() >= 0)
		cfn = tabs->GetFile(tabs->дайКурсор());
	for(int i = tabs->дайСчёт() - 1; i >= 0; i--)
		if(фн.найди(tabs->GetFile(i)) < 0)
			tabs->закрой(i);
	tabs->FindSetFile(cfn);
}

void Иср::TabsLR(int jd)
{
	БарТаб::JumpStack js;
	int tc = tabs.дайСчёт();
	
	int n = tabs.GetTabLR(jd);
	if(n >= 0 && n < tc) {
		js = tabs.jump_stack;
		EditFile(tabs.GetFile(n));
		tabs.jump_stack = js;
	}
}

void Иср::TabsStackLR(int jd)
{
	int tc = tabs.дайСчёт();
  
	int n = tabs.GetTabStackLR(jd);
	if(n >= 0 && n < tc)
		EditFile(tabs.GetFile(n));
}

void Иср::FileSelected()
{
	if(пусто_ли(editfile))
		return;
	
	tabs.SetAddFile(editfile);
}
