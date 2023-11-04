#include "IconDes.h"

#include <plugin/bmp/bmp.h>
#include <plugin/png/png.h>

bool IdeIconDes::Load(const char *_filename)
{
	Clear();
	filename = _filename;
	filetime = FileGetTime(filename);
	Array<ImlImage> iml;
	int f;
	String s = LoadFile(filename);
	EOL = GetLineEndings(s);
	if(!LoadIml(s, iml, f))
		return false;
	format = f;
	for(const ImlImage& m : iml)
		AddImage(m.name, m.image, m.exp).flags = m.flags;
	return true;
}

void IdeIconDes::Save()
{
	if(format == 1) {
		for(int i = 0; i < GetCount(); i++) {
			Image m = GetImage(i);
			Point p = m.Get2ndSpot();
			if(m.GetKind() == IMAGE_ALPHA || p.x || p.y) {
				if(PromptYesNo("Унаследованный формат файла не поддерживает рисунки "
				               "с полным альфа-каналом или 2-м хотспотом - "
				               "эта информация будет потеряна.&"
				               "Хотите преобразовать этот файл в новый формат?")) {
					format = 0;
				}
				break;
			}
		}
	}
	StoreToGlobal(*this, "icondes-ctrl");
	Array<ImlImage> m;
	VectorMap<Size, Image> exp;
	String folder = GetFileFolder(filename);
	for(int i = 0; i < GetCount(); i++) {
		ImlImage& c = m.Add();
		c.name = GetName(i);
		c.image = GetImage(i);
		c.exp = GetExport(i);
		c.flags = GetFlags(i);
		if(c.exp) {
			Size sz = c.image.GetSize();
			exp.GetAdd(sz) = c.image;
			PNGEncoder png;
			SaveChangedFile(AppendFileName(folder, String().Cat() << "icon" << sz.cx << 'x' << sz.cy << ".png"),
			                png.SaveString(c.image));
		}
	}
	String d = SaveIml(m, format, EOL);
	if(!SaveChangedFileFinish(filename, d))
		return;
	filetime = FileGetTime(filename);
	if(exp.GetCount())
		SaveChangedFile(AppendFileName(folder, "icon.ico"), WriteIcon(exp.GetValues()));
}

void IdeIconDes::ToolEx(Bar& bar)
{
	bar.Separator();
	if(!IsSingleMode())
		bar.Add("Свойства файла..", IconDesImg::FileProperties(), THISBACK(FileProperties));
}

void IdeIconDes::FileProperties()
{
	WithFilePropertiesLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Свойства файла");
	dlg.format <<= format;
	if(dlg.Run() == IDOK)
		format = ~dlg.format;
}

void IdeIconDes::Serialize(Stream& s)
{
	SerializeSettings(s);
}

String IdeIconDes::GetFileName() const
{
	return filename;
}

void IdeIconDes::EditMenu(Bar& bar)
{
	EditBar(bar);
	ToolEx(bar);
	bar.Add(IsSingleMode() ? "PNG" : "Список", THISBACK(ListMenu));
	bar.Add("Выбор", THISBACK(SelectBar));
	bar.Add("Рисунок", THISBACK(ImageBar));
	bar.Add("Нарисовать", THISBACK(DrawBar));
	bar.Add("Настройки", THISBACK(SettingBar));
}