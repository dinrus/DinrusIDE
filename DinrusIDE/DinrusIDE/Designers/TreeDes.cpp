#include "Designers.h"

void TreeDesPos::сохрани(Время afiletime, КтрлДерево& tree)
{
	filetime = afiletime;
	scroll = tree.дайПромотку();
	cursor = tree.дайКурсор();
	openid = tree.GetOpenIds();
}

void TreeDesPos::Restore(Время afiletime, КтрлДерево& tree)
{
	if(afiletime == filetime) {
		tree.OpenIds(openid);
		if(tree.пригоден(cursor))
			tree.устКурсор(cursor);
		tree.ScrollTo(scroll);
	}
	tree.устФокус();
}

void сериализуй(Поток& s, МассивМап<Ткст, TreeDesPos>& pos)
{
	int version = 0;
	s / version;
	s % pos;
}

TreeViewDes::TreeViewDes()
{
	tree.NoRoot();

	Ошибка.устШрифт(Arial(20)).устЧернила(красный);
	errorbg.устВысоту(Zy(25)).добавь(Ошибка.SizePos());

	добавь(tree.SizePos());

	tree.WhenLeftDouble = [=] { CopyPath(); };
}

МассивМап<Ткст, TreeDesPos> TreeViewDes::pos;

bool TreeViewDes::грузи(const Ткст& фн)
{
	удалиФрейм(errorbg);
	tree.очисть();

	filename = фн;

	ФайлВвод in(filename);
	if(!in)
		return false;

	filetime = in.дайВремя();
	
	const int limit = 20000000;
	
	Ткст txt = in.дай(limit);
	
	Ткст parsingError = загрузи0(txt);
	if(in.дайРазм() >= limit)
		MergeWith(parsingError, ", ", "file was longer than 20MB and was truncated");
	if(parsingError.дайСчёт()) {
		Ошибка = Ткст() << "XML parsing Ошибка: " << parsingError << ".";
		добавьФрейм(errorbg);
	}

	return tree.GetChildCount(0);
}

void TreeViewDes::EditMenu(Бар& menu)
{
	menu.добавь(tree.курсор_ли(), "копируй current path to clipboard", [=] { CopyPath(); });
}

void TreeViewDes::RestoreEditPos()
{
	pos.дайДобавь(filename + "\n" + дайИд()).Restore(filetime, tree);
}

void TreeViewDes::SaveEditPos()
{
	pos.дайДобавь(filename + "\n" + дайИд()).сохрани(filetime, tree);
}

ИНИЦИАЛИЗАТОР(TreeViewDes)
{
	региструйГлобСериализуй("TreeViewDes", [](Поток& s) {
		int version = 0;
		s / version;
		s % TreeViewDes::pos;
	});
}
