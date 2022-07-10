#include "Browser.h"

void ReferenceDlg::EnterItem()
{
	reference <<= GetCodeRef();
}

void ReferenceDlg::EnterItemOk()
{
	if(элт.курсор_ли()) {
		EnterItem();
		Break(IDOK);
	}
}

void ReferenceDlg::уст(const Ткст& s)
{
	идиК(s, Null);
	reference <<= s;
}

ReferenceDlg::ReferenceDlg()
{
	CtrlLayoutOKCancel(*this, "Справочник");
	Breaker(classlist, IDYES);
	элт.WhenEnterRow = THISBACK(EnterItem);
	элт.WhenLeftDouble = THISBACK(EnterItemOk);
	Sizeable().Zoomable();
	Иконка(TopicImg::Topic());
	дисплей.showtopic = true;
	грузи();
}
