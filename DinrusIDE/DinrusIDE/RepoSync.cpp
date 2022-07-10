#include "DinrusIDE.h"

Ткст GetGitUrl(const Ткст& repo_dir)
{
	Вектор<Ткст> ln = разбей(GitCmd(repo_dir, "config --get remote.origin.url"), CharFilterCrLf);
	return ln.дайСчёт() ? ln[0] : Ткст();
}


Ткст GetSvnUrl(const Ткст& repo_dir)
{
	Вектор<Ткст> ln = разбей(RepoSys("svn info --show-элт repos-root-url " + repo_dir), CharFilterCrLf);
	return ln.дайСчёт() ? ln[0] : Ткст();
}

int UrepoConsole::Git(const char *dir, const char *command, bool pwd)
{
	Ткст h = дайТекДир();
	измТекДир(dir);
	list.добавь(AttrText(Ткст("cd ") + dir).устШрифт(font().Bold().Italic()).Ink(SLtBlue()));
	Ткст cmd = Ткст() << "git " << command;
	if(pwd) {
		Ткст url = GetGitUrl(dir);
		Ткст username, password;
		if(url.начинаетсяС("https://") && GetCredentials(url, dir, username, password)) {
			Ткст https = "https://";
			cmd << ' ' << https;
			if(username.дайСчёт())
				cmd << UrlEncode(username) + ":";
			int p = cmd.дайСчёт();
			cmd << UrlEncode(password);
			HidePassword(p, cmd.дайСчёт());
			cmd << "@" << url.середина(https.дайСчёт());
		}
	}
	int code = CheckSystem(cmd);
	измТекДир(h);
	return code;
}

Ткст RepoSync::SvnCmd(UrepoConsole& sys, const char *svncmd, const Ткст& dir)
{
	Ткст cmd;
	cmd << "svn " << svncmd << " --non-interactive ";
	Ткст username, password;
	if(GetCredentials(GetSvnUrl(dir), dir, username, password)) {
		cmd << "--username " << username << " --password ";
		int i0 = cmd.дайСчёт();
		cmd << password;
		sys.HidePassword(i0, cmd.дайСчёт());
		cmd << " ";
	}
	return cmd;
}

RepoSync::RepoSync()
{
	CtrlLayoutOKCancel(*this, "Синхронизировать репозитории контроля версий");
	list.добавьИндекс();
	list.добавьИндекс();
	list.добавьКолонку("Действие");
	list.добавьКолонку("Путь");
	list.добавьКолонку("Изменения");
	list.ColumnWidths("220 500 100");
	list.NoCursor().EvenRowColor();
	list.SetLineCy(max(Draw::GetStdFontCy() + Zy(4), Zy(20)));
	Sizeable().Zoomable();
	BackPaint();
	credentials << [=] {
		Индекс<Ткст> hint;
		for(const auto& w : ~work) {
			Ткст path = w.ключ;
			Ткст s = decode(w.значение, SVN_DIR, GetSvnUrl(path), GIT_DIR, GetGitUrl(path), Null);
			if(s.дайСчёт())
				hint.найдиДобавь(s);
			if(path.дайСчёт())
				hint.найдиДобавь(path);
		}
		EditCredentials(hint.подбериКлючи());
	};
}

int CharFilterSvnMsgRepo(int c)
{
	return c >= 32 && c < 128 && c != '\"' ? c : 0;
}

bool IsConflictFile(Ткст path)
{
	Ткст ext = дайРасшф(path);
	if(*ext == '.') {
		ext = ext.середина(1);
		if(findarg(ext, "мой", "их", "рабочий") >= 0 || *ext == 'r' && цифра_ли(ext[1])) {
			for(int i = 0; i < 3; i++) {
				int q = path.найдирек('.');
				if(q < 0)
					return false;
				path.обрежь(q);
				if(файлЕсть(path))
					return true;
			}
		}
	}
	return false;
}

bool RepoSync::ListSvn(const Ткст& path)
{
	Вектор<Ткст> ln = разбей(RepoSys("svn status " + path), CharFilterCrLf);
	bool actions = false;
	for(int pass = 0; pass < 2; pass++)
		for(int i = 0; i < ln.дайСчёт(); i++) {
			Ткст h = ln[i];
			if(h.дайСчёт() > 7) {
				Ткст file = дайПолнПуть(обрежьЛево(h.середина(7)));
				if(полнпуть_ли(file)) {
					actions = true;
					h.обрежь(7);
					bool simple = h.середина(1, 6) == "      ";
					int action = simple ? Ткст("MC?!~").найди(h[0]) : SVN_IGNORE;
					if(h == "    S  ")
						action = REPLACE;
					Ткст an;
					Цвет  color;
					if(action == SVN_IGNORE) {
						color = чёрный;
						if(simple && h[0] == 'A') {
							an = "svn add";
							action = SVN_ACTION;
						}
						else
						if(simple && h[0] == 'D') {
							an = "svn delete";
							action = SVN_ACTION;
						}
						else {
							an = h.середина(0, 7);
							color = серый;
						}
					}
					else {
						if(action == ADD && IsConflictFile(file)) {
							action = DELETEC;
							an = "Удалить (конфликт разрешён)";
							color = AdjustIfDark(чёрный());
						}
						else {
							static const char *as[] = {
								"Modify", "Resolved", "добавь", "удали", "замени"
							};
							static Цвет c[] = { светлоСиний, магента, зелёный, светлоКрасный, светлоМагента };
							an = as[action];
							color = AdjustIfDark(c[action]);
						}
					}
					if(pass == action < 0 && action != DELETEC) {
						int ii = list.дайСчёт();
						list.добавь(action, file, Null,
						         AttrText(action < 0 ? ln[i] : "  " + file.середина(path.дайСчёт() + 1)).Ink(color));
						if(action >= 0) {
							list.устКтрл(ii, 0, revert.добавь().устНадпись(an + (action == ADD ? "\nSkip" : "\nRevert")).NoWantFocus());
							revert.верх() <<= 0;
							Ктрл& b = diff.добавь().устНадпись("Changes..").SizePos().NoWantFocus();
							b << [=] { DoDiff(ii); };
							list.устКтрл(ii, 2, b);
						}
					}
				}
			}
		}
	return actions;
}

Ткст GitCmd(const char *dir, const char *command)
{
	LOG("GitCmd " << dir << ", " << command);
	Ткст h = дайТекДир();
	измТекДир(dir);
	Ткст r = RepoSys(Ткст() << "git " << command);
	измТекДир(h);
	return r;
}

bool RepoSync::ListGit(const Ткст& path)
{
	Вектор<Ткст> ln = разбей(GitCmd(path, "status --porcelain ."), CharFilterCrLf);
	bool actions = false;
	for(int i = 0; i < ln.дайСчёт(); i++) {
		Ткст h = ln[i];
		if(h.дайСчёт() > 3) {
			if(!h.обрежьКонец("/"))
				h.обрежьКонец("\\");
			Ткст file = приставьИмяф(path, h.середина(3));
			actions = true;
			int action = Ткст("M.?DR").найди(h[1]);
			if(action < 0 || h[0] != '?' && h[0] != ' ')
				action = SVN_IGNORE;
			Ткст an;
			Цвет  color;
			if(action == SVN_IGNORE) {
				an = h;
				color = серый;
			}
			else {
				static const char *as[] = {
					"Изменить", "Разрещён", "Добавить", "Удалить", "Переименовать"
				};
				static Цвет c[] = { светлоСиний, магента, зелёный, светлоКрасный, светлоМагента };
				an = as[action];
				color = AdjustIfDark(c[action]);
			}
			int ii = list.дайСчёт();
			list.добавь(action, file, Null, AttrText(action < 0 ? h : file).Ink(color));
			if(action >= 0) {
				list.устКтрл(ii, 0, revert.добавь().устНадпись(an + (action == ADD ? "\nSkip" : "\nОбратить")).NoWantFocus());
				revert.верх() <<= 0;
				Ктрл& b = diff.добавь().устНадпись("Изменения..").SizePos().NoWantFocus();
				b <<= THISBACK1(DoDiff, ii);
				list.устКтрл(ii, 2, b);
			}
		}
	}
	return actions;
}

void RepoSync::SyncCommits()
{
	bool commit = true;
	for(int i = 0; i < list.дайСчёт(); i++) {
		if(SvnOptions *o = dynamic_cast<SvnOptions *>(list.дайКтрл(i, 0)))
			commit = o->commit;
		else
		if(GitOptions *o = dynamic_cast<GitOptions *>(list.дайКтрл(i, 0)))
			commit = o->commit;
		else {
			for(int j = 0; j < 2; j++) {
				Ктрл *ctrl = list.дайКтрл(i, j);
				if(ctrl)
					ctrl->вкл(commit);
			}
		}
	}
}

void RepoSync::SyncList()
{
	list.очисть();
	credentials.покажи();
	svndir.очисть();
	for(const auto& w : ~work) {
		Ткст path = дайПолнПуть(w.ключ);
		int hi = list.дайСчёт();
		Цвет bk = AdjustIfDark(светлоЖёлтый());
		list.добавь(REPOSITORY, path,
		         AttrText().Paper(bk),
		         AttrText(path).устШрифт(ArialZ(20).Bold()).Paper(bk),
		         AttrText().Paper(bk));
		list.SetLineCy(hi, Zy(26));
		bool actions = false;
		if(w.значение == SVN_DIR) {
			auto& o = list.CreateCtrl<SvnOptions>(hi, 0, false);
			o.SizePos();
			o.commit = true;
			o.commit << [=] { SyncCommits(); };
			o.update = true;
			actions = ListSvn(path);
			if(!actions) {
				o.commit = false;
				o.commit.откл();
			}
			credentials.покажи();
			svndir.найдиДобавь(GetSvnDir(w.ключ));
		}
		if(w.значение == GIT_DIR) {
			auto& o = list.CreateCtrl<GitOptions>(hi, 0, false);
			o.SizePos();
			o.commit = true;
			o.commit << [=] { SyncCommits(); };
			o.push = true;
			o.pull = true;
			actions = ListGit(path);
			if(!actions) {
				o.commit = false;
				o.push = false;
				o.commit.откл();
			}
		}
		if(actions) {
			list.добавь(MESSAGE, Null, AttrText("Сообщение коммита:").устШрифт(StdFont().Bold()));
			list.SetLineCy(list.дайСчёт() - 1, (3 * EditField::GetStdHeight()) + 4);
			list.устКтрл(list.дайСчёт() - 1, 1, message.добавь().устФильтр(CharFilterSvnMsgRepo).VSizePos(2, 2).HSizePos());
			int q = msgmap.найди(w.ключ);
			if(q >= 0) {
				message.верх() <<= msgmap[q];
				msgmap.отлинкуй(q);
			}
		}
		else
			list.добавь(-1, Null, "", AttrText("Нечего делать").устШрифт(StdFont().Italic()));
	}
}

void RepoSync::DoDiff(int ii)
{
	Ткст f = list.дай(ii, 1);
	if(!пусто_ли(f))
		RunRepoDiff(f);
}

#ifdef PLATFORM_WIN32
void sRepoDeleteFolderDeep(const char *dir)
{
	{
		ФайлПоиск ff(приставьИмяф(dir, "*.*"));
		while(ff) {
			Ткст имя = ff.дайИмя();
			Ткст p = приставьИмяф(dir, имя);
			if(ff.файл_ли()) {
				SetFileAttributes(p, GetFileAttributes(p) & ~FILE_ATTRIBUTE_READONLY);
				удалифл(p);
			}
			else
			if(ff.папка_ли())
				sRepoDeleteFolderDeep(p);
			ff.следщ();
		}
	}
	удалиДир(dir);
}
#else
void sRepoDeleteFolderDeep(const char *path)
{
	DeleteFolderDeep(path);
}
#endif

void RepoSvnDel(const char *path)
{
	ФайлПоиск ff(приставьИмяф(path, "*.*"));
	while(ff) {
		if(ff.папка_ли()) {
			Ткст dir = приставьИмяф(path, ff.дайИмя());
			if(ff.дайИмя() == ".svn")
				sRepoDeleteFolderDeep(dir);
			else
				RepoSvnDel(dir);
		}
		ff.следщ();
	}
}

void RepoSync::Dir(const char *dir)
{
	Ткст d = dir;
	int kind = GetRepo(d);
	if(kind)
		work.дайДобавь(kind == GIT_DIR ? d : Ткст(dir)) = kind;
}

void RepoMoveSvn(const Ткст& path, const Ткст& tp)
{
	ФайлПоиск ff(приставьИмяф(path, "*.*"));
	while(ff) {
		Ткст nm = ff.дайИмя();
		Ткст s = приставьИмяф(path, nm);
		Ткст t = приставьИмяф(tp, nm);
		if(ff.папка_ли()) {
			if(nm == ".svn")
				переместифл(s, t);
			else
				RepoMoveSvn(s, t);
		}
		ff.следщ();
	}
}

void RepoSync::DoSync()
{
	SyncList();
	msgmap.смети();
again:
	вкл();
	if(пуск() != IDOK || list.дайСчёт() == 0) {
		int repoi = 0;
		for(int i = 0; i < list.дайСчёт(); i++)
			if(list.дай(i, 0) == MESSAGE)
				msgmap.дайДобавь(work.дайКлюч(repoi++)) = list.дай(i, 3);
		return;
	}
	откл();
	bool changes = false;
	for(int i = 0; i < list.дайСчёт(); i++) {
		int action = list.дай(i, 0);
		if(action == MESSAGE) {
			if(changes && пусто_ли(list.дай(i, 3)) && list.дайКтрл(i, 1)->включен_ли()
			   && !PromptYesNo("Сообщение коммита пустое.&Продолжить?"))
				goto again;
			changes = false;
		}
		else if(action != REPOSITORY && list.дай(i, 2) == 0)
			changes = true;
	}
	UrepoConsole sys;
	int repoi = 0;
	int l = 0;
	while(l < list.дайСчёт()) {
		SvnOptions *svn = dynamic_cast<SvnOptions *>(list.дайКтрл(l, 0));
		GitOptions *git = dynamic_cast<GitOptions *>(list.дайКтрл(l, 0));
		Ткст repo_dir = work.дайКлюч(repoi++);
		Ткст url;
		if(git) {
			url = GetGitUrl(repo_dir);
			if(url.дайСчёт())
				sys.Log("git origin url: " + url, серый());
			
		}
		if(svn) {
			url = GetSvnUrl(repo_dir);
			if(url.дайСчёт())
				sys.Log("svn repository url: " + url, серый());
		}
		l++;
		Ткст message;
		Ткст filelist;   // <-- list of files to update
		if(git && git->pull)
			if(sys.Git(repo_dir, "pull --ff-only", true)) {
				while(l < list.дайСчёт()) {
					int action = list.дай(l, 0);
					if(action == REPOSITORY)
						break;
					l++;
				}
				continue;
			}
		bool commit = false;
		while(l < list.дайСчёт()) {
			int action = list.дай(l, 0);
			if(action == REPOSITORY)
				break;
			Ткст path = list.дай(l, 1);
			bool revert = list.дай(l, 2) == 1;
			if(svn && svn->commit) {
				if(action == MESSAGE && commit) {
					Ткст msg = list.дай(l, 3);
					if(sys.CheckSystem(SvnCmd(sys, "commit", repo_dir) << filelist << " -m \"" << msg << "\""))
						msgmap.дайДобавь(repo_dir) = msg;
					l++;
					break;
				}
				
				if(SvnFile(sys, filelist, action, path, revert))
					commit = true;
			}
			if(git && git->commit) {
				if(action == MESSAGE && commit) {
					Ткст msg = list.дай(l, 3);
					if(sys.Git(repo_dir, "commit -a -m \"" << msg << "\""))
						msgmap.дайДобавь(repo_dir) = msg;
					l++;
					break;
				}
				
				if(GitFile(sys, action, path, revert))
					commit = true;
			}
			l++;
		}
		if(svn && svn->update)
			sys.CheckSystem(SvnCmd(sys, "update", repo_dir).конкат() << repo_dir);
		if(git && git->push)
			sys.Git(repo_dir, "push", true);
	}
	sys.Log("Готово", серый());
	ResetBlitz();
	sys.Perform();
}

bool RepoSync::GitFile(UrepoConsole& sys, int action, const Ткст& path, bool revert)
{
	Ткст repo_dir = дайПапкуФайла(path);
	Ткст file = дайИмяф(path);
	if(revert) {
		if(action != ADD)
			sys.Git(repo_dir, "checkout \"" + file + "\"");
		return false;
	}
	if(action == ADD)
		sys.Git(repo_dir, "add \"" + file + "\"");
	return true;
}

bool RepoSync::SvnFile(UrepoConsole& sys, Ткст& filelist, int action, const Ткст& path, bool revert)
{
	if(revert) {
		if(action == REPLACE)
			DeleteFolderDeep(path);
		if(action != ADD)
			sys.CheckSystem("svn revert \"" + path + "\"");
		return false;
	}
	if(action >= 0 || action == SVN_ACTION)
		filelist << " \"" << path << "\"";   // <-- add the file to the list
	switch(action) {
	case ADD:
		RepoSvnDel(path);
		sys.CheckSystem("svn add --force \"" + path + "\"");
		break;
	case REMOVE:
		sys.CheckSystem("svn delete \"" + path + "\"");
		break;
	case CONFLICT:
		sys.CheckSystem("svn resolved \"" + path + "\"");
		break;
	case REPLACE: {
			RepoSvnDel(path);
			Ткст tp = приставьИмяф(дайПапкуФайла(path), фмт(Uuid::создай()));
			переместифл(path, tp);
			sys.CheckSystem(SvnCmd(sys, "update", path) << " \"" << path << "\"");
			RepoMoveSvn(path, tp);
			sRepoDeleteFolderDeep(path);
			переместифл(tp, path);
			Вектор<Ткст> ln = разбей(RepoSys("svn status \"" + path + "\""), CharFilterCrLf);
			for(int l = 0; l < ln.дайСчёт(); l++) {
				Ткст h = ln[l];
				if(h.дайСчёт() > 7) {
					Ткст file = h.середина(7);
					if(полнпуть_ли(file)) {
						h.обрежь(7);
						if(h == "?      ")
							sys.CheckSystem("svn add --force \"" + file + "\"");
						if(h == "!      ")
							sys.CheckSystem("svn delete \"" + file + "\"");
					}
				}
			}
		}
		break;
	case DELETEC:
		удалифл(path);
		break;
	}
	return findarg(action, SVN_IGNORE, DELETEC) < 0;
}

void RepoSync::сериализуй(Поток& s)
{
	int version = 0;
	s / version;
	s % msgmap;
}

void RepoSync::SetMsgs(const Ткст& s)
{
	грузиИзТкст(*this, Garble(s));
}

Ткст RepoSync::GetMsgs()
{
	return Garble(сохраниКакТкст(*this));
}
