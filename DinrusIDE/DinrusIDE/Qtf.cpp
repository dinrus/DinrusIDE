#include "DinrusIDE.h"

ИНИЦБЛОК {
	региструйГлобКонфиг("QTF-designer");
	региструйГлобКонфиг("QTF-designer-editor");
}

class QtfDlg : public ТопОкно {
	void устТекст();
	void очисть();
	void копируй();
	void Editor();
	void OnHelpLink(const Ткст& link);

public:
	Сплиттер leftSplit, mainSplit;
	WithQtfLayout<КтрлРодитель> qtfText;
	RichTextCtrl qtf;
	RichTextView help;

	void сериализуй(Поток& s);

	QtfDlg();
};

void QtfDlg::устТекст()
{
	qtf <<= ~qtfText.text;
}

void QtfDlg::очисть()
{
	qtfText.text.очисть();
	устТекст();
}

void QtfDlg::копируй()
{
	WriteClipboardText(~qtfText.text);
	Break();
}

struct QtfDlgEditor : ТопОкно {
	RichEditWithToolBar editor;

	void сериализуй(Поток& s);
	
	QtfDlgEditor();
};

void QtfDlgEditor::сериализуй(Поток& s)
{
	SerializePlacement(s);
}

QtfDlgEditor::QtfDlgEditor()
{
    добавь(editor.SizePos());
    Прям r = GetWorkArea();
    Sizeable().Zoomable();
    устПрям(0, 0, r.дайШирину() - 100, r.дайВысоту() - 100);
    устМинРазм(Размер(min(640, r.дайШирину() - 100), min(480, r.дайВысоту() - 100)));
    Титул("Редактор");
}

void QtfDlg::Editor()
{
	QtfDlgEditor dlg;
	грузиИзГлоба(dlg, "QTF-designer-editor");
	dlg.editor.SetQTF((Ткст)~qtfText.text);
	dlg.пуск();
	if(PromptYesNo("Использовать этот текст?")) {
		qtfText.text <<= AsQTF(dlg.editor.дай(),
		               CHARSET_UTF8, QTF_BODY|QTF_NOSTYLES|QTF_NOCHARSET|QTF_NOLANG);
		устТекст();
	}
	сохраниВГлоб(dlg, "QTF-designer-editor");
}

void QtfDlg::OnHelpLink(const Ткст& link)
{
	auto label_idx = link.найдирек("#");
	if (label_idx == -1) {
		return;
	}
	
	constexpr bool highlight = true;
	help.GotoLabel(link.середина(label_idx + 1), highlight);
}

QtfDlg::QtfDlg()
{
	Титул("QTF дизайнер");
	CtrlLayout(qtfText);
	qtfText.text << [=] { устТекст(); };
	qtf.устФрейм(ViewFrame());
	qtf.фон(SColorPaper);
	Sizeable().Zoomable();
	Прям r = GetWorkArea();
	устПрям(0, 0, r.дайШирину() - 100, r.дайВысоту() - 200);
	устМинРазм(Размер(min(640, r.дайШирину() - 100), min(480, r.дайВысоту() - 200)));

	help.Margins(Прям(12, 0, 12, 0));
	Ткст path = приставьИмяф(приставьИмяф(PackageDirectory("RichText"), "srcdoc.tpp"), "QTF_en-us.tpp");
	if(файлЕсть(path)) {
		help.SetQTF(ReadTopic(загрузиФайл(path)).text);
	}
	else {
		help <<= "[= &&&QTF документация не найдена";
	}
	help.WhenLink = [=] (const Ткст& link) { OnHelpLink(link); };

	qtfText.clear << [=] { очисть(); };
	qtfText.copy << [=] { копируй(); };
	qtfText.editor << [=] { Editor(); };

	leftSplit.верт(qtfText, qtf).устПоз(4000);
	mainSplit.гориз(leftSplit, help).устПоз(3500);
	добавь(mainSplit.HSizePosZ(4, 4).VSizePosZ(4, 4));
}

void QtfDlg::сериализуй(Поток& s)
{
	int version = 1;
	s / version;
	s % qtfText.text;
	SerializePlacement(s);
	if(version >= 1) {
		leftSplit.сериализуй(s);
		mainSplit.сериализуй(s);
	}
	устТекст();
}

void Иср::Qtf()
{
	QtfDlg dlg;
	грузиИзГлоба(dlg, "QTF-designer");
	int l, h;
	bool sel = editor.дайВыделение(l,h);
	if(qtfsel && sel) {
		dlg.qtfText.text <<= (~editor).вТкст().середина(l, h - l);
		dlg.qtfText.copy.устНадпись("Применить и закрыть");
		dlg.пуск();
		editor.удали(l, h - l);
		editor.вставь(l, (~dlg.qtfText.text).вТкст());
	}
	else {
		dlg.qtfText.copy.устНадпись("Копировать и закрыть");
		dlg.пуск();
	}
	сохраниВГлоб(dlg, "QTF-designer");
}
