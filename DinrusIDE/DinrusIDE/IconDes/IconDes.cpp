#include "IconDes.h"

#include <plugin/bmp/bmp.h>
#include <plugin/png/png.h>

bool IdeIconDes::грузи(const char *_filename)
{
	очисть();
	filename = _filename;
	filetime = дайВремяф(filename);
	Массив<ImlImage> iml;
	int f;
	Ткст s = загрузиФайл(filename);
	EOL = GetLineEndings(s);
	if(!LoadIml(s, iml, f))
		return false;
	формат = f;
	for(const ImlImage& m : iml)
		добавьРисунок(m.имя, m.image, m.exp).flags = m.flags;
	return true;
}

void IdeIconDes::сохрани()
{
	if(формат == 1) {
		for(int i = 0; i < дайСчёт(); i++) {
			Рисунок m = GetImage(i);
			Точка p = m.Get2ndSpot();
			if(m.GetKind() == IMAGE_ALPHA || p.x || p.y) {
				if(PromptYesNo("Legacy file формат does not support images "
				               "with full alpha channel or 2nd hotspot - "
				               "the information would be lost.&"
				               "Do you wish to convert the file to the new формат?")) {
					формат = 0;
				}
				break;
			}
		}
	}
	сохраниВГлоб(*this, "icondes-ctrl");
	Массив<ImlImage> m;
	ВекторМап<Размер, Рисунок> exp;
	Ткст folder = дайПапкуФайла(filename);
	for(int i = 0; i < дайСчёт(); i++) {
		ImlImage& c = m.добавь();
		c.имя = дайИмя(i);
		c.image = GetImage(i);
		c.exp = GetExport(i);
		c.flags = GetFlags(i);
		if(c.exp) {
			Размер sz = c.image.дайРазм();
			exp.дайДобавь(sz) = c.image;
			PNGEncoder png;
			сохраниИзменёнФайл(приставьИмяф(folder, Ткст().конкат() << "icon" << sz.cx << 'x' << sz.cy << ".png"),
			                png.SaveString(c.image));
		}
	}
	Ткст d = SaveIml(m, формат, EOL);
	if(!SaveChangedFileFinish(filename, d))
		return;
	filetime = дайВремяф(filename);
	if(exp.дайСчёт())
		сохраниИзменёнФайл(приставьИмяф(folder, "icon.ico"), WriteIcon(exp.дайЗначения()));
}

void IdeIconDes::ToolEx(Бар& bar)
{
	bar.Separator();
	if(!IsSingleMode())
		bar.добавь("Свойства файла..", IconDesImg::FileProperties(), THISBACK(FileProperties));
}

void IdeIconDes::FileProperties()
{
	WithFilePropertiesLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, "Свойства файла");
	dlg.формат <<= формат;
	if(dlg.пуск() == IDOK)
		формат = ~dlg.формат;
}

void IdeIconDes::сериализуй(Поток& s)
{
	сериализуйНастройки(s);
}

Ткст IdeIconDes::дайИмяф() const
{
	return filename;
}

void IdeIconDes::EditMenu(Бар& bar)
{
	EditBar(bar);
	ToolEx(bar);
	bar.добавь(IsSingleMode() ? "PNG" : "Список", THISBACK(ListMenu));
	bar.добавь("Выбор", THISBACK(SelectBar));
	bar.добавь("Рисунок", THISBACK(ImageBar));
	bar.добавь("Нарисовать", THISBACK(DrawBar));
	bar.добавь("Настройки", THISBACK(SettingBar));
}
