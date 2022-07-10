#include "DinrusIDE.h"

static const int MIN_LINE_NUMBER = 1;

class GoToLineDialog : public WithGoToLineLayout<ТопОкно> {
public:
	GoToLineDialog(int currentLine, int maxLine)
	{
		CtrlLayoutOKCancel(*this, t_("Перейти к строке"));
		
		lineInformation.устТекст(GenerateLineInfo(maxLine));
		lineEdit.мин(MIN_LINE_NUMBER).макс(maxLine);
		lineEdit.устФокус();
		lineEdit.устДанные(currentLine);
		lineEdit.выбериВсе();
	}
	
	int дайСтроку() const
	{
		return static_cast<int>(lineEdit.дайДанные());
	}
	
private:
	Ткст GenerateLineInfo(int maxLine)
	{
		ТкстПоток ss;
		
		ss << t_("Перейти к строке") << " (" << MIN_LINE_NUMBER << ", " << maxLine << "):";
		
		return ss.дайРез();
	}
};

void Иср::GoToLine()
{
	const int currentLine = editor.GetCurrentLine() + 1;
	
	GoToLineDialog dlg(currentLine, editor.дайСчётСтрок());
	if(dlg.выполни() != IDOK)
		return;
	
	const int newLine = dlg.дайСтроку();
	if (currentLine == newLine)
		return;
	
	AddHistory();
	editor.устКурсор(editor.дайПоз64(newLine - 1));
	editor.устФокус();
	AddHistory();
}
