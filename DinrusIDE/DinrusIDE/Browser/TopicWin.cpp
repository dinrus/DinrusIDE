#include "Browser.h"

#define IMAGEFILE <DinrusIDE/Browser/Topic.iml>
#define IMAGECLASS TopicImg
#include <Draw/iml_source.h>

TopicEditor::TopicEditor()
{
	SizePos();
	editor.вставьФрейм(0, фреймВерхнСепаратора());
	editor.вставьФрейм(1, tool);
	SetBar();
	Шрифт tf = StdFont().Bold();
	int dcy = EditField::GetStdHeight(tf);
	title.устШрифт(tf);
	right.добавь(title.HSizePos(2, 2).TopPos(0, dcy));
	title.Подсказка("Титул тематики");
	title.NullText("Титул тематики", tf().Italic(), SColorDisabled());
	right.добавь(editor.VSizePos(dcy + 4, 0).HSizePos());
	добавь(left_right.гориз(topics_parent, right));
	left_right.устПоз(1200);
	
	const auto edit_string_cy = EditString::GetStdHeight();
	topics_parent.добавь(topics_search.HSizePos().TopPos(0, edit_string_cy));
	topics_parent.добавь(topics_list.HSizePos().VSizePos(edit_string_cy, 0));
	
	topics_list.NoRoundSize().Columns(1);

	topics_list.WhenSel = THISBACK(TopicCursor);
	topics_list.WhenBar = THISBACK(TopicMenu);
	topics_list.NoWantFocus();
	
	topics_search.NullText(Ткст(t_("Поиск")) + " " + "(Ктрл+Alt+F)");
	topics_search.WhenAction = THISBACK(OnSearch);
	topics_search.устФильтр(CharFilterToUpper);
	
	editor.устСтраницу(TopicPage());
	editor.WhenRefreshBar = THISBACK(SetBar);
	editor.WhenHyperlink = THISBACK(Hyperlink);

	editor.откл();
	title.откл();

	lastlang = LNG_ENGLISH;

	allfonts = false;

	editor.WhenLabel = THISBACK(Надпись);
	
	грузиИзГлоба(*this, "topic-editor");
}

TopicEditor::~TopicEditor()
{
	сохраниВГлоб(*this, "topic-editor");
	слей();
}

ИНИЦБЛОК {
	региструйГлобКонфиг("topic-editor");
}

void TopicEditor::сериализуй(Поток& s)
{
	int version = 0;
	s / version;
	editor.сериализуйНастройки(s);
	s % left_right;
	topics_list.сериализуйНастройки(s);
	s % allfonts;
	SyncFonts();
}

void TopicEditor::SerializeEditPos(Поток& s)
{
	int version = 0;
	s / version;
	if(s.сохраняется()) {
		for(int i = 0; i < editstate.дайСчёт(); i++) {
			Ткст фн = editstate.дайКлюч(i);
			if(!пусто_ли(фн) && файлЕсть(фн)) {
				s % фн;
				s % editstate[i].pos;
			}
		}
		Ткст empty;
		s % empty;
	}
	else
		for(;;) {
			Ткст фн;
			s % фн;
			if(пусто_ли(фн))
				break;
			s % editstate.дайДобавь(фн).pos;
		}
	s % grouptopic;
	s % lastlang;
	s % lasttemplate;
}

void TopicEditor::ExportPdf()
{
	FileSel fs;
	fs.Type("Файлы PDF", "*.pdf")
	  .AllFilesType()
	  .DefaultExt("pdf");
	if(!fs.ExecuteSaveAs("Выходной файл PDF"))
		return;
	Размер page = Размер(3968, 6074);
	PdfDraw pdf(page + 400);
	::Print(pdf, editor.дай(), page);
	сохраниФайл(~fs, pdf.финиш());
}

void TopicEditor::ExportGroupPdf()
{
	Ткст dir = SelectDirectory();
	if(пусто_ли(dir))
		return;
	SaveTopic();
	ФайлПоиск ff(приставьИмяф(grouppath, "*.tpp"));
	while(ff) {
		Topic t = ReadTopic(загрузиФайл(приставьИмяф(grouppath, ff.дайИмя())));
		if(!t.text.проц_ли()) {
			Размер page = Размер(3968, 6074);
			PdfDraw pdf(page + 400);
			::Print(pdf, ParseQTF(t.text), page);
			Ткст pdfdata = pdf.финиш();
			Ткст path = приставьИмяф(dir, дайТитулф(ff.дайИмя()) + ".pdf");
			if(загрузиФайл(path) != pdfdata)
				сохраниФайл(path, pdfdata);
		}
		ff.следщ();
	}
}

void TopicEditor::ExportHTML()
{
	Ткст path = SelectFileSaveAs("Файлы HTML\t*.html\nВсе файлы\t*.*");
	if(пусто_ли(path))
		return;
	Индекс<Ткст> css;
	Ткст html = EncodeHtml(editor.дай(), css,
	                         ВекторМап<Ткст, Ткст>(), ВекторМап<Ткст, Ткст>(),
	                         дайПапкуФайла(path));
	сохраниФайл(path, MakeHtml((Ткст)~title, AsCss(css), html));
}

void TopicEditor::ExportGroupHTML()
{
	Ткст dir = SelectDirectory();
	if(пусто_ли(dir))
		return;
	SaveTopic();
	ФайлПоиск ff(приставьИмяф(grouppath, "*.tpp"));
	while(ff) {
		Topic t = ReadTopic(загрузиФайл(приставьИмяф(grouppath, ff.дайИмя())));
		if(!t.text.проц_ли()) {
			Индекс<Ткст> css;
			Ткст html = EncodeHtml(ParseQTF(t.text), css,
			                         ВекторМап<Ткст, Ткст>(), ВекторМап<Ткст, Ткст>(),
			                         dir);
			html = MakeHtml(t.title, AsCss(css), html);
			Ткст path = приставьИмяф(dir, дайТитулф(ff.дайИмя()) + ".html");
			if(загрузиФайл(path) != html)
				сохраниФайл(path, html);
		}
		ff.следщ();
	}
}

void TopicEditor::Print()
{
	РНЦП::Print(editor.дай(), Размер(3968, 6074), 0);
}

void TopicEditor::SyncFonts()
{
	Вектор<int> ff;
	ff.добавь(Шрифт::ARIAL);
	ff.добавь(Шрифт::ROMAN);
	ff.добавь(Шрифт::COURIER);
	if(allfonts)
		for(int i = Шрифт::COURIER + 1; i < Шрифт::GetFaceCount(); i++)
			if(Шрифт::GetFaceInfo(i) & Шрифт::SCALEABLE)
				ff.добавь(i);
	editor.FontFaces(ff);
}

void TopicEditor::AllFonts()
{
	allfonts = !allfonts;
	SyncFonts();
}

void TopicEditor::TopicMenu(Бар& bar)
{
	if(пусто_ли(grouppath))
		return;
	bar.добавь("Новая тематика..", THISBACK(NewTopic))
	   .Ключ(K_CTRL_N)
#ifndef PLATFORM_COCOA
	   .Ключ(K_ALT_INSERT)
#endif
	;
	bar.добавь(topics_list.курсор_ли(), "Переместить тематику..", THISBACK(MoveTopic));
	bar.добавь(topics_list.курсор_ли(), "Удалить тематику", THISBACK(RemoveTopic))
	   .Ключ(K_ALT_DELETE);
	bar.Separator();
	bar.добавь(topics_list.курсор_ли() && дайТитулф(topicpath) != "$.tpp",
	        "Сохранить как шаблон..", THISBACK(SaveAsTemplate));
	bar.добавь(topics_list.курсор_ли(), "Применить щаблонный stylesheet..", THISBACK(ApplyStylesheet));
	bar.добавь("Применитьь шаблонный stylesheet к группе..", THISBACK(ApplyStylesheetGroup));
	editor.StyleKeysTool(bar);
}

void TopicEditor::FileBar(Бар& bar)
{
	if(!пусто_ли(grouppath)) {
		TopicMenu(bar);
		bar.Separator();
	}
	bar.добавь("Печать", CtrlImg::print(), THISBACK(Print))
	   .Ключ(K_CTRL_P);
	bar.добавь("Экспорт в PDF..", THISBACK(ExportPdf));
	bar.добавь("Экспорт группы  PDF..", THISBACK(ExportGroupPdf));
	bar.добавь("Экспорт в HTML..", THISBACK(ExportHTML));
	bar.добавь("Экспорт группы в HTML..", THISBACK(ExportGroupHTML));
}

void TopicEditor::EditMenu(Бар& bar)
{
	FileBar(bar);
	bar.Separator();
	editor.CutTool(bar);
	editor.CopyTool(bar);
	editor.PasteTool(bar);
	editor.InsertImageTool(bar);
	bar.Separator();
	editor.UndoTool(bar);
	editor.RedoTool(bar);
	bar.Separator();
	editor.FindReplaceTool(bar);
	bar.Separator();
	bar.добавь("Все шрифты", THISBACK(AllFonts))
	   .Check(allfonts);
	bar.Separator();
	bar.добавь("Таблица", THISBACK(TableMenu));
}

void TopicEditor::FormatMenu(Бар& bar)
{
	editor.StyleTool(bar);
	bar.Separator();
	editor.FontTools(bar);
	bar.Separator();
	editor.InkTool(bar);
	editor.PaperTool(bar);
	bar.Separator();
	editor.LanguageTool(bar);
	editor.SpellCheckTool(bar);
	bar.Separator();
	editor.IndexEntryTool(bar);
	bar.Break();
	editor.HyperlinkTool(bar, Zx(1300), K_CTRL_H);
	bar.Separator();
	editor.ParaTools(bar);
}

void TopicEditor::TableMenu(Бар& bar)
{
	editor.TableTools(bar);
}

void CreateTopic(const char *фн, int lang, const Ткст& ss)
{
	сохраниФайл(фн,  WriteTopic("", ParseQTF(ss + "[{_}%" + LNGAsText(lang) + " ")));
}

bool TopicEditor::NewTopicEx(const Ткст& iname, const Ткст& create)
{
	TopicDlg<WithNewTopicLayout<ТопОкно> > d("Новая тематика");
	d.lang <<= lastlang;
	if(iname.дайСчёт()) {
		int i = 0;
		do {
			Ткст n = iname;
			if(i)
				n << i;
			d.topic <<= n;
			i++;
		}
		while(файлЕсть(приставьИмяф(grouppath, d.дайИмя())));
	}

	Вектор<Ткст> path, имя;
	ListTemplates(path, имя);
	d.tmpl.добавь(Null, "<none>");
	for(int i = 0; i < path.дайСчёт(); i++)
		d.tmpl.добавь(path[i], имя[i]);

	if(d.tmpl.HasKey(lasttemplate))
		d.tmpl <<= lasttemplate;
	else
		d.tmpl.идиВНач();

	d.ActiveFocus(d.topic);
	Ткст фн;
	for(;;) {
		if(d.пуск() != IDOK)
			return false;
		фн = приставьИмяф(grouppath, d.дайИмя());
		if(!файлЕсть(фн))
			break;
		if(PromptYesNo("Тематика уже существует.&Переписать её?"))
			break;
	}
	lasttemplate = ~d.tmpl;
	lastlang = ~d.lang;
	CreateTopic(фн, ~d.lang, пусто_ли(~d.tmpl) ? Ткст() : ReadTopic(загрузиФайл((Ткст)~d.tmpl)).text);
	слей();
	открой(grouppath);
	грузи(фн);
	SaveInc();
	topics_list.FindSetCursor(дайТитулф(фн));
	editor.устФокус();
	serial++;
	if(create.дайСчёт())
		InsertNew(create);
	return true;
}

void TopicEditor::NewTopic()
{
	NewTopicEx(Null, Null);
}

void TopicEditor::RemoveTopic()
{
	if(!topics_list.курсор_ли() ||
	   !PromptYesNo("Удалить тематику [* " + DeQtf(topics_list.GetCurrentName()) + "] ?"))
		return;
	Ткст p = GetCurrentTopicPath();
	int q = topics_list.дайКурсор();
	слей();
	DeleteFile(p);
	открой(grouppath);
	SaveInc();
	topics_list.устКурсор(q);
	if(q >= 0)
		editor.устФокус();
	InvalidateTopicInfoPath(p);
}

void TopicEditor::SaveAsTemplate()
{
	TopicDlg<WithSaveTemplateLayout<ТопОкно> > d("Сохранить как шаблон");
	d.lang <<= lastlang;
	Вектор<Ткст> ud = GetUppDirs();
	Ткст p = GetCurrentTopicPath();
	for(int i = 0; i < ud.дайСчёт(); i++) {
		d.nest.добавь(ud[i]);
		if(p.начинаетсяС(ud[i]))
			d.nest.SetIndex(i);
	}
	if(d.nest.дайИндекс() < 0)
		d.nest.идиВНач();
	if(d.выполни() != IDOK || пусто_ли(~d.nest))
		return;
	сохраниФайл(приставьИмяф(приставьИмяф(~d.nest, "_.tpp"), d.дайИмя()),
	         WriteTopic((Ткст)~title, editor.дай()));
}

void TopicEditor::SetBar()
{
	tool.уст(THISBACK(MainTool));
}

bool TopicEditor::Ключ(dword ключ, int cnt)
{
	switch(ключ) {
	case K_ALT_F10:
		FixTopic();
		return true;
	case K_ALT_UP:
		return topics_list.Ключ(K_UP, 0);
	case K_ALT_DOWN:
		return topics_list.Ключ(K_DOWN, 0);
	case K_CTRL|K_ALT_F:
		topics_search.устФокус();
		return true;
	}
	return false;
}
