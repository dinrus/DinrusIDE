#ifndef _ide_Builders_Components_h_
#define _ide_Builders_Components_h_

#include <Core/Core.h>

namespace РНЦП {

class BuilderComponent {
public:
	BuilderComponent() { this->builder = NULL; }
	BuilderComponent(Построитель *builder) : builder(builder) {}
	
	void SetBuilder(Построитель *builder) { this->builder = builder; }
	
	bool IsBuilder() const { return builder != NULL; }
	
protected:
	Построитель *builder;
};

struct Blitz {
	bool   build;
	int    count;
	Ткст path;
	Ткст object;
	Ткст info;
};

Ткст BlitzBaseFile();
void   ResetBlitz();
void   InitBlitz();

class BlitzBuilderComponent : public BuilderComponent {
public:
	BlitzBuilderComponent(Построитель *builder);
	
	void SetWorkingDir(const Ткст& workingDir)       { this->workingDir = workingDir; }
	void SetBlitzFileName(const Ткст& blitzFileName) { this->blitzFileName = blitzFileName; }
	
	Blitz MakeBlitzStep(
		Вектор<Ткст>& sfile, Вектор<Ткст>& soptions,
		Вектор<Ткст>& obj, Вектор<Ткст>& immfile,
		const char *objext, const Индекс<Ткст>& noblitz,
		const Ткст& package = Null);
	                    
private:
	Ткст workingDir;
	Ткст blitzFileName;
};

}

#endif
