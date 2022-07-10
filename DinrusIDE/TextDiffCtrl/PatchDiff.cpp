#include "TextDiffCtrl.h"

namespace РНЦП {

PatchDiff::PatchDiff()
{
	Иконка(DiffImg::PatchDiff());

	hidden.скрой();
	
	copyright.удали();
	removeright.удали();
	revertright.удали();

	recent.скрой();

	left.устВысоту(EditField::GetStdHeight());
	left.добавь(lfile.HSizePosZ(0, 148));
	left.добавь(removeleft.VSizePos().RightPosZ(0, 70));
	left.добавь(revertleft.VSizePos().RightPosZ(74, 70));

	right.добавь(rfile.VSizePos().HSizePosZ(74, 0));
	right.добавь(copyleft.VSizePos().LeftPosZ(0, 70));
	
	copyleft.устНадпись("Патч");
	
	files.WhenSel = [=] { Файл(); };
	
	dir1.Ктрл::удали();
	dir2.Ктрл::удали();

	int cy = patch_file.дайСтдРазм().cy;
	int div = Zy(4);
	files_pane.добавь(target_dir.TopPos(0, cy).HSizePos());
	files_pane.добавь(patch_file.TopPos(cy + div, cy).HSizePos());
	patch_file.устТолькоЧтен();
	target_dir.устТолькоЧтен();
	
	seldir.прикрепи(target_dir);
	seldir.Титул("Target directory");
	selfile.прикрепи(patch_file);
	selfile.Титул("Patch file");
	selfile.Types("Patch files (*.diff *.patch)\t*.diff *.patch\nAll files\t*.*");
	
	seldir.WhenSelected = selfile.WhenSelected = [=] {
		открой(~~patch_file, Вектор<Ткст>() << ~seldir);
	};
	
	files_pane.добавь(failed.TopPos(2 * cy + 2 * div, cy).HSizePos());
	
	compare.устНадпись("Patch All");
	compare ^= [=] {
		Ткст msg = "Patch everything?";
		if(failed_count)
			msg << "&[/ (" << failed_count << " files cannot be patched)";
		if(files.дайСчёт() == 0 || !PromptYesNo(msg))
			return;
		Progress pi("Patching", patch.дайСчёт());
		for(int i = 0; i < patch.дайСчёт(); i++) {
			if(pi.StepCanceled())
				return;
			Ткст p = patch.дайПуть(i);
			Ткст h = patch.GetPatchedFile(i, GetBackup(p));
			if(!h.проц_ли()) {
				if(пусто_ли(h))
					удалифл(p);
				else
					сохраниФайл(p, h);
			}
		}
		Break(IDOK);
	};

	removeleft ^= [=] {
		Backup(file_path);
		сохраниФайл(file_path, diff.left.RemoveSelected(HasCrs(file_path)));
		освежи();
	};

	copyleft ^= [=] {
		int ii = GetFileIndex();
		if(ii < 0 || patched_file.проц_ли())
			return;
		if(diff.right.выделение_ли()) {
			Backup(file_path);
			сохраниФайл(file_path, diff.Merge(true, HasCrs(file_path)));
			освежи();
			return;
		}
		if(PromptYesNo("Patch [* \1" + file_path + "\1] ?")) {
			Backup(file_path);
			if(пусто_ли(patched_file))
				удалифл(file_path);
			else
				сохраниФайл(file_path, patched_file);
			list[ii].d = 4;
			files.уст(files.дайКурсор(), MakeFile(ii));
			освежи();
		}
	};

	revertleft ^= [=] {
		int q = backup.найди(file_path);
		if(q >= 0 && PromptYesNo("Revert changes?")) {
			сохраниФайл(file_path, ZDecompress(backup[q]));
			backup.удали(q);
			освежи();
		}
	};

	Титул("Patch");
}

bool PatchDiff::открой(const char *patch_path, const Вектор<Ткст>& target_dirs0)
{
	failed_count = 0;
	list.очисть();
	failed.скрой();
	ShowResult();
	patch_file <<= Null;
	target_dir <<= Null;
	
	Вектор<Ткст> target_dirs;
	for(Ткст s : target_dirs0)
		target_dirs.добавь(UnixPath(s));

	Progress pi;
	if(!patch.грузи(patch_path, pi)) {
		Exclamation("Failed to load the patch file!");
		return false;
	}
	
	patch_file <<= patch_path;
	

	if(!patch.MatchFiles(target_dirs, pi)) {
		Exclamation("Unable to match the directory structure!");
		return true;
	}

	Ткст target_dir = patch.GetTargetDir();

	this->target_dir <<= target_dir;

	list.очисть();
	pi.устТекст("Checking files");
	pi.устВсего(patch.дайСчёт());
	for(int i = 0; i < patch.дайСчёт(); i++) {
		if(pi.StepCanceled())
			return false;
		Ткст фн = patch.GetFile(i);
		Ткст p = patch.дайПуть(i);
		Ткст h = patch.GetPatchedFile(i, GetBackup(p));
		bool pe = h.дайСчёт();
		bool x = файлЕсть(p);
		bool failed = h.проц_ли();
		list.добавь(сделайКортеж(фн, p, p, failed ? 3 : pe && x ? 0 : pe ? 2 : 1));
		if(failed)
			failed_count++;
	}
	
	failed.покажи(failed_count);
	failed.устЧернила(SRed());
	failed = Ткст() << failed_count << " file(s) failed";

	ShowResult();
	
	if(files.дайСчёт())
		files.устКурсор(0);
	
	return true;
}

int PatchDiff::GetFileIndex() const
{
	int ii = files.дайКурсор();
	return ii >= 0 ? (int)files.дай(ii).data : -1;
}

Ткст PatchDiff::GetBackup(const Ткст& path)
{
	int q = backup.найди(path);
	return q >= 0 ? ZDecompress(backup[q]) : Ткст();
}

void PatchDiff::Файл()
{
	diff.уст(Null, Null);
	Ткст p2;
	Ткст op = " [PATCHED]";
	copyleft.откл();
	file_path.очисть();
	patched_file.очисть();
	int ii = GetFileIndex();
	if(ii >= 0) {
		file_path = patch.дайПуть(ii);
		if(дайДлинуф(file_path) < 4 * 1024 * 1024) {
			Ткст content = загрузиФайл(file_path);
			if(list[ii].d == PATCHED_FILE) {
				p2 = "[FILE IS PATCHED]";
				diff.уст(content, content);
			}
			else {
				patched_file = patch.GetPatchedFile(ii, GetBackup(file_path));
				if(patched_file.проц_ли()) {
					diff.уст(content, patch.GetPatch(ii));
					p2 = "[FAILED TO APPLY THE PATCH]";
				}
				else {
					diff.уст(content, patched_file);
					p2 = file_path + " [PATCHED]";
					copyleft.вкл();
				}
			}
		}
	}
	lfile <<= file_path;
	rfile <<= p2;
	revertleft.вкл(backup.найди(file_path) >= 0);
}

};