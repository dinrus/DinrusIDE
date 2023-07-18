#include "DinrusIDE2.h"

bool ReFormatJSON_XML(String& text, bool xml)
{
	if(xml) {
		try {
			XmlNode n = ParseXML(text);
			text = AsXML(n);
		}
		catch(XmlError) {
			Exclamation("Ошибка при передаче XML!");
			return false;
		}
	}
	else {
		Value v = ParseJSON(text);
		if(v.IsError()) {
			Exclamation("Ошибка при передаче JSON!");
			return false;
		}
		text = AsJSON(v, true);
	}
	return true;
}

void Ide::FormatJSON_XML(bool xml)
{
	int l, h;
	bool sel = editor.GetSelection(l, h);
	if((sel ? h - l : editor.GetLength()) > 75*1024*1024) {
		Exclamation("Слишком велик для реформатирования");
		return;
	}
	String text;
	if(sel)
		text = editor.GetSelection();
	else {
		SaveFile();
		text = LoadFile(editfile);
	}
	if(!ReFormatJSON_XML(text, xml))
		return;
	editor.NextUndo();
	if(sel) {
		editor.Remove(l, h - l);
		editor.SetSelection(l, l + editor.Insert(l, text));
	}
	else {
		editor.Remove(0, editor.GetLength());
		editor.Insert(0, text);
	}
}

void Ide::FormatJSON()
{
	FormatJSON_XML(false);
}

void Ide::FormatXML()
{
	FormatJSON_XML(true);
}

void Ide::FormatJSON_XML_File(bool xml)
{
	if(IsNull(editfile))
		return;
	if(GetFileLength(editfile) >= 75*1024*1024)
		Exclamation("Слишком велик для реформатирования");
	SaveFile();
	String text = LoadFile(editfile);
	if(!ReFormatJSON_XML(text, xml))
		return;
	if(PromptYesNo("Переписать \1" + editfile + "\1 с реформатированием " + (xml ? "XML" : "JSON") + "?")) {
		Upp::SaveFile(editfile, text);
		EditAsText();
	}
}