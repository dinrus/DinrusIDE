#include "CppBase.h"
#include "Internal.h"

namespace РНЦП {

#define LTIMING(x) // DTIMING(x)
#define LLOG(x) // DLOG(x)

static ВекторМап<Ткст, Ткст> sSrcFile;
static Индекс<uint64>             sIncludes;

Ткст NormalizeSourcePath(const Ткст& path, const Ткст& currdir)
{
	LTIMING("NormalizeSourcePath");
#ifdef PLATFORM_WIN32
	return впроп(нормализуйПуть(path, currdir));
#else
	return нормализуйПуть(path, currdir);
#endif
}

Ткст NormalizeSourcePath(const Ткст& path)
{
	return NormalizeSourcePath(path, дайТекДир());
}

void ClearSources()
{
	sSrcFile.очисть();
	sIncludes.очисть();
}

const Индекс<Ткст>& GetAllSources()
{
	return sSrcFile.дайИндекс();
}

const ВекторМап<Ткст, Ткст>& GetAllSourceMasters()
{
	return sSrcFile;
}

void GatherSources(const Ткст& master_path, const Ткст& path_, Вектор<int>& parents)
{
	Ткст path = NormalizeSourcePath(path_);
	LLOG("--- GatherSources " << master_path << " " << path);
	if(sSrcFile.найди(path) >= 0)
		return;
	int ii = sSrcFile.дайСчёт();
	for(int i = 0; i < parents.дайСчёт(); i++)
		sIncludes.добавь(MAKEQWORD(parents[i], ii));
	sSrcFile.добавь(path, master_path);
	parents.добавь(ii);
	const PPFile& f = GetPPFile(path);
	Индекс<Ткст> todo;
	for(Ткст inc : f.includes) {
		Ткст p = GetIncludePath(inc, дайПапкуФайла(path));
		if(p.дайСчёт())
			todo.найдиДобавь(p);
	}
	MakePP(todo); // parse PP files in parallel to accelerate things...
	for(Ткст p : todo)
		GatherSources(master_path, p, parents);
	parents.сбрось();
}

void GatherSources(const Ткст& path)
{
	LTIMING("GatherSources");
	Вектор<int> parents;
	LLOG("=== GatherSources " << path);
	MakePP({ NormalizeSourcePath(path) });
	GatherSources(NormalizeSourcePath(path), path, parents);
}

Ткст GetMasterFile(const Ткст& file)
{
	return sSrcFile.дай(file, Null);
}

bool   IncludesFile(const Ткст& parent_path, const Ткст& header_path)
{
	LTIMING("IncludesFile");
	int pi = sSrcFile.найди(parent_path);
	int i = sSrcFile.найди(header_path);
	return pi >= 0 && i >= 0 && sIncludes.найди(MAKEQWORD(pi, i)) >= 0;
}

}
