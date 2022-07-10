#include "Browser.h"

void TopicEditor::ListTemplates(Вектор<Ткст>& path, Вектор<Ткст>& имя)
{
	Вектор<Ткст> ud = GetUppDirs();
	for(int i = 0; i < ud.дайСчёт(); i++) {
		Ткст nest = ud[i];
		Ткст tmpl = приставьИмяф(nest, "_.tpp");
		ФайлПоиск ff(приставьИмяф(tmpl, "*.tpp"));
		while(ff) {
			if(ff.файл_ли()) {
				path.добавь(приставьИмяф(tmpl, ff.дайИмя()));
				имя.добавь(дайТитулф(ff.дайИмя()) + " (in " + дайТитулф(nest) + ")");
			}
			ff.следщ();
		}
	}
	IndexSort(имя, path);
}

Ткст TopicEditor::ChooseTemplate(const char *title)
{
	WithTemplateListLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, title);
	Вектор<Ткст> path, имя;
	ListTemplates(path, имя);
	dlg.list.добавьИндекс();
	dlg.list.добавьКолонку();
	for(int i = 0; i < path.дайСчёт(); i++)
		dlg.list.добавь(path[i], имя[i]);
	dlg.list.идиВНач();
	if(dlg.выполни() != IDOK)
		return Null;
	return dlg.list.дайКлюч();
}

void TopicEditor::ApplyStylesheet()
{
	if(!topics_list.курсор_ли())
		return;
	Ткст t = ChooseTemplate("Apply template stylesheet");
	if(t.дайСчёт())
		editor.ApplyStylesheet(ParseQTF(загрузиФайл(t)));
}

void TopicEditor::ApplyStylesheetGroup()
{
	Ткст t = ChooseTemplate("Apply template stylesheet to current группа");
	if(пусто_ли(t))
		return;
	RichText ss = ParseQTF(загрузиФайл(t));
	int c = topics_list.дайКурсор();
	Progress pi("Applying stylesheet");
	for(int i = 0; i < topics_list.дайСчёт(); i++) {
		topics_list.устКурсор(i);
		editor.ApplyStylesheet(ss);
	}
	topics_list.устКурсор(c);
}
