#include "AndroidBuilder.h"

#define METHOD_NAME "AndroidModuleMakeFileCreator::" << UPP_FUNCTION_NAME << "(): "

namespace РНЦП {

AndroidModuleMakeFileCreator::AndroidModuleMakeFileCreator(const Индекс<Ткст>& builderConfig)
	: config(builderConfig)
{
}

void AndroidModuleMakeFileCreator::AddSources(Вектор<Ткст>& sources)
{
	for(const Ткст& source : sources) {
		makeFile.AddSourceFile(source);
	}
}

void AndroidModuleMakeFileCreator::AddSources(Индекс<Ткст>& sources)
{
	for(const Ткст& source : sources) {
		makeFile.AddSourceFile(source);
	}
}

void AndroidModuleMakeFileCreator::AddInclude(const Ткст& path)
{
	makeFile.AddInclude(path);
}

void AndroidModuleMakeFileCreator::AddIncludes(const Массив<OptItem>& uses)
{
	for(const OptItem& use : uses) {
		makeFile.AddInclude(AndroidBuilderUtils::GetAssemblyDir(use.text));
	}
}

void AndroidModuleMakeFileCreator::добавьФлаги(const Массив<OptItem>& flags)
{
	for(const OptItem& flag : flags) {
		makeFile.AddCppFlag(flag.text);
	}
}

void AndroidModuleMakeFileCreator::AddLdLibraries(const Массив<OptItem>& libraries)
{
	Вектор<Ткст> libs = разбей(Gather(libraries, config.дайКлючи()), ' ');
	for(const Ткст& lib : libs) {
		makeFile.AddLdLibrary(lib);
	}
}

void AndroidModuleMakeFileCreator::AddStaticModuleLibrary(Массив<OptItem>& staticLibraries)
{
	Вектор<Ткст> slibs = разбей(Gather(staticLibraries, config.дайКлючи()), ' ');
	for(const Ткст& slib : slibs) {
		makeFile.AddStaticModuleLibrary(slib);
	}
}

void AndroidModuleMakeFileCreator::AddSharedLibraries(const Массив<OptItem>& uses)
{
	for(const OptItem& use : uses) {
		makeFile.AddSharedLibrary(use.text);
	}
}

bool AndroidModuleMakeFileCreator::сохрани(const Ткст& path)
{
	Ткст directory = дайДиректориюФайла(path);
	if (!реализуйДир(directory)) {
		Loge() << METHOD_NAME << "Creating module directory failed \"" << directory << "\".";
		return false;
	}
	
	Ткст data = создай();
	if (файлЕсть(path) && загрузиФайл(path) == data) {
		Logi() << METHOD_NAME << "Following file \"" << path << "\" content is the same as previous one.";
		return true;
	}
	
	if (!сохраниФайл(path, создай())) {
		Loge() << METHOD_NAME << "Saving module file failed \"" << path << "\".";
		return false;
	}
	
	return true;
}

}
