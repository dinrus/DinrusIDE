#include "TextDiffCtrl.h"

namespace РНЦП {

DirDiffDlg::DirDiffDlg()
{
	int div = HorzLayoutZoom(4);
	int cy = dir1.дайСтдРазм().cy;

	int bcx = дайРазмТекста(t_("Сравнить"), StdFont()).cx * 12 / 10 + 2 * div;

	hidden.устНадпись(t_("Скрытый"));
	split_lines.устНадпись(t_("Перенос длинных строк"));
	
	added.устЦвет(зелёный()).устНадпись(t_("Новый"));
	modified.устНадпись(t_("Изменён"));
	removed.устЦвет(красный()).устНадпись(t_("Удалён"));
	
	recent <<= Null;
	recent.добавь(Null, "Все");
	recent.добавь(1, "1 День");
	recent.добавь(3, "3 Дня");
	recent.добавь(7, "7 Дней");
	recent.добавь(14, "14 Дней");
	recent.добавь(32, "28 Дней");
	
	compare.устНадпись(t_("Сравнить"));
	int bcy = max(cy, compare.дайСтдРазм().cy);
	
	files_pane.добавь(dir1.TopPos(0, cy).HSizePos());
	files_pane.добавь(dir2.TopPos(cy + div, cy).HSizePos());
	files_pane.добавь(hidden.TopPos(2 * cy + 2 * div, bcy).LeftPos(0, bcx));
	files_pane.добавь(split_lines.TopPos(2 * cy + 2 * div, bcy).LeftPosZ(52, 100));
	
	files_pane.добавь(   added.TopPos(3 * cy + 3 * div, bcy).LeftPosZ(2, 60));
	files_pane.добавь(modified.TopPos(3 * cy + 3 * div, bcy).LeftPosZ(52, 70));
	files_pane.добавь( removed.TopPos(3 * cy + 3 * div, bcy).LeftPosZ(128, 80));
	files_pane.добавь(  recent.TopPos(3 * cy + 3 * div, bcy).RightPos(0, bcx));
	
	removed = 1;
	added = 1;
	modified = 1;
	find.NullText(t_("Поиск (Ктрл+F)"));
	clearFind.устНадпись("X");
	clearFind.RightPosZ(1, 16).VSizePosZ(1, 1);
	find.добавьОтпрыск(&clearFind);
	
	files_pane.добавь(compare.TopPos(2 * cy + 2 * div, bcy).RightPos(0, bcx));
	files_pane.добавь(files.VSizePos(3 * cy + bcy + 4 * div, Zy(24)).HSizePos());
	files_pane.добавь(find.BottomPosZ(4, 19).HSizePosZ());

	добавь(files_diff.SizePos());
	files_diff.уст(files_pane, diff);
	files_diff.устПоз(2000);
	files_diff.SetMinPixels(0, Zx(256));
	
	Sizeable().Zoomable();
	
	seldir1.прикрепи(dir1);
	seldir2.прикрепи(dir2);
	
	seldir1.Титул("Первая сравниваемая директория");
	seldir2.Титул("Вторая сравниваемая директория");
	
	compare <<= THISBACK(сравни);
	dir1 <<= THISBACK(ClearFiles);
	dir2 <<= THISBACK(ClearFiles);
	
	modified	<< [=] { ShowResult(); };
	removed		<< [=] { ShowResult(); };
	added		<< [=] { ShowResult(); };
	find		<< [=] { ShowResult(); };
	clearFind	<< [=] { find.очисть(); ShowResult();};
	
	files.WhenSel = THISBACK(Файл);

	diff.вставьФреймСлева(left);
	diff.вставьФреймСправа(right);

	left.устВысоту(EditField::GetStdHeight());
	lfile.устТолькоЧтен();
	left.добавь(lfile.VSizePos().HSizePosZ(0, 222));
	left.добавь(copyright.VSizePos().RightPosZ(0, 70));
	left.добавь(removeleft.VSizePos().RightPosZ(74, 70));
	left.добавь(revertleft.VSizePos().RightPosZ(148, 70));

	right.устВысоту(EditField::GetStdHeight());
	rfile.устТолькоЧтен();
	right.добавь(rfile.VSizePos().HSizePosZ(222, 0));
	right.добавь(copyleft.VSizePos().LeftPosZ(0, 70));
	right.добавь(removeright.VSizePos().LeftPosZ(74, 70));
	right.добавь(revertright.VSizePos().LeftPosZ(148, 70));
	
	auto SetupCopy = [=](Кнопка& copy, bool left) {
		copy.устРисунок(left ? DiffImg::CopyLeft() : DiffImg::CopyRight());
		copy.устНадпись("Копировать");
		copy.Подсказка("F5");
		copy.откл();
		copy << [=] { копируй(left); };
	};
	
	SetupCopy(copyleft, true);
	SetupCopy(copyright, false);

	auto SetupRevert = [=](Кнопка& revert, EditString *dir) {
		revert.откл();
		revert.устНадпись("Обратить");
		revert.устРисунок(CtrlImg::undo());
		revert << [=] {
			Ткст path = приставьИмяф(~*dir, files.GetCurrentName());
			int q = backup.найди(path);
			if(q >= 0 && PromptYesNo("Обратить изменения?")) {
				сохраниФайл(path, ZDecompress(backup[q]));
				backup.удали(q);
				освежи();
			}
		};
	};
	
	SetupRevert(revertleft, &dir1);
	SetupRevert(revertright, &dir2);
	
	auto SetupRemove = [=](Кнопка& remove, КтрлСравниТекст *text, EditString *dir)
	{
		remove.устНадпись("Удалить");
		remove.Подсказка("F8");
		remove.устРисунок(CtrlImg::remove());
		
		remove << [=] {
			Ткст path = приставьИмяф(~*dir, files.GetCurrentName());
			Backup(path);
			сохраниФайл(path, text->RemoveSelected(HasCrs(path)));
			освежи();
		};
	
		text->WhenSel << [=, &remove] {
			remove.вкл(text->выделение_ли());
		};
	};
	
	SetupRemove(removeleft, &diff.left, &dir1);
	SetupRemove(removeright, &diff.right, &dir2);
	
	split_lines << [=] { Файл(); };

	Иконка(DiffImg::DirDiff());

	WhenIcon = [](const char *path) -> Рисунок { return NativePathIcon(path); };
	
	Титул("Сравнить директории");
};

void DirDiffDlg::GatherFilesDeep(Индекс<Ткст>& files, const Ткст& base, const Ткст& path)
{
	ФайлПоиск ff(приставьИмяф(приставьИмяф(base, path), "*.*"));
	while(ff) {
		Ткст p = (path.дайСчёт() ? path + '/' : Ткст()) + ff.дайИмя();
		if(hidden || !ff.скрыт_ли()) {
			if(ff.файл_ли())
				files.найдиДобавь(p);
			else
			if(ff.папка_ли())
				GatherFilesDeep(files, base, p);
		}
		ff.следщ();
	}
}

bool DirDiffDlg::FileEqual(const Ткст& f1, const Ткст& f2, int& n)
{
	ФайлВвод in1(f1);
	ФайлВвод in2(f2);
	if(in1 && in2) {
		in1.устРазмБуф(256 * 1024);
		in2.устРазмБуф(256 * 1024);
		while(!in1.кф_ли() && !in2.кф_ли()) {
			Ткст a = in1.дайСтроку();
			Ткст b = in2.дайСтроку();
			if(a != b)
				return false;
		}
		return true;
	}
	else
	{
		n = (in1 ? DELETED_FILE : NEW_FILE);
	}
	
	return false;
}

void DirDiffDlg::сравни()
{
	Индекс<Ткст> fs;
	GatherFilesDeep(fs, ~dir1, Null);
	GatherFilesDeep(fs, ~dir2, Null);

	copyleft.откл();
	copyright.откл();
	
	files.очисть();
	Вектор<Ткст> f = fs.подбериКлючи();
	сортируй(f);
	Progress pi(t_("Сравниваются.."));
	pi.устВсего(f.дайСчёт());
	
	Дата dlim = пусто_ли(recent) ? Null : дайСисДату() - (int)~recent;

	list.очисть();
	for(int i = 0; i < f.дайСчёт(); i++) {
		if(pi.StepCanceled())
			break;
		Ткст p1 = приставьИмяф(~dir1, f[i]);
		Ткст p2 = приставьИмяф(~dir2, f[i]);
		int n = NORMAL_FILE;
		if((пусто_ли(dlim) || дайВремяф(p1) >= dlim || дайВремяф(p2) >= dlim) && !FileEqual(p1, p2, n))
			list.добавь(сделайКортеж(f[i], p1, p2, n));
	}
	
	ShowResult();
}

СписокФайлов::Файл DirDiffDlg::MakeFile(int i)
{
	static Цвет cs[] = { SColorText(), SRed(), SGreen(), SRed(), SLtBlue() };
	СписокФайлов::Файл m;
	m.isdir = false;
	m.unixexe = false;
	m.hidden = false;
	Рисунок icn = WhenIcon(файлЕсть(list[i].b) ? list[i].b : list[i].c);
	int k = list[i].d;
	if(пусто_ли(icn))
		icn = CtrlImg::File();
	m.icon = decode(k, FAILED_FILE, MakeImage(icn, [] (const Рисунок& m) { return GetOver(m, DiffImg::Failed()); }),
	                   PATCHED_FILE, MakeImage(icn, [] (const Рисунок& m) { return GetOver(m, DiffImg::Patched()); }),
	                   icn);
	m.имя = list[i].a;
	m.font = decode(k, FAILED_FILE, StdFont().Strikeout().Italic(),
	                   PATCHED_FILE, StdFont().Italic(), StdFont());
	m.ink = cs[k];
	m.length = 0;
	m.time = Null;
	m.extink = m.ink;
	m.data = i;
	return m;
}

void DirDiffDlg::ShowResult()
{
	files.очисть();
	Ткст sFind = впроп((Ткст)~find);
	for(int i = 0; i < list.дайСчёт(); i++)
	{
		int n = list[i].d;
		if((n == NORMAL_FILE && modified || n == DELETED_FILE && removed
		    || n == NEW_FILE && added || n == FAILED_FILE || n == PATCHED_FILE)
		   && впроп(list[i].a).найди(sFind) >= 0)
			files.добавь(MakeFile(i));
	}
	Титул(какТкст(files.дайСчёт()) + " файлов");
	clearFind.покажи(!пусто_ли(find));
}

void DirDiffDlg::ClearFiles()
{
	files.очисть();
	compare.вкл(!пусто_ли(dir1) && !пусто_ли(dir2));
}

ШТкст разверниТабы(const wchar *text)
{
	ШТкст out;
	for(wchar c; (c = *text++);)
		if(c == '\t')
			out.конкат(' ', 4 - out.дайДлину() % 4);
		else
			out.конкат(c);
	return out;
}

Ткст SplitLines(const Ткст& s)
{
    ТкстПоток ss(s);
    ШТкст result;
    while(!ss.кф_ли()) {
        ШТкст l = разверниТабы(ss.дайСтроку().вШТкст());
        int q = 0;
        while(l.дайСчёт() - q > 80) {
            result.конкат(~l + q, 80);
            result.конкат('\n');
            q += 80;
        }
        result.конкат(~l + q, l.дайСчёт() - q);
        result.конкат('\n');
    }
    return result.вТкст();
}

void DirDiffDlg::Файл()
{
	Ткст фн = files.GetCurrentName();
	Ткст p1 = приставьИмяф(~dir1, фн);
	Ткст p2 = приставьИмяф(~dir2, фн);

	diff.right.WhenHighlight = diff.left.WhenHighlight = [=](Вектор<СтрокРедакт::Highlight>& hl, const ШТкст& ln) {
		ДиффДлг::WhenHighlight(приставьИмяф(p1, files.GetCurrentName()), hl, ln);
	};

	diff.уст(Null, Null);
	Ткст f1 = загрузиФайл(p1);
	Ткст f2 = загрузиФайл(p2);
	if(split_lines) {
	    f1 = SplitLines(f1);
	    f2 = SplitLines(f2);
	}
	if(дайДлинуф(p1) < 4 * 1024 * 1024 && дайДлинуф(p2) < 4 * 1024 * 1024)
		diff.уст(f1, f2);
	lfile <<= p1;
	rfile <<= p2;
	copyleft.вкл();
	copyright.вкл();
	revertleft.вкл(backup.найди(p1) >= 0);
	revertright.вкл(backup.найди(p2) >= 0);
}

void DirDiffDlg::освежи()
{
	int sc = diff.GetSc();
	Файл();
	diff.Sc(sc);
}

void DirDiffDlg::Backup(const Ткст& path)
{
	int q = backup.найди(path);
	if(q < 0 && дайДлинуф(path) < 4 * 1024 * 1024 && backup.дайСчёт() < 100)
		backup.добавь(path, ZCompress(загрузиФайл(path)));
}

void DirDiffDlg::копируй(bool left)
{
	Ткст src = ~lfile;
	Ткст dst = ~rfile;
	if(left)
		РНЦП::разверни(src, dst);
	if(left ? diff.right.выделение_ли() : diff.left.выделение_ли()) {
		Backup(dst);
		сохраниФайл(dst, diff.Merge(left, HasCrs(dst)));
		освежи();
		return;
	}
	if(PromptYesNo("Копировать [* \1" + src + "\1]&в [* \1" + dst + "\1] ?")) {
		Backup(dst);
		ФайлВвод  in(src);
		ФайлВывод out(dst);
		копируйПоток(out, in);
		out.закрой();
		освежи();
	}
}

bool РНЦП::DirDiffDlg::горячаяКлав(dword ключ)
{
	if(ключ == K_CTRL_F) {
		ActiveFocus(find);
		return true;
	}
	return false;
}

bool DirDiffDlg::Ключ(dword ключ, int count)
{
	bool left;
	if(diff.left.естьФокус())
		left = true;
	else
	if(diff.right.естьФокус())
		left = false;
	else
		return ТопОкно::Ключ(ключ, count);
	switch(ключ) {
	case K_F5:
	case K_INSERT:
	case K_ENTER:
	case K_SPACE:
		(left ? copyright : copyleft).WhenAction();
		return true;
	case K_F8:
	case K_DELETE:
		(left ? removeleft : removeright).WhenAction();
		return true;
	}
	return ТопОкно::Ключ(ключ, count);
}

};