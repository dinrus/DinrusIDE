#ifndef _ide_Builders_BuilderUtils_h_
#define _ide_Builders_BuilderUtils_h_

#include <Core/Core.h>

namespace РНЦП {
	
class BuilderUtils final {
public:
	BuilderUtils() = delete;
	BuilderUtils(BuilderUtils&) = delete;
	BuilderUtils& operator=(BuilderUtils&) = delete;
	~BuilderUtils() = delete;
	
	static bool IsJavaFile(const Ткст& path);
	static bool IsHeaderFile(const Ткст& path);
	static bool IsCFile(const Ткст& path);
	static bool IsDinrusFile(const Ткст& path);
	static bool IsCppFile(const Ткст& path);
	static bool IsCppOrCFile(const Ткст& path);
	static bool IsObjectFile(const Ткст& path);
	static bool IsXmlFile(const Ткст& path);
	
	static bool IsTranslationFile(const Ткст& path);
	
private:
	static bool   HasExt(const Ткст& path, const Индекс<Ткст>& exts);
	static Ткст NormalizeAndGetFileExt(const Ткст& path);
};
	
}

#endif
