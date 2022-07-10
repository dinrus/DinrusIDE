#include "Builders.h"

#ifdef PLATFORM_OSX

#include <Draw/Draw.h>
#include <plugin/png/png.h>

void GccBuilder::CocoaAppBundle()
{
	if(!естьФлаг("OSX") || !естьФлаг("GUI"))
		return;

	Ткст icns = дайПапкуФайла(дайПапкуФайла(target)) + "/Resources/icons.icns";
	RealizePath(icns);
	Время icns_tm = Nvl(дайФВремя(icns), Время::наименьш());
	bool convert_icons = false;

	SortedVectorMap<int, Рисунок> imgs;
	for(ФайлПоиск ff(PackageDirectory(mainpackage) + "/icon*.png"); ff; ff.следщ()) {
		Рисунок m = StreamRaster::LoadFileAny(ff.дайПуть());
		Размер sz = m.дайРазм();
		if(sz.cx == sz.cy) {
			imgs.добавь(sz.cx, m);
			PutVerbose("Found icon " << ff.дайИмя());
			if((Время)ff.дайВремяПоследнЗаписи() >= icns_tm)
				convert_icons = true;
		}
	}
	if(imgs.дайСчёт() && convert_icons) {
		Ткст icons = приставьИмяф(outdir, "icons.iconset");
		реализуйДир(icons);
		вКонсоль("Exporting bundle icons to " + icons);
		
		for(Ткст фн : {
			"icon_16x16.png",
			"icon_16x16@2x.png",
			"icon_32x32.png",
			"icon_32x32@2x.png",
			"icon_128x128.png",
			"icon_128x128@2x.png",
			"icon_256x256.png",
			"icon_256x256@2x.png",
			"icon_512x512.png",
			"icon_512x512@2x.png",
		}) {
			int n = atoi(~фн + strlen("icon_"));
			if(фн.найди("@2x") >= 0)
				n *= 2;
			int q = imgs.найдиНижнГран(n);
			Рисунок img = q >= 0 && q < imgs.дайСчёт() ? imgs[q] : imgs[imgs.дайСчёт() - 1];
			PutVerbose(Ткст() << "Exporting " << фн << " from "
			                    << img.дайРазм().cx << "x" << img.дайРазм().cx);
			PNGEncoder().сохраниФайл(приставьИмяф(icons, фн), Rescale(img, n, n));
		}

		выполни(Ткст() << "iconutil --convert icns --output " << icns << " " << icons);
	}

	if(пусто_ли(Info_plist)) {
		Info_plist
			<< "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			<< "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
			<< "<plist version=\"1.0\">\n"
			<< "<dict>\n"
			<< "    <ключ>CFBundleExecutable</ключ>\n"
			<< "    <string>" << дайИмяф(target) << "</string>\n"
			<< "    <ключ>NSHighResolutionCapable</ключ>\n"
		    << "    <string>True</string>\n"
		    << "	<ключ>LSMinimumSystemVersion</ключ>\n"
			<< "    <string>10.13.0</string>\n"
		;
		if(imgs.дайСчёт())
			Info_plist
				<< "    <ключ>CFBundleIconFile</ключ>\n"
				<< "	<string>icons.icns</string>\n"
			;
		Info_plist
			<< "</dict>\n"
			<< "</plist>\n"
		;
	}
	Ткст Info_plist_path = дайПапкуФайла(дайПапкуФайла(target)) + "/Info.plist";
	if(загрузиФайл(Info_plist_path) != Info_plist) {
		if(файлЕсть(Info_plist_path))
			выполни("defaults delete " + Info_plist_path); // Force MacOS to reload plist
		сохраниФайл(Info_plist_path, Info_plist);
		вКонсоль("Saving " << Info_plist_path);
	}
}

#endif