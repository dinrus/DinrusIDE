#include "Core.h"

int GetRepo(Ткст& path)
{
	if(пусто_ли(path))
		return NOT_REPO_DIR;
	if(дирЕсть(приставьИмяф(path, ".svn")) || дирЕсть(приставьИмяф(path, "_svn")))
		return SVN_DIR;
	for(;;) {
		Ткст git = приставьИмяф(path, ".git");
		if(дирЕсть(git) || файлЕсть(git))
			return GIT_DIR;
		if(дирЕсть(приставьИмяф(path, ".svn")))
			return SVN_DIR;
		Ткст path0 = path;
		path = дайПапкуФайла(path);
		if(path == path0)
			break;
	}
	return NOT_REPO_DIR;
}

int GetRepoKind(const Ткст& p)
{
	Ткст pp = p;
	return GetRepo(pp);
}
