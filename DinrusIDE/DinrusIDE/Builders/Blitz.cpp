#include "Builders.h"

namespace РНЦП {

Ткст BlitzBaseFile()
{
	return конфигФайл("blitzbase");
}

void ResetBlitz()
{
	сохраниФайл(BlitzBaseFile(), "");
}

Время blitz_base_time;

void InitBlitz()
{
	Время ltm = Время::наибольш();

	const РОбласть& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.дайСчёт(); i++) { // find lowest file time
		const Пакет& pk = wspc.дайПакет(i);
		Ткст n = wspc[i];
		for(int i = 0; i < pk.дайСчёт(); i++) {
			Ткст path = SourcePath(n, pk.file[i]);
			if(файлЕсть(path))
				ltm = min(ltm, дайВремяф(path));
		}
	}
	
	blitz_base_time = max(дайСисВремя() - 3600, Время(дайФВремя(BlitzBaseFile())));
	if(ltm != Время::наибольш())
		blitz_base_time = max(blitz_base_time, ltm + 3 * 60); // should solve first build after install/checkout
}

BlitzBuilderComponent::BlitzBuilderComponent(Построитель* builder)
	: BuilderComponent(builder)
	, workingDir(builder->outdir)
	, blitzFileName("$blitz")
{
}

Blitz BlitzBuilderComponent::MakeBlitzStep(
	Вектор<Ткст>& sourceFiles, Вектор<Ткст>& soptions,
	Вектор<Ткст>& obj, Вектор<Ткст>& immfile,
	const char *objext, const Индекс<Ткст>& noblitz,
	const Ткст& package)
{
	Blitz b;
	b.count = 0;
	b.build = false;

	if(!IsBuilder())
		return b;
	
	if(package.дайСчёт())
		blitzFileName = package + "$blitz";

	Вектор<Ткст> excluded;
	Вектор<Ткст> excludedoptions;
	b.object = CatAnyPath(workingDir, blitzFileName + Ткст(objext));
	Время blitztime = дайФВремя(b.object);
	Ткст blitz;
	if(!исрДайОдинФайл().пустой())
		return b;
	for(int i = 0; i < sourceFiles.дайСчёт(); i++) {
		Ткст sourceFile = sourceFiles[i];
		Ткст ext = впроп(дайРасшф(sourceFile));
		Ткст objfile = CatAnyPath(workingDir, дайТитулф(sourceFile) + objext);
		Время sourceFileTime = дайФВремя(sourceFile);
		if((ext == ".cpp" || ext == ".cc" || ext == ".cxx" || ext == ".icpp")
		   && HdependBlitzApproved(sourceFile) && пусто_ли(soptions[i])
		   && sourceFileTime < blitz_base_time
		   && noblitz.найди(sourceFile) < 0) {
			if(HdependFileTime(sourceFile) > blitztime)
				b.build = true;
			blitz << "\r\n"
			      << "#define BLITZ_INDEX__ F" << i << "\r\n"
			      << "#include \"" << sourceFile << "\"\r\n";
			b.info << ' ' << дайИмяф(sourceFile);
			const Вектор<Ткст>& d = HdependGetDefines(sourceFile);
			for(int i = 0; i < d.дайСчёт(); i++)
				blitz << "#ifdef " << d[i] << "\r\n"
				      << "#undef " << d[i] << "\r\n"
				      << "#endif\r\n";
			blitz << "#undef BLITZ_INDEX__\r\n";
			b.count++;
		}
		else {
			excluded.добавь(sourceFile);
			excludedoptions.добавь(soptions[i]);
		}
	}
	
	b.path = CatAnyPath(workingDir, blitzFileName + ".cpp");
	if(b.count > 1) {
		sourceFiles = pick(excluded);
		soptions = pick(excludedoptions);
		if(загрузиФайл(b.path) != blitz) {
			реализуйДир(дайДиректориюФайла(b.path));
			сохраниФайл(b.path, blitz);
			b.build = true;
		}
		obj.добавь(b.object);
		immfile.добавь(b.object);
	}
	else {
		DeleteFile(b.path);
		b.build = false;
	}
	return b;
}

}
