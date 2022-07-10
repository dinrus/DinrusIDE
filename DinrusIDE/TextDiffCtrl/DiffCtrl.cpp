#include "TextDiffCtrl.h"

namespace РНЦП {

#define IMAGECLASS DiffImg
#define IMAGEFILE <TextDiffCtrl/Diff.iml>
#include <Draw/iml_source.h>

КтрлДиффТекст::КтрлДиффТекст()
{
	left.устЛев();
	left.Gutter(30);
	next.устРисунок(DiffImg::Next());
	prev.устРисунок(DiffImg::Prev());
	left.scroll.y.добавьФрейм(prev);
	left.scroll.y.добавьФрейм(next);
	next << [=] { найдиДифф(true); };
	prev << [=] { найдиДифф(false); };
	right.NoGutter();
	гориз(left, right);
	left.ПриПромоте = right.ScrollWhen(left);
	right.ПриПромоте = left.ScrollWhen(right);
	right.HideSb();
	left.WhenLeftDouble = THISBACK(GetLeftLine);
	right.WhenLeftDouble = THISBACK(GetRightLine);
}

void КтрлДиффТекст::GetLeftLine(int number, int line)
{
	WhenLeftLine(number);
}

void КтрлДиффТекст::GetRightLine(int number, int line)
{
	WhenRightLine(number);
}

static bool SmallDiff(const char *s1, const char *s2)
{
	for(;;) {
		while(*s1 && (byte)*s1 <= ' ')
			s1++;
		while(*s2 && (byte)*s2 <= ' ')
			s2++;
		if(!*s1 || !*s2)
			return !*s1 && !*s2;
		if(*s1 != *s2)
			return false;
		while(*s1 && *s1 == *s2)
			s1++, s2++;
		if((byte)s1[-1] <= ' ')
			continue;
		if((byte)*s1 > ' ' || (byte)*s2 > ' ')
			return false;
	}
}

void КтрлДиффТекст::уст(Поток& l, Поток& r)
{
	Вектор<Ткст> ll = дайМапСтрок(l);
	Вектор<Ткст> rl = дайМапСтрок(r);
	Массив<ТекстСекция> sections = сравниМапыСтрок(ll, rl);
	int outln = 0;
	left.устСчёт(0);
	right.устСчёт(0);
	int firstdiff = -1;
	for(int i = 0; i < sections.дайСчёт(); i++) {
		const ТекстСекция& sec = sections[i];
		bool diff = !sec.same;
		if(firstdiff < 0 && diff)
			firstdiff = outln;
		int maxcount = max(sec.count1, sec.count2);
		left.AddCount(maxcount);
		int l;
		for(l = 0; l < sec.count1; l++) {
			int level = (diff ? l < sec.count2 && SmallDiff(ll[sec.start1 + l], rl[sec.start2 + l]) ? 1 : 2 : 0);
			left.уст(outln + l, ll[sec.start1 + l], diff, sec.start1 + l + 1, level, diff && l < sec.count2 ? rl[sec.start2 + l] : Null, sec.start1 + l + 1);
		}
		for(; l < maxcount; l++)
			left.уст(outln + l, Null, diff, Null, 2, Null, Null);
		right.AddCount(maxcount);
		for(l = 0; l < sec.count2; l++) {
			int level = (diff ? l < sec.count1 && SmallDiff(rl[sec.start2 + l], ll[sec.start1 + l]) ? 1 : 2 : 0);
			right.уст(outln + l, rl[sec.start2 + l], diff, sec.start2 + l + 1, level,  diff && l < sec.count1 ? ll[sec.start1 + l] : Null, sec.start2 + l + 1);
		}
		for(; l < maxcount; l++)
			right.уст(outln + l, Null, diff, Null, 2, Null, Null);
		outln += maxcount;
	}
	if(firstdiff >= 0)
		left.SetSb(max(firstdiff - 2, 0));
	left.очистьВыделение();
	right.очистьВыделение();
}

Ткст КтрлДиффТекст::Merge(bool l, bool cr)
{
	ПРОВЕРЬ(left.дайСчёт() == right.дайСчёт());
	const КтрлСравниТекст& target = l ? left : right;
	const КтрлСравниТекст& source = l ? right : left;
	Ткст r;
	Ткст eol = cr ? "\r\n" : "\n";
	for(int i = 0; i < target.дайСчёт(); i++) {
		if(source.выделен(i) && source.HasLine(i))
			r << source.дайТекст(i) << eol;
		else
		if(target.HasLine(i))
			r << target.дайТекст(i) << eol;
	}
	r.обрежьКонец(eol);
	return r;
}

Ткст КтрлСравниТекст::RemoveSelected(bool cr)
{
	Ткст r;
	Ткст eol = cr ? "\r\n" : "\n";
	for(int i = 0; i < дайСчёт(); i++)
		if(!выделен(i) && HasLine(i))
			r << дайТекст(i) << eol;
	r.обрежьКонец(eol);
	return r;
}

void КтрлДиффТекст::уст(const Ткст& l, const Ткст& r)
{
	ТкстПоток sl(l);
	ТкстПоток sr(r);
	уст(sl, sr);
}

void КтрлДиффТекст::найдиДифф(bool fw)
{
	int i = left.scroll.дайУ() + (fw ? 2 * left.scroll.дайСтраницу().cy / 3 : -1);
	while(i > 0 && i < left.lines.дайСчёт()) {
		if(left.lines[i].diff) {
			left.SetSb(max(i - 2, 0));
			return;
		}
		i += fw ? 1 : -1;
	}
}

bool КтрлДиффТекст::Ключ(dword ключ, int count)
{
	switch(ключ) {
	case K_F3:
		найдиДифф(true);
		return true;
	case K_SHIFT_F3:
		найдиДифф(false);
		return true;
	}
	return Сплиттер::Ключ(ключ, count);
}

ИНИЦБЛОК {
	региструйГлобКонфиг("diff");
}

bool ДиффДлг::Ключ(dword ключ, int count)
{
	switch(ключ) {
	case K_F5:
	case K_INSERT:
	case K_ENTER:
	case K_SPACE:
		пиши();
		return true;
	case K_F8:
	case K_DELETE:
		remove.WhenAction();
		return true;
	}
	return ТопОкно::Ключ(ключ, count);
}

void ДиффДлг::выполни(const Ткст& f)
{
	editfile = f;
	l <<= editfile;
	Титул(editfile);
	Ткст h;
	{
		грузиИзГлоба(h, "diff");
		ТкстПоток ss(h);
		SerializePlacement(ss);
	}
	ТопОкно::выполни();
	{
		ТкстПоток ss;
		SerializePlacement(ss);
		h = ss;
		сохраниВГлоб(h, "diff");
	}
}

void ДиффДлг::освежи()
{
	int sc = diff.GetSc();
	diff.уст(загрузиФайл(editfile), extfile);
	diff.Sc(sc);
}

bool HasCrs(const Ткст& path)
{
	ФайлВвод in(path);
	if(in) {
		while(!in.кф_ли()) {
			int c = in.дай();
			if(c == '\r')
				return true;
			if(c == '\n')
				return false;
		}
	}
	return false;
}

void ДиффДлг::пиши()
{
	if(diff.right.выделение_ли()) {
		сохраниФайл(editfile, diff.Merge(true, HasCrs(editfile)));
		освежи();
		revert.вкл();
		return;
	}
	if(PromptYesNo("Do you want to overwrite&[* " + DeQtf(editfile) + "] ?")) {
		сохраниФайл(editfile, extfile);
		Break(IDOK);
		revert.вкл();
	}
}

Событие<const Ткст&, Вектор<СтрокРедакт::Highlight>&, const ШТкст&> ДиффДлг::WhenHighlight;

ДиффДлг::ДиффДлг()
{
	добавь(diff.SizePos());
	Sizeable().Zoomable();
	diff.вставьФреймСлева(p);

	p.устВысоту(EditField::GetStdHeight());
	p.добавь(l.VSizePos().HSizePosZ(0, 222));
	p.добавь(write.VSizePos().RightPosZ(0, 70));
	p.добавь(remove.VSizePos().RightPosZ(74, 70));
	p.добавь(revert.VSizePos().RightPosZ(148, 70));

	write << [=] { пиши(); };
	write.устНадпись("копируй");
	write.устРисунок(DiffImg::CopyLeft());
	write.Подсказка("F5");

	revert.откл();
	revert.устНадпись("Revert");
	revert.устРисунок(CtrlImg::undo());
	revert << [=] {
		if(PromptYesNo("Revert changes?")) {
			сохраниФайл(editfile, backup);
			освежи();
		}
	};
	
	remove.устНадпись("удали");
	remove.устРисунок(CtrlImg::remove());
	remove.Подсказка("F8");
	remove << [=] {
		сохраниФайл(editfile, diff.left.RemoveSelected(HasCrs(editfile)));
		освежи();
		revert.вкл();
	};
	
	diff.left.WhenSel << [=] {
		remove.вкл(diff.left.выделение_ли());
	};
	
	l.устТолькоЧтен();

	diff.right.WhenHighlight = diff.left.WhenHighlight = [=](Вектор<СтрокРедакт::Highlight>& hl, const ШТкст& ln) {
		ДиффДлг::WhenHighlight(editfile, hl, ln);
	};
}

void FileDiff::открой()
{
	if(пусто_ли(r)) {
		if(!fs.ExecuteOpen())
			return;
		r <<= ~fs;
	}
	if(пусто_ли(r))
		return;
	backup = загрузиФайл(editfile);
	diff.уст(backup, extfile = загрузиФайл(~~r));
}

void FileDiff::выполни(const Ткст& f)
{
	editfile = f;
	открой();
	if(пусто_ли(r))
		return;
	ДиффДлг::выполни(f);
}

FileDiff::FileDiff(FileSel& fs_)
: fs(fs_)
{
	r.устВысоту(EditField::GetStdHeight());
	Иконка(DiffImg::Diff());
	diff.вставьФреймСправа(r);
	r <<= THISBACK(открой);
}

void FileDiff::выполни(const Ткст& lpath, const Ткст& rpath)
{
	r <<= rpath;
	выполни(lpath);
}

FileSel& DiffFs() {
	static FileSel fs;
	ONCELOCK {
		fs.Type("Patch file (*.diff, *.patch)", "*.diff *.patch");
		fs.AllFilesType();
	}
	return fs;
}

};
