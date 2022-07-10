#include "AndroidBuilder.h"

namespace РНЦП {

Ткст AndroidBuilderUtils::GetAssemblyDir(const Ткст& package)
{
	Ткст packageManifest = PackagePath(package);
	Ткст packageDir = дайПапкуФайла(packageManifest);
	
	return GetAssemblyDir(packageDir, package);
}

Ткст AndroidBuilderUtils::GetAssemblyDir(const Ткст& packageDir, const Ткст& package)
{
	Ткст assemblyDir = packageDir;
	
	int pos = packageDir.дайСчёт() - package.дайСчёт() - 1;
	if (pos >= 0 && pos < assemblyDir.дайСчёт())
		assemblyDir.обрежь(pos);
	
	return assemblyDir != packageDir ? assemblyDir : "";
}

}
