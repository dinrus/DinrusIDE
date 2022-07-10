#include "BuilderUtils.h"

using namespace РНЦП;

bool BuilderUtils::IsJavaFile(const Ткст& path)
{
	return HasExt(path, { "java" });
}

bool BuilderUtils::IsHeaderFile(const Ткст& path)
{
	return HasExt(path, { "i", "h", "hpp", "hxx" });
}

bool BuilderUtils::IsCFile(const Ткст& path)
{
	return HasExt(path, { "c" });
}

bool BuilderUtils::IsCppFile(const Ткст& path)
{
	return HasExt(path, { "icpp", "cpp", "cc", "cxx" });
}

bool BuilderUtils::IsDinrusFile(const Ткст& path)
{
	return HasExt(path, { "d", "di" });
}

bool BuilderUtils::IsCppOrCFile(const Ткст& path)
{
	return HasExt(path, { "c", "icpp", "cpp", "cc", "cxx" });
}

bool BuilderUtils::IsXmlFile(const Ткст& path)
{
	return HasExt(path, { "xml" });
}

bool BuilderUtils::IsObjectFile(const Ткст& path)
{
	return HasExt(path, { "o" });
}

bool BuilderUtils::IsTranslationFile(const Ткст& path)
{
	return HasExt(path, { "t" });
}

bool BuilderUtils::HasExt(const Ткст& path, const Индекс<Ткст>& exts)
{
	return exts.найди(NormalizeAndGetFileExt(path)) != -1;
}

Ткст BuilderUtils::NormalizeAndGetFileExt(const Ткст& path)
{
	Ткст ext = впроп(дайРасшф(path));
	if (ext.начинаетсяС(".")) {
		ext.удали(0);
	}
	return ext;
}
