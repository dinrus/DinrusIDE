#include "DinrusIDE.h"

bool CheckLicense()
{
#ifdef PLATFORM_MACOS
	Ткст chkfile = дайПапкуФайла(дайПапкуПрил()) + "/license.chk";
#else
	Ткст chkfile = дайФайлИзПапкиИсп("license.chk");
#endif
	if(!файлЕсть(chkfile))
		return true;
	ShowSplash();
	Ктрл::обработайСобытия();
	Sleep(2000);
	HideSplash();
	Ктрл::обработайСобытия();
	WithLicenseLayout<ТопОкно> d;
	CtrlLayoutOKCancel(d, "Лицензионное соглашение");
	d.license = GetTopic("DinrusIDE/app/BSD_en-us").text;
	d.license.Margins(4);
	d.license.SetZoom(Zoom(Zy(18), 100));
	d.ActiveFocus(d.license);
	if(d.пуск() != IDOK)
		return false;
	DeleteFile(chkfile);
	return true;
}

#ifdef PLATFORM_POSIX

bool Install(bool& hasvars)
{
	Ткст ass = GetConfigFolder();

	Ткст uppsrc, bazaar;
	
	Ткст out = GetDefaultUppOut();
	
	int pass = 0;

	auto MakeAssembly = [&](Ткст b) {
		Ткст имя = дайТитулф(b);
		Ткст a = ass + '/' + имя + ".var";
		if(pass == 0) {
			if(имя == "uppsrc" && пусто_ли(uppsrc))
				uppsrc = b;
		}
		else {
			if(имя != "uppsrc")
				b << ';' << uppsrc;
			if(!файлЕсть(a))
				сохраниФайл(a,
					"РНЦП = " + какТкстСи(b) + ";\r\n"
					"OUTPUT = " + какТкстСи(out) + ";\r\n"
				);
		}
	};
	
	auto скан = [&](Ткст p) {
		for(ФайлПоиск ff(p); ff; ff.следщ())
			if(ff.папка_ли()) {
				Ткст path = ff.дайПуть();
				for(ФайлПоиск ff2(path + "/*"); ff2; ff2.следщ()) {
					Ткст p = ff2.дайПуть();
					Ткст upp = загрузиФайл(p + '/' + дайТитулф(p) + ".upp");
					if(upp.найди("mainconfig") >= 0) {
						MakeAssembly(path);
						hasvars = true;
						break;
					}
				}
			}
	};

#ifdef PLATFORM_COCOA
	Ткст idir = дайПапкуФайла(дайПапкуПрил());
	Ткст myapps = дирЕсть(idir + "/uppsrc") ? idir + "/MyApps"  : дайФайлИзДомПапки("MyApps");
#else
	Ткст myapps = (дирЕсть(дайФайлИзПапкиИсп("uppsrc")) ? дайФайлИзПапкиИсп  : дайФайлИзДомПапки)("MyApps");
#endif

	for(pass = 0; pass < 2; pass++) {
		if(pass)
			MakeAssembly(myapps);
	#ifdef PLATFORM_COCOA
		скан(idir + "/uppsrc");
		скан(idir + "/*");
	#endif
		скан(дайПапкуИсп() + "/uppsrc");
		скан(дайПапкуИсп() + "/*");
		скан(дайФайлИзДомПапки("upp.src/uppsrc"));
		скан(дайФайлИзДомПапки("upp.src/*"));
		скан(дайФайлИзДомПапки("upp/uppsrc"));
		скан(дайФайлИзДомПапки("upp/*"));
		скан(дайФайлИзДомПапки("*"));
		for(ФайлПоиск ff(дайФайлИзДомПапки("*")); ff; ff.следщ())
			if(ff.папка_ли())
				скан(ff.дайПуть() + "/*");
	}

	CreateBuildMethods();
	return true;
}

#endif
