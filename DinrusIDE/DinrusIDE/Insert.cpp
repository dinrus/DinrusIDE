#include "DinrusIDE.h"

class InsertColorDlg : public WithInsertColorLayout<ТопОкно> {
	typedef InsertColorDlg ИМЯ_КЛАССА;

	Ткст r[5];
	bool canceled = true;
	
	void синх();
	void выдели(int i);
	
	RGBACtrl rgbactrl;

public:
	Ткст result;

	InsertColorDlg();
	bool IsCanceled();
};

void InsertColorDlg::выдели(int i)
{
	result = r[i];
	canceled = false;
	Break(IDOK);
}

void InsertColorDlg::синх()
{
	КЗСА c0 = rgbactrl.дайЦвет();
	КЗСА c = rgbactrl.дай();
	r[0] = фмт("%d, %d, %d, %d", c.a, c.r, c.g, c.b);
	rgba.устНадпись(r[0]);
	r[1] = c.a == 255 ? фмт("Цвет(%d, %d, %d)", c.r, c.g, c.b)
	                  : фмт("%d * Цвет(%d, %d, %d)", c.a, c0.r, c0.g, c0.b);
	color.устНадпись(r[1]);
	r[2] = фмт("%02x%02x%02x%02x", c.a, c.r, c.g, c.b);
	ahex.устНадпись(r[2]);
	r[3] = фмт("%02x%02x%02x", c.r, c.g, c.b);
	hex.устНадпись(r[3]);
	r[4] = фмт("(%d.%d.%d)", c.r, c.g, c.b);
	qtf.устНадпись(r[4]);
}

InsertColorDlg::InsertColorDlg()
{
	CtrlLayoutCancel(*this, "Вставить цвет");
	rgbactrl <<= THISBACK(синх);
	rgba <<= THISBACK1(выдели, 0);
	color <<= THISBACK1(выдели, 1);
	ahex <<= THISBACK1(выдели, 2);
	hex <<= THISBACK1(выдели, 3);
	qtf <<= THISBACK1(выдели, 4);
	синх();

	int m = color.дайПоз().y.GetA();
	int cx = color.дайПоз().x.GetA();
	int cy = rgbactrl.дайВысоту(cx - 2 * m);
	Прям r = дайПрям();
	r.bottom = r.top + cy + 2 * m;
	rgbactrl.устПрям(m, m, cx - 2 * m, cy);
	добавь(rgbactrl);
	устМинРазм(r.дайРазм());
	устПрям(r);
}

bool InsertColorDlg::IsCanceled()
{
	return canceled;
}

void Иср::InsertColor()
{
	if(editor.толькочтен_ли())
		return;
	InsertColorDlg dlg;
	dlg.выполни();
	if (!dlg.IsCanceled())
		editor.Paste(dlg.result.вШТкст());
}

void Иср::InsertLay(const Ткст& фн)
{
	if(editor.толькочтен_ли())
		return;
	Ткст s;
	s << "#define LAYOUTFILE <" << фн << ">\n"
	  << "#include <CtrlCore/lay.h>\n";
	editor.Paste(s.вШТкст());
}

void Иср::InsertIml(const Пакет& pkg, const Ткст& фн, Ткст classname)
{
	if(editor.толькочтен_ли())
		return;
	if(!редактируйТекст(classname, "Вставить .iml include", "Img class"))
		return;
	Ткст h;
	h << "#define IMAGECLASS " << classname << "\n"
	  << "#define IMAGEFILE <" << фн << ">\n";
	editor.Paste((h + "#include <Draw/iml_header.h>\n").вШТкст());
	ClearClipboard();
	int q = фн.найдирек('.');
	if(q >= 0) {
		Ткст fn0 = фн.середина(0, q);

		Индекс<Ткст> done;
		auto Variant = [&](const char *add, const char *m) {
			if(done.найди(m) >= 0)
				return;
			Ткст фн = fn0 + add + ".iml";
			for(int i = 0; i < pkg.дайСчёт(); i++) {
				if(фн.заканчиваетсяНа('/' + pkg[i])) {
					h << "#define IMAGEFILE" << m << " <" << фн << ">\n";
					done.добавь(m);
					break;
				}
			}
		};
		
		Variant("HD_DARK", "_DARK_UHD");
		Variant("HD_DK", "_DARK_UHD");
		Variant("HDDK", "_DARK_UHD");
		Variant("HDK", "_DARK_UHD");

		Variant("DARK_UHD", "_DARK_UHD");
		Variant("DK_HD", "_DARK_UHD");
		Variant("DK_UHD", "_DARK_UHD");
		Variant("DKHD", "_DARK_UHD");
		Variant("DKUHD", "_DARK_UHD");
	
		Variant("HD", "_UHD");
		
		Variant("DK", "_DARK");
		Variant("DARK", "_DARK");
	}
	
	h << "#include <Draw/iml_source.h>\n";
	
	AppendClipboardText(h);
	PromptOK("Часть .cpp сохранена в буфер обмена");
}

void Иср::InsertText(const Ткст& text)
{
	if(editor.толькочтен_ли())
		return;
	editor.Paste(text.вШТкст());
}

Ткст SelectInsertFile()
{
	return SelectFileOpen(
		"Все файлы (*.*)\t*.*\n"
		"Графические (*.png *.bmp *.jpg *.jpeg *.gif *.ico *.svg)\t*.png *.bmp *.jpg *.jpeg *.gif *.ico *.svg\n"
		"Исходники (*.cpp *.h *.hpp *.c *.C *.cc *.cxx *.icpp *.diff *.patch *.lay *.py *.pyc *.pyd *.pyo *.iml *.java *.lng *.sch *.usc *.rc *.brc *.upt *.witz *.js)\t"
			"*.cpp *.h *.hpp *.c *.C *.cc *.cxx *.icpp *.diff *.patch *.lay *.py *.pyc *.pyd *.pyo *.iml *.java *.lng *.sch *.usc *.rc *.brc *.upt *.witz *.js\n"
		"Веб-файлы (*.js *.css *.html *.htm *.htmls)\t*.js *.css *.html *.htm *.htmls\n"
		"Данные (*.csv *.xml *.json)\t*.csv *.xml *.json\n"
		"Текстовые (*.txt *.log *.info)\t*.txt *.log *.info\n"
		"Документы (*.xlsx *.xls *.doc *.qtf *.odt *.ods *.pdf)\t*.xlsx *.xls *.doc *.qtf *.odt *.ods *.pdf\n"
		"Сжатые (*.zip *.7z *.gz *.xz)\t*.zip *.7z *.gz *.xz\n"
	);
}

void Иср::InsertFilePath(bool c)
{
	if(editor.толькочтен_ли())
		return;
	Ткст path = SelectInsertFile();
	path.замени("\\", "/");
	if(path.дайСчёт()) {
		if(c)
			path = какТкстСи(path);
		editor.Paste(path.вШТкст());
	}
}


void Иср::InsertAs(const Ткст& data)
{
	WithInsertAsLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, "Вставить данные");
	if(data.дайСчёт() > 20*1024)
		Exclamation("Данные слишком большие по размеру!&(Предел равен 20KB.)");
	Ткст f[6];
	f[0] = data;
	f[1] = кодируй64(data);
	f[2] = data;
	f[3] = LZ4Compress(data);
	f[4] = ZCompress(data);
	f[5] = LZMACompress(data);
	for(int i = 0; i < 6; i++)
		dlg.формат.устНадпись(i, dlg.формат.GetLabel(i) + " (" + какТкст(f[i].дайСчёт()) + ")");
	
	if(dlg.выполни() != IDOK)
		return;
	int i = ~dlg.формат;
	if(i < 0 || i >= 6)
		return;
	Ткст d = f[i];
	WriteClipboardText(какТкст(d.дайСчёт()));
	if(i == 0 || i == 1)
		editor.Paste(какТкстСи(d).вШТкст());
	else {
		for(int i = 0; i < d.дайСчёт(); i += 256) {
			int e = min(i + 256, d.дайСчёт());
			Ткст h;
			for(int j = i; j < e; j++)
				h << какТкст((int)(byte)d[j]) << ',';
			h << '\n';
			editor.Paste(h.вШТкст());
		}
	}
}

void Иср::InsertAs()
{
	if(editor.толькочтен_ли())
		return;
	Ткст txt = ReadClipboardText();
	if(txt.дайСчёт())
		InsertAs(txt);
}

void Иср::InsertFileBase64()
{
	if(editor.толькочтен_ли())
		return;
	Ткст path = SelectInsertFile();
	path.замени("\\", "/");
	if(path.дайСчёт()) {
		if(дайДлинуф(path) >= 20*1024) {
			Exclamation("Файл слишком большой!&(Предел равен 20KB.)");
			return;
		}
		InsertAs(загрузиФайл(path));
	}
}

void Иср::InsertMenu(Бар& bar)
{
	if(bar.IsScanKeys())
		return;
	bar.добавь("Вставить цвет..", THISBACK(InsertColor));
	int pi = GetPackageIndex();
	const РОбласть& wspc = роблИср();
	if(pi >= 0 && pi < wspc.дайСчёт()) {
		Ткст pn = wspc[pi];
		const Пакет& p = wspc.дайПакет(pi);
		int n = 0;
		for(int i = 0; i < p.дайСчёт() && n < 12; i++) {
			Ткст фн = p[i];
			Ткст ext = впроп(дайРасшф(фн));
			Ткст pp = pn + '/' + фн;
			if(ext == ".lay") {
				bar.добавь(фн + " include", THISBACK1(InsertLay, pp));
				n++;
			}
			if(ext == ".iml") {
				Ткст c = дайТитулф(фн);
				c.уст(0, взаг(c[0]));
				bar.добавь(фн + " include", [=] { InsertIml(роблИср().дайПакет(pi), pp, c.заканчиваетсяНа("Img") ? c : c + "Img"); });
				n++;
			}
			if(ext == ".tpp") {
				Ткст s;
				s << "#define TOPICFILE <" << pp << "/all.i>\n"
				  << "#include <Core/topic_group.h>\n";
				bar.добавь(фн + " include", THISBACK1(InsertText, s));
				n++;
			}
		}
	}
	bar.добавь("Вставить путь к файлу..", THISBACK1(InsertFilePath, false));
	bar.добавь("Вставить путь к файлу как строку Си..", THISBACK1(InsertFilePath, true));
	bar.добавь("Вставить буфер обмена как..", [=] { InsertAs(); });
	bar.добавь("Вставить файл как..", THISBACK(InsertFileBase64));
}

void Иср::InsertInclude(Бар& bar)
{
	if(bar.IsScanKeys()) // this takes time and contains no keys
		return;
	const РОбласть& w = GetIdeWorkspace();
	Ткст all;
	for(int i = 0; i < w.дайСчёт(); i++) {
		const Пакет& p = w.дайПакет(i);
		for(int j = 0; j < p.дайСчёт(); j++)
			if(findarg(впроп(дайРасшф(p[j])), ".h", ".hpp") >= 0) {
				Ткст h; h << "#include <" << w[i] << "/" << p[j] << '>';
				bar.добавь(h, THISBACK1(InsertText, h + '\n'));
				all << h << '\n';
				break;
			}
	}
	bar.добавь("Все #includes", THISBACK1(InsertText, all));
}

void Иср::ToggleWordwrap()
{
	RLOG("===========");
	RDUMPHEX((int)*(byte *)&wordwrap);
	RDUMP(wordwrap);
	wordwrap = !wordwrap;
	RDUMPHEX((int)*(byte *)&wordwrap);
	RDUMP(wordwrap);
	SetupEditor();
	RDUMP(wordwrap);
	RLOG(".........");
}

void Иср::EditorMenu(Бар& bar)
{
	InsertAdvanced(bar);
	bar.MenuSeparator();
	OnlineSearchMenu(bar);
    bar.добавь(IsClipboardAvailableText() && (editor.выделение_ли() || editor.дайДлину() < 1024*1024),
            "Сравнить с буфером обмена..", [=]() {
        ДиффДлг dlg;
        dlg.diff.left.удалиФрейм(dlg.p);
        dlg.diff.уст(ReadClipboardText(), editor.выделение_ли() ? editor.дайВыделение()
                                                               : editor.дай());
		dlg.Титул("Сравнить с буфером обмена");
        dlg.пуск();
    });
	bar.MenuSeparator();
	editor.StdBar(bar);
}
