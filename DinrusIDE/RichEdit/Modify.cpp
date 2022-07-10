#include "RichEdit.h"

namespace РНЦП {


void RichEdit::PasteFilter(RichText& txt, const Ткст&) { фильтруй(txt); }
void RichEdit::фильтруй(RichText& txt) {}

void BegSelFixRaw(RichText& text)
{
	RichPos p = text.GetRichPos(0, 1);
	ПРОВЕРЬ(p.table == 1);
	if(p.table != 1)
		return;
	RichPara::фмт fmt;
	text.InsertParaSpecial(1, true, fmt);
}

void BegSelUnFixRaw(RichText& text)
{
	ПРОВЕРЬ(text.дайДлину() > 0);
	RichPos p = text.GetRichPos(1, 1);
	ПРОВЕРЬ(p.table == 1);
	if(p.table != 1)
		return;
	text.RemoveParaSpecial(1, true);
}

void RichEdit::UndoBegSelFix::Apply(RichText& txt)
{
	BegSelUnFixRaw(txt);
}

Один<RichEdit::UndoRec> RichEdit::UndoBegSelFix::GetRedo(const RichText& txt)
{
	return сделайОдин<RichEdit::UndoBegSelUnFix>();
}

void RichEdit::UndoBegSelUnFix::Apply(RichText& text)
{
	BegSelFixRaw(text);
}

Один<RichEdit::UndoRec> RichEdit::UndoBegSelUnFix::GetRedo(const RichText& txt)
{
	return сделайОдин<RichEdit::UndoBegSelFix>();
}

bool RichEdit::BegSelTabFix(int& count)
{
	if(begtabsel) { // If selection starts with first table which is the first element in the text
		int c = cursor;
		AddUndo(сделайОдин<UndoBegSelFix>());
		BegSelFixRaw(text); // adds an empty paragraph at the start
		Move(0);
		Move(c + 1, true); // and changes the selection
		count++;
		begtabsel = false;
		return true;
	}
	return false;
}

void RichEdit::BegSelTabFixEnd(bool fix)
{ // removes empty paragraph added by BegSelTabFix
	if(fix && дайДлину() > 0) {
		int c = cursor;
		AddUndo(сделайОдин<UndoBegSelUnFix>());
		BegSelUnFixRaw(text);
		Move(0);
		Move(c - 1, true);
		begtabsel = true;
	}
}

bool RichEdit::InvalidRange(int l, int h)
{
	return !InSameTxt(text.GetRichPos(min(l, h)), text.GetRichPos(max(l, h)));
}

void RichEdit::AddUndo(Один<UndoRec>&& ur)
{
	redo.очисть();
	SetModify();
	modified = true;
	incundoserial = true;
	while(undo.дайСчёт() > undosteps)
		undo.сбросьГолову();
	found = false;
	ur->cursor = cursor;
	ur->serial = undoserial;
	undo.добавьХвост(pick(ur));
}

void RichEdit::SaveStylesUndo()
{
	AddUndo(сделайОдин<UndoStyles>(text));
}

void RichEdit::SaveStyleUndo(const Uuid& id)
{
	AddUndo(сделайОдин<UndoStyle>(text, id));
}

void RichEdit::SaveFormat(int pos, int count)
{
	Limit(pos, count);
	AddUndo(сделайОдин<UndoFormat>(text, pos, count));
}

void RichEdit::SaveFormat()
{
	int pos, count;
	if(выделение_ли()) {
		if(tablesel) {
			SaveTable(tablesel);
			return;
		}
		pos = min(cursor, anchor);
		count = абс(cursor - anchor);
	}
	else {
		pos = cursor;
		count = 0;
	}
	bool b = BegSelTabFix(count);
	SaveFormat(pos, count);
	BegSelTabFixEnd(b);
}

void RichEdit::Limit(int& pos, int& count)
{
	int h = pos + count;
	pos = min(дайДлину(), pos);
	count = min(дайДлину(), h) - pos;
}

void RichEdit::ModifyFormat(int pos, const RichText::FormatInfo& fi, int count)
{
	if(толькочтен_ли())
		return;
	bool b = BegSelTabFix(count);
	Limit(pos, count);
	SaveFormat(pos, count);
	text.ApplyFormatInfo(pos, fi, count);
	BegSelTabFixEnd(b);
}

void RichEdit::удали(int pos, int len, bool forward)
{
	if(толькочтен_ли())
		return;
	Limit(pos, len);
	if(InvalidRange(pos, pos + len))
		return;
	RichTxt::FormatInfo fi;
	if(forward)
		fi = text.GetFormatInfo(pos, 0);
	AddUndo(сделайОдин<UndoRemove>(text, pos, len));
	text.удали(pos, len);
	if(forward) {
		SaveFormat(pos, 0);
		text.ReplaceStyle(pos, fi.styleid);
		fi.paravalid &= ~RichText::STYLE;
		text.ApplyFormatInfo(pos, fi, 0);
	}
	SetModify();
	modified = true;
}

void RichEdit::вставь(int pos, const RichText& txt, bool typing)
{
	if(толькочтен_ли())
		return;
	Индекс<int> lng;
	for(int i = 0; i < language.дайСчёт(); i++)
		lng.добавь(language.дайКлюч(i));
	Вектор<int> lngn = txt.GetAllLanguages();
	for(int i = 0; i < lngn.дайСчёт(); i++)
		lng.найдиДобавь(lngn[i]);
	SetupLanguage(lng.подбериКлючи());
	int l = text.дайДлину();
	text.вставь(pos, txt);
	l = text.дайДлину() - l;
	SetModify();
	modified = true;
	if(undo.дайСчёт()) {
		UndoRec& u = undo.дайХвост();
		if(typing) {
			UndoInsert *ui = dynamic_cast<UndoInsert *>(&u);
			if(ui && ui->length > 0 && ui->typing && ui->pos + ui->length == pos) {
				ui->length += l;
				return;
			}
		}
	}
	AddUndo(сделайОдин<UndoInsert>(pos, l, typing));
}

void RichEdit::Undo()
{
	if(толькочтен_ли())
		return;
	if(undo.пустой()) return;
	CancelSelection();
	int serial = undo.дайХвост().serial;
	int c = cursor;
	while(undo.дайСчёт()) {
		UndoRec& u = undo.дайХвост();
		if(u.serial != serial) break;
		Один<UndoRec> r = u.GetRedo(text);
		r->serial = u.serial;
		r->cursor = cursor;
		redo.добавь(pick(r));
		u.Apply(text);
		c = u.cursor;
		undo.сбросьХвост();
		modified = true;
	}
	ReadStyles();
	Move(c);	
}

void RichEdit::Redo()
{
	if(толькочтен_ли())
		return;
	if(redo.пустой()) return;
	NextUndo();
	CancelSelection();
	int serial = redo.верх().serial;
	int c = cursor;
	while(redo.дайСчёт()) {
		UndoRec& r = redo.верх();
		if(r.serial != serial) break;
		Один<UndoRec> u = r.GetRedo(text);
		u->serial = r.serial;
		u->cursor = cursor;
		undo.добавьХвост(pick(u));
		r.Apply(text);
		c = r.cursor;
		redo.сбрось();
		modified = true;
	}
	ReadStyles();
	Move(c);
}

#ifdef PLATFORM_WIN32
#define RTFS "Rich устТекст фмт"
#else
#define RTFS "text/richtext"
#endif

RichText RichEdit::дайВыделение(int maxcount) const
{
	RichText clip;
	if(tablesel) {
		RichTable tab = text.CopyTable(tablesel, cells);
		clip.SetStyles(text.GetStyles());
		clip.CatPick(pick(tab));
	}
	else {
		if(begtabsel) {
			int pos = 0;
			RichPos p = text.GetRichPos(0, 1);
			if(p.table) {
				clip.SetStyles(text.GetStyles());
				do {
					RichTable tab = text.CopyTable(p.table);
					clip.CatPick(pick(tab));
					pos += p.tablen + 1;
					p = text.GetRichPos(pos, 1);
				}
				while(p.table);
				clip.CatPick(text.копируй(pos, minmax(абс(cursor - pos), 0, maxcount)));
			}
		}
		else
			clip = text.копируй(min(cursor, anchor), min(maxcount, абс(cursor - anchor)));
	}
	return clip;
}

void RichEdit::вырежь()
{
	if(толькочтен_ли())
		return;
	копируй();
	if(выделение_ли())
		удалиВыделение();
	else
	if(objectpos >= 0) {
		удали(cursor, 1);
		Move(cursor, false);
	}
}

void RichEdit::PasteText(const RichText& text)
{
	SetModify();
	modified = true;
	удалиВыделение();
	int n = text.GetPartCount() - 1;
	if(!text.IsPara(0) || !text.IsPara(n)) { // inserted section must start/end with para
		RichText pp = clone(text);
		pp.нормализуй();
		вставь(cursor, pp, false);
		ReadStyles();
		Move(cursor + pp.дайДлину(), false);
	}
	else {
		вставь(cursor, text, false);
		ReadStyles();
		Move(cursor + text.дайДлину(), false);
	}
}

struct ToParaIterator : RichText::Обходчик {
	RichPara para;
	bool     space;

	virtual bool operator()(int pos, const RichPara& p) {
		for(int i = 0; i < p.дайСчёт(); i++) {
			const RichPara::Part& part = p[i];
			if(part.текст_ли()) {
				const wchar *s = part.text;
				while(*s) {
					while(*s && *s <= ' ') {
						space = true;
						s++;
					}
					const wchar *t = s;
					while(*s > ' ') s++;
					if(s > t) {
						if(space)
							para.конкат(" ", part.формат);
						para.конкат(ШТкст(t, s), part.формат);
						space = false;
					}
				}
			}
			else if(!part.field.пусто_ли()) {
				para.конкат(part.field, part.fieldparam, part.формат);
				space = false;
			}
			else if(part.object) {
				para.конкат(part.object, part.формат);
				space = false;
			}
		}
		space = true;
		return false;
	}

	ToParaIterator() { space = false; }
};

void RichEdit::ToPara()
{
	if(толькочтен_ли())
		return;
	if(!выделение_ли() || tablesel)
		return;
	NextUndo();
	RichText txt = text.копируй(min(cursor, anchor), абс(cursor - anchor));
	ToParaIterator it;
	txt.Iterate(it);
	RichText h;
	h.SetStyles(txt.GetStyles());
	h.конкат(it.para);
	PasteText(h);
}

void RichEdit::RemoveText(int count)
{
	CancelSelection();
	удали(cursor, count);
	финиш();
}

RichText RichEdit::CopyText(int pos, int count) const
{
	return text.копируй(pos, count);
}

void RichEdit::InsertObject(int тип)
{
	RichObjectType& richtype = RichObject::дайТип(тип);
	RichObject object = RichObject(&richtype, Значение());
	RichObject o = object;
	o.DefaultAction(context);
	if(o.GetSerialId() != object.GetSerialId()) {
		RichText::FormatInfo finfo = GetFormatInfo();
		удалиВыделение();
		RichPara p;
		p.конкат(o, finfo);
		RichText clip;
		clip.конкат(p);
		вставь(дайКурсор(), clip, false);
		финиш();
	}
}

void RichEdit::ReplaceObject(const RichObject& obj)
{
	удали(objectpos, 1);
	RichPara p;
	p.конкат(obj, formatinfo);
	RichText clip;
	clip.конкат(p);
	вставь(objectpos, clip, false);
	финиш();
	objectrect = GetObjectRect(objectpos);
}

RichObject RichEdit::GetObject() const
{
	return text.GetRichPos(objectpos).object;
}

void RichEdit::выдели(int pos, int count)
{
	found = false;
	Move(pos);
	Move(pos + count, true);
}

void RichEdit::InsertLine()
{
	if(толькочтен_ли())
		return;
	RichText::FormatInfo b = formatinfo;
	RichText h;
	h.SetStyles(text.GetStyles());
	RichPara p;
	p.формат = formatinfo;
	h.конкат(p);
	h.конкат(p);
	bool st = cursorp.paralen == cursorp.posinpara;
	bool f = cursorp.posinpara == 0 && formatinfo.label.дайСчёт();
	вставь(cursor, h, false);
	if(f) {
		Ткст lbl = formatinfo.label;
		formatinfo.label.очисть();
		ApplyFormat(0, RichText::LABEL);
		formatinfo.label = lbl;
	}
	anchor = cursor = cursor + 1;
	begtabsel = false;
	formatinfo.firstonpage = formatinfo.newpage = formatinfo.newhdrftr = false;
	if(st) {
		Uuid next = text.дайСтиль(b.styleid).next;
		if(next != formatinfo.styleid) {
			formatinfo.label.очисть();
			formatinfo.styleid = next;
			ApplyFormat(0, RichText::STYLE|RichText::NEWPAGE|RichText::LABEL|RichText::NEWHDRFTR);
			return;
		}
	}
	ApplyFormat(0, RichText::NEWPAGE|RichText::LABEL|RichText::NEWHDRFTR);
	objectpos = -1;
}

}
