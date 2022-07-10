#include "Common.h"

#define IMAGECLASS IdeCommonImg
#define IMAGEFILE  <DinrusIDE/Common/common.iml>
#include <Draw/iml_source.h>

void выборФайла(Ктрл& t, Кнопка& b, Событие<> ev, const char *types, bool saveas)
{
	b.Подсказка("Выберите файл..");
	b.устРисунок(CtrlImg::File());
	Ткст p = types ? types : "Все файлы (*.*)\t*.*";
	b << [=, &t] {
		Ткст s = (saveas ? SelectFileSaveAs : SelectFileOpen)(p);
		if(s.дайСчёт()) {
			t <<= s;
			ev();
		}
	};
}

void выборФайлаСохраниКак(Ктрл& t, Кнопка& b, Событие<> ev, const char *types)
{
	return выборФайла(t, b, ev, types, true);
}

void выборФайлаОткрой(Ктрл& t, Кнопка& b, Событие<> ev, const char *types)
{
	return выборФайла(t, b, ev, types, false);
}

void выборФайлаСохраниКак(Ктрл& t, Кнопка& b, const char *types)
{
	return выборФайла(t, b, Null, types, true);
}

void выборФайлаОткрой(Ктрл& t, Кнопка& b, const char *types)
{
	return выборФайла(t, b, Null, types, false);
}

void выборДир(Ктрл& t, Кнопка& b)
{
	b.Подсказка("Выберите папку..");
	b.устРисунок(CtrlImg::Dir());
	b << [&] {
		Ткст s = SelectDirectory();
		if(s.дайСчёт()) {
			t <<= s;
			t.WhenAction();
		}
	};
}

void выбДир(EditField& f, ФреймПраво<Кнопка>& b)
{
	b.устШирину(DPI(20));
	f.вставьФрейм(0, b);
	f.вставьФрейм(1, фреймПравПроёма());
	выборДир(f, b);
}

void IdeFileIcon0(bool dir, const Ткст& filename, Рисунок& img)
{
	if(dir) return;
	Ткст ext = впроп(дайРасшф(filename));
	for(int i = 0; i < дайСчётМодульИСР(); i++) {
		Рисунок m = дайМодульИСР(i).FileIcon(filename);
		if(!пусто_ли(m)) {
			img = m;
			break;
		}
	}
	
	if(ext == ".html")
		img = IdeCommonImg::html();
	else
	if(ext == ".css")
		img = IdeCommonImg::css();
	else
	if(ext == ".witz")
		img = IdeCommonImg::witz();
	else
	if(ext == ".js")
		img = IdeCommonImg::js();
	else
	if(ext == ".json")
		img = IdeCommonImg::json();
	else
	if(ext == ".java" || ext == ".class")
		img = IdeCommonImg::java();
	else
	if(ext == ".log")
		img = IdeCommonImg::Log();
	else
	if(ext == ".xml" || ext == ".xsd")
		img = IdeCommonImg::xml();
	else
	if(ext == ".diff" || ext == ".patch")
		img = IdeCommonImg::diff();
	else
	if(ext == ".py" || ext == ".pyc" || ext == ".pyd" || ext == ".pyo")
		img = IdeCommonImg::Python();
	else
	if(ext == ".usc")
		img = IdeCommonImg::Script();
	else
	if(ext == ".lng" || ext == ".lngj" || ext == ".t" || ext == ".jt")
		img = IdeCommonImg::Language();
	else
	if(ext == ".icpp")
		img = IdeCommonImg::ISource();
	else
	if(findarg(ext, ".cpp", ".cc", ".cxx", ".mm") >= 0)
		img = IdeCommonImg::Cpp();
	else
	if(findarg(ext, ".c", ".m") >= 0)
		img = IdeCommonImg::Source();
	else
	if(ext == ".h" || ext == ".hpp" || ext == ".hh" || ext == ".hxx")
		img = IdeCommonImg::Header();
	else
	if(ext == ".sch")
		img = IdeCommonImg::Sch();
	else
	if(ext == ".ddl")
		img = IdeCommonImg::Ddl();
	else
	if(ext == ".sql")
		img = IdeCommonImg::Sql();
	else
	if(filename == "Copying")
		img = IdeCommonImg::License();
	else
	if(filename == "main.conf")
		img = IdeCommonImg::MainConf();
	else
	if(ext == ".ключ")
		img = IdeCommonImg::keyboard();
	else
	if(ext == ".defs")
		img = IdeCommonImg::Defs();
}

struct sImageAdd : ImageMaker {
	Рисунок i1, i2;

	virtual Ткст Ключ() const;
	virtual Рисунок сделай() const;
};

Ткст sImageAdd::Ключ() const
{
	int64 a[2];
	a[0] = i1.GetSerialId();
	a[1] = i2.GetSerialId();
	return Ткст((const char *)&a, 2 * sizeof(int64));
}

Рисунок sImageAdd::сделай() const
{
	Рисунок dest = i1;
	Over(dest, Точка(0, 0), i2, i2.дайРазм());
	return dest;
}

Рисунок ImageOver(const Рисунок& back, const Рисунок& over)
{
	sImageAdd h;
	h.i1 = back;
	h.i2 = over;
	return MakeImage(h);
}

Рисунок IdeFileImage(const Ткст& filename, bool include_path, bool pch)
{
	Рисунок img = CtrlImg::File();
	IdeFileIcon0(false, filename, img);
	if(include_path)
		img = ImageOver(img, IdeCommonImg::IncludePath());
	if(pch)
		img = ImageOver(img, IdeCommonImg::Precompile());
	return img;
}

void IdeFileIcon(bool dir, const Ткст& filename, Рисунок& img)
{
	IdeFileIcon0(dir, filename, img);
}

void IdeFs(FileSel& fs)
{
	fs.WhenIcon = callback(IdeFileIcon);
	fs.AllFilesType();
	fs.Multi();
	fs.NoAsking();
	fs.ReadOnlyOption();
}

void SourceFs(FileSel& fs)
{
	fs.Type("Файлы C/C++ (*.cpp *.h *.hpp *.c *.C *.cc *.cxx *.icpp)", "*.cpp *.h *.hpp *.c *.C *.cc *.cxx *.icpp");
	fs.Type("Файлы Diff/Patch (*.diff *.patch)", "*.diff *.patch");
	fs.Type("Файлы Рисунок (*.iml)", "*.iml");
	fs.Type("Файлы Java (*.java)", "*.java");
	fs.Type("Файлы Json (*.json)", "*.json");
	fs.Type("Языковые файлы (*.lng)", "*.lng");
	fs.Type("Файлы разметки (*.lay)", "*.lay");
	fs.Type("Файлы Python (*.py *.pyc *.pyd *.pyo)", "*.py *.pyc *.pyd *.pyo");
	fs.Type("Файлы веб-разработки (*.html *.js *.css *.witz)", "*.html *.js *.css *.witz");
	fs.Type("Файлы Xml (*.xml *.xsd)", "*.xml *.xsd");
	fs.Type("Другие особые файлы (*.sch *.usc *.rc *.brc *.upt)", "*.sch *.usc *.rc *.brc *.upt");
	Ткст mask = "*.cpp *.h *.hpp *.c *.C *.cc *.cxx *.icpp *.diff *.patch *.lay *.py *.pyc *.pyd *.pyo *.iml *.java *.json *.lng *.sch *.usc *.rc *.brc *.upt *.html *.js *.css *.witz *.xml *.xsd *.qtf";
	fs.Type("Ысе исходники (" + mask + ")", mask);
	IdeFs(fs);
}

FileSel& AnySourceFs()
{
	static FileSel *fsp;
	if(!fsp) {
		static FileSel fs;
		SourceFs(fs);
		fsp = &fs;
	}
	fsp->Multi();
	return *fsp;
}

FileSel& AnyPackageFs()
{
	static FileSel fs;
	static bool b;
	if(!b) {
		fs.Type("Пакет Ultimate++ (*.upp)", "*.upp");
		fs.AllFilesType();
		b = true;
	}
	return fs;
}

FileSel& BasedSourceFs()
{
	static FileSel *fsp;
	if(!fsp) {
		static FileSel fs;
		SourceFs(fs);
		fsp = &fs;
	}
	fsp->Multi();
	return *fsp;
}

FileSel& OutputFs()
{
	static FileSel *fsp;
	if(!fsp) {
		static FileSel fs;
		fs.AllFilesType();
		fs.Type("Различное (*.log *.map *.ini *.sql)", "*.log *.map *.ini *.sql");
		fs.Type("Логи (*.log)", "*.log");
		fs.Type("вКарту файлы (*.map)", "*.map");
		fs.Type("Ини файлы (*.ini)", "*.ini");
		fs.Type("Сценарии SQL (*.sql)", "*.sql");
		IdeFs(fs);
		fsp = &fs;
	}
	fsp->Multi();
	return *fsp;
}

void ShellOpenFolder(const Ткст& dir)
{
	#if defined(PLATFORM_WIN32)
		запустиВебБраузер(dir);
	#elif __APPLE__
		Ткст tempDir = dir;
		tempDir.замени(" ", "\\ ");

		IGNORE_RESULT(
			system("open " + tempDir + " &")
		);
	#else
		Ткст tempDir = dir;
		tempDir.замени(" ", "\\ ");
		
		IGNORE_RESULT(
			system("xdg-open " + tempDir + " &")
		);
	#endif
}
