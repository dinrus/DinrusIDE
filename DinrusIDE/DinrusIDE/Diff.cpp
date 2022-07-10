#include "DinrusIDE.h"

#define IMAGECLASS UrepoImg
#define IMAGEFILE  <DinrusIDE/urepo.iml>
#include <Draw/iml.h>

struct RepoDiff : ДиффДлг {
	ФреймВерх<СписокБроса> r;
	
	int  kind;
	
	void грузи();
	void выполни(const Ткст& f);
	
	typedef RepoDiff ИМЯ_КЛАССА;
	
	RepoDiff();
};

void RepoDiff::выполни(const Ткст& f)
{
	kind = GetRepoKind(f);
	editfile = f;
	if(kind == SVN_DIR) {
		WaitCursor wait;
		Ткст log = RepoSys("svn log " + f);
		if(log.проц_ли()) {
			Exclamation("Ошибка при выполнении 'svn log'");
			return;
		}
		ТкстПоток ss(log);
		while(!ss.кф_ли()) {
			Ткст l = ss.дайСтроку();
			if(l[0] == 'r') {
				Вектор<Ткст> h = разбей(l, '|');
				if(h.дайСчёт() > 3) {
					Ткст rev = обрежьОба(h[0]);
					Ткст s = rev;
					Вектор<Ткст> t = разбей(h[2], ' ');
					if(t.дайСчёт() > 1)
						s << ' ' << t[0];
					s << ' ' << обрежьОба(h[1]);
					while(!ss.кф_ли()) {
						l = ss.дайСтроку();
						if(l.дайСчёт()) {
							if(l[0] != '-')
								s << ": " << l;
							break;
						}
					}
					r.добавь(rev, s);
				}
			}
		}
	}

	if(kind == GIT_DIR) {
		Ткст log = GitCmd(дайПапкуФайла(f), "log --fornat=medium --date=short " + дайИмяф(f));
		ТкстПоток ss(log);
		Ткст author, date, commit;
		while(!ss.кф_ли()) {
			Ткст l = ss.дайСтроку();
			Вектор<Ткст> s = разбей(l, CharFilterWhitespace);
			if(s.дайСчёт() == 0) {
				while(!ss.кф_ли()) {
					l = ss.дайСтроку();
					if(l.дайСчёт())
						break;
				}
				Ткст msg = l;
				while(!ss.кф_ли()) {
					l = ss.дайСтроку();
					if(l.дайСчёт() == 0)
						break;
					msg << l;
				}
				if(commit.дайСчёт())
					r.добавь(commit, date + ' ' + author + ": " + Join(разбей(msg, CharFilterWhitespace), " "));
				date = commit = author = Null;
			}
			else
			if(s.дайСчёт() >= 2) {
				Ткст k = впроп(s[0]);
				if(k == "author:") {
					s.удали(0);
					author = Join(s, " ");
				}
				if(k == "date:")
					date = s[1];
				if(k == "commit")
					commit = s[1];
			}
		}
		грузи();
	}

	if(r.дайСчёт() == 0) {
		Exclamation("Нет разборчивой истории для этого файла");
		return;
	}

	r.SetIndex(0);
	грузи();
	
	ДиффДлг::выполни(f);
}

void RepoDiff::грузи()
{
	if(kind == SVN_DIR)
		extfile = RepoSys("svn cat " + editfile + '@' + какТкст(~r));
	if(kind == GIT_DIR)
		extfile = GitCmd(дайПапкуФайла(editfile), "show " + ~~r + ":./" + дайИмяф(editfile));
	diff.уст(backup = загрузиФайл(editfile), extfile);
}

RepoDiff::RepoDiff()
{
	r.устВысоту(EditField::GetStdHeight());
	r.SetDropLines(32);
	Иконка(UrepoImg::RepoDiff());
	diff.вставьФреймСправа(r);
	r <<= THISBACK(грузи);
}

void RunRepoDiff(const Ткст& filepath)
{
	if(пусто_ли(filepath))
		return;
	RepoDiff dlg;
	dlg.выполни(filepath);
}
