#include "TabBar.h"

namespace РНЦП {

Ткст FileTabs::GetFileGroup(const Ткст &file)
{
	return дайДиректориюФайла(file);
}

Ткст FileTabs::GetStackId(const Вкладка &a)
{
	Ткст s = a.значение;
	return дайТитулф(s);
}

hash_t FileTabs::GetStackSortOrder(const Вкладка &a)
{
	Ткст s = a.значение;
	return дайРасшф(s).дайХэшЗнач();
}

void FileTabs::ComposeTab(Вкладка& tab, const Шрифт &font, Цвет ink, int style)
{
	if(PaintIcons() && tab.естьИконка())
	{
		tab.добавьРисунок(tab.img);
		tab.добавьМеста(TB_SPACEICON);
	}

	ШТкст txt = ткст_ли(tab.значение) ? tab.значение : стдПреобр().фмт(tab.значение);
	int extpos = txt.найдирек('.');
	tab.добавьТекст(extpos >= 0 ? txt.лево(extpos) : txt, font, filecolor);

	if (extpos >= 0) {
		tab.добавьТекст(txt.право(txt.дайДлину() - extpos), font, extcolor);
	}
}

void FileTabs::ComposeStackedTab(Вкладка& tab, const Вкладка& stacked_tab, const Шрифт &font, Цвет ink, int style)
{
	tab.добавьРисунок(TabBarImg::STSEP);

	if (stackedicons && tab.естьИконка()) {
		tab.добавьРисунок(style == CTRL_HOT ? stacked_tab.img : (greyedicons ? DisabledImage(stacked_tab.img) : stacked_tab.img))
			.кликаем();
	}
	else {
		ШТкст txt = ткст_ли(stacked_tab.значение) ? stacked_tab.значение : стдПреобр().фмт(stacked_tab.значение);
		int extpos = txt.найдирек('.');
	
		Цвет c = (style == CTRL_HOT) ? extcolor : SColorDisabled();
		if (extpos >= 0) {
			tab.добавьТекст(
				txt.середина(extpos + 1),
				font,
				c
			).кликаем();
		}
		else {
			tab.добавьТекст("-", font, c).кликаем();
		}
	}
}

Размер FileTabs::GetStackedSize(const Вкладка &t)
{
	if (stackedicons && t.естьИконка())
		return min(t.img.дайРазм(), Размер(TB_ICON, TB_ICON)) + Размер(TB_SPACEICON, 0) + 5;

	ШТкст txt = ткст_ли(t.значение) ? t.значение : стдПреобр().фмт(t.значение);
	int extpos = txt.найдирек('.');
	txt = extpos >= 0 ? txt.середина(extpos + 1) : "-";
	return дайРазмТекста(txt, дайСтиль().font) + Размер(TabBarImg::STSEP().дайРазм().cx, 0);
}

void FileTabs::сериализуй(Поток& s)
{
	БарТаб::сериализуй(s);
	if(s.грузится() && icons)
	{
		for(int i = 0; i < tabs.дайСчёт(); i++)
			tabs[i].img = NativePathIcon(Ткст(tabs[i].значение));
	}
}

void FileTabs::AddFile(const ШТкст &file, bool make_active)
{
	AddFile(file, NativePathIcon(file.вТкст()), make_active);
}

void FileTabs::AddFile(const ШТкст &file, Рисунок img, bool make_active)
{
	InsertFile(дайСчёт(), file, img, make_active);
}

void FileTabs::InsertFile(int ix, const ШТкст &file, bool make_active)
{
	InsertFile(ix, file, NativePathIcon(file.вТкст()), make_active);
}

void FileTabs::InsertFile(int ix, const ШТкст &file, Рисунок img, bool make_active)
{
	Ткст s = file.вТкст();
	БарТаб::вставьКлюч(ix, file, дайИмяф(s), img, GetFileGroup(s), make_active);
}

void FileTabs::AddFiles(const Вектор<Ткст> &files, bool make_active)
{
	AddFiles(files, Вектор<Рисунок>(), make_active);
}

void FileTabs::AddFiles(const Вектор<Ткст> &files, const Вектор<Рисунок> &img, bool make_active)
{
	InsertFiles(дайСчёт(), files, img, make_active);
}

void FileTabs::InsertFiles(int ix, const Вектор<Ткст> &files, bool make_active)
{
	InsertFiles(ix, files, Вектор<Рисунок>(), make_active);
}

void FileTabs::InsertFiles(int ix, const Вектор<Ткст> &files, const Вектор<Рисунок> &img, bool make_active)
{
	if (!files.дайСчёт()) return;
	bool useimg = img.дайСчёт() == files.дайСчёт();
	for (int i = files.дайСчёт() - 1; i > 0; i--) {
		БарТаб::InsertKey0(ix, files[i].вШТкст(), дайИмяф(files[i]), 
			useimg ? img[i] : NativePathIcon(files[i]), GetFileGroup(files[i]));	
	}
	InsertFile(ix, files[0].вШТкст(), useimg ? img[0] : NativePathIcon(files[0]), make_active);
}

void FileTabs::RenameFile(const ШТкст &from, const ШТкст &to, Рисунок icon)
{
	int n = найдиКлюч(from);
	if (n >= 0)
		уст(n, to, дайИмяф(to.вТкст()), пусто_ли(icon) ? NativePathIcon(to.вТкст()) : icon);
}

FileTabs& FileTabs::FileIcons(bool normal, bool stacked, bool stacked_greyedout)
{
	stackedicons = stacked;
	greyedicons = stacked_greyedout;
	БарТаб::сИконками(normal); 
	return *this;	
}

Вектор<Ткст> FileTabs::GetFiles() const
{
	Вектор<Ткст> files;
	files.устСчёт(tabs.дайСчёт());
	for (int i = 0; i < tabs.дайСчёт(); i++)
		files[i] = tabs[i].ключ;
	return files;	
}

FileTabs& FileTabs::operator<<(const FileTabs &src)
{
	очисть();
	сСтэкингом(false);
	сортируйГруппы(false);
	сГруппингом(false);
	AddFiles(src.GetFiles(), src.дайИконки(), false);
	крпируйНастройки(src);	
	stackedicons = src.stackedicons;
	greyedicons = src.greyedicons;
	
	if (src.естьКурсор())
		устДанные(~src);
	sc.устПоз(src.дайПозПромота());
	освежи();
	return *this;
}

FileTabs::FileTabs() :
stackedicons(false), 
greyedicons(true),
filecolor(SColorLabel),
extcolor(тёмен(SColorFace()) ? смешай(белый, светлоСиний) : светлоСиний)
{

}

}
